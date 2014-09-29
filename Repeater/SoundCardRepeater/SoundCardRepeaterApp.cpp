/*
 *   Copyright (C) 2009-2013 by Jonathan Naylor G4KLX
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

#include "SoundCardRepeaterTXRXThread.h"
#include "SoundCardRepeaterTRXThread.h"
#include "SoundCardRepeaterTXThread.h"
#include "SoundCardRepeaterRXThread.h"
#include "SoundCardRepeaterLogger.h"
#include "RepeaterProtocolHandler.h"
#include "SoundCardRepeaterThread.h"
#include "SoundCardRepeaterApp.h"
#include "SerialLineController.h"
#if defined(RASPBERRY_PI)
#include "RaspberryController.h"
#endif
#include "ArduinoController.h"
#include "URIUSBController.h"
#include "K8055Controller.h"
#include "DummyController.h"
#include "DStarDefines.h"
#include "Version.h"
#include "Logger.h"

#include <wx/cmdline.h>
#include <wx/filename.h>

IMPLEMENT_APP(CSoundCardRepeaterApp)

const wxChar*       NAME_PARAM = wxT("Repeater Name");
const wxChar* NOLOGGING_SWITCH = wxT("nolog");
const wxChar*       GUI_SWITCH = wxT("gui");
const wxChar*    LOGDIR_OPTION = wxT("logdir");
const wxChar*   CONFDIR_OPTION = wxT("confdir");
const wxChar*  AUDIODIR_OPTION = wxT("audiodir");


CSoundCardRepeaterApp::CSoundCardRepeaterApp() :
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

CSoundCardRepeaterApp::~CSoundCardRepeaterApp()
{
}

bool CSoundCardRepeaterApp::OnInit()
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

	m_logChain = new wxLogChain(new CSoundCardRepeaterLogger);

#if defined(__WINDOWS__)
	m_config = new CSoundCardRepeaterConfig(new wxConfig(APPLICATION_NAME), m_name);
#else
	if (m_confDir.IsEmpty())
		m_confDir = CONF_DIR;

	m_config = new CSoundCardRepeaterConfig(m_confDir, m_name);
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

	m_frame = new CSoundCardRepeaterFrame(frameName, position, m_gui);
	m_frame->Show();

	SetTopWindow(m_frame);

	wxLogInfo(wxT("Starting ") + APPLICATION_NAME + wxT(" - ") + VERSION);

	// Log the SVN revsion and the version of wxWidgets and the Operating System
	wxLogInfo(SVNREV);
	wxLogInfo(wxT("Using wxWidgets %d.%d.%d on %s"), wxMAJOR_VERSION, wxMINOR_VERSION, wxRELEASE_NUMBER, ::wxGetOsDescription().c_str());

	createThread();

	return true;
}

int CSoundCardRepeaterApp::OnExit()
{
	m_logChain->SetLog(NULL);

	wxLogInfo(APPLICATION_NAME + wxT(" is exiting"));

	m_thread->kill();

	delete m_config;

	return 0;
}

void CSoundCardRepeaterApp::OnInitCmdLine(wxCmdLineParser& parser)
{
	parser.AddSwitch(NOLOGGING_SWITCH, wxEmptyString, wxEmptyString, wxCMD_LINE_PARAM_OPTIONAL);
	parser.AddSwitch(GUI_SWITCH,       wxEmptyString, wxEmptyString, wxCMD_LINE_PARAM_OPTIONAL);
	parser.AddOption(LOGDIR_OPTION,    wxEmptyString, wxEmptyString, wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL);
	parser.AddOption(CONFDIR_OPTION,   wxEmptyString, wxEmptyString, wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL);
	parser.AddOption(AUDIODIR_OPTION,  wxEmptyString, wxEmptyString, wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL);
	parser.AddParam(NAME_PARAM, wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL);

	wxApp::OnInitCmdLine(parser);
}

bool CSoundCardRepeaterApp::OnCmdLineParsed(wxCmdLineParser& parser)
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
void CSoundCardRepeaterApp::OnAssertFailure(const wxChar* file, int line, const wxChar* func, const wxChar* cond, const wxChar* msg)
{
	wxLogFatalError(wxT("Assertion failed on line %d in file %s and function %s: %s %s"), line, file, func, cond, msg);
}
#endif

void CSoundCardRepeaterApp::setReader(CWAVFileReader* reader)
{
	return m_thread->setReader(reader);
}

void CSoundCardRepeaterApp::setRXLevel(wxFloat32 level)
{
	m_thread->setRXLevel(level);
}

void CSoundCardRepeaterApp::setTXLevel(wxFloat32 level)
{
	m_thread->setTXLevel(level);
}

void CSoundCardRepeaterApp::setSquelchLevel(wxFloat32 level)
{
	m_thread->setSquelchLevel(level);
}

CSoundCardRepeaterStatusData* CSoundCardRepeaterApp::getStatus() const
{
	return m_thread->getStatus();
}

void CSoundCardRepeaterApp::showLog(const wxString& text)
{
	m_frame->showLog(text);
}

void CSoundCardRepeaterApp::getCallsign(wxString& callsign, wxString& gateway, DSTAR_MODE& mode, ACK_TYPE& ack, bool& restriction, bool& rpt1Validation, bool& dtmfBlanking) const
{
	m_config->getCallsign(callsign, gateway, mode, ack, restriction, rpt1Validation, dtmfBlanking);
}

void CSoundCardRepeaterApp::setCallsign(const wxString& callsign, const wxString& gateway, DSTAR_MODE mode, ACK_TYPE ack, bool restriction, bool rpt1Validation, bool dtmfBlanking)
{
	m_config->setCallsign(callsign, gateway, mode, ack, restriction, rpt1Validation, dtmfBlanking);
}

void CSoundCardRepeaterApp::getNetwork(wxString& gatewayAddress, unsigned int& gatewayPort, wxString& localAddress, unsigned int& localPort) const
{
	m_config->getNetwork(gatewayAddress, gatewayPort, localAddress, localPort);
}

void CSoundCardRepeaterApp::setNetwork(const wxString& gatewayAddress, unsigned int gatewayPort, const wxString& localAddress, unsigned int localPort)
{
	m_config->setNetwork(gatewayAddress, gatewayPort, localAddress, localPort);
}

void CSoundCardRepeaterApp::getRadio(wxString& readDevice, wxString& writeDevice, wxFloat32& rxLevel, wxFloat32& txLevel, SQUELCH_MODE& squelchMode, wxFloat32& squelchLevel, bool& rxInvert, bool& txInvert) const
{
	m_config->getRadio(readDevice, writeDevice, rxLevel, txLevel, squelchMode, squelchLevel, rxInvert, txInvert);
}

void CSoundCardRepeaterApp::setRadio(const wxString& readDevice, const wxString& writeDevice, wxFloat32 rxLevel, wxFloat32 txLevel, SQUELCH_MODE squelchMode, wxFloat32 squelchLevel, bool rxInvert, bool txInvert)
{
	m_config->setRadio(readDevice, writeDevice, rxLevel, txLevel, squelchMode, squelchLevel, rxInvert, txInvert);
}

void CSoundCardRepeaterApp::getController(wxString& type, unsigned int& config, int& pttDelay, bool& pttInvert) const
{
	m_config->getController(type, config, pttDelay, pttInvert);
}

void CSoundCardRepeaterApp::setController(const wxString& type, unsigned int config, int pttDelay, bool pttInvert)
{
	m_config->setController(type, config, pttDelay, pttInvert);
}

void CSoundCardRepeaterApp::getTimes(unsigned int& timeout, unsigned int& ackTime, unsigned int& hangTime) const
{
	m_config->getTimes(timeout, ackTime, hangTime);
}

void CSoundCardRepeaterApp::setTimes(unsigned int timeout, unsigned int ackTime, unsigned int hangTime)
{
	m_config->setTimes(timeout, ackTime, hangTime);
}

void CSoundCardRepeaterApp::getBeacon(unsigned int& time, wxString& text, bool& voice, TEXT_LANG& language) const
{
	m_config->getBeacon(time, text, voice, language);
}

void CSoundCardRepeaterApp::setBeacon(unsigned int time, const wxString& text, bool voice, TEXT_LANG language)
{
	m_config->setBeacon(time, text, voice, language);
}

void CSoundCardRepeaterApp::getAnnouncement(bool& enabled, unsigned int& time, wxString& recordRPT1, wxString& recordRPT2, wxString& deleteRPT1, wxString& deleteRPT2) const
{
	m_config->getAnnouncement(enabled, time, recordRPT1, recordRPT2, deleteRPT1, deleteRPT2);
}

void CSoundCardRepeaterApp::setAnnouncement(bool enabled, unsigned int time, const wxString& recordRPT1, const wxString& recordRPT2, const wxString& deleteRPT1, const wxString& deleteRPT2)
{
	m_config->setAnnouncement(enabled, time, recordRPT1, recordRPT2, deleteRPT1, deleteRPT2);
}

void CSoundCardRepeaterApp::getControl(bool& enabled, wxString& rpt1Callsign, wxString& rpt2Callsign, wxString& shutdown, wxString& startup, wxString& status1, wxString& status2, wxString& status3, wxString& status4, wxString& status5, wxString& command1, wxString& command1Line, wxString& command2, wxString& command2Line, wxString& command3, wxString& command3Line, wxString& command4, wxString& command4Line, wxString& output1, wxString& output2, wxString& output3, wxString& output4) const
{
	m_config->getControl(enabled, rpt1Callsign, rpt2Callsign, shutdown, startup, status1, status2, status3, status4, status5, command1, command1Line, command2, command2Line, command3, command3Line, command4, command4Line, output1, output2, output3, output4);
}

void CSoundCardRepeaterApp::setControl(bool enabled, const wxString& rpt1Callsign, const wxString& rpt2Callsign, const wxString& shutdown, const wxString& startup, const wxString& status1, const wxString& status2, const wxString& status3, const wxString& status4, const wxString& status5, const wxString& command1, const wxString& command1Line, const wxString& command2, const wxString& command2Line, const wxString& command3, const wxString& command3Line, const wxString& command4, const wxString& command4Line, const wxString& output1, const wxString& output2, const wxString& output3, const wxString& output4)
{
	m_config->setControl(enabled, rpt1Callsign, rpt2Callsign, shutdown, startup, status1, status2, status3, status4, status5, command1, command1Line, command2, command2Line, command3, command3Line, command4, command4Line, output1, output2, output3, output4);
}

void CSoundCardRepeaterApp::getActiveHang(unsigned int& time) const
{
	m_config->getActiveHang(time);
}

void CSoundCardRepeaterApp::setActiveHang(unsigned int time)
{
	m_config->setActiveHang(time);
}

void CSoundCardRepeaterApp::getOutputs(bool& out1, bool& out2, bool& out3, bool& out4) const
{
	m_config->getOutputs(out1, out2, out3, out4);
}

void CSoundCardRepeaterApp::setOutputs(bool out1, bool out2, bool out3, bool out4)
{
	m_config->setOutputs(out1, out2, out3, out4);
	m_thread->setOutputs(out1, out2, out3, out4);

	wxLogInfo(wxT("Output 1 = %d, output 2 = %d, output 3 = %d, output 4 = %d"), out1, out2, out3, out4);
}

void CSoundCardRepeaterApp::getLogging(bool& logging) const
{
	m_config->getLogging(logging);
}

void CSoundCardRepeaterApp::setLogging(bool logging)
{
	wxLogInfo(wxT("Frame logging set to %d, in %s"), int(logging), m_audioDir.c_str());

	m_config->setLogging(logging);
	m_thread->setLogging(logging, m_audioDir);
}

void CSoundCardRepeaterApp::getPosition(int& x, int& y) const
{
	m_config->getPosition(x, y);
}

void CSoundCardRepeaterApp::setPosition(int x, int y)
{
	m_config->setPosition(x, y);
}

bool CSoundCardRepeaterApp::writeConfig()
{
	return m_config->write();
}

void CSoundCardRepeaterApp::shutdown()
{
	m_thread->shutdown();
}

void CSoundCardRepeaterApp::startup()
{
	m_thread->startup();
}

void CSoundCardRepeaterApp::command1()
{
	m_thread->command1();
}

void CSoundCardRepeaterApp::command2()
{
	m_thread->command2();
}

void CSoundCardRepeaterApp::command3()
{
	m_thread->command3();
}

void CSoundCardRepeaterApp::command4()
{
	m_thread->command4();
}

void CSoundCardRepeaterApp::createThread()
{
	wxString callsign, gateway;
	DSTAR_MODE mode;
	ACK_TYPE ack;
	bool restriction, rpt1Validation, dtmfBlanking;
	getCallsign(callsign, gateway, mode, ack, restriction, rpt1Validation, dtmfBlanking);

	ISoundCardRepeaterThread* thread = NULL;
	switch (mode) {
		case MODE_RXONLY:
			thread = new CSoundCardRepeaterRXThread;
			break;
		case MODE_TXONLY:
			thread = new CSoundCardRepeaterTXThread;
			break;
		case MODE_TXANDRX:
			thread = new CSoundCardRepeaterTXRXThread;
			break;
		default:
			thread = new CSoundCardRepeaterTRXThread;
			break;
	}

	thread->setCallsign(callsign, gateway, mode, ack, restriction, rpt1Validation, dtmfBlanking);
	wxLogInfo(wxT("Callsign set to \"%s\", gateway set to \"%s\", mode: %d, ack: %d, restriction: %d, RPT1 validation: %d, DTMF blanking: %d"), callsign.c_str(), gateway.c_str(), int(mode), int(ack), int(restriction), int(rpt1Validation), int(dtmfBlanking));

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

	unsigned int timeout, ackTime, hangTime;
	getTimes(timeout, ackTime, hangTime);
	thread->setTimes(timeout, ackTime, hangTime);
	wxLogInfo(wxT("Timeout set to %u secs, Ack time set to %u ms, Hang time set to %u ms"), timeout, ackTime, hangTime);

	unsigned int beaconTime;
	wxString beaconText;
	bool beaconVoice;
	TEXT_LANG language;
	getBeacon(beaconTime, beaconText, beaconVoice, language);
	if (mode == MODE_GATEWAY)
		beaconTime = 0U;
	thread->setBeacon(beaconTime, beaconText, beaconVoice, language);
	wxLogInfo(wxT("Beacon set to %u mins, text set to \"%s\", voice set to %d, language set to %d"), beaconTime / 60U, beaconText.c_str(), int(beaconVoice), int(language));

	bool announcementEnabled;
	unsigned int announcementTime;
	wxString announcementRecordRPT1, announcementRecordRPT2;
	wxString announcementDeleteRPT1, announcementDeleteRPT2;
	getAnnouncement(announcementEnabled, announcementTime, announcementRecordRPT1, announcementRecordRPT2, announcementDeleteRPT1, announcementDeleteRPT2);
	if (mode == MODE_GATEWAY)
		announcementEnabled = false;
	thread->setAnnouncement(announcementEnabled, announcementTime, announcementRecordRPT1, announcementRecordRPT2, announcementDeleteRPT1, announcementDeleteRPT2);
	wxLogInfo(wxT("Announcement enabled: %d, time: %u mins, record RPT1: \"%s\", record RPT2: \"%s\", delete RPT1: \"%s\", delete RPT2: \"%s\""), int(announcementEnabled), announcementTime / 60U, announcementRecordRPT1.c_str(), announcementRecordRPT2.c_str(), announcementDeleteRPT1.c_str(), announcementDeleteRPT2.c_str());

	wxString readDevice, writeDevice;
	bool rxInvert, txInvert;
	wxFloat32 rxLevel, txLevel, squelchLevel;
	SQUELCH_MODE squelchMode;
	getRadio(readDevice, writeDevice, rxLevel, txLevel, squelchMode, squelchLevel, rxInvert, txInvert);
	wxLogInfo(wxT("Soundcard set to %s:%s, levels: %.2f:%.2f, GMSK Inversion set to %d:%d, squelch: mode: %d level: %.2f"), readDevice.c_str(), writeDevice.c_str(), rxLevel, txLevel, rxInvert, txInvert, int(squelchMode), squelchLevel);

	if (!readDevice.IsEmpty() && !writeDevice.IsEmpty()) {
#if defined(__WINDOWS__)
		CSoundCardReaderWriter* soundcard = new CSoundCardReaderWriter(readDevice, writeDevice, DSTAR_RADIO_SAMPLE_RATE, DSTAR_RADIO_BLOCK_SIZE);
#else
		CSoundCardReaderWriter* soundcard = new CSoundCardReaderWriter(readDevice, writeDevice, DSTAR_RADIO_SAMPLE_RATE, 64U);
#endif
		soundcard->setCallback(thread, 0U);

		bool res = soundcard->open();
		if (!res)
			wxLogError(wxT("Cannot open the sound card"));
		else
			thread->setSoundCard(soundcard, rxLevel, txLevel, squelchMode, squelchLevel, rxInvert, txInvert);
	}

	wxString type;
	unsigned int cfg;
	int pttDelay;
	bool pttInvert;
	getController(type, cfg, pttDelay, pttInvert);
	wxLogInfo(wxT("Controller set to %s, config: %u, ptt delay: %d ms, PTT Inversion set to %d"), type.c_str(), cfg, pttDelay * 20, pttInvert);

	CExternalController* controller = NULL;

	wxString port;
	if (type.StartsWith(wxT("Velleman K8055 - "), &port)) {
		unsigned long num;
		port.ToULong(&num);
		controller = new CExternalController(new CK8055Controller(num), pttInvert, false);
	} else if (type.StartsWith(wxT("URI USB - "), &port)) {
		unsigned long num;
		port.ToULong(&num);
		controller = new CExternalController(new CURIUSBController(num, false), pttInvert, false);
	} else if (type.StartsWith(wxT("Serial - "), &port)) {
		controller = new CExternalController(new CSerialLineController(port, cfg), pttInvert, false);
	} else if (type.StartsWith(wxT("Arduino - "), &port)) {
		controller = new CExternalController(new CArduinoController(port), pttInvert, false);
#if defined(RASPBERRY_PI)
	} else if (type.IsSameAs(wxT("Raspberry Pi"))) {
		controller = new CExternalController(new CRaspberryController, pttInvert, false);
#endif
	} else {
		controller = new CExternalController(new CDummyController, pttInvert, false);
	}

	bool res = controller->open();
	if (!res)
		wxLogError(wxT("Cannot open the hardware interface - %s"), type.c_str());
	else
		thread->setController(controller, pttDelay);

	bool out1, out2, out3, out4;
	getOutputs(out1, out2, out3, out4);
	thread->setOutputs(out1, out2, out3, out4);
	m_frame->setOutputs(out1, out2, out3, out4);
	wxLogInfo(wxT("Output 1 = %d, output 2 = %d, output 3 = %d, output 4 = %d"), out1, out2, out3, out4);

	bool enabled;
	wxString rpt1Callsign, rpt2Callsign;
	wxString shutdown, startup;
	wxString status1, status2, status3, status4, status5;
	wxString command1, command1Line, command2, command2Line;
	wxString command3, command3Line, command4, command4Line;
	wxString output1, output2, output3, output4;
	getControl(enabled, rpt1Callsign, rpt2Callsign, shutdown, startup, status1, status2, status3, status4, status5, command1, command1Line, command2, command2Line, command3, command3Line, command4, command4Line, output1, output2, output3, output4);
	thread->setControl(enabled, rpt1Callsign, rpt2Callsign, shutdown, startup, status1, status2, status3, status4, status5, command1, command1Line, command2, command2Line, command3, command3Line, command4, command4Line, output1, output2, output3, output4);
	wxLogInfo(wxT("Control: enabled: %d, RPT1: %s, RPT2: %s, shutdown: %s, startup: %s, status1: %s, status2: %s, status3: %s, status4: %s, status5: %s, command1: %s = %s, command2: %s = %s, command3: %s = %s, command4: %s = %s, output1: %s, output2: %s, output3: %s, output4: %s"), enabled, rpt1Callsign.c_str(), rpt2Callsign.c_str(), shutdown.c_str(), startup.c_str(), status1.c_str(), status2.c_str(), status3.c_str(), status4.c_str(), status5.c_str(), command1.c_str(), command1Line.c_str(), command2.c_str(), command2Line.c_str(), command1.c_str(), command1Line.c_str(), command2.c_str(), command2Line.c_str(), command3.c_str(), command3Line.c_str(), command4.c_str(), command4Line.c_str(), output1.c_str(), output2.c_str(), output3.c_str(), output4.c_str());

	unsigned int activeHangTime;
	getActiveHang(activeHangTime);
	thread->setActiveHang(activeHangTime);
	wxLogInfo(wxT("Active Hang: time: %u"), activeHangTime);

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
	m_thread = new CSoundCardRepeaterThreadHelper(thread);
	m_thread->start();
}
