/*
 *   Copyright (C) 2011,2012 by Jonathan Naylor G4KLX
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "TimerControlItemFile.h"
#include "TimerControlThread.h"
#include "SHA256.h"


CTimerControlThread::CTimerControlThread() :
m_password(),
m_fileName(),
m_delay(false),
m_count(0U),
m_items(NULL),
m_mutex(),
m_handler(NULL),
m_killed(false)
{
}

CTimerControlThread::~CTimerControlThread()
{
	if (m_items != NULL) {
		for (unsigned int i = 0U; i < m_count; i++)
			delete m_items[i];

		delete[] m_items;
	}

	delete m_handler;
}

void CTimerControlThread::run()
{
	if (m_delay) {
		for (unsigned int i = 0U; i < 20U; i++) {
			if (m_killed)
				return;

			::wxMilliSleep(1000UL);
		}
	} else {
		::wxMilliSleep(100UL);
	}

	int lastDay  = 0;
	int lastHour = 0;
	int lastMin  = 0;

	while (!m_killed) {
		time_t t;
		::time(&t);

		struct tm* tm = ::localtime(&t);
		int day  = tm->tm_wday;
		int hour = tm->tm_hour;
		int min  = tm->tm_min;

		if (day != lastDay || hour != lastHour || min != lastMin) {
			m_mutex.Lock();

			bool open = false;
			for (unsigned int i = 0U; i < m_count; i++) {
				// Check the day matching
				if (int(m_items[i]->m_day) == day ||							// Exact day match
					    m_items[i]->m_day  == 7U  ||							// Every day match
					   (m_items[i]->m_day  == 8U && day >= 1 && day <= 5) ||	// Weekday match
					   (m_items[i]->m_day  == 9U && (day == 0 || day == 6))) {	// Weekend match

					// Check the time matching	   
					if (int(m_items[i]->m_hour) == hour && int(m_items[i]->m_minute) == min) {
						if (m_items[i]->m_reflector.IsEmpty())
							wxLogMessage(wxT("Linking \"%s\" to \"None\" with reconnect %d"), m_items[i]->m_repeater.c_str(), m_items[i]->m_reconnect);
						else
							wxLogMessage(wxT("Linking \"%s\" to \"%s\" with reconnect %d"), m_items[i]->m_repeater.c_str(), m_items[i]->m_reflector.c_str(), m_items[i]->m_reconnect);

						if (!open)
							open = login();

						if (open)
							link(m_items[i]->m_repeater, m_items[i]->m_reconnect, m_items[i]->m_reflector);
					}
				}
			}

			m_mutex.Unlock();

			if (open)
				logoff();

			lastDay  = day;
			lastHour = hour;
			lastMin  = min;
		}

		::wxMilliSleep(5000UL);
	}
}

void CTimerControlThread::setGateway(const wxString& address, unsigned int port, const wxString& password)
{
	m_password = password;

	delete m_handler;

	m_handler = new CTimerControlRemoteControlHandler(address, port);
}

void CTimerControlThread::setDelay(bool enabled)
{
	m_delay = enabled;
}

void CTimerControlThread::setFileName(const wxString& fileName)
{
	m_fileName = fileName;
}

void CTimerControlThread::reload()
{
	wxMutexLocker lock(m_mutex);

	wxLogMessage(wxT("Reloading the schedule file"));

	// Remove the old schedule
	if (m_items != NULL) {
		for (unsigned int i = 0U; i < m_count; i++)
			delete m_items[i];

		delete[] m_items;
		m_items = NULL;
	}

	CTimerControlItemFile file(m_fileName);
	m_items = file.readItems(m_count);
}

void CTimerControlThread::kill()
{
	m_killed = true;
}

bool CTimerControlThread::login()
{
	wxASSERT(m_handler != NULL);

	wxLogMessage(wxT("Logging into the gateway"));

	bool ret = m_handler->open();
	if (!ret) {
		wxLogError(wxT("Error opening the port"));
		return false;
	}

	TC_TYPE type;

	do {
		ret = m_handler->login();
		if (!ret) {
			wxLogError(wxT("Error when sending the login command"));
			m_handler->close();
			return false;
		}

		::wxMilliSleep(100UL);

		type = m_handler->readType();
		switch (type) {
			case TCT_RANDOM:
				wxLogMessage(wxT("Read the random number"));
				break;
			case TCT_NAK:
				wxLogError(wxT("Received a NAK when asking for the random number"));
				m_handler->setLoggedIn(false);
				m_handler->readNAK();
				m_handler->close();
				return false;
			default:
				::wxMilliSleep(100UL);
				break;
		}
	} while (type != TCT_RANDOM);

	unsigned int rnd = m_handler->readRandom();

	do {
		bool ret = sendHash(rnd);
		if (!ret) {
			wxLogError(wxT("Error when sending the hash"));
			m_handler->close();
			return false;
		}

		::wxMilliSleep(100UL);

		type = m_handler->readType();
		switch (type) {
			case TCT_ACK:
				wxLogMessage(wxT("Logged in to the gateway"));
				m_handler->setLoggedIn(true);
				break;
			case TCT_NAK:
				wxLogError(wxT("Received a NAK after sending the hash"));
				m_handler->setLoggedIn(false);
				m_handler->readNAK();
				m_handler->close();
				return false;
			default:
				::wxMilliSleep(100UL);
				break;
		}
	} while (type != TCT_ACK);

	return true;
}

bool CTimerControlThread::link(const wxString& repeater, RECONNECT reconnect, const wxString& reflector)
{
	wxASSERT(m_handler != NULL);

	for (;;) {
		bool ret = m_handler->link(repeater, reconnect, reflector);
		if (!ret) {
			wxLogError(wxT("Error when sending the link command"));
			return false;
		}

		::wxMilliSleep(100UL);

		TC_TYPE type = m_handler->readType();
		switch (type) {
			case TCT_ACK:
				wxLogMessage(wxT("Sent the link command OK"));
				return true;
			case TCT_NAK:
				wxLogError(wxT("Received a NAK after sending the link command"));
				m_handler->readNAK();
				return false;
			default:
				::wxMilliSleep(100UL);
				break;
		}
	}
}

void CTimerControlThread::logoff()
{
	wxASSERT(m_handler != NULL);

	wxLogMessage(wxT("Logging out of the gateway"));

	m_handler->logout();
	m_handler->setLoggedIn(false);
	m_handler->close();
}

bool CTimerControlThread::sendHash(unsigned int rnd)
{
	unsigned int len = m_password.Len() + sizeof(unsigned int);
	unsigned char*  in = new unsigned char[len];
	unsigned char* out = new unsigned char[32U];

	::memcpy(in, &rnd, sizeof(unsigned int));
	for (unsigned int i = 0U; i < m_password.Len(); i++)
		in[i + sizeof(unsigned int)] = m_password.GetChar(i);

	CSHA256 sha256;
	sha256.buffer(in, len, out);

	bool ret = m_handler->sendHash(out, 32U);

	delete[] in;
	delete[] out;

	return ret;
}
