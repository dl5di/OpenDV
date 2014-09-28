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

#include "LimitedPatternMatcher.h"

CLimitedPatternMatcher::CLimitedPatternMatcher(unsigned int length, const bool* pattern, unsigned int count, unsigned int limit) :
m_matcher(length, pattern, count),
m_limit(limit),
m_count(0U)
{
}

CLimitedPatternMatcher::~CLimitedPatternMatcher()
{
}

LPM_STATUS CLimitedPatternMatcher::add(bool bit)
{
	if (m_limit > 0U) {
		m_count++;

		if (m_count > m_limit)
			return LPM_FAILED;
	}

	bool res = m_matcher.add(bit);

	return res ? LPM_FOUND : LPM_SEARCHING;
}

void CLimitedPatternMatcher::resetData()
{
	m_matcher.reset();
}

void CLimitedPatternMatcher::resetCount()
{
	m_count = 0U;
}

void CLimitedPatternMatcher::resetAll()
{
	resetData();
	resetCount();
}
