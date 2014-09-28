/*
 *	Copyright (C) 2009,2012,2013 by Jonathan Naylor, G4KLX
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

#include "CWKeyer.h"

const struct SSymbols {
#if defined(__WINDOWS__)
	wxChar  c;
	wxChar* symbol;
#else
	const wxChar  c;
	const wxChar* symbol;
#endif
} cwTable[] = {
	{wxT('A'), wxT(".-")},
	{wxT('B'), wxT("-...")},
	{wxT('C'), wxT("-.-.")},
	{wxT('D'), wxT("-..")},
	{wxT('E'), wxT(".")},
	{wxT('F'), wxT("..-.")},
	{wxT('G'), wxT("--.")},
	{wxT('H'), wxT("....")},
	{wxT('I'), wxT("..")},
	{wxT('J'), wxT(".---")},
	{wxT('K'), wxT("-.-")},
	{wxT('L'), wxT(".-..")},
	{wxT('M'), wxT("--")},
	{wxT('N'), wxT("-.")},
	{wxT('O'), wxT("---")},
	{wxT('P'), wxT(".--.")},
	{wxT('Q'), wxT("--.-")},
	{wxT('R'), wxT(".-.")},
	{wxT('S'), wxT("...")},
	{wxT('T'), wxT("-")},
	{wxT('U'), wxT("..-")},
	{wxT('V'), wxT("...-")},
	{wxT('W'), wxT(".--")},
	{wxT('X'), wxT("-..-")},
	{wxT('Y'), wxT("-.--")},
	{wxT('Z'), wxT("--..")},
	{wxT('0'), wxT("-----")},
	{wxT('1'), wxT(".----")},
	{wxT('2'), wxT("..---")},
	{wxT('3'), wxT("...--")},
	{wxT('4'), wxT("....-")},
	{wxT('5'), wxT(".....")},
	{wxT('6'), wxT("-....")},
	{wxT('7'), wxT("--...")},
	{wxT('8'), wxT("---..")},
	{wxT('9'), wxT("----.")},
	{wxT('/'), wxT("-..-.")},
	{wxT('='), wxT("-...-")},
	{wxT(','), wxT("--..--")},
	{wxT('?'), wxT("..--..")},
	{wxT(' '), wxT("  ")},
	{wxT('$'), wxT("")}
};

const unsigned int RC_LEN = 5U;			// The length of the raised cosine in ms

CCWKeyer::CCWKeyer(const wxString& text, unsigned int speed, unsigned int freq, unsigned int sampleRate) :
m_data(NULL),
m_length(0U),
m_current(0U)
{
	wxASSERT(speed > 0U);
	wxASSERT(freq > 0U);
	wxASSERT(sampleRate > 0U);

	if (text.IsEmpty())
		return;

	wxString cw = wxT(" ");
	for (unsigned int i = 0U; i < text.Len(); i++) {
		cw += charToCW(text.GetChar(i));
		cw += wxT(" ");
	}
	cw += wxT("  ");

	for (unsigned int i = 0U; i < cw.Len(); i++) {
		wxChar c = cw.GetChar(i);

		switch (c) {
			case wxT(' '):
				m_length += 2U;
				break;
			case wxT('-'):
				m_length += 4U;
				break;
			case wxT('.'):
				m_length += 2U;
				break;
		}
	}

	wxFloat32 dotLenS = 1.2F / wxFloat32(speed);
	unsigned int dotLenSamples = (unsigned int)(wxFloat32(sampleRate) * dotLenS + 0.5F);

	unsigned int dotLen  = dotLenSamples * 1U;
	unsigned int dashLen = dotLenSamples * 3U;

	m_length *= dotLenSamples;

	m_data = new wxFloat32[m_length];

	unsigned int sqLen = (sampleRate / freq) / 2U;

	wxFloat32* dot  = new wxFloat32[dotLen];
	wxFloat32* dash = new wxFloat32[dashLen];

	bool high = false;
	for (unsigned int i = 0U; i < dashLen; i++) {
		if ((i % sqLen) == 0U)
			high = !high;

		dash[i] = high ? 1.0F : -1.0F;
		if (i < dotLen)
			dot[i] = high ? 1.0F : -1.0F;
	}

	// Calculate the length of the raised cosine shaping section
	unsigned int rcLen = RC_LEN * sampleRate / 1000U;

	// Shape the start and end of the tones
	for (unsigned int i = 0U; i < rcLen; i++) {
		wxFloat32 ampl = 0.5F * (1.0F - ::cos(M_PI * (wxFloat32(i) / wxFloat32(rcLen))));

		dot[i]  *= ampl;
		dash[i] *= ampl;

		dot[dotLen - 1U - i]   *= ampl;
		dash[dashLen - 1U - i] *= ampl;
	}

	// Clear the output buffer to ensure silence between the symbols
	::memset(m_data, 0x00, m_length * sizeof(wxFloat32));

	unsigned int pos = 0U;
	for (unsigned int i = 0U; i < cw.Len(); i++) {
		wxChar c = cw.GetChar(i);

		switch (c) {
			case wxT(' '):
				pos += dotLenSamples * 2U;
				break;
			case wxT('.'):
				::memcpy(m_data + pos, dot, dotLen * sizeof(wxFloat32));
				pos += dotLenSamples * 2U;
				break;
			case wxT('-'):
				::memcpy(m_data + pos, dash, dashLen * sizeof(wxFloat32));
				pos += dotLenSamples * 4U;
				break;
		}
	}

	delete[] dot;
	delete[] dash;
}

CCWKeyer::~CCWKeyer()
{
	delete[] m_data;
}

bool CCWKeyer::isEmpty() const
{
	return m_length == 0U;
}

unsigned int CCWKeyer::getAudio(wxFloat32* audio, unsigned int length, wxFloat32 amplitude)
{
	wxASSERT(audio != NULL);

	if (m_length == 0U)
		return 0U;

	unsigned int n = 0U;
	while (n < length && m_current < m_length)
		audio[n++] += m_data[m_current++] * amplitude;

	return n;
}

void CCWKeyer::reset()
{
	m_current = 0U;
}

wxString CCWKeyer::charToCW(wxChar c) const
{
	for (const SSymbols* s = cwTable; s->c != wxT('$'); s++) {
		if (s->c == c)
			return s->symbol;
	}

	return wxEmptyString;
}
