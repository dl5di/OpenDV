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

#ifndef	XReflectorUserLog_H
#define	XReflectorUserLog_H

#include "Defs.h"

#include <wx/wx.h>
#include <wx/ffile.h>

class CXReflectorUserLog {
public:
	CXReflectorUserLog(const wxString& directory, const wxString& name);
	~CXReflectorUserLog();

	bool open();

	void write(const wxString& user, const wxString& destination, const wxString& source, PROTOCOL protocol);

	void close();

private:
	wxString m_name;
	wxFFile  m_file;
};

#endif
