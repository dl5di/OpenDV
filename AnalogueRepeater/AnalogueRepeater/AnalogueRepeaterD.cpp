/*
 *   Copyright (C) 2009-2015 by Jonathan Naylor G4KLX
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

#include "AnalogueRepeaterConfig.h"
#include "SerialLineController.h"
#include "ExternalController.h"
#include "ArduinoController.h"
#include "AnalogueRepeaterD.h"
#include "DummyController.h"
#include "K8055Controller.h"
#if defined(GPIO)
#include "GPIOController.h"
#endif
#include "WAVFileStore.h"
#include "CWKeyer.h"
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
const wxChar*    DAEMON_SWITCH = wxT("daemon");


int main(int argc, char** argv)
{
	bool res = ::wxInitialize();
	if (!res) {
		::fprintf(stderr, "analoguerepeaterd: failed to initialise the wxWidgets library, exiting\n");
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
			::fprintf(stderr, "analoguerepeaterd: error in fork(), exiting\n");
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

	CAnalogueRepeaterD repeater(nolog, logDir, confDir, name);

	if (!repeater.init()) {
		::wxUninitialize();
		return 1;
	}

	repeater.run();

	::wxUninitialize();
	return 0;
}

CAnalogueRepeaterD::CAnalogueRepeaterD(bool nolog, const wxString& logDir, const wxString& confDir, const wxString& name) :
m_name(name),
m_nolog(nolog),
m_logDir(logDir),
m_confDir(confDir),
m_thread(NULL)
{
}

CAnalogueRepeaterD::~CAnalogueRepeaterD()
{
}

bool CAnalogueRepeaterD::init()
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

	// Log the version of wxWidgets and the Operating System
	wxLogInfo(wxT("Using wxWidgets %d.%d.%d on %s"), wxMAJOR_VERSION, wxMINOR_VERSION, wxRELEASE_NUMBER, ::wxGetOsDescription().c_str());

	return createThread();
}

void CAnalogueRepeaterD::run()
{
	m_thread->run();

	wxLogInfo(APPLICATION_NAME + wxT(" is exiting"));
}

bool CAnalogueRepeaterD::createThread()
{
	CAnalogueRepeaterConfig config(m_confDir, m_name);

	m_thread = new CAnalogueRepeaterThread;

	wxString openId, closeId, beacon1, beacon2;
	unsigned int speed, freq;
	wxFloat32 level1, level2;
	AUDIO_SOURCE openIdSource, closeIdSource, beacon1Source, beacon2Source;
	config.getCallsign(openIdSource, openId, closeIdSource, closeId, beacon1Source, beacon1, beacon2Source, beacon2, speed, freq, level1, level2);
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

	m_thread->setCallsign(openIdAudio, closeIdAudio, beacon1Audio, beacon2Audio, level1, level2);

	wxString radioAck, extAck, batteryAck;
	unsigned int ack, minimum;
	wxFloat32 level;
	AUDIO_SOURCE radioAckSource, extAckSource, batteryAckSource;
	config.getAck(radioAckSource, radioAck, extAckSource, extAck, batteryAckSource, batteryAck, speed, freq, level, ack, minimum);
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

	m_thread->setAck(radioAckAudio, extAckAudio, batteryAckAudio, level, ack, minimum);

	unsigned int callsignTime, timeout, lockoutTime, hangTime, latchTime;
	config.getTimes(callsignTime, timeout, lockoutTime, hangTime, latchTime);
	m_thread->setTimes(callsignTime, timeout, lockoutTime, hangTime, latchTime);
	wxLogInfo(wxT("Times set to: callsign time: %u secs, timeout: %u secs, lockout time: %u secs, hang time: %u secs, latch time: %u secs"), callsignTime, timeout, lockoutTime, hangTime, latchTime);

	bool tbEnable, ctcssInternal;
	wxFloat32 tbThreshold, ctcssFreq, ctcssThresh, ctcssLevel;
	unsigned int ctcssHangTime;
	ANALOGUE_CTCSS_OUTPUT ctcssOutput;
	config.getTones(tbEnable, tbThreshold, ctcssFreq, ctcssInternal, ctcssThresh, ctcssLevel, ctcssHangTime, ctcssOutput);
	m_thread->setTones(tbEnable, tbThreshold, ctcssFreq, ctcssInternal, ctcssThresh, ctcssLevel, ctcssHangTime, ctcssOutput);
	wxLogInfo(wxT("Tones set to: toneburst enable: %u, threshold: %.3f, CTCSS freq: %.1f Hz, internal: %u, threshold: %.3f, level: %.3f, hang time: %u ms, output: %d"), tbEnable, tbThreshold, ctcssFreq, ctcssInternal, ctcssThresh, ctcssLevel, ctcssHangTime * 20U, ctcssOutput);

	ANALOGUE_CALLSIGN_START callsignAtStart;
	unsigned int callsignStartDelay;
	bool callsignAtEnd;
	ANALOGUE_TIMEOUT_TYPE timeoutType;
	ANALOGUE_CALLSIGN_HOLDOFF callsignHoldoff;
	config.getFeel(callsignAtStart, callsignStartDelay, callsignAtEnd, timeoutType, callsignHoldoff);
	m_thread->setFeel(callsignAtStart, callsignStartDelay, callsignAtEnd, timeoutType, callsignHoldoff);
	wxLogInfo(wxT("Feel set to: callsignAtStart: %d, callsignStartDelay: %u s, callsignAtEnd: %u, timeoutType: %d, callsignHoldoff: %d"), callsignAtStart, callsignStartDelay, callsignAtEnd, timeoutType, callsignHoldoff);

	wxString readDevice, writeDevice;
	unsigned int audioDelay;
	bool deEmphasis, preEmphasis, vogad;
	config.getRadio(readDevice, writeDevice, audioDelay, deEmphasis, preEmphasis, vogad);
	wxLogInfo(wxT("Radio soundcard set to %s:%s, delay: %u ms, de-emphasis: %d, pre-emphasis %d, vogad: %d"), readDevice.c_str(), writeDevice.c_str(), audioDelay * 20U, int(deEmphasis), int(preEmphasis), int(vogad));

	if (!readDevice.IsEmpty() && !writeDevice.IsEmpty()) {
		CSoundCardReaderWriter* soundcard = new CSoundCardReaderWriter(readDevice, writeDevice, ANALOGUE_RADIO_SAMPLE_RATE, ANALOGUE_RADIO_BLOCK_SIZE);
		soundcard->setCallback(m_thread, SOUNDCARD_RADIO);

		bool res = soundcard->open();
		if (!res) {
			wxLogError(wxT("Cannot open the radio sound card"));
			return false;
		}

		m_thread->setRadio(soundcard, audioDelay, deEmphasis, preEmphasis, vogad);
	}

	wxString type;
	unsigned int pttDelay, squelchDelay, cfg;
	bool pttInvert, squelchInvert;
	config.getController(type, cfg, pttDelay, squelchDelay, pttInvert, squelchInvert);
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
#if defined(GPIO)
	} else if (type.IsSameAs(wxT("GPIO"))) {
		controller = new CExternalController(new CGPIOController(cfg), pttInvert, squelchInvert);
#endif
	} else {
		controller = new CExternalController(new CDummyController, pttInvert, squelchInvert);
	}

	bool res = controller->open();
	if (!res) {
		wxLogError(wxT("Cannot open the hardware interface - %s"), type.c_str());
		return false;
	}

	m_thread->setController(controller, pttDelay, squelchDelay);

	ANALOGUE_EXTERNAL_MODE mode;
	wxString device;
	SERIALPIN txPin, rxPin;
	bool background;
	config.getExternal(mode, readDevice, writeDevice, audioDelay, deEmphasis, preEmphasis, vogad, device, txPin, rxPin, background);
	wxLogInfo(wxT("External mode: %d, soundcard set to %s:%s, delay: %u ms, de-emphasis: %d, pre-emphasis %d, vogad: %u, interface set to %s, tx %d, rx %d, background: %d"), mode, readDevice.c_str(), writeDevice.c_str(), audioDelay * 20U, int(deEmphasis), int(preEmphasis), int(vogad), device.c_str(), txPin, rxPin, int(background));

	if (mode != AEM_DISABLED) {
		CSoundCardReaderWriter* soundcard = new CSoundCardReaderWriter(readDevice, writeDevice, ANALOGUE_RADIO_SAMPLE_RATE, ANALOGUE_RADIO_BLOCK_SIZE);
		soundcard->setCallback(m_thread, SOUNDCARD_EXTERNAL);

		bool res = soundcard->open();
		if (!res) {
			wxLogError(wxT("Cannot open the external sound card"));
			return false;
		}

		// This works even for an empty device name
		CNetworkController* controller = new CNetworkController(device, txPin, rxPin);

		res = controller->open();
		if (!res) {
			wxLogError(wxT("Cannot open serial port - %s"), device.c_str());
			return false;
		}

		m_thread->setExternal(mode, soundcard, audioDelay, deEmphasis, preEmphasis, vogad, controller, background);
	}

	bool out1, out2, out3, out4;
	config.getOutputs(out1, out2, out3, out4);
	m_thread->setOutputs(out1, out2, out3, out4);
	wxLogInfo(wxT("Output 1: %d, output 2: %d, output 3: %d, output 4: %d"), out1, out2, out3, out4);

	bool dtmfRadio, dtmfExternal;
	wxString dtmfShutdown, dtmfStartup, dtmfTimeout, dtmfTimeReset, dtmfOutput1, dtmfOutput2, dtmfOutput3, dtmfOutput4;
	wxString dtmfCommand1, dtmfCommand1Line, dtmfCommand2, dtmfCommand2Line;
	wxFloat32 dtmfThreshold;
	config.getDTMF(dtmfRadio, dtmfExternal, dtmfShutdown, dtmfStartup, dtmfTimeout, dtmfTimeReset, dtmfCommand1, dtmfCommand1Line, dtmfCommand2, dtmfCommand2Line, dtmfOutput1, dtmfOutput2, dtmfOutput3, dtmfOutput4, dtmfThreshold);
	m_thread->setDTMF(dtmfRadio, dtmfExternal, dtmfShutdown, dtmfStartup, dtmfTimeout, dtmfTimeReset, dtmfCommand1, dtmfCommand1Line, dtmfCommand2, dtmfCommand2Line, dtmfOutput1, dtmfOutput2, dtmfOutput3, dtmfOutput4, dtmfThreshold);
	wxLogInfo(wxT("DTMF: Radio: %d, External: %d, Shutdown: %s, Startup: %s, Timeout: %s, Time Reset: %s, Command1: %s = %s, Command2: %s = %s, Output1: %s, Output2: %s, Output3: %s, Output4: %s, Threshold: %f"), dtmfRadio, dtmfExternal, dtmfShutdown.c_str(), dtmfStartup.c_str(), dtmfTimeout.c_str(), dtmfTimeReset.c_str(), dtmfCommand1.c_str(), dtmfCommand1Line.c_str(), dtmfCommand2.c_str(), dtmfCommand2Line.c_str(), dtmfOutput1.c_str(), dtmfOutput2.c_str(), dtmfOutput3.c_str(), dtmfOutput4.c_str(), dtmfThreshold);

	unsigned int activeHangTime;
	config.getActiveHang(activeHangTime);
	m_thread->setActiveHang(activeHangTime);
	wxLogInfo(wxT("Active Hang: time: %u"), activeHangTime);

	return true;
}
