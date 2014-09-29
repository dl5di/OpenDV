/*
 *   Copyright (C) 2009-2013 by Jonathan Naylor G4KLX
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

#include "SplitRepeaterProtocolHandler.h"
#include "DStarDefines.h"
#include "Utils.h"

// #define	DUMP_TX

const unsigned int BUFFER_LENGTH = 255U;

CSplitRepeaterProtocolHandler::CSplitRepeaterProtocolHandler(const wxString& address, unsigned int port) :
m_socket(address, port),
m_type(NETWORK_NONE),
m_buffer(NULL),
m_length(0U),
m_address(),
m_port(0U)
{
	m_buffer = new unsigned char[BUFFER_LENGTH];
}

CSplitRepeaterProtocolHandler::~CSplitRepeaterProtocolHandler()
{
	delete[] m_buffer;
}

void CSplitRepeaterProtocolHandler::initialise()
{
	wxDateTime now = wxDateTime::UNow();

	::srand(now.GetMillisecond());
}

unsigned int CSplitRepeaterProtocolHandler::createId()
{
	return (::rand() % 65535U) + 1U;
}

bool CSplitRepeaterProtocolHandler::open()
{
	return m_socket.open();
}

bool CSplitRepeaterProtocolHandler::writeHeader(const CSplitRepeaterHeaderData& header)
{
	unsigned char buffer[50U];

	unsigned int length = header.getRepeaterData(buffer, 50U, true);

#if defined(DUMP_TX)
	CUtils::dump(wxT("Sending Header"), buffer, length);
	return true;
#else
	return m_socket.write(buffer, length, header.getAddress(), header.getPort());
#endif
}

bool CSplitRepeaterProtocolHandler::writeData(const CSplitRepeaterAMBEData& data)
{
	unsigned char buffer[30U];

	unsigned int length = data.getRepeaterData(buffer, 30U);

#if defined(DUMP_TX)
	CUtils::dump(wxT("Sending Data"), buffer, length);
	return true;
#else
	return m_socket.write(buffer, length, data.getAddress(), data.getPort());
#endif
}

bool CSplitRepeaterProtocolHandler::writeBusyHeader(const CSplitRepeaterHeaderData& header)
{
	unsigned char buffer[50U];

	unsigned int length = header.getRepeaterData(buffer, 50U, true);

	buffer[4] = 0x22U;

#if defined(DUMP_TX)
	CUtils::dump(wxT("Sending Busy Header"), buffer, length);
	return true;
#else
	return m_socket.write(buffer, length, header.getAddress(), header.getPort());
#endif
}

bool CSplitRepeaterProtocolHandler::writeBusyData(const CSplitRepeaterAMBEData& data)
{
	unsigned char buffer[30U];

	unsigned int length = data.getRepeaterData(buffer, 30U);

	buffer[4U] = 0x23U;

#if defined(DUMP_TX)
	CUtils::dump(wxT("Sending Busy Data"), buffer, length);
	return true;
#else
	return m_socket.write(buffer, length, data.getAddress(), data.getPort());
#endif
}

bool CSplitRepeaterProtocolHandler::writePoll(const wxString& text, const in_addr& address, unsigned int port)
{
	unsigned char buffer[40U];

	buffer[0] = 'D';
	buffer[1] = 'S';
	buffer[2] = 'R';
	buffer[3] = 'P';

	buffer[4] = 0x0A;				// Poll with text

	unsigned int length = text.Length();

	for (unsigned int i = 0U; i < length; i++)
		buffer[5U + i] = text.GetChar(i);

	buffer[5U + length] = 0x00;

#if defined(DUMP_TX)
	CUtils::dump(wxT("Sending Poll"), buffer, 6U + length);
	return true;
#else
	return m_socket.write(buffer, 6U + length, address, port);
#endif
}

NETWORK_TYPE CSplitRepeaterProtocolHandler::read()
{
	bool res = true;

	// Loop until we have no more data from the socket or we have data for the higher layers
	while (res)
		res = readPackets();

	return m_type;
}

bool CSplitRepeaterProtocolHandler::readPackets()
{
	m_type = NETWORK_NONE;

	// No more data?
	int length = m_socket.read(m_buffer, BUFFER_LENGTH, m_address, m_port);
	if (length <= 0)
		return false;

	m_length = length;

	// Invalid packet type?
	if (m_buffer[0] == 'D' && m_buffer[1] == 'S' && m_buffer[2] == 'R' && m_buffer[3] == 'P') {
		if (m_buffer[4] == 0x0AU)
			return true;

		if (m_buffer[4] == 0x00U) {
			m_type = NETWORK_TEXT;
			return false;
		}

		if (m_buffer[4] == 0x01U) {
			m_type = NETWORK_TEMPTEXT;
			return false;
		}

		// Status data 1
		else if (m_buffer[4] == 0x04U && m_buffer[5] == 0x00U) {
			m_type = NETWORK_STATUS1;
			return false;
		}

		// Status data 2
		else if (m_buffer[4] == 0x04U && m_buffer[5] == 0x01U) {
			m_type = NETWORK_STATUS2;
			return false;
		}

		// Status data 3
		else if (m_buffer[4] == 0x04U && m_buffer[5] == 0x02U) {
			m_type = NETWORK_STATUS3;
			return false;
		}

		// Status data 4
		else if (m_buffer[4] == 0x04U && m_buffer[5] == 0x03U) {
			m_type = NETWORK_STATUS4;
			return false;
		}

		// Status data 5
		else if (m_buffer[4] == 0x04U && m_buffer[5] == 0x04U) {
			m_type = NETWORK_STATUS5;
			return false;
		}

		// Header data
		else if (m_buffer[4] == 0x20U) {
			m_type = NETWORK_HEADER;
			return false;
		}

		// User data
		else if (m_buffer[4] == 0x21U) {
			m_type = NETWORK_DATA;
			return false;
		}
	}

	CUtils::dump(wxT("Unknown packet received"), m_buffer, m_length);

	return true;
}

CSplitRepeaterHeaderData* CSplitRepeaterProtocolHandler::readHeader()
{
	if (m_type != NETWORK_HEADER)
		return NULL;

	// If the checksum is 0xFFFF then we accept the header without testing the checksum
	if (m_buffer[47U] == 0xFFU && m_buffer[48U] == 0xFFU) {
		CSplitRepeaterHeaderData* header = new CSplitRepeaterHeaderData;
		header->setRepeaterData(m_buffer, m_length, false, m_address, m_port);
		return header;
	}

	// Header checksum testing is enabled
	CSplitRepeaterHeaderData* header = new CSplitRepeaterHeaderData;
	bool valid = header->setRepeaterData(m_buffer, m_length, true, m_address, m_port);
	if (!valid) {
		CUtils::dump(wxT("Header checksum failure"), m_buffer + 8U, RADIO_HEADER_LENGTH_BYTES);
		delete header;
		return NULL;
	}

	return header;
}

CSplitRepeaterAMBEData* CSplitRepeaterProtocolHandler::readData()
{
	if (m_type != NETWORK_DATA)
		return 0U;

	CSplitRepeaterAMBEData* data = new CSplitRepeaterAMBEData;
	data->setRepeaterData(m_buffer, m_length, m_address, m_port);

	return data;
}

void CSplitRepeaterProtocolHandler::readText(wxString& text, LINK_STATUS& status, wxString& reflector)
{
	if (m_type != NETWORK_TEXT) {
		text = wxT("                    ");
		reflector = wxT("        ");
		status = LS_NONE;
		return;
	}

	text = wxString((char*)(m_buffer + 5U), wxConvLocal, 20U);

	status = LINK_STATUS(m_buffer[25U]);

	reflector = wxString((char*)(m_buffer + 26U), wxConvLocal, 8U);
}

void CSplitRepeaterProtocolHandler::readTempText(wxString& text)
{
	if (m_type != NETWORK_TEMPTEXT) {
		text = wxT("                    ");
		return;
	}

	text = wxString((char*)(m_buffer + 5U), wxConvLocal, 20U);
}

wxString CSplitRepeaterProtocolHandler::readStatus1()
{
	if (m_type != NETWORK_STATUS1)
		return wxEmptyString;

	return wxString((char*)(m_buffer + 6U), wxConvLocal, 20U);
}

wxString CSplitRepeaterProtocolHandler::readStatus2()
{
	if (m_type != NETWORK_STATUS2)
		return wxEmptyString;

	return wxString((char*)(m_buffer + 6U), wxConvLocal, 20U);
}

wxString CSplitRepeaterProtocolHandler::readStatus3()
{
	if (m_type != NETWORK_STATUS3)
		return wxEmptyString;

	return wxString((char*)(m_buffer + 6U), wxConvLocal, 20U);
}

wxString CSplitRepeaterProtocolHandler::readStatus4()
{
	if (m_type != NETWORK_STATUS4)
		return wxEmptyString;

	return wxString((char*)(m_buffer + 6U), wxConvLocal, 20U);
}

wxString CSplitRepeaterProtocolHandler::readStatus5()
{
	if (m_type != NETWORK_STATUS5)
		return wxEmptyString;

	return wxString((char*)(m_buffer + 6U), wxConvLocal, 20U);
}

void CSplitRepeaterProtocolHandler::close()
{
	m_socket.close();
}
