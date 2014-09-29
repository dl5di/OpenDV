/*
 *   Copyright (C) 2009,2011,2012 by Jonathan Naylor G4KLX
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

#include "ParrotControllerProtocolHandler.h"
#include "CCITTChecksumReverse.h"
#include "DStarDefines.h"
#include "Utils.h"

const unsigned int BUFFER_LENGTH = 255U;

CParrotControllerProtocolHandler::CParrotControllerProtocolHandler(const wxString& repeaterAddress, unsigned int repeaterPort, const wxString& localAddress, unsigned int localPort) :
m_socket(repeaterAddress, repeaterPort, localAddress, localPort),
m_outId(0U),
m_outSeq(0U),
m_type(NETWORK_NONE),
m_buffer(NULL),
m_length(0U)
{
	m_buffer = new unsigned char[BUFFER_LENGTH];

	wxDateTime now = wxDateTime::UNow();
	::srand(now.GetMillisecond());
}

CParrotControllerProtocolHandler::~CParrotControllerProtocolHandler()
{
	delete[] m_buffer;
}

bool CParrotControllerProtocolHandler::open()
{
	return m_socket.open();
}

bool CParrotControllerProtocolHandler::writeHeader(const CHeaderData& header)
{
	unsigned char buffer[50U];

	buffer[0] = 'D';
	buffer[1] = 'S';
	buffer[2] = 'R';
	buffer[3] = 'P';

	buffer[4] = 0x20U;

	// Create a random id for this transmission
	m_outId = (::rand() % 65535U) + 1U;

	buffer[5] = m_outId / 256U;	// Unique session id
	buffer[6] = m_outId % 256U;

	buffer[7] = 0U;

	buffer[8]  = header.getFlag1();				// Flags 1, 2 and 3
	buffer[9]  = header.getFlag2();
	buffer[10] = header.getFlag3();

	for (unsigned int i = 0U; i < LONG_CALLSIGN_LENGTH; i++)
		buffer[11 + i] = header.getRptCall1().GetChar(i);

	for (unsigned int i = 0U; i < LONG_CALLSIGN_LENGTH; i++)
		buffer[19 + i] = header.getRptCall2().GetChar(i);

	for (unsigned int i = 0U; i < LONG_CALLSIGN_LENGTH; i++)
		buffer[27 + i] = header.getYourCall().GetChar(i);

	for (unsigned int i = 0U; i < LONG_CALLSIGN_LENGTH; i++)
		buffer[35 + i] = header.getMyCall1().GetChar(i);

	for (unsigned int i = 0U; i < SHORT_CALLSIGN_LENGTH; i++)
		buffer[43 + i] = header.getMyCall2().GetChar(i);

	// Get the checksum for the header
	CCCITTChecksumReverse csum;
	csum.update(buffer + 8U, 4U * LONG_CALLSIGN_LENGTH + SHORT_CALLSIGN_LENGTH + 3U);
	csum.result(buffer + 47U);

	m_outSeq = 0U;

#if defined(DUMP_TX)
	CUtils::dump(wxT("Sending Header"), buffer, 49U);
	return true;
#else
	return m_socket.write(buffer, 49U);
#endif
}

bool CParrotControllerProtocolHandler::writeData(const unsigned char* data, unsigned int length, bool end)
{
	wxASSERT(data != NULL);
	wxASSERT(length >= DV_FRAME_LENGTH_BYTES);

	unsigned char buffer[25U];

	buffer[0] = 'D';
	buffer[1] = 'S';
	buffer[2] = 'R';
	buffer[3] = 'P';

	buffer[4] = 0x21U;

	buffer[5] = m_outId / 256U;	// Unique session id
	buffer[6] = m_outId % 256U;

	buffer[7] = m_outSeq;
	if (end)
		buffer[7] |= 0x40U;

	buffer[8] = 0U;

	// Copy the AMBE data
	buffer[9] = data[0];
	buffer[10] = data[1];
	buffer[11] = data[2];
	buffer[12] = data[3];
	buffer[13] = data[4];
	buffer[14] = data[5];
	buffer[15] = data[6];
	buffer[16] = data[7];
	buffer[17] = data[8];

	buffer[18] = data[9];
	buffer[19] = data[10];
	buffer[20] = data[11];

	m_outSeq++;
	if (m_outSeq > 0x14U)
		m_outSeq = 0U;

#if defined(DUMP_TX)
	CUtils::dump(wxT("Sending Data"), buffer, 21U);
	return true;
#else
	return m_socket.write(buffer, 21U);
#endif
}

NETWORK_TYPE CParrotControllerProtocolHandler::read()
{
	bool res = true;

	// Loop until we have no more data from the socket or we have data for the higher layers
	while (res)
		res = readPackets();

	return m_type;
}

bool CParrotControllerProtocolHandler::readPackets()
{
	m_type = NETWORK_NONE;

	// No more data?
	int length = m_socket.read(m_buffer, BUFFER_LENGTH);
	if (length <= 0)
		return false;

	m_length = length;

	// Invalid packet type?
	if (m_buffer[0] == 'D' || m_buffer[1] == 'S' || m_buffer[2] == 'R' || m_buffer[3] == 'P') {
		// Poll data
		if (m_buffer[4] == 0x0AU)
			return true;

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

CHeaderData* CParrotControllerProtocolHandler::readHeader()
{
	if (m_type != NETWORK_HEADER)
		return NULL;

	// Header checksum testing is enabled
	CHeaderData* header = new CHeaderData(m_buffer + 8U, RADIO_HEADER_LENGTH_BYTES, true);

	if (!header->isValid()) {
		CUtils::dump(wxT("Header checksum failure from the Repeater"), m_buffer + 8U, RADIO_HEADER_LENGTH_BYTES);
		delete header;
		return NULL;
	}

	return header;
}

unsigned int CParrotControllerProtocolHandler::readData(unsigned char* buffer, unsigned int length, bool& end)
{
	if (m_type != NETWORK_DATA)
		return 0U;

	// Get the data length
	unsigned int dataLen = m_length - 9U;

	// Is our buffer too small?
	if (dataLen > length)
		dataLen = length;

	// Is it the end of the data?
	end = (m_buffer[7] & 0x40) == 0x40;

	::memcpy(buffer, m_buffer + 9U, dataLen);

	return dataLen;
}

void CParrotControllerProtocolHandler::close()
{
	m_socket.close();
}
