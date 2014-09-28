/*
 *   Copyright (C) 2009,2010 by Jonathan Naylor G4KLX
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "Bleeper.h"

#include <cmath>

CBleeper::CBleeper(unsigned int sampleRate, unsigned int hz, unsigned int ms, float amplitude) :
m_audio(NULL),
m_length(0U),
m_total(0U),
m_position(0U)
{
	wxASSERT(sampleRate > 0U);
	wxASSERT(hz > 0U);
	wxASSERT(ms > 0U);
	wxASSERT(amplitude > 0.0F);

	m_length = sampleRate / hz;
	m_total  = (sampleRate * ms) / 1000U;

	m_audio = new wxFloat32[m_length];

	for (unsigned int i = 0U; i < m_length; i++) {
		if (i < m_length / 2U)
			m_audio[i] = amplitude;
		else
			m_audio[i] = -amplitude;
	}
}

CBleeper::~CBleeper()
{
	delete[] m_audio;
}

unsigned int CBleeper::getAudio(wxFloat32* audio, unsigned int length)
{
	wxASSERT(audio != NULL);
	wxASSERT(length > 0U);

	unsigned int n = 0U;
	while (n < length && m_position < m_total) {
		unsigned int pos = m_position % m_length;

		audio[n] = m_audio[pos];

		m_position++;
		n++;
	}

	return n;
}

void CBleeper::reset()
{
	m_position = 0U;
}
