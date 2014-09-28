/*
 *	Copyright (C) 2010,2011 by Jonathan Naylor, G4KLX
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

const unsigned int  SLOW_DATA_BLOCK_SIZE = 6U;

const unsigned int  SLOW_DATA_FULL_BLOCK_SIZE = SLOW_DATA_BLOCK_SIZE * 10U;

CSlowDataEncoder::CSlowDataEncoder() :
m_headerData(NULL),
m_textData(NULL),
m_headerPtr(0U),
m_textPtr(0U)
{
	m_headerData = new unsigned char[SLOW_DATA_FULL_BLOCK_SIZE];
	m_textData   = new unsigned char[SLOW_DATA_FULL_BLOCK_SIZE];

	::memset(m_headerData, 'f', SLOW_DATA_FULL_BLOCK_SIZE);
	::memset(m_textData,   'f', SLOW_DATA_FULL_BLOCK_SIZE);
}

CSlowDataEncoder::~CSlowDataEncoder()
{
	delete[] m_headerData;
	delete[] m_textData;
}

void CSlowDataEncoder::getHeaderData(unsigned char* data)
{
	wxASSERT(data != NULL);

	data[0U] = m_headerData[m_headerPtr++] ^ SCRAMBLER_BYTE1;
	data[1U] = m_headerData[m_headerPtr++] ^ SCRAMBLER_BYTE2;
	data[2U] = m_headerData[m_headerPtr++] ^ SCRAMBLER_BYTE3;

	if (m_headerPtr >= SLOW_DATA_FULL_BLOCK_SIZE)
		m_headerPtr = 0U;
}

void CSlowDataEncoder::getTextData(unsigned char* data)
{
	wxASSERT(data != NULL);

	data[0U] = m_textData[m_textPtr++] ^ SCRAMBLER_BYTE1;
	data[1U] = m_textData[m_textPtr++] ^ SCRAMBLER_BYTE2;
	data[2U] = m_textData[m_textPtr++] ^ SCRAMBLER_BYTE3;

	if (m_textPtr >= SLOW_DATA_FULL_BLOCK_SIZE)
		m_textPtr = 0U;
}

void CSlowDataEncoder::sync()
{
	m_headerPtr = 0U;
	m_textPtr   = 0U;
}

void CSlowDataEncoder::reset()
{
	m_headerPtr = 0U;
	m_textPtr   = 0U;

	::memset(m_headerData, 'f', SLOW_DATA_FULL_BLOCK_SIZE);
	::memset(m_textData,   'f', SLOW_DATA_FULL_BLOCK_SIZE);
}

void CSlowDataEncoder::setHeaderData(const CHeaderData& header)
{
	::memset(m_headerData, 'f', SLOW_DATA_FULL_BLOCK_SIZE);

	m_headerData[0U]  = SLOW_DATA_TYPE_HEADER | 5U;
	m_headerData[1U]  = header.getFlag1();
	m_headerData[2U]  = header.getFlag2();
	m_headerData[3U]  = header.getFlag3();
	m_headerData[4U]  = header.getRptCall2().GetChar(0);
	m_headerData[5U]  = header.getRptCall2().GetChar(1);

	m_headerData[6U]  = SLOW_DATA_TYPE_HEADER | 5U;
	m_headerData[7U]  = header.getRptCall2().GetChar(2);
	m_headerData[8U]  = header.getRptCall2().GetChar(3);
	m_headerData[9U]  = header.getRptCall2().GetChar(4);
	m_headerData[10U] = header.getRptCall2().GetChar(5);
	m_headerData[11U] = header.getRptCall2().GetChar(6);

	m_headerData[12U] = SLOW_DATA_TYPE_HEADER | 5U;
	m_headerData[13U] = header.getRptCall2().GetChar(7);
	m_headerData[14U] = header.getRptCall1().GetChar(0);
	m_headerData[15U] = header.getRptCall1().GetChar(1);
	m_headerData[16U] = header.getRptCall1().GetChar(2);
	m_headerData[17U] = header.getRptCall1().GetChar(3);

	m_headerData[18U] = SLOW_DATA_TYPE_HEADER | 5U;
	m_headerData[19U] = header.getRptCall1().GetChar(4);
	m_headerData[20U] = header.getRptCall1().GetChar(5);
	m_headerData[21U] = header.getRptCall1().GetChar(6);
	m_headerData[22U] = header.getRptCall1().GetChar(7);
	m_headerData[23U] = header.getYourCall().GetChar(0);

	m_headerData[24U] = SLOW_DATA_TYPE_HEADER | 5U;
	m_headerData[25U] = header.getYourCall().GetChar(1);
	m_headerData[26U] = header.getYourCall().GetChar(2);
	m_headerData[27U] = header.getYourCall().GetChar(3);
	m_headerData[28U] = header.getYourCall().GetChar(4);
	m_headerData[29U] = header.getYourCall().GetChar(5);

	m_headerData[30U] = SLOW_DATA_TYPE_HEADER | 5U;
	m_headerData[31U] = header.getYourCall().GetChar(6);
	m_headerData[32U] = header.getYourCall().GetChar(7);
	m_headerData[33U] = header.getMyCall1().GetChar(0);
	m_headerData[34U] = header.getMyCall1().GetChar(1);
	m_headerData[35U] = header.getMyCall1().GetChar(2);

	m_headerData[36U] = SLOW_DATA_TYPE_HEADER | 5U;
	m_headerData[37U] = header.getMyCall1().GetChar(3);
	m_headerData[38U] = header.getMyCall1().GetChar(4);
	m_headerData[39U] = header.getMyCall1().GetChar(5);
	m_headerData[40U] = header.getMyCall1().GetChar(6);
	m_headerData[41U] = header.getMyCall1().GetChar(7);

	m_headerData[42U] = SLOW_DATA_TYPE_HEADER | 5U;
	m_headerData[43U] = header.getMyCall2().GetChar(0);
	m_headerData[44U] = header.getMyCall2().GetChar(1);
	m_headerData[45U] = header.getMyCall2().GetChar(2);
	m_headerData[46U] = header.getMyCall2().GetChar(3);

	CCCITTChecksumReverse cksum;
	cksum.update(m_headerData + 1U, 5U);
	cksum.update(m_headerData + 7U, 5U);
	cksum.update(m_headerData + 13U, 5U);
	cksum.update(m_headerData + 19U, 5U);
	cksum.update(m_headerData + 25U, 5U);
	cksum.update(m_headerData + 31U, 5U);
	cksum.update(m_headerData + 37U, 5U);
	cksum.update(m_headerData + 43U, 4U);

	unsigned char checkSum[2U];
	cksum.result(checkSum);

	m_headerData[47U] = checkSum[0];

	m_headerData[48U] = SLOW_DATA_TYPE_HEADER | 1U;
	m_headerData[49U] = checkSum[1];

	m_headerPtr = 0U;
}

void CSlowDataEncoder::setTextData(const wxString& text)
{
	::memset(m_textData, 'f', SLOW_DATA_FULL_BLOCK_SIZE);

	m_textData[0U]  = SLOW_DATA_TYPE_TEXT | 0U;
	m_textData[1U]  = text.GetChar(0);
	m_textData[2U]  = text.GetChar(1);
	m_textData[3U]  = text.GetChar(2);
	m_textData[4U]  = text.GetChar(3);
	m_textData[5U]  = text.GetChar(4);

	m_textData[6U]  = SLOW_DATA_TYPE_TEXT | 1U;
	m_textData[7U]  = text.GetChar(5);
	m_textData[8U]  = text.GetChar(6);
	m_textData[9U]  = text.GetChar(7);
	m_textData[10U] = text.GetChar(8);
	m_textData[11U] = text.GetChar(9);

	m_textData[12U] = SLOW_DATA_TYPE_TEXT | 2U;
	m_textData[13U] = text.GetChar(10);
	m_textData[14U] = text.GetChar(11);
	m_textData[15U] = text.GetChar(12);
	m_textData[16U] = text.GetChar(13);
	m_textData[17U] = text.GetChar(14);

	m_textData[18U] = SLOW_DATA_TYPE_TEXT | 3U;
	m_textData[19U] = text.GetChar(15);
	m_textData[20U] = text.GetChar(16);
	m_textData[21U] = text.GetChar(17);
	m_textData[22U] = text.GetChar(18);
	m_textData[23U] = text.GetChar(19);

	m_textPtr = 0U;
}

