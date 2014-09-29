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
#include "DVAPNodeTXThread.h"
#include "DVAPNodeRXThread.h"
#include "DVAPController.h"
#include "DVAPNodeConfig.h"
#include "DStarDefines.h"
#include "CallsignList.h"
#include "DVAPNodeD.h"
#include "Version.h"
#include "Logger.h"
#include "Utils.h"

#include <wx/cmdline.h>
#include <wx/wfstream.h>
#include <wx/fileconf.h>
#include <wx/filename.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

const wxChar*       NAME_PARAM = wxT("Node Name");
const wxChar* NOLOGGING_SWITCH = wxT("nolog");
const wxChar*    LOGDIR_OPTION = wxT("logdir");
const wxChar*   CONFDIR_OPTION = wxT("confdir");
const wxChar*  AUDIODIR_OPTION = wxT("audiodir");
const wxChar*    DAEMON_SWITCH = wxT("daemon");


int main(int argc, char** argv)
{
	bool res = ::wxInitialize();
	if (!res) {
		::fprintf(stderr, "dvapnoded: failed to initialise the wxWidgets library, exiting\n");
		return -1;
	}

	wxCmdLineParser parser(argc, argv);
	parser.AddSwitch(NOLOGGING_SWITCH, wxEmptyString, wxEmptyString, wxCMD_LINE_PARAM_OPTIONAL);
	parser.AddSwitch(DAEMON_SWITCH,    wxEmptyString, wxEmptyString, wxCMD_LINE_PARAM_OPTIONAL);
	parser.AddOption(LOGDIR_OPTION,    wxEmptyString, wxEmptyString, wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL);
	parser.AddOption(CONFDIR_OPTION,   wxEmptyString, wxEmptyString, wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL);
	parser.AddOption(AUDIODIR_OPTION,  wxEmptyString, wxEmptyString, wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL);
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

	wxString audioDir;
	found = parser.Found(AUDIODIR_OPTION, &audioDir);
	if (!found)
		audioDir = ::wxGetHomeDir();

	wxString name;
	if (parser.GetParamCount() > 0U)
		name = parser.GetParam(0U);

	if (daemon) {
		pid_t pid = ::fork();

		if (pid < 0) {
			::fprintf(stderr, "dvapnoded: error in fork(), exiting\n");
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

	CDVAPNodeD repeater(nolog, logDir, confDir, audioDir, name);

	if (!repeater.init()) {
		::wxUninitialize();
		return 1;
	}

	repeater.run();

	::wxUninitialize();
	return 0;
}

CDVAPNodeD::CDVAPNodeD(bool nolog, const wxString& logDir, const wxString& confDir, const wxString& audioDir, const wxString& name) :
m_name(name),
m_nolog(nolog),
m_logDir(logDir),
m_confDir(confDir),
m_audioDir(audioDir),
m_thread(NULL)
{
}

CDVAPNodeD::~CDVAPNodeD()
{
}

bool CDVAPNodeD::init()
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

	// Log the SVN revsion and the version of wxWidgets and the Operating System
	wxLogInfo(SVNREV);
	wxLogInfo(wxT("Using wxWidgets %d.%d.%d on %s"), wxMAJOR_VERSION, wxMINOR_VERSION, wxRELEASE_NUMBER, ::wxGetOsDescription().c_str());

	return createThread();
}

void CDVAPNodeD::run()
{
	m_thread->run();

	wxLogInfo(APPLICATION_NAME + wxT(" is exiting"));
}

bool CDVAPNodeD::createThread()
{
	CDVAPNodeConfig config(m_confDir, m_name);

	wxString callsign, gateway;
	DSTAR_MODE mode;
	ACK_TYPE ack;
	bool restriction, rpt1Validation;
	config.getCallsign(callsign, gateway, mode, ack, restriction, rpt1Validation);

	switch (mode) {
		case MODE_RXONLY:
			m_thread = new CDVAPNodeRXThread;
			break;
		case MODE_TXONLY:
			m_thread = new CDVAPNodeTXThread;
			break;
		default:
			m_thread = new CDVAPNodeTRXThread;
			break;
	}

	m_thread->setCallsign(callsign, gateway, mode, ack, restriction, rpt1Validation);
	wxLogInfo(wxT("Callsign set to \"%s\", gateway set to \"%s\", mode: %d, ack: %d, restriction: %d, RPT1 validation: %d"), callsign.c_str(), gateway.c_str(), int(mode), int(ack), restriction, rpt1Validation);

	wxString gatewayAddress, localAddress;
	unsigned int gatewayPort, localPort;
	config.getNetwork(gatewayAddress, gatewayPort, localAddress, localPort);
	wxLogInfo(wxT("Gateway set to %s:%u, local set to %s:%u"), gatewayAddress.c_str(), gatewayPort, localAddress.c_str(), localPort);

	if (!gatewayAddress.IsEmpty()) {
		CRepeaterProtocolHandler* handler = new CRepeaterProtocolHandler(gatewayAddress, gatewayPort, localAddress, localPort);

		bool res = handler->open();
		if (!res) {
			wxLogError(wxT("Cannot open the protocol handler"));
			return false;
		}

		m_thread->setProtocolHandler(handler);
	}

	unsigned int timeout, ackTime;
	config.getTimes(timeout, ackTime);
	m_thread->setTimes(timeout, ackTime);
	wxLogInfo(wxT("Timeout set to %u secs, ack time set to %u ms"), timeout, ackTime);

	unsigned int beaconTime;
	wxString beaconText;
	bool beaconVoice;
	TEXT_LANG language;
	config.getBeacon(beaconTime, beaconText, beaconVoice, language);
	m_thread->setBeacon(beaconTime, beaconText, beaconVoice, language);
	wxLogInfo(wxT("Beacon set to %u mins, text set to \"%s\", voice set to %d, language set to %d"), beaconTime / 60U, beaconText.c_str(), int(beaconVoice), int(language));

	bool announcementEnabled;
	unsigned int announcementTime;
	wxString announcementRecordRPT1, announcementRecordRPT2;
	wxString announcementDeleteRPT1, announcementDeleteRPT2;
	config.getAnnouncement(announcementEnabled, announcementTime, announcementRecordRPT1, announcementRecordRPT2, announcementDeleteRPT1, announcementDeleteRPT2);
	m_thread->setAnnouncement(announcementEnabled, announcementTime, announcementRecordRPT1, announcementRecordRPT2, announcementDeleteRPT1, announcementDeleteRPT2);
	wxLogInfo(wxT("Announcement enabled: %d, time: %u mins, record RPT1: \"%s\", record RPT2: \"%s\", delete RPT1: \"%s\", delete RPT2: \"%s\""), int(announcementEnabled), announcementTime / 60U, announcementRecordRPT1.c_str(), announcementRecordRPT2.c_str(), announcementDeleteRPT1.c_str(), announcementDeleteRPT2.c_str());

	wxString dvapPort;
	unsigned int dvapFrequency;
	int dvapPower, dvapSquelch;
	config.getDVAP(dvapPort, dvapFrequency, dvapPower, dvapSquelch);
	wxLogInfo(wxT("DVAP: port: %s, frequency: %u Hz, power: %d dBm, squelch: %d dBm"), dvapPort.c_str(), dvapFrequency, dvapPower, dvapSquelch);

	if (!dvapPort.IsEmpty()) {
		CDVAPController* dvap = new CDVAPController(dvapPort, dvapFrequency, dvapPower, dvapSquelch);
		bool res = dvap->open();
		if (!res)
			wxLogError(wxT("Unable to open the DVAP"));
		else
			m_thread->setDVAP(dvap);
	}

	bool logging;
	config.getLogging(logging);
	m_thread->setLogging(logging, m_audioDir);
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
			m_thread->setBlackList(list);
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
			m_thread->setGreyList(list);
		}
	}

	return true;
}
