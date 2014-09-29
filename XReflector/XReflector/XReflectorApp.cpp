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

#include "XReflectorReflectorHandler.h"
#include "XReflectorLogRedirect.h"
#include "XReflectorThread.h"
#include "XReflectorApp.h"
#include "DStarDefines.h"
#include "Version.h"
#include "Logger.h"

#include <wx/cmdline.h>
#include <wx/filename.h>

IMPLEMENT_APP(CXReflectorApp)

const wxChar*       NAME_PARAM = wxT("Reflector Name");
const wxChar* NOLOGGING_SWITCH = wxT("nolog");
const wxChar*       GUI_SWITCH = wxT("gui");
const wxChar*    LOGDIR_OPTION = wxT("logdir");
const wxChar*   CONFDIR_OPTION = wxT("confdir");


CXReflectorApp::CXReflectorApp() :
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

CXReflectorApp::~CXReflectorApp()
{
}

bool CXReflectorApp::OnInit()
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
			m_logDir = LOG_DIR;
#endif

		wxLog* log = new CLogger(m_logDir, logBaseName);
		wxLog::SetActiveTarget(log);
	} else {
		new wxLogNull;
	}

	m_logChain = new wxLogChain(new CXReflectorLogRedirect);

#if defined(__WINDOWS__)
	m_config = new CXReflectorConfig(new wxConfig(APPLICATION_NAME), m_name);
#else
	if (m_confDir.IsEmpty())
		m_confDir = CONF_DIR;

	m_config = new CXReflectorConfig(m_confDir, m_name);
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

	m_frame = new CXReflectorFrame(frameName, position, m_gui);
	m_frame->Show();

	SetTopWindow(m_frame);

	wxLogInfo(wxT("Starting ") + APPLICATION_NAME + wxT(" - ") + VERSION);

	// Log the SVN revsion and the version of wxWidgets and the Operating System
	wxLogInfo(SVNREV);
	wxLogInfo(wxT("Using wxWidgets %d.%d.%d on %s"), wxMAJOR_VERSION, wxMINOR_VERSION, wxRELEASE_NUMBER, ::wxGetOsDescription().c_str());

	createThread();

	return true;
}

int CXReflectorApp::OnExit()
{
	m_logChain->SetLog(NULL);

	wxLogInfo(APPLICATION_NAME + wxT(" is exiting"));

	m_thread->kill();

	delete m_config;

	return 0;
}

void CXReflectorApp::OnInitCmdLine(wxCmdLineParser& parser)
{
	parser.AddSwitch(NOLOGGING_SWITCH, wxEmptyString, wxEmptyString, wxCMD_LINE_PARAM_OPTIONAL);
	parser.AddSwitch(GUI_SWITCH,       wxEmptyString, wxEmptyString, wxCMD_LINE_PARAM_OPTIONAL);
	parser.AddOption(LOGDIR_OPTION,    wxEmptyString, wxEmptyString, wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL);
	parser.AddOption(CONFDIR_OPTION,   wxEmptyString, wxEmptyString, wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL);
	parser.AddParam(NAME_PARAM, wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL);

	wxApp::OnInitCmdLine(parser);
}

bool CXReflectorApp::OnCmdLineParsed(wxCmdLineParser& parser)
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
void CXReflectorApp::OnAssertFailure(const wxChar* file, int line, const wxChar* func, const wxChar* cond, const wxChar* msg)
{
	wxLogFatalError(wxT("Assertion failed on line %d in file %s and function %s: %s %s"), line, file, func, cond, msg);
}
#endif

void CXReflectorApp::showLog(const wxString& text)
{
	m_frame->showLog(text);
}

void CXReflectorApp::getReflector(wxString& callsign, wxString& address) const
{
	m_config->getReflector(callsign, address);
}

void CXReflectorApp::setReflector(const wxString& callsign, const wxString& address)
{
	m_config->setReflector(callsign, address);
}

void CXReflectorApp::getDPlus(bool& enabled) const
{
	m_config->getDPlus(enabled);
}

void CXReflectorApp::setDPlus(bool enabled)
{
	m_config->setDPlus(enabled);
}

void CXReflectorApp::getMiscellaneous(bool& logEnabled) const
{
	m_config->getMiscellaneous(logEnabled);
}

void CXReflectorApp::setMiscellaneous(bool logEnabled)
{
	m_config->setMiscellaneous(logEnabled);
}

void CXReflectorApp::getPosition(int& x, int& y) const
{
	m_config->getPosition(x, y);
}

void CXReflectorApp::setPosition(int x, int y)
{
	m_config->setPosition(x, y);
}

bool CXReflectorApp::writeConfig()
{
	return m_config->write();
}

void CXReflectorApp::createThread()
{
	CXReflectorThread* thread = new CXReflectorThread(m_name, m_logDir);

	wxString reflector, address;
	getReflector(reflector, address);
	thread->setReflector(reflector, address);
	wxLogInfo(wxT("Callsign set to \"%s\", address set to \"%s\""), reflector.c_str(), address.c_str());

	reflector.Append(wxT("        "));

	reflector.Truncate(LONG_CALLSIGN_LENGTH - 1U);
	reflector.Append(wxT("A"));
	CXReflectorReflectorHandler::add(reflector);
	wxLogInfo(wxT("Creating reflector 1 with callsign \"%s\""), reflector.c_str());

	reflector.Truncate(LONG_CALLSIGN_LENGTH - 1U);
	reflector.Append(wxT("B"));
	CXReflectorReflectorHandler::add(reflector);
	wxLogInfo(wxT("Creating reflector 2 with callsign \"%s\""), reflector.c_str());

	reflector.Truncate(LONG_CALLSIGN_LENGTH - 1U);
	reflector.Append(wxT("C"));
	CXReflectorReflectorHandler::add(reflector);
	wxLogInfo(wxT("Creating reflector 3 with callsign \"%s\""), reflector.c_str());

	reflector.Truncate(LONG_CALLSIGN_LENGTH - 1U);
	reflector.Append(wxT("D"));
	CXReflectorReflectorHandler::add(reflector);
	wxLogInfo(wxT("Creating reflector 4 with callsign \"%s\""), reflector.c_str());

	reflector.Truncate(LONG_CALLSIGN_LENGTH - 1U);
	reflector.Append(wxT("E"));
	CXReflectorReflectorHandler::add(reflector);
	wxLogInfo(wxT("Creating reflector 5 with callsign \"%s\""), reflector.c_str());

	bool dplusEnabled;
	getDPlus(dplusEnabled);
	wxLogInfo(wxT("D-Plus enabled set to %d"), dplusEnabled);
	thread->setDPlus(dplusEnabled);

	bool logEnabled;
	getMiscellaneous(logEnabled);
	wxLogInfo(wxT("Log enabled set to %d"), logEnabled);
	thread->setMiscellaneous(logEnabled);

	// Convert the worker class into a thread
	m_thread = new CXReflectorThreadHelper(thread);
	m_thread->start();
}
