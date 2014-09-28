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

#include "LimitedLengthBuffer.h"

#include <wx/wx.h>

CLimitedLengthBuffer::CLimitedLengthBuffer(unsigned int maxLength) :
m_buffer(NULL),
m_maxLength(maxLength),
m_currLength(0U),
m_allocLength(maxLength)
{
	wxASSERT(maxLength > 0U);

	m_buffer = new bool[maxLength];
}

CLimitedLengthBuffer::~CLimitedLengthBuffer()
{
	delete[] m_buffer;
}

void CLimitedLengthBuffer::clear()
{
	m_currLength = 0U;
}

void CLimitedLengthBuffer::setMaxLength(unsigned int maxLength)
{
	wxASSERT(maxLength > 0U);

	if (maxLength > m_allocLength) {
		delete[] m_buffer;
		m_buffer = new bool[maxLength];
		m_allocLength = maxLength;
	}

	m_maxLength  = maxLength;
	m_currLength = 0U;
}

void CLimitedLengthBuffer::add(bool bit)
{
	if (m_currLength < m_maxLength) {
		m_buffer[m_currLength] = bit;
		m_currLength++;
	} else {
		// Shuffle the data up
		for (unsigned int i = 1U; i < m_maxLength; i++)
			m_buffer[i - 1U] = m_buffer[i];
		m_buffer[m_maxLength - 1U] = bit;
	}
}

unsigned int CLimitedLengthBuffer::getLength() const
{
	return m_currLength;
}

unsigned int CLimitedLengthBuffer::getData(bool* buffer, unsigned int length) const
{
	wxASSERT(buffer != NULL);
	wxASSERT(length > 0U);

	unsigned int n = 0U;

	while (n < m_currLength && n < length) {
		buffer[n] = m_buffer[n];
		n++;
	}

	return n;
}
