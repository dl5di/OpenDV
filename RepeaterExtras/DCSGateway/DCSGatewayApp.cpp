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
#include "DCSGatewayApp.h"
#include "Version.h"
#include "Logger.h"

#include <wx/config.h>
#include <wx/cmdline.h>
#include <wx/filename.h>

IMPLEMENT_APP(CDCSGatewayApp)


const wxChar*       NAME_PARAM = wxT("Gateway Name");
const wxChar* NOLOGGING_SWITCH = wxT("nolog");
const wxChar*       GUI_SWITCH = wxT("gui");
const wxChar*    LOGDIR_OPTION = wxT("logdir");
const wxChar*   CONFDIR_OPTION = wxT("confdir");


CDCSGatewayApp::CDCSGatewayApp() :
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

CDCSGatewayApp::~CDCSGatewayApp()
{
}

bool CDCSGatewayApp::OnInit()
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
	m_config = new CDCSGatewayConfig(new wxConfig(APPLICATION_NAME), m_name);
#else
	if (m_confDir.IsEmpty())
		m_confDir = CONF_DIR;

	m_config = new CDCSGatewayConfig(m_confDir, m_name);
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

	m_frame = new CDCSGatewayFrame(frameName, position, m_gui);
	m_frame->Show();

	SetTopWindow(m_frame);

	wxLogInfo(wxT("Starting ") + APPLICATION_NAME + wxT(" - ") + VERSION);

	// Log the version of wxWidgets and the Operating System
	wxLogInfo(wxT("Using wxWidgets %d.%d.%d on %s"), wxMAJOR_VERSION, wxMINOR_VERSION, wxRELEASE_NUMBER, ::wxGetOsDescription().c_str());

	createThread();

	return true;
}

int CDCSGatewayApp::OnExit()
{
	wxLogInfo(APPLICATION_NAME + wxT(" is exiting"));

	m_thread->kill();
	m_thread->Wait();
	delete m_thread;

	delete m_config;

	return 0;
}

void CDCSGatewayApp::OnInitCmdLine(wxCmdLineParser& parser)
{
	parser.AddSwitch(NOLOGGING_SWITCH, wxEmptyString, wxEmptyString, wxCMD_LINE_PARAM_OPTIONAL);
	parser.AddSwitch(GUI_SWITCH,       wxEmptyString, wxEmptyString, wxCMD_LINE_PARAM_OPTIONAL);
	parser.AddOption(LOGDIR_OPTION,    wxEmptyString, wxEmptyString, wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL);
	parser.AddOption(CONFDIR_OPTION,   wxEmptyString, wxEmptyString, wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL);
	parser.AddParam(NAME_PARAM, wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL);

	wxApp::OnInitCmdLine(parser);
}

bool CDCSGatewayApp::OnCmdLineParsed(wxCmdLineParser& parser)
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
void CDCSGatewayApp::OnAssertFailure(const wxChar* file, int line, const wxChar* func, const wxChar* cond, const wxChar* msg)
{
	wxLogFatalError(wxT("Assertion failed on line %d in file %s and function %s: %s %s"), line, file, func, cond, msg);
}
#endif

CDCSGatewayStatusData* CDCSGatewayApp::getStatus() const
{
	return m_thread->getStatus();
}

void CDCSGatewayApp::getReflector(wxString& callsign, wxString& reflector, bool& atStartup, RECONNECT& reconnect, TEXT_LANG& language) const
{
	m_config->getReflector(callsign, reflector, atStartup, reconnect, language);
}

void CDCSGatewayApp::setReflector(const wxString& callsign, const wxString& reflector, bool atStartup, RECONNECT reconnect, TEXT_LANG language)
{
	m_config->setReflector(callsign, reflector, atStartup, reconnect, language);
}

void CDCSGatewayApp::getRepeater(wxString& repeaterCallsign, wxString& repeaterAddress, unsigned int& repeaterPort, wxString& localAddress, unsigned int& localPort) const
{
	m_config->getRepeater(repeaterCallsign, repeaterAddress, repeaterPort, localAddress, localPort);
}

void CDCSGatewayApp::setRepeater(const wxString& repeaterCallsign, const wxString& repeaterAddress, unsigned int repeaterPort, const wxString& localAddress, unsigned int localPort)
{
	m_config->setRepeater(repeaterCallsign, repeaterAddress, repeaterPort, localAddress, localPort);
}

void CDCSGatewayApp::getPosition(int& x, int& y) const
{
	m_config->getPosition(x, y);
}

void CDCSGatewayApp::setPosition(int x, int y)
{
	m_config->setPosition(x, y);
}

bool CDCSGatewayApp::writeConfig()
{
	return m_config->write();
}

void CDCSGatewayApp::createThread()
{
	m_thread = new CDCSGatewayThread;

	wxString callsign, reflector;
	bool atStartup;
	RECONNECT reconnect;
	TEXT_LANG language;
	getReflector(callsign, reflector, atStartup, reconnect, language);
	wxLogInfo(wxT("Callsign set to %s, reflector set to %s, at startup: %d, reconnect: %d, language: %d"), callsign.c_str(), reflector.c_str(), atStartup, reconnect, language);
	m_thread->setReflector(callsign, reflector, atStartup, reconnect, language);

	wxString repeaterCallsign, repeaterAddress, localAddress;
	unsigned int repeaterPort, localPort;
	getRepeater(repeaterCallsign, repeaterAddress, repeaterPort, localAddress, localPort);
	wxLogInfo(wxT("Repeater callsign set to %s, address set to %s:%u, local set to %s:%u"), repeaterCallsign.c_str(), repeaterAddress.c_str(), repeaterPort, localAddress.c_str(), localPort);

	if (!repeaterCallsign.IsEmpty() && !repeaterAddress.IsEmpty()) {
		CDCSGatewayRepeaterProtocolHandler* repeaterHandler = new CDCSGatewayRepeaterProtocolHandler(repeaterAddress, repeaterPort, localAddress, localPort);

		bool res = repeaterHandler->open();
		if (!res)
			wxLogError(wxT("Cannot open the repeater protocol handler"));
		else
			m_thread->setRepeater(repeaterCallsign, repeaterHandler);
	}

	m_thread->Create();
	m_thread->SetPriority(100U);
	m_thread->Run();
}
