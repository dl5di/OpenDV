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

#include "Goertzel.h"

#include <cmath>

CGoertzel::CGoertzel(unsigned int sampleRate, wxFloat32 freq, unsigned int n, wxFloat32 threshold) :
m_freq(freq),
m_n(n),
m_threshold(threshold),
m_coeff(0.0F),
m_q1(0.0F),
m_q2(0.0F),
m_value(0.0F),
m_count(0U),
m_result(false)
{
	wxASSERT(threshold >= 0.0F);
	wxASSERT(sampleRate > 0U);
	wxASSERT(freq > 0.0F);
	wxASSERT(n > 0U);

	m_coeff = 2.0F * ::cos(2.0F * M_PI * freq / wxFloat32(sampleRate));
}

CGoertzel::~CGoertzel()
{
}

void CGoertzel::setThreshold(wxFloat32 threshold)
{
	wxASSERT(threshold >= 0.0F);

	m_threshold = threshold;
}

void CGoertzel::process(const wxFloat32 *data, unsigned int length)
{
	wxASSERT(data != NULL);

	for (unsigned int i = 0U; i < length; i++) {
		wxFloat32 q0 = m_coeff * m_q1 - m_q2 + data[i];
		m_q2 = m_q1;
		m_q1 = q0;

		m_count++;
		if (m_count == m_n) {
			wxFloat32 value = m_q1 * m_q1 + m_q2 * m_q2 - m_q1 * m_q2 * m_coeff;
			if (!m_result)
				m_value = value;
			if (m_result && value > m_value)
				m_value = value;

			// wxLogMessage(wxT("Tone detector: freq=%f Hz  threshold=%f  value=%f"), m_freq, m_threshold, value);

			m_result = true;
			m_count  = 0U;
			m_q1 = 0.0F;
			m_q2 = 0.0F;
		}
	}
}

wxFloat32 CGoertzel::getResult() const
{
	return m_value;
}

TRISTATE CGoertzel::getDetected()
{
	if (!m_result)
		return STATE_UNKNOWN;

	if (m_value >= m_threshold) {
		// wxLogMessage(wxT("Goertzel: result = %f  thresh = %f  TRUE"), m_value, m_threshold);
		m_result = false;
		return STATE_TRUE;
	} else {
		// wxLogMessage(wxT("Goertzel: result = %f  thresh = %f   FALSE"), m_value, m_threshold);
		m_result = false;
		return STATE_FALSE;
	}
}
