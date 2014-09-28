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

#include "DExtraProtocolHandler.h"

#include "DStarDefines.h"
#include "Utils.h"

// #define	DUMP_TX

const unsigned int BUFFER_LENGTH = 255U;

CDExtraProtocolHandler::CDExtraProtocolHandler() :
m_socket(NULL),
m_callsign(NULL),
m_module('C'),
m_outId(0U),
m_outSeq(0U),
m_type(NETWORK_NONE),
m_inId(0U),
m_inSeq(0U),
m_buffer(NULL),
m_length(0U),
m_pollTimer(TICKS_PER_SEC, 10U),
m_connectTimer(TICKS_PER_SEC, 60U)
{
	m_callsign = new unsigned char[LONG_CALLSIGN_LENGTH];

	::memset(m_callsign, ' ', LONG_CALLSIGN_LENGTH);

	m_buffer = new unsigned char[BUFFER_LENGTH];

	wxDateTime now = wxDateTime::Now();
	::srand(now.GetMillisecond());
}

CDExtraProtocolHandler::~CDExtraProtocolHandler()
{
	delete[] m_callsign;
	delete[] m_buffer;

	if (m_socket != NULL) {
		m_socket->close();
		delete m_socket;
	}
}

void CDExtraProtocolHandler::setCallsign(const wxString& callsign)
{
	for (unsigned int i = 0U; i < callsign.Len(); i++)
		m_callsign[i] = callsign.GetChar(i);
}

bool CDExtraProtocolHandler::open(const wxString& address, unsigned int port)
{
	if (m_socket != NULL) {
		m_socket->close();
		delete m_socket;
		m_socket = NULL;
	}

	CUDPReaderWriter* socket = new CUDPReaderWriter;
	bool res = socket->open(address, port);
	if (!res) {
		delete socket;
		return false;
	}

	m_socket = socket;

	writePoll();
	m_pollTimer.start();

	return true;
}

bool CDExtraProtocolHandler::writeHeader(const CHeaderData& header)
{
	wxASSERT(m_socket != NULL);

	// Create a random id for this transmission
	m_outId = (::rand() % 65535U) + 1U;

	unsigned char buffer[56U];

	buffer[0]  = 'D';
	buffer[1]  = 'S';
	buffer[2]  = 'V';
	buffer[3]  = 'T';

	buffer[4]  = 0x10;
	buffer[5]  = 0x00;
	buffer[6]  = 0x00;
	buffer[7]  = 0x00;
	buffer[8]  = 0x20;
	buffer[9]  = 0x00;
	buffer[10] = 0x01;

	buffer[11] = 0x00;				// Band

	buffer[12] = m_outId % 256U;	// Unique session id
	buffer[13] = m_outId / 256U;

	buffer[14] = 0x80;

	buffer[15] = 0x00;				// Flags 1, 2 and 3
	buffer[16] = 0x00;
	buffer[17] = 0x00;

	for (unsigned int i = 0U; i < LONG_CALLSIGN_LENGTH; i++)
		buffer[18 + i] = header.getRptCall1().GetChar(i);

	for (unsigned int i = 0U; i < LONG_CALLSIGN_LENGTH; i++)
		buffer[26 + i] = header.getRptCall2().GetChar(i);

	for (unsigned int i = 0U; i < LONG_CALLSIGN_LENGTH; i++)
		buffer[34 + i] = header.getYourCall().GetChar(i);

	for (unsigned int i = 0U; i < LONG_CALLSIGN_LENGTH; i++)
		buffer[42 + i] = header.getMyCall1().GetChar(i);

	for (unsigned int i = 0U; i < SHORT_CALLSIGN_LENGTH; i++)
		buffer[50 + i] = header.getMyCall2().GetChar(i);

	// A dummy checksum
	buffer[54] = 0xFF;
	buffer[55] = 0xFF;

	m_outSeq = 0U;

#if defined(DUMP_TX)
	CUtils::dump(wxT("Sending Header"), buffer, 56U);
	return true;
#else
	for (unsigned int i = 0U; i < 5U; i++) {
		bool res = m_socket->write(buffer, 56U);
		if (!res)
			return false;
	}

	return true;
#endif
}

bool CDExtraProtocolHandler::writeData(const unsigned char* data, unsigned int length, bool end)
{
	wxASSERT(data != NULL);
	wxASSERT(m_socket != NULL);
	wxASSERT(length == DV_FRAME_LENGTH_BYTES);

	unsigned char buffer[27U];

	buffer[0]  = 'D';
	buffer[1]  = 'S';
	buffer[2]  = 'V';
	buffer[3]  = 'T';

	buffer[4]  = 0x20;
	buffer[5]  = 0x00;
	buffer[6]  = 0x00;
	buffer[7]  = 0x00;
	buffer[8]  = 0x20;
	buffer[9]  = 0x00;
	buffer[10] = 0x01;

	buffer[11] = 0x00;				// Band

	buffer[12] = m_outId % 256U;	// Unique session id
	buffer[13] = m_outId / 256U;

	// If this is a data sync, reset the sequence to zero
	if (data[9] == 0x55 && data[10] == 0x2D && data[11] == 0x16)
		m_outSeq = 0U;

	if (end)
		buffer[14] = m_outSeq | 0x40;
	else
		buffer[14] = m_outSeq;

	// Copy the AMBE data
	buffer[15] = data[0];
	buffer[16] = data[1];
	buffer[17] = data[2];
	buffer[18] = data[3];
	buffer[19] = data[4];
	buffer[20] = data[5];
	buffer[21] = data[6];
	buffer[22] = data[7];
	buffer[23] = data[8];

	// Copy Slow Data or the first part of the end marker
	if (m_outSeq == 0U) {
		buffer[24] = 0x55;
		buffer[25] = 0x2D;
		buffer[26] = 0x16;
	} else {
		buffer[24] = data[9];
		buffer[25] = data[10];
		buffer[26] = data[11];
	}

	m_outSeq++;
	if (m_outSeq > 0x14U)
		m_outSeq = 0U;

#if defined(DUMP_TX)
	CUtils::dump(wxT("Sending Data"), buffer, 27U);
	return true;
#else
	return m_socket->write(buffer, 27U);
#endif
}

