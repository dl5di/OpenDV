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

#include "RepeaterProtocolHandler.h"
#include "DVAPNodeTRXThread.h"
#include "DVAPNodeRXThread.h"
#include "DVAPNodeTXThread.h"
#include "DVAPController.h"
#include "DVAPNodeLogger.h"
#include "DVAPNodeThread.h"
#include "DStarDefines.h"
#include "DVAPNodeApp.h"
#include "Version.h"
#include "Logger.h"
#include "Utils.h"

#include <wx/cmdline.h>
#include <wx/filename.h>

IMPLEMENT_APP(CDVAPNodeApp)

const wxChar*       NAME_PARAM = wxT("Node Name");
const wxChar* NOLOGGING_SWITCH = wxT("nolog");
const wxChar*       GUI_SWITCH = wxT("gui");
const wxChar*    LOGDIR_OPTION = wxT("logdir");
const wxChar*   CONFDIR_OPTION = wxT("confdir");
const wxChar*  AUDIODIR_OPTION = wxT("audiodir");


CDVAPNodeApp::CDVAPNodeApp() :
wxApp(),
m_name(),
m_nolog(false),
m_gui(false),
m_logDir(),
m_confDir(),
m_audioDir(),
m_frame(NULL),
m_thread(NULL),
m_config(NULL),
m_logChain(NULL)
{
}

CDVAPNodeApp::~CDVAPNodeApp()
{
}

bool CDVAPNodeApp::OnInit()
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

	m_logChain = new wxLogChain(new CDVAPNodeLogger);

#if defined(__WINDOWS__)
	m_config = new CDVAPNodeConfig(new wxConfig(APPLICATION_NAME), m_name);
#else
	if (m_confDir.IsEmpty())
		m_confDir = CONF_DIR;

	m_config = new CDVAPNodeConfig(m_confDir, m_name);
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

	m_frame = new CDVAPNodeFrame(frameName, position, m_gui);
	m_frame->Show();

	SetTopWindow(m_frame);

	wxLogInfo(wxT("Starting ") + APPLICATION_NAME + wxT(" - ") + VERSION);

	// Log the SVN revsion and the version of wxWidgets and the Operating System
	wxLogInfo(SVNREV);
	wxLogInfo(wxT("Using wxWidgets %d.%d.%d on %s"), wxMAJOR_VERSION, wxMINOR_VERSION, wxRELEASE_NUMBER, ::wxGetOsDescription().c_str());

	createThread();

	return true;
}

int CDVAPNodeApp::OnExit()
{
	m_logChain->SetLog(NULL);

	wxLogInfo(APPLICATION_NAME + wxT(" is exiting"));

	m_thread->kill();

	delete m_config;

	return 0;
}

void CDVAPNodeApp::OnInitCmdLine(wxCmdLineParser& parser)
{
	parser.AddSwitch(NOLOGGING_SWITCH, wxEmptyString, wxEmptyString, wxCMD_LINE_PARAM_OPTIONAL);
	parser.AddSwitch(GUI_SWITCH,       wxEmptyString, wxEmptyString, wxCMD_LINE_PARAM_OPTIONAL);
	parser.AddOption(LOGDIR_OPTION,    wxEmptyString, wxEmptyString, wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL);
	parser.AddOption(CONFDIR_OPTION,   wxEmptyString, wxEmptyString, wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL);
	parser.AddOption(AUDIODIR_OPTION,  wxEmptyString, wxEmptyString, wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL);
	parser.AddParam(NAME_PARAM, wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL);

	wxApp::OnInitCmdLine(parser);
}

bool CDVAPNodeApp::OnCmdLineParsed(wxCmdLineParser& parser)
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

	wxString audioDir;
	found = parser.Found(AUDIODIR_OPTION, &audioDir);
	if (found)
		m_audioDir = audioDir;
	else
		m_audioDir = ::wxGetHomeDir();

	if (parser.GetParamCount() > 0U)
		m_name = parser.GetParam(0U);

	return true;
}

#if defined(__WXDEBUG__)
void CDVAPNodeApp::OnAssertFailure(const wxChar* file, int line, const wxChar* func, const wxChar* cond, const wxChar* msg)
{
	wxLogFatalError(wxT("Assertion failed on line %d in file %s and function %s: %s %s"), line, file, func, cond, msg);
}
#endif

CDVAPNodeStatusData* CDVAPNodeApp::getStatus() const
{
	return m_thread->getStatus();
}

void CDVAPNodeApp::showLog(const wxString& text)
{
	m_frame->showLog(text);
}

void CDVAPNodeApp::getCallsign(wxString& callsign, wxString& gateway, DSTAR_MODE& mode, ACK_TYPE& ack, bool& restriction, bool& rpt1Validation) const
{
	m_config->getCallsign(callsign, gateway, mode, ack, restriction, rpt1Validation);
}

void CDVAPNodeApp::setCallsign(const wxString& callsign, const wxString& gateway, DSTAR_MODE mode, ACK_TYPE ack, bool restriction, bool rpt1Validation)
{
	m_config->setCallsign(callsign, gateway, mode, ack, restriction, rpt1Validation);
}

void CDVAPNodeApp::getNetwork(wxString& gatewayAddress, unsigned int& gatewayPort, wxString& localAddress, unsigned int& localPort) const
{
	m_config->getNetwork(gatewayAddress, gatewayPort, localAddress, localPort);
}

void CDVAPNodeApp::setNetwork(const wxString& gatewayAddress, unsigned int gatewayPort, const wxString& localAddress, unsigned int localPort)
{
	m_config->setNetwork(gatewayAddress, gatewayPort, localAddress, localPort);
}

