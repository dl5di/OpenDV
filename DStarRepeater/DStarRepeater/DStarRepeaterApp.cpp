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
#include "DStarRepeaterTXThread.h"
#include "DStarRepeaterRXThread.h"
#include "SerialLineController.h"
#include "DStarRepeaterLogger.h"
#include "DStarRepeaterThread.h"
#if defined(RASPBERRY_PI)
#include "RaspberryController.h"
#endif
#include "SoundCardController.h"
#include "DVRPTRV1Controller.h"
#include "DVRPTRV2Controller.h"
#include "DVRPTRV3Controller.h"
#include "ArduinoController.h"
#include "DVMegaController.h"
#include "DStarRepeaterApp.h"
#include "K8055Controller.h"
#include "DummyController.h"
#include "SplitController.h"
#include "DVAPController.h"
#include "GMSKController.h"
#include "DStarDefines.h"
#include "Version.h"
#include "Logger.h"

#include <wx/cmdline.h>
#include <wx/filename.h>

IMPLEMENT_APP(CDStarRepeaterApp)

const wxChar*       NAME_PARAM = wxT("Repeater Name");
const wxChar* NOLOGGING_SWITCH = wxT("nolog");
const wxChar*       GUI_SWITCH = wxT("gui");
const wxChar*    LOGDIR_OPTION = wxT("logdir");
const wxChar*   CONFDIR_OPTION = wxT("confdir");
const wxChar*  AUDIODIR_OPTION = wxT("audiodir");


CDStarRepeaterApp::CDStarRepeaterApp() :
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
m_checker(NULL),
m_logChain(NULL)
{
}

CDStarRepeaterApp::~CDStarRepeaterApp()
{
}

bool CDStarRepeaterApp::OnInit()
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
			m_logDir = ::wxGetHomeDir();
#else
		if (m_logDir.IsEmpty())
			m_logDir = wxT(LOG_DIR);
#endif

		wxLog* log = new CLogger(m_logDir, logBaseName);
		wxLog::SetActiveTarget(log);
	} else {
		new wxLogNull;
	}

	m_logChain = new wxLogChain(new CDStarRepeaterLogger);

	wxString appName;
	if (!m_name.IsEmpty())
		appName = APPLICATION_NAME + wxT(" ") + m_name;
	else
		appName = APPLICATION_NAME;

#if !defined(__WINDOWS__)
	appName.Replace(wxT(" "), wxT("_"));
	m_checker = new wxSingleInstanceChecker(appName, wxT("/tmp"));
#else
	m_checker = new wxSingleInstanceChecker(appName);
#endif

	bool ret = m_checker->IsAnotherRunning();
	if (ret) {
		wxLogError(wxT("Another copy of the D-Star Repeater is running, exiting"));
		return false;
	}

#if defined(__WINDOWS__)
	if (m_confDir.IsEmpty())
		m_confDir = ::wxGetHomeDir();

	m_config = new CDStarRepeaterConfig(new wxConfig(APPLICATION_NAME), m_confDir, CONFIG_FILE_NAME, m_name);
#else
	if (m_confDir.IsEmpty())
		m_confDir = wxT(CONF_DIR);

	m_config = new CDStarRepeaterConfig(m_confDir, CONFIG_FILE_NAME, m_name);
#endif

	wxString type;
	m_config->getModem(type);

	wxString frameName = APPLICATION_NAME + wxT(" (") + type + wxT(") - ");
	if (!m_name.IsEmpty()) {
		frameName.Append(m_name);
		frameName.Append(wxT(" - "));
	}
	frameName.Append(VERSION);

	wxPoint position = wxDefaultPosition;

	int x, y;
	m_config->getPosition(x, y);
	if (x >= 0 && y >= 0)
		position = wxPoint(x, y);

	m_frame = new CDStarRepeaterFrame(frameName, type, position, m_gui);
	m_frame->Show();

	SetTopWindow(m_frame);

	wxLogInfo(wxT("Starting ") + APPLICATION_NAME + wxT(" - ") + VERSION);

	// Log the SVN revsion and the version of wxWidgets and the Operating System
	wxLogInfo(SVNREV);
	wxLogInfo(wxT("Using wxWidgets %d.%d.%d on %s"), wxMAJOR_VERSION, wxMINOR_VERSION, wxRELEASE_NUMBER, ::wxGetOsDescription().c_str());

	createThread();

	return true;
}

