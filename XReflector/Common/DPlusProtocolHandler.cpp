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

#include "DPlusProtocolHandler.h"

#include "DStarDefines.h"
#include "Utils.h"

// #define	DUMP_TX

const unsigned int BUFFER_LENGTH = 1000U;

CDPlusProtocolHandler::CDPlusProtocolHandler(unsigned int port, const wxString& addr) :
m_socket(addr, port),
m_type(DP_NONE),
m_buffer(NULL),
m_length(0U),
m_yourAddress(),
m_yourPort(0U),
m_myPort(port)
{
	m_buffer = new unsigned char[BUFFER_LENGTH];
}

CDPlusProtocolHandler::~CDPlusProtocolHandler()
{
	delete[] m_buffer;
}

bool CDPlusProtocolHandler::open()
{
	return m_socket.open();
}

unsigned int CDPlusProtocolHandler::getPort() const
{
	return m_myPort;
}

bool CDPlusProtocolHandler::writeHeader(const CHeaderData& header)
{
	unsigned char buffer[60U];
	unsigned int length = header.getDPlusData(buffer, 60U, true);

#if defined(DUMP_TX)
	CUtils::dump(wxT("Sending Header"), buffer, length);
#endif

	for (unsigned int i = 0U; i < 5U; i++) {
		bool res = m_socket.write(buffer, length, header.getYourAddress(), header.getYourPort());
		if (!res)
			return false;
	}

	return true;
}

bool CDPlusProtocolHandler::writeAMBE(const CAMBEData& data)
{
	unsigned char buffer[40U];
	unsigned int length = data.getDPlusData(buffer, 40U);

#if defined(DUMP_TX)
	CUtils::dump(wxT("Sending Data"), buffer, length);
#endif

	return m_socket.write(buffer, length, data.getYourAddress(), data.getYourPort());
}

bool CDPlusProtocolHandler::writePoll(const CPollData& poll)
{
	unsigned char buffer[10U];
	unsigned int length = poll.getDPlusData(buffer, 10U);

#if defined(DUMP_TX)
	CUtils::dump(wxT("Sending Poll"), buffer, length);
#endif

	return m_socket.write(buffer, length, poll.getYourAddress(), poll.getYourPort());
}

bool CDPlusProtocolHandler::writeConnect(const CConnectData& connect)
{
	unsigned char buffer[40U];
	unsigned int length = connect.getDPlusData(buffer, 40U);

#if defined(DUMP_TX)
	CUtils::dump(wxT("Sending Connect"), buffer, length);
#endif

	return m_socket.write(buffer, length, connect.getYourAddress(), connect.getYourPort());
}

DPLUS_TYPE CDPlusProtocolHandler::read()
{
	bool res = true;

	// Loop until we have no more data from the socket or we have data for the higher layers
	while (res)
		res = readPackets();

	return m_type;
}

bool CDPlusProtocolHandler::readPackets()
{
	m_type = DP_NONE;

	// No more data?
	int length = m_socket.read(m_buffer, BUFFER_LENGTH, m_yourAddress, m_yourPort);
	if (length <= 0)
		return false;

	m_length = length;

	if (m_buffer[2] != 'D' || m_buffer[3] != 'S' || m_buffer[4] != 'V' || m_buffer[5] != 'T') {
		switch (m_length) {
			case 3U:
				m_type = DP_POLL;
				return false;
			case 5U:
			case 8U:
			case 28U:
				m_type = DP_CONNECT;
				return false;
			default:
				// An unknown type
				// CUtils::dump(wxT("Unknown packet type from D-Plus"), m_buffer, m_length);
				return true;
		}
	} else {
		// Header or data packet type?
		if (m_buffer[0] == 0x3A && m_buffer[1] == 0x80) {
			m_type = DP_HEADER;
			return false;
		} else if (m_buffer[0] == 0x1D && m_buffer[1] == 0x80) {
			m_type = DP_AMBE;
			return false;
		} else if (m_buffer[0] == 0x20 && m_buffer[1] == 0x80) {
			m_type = DP_AMBE;
			return false;
		} else {
			// An unknown type
			CUtils::dump(wxT("Unknown packet type from D-Plus"), m_buffer, m_length);
			return true;
		}
	}
}

CHeaderData* CDPlusProtocolHandler::readHeader()
{
	if (m_type != DP_HEADER)
		return NULL;

	CHeaderData* header = new CHeaderData;

	// DPlus checksums are unreliable
	bool res = header->setDPlusData(m_buffer, m_length, false, m_yourAddress, m_yourPort, m_myPort);
	if (!res) {
		delete header;
		return NULL;
	}

	return header;
}

CAMBEData* CDPlusProtocolHandler::readAMBE()
{
	if (m_type != DP_AMBE)
		return NULL;

	CAMBEData* data = new CAMBEData;

	bool res = data->setDPlusData(m_buffer, m_length, m_yourAddress, m_yourPort, m_myPort);
	if (!res) {
		delete data;
		return NULL;
	}

	return data;
}

CPollData* CDPlusProtocolHandler::readPoll()
{
	if (m_type != DP_POLL)
		return NULL;

	CPollData* poll = new CPollData;

	bool res = poll->setDPlusData(m_buffer, m_length, m_yourAddress, m_yourPort, m_myPort);
	if (!res) {
		delete poll;
		return NULL;
	}

	return poll;
}

CConnectData* CDPlusProtocolHandler::readConnect()
{
	if (m_type != DP_CONNECT)
		return NULL;

	CConnectData* connect = new CConnectData;

	bool res = connect->setDPlusData(m_buffer, m_length, m_yourAddress, m_yourPort, m_myPort);
	if (!res) {
		delete connect;
		return NULL;
	}

	return connect;
}

void CDPlusProtocolHandler::close()
{
	m_socket.close();
}
