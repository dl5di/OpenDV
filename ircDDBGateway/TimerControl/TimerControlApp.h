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

#ifndef	TimerControlApp_H
#define	TimerControlApp_H

#include "TimerControlThreadHelper.h"
#include "TimerControlConfig.h"
#include "TimerControlFrame.h"
#include "TimerControlItem.h"

#include <wx/wx.h>

class CTimerControlApp : public wxApp {

public:
	CTimerControlApp();
	virtual ~CTimerControlApp();

	virtual bool OnInit();
	virtual int  OnExit();
	
	virtual void OnInitCmdLine(wxCmdLineParser& parser);
	virtual bool OnCmdLineParsed(wxCmdLineParser& parser);

	// This is overridden because dialog boxes from threads are bad news
#if defined(__WXDEBUG__)
	virtual void OnAssertFailure(const wxChar* file, int line, const wxChar* func, const wxChar* cond, const wxChar* msg);
#endif

	virtual void getGateway(wxString& address, unsigned int& port, wxString& password) const;
	virtual void setGateway(const wxString& address, unsigned int port, const wxString& password);

	virtual void getDelay(bool& enabled) const;
	virtual void setDelay(bool enabled);

	virtual void getPosition(int& x, int& y) const;
	virtual void setPosition(int x, int y);

	virtual void writeConfig();

	virtual void writeItems();

private:
	wxString                   m_name;
	wxString                   m_fileName;
	bool                       m_nolog;
	wxString                   m_logDir;
	wxString                   m_confDir;
	CTimerControlFrame*        m_frame;
	CTimerControlConfig*       m_config;
	CTimerControlThreadHelper* m_thread;

	void createThread();
};

DECLARE_APP(CTimerControlApp)

#endif
