/*
 *   Copyright (C) 2010-2014 by Jonathan Naylor G4KLX
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
m_username(callsign),
m_ssid(callsign),
m_socket(hostname, port, address),
m_queue(20U),
m_exit(false),
m_connected(false),
m_APRSReadCallback(NULL),
m_filter(wxT("")),
m_clientName(wxT("ircDDBGateway"))
{
	wxASSERT(!callsign.IsEmpty());
	wxASSERT(!hostname.IsEmpty());
	wxASSERT(port > 0U);

	m_username.SetChar(LONG_CALLSIGN_LENGTH - 1U, wxT(' '));
	m_username.Trim();
	m_username.MakeUpper();

	m_ssid = m_ssid.SubString(LONG_CALLSIGN_LENGTH - 1U, 1);
}

CAPRSWriterThread::CAPRSWriterThread(const wxString& callsign, const wxString& address, const wxString& hostname, unsigned int port, const wxString& filter, const wxString& clientName) :
wxThread(wxTHREAD_JOINABLE),
m_username(callsign),
m_ssid(callsign),
m_socket(hostname, port, address),
m_queue(20U),
m_exit(false),
m_connected(false),
m_APRSReadCallback(NULL),
m_filter(filter),
m_clientName(clientName)
{
	wxASSERT(!callsign.IsEmpty());
	wxASSERT(!hostname.IsEmpty());
	wxASSERT(port > 0U);

	m_username.SetChar(LONG_CALLSIGN_LENGTH - 1U, wxT(' '));
	m_username.Trim();
	m_username.MakeUpper();

	m_ssid = m_ssid.SubString(LONG_CALLSIGN_LENGTH - 1U, 1);
}

CAPRSWriterThread::~CAPRSWriterThread()
{
	m_username.Clear();
}

bool CAPRSWriterThread::start()
{
	Create();
	Run();

	return true;
}

void* CAPRSWriterThread::Entry()
{
	wxLogMessage(wxT("Starting the APRS Writer thread"));

	m_connected = connect();

	try {
		while (!m_exit) {
			if (!m_connected) {
				m_connected = connect();

				if (!m_connected){
					wxLogError(wxT("Reconnect attempt to the APRS server has failed"));
					Sleep(10000UL);		// 10 secs
				}
			}

			if (m_connected) {
				if(!m_queue.isEmpty()){
					char* p = m_queue.getData();

					wxString text(p, wxConvLocal);
					wxLogMessage(wxT("APRS ==> %s"), text.c_str());

					::strcat(p, "\r\n");

					bool ret = m_socket.write((unsigned char*)p, ::strlen(p));
					if (!ret) {
						m_connected = false;
						m_socket.close();
						wxLogError(wxT("Connection to the APRS thread has failed"));
					}

					delete[] p;
				}
				{
					wxString line;
					int length = m_socket.readLine(line, APRS_TIMEOUT);

					/*if (length == 0)
						wxLogWarning(wxT("No response from the APRS server after %u seconds"), APRS_TIMEOUT);*/

					if (length < 0) {
						m_connected = false;
						m_socket.close();
						wxLogError(wxT("Error when reading from the APRS server"));
					}

					if(length > 0 && line.GetChar(0) != '#'//check if we have something and if that something is an APRS frame
					    && m_APRSReadCallback != NULL)//do we have someone wanting an APRS Frame?
					{	
						//wxLogMessage(wxT("Received APRS Frame : ") + line);
						m_APRSReadCallback(wxString(line));
					}
				}

			}
		}

		if (m_connected)
			m_socket.close();

		while (!m_queue.isEmpty()) {
			char* p = m_queue.getData();
			delete[] p;
		}
	}
	catch (std::exception& e) {
		wxString message(e.what(), wxConvLocal);
		wxLogError(wxT("Exception raised in the APRS Writer thread - \"%s\""), message.c_str());
	}
	catch (...) {
		wxLogError(wxT("Unknown exception raised in the APRS Writer thread"));
	}

	wxLogMessage(wxT("Stopping the APRS Writer thread"));

	return NULL;
}

void CAPRSWriterThread::setReadAPRSCallback(ReadAPRSFrameCallback cb)
{
	m_APRSReadCallback = cb;
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

	//wait for lgin banner
	int length;
	wxString serverResponse(wxT(""));
	length = m_socket.readLine(serverResponse, APRS_TIMEOUT);
	if (length == 0) {
		wxLogError(wxT("No reply from the APRS server after %u seconds"), APRS_TIMEOUT);
		m_socket.close();
		return false;
	}
	wxLogMessage(wxT("Received login banner : ") + serverResponse);

	wxString filter(m_filter);
	if (filter.Length() > 0) filter.Prepend(wxT(" filter "));
	wxString connectString = wxString::Format(wxT("user %s-%s pass %u vers %s%s\n"), m_username.c_str(), m_ssid.c_str(), password,
							(m_clientName.Length() ? m_clientName : wxT("ircDDBGateway")).c_str(),
							filter.c_str());
	//wxLogMessage(wxT("Connect String : ") + connectString);
	ret = m_socket.writeLine(connectString);
	if (!ret) {
		m_socket.close();
		return false;
	}
	

	length = m_socket.readLine(serverResponse, APRS_TIMEOUT);
	if (length == 0) {
		wxLogError(wxT("No reply from the APRS server after %u seconds"), APRS_TIMEOUT);
		m_socket.close();
		return false;
	}
	if (length < 0) {
		wxLogError(wxT("Error when reading from the APRS server"));
		m_socket.close();
		return false;
	}

	wxLogMessage(wxT("Response from APRS server: ") + serverResponse);

	wxLogMessage(wxT("Connected to the APRS server"));

	return true;
}

unsigned int CAPRSWriterThread::getAPRSPassword(wxString callsign) const
{
	unsigned int len = callsign.Length();

	wxUint16 hash = 0x73E2U;

	for (unsigned int i = 0U; i < len; i += 2U) {
		hash ^= (char)callsign.GetChar(i) << 8;
		if(i + 1 < len)
			hash ^= (char)callsign.GetChar(i + 1);
	}

	return hash & 0x7FFFU;
}
