/*
 *   Copyright (C) 2002,2003,2009-2013 by Jonathan Naylor G4KLX
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

#include "TimeServerLogRedirect.h"
#include "TimeServerApp.h"

CTimeServerLogRedirect::CTimeServerLogRedirect() :
wxLog()
{
}

CTimeServerLogRedirect::~CTimeServerLogRedirect()
{
}

void CTimeServerLogRedirect::DoLog(wxLogLevel level, const wxChar* msg, time_t timestamp)
{
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

void CTimeServerLogRedirect::DoLogString(const wxChar* msg, time_t)
{
	wxASSERT(msg != NULL);

	::wxGetApp().showLog(msg);
}
