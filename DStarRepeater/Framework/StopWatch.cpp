/*
 *   Copyright (C) 2015 by Jonathan Naylor G4KLX
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

#include "StopWatch.h"

#if defined(WIN32)

CStopWatch::CStopWatch() :
m_frequency(),
m_start()
{
	::QueryPerformanceFrequency(&m_frequency);
}

CStopWatch::~CStopWatch()
{
}

void CStopWatch::start()
{
	::QueryPerformanceCounter(&m_start);
}

unsigned int CStopWatch::elapsed()
{
	LARGE_INTEGER now;
	::QueryPerformanceCounter(&now);

	LARGE_INTEGER temp;
	temp.QuadPart = (now.QuadPart - m_start.QuadPart) * 1000;

	return (unsigned int)(temp.QuadPart / m_frequency.QuadPart);
}

#else

CStopWatch::CStopWatch() :
m_start()
{
}

CStopWatch::~CStopWatch()
{
}

void CStopWatch::start()
{
	::clock_gettime(CLOCK_MONOTONIC, &m_start);
}

unsigned int CStopWatch::elapsed()
{
	timespec now;
	::clock_gettime(CLOCK_MONOTONIC, &now);

	if (m_start.tv_sec == now.tv_sec) {
		return (now.tv_nsec - m_start.tv_nsec) / 1000000U;
	} else {
		long temp = -m_start.tv_nsec / 1000000L;
		temp += (now.tv_sec - m_start.tn_sec) * 1000L;
		temp += m_start.tn_nsec / 1000000L;
		return temp;
	}
}

#endif
