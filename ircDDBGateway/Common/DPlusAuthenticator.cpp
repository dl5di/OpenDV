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

#include "DPlusAuthenticator.h"
#include "UDPReaderWriter.h"
#include "DStarDefines.h"
#include "Utils.h"
#include "Defs.h"

const wxString OPENDSTAR_HOSTNAME = wxT("opendstar.org");
const unsigned int OPENDSTAR_PORT = 20001U;

const wxString DUTCHSTAR_HOSTNAME = wxT("dpns.dutch-star.eu");
const unsigned int DUTCHSTAR_PORT = 20001U;

const unsigned int TCP_TIMEOUT = 10U;

CDPlusAuthenticator::CDPlusAuthenticator(const wxString& loginCallsign, const wxString& gatewayCallsign, const wxString& address, CCacheManager* cache) :
wxThread(wxTHREAD_JOINABLE),
m_loginCallsign(loginCallsign),
m_gatewayCallsign(gatewayCallsign),
m_address(address),
m_cache(cache),
m_timer(1U, 6U * 3600U),		// 6 hours
m_pollTimer(1U, 60U),			// 1 minute
m_killed(false)
{
	wxASSERT(!loginCallsign.IsEmpty());
	wxASSERT(!gatewayCallsign.IsEmpty());
	wxASSERT(cache != NULL);

	m_gatewayCallsign.Truncate(LONG_CALLSIGN_LENGTH - 1U);

	m_loginCallsign.Trim();
	m_gatewayCallsign.Trim();

	if (m_loginCallsign.IsEmpty())
		m_loginCallsign = m_gatewayCallsign;
}

CDPlusAuthenticator::~CDPlusAuthenticator()
{
}

void CDPlusAuthenticator::start()
{
	Create();
	Run();
}

void* CDPlusAuthenticator::Entry()
{
	wxLogMessage(wxT("Starting the D-Plus authentication thread"));

	authenticate(m_loginCallsign, OPENDSTAR_HOSTNAME, OPENDSTAR_PORT, '2', true);
	authenticate(m_gatewayCallsign, DUTCHSTAR_HOSTNAME, DUTCHSTAR_PORT, 'K', false);

	m_timer.start();
	m_pollTimer.start();

	while (!m_killed) {
		if (m_pollTimer.hasExpired()) {
			poll(m_gatewayCallsign, DUTCHSTAR_HOSTNAME, DUTCHSTAR_PORT, 'K');
			m_pollTimer.start();
		}

		if (m_timer.hasExpired()) {
			authenticate(m_loginCallsign, OPENDSTAR_HOSTNAME, OPENDSTAR_PORT, '2', true);
			authenticate(m_gatewayCallsign, DUTCHSTAR_HOSTNAME, DUTCHSTAR_PORT, 'K', false);
			m_timer.start();
		}

		Sleep(1000UL);

		m_timer.clock();
		m_pollTimer.clock();
	}

	wxLogMessage(wxT("Stopping the D-Plus authentication thread"));

	return NULL;
}

void CDPlusAuthenticator::stop()
{
	m_killed = true;

	Wait();
}

