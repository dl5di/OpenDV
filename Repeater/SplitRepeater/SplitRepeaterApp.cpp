/*
 *   Copyright (C) 2010-2013 by Jonathan Naylor G4KLX
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

#include "SplitRepeaterProtocolHandler.h"
#include "SplitRepeaterThread.h"
#include "SplitRepeaterLogger.h"
#include "SplitRepeaterApp.h"
#include "CallsignList.h"
#include "DStarDefines.h"
#include "Version.h"
#include "Logger.h"

#include <wx/cmdline.h>
#include <wx/filename.h>

IMPLEMENT_APP(CSplitRepeaterApp)

const wxChar*       NAME_PARAM = wxT("Repeater Name");
const wxChar* NOLOGGING_SWITCH = wxT("nolog");
const wxChar*       GUI_SWITCH = wxT("gui");
const wxChar*    LOGDIR_OPTION = wxT("logdir");
const wxChar*   CONFDIR_OPTION = wxT("confdir");


CSplitRepeaterApp::CSplitRepeaterApp() :
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

CSplitRepeaterApp::~CSplitRepeaterApp()
{
}

bool CSplitRepeaterApp::OnInit()
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

	m_logChain = new wxLogChain(new CSplitRepeaterLogger);

#if defined(__WINDOWS__)
	m_config = new CSplitRepeaterConfig(new wxConfig(APPLICATION_NAME), m_name);
#else
	if (m_confDir.IsEmpty())
		m_confDir = CONF_DIR;

	m_config = new CSplitRepeaterConfig(m_confDir, m_name);
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

	m_frame = new CSplitRepeaterFrame(frameName, position, m_gui);
	m_frame->Show();

	SetTopWindow(m_frame);

	wxLogInfo(wxT("Starting ") + APPLICATION_NAME + wxT(" - ") + VERSION);

	// Log the SVN revsion and the version of wxWidgets and the Operating System
	wxLogInfo(SVNREV);
	wxLogInfo(wxT("Using wxWidgets %d.%d.%d on %s"), wxMAJOR_VERSION, wxMINOR_VERSION, wxRELEASE_NUMBER, ::wxGetOsDescription().c_str());

	createThread();

	return true;
}

int CSplitRepeaterApp::OnExit()
{
	m_logChain->SetLog(NULL);

	wxLogInfo(APPLICATION_NAME + wxT(" is exiting"));

	m_thread->kill();

	delete m_config;

	return 0;
}

void CSplitRepeaterApp::OnInitCmdLine(wxCmdLineParser& parser)
{
	parser.AddSwitch(NOLOGGING_SWITCH, wxEmptyString, wxEmptyString, wxCMD_LINE_PARAM_OPTIONAL);
	parser.AddSwitch(GUI_SWITCH,       wxEmptyString, wxEmptyString, wxCMD_LINE_PARAM_OPTIONAL);
	parser.AddOption(LOGDIR_OPTION,    wxEmptyString, wxEmptyString, wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL);
	parser.AddOption(CONFDIR_OPTION,   wxEmptyString, wxEmptyString, wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL);
	parser.AddParam(NAME_PARAM, wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL);

	wxApp::OnInitCmdLine(parser);
}

bool CSplitRepeaterApp::OnCmdLineParsed(wxCmdLineParser& parser)
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
void CSplitRepeaterApp::OnAssertFailure(const wxChar* file, int line, const wxChar* func, const wxChar* cond, const wxChar* msg)
{
	wxLogFatalError(wxT("Assertion failed on line %d in file %s and function %s: %s %s"), line, file, func, cond, msg);
}
#endif

CSplitRepeaterStatusData* CSplitRepeaterApp::getStatus() const
{
	return m_thread->getStatus();
}

void CSplitRepeaterApp::showLog(const wxString& text)
{
	m_frame->showLog(text);
}

void CSplitRepeaterApp::getCallsign(wxString& callsign, wxString& gateway, DSTAR_MODE& mode, ACK_TYPE& ack, bool& restriction, bool& rpt1Validation, bool& dtmfBlanking) const
{
	m_config->getCallsign(callsign, gateway, mode, ack, restriction, rpt1Validation, dtmfBlanking);
}

void CSplitRepeaterApp::setCallsign(const wxString& callsign, const wxString& gateway, DSTAR_MODE mode, ACK_TYPE ack, bool restriction, bool rpt1Validation, bool dtmfBlanking)
{
	m_config->setCallsign(callsign, gateway, mode, ack, restriction, rpt1Validation, dtmfBlanking);
}

void CSplitRepeaterApp::getNetwork(wxString& gatewayAddress, unsigned int& gatewayPort, wxString& localAddress, unsigned int& localPort) const
{
	m_config->getNetwork(gatewayAddress, gatewayPort, localAddress, localPort);
}

void CSplitRepeaterApp::setNetwork(const wxString& gatewayAddress, unsigned int gatewayPort, const wxString& localAddress, unsigned int localPort)
{
	m_config->setNetwork(gatewayAddress, gatewayPort, localAddress, localPort);
}

void CSplitRepeaterApp::getReceiver1(wxString& address, unsigned int& port) const
{
	m_config->getReceiver1(address, port);
}

void CSplitRepeaterApp::setReceiver1(const wxString& address, unsigned int port)
{
	m_config->setReceiver1(address, port);
}

void CSplitRepeaterApp::getReceiver2(wxString& address, unsigned int& port) const
{
	m_config->getReceiver2(address, port);
}

void CSplitRepeaterApp::setReceiver2(const wxString& address, unsigned int port)
{
	m_config->setReceiver2(address, port);
}

void CSplitRepeaterApp::getTransmitter1(wxString& address, unsigned int& port) const
{
	m_config->getTransmitter1(address, port);
}

void CSplitRepeaterApp::setTransmitter1(const wxString& address, unsigned int port)
{
	m_config->setTransmitter1(address, port);
}

void CSplitRepeaterApp::getTransmitter2(wxString& address, unsigned int& port) const
{
	m_config->getTransmitter2(address, port);
}

void CSplitRepeaterApp::setTransmitter2(const wxString& address, unsigned int port)
{
	m_config->setTransmitter2(address, port);
}

void CSplitRepeaterApp::getTimes(unsigned int& timeout, unsigned int& ackTime, unsigned int& frameWaitTime) const
{
	m_config->getTimes(timeout, ackTime, frameWaitTime);
}

void CSplitRepeaterApp::setTimes(unsigned int timeout, unsigned int ackTime, unsigned int frameWaitTime)
{
	m_config->setTimes(timeout, ackTime, frameWaitTime);
}

void CSplitRepeaterApp::getBeacon(unsigned int& time, wxString& text, bool& voice, TEXT_LANG& language) const
{
	m_config->getBeacon(time, text, voice, language);
}

void CSplitRepeaterApp::setBeacon(unsigned int time, const wxString& text, bool voice, TEXT_LANG language)
{
	m_config->setBeacon(time, text, voice, language);
}

void CSplitRepeaterApp::getAnnouncement(bool& enabled, unsigned int& time, wxString& recordRPT1, wxString& recordRPT2, wxString& deleteRPT1, wxString& deleteRPT2) const
{
	m_config->getAnnouncement(enabled, time, recordRPT1, recordRPT2, deleteRPT1, deleteRPT2);
}

void CSplitRepeaterApp::setAnnouncement(bool enabled, unsigned int time, const wxString& recordRPT1, const wxString& recordRPT2, const wxString& deleteRPT1, const wxString& deleteRPT2)
{
	m_config->setAnnouncement(enabled, time, recordRPT1, recordRPT2, deleteRPT1, deleteRPT2);
}

void CSplitRepeaterApp::getControl(bool& enabled, wxString& rpt1Callsign, wxString& rpt2Callsign, wxString& shutdown, wxString& startup, wxString& status1, wxString& status2, wxString& status3, wxString& status4, wxString& status5, wxString& command1, wxString& command1Line, wxString& command2, wxString& command2Line, wxString& command3, wxString& command3Line, wxString& command4, wxString& command4Line) const
{
	m_config->getControl(enabled, rpt1Callsign, rpt2Callsign, shutdown, startup, status1, status2, status3, status4, status5, command1, command1Line, command2, command2Line, command3, command3Line, command4, command4Line);
}

void CSplitRepeaterApp::setControl(bool enabled, const wxString& rpt1Callsign, const wxString& rpt2Callsign, const wxString& shutdown, const wxString& startup, const wxString& status1, const wxString& status2, const wxString& status3, const wxString& status4, const wxString& status5, const wxString& command1, const wxString& command1Line, const wxString& command2, const wxString& command2Line, const wxString& command3, const wxString& command3Line, const wxString& command4, const wxString& command4Line)
{
	m_config->setControl(enabled, rpt1Callsign, rpt2Callsign, shutdown, startup, status1, status2, status3, status4, status5, command1, command1Line, command2, command2Line, command3, command3Line, command4, command4Line);
}

void CSplitRepeaterApp::getPosition(int& x, int& y) const
{
	m_config->getPosition(x, y);
}

void CSplitRepeaterApp::setPosition(int x, int y)
{
	m_config->setPosition(x, y);
}

bool CSplitRepeaterApp::writeConfig()
{
	return m_config->write();
}

void CSplitRepeaterApp::shutdown()
{
	m_thread->shutdown();
}

void CSplitRepeaterApp::startup()
{
	m_thread->startup();
}

void CSplitRepeaterApp::command1()
{
	m_thread->command1();
}

void CSplitRepeaterApp::command2()
{
	m_thread->command2();
}

void CSplitRepeaterApp::command3()
{
	m_thread->command3();
}

void CSplitRepeaterApp::command4()
{
	m_thread->command4();
}

void CSplitRepeaterApp::createThread()
{
	CSplitRepeaterThread* thread = new CSplitRepeaterThread;

	wxString callsign, gateway;
	DSTAR_MODE mode;
	ACK_TYPE ack;
	bool restriction, rpt1Validation, dtmfBlanking;
	getCallsign(callsign, gateway, mode, ack, restriction, rpt1Validation, dtmfBlanking);
	thread->setCallsign(callsign, gateway, mode, ack, restriction, rpt1Validation, dtmfBlanking);
	wxLogInfo(wxT("Callsign set to \"%s\", gateway set to \"%s\", mode: %d, ack: %d, restriction: %d, RPT1 validation: %d, DTMF blanking: %d"), callsign.c_str(), gateway.c_str(), int(mode), int(ack), int(restriction), int(rpt1Validation), int(dtmfBlanking));

	wxString gatewayAddress, localAddress;
	unsigned int gatewayPort, localPort;
	getNetwork(gatewayAddress, gatewayPort, localAddress, localPort);
	wxLogInfo(wxT("Gateway set to %s:%u, local set to %s:%u"), gatewayAddress.c_str(), gatewayPort, localAddress.c_str(), localPort);

	if (!gatewayAddress.IsEmpty() && gatewayPort > 0U && localPort > 0U) {
		CSplitRepeaterProtocolHandler* handler = new CSplitRepeaterProtocolHandler(localAddress, localPort);

		bool res = handler->open();
		if (!res)
			wxLogError(wxT("Cannot open the protocol handler"));
		else
			thread->setProtocolHandler(handler);

		thread->setGateway(gatewayAddress, gatewayPort);
	}

	unsigned int timeout, ackTime, frameWaitTime;
	getTimes(timeout, ackTime, frameWaitTime);
	thread->setTimes(timeout, ackTime, frameWaitTime);
	wxLogInfo(wxT("Timeout set to %u secs, ack time set to %u ms, frame wait time set to %u ms"), timeout, ackTime, frameWaitTime);

	unsigned int beaconTime;
	wxString beaconText;
	bool beaconVoice;
	TEXT_LANG language;
	getBeacon(beaconTime, beaconText, beaconVoice, language);
	thread->setBeacon(beaconTime, beaconText, beaconVoice, language);
	wxLogInfo(wxT("Beacon set to %u mins, text set to \"%s\", voice set to %d, language set to %d"), beaconTime / 60U, beaconText.c_str(), int(beaconVoice), int(language));

	bool announcementEnabled;
	unsigned int announcementTime;
	wxString announcementRecordRPT1, announcementRecordRPT2;
	wxString announcementDeleteRPT1, announcementDeleteRPT2;
	getAnnouncement(announcementEnabled, announcementTime, announcementRecordRPT1, announcementRecordRPT2, announcementDeleteRPT1, announcementDeleteRPT2);
	thread->setAnnouncement(announcementEnabled, announcementTime, announcementRecordRPT1, announcementRecordRPT2, announcementDeleteRPT1, announcementDeleteRPT2);
	wxLogInfo(wxT("Announcement enabled: %d, time: %u mins, record RPT1: \"%s\", record RPT2: \"%s\", delete RPT1: \"%s\", delete RPT2: \"%s\""), int(announcementEnabled), announcementTime / 60U, announcementRecordRPT1.c_str(), announcementRecordRPT2.c_str(), announcementDeleteRPT1.c_str(), announcementDeleteRPT2.c_str());

	wxString receiver1Address;
	unsigned int receiver1Port;
	getReceiver1(receiver1Address, receiver1Port);

	if (!receiver1Address.IsEmpty() && receiver1Port > 0U) {
		wxLogInfo(wxT("Receiver 1 set to %s:%u"), receiver1Address.c_str(), receiver1Port);

		bool res = thread->setReceiver1(receiver1Address, receiver1Port);
		if (!res)
			wxLogError(wxT("The IP address is invalid"));
	}

	wxString receiver2Address;
	unsigned int receiver2Port;
	getReceiver2(receiver2Address, receiver2Port);

	if (!receiver2Address.IsEmpty() && receiver2Port > 0U) {
		wxLogInfo(wxT("Receiver 2 set to %s:%u"), receiver2Address.c_str(), receiver2Port);

		bool res = thread->setReceiver2(receiver2Address, receiver2Port);
		if (!res)
			wxLogError(wxT("The IP address is invalid"));
	}

	wxString transmitter1Address;
	unsigned int transmitter1Port;
	getTransmitter1(transmitter1Address, transmitter1Port);

	if (!transmitter1Address.IsEmpty() && transmitter1Port > 0U) {
		wxLogInfo(wxT("Transmitter 1 set to %s:%u"), transmitter1Address.c_str(), transmitter1Port);

		bool res = thread->setTransmitter1(transmitter1Address, transmitter1Port);
		if (!res)
			wxLogError(wxT("The IP address is invalid"));
	}

	wxString transmitter2Address;
	unsigned int transmitter2Port;
	getTransmitter2(transmitter2Address, transmitter2Port);

	if (!transmitter2Address.IsEmpty() && transmitter2Port > 0U) {
		wxLogInfo(wxT("Transmitter 2 set to %s:%u"), transmitter2Address.c_str(), transmitter2Port);

		bool res = thread->setTransmitter2(transmitter2Address, transmitter2Port);
		if (!res)
			wxLogError(wxT("The IP address is invalid"));
	}

	bool enabled;
	wxString rpt1Callsign, rpt2Callsign;
	wxString shutdown, startup;
	wxString status1, status2, status3, status4, status5;
	wxString command1, command1Line, command2, command2Line;
	wxString command3, command3Line, command4, command4Line;
	getControl(enabled, rpt1Callsign, rpt2Callsign, shutdown, startup, status1, status2, status3, status4, status5, command1, command1Line, command2, command2Line, command3, command3Line, command4, command4Line);
	thread->setControl(enabled, rpt1Callsign, rpt2Callsign, shutdown, startup, status1, status2, status3, status4, status5, command1, command1Line, command2, command2Line, command3, command3Line, command4, command4Line);
	wxLogInfo(wxT("Control: enabled: %d, RPT1: %s, RPT2: %s, shutdown: %s, startup: %s, status1: %s, status2: %s, status3: %s, status4: %s, status5: %s, command1: %s = %s, command2: %s = %s, command3: %s = %s, command4: %s = %s"), enabled, rpt1Callsign.c_str(), rpt2Callsign.c_str(), shutdown.c_str(), startup.c_str(), status1.c_str(), status2.c_str(), status3.c_str(), status4.c_str(), status5.c_str(), command1.c_str(), command1Line.c_str(), command2.c_str(), command2Line.c_str(), command3.c_str(), command3Line.c_str(), command4.c_str(), command4Line.c_str());

	wxFileName blFilename(wxFileName::GetHomeDir(), BLACKLIST_FILE_NAME);
	bool exists = blFilename.FileExists();
	if (exists) {
		CCallsignList* list = new CCallsignList(blFilename.GetFullPath());
		bool res = list->load();
		if (!res) {
			wxLogError(wxT("Unable to open black list file - %s"), blFilename.GetFullPath().c_str());
			delete list;
		} else {
			wxLogInfo(wxT("%u callsigns loaded into the black list"), list->getCount());
			thread->setBlackList(list);
		}
	}

	wxFileName glFilename(wxFileName::GetHomeDir(), GREYLIST_FILE_NAME);
	exists = glFilename.FileExists();
	if (exists) {
		CCallsignList* list = new CCallsignList(glFilename.GetFullPath());
		bool res = list->load();
		if (!res) {
			wxLogError(wxT("Unable to open grey list file - %s"), glFilename.GetFullPath().c_str());
			delete list;
		} else {
			wxLogInfo(wxT("%u callsigns loaded into the grey list"), list->getCount());
			thread->setGreyList(list);
		}
	}

	// Convert the worker class into a thread
	m_thread = new CSplitRepeaterThreadHelper(thread);
	m_thread->start();
}
