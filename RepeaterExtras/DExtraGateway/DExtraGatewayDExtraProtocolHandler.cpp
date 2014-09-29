/*
 *   Copyright (C) 2010,2011 by Jonathan Naylor G4KLX
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

#include "DExtraGatewayDExtraProtocolHandler.h"

#include "DStarDefines.h"
#include "Utils.h"

// #define	DUMP_TX

const unsigned int BUFFER_LENGTH = 255U;

CDExtraGatewayDExtraProtocolHandler::CDExtraGatewayDExtraProtocolHandler() :
m_socket(NULL),
m_callsign(NULL),
m_type(DT_NONE),
m_buffer(NULL),
m_length(0U),
m_pollTimer(1000U, 10U),
m_connectTimer(1000U, 60U)
{
	m_callsign = new unsigned char[LONG_CALLSIGN_LENGTH + 1U];

	::memset(m_callsign, ' ', LONG_CALLSIGN_LENGTH);
	m_callsign[LONG_CALLSIGN_LENGTH] = 0x44;

	m_buffer = new unsigned char[BUFFER_LENGTH];

	wxDateTime now = wxDateTime::Now();
	::srand(now.GetMillisecond());
}

CDExtraGatewayDExtraProtocolHandler::~CDExtraGatewayDExtraProtocolHandler()
{
	delete[] m_callsign;
	delete[] m_buffer;

	if (m_socket != NULL)
		m_socket->close();
	delete m_socket;
}

void CDExtraGatewayDExtraProtocolHandler::setCallsign(const wxString& callsign)
{
	unsigned int length = callsign.Len();
	if (length >= LONG_CALLSIGN_LENGTH)
		length = LONG_CALLSIGN_LENGTH - 1U;

	::memset(m_callsign, ' ', LONG_CALLSIGN_LENGTH);

	for (unsigned int i = 0U; i < length; i++)
		m_callsign[i] = callsign.GetChar(i);

	m_callsign[LONG_CALLSIGN_LENGTH] = 0x44;
}

bool CDExtraGatewayDExtraProtocolHandler::link(const wxString& hostname)
{
	unlink();

	m_socket = new CUDPReaderWriter(hostname, DEXTRA_PORT);

	bool res = m_socket->open();
	if (!res) {
		delete m_socket;
		m_socket = NULL;
		return false;
	}

	writePoll();
	m_pollTimer.start();

	return true;
}

bool CDExtraGatewayDExtraProtocolHandler::writeHeader(const CDExtraGatewayHeaderData& header)
{
	unsigned char buffer[56U];
	unsigned int length = header.getDExtraData(buffer, 56U, true);

	// CUtils::dump(wxT("Sending Header to DExtra"), buffer, length);

#if defined(DUMP_TX)
	CUtils::dump(wxT("Sending Header"), buffer, length);
	return true;
#else
	for (unsigned int i = 0U; i < 5U; i++) {
		bool res = m_socket->write(buffer, length);
		if (!res)
			return false;
	}

	return true;
#endif
}

bool CDExtraGatewayDExtraProtocolHandler::writeData(const CDExtraGatewayAMBEData& data)
{
	unsigned char buffer[27U];
	unsigned int length = data.getDExtraData(buffer, 27U);

	// CUtils::dump(wxT("Sending AMBE to DExtra"), buffer, length);

#if defined(DUMP_TX)
	CUtils::dump(wxT("Sending Data"), buffer, length);
	return true;
#else
	return m_socket->write(buffer, length);
#endif
}

bool CDExtraGatewayDExtraProtocolHandler::writePoll()
{
	// CUtils::dump(wxT("Sending poll to DExtra"), m_callsign, LONG_CALLSIGN_LENGTH + 1U);

#if defined(DUMP_TX)
	CUtils::dump(wxT("Sending Poll"), m_callsign, LONG_CALLSIGN_LENGTH + 1U);
	return true;
#else
	return m_socket->write(m_callsign, LONG_CALLSIGN_LENGTH + 1U);
#endif
}

DEXTRA_TYPE CDExtraGatewayDExtraProtocolHandler::read()
{
	if (m_socket == NULL) {
		m_type = DT_NONE;
		return m_type;
	}

	bool res = true;

	// Loop until we have no more data from the socket or we have data for the higher layers
	while (res)
		res = readPackets();

	return m_type;
}

bool CDExtraGatewayDExtraProtocolHandler::readPackets()
{
	m_type = DT_NONE;

	// No more data?
	int length = m_socket->read(m_buffer, BUFFER_LENGTH);
	if (length <= 0)
		return false;

	m_length = length;

	if (m_buffer[0] != 'D' || m_buffer[1] != 'S' || m_buffer[2] != 'V' || m_buffer[3] != 'T') {
		if (m_length == 9U)		// Reply from the far end
			m_connectTimer.start();

		return true;
	} else {
		// Header or data packet type?
		if (m_buffer[14] == 0x80)
			m_type = DT_HEADER;
		else
			m_type = DT_DATA;

		return false;
	}
}

CDExtraGatewayHeaderData* CDExtraGatewayDExtraProtocolHandler::readHeader()
{
	if (m_type != DT_HEADER)
		return NULL;

	// CUtils::dump(wxT("Receiving Header from DExtra"), m_buffer, m_length);

	CDExtraGatewayHeaderData* header = new CDExtraGatewayHeaderData;

	// DExtra checksums are unreliable
	bool res = header->setDExtraData(m_buffer, m_length, false);
	if (!res) {
		delete header;
		return NULL;
	}

	return header;
}

CDExtraGatewayAMBEData* CDExtraGatewayDExtraProtocolHandler::readData()
{
	if (m_type != DT_DATA)
		return NULL;

	// CUtils::dump(wxT("Receiving AMBE from DExtra"), m_buffer, m_length);

	CDExtraGatewayAMBEData* data = new CDExtraGatewayAMBEData;

	bool res = data->setDExtraData(m_buffer, m_length);
	if (!res) {
		delete data;
		return NULL;
	}

	return data;
}

void CDExtraGatewayDExtraProtocolHandler::clock(unsigned int ms)
{
	m_pollTimer.clock(ms);
	m_connectTimer.clock(ms);

	if (m_pollTimer.hasExpired()) {
		writePoll();
		m_pollTimer.reset();
	}
}

bool CDExtraGatewayDExtraProtocolHandler::isConnected()
{
	return m_socket != NULL && m_connectTimer.isRunning() && !m_connectTimer.hasExpired();
}

void CDExtraGatewayDExtraProtocolHandler::unlink()
{
	m_pollTimer.stop();
	m_connectTimer.stop();

	if (m_socket != NULL)
		m_socket->close();

	delete m_socket;
	m_socket = NULL;
}
