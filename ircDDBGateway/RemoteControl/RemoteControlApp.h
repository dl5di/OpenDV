/*
 *   Copyright (C) 2011,2012,2013 by Jonathan Naylor G4KLX
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

#ifndef	RemoteControlApp_H
#define	RemoteControlApp_H

#include "RemoteControlConfig.h"
#include "RemoteControlFrame.h"

#include <wx/wx.h>

class CRemoteControlApp : public wxApp {

public:
	CRemoteControlApp();
	virtual ~CRemoteControlApp();

	virtual bool OnInit();
	virtual int  OnExit();
	
	virtual void OnInitCmdLine(wxCmdLineParser& parser);
	virtual bool OnCmdLineParsed(wxCmdLineParser& parser);

	virtual void getConfig(wxString& address, unsigned int& port, wxString& password) const;
	virtual void setConfig(const wxString& address, unsigned int port, const wxString& password) const;

	virtual void getPosition(int& x, int& y) const;
	virtual void setPosition(int x, int y);

	virtual void repeaterRefresh(const wxString& callsign);
	virtual void starNetRefresh(const wxString& callsign);
	virtual void link(const wxString& callsign, RECONNECT reconnect, const wxString& reflector);
	virtual void unlink(const wxString& callsign, PROTOCOL protocol, const wxString& reflector);
	virtual void starNetLogoff(const wxString& callsign, const wxString& user);

private:
	wxString              m_name;
	CRemoteControlFrame*  m_frame;
	CRemoteControlConfig* m_config;
};

DECLARE_APP(CRemoteControlApp)

#endif
