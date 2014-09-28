/*
 *	Copyright (C) 2009,2010 by Jonathan Naylor, G4KLX
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

#ifndef	LimitedPatternMatcher_H
#define	LimitedPatternMatcher_H

#include "PatternMatcher.h"

enum LPM_STATUS {
	LPM_FOUND,
	LPM_SEARCHING,
	LPM_FAILED
};

class CLimitedPatternMatcher {
public:
	CLimitedPatternMatcher(unsigned int length, const bool* pattern, unsigned int count = 1U, unsigned int limit = 0U);
	~CLimitedPatternMatcher();

	LPM_STATUS add(bool bit);

	void resetData();
	void resetCount();
	void resetAll();

private:
	CPatternMatcher m_matcher;
	unsigned int    m_limit;
	unsigned int    m_count;
};

#endif
