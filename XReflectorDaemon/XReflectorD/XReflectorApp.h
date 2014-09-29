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

#ifndef	XReflectorApp_H
#define	XReflectorApp_H

#include "XReflectorThreadHelper.h"
#include "XReflectorConfig.h"
#include "XReflectorFrame.h"
#include "XReflectorDefs.h"

#include <wx/wx.h>

class CXReflectorApp : public wxApp {

public:
	CXReflectorApp();
	virtual ~CXReflectorApp();

	virtual bool OnInit();
	virtual int  OnExit();
	
	virtual void OnInitCmdLine(wxCmdLineParser& parser);
	virtual bool OnCmdLineParsed(wxCmdLineParser& parser);

	// This is overridden because dialog boxes from threads are bad news
#if defined(__WXDEBUG__)
	virtual void OnAssertFailure(const wxChar* file, int line, const wxChar* func, const wxChar* cond, const wxChar* msg);
#endif

	virtual void showLog(const wxString& text);

	virtual void getReflector(wxString& callsign, wxString& address) const;
	virtual void setReflector(const wxString& callsign, const wxString& address);

	virtual void getDPlus(bool& enabled) const;
	virtual void setDPlus(bool enabled);

	virtual void getMiscellaneous(bool& enabled) const;
	virtual void setMiscellaneous(bool enabled);

	virtual void getPosition(int& x, int& y) const;
	virtual void setPosition(int x, int y);

	virtual bool writeConfig();

private:
	wxString                 m_name;
	bool                     m_nolog;
	bool                     m_gui;
	wxString                 m_logDir;
	wxString                 m_confDir;
	CXReflectorFrame*        m_frame;
	CXReflectorThreadHelper* m_thread;
	CXReflectorConfig*       m_config;
	wxLogChain*              m_logChain;

	void createThread();
};

DECLARE_APP(CXReflectorApp)

#endif
