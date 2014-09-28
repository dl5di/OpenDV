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

#ifndef	TimeServerApp_H
#define	TimeServerApp_H

#include "TimeServerThreadHelper.h"
#include "TimeServerConfig.h"
#include "TimeServerFrame.h"
#include "TimeServerDefs.h"

#include <wx/wx.h>

class CTimeServerApp : public wxApp {

public:
	CTimeServerApp();
	virtual ~CTimeServerApp();

	virtual bool OnInit();
	virtual int  OnExit();
	
	virtual void OnInitCmdLine(wxCmdLineParser& parser);
	virtual bool OnCmdLineParsed(wxCmdLineParser& parser);

	// This is overridden because dialog boxes from threads are bad news
#if defined(__WXDEBUG__)
	virtual void OnAssertFailure(const wxChar* file, int line, const wxChar* func, const wxChar* cond, const wxChar* msg);
#endif

	virtual void showLog(const wxString& text);

	virtual void getGateway(wxString& callsign, bool& sendA, bool& sendB, bool& sendC, bool& sendD, bool& sendE, wxString& address) const;
	virtual void setGateway(const wxString& callsign, bool sendA, bool sendB, bool sendC, bool sendD, bool sendE, const wxString& address);

	virtual void getAnnouncements(LANGUAGE& language, FORMAT& format, INTERVAL& interval) const;
	virtual void setAnnouncements(LANGUAGE language, FORMAT format, INTERVAL interval);

	virtual void getPosition(int& x, int& y) const;
	virtual void setPosition(int x, int y);

	virtual bool writeConfig();

private:
	wxString                 m_name;
	bool                     m_nolog;
	bool                     m_gui;
	wxString                 m_logDir;
	wxString                 m_confDir;
	CTimeServerFrame*        m_frame;
	CTimeServerThreadHelper* m_thread;
	CTimeServerConfig*       m_config;
	wxLogChain*              m_logChain;

	void createThread();
};

DECLARE_APP(CTimeServerApp)

#endif
