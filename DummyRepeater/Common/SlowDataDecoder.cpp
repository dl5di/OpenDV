/*
 *	Copyright (C) 2009,2011,2014 by Jonathan Naylor, G4KLX
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

CSlowDataDecoder::CSlowDataDecoder() :
m_buffer(NULL),
m_state(SDD_FIRST),
m_message(NULL),
m_messageData(NULL)
{
	m_buffer  = new unsigned char[6U];
	m_message = new unsigned char[SLOW_DATA_TEXT_LENGTH]; 

	::memset(m_message, 0xFFU, SLOW_DATA_TEXT_LENGTH);
}

CSlowDataDecoder::~CSlowDataDecoder()
{
	delete[] m_buffer;
	delete[] m_message;

	delete m_messageData;
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
		case SLOW_DATA_TYPE_TEXT:
			processMessage();
			break;
		default:
			break;
	}
}

CMessageData* CSlowDataDecoder::getMessageData()
{
	if (m_messageData == NULL)
		return NULL;

	CMessageData* temp = new CMessageData(*m_messageData);

	delete m_messageData;
	m_messageData = NULL;

	::memset(m_message, 0xFFU, SLOW_DATA_TEXT_LENGTH);

	return temp;
}

void CSlowDataDecoder::sync()
{
	m_state = SDD_FIRST;
}

void CSlowDataDecoder::reset()
{
	::memset(m_message, 0xFFU, SLOW_DATA_TEXT_LENGTH);

	m_state = SDD_FIRST;

	delete m_messageData;
	m_messageData = NULL;
}

void CSlowDataDecoder::processMessage()
{
	// Do we have a complete message?
	if (m_messageData != NULL)
		return;

	unsigned int num = m_buffer[0] & SLOW_DATA_LENGTH_MASK;

	switch (num) {
		case 0U:
			m_message[0U] = m_buffer[1U] & 0x7FU;
			m_message[1U] = m_buffer[2U] & 0x7FU;
			m_message[2U] = m_buffer[3U] & 0x7FU;
			m_message[3U] = m_buffer[4U] & 0x7FU;
			m_message[4U] = m_buffer[5U] & 0x7FU;
			break;
		case 1U:
			m_message[5U] = m_buffer[1U] & 0x7FU;
			m_message[6U] = m_buffer[2U] & 0x7FU;
			m_message[7U] = m_buffer[3U] & 0x7FU;
			m_message[8U] = m_buffer[4U] & 0x7FU;
			m_message[9U] = m_buffer[5U] & 0x7FU;
			break;
		case 2U:
			m_message[10U] = m_buffer[1U] & 0x7FU;
			m_message[11U] = m_buffer[2U] & 0x7FU;
			m_message[12U] = m_buffer[3U] & 0x7FU;
			m_message[13U] = m_buffer[4U] & 0x7FU;
			m_message[14U] = m_buffer[5U] & 0x7FU;
			break;
		case 3U:
			m_message[15U] = m_buffer[1U] & 0x7FU;
			m_message[16U] = m_buffer[2U] & 0x7FU;
			m_message[17U] = m_buffer[3U] & 0x7FU;
			m_message[18U] = m_buffer[4U] & 0x7FU;
			m_message[19U] = m_buffer[5U] & 0x7FU;
			break;
		default:
			return;
	}

	for (unsigned int i = 0U; i < SLOW_DATA_TEXT_LENGTH; i++) {
		if (m_message[i] == 0xFFU)
			return;
	}

	m_messageData = new CMessageData(m_message, SLOW_DATA_TEXT_LENGTH);
}

