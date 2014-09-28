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

#include "DStarScrambler.h"

#include <wx/wx.h>

static const bool SCRAMBLER_TABLE_BITS[] = {
	false, false, false, false, true,  true,  true,  false, true,  true,  true,  true,  false, false, true,  false,
	true,  true,  false, false, true,  false, false, true,  false, false, false, false, false, false, true,  false,
	false, false, true,  false, false, true,  true,  false, false, false, true,  false, true,  true,  true,  false,
	true,  false, true,  true,  false, true,  true,  false, false, false, false, false, true,  true,  false, false,
	true,  true,  false, true,  false, true,  false, false, true,  true,  true,  false, false, true,  true,  true,
	true,  false, true,  true,  false, true,  false, false, false, false, true,  false, true,  false, true,  false,
	true,  true,  true,  true,  true,  false, true,  false, false, true,  false, true,  false, false, false, true, 
	true,  false, true,  true,  true,  false, false, false, true,  true,  true,  true,  true,  true,  true,  false,
	false, false, false, true,  true,  true,  false, true,  true,  true,  true,  false, false, true,  false, true, 
	true,  false, false, true,  false, false, true,  false, false, false, false, false, false, true,  false, false,
	false, true,  false, false, true,  true,  false, false, false, true,  false, true,  true,  true,  false, true,
	false, true,  true,  false, true,  true,  false, false, false, false, false, true,  true,  false, false, true,
	true,  false, true,  false, true,  false, false, true,  true,  true,  false, false, true,  true,  true,  true,
	false, true,  true,  false, true,  false, false, false, false, true,  false, true,  false, true,  false, true,
	true,  true,  true,  true,  false, true,  false, false, true,  false, true,  false, false, false, true,  true,
	false, true,  true,  true,  false, false, false, true,  true,  true,  true,  true,  true,  true,  false, false,
	false, false, true,  true,  true,  false, true,  true,  true,  true,  false, false, true,  false, true,  true,
	false, false, true,  false, false, true,  false, false, false, false, false, false, true,  false, false, false,
	true,  false, false, true,  true,  false, false, false, true,  false, true,  true,  true,  false, true,  false,
	true,  true,  false, true,  true,  false, false, false, false, false, true,  true,  false, false, true,  true,
	false, true,  false, true,  false, false, true,  true,  true,  false, false, true,  true,  true,  true,  false,
	true,  true,  false, true,  false, false, false, false, true,  false, true,  false, true,  false, true,  true,
	true,  true,  true,  false, true,  false, false, true,  false, true,  false, false, false, true,  true,  false,
	true,  true,  true,  false, false, false, true,  true,  true,  true,  true,  true,  true,  false, false, false,
	false, true,  true,  true,  false, true,  true,  true,  true,  false, false, true,  false, true,  true,  false,
	false, true,  false, false, true,  false, false, false, false, false, false, true,  false, false, false, true,
	false, false, true,  true,  false, false, false, true,  false, true,  true,  true,  false, true,  false, true,
	true,  false, true,  true,  false, false, false, false, false, true,  true,  false, false, true,  true,  false,
	true,  false, true,  false, false, true,  true,  true,  false, false, true,  true,  true,  true,  false, true,
	true,  false, true,  false, false, false, false, true,  false, true,  false, true,  false, true,  true,  true,
	true,  true,  false, true,  false, false, true,  false, true,  false, false, false, true,  true,  false, true,
	true,  true,  false, false, false, true,  true,  true,  true,  true,  true,  true,  false, false, false, false,
	true,  true,  true,  false, true,  true,  true,  true,  false, false, true,  false, true,  true,  false, false,
	true,  false, false, true,  false, false, false, false, false, false, true,  false, false, false, true,  false,
	false, true,  true,  false, false, false, true,  false, true,  true,  true,  false, true,  false, true,  true,
	false, true,  true,  false, false, false, false, false, true,  true,  false, false, true,  true,  false, true,
	false, true,  false, false, true,  true,  true,  false, false, true,  true,  true,  true,  false, true,  true,
	false, true,  false, false, false, false, true,  false, true,  false, true,  false, true,  true,  true,  true,
	true,  false, true,  false, false, true,  false, true,  false, false, false, true,  true,  false, true,  true,
	true,  false, false, false, true,  true,  true,  true,  true,  true,  true,  false, false, false, false, true,
	true,  true,  false, true,  true,  true,  true,  false, false, true,  false, true,  true,  false, false, true,
	false, false, true,  false, false, false, false, false, false, true,  false, false, false, true,  false, false,
	true,  true,  false, false, false, true,  false, true,  true,  true,  false, true,  false, true,  true,  false,
	true,  true,  false, false, false, false, false, true,  true,  false, false, true,  true,  false, true,  false,
	true,  false, false, true,  true,  true,  false, false, true,  true,  true,  true,  false, true,  true,  false};

