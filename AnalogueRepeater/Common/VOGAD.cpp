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

#include "VOGAD.h"

#include <cmath>

CVOGAD::CVOGAD(wxFloat32 maxGain, wxFloat32 maxLevel) :
m_maxGain(0.0F),
m_maxLevel(maxLevel),
m_maxAbsLevel(0.0F),
m_gain(1.0F)
{
	wxASSERT(maxGain >= 0.0F);
	wxASSERT(maxLevel > 0.0F);

	// dB to gain
	m_maxGain = ::pow(10.0F, maxGain / 20.0F);
}

CVOGAD::~CVOGAD()
{
}

void CVOGAD::reset()
{
	m_maxAbsLevel = 0.0F;
	m_gain = 1.0F;
}

void CVOGAD::process(wxFloat32* inOut, unsigned int length)
{
	wxASSERT(inOut != NULL);

	process(inOut, inOut, length);
}

void CVOGAD::process(const wxFloat32* in, wxFloat32* out, unsigned int length)
{
	wxASSERT(in != NULL);
	wxASSERT(out != NULL);

	if (length == 0U)
		return;

	// Find the maximum level in the whole buffer.
	for (unsigned int i = 0U; i < length; i++) {
		wxFloat32 absVal = ::fabs(in[i]);

		if (absVal > m_maxAbsLevel)
			m_maxAbsLevel = absVal;
	}

	// When scaled with the current gain, is it still too loud?
	m_gain = m_maxLevel / m_maxAbsLevel;
	if (m_gain > m_maxGain)
		m_gain = m_maxGain;

	// Scale with the current (maybe new) gain.
	for (unsigned int i = 0U; i < length; i++)
		out[i] = m_gain * in[i];
}

void CVOGAD::setMaxGain(wxFloat32 gain)
{
	wxASSERT(gain >= 0.0F);

	// dB to gain
	m_maxGain = ::pow(10.0F, gain / 20.0F);
}

wxFloat32 CVOGAD::getGain() const
{
	return m_gain;
}
