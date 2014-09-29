/*
 *   Copyright (C) 2010,2011,2012 by Jonathan Naylor G4KLX
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

#include "DCSGatewayRepeaterProtocolHandler.h"
#include "CCITTChecksumReverse.h"
#include "DStarDefines.h"
#include "Utils.h"

const unsigned int BUFFER_LENGTH = 255U;

CDCSGatewayRepeaterProtocolHandler::CDCSGatewayRepeaterProtocolHandler(const wxString& repeaterAddress, unsigned int repeaterPort, const wxString& localAddress, unsigned int localPort) :
m_socket(repeaterAddress, repeaterPort, localAddress, localPort),
m_outId(0U),
m_type(RT_NONE),
m_inId(0U),
m_buffer(NULL),
m_length(0U)
{
	m_buffer = new unsigned char[BUFFER_LENGTH];

	wxDateTime now = wxDateTime::UNow();
	::srand(now.GetMillisecond());
}

CDCSGatewayRepeaterProtocolHandler::~CDCSGatewayRepeaterProtocolHandler()
{
	delete[] m_buffer;
}

bool CDCSGatewayRepeaterProtocolHandler::open()
{
	return m_socket.open();
}

bool CDCSGatewayRepeaterProtocolHandler::writeHeader(const CDCSGatewayHeaderData& header)
{
	unsigned char buffer[50U];
	unsigned int length = header.getRepeaterData(buffer, 50U, true);

	// CUtils::dump(wxT("Sending Header to Repeater"), buffer, length);

#if defined(DUMP_TX)
	CUtils::dump(wxT("Sending Header"), buffer, length);
	return true;
#else
	return m_socket.write(buffer, length);
#endif
}

bool CDCSGatewayRepeaterProtocolHandler::writeData(const CDCSGatewayAMBEData& data)
{
	unsigned char buffer[30U];
	unsigned int length = data.getRepeaterData(buffer, 30U);

	// CUtils::dump(wxT("Sending AMBE to Repeater"), buffer, length);

#if defined(DUMP_TX)
	CUtils::dump(wxT("Sending Data"), buffer, length);
	return true;
#else
	return m_socket.write(buffer, length);
#endif
}

bool CDCSGatewayRepeaterProtocolHandler::writeText(const wxString& text, LINK_STATUS status, const wxString& reflector)
{
	unsigned char data[40U];

	data[0] = 'D';
	data[1] = 'S';
	data[2] = 'R';
	data[3] = 'P';

	data[4] = 0x00;

	::memset(data + 5U, ' ', 20U);

	for (unsigned int i = 0U; i < text.Length() && i < 20U; i++)
		data[i + 5U] = text.GetChar(i);

	data[25U] = (unsigned char)status;

	::memset(data + 26U, ' ', 8U);

	if (status != LS_NONE) {
		for (unsigned int i = 0U; i < reflector.Length() && i < 8U; i++)
			data[i + 26U] = reflector.GetChar(i);
	}

#if defined(DUMP_TX)
	CUtils::dump(wxT("Sending Text"), data, 34U);
	return true;
#else
	return m_socket.write(data, 34U);
#endif
}

REPEATER_TYPE CDCSGatewayRepeaterProtocolHandler::read()
{
	bool res = true;

	// Loop until we have no more data from the socket or we have data for the higher layers
	while (res)
		res = readPackets();

	return m_type;
}

bool CDCSGatewayRepeaterProtocolHandler::readPackets()
{
	m_type = RT_NONE;

	// No more data?
	int length = m_socket.read(m_buffer, BUFFER_LENGTH);
	if (length <= 0)
		return false;

	m_length = length;

	// Invalid packet type?
	if (m_buffer[0] == 'D' && m_buffer[1] == 'S' && m_buffer[2] == 'R' && m_buffer[3] == 'P') {
		// Poll data
		if (m_buffer[4] == 0x0AU)
			return true;

		// Header data
		else if (m_buffer[4] == 0x20U) {
			m_type = RT_HEADER;
			return false;
		}

		// User data
		else if (m_buffer[4] == 0x21U) {
			m_type = RT_DATA;
			return false;
		}

		// Busy header data
		else if (m_buffer[4] == 0x22U)
			return true;

		// Busy AMBE data
		else if (m_buffer[4] == 0x23U)
			return true;
	}

	CUtils::dump(wxT("Unknown packet from the Repeater"), m_buffer, m_length);

	return true;
}

CDCSGatewayHeaderData* CDCSGatewayRepeaterProtocolHandler::readHeader()
{
	if (m_type != RT_HEADER)
		return NULL;

	// CUtils::dump(wxT("Receiving Header from Repeater"), m_buffer, m_length);

	CDCSGatewayHeaderData* header = new CDCSGatewayHeaderData;

	bool res = header->setRepeaterData(m_buffer, m_length, true);
	if (!res) {
		delete header;
		return NULL;
	}

	return header;
}

CDCSGatewayAMBEData* CDCSGatewayRepeaterProtocolHandler::readData()
{
	if (m_type != RT_DATA)
		return NULL;

	// CUtils::dump(wxT("Receiving AMBE from Repeater"), m_buffer, m_length);

	CDCSGatewayAMBEData* data = new CDCSGatewayAMBEData;

	bool res = data->setRepeaterData(m_buffer, m_length);
	if (!res) {
		delete data;
		return NULL;
	}

	return data;
}

void CDCSGatewayRepeaterProtocolHandler::close()
{
	m_socket.close();
}