static const unsigned int SCRAMBLER_TABLE_BITS_LENGTH = 720U;

static const unsigned char SCRAMBLER_TABLE_BYTES[] = {
	0x0e, 0xf2, 0xc9, 0x02, 0x26, 0x2e, 0xb6, 0x0c, 0xd4, 0xe7, 0xb4, 0x2a, 0xfa, 0x51, 0xb8, 0xfe,
	0x1d, 0xe5, 0x92, 0x04, 0x4c, 0x5d, 0x6c, 0x19, 0xa9, 0xcf, 0x68, 0x55, 0xf4, 0xa3, 0x71, 0xfc,
	0x3b, 0xcb, 0x24, 0x08, 0x98, 0xba, 0xd8, 0x33, 0x53, 0x9e, 0xd0, 0xab, 0xe9, 0x46, 0xe3, 0xf8,
	0x77, 0x96, 0x48, 0x11, 0x31, 0x75, 0xb0, 0x66, 0xa7, 0x3d, 0xa1, 0x57, 0xd2, 0x8d, 0xc7, 0xf0,
	0xef, 0x2c, 0x90, 0x22, 0x62, 0xeb, 0x60, 0xcd, 0x4e, 0x7b, 0x42, 0xaf, 0xa5, 0x1b, 0x8f, 0xe1,
	0xde, 0x59, 0x20, 0x44, 0xc5, 0xd6, 0xc1, 0x9a, 0x9c, 0xf6};

static const unsigned int SCRAMBLER_TABLE_BYTES_LENGTH = 90U;

CDStarScrambler::CDStarScrambler() :
m_count(0U)
{
}

CDStarScrambler::~CDStarScrambler()
{
}

void CDStarScrambler::process(bool* inOut, unsigned int length)
{
	wxASSERT(inOut != 0);

	for (unsigned int i = 0U; i < length; i++) {
		inOut[i] ^= SCRAMBLER_TABLE_BITS[m_count++];

		if (m_count >= SCRAMBLER_TABLE_BITS_LENGTH)
			m_count = 0U;
	}
}

void CDStarScrambler::process(const bool* in, bool* out, unsigned int length)
{
	wxASSERT(in != 0);
	wxASSERT(out != 0);

	for (unsigned int i = 0U; i < length; i++) {
		out[i] = in[i] ^ SCRAMBLER_TABLE_BITS[m_count++];

		if (m_count >= SCRAMBLER_TABLE_BITS_LENGTH)
			m_count = 0U;
	}
}

void CDStarScrambler::process(unsigned char* inOut, unsigned int length)
{
	wxASSERT(inOut != 0);

	for (unsigned int i = 0U; i < length; i++) {
		inOut[i] ^= SCRAMBLER_TABLE_BYTES[m_count++];

		if (m_count >= SCRAMBLER_TABLE_BYTES_LENGTH)
			m_count = 0U;
	}
}

void CDStarScrambler::process(const unsigned char* in, unsigned char* out, unsigned int length)
{
	wxASSERT(in != 0);
	wxASSERT(out != 0);

	for (unsigned int i = 0U; i < length; i++) {
		out[i] = in[i] ^ SCRAMBLER_TABLE_BYTES[m_count++];

		if (m_count >= SCRAMBLER_TABLE_BYTES_LENGTH)
			m_count = 0U;
	}
}

void CDStarScrambler::reset()
{
	m_count = 0U;
}