int CDStarRepeaterApp::OnExit()
{
	m_logChain->SetLog(NULL);

	wxLogInfo(APPLICATION_NAME + wxT(" is exiting"));

	m_thread->kill();

	delete m_config;

	delete m_checker;

	return 0;
}

void CDStarRepeaterApp::OnInitCmdLine(wxCmdLineParser& parser)
{
	parser.AddSwitch(NOLOGGING_SWITCH, wxEmptyString, wxEmptyString, wxCMD_LINE_PARAM_OPTIONAL);
	parser.AddSwitch(GUI_SWITCH,       wxEmptyString, wxEmptyString, wxCMD_LINE_PARAM_OPTIONAL);
	parser.AddOption(LOGDIR_OPTION,    wxEmptyString, wxEmptyString, wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL);
	parser.AddOption(CONFDIR_OPTION,   wxEmptyString, wxEmptyString, wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL);
	parser.AddOption(AUDIODIR_OPTION,  wxEmptyString, wxEmptyString, wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL);
	parser.AddParam(NAME_PARAM, wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL);

	wxApp::OnInitCmdLine(parser);
}

bool CDStarRepeaterApp::OnCmdLineParsed(wxCmdLineParser& parser)
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
void CDStarRepeaterApp::OnAssertFailure(const wxChar* file, int line, const wxChar* func, const wxChar* cond, const wxChar* msg)
{
	wxLogFatalError(wxT("Assertion failed on line %d in file %s and function %s: %s %s"), line, file, func, cond, msg);
}
#endif

CDStarRepeaterStatusData* CDStarRepeaterApp::getStatus() const
{
	return m_thread->getStatus();
}

void CDStarRepeaterApp::showLog(const wxString& text)
{
	m_frame->showLog(text);
}

void CDStarRepeaterApp::setOutputs(bool out1, bool out2, bool out3, bool out4)
{
	wxLogInfo(wxT("Output 1 = %d, output 2 = %d, output 3 = %d, output 4 = %d"), int(out1), int(out2), int(out3), int(out4));

	m_thread->setOutputs(out1, out2, out3, out4);
}

void CDStarRepeaterApp::setLogging(bool logging)
{
	wxLogInfo(wxT("Frame logging set to %d, in %s"), int(logging), m_audioDir.c_str());

	m_thread->setLogging(logging, m_audioDir);
}

void CDStarRepeaterApp::setPosition(int x, int y)
{
	m_config->setPosition(x, y);
	m_config->write();
}

void CDStarRepeaterApp::shutdown()
{
	m_thread->shutdown();
}

void CDStarRepeaterApp::startup()
{
	m_thread->startup();
}

void CDStarRepeaterApp::command1()
{
	m_thread->command1();
}

void CDStarRepeaterApp::command2()
{
	m_thread->command2();
}

void CDStarRepeaterApp::command3()
{
	m_thread->command3();
}

void CDStarRepeaterApp::command4()
{
	m_thread->command4();
}

void CDStarRepeaterApp::command5()
{
	m_thread->command5();
}

void CDStarRepeaterApp::command6()
{
	m_thread->command6();
}

