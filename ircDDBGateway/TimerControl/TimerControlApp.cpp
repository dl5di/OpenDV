/*
 *   Copyright (C) 2011-2014 by Jonathan Naylor G4KLX
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

#include "TimerControlThread.h"
#include "TimerControlDefs.h"
#include "TimerControlApp.h"
#include "Version.h"
#include "Logger.h"

#include <wx/config.h>
#include <wx/cmdline.h>
#include <wx/filename.h>

IMPLEMENT_APP(CTimerControlApp)

const wxChar*       NAME_PARAM = wxT("Name");
const wxChar* NOLOGGING_SWITCH = wxT("nolog");
const wxChar*    LOGDIR_OPTION = wxT("logdir");
const wxChar*   CONFDIR_OPTION = wxT("confdir");

CTimerControlApp::CTimerControlApp() :
wxApp(),
m_name(),
m_fileName(),
m_nolog(false),
m_logDir(),
m_confDir(),
m_frame(NULL),
m_config(NULL),
m_thread(NULL)
{
}

CTimerControlApp::~CTimerControlApp()
{
}

bool CTimerControlApp::OnInit()
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

		if (m_logDir.IsEmpty())
			m_logDir = wxFileName::GetHomeDir();

		wxLog* log = new CLogger(m_logDir, logBaseName);
		wxLog::SetActiveTarget(log);
	} else {
		new wxLogNull;
	}

#if defined(__WINDOWS__)
	m_config = new CTimerControlConfig(new wxConfig(APPLICATION_NAME), m_name);
#else
	if (m_confDir.IsEmpty())
		m_confDir = wxT(CONF_DIR);

	m_config = new CTimerControlConfig(m_confDir, m_name);
#endif

	wxString frameName = APPLICATION_NAME + wxT(" - ");
	if (!m_name.IsEmpty()) {
		frameName.Append(m_name);
		frameName.Append(wxT(" - "));
	}
	frameName.Append(VERSION);

	if (!m_name.IsEmpty()) {
		wxString fileBase = SCHEDULE_BASE_NAME;
		fileBase.Append(wxT("_"));
		fileBase.Append(m_name);
		fileBase.Replace(wxT(" "), wxT("_"));

		wxString dir = m_confDir;
		if (dir.IsEmpty())
			dir = wxFileName::GetHomeDir();

		wxFileName fileName(dir, fileBase, wxT("dat"));
		m_fileName = fileName.GetFullPath();
	} else {
		wxString dir = m_confDir;
		if (dir.IsEmpty())
			dir = wxFileName::GetHomeDir();

		wxFileName fileName(dir, SCHEDULE_BASE_NAME, wxT("dat"));
		m_fileName = fileName.GetFullPath();
	}

	wxPoint position = wxDefaultPosition;

	int x, y;
	getPosition(x, y);
	if (x >= 0 && y >= 0)
		position = wxPoint(x, y);

	bool delay;
	getDelay(delay);

	m_frame = new CTimerControlFrame(frameName, position, delay);
	m_frame->Show();

	SetTopWindow(m_frame);

	wxLogInfo(wxT("Starting ") + APPLICATION_NAME + wxT(" - ") + VERSION);

	// Log the SVN revsion and the version of wxWidgets and the Operating System
	wxLogInfo(SVNREV);
	wxLogInfo(wxT("Using wxWidgets %d.%d.%d on %s"), wxMAJOR_VERSION, wxMINOR_VERSION, wxRELEASE_NUMBER, ::wxGetOsDescription().c_str());

	createThread();

	return wxApp::OnInit();
}

int CTimerControlApp::OnExit()
{
	wxLogInfo(APPLICATION_NAME + wxT(" is exiting"));

	m_thread->kill();

	delete m_config;

	return 0;
}

void CTimerControlApp::OnInitCmdLine(wxCmdLineParser& parser)
{
	parser.AddSwitch(NOLOGGING_SWITCH, wxEmptyString, wxEmptyString, wxCMD_LINE_PARAM_OPTIONAL);
	parser.AddOption(LOGDIR_OPTION,    wxEmptyString, wxEmptyString, wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL);
	parser.AddOption(CONFDIR_OPTION,   wxEmptyString, wxEmptyString, wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL);
	parser.AddParam(NAME_PARAM, wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL);

	wxApp::OnInitCmdLine(parser);
}

bool CTimerControlApp::OnCmdLineParsed(wxCmdLineParser& parser)
{
	if (!wxApp::OnCmdLineParsed(parser))
		return false;

	m_nolog = parser.Found(NOLOGGING_SWITCH);

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
void CTimerControlApp::OnAssertFailure(const wxChar* file, int line, const wxChar* func, const wxChar* cond, const wxChar* msg)
{
	wxLogFatalError(wxT("Assertion failed on line %d in file %s and function %s: %s %s"), line, file, func, cond, msg);
}
#endif

void CTimerControlApp::getGateway(wxString& address, unsigned int& port, wxString& password) const
{
	m_config->getGateway(address, port, password);
}

void CTimerControlApp::setGateway(const wxString& address, unsigned int port, const wxString& password)
{
	m_config->setGateway(address, port, password);
}

void CTimerControlApp::getDelay(bool& delay) const
{
	m_config->getDelay(delay);
}

void CTimerControlApp::setDelay(bool delay)
{
	m_config->setDelay(delay);
}

void CTimerControlApp::getPosition(int& x, int& y) const
{
	m_config->getPosition(x, y);
}

void CTimerControlApp::setPosition(int x, int y)
{
	m_config->setPosition(x, y);
}

void CTimerControlApp::writeConfig()
{
	m_config->write();
}

void CTimerControlApp::writeItems()
{
	m_frame->writeItems();

	m_thread->reload();
}

void CTimerControlApp::createThread()
{
	CTimerControlThread* thread = new CTimerControlThread;

	wxString address, password;
	unsigned int port;
	getGateway(address, port, password);
	thread->setGateway(address, port, password);
	wxLogInfo(wxT("Gateway set to %s:%u"), address.c_str(), port);

	bool delay;
	getDelay(delay);
	thread->setDelay(delay);
	wxLogInfo(wxT("Delay set to %d"), int(delay));

	wxLogInfo(wxT("Schedule file is %s"), m_fileName.c_str());
	m_frame->setFileName(m_fileName);
	thread->setFileName(m_fileName);

	thread->reload();

	// Convert the worker class into a thread
	m_thread = new CTimerControlThreadHelper(thread);
	m_thread->start();
}
