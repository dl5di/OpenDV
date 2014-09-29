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
#include "RadioHeaderEncoder.h"
#include "DStarInterleaver.h"
#include "DStarFECEncoder.h"
#include "DStarScrambler.h"
#include "DStarDefines.h"

// #include "Utils.h"

CRadioHeaderEncoder::CRadioHeaderEncoder(const CHeaderData& header) :
m_header(NULL)
{
	m_header = new bool[FEC_SECTION_LENGTH_BITS];

	bool* buffer1 = new bool[700U];
	bool* buffer2 = new bool[700U];

	// Clear everything
	::memset(buffer1, 0x00, 700U * sizeof(bool));

	// Flag 1
	unsigned char flag1 = header.getFlag1();
	buffer1[0]  = (flag1 & 0x80U) == 0x80U; buffer1[1]  = (flag1 & 0x40U) == 0x40U;
	buffer1[2]  = (flag1 & 0x20U) == 0x20U; buffer1[3]  = (flag1 & 0x10U) == 0x10U;
	buffer1[4]  = (flag1 & 0x08U) == 0x08U; buffer1[5]  = (flag1 & 0x04U) == 0x04U;
	buffer1[6]  = (flag1 & 0x02U) == 0x02U; buffer1[7]  = (flag1 & 0x01U) == 0x01U;

	// Flag 2
	unsigned char flag2 = header.getFlag2();
	buffer1[8]  = (flag2 & 0x80U) == 0x80U; buffer1[9]  = (flag2 & 0x40U) == 0x40U;
	buffer1[10] = (flag2 & 0x20U) == 0x20U; buffer1[11] = (flag2 & 0x10U) == 0x10U;
	buffer1[12] = (flag2 & 0x08U) == 0x08U; buffer1[13] = (flag2 & 0x04U) == 0x04U;
	buffer1[14] = (flag2 & 0x02U) == 0x02U; buffer1[15] = (flag2 & 0x01U) == 0x01U;

	// Flag 3
	unsigned char flag3 = header.getFlag3();
	buffer1[16] = (flag3 & 0x80U) == 0x80U; buffer1[17] = (flag3 & 0x40U) == 0x40U;
	buffer1[18] = (flag3 & 0x20U) == 0x20U; buffer1[19] = (flag3 & 0x10U) == 0x10U;
	buffer1[20] = (flag3 & 0x08U) == 0x08U; buffer1[21] = (flag3 & 0x04U) == 0x04U;
	buffer1[22] = (flag3 & 0x02U) == 0x02U; buffer1[23] = (flag3 & 0x01U) == 0x01U;

	unsigned int n = 24U;

	stringToBits(header.getRptCall2(), buffer1 + n, LONG_CALLSIGN_LENGTH);
	n += LONG_CALLSIGN_LENGTH * 8U;

	stringToBits(header.getRptCall1(), buffer1 + n, LONG_CALLSIGN_LENGTH);
	n += LONG_CALLSIGN_LENGTH * 8U;

	stringToBits(header.getYourCall(), buffer1 + n, LONG_CALLSIGN_LENGTH);
	n += LONG_CALLSIGN_LENGTH * 8U;

	stringToBits(header.getMyCall1(),  buffer1 + n, LONG_CALLSIGN_LENGTH);
	n += LONG_CALLSIGN_LENGTH * 8U;

	stringToBits(header.getMyCall2(),  buffer1 + n, SHORT_CALLSIGN_LENGTH);
	n += SHORT_CALLSIGN_LENGTH * 8U;

	CCCITTChecksumReverse cksum;
	for (unsigned int i = 0U; i < n; i += 8U)
		cksum.update(buffer1 + i);
	cksum.result(buffer1 + n);

	// CUtils::dump(wxT("TX Raw Data"), buffer1, n + 16U);

	unsigned int length;
	CDStarFECEncoder fec;
	fec.encode(buffer1, buffer2, RADIO_HEADER_LENGTH_BITS, length);

	// CUtils::dump(wxT("TX After FEC"), buffer2, FEC_SECTION_LENGTH_BITS);

	CDStarInterleaver interleaver;
	interleaver.interleave(buffer2, m_header, FEC_SECTION_LENGTH_BITS);

	// CUtils::dump(wxT("TX After Interleaver"), m_header, FEC_SECTION_LENGTH_BITS);

	CDStarScrambler scrambler;
	scrambler.process(m_header, FEC_SECTION_LENGTH_BITS);

	// CUtils::dump(wxT("TX After Scrambler"), m_header, FEC_SECTION_LENGTH_BITS);

	delete[] buffer1;
	delete[] buffer2;
}

CRadioHeaderEncoder::~CRadioHeaderEncoder()
{
	delete[] m_header;
}

unsigned int CRadioHeaderEncoder::getRadioData(bool* data, unsigned int length)
{
	wxASSERT(data != NULL);
	wxASSERT(length == FEC_SECTION_LENGTH_BITS);

	::memcpy(data, m_header, FEC_SECTION_LENGTH_BITS * sizeof(bool));

	return FEC_SECTION_LENGTH_BITS;
}

void CRadioHeaderEncoder::stringToBits(const wxString &text, bool *bits, unsigned int length) const
{
	wxASSERT(bits != NULL);

	for (unsigned int i = 0U; i < length; i++) {
		unsigned char byte = text.GetChar(i);
		unsigned char mask = 0x80U;
		for (unsigned int j = 0U; j < 8U; j++, mask >>= 1)
			*bits++ = byte & mask ? true : false;
	}
}
