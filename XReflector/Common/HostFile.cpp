/*
 *   Copyright (C) 2010,2011,2012 by Jonathan Naylor G4KLX
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

#include "DStarDefines.h"
#include "HostFile.h"

#include <wx/textfile.h>
#include <wx/tokenzr.h>
#include <wx/file.h>

CHostFile::CHostFile(const wxString& fileName, bool logging) :
m_names(),
m_addresses(),
m_locks()
{
	if (!wxFile::Exists(fileName))
		return;

	wxTextFile file;

	bool ret = file.Open(fileName);
	if (!ret)
		return;

	if (logging)
		wxLogMessage(wxT("Reading %s"), fileName.c_str());

	unsigned int nLines = file.GetLineCount();

	for (unsigned int i = 0; i < nLines; i++) {
		wxString line = file.GetLine(i);

		if (line.length() > 0 && line.GetChar(0) != wxT('#')) {
			wxStringTokenizer t(line, wxT(" \t\r\n"), wxTOKEN_STRTOK);
			wxString name    = t.GetNextToken();
			wxString address = t.GetNextToken();
			wxString lock    = t.GetNextToken();

			name.Append(wxT("        "));
			name.Truncate(LONG_CALLSIGN_LENGTH);

			if (!name.IsEmpty() && !address.IsEmpty()) {
				m_names.Add(name);
				m_addresses.Add(address);
				m_locks.Add(lock.IsEmpty() ? 0 : 1);
			}
		}
	}

	file.Close();
}

CHostFile::~CHostFile()
{
}

unsigned int CHostFile::getCount() const
{
	return m_names.GetCount();
}

wxString CHostFile::getName(unsigned int n) const
{
	return m_names.Item(n);
}

wxString CHostFile::getAddress(unsigned int n) const
{
	return m_addresses.Item(n);
}

bool CHostFile::getLock(unsigned int n) const
{
	return m_locks.Item(n) == 1;
}

wxArrayString CHostFile::getNames() const
{
	return m_names;
}
