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

#include "DExtraProtocolHandler.h"

#include "DStarDefines.h"
#include "Utils.h"

// #define	DUMP_TX

const unsigned int BUFFER_LENGTH = 1000U;

CDExtraProtocolHandler::CDExtraProtocolHandler(unsigned int port, const wxString& addr) :
m_socket(addr, port),
m_type(DE_NONE),
m_buffer(NULL),
m_length(0U),
m_yourAddress(),
m_yourPort(0U),
m_myPort(port)
{
	m_buffer = new unsigned char[BUFFER_LENGTH];
}

CDExtraProtocolHandler::~CDExtraProtocolHandler()
{
	delete[] m_buffer;
}

bool CDExtraProtocolHandler::open()
{
	return m_socket.open();
}

unsigned int CDExtraProtocolHandler::getPort() const
{
	return m_myPort;
}

bool CDExtraProtocolHandler::writeHeader(const CHeaderData& header)
{
	unsigned char buffer[60U];
	unsigned int length = header.getDExtraData(buffer, 60U, true);

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

bool CDExtraProtocolHandler::writeAMBE(const CAMBEData& data)
{
	unsigned char buffer[40U];
	unsigned int length = data.getDExtraData(buffer, 40U);

#if defined(DUMP_TX)
	CUtils::dump(wxT("Sending Data"), buffer, length);
#endif

	return m_socket.write(buffer, length, data.getYourAddress(), data.getYourPort());
}

bool CDExtraProtocolHandler::writePoll(const CPollData& poll)
{
	unsigned char buffer[20U];
	unsigned int length = poll.getDExtraData(buffer, 20U);

#if defined(DUMP_TX)
	CUtils::dump(wxT("Sending Poll"), buffer, length);
#endif

	return m_socket.write(buffer, length, poll.getYourAddress(), poll.getYourPort());
}

bool CDExtraProtocolHandler::writeConnect(const CConnectData& connect)
{
	unsigned char buffer[20U];
	unsigned int length = connect.getDExtraData(buffer, 20U);

#if defined(DUMP_TX)
	CUtils::dump(wxT("Sending Connect"), buffer, length);
#endif

	for (unsigned int i = 0U; i < 2U; i++) {
		bool res = m_socket.write(buffer, length, connect.getYourAddress(), connect.getYourPort());
		if (!res)
			return false;
	}

	return true;
}

DEXTRA_TYPE CDExtraProtocolHandler::read()
{
	bool res = true;

	// Loop until we have no more data from the socket or we have data for the higher layers
	while (res)
		res = readPackets();

	return m_type;
}

bool CDExtraProtocolHandler::readPackets()
{
	m_type = DE_NONE;

	// No more data?
	int length = m_socket.read(m_buffer, BUFFER_LENGTH, m_yourAddress, m_yourPort);
	if (length <= 0)
		return false;

	m_length = length;

	if (m_buffer[0] != 'D' || m_buffer[1] != 'S' || m_buffer[2] != 'V' || m_buffer[3] != 'T') {
		switch (m_length) {
			case 9U:
				m_type = DE_POLL;
				return false;
			case 11U:
			case 14U:
				m_type = DE_CONNECT;
				return false;
			default:
				return true;
		}
	} else {
		// Header or data packet type?
		if (m_buffer[14] == 0x80)
			m_type = DE_HEADER;
		else
			m_type = DE_AMBE;

		return false;
	}
}

CHeaderData* CDExtraProtocolHandler::readHeader()
{
	if (m_type != DE_HEADER)
		return NULL;

	CHeaderData* header = new CHeaderData;

	// DExtra checksums are unreliable
	bool res = header->setDExtraData(m_buffer, m_length, false, m_yourAddress, m_yourPort, m_myPort);
	if (!res) {
		delete header;
		return NULL;
	}

	return header;
}

CAMBEData* CDExtraProtocolHandler::readAMBE()
{
	if (m_type != DE_AMBE)
		return NULL;

	CAMBEData* data = new CAMBEData;

	bool res = data->setDExtraData(m_buffer, m_length, m_yourAddress, m_yourPort, m_myPort);
	if (!res) {
		delete data;
		return NULL;
	}

	return data;
}

CPollData* CDExtraProtocolHandler::readPoll()
{
	if (m_type != DE_POLL)
		return NULL;

	CPollData* poll = new CPollData;

	bool res = poll->setDExtraData(m_buffer, m_length, m_yourAddress, m_yourPort, m_myPort);
	if (!res) {
		delete poll;
		return NULL;
	}

	return poll;
}

CConnectData* CDExtraProtocolHandler::readConnect()
{
	if (m_type != DE_CONNECT)
		return NULL;

	CConnectData* connect = new CConnectData;

	bool res = connect->setDExtraData(m_buffer, m_length, m_yourAddress, m_yourPort, m_myPort);
	if (!res) {
		delete connect;
		return NULL;
	}

	return connect;
}

void CDExtraProtocolHandler::close()
{
	m_socket.close();
}
