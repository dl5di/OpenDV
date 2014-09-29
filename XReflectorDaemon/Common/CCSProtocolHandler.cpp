/*
 *   Copyright (C) 2013 by Jonathan Naylor G4KLX
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

#include "CCSProtocolHandler.h"

#include "DStarDefines.h"
#include "Utils.h"

// #define	DUMP_TX

const unsigned int BUFFER_LENGTH = 2000U;

CCCSProtocolHandler::CCCSProtocolHandler(unsigned int port, const wxString& addr) :
m_socket(addr, port),
m_type(CT_NONE),
m_buffer(NULL),
m_length(0U),
m_yourAddress(),
m_yourPort(0U),
m_myPort(port)
{
	m_buffer = new unsigned char[BUFFER_LENGTH];
}

CCCSProtocolHandler::~CCCSProtocolHandler()
{
	delete[] m_buffer;
}

bool CCCSProtocolHandler::open()
{
	return m_socket.open();
}

unsigned int CCCSProtocolHandler::getPort() const
{
	return m_myPort;
}

bool CCCSProtocolHandler::writeData(const CAMBEData& data)
{
	unsigned char buffer[100U];
	unsigned int length = data.getCCSData(buffer, 100U);

// #if defined(DUMP_TX)
	CUtils::dump(wxT("Sending Data"), buffer, length);
// #endif

	return m_socket.write(buffer, length, data.getYourAddress(), data.getYourPort());
}

bool CCCSProtocolHandler::writePoll(const CPollData& poll)
{
	unsigned char buffer[30U];
	unsigned int length = poll.getCCSData(buffer, 30U);

#if defined(DUMP_TX)
	CUtils::dump(wxT("Sending Poll"), buffer, length);
#endif

	return m_socket.write(buffer, length, poll.getYourAddress(), poll.getYourPort());
}

bool CCCSProtocolHandler::writeHeard(const CHeardData& heard)
{
	unsigned char buffer[100U];
	unsigned int length = heard.getCCSData(buffer, 100U);

#if defined(DUMP_TX)
	CUtils::dump(wxT("Sending Heard"), buffer, length);
#endif

	return m_socket.write(buffer, length, heard.getAddress(), heard.getPort());
}

bool CCCSProtocolHandler::writeConnect(const CConnectData& connect)
{
	unsigned char buffer[40U];
	unsigned int length = connect.getCCSData(buffer, 40U);

#if defined(DUMP_TX)
	CUtils::dump(wxT("Sending Connect"), buffer, length);
#endif

	return m_socket.write(buffer, length, connect.getYourAddress(), connect.getYourPort());
}

bool CCCSProtocolHandler::writeBusy(const wxString& text, const in_addr& address, unsigned int port)
{
	unsigned char buffer[38U];

	::memset(buffer, ' ', 38U);
	for (unsigned int i = 0U; i < text.Len() && i < 38U; i++)
		buffer[i] = text.GetChar(i);

// #if defined(DUMP_TX)
	CUtils::dump(wxT("Sending Busy"), buffer, 38U);
// #endif

	return m_socket.write(buffer, 38U, address, port);
}

bool CCCSProtocolHandler::writeEnd(const wxString& local, const wxString& remote, const in_addr& address, unsigned int port)
{
	unsigned char buffer[38U];

	::memset(buffer, 0x00U, 38U);

	::memset(buffer + 0U, ' ', LONG_CALLSIGN_LENGTH);
	for (unsigned int i = 0U; i < remote.Len() && i < LONG_CALLSIGN_LENGTH; i++)
		buffer[i + 0U] = remote.GetChar(i);

	buffer[8U]  = '0';
	buffer[9U]  = '0';
	buffer[10U] = '0';
	buffer[11U] = '1';

	::memset(buffer + 12U, ' ', LONG_CALLSIGN_LENGTH);
	for (unsigned int i = 0U; i < local.Len() && i < LONG_CALLSIGN_LENGTH; i++)
		buffer[i + 12U] = local.GetChar(i);

// #if defined(DUMP_TX)
	CUtils::dump(wxT("Sending End"), buffer, 38U);
// #endif

	return m_socket.write(buffer, 38U, address, port);
}

CCS_TYPE CCCSProtocolHandler::read()
{
	bool res = true;

	// Loop until we have no more data from the socket or we have data for the higher layers
	while (res)
		res = readPackets();

	return m_type;
}

bool CCCSProtocolHandler::readPackets()
{
	m_type = CT_NONE;

	// No more data?
	int length = m_socket.read(m_buffer, BUFFER_LENGTH, m_yourAddress, m_yourPort);
	if (length <= 0)
		return false;

	m_length = length;

	if (m_buffer[0] == '0' && m_buffer[1] == '0' && m_buffer[2] == '0' && m_buffer[3] == '1') {
		CUtils::dump(wxT("Receiving data"), m_buffer, m_length);
		m_type = CT_DATA;
		return false;
	} else if (m_buffer[0] == 'L' && m_buffer[1] == 'L' && m_buffer[2] == 'L') {
		// CUtils::dump(wxT("Status data"), m_buffer, m_length);
		return true;
	} else if (m_buffer[0] == 'D' && m_buffer[1] == 'D' && m_buffer[2] == 'D' && m_buffer[3] == 'D') {
		// CUtils::dump(wxT("Status data"), m_buffer, m_length);
		return true;
	} else {
		switch (m_length) {
			case 14U:
				m_type = CT_CONNECT;
				return false;
			case 25U:
				m_type = CT_POLL;
				return false;
			case 100U:
				CUtils::dump(wxT("Busy data"), m_buffer, m_length);
				return true;
			default:
				break;
		}
	}

	// An unknown type
	CUtils::dump(wxT("Unknown packet type from CCS"), m_buffer, m_length);

	return true;
}

CAMBEData* CCCSProtocolHandler::readData()
{
	if (m_type != CT_DATA)
		return NULL;

	CAMBEData* data = new CAMBEData;

	bool res = data->setCCSData(m_buffer, m_length, m_yourAddress, m_yourPort, m_myPort);
	if (!res) {
		delete data;
		return NULL;
	}

	return data;
}

CConnectData* CCCSProtocolHandler::readConnect()
{
	if (m_type != CT_CONNECT)
		return NULL;

	CConnectData* connect = new CConnectData;

	bool res = connect->setCCSData(m_buffer, m_length, m_yourAddress, m_yourPort, m_myPort);
	if (!res) {
		delete connect;
		return NULL;
	}

	return connect;
}

CPollData* CCCSProtocolHandler::readPoll()
{
	if (m_type != CT_POLL)
		return NULL;

	CPollData* poll = new CPollData;

	bool res = poll->setCCSData(m_buffer, m_length, m_yourAddress, m_yourPort, m_myPort);
	if (!res) {
		delete poll;
		return NULL;
	}

	return poll;
}

void CCCSProtocolHandler::close()
{
	m_socket.close();
}
