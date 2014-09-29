/*
 *   Copyright (C) 2011,2012 by Jonathan Naylor G4KLX
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

#include "XReflectorUserLog.h"

#include <wx/filename.h>

CXReflectorUserLog::CXReflectorUserLog(const wxString& directory, const wxString& name) :
m_name(),
m_file()
{
	wxString fullName = USERS_BASE_NAME;
	if (!name.IsEmpty()) {
		fullName.Append(wxT("_"));
		fullName.Append(name);
	}

	wxFileName fileName(directory, fullName, wxT("log"));
	m_name = fileName.GetFullPath();
}

CXReflectorUserLog::~CXReflectorUserLog()
{
}

bool CXReflectorUserLog::open()
{
	bool ret = m_file.Open(m_name, wxT("a+t"));
	if (!ret) {
		wxLogError(wxT("Cannot open %s file for appending"), m_name.c_str());
		return false;
	}

	return true;
}

void CXReflectorUserLog::write(const wxString& user, const wxString& destination, const wxString& source, PROTOCOL protocol)
{
	time_t timeNow = ::time(NULL);
	struct tm* tm = ::gmtime(&timeNow);

	wxString proto;
	switch (protocol) {
		case PROTO_DEXTRA:
			proto = wxT("DExtra");
			break;
		case PROTO_DPLUS:
			proto = wxT("DPlus");
			break;
		default:
			proto = wxT("????");
			break;
	}

	wxString text;
	text.Printf(wxT("%04d-%02d-%02d %02d:%02d:%02d: User: %-8.8s Destination: %-8.8s Source: %-8.8s Proto: %-6.6s\n"),
		tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec,
		user.c_str(), destination.c_str(), source.c_str(), proto.c_str());

	m_file.Write(text);
	m_file.Flush();
}

void CXReflectorUserLog::close()
{
	m_file.Close();
}