void CDVAPNodeApp::getDVAP(wxString& port, unsigned int& frequency, int& power, int& squelch) const
{
	m_config->getDVAP(port, frequency, power, squelch);
}

void CDVAPNodeApp::setDVAP(const wxString& port, unsigned int frequency, int power, int squelch)
{
	m_config->setDVAP(port, frequency, power, squelch);
}

void CDVAPNodeApp::getTimes(unsigned int& timeout, unsigned int& ackTime) const
{
	m_config->getTimes(timeout, ackTime);
}

void CDVAPNodeApp::setTimes(unsigned int timeout, unsigned int ackTime)
{
	m_config->setTimes(timeout, ackTime);
}

void CDVAPNodeApp::getBeacon(unsigned int& time, wxString& text, bool& voice, TEXT_LANG& language) const
{
	m_config->getBeacon(time, text, voice, language);
}

void CDVAPNodeApp::setBeacon(unsigned int time, const wxString& text, bool voice, TEXT_LANG language)
{
	m_config->setBeacon(time, text, voice, language);
}

void CDVAPNodeApp::getAnnouncement(bool& enabled, unsigned int& time, wxString& recordRPT1, wxString& recordRPT2, wxString& deleteRPT1, wxString& deleteRPT2) const
{
	m_config->getAnnouncement(enabled, time, recordRPT1, recordRPT2, deleteRPT1, deleteRPT2);
}

void CDVAPNodeApp::setAnnouncement(bool enabled, unsigned int time, const wxString& recordRPT1, const wxString& recordRPT2, const wxString& deleteRPT1, const wxString& deleteRPT2)
{
	m_config->setAnnouncement(enabled, time, recordRPT1, recordRPT2, deleteRPT1, deleteRPT2);
}

void CDVAPNodeApp::getLogging(bool& logging) const
{
	m_config->getLogging(logging);
}

void CDVAPNodeApp::setLogging(bool logging)
{
	wxLogInfo(wxT("Frame logging set to %d, in %s"), int(logging), m_audioDir.c_str());

	m_config->setLogging(logging);
	m_thread->setLogging(logging, m_audioDir);
}

void CDVAPNodeApp::getPosition(int& x, int& y) const
{
	m_config->getPosition(x, y);
}

void CDVAPNodeApp::setPosition(int x, int y)
{
	m_config->setPosition(x, y);
}

bool CDVAPNodeApp::writeConfig()
{
	return m_config->write();
}

void CDVAPNodeApp::createThread()
{
	wxString callsign, gateway;
	DSTAR_MODE mode;
	ACK_TYPE ack;
	bool restriction, rpt1Validation;
	getCallsign(callsign, gateway, mode, ack, restriction, rpt1Validation);

	IDVAPNodeThread* thread = NULL;
	switch (mode) {
		case MODE_RXONLY:
			thread = new CDVAPNodeRXThread;
			break;
		case MODE_TXONLY:
			thread = new CDVAPNodeTXThread;
			break;
		default:
			thread = new CDVAPNodeTRXThread;
			break;
	}

	thread->setCallsign(callsign, gateway, mode, ack, restriction, rpt1Validation);
	wxLogInfo(wxT("Callsign set to \"%s\", gateway set to \"%s\", mode: %d, ack: %d, restriction: %d, RPT1 validation: %d"), callsign.c_str(), gateway.c_str(), int(mode), int(ack), restriction, rpt1Validation);

	wxString gatewayAddress, localAddress;
	unsigned int gatewayPort, localPort;
	getNetwork(gatewayAddress, gatewayPort, localAddress, localPort);
	wxLogInfo(wxT("Gateway set to %s:%u, local set to %s:%u"), gatewayAddress.c_str(), gatewayPort, localAddress.c_str(), localPort);

	if (!gatewayAddress.IsEmpty()) {
		CRepeaterProtocolHandler* handler = new CRepeaterProtocolHandler(gatewayAddress, gatewayPort, localAddress, localPort);

		bool res = handler->open();
		if (!res)
			wxLogError(wxT("Cannot open the protocol handler"));
		else
			thread->setProtocolHandler(handler);
	}

	unsigned int timeout, ackTime;
	getTimes(timeout, ackTime);
	thread->setTimes(timeout, ackTime);
	wxLogInfo(wxT("Timeout set to %u secs, ack time set to %u ms"), timeout, ackTime);

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

	wxString dvapPort;
	unsigned int dvapFrequency;
	int dvapPower, dvapSquelch;
	getDVAP(dvapPort, dvapFrequency, dvapPower, dvapSquelch);
	wxLogInfo(wxT("DVAP: port: %s, frequency: %u Hz, power: %d dBm, squelch: %d dBm"), dvapPort.c_str(), dvapFrequency, dvapPower, dvapSquelch);

	if (!dvapPort.IsEmpty()) {
		CDVAPController* dvap = new CDVAPController(dvapPort, dvapFrequency, dvapPower, dvapSquelch);
		bool res = dvap->open();
		if (!res)
			wxLogError(wxT("Unable to open the DVAP"));
		else
			thread->setDVAP(dvap);
	}

	bool logging;
	getLogging(logging);
	thread->setLogging(logging, m_audioDir);
	m_frame->setLogging(logging);
	wxLogInfo(wxT("Frame logging set to %d, in %s"), int(logging), m_audioDir.c_str());

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
	m_thread = new CDVAPNodeThreadHelper(thread);
	m_thread->start();
}
