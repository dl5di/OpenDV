/*
 *   Copyright (C) 2010-2013 by Jonathan Naylor G4KLX
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

#include "AMBEData.h"

#include "DStarDefines.h"
#include "Utils.h"

CAMBEData::CAMBEData() :
m_rptSeq(0U),
m_outSeq(0U),
m_id(0U),
m_band1(0x00U),
m_band2(0x02U),
m_band3(0x01U),
m_data(NULL),
m_yourAddress(),
m_yourPort(0U),
m_myPort(0U),
m_errors(0U),
m_text(),
m_header()
{
	m_data = new unsigned char[DV_FRAME_LENGTH_BYTES];
}

CAMBEData::CAMBEData(const CAMBEData& data) :
m_rptSeq(data.m_rptSeq),
m_outSeq(data.m_outSeq),
m_id(data.m_id),
m_band1(data.m_band1),
m_band2(data.m_band2),
m_band3(data.m_band3),
m_data(NULL),
m_yourAddress(data.m_yourAddress),
m_yourPort(data.m_yourPort),
m_myPort(data.m_myPort),
m_errors(data.m_errors),
m_text(data.m_text),
m_header(data.m_header)
{
	m_data = new unsigned char[DV_FRAME_LENGTH_BYTES];
	::memcpy(m_data, data.m_data, DV_FRAME_LENGTH_BYTES);
}

CAMBEData::~CAMBEData()
{
	delete[] m_data;
}

bool CAMBEData::setIcomRepeaterData(const unsigned char *data, unsigned int length, const in_addr& yourAddress, unsigned int yourPort)
{
	wxASSERT(data != NULL);
	wxASSERT(length >= 29U);

	m_rptSeq = data[4] * 256U + data[5];
	m_band1  = data[11];
	m_band2  = data[12];
	m_band3  = data[13];
	m_id     = data[14] * 256U + data[15];
	m_outSeq = data[16];

	// A repeater end packet is longer than usual, so we substitute a normal length set of data
	if (isEnd()) {
		::memset(m_data, 0x00U, DV_FRAME_LENGTH_BYTES);
		::memcpy(m_data, END_PATTERN_BYTES, END_PATTERN_LENGTH_BYTES);
	} else {
		::memcpy(m_data, data + 17U, DV_FRAME_LENGTH_BYTES);
	}

	m_yourAddress = yourAddress;
	m_yourPort    = yourPort;

	return true;
}

bool CAMBEData::setHBRepeaterData(const unsigned char *data, unsigned int length, const in_addr& yourAddress, unsigned int yourPort)
{
	wxASSERT(data != NULL);
	wxASSERT(length >= 21U);

	m_id     = data[5U] * 256U + data[6U];
	m_outSeq = data[7U];
	m_errors = data[8U];

	// A repeater end packet is longer than usual, so we substitute a normal length set of data
	if (isEnd()) {
		::memset(m_data, 0x00U, DV_FRAME_LENGTH_BYTES);
		::memcpy(m_data, END_PATTERN_BYTES, END_PATTERN_LENGTH_BYTES);
	} else {
		::memcpy(m_data, data + 9U, DV_FRAME_LENGTH_BYTES);
	}

	m_yourAddress = yourAddress;
	m_yourPort    = yourPort;

	return true;
}

bool CAMBEData::setG2Data(const unsigned char *data, unsigned int length, const in_addr& yourAddress, unsigned int yourPort)
{
	wxASSERT(data != NULL);
	wxASSERT(length >= 27U);

	m_band1  = data[9];
	m_band2  = data[10];
	m_band3  = data[11];
	m_id     = data[12] * 256U + data[13];
	m_outSeq = data[14];

	::memcpy(m_data, data + 15U, DV_FRAME_LENGTH_BYTES);

	m_yourAddress = yourAddress;
	m_yourPort    = yourPort;

	return true;
}

bool CAMBEData::setDExtraData(const unsigned char *data, unsigned int length, const in_addr& yourAddress, unsigned int yourPort, unsigned int myPort)
{
	wxASSERT(data != NULL);
	wxASSERT(length >= 27U);

	m_band1  = data[9];
	m_band2  = data[10];
	m_band3  = data[11];
	m_id     = data[12] * 256U + data[13];
	m_outSeq = data[14];

	::memcpy(m_data, data + 15U, DV_FRAME_LENGTH_BYTES);

	m_yourAddress = yourAddress;
	m_yourPort    = yourPort;
	m_myPort      = myPort;

	return true;
}

bool CAMBEData::setDPlusData(const unsigned char *data, unsigned int length, const in_addr& yourAddress, unsigned int yourPort, unsigned int myPort)
{
	wxASSERT(data != NULL);
	wxASSERT(length >= 29U);

	if ((data[0] != 0x1D && data[0] != 0x20) || data[1] != 0x80) {
		CUtils::dump(wxT("Invalid AMBE length from D-Plus"), data, length);
		return false;
	}

	m_band1  = data[11];
	m_band2  = data[12];
	m_band3  = data[13];
	m_id     = data[14] * 256U + data[15];
	m_outSeq = data[16];

	if (isEnd()) {
		::memset(m_data, 0x00U, DV_FRAME_LENGTH_BYTES);
		::memcpy(m_data, END_PATTERN_BYTES, END_PATTERN_LENGTH_BYTES);
	} else {
		::memcpy(m_data, data + 17U, DV_FRAME_LENGTH_BYTES);
	}

	m_yourAddress = yourAddress;
	m_yourPort    = yourPort;
	m_myPort      = myPort;

	return true;
}

bool CAMBEData::setDCSData(const unsigned char *data, unsigned int length, const in_addr& yourAddress, unsigned int yourPort, unsigned int myPort)
{
	wxASSERT(data != NULL);
	wxASSERT(length >= 100U);

	m_header.setDCSData(data, length, yourAddress, yourPort, myPort);

	m_id     = data[44] * 256U + data[43];

	m_outSeq = data[45];

	::memcpy(m_data, data + 46U, DV_FRAME_LENGTH_BYTES);

	m_rptSeq = data[60] * 65536U + data[59] * 256U + data[58];

	m_yourAddress = yourAddress;
	m_yourPort    = yourPort;
	m_myPort      = myPort;

	return true;
}

bool CAMBEData::setCCSData(const unsigned char *data, unsigned int length, const in_addr& yourAddress, unsigned int yourPort, unsigned int myPort)
{
	wxASSERT(data != NULL);
	wxASSERT(length >= 100U);

	m_header.setCCSData(data, length, yourAddress, yourPort, myPort);

	m_id     = data[44] * 256U + data[43];

	m_outSeq = data[45];

	::memcpy(m_data, data + 46U, DV_FRAME_LENGTH_BYTES);

	m_rptSeq = data[60] * 65536U + data[59] * 256U + data[58];

	m_yourAddress = yourAddress;
	m_yourPort    = yourPort;
	m_myPort      = myPort;

	return true;
}

unsigned int CAMBEData::getIcomRepeaterData(unsigned char *data, unsigned int length) const
{
	wxASSERT(data != NULL);
	wxASSERT(length >= 32U);

	data[0] = 'D';
	data[1] = 'S';
	data[2] = 'T';
	data[3] = 'R';

	data[4] = m_rptSeq / 256U;		// Packet sequence number
	data[5] = m_rptSeq % 256U;

	data[6] = 0x73;					// Not a response
	data[7] = 0x12;					// Data type

	data[8] = 0x00;					// Length MSB
	data[9] = 0x13U;				// Length LSB

	data[10] = 0x20;				// AMBE plus Slow Data following

	data[11] = m_band1;
	data[12] = m_band2;
	data[13] = m_band3;

	data[14] = m_id / 256U;			// Unique session id
	data[15] = m_id % 256U;

	data[16] = m_outSeq;

	::memcpy(data + 17U, m_data, DV_FRAME_LENGTH_BYTES);

	return 17U + DV_FRAME_LENGTH_BYTES;
}

unsigned int CAMBEData::getHBRepeaterData(unsigned char *data, unsigned int length) const
{
	wxASSERT(data != NULL);
	wxASSERT(length >= 21U);

	data[0] = 'D';
	data[1] = 'S';
	data[2] = 'R';
	data[3] = 'P';

	data[4] = 0x21U;

	data[5] = m_id / 256U;			// Unique session id
	data[6] = m_id % 256U;

	data[7] = m_outSeq;

	data[8] = 0U;

	::memcpy(data + 9U, m_data, DV_FRAME_LENGTH_BYTES);

	return 9U + DV_FRAME_LENGTH_BYTES;
}

unsigned int CAMBEData::getG2Data(unsigned char *data, unsigned int length) const
{
	wxASSERT(data != NULL);
	wxASSERT(length >= 30U);

	data[0] = 'D';
	data[1] = 'S';
	data[2] = 'V';
	data[3] = 'T';

	data[4]  = 0x20;
	data[5]  = 0x00;
	data[6]  = 0x15;
	data[7]  = 0x09;
	data[8]  = 0x20;

	data[9]  = m_band1;
	data[10] = m_band2;
	data[11] = m_band3;

	data[12] = m_id / 256U;		// Unique session id
	data[13] = m_id % 256U;

	data[14] = m_outSeq;

	::memcpy(data + 15U, m_data, DV_FRAME_LENGTH_BYTES);

	return 15U + DV_FRAME_LENGTH_BYTES;
}

unsigned int CAMBEData::getDExtraData(unsigned char* data, unsigned int length) const
{
	wxASSERT(data != NULL);
	wxASSERT(length >= 30U);

	data[0]  = 'D';
	data[1]  = 'S';
	data[2]  = 'V';
	data[3]  = 'T';

	data[4]  = 0x20;
	data[5]  = 0x00;
	data[6]  = 0x00;
	data[7]  = 0x00;
	data[8]  = 0x20;

	data[9]  = m_band1;
	data[10] = m_band2;
	data[11] = m_band3;

	data[12] = m_id % 256U;			// Unique session id
	data[13] = m_id / 256U;

	data[14] = m_outSeq;

	::memcpy(data + 15U, m_data, DV_FRAME_LENGTH_BYTES);

	return 15U + DV_FRAME_LENGTH_BYTES;
}

unsigned int CAMBEData::getDPlusData(unsigned char* data, unsigned int length) const
{
	wxASSERT(data != NULL);
	wxASSERT(length >= 32U);

	if (isEnd()) {
		data[0]  = 0x20;
		data[1]  = 0x80;
	} else {
		data[0]  = 0x1D;
		data[1]  = 0x80;
	}

	data[2]  = 'D';
	data[3]  = 'S';
	data[4]  = 'V';
	data[5]  = 'T';

	data[6]  = 0x20;
	data[7]  = 0x00;
	data[8]  = 0x00;
	data[9]  = 0x00;
	data[10] = 0x20;

	data[11] = m_band1;
	data[12] = m_band2;
	data[13] = m_band3;

	data[14] = m_id % 256U;			// Unique session id
	data[15] = m_id / 256U;

	data[16] = m_outSeq;

	if (isEnd()) {
		::memcpy(data + 17U, NULL_AMBE_DATA_BYTES, VOICE_FRAME_LENGTH_BYTES);
		::memcpy(data + 26U, END_PATTERN_BYTES, END_PATTERN_LENGTH_BYTES);	// Add the end flag
		return 17U + DV_FRAME_MAX_LENGTH_BYTES;
	} else {
		// All other cases, just copy the payload
		::memcpy(data + 17U, m_data, DV_FRAME_LENGTH_BYTES);
		return 17U + DV_FRAME_LENGTH_BYTES;
	}
}

unsigned int CAMBEData::getDCSData(unsigned char* data, unsigned int length) const
{
	wxASSERT(data != NULL);
	wxASSERT(length >= 100U);

	::memset(data, 0x00U, 100U);

	data[0]  = '0';
	data[1]  = '0';
	data[2]  = '0';
	data[3]  = '1';

	data[43] = m_id % 256U;			// Unique session id
	data[44] = m_id / 256U;

	data[45] = m_outSeq;

	::memcpy(data + 46U, m_data, DV_FRAME_LENGTH_BYTES);

	if (isEnd()) {
		data[55] = 0x55U;
		data[56] = 0x55U;
		data[57] = 0x55U;
	}

	data[58] = (m_rptSeq >> 0)  & 0xFFU;
	data[59] = (m_rptSeq >> 8)  & 0xFFU;
	data[60] = (m_rptSeq >> 16) & 0xFFU;

	data[61] = 0x01U;
	data[62] = 0x00U;

	data[63] = 0x21U;

	for (unsigned int i = 0U; i < m_text.Len(); i++)
		data[64 + i] = m_text.GetChar(i);

	m_header.getDCSData(data, 100U);

	return 100U;
}

unsigned int CAMBEData::getCCSData(unsigned char* data, unsigned int length) const
{
	wxASSERT(data != NULL);
	wxASSERT(length >= 100U);

	::memset(data, 0x00U, 100U);

	data[0]  = '0';
	data[1]  = '0';
	data[2]  = '0';
	data[3]  = '1';

	data[43] = m_id % 256U;			// Unique session id
	data[44] = m_id / 256U;

	data[45] = m_outSeq;

	::memcpy(data + 46U, m_data, DV_FRAME_LENGTH_BYTES);

	if (isEnd()) {
		data[55] = 0x55U;
		data[56] = 0x55U;
		data[57] = 0x55U;
	}

	data[58] = (m_rptSeq >> 0)  & 0xFFU;
	data[59] = (m_rptSeq >> 8)  & 0xFFU;
	data[60] = (m_rptSeq >> 16) & 0xFFU;

	data[61] = 0x01U;
	data[62] = 0x00U;

	data[63] = 0x21U;

	for (unsigned int i = 0U; i < m_text.Len(); i++)
		data[64 + i] = m_text.GetChar(i);

	data[93U] = 0x36U;

	m_header.getCCSData(data, 100U);

	return 100U;
}

unsigned int CAMBEData::getId() const
{
	return m_id;
}

void CAMBEData::setId(unsigned int id)
{
	m_id = id;
}

unsigned char CAMBEData::getBand1() const
{
	return m_band1;
}

unsigned char CAMBEData::getBand2() const
{
	return m_band2;
}

unsigned char CAMBEData::getBand3() const
{
	return m_band3;
}

void CAMBEData::setBands(unsigned char band1, unsigned char band2, unsigned char band3)
{
	m_band1 = band1;
	m_band2 = band2;
	m_band3 = band3;
}

unsigned int CAMBEData::getRptSeq() const
{
	return m_rptSeq;
}

void CAMBEData::setRptSeq(unsigned int seqNo)
{
	m_rptSeq = seqNo;
}

unsigned int CAMBEData::getSeq() const
{
	return m_outSeq & 0x1FU;
}

void CAMBEData::setSeq(unsigned int seqNo)
{
	m_outSeq = seqNo;
}

bool CAMBEData::isEnd() const
{
	return (m_outSeq & 0x40U) == 0x40U;
}

void CAMBEData::setEnd(bool end)
{
	if (end)
		m_outSeq |= 0x40U;
	else
		m_outSeq &= ~0x40U;
}

bool CAMBEData::isSync() const
{
	return (m_outSeq & 0x1FU) == 0x00U;
}

void CAMBEData::setDestination(const in_addr& address, unsigned int port)
{
	m_yourAddress = address;
	m_yourPort    = port;
}

void CAMBEData::setText(const wxString& text)
{
	m_text = text;
}

in_addr CAMBEData::getYourAddress() const
{
	return m_yourAddress;
}

unsigned int CAMBEData::getYourPort() const
{
	return m_yourPort;
}

unsigned int CAMBEData::getMyPort() const
{
	return m_myPort;
}

CHeaderData& CAMBEData::getHeader()
{
	return m_header;
}

unsigned int CAMBEData::getErrors() const
{
	return m_errors;
}

void CAMBEData::setData(const unsigned char *data, unsigned int length)
{
	wxASSERT(data != NULL);
	wxASSERT(length >= DV_FRAME_LENGTH_BYTES);

	::memcpy(m_data, data, DV_FRAME_LENGTH_BYTES);
}

unsigned int CAMBEData::getData(unsigned char *data, unsigned int length) const
{
	wxASSERT(data != NULL);
	wxASSERT(length >= DV_FRAME_LENGTH_BYTES);

	::memcpy(data, m_data, DV_FRAME_LENGTH_BYTES);

	return DV_FRAME_LENGTH_BYTES;
}
