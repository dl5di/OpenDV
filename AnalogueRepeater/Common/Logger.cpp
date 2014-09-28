/*
 *   Copyright (C) 2002,2003,2009,2011,2012 by Jonathan Naylor G4KLX
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

#include "Logger.h"

CLogger::CLogger(const wxString& directory, const wxString& name) :
wxLog(),
m_name(name),
m_file(NULL),
m_fileName(),
m_day(0)
{
	m_file = new wxFFile;

	m_fileName.SetPath(directory);
	m_fileName.SetExt(wxT("log"));

	time_t timestamp;
	::time(&timestamp);
	struct tm* tm = ::gmtime(&timestamp);

	wxString text;
	text.Printf(wxT("%s-%04d-%02d-%02d"), m_name.c_str(), tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday);

	m_day = tm->tm_yday;
	m_fileName.SetName(text);

	bool ret = m_file->Open(m_fileName.GetFullPath(), wxT("a+t"));
	if (!ret) {
		wxLogError(wxT("Cannot open %s file for appending"), m_fileName.GetFullPath().c_str());
		return;
	}
}

CLogger::~CLogger()
{
	wxASSERT(m_file != NULL);

	m_file->Close();
	delete m_file;
}

void CLogger::DoLog(wxLogLevel level, const wxChar* msg, time_t timestamp)
{
	wxASSERT(m_file != NULL);
	wxASSERT(m_file->IsOpened());
	wxASSERT(msg != NULL);

	wxString letter;

	switch (level) {
		case wxLOG_FatalError: letter = wxT("F"); break;
		case wxLOG_Error:      letter = wxT("E"); break;
		case wxLOG_Warning:    letter = wxT("W"); break;
		case wxLOG_Info:       letter = wxT("I"); break;
		case wxLOG_Message:    letter = wxT("M"); break;
		case wxLOG_Status:     letter = wxT("M"); break;
		case wxLOG_Trace:      letter = wxT("T"); break;
		case wxLOG_Debug:      letter = wxT("D"); break;
		default:               letter = wxT("U"); break;
	}

	struct tm* tm = ::gmtime(&timestamp);

	wxString message;
	message.Printf(wxT("%s: %04d-%02d-%02d %02d:%02d:%02d: %s\n"), letter.c_str(), tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec, msg);

	DoLogString(message.c_str(), timestamp);

	if (level == wxLOG_FatalError)
		::abort();
}

void CLogger::DoLogString(const wxChar* msg, time_t timestamp)
{
	wxASSERT(m_file != NULL);
	wxASSERT(m_file->IsOpened());
	wxASSERT(msg != NULL);

	struct tm* tm = ::gmtime(&timestamp);

	int day = tm->tm_yday;
	if (day != m_day) {
		wxString text;
		text.Printf(wxT("%s-%04d-%02d-%02d"), m_name.c_str(), tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday);

		m_day = day;
		m_fileName.SetName(text);

		m_file->Close();

		bool ret = m_file->Open(m_fileName.GetFullPath(), wxT("a+t"));
		if (!ret) {
			wxLogError(wxT("Cannot open %s file for appending"), m_fileName.GetFullPath().c_str());
			return;
		}
	}

	m_file->Write(wxString(msg));
	m_file->Flush();
}
