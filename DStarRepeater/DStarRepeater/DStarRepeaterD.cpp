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

#include "DStarRepeaterTXRXThread.h"
#include "RepeaterProtocolHandler.h"
#include "DStarRepeaterTRXThread.h"
#include "DStarRepeaterRXThread.h"
#include "DStarRepeaterTXThread.h"
#include "SerialLineController.h"
#include "DStarRepeaterConfig.h"
#if defined(RASPBERRY_PI)
#include "RaspberryController.h"
#endif
#include "SoundCardController.h"
#include "DVRPTRV1Controller.h"
#include "DVRPTRV2Controller.h"
#include "DVRPTRV3Controller.h"
#include "ArduinoController.h"
#include "DVMegaController.h"
#include "K8055Controller.h"
#include "DummyController.h"
#include "SplitController.h"
#include "DStarRepeaterD.h"
#include "DVAPController.h"
#include "GMSKController.h"
#include "CallsignList.h"
#include "DStarDefines.h"
#include "Version.h"
#include "Logger.h"

#include <wx/cmdline.h>
#include <wx/wfstream.h>
#include <wx/fileconf.h>
#include <wx/filename.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <fcntl.h>

const wxChar*       NAME_PARAM = wxT("Repeater Name");
const wxChar* NOLOGGING_SWITCH = wxT("nolog");
const wxChar*    LOGDIR_OPTION = wxT("logdir");
const wxChar*   CONFDIR_OPTION = wxT("confdir");
const wxChar*  AUDIODIR_OPTION = wxT("audiodir");
const wxChar*    DAEMON_SWITCH = wxT("daemon");

static CDStarRepeaterD* m_repeater = NULL;

static void handler(int signum)
{
	m_repeater->kill();
}

