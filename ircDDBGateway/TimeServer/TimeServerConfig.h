/*
 *   Copyright (C) 2012 by Jonathan Naylor G4KLX
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

#ifndef	TimeServerConfig_H
#define	TimeServerConfig_H

#include "TimeServerDefs.h"

#include <wx/wx.h>
#include <wx/config.h>
#include <wx/filename.h>

class CTimeServerConfig {
public:
#if defined(__WINDOWS__)
	CTimeServerConfig(wxConfigBase* config, const wxString& name);
#else
	CTimeServerConfig(const wxString& dir, const wxString& name);
#endif
	~CTimeServerConfig();

	void getGateway(wxString& callsign, bool& sendA, bool& sendB, bool& sendC, bool& sendD, bool& sendE, wxString& address) const;
	void setGateway(const wxString& callsign, bool sendA, bool sendB, bool sendC, bool sendD, bool sendE, const wxString& address);

	void getAnnouncements(LANGUAGE& language, FORMAT& format, INTERVAL& interval) const;
	void setAnnouncements(LANGUAGE language, FORMAT format, INTERVAL interval);

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
	bool          m_sendA;
	bool          m_sendB;
	bool          m_sendC;
	bool          m_sendD;
	bool          m_sendE;
	wxString      m_address;
	LANGUAGE      m_language;
	FORMAT        m_format;
	INTERVAL      m_interval;
	int           m_x;
	int           m_y;
};

#endif
