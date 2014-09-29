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

#include "RepeaterProtocolHandler.h"
#include "GMSKRepeaterTXRXThread.h"
#include "GMSKRepeaterTRXThread.h"
#include "GMSKRepeaterTXThread.h"
#include "GMSKRepeaterRXThread.h"
#if defined(RASPBERRY_PI)
#include "RaspberryController.h"
#endif
#include "GMSKRepeaterConfig.h"
#include "ArduinoController.h"
#include "GMSKModemLibUsb.h"
#include "K8055Controller.h"
#include "DummyController.h"
#include "GMSKRepeaterD.h"
#include "DStarDefines.h"
#include "CallsignList.h"
#include "GMSKModem.h"
#include "Version.h"
#include "Logger.h"

#include <wx/cmdline.h>
#include <wx/wfstream.h>
#include <wx/fileconf.h>
#include <wx/filename.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

const wxChar*       NAME_PARAM = wxT("Repeater Name");
const wxChar* NOLOGGING_SWITCH = wxT("nolog");
const wxChar*    LOGDIR_OPTION = wxT("logdir");
const wxChar*   CONFDIR_OPTION = wxT("confdir");
const wxChar*  AUDIODIR_OPTION = wxT("audiodir");
const wxChar*    DAEMON_SWITCH = wxT("daemon");


int main(int argc, char** argv)
{
	bool res = ::wxInitialize();
	if (!res) {
		::fprintf(stderr, "gmskrepeaterd: failed to initialise the wxWidgets library, exiting\n");
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
			::fprintf(stderr, "gmskrepeaterd: error in fork(), exiting\n");
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

	CGMSKRepeaterD repeater(nolog, logDir, confDir, audioDir, name);

	if (!repeater.init()) {
		::wxUninitialize();
		return 1;
	}

	repeater.run();

	::wxUninitialize();
	return 0;
}

CGMSKRepeaterD::CGMSKRepeaterD(bool nolog, const wxString& logDir, const wxString& confDir, const wxString& audioDir, const wxString& name) :
m_name(name),
m_nolog(nolog),
m_logDir(logDir),
m_confDir(confDir),
m_audioDir(audioDir),
m_thread(NULL)
{
}

CGMSKRepeaterD::~CGMSKRepeaterD()
{
}

bool CGMSKRepeaterD::init()
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

	wxLogInfo(wxT("Starting ") + APPLICATION_NAME + wxT(" daemon - ") + VERSION);

	// Log the SVN revsion and the version of wxWidgets and the Operating System
	wxLogInfo(SVNREV);
	wxLogInfo(wxT("Using wxWidgets %d.%d.%d on %s"), wxMAJOR_VERSION, wxMINOR_VERSION, wxRELEASE_NUMBER, ::wxGetOsDescription().c_str());

	return createThread();
}

void CGMSKRepeaterD::run()
{
	m_thread->run();

	wxLogInfo(APPLICATION_NAME + wxT(" is exiting"));
}

