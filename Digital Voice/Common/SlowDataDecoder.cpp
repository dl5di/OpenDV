/*
 *	Copyright (C) 2009 by Jonathan Naylor, G4KLX
 *
 *	This program is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation; version 2 of the License.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 */

#include "SlowDataDecoder.h"
#include "DStarDefines.h"

const unsigned char SCRAMBLER_BYTE1 = 0x70;
const unsigned char SCRAMBLER_BYTE2 = 0x4F;
const unsigned char SCRAMBLER_BYTE3 = 0x93;

CSlowDataDecoder::CSlowDataDecoder() :
m_buffer(NULL),
m_state(SDD_FIRST),
m_header(),
m_message(),
m_messageExp(0U),
// m_gpsMessage(),
m_headerData(NULL),
m_messageData(NULL),
// m_gpsMessageData(NULL),
m_codeSquelchData(0U),
m_hasCodeSquelch(false)
{
	m_buffer = new unsigned char[6U];
}

CSlowDataDecoder::~CSlowDataDecoder()
{
	delete[] m_buffer;

	delete m_headerData;
	delete m_messageData;
//	delete m_gpsMessageData;
}

void CSlowDataDecoder::addData(const unsigned char* data)
{
	wxASSERT(data != NULL);

	switch (m_state) {
		case SDD_FIRST:
			m_buffer[0] = data[0] ^ SCRAMBLER_BYTE1;
			m_buffer[1] = data[1] ^ SCRAMBLER_BYTE2;
			m_buffer[2] = data[2] ^ SCRAMBLER_BYTE3;
			m_state = SDD_SECOND;
			return;

		case SDD_SECOND:
			m_buffer[3] = data[0] ^ SCRAMBLER_BYTE1;
			m_buffer[4] = data[1] ^ SCRAMBLER_BYTE2;
			m_buffer[5] = data[2] ^ SCRAMBLER_BYTE3;
			m_state = SDD_FIRST;
			break;
	}			

	switch (m_buffer[0] & SLOW_DATA_TYPE_MASK) {
//		case SLOW_DATA_TYPE_GPSDATA:
//			processGPSMessage();
//			break;
		case SLOW_DATA_TYPE_MESSAGE:
			processMessage();
			break;
		case SLOW_DATA_TYPE_HEADER:
			processHeader();
			break;
		case SLOW_DATA_TYPE_SQUELCH:
			processCodeSquelch();
			break;
		default:
			break;
	}
}

/*
CGPSMessageData* CSlowDataDecoder::getGPSMessageData()
{
	if (m_gpsMessageData == NULL)
		return NULL;

	CGPSMessageData* temp = new CGPSMessageData(*m_gpsMessageData);

	delete m_gpsMessageData;
	m_gpsMessageData = NULL;

	m_gpsMessage.clear();

	return temp;
}
*/
CMessageData* CSlowDataDecoder::getMessageData()
{
	if (m_messageData == NULL)
		return NULL;

	CMessageData* temp = new CMessageData(*m_messageData);

	delete m_messageData;
	m_messageData = NULL;

	m_message.clear();

	return temp;
}

unsigned int CSlowDataDecoder::getHeader(unsigned char* data, unsigned int length)
{
	wxASSERT(data != NULL);
	wxASSERT(length == RADIO_HEADER_LENGTH_BYTES);

	if (m_headerData == NULL)
		return 0U;

	unsigned int n = 0U;
	for (vector<unsigned char>::const_iterator it = m_header.begin(); it != m_header.end(); ++it, n++)
		data[n] = *it;

	delete m_headerData;
	m_headerData = NULL;

	m_header.clear();

	return RADIO_HEADER_LENGTH_BYTES;
}

CHeaderData* CSlowDataDecoder::getHeaderData()
{
	if (m_headerData == NULL)
		return NULL;

	CHeaderData* temp = new CHeaderData(*m_headerData);

	delete m_headerData;
	m_headerData = NULL;

	m_header.clear();

	return temp;
}

bool CSlowDataDecoder::getCodeSquelchData(unsigned int& value)
{
	value = m_codeSquelchData;

	return m_hasCodeSquelch;
}

void CSlowDataDecoder::sync()
{
	m_state = SDD_FIRST;
}

