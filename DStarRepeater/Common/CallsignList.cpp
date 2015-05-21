/*
 *   Copyright (C) 2011,2015 by Jonathan Naylor G4KLX
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

#include "CallsignList.h"
#include "DStarDefines.h"

#include <algorithm>

#include <cctype>

CCallsignList::CCallsignList(const std::string& filename) :
m_filename(filename),
m_callsigns()
{
}


CCallsignList::~CCallsignList()
{
}

bool CCallsignList::load()
{
	FILE* fp = ::fopen(m_filename.c_str(), "rt");
	if (fp == NULL)
		return false;

	char line[80U];
	while (::fgets(line, 80U, fp) != NULL) {
		for (unsigned int i = 0U; line[i] != '\0'; i++)
			line[i] = char(::toupper(line[i]));

		std::string callsign(line);
		callsign.resize(LONG_CALLSIGN_LENGTH, ' ');

		m_callsigns.push_back(callsign);
	}

	::fclose(fp);

	return true;
}

unsigned int CCallsignList::getCount() const
{
	return m_callsigns.size();
}

bool CCallsignList::isInList(const std::string& callsign) const
{
	return std::find(m_callsigns.begin(), m_callsigns.end(), callsign) != m_callsigns.end();
}
