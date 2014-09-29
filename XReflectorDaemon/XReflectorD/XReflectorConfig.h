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

#ifndef	XReflectorConfig_H
#define	XReflectorConfig_H

#include <wx/wx.h>
#include <wx/config.h>
#include <wx/filename.h>

class CXReflectorConfig {
public:
#if defined(__WINDOWS__)
	CXReflectorConfig(wxConfigBase* config, const wxString& name);
#else
	CXReflectorConfig(const wxString& dir, const wxString& name);
#endif
	~CXReflectorConfig();

	void getReflector(wxString& callsign, wxString& address) const;
	void setReflector(const wxString& callsign, const wxString& address);

	void getDPlus(bool& enabled) const;
	void setDPlus(bool enabled);

	void getMiscellaneous(bool& enabled) const;
	void setMiscellaneous(bool enabled);

	void getPosition(int& x, int& y) const;
	void setPosition(int x, int y);

	bool write();

private:
#if defined(__WINDOWS__)
	wxConfigBase* m_config;
	wxString      m_name;
#else
	wxFileName    m_fileName;
#endif
	wxString      m_callsign;
	wxString      m_address;
	bool          m_dplusEnabled;
	bool          m_logEnabled;
	int           m_x;
	int           m_y;
};

#endif
