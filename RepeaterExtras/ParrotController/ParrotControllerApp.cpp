/*
 *   Copyright (C) 2009-2012 by Jonathan Naylor G4KLX
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

#include "ParrotControllerProtocolHandler.h"
#include "ParrotControllerApp.h"
#include "Version.h"
#include "Logger.h"

#include <wx/config.h>
#include <wx/cmdline.h>
#include <wx/filename.h>

IMPLEMENT_APP(CParrotControllerApp)


const wxChar*       NAME_PARAM = wxT("Parrot Name");
const wxChar* NOLOGGING_SWITCH = wxT("nolog");
const wxChar*       GUI_SWITCH = wxT("gui");
const wxChar*    LOGDIR_OPTION = wxT("logdir");
const wxChar*   CONFDIR_OPTION = wxT("confdir");


CParrotControllerApp::CParrotControllerApp() :
wxApp(),
m_name(),
m_nolog(false),
m_gui(false),
m_logDir(),
m_confDir(),
m_frame(NULL),
m_thread(NULL),
m_config(NULL)
{
}

CParrotControllerApp::~CParrotControllerApp()
{
}

bool CParrotControllerApp::OnInit()
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

#if defined(__WINDOWS__)
	m_config = new CParrotControllerConfig(new wxConfig(APPLICATION_NAME), m_name);
#else
	if (m_confDir.IsEmpty())
		m_confDir = CONF_DIR;

	m_config = new CParrotControllerConfig(m_confDir, m_name);
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

	m_frame = new CParrotControllerFrame(frameName, position, m_gui);
	m_frame->Show();

	SetTopWindow(m_frame);

	wxLogInfo(wxT("Starting ") + APPLICATION_NAME + wxT(" - ") + VERSION);

	// Log the SVN revsion and the version of wxWidgets and the Operating System
	wxLogInfo(SVNREV);
	wxLogInfo(wxT("Using wxWidgets %d.%d.%d on %s"), wxMAJOR_VERSION, wxMINOR_VERSION, wxRELEASE_NUMBER, ::wxGetOsDescription().c_str());

	createThread();

	return true;
}

int CParrotControllerApp::OnExit()
{
	wxLogInfo(APPLICATION_NAME + wxT(" is exiting"));

	m_thread->kill();

	delete m_config;

	return 0;
}

void CParrotControllerApp::OnInitCmdLine(wxCmdLineParser& parser)
{
	parser.AddSwitch(NOLOGGING_SWITCH, wxEmptyString, wxEmptyString, wxCMD_LINE_PARAM_OPTIONAL);
	parser.AddSwitch(GUI_SWITCH,       wxEmptyString, wxEmptyString, wxCMD_LINE_PARAM_OPTIONAL);
	parser.AddOption(LOGDIR_OPTION,    wxEmptyString, wxEmptyString, wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL);
	parser.AddOption(CONFDIR_OPTION,   wxEmptyString, wxEmptyString, wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL);
	parser.AddParam(NAME_PARAM, wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL);

	wxApp::OnInitCmdLine(parser);
}

bool CParrotControllerApp::OnCmdLineParsed(wxCmdLineParser& parser)
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
void CParrotControllerApp::OnAssertFailure(const wxChar* file, int line, const wxChar* func, const wxChar* cond, const wxChar* msg)
{
	wxLogFatalError(wxT("Assertion failed on line %d in file %s and function %s: %s %s"), line, file, func, cond, msg);
}
#endif

CParrotControllerStatusData* CParrotControllerApp::getStatus() const
{
	return m_thread->getStatus();
}

void CParrotControllerApp::getMode(PARROT_MODE& mode, unsigned int& beaconTime, wxString& beaconFileName, unsigned int& turnaroundTime, bool& keepFile) const
{
	m_config->getMode(mode, beaconTime, beaconFileName, turnaroundTime, keepFile);
}

void CParrotControllerApp::setMode(PARROT_MODE mode, unsigned int beaconTime, const wxString& beaconFileName, unsigned int turnaroundTime, bool keepFile)
{
	m_config->setMode(mode, beaconTime, beaconFileName, turnaroundTime, keepFile);
}

void CParrotControllerApp::getNetwork(wxString& repeaterAddress, unsigned int& repeaterPort, wxString& localAddress, unsigned int& localPort) const
{
	m_config->getNetwork(repeaterAddress, repeaterPort, localAddress, localPort);
}

void CParrotControllerApp::setNetwork(const wxString& repeaterAddress, unsigned int repeaterPort, const wxString& localAddress, unsigned int localPort)
{
	m_config->setNetwork(repeaterAddress, repeaterPort, localAddress, localPort);
}

void CParrotControllerApp::getPosition(int& x, int& y) const
{
	m_config->getPosition(x, y);
}

void CParrotControllerApp::setPosition(int x, int y)
{
	m_config->setPosition(x, y);
}

bool CParrotControllerApp::writeConfig()
{
	return m_config->write();
}

void CParrotControllerApp::createThread()
{
	CParrotControllerThread* thread = new CParrotControllerThread;

	PARROT_MODE mode;
	unsigned int beaconTime, turnaroundTime;
	wxString beaconFileName;
	bool keepFile;
	getMode(mode, beaconTime, beaconFileName, turnaroundTime, keepFile);
	thread->setMode(mode, beaconTime, beaconFileName, turnaroundTime, keepFile);
	wxLogInfo(wxT("Mode set to %d, beacon time set to %u secs, beacon file name set to %s, turnaround time set to %u secs, keep file set to %d"), mode, beaconTime, beaconFileName.c_str(), turnaroundTime, keepFile);

	wxString repeaterAddress, localAddress;
	unsigned int repeaterPort, localPort;
	getNetwork(repeaterAddress, repeaterPort, localAddress, localPort);
	wxLogInfo(wxT("Repeater set to %s:%u, local set to %s:%u"), repeaterAddress.c_str(), repeaterPort, localAddress.c_str(), localPort);

	if (!repeaterAddress.IsEmpty()) {
		CParrotControllerProtocolHandler* handler = new CParrotControllerProtocolHandler(repeaterAddress, repeaterPort, localAddress, localPort);

		bool res = handler->open();
		if (!res)
			wxLogError(wxT("Cannot open the protocol handler"));
		else
			thread->setNetwork(handler);
	}

	// Convert the worker class into a thread
	m_thread = new CParrotControllerThreadHelper(thread);
	m_thread->start();
}
