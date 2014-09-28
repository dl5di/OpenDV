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

#include "TimerControlConfig.h"
#include "TimerControlDefs.h"
#include "TimerControlAppD.h"
#include "Version.h"
#include "Logger.h"

#include <wx/cmdline.h>
#include <wx/wfstream.h>
#include <wx/fileconf.h>
#include <wx/filename.h>

const wxChar*       NAME_PARAM = wxT("Name");
const wxChar* NOLOGGING_SWITCH = wxT("nolog");
const wxChar*    LOGDIR_OPTION = wxT("logdir");
const wxChar*   CONFDIR_OPTION = wxT("confdir");
const wxChar*    DAEMON_SWITCH = wxT("daemon");

int main(int argc, char** argv)
{
	bool res = ::wxInitialize();
	if (!res) {
		::fprintf(stderr, "timercontrold: failed to initialise the wxWidgets library, exiting\n");
		return -1;
	}

	wxCmdLineParser parser(argc, argv);
	parser.AddSwitch(NOLOGGING_SWITCH, wxEmptyString, wxEmptyString, wxCMD_LINE_PARAM_OPTIONAL);
	parser.AddSwitch(DAEMON_SWITCH,    wxEmptyString, wxEmptyString, wxCMD_LINE_PARAM_OPTIONAL);
	parser.AddOption(LOGDIR_OPTION,    wxEmptyString, wxEmptyString, wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL);
	parser.AddOption(CONFDIR_OPTION,   wxEmptyString, wxEmptyString, wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL);
	parser.AddParam(NAME_PARAM, wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL);

	int cmd = parser.Parse();
	if (cmd != 0) {
		::wxUninitialize();
		return 0;
	}

	bool  nolog = parser.Found(NOLOGGING_SWITCH);
	bool daemon = parser.Found(DAEMON_SWITCH);

	wxString logDir;
	bool found = parser.Found(LOGDIR_OPTION, &logDir);
	if (!found)
		logDir.Clear();

	wxString confDir;
	found = parser.Found(CONFDIR_OPTION, &confDir);
	if (!found)
		confDir = wxT(CONF_DIR);

	wxString name;
	if (parser.GetParamCount() > 0U)
		name = parser.GetParam(0U);

	if (daemon) {
		pid_t pid = ::fork();

		if (pid < 0) {
			::fprintf(stderr, "timercontrold: error in fork(), exiting\n");
			::wxUninitialize();
			return 1;
		}

		// If this is the parent, exit
		if (pid > 0)
			return 0;

		// We are the child from here onwards
		::setsid();

		::chdir("/");

		::umask(0);
	}

	CTimerControlAppD controller(nolog, logDir, confDir, name);

	if (!controller.init()) {
		::wxUninitialize();
		return 1;
	}

	controller.run();

	::wxUninitialize();
	return 0;
}

CTimerControlAppD::CTimerControlAppD(bool nolog, const wxString& logDir, const wxString& confDir, const wxString& name) :
m_name(name),
m_nolog(nolog),
m_logDir(logDir),
m_confDir(confDir),
m_fileName(),
m_thread(NULL)
{
}

CTimerControlAppD::~CTimerControlAppD()
{
}

bool CTimerControlAppD::init()
{
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

	wxLogInfo(wxT("Starting ") + APPLICATION_NAME + wxT(" daemon - ") + VERSION);

	// Log the SVN revsion and the version of wxWidgets and the Operating System
	wxLogInfo(SVNREV);
	wxLogInfo(wxT("Using wxWidgets %d.%d.%d on %s"), wxMAJOR_VERSION, wxMINOR_VERSION, wxRELEASE_NUMBER, ::wxGetOsDescription().c_str());

	return createThread();
}

void CTimerControlAppD::run()
{
	m_thread->run();

	wxLogInfo(APPLICATION_NAME + wxT(" is exiting"));
}

bool CTimerControlAppD::createThread()
{
	CTimerControlConfig config(m_confDir, m_name);

	m_thread = new CTimerControlThread;

	wxString address, password;
	unsigned int port;
	config.getGateway(address, port, password);
	m_thread->setGateway(address, port, password);
	wxLogInfo(wxT("Gateway set to %s:%u"), address.c_str(), port);

	bool delay;
	config.getDelay(delay);
	m_thread->setDelay(delay);
	wxLogInfo(wxT("Delay set to %d"), int(delay));

	wxLogInfo(wxT("Schedule file is %s"), m_fileName.c_str());
	m_thread->setFileName(m_fileName);

	m_thread->reload();

	return true;
}
