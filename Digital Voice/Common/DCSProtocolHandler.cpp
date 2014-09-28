/*
 *   Copyright (C) 2012,2013 by Jonathan Naylor G4KLX
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

#include "DCSProtocolHandler.h"

#include "DStarDefines.h"
#include "Version.h"
#include "Utils.h"

// #define	DUMP_TX

const wxString HTML = wxT("<table border=\"0\" width=\"95%%\"><tr><td width=\"4%%\"><img border=\"0\" src=dvdongle.jpg></td><td width=\"96%%\"><font size=\"2\"><b>DONGLE</b> DCS Client %s</font></td></tr></table>");

const unsigned int BUFFER_LENGTH = 2000U;

CDCSProtocolHandler::CDCSProtocolHandler() :
m_socket(NULL),
m_callsign(NULL),
m_text(NULL),
m_reflector(NULL),
m_html(NULL),
m_outId(0U),
m_outSeq(0U),
m_outRptSeq(0U),
m_type(NETWORK_NONE),
m_inId(0U),
m_inRptSeq(0U),
m_buffer(NULL),
m_length(0U),
m_connectTimer(TICKS_PER_SEC, 1U),
m_linked(false)
{
	m_callsign  = new unsigned char[LONG_CALLSIGN_LENGTH];
	m_reflector = new unsigned char[LONG_CALLSIGN_LENGTH];
	m_text      = new unsigned char[MESSAGE_LENGTH];

	::memset(m_callsign,  ' ', LONG_CALLSIGN_LENGTH);
	::memset(m_reflector, ' ', LONG_CALLSIGN_LENGTH);
	::memset(m_text,      ' ', MESSAGE_LENGTH);

	m_buffer = new unsigned char[BUFFER_LENGTH];

	wxDateTime now = wxDateTime::Now();
	::srand(now.GetMillisecond());

	wxString html;
	html.Printf(HTML, VERSION.Left(8U).c_str());

	unsigned int len = html.Len();

	m_html = new char[len + 1U];
	::memset(m_html, 0x00, len + 1U);

	for (unsigned int i = 0U; i < len; i++)
		m_html[i] = html.GetChar(i);
}

CDCSProtocolHandler::~CDCSProtocolHandler()
{
	delete[] m_callsign;
	delete[] m_reflector;
	delete[] m_html;
	delete[] m_text;
	delete[] m_buffer;

	if (m_socket != NULL) {
		m_socket->close();
		delete m_socket;
	}
}

void CDCSProtocolHandler::setCallsign(const wxString& callsign)
{
	for (unsigned int i = 0U; i < callsign.Len(); i++)
		m_callsign[i] = callsign.GetChar(i);
}

void CDCSProtocolHandler::setText(const wxString& text)
{
	for (unsigned int i = 0U; i < text.Len(); i++)
		m_text[i] = text.GetChar(i);
}

bool CDCSProtocolHandler::open(const wxString& reflector, const wxString& address, unsigned int port)
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

	for (unsigned int i = 0U; i < reflector.Len(); i++)
		m_reflector[i] = reflector.GetChar(i);

	m_socket = socket;

	res = writeConnect();
	if (!res)
		return false;

	m_connectTimer.setTimeout(1U);
	m_connectTimer.start();

	return true;
}

void CDCSProtocolHandler::startTX()
{
	// Create a random id for this transmission
	m_outId = (::rand() % 65535U) + 1U;
	m_outSeq = 0U;
	m_outRptSeq = 0U;
}

bool CDCSProtocolHandler::writeData(const unsigned char* data, unsigned int length, bool end)
{
	wxASSERT(data != NULL);
	wxASSERT(m_socket != NULL);
	wxASSERT(length == DV_FRAME_LENGTH_BYTES);

	unsigned char buffer[100U];

	::memset(buffer, 0x00U, 100U);

	buffer[0]  = '0';
	buffer[1]  = '0';
	buffer[2]  = '0';
	buffer[3]  = '1';

	buffer[4]  = 0x00;
	buffer[5]  = 0x00;
	buffer[6]  = 0x00;

	::memcpy(buffer + 7U,  m_reflector, LONG_CALLSIGN_LENGTH);

	::memcpy(buffer + 15U, m_callsign,  LONG_CALLSIGN_LENGTH - 1U);
	buffer[22U] = 'D';

	::memcpy(buffer + 23U, "CQCQCQ  ",  LONG_CALLSIGN_LENGTH);

	::memcpy(buffer + 31U, m_callsign,  LONG_CALLSIGN_LENGTH);

	::memcpy(buffer + 39U, "DNGL",      SHORT_CALLSIGN_LENGTH);

	buffer[43] = m_outId % 256U;	// Unique session id
	buffer[44] = m_outId / 256U;

	// If this is a data sync, reset the sequence to zero
	if (data[9] == 0x55 && data[10] == 0x2D && data[11] == 0x16)
		m_outSeq = 0U;

	if (end)
		buffer[45] = m_outSeq | 0x40;
	else
		buffer[45] = m_outSeq;

	// Copy the AMBE data
	buffer[46] = data[0];
	buffer[47] = data[1];
	buffer[48] = data[2];
	buffer[49] = data[3];
	buffer[50] = data[4];
	buffer[51] = data[5];
	buffer[52] = data[6];
	buffer[53] = data[7];
	buffer[54] = data[8];

	// Copy Slow Data or the first part of the end marker
	if (end) {
		buffer[55] = 0x55;
		buffer[56] = 0x55;
		buffer[57] = 0x55;
	} else if (m_outSeq == 0U) {
		buffer[55] = 0x55;
		buffer[56] = 0x2D;
		buffer[57] = 0x16;
	} else {
		buffer[55] = data[9];
		buffer[56] = data[10];
		buffer[57] = data[11];
	}

	buffer[58] = (m_outRptSeq >> 0)  & 0xFFU;
	buffer[59] = (m_outRptSeq >> 8)  & 0xFFU;
	buffer[60] = (m_outRptSeq >> 16) & 0xFFU;

	buffer[61] = 0x01U;
	buffer[62] = 0x00U;

	buffer[63] = 0x21U;

	// Copy the slow data text
	buffer[64] = m_text[0];
	buffer[65] = m_text[1];
	buffer[66] = m_text[2];
	buffer[67] = m_text[3];
	buffer[68] = m_text[4];
	buffer[69] = m_text[5];
	buffer[70] = m_text[6];
	buffer[71] = m_text[7];
	buffer[72] = m_text[8];
	buffer[73] = m_text[9];
	buffer[74] = m_text[10];
	buffer[75] = m_text[11];
	buffer[76] = m_text[12];
	buffer[77] = m_text[13];
	buffer[78] = m_text[14];
	buffer[79] = m_text[15];
	buffer[80] = m_text[16];
	buffer[81] = m_text[17];
	buffer[82] = m_text[18];
	buffer[83] = m_text[19];

	m_outSeq++;
	if (m_outSeq > 0x14U)
		m_outSeq = 0U;

	m_outRptSeq++;

#if defined(DUMP_TX)
	CUtils::dump(wxT("Sending Data"), buffer, 100U);
	return true;
#else
	return m_socket->write(buffer, 100U);
#endif
}

NETWORK_TYPE CDCSProtocolHandler::read()
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

bool CDCSProtocolHandler::readPackets()
{
	m_type = NETWORK_NONE;

	// No more data?
	m_length = m_socket->read(m_buffer, BUFFER_LENGTH);
	if (m_length == 0U)
		return false;

	if (m_buffer[0] == '0' && m_buffer[1] == '0' && m_buffer[2] == '0' && m_buffer[3] == '1') {
		if (m_length == 100U) {
			m_type = NETWORK_DATA;
			return false;
		}
	} else if (m_buffer[0] == 'E' && m_buffer[1] == 'E' && m_buffer[2] == 'E' && m_buffer[3] == 'E') {
		// CUtils::dump(wxT("Status data"), m_buffer, m_length);
		return true;
	} else {
		switch (m_length) {
			case 22U:
				writePollReply();
				m_connectTimer.reset();
				return false;
			case 14U:
				if (::memcmp(m_buffer + 10U, "ACK", 3U) == 0 && !m_linked) {
					m_linked = true;
					m_connectTimer.setTimeout(60U);
					m_connectTimer.start();
				} else if (::memcmp(m_buffer + 10U, "NAK", 3U) == 0 && !m_linked) {
					wxString text((char*)m_reflector, wxConvLocal, LONG_CALLSIGN_LENGTH);
					wxLogMessage(wxT("Link to %s refused"), text.c_str());
					m_linked = false;
					m_connectTimer.stop();
				}
				return false;
			case 35U:
				// CUtils::dump(wxT("Status data"), m_buffer, m_length);
				return true;
			default:
				break;
		}
	}

	// An unknown type
	// CUtils::dump(wxT("Unknown packet type from DCS"), m_buffer, m_length);

	return true;
}

CHeaderData* CDCSProtocolHandler::getHeader()
{
	if (m_type != NETWORK_DATA)
		return NULL;

	return new CHeaderData(m_buffer + 4U, RADIO_HEADER_LENGTH_BYTES, false);
}

unsigned int CDCSProtocolHandler::readData(unsigned char* buffer, unsigned int length, bool& start, bool& sync, bool& end)
{
	start = false;
	sync  = false;
	end   = false;

	if (m_type != NETWORK_DATA)
		return 0U;

	if (::memcmp(m_buffer + 7U, m_reflector, LONG_CALLSIGN_LENGTH) != 0)
		return 0U;

	wxUint16 id = m_buffer[43U] * 256U + m_buffer[44U];
	if (m_inId != 0U && m_inId != id)
		return 0U;

	wxUint32 seqNo = m_buffer[60U] * 65536U + m_buffer[59U] * 256U + m_buffer[58U];

	if (m_inId == 0U) {
		start  = true;
		m_inId = id;
	} else {
		if (seqNo <= m_inRptSeq)
			return 0U;
	}

	m_inRptSeq = seqNo;

	if (length > DV_FRAME_LENGTH_BYTES)
		length = DV_FRAME_LENGTH_BYTES;

	unsigned char seq = m_buffer[45U];

	// Is it the end of the data?
	end = (seq & 0x40U) == 0x40U;
	if (end)
		m_inId = 0U;

	// Rebuild the sync vector
	if ((seq & 0x1FU) == 0U) {
		m_buffer[55] = 0x55;
		m_buffer[56] = 0x2D;
		m_buffer[57] = 0x16;
		sync = true;
}

	::memcpy(buffer, m_buffer + 46U, length);

	return length;
}

void CDCSProtocolHandler::clock()
{
	m_connectTimer.clock();

	if (m_connectTimer.hasExpired()) {
		if (!m_linked) {
			writeConnect();
			m_connectTimer.reset();
		} else {
			wxString text((char*)m_reflector, wxConvLocal, LONG_CALLSIGN_LENGTH);
			wxLogMessage(wxT("Link to %s has failed"), text.c_str());
			m_linked = false;
		}
	}
}

void CDCSProtocolHandler::reset()
{
	m_inId = 0U;
}

bool CDCSProtocolHandler::isConnected()
{
	return m_socket != NULL && m_linked;
}

void CDCSProtocolHandler::close()
{
	if (m_socket != NULL) {
		if (m_linked) {
			writeDisconnect();
			m_linked = false;
		}

		m_socket->close();
		delete m_socket;
		m_socket = NULL;

		m_connectTimer.stop();
	}
}

bool CDCSProtocolHandler::writeConnect()
{
	unsigned char buffer[520U];

	::memset(buffer, 0x00U, 519U);
	::memset(buffer, 0x20U, 19U);

	::memcpy(buffer, m_callsign, LONG_CALLSIGN_LENGTH - 1U);

	buffer[8U]  = 'D';

	buffer[9U]  = m_reflector[LONG_CALLSIGN_LENGTH - 1U];

	buffer[10U] = 0x00U;

	::memcpy(buffer + 11U, m_reflector, LONG_CALLSIGN_LENGTH - 1U);

	::memcpy(buffer + 19U, m_html, ::strlen(m_html));

#if defined(DUMP_TX)
	CUtils::dump(wxT("Sending Connect"), buffer, 519U);
	return true;
#else
	return m_socket->write(buffer, 519U);
#endif
}

bool CDCSProtocolHandler::writePollReply()
{
	unsigned char buffer[20U];

	::memset(buffer, 0x20U, 17U);

	::memcpy(buffer, m_callsign, LONG_CALLSIGN_LENGTH - 1U);

	buffer[7U]  = 'D';

	buffer[8U]  = 0x00U;

	::memcpy(buffer + 9U, m_reflector, LONG_CALLSIGN_LENGTH);

#if defined(DUMP_TX)
	CUtils::dump(wxT("Sending Poll Reply"), buffer, 17U);
	return true;
#else
	return m_socket->write(buffer, 17U);
#endif
}

bool CDCSProtocolHandler::writeDisconnect()
{
	unsigned char buffer[20U];

	::memset(buffer, 0x20U, 19U);

	::memcpy(buffer, m_callsign, LONG_CALLSIGN_LENGTH - 1U);

	buffer[8U]  = 'D';

	buffer[9U]  = 0x20U;

	buffer[10U] = 0x00U;

	::memcpy(buffer + 11U, m_reflector, LONG_CALLSIGN_LENGTH - 1U);

#if defined(DUMP_TX)
	CUtils::dump(wxT("Sending Disconnect"), buffer, 19U);
	return true;
#else
	return m_socket->write(buffer, 19U);
#endif
}
