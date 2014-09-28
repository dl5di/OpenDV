/*
 *	Copyright (C) 2009,2010,2011 by Jonathan Naylor, G4KLX
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

#include "CCITTChecksumReverse.h"
#include "SlowDataDecoder.h"
#include "DStarDefines.h"

const unsigned int  SLOW_DATA_BLOCK_SIZE = 6U;

const unsigned int  SLOW_DATA_FULL_BLOCK_SIZE = (SLOW_DATA_BLOCK_SIZE - 1U) * 10U;

CSlowDataDecoder::CSlowDataDecoder() :
m_buffer(NULL),
m_state(SDD_FIRST),
m_headerData(NULL),
m_headerPtr(0U),
m_header(NULL)
{
	m_buffer     = new unsigned char[SLOW_DATA_BLOCK_SIZE];
	m_headerData = new unsigned char[SLOW_DATA_FULL_BLOCK_SIZE];
}

CSlowDataDecoder::~CSlowDataDecoder()
{
	delete[] m_buffer;
	delete[] m_headerData;

	delete m_header;
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
			processHeader();
			break;
	}			
}

CHeaderData* CSlowDataDecoder::getHeaderData()
{
	if (m_header == NULL)
		return NULL;

	CHeaderData* temp = new CHeaderData(*m_header);

	reset();

	return temp;
}

void CSlowDataDecoder::sync()
{
	m_headerPtr = 0U;

	m_state = SDD_FIRST;
}

void CSlowDataDecoder::reset()
{
	m_headerPtr = 0U;

	m_state = SDD_FIRST;

	::memset(m_headerData, 0x00, SLOW_DATA_FULL_BLOCK_SIZE * sizeof(unsigned char));

	delete m_header;
	m_header = NULL;
}

void CSlowDataDecoder::processHeader()
{
	// Do we have a complete and valid header already?
	if (m_header != NULL)
		return;

	for (unsigned int i = 1U; i <= (SLOW_DATA_BLOCK_SIZE - 1U); i++, m_headerPtr++) {
		m_headerData[m_headerPtr] = m_buffer[i];

		if (m_headerPtr >= SLOW_DATA_FULL_BLOCK_SIZE)
			m_headerPtr = 0U;
	}

	// Look for a header starting immediately after the data sync
	bool res = processHeader(m_headerData);
	if (res)
		return;

	// Look for a header starting one block after the data sync
	processHeader(m_headerData + SLOW_DATA_BLOCK_SIZE - 1U);
}

bool CSlowDataDecoder::processHeader(const unsigned char* bytes)
{
	unsigned char buffer[RADIO_HEADER_LENGTH_BYTES];

	// Clean up the header data
	for (unsigned int n = 0U; n < RADIO_HEADER_LENGTH_BYTES; n++) {
		switch (n) {
			case 0U:		// Flag 1
				// Only allow BK and EMR bits and always assert the repeater bit
				buffer[n] = (bytes[n] & (INTERRUPTED_MASK | URGENT_MASK)) | REPEATER_MASK;
				break;
			case 1U:		// Flag 2
			case 2U:		// Flag 3
				// Always 0x00 in current implementations
				buffer[n] = 0x00;
				break;
			case 39U:	// Checksum 1
			case 40U:	// Checksum 2
				// These bytes can be any value
				buffer[n] = bytes[n];
				break;
			default:	// Callsign text
				// Only ASCII text so MSB is always clear
				buffer[n] = bytes[n] & 0x7F;
				break;
		}
	}

	CCCITTChecksumReverse cksum;
	cksum.update(buffer, RADIO_HEADER_LENGTH_BYTES - 2U);

	bool valid = cksum.check(buffer + RADIO_HEADER_LENGTH_BYTES - 2U);
	if (!valid)
		return false;

	// The checksum has already been checked
	m_header = new CHeaderData(buffer, RADIO_HEADER_LENGTH_BYTES, false);

	return true;
}
