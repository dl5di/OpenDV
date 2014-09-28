/*
 *	Copyright (C) 2009,2012 by Jonathan Naylor, G4KLX
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

#include "RadioHeaderEncoder.h"

#include "CCITTChecksumReverse.h"
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
	buffer1[0] = header.isDataPacket();
	buffer1[1] = header.isRepeaterMode();
	buffer1[2] = header.isInterrupted();
	buffer1[3] = header.isControlSignal();
	buffer1[4] = header.isUrgent();

	switch (header.getRepeaterFlags()) {
		default:
			buffer1[5] = false;
			buffer1[6] = false;
			buffer1[7] = false;
			break;
		case RELAY_UNAVAILABLE:
			buffer1[5] = false;
			buffer1[6] = false;
			buffer1[7] = true;
			break;
		case NO_RESPONSE:
			buffer1[5] = false;
			buffer1[6] = true;
			buffer1[7] = false;
			break;
		case ACK_FLAG:
			buffer1[5] = false;
			buffer1[6] = true;
			buffer1[7] = true;
			break;
		case RESEND_REQUESTED:
			buffer1[5] = true;
			buffer1[6] = false;
			buffer1[7] = false;
			break;
		case AUTO_REPLY:
			buffer1[5] = false;
			buffer1[6] = false;
			buffer1[7] = true;
			break;
		case REPEATER_CONTROL:
			buffer1[5] = true;
			buffer1[6] = true;
			buffer1[7] = true;
			break;
	}

	// Flags 2 & 3 are empty

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
	wxASSERT(length > 0U);

	for (unsigned int i = 0U; i < length; i++) {
		unsigned char byte = text.GetChar(i);
		unsigned char mask = 0x80U;
		for (unsigned int j = 0U; j < 8U; j++, mask >>= 1)
			*bits++ = byte & mask ? true : false;
	}
}
