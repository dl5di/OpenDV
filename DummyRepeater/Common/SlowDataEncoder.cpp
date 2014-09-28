/*
 *	Copyright (C) 2009,2010,2011,2014 by Jonathan Naylor, G4KLX
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
#include "SlowDataEncoder.h"
#include "DStarDefines.h"

CSlowDataEncoder::CSlowDataEncoder() :
m_initial(),
m_body(),
m_it(),
m_end(),
m_header(),
m_message(),
m_codeSquelch()
{
}

CSlowDataEncoder::~CSlowDataEncoder()
{
}

void CSlowDataEncoder::getData(unsigned char* data)
{
	wxASSERT(data != NULL);

	// First time, we need to build the output data
	if (m_body.empty()) {
		buildData();

		// Do we have any initial data to send?
		if (m_initial.empty()) {
			m_it  = m_body.begin();
			m_end = m_body.end();
		} else {
			m_it  = m_initial.begin();
			m_end = m_initial.end();
		}
	}

	// Create the sequence: initial, body, body, body, ...
	if (m_it == m_end) {
		m_it  = m_body.begin();
		m_end = m_body.end();
	}

	// This assumes that the data is in three byte blocks and won't end half-way through
	// the loop.
	unsigned char buffer[DATA_FRAME_LENGTH_BYTES];
	for (unsigned int n = 0U; m_it != m_end && n < DATA_FRAME_LENGTH_BYTES; ++m_it, n++)
		buffer[n] = *m_it;

	data[0U] = buffer[0U] ^ SCRAMBLER_BYTE1;
	data[1U] = buffer[1U] ^ SCRAMBLER_BYTE2;
	data[2U] = buffer[2U] ^ SCRAMBLER_BYTE3;
}

void CSlowDataEncoder::reset()
{
	m_initial.clear();
	m_body.clear();

	m_header.clear();
	m_message.clear();
	m_codeSquelch.clear();
}

void CSlowDataEncoder::setHeaderData(const CHeaderData& header)
{
	CCCITTChecksumReverse cksum;

	unsigned char buffer[5U];

	m_header.push_back(SLOW_DATA_TYPE_HEADER | 5U);
	m_header.push_back(buffer[0] = header.getFlag1());
	m_header.push_back(buffer[1] = header.getFlag2());
	m_header.push_back(buffer[2] = header.getFlag3());
	m_header.push_back(buffer[3] = header.getRptCall1().GetChar(0));
	m_header.push_back(buffer[4] = header.getRptCall1().GetChar(1));
	cksum.update(buffer, 5U);

	m_header.push_back(SLOW_DATA_TYPE_HEADER | 5U);
	m_header.push_back(buffer[0] = header.getRptCall1().GetChar(2));
	m_header.push_back(buffer[1] = header.getRptCall1().GetChar(3));
	m_header.push_back(buffer[2] = header.getRptCall1().GetChar(4));
	m_header.push_back(buffer[3] = header.getRptCall1().GetChar(5));
	m_header.push_back(buffer[4] = header.getRptCall1().GetChar(6));
	cksum.update(buffer, 5U);

	m_header.push_back(SLOW_DATA_TYPE_HEADER | 5U);
	m_header.push_back(buffer[0] = header.getRptCall1().GetChar(7));
	m_header.push_back(buffer[1] = header.getRptCall2().GetChar(0));
	m_header.push_back(buffer[2] = header.getRptCall2().GetChar(1));
	m_header.push_back(buffer[3] = header.getRptCall2().GetChar(2));
	m_header.push_back(buffer[4] = header.getRptCall2().GetChar(3));
	cksum.update(buffer, 5U);

	m_header.push_back(SLOW_DATA_TYPE_HEADER | 5U);
	m_header.push_back(buffer[0] = header.getRptCall2().GetChar(4));
	m_header.push_back(buffer[1] = header.getRptCall2().GetChar(5));
	m_header.push_back(buffer[2] = header.getRptCall2().GetChar(6));
	m_header.push_back(buffer[3] = header.getRptCall2().GetChar(7));
	m_header.push_back(buffer[4] = header.getYourCall().GetChar(0));
	cksum.update(buffer, 5U);

	m_header.push_back(SLOW_DATA_TYPE_HEADER | 5U);
	m_header.push_back(buffer[0] = header.getYourCall().GetChar(1));
	m_header.push_back(buffer[1] = header.getYourCall().GetChar(2));
	m_header.push_back(buffer[2] = header.getYourCall().GetChar(3));
	m_header.push_back(buffer[3] = header.getYourCall().GetChar(4));
	m_header.push_back(buffer[4] = header.getYourCall().GetChar(5));
	cksum.update(buffer, 5U);

	m_header.push_back(SLOW_DATA_TYPE_HEADER | 5U);
	m_header.push_back(buffer[0] = header.getYourCall().GetChar(6));
	m_header.push_back(buffer[1] = header.getYourCall().GetChar(7));
	m_header.push_back(buffer[2] = header.getMyCall1().GetChar(0));
	m_header.push_back(buffer[3] = header.getMyCall1().GetChar(1));
	m_header.push_back(buffer[4] = header.getMyCall1().GetChar(2));
	cksum.update(buffer, 5U);

	m_header.push_back(SLOW_DATA_TYPE_HEADER | 5U);
	m_header.push_back(buffer[0] = header.getMyCall1().GetChar(3));
	m_header.push_back(buffer[1] = header.getMyCall1().GetChar(4));
	m_header.push_back(buffer[2] = header.getMyCall1().GetChar(5));
	m_header.push_back(buffer[3] = header.getMyCall1().GetChar(6));
	m_header.push_back(buffer[4] = header.getMyCall1().GetChar(7));
	cksum.update(buffer, 5U);

	m_header.push_back(SLOW_DATA_TYPE_HEADER | 5U);
	m_header.push_back(buffer[0] = header.getMyCall2().GetChar(0));
	m_header.push_back(buffer[1] = header.getMyCall2().GetChar(1));
	m_header.push_back(buffer[2] = header.getMyCall2().GetChar(2));
	m_header.push_back(buffer[3] = header.getMyCall2().GetChar(3));
	cksum.update(buffer, 4U);

	cksum.result(buffer);
	m_header.push_back(buffer[0]);

	m_header.push_back(SLOW_DATA_TYPE_HEADER | 1U);
	m_header.push_back(buffer[1]);
	m_header.push_back('f');
	m_header.push_back('f');
	m_header.push_back('f');
	m_header.push_back('f');
}

void CSlowDataEncoder::setMessageData(const wxString& message)
{
	if (message.IsEmpty())
		return;

	wxString text = message;
	text.Append(wxT("                    "));
	text.Truncate(SLOW_DATA_TEXT_LENGTH);

	unsigned int n = 0U;
	for (unsigned int i = 0U; i < SLOW_DATA_TEXT_LENGTH; i++) {
		if ((i % 5U) == 0U) {
			m_message.push_back(SLOW_DATA_TYPE_TEXT | n);
			n++;
		}

		m_message.push_back(text.GetChar(i));
	}
}

void CSlowDataEncoder::setCodeSquelch(unsigned char value)
{
	// Convert decimal to hex, i.e. 99 = 0x99
	unsigned char hexValue = (value / 10U) * 16U + (value % 10U);

	m_codeSquelch.push_back(SLOW_DATA_TYPE_SQUELCH | 2U);
	m_codeSquelch.push_back(hexValue);
	m_codeSquelch.push_back(hexValue);
	m_codeSquelch.push_back('f');
	m_codeSquelch.push_back('f');
	m_codeSquelch.push_back('f');
}

void CSlowDataEncoder::buildData()
{
	// Create the initial section, if needed
	if (!m_message.empty()) {
		if (m_codeSquelch.empty()) {
			// T T T T F F F F F F
			m_initial.insert(m_initial.begin(), m_message.begin(), m_message.end());
			m_initial.insert(m_initial.end(), 12U * DATA_FRAME_LENGTH_BYTES, 'f');
		} else {
			// C T T T T F F F F F
			m_initial.insert(m_initial.begin(), m_codeSquelch.begin(), m_codeSquelch.end());
			m_initial.insert(m_initial.end(), m_message.begin(), m_message.end());
			m_initial.insert(m_initial.end(), 10U * DATA_FRAME_LENGTH_BYTES, 'f');
		}
	}

	// Create the body section
	if (!m_codeSquelch.empty()) {
		// C H H H H H H H H H
		m_body.insert(m_body.begin(), m_codeSquelch.begin(), m_codeSquelch.end());
		m_body.insert(m_body.end(), m_header.begin(), m_header.end());
	} else {
		// H H H H H H H H H F
		m_body.insert(m_body.begin(), m_header.begin(), m_header.end());
		m_body.insert(m_body.end(), 2U * DATA_FRAME_LENGTH_BYTES, 'f');
	}
}
