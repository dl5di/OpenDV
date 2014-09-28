/*
 *   Copyright (C) 2009-2014 by Jonathan Naylor G4KLX
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

#include "AnalogueRepeaterThread.h"
#include "AnalogueRepeaterLogger.h"
#include "AnalogueRepeaterApp.h"
#if defined(RASPBERRY_PI)
#include "RaspberryController.h"
#endif
#include "SerialLineController.h"
#include "ExternalController.h"
#include "ArduinoController.h"
#include "DummyController.h"
#include "K8055Controller.h"
#include "WAVFileStore.h"
#include "CWKeyer.h"
#include "Version.h"
#include "Logger.h"
#include "APRSTX.h"

#include <wx/cmdline.h>
#include <wx/filename.h>

IMPLEMENT_APP(CAnalogueRepeaterApp)

const wxChar*       NAME_PARAM = wxT("Repeater Name");
const wxChar* NOLOGGING_SWITCH = wxT("nolog");
const wxChar*       GUI_SWITCH = wxT("gui");
const wxChar*    LOGDIR_OPTION = wxT("logdir");
const wxChar*   CONFDIR_OPTION = wxT("confdir");


CAnalogueRepeaterApp::CAnalogueRepeaterApp() :
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

CAnalogueRepeaterApp::~CAnalogueRepeaterApp()
{
}

bool CAnalogueRepeaterApp::OnInit()
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

	m_logChain = new wxLogChain(new CAnalogueRepeaterLogger);

#if defined(__WINDOWS__)
	m_config = new CAnalogueRepeaterConfig(new wxConfig(APPLICATION_NAME), m_name);
#else
	if (m_confDir.IsEmpty())
		m_confDir = CONF_DIR;

	m_config = new CAnalogueRepeaterConfig(m_confDir, m_name);
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

	m_frame = new CAnalogueRepeaterFrame(frameName, position, m_gui);
	m_frame->Show();

	SetTopWindow(m_frame);

	wxLogInfo(wxT("Starting ") + APPLICATION_NAME + wxT(" - ") + VERSION);

	// Log the SVN revsion and the version of wxWidgets and the Operating System
	wxLogInfo(SVNREV);
	wxLogInfo(wxT("Using wxWidgets %d.%d.%d on %s"), wxMAJOR_VERSION, wxMINOR_VERSION, wxRELEASE_NUMBER, ::wxGetOsDescription().c_str());

	createThread();

	return true;
}

int CAnalogueRepeaterApp::OnExit()
{
	m_logChain->SetLog(NULL);

	wxLogInfo(APPLICATION_NAME + wxT(" is exiting"));

	m_thread->kill();

	delete m_config;

	return 0;
}

void CAnalogueRepeaterApp::OnInitCmdLine(wxCmdLineParser& parser)
{
	parser.AddSwitch(NOLOGGING_SWITCH, wxEmptyString, wxEmptyString, wxCMD_LINE_PARAM_OPTIONAL);
	parser.AddSwitch(GUI_SWITCH,       wxEmptyString, wxEmptyString, wxCMD_LINE_PARAM_OPTIONAL);
	parser.AddOption(LOGDIR_OPTION,    wxEmptyString, wxEmptyString, wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL);
	parser.AddOption(CONFDIR_OPTION,   wxEmptyString, wxEmptyString, wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL);
	parser.AddParam(NAME_PARAM, wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL);

	wxApp::OnInitCmdLine(parser);
}

bool CAnalogueRepeaterApp::OnCmdLineParsed(wxCmdLineParser& parser)
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
void CAnalogueRepeaterApp::OnAssertFailure(const wxChar* file, int line, const wxChar* func, const wxChar* cond, const wxChar* msg)
{
	wxLogFatalError(wxT("Assertion failed on line %d in file %s and function %s: %s %s"), line, file, func, cond, msg);
}

void CAnalogueRepeaterApp::setReader(CWAVFileReader* reader)
{
	return m_thread->setReader(reader);
}
#endif

void CAnalogueRepeaterApp::setTbThreshold(wxFloat32 threshold)
{
	m_thread->setTbThreshold(threshold);
}

void CAnalogueRepeaterApp::setCTCSSThreshold(wxFloat32 threshold)
{
	m_thread->setCTCSSThreshold(threshold);
}

void CAnalogueRepeaterApp::setDTMFThreshold(wxFloat32 threshold)
{
	m_thread->setDTMFThreshold(threshold);
}

void CAnalogueRepeaterApp::setId1Level(wxFloat32 level)
{
	m_thread->setId1Level(level);
}

void CAnalogueRepeaterApp::setId2Level(wxFloat32 level)
{
	m_thread->setId2Level(level);
}

void CAnalogueRepeaterApp::setAckLevel(wxFloat32 level)
{
	m_thread->setAckLevel(level);
}

void CAnalogueRepeaterApp::setCTCSSLevel(wxFloat32 level)
{
	m_thread->setCTCSSLevel(level);
}

CAnalogueRepeaterStatusData* CAnalogueRepeaterApp::getStatus() const
{
	return m_thread->getStatus();
}

void CAnalogueRepeaterApp::showLog(const wxString& text)
{
	m_frame->showLog(text);
}

void CAnalogueRepeaterApp::getCallsign(AUDIO_SOURCE& openIdSource, wxString& openId, AUDIO_SOURCE& closeIdSource, wxString& closeId, AUDIO_SOURCE& beacon1Source, wxString& beacon1, AUDIO_SOURCE& beacon2Source, wxString& beacon2, unsigned int& speed, unsigned int& freq, wxFloat32& level1, wxFloat32& level2) const
{
	m_config->getCallsign(openIdSource, openId, closeIdSource, closeId, beacon1Source, beacon1, beacon2Source, beacon2, speed, freq, level1, level2);
}

void CAnalogueRepeaterApp::setCallsign(AUDIO_SOURCE openIdSource, const wxString& openId, AUDIO_SOURCE closeIdSource, const wxString& closeId, AUDIO_SOURCE beacon1Source, const wxString& beacon1, AUDIO_SOURCE beacon2Source, const wxString& beacon2, unsigned int speed, unsigned int freq, wxFloat32 level1, wxFloat32 level2)
{
	m_config->setCallsign(openIdSource, openId, closeIdSource, closeId, beacon1Source, beacon1, beacon2Source, beacon2, speed, freq, level1, level2);
}

void CAnalogueRepeaterApp::getAck(AUDIO_SOURCE& radioAckSource, wxString& radioAck, AUDIO_SOURCE& extAckSource, wxString& extAck, AUDIO_SOURCE& batteryAckSource, wxString& batteryAck, unsigned int& speed, unsigned int& freq, wxFloat32& level, unsigned int& ack, unsigned int& minimum) const
{
	m_config->getAck(radioAckSource, radioAck, extAckSource, extAck, batteryAckSource, batteryAck, speed, freq, level, ack, minimum);
}

void CAnalogueRepeaterApp::setAck(AUDIO_SOURCE radioAckSource, const wxString& radioAck, AUDIO_SOURCE extAckSource, const wxString& extAck, AUDIO_SOURCE batteryAckSource, const wxString& batteryAck, unsigned int speed, unsigned int freq, wxFloat32 level, unsigned int ack, unsigned int minimum)
{
	m_config->setAck(radioAckSource, radioAck, extAckSource, extAck, batteryAckSource, batteryAck, speed, freq, level, ack, minimum);
}

void CAnalogueRepeaterApp::getTimes(unsigned int& callsignTime, unsigned int& timeout, unsigned int& lockoutTime, unsigned int& hangTime, unsigned int& latchTime) const
{
	m_config->getTimes(callsignTime, timeout, lockoutTime, hangTime, latchTime);
}

void CAnalogueRepeaterApp::setTimes(unsigned int callsignTime, unsigned int timeout, unsigned int lockoutTime, unsigned int hangTime, unsigned int latchTime)
{
	m_config->setTimes(callsignTime, timeout, lockoutTime, hangTime, latchTime);
}

void CAnalogueRepeaterApp::getTones(bool& tbEnable, wxFloat32& tbThreshold, wxFloat32& ctcssFreq, bool& ctcssInternal, wxFloat32& ctcssThresh, wxFloat32& ctcssLevel, unsigned int& ctcssHangTime, ANALOGUE_CTCSS_OUTPUT& ctcssOutput) const
{
	m_config->getTones(tbEnable, tbThreshold, ctcssFreq, ctcssInternal, ctcssThresh, ctcssLevel, ctcssHangTime, ctcssOutput);
}

void CAnalogueRepeaterApp::setTones(bool tbEnable, wxFloat32 tbThreshold, wxFloat32 ctcssFreq, bool ctcssInternal, wxFloat32 ctcssThresh, wxFloat32 ctcssLevel, unsigned int ctcssHangTime, ANALOGUE_CTCSS_OUTPUT ctcssOutput)
{
	m_config->setTones(tbEnable, tbThreshold, ctcssFreq, ctcssInternal, ctcssThresh, ctcssLevel, ctcssHangTime, ctcssOutput);
}

void CAnalogueRepeaterApp::getFeel(ANALOGUE_CALLSIGN_START& callsignAtStart, unsigned int& callsignStartDelay, bool& callsignAtEnd, ANALOGUE_TIMEOUT_TYPE& timeoutType, ANALOGUE_CALLSIGN_HOLDOFF& callsignHoldoff) const
{
	m_config->getFeel(callsignAtStart, callsignStartDelay, callsignAtEnd, timeoutType, callsignHoldoff);
}

void CAnalogueRepeaterApp::setFeel(ANALOGUE_CALLSIGN_START callsignAtStart, unsigned int callsignStartDelay, bool callsignAtEnd, ANALOGUE_TIMEOUT_TYPE timeoutType, ANALOGUE_CALLSIGN_HOLDOFF callsignHoldoff)
{
	m_config->setFeel(callsignAtStart, callsignStartDelay, callsignAtEnd, timeoutType, callsignHoldoff);
}

void CAnalogueRepeaterApp::getRadio(wxString& readDevice, wxString& writeDevice, unsigned int& delay, bool& deEmphasis, bool& preEmphasis, bool& vogad) const
{
	m_config->getRadio(readDevice, writeDevice, delay, deEmphasis, preEmphasis, vogad);
}

void CAnalogueRepeaterApp::setRadio(const wxString& readDevice, const wxString& writeDevice, unsigned int delay, bool deEmphasis, bool preEmphasis, bool vogad)
{
	m_config->setRadio(readDevice, writeDevice, delay, deEmphasis, preEmphasis, vogad);
}

void CAnalogueRepeaterApp::getExternal(ANALOGUE_EXTERNAL_MODE& mode, wxString& readDevice, wxString& writeDevice, unsigned int& delay, bool& deEmphasis, bool& preEmphasis, bool& vogad, wxString& device, SERIALPIN& txPin, SERIALPIN& rxPin, bool& background) const
{
	m_config->getExternal(mode, readDevice, writeDevice, delay, deEmphasis, preEmphasis, vogad, device, txPin, rxPin, background);
}

void CAnalogueRepeaterApp::setExternal(ANALOGUE_EXTERNAL_MODE mode, const wxString& readDevice, const wxString& writeDevice, unsigned int delay, bool deEmphasis, bool preEmphasis, bool vogad, const wxString& device, SERIALPIN txPin, SERIALPIN rxPin, bool background)
{
	m_config->setExternal(mode, readDevice, writeDevice, delay, deEmphasis, preEmphasis, vogad, device, txPin, rxPin, background);
}

void CAnalogueRepeaterApp::getController(wxString& type, unsigned int& config, unsigned int& pttDelay, unsigned int& squelchDelay, bool& pttInvert, bool& squelchInvert) const
{
	m_config->getController(type, config, pttDelay, squelchDelay, pttInvert, squelchInvert);
}

void CAnalogueRepeaterApp::setController(const wxString& type, unsigned int config, unsigned int pttDelay, unsigned int squelchDelay, bool pttInvert, bool squelchInvert)
{
	m_config->setController(type, config, pttDelay, squelchDelay, pttInvert, squelchInvert);
}

void CAnalogueRepeaterApp::getDTMF(bool& radio, bool& external, wxString& shutdown, wxString& startup, wxString& timeout, wxString& timeReset, wxString& command1, wxString& command1Line, wxString& command2, wxString& command2Line ,wxString& output1, wxString& output2, wxString& output3, wxString& output4, wxFloat32& threshold) const
{
	m_config->getDTMF(radio, external, shutdown, startup, timeout, timeReset, command1, command1Line, command2, command2Line, output1, output2, output3, output4, threshold);
}

void CAnalogueRepeaterApp::setDTMF(bool radio, bool external, const wxString& shutdown, const wxString& startup, const wxString& timeout, const wxString& timeReset, const wxString& command1, const wxString& command1Line, const wxString& command2, const wxString& command2Line, const wxString& output1, const wxString& output2, const wxString& output3, const wxString& output4, wxFloat32 threshold)
{
	m_config->setDTMF(radio, external, shutdown, startup, timeout, timeReset, command1, command1Line, command2, command2Line, output1, output2, output3, output4, threshold);
}

void CAnalogueRepeaterApp::getAPRS(bool& txEnabled, wxString& callsign, wxFloat32& latitude, wxFloat32& longitude, int& height, wxString& description) const
{
	m_config->getAPRS(txEnabled, callsign, latitude, longitude, height, description);
}

void CAnalogueRepeaterApp::setAPRS(bool txEnabled, const wxString& callsign, wxFloat32 latitude, wxFloat32 longitude, int height, const wxString& description)
{
	m_config->setAPRS(txEnabled, callsign, latitude, longitude, height, description);
}

void CAnalogueRepeaterApp::getActiveHang(unsigned int& time) const
{
	m_config->getActiveHang(time);
}

void CAnalogueRepeaterApp::setActiveHang(unsigned int time)
{
	m_config->setActiveHang(time);
}

void CAnalogueRepeaterApp::getOutputs(bool& out1, bool& out2, bool& out3, bool& out4) const
{
	m_config->getOutputs(out1, out2, out3, out4);
}

void CAnalogueRepeaterApp::setOutputs(bool out1, bool out2, bool out3, bool out4)
{
	m_config->setOutputs(out1, out2, out3, out4);
	m_thread->setOutputs(out1, out2, out3, out4);

	wxLogInfo(wxT("Output 1: %d, output 2: %d, output 3: %d, output 4: %d"), out1, out2, out3, out4);
}

void CAnalogueRepeaterApp::getPosition(int& x, int& y) const
{
	m_config->getPosition(x, y);
}

void CAnalogueRepeaterApp::setPosition(int x, int y)
{
	m_config->setPosition(x, y);
}

bool CAnalogueRepeaterApp::writeConfig()
{
	return m_config->write();
}

void CAnalogueRepeaterApp::shutdown()
{
	m_thread->shutdown();
}

void CAnalogueRepeaterApp::startup()
{
	m_thread->startup();
}

void CAnalogueRepeaterApp::timeout()
{
	m_thread->timeout();
}

void CAnalogueRepeaterApp::timeReset()
{
	m_thread->timeReset();
}

void CAnalogueRepeaterApp::command1()
{
	m_thread->command1();
}

void CAnalogueRepeaterApp::command2()
{
	m_thread->command2();
}

void CAnalogueRepeaterApp::createThread()
{
	CAnalogueRepeaterThread* thread = new CAnalogueRepeaterThread;

	wxString openId, closeId, beacon1, beacon2;
	unsigned int speed, freq;
	wxFloat32 level1, level2;
	AUDIO_SOURCE openIdSource, closeIdSource, beacon1Source, beacon2Source;
	getCallsign(openIdSource, openId, closeIdSource, closeId, beacon1Source, beacon1, beacon2Source, beacon2, speed, freq, level1, level2);
	wxLogInfo(wxT("Open ID source: %d, value: \"%s\", close ID source: %d, value: \"%s\", beacon 1 source: %d, value: \"%s\", beacon 2 source: %d, value: \"%s\", speed: %u WPM, freq: %u Hz, level1: %.3f, level2: %.3f"), int(openIdSource), openId.c_str(), int(closeIdSource), closeId.c_str(), int(beacon1Source), beacon1.c_str(), int(beacon2Source), beacon2.c_str(), speed, freq, level1, level2);

	IFixedAudioSource* openIdAudio  = NULL;
	IFixedAudioSource* closeIdAudio = NULL;
	IFixedAudioSource* beacon1Audio = NULL;
	IFixedAudioSource* beacon2Audio = NULL;

	switch (openIdSource) {
		case AS_CW_KEYER:
			openIdAudio = new CCWKeyer(openId, speed, freq, ANALOGUE_RADIO_SAMPLE_RATE);
			break;
		case AS_WAV_FILE: {
				CWAVFileStore* audio = new CWAVFileStore;
				bool res = audio->load(openId, ANALOGUE_RADIO_SAMPLE_RATE);
				if (!res)
					delete audio;
				else
					openIdAudio = audio;
			}
			break;
	}

	switch (closeIdSource) {
		case AS_CW_KEYER:
			closeIdAudio = new CCWKeyer(closeId, speed, freq, ANALOGUE_RADIO_SAMPLE_RATE);
			break;
		case AS_WAV_FILE: {
				CWAVFileStore* audio = new CWAVFileStore;
				bool res = audio->load(closeId, ANALOGUE_RADIO_SAMPLE_RATE);
				if (!res)
					delete audio;
				else
					closeIdAudio = audio;
			}
			break;
	}

	switch (beacon1Source) {
		case AS_CW_KEYER:
			beacon1Audio = new CCWKeyer(beacon1, speed, freq, ANALOGUE_RADIO_SAMPLE_RATE);
			break;
		case AS_WAV_FILE: {
				CWAVFileStore* audio = new CWAVFileStore;
				bool res = audio->load(beacon1, ANALOGUE_RADIO_SAMPLE_RATE);
				if (!res)
					delete audio;
				else
					beacon1Audio = audio;
			}
			break;
	}

	switch (beacon2Source) {
		case AS_CW_KEYER:
			beacon2Audio = new CCWKeyer(beacon2, speed, freq, ANALOGUE_RADIO_SAMPLE_RATE);
			break;
		case AS_WAV_FILE: {
				CWAVFileStore* audio = new CWAVFileStore;
				bool res = audio->load(beacon2, ANALOGUE_RADIO_SAMPLE_RATE);
				if (!res)
					delete audio;
				else
					beacon2Audio = audio;
			}
			break;
	}

	thread->setCallsign(openIdAudio, closeIdAudio, beacon1Audio, beacon2Audio, level1, level2);

	wxString radioAck, extAck, batteryAck;
	unsigned int ack, minimum;
	wxFloat32 level;
	AUDIO_SOURCE radioAckSource, extAckSource, batteryAckSource;
	getAck(radioAckSource, radioAck, extAckSource, extAck, batteryAckSource, batteryAck, speed, freq, level, ack, minimum);
	wxLogInfo(wxT("Radio ack source: %d, radio ack: \"%s\", network ack source: %d, network ack: \"%s\", battery ack source: %d, battery ack: \"%s\", speed: %u WPM, freq: %u Hz, level: %.3f, ack: %u ms, minimum: %u ms"), int(radioAckSource), radioAck.c_str(), int(extAckSource), extAck.c_str(), int(batteryAckSource), batteryAck.c_str(), speed, freq, level, ack, minimum);

	IFixedAudioSource* radioAckAudio   = NULL;
	IFixedAudioSource* extAckAudio     = NULL;
	IFixedAudioSource* batteryAckAudio = NULL;

	switch (radioAckSource) {
		case AS_CW_KEYER:
			radioAckAudio = new CCWKeyer(radioAck, speed, freq, ANALOGUE_RADIO_SAMPLE_RATE);
			break;
		case AS_WAV_FILE: {
				CWAVFileStore* audio = new CWAVFileStore;
				bool res = audio->load(radioAck, ANALOGUE_RADIO_SAMPLE_RATE);
				if (!res)
					delete audio;
				else
					radioAckAudio = audio;
			}
			break;
	}

	switch (extAckSource) {
		case AS_CW_KEYER:
			extAckAudio = new CCWKeyer(extAck, speed, freq, ANALOGUE_RADIO_SAMPLE_RATE);
			break;
		case AS_WAV_FILE: {
				CWAVFileStore* audio = new CWAVFileStore;
				bool res = audio->load(extAck, ANALOGUE_RADIO_SAMPLE_RATE);
				if (!res)
					delete audio;
				else
					extAckAudio = audio;
			}
			break;
	}

	switch (batteryAckSource) {
		case AS_CW_KEYER:
			batteryAckAudio = new CCWKeyer(batteryAck, speed, freq, ANALOGUE_RADIO_SAMPLE_RATE);
			break;
		case AS_WAV_FILE: {
				CWAVFileStore* audio = new CWAVFileStore;
				bool res = audio->load(extAck, ANALOGUE_RADIO_SAMPLE_RATE);
				if (!res)
					delete audio;
				else
					batteryAckAudio = audio;
			}
			break;
	}

	thread->setAck(radioAckAudio, extAckAudio, batteryAckAudio, level, ack, minimum);

	unsigned int callsignTime, timeout, lockoutTime, hangTime, latchTime;
	getTimes(callsignTime, timeout, lockoutTime, hangTime, latchTime);
	thread->setTimes(callsignTime, timeout, lockoutTime, hangTime, latchTime);
	wxLogInfo(wxT("Times set to: callsign time: %u secs, timeout: %u secs, lockout time: %u secs, hang time: %u secs, latch time: %u secs"), callsignTime, timeout, lockoutTime, hangTime, latchTime);

	bool tbEnable, ctcssInternal;
	wxFloat32 tbThreshold, ctcssFreq, ctcssThresh, ctcssLevel;
	unsigned int ctcssHangTime;
	ANALOGUE_CTCSS_OUTPUT ctcssOutput;
	getTones(tbEnable, tbThreshold, ctcssFreq, ctcssInternal, ctcssThresh, ctcssLevel, ctcssHangTime, ctcssOutput);
	thread->setTones(tbEnable, tbThreshold, ctcssFreq, ctcssInternal, ctcssThresh, ctcssLevel, ctcssHangTime, ctcssOutput);
	wxLogInfo(wxT("Tones set to: toneburst enable: %u, threshold: %.3f, CTCSS freq: %.1f Hz, internal: %u, threshold: %.3f, level: %.3f, hang time: %u ms, output: %d"), tbEnable, tbThreshold, ctcssFreq, ctcssInternal, ctcssThresh, ctcssLevel, ctcssHangTime * 20U, ctcssOutput);

	ANALOGUE_CALLSIGN_START callsignAtStart;
	unsigned int callsignStartDelay;
	bool callsignAtEnd;
	ANALOGUE_TIMEOUT_TYPE timeoutType;
	ANALOGUE_CALLSIGN_HOLDOFF callsignHoldoff;
	getFeel(callsignAtStart, callsignStartDelay, callsignAtEnd, timeoutType, callsignHoldoff);
	thread->setFeel(callsignAtStart, callsignStartDelay, callsignAtEnd, timeoutType, callsignHoldoff);
	wxLogInfo(wxT("Feel set to: callsignAtStart: %d, callsignStartDelay: %u s, callsignAtEnd: %u, timeoutType: %d, callsignHoldoff: %d"), callsignAtStart, callsignStartDelay, callsignAtEnd, timeoutType, callsignHoldoff);

	wxString readDevice, writeDevice;
	unsigned int audioDelay;
	bool deEmphasis, preEmphasis, vogad;
	getRadio(readDevice, writeDevice, audioDelay, deEmphasis, preEmphasis, vogad);
	wxLogInfo(wxT("Radio soundcard set to %s:%s, delay: %u ms, de-emphasis: %d, pre-emphasis %d, vogad: %d"), readDevice.c_str(), writeDevice.c_str(), audioDelay * 20U, int(deEmphasis), int(preEmphasis), int(vogad));

	if (!readDevice.IsEmpty() && !writeDevice.IsEmpty()) {
#if defined(__WINDOWS__)
		CSoundCardReaderWriter* soundcard = new CSoundCardReaderWriter(readDevice, writeDevice, ANALOGUE_RADIO_SAMPLE_RATE, ANALOGUE_RADIO_BLOCK_SIZE);
#else
		CSoundCardReaderWriter* soundcard = new CSoundCardReaderWriter(readDevice, writeDevice, ANALOGUE_RADIO_SAMPLE_RATE, 64U);
#endif
		soundcard->setCallback(thread, SOUNDCARD_RADIO);

		bool res = soundcard->open();
		if (!res)
			wxLogError(wxT("Cannot open the radio sound card"));
		else
			thread->setRadio(soundcard, audioDelay, deEmphasis, preEmphasis, vogad);
	}

	wxString type;
	unsigned int pttDelay, squelchDelay, cfg;
	bool pttInvert, squelchInvert;
	getController(type, cfg, pttDelay, squelchDelay, pttInvert, squelchInvert);
	wxLogInfo(wxT("Controller set to %s, config: %u, ptt delay: %u ms, squelch delay: %u ms, ptt invert: %d, squelch invert: %d"), type.c_str(), cfg, pttDelay * 20U, squelchInvert * 20U, pttInvert, squelchInvert);

	CExternalController* controller = NULL;

	wxString port;
	if (type.StartsWith(wxT("Velleman K8055 - "), &port)) {
		unsigned long num;
		port.ToULong(&num);
		controller = new CExternalController(new CK8055Controller(num), pttInvert, squelchInvert);
	} else if (type.StartsWith(wxT("Serial - "), &port)) {
		controller = new CExternalController(new CSerialLineController(port, cfg), pttInvert, squelchInvert);
	} else if (type.StartsWith(wxT("Arduino - "), &port)) {
		controller = new CExternalController(new CArduinoController(port), pttInvert, squelchInvert);
#if defined(RASPBERRY_PI)
	} else if (type.IsSameAs(wxT("Raspberry Pi"))) {
		controller = new CExternalController(new CRaspberryController, pttInvert, squelchInvert);
#endif
	} else {
		controller = new CExternalController(new CDummyController, pttInvert, squelchInvert);
	}

	bool res = controller->open();
	if (!res)
		wxLogError(wxT("Cannot open the hardware interface - %s"), type.c_str());
	else
		thread->setController(controller, pttDelay, squelchDelay);

	ANALOGUE_EXTERNAL_MODE mode;
	wxString device;
	SERIALPIN txPin, rxPin;
	bool background;
	getExternal(mode, readDevice, writeDevice, audioDelay, deEmphasis, preEmphasis, vogad, device, txPin, rxPin, background);
	wxLogInfo(wxT("External mode: %d, soundcard set to %s:%s, delay: %u ms, de-emphasis: %d, pre-emphasis %d, vogad: %u, interface set to %s, tx %d, rx %d, background: %d"), mode, readDevice.c_str(), writeDevice.c_str(), audioDelay * 20U, int(deEmphasis), int(preEmphasis), int(vogad), device.c_str(), txPin, rxPin, int(background));

	if (mode != AEM_DISABLED) {
#if defined(__WINDOWS__)
		CSoundCardReaderWriter* soundcard = new CSoundCardReaderWriter(readDevice, writeDevice, ANALOGUE_RADIO_SAMPLE_RATE, ANALOGUE_RADIO_BLOCK_SIZE);
#else
		CSoundCardReaderWriter* soundcard = new CSoundCardReaderWriter(readDevice, writeDevice, ANALOGUE_RADIO_SAMPLE_RATE, 64U);
#endif
		soundcard->setCallback(thread, SOUNDCARD_EXTERNAL);

		bool res = soundcard->open();
		if (!res) {
			wxLogError(wxT("Cannot open the external sound card"));
		} else {
			// This works even for an empty device name
			CNetworkController* controller = new CNetworkController(device, txPin, rxPin);

			res = controller->open();
			if (!res)
				wxLogError(wxT("Cannot open serial port - %s"), device.c_str());
			else
				thread->setExternal(mode, soundcard, audioDelay, deEmphasis, preEmphasis, vogad, controller, background);
		}
	}

	bool out1, out2, out3, out4;
	getOutputs(out1, out2, out3, out4);
	thread->setOutputs(out1, out2, out3, out4);
	m_frame->setOutputs(out1, out2, out3, out4);
	wxLogInfo(wxT("Output 1: %d, output 2: %d, output 3: %d, output 4: %d"), out1, out2, out3, out4);

	bool dtmfRadio, dtmfExternal;
	wxString dtmfShutdown, dtmfStartup, dtmfTimeout, dtmfTimeReset, dtmfOutput1, dtmfOutput2, dtmfOutput3, dtmfOutput4;
	wxString dtmfCommand1, dtmfCommand1Line, dtmfCommand2, dtmfCommand2Line;
	wxFloat32 dtmfThreshold;
	getDTMF(dtmfRadio, dtmfExternal, dtmfShutdown, dtmfStartup, dtmfTimeout, dtmfTimeReset, dtmfCommand1, dtmfCommand1Line, dtmfCommand2, dtmfCommand2Line, dtmfOutput1, dtmfOutput2, dtmfOutput3, dtmfOutput4, dtmfThreshold);
	thread->setDTMF(dtmfRadio, dtmfExternal, dtmfShutdown, dtmfStartup, dtmfTimeout, dtmfTimeReset, dtmfCommand1, dtmfCommand1Line, dtmfCommand2, dtmfCommand2Line, dtmfOutput1, dtmfOutput2, dtmfOutput3, dtmfOutput4, dtmfThreshold);
	wxLogInfo(wxT("DTMF: Radio: %d, External: %d, Shutdown: %s, Startup: %s, Timeout: %s, Time Reset: %s, Command1: %s = %s, Command2: %s = %s, Output1: %s, Output2: %s, Output3: %s, Output4: %s, Threshold: %f"), dtmfRadio, dtmfExternal, dtmfShutdown.c_str(), dtmfStartup.c_str(), dtmfTimeout.c_str(), dtmfTimeReset.c_str(), dtmfCommand1.c_str(), dtmfCommand1Line.c_str(), dtmfCommand2.c_str(), dtmfCommand2Line.c_str(), dtmfOutput1.c_str(), dtmfOutput2.c_str(), dtmfOutput3.c_str(), dtmfOutput4.c_str(), dtmfThreshold);

	bool txEnabled;
	wxString aprsCallsign, description;
	wxFloat32 latitude, longitude;
	int height;
	getAPRS(txEnabled, aprsCallsign, latitude, longitude, height, description);
	wxLogInfo(wxT("APRS: TX Enabled: %d, Callsign: %s, Latitude: %.4f, Longitude: %.4f, Height: %d m, Description: %s"), int(txEnabled), aprsCallsign.c_str(), latitude, longitude, height, description.c_str());
	if (txEnabled) {
		CAPRSTX* aprsTx = new CAPRSTX(aprsCallsign, latitude, longitude, height, description);
		thread->setAPRSTX(aprsTx);
	}

	unsigned int activeHangTime;
	getActiveHang(activeHangTime);
	thread->setActiveHang(activeHangTime);
	wxLogInfo(wxT("Active Hang: time: %u"), activeHangTime);

	// Convert the worker class into a thread
	m_thread = new CAnalogueRepeaterThreadHelper(thread);
	m_thread->start();
}
