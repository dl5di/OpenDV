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

#include "DCSGatewayRepeaterProtocolHandler.h"
#include "DCSGatewayConfig.h"
#include "DCSGatewayD.h"
#include "Version.h"
#include "Logger.h"

#include <wx/cmdline.h>
#include <wx/wfstream.h>
#include <wx/fileconf.h>
#include <wx/filename.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

const wxChar*       NAME_PARAM = wxT("Gateway Name");
const wxChar* NOLOGGING_SWITCH = wxT("nolog");
const wxChar*    LOGDIR_OPTION = wxT("logdir");
const wxChar*   CONFDIR_OPTION = wxT("confdir");
const wxChar*    DAEMON_SWITCH = wxT("daemon");


int main(int argc, char** argv)
{
	bool res = ::wxInitialize();
	if (!res) {
		::fprintf(stderr, "dcsgatewayd: failed to initialise the wxWidgets library, exiting\n");
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
		confDir = CONF_DIR;

	wxString name;
	if (parser.GetParamCount() > 0U)
		name = parser.GetParam(0U);

	if (daemon) {
		pid_t pid = ::fork();

		if (pid < 0) {
			::fprintf(stderr, "dcsgatewayd: error in fork(), exiting\n");
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

	CDCSGatewayD gateway(nolog, logDir, confDir, name);

	if (!gateway.init()) {
		::wxUninitialize();
		return 1;
	}

	gateway.run();

	::wxUninitialize();
	return 0;
}

CDCSGatewayD::CDCSGatewayD(bool nolog, const wxString& logDir, const wxString& confDir, const wxString& name) :
m_name(name),
m_nolog(nolog),
m_logDir(logDir),
m_confDir(confDir),
m_thread(NULL)
{
}

CDCSGatewayD::~CDCSGatewayD()
{
}

bool CDCSGatewayD::init()
{
	if (!m_nolog) {
		wxString logBaseName = LOG_BASE_NAME;
		if (!m_name.IsEmpty()) {
			logBaseName.Append(wxT("_"));
			logBaseName.Append(m_name);
		}

		if (m_logDir.IsEmpty())
			m_logDir = LOG_DIR;

		wxLog* log = new CLogger(m_logDir, logBaseName);
		wxLog::SetActiveTarget(log);
	} else {
		new wxLogNull;
	}

	wxLogInfo(wxT("Starting ") + APPLICATION_NAME + wxT(" - ") + VERSION);

	// Log the version of wxWidgets and the Operating System
	wxLogInfo(wxT("Using wxWidgets %d.%d.%d on %s"), wxMAJOR_VERSION, wxMINOR_VERSION, wxRELEASE_NUMBER, ::wxGetOsDescription().c_str());

	return createThread();
}

void CDCSGatewayD::run()
{
	m_thread->Create();
	m_thread->Run();

	m_thread->Wait();
	delete m_thread;

	wxLogInfo(APPLICATION_NAME + wxT(" is exiting"));
}

bool CDCSGatewayD::createThread()
{
	CDCSGatewayConfig config(m_confDir, m_name);

	m_thread = new CDCSGatewayThread;

	wxString callsign, reflector;
	bool atStartup;
	RECONNECT reconnect;
	TEXT_LANG language;
	config.getReflector(callsign, reflector, atStartup, reconnect, language);
	wxLogInfo(wxT("Callsign set to %s, reflector set to %s, at startup: %d, reconnect: %d, language: %d"), callsign.c_str(), reflector.c_str(), atStartup, reconnect, language);
	m_thread->setReflector(callsign, reflector, atStartup, reconnect, language);

	wxString repeaterCallsign, repeaterAddress, localAddress;
	unsigned int repeaterPort, localPort;
	config.getRepeater(repeaterCallsign, repeaterAddress, repeaterPort, localAddress, localPort);
	wxLogInfo(wxT("Repeater callsign set to %s, address set to %s:%u, local set to %s:%u"), repeaterCallsign.c_str(), repeaterAddress.c_str(), repeaterPort, localAddress.c_str(), localPort);

	if (!repeaterCallsign.IsEmpty() && !repeaterAddress.IsEmpty()) {
		CDCSGatewayRepeaterProtocolHandler* repeaterHandler = new CDCSGatewayRepeaterProtocolHandler(repeaterAddress, repeaterPort, localAddress, localPort);

		bool res = repeaterHandler->open();
		if (!res) {
			wxLogError(wxT("Cannot open the repeater protocol handler"));
			return false;
		}

		m_thread->setRepeater(repeaterCallsign, repeaterHandler);
	}

	return true;
}
