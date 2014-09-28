/*
 *   Copyright (C) 2009 by Jonathan Naylor G4KLX
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

#include "NCO.h"

#include <cmath>

CNCO::CNCO(unsigned int sampleRate, wxFloat32 freq) :
m_audio(NULL),
m_length(0U),
m_position(0U)
{
	wxASSERT(sampleRate > 0U);
	wxASSERT(freq > 0.0F);

	// Fill a buffer with 10 seconds of audio
	m_length = sampleRate * 10U;
	m_audio  = new wxFloat32[m_length];

	wxFloat32 incr = 2.0F * M_PI * freq / wxFloat32(sampleRate);
	for (unsigned int i = 0U; i < m_length; i++) {
		wxFloat32 phase = incr * wxFloat32(i);
		m_audio[i] = ::sin(phase);
	}
}

CNCO::~CNCO()
{
	delete[] m_audio;
}

void CNCO::getAudio(wxFloat32* audio, unsigned int length, wxFloat32 amplitude)
{
	wxASSERT(audio != NULL);

	for (unsigned int i = 0U; i < length; i++) {
		audio[i] += m_audio[m_position++] * amplitude;

		if (m_position == m_length)
			m_position = 0U;
	}
}
