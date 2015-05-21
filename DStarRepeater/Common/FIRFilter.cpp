/*
 *  Copyright (C) 2001, 2002, 2003 by Tomi Manninen, OH2BNS
 *	Copyright (C) 2009,2015 by Jonathan Naylor, G4KLX
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

#include <cstdio>
#include <cassert>
#include <cstring>

CFIRFilter::CFIRFilter(const float* taps, unsigned int length) :
m_taps(NULL),
m_length(length),
m_buffer(NULL),
m_bufLen(20U * m_length),
m_pointer(length)
{
	assert(taps != NULL);
	assert(length > 0U);

	m_taps   = new float[m_length];
	m_buffer = new float[m_bufLen];

	::memcpy(m_taps,   taps, m_length * sizeof(float));
	::memset(m_buffer, 0x00, m_bufLen * sizeof(float));
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

void CFIRFilter::setTaps(const float* taps, unsigned int length)
{
	assert(taps != NULL);
	assert(length > 0U);

	delete[] m_taps;
	delete[] m_buffer;

	m_length  = length;
	m_pointer = length;
	m_bufLen  = 20U * m_length;

	m_taps   = new float[m_length];
	m_buffer = new float[m_bufLen];

	::memcpy(m_taps,   taps, m_length * sizeof(float));
	::memset(m_buffer, 0x00, m_bufLen * sizeof(float));
}

float CFIRFilter::process(float val)
{
	float* ptr = m_buffer + m_pointer++;

	*ptr = val;

	float* a = ptr - m_length;
	float* b = m_taps;

	float out = 0.0F;
	for (unsigned int i = 0U; i < m_length; i++)
		out += (*a++) * (*b++);

	if (m_pointer == m_bufLen) {
		::memcpy(m_buffer, m_buffer + m_bufLen - m_length, m_length * sizeof(float));
		m_pointer = m_length;
	}

	return out;
}

void CFIRFilter::process(float* inOut, unsigned int length)
{
	assert(inOut != NULL);

	for (unsigned int i = 0U; i < length; i++)
		inOut[i] = process(inOut[i]);
}

void CFIRFilter::process(const float* in, float* out, unsigned int length)
{
	assert(in != NULL);
	assert(out != NULL);

	for (unsigned int i = 0U; i < length; i++)
		out[i] = process(in[i]);
}

void CFIRFilter::reset()
{
	::memset(m_buffer, 0x00, m_bufLen * sizeof(float));;
}
