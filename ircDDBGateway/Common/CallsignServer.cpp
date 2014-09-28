/*
 *   Copyright (C) 2012,2013,2014 by Jonathan Naylor G4KLX
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

#include "TCPReaderWriterClient.h"
#include "CallsignServer.h"
#include "DStarDefines.h"
#include "Utils.h"
#include "Defs.h"

const wxString CALLSERVER_HOSTNAME = wxT("dns.xreflector.net");
const unsigned int CALLSERVER_PORT = 20001U;

const unsigned int TCP_TIMEOUT = 10U;

CCallsignServer::CCallsignServer(const wxString& callsign, const wxString& address, CCacheManager* cache) :
wxThread(wxTHREAD_JOINABLE),
m_callsign(callsign),
m_address(address),
m_cache(cache),
m_timer(1U, 1U * 3600U),		// 1 hour
m_killed(false)
{
	wxASSERT(!callsign.IsEmpty());
	wxASSERT(cache != NULL);
}

CCallsignServer::~CCallsignServer()
{
}

void CCallsignServer::start()
{
	process(CALLSERVER_HOSTNAME, CALLSERVER_PORT);

	Create();
	Run();
}

void* CCallsignServer::Entry()
{
	wxLogMessage(wxT("Starting the Callsign Server thread"));

	m_timer.start();

	while (!m_killed) {
		if (m_timer.hasExpired()) {
			process(CALLSERVER_HOSTNAME, CALLSERVER_PORT);
			m_timer.start();
		}

		Sleep(1000UL);

		m_timer.clock();
	}

	wxLogMessage(wxT("Stopping the Callsign Server thread"));

	return NULL;
}

void CCallsignServer::stop()
{
	m_killed = true;

	Wait();
}

unsigned int CCallsignServer::process(const wxString& hostname, unsigned int port)
{
	CTCPReaderWriterClient socket(hostname, port, m_address);

	bool ret = socket.open();
	if (!ret) {
		wxLogMessage(wxT("Cannot connect to %s"), hostname.c_str());
		return 0U;
	}

	// Space for 5000 entries
	unsigned int length = 5000U * (6U + 1U + 15U + 1U);

	unsigned char* buffer = new unsigned char[length + 1U];
	::memset(buffer, ' ', 29U);

	for (unsigned int i = 0U; i < m_callsign.Len() && i < LONG_CALLSIGN_LENGTH - 1U; i++)
		buffer[i + 0U] = m_callsign.GetChar(i);

	::memcpy(buffer + 9U, "ircDDB Gateway", 14U);

	socket.write(buffer, 29U);

	unsigned int offset = 0U;

	int n = socket.read(buffer, length, TCP_TIMEOUT);
	if (n >= 0)
		offset += n;

	while (n >= 0 && offset < length) {
		n = socket.read(buffer + offset, length - offset, TCP_TIMEOUT);
		if (n == 0)
			Sleep(TCP_TIMEOUT * 1000UL);
		else if (n > 0)
			offset += n;
	}

	buffer[offset] = 0x00U;

	unsigned int dextraCount = 0U;
	unsigned int dcsCount    = 0U;

	char* p = (char*)buffer;

	for (;;) {
		// Split into lines
		char* p1 = ::strchr(p, 0x0A);
		if (p1 != NULL)
			*p1 = 0x00;

		if (::strncmp(p, "XRF", 3U) == 0) {
			char* p2 = ::strtok(p, " \t\r\n");
			char* p3 = ::strtok(NULL, " \t\r\n");

			if (p2 != NULL && p3 != NULL) {
				wxString name    = wxString(p2, wxConvLocal);
				wxString address = wxString(p3, wxConvLocal);

				if (!address.IsSameAs(wxT("0.0.0.0"))) {
					wxLogMessage(wxT("DExtra: %s\t%s"), name.c_str(), address.c_str());

					name.resize(LONG_CALLSIGN_LENGTH - 1U, wxT(' '));
					name.Append(wxT("G"));
					m_cache->updateGateway(name, address, DP_DEXTRA, false, true);

					dextraCount++;
				}
			}
		} else if (::strncmp(p, "DCS", 3U) == 0) {
			char* p2 = ::strtok(p, " \t\r\n");
			char* p3 = ::strtok(NULL, " \t\r\n");

			if (p2 != NULL && p3 != NULL) {
				wxString name    = wxString(p2, wxConvLocal);
				wxString address = wxString(p3, wxConvLocal);

				if (!address.IsSameAs(wxT("0.0.0.0"))) {
					wxLogMessage(wxT("DCS: %s\t%s"), name.c_str(), address.c_str());

					name.resize(LONG_CALLSIGN_LENGTH - 1U, wxT(' '));
					name.Append(wxT("G"));
					m_cache->updateGateway(name, address, DP_DCS, false, true);

					dcsCount++;
				}
			}
		}

		if (p1 == NULL)
			break;

		p = p1 + 1U;
	}

	wxLogMessage(wxT("Registered with %s using callsign %s"), hostname.c_str(), m_callsign.Left(LONG_CALLSIGN_LENGTH - 1U).c_str());

	wxLogMessage(wxT("Loaded %u DExtra and %u DCS reflectors from %s"), dextraCount, dcsCount, hostname.c_str());

	delete[] buffer;
	socket.close();

	return dextraCount + dcsCount;
}
