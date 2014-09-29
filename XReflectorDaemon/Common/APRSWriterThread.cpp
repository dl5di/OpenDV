/*
 *   Copyright (C) 2010,2011,2012,2013 by Jonathan Naylor G4KLX
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

#include "APRSWriterThread.h"
#include "DStarDefines.h"
#include "Utils.h"
#include "Defs.h"

// #define	DUMP_TX

const unsigned int APRS_TIMEOUT = 10U;

CAPRSWriterThread::CAPRSWriterThread(const wxString& callsign, const wxString& address, const wxString& hostname, unsigned int port) :
wxThread(wxTHREAD_JOINABLE),
m_username(NULL),
m_socket(hostname, port, address),
m_queue(20U),
m_exit(false),
m_connected(false)
{
	wxASSERT(!callsign.IsEmpty());
	wxASSERT(!hostname.IsEmpty());
	wxASSERT(port > 0U);

	m_username = new char[LONG_CALLSIGN_LENGTH + 1U];

	wxString username = callsign;
	username.SetChar(LONG_CALLSIGN_LENGTH - 1U, wxT(' '));
	username.Trim();
	username.MakeUpper();

	unsigned int n = 0U;
	for (unsigned int i = 0U; i < LONG_CALLSIGN_LENGTH && i < username.Len(); i++, n++)
		m_username[n] = username.GetChar(i);
	m_username[n] = 0x00;
}

CAPRSWriterThread::~CAPRSWriterThread()
{
	delete[] m_username;
}

bool CAPRSWriterThread::start()
{
	Create();
	Run();

	return true;
}

void* CAPRSWriterThread::Entry()
{
	wxLogMessage(wxT("Starting the APRS writer thread"));

	m_connected = connect();

	while (!m_exit) {
		if (!m_connected) {
			m_connected = connect();

			if (!m_connected)
				wxLogError(wxT("Reconnect attempt to the APRS server has failed"));
		}

		if (m_connected && !m_queue.isEmpty()) {
			char* p = m_queue.getData();

			wxString text(p, wxConvLocal);
			wxLogMessage(wxT("APRS ==> %s"), text.c_str());

			::strcat(p, "\r\n");

#if defined(DUMP_TX)
			CUtils::dump(wxT("Sending APRS Data"), (unsigned char*)p, ::strlen(p));
#else
			bool ret = m_socket.write((unsigned char*)p, ::strlen(p));
			if (!ret) {
				m_connected = false;
				m_socket.close();
				wxLogError(wxT("Connection to the APRS thread has failed"));
			} else {
				unsigned char buffer[200U];
				int length = m_socket.read(buffer, 200U, APRS_TIMEOUT);

				if (length == 0)
					wxLogWarning(wxT("No response from the APRS server after %u seconds"), APRS_TIMEOUT);

				if (length == -1) {
					m_connected = false;
					m_socket.close();
					wxLogError(wxT("Error when reading from the APRS server"));
				}
			}
#endif
			delete[] p;
		}

		Sleep(10000UL);		// 10 secs
	}

	if (m_connected)
		m_socket.close();

	while (!m_queue.isEmpty()) {
		char* p = m_queue.getData();
		delete[] p;
	}

	wxLogMessage(wxT("Stopping the APRS writer thread"));

	return NULL;
}

void CAPRSWriterThread::write(const char* data)
{
	wxASSERT(data != NULL);

	if (!m_connected)
		return;

	unsigned int len = ::strlen(data);

	char* p = new char[len + 5U];
	::strcpy(p, data);

	m_queue.addData(p);
}

bool CAPRSWriterThread::isConnected() const
{
	return m_connected;
}

void CAPRSWriterThread::stop()
{
	m_exit = true;

	Wait();
}

bool CAPRSWriterThread::connect()
{
	unsigned int password = getAPRSPassword(m_username);

	bool ret = m_socket.open();
	if (!ret)
		return false;

	unsigned char buffer[200U];
	::sprintf((char*)buffer, "user %s-G pass %u vers ircDDBGateway\n", m_username, password);

	ret = m_socket.write(buffer, ::strlen((char*)buffer));
	if (!ret) {
		m_socket.close();
		return false;
	}

	int length = m_socket.read(buffer, 200U, APRS_TIMEOUT);
	if (length == 0) {
		wxLogError(wxT("No reply from the APRS server after %u seconds"), APRS_TIMEOUT);
		m_socket.close();
		return false;
	}
	if (length == -1) {
		wxLogError(wxT("Error when reading from the APRS server"));
		m_socket.close();
		return false;
	}

	wxString text((char*)buffer, wxConvLocal, length - 1U);
	wxLogMessage(wxT("Response from APRS server: %s"), text.c_str());

	wxLogMessage(wxT("Connected to the APRS server"));

	return true;
}

unsigned int CAPRSWriterThread::getAPRSPassword(const char* callsign) const
{
	const char* p = callsign;
	unsigned int len = ::strlen(callsign);

	wxUint16 hash = 0x73E2U;

	for (unsigned int i = 0U; i < len; i += 2U) {
		hash ^= (*p++) << 8;
		hash ^= (*p++);
	}

	return hash & 0x7FFFU;
}
