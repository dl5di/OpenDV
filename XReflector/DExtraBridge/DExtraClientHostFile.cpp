/*
 *   Copyright (C) 2010 by Jonathan Naylor G4KLX
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

#include "DExtraClientHostFile.h"

#include <wx/textfile.h>
#include <wx/tokenzr.h>
#include <wx/file.h>

CDExtraClientHostFile::CDExtraClientHostFile(const wxString& fileName) :
m_names(),
m_hosts()
{
	if (!wxFile::Exists(fileName))
		return;

	wxTextFile file;

	bool ret = file.Open(fileName);
	if (!ret)
		return;

	wxLogMessage(wxT("Reading %s"), fileName.c_str());

	unsigned int nLines = file.GetLineCount();

	for (unsigned int i = 0; i < nLines; i++) {
		wxString line = file.GetLine(i);

		if (line.length() > 0 && line.GetChar(0) != wxT('#')) {
			wxStringTokenizer t(line);
			wxString name = t.GetNextToken();
			wxString host = t.GetNextToken();

			if (!name.IsEmpty() && !host.IsEmpty()) {
				// wxLogMessage(wxT("Name=%s Host=%s"), name.c_str(), host.c_str());
				m_names.Add(name);
				m_hosts.Add(host);
			}
		}
	}

	file.Close();
}

CDExtraClientHostFile::~CDExtraClientHostFile()
{
}

unsigned int CDExtraClientHostFile::getCount() const
{
	return m_hosts.GetCount();
}

wxString CDExtraClientHostFile::getName(unsigned int n) const
{
	return m_names.Item(n);
}

wxString CDExtraClientHostFile::getHost(unsigned int n) const
{
	return m_hosts.Item(n);
}