bool CGMSKRepeaterD::createThread()
{
	CGMSKRepeaterConfig config(m_confDir, m_name);

	wxString callsign, gateway;
	DSTAR_MODE mode;
	ACK_TYPE ack;
	bool restriction, rpt1Validation, dtmfBlanking;
	config.getCallsign(callsign, gateway, mode, ack, restriction, rpt1Validation, dtmfBlanking);

	switch (mode) {
		case MODE_RXONLY:
			m_thread = new CGMSKRepeaterRXThread;
			break;
		case MODE_TXONLY:
			m_thread = new CGMSKRepeaterTXThread;
			break;
		case MODE_TXANDRX:
			m_thread = new CGMSKRepeaterTXRXThread;
			break;
		default:
			m_thread = new CGMSKRepeaterTRXThread;
			break;
	}

	m_thread->setCallsign(callsign, gateway, mode, ack, restriction, rpt1Validation, dtmfBlanking);
	wxLogInfo(wxT("Callsign set to \"%s\", gateway set to \"%s\", mode: %d, ack: %d, restriction: %d, RPT1 validation: %d, DTMF blanking: %d"), callsign.c_str(), gateway.c_str(), int(mode), int(ack), int(restriction), int(rpt1Validation), int(dtmfBlanking));

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
	if (mode == MODE_GATEWAY)
		beaconTime = 0U;
	m_thread->setBeacon(beaconTime, beaconText, beaconVoice, language);
	wxLogInfo(wxT("Beacon set to %u mins, text set to \"%s\", voice set to %d, language set to %d"), beaconTime / 60U, beaconText.c_str(), int(beaconVoice), int(language));

	bool announcementEnabled;
	unsigned int announcementTime;
	wxString announcementRecordRPT1, announcementRecordRPT2;
	wxString announcementDeleteRPT1, announcementDeleteRPT2;
	config.getAnnouncement(announcementEnabled, announcementTime, announcementRecordRPT1, announcementRecordRPT2, announcementDeleteRPT1, announcementDeleteRPT2);
	if (mode == MODE_GATEWAY)
		announcementEnabled = false;
	m_thread->setAnnouncement(announcementEnabled, announcementTime, announcementRecordRPT1, announcementRecordRPT2, announcementDeleteRPT1, announcementDeleteRPT2);
	wxLogInfo(wxT("Announcement enabled: %d, time: %u mins, record RPT1: \"%s\", record RPT2: \"%s\", delete RPT1: \"%s\", delete RPT2: \"%s\""), int(announcementEnabled), announcementTime / 60U, announcementRecordRPT1.c_str(), announcementRecordRPT2.c_str(), announcementDeleteRPT1.c_str(), announcementDeleteRPT2.c_str());

	GMSK_MODEM_TYPE modemType;
	unsigned int modemAddress;
	config.getModem(modemType, modemAddress);
	wxLogInfo(wxT("GMSK modem: type: LibUsb, address: 0x%04X"), modemAddress);

	IGMSKModem* modem = new CGMSKModemLibUsb(modemAddress);
	bool res = modem->open();
	if (!res) {
		wxLogError(wxT("Cannot open the GMSK modem"));
		return false;
	}

	m_thread->setModem(modem);

	wxString controllerType;
	unsigned int activeHangTime;
	config.getController(controllerType, activeHangTime);
	wxLogInfo(wxT("Controller set to %s, active hang time: %u ms"), controllerType.c_str(), activeHangTime);

	CExternalController* controller = NULL;

	wxString port;
	if (controllerType.StartsWith(wxT("Velleman K8055 - "), &port)) {
		unsigned long num;
		port.ToULong(&num);
		controller = new CExternalController(new CK8055Controller(num));
	} else if (controllerType.StartsWith(wxT("Arduino - "), &port)) {
		controller = new CExternalController(new CArduinoController(port));
#if defined(RASPBERRY_PI)
	} else if (controllerType.IsSameAs(wxT("Raspberry Pi"))) {
		controller = new CExternalController(new CRaspberryController);
#endif
	} else {
		controller = new CExternalController(new CDummyController);
	}

	res = controller->open();
	if (!res)
		wxLogError(wxT("Cannot open the hardware interface - %s"), controllerType.c_str());
	else
		m_thread->setController(controller, activeHangTime);

	bool out1, out2, out3, out4;
	config.getOutputs(out1, out2, out3, out4);
	m_thread->setOutputs(out1, out2, out3, out4);
	wxLogInfo(wxT("Output 1 = %d, output 2 = %d, output 3 = %d, output 4 = %d"), out1, out2, out3, out4);

	bool enabled;
	wxString rpt1Callsign, rpt2Callsign;
	wxString shutdown, startup;
	wxString status1, status2, status3, status4, status5;
	wxString command1, command1Line, command2, command2Line;
	wxString command3, command3Line, command4, command4Line;
	wxString output1, output2, output3, output4;
	config.getControl(enabled, rpt1Callsign, rpt2Callsign, shutdown, startup, status1, status2, status3, status4, status5, command1, command1Line, command2, command2Line, command3, command3Line, command4, command4Line, output1, output2, output3, output4);
	m_thread->setControl(enabled, rpt1Callsign, rpt2Callsign, shutdown, startup, status1, status2, status3, status4, status5, command1, command1Line, command2, command2Line, command3, command3Line, command4, command4Line, output1, output2, output3, output4);
	wxLogInfo(wxT("Control: enabled: %d, RPT1: %s, RPT2: %s, shutdown: %s, startup: %s, status1: %s, status2: %s, status3: %s, status4: %s, status5: %s, command1: %s = %s, command2: %s = %s, command3: %s = %s, command4: %s = %s, output1: %s, output2: %s, output3: %s, output4: %s"), enabled, rpt1Callsign.c_str(), rpt2Callsign.c_str(), shutdown.c_str(), startup.c_str(), status1.c_str(), status2.c_str(), status3.c_str(), status4.c_str(), status5.c_str(), command1.c_str(), command1Line.c_str(), command2.c_str(), command2Line.c_str(), command3.c_str(), command3Line.c_str(), command4.c_str(), command4Line.c_str(), output1.c_str(), output2.c_str(), output3.c_str(), output4.c_str());

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
