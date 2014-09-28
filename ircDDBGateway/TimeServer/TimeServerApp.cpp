/*
 *   Copyright (C) 2012,2014 by Jonathan Naylor G4KLX
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

#include "TimeServerLogRedirect.h"
#include "TimeServerThread.h"
#include "TimeServerApp.h"
#include "Version.h"
#include "Logger.h"

#include <wx/config.h>
#include <wx/cmdline.h>
#include <wx/filename.h>

IMPLEMENT_APP(CTimeServerApp)

const wxChar*       NAME_PARAM = wxT("Name");
const wxChar* NOLOGGING_SWITCH = wxT("nolog");
const wxChar*       GUI_SWITCH = wxT("gui");
const wxChar*    LOGDIR_OPTION = wxT("logdir");
const wxChar*   CONFDIR_OPTION = wxT("confdir");

CTimeServerApp::CTimeServerApp() :
wxApp(),
m_name(),
m_nolog(false),
m_gui(false),
m_logDir(),
m_confDir(),
m_frame(NULL),
m_thread(NULL),
m_config(NULL),
m_logChain(NULL)
{
}

CTimeServerApp::~CTimeServerApp()
{
}

bool CTimeServerApp::OnInit()
{
	SetVendorName(VENDOR_NAME);

	if (!wxApp::OnInit())
		return false;

	if (!m_nolog) {
		wxString logBaseName = LOG_BASE_NAME;
		if (!m_name.IsEmpty()) {
			logBaseName.Append(wxT("_"));
			logBaseName.Append(m_name);
		}

#if defined(__WINDOWS__)
		if (m_logDir.IsEmpty())
			m_logDir = wxFileName::GetHomeDir();
#else
		if (m_logDir.IsEmpty())
			m_logDir = wxT(LOG_DIR);
#endif

		wxLog* log = new CLogger(m_logDir, logBaseName);
		wxLog::SetActiveTarget(log);
	} else {
		new wxLogNull;
	}

	m_logChain = new wxLogChain(new CTimeServerLogRedirect);

#if defined(__WINDOWS__)
	m_config = new CTimeServerConfig(new wxConfig(APPLICATION_NAME), m_name);
#else
	if (m_confDir.IsEmpty())
		m_confDir = wxT(CONF_DIR);

	m_config = new CTimeServerConfig(m_confDir, m_name);
#endif

	wxString frameName = APPLICATION_NAME + wxT(" - ");
	if (!m_name.IsEmpty()) {
		frameName.Append(m_name);
		frameName.Append(wxT(" - "));
	}
	frameName.Append(VERSION);

	wxPoint position = wxDefaultPosition;

	int x, y;
	getPosition(x, y);
	if (x >= 0 && y >= 0)
		position = wxPoint(x, y);

	m_frame = new CTimeServerFrame(frameName, position, m_gui);
	m_frame->Show();

	SetTopWindow(m_frame);

	wxLogInfo(wxT("Starting ") + APPLICATION_NAME + wxT(" - ") + VERSION);

	// Log the SVN revsion and the version of wxWidgets and the Operating System
	wxLogInfo(SVNREV);
	wxLogInfo(wxT("Using wxWidgets %d.%d.%d on %s"), wxMAJOR_VERSION, wxMINOR_VERSION, wxRELEASE_NUMBER, ::wxGetOsDescription().c_str());

	createThread();

	return true;
}

int CTimeServerApp::OnExit()
{
	m_logChain->SetLog(NULL);

	wxLogInfo(APPLICATION_NAME + wxT(" is exiting"));

	m_thread->kill();

	delete m_config;

	return 0;
}

void CTimeServerApp::OnInitCmdLine(wxCmdLineParser& parser)
{
	parser.AddSwitch(NOLOGGING_SWITCH, wxEmptyString, wxEmptyString, wxCMD_LINE_PARAM_OPTIONAL);
	parser.AddSwitch(GUI_SWITCH,       wxEmptyString, wxEmptyString, wxCMD_LINE_PARAM_OPTIONAL);
	parser.AddOption(LOGDIR_OPTION,    wxEmptyString, wxEmptyString, wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL);
	parser.AddOption(CONFDIR_OPTION,   wxEmptyString, wxEmptyString, wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL);
	parser.AddParam(NAME_PARAM, wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL);

	wxApp::OnInitCmdLine(parser);
}

bool CTimeServerApp::OnCmdLineParsed(wxCmdLineParser& parser)
{
	if (!wxApp::OnCmdLineParsed(parser))
		return false;

	m_nolog = parser.Found(NOLOGGING_SWITCH);
	m_gui   = parser.Found(GUI_SWITCH);

	wxString logDir;
	bool found = parser.Found(LOGDIR_OPTION, &logDir);
	if (found)
		m_logDir = logDir;

	wxString confDir;
	found = parser.Found(CONFDIR_OPTION, &confDir);
	if (found)
		m_confDir = confDir;

	if (parser.GetParamCount() > 0U)
		m_name = parser.GetParam(0U);

	return true;
}

#if defined(__WXDEBUG__)
void CTimeServerApp::OnAssertFailure(const wxChar* file, int line, const wxChar* func, const wxChar* cond, const wxChar* msg)
{
	wxLogFatalError(wxT("Assertion failed on line %d in file %s and function %s: %s %s"), line, file, func, cond, msg);
}
#endif

void CTimeServerApp::showLog(const wxString& text)
{
	m_frame->showLog(text);
}

void CTimeServerApp::getGateway(wxString& callsign, bool& sendA, bool& sendB, bool& sendC, bool& sendD, bool& sendE, wxString& address) const
{
	m_config->getGateway(callsign, sendA, sendB, sendC, sendD, sendE, address);
}

void CTimeServerApp::setGateway(const wxString& callsign, bool sendA, bool sendB, bool sendC, bool sendD, bool sendE, const wxString& address)
{
	m_config->setGateway(callsign, sendA, sendB, sendC, sendD, sendE, address);
}

void CTimeServerApp::getAnnouncements(LANGUAGE& language, FORMAT& format, INTERVAL& interval) const
{
	m_config->getAnnouncements(language, format, interval);
}

void CTimeServerApp::setAnnouncements(LANGUAGE language, FORMAT format, INTERVAL interval)
{
	m_config->setAnnouncements(language, format, interval);
}

void CTimeServerApp::getPosition(int& x, int& y) const
{
	m_config->getPosition(x, y);
}

void CTimeServerApp::setPosition(int x, int y)
{
	m_config->setPosition(x, y);
}

bool CTimeServerApp::writeConfig()
{
	return m_config->write();
}

void CTimeServerApp::createThread()
{
	CTimeServerThread* thread = new CTimeServerThread;

	wxString callsign, address;
	bool sendA, sendB, sendC, sendD, sendE;
	getGateway(callsign, sendA, sendB, sendC, sendD, sendE, address);
	callsign.MakeUpper();
	thread->setGateway(callsign, sendA, sendB, sendC, sendD, sendE, address);
	wxLogInfo(wxT("Callsign set to %s, module %s%s%s%s, address: %s"), callsign.c_str(), sendA ? wxT("A") : wxT(""), sendB ? wxT("B") : wxT(""), sendC ? wxT("C") : wxT(""), sendD ? wxT("D") : wxT(""), sendE ? wxT("E") : wxT(""), address.c_str());

	LANGUAGE language;
	FORMAT format;
	INTERVAL interval;
	getAnnouncements(language, format, interval);
	thread->setAnnouncements(language, format, interval);
	wxLogInfo(wxT("Language: %d, format: %d, interval: %d"), int(language), int(format), int(interval));

	// Convert the worker class into a thread
	m_thread = new CTimeServerThreadHelper(thread);
	m_thread->start();
}