int main(int argc, char** argv)
{
	bool res = ::wxInitialize();
	if (!res) {
		::fprintf(stderr, "dstarrepeaterd: failed to initialise the wxWidgets library, exiting\n");
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
		confDir = wxT(CONF_DIR);

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
			::fprintf(stderr, "dstarrepeaterd: error in fork(), exiting\n");
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

	wxString pidFileName;
	if (!name.IsEmpty())
		pidFileName.Printf(wxT("/var/run/dstarrepeater_%s.pid"), name.c_str());
	else
		pidFileName = wxT("/var/run/dstarrepeater.pid");
	pidFileName.Replace(wxT(" "), wxT("_"));

	char fileName[128U];
	::memset(fileName, 0x00U, 128U);
	for (unsigned int i = 0U; i < pidFileName.Len(); i++)
		fileName[i] = pidFileName.GetChar(i);

	FILE* fp = ::fopen(fileName, "wt");
	if (fp != NULL) {
		::fprintf(fp, "%u\n", ::getpid());
		::fclose(fp);
	}

	::signal(SIGUSR1, handler);

	m_repeater = new CDStarRepeaterD(nolog, logDir, confDir, audioDir, name);

	if (!m_repeater->init()) {
		::wxUninitialize();
		return 1;
	}

	m_repeater->run();

	delete m_repeater;

	::unlink(fileName);

	::wxUninitialize();

	return 0;
}

CDStarRepeaterD::CDStarRepeaterD(bool nolog, const wxString& logDir, const wxString& confDir, const wxString& audioDir, const wxString& name) :
m_name(name),
m_nolog(nolog),
m_logDir(logDir),
m_confDir(confDir),
m_audioDir(audioDir),
m_thread(NULL),
m_checker(NULL)
{
}

CDStarRepeaterD::~CDStarRepeaterD()
{
}

bool CDStarRepeaterD::init()
{
	if (!m_nolog) {
		wxString logBaseName = LOG_BASE_NAME;
		if (!m_name.IsEmpty()) {
			logBaseName.Append(wxT("_"));
			logBaseName.Append(m_name);
		}

		if (m_logDir.IsEmpty())
			m_logDir = wxT(LOG_DIR);

		wxLog* log = new CLogger(m_logDir, logBaseName);
		wxLog::SetActiveTarget(log);
	} else {
		new wxLogNull;
	}

	wxString appName;
	if (!m_name.IsEmpty())
		appName = APPLICATION_NAME + wxT(" ") + m_name;
	else
		appName = APPLICATION_NAME;
	appName.Replace(wxT(" "), wxT("_"));

	m_checker = new wxSingleInstanceChecker(appName, wxT("/tmp"));
	bool ret = m_checker->IsAnotherRunning();
	if (ret) {
		wxLogError(wxT("Another copy of the D-Star Repeater is running, exiting"));
		return false;
	}

	wxLogInfo(wxT("Starting ") + APPLICATION_NAME + wxT(" daemon - ") + VERSION);

	// Log the SVN revsion and the version of wxWidgets and the Operating System
	wxLogInfo(SVNREV);
	wxLogInfo(wxT("Using wxWidgets %d.%d.%d on %s"), wxMAJOR_VERSION, wxMINOR_VERSION, wxRELEASE_NUMBER, ::wxGetOsDescription().c_str());

	return createThread();
}

void CDStarRepeaterD::run()
{
	m_thread->run();

	delete m_checker;

	wxLogInfo(APPLICATION_NAME + wxT(" is exiting"));
}

bool CDStarRepeaterD::createThread()
{
	CDStarRepeaterConfig config(m_confDir, CONFIG_FILE_NAME, m_name);

	wxString callsign, gateway;
	DSTAR_MODE mode;
	ACK_TYPE ack;
	bool restriction, rpt1Validation, dtmfBlanking, errorReply;
	config.getCallsign(callsign, gateway, mode, ack, restriction, rpt1Validation, dtmfBlanking, errorReply);

	wxString modemType;
	config.getModem(modemType);

	// DVAP can only do simplex, force the mode accordingly
	if (modemType.IsSameAs(wxT("DVAP"))) {
		if (mode == MODE_DUPLEX) {
			wxLogInfo(wxT("DVAP: changing mode from DUPLEX to SIMPLEX"));
			mode = MODE_SIMPLEX;
		} else if (mode == MODE_TXANDRX) {
			wxLogInfo(wxT("DVAP: changing mode from TX_AND_RX to RX_ONLY"));
			mode = MODE_RXONLY;
		}
	}

	switch (mode) {
		case MODE_RXONLY:
			m_thread = new CDStarRepeaterRXThread(modemType);
			break;
		case MODE_TXONLY:
			m_thread = new CDStarRepeaterTXThread(modemType);
			break;
		case MODE_TXANDRX:
			m_thread = new CDStarRepeaterTXRXThread(modemType);
			break;
		default:
			m_thread = new CDStarRepeaterTRXThread(modemType);
			break;
	}

	m_thread->setCallsign(callsign, gateway, mode, ack, restriction, rpt1Validation, dtmfBlanking, errorReply);
	wxLogInfo(wxT("Callsign set to \"%s\", gateway set to \"%s\", mode: %d, ack: %d, restriction: %d, RPT1 validation: %d, DTMF blanking: %d, Error reply: %d"), callsign.c_str(), gateway.c_str(), int(mode), int(ack), int(restriction), int(rpt1Validation), int(dtmfBlanking), int(errorReply));

	wxString gatewayAddress, localAddress, name;
	unsigned int gatewayPort, localPort;
	config.getNetwork(gatewayAddress, gatewayPort, localAddress, localPort, name);
	wxLogInfo(wxT("Gateway set to %s:%u, local set to %s:%u, name set to \"%s\""), gatewayAddress.c_str(), gatewayPort, localAddress.c_str(), localPort, name.c_str());

	if (!gatewayAddress.IsEmpty()) {
		bool local = gatewayAddress.IsSameAs(wxT("127.0.0.1"));

		CRepeaterProtocolHandler* handler = new CRepeaterProtocolHandler(gatewayAddress, gatewayPort, localAddress, localPort, name);

		bool res = handler->open();
		if (!res) {
			wxLogError(wxT("Cannot open the protocol handler"));
			return false;
		}

		m_thread->setProtocolHandler(handler, local);
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

	wxLogInfo(wxT("Modem type set to \"%s\""), modemType.c_str());

	CModem* modem = NULL;
	if (modemType.IsSameAs(wxT("DVAP"))) {
		wxString port;
		unsigned int frequency;
		int power, squelch;
		config.getDVAP(port, frequency, power, squelch);
		wxLogInfo(wxT("DVAP: port: %s, frequency: %u Hz, power: %d dBm, squelch: %d dBm"), port.c_str(), frequency, power, squelch);
		modem = new CDVAPController(port, frequency, power, squelch);
	} else if (modemType.IsSameAs(wxT("DV-RPTR V1"))) {
		wxString port;
		bool rxInvert, txInvert, channel;
		unsigned int modLevel, txDelay;
		config.getDVRPTR1(port, rxInvert, txInvert, channel, modLevel, txDelay);
		wxLogInfo(wxT("DV-RPTR V1, port: %s, RX invert: %d, TX invert: %d, channel: %s, mod level: %u%%, TX delay: %u ms"), port.c_str(), int(rxInvert), int(txInvert), channel ? wxT("B") : wxT("A"), modLevel, txDelay);
		modem = new CDVRPTRV1Controller(port, wxEmptyString, rxInvert, txInvert, channel, modLevel, txDelay);
	} else if (modemType.IsSameAs(wxT("DV-RPTR V2"))) {
		CONNECTION_TYPE connType;
		wxString usbPort, address;
		bool txInvert;
		unsigned int port, modLevel, txDelay;
		config.getDVRPTR2(connType, usbPort, address, port, txInvert, modLevel, txDelay);
		wxLogInfo(wxT("DV-RPTR V2, type: %d, address: %s:%u, TX invert: %d, mod level: %u%%, TX delay: %u ms"), int(connType), address.c_str(), port, int(txInvert), modLevel, txDelay);
		switch (connType) {
			case CT_USB:
				modem = new CDVRPTRV2Controller(usbPort, wxEmptyString, txInvert, modLevel, mode == MODE_DUPLEX || mode == MODE_TXANDRX, callsign, txDelay);
				break;
			case CT_NETWORK:
				modem = new CDVRPTRV2Controller(address, port, txInvert, modLevel, mode == MODE_DUPLEX || mode == MODE_TXANDRX, callsign, txDelay);
				break;
		}
	} else if (modemType.IsSameAs(wxT("DV-RPTR V3"))) {
		CONNECTION_TYPE connType;
		wxString usbPort, address;
		bool txInvert;
		unsigned int port, modLevel, txDelay;
		config.getDVRPTR3(connType, usbPort, address, port, txInvert, modLevel, txDelay);
		wxLogInfo(wxT("DV-RPTR V3, type: %d, address: %s:%u, TX invert: %d, mod level: %u%%, TX delay: %u ms"), int(connType), address.c_str(), port, int(txInvert), modLevel, txDelay);
		switch (connType) {
			case CT_USB:
				modem = new CDVRPTRV3Controller(usbPort, wxEmptyString, txInvert, modLevel, mode == MODE_DUPLEX || mode == MODE_TXANDRX, callsign, txDelay);
				break;
			case CT_NETWORK:
				modem = new CDVRPTRV3Controller(address, port, txInvert, modLevel, mode == MODE_DUPLEX || mode == MODE_TXANDRX, callsign, txDelay);
				break;
		}
	} else if (modemType.IsSameAs(wxT("DVMEGA"))) {
		wxString port;
		DVMEGA_VARIANT variant;
		bool rxInvert, txInvert;
		unsigned int txDelay, frequency, power;
		config.getDVMEGA(port, variant, rxInvert, txInvert, txDelay, frequency, power);
		wxLogInfo(wxT("DVMEGA, port: %s, variant: %d, RX invert: %d, TX invert: %d, TX delay: %u ms, frequency: %u Hz, power: %u %%"), port.c_str(), int(variant), int(rxInvert), int(txInvert), txDelay, frequency, power);
		switch (variant) {
			case DVMV_NODE:
				modem = new CDVMegaController(port, wxEmptyString, rxInvert, txInvert, txDelay);
				break;
			case DVMV_RADIO:
				modem = new CDVMegaController(port, wxEmptyString, txDelay, frequency, power);
				break;
			default:
				wxLogError(wxT("Unknown DVMEGA variant - %d"), int(variant));
				break;
		}
	} else if (modemType.IsSameAs(wxT("GMSK Modem"))) {
		USB_INTERFACE iface;
		unsigned int address;
		config.getGMSK(iface, address);
		wxLogInfo(wxT("GMSK, interface: %d, address: %04X"), int(iface), address);
		modem = new CGMSKController(iface, address, mode == MODE_DUPLEX || mode == MODE_TXANDRX);
	} else if (modemType.IsSameAs(wxT("Sound Card"))) {
		wxString rxDevice, txDevice;
		bool rxInvert, txInvert;
		wxFloat32 rxLevel, txLevel;
		unsigned int txDelay, txTail;
		config.getSoundCard(rxDevice, txDevice, rxInvert, txInvert, rxLevel, txLevel, txDelay, txTail);
		wxLogInfo(wxT("Sound Card, devices: %s:%s, invert: %d:%d, levels: %.2f:%.2f, tx delay: %u ms, tx tail: %u ms"), rxDevice.c_str(), txDevice.c_str(), int(rxInvert), int(txInvert), rxLevel, txLevel, txDelay, txTail);
		modem = new CSoundCardController(rxDevice, txDevice, rxInvert, txInvert, rxLevel, txLevel, txDelay, txTail);
	} else if (modemType.IsSameAs(wxT("Split"))) {
		wxString localAddress;
		unsigned int localPort;
		wxArrayString transmitterNames, receiverNames;
		unsigned int timeout;
		config.getSplit(localAddress, localPort, transmitterNames, receiverNames, timeout);
		wxLogInfo(wxT("Split, local: %s:%u, timeout: %u ms"), localAddress.c_str(), localPort, timeout);
		for (unsigned int i = 0U; i < transmitterNames.GetCount(); i++) {
			wxString name = transmitterNames.Item(i);
			if (!name.IsEmpty())
				wxLogInfo(wxT("\tTX %u name: %s"), i + 1U, name.c_str());
		}
		for (unsigned int i = 0U; i < receiverNames.GetCount(); i++) {
			wxString name = receiverNames.Item(i);
			if (!name.IsEmpty())
				wxLogInfo(wxT("\tRX %u name: %s"), i + 1U, name.c_str());
		}
		modem = new CSplitController(localAddress, localPort, transmitterNames, receiverNames, timeout);
	} else {
		wxLogError(wxT("Unknown modem type: %s"), modemType.c_str());
	}

	if (modem != NULL) {
		bool res = modem->start();
		if (!res)
			wxLogError(wxT("Cannot open the D-Star modem"));
		else
			m_thread->setModem(modem);
	}

	wxString controllerType;
	unsigned int serialConfig, activeHangTime;
	bool pttInvert;
	config.getController(controllerType, serialConfig, pttInvert, activeHangTime);
	wxLogInfo(wxT("Controller set to %s, config: %u, PTT invert: %d, active hang time: %u ms"), controllerType.c_str(), serialConfig, int(pttInvert), activeHangTime);

	CExternalController* controller = NULL;

	wxString port;
	if (controllerType.StartsWith(wxT("Velleman K8055 - "), &port)) {
		unsigned long num;
		port.ToULong(&num);
		controller = new CExternalController(new CK8055Controller(num), pttInvert);
	} else if (controllerType.StartsWith(wxT("Serial - "), &port)) {
		controller = new CExternalController(new CSerialLineController(port, serialConfig), pttInvert);
	} else if (controllerType.StartsWith(wxT("Arduino - "), &port)) {
		controller = new CExternalController(new CArduinoController(port), pttInvert);
#if defined(RASPBERRY_PI)
	} else if (controllerType.IsSameAs(wxT("Raspberry Pi"))) {
		controller = new CExternalController(new CRaspberryController, pttInvert);
#endif
	} else {
		controller = new CExternalController(new CDummyController, pttInvert);
	}

	bool res = controller->open();
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
	wxString command5, command5Line, command6, command6Line;
	wxString output1, output2, output3, output4;
	config.getControl(enabled, rpt1Callsign, rpt2Callsign, shutdown, startup, status1, status2, status3, status4, status5, command1, command1Line, command2, command2Line, command3, command3Line, command4, command4Line, command5, command5Line, command6, command6Line, output1, output2, output3, output4);
	m_thread->setControl(enabled, rpt1Callsign, rpt2Callsign, shutdown, startup, status1, status2, status3, status4, status5, command1, command1Line, command2, command2Line, command3, command3Line, command4, command4Line, command5, command5Line, command6, command6Line, output1, output2, output3, output4);
	wxLogInfo(wxT("Control: enabled: %d, RPT1: %s, RPT2: %s, shutdown: %s, startup: %s, status1: %s, status2: %s, status3: %s, status4: %s, status5: %s, command1: %s = %s, command2: %s = %s, command3: %s = %s, command4: %s = %s, command5: %s = %s, command6: %s = %s, output1: %s, output2: %s, output3: %s, output4: %s"), enabled, rpt1Callsign.c_str(), rpt2Callsign.c_str(), shutdown.c_str(), startup.c_str(), status1.c_str(), status2.c_str(), status3.c_str(), status4.c_str(), status5.c_str(), command1.c_str(), command1Line.c_str(), command2.c_str(), command2Line.c_str(), command3.c_str(), command3Line.c_str(), command4.c_str(), command4Line.c_str(), command5.c_str(), command5Line.c_str(), command6.c_str(), command6Line.c_str(), output1.c_str(), output2.c_str(), output3.c_str(), output4.c_str());

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

void CDStarRepeaterD::kill()
{
	wxASSERT(m_thread != NULL);

	m_thread->kill();
}