void CDStarRepeaterApp::createThread()
{
	wxASSERT(m_config != NULL);

	wxString callsign, gateway;
	DSTAR_MODE mode;
	ACK_TYPE ack;
	bool restriction, rpt1Validation, dtmfBlanking, errorReply;
	m_config->getCallsign(callsign, gateway, mode, ack, restriction, rpt1Validation, dtmfBlanking, errorReply);

	wxString modemType;
	m_config->getModem(modemType);

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

	IDStarRepeaterThread* thread = NULL;
	switch (mode) {
		case MODE_RXONLY:
			thread = new CDStarRepeaterRXThread(modemType);
			break;
		case MODE_TXONLY:
			thread = new CDStarRepeaterTXThread(modemType);
			break;
		case MODE_TXANDRX:
			thread = new CDStarRepeaterTXRXThread(modemType);
			break;
		default:
			thread = new CDStarRepeaterTRXThread(modemType);
			break;
	}

	thread->setCallsign(callsign, gateway, mode, ack, restriction, rpt1Validation, dtmfBlanking, errorReply);
	wxLogInfo(wxT("Callsign set to \"%s\", gateway set to \"%s\", mode: %d, ack: %d, restriction: %d, RPT1 validation: %d, DTMF blanking: %d, Error reply: %d"), callsign.c_str(), gateway.c_str(), int(mode), int(ack), int(restriction), int(rpt1Validation), int(dtmfBlanking), int(errorReply));

	wxString gatewayAddress, localAddress, name;
	unsigned int gatewayPort, localPort;
	m_config->getNetwork(gatewayAddress, gatewayPort, localAddress, localPort, name);
	wxLogInfo(wxT("Gateway set to %s:%u, local set to %s:%u, name set to \"%s\""), gatewayAddress.c_str(), gatewayPort, localAddress.c_str(), localPort, name.c_str());

	if (!gatewayAddress.IsEmpty()) {
		bool local = gatewayAddress.IsSameAs(wxT("127.0.0.1"));

		CRepeaterProtocolHandler* handler = new CRepeaterProtocolHandler(gatewayAddress, gatewayPort, localAddress, localPort, name);

		bool res = handler->open();
		if (!res)
			wxLogError(wxT("Cannot open the protocol handler"));
		else
			thread->setProtocolHandler(handler, local);
	}

	unsigned int timeout, ackTime;
	m_config->getTimes(timeout, ackTime);
	thread->setTimes(timeout, ackTime);
	wxLogInfo(wxT("Timeout set to %u secs, ack time set to %u ms"), timeout, ackTime);

	unsigned int beaconTime;
	wxString beaconText;
	bool beaconVoice;
	TEXT_LANG language;
	m_config->getBeacon(beaconTime, beaconText, beaconVoice, language);
	if (mode == MODE_GATEWAY)
		beaconTime = 0U;
	thread->setBeacon(beaconTime, beaconText, beaconVoice, language);
	wxLogInfo(wxT("Beacon set to %u mins, text set to \"%s\", voice set to %d, language set to %d"), beaconTime / 60U, beaconText.c_str(), int(beaconVoice), int(language));

	bool announcementEnabled;
	unsigned int announcementTime;
	wxString announcementRecordRPT1, announcementRecordRPT2;
	wxString announcementDeleteRPT1, announcementDeleteRPT2;
	m_config->getAnnouncement(announcementEnabled, announcementTime, announcementRecordRPT1, announcementRecordRPT2, announcementDeleteRPT1, announcementDeleteRPT2);
	if (mode == MODE_GATEWAY)
		announcementEnabled = false;
	thread->setAnnouncement(announcementEnabled, announcementTime, announcementRecordRPT1, announcementRecordRPT2, announcementDeleteRPT1, announcementDeleteRPT2);
	wxLogInfo(wxT("Announcement enabled: %d, time: %u mins, record RPT1: \"%s\", record RPT2: \"%s\", delete RPT1: \"%s\", delete RPT2: \"%s\""), int(announcementEnabled), announcementTime / 60U, announcementRecordRPT1.c_str(), announcementRecordRPT2.c_str(), announcementDeleteRPT1.c_str(), announcementDeleteRPT2.c_str());

	wxLogInfo(wxT("Modem type set to \"%s\""), modemType.c_str());

	CModem* modem = NULL;
	if (modemType.IsSameAs(wxT("DVAP"))) {
		wxString port;
		unsigned int frequency;
		int power, squelch;
		m_config->getDVAP(port, frequency, power, squelch);
		wxLogInfo(wxT("DVAP: port: %s, frequency: %u Hz, power: %d dBm, squelch: %d dBm"), port.c_str(), frequency, power, squelch);
		modem = new CDVAPController(port, frequency, power, squelch);
	} else if (modemType.IsSameAs(wxT("DV-RPTR V1"))) {
		wxString port;
		bool rxInvert, txInvert, channel;
		unsigned int modLevel, txDelay;
		m_config->getDVRPTR1(port, rxInvert, txInvert, channel, modLevel, txDelay);
		wxLogInfo(wxT("DV-RPTR V1, port: %s, RX invert: %d, TX invert: %d, channel: %s, mod level: %u%%, TX delay: %u ms"), port.c_str(), int(rxInvert), int(txInvert), channel ? wxT("B") : wxT("A"), modLevel, txDelay);
		modem = new CDVRPTRV1Controller(port, wxEmptyString, rxInvert, txInvert, channel, modLevel, txDelay);
	} else if (modemType.IsSameAs(wxT("DV-RPTR V2"))) {
		CONNECTION_TYPE connType;
		wxString usbPort, address;
		bool txInvert;
		unsigned int port, modLevel, txDelay;
		m_config->getDVRPTR2(connType, usbPort, address, port, txInvert, modLevel, txDelay);
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
		m_config->getDVRPTR3(connType, usbPort, address, port, txInvert, modLevel, txDelay);
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
		m_config->getDVMEGA(port, variant, rxInvert, txInvert, txDelay, frequency, power);
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
		m_config->getGMSK(iface, address);
		wxLogInfo(wxT("GMSK, interface: %d, address: %04X"), int(iface), address);
		modem = new CGMSKController(iface, address, mode == MODE_DUPLEX || mode == MODE_TXANDRX);
	} else if (modemType.IsSameAs(wxT("Sound Card"))) {
		wxString rxDevice, txDevice;
		bool rxInvert, txInvert;
		wxFloat32 rxLevel, txLevel;
		unsigned int txDelay, txTail;
		m_config->getSoundCard(rxDevice, txDevice, rxInvert, txInvert, rxLevel, txLevel, txDelay, txTail);
		wxLogInfo(wxT("Sound Card, devices: %s:%s, invert: %d:%d, levels: %.2f:%.2f, tx delay: %u ms, tx tail: %u ms"), rxDevice.c_str(), txDevice.c_str(), int(rxInvert), int(txInvert), rxLevel, txLevel, txDelay, txTail);
		modem = new CSoundCardController(rxDevice, txDevice, rxInvert, txInvert, rxLevel, txLevel, txDelay, txTail);
	} else if (modemType.IsSameAs(wxT("Split"))) {
		wxString localAddress;
		unsigned int localPort;
		wxArrayString transmitterNames, receiverNames;
		unsigned int timeout;
		m_config->getSplit(localAddress, localPort, transmitterNames, receiverNames, timeout);
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
			thread->setModem(modem);
	}

	wxString controllerType;
	unsigned int serialConfig, activeHangTime;
	bool pttInvert;
	m_config->getController(controllerType, serialConfig, pttInvert, activeHangTime);
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
		thread->setController(controller, activeHangTime);

	bool out1, out2, out3, out4;
	m_config->getOutputs(out1, out2, out3, out4);
	thread->setOutputs(out1, out2, out3, out4);
	m_frame->setOutputs(out1, out2, out3, out4);
	wxLogInfo(wxT("Output 1 = %d, output 2 = %d, output 3 = %d, output 4 = %d"), int(out1), int(out2), int(out3), int(out4));

	bool enabled;
	wxString rpt1Callsign, rpt2Callsign;
	wxString shutdown, startup;
	wxString status1, status2, status3, status4, status5;
	wxString command1, command1Line, command2, command2Line;
	wxString command3, command3Line, command4, command4Line;
	wxString command5, command5Line, command6, command6Line;
	wxString output1, output2, output3, output4;
	m_config->getControl(enabled, rpt1Callsign, rpt2Callsign, shutdown, startup, status1, status2, status3, status4, status5, command1, command1Line, command2, command2Line, command3, command3Line, command4, command4Line, command5, command5Line, command6, command6Line, output1, output2, output3, output4);
	thread->setControl(enabled, rpt1Callsign, rpt2Callsign, shutdown, startup, status1, status2, status3, status4, status5, command1, command1Line, command2, command2Line, command3, command3Line, command4, command4Line, command5, command5Line, command6, command6Line, output1, output2, output3, output4);
	wxLogInfo(wxT("Control: enabled: %d, RPT1: %s, RPT2: %s, shutdown: %s, startup: %s, status1: %s, status2: %s, status3: %s, status4: %s, status5: %s, command1: %s = %s, command2: %s = %s, command3: %s = %s, command4: %s = %s, command5: %s = %s, command6: %s = %s, output1: %s, output2: %s, output3: %s, output4: %s"), enabled, rpt1Callsign.c_str(), rpt2Callsign.c_str(), shutdown.c_str(), startup.c_str(), status1.c_str(), status2.c_str(), status3.c_str(), status4.c_str(), status5.c_str(), command1.c_str(), command1Line.c_str(), command2.c_str(), command2Line.c_str(), command3.c_str(), command3Line.c_str(), command4.c_str(), command4Line.c_str(), command5.c_str(), command5Line.c_str(), command6.c_str(), command6Line.c_str(), output1.c_str(), output2.c_str(), output3.c_str(), output4.c_str());

	bool logging;
	m_config->getLogging(logging);
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
	m_thread = new CDStarRepeaterThreadHelper(thread);
	m_thread->start();
}
