/*
 *   Copyright (C) 2009,2012 by Jonathan Naylor G4KLX
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

#ifndef	ParrotControllerConfig_H
#define	ParrotControllerConfig_H

#include "ParrotControllerDefs.h"

#include <wx/wx.h>
#include <wx/config.h>
#include <wx/filename.h>

class CParrotControllerConfig {
public:
#if defined(__WINDOWS__)
	CParrotControllerConfig(wxConfigBase* config, const wxString& name);
#else
	CParrotControllerConfig(const wxString& dir, const wxString& name);
#endif
	~CParrotControllerConfig();

	void getMode(PARROT_MODE& mode, unsigned int& beaconTime, wxString& beaconFileName, unsigned int& turnaroundTime, bool& keepFile) const;
	void setMode(PARROT_MODE mode, unsigned int beaconTime, const wxString& beaconFileName, unsigned int turnaroundTime, bool keepFile);

	void getNetwork(wxString& repeaterAddress, unsigned int& repeaterPort, wxString& localAddress, unsigned int& localPort) const;
	void setNetwork(const wxString& repeaterAddress, unsigned int repeaterPort, const wxString& localAddress, unsigned int localPort);

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
	PARROT_MODE   m_mode;
	unsigned int  m_beaconTime;
	wxString      m_beaconFileName;
	unsigned int  m_turnaroundTime;
	bool          m_keepFile;
	wxString      m_repeaterAddress;
	unsigned int  m_repeaterPort;
	wxString      m_localAddress;
	unsigned int  m_localPort;
	int           m_x;
	int           m_y;
};

#endif