bool CDPlusAuthenticator::authenticate(const wxString& callsign, const wxString& hostname, unsigned int port, unsigned char id, bool writeToCache)
{
	CTCPReaderWriterClient socket(hostname, port, m_address);

	bool ret = socket.open();
	if (!ret)
		return false;

	unsigned char* buffer = new unsigned char[600U];
	::memset(buffer, ' ', 56U);

	buffer[0U] = 0x38U;
	buffer[1U] = 0xC0U;
	buffer[2U] = 0x01U;
	buffer[3U] = 0x00U;

	for (unsigned int i = 0U; i < callsign.Len(); i++)
		buffer[i + 4U] = callsign.GetChar(i);

	buffer[12U] = 'D';
	buffer[13U] = 'V';
	buffer[14U] = '0';
	buffer[15U] = '1';
	buffer[16U] = '9';
	buffer[17U] = '9';
	buffer[18U] = '9';
	buffer[19U] = '9';

	buffer[28U] = 'W';
	buffer[29U] = '7';
	buffer[30U] = 'I';
	buffer[31U] = 'B';
	buffer[32U] = id;

	buffer[40U] = 'D';
	buffer[41U] = 'H';
	buffer[42U] = 'S';
	buffer[43U] = '0';
	buffer[44U] = '2';
	buffer[45U] = '5';
	buffer[46U] = '7';

	ret = socket.write(buffer, 56U);
	if (!ret) {
		socket.close();
		delete[] buffer;
		return false;
	}

	ret = read(socket, buffer + 0U, 2U);

	while (ret) {
		unsigned int len = (buffer[1U] & 0x0FU) * 256U + buffer[0U];

		// Ensure that we get exactly len - 2U bytes from the TCP stream
		ret = read(socket, buffer + 2U, len - 2U);
		if (!ret) {
			wxLogError(wxT("Short read from %s:%u"), hostname.c_str(), port);
			break;
		}

		if ((buffer[1U] & 0xC0U) != 0xC0U || buffer[2U] != 0x01U) {
			wxLogError(wxT("Invalid packet received from %s:%u"), hostname.c_str(), port);
			CUtils::dump(wxT("Details:"), buffer, len);
			ret = read(socket, buffer + 0U, 2U);
			continue;
		}
	
		for (unsigned int i = 8U; i < len; i += 26U) {
			wxString address = wxString((char*)(buffer + i + 0U),  wxConvLocal);
			wxString    name = wxString((char*)(buffer + i + 16U), wxConvLocal);

			address.Trim();
			name.Trim();

			// Get the active flag
			bool active = (buffer[i + 25U] & 0x80U) == 0x80U;

			// An empty name or IP address or an inactive gateway/reflector is not written out
			if (address.Len() > 0U && name.Len() > 0U && !name.Left(3U).IsSameAs(wxT("XRF")) && active && writeToCache){
				if (name.Left(3U).IsSameAs(wxT("REF")))
					wxLogMessage(wxT("D-Plus: %s\t%s"), name.c_str(), address.c_str());

				name.Append(wxT("        "));
				name.Truncate(LONG_CALLSIGN_LENGTH - 1U);
				name.Append(wxT("G"));
				m_cache->updateGateway(name, address, DP_DPLUS, false, true);
			}
		}

		ret = read(socket, buffer + 0U, 2U);
	}

	wxLogMessage(wxT("Registered with %s using callsign %s"), hostname.c_str(), callsign.c_str());

	socket.close();

	delete[] buffer;

	return true;
}

bool CDPlusAuthenticator::poll(const wxString& callsign, const wxString& hostname, unsigned int port, unsigned char id)
{
	CUDPReaderWriter socket(m_address, 0U);
	bool ret = socket.open();
	if (!ret)
		return false;

	unsigned char buffer[56U];
	::memset(buffer, ' ', 56U);

	buffer[0U] = 0x38U;
	buffer[1U] = 0x20U;
	buffer[2U] = 0x01U;
	buffer[3U] = 0x01U;

	for (unsigned int i = 0U; i < callsign.Len(); i++)
		buffer[i + 4U] = callsign.GetChar(i);

	buffer[12U] = 'D';
	buffer[13U] = 'V';
	buffer[14U] = '0';
	buffer[15U] = '1';
	buffer[16U] = '9';
	buffer[17U] = '9';
	buffer[18U] = '9';
	buffer[19U] = '9';

	for (unsigned int i = 0U; i < callsign.Len(); i++)
		buffer[i + 20U] = callsign.GetChar(i);

	buffer[28U] = 'W';
	buffer[29U] = '7';
	buffer[30U] = 'I';
	buffer[31U] = 'B';
	buffer[32U] = id;

	buffer[40U] = 'D';
	buffer[41U] = 'H';
	buffer[42U] = 'S';
	buffer[43U] = '0';
	buffer[44U] = '2';
	buffer[45U] = '5';
	buffer[46U] = '7';

	in_addr address = socket.lookup(hostname);
	if (address.s_addr == INADDR_NONE) {
		socket.close();
		return false;
	}

	ret = socket.write(buffer, 56U, address, port);

	socket.close();

	return ret;
}

bool CDPlusAuthenticator::read(CTCPReaderWriterClient &socket, unsigned char *buffer, unsigned int len) const
{
	unsigned int offset = 0U;

	do {
		int n = socket.read(buffer + offset, len - offset, TCP_TIMEOUT);
		if (n < 0)
			return false;

		offset += n;
	} while ((len - offset) > 0U);

	return true;
}
