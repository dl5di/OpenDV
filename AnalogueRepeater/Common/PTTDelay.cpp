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

#include "PTTDelay.h"

CPTTDelay::CPTTDelay(unsigned int delay) :
m_delay(2U * delay),
m_in(delay),
m_out(0U),
m_buffer(NULL)
{
	wxASSERT(delay > 0U);

	m_buffer = new bool[m_delay];

	for (unsigned int i = 0U; i < m_delay; i++)
		m_buffer[i] = false;
}

CPTTDelay::~CPTTDelay()
{
	delete[] m_buffer;
}

bool CPTTDelay::delay(bool in)
{
	bool out = m_buffer[m_out];
	m_buffer[m_in] = in;

	m_in++;
	m_out++;

	if (m_in >= m_delay)
		m_in = 0U;
	if (m_out >= m_delay)
		m_out = 0U;

	return out;
}
