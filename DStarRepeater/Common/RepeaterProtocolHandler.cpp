/*
 *   Copyright (C) 2009-2014 by Jonathan Naylor G4KLX
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

#include "RepeaterProtocolHandler.h"
#include "CCITTChecksumReverse.h"
#include "DStarDefines.h"
#include "Utils.h"

// #define	DUMP_TX

const unsigned int BUFFER_LENGTH = 255U;

CRepeaterProtocolHandler::CRepeaterProtocolHandler(const wxString& gatewayAddress, unsigned int gatewayPort, const wxString& localAddress, unsigned int localPort, const wxString& name) :
m_socket(localAddress, localPort),
m_address(),
m_port(gatewayPort),
m_name(name),
m_outId(0U),
m_outSeq(0U),
m_type(NETWORK_NONE),
m_inId(0U),
m_buffer(NULL),
m_length(0U)
{
	m_address = CUDPReaderWriter::lookup(gatewayAddress);

	m_buffer = new unsigned char[BUFFER_LENGTH];

	wxDateTime now = wxDateTime::UNow();
	::srand(now.GetMillisecond());
}

CRepeaterProtocolHandler::~CRepeaterProtocolHandler()
{
	delete[] m_buffer;
}

bool CRepeaterProtocolHandler::open()
{
	if (m_address.s_addr == INADDR_NONE)
		return false;

	return m_socket.open();
}

bool CRepeaterProtocolHandler::writeHeader(const CHeaderData& header)
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

	buffer[8]  = header.getFlag1();
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
#endif

	for (unsigned int i = 0U; i < 2U; i++) {
		bool ret = m_socket.write(buffer, 49U, m_address, m_port);
		if (!ret)
			return false;
	}

	return true;
}

bool CRepeaterProtocolHandler::writeData(const unsigned char* data, unsigned int length, unsigned int errors, bool end)
{
	wxASSERT(data != NULL);
	wxASSERT(length == DV_FRAME_LENGTH_BYTES || length == DV_FRAME_MAX_LENGTH_BYTES);

	unsigned char buffer[30U];

	buffer[0] = 'D';
	buffer[1] = 'S';
	buffer[2] = 'R';
	buffer[3] = 'P';

	buffer[4] = 0x21U;

	buffer[5] = m_outId / 256U;	// Unique session id
	buffer[6] = m_outId % 256U;

	// If this is a data sync, reset the sequence to zero
	if (data[9] == 0x55 && data[10] == 0x2D && data[11] == 0x16)
		m_outSeq = 0U;

	buffer[7] = m_outSeq;
	if (end)
		buffer[7] |= 0x40U;			// End of data marker

	buffer[8] = errors;

	m_outSeq++;
	if (m_outSeq > 0x14U)
		m_outSeq = 0U;

	::memcpy(buffer + 9U, data, length);

#if defined(DUMP_TX)
	CUtils::dump(wxT("Sending Data"), buffer, length + 9U);
#endif

	return m_socket.write(buffer, length + 9U, m_address, m_port);
}

bool CRepeaterProtocolHandler::writeBusyHeader(const CHeaderData& header)
{
	unsigned char buffer[50U];

	buffer[0] = 'D';
	buffer[1] = 'S';
	buffer[2] = 'R';
	buffer[3] = 'P';

	buffer[4] = 0x22U;

	// Create a random id for this header
	m_outId = (::rand() % 65535U) + 1U;

	buffer[5] = m_outId / 256U;		// Unique session id
	buffer[6] = m_outId % 256U;

	buffer[7] = 0U;

	buffer[8]  = header.getFlag1();
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
	CUtils::dump(wxT("Sending Busy Header"), buffer, 49U);
#endif

	return m_socket.write(buffer, 49U, m_address, m_port);
}

bool CRepeaterProtocolHandler::writeBusyData(const unsigned char* data, unsigned int length, unsigned int errors, bool end)
{
	wxASSERT(data != NULL);
	wxASSERT(length == DV_FRAME_LENGTH_BYTES || length == DV_FRAME_MAX_LENGTH_BYTES);

	unsigned char buffer[30U];

	buffer[0] = 'D';
	buffer[1] = 'S';
	buffer[2] = 'R';
	buffer[3] = 'P';

	buffer[4] = 0x23U;

	buffer[5] = m_outId / 256U;	// Unique session id
	buffer[6] = m_outId % 256U;

	// If this is a data sync, reset the sequence to zero
	if (data[9] == 0x55 && data[10] == 0x2D && data[11] == 0x16)
		m_outSeq = 0U;

	buffer[7] = m_outSeq;
	if (end)
		buffer[7] |= 0x40U;			// End of data marker

	buffer[8] = errors;

	m_outSeq++;
	if (m_outSeq > 0x14U)
		m_outSeq = 0U;

	::memcpy(buffer + 9U, data, length);

#if defined(DUMP_TX)
	CUtils::dump(wxT("Sending Busy Data"), buffer, length + 9U);
#endif

	return m_socket.write(buffer, length + 9U, m_address, m_port);
}

bool CRepeaterProtocolHandler::writePoll(const wxString& text)
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
#endif

	return m_socket.write(buffer, 6U + length, m_address, m_port);
}

bool CRepeaterProtocolHandler::writeRegister()
{
	unsigned char buffer[40U];

	buffer[0] = 'D';
	buffer[1] = 'S';
	buffer[2] = 'R';
	buffer[3] = 'P';

	buffer[4] = 0x0B;				// Register with name

	unsigned int length = m_name.Length();

	for (unsigned int i = 0U; i < length; i++)
		buffer[5U + i] = m_name.GetChar(i);

	buffer[5U + length] = 0x00;

#if defined(DUMP_TX)
	CUtils::dump(wxT("Sending Register"), buffer, 6U + length);
#endif

	return m_socket.write(buffer, 6U + length, m_address, m_port);
}

NETWORK_TYPE CRepeaterProtocolHandler::read()
{
	bool res = true;

	// Loop until we have no more data from the socket or we have data for the higher layers
	while (res)
		res = readPackets();

	return m_type;
}

bool CRepeaterProtocolHandler::readPackets()
{
	m_type = NETWORK_NONE;

	// No more data?
	in_addr address;
	unsigned int port;
	int length = m_socket.read(m_buffer, BUFFER_LENGTH, address, port);
	if (length <= 0)
		return false;

	// Check if the data is for us
	if (m_address.s_addr != address.s_addr || m_port != port) {
		wxLogMessage(wxT("Packet received from an invalid source, %08X != %08X and/or %u != %u"), m_address.s_addr, address.s_addr, m_port, port);
		CUtils::dump(wxT("Data"), m_buffer, length);
		return false;
	}

	m_length = length;

	// Invalid packet type?
	if (m_buffer[0] == 'D' && m_buffer[1] == 'S' && m_buffer[2] == 'R' && m_buffer[3] == 'P') {
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
			wxUint16 id = m_buffer[5] * 256U + m_buffer[6];

			// Are we listening for headers?
			if (m_inId != 0U)
				return true;

			m_inId = id;					// Take the stream id
			m_type = NETWORK_HEADER;
			return false;
		}

		// User data
		else if (m_buffer[4] == 0x21U) {
			wxUint16 id = m_buffer[5] * 256U + m_buffer[6];

			// Check that the stream id matches the valid header, reject otherwise
			if (id != m_inId)
				return true;

			// Is this the last packet in the stream?
			if ((m_buffer[7] & 0x40) == 0x40)
				m_inId = 0U;

			m_type = NETWORK_DATA;
			return false;
		}

		else if (m_buffer[4] == 0x24U) {
			// Silently ignore DD data
		}
	}

	CUtils::dump(wxT("Unknown packet from the Gateway"), m_buffer, m_length);

	return true;
}

CHeaderData* CRepeaterProtocolHandler::readHeader()
{
	if (m_type != NETWORK_HEADER)
		return NULL;

	// If the checksum is 0xFFFF then we accept the header without testing the checksum
	if (m_buffer[47U] == 0xFFU && m_buffer[48U] == 0xFFU)
		return new CHeaderData(m_buffer + 8U, RADIO_HEADER_LENGTH_BYTES, false);

	// Header checksum testing is enabled
	CHeaderData* header = new CHeaderData(m_buffer + 8U, RADIO_HEADER_LENGTH_BYTES, true);

	if (!header->isValid()) {
		CUtils::dump(wxT("Header checksum failure from the Gateway"), m_buffer + 8U, RADIO_HEADER_LENGTH_BYTES);
		delete header;
		return NULL;
	}

	return header;
}

unsigned int CRepeaterProtocolHandler::readData(unsigned char* buffer, unsigned int length, unsigned char& seqNo)
{
	if (m_type != NETWORK_DATA)
		return 0U;

	unsigned int dataLen = m_length - 9U;

	// Is our buffer too small?
	if (dataLen > length)
		dataLen = length;

	seqNo = m_buffer[7U];

	::memcpy(buffer, m_buffer + 9U, dataLen);

	// Simple sanity checks of the incoming sync bits
	if (seqNo == 0U) {
		// Regenerate sync bytes
		buffer[9U]  = DATA_SYNC_BYTES[0U];
		buffer[10U] = DATA_SYNC_BYTES[1U];
		buffer[11U] = DATA_SYNC_BYTES[2U];
	} else if (::memcmp(buffer + 9U, DATA_SYNC_BYTES, DATA_FRAME_LENGTH_BYTES) == 0) {
		// Sync bytes appearing where they shouldn't!
		buffer[9U]  = 0x70U;
		buffer[10U] = 0x4FU;
		buffer[11U] = 0x93U;
	}

	return dataLen;
}

void CRepeaterProtocolHandler::readText(wxString& text, LINK_STATUS& status, wxString& reflector)
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

void CRepeaterProtocolHandler::readTempText(wxString& text)
{
	if (m_type != NETWORK_TEMPTEXT) {
		text = wxT("                    ");
		return;
	}

	text = wxString((char*)(m_buffer + 5U), wxConvLocal, 20U);
}

wxString CRepeaterProtocolHandler::readStatus1()
{
	if (m_type != NETWORK_STATUS1)
		return wxEmptyString;

	return wxString((char*)(m_buffer + 6U), wxConvLocal, 20U);
}

wxString CRepeaterProtocolHandler::readStatus2()
{
	if (m_type != NETWORK_STATUS2)
		return wxEmptyString;

	return wxString((char*)(m_buffer + 6U), wxConvLocal, 20U);
}

wxString CRepeaterProtocolHandler::readStatus3()
{
	if (m_type != NETWORK_STATUS3)
		return wxEmptyString;

	return wxString((char*)(m_buffer + 6U), wxConvLocal, 20U);
}

wxString CRepeaterProtocolHandler::readStatus4()
{
	if (m_type != NETWORK_STATUS4)
		return wxEmptyString;

	return wxString((char*)(m_buffer + 6U), wxConvLocal, 20U);
}

wxString CRepeaterProtocolHandler::readStatus5()
{
	if (m_type != NETWORK_STATUS5)
		return wxEmptyString;

	return wxString((char*)(m_buffer + 6U), wxConvLocal, 20U);
}

void CRepeaterProtocolHandler::reset()
{
	m_inId = 0U;
}

void CRepeaterProtocolHandler::close()
{
	m_socket.close();
}
