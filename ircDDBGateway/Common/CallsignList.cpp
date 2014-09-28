/*
 *   Copyright (C) 2011 by Jonathan Naylor G4KLX
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

#include <wx/textfile.h>

CCallsignList::CCallsignList(const wxString& filename) :
m_filename(filename),
m_callsigns()
{
}


CCallsignList::~CCallsignList()
{
}

bool CCallsignList::load()
{
	wxTextFile file;

	bool res = file.Open(m_filename);
	if (!res)
		return false;

	unsigned int lines = file.GetLineCount();
	if (lines == 0U) {
		file.Close();
		return true;
	}

	m_callsigns.Alloc(lines);

	wxString callsign = file.GetFirstLine();

	while (!file.Eof()) {
		callsign.MakeUpper();
		callsign.Append(wxT("        "));
		callsign.Truncate(LONG_CALLSIGN_LENGTH);

		m_callsigns.Add(callsign);

		callsign = file.GetNextLine();
	}

	file.Close();

	return true;
}

unsigned int CCallsignList::getCount() const
{
	return m_callsigns.GetCount();
}

bool CCallsignList::isInList(const wxString& callsign) const
{
	return m_callsigns.Index(callsign) != wxNOT_FOUND;
}
