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

#ifndef	DStarRepeaterApp_H
#define	DStarRepeaterApp_H

#include "DStarRepeaterThreadHelper.h"
#include "DStarRepeaterStatusData.h"
#include "DStarRepeaterConfig.h"
#include "DStarRepeaterFrame.h"
#include "DStarRepeaterDefs.h"

#include <wx/wx.h>
#include <wx/snglinst.h>

class CDStarRepeaterApp : public wxApp {

public:
	CDStarRepeaterApp();
	virtual ~CDStarRepeaterApp();

	virtual bool OnInit();
	virtual int  OnExit();
	
	virtual void OnInitCmdLine(wxCmdLineParser& parser);
	virtual bool OnCmdLineParsed(wxCmdLineParser& parser);

	// This is overridden because dialog boxes from threads are bad news
#if defined(__WXDEBUG__)
	virtual void OnAssertFailure(const wxChar* file, int line, const wxChar* func, const wxChar* cond, const wxChar* msg);
#endif

	virtual CDStarRepeaterStatusData* getStatus() const;

	virtual void showLog(const wxString& text);

	virtual void setOutputs(bool out1, bool out2, bool out3, bool out4);

	virtual void setLogging(bool logging);

	virtual void setPosition(int x, int y);

	virtual void shutdown();
	virtual void startup();
	virtual void command1();
	virtual void command2();
	virtual void command3();
	virtual void command4();
	virtual void command5();
	virtual void command6();

private:
	wxString                    m_name;
	bool                        m_nolog;
	bool                        m_gui;
	wxString                    m_logDir;
	wxString                    m_confDir;
	wxString                    m_audioDir;
	CDStarRepeaterFrame*        m_frame;
	CDStarRepeaterThreadHelper* m_thread;
	CDStarRepeaterConfig*       m_config;
	wxSingleInstanceChecker*    m_checker;
	wxLogChain*                 m_logChain;

	void createThread();
};

DECLARE_APP(CDStarRepeaterApp)

#endif
