/*
 *  Copyright (C) 2001, 2002, 2003 by Tomi Manninen, OH2BNS
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

#include "FIRFilter.h"

CFIRFilter::CFIRFilter(const wxFloat32* taps, unsigned int length) :
m_taps(NULL),
m_length(length),
m_buffer(NULL),
m_bufLen(20U * m_length),
m_pointer(length)
{
	wxASSERT(taps != NULL);
	wxASSERT(length > 0U);

	m_taps   = new wxFloat32[m_length];
	m_buffer = new wxFloat32[m_bufLen];

	::memcpy(m_taps,   taps, m_length * sizeof(wxFloat32));
	::memset(m_buffer, 0x00, m_bufLen * sizeof(wxFloat32));
}

CFIRFilter::CFIRFilter() :
m_taps(NULL),
m_length(0U),
m_buffer(NULL),
m_bufLen(0U),
m_pointer(0U)
{
}

CFIRFilter::~CFIRFilter()
{
	delete[] m_taps;
	delete[] m_buffer;
}

void CFIRFilter::setTaps(const wxFloat32* taps, unsigned int length)
{
	wxASSERT(taps != NULL);
	wxASSERT(length > 0U);

	delete[] m_taps;
	delete[] m_buffer;

	m_length  = length;
	m_pointer = length;
	m_bufLen  = 20U * m_length;

	m_taps   = new wxFloat32[m_length];
	m_buffer = new wxFloat32[m_bufLen];

	::memcpy(m_taps,   taps, m_length * sizeof(wxFloat32));
	::memset(m_buffer, 0x00, m_bufLen * sizeof(wxFloat32));
}

wxFloat32 CFIRFilter::process(wxFloat32 val)
{
	wxFloat32* ptr = m_buffer + m_pointer++;

	*ptr = val;

	wxFloat32* a = ptr - m_length;
	wxFloat32* b = m_taps;

	wxFloat32 out = 0.0F;
	for (unsigned int i = 0U; i < m_length; i++)
		out += (*a++) * (*b++);

	if (m_pointer == m_bufLen) {
		::memcpy(m_buffer, m_buffer + m_bufLen - m_length, m_length * sizeof(wxFloat32));
		m_pointer = m_length;
	}

	return out;
}

void CFIRFilter::process(wxFloat32* inOut, unsigned int length)
{
	wxASSERT(inOut != NULL);

	for (unsigned int i = 0U; i < length; i++)
		inOut[i] = process(inOut[i]);
}

void CFIRFilter::process(const wxFloat32* in, wxFloat32* out, unsigned int length)
{
	wxASSERT(in != NULL);
	wxASSERT(out != NULL);

	for (unsigned int i = 0U; i < length; i++)
		out[i] = process(in[i]);
}

void CFIRFilter::reset()
{
	::memset(m_buffer, 0x00, m_bufLen * sizeof(wxFloat32));;
}
