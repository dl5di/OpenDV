/*
 *   Copyright (C) 2010,2011,2012 by Jonathan Naylor G4KLX
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

#ifndef	DExtraGatewayApp_H
#define	DExtraGatewayApp_H

#include "DExtraGatewayThread.h"
#include "DExtraGatewayFrame.h"
#include "DExtraGatewayDefs.h"
#include "DExtraGatewayStatusData.h"
#include "DExtraGatewayConfig.h"

#include <wx/wx.h>

class CDExtraGatewayApp : public wxApp {

public:
	CDExtraGatewayApp();
	virtual ~CDExtraGatewayApp();

	virtual bool OnInit();
	virtual int  OnExit();
	
	virtual void OnInitCmdLine(wxCmdLineParser& parser);
	virtual bool OnCmdLineParsed(wxCmdLineParser& parser);

	// This is overridden because dialog boxes from threads are bad news
#if defined(__WXDEBUG__)
	virtual void OnAssertFailure(const wxChar* file, int line, const wxChar* func, const wxChar* cond, const wxChar* msg);
#endif

	virtual CDExtraGatewayStatusData* getStatus() const;

	virtual void getReflector(wxString& callsign, wxString& reflector, bool& atStartup, RECONNECT& reconnect, TEXT_LANG& language) const;
	virtual void setReflector(const wxString& callsign, const wxString& reflector, bool atStartup, RECONNECT reconnect, TEXT_LANG language);

	virtual void getRepeater(wxString& repeaterCallsign, wxString& repeaterAddress, unsigned int& repeaterPort, wxString& localAddress, unsigned int& localPort) const;
	virtual void setRepeater(const wxString& repeaterCallsign, const wxString& repeaterAddress, unsigned int repeaterPort, const wxString& localAddress, unsigned int localPort);

	virtual void getPosition(int& x, int& y) const;
	virtual void setPosition(int x, int y);

	virtual bool writeConfig();

private:
	wxString              m_name;
	bool                  m_nolog;
	bool                  m_gui;
	wxString              m_logDir;
	wxString              m_confDir;
	CDExtraGatewayFrame*  m_frame;
	CDExtraGatewayThread* m_thread;
	CDExtraGatewayConfig* m_config;

	void createThread();
};

DECLARE_APP(CDExtraGatewayApp)

#endif
