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

#ifndef	PatternMatcher_H
#define	PatternMatcher_H

class CPatternMatcher {
public:
	CPatternMatcher(unsigned int length, const bool* pattern, unsigned int count = 1U);
	~CPatternMatcher();

	bool add(bool bit);
	void reset();

private:
	unsigned int m_patternLength;
	bool*        m_pattern;
	unsigned int m_dataLength;
	bool*        m_data;
};

#endif