void CSlowDataDecoder::reset()
{
	m_header.clear();
	m_message.clear();
//	m_gpsMessage.clear();

	m_hasCodeSquelch = false;

	m_state = SDD_FIRST;

	delete m_headerData;
	delete m_messageData;
//	delete m_gpsMessageData;

	m_headerData     = NULL;
	m_messageData    = NULL;
//	m_gpsMessageData = NULL;

	m_messageExp = 0U;
	m_codeSquelchData = 0U;
}

void CSlowDataDecoder::processHeader()
{
	// Do we have a complete and valid header?
	if (m_headerData != NULL)
		return;

	unsigned int length = m_buffer[0] & SLOW_DATA_LENGTH_MASK;
	if (length > 5U)
		return;

	for (unsigned int i = 1U; i <= length; i++)
		m_header.push_back(m_buffer[i]);

	// Do we have a complete header?
	if (m_header.size() < RADIO_HEADER_LENGTH_BYTES)
		return;

	// Something has been corrupted, discard
	if (m_header.size() != RADIO_HEADER_LENGTH_BYTES) {
		m_header.clear();
		return;
	}

	// Convert the vector to an array
	unsigned char buffer[RADIO_HEADER_LENGTH_BYTES];
	unsigned int n = 0U;
	for (vector<unsigned char>::iterator it = m_header.begin(); it != m_header.end(); ++it, n++)
		buffer[n] = *it;

	m_headerData = new CHeaderData(buffer, RADIO_HEADER_LENGTH_BYTES, true);

	if (!m_headerData->isValid()) {
		m_header.clear();
		delete m_headerData;
		m_headerData = NULL;
	}
}

void CSlowDataDecoder::processMessage()
{
	// Do we have a complete message?
	if (m_messageData != NULL)
		return;

	unsigned int num = m_buffer[0] & SLOW_DATA_LENGTH_MASK;
	if (num > 3U)			// Sanity check
		return;

	// Handle any gaps in the received message
	if (num > m_messageExp) {
		for (; m_messageExp < num; m_messageExp++) {
			m_message.push_back(' ');
			m_message.push_back(' ');
			m_message.push_back(' ');
			m_message.push_back(' ');
			m_message.push_back(' ');
		}

		m_messageExp = num;
	} else {
		m_messageExp++;
	}

	m_message.push_back(m_buffer[1U]);
	m_message.push_back(m_buffer[2U]);
	m_message.push_back(m_buffer[3U]);
	m_message.push_back(m_buffer[4U]);
	m_message.push_back(m_buffer[5U]);

	// Do we have a complete message?
	if (m_message.size() < MESSAGE_LENGTH)
		return;

	// Something has become corrupted
	if (m_message.size() != MESSAGE_LENGTH) {
		m_message.clear();
		return;
	}

	// Convert the vector to an array
	unsigned char buffer[MESSAGE_LENGTH];
	unsigned int n = 0U;
	for (vector<unsigned char>::iterator it = m_message.begin(); it != m_message.end(); ++it, n++)
		buffer[n] = *it;

	m_messageData = new CMessageData(buffer, MESSAGE_LENGTH);
}
/*
void CSlowDataDecoder::processGPSMessage()
{
	if (m_gpsMessageData != NULL)
		return;

	unsigned int length = m_buffer[0] & SLOW_DATA_LENGTH_MASK;
	if (length > 5U)
		length = 5U;

	for (unsigned int i = 0U; i < length; i++)
		m_gpsMessage.push_back(m_buffer[i + 1U]);

	// Do we have a complete message?
	if (length == 5U)
		return;

	// Convert the vector to an array
	length = m_gpsMessage.size();
	unsigned char* buffer = new unsigned char[length];
	unsigned int n = 0U;
	for (vector<unsigned char>::iterator it = m_gpsMessage.begin(); it != m_gpsMessage.end(); ++it, n++)
		buffer[n] = *it;

	m_gpsMessageData = new CGPSMessageData(buffer, length);

	delete[] buffer;
}
*/
void CSlowDataDecoder::processCodeSquelch()
{
	bool ret = m_buffer[1] == m_buffer[2];
	if (ret) {
		// Convert the hex to decimal, i.e. 0x99 = 99
		m_codeSquelchData = (m_buffer[1] / 16U) * 10U + (m_buffer[1] % 16U);
		m_hasCodeSquelch  = true;
	}
}