bool CDExtraProtocolHandler::writePoll()
{
	wxASSERT(m_socket != NULL);

	unsigned char buffer[9U];

	::memcpy(buffer, m_callsign, LONG_CALLSIGN_LENGTH);
	buffer[LONG_CALLSIGN_LENGTH] = 0x44;

#if defined(DUMP_TX)
	CUtils::dump(wxT("Sending Poll"), buffer, 9U);
	return true;
#else
	return m_socket->write(buffer, 9U);
#endif
}

NETWORK_TYPE CDExtraProtocolHandler::read()
{
	if (m_socket == NULL) {
		m_type = NETWORK_NONE;
		return NETWORK_NONE;
	}

	bool res = true;

	// Loop until we have no more data from the socket or we have data for the higher layers
	while (res)
		res = readPackets();

	return m_type;
}

bool CDExtraProtocolHandler::readPackets()
{
	m_type = NETWORK_NONE;

	// No more data?
	m_length = m_socket->read(m_buffer, BUFFER_LENGTH);
	if (m_length == 0U)
		return false;

	// Header or data packet type?
	if (m_buffer[0] != 'D' || m_buffer[1] != 'S' || m_buffer[2] != 'V' || m_buffer[3] != 'T') {
		if (m_length == 9U)		// Reply from the far end
			m_connectTimer.reset();

		return true;

	// User data
	} else {
		wxUint16 id = m_buffer[13] * 256U + m_buffer[12];

		if (m_buffer[14] == 0x80 && m_inId == 0U) {
			unsigned char module = m_buffer[25];	// Take the module letter from the header
			if (module != m_module)
				return true;

			m_inId = id;					// Take the stream id
			m_type = NETWORK_HEADER;
			return false;
		}

		// We're not listening for headers, throw it away
		if (m_buffer[14] == 0x80 && m_inId != 0U)
			return true;

		// Check that the stream id matches the valid header, reject otherwise
		if (id == m_inId) {
			m_inSeq = m_buffer[14];

			// Is this the last packet in the stream?
			if ((m_inSeq & 0x40) == 0x40)
				m_inId = 0U;

			m_type = NETWORK_DATA;
			return false;
		} else {
			return true;
		}
	}

	// An unknown type
	CUtils::dump(wxT("Unknown packet type from DExtra"), m_buffer, m_length);

	return true;
}

CHeaderData* CDExtraProtocolHandler::readHeader()
{
	if (m_type != NETWORK_HEADER)
		return NULL;

	// Header checksum testing is disabled
	return new CHeaderData(m_buffer + 15, RADIO_HEADER_LENGTH_BYTES, false);
}

unsigned int CDExtraProtocolHandler::readData(unsigned char* buffer, unsigned int length, bool& sync, bool& end)
{
	sync = false;
	end  = false;

	if (m_type != NETWORK_DATA)
		return 0U;

	if (length > DV_FRAME_LENGTH_BYTES)
		length = DV_FRAME_LENGTH_BYTES;

	// Is it the end of the data?
	end = (m_inSeq & 0x40) == 0x40;

	// Rebuild the sync vector
	wxUint8 seqNo = m_inSeq & 0x1F;
	if (seqNo == 0U) {
		m_buffer[24] = 0x55;
		m_buffer[25] = 0x2D;
		m_buffer[26] = 0x16;
		sync = true;
	}

	::memcpy(buffer, m_buffer + 15, length);

	return length;
}

void CDExtraProtocolHandler::clock()
{
	m_pollTimer.clock();
	m_connectTimer.clock();

	if (m_pollTimer.hasExpired()) {
		if (m_socket != NULL)
			writePoll();

		m_pollTimer.reset();
	}
}

void CDExtraProtocolHandler::reset()
{
	m_inId = 0U;
}

bool CDExtraProtocolHandler::isConnected()
{
	return m_socket != NULL && !m_connectTimer.hasExpired();
}

void CDExtraProtocolHandler::setModule(wxChar module)
{
	switch (module) {
		case wxT('A'):
			m_module = 'A';
			break;
		case wxT('B'):
			m_module = 'B';
			break;
		case wxT('C'):
			m_module = 'C';
			break;
		default:
			m_module = 'B';
			break;
	}
}

void CDExtraProtocolHandler::close()
{
	if (m_socket != NULL) {
		m_socket->close();
		delete m_socket;
		m_socket = NULL;
	}
}
