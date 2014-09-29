/*
 *  Copyright (C) 2001, 2002, 2003 by Tomi Manninen, OH2BNS
 *	Copyright (C) 2010 by Jonathan Naylor, G4KLX
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

#include "Correlator.h"

#include <cmath>

CCorrelator::CCorrelator(const wxFloat32* taps, unsigned int length) :
m_taps(NULL),
m_length(length),
m_aa(0.0F),
m_buffer(NULL),
m_bufLen(20U * m_length),
m_pointer(length),
m_bb(0.0F),
m_corr(0.0F)
{
	wxASSERT(taps != NULL);
	wxASSERT(length > 0U);

	m_taps   = new wxFloat32[m_length];
	m_buffer = new wxFloat32[m_bufLen];

	::memcpy(m_taps,   taps, m_length * sizeof(wxFloat32));
	::memset(m_buffer, 0x00, m_bufLen * sizeof(wxFloat32));

	for (unsigned int i = 0U; i < m_length; i++)
		m_aa += m_taps[i] * m_taps[i];
}

CCorrelator::~CCorrelator()
{
	delete[] m_taps;
	delete[] m_buffer;
}

void CCorrelator::process(wxFloat32 val)
{
	wxFloat32* ptr = m_buffer + m_pointer++;

	*ptr = val;

	wxFloat32* a = ptr - m_length;
	wxFloat32* b = m_taps;

	m_bb   = 0.0F;
	m_corr = 0.0F;
	for (unsigned int i = 0U; i < m_length; i++) {
		m_corr += a[i] * b[i];
		m_bb   += a[i] * a[i];
	}

	if (m_pointer == m_bufLen) {
		::memcpy(m_buffer, m_buffer + m_bufLen - m_length, m_length * sizeof(wxFloat32));
		m_pointer = m_length;
	}
}

wxFloat32 CCorrelator::getValue(bool normalised) const
{
	if (normalised) {
		wxFloat32 div = m_aa * m_bb;
		if (div == 0.0F)
			return 0.0F;

		return m_corr / ::sqrt(div);
	} else {
		return m_corr;
	}
}

void CCorrelator::reset()
{
	::memset(m_buffer, 0x00, m_bufLen * sizeof(wxFloat32));;
	m_pointer = m_length;
	m_bb      = 0.0F;
}

void CCorrelator::dump()
{
	wxString text;
	text.Printf(wxT("Pointer=%3u Buffer="), m_pointer);

	wxFloat32* p = m_buffer + m_pointer - m_length;

	for (unsigned i = 0U; i < m_length; i++, p++) {
		wxString d;
		d.Printf(wxT("%5.2f "), *p);
		text.Append(d);
	}

	wxLogMessage(text);
}
