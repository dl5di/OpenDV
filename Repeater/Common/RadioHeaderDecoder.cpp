/*
 *	Copyright (C) 2009,2011 by Jonathan Naylor, G4KLX
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
#include "RadioHeaderDecoder.h"
#include "RadioHeaderEncoder.h"
#include "DStarInterleaver.h"
#include "DStarFECDecoder.h"
#include "DStarScrambler.h"
#include "DStarDefines.h"

// #include "Utils.h"

#include <wx/wx.h>

CRadioHeaderDecoder::CRadioHeaderDecoder(const bool* data, unsigned int length) :
m_header(NULL),
m_headerBytes(NULL),
m_ber(0U)
{
	wxASSERT(data != NULL);
	wxASSERT(length == FEC_SECTION_LENGTH_BITS);

	bool* buffer1 = new bool[700U];
	bool* buffer2 = new bool[700U];

	CDStarScrambler scrambler;
	scrambler.process(data, buffer1, FEC_SECTION_LENGTH_BITS);

	CDStarInterleaver interleaver;
	interleaver.deinterleave(buffer1, buffer2, FEC_SECTION_LENGTH_BITS);

	CDStarFECDecoder fec;
	unsigned int outLen;
	bool res = fec.decode(buffer2, buffer1, FEC_SECTION_LENGTH_BITS, outLen);
	if (!res) {
		delete[] buffer1;
		delete[] buffer2;
		return;
	}

	// CUtils::dump(wxT("Header"), buffer1, RADIO_HEADER_LENGTH_BYTES * 8U);

	CCCITTChecksumReverse cksum;
	for (unsigned int i = 0U; i < 312U; i += 8U)
		cksum.update(buffer1 + i);

	res = cksum.check(buffer1 + 312U);
	if (!res) {
		delete[] buffer1;
		delete[] buffer2;
		return;
	}

	// If we're here then we have a valid header
	m_headerBytes = new unsigned char[RADIO_HEADER_LENGTH_BYTES];

	for (unsigned int i = 0U, n = 0U; i < RADIO_HEADER_LENGTH_BYTES; i++, n += 8U)
		m_headerBytes[i] = bitsToByte(buffer1 + n);

	m_header = new CHeaderData(m_headerBytes, RADIO_HEADER_LENGTH_BYTES, false);

	// Calculate the BER by regenerating the header
	CRadioHeaderEncoder encoder(*m_header);
	encoder.getRadioData(buffer1, FEC_SECTION_LENGTH_BITS);

	unsigned int errors = 0U;
	for (unsigned int i = 0U; i < FEC_SECTION_LENGTH_BITS; i++) {
		if (buffer1[i] != data[i])
			errors++;
	}

	m_ber = (errors * 100U) / FEC_SECTION_LENGTH_BITS;

	// wxLogMessage(wxT("Errors = %u/%u, %u%%"), errors, FEC_SECTION_LENGTH_BITS, m_ber);

	delete[] buffer1;
	delete[] buffer2;
}

CRadioHeaderDecoder::~CRadioHeaderDecoder()
{
	delete   m_header;
	delete[] m_headerBytes;
}

unsigned int CRadioHeaderDecoder::getHeaderBytes(unsigned char* buffer, unsigned int length)
{
	wxASSERT(buffer != NULL);
	wxASSERT(length >= RADIO_HEADER_LENGTH_BYTES);

	if (m_headerBytes == NULL)
		return 0U;

	::memcpy(buffer, m_headerBytes, RADIO_HEADER_LENGTH_BYTES * sizeof(unsigned char));

	return RADIO_HEADER_LENGTH_BYTES;
}

CHeaderData* CRadioHeaderDecoder::getHeaderData()
{
	if (m_header == NULL)
		return NULL;

	return new CHeaderData(*m_header);
}

unsigned int CRadioHeaderDecoder::getBER() const
{
	return m_ber;
}

unsigned char CRadioHeaderDecoder::bitsToByte(const bool *bits) const
{
	wxASSERT(bits != NULL);

	unsigned char val = 0x00;

	for (unsigned int i = 0U; i < 8U; i++) {
		val <<= 1;

		if (bits[i])
			val |= 0x01;
	}

	return val;
}
