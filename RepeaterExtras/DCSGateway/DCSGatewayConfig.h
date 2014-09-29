/*
 *   Copyright (C) 2010,2012,2013 by Jonathan Naylor G4KLX
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

#ifndef	DCSGatewayConfig_H
#define	DCSGatewayConfig_H

#include "DCSGatewayDefs.h"
#include "DStarDefines.h"

#include <wx/wx.h>
#include <wx/config.h>
#include <wx/filename.h>

class CDCSGatewayConfig {
public:
#if defined(__WINDOWS__)
	CDCSGatewayConfig(wxConfigBase* config, const wxString& name);
#else
	CDCSGatewayConfig(const wxString& dir, const wxString& name);
#endif
	~CDCSGatewayConfig();

	void getReflector(wxString& callsign, wxString& reflector, bool& atStartup, RECONNECT& reconnect, TEXT_LANG& language) const;
	void setReflector(const wxString& callsign, const wxString& reflector, bool atStartup, RECONNECT reconnect, TEXT_LANG language);

	void getRepeater(wxString& repeaterCallsign, wxString& repeaterAddress, unsigned int& repeaterPort, wxString& localAddress, unsigned int& localPort) const;
	void setRepeater(const wxString& repeaterCallsign, const wxString& repeaterAddress, unsigned int repeaterPort, const wxString& localAddress, unsigned int localPort);

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
	wxString      m_reflector;
	bool          m_atStartup;
	RECONNECT     m_reconnect;
	TEXT_LANG     m_language;
	wxString      m_repeaterCallsign;
	wxString      m_repeaterAddress;
	unsigned int  m_repeaterPort;
	wxString      m_localAddress;
	unsigned int  m_localPort;
	int           m_x;
	int           m_y;
};

#endif
