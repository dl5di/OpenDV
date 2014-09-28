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

#include "FirstOrderIIR.h"

CFirstOrderIIR::CFirstOrderIIR(wxFloat32 a1, wxFloat32 b0, wxFloat32 b1, wxFloat32 gain) :
m_a1(a1),
m_b0(b0),
m_b1(b1),
m_gain(gain),
m_x0(0.0F),
m_x1(0.0F),
m_y0(0.0F),
m_y1(0.0F)
{
	wxASSERT(gain > 0.0F);
}

CFirstOrderIIR::~CFirstOrderIIR()
{
}

wxFloat32 CFirstOrderIIR::process(wxFloat32 val)
{
	m_x0 = m_x1;
	m_x1 = val;
	m_y0 = m_y1;
	m_y1 = m_b0 * m_x1 + m_b1 * m_x0 - m_a1 * m_y0;

	return m_y1 * m_gain;
}

void CFirstOrderIIR::process(const wxFloat32* in, wxFloat32* out, unsigned int length)
{
	wxASSERT(out != NULL);
	wxASSERT(in != NULL);

	for (unsigned int i = 0U; i < length; i++)
		out[i] = process(in[i]);
}

void CFirstOrderIIR::process(wxFloat32* inOut, unsigned int length)
{
	wxASSERT(inOut != NULL);

	for (unsigned int i = 0U; i < length; i++)
		inOut[i] = process(inOut[i]);
}

void CFirstOrderIIR::reset()
{
	m_x0 = 0.0F;
	m_x1 = 0.0F;
	m_y0 = 0.0F;
	m_y1 = 0.0F;
}
