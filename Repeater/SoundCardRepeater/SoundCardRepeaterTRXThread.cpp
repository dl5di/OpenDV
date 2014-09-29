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

#include "SoundCardRepeaterTRXThread.h"
#include "SoundCardRepeaterApp.h"
#include "RadioHeaderDecoder.h"
#include "RadioHeaderEncoder.h"
#include "DStarDefines.h"
#include "HeaderData.h"
#include "Version.h"
#include "Utils.h"

const unsigned char DTMF_MASK[] = {0x82U, 0x08U, 0x20U, 0x82U, 0x00U, 0x00U, 0x82U, 0x00U, 0x00U};
const unsigned char DTMF_SIG[]  = {0x82U, 0x08U, 0x20U, 0x82U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U};

// #define	TX_TO_WAV_FILE

const wxFloat32 GOERTZEL_FREQ = 4000.0F; 
const unsigned int GOERTZEL_N = DSTAR_RADIO_SAMPLE_RATE / 500U;

const unsigned int MAX_DATA_SYNC_BIT_ERRS   = 2U;
const unsigned int MAX_END_PATTERN_BIT_ERRS = 4U;
const unsigned int MAX_SQUELCH_COUNT = 5U;
const unsigned int MAX_LOST_SYNCS    = 3U;

const unsigned int SILENCE_THRESHOLD = 2U;

CSoundCardRepeaterTRXThread::CSoundCardRepeaterTRXThread() :
m_soundcard(NULL),
m_protocolHandler(NULL),
m_controller(NULL),
m_goertzel(DSTAR_RADIO_SAMPLE_RATE, GOERTZEL_FREQ, GOERTZEL_N, 0.1F),
m_pttDelay(NULL),
m_audioDelay(NULL),
m_stopped(true),
m_inBuffer(DSTAR_RADIO_SAMPLE_RATE * 1U),			// One second of data
m_outBuffer(DSTAR_RADIO_SAMPLE_RATE * 1U),			// One second of data
m_localBuffer(DSTAR_GMSK_SYMBOL_RATE * 1U),			// One second of data
m_radioBuffer(DSTAR_GMSK_SYMBOL_RATE * 1U),			// One second of data
m_networkBuffer(DSTAR_GMSK_SYMBOL_RATE * 2U),		// Two seconds of data
m_networkRun(0U),
m_localStarted(false),
m_radioStarted(false),
m_networkStarted(false),
m_beacon(NULL),
m_announcement(NULL),
m_recordRPT1(),
m_recordRPT2(),
m_deleteRPT1(),
m_deleteRPT2(),
m_rptCallsign(),
m_gwyCallsign(),
m_reader(NULL),
m_demodulator(),
m_modulator(),
m_bitBuffer(DV_FRAME_LENGTH_BITS),
m_rxState(DSRXS_LISTENING),
m_frameMatcher(FRAME_SYNC_LENGTH_BITS, FRAME_SYNC_BITS),
m_dataMatcher(DATA_FRAME_LENGTH_BITS, DATA_SYNC_BITS),
m_endMatcher(END_PATTERN_LENGTH_BITS, END_PATTERN_BITS),
m_writer(NULL),
m_header(NULL),
m_headerBER(0U),
m_radioSeqNo(0U),
m_radioSyncsLost(0U),
m_networkSeqNo(0U),
m_timeoutTimer(1000U, 180U),	// 180s
m_watchdogTimer(1000U, 2U),		// 2s
m_pollTimer(1000U, 60U),		// 60s
m_ackTimer(1000U, 0U, 500U),	// 0.5s
m_status1Timer(1000U, 3U),		// 3s
m_status2Timer(1000U, 3U),		// 3s
m_status3Timer(1000U, 3U),		// 3s
m_status4Timer(1000U, 3U),		// 3s
m_status5Timer(1000U, 3U),		// 3s
m_hangTimer(1000U, 0U, 0U),		// 0ms
m_beaconTimer(1000U, 600U),		// 10 mins
m_announcementTimer(1000U, 0U),	// not running
m_rptState(DSRS_LISTENING),
m_slowDataDecoder(),
m_ackEncoder(),
m_linkEncoder(),
m_headerEncoder(),
m_status1Encoder(),
m_status2Encoder(),
m_status3Encoder(),
m_status4Encoder(),
m_status5Encoder(),
m_tx(false),
m_squelch(false),
m_squelchCount(0U),
m_noise(0.0F),
m_noiseCount(0U),
m_killed(false),
m_mode(MODE_DUPLEX),
m_ack(AT_BER),
m_restriction(false),
m_rpt1Validation(true),
m_rxLevel(1.0F),
m_txLevel(1.0F),
m_squelchMode(SM_NORMAL),
m_controlEnabled(false),
m_controlRPT1(),
m_controlRPT2(),
m_controlShutdown(),
m_controlStartup(),
m_controlStatus1(),
m_controlStatus2(),
m_controlStatus3(),
m_controlStatus4(),
m_controlStatus5(),
m_controlCommand1(),
m_controlCommand1Line(),
m_controlCommand2(),
m_controlCommand2Line(),
m_controlCommand3(),
m_controlCommand3Line(),
m_controlCommand4(),
m_controlCommand4Line(),
m_logging(NULL),
m_controlOutput1(),
m_controlOutput2(),
m_controlOutput3(),
m_controlOutput4(),
m_output1(false),
m_output2(false),
m_output3(false),
m_output4(false),
m_activeHangTimer(1000U),
m_shutdown(false),
m_disable(false),
m_lastData(NULL),
m_ambe(),
m_ambeFrames(0U),
m_ambeSilence(0U),
m_ambeBits(1U),
m_ambeErrors(0U),
m_lastAMBEBits(0U),
m_lastAMBEErrors(0U),
m_radioCount(0U),
m_networkCount(0U),
m_transmitCount(0U),
m_timeCount(0U),
m_lastHour(0U),
m_ackText(),
m_tempAckText(),
m_linkStatus(LS_NONE),
m_reflector(),
m_status1Text(),
m_status2Text(),
m_status3Text(),
m_status4Text(),
m_status5Text(),
m_regEx(wxT("^[A-Z0-9]{1}[A-Z0-9]{0,1}[0-9]{1,2}[A-Z]{1,4} {0,4}[ A-Z]{1}$")),
m_headerTime(),
m_packetTime(),
m_packetCount(0U),
m_packetSilence(0U),
m_blackList(NULL),
m_greyList(NULL),
m_blocked(false),
m_busyData(false),
m_blanking(true),
m_recording(false),
m_deleting(false)
{
	m_lastData = new unsigned char[DV_FRAME_MAX_LENGTH_BYTES];

	setRadioState(DSRXS_LISTENING);
	setRepeaterState(DSRS_LISTENING);
}

CSoundCardRepeaterTRXThread::~CSoundCardRepeaterTRXThread()
{
	delete[] m_lastData;
	delete   m_header;
}

void CSoundCardRepeaterTRXThread::run()
{
	// Wait here until we have the essentials to run
	while (!m_killed && (m_soundcard == NULL || m_rptCallsign.IsEmpty() || m_rptCallsign.IsSameAs(wxT("        ")) || m_controller == NULL))
		::wxMilliSleep(500UL);		// 1/2 sec

	if (m_killed)
		return;

	m_stopped = false;

	m_beaconTimer.start();
	m_announcementTimer.start();
	m_controller->setActive(false);
	m_controller->setRadioTransmit(false);

	if (m_protocolHandler != NULL)
		m_pollTimer.start();

	wxDateTime dateTime = wxDateTime::Now();
	m_lastHour = dateTime.GetHour();

	m_inBuffer.clear();

	wxLogMessage(wxT("Starting the sound card repeater thread"));

	unsigned int count = 0U;

	wxStopWatch timer;

	while (!m_killed) {
		timer.Start();

		// Process the incoming D-Star transmission
		receiveRadio();

		// Process network traffic
		receiveNetwork();

		repeaterStateMachine();

		// Send the network poll if needed and restart the timer
		if (m_pollTimer.hasExpired()) {
#if defined(__WINDOWS__)
			m_protocolHandler->writePoll(wxT("win_sound-") + VERSION);
#else
			m_protocolHandler->writePoll(wxT("linux_sound-") + VERSION);
#endif
			m_pollTimer.reset();
		}

		// Send the beacon and restart the timer
		if (m_beaconTimer.isRunning() && m_beaconTimer.hasExpired()) {
			m_beacon->sendBeacon();
			m_beaconTimer.reset();
		}

		// Send the announcement and restart the timer
		if (m_announcementTimer.isRunning() && m_announcementTimer.hasExpired()) {
			m_announcement->startAnnouncement();
			m_announcementTimer.reset();
		}

		// Send the status 1 message after a few seconds
		if (m_status1Timer.isRunning() && m_status1Timer.hasExpired()) {
			m_status1Timer.stop();
			if (m_rptState == DSRS_LISTENING)
				transmitUserStatus(0U);
		}

		// Send the status 2 message after a few seconds
		if (m_status2Timer.isRunning() && m_status2Timer.hasExpired()) {
			m_status2Timer.stop();
			if (m_rptState == DSRS_LISTENING)
				transmitUserStatus(1U);
		}

		// Send the status 3 message after a few seconds
		if (m_status3Timer.isRunning() && m_status3Timer.hasExpired()) {
			m_status3Timer.stop();
			if (m_rptState == DSRS_LISTENING)
				transmitUserStatus(2U);
		}

		// Send the status 4 message after a few seconds
		if (m_status4Timer.isRunning() && m_status4Timer.hasExpired()) {
			m_status4Timer.stop();
			if (m_rptState == DSRS_LISTENING)
				transmitUserStatus(3U);
		}

		// Send the status 5 message after a few seconds
		if (m_status5Timer.isRunning() && m_status5Timer.hasExpired()) {
			m_status5Timer.stop();
			if (m_rptState == DSRS_LISTENING)
				transmitUserStatus(4U);
		}

		// Clock the heartbeat output every one second
		count++;
		if (count == 50U) {
			m_controller->setHeartbeat();
			count = 0U;
		}

		// Set the output state
		if (m_rptState == DSRS_VALID      || m_rptState == DSRS_INVALID      || m_rptState == DSRS_TIMEOUT      ||
			m_rptState == DSRS_VALID_WAIT || m_rptState == DSRS_INVALID_WAIT || m_rptState == DSRS_TIMEOUT_WAIT ||
			m_rptState == DSRS_NETWORK    || (m_activeHangTimer.isRunning() && !m_activeHangTimer.hasExpired())) {
			m_controller->setActive(true);
		} else {
			m_controller->setActive(false);
			m_activeHangTimer.stop();
		}

		// Check the shutdown state, state changes are done here to bypass the state machine which is
		// frozen when m_disable or m_shutdown are asserted
		m_disable = m_controller->getDisable();
		if (m_disable || m_shutdown) {
			if (m_rptState != DSRS_SHUTDOWN) {
				m_timeoutTimer.stop();
				m_watchdogTimer.stop();
				m_activeHangTimer.stop();
				m_ackTimer.stop();
				m_hangTimer.stop();
				m_beaconTimer.stop();
				m_announcementTimer.stop();
				m_localBuffer.clear();
				m_radioBuffer.clear();
				m_networkBuffer.clear();
				m_bitBuffer.clear();
				m_networkRun = 0U;
				m_localStarted = false;
				m_radioStarted = false;
				m_networkStarted = false;
				m_controller->setActive(false);
				m_controller->setRadioTransmit(false);
				m_rptState = DSRS_SHUTDOWN;
			}
		} else {
			if (m_rptState == DSRS_SHUTDOWN) {
				m_timeoutTimer.stop();
				m_watchdogTimer.stop();
				m_ackTimer.stop();
				m_hangTimer.stop();
				m_beaconTimer.start();
				m_announcementTimer.start();
				m_rptState = DSRS_LISTENING;
				if (m_protocolHandler != NULL)	// Tell the protocol handler
					m_protocolHandler->reset();
			}
		}

		// Send the output data
		if (m_localStarted)
			transmitLocal();
		else if (m_radioStarted)
			transmitRadio();
		else if (m_networkStarted)
			transmitNetwork();
		else if (!m_localBuffer.isEmpty())
			transmitLocal();
		else if (!m_radioBuffer.isEmpty())
			transmitRadio();
		else if (m_networkRun >= NETWORK_RUN_FRAME_COUNT)
			transmitNetwork();
		else
			transmit();

		getStatistics();

		unsigned int ms = timer.Time();
		clock(ms);
	}

	writeStatistics();

	wxLogMessage(wxT("Stopping the sound card repeater thread"));

	m_controller->setActive(false);
	m_controller->setRadioTransmit(false);
	m_controller->close();
	delete m_controller;

	m_soundcard->close();
	delete m_soundcard;

	delete m_audioDelay;
	delete m_pttDelay;

	if (m_logging != NULL) {
		m_logging->close();
		delete m_logging;
	}

	delete m_beacon;
	delete m_announcement;

	delete m_blackList;
	delete m_greyList;

	if (m_reader != NULL) {
		m_reader->close();
		delete m_reader;
	}

	if (m_protocolHandler != NULL) {
		m_protocolHandler->close();
		delete m_protocolHandler;
	}

#if defined(TX_TO_WAV_FILE)
	if (m_writer != NULL) {
		m_writer->close();
		delete m_writer;
	}
#endif
}

void CSoundCardRepeaterTRXThread::kill()
{
	m_killed = true;
}

void CSoundCardRepeaterTRXThread::setReader(CWAVFileReader* reader)
{
	wxASSERT(reader != NULL);

	// Already busy?
	if (m_reader != NULL) {
		reader->close();
		delete reader;
		return;
	}

	wxLogInfo(wxT("Reading from WAV file - %s"), reader->getFilename().c_str());

	m_reader = reader;
}

void CSoundCardRepeaterTRXThread::setCallsign(const wxString& callsign, const wxString& gateway, DSTAR_MODE mode, ACK_TYPE ack, bool restriction, bool rpt1Validation, bool dtmfBlanking)
{
	// Pad the callsign up to eight characters
	m_rptCallsign = callsign;
	m_rptCallsign.resize(LONG_CALLSIGN_LENGTH, wxT(' '));

	if (gateway.IsEmpty()) {
		m_gwyCallsign = callsign;
		m_gwyCallsign.resize(LONG_CALLSIGN_LENGTH - 1U, wxT(' '));
		m_gwyCallsign.Append(wxT("G"));
	} else {
		m_gwyCallsign = gateway;
		m_gwyCallsign.resize(LONG_CALLSIGN_LENGTH, wxT(' '));
	}

	m_mode           = mode;
	m_ack            = ack;
	m_restriction    = restriction;
	m_rpt1Validation = rpt1Validation;
	m_blanking       = dtmfBlanking;
}

void CSoundCardRepeaterTRXThread::setProtocolHandler(CRepeaterProtocolHandler* handler)
{
	m_protocolHandler = handler;
}

void CSoundCardRepeaterTRXThread::setSoundCard(CSoundCardReaderWriter* soundcard, wxFloat32 rxLevel, wxFloat32 txLevel, SQUELCH_MODE squelchMode, wxFloat32 squelchLevel, bool rxInvert, bool txInvert)
{
	wxASSERT(soundcard != NULL);

	m_soundcard = soundcard;
	m_rxLevel   = rxLevel;
	m_txLevel   = txLevel;

	m_squelchMode = squelchMode;
	m_goertzel.setThreshold(squelchLevel);

	m_demodulator.setInvert(rxInvert);
	m_modulator.setInvert(txInvert);
}

void CSoundCardRepeaterTRXThread::setTimes(unsigned int timeout, unsigned int ackTime, unsigned int hangTime)
{
	m_timeoutTimer.setTimeout(timeout);
	m_ackTimer.setTimeout(0U, ackTime);
	m_hangTimer.setTimeout(0U, hangTime);
}

void CSoundCardRepeaterTRXThread::setBeacon(unsigned int time, const wxString& text, bool voice, TEXT_LANG language)
{
	m_beaconTimer.setTimeout(time);

	if (time > 0U)
		m_beacon = new CBeaconUnit(this, m_rptCallsign, text, voice, language);
}

void CSoundCardRepeaterTRXThread::setAnnouncement(bool enabled, unsigned int time, const wxString& recordRPT1, const wxString& recordRPT2, const wxString& deleteRPT1, const wxString& deleteRPT2)
{
	if (enabled && time > 0U) {
		m_announcement = new CAnnouncementUnit(this, m_rptCallsign);

		m_announcementTimer.setTimeout(time);

		m_recordRPT1 = recordRPT1;
		m_recordRPT2 = recordRPT2;
		m_deleteRPT1 = deleteRPT1;
		m_deleteRPT2 = deleteRPT2;

		m_recordRPT1.Append(wxT(' '), LONG_CALLSIGN_LENGTH);
		m_recordRPT2.Append(wxT(' '), LONG_CALLSIGN_LENGTH);
		m_deleteRPT1.Append(wxT(' '), LONG_CALLSIGN_LENGTH);
		m_deleteRPT2.Append(wxT(' '), LONG_CALLSIGN_LENGTH);

		m_recordRPT1.Truncate(LONG_CALLSIGN_LENGTH);
		m_recordRPT2.Truncate(LONG_CALLSIGN_LENGTH);
		m_deleteRPT1.Truncate(LONG_CALLSIGN_LENGTH);
		m_deleteRPT2.Truncate(LONG_CALLSIGN_LENGTH);
	}
}

void CSoundCardRepeaterTRXThread::setController(CExternalController* controller, int pttDelay)
{
	wxASSERT(controller != NULL);

	m_controller = controller;

	if (pttDelay != 0) {
		if (pttDelay > 0)
			m_pttDelay = new CPTTDelay(pttDelay);
		else
			m_audioDelay = new CAudioDelay(-pttDelay * DSTAR_FRAME_TIME_MS * DSTAR_RADIO_SAMPLE_RATE / 1000U);
	}
}

void CSoundCardRepeaterTRXThread::setControl(bool enabled, const wxString& rpt1Callsign, const wxString& rpt2Callsign, const wxString& shutdown, const wxString& startup, const wxString& status1, const wxString& status2, const wxString& status3, const wxString& status4, const wxString& status5, const wxString& command1, const wxString& command1Line, const wxString& command2, const wxString& command2Line, const wxString& command3, const wxString& command3Line, const wxString& command4, const wxString& command4Line, const wxString& output1, const wxString& output2, const wxString& output3, const wxString& output4)
{
	m_controlEnabled      = enabled;
	m_controlRPT1         = rpt1Callsign;
	m_controlRPT2         = rpt2Callsign;
	m_controlShutdown     = shutdown;
	m_controlStartup      = startup;
	m_controlStatus1      = status1;
	m_controlStatus2      = status2;
	m_controlStatus3      = status3;
	m_controlStatus4      = status4;
	m_controlStatus5      = status5;
	m_controlCommand1     = command1;
	m_controlCommand1Line = command1Line;
	m_controlCommand2     = command2;
	m_controlCommand2Line = command2Line;
	m_controlCommand3     = command3;
	m_controlCommand3Line = command3Line;
	m_controlCommand4     = command4;
	m_controlCommand4Line = command4Line;
	m_controlOutput1      = output1;
	m_controlOutput2      = output2;
	m_controlOutput3      = output3;
	m_controlOutput4      = output4;

	m_controlRPT1.Append(wxT(' '), LONG_CALLSIGN_LENGTH);
	m_controlRPT2.Append(wxT(' '), LONG_CALLSIGN_LENGTH);
	m_controlShutdown.Append(wxT(' '), LONG_CALLSIGN_LENGTH);
	m_controlStartup.Append(wxT(' '), LONG_CALLSIGN_LENGTH);
	m_controlStatus1.Append(wxT(' '), LONG_CALLSIGN_LENGTH);
	m_controlStatus2.Append(wxT(' '), LONG_CALLSIGN_LENGTH);
	m_controlStatus3.Append(wxT(' '), LONG_CALLSIGN_LENGTH);
	m_controlStatus4.Append(wxT(' '), LONG_CALLSIGN_LENGTH);
	m_controlStatus5.Append(wxT(' '), LONG_CALLSIGN_LENGTH);
	m_controlCommand1.Append(wxT(' '), LONG_CALLSIGN_LENGTH);
	m_controlCommand2.Append(wxT(' '), LONG_CALLSIGN_LENGTH);
	m_controlCommand3.Append(wxT(' '), LONG_CALLSIGN_LENGTH);
	m_controlCommand4.Append(wxT(' '), LONG_CALLSIGN_LENGTH);
	m_controlOutput1.Append(wxT(' '), LONG_CALLSIGN_LENGTH);
	m_controlOutput2.Append(wxT(' '), LONG_CALLSIGN_LENGTH);
	m_controlOutput3.Append(wxT(' '), LONG_CALLSIGN_LENGTH);
	m_controlOutput4.Append(wxT(' '), LONG_CALLSIGN_LENGTH);

	m_controlRPT1.Truncate(LONG_CALLSIGN_LENGTH);
	m_controlRPT2.Truncate(LONG_CALLSIGN_LENGTH);
	m_controlShutdown.Truncate(LONG_CALLSIGN_LENGTH);
	m_controlStartup.Truncate(LONG_CALLSIGN_LENGTH);
	m_controlStatus1.Truncate(LONG_CALLSIGN_LENGTH);
	m_controlStatus2.Truncate(LONG_CALLSIGN_LENGTH);
	m_controlStatus3.Truncate(LONG_CALLSIGN_LENGTH);
	m_controlStatus4.Truncate(LONG_CALLSIGN_LENGTH);
	m_controlStatus5.Truncate(LONG_CALLSIGN_LENGTH);
	m_controlCommand1.Truncate(LONG_CALLSIGN_LENGTH);
	m_controlCommand2.Truncate(LONG_CALLSIGN_LENGTH);
	m_controlCommand3.Truncate(LONG_CALLSIGN_LENGTH);
	m_controlCommand4.Truncate(LONG_CALLSIGN_LENGTH);
	m_controlOutput1.Truncate(LONG_CALLSIGN_LENGTH);
	m_controlOutput2.Truncate(LONG_CALLSIGN_LENGTH);
	m_controlOutput3.Truncate(LONG_CALLSIGN_LENGTH);
	m_controlOutput4.Truncate(LONG_CALLSIGN_LENGTH);
}

void CSoundCardRepeaterTRXThread::setActiveHang(unsigned int time)
{
	m_activeHangTimer.setTimeout(time);
}

void CSoundCardRepeaterTRXThread::setOutputs(bool out1, bool out2, bool out3, bool out4)
{
	if (m_controller == NULL)
		return;

	m_output1 = out1;
	m_output2 = out2;
	m_output3 = out3;
	m_output4 = out4;

	m_controller->setOutput1(m_output1);
	m_controller->setOutput2(m_output2);
	m_controller->setOutput3(m_output3);
	m_controller->setOutput4(m_output4);
}

void CSoundCardRepeaterTRXThread::setRXLevel(wxFloat32 level)
{
	m_rxLevel = level;
}

void CSoundCardRepeaterTRXThread::setTXLevel(wxFloat32 level)
{
	m_txLevel = level;
}

void CSoundCardRepeaterTRXThread::setSquelchLevel(wxFloat32 level)
{
	m_goertzel.setThreshold(level);
}

void CSoundCardRepeaterTRXThread::setLogging(bool logging, const wxString& dir)
{
	if (logging && m_logging == NULL) {
		m_logging = new CDVTOOLFileWriter;
		m_logging->setDirectory(dir);
		return;
	}

	if (!logging && m_logging != NULL) {
		delete m_logging;
		m_logging = NULL;
		return;
	}
}

void CSoundCardRepeaterTRXThread::setBlackList(CCallsignList* list)
{
	wxASSERT(list != NULL);

	m_blackList = list;
}

void CSoundCardRepeaterTRXThread::setGreyList(CCallsignList* list)
{
	wxASSERT(list != NULL);

	m_greyList = list;
}

void CSoundCardRepeaterTRXThread::receiveRadio()
{
	// Get an audio block from the RX
	wxFloat32 audio[DSTAR_RADIO_BLOCK_SIZE];

	unsigned int length = 0U;
	for (unsigned int n = 0U; length < DSTAR_RADIO_BLOCK_SIZE && n < 20U; n++) {
		length += m_inBuffer.getData(audio + length, DSTAR_RADIO_BLOCK_SIZE - length);

		if (length < DSTAR_RADIO_BLOCK_SIZE)
			::wxMilliSleep(DSTAR_FRAME_TIME_MS / 4UL);
	}

	if (length < DSTAR_RADIO_BLOCK_SIZE)
		wxLogError(wxT("No audio is being received"));

	// With GOERTZEL_N equal to 96 and a block size of 960 samples, there are ten results every block. There are
	// 96 bits (960 samples) per audio frame and so ten results per audio frame.
	for (unsigned int offset = 0U; offset < length; offset += GOERTZEL_N) {
		m_goertzel.process(audio + offset, GOERTZEL_N);

		TRISTATE state = m_goertzel.getDetected();
		switch (state) {
			case STATE_TRUE:
				m_noise += m_goertzel.getResult();
				m_noiseCount++;

				switch (m_squelchMode) {
					case SM_NORMAL:
						m_squelchCount++;
						m_squelch = false;
						break;
					case SM_INVERTED:
						m_squelch = true;
						break;
					case SM_OPEN:
						m_squelchCount = 0U;
						m_squelch = true;
						break;
				}

				break;
			case STATE_FALSE:
				m_noise += m_goertzel.getResult();
				m_noiseCount++;

				switch (m_squelchMode) {
					case SM_NORMAL:
						m_squelch = true;
						break;
					case SM_INVERTED:
						m_squelchCount++;
						m_squelch = false;
						break;
					case SM_OPEN:
						m_squelchCount = 0U;
						m_squelch = true;
						break;
				}

				break;
			case STATE_UNKNOWN:
				break;
		}
	}

	for (unsigned int i = 0U; i < length; i++) {
		TRISTATE state = m_demodulator.decode(audio[i] * m_rxLevel);

		switch (state) {
			case STATE_TRUE:
				radioStateMachine(true);
				break;
			case STATE_FALSE:
				radioStateMachine(false);
				break;
			case STATE_UNKNOWN:
				break;
		}
	}
}

void CSoundCardRepeaterTRXThread::receiveNetwork()
{
	if (m_protocolHandler == NULL)
		return;
	
	NETWORK_TYPE type;

	for (;;) {
		type = m_protocolHandler->read();

		// Get the data from the network
		if (type == NETWORK_NONE) {					// Nothing received
			break;
		} else if (type == NETWORK_HEADER) {		// A header
			CHeaderData* header = m_protocolHandler->readHeader();
			if (header != NULL) {
				::memcpy(m_lastData, NULL_FRAME_DATA_BYTES, DV_FRAME_LENGTH_BYTES);

				processNetworkHeader(header);

				m_headerTime.Start();
				m_packetTime.Start();
				m_packetCount   = 0U;
				m_packetSilence = 0U;
			}
		} else if (type == NETWORK_DATA) {			// AMBE data and slow data
			unsigned char data[DV_FRAME_MAX_LENGTH_BYTES];
			unsigned char seqNo;
			unsigned int length = m_protocolHandler->readData(data, DV_FRAME_MAX_LENGTH_BYTES, seqNo);
			if (length != 0U) {
				::memcpy(m_lastData, data, length);
				m_packetCount += processNetworkFrame(data, length, seqNo);
				m_watchdogTimer.reset();
			}
		} else if (type == NETWORK_TEXT) {			// Slow data text for the Ack
			m_protocolHandler->readText(m_ackText, m_linkStatus, m_reflector);
			m_linkEncoder.setTextData(m_ackText);
			wxLogMessage(wxT("Slow data set to \"%s\""), m_ackText.c_str());
		} else if (type == NETWORK_TEMPTEXT) {			// Temporary slow data text for the Ack
			m_protocolHandler->readTempText(m_tempAckText);
			wxLogMessage(wxT("Temporary slow data set to \"%s\""), m_tempAckText.c_str());
		} else if (type == NETWORK_STATUS1) {		// Status 1 data text
			m_status1Text = m_protocolHandler->readStatus1();
			m_status1Encoder.setTextData(m_status1Text);
			wxLogMessage(wxT("Status 1 data set to \"%s\""), m_status1Text.c_str());
		} else if (type == NETWORK_STATUS2) {		// Status 2 data text
			m_status2Text = m_protocolHandler->readStatus2();
			m_status2Encoder.setTextData(m_status2Text);
			wxLogMessage(wxT("Status 2 data set to \"%s\""), m_status2Text.c_str());
		} else if (type == NETWORK_STATUS3) {		// Status 3 data text
			m_status3Text = m_protocolHandler->readStatus3();
			m_status3Encoder.setTextData(m_status3Text);
			wxLogMessage(wxT("Status 3 data set to \"%s\""), m_status3Text.c_str());
		} else if (type == NETWORK_STATUS4) {		// Status 4 data text
			m_status4Text = m_protocolHandler->readStatus4();
			m_status4Encoder.setTextData(m_status4Text);
			wxLogMessage(wxT("Status 4 data set to \"%s\""), m_status4Text.c_str());
		} else if (type == NETWORK_STATUS5) {		// Status 5 data text
			m_status5Text = m_protocolHandler->readStatus5();
			m_status5Encoder.setTextData(m_status5Text);
			wxLogMessage(wxT("Status 5 data set to \"%s\""), m_status5Text.c_str());
		}
	}

	// Have we missed any data frames?
	if (m_rptState == DSRS_NETWORK && m_packetTime.Time() > 200L) {
		unsigned int packetsNeeded = m_headerTime.Time() / DSTAR_FRAME_TIME_MS;

		// wxLogMessage(wxT("Time: %u ms, need %u packets and received %u packets"), ms - m_headerMS, packetsNeeded, m_packetCount);

		if (packetsNeeded > m_packetCount) {
			unsigned int count = packetsNeeded - m_packetCount;

			if (count > 5U) {
				// wxLogMessage(wxT("Inserting silence into the network data stream"));

				count -= 2U;

				// Create silence frames
				for (unsigned int i = 0U; i < count; i++) {
					unsigned char data[DV_FRAME_LENGTH_BYTES];
					::memcpy(data, NULL_FRAME_DATA_BYTES, DV_FRAME_LENGTH_BYTES);
					m_packetCount += processNetworkFrame(data, DV_FRAME_LENGTH_BYTES, m_networkSeqNo);
					m_packetSilence++;
				}
			}
		}

		m_packetTime.Start();
	}
}

void CSoundCardRepeaterTRXThread::transmitLocalHeader(CHeaderData& header)
{
	wxLogMessage(wxT("Transmitting to - My: %s/%s  Your: %s  Rpt1: %s  Rpt2: %s  Flags: %02X %02X %02X"), header.getMyCall1().c_str(), header.getMyCall2().c_str(), header.getYourCall().c_str(), header.getRptCall1().c_str(), header.getRptCall2().c_str(), header.getFlag1(), header.getFlag2(), header.getFlag3());

	m_headerEncoder.setHeaderData(header);

	// Send the bit sync, 700 bits in 4 bit chunks
	for (unsigned int i = 0U; i < 175U; i++)
		m_localBuffer.addData(BIT_SYNC_BITS, BIT_SYNC_LENGTH_BITS);

	// Send the frame sync
	m_localBuffer.addData(FRAME_SYNC_BITS, FRAME_SYNC_LENGTH_BITS);

	// Send the header
	bool headerBits[FEC_SECTION_LENGTH_BITS];
	CRadioHeaderEncoder encoder(header);
	encoder.getRadioData(headerBits, FEC_SECTION_LENGTH_BITS);
	m_localBuffer.addData(headerBits, FEC_SECTION_LENGTH_BITS);

	m_localStarted = false;
}

void CSoundCardRepeaterTRXThread::transmitBeaconHeader()
{
	CHeaderData header(m_rptCallsign, wxT("RPTR"), wxT("CQCQCQ  "), m_gwyCallsign, m_rptCallsign);
	transmitLocalHeader(header);
}

void CSoundCardRepeaterTRXThread::transmitBeaconData(const unsigned char* data, unsigned int length, bool end)
{
	if (end) {
		m_localBuffer.addData(END_PATTERN_BITS, END_PATTERN_LENGTH_BITS);
		m_localBuffer.addData(END_PATTERN_BITS, END_PATTERN_LENGTH_BITS);
		m_localBuffer.addData(END_PATTERN_BITS, END_PATTERN_LENGTH_BITS);
	} else {
		// Convert the bytes to bits for transmission
		bool bits[DV_FRAME_LENGTH_BITS * 2U];
		unsigned int n = 0U;
		for (unsigned int i = 0U; i < length; i++, n += 8U)
			CUtils::byteToBitsRev(data[i], bits + n);
		m_localBuffer.addData(bits, length * 8U);
	}
}

void CSoundCardRepeaterTRXThread::transmitAnnouncementHeader(CHeaderData* header)
{
	header->setRptCall1(m_gwyCallsign);
	header->setRptCall2(m_rptCallsign);

	transmitLocalHeader(*header);

	delete header;
}

void CSoundCardRepeaterTRXThread::transmitAnnouncementData(const unsigned char* data, unsigned int length, bool end)
{
	if (end) {
		m_localBuffer.addData(END_PATTERN_BITS, END_PATTERN_LENGTH_BITS);
		m_localBuffer.addData(END_PATTERN_BITS, END_PATTERN_LENGTH_BITS);
		m_localBuffer.addData(END_PATTERN_BITS, END_PATTERN_LENGTH_BITS);
	} else {
		// Convert the bytes to bits for transmission
		bool bits[DV_FRAME_LENGTH_BITS * 2U];
		unsigned int n = 0U;
		for (unsigned int i = 0U; i < length; i++, n += 8U)
			CUtils::byteToBitsRev(data[i], bits + n);
		m_localBuffer.addData(bits, length * 8U);
	}
}

void CSoundCardRepeaterTRXThread::transmitRadioHeader(CHeaderData& header)
{
	wxLogMessage(wxT("Transmitting to - My: %s/%s  Your: %s  Rpt1: %s  Rpt2: %s  Flags: %02X %02X %02X"), header.getMyCall1().c_str(), header.getMyCall2().c_str(), header.getYourCall().c_str(), header.getRptCall1().c_str(), header.getRptCall2().c_str(), header.getFlag1(), header.getFlag2(), header.getFlag3());

	m_headerEncoder.setHeaderData(header);

	// Send the bit sync, 700 bits in 4 bit chunks
	for (unsigned int i = 0U; i < 175U; i++)
		m_radioBuffer.addData(BIT_SYNC_BITS, BIT_SYNC_LENGTH_BITS);

	// Send the frame sync
	m_radioBuffer.addData(FRAME_SYNC_BITS, FRAME_SYNC_LENGTH_BITS);

	// Send the header
	bool headerBits[FEC_SECTION_LENGTH_BITS];
	CRadioHeaderEncoder encoder(header);
	encoder.getRadioData(headerBits, FEC_SECTION_LENGTH_BITS);
	m_radioBuffer.addData(headerBits, FEC_SECTION_LENGTH_BITS);

	m_radioStarted = false;
}

void CSoundCardRepeaterTRXThread::transmitNetworkHeader(CHeaderData& header)
{
	wxLogMessage(wxT("Transmitting to - My: %s/%s  Your: %s  Rpt1: %s  Rpt2: %s  Flags: %02X %02X %02X"), header.getMyCall1().c_str(), header.getMyCall2().c_str(), header.getYourCall().c_str(), header.getRptCall1().c_str(), header.getRptCall2().c_str(), header.getFlag1(), header.getFlag2(), header.getFlag3());

	m_headerEncoder.setHeaderData(header);

	// Send the bit sync, 700 bits in 4 bit chunks
	for (unsigned int i = 0U; i < 175U; i++)
		m_networkBuffer.addData(BIT_SYNC_BITS, BIT_SYNC_LENGTH_BITS);

	// Send the frame sync
	m_networkBuffer.addData(FRAME_SYNC_BITS, FRAME_SYNC_LENGTH_BITS);

	// Send the header
	bool headerBits[FEC_SECTION_LENGTH_BITS];
	CRadioHeaderEncoder encoder(header);
	encoder.getRadioData(headerBits, FEC_SECTION_LENGTH_BITS);
	m_networkBuffer.addData(headerBits, FEC_SECTION_LENGTH_BITS);

	m_networkStarted = false;
	m_networkRun = 0U;
}

void CSoundCardRepeaterTRXThread::transmitStatus()
{
	CHeaderData header(m_rptCallsign, wxT("    "), m_header->getMyCall1(), m_gwyCallsign, m_rptCallsign, RELAY_UNAVAILABLE);
	transmitLocalHeader(header);

	// Filler data
	for (unsigned int i = 0U; i < 21U; i++) {
		if (i == 0U) {
			m_ackEncoder.sync();
			m_localBuffer.addData(NULL_AMBE_DATA_BITS, VOICE_FRAME_LENGTH_BITS);
			m_localBuffer.addData(DATA_SYNC_BITS,      DATA_FRAME_LENGTH_BITS);
		} else {
			unsigned char text[DATA_FRAME_LENGTH_BYTES];
			m_ackEncoder.getTextData(text);

			bool bits[DATA_FRAME_LENGTH_BITS];
			CUtils::byteToBitsRev(text[0U], bits + 0U);
			CUtils::byteToBitsRev(text[1U], bits + 8U);
			CUtils::byteToBitsRev(text[2U], bits + 16U);

			m_localBuffer.addData(NULL_AMBE_DATA_BITS, VOICE_FRAME_LENGTH_BITS);
			m_localBuffer.addData(bits,                DATA_FRAME_LENGTH_BITS);
		}
	}

	m_localBuffer.addData(END_PATTERN_BITS, END_PATTERN_LENGTH_BITS);
	m_localBuffer.addData(END_PATTERN_BITS, END_PATTERN_LENGTH_BITS);
	m_localBuffer.addData(END_PATTERN_BITS, END_PATTERN_LENGTH_BITS);
}

void CSoundCardRepeaterTRXThread::transmitErrorStatus()
{
	CHeaderData header(m_rptCallsign, wxT("    "), m_header->getMyCall1(), m_rptCallsign, m_rptCallsign, RELAY_UNAVAILABLE);
	transmitLocalHeader(header);

	// Filler data
	for (unsigned int i = 0U; i < 21U; i++) {
		if (i == 0U) {
			m_linkEncoder.sync();
			m_localBuffer.addData(NULL_AMBE_DATA_BITS, VOICE_FRAME_LENGTH_BITS);
			m_localBuffer.addData(DATA_SYNC_BITS,      DATA_FRAME_LENGTH_BITS);
		} else {
			unsigned char text[DATA_FRAME_LENGTH_BYTES];
			m_linkEncoder.getTextData(text);

			bool bits[DATA_FRAME_LENGTH_BITS];
			CUtils::byteToBitsRev(text[0U], bits + 0U);
			CUtils::byteToBitsRev(text[1U], bits + 8U);
			CUtils::byteToBitsRev(text[2U], bits + 16U);

			m_localBuffer.addData(NULL_AMBE_DATA_BITS, VOICE_FRAME_LENGTH_BITS);
			m_localBuffer.addData(bits,                DATA_FRAME_LENGTH_BITS);
		}
	}

	m_localBuffer.addData(END_PATTERN_BITS, END_PATTERN_LENGTH_BITS);
	m_localBuffer.addData(END_PATTERN_BITS, END_PATTERN_LENGTH_BITS);
	m_localBuffer.addData(END_PATTERN_BITS, END_PATTERN_LENGTH_BITS);
}

void CSoundCardRepeaterTRXThread::transmitUserStatus(unsigned int n)
{
	CSlowDataEncoder* encoder = NULL;
	switch (n) {
		case 0U: {
				CHeaderData header(m_rptCallsign, wxT("    "), wxT("STATUS 1"), m_gwyCallsign, m_rptCallsign);
				transmitLocalHeader(header);
				encoder = &m_status1Encoder;
			}
			break;
		case 1U: {
				CHeaderData header(m_rptCallsign, wxT("    "), wxT("STATUS 2"), m_gwyCallsign, m_rptCallsign);
				transmitLocalHeader(header);
				encoder = &m_status2Encoder;
			}
			break;
		case 2U: {
				CHeaderData header(m_rptCallsign, wxT("    "), wxT("STATUS 3"), m_gwyCallsign, m_rptCallsign);
				transmitLocalHeader(header);
				encoder = &m_status3Encoder;
			}
			break;
		case 3U: {
				CHeaderData header(m_rptCallsign, wxT("    "), wxT("STATUS 4"), m_gwyCallsign, m_rptCallsign);
				transmitLocalHeader(header);
				encoder = &m_status4Encoder;
			}
			break;
		case 4U: {
				CHeaderData header(m_rptCallsign, wxT("    "), wxT("STATUS 5"), m_gwyCallsign, m_rptCallsign);
				transmitLocalHeader(header);
				encoder = &m_status5Encoder;
			}
			break;
		default:
			wxLogWarning(wxT("Invalid status number - %u"), n);
			return;
	}

	// Filler data
	for (unsigned int i = 0U; i < 21U; i++) {
		if (i == 0U) {
			encoder->sync();
			m_localBuffer.addData(NULL_AMBE_DATA_BITS, VOICE_FRAME_LENGTH_BITS);
			m_localBuffer.addData(DATA_SYNC_BITS,      DATA_FRAME_LENGTH_BITS);
		} else {
			unsigned char text[DATA_FRAME_LENGTH_BYTES];
			encoder->getTextData(text);

			bool bits[DATA_FRAME_LENGTH_BITS];
			CUtils::byteToBitsRev(text[0U], bits + 0U);
			CUtils::byteToBitsRev(text[1U], bits + 8U);
			CUtils::byteToBitsRev(text[2U], bits + 16U);

			m_localBuffer.addData(NULL_AMBE_DATA_BITS, VOICE_FRAME_LENGTH_BITS);
			m_localBuffer.addData(bits,                DATA_FRAME_LENGTH_BITS);
		}
	}

	m_localBuffer.addData(END_PATTERN_BITS, END_PATTERN_LENGTH_BITS);
	m_localBuffer.addData(END_PATTERN_BITS, END_PATTERN_LENGTH_BITS);
	m_localBuffer.addData(END_PATTERN_BITS, END_PATTERN_LENGTH_BITS);
}

void CSoundCardRepeaterTRXThread::transmitLocal()
{
	if (m_localBuffer.isEmpty() && m_outBuffer.isEmpty()) {
#if defined(TX_TO_WAV_FILE)
		if (m_writer != NULL) {
			wxLogMessage(wxT("Closing the output WAV file"));
			m_writer->close();
			delete m_writer;
			m_writer = NULL;
		}
#endif
		m_localStarted = false;

		// Switch off the transmitter?
		if (m_tx) {
			// If no hang time, switch off the TX immediately
			if (m_hangTimer.getTimeout() == 0U) {
				m_tx = false;
			} else {
				// Start the hang timer if not already running
				if (!m_hangTimer.isRunning())
					m_hangTimer.start();

				if (m_hangTimer.isRunning() && m_hangTimer.hasExpired()) {
					m_hangTimer.stop();
					m_tx = false;
				}
			}
		}
	} else if (!m_localBuffer.isEmpty() && m_outBuffer.isEmpty()) {
		// Switch on the transmitter
		m_localStarted = true;
		m_tx = true;

#if defined(TX_TO_WAV_FILE)
		wxDateTime time;
		time.SetToCurrent();

		// This is for logging the output audio to a WAV file
		wxString fileName = time.Format(wxT("%Y%m%d-%H%M%S.wav"));
		CWAVFileWriter* writer = new CWAVFileWriter(fileName, DSTAR_RADIO_SAMPLE_RATE, 1U, 16U, DSTAR_RADIO_BLOCK_SIZE);

		bool res = writer->open();
		if (!res) {
			wxLogError(wxT("Cannot open the output WAV file - %s"), fileName.c_str());
			delete writer;
		} else {
			wxLogMessage(wxT("Opened the output WAV file - %s"), fileName.c_str());
			m_writer = writer;
		}
#endif
		// Put quite a lot of data into the buffer to allow for slow downs
		bool bits[DV_FRAME_LENGTH_BITS * 3U];
		unsigned int length = m_localBuffer.getData(bits, DV_FRAME_LENGTH_BITS * 3U);

		transmitBits(bits, length);
	} else if (!m_localBuffer.isEmpty() && !m_outBuffer.isEmpty()) {
		m_localStarted = true;
		m_tx = true;

		bool bits[DV_FRAME_LENGTH_BITS];
		unsigned int length = m_localBuffer.getData(bits, DV_FRAME_LENGTH_BITS);

		transmitBits(bits, length);
	} else {	// m_localBuffer.isEmpty() && !m_outBuffer.isEmpty()
		m_localStarted = true;
		m_tx = true;
	}

	if (m_pttDelay != NULL)
		m_controller->setRadioTransmit(m_pttDelay->delay(m_tx));
	else
		m_controller->setRadioTransmit(m_tx);
}

void CSoundCardRepeaterTRXThread::transmitRadio()
{
	if (m_radioBuffer.isEmpty() && m_outBuffer.isEmpty()) {
#if defined(TX_TO_WAV_FILE)
		if (m_writer != NULL) {
			wxLogMessage(wxT("Closing the output WAV file"));
			m_writer->close();
			delete m_writer;
			m_writer = NULL;
		}
#endif
		m_radioStarted = false;

		// Switch off the transmitter?
		if (m_tx) {
			// If no hang time, switch off the TX immediately
			if (m_hangTimer.getTimeout() == 0U) {
				m_tx = false;
			} else {
				// Start the hang timer if not already running
				if (!m_hangTimer.isRunning())
					m_hangTimer.start();

				if (m_hangTimer.isRunning() && m_hangTimer.hasExpired()) {
					m_hangTimer.stop();
					m_tx = false;
				}
			}
		}
	} else if (!m_radioBuffer.isEmpty() && m_outBuffer.isEmpty()) {
		// Switch on the transmitter
		m_radioStarted = true;
		m_tx = true;

#if defined(TX_TO_WAV_FILE)
		wxDateTime time;
		time.SetToCurrent();

		// This is for logging the output audio to a WAV file
		wxString fileName = time.Format(wxT("%Y%m%d-%H%M%S.wav"));
		CWAVFileWriter* writer = new CWAVFileWriter(fileName, DSTAR_RADIO_SAMPLE_RATE, 1U, 16U, DSTAR_RADIO_BLOCK_SIZE);

		bool res = writer->open();
		if (!res) {
			wxLogError(wxT("Cannot open the output WAV file - %s"), fileName.c_str());
			delete writer;
		} else {
			wxLogMessage(wxT("Opened the output WAV file - %s"), fileName.c_str());
			m_writer = writer;
		}
#endif
		// Put quite a lot of data into the buffer to allow for slow downs
		bool bits[DV_FRAME_LENGTH_BITS * 3U];
		unsigned int length = m_radioBuffer.getData(bits, DV_FRAME_LENGTH_BITS * 3U);

		transmitBits(bits, length);
	} else if (!m_radioBuffer.isEmpty() && !m_outBuffer.isEmpty()) {
		m_radioStarted = true;
		m_tx = true;

		bool bits[DV_FRAME_LENGTH_BITS];
		unsigned int length = m_radioBuffer.getData(bits, DV_FRAME_LENGTH_BITS);

		transmitBits(bits, length);
	} else {	// m_radioBuffer.isEmpty() && !m_outBuffer.isEmpty()
		m_radioStarted = true;
		m_tx = true;
	}

	if (m_pttDelay != NULL)
		m_controller->setRadioTransmit(m_pttDelay->delay(m_tx));
	else
		m_controller->setRadioTransmit(m_tx);
}

void CSoundCardRepeaterTRXThread::transmitNetwork()
{
	if (m_networkBuffer.isEmpty() && m_outBuffer.isEmpty()) {
#if defined(TX_TO_WAV_FILE)
		if (m_writer != NULL) {
			wxLogMessage(wxT("Closing the output WAV file"));
			m_writer->close();
			delete m_writer;
			m_writer = NULL;
		}
#endif
		m_networkStarted = false;
		m_networkRun = 0U;

		// Switch off the transmitter?
		if (m_tx) {
			// If no hang time, switch off the TX immediately
			if (m_hangTimer.getTimeout() == 0U) {
				m_tx = false;
			} else {
				// Start the hang timer if not already running
				if (!m_hangTimer.isRunning())
					m_hangTimer.start();

				if (m_hangTimer.isRunning() && m_hangTimer.hasExpired()) {
					m_hangTimer.stop();
					m_tx = false;
				}
			}
		}
	} else if (!m_networkBuffer.isEmpty() && m_outBuffer.isEmpty()) {
		// Switch on the transmitter
		m_networkStarted = true;
		m_tx = true;

#if defined(TX_TO_WAV_FILE)
		wxDateTime time;
		time.SetToCurrent();

		// This is for logging the output audio to a WAV file
		wxString fileName = time.Format(wxT("%Y%m%d-%H%M%S.wav"));
		CWAVFileWriter* writer = new CWAVFileWriter(fileName, DSTAR_RADIO_SAMPLE_RATE, 1U, 16U, DSTAR_RADIO_BLOCK_SIZE);

		bool res = writer->open();
		if (!res) {
			wxLogError(wxT("Cannot open the output WAV file - %s"), fileName.c_str());
			delete writer;
		} else {
			wxLogMessage(wxT("Opened the output WAV file - %s"), fileName.c_str());
			m_writer = writer;
		}
#endif
		// Put quite a lot of data into the buffer to allow for slow downs
		bool bits[DV_FRAME_LENGTH_BITS * 3U];
		unsigned int length = m_networkBuffer.getData(bits, DV_FRAME_LENGTH_BITS * 3U);

		transmitBits(bits, length);
	} else if (!m_networkBuffer.isEmpty() && !m_outBuffer.isEmpty()) {
		m_networkStarted = true;
		m_tx = true;

		bool bits[DV_FRAME_LENGTH_BITS];
		unsigned int length = m_networkBuffer.getData(bits, DV_FRAME_LENGTH_BITS);

		transmitBits(bits, length);
	} else {	// m_networkBuffer.isEmpty() && !m_outBuffer.isEmpty()
		m_networkStarted = true;
		m_tx = true;
	}

	if (m_pttDelay != NULL)
		m_controller->setRadioTransmit(m_pttDelay->delay(m_tx));
	else
		m_controller->setRadioTransmit(m_tx);
}

void CSoundCardRepeaterTRXThread::transmit()
{
	// Switch off the transmitter?
	if (m_tx) {
		// If no hang time, switch off the TX immediately
		if (m_hangTimer.getTimeout() == 0U) {
			m_tx = false;
		} else {
			// Start the hang timer if not already running
			if (!m_hangTimer.isRunning())
				m_hangTimer.start();

			if (m_hangTimer.isRunning() && m_hangTimer.hasExpired()) {
				m_hangTimer.stop();
				m_tx = false;
			}
		}
	}

	if (m_pttDelay != NULL)
		m_controller->setRadioTransmit(m_pttDelay->delay(m_tx));
	else
		m_controller->setRadioTransmit(m_tx);
}

void CSoundCardRepeaterTRXThread::transmitBits(const bool* bits, unsigned int length)
{
	wxASSERT(bits != NULL);

	for (unsigned int i = 0U; i < length; i++) {
		wxFloat32 buffer[DSTAR_RADIO_BIT_LENGTH];
		unsigned int len = m_modulator.code(bits[i], buffer, DSTAR_RADIO_BIT_LENGTH);

#if defined(TX_TO_WAV_FILE)
		// If writing the transmit audio to a WAV file
		if (m_writer != NULL)
			m_writer->write(buffer, len);
#endif
		m_outBuffer.addData(buffer, len);
	}
}

void CSoundCardRepeaterTRXThread::radioStateMachine(bool bit)
{
	switch (m_rxState) {
		case DSRXS_LISTENING: {
				unsigned int errs1 = m_frameMatcher.add(bit);
				unsigned int errs2 = m_dataMatcher.add(bit);

				// The frame sync has been seen, an exact match only
				if (errs1 == 0U) {
					// wxLogMessage(wxT("Found frame sync"));
					setRadioState(DSRXS_PROCESS_HEADER);
				}

				// The data sync has been seen, an exact match only
				if (errs2 == 0U) {
					// wxLogMessage(wxT("Found data sync"));
					setRadioState(DSRXS_PROCESS_SLOW_DATA);
				}
			}
			break;

		case DSRXS_PROCESS_HEADER:
			m_bitBuffer.add(bit);
			if (m_bitBuffer.getLength() == FEC_SECTION_LENGTH_BITS) {
				CHeaderData* header = processFECHeader();

				if (header != NULL) {
					bool res = processRadioHeader(header);
					if (res) {
						// A valid header and is a DV packet
						m_radioSeqNo     = 20U;
						m_radioSyncsLost = 0U;
						setRadioState(DSRXS_PROCESS_DATA);
					} else {
						// This is a DD packet or some other problem
						// wxLogMessage(wxT("Invalid header"));
						setRadioState(DSRXS_LISTENING);
					}
				} else {
					// The checksum failed
					setRadioState(DSRXS_LISTENING);
				}
			}
			break;

		case DSRXS_PROCESS_DATA: {
				m_bitBuffer.add(bit);
				unsigned int errs1 = m_endMatcher.add(bit);
				unsigned int errs2 = m_dataMatcher.add(bit);

				// The end pattern has been seen, a fuzzy match is used, four bit errors or less
				if (errs1 <= MAX_END_PATTERN_BIT_ERRS) {
					// wxLogMessage(wxT("Found end pattern, errs: %u"), errs1);
					processRadioFrame(FRAME_END);
					setRadioState(DSRXS_LISTENING);
					endOfRadioData();
					break;
				}

				if (m_bitBuffer.getLength() == DV_FRAME_LENGTH_BITS) {
					// The squelch is closed so replace the data with silence
					if (m_squelchCount >= MAX_SQUELCH_COUNT) {
						m_bitBuffer.clear();

						// Add AMBE silence and slow data
						for (unsigned int i = 0U; i < DV_FRAME_LENGTH_BITS; i++)
							m_bitBuffer.add(NULL_FRAME_DATA_BITS[i]);
					}

					// The data sync has been seen, a fuzzy match is used, two bit errors or less
					if (errs2 <= MAX_DATA_SYNC_BIT_ERRS) {
						// wxLogMessage(wxT("Found data sync at frame %u, errs: %u"), m_radioSeqNo, errs2);
						m_radioSeqNo     = 0U;
						m_radioSyncsLost = 0U;
						processRadioFrame(FRAME_SYNC);
					} else if (m_radioSeqNo == 20U) {
						// wxLogMessage(wxT("Regenerating data sync"));
						m_radioSeqNo = 0U;
						m_radioSyncsLost++;
						if (m_radioSyncsLost == MAX_LOST_SYNCS) {
							// wxLogMessage(wxT("Lost sync"));
							processRadioFrame(FRAME_END);
							setRadioState(DSRXS_LISTENING);
							endOfRadioData();
						} else {
							processRadioFrame(FRAME_SYNC);
						}
					} else {
						m_radioSeqNo++;
						processRadioFrame(FRAME_NORMAL);
					}

					m_squelchCount = 0U;
					m_bitBuffer.clear();
				}
			}
			break;

		case DSRXS_PROCESS_SLOW_DATA: {
				m_bitBuffer.add(bit);
				unsigned int errs1 = m_endMatcher.add(bit);
				unsigned int errs2 = m_dataMatcher.add(bit);

				// The end pattern has been seen, a fuzzy match is used, four bit errors or less
   				if (errs1 <= MAX_END_PATTERN_BIT_ERRS) {
					// wxLogMessage(wxT("Found end pattern, errs: %u"), errs1);
					setRadioState(DSRXS_LISTENING);
					break;
				}

				if (m_bitBuffer.getLength() == DV_FRAME_LENGTH_BITS) {
					// The squelch is closed so abort the slow data search
					if (m_squelchCount >= MAX_SQUELCH_COUNT) {
						setRadioState(DSRXS_LISTENING);
						break;
					}

					// The data sync has been seen, a fuzzy match is used, two bit errors or less
					if (errs2 <= MAX_DATA_SYNC_BIT_ERRS) {
						// wxLogMessage(wxT("Found data sync at frame %u, errs: %u"), m_radioSeqNo, errs2);
						m_radioSeqNo     = 0U;
						m_radioSyncsLost = 0U;
						processSlowData(true);
					} else if (m_radioSeqNo == 20U) {
						// wxLogMessage(wxT("Assuming data sync"));
						m_radioSeqNo = 0U;
						m_radioSyncsLost++;
						if (m_radioSyncsLost == MAX_LOST_SYNCS) {
							// wxLogMessage(wxT("Lost sync"));
							setRadioState(DSRXS_LISTENING);
						} else {
							processSlowData(true);
						}
					} else {
						m_radioSeqNo++;
						CHeaderData* header = processSlowData(false);

						if (header != NULL) {
							bool res = processRadioHeader(header);
							if (res) {
								// A valid header and is a DV packet, go to normal data relaying
								setRadioState(DSRXS_PROCESS_DATA);
							} else {
								// This is a DD packet or some other problem
								// wxLogMessage(wxT("Invalid header"));
							}
						}
					}

					m_squelchCount = 0U;
					m_bitBuffer.clear();
				}
			}
			break;
	}
}

void CSoundCardRepeaterTRXThread::repeaterStateMachine()
{
	switch (m_rptState) {
		case DSRS_VALID:
			if (m_timeoutTimer.isRunning() && m_timeoutTimer.hasExpired()) {
				wxLogMessage(wxT("User has timed out"));
				setRepeaterState(DSRS_TIMEOUT);
			}
			break;

		case DSRS_VALID_WAIT:
			if (m_ackTimer.hasExpired()) {
				if (m_mode != MODE_GATEWAY)
					transmitStatus();
				setRepeaterState(DSRS_LISTENING);
				m_activeHangTimer.start();
			}
			break;

		case DSRS_INVALID_WAIT:
			if (m_ackTimer.hasExpired()) {
				if (m_mode != MODE_GATEWAY)
					transmitErrorStatus();
				setRepeaterState(DSRS_LISTENING);
				m_activeHangTimer.start();
			}
			break;

		case DSRS_TIMEOUT_WAIT:
			if (m_ackTimer.hasExpired()) {
				if (m_mode != MODE_GATEWAY)
					transmitStatus();
				setRepeaterState(DSRS_LISTENING);
				m_activeHangTimer.start();
			}
			break;

		case DSRS_NETWORK:
			if (m_watchdogTimer.hasExpired()) {
				wxLogMessage(wxT("Network watchdog has expired"));
				// Send end of transmission data to the radio
				m_networkBuffer.addData(END_PATTERN_BITS, END_PATTERN_LENGTH_BITS);
				m_networkBuffer.addData(END_PATTERN_BITS, END_PATTERN_LENGTH_BITS);
				m_networkBuffer.addData(END_PATTERN_BITS, END_PATTERN_LENGTH_BITS);
				endOfNetworkData();
			}
			break;

		default:
			break;
	}
}

bool CSoundCardRepeaterTRXThread::setRepeaterState(DSTAR_RPT_STATE state)
{
	// Can't change state when shutdown
	if (m_disable || m_shutdown)
		return false;

	// The "from" state
	switch (m_rptState) {
		case DSRS_SHUTDOWN:
			m_beaconTimer.start();
			m_announcementTimer.start();
			break;

		case DSRS_LISTENING:
			m_beaconTimer.stop();
			break;

		default:
			break;
	}

	// The "to" state
	switch (state) {
		case DSRS_SHUTDOWN:
			m_timeoutTimer.stop();
			m_watchdogTimer.stop();
			m_activeHangTimer.stop();
			m_ackTimer.stop();
			m_beaconTimer.stop();
			m_announcementTimer.stop();
			m_radioBuffer.clear();
			m_localBuffer.clear();
			m_networkBuffer.clear();
			m_networkRun = 0U;
			m_radioStarted = false;
			m_localStarted = false;
			m_networkStarted = false;
			m_controller->setActive(false);
			m_controller->setRadioTransmit(false);
			m_rptState = DSRS_SHUTDOWN;
			break;

		case DSRS_LISTENING:
			m_timeoutTimer.stop();
			m_watchdogTimer.stop();
			m_ackTimer.stop();
			m_beaconTimer.start();
			m_rptState = DSRS_LISTENING;
			if (m_protocolHandler != NULL)	// Tell the protocol handler
				m_protocolHandler->reset();
			break;

		case DSRS_VALID:
			if (m_rptState != DSRS_LISTENING && m_rptState != DSRS_VALID_WAIT)
				return false;

			if (m_rptState == DSRS_LISTENING)
				m_timeoutTimer.start();
			else
				m_ackTimer.stop();

			m_activeHangTimer.stop();
			m_rptState = DSRS_VALID;
			break;

		case DSRS_VALID_WAIT:
			m_ackTimer.start();
			m_rptState = DSRS_VALID_WAIT;
			break;

		case DSRS_TIMEOUT:
			// Send end of transmission data to the radio and the network
			if (m_mode == MODE_DUPLEX) {
				m_radioBuffer.addData(END_PATTERN_BITS, END_PATTERN_LENGTH_BITS);
				m_radioBuffer.addData(END_PATTERN_BITS, END_PATTERN_LENGTH_BITS);
				m_radioBuffer.addData(END_PATTERN_BITS, END_PATTERN_LENGTH_BITS);
			}

			if (!m_blocked && m_protocolHandler != NULL) {
				unsigned char bytes[DV_FRAME_MAX_LENGTH_BYTES];
				::memcpy(bytes, NULL_AMBE_DATA_BYTES, VOICE_FRAME_LENGTH_BYTES);
				::memcpy(bytes + VOICE_FRAME_LENGTH_BYTES, END_PATTERN_BYTES, END_PATTERN_LENGTH_BYTES);
				m_protocolHandler->writeData(bytes, DV_FRAME_MAX_LENGTH_BYTES, 0U, true);
			}

			m_timeoutTimer.stop();
			m_rptState = DSRS_TIMEOUT;
			break;

		case DSRS_TIMEOUT_WAIT:
			m_ackTimer.start();
			m_timeoutTimer.stop();
			m_rptState = DSRS_TIMEOUT_WAIT;
			break;

		case DSRS_INVALID:
			if (m_rptState != DSRS_LISTENING)
				return false;

			m_activeHangTimer.stop();
			m_timeoutTimer.stop();
			m_rptState = DSRS_INVALID;
			break;

		case DSRS_INVALID_WAIT:
			m_ackTimer.start();
			m_timeoutTimer.stop();
			m_rptState = DSRS_INVALID_WAIT;
			break;

		case DSRS_NETWORK:
			if (m_rptState != DSRS_LISTENING && m_rptState != DSRS_VALID_WAIT && m_rptState != DSRS_INVALID_WAIT && m_rptState != DSRS_TIMEOUT_WAIT)
				return false;

			m_rptState = DSRS_NETWORK;
			m_networkSeqNo = 0U;
			m_activeHangTimer.stop();
			m_timeoutTimer.stop();
			m_watchdogTimer.start();
			m_ackTimer.stop();
			break;
	}

	return true;
}

CHeaderData* CSoundCardRepeaterTRXThread::processFECHeader()
{
	bool buffer[FEC_SECTION_LENGTH_BITS];
	m_bitBuffer.getData(buffer, FEC_SECTION_LENGTH_BITS);

	CRadioHeaderDecoder decoder(buffer, FEC_SECTION_LENGTH_BITS);

	CHeaderData* header = decoder.getHeaderData();
	if (header == NULL)
		return NULL;

	m_headerBER = decoder.getBER();

	wxLogMessage(wxT("Radio header decoded - My: %s/%s  Your: %s  Rpt1: %s  Rpt2: %s  Flags: %02X %02X %02X  BER: %u%%"), header->getMyCall1().c_str(), header->getMyCall2().c_str(), header->getYourCall().c_str(), header->getRptCall1().c_str(), header->getRptCall2().c_str(), header->getFlag1(), header->getFlag2(), header->getFlag3(), m_headerBER);

	return header;
}

bool CSoundCardRepeaterTRXThread::processRadioHeader(CHeaderData* header)
{
	wxASSERT(header != NULL);

	// Check control messages
	bool res = checkControl(*header);
	if (res) {
		delete header;
		return true;
	}

	// Check announcement messages
	res = checkAnnouncements(*header);
	if (res) {
		bool res = setRepeaterState(DSRS_INVALID);
		if (res) {
			delete m_header;
			m_header = header;
		} else {
			delete header;
		}
		return true;
	}

	// If shutdown we ignore incoming headers
	if (m_rptState == DSRS_SHUTDOWN) {
		delete header;
		return true;
	}

	if (m_blackList != NULL) {
		bool res = m_blackList->isInList(header->getMyCall1());
		if (res) {
			wxLogMessage(wxT("%s rejected due to being in the black list"), header->getMyCall1().c_str());
			delete header;
			return true;
		}
	}

	m_blocked = false;
	if (m_greyList != NULL) {
		bool res = m_greyList->isInList(header->getMyCall1());
		if (res) {
			wxLogMessage(wxT("%s blocked from the network due to being in the grey list"), header->getMyCall1().c_str());
			m_blocked = true;
		}
	}

	// Check for receiving our own gateway data, and ignore it
	if (m_mode == MODE_GATEWAY) {
		if (header->getFlag2() == 0x01U) {
			wxLogMessage(wxT("Receiving a gateway header, ignoring"));
			delete header;
			return true;
		}

		header->setFlag2(0x00U);
	}

	// We don't handle DD data packets
	if (header->isDataPacket()) {
		wxLogMessage(wxT("Received a DD packet, ignoring"));
		delete header;
		return false;
	}

	TRISTATE valid = checkHeader(*header);
	switch (valid) {
		case STATE_FALSE: {
				bool res = setRepeaterState(DSRS_INVALID);
				if (res) {
					delete m_header;
					m_header = header;
				} else {
					delete header;
				}
			}
			return true;

		case STATE_UNKNOWN:
			delete header;
			return true;

		case STATE_TRUE:
			break;
	}

	// If we're in network mode, send the header as a busy header to the gateway in case it's an unlink
	// command
	if (m_rptState == DSRS_NETWORK) {
		// Only send on the network if the user isn't blocked and we have one and RPT2 is not blank or the repeater callsign
		if (!header->getRptCall2().IsSameAs(wxT("        ")) && !header->getRptCall2().IsSameAs(m_rptCallsign)) {
			if (!m_blocked && m_protocolHandler != NULL) {
				CHeaderData netHeader(*header);
				netHeader.setRptCall1(header->getRptCall2());
				netHeader.setRptCall2(header->getRptCall1());
				netHeader.setFlag1(header->getFlag1() & ~REPEATER_MASK);
				m_protocolHandler->writeBusyHeader(netHeader);
			}

			m_busyData = true;
		}

		delete header;

		return true;
	}

	// Send the valid header to the gateway if we are accepted
	res = setRepeaterState(DSRS_VALID);
	if (res) {
		delete m_header;
		m_header = header;

		if (m_logging != NULL)
			m_logging->open(*m_header);

		// Only send on the network if the user isn't blocked and we have one and RPT2 is not blank or the repeater callsign
		if (!m_blocked && m_protocolHandler != NULL && !m_header->getRptCall2().IsSameAs(wxT("        ")) && !m_header->getRptCall2().IsSameAs(m_rptCallsign)) {
			CHeaderData netHeader(*m_header);
			netHeader.setRptCall1(m_header->getRptCall2());
			netHeader.setRptCall2(m_header->getRptCall1());
			netHeader.setFlag1(m_header->getFlag1() & ~REPEATER_MASK);
			m_protocolHandler->writeHeader(netHeader);
		}

		// Create the new radio header and transmit it, but only in duplex mode
		if (m_mode == MODE_DUPLEX) {
			CHeaderData rfHeader(*m_header);
			rfHeader.setRptCall1(m_rptCallsign);
			rfHeader.setRptCall2(m_rptCallsign);
			rfHeader.setFlag1(m_header->getFlag1() & ~REPEATER_MASK);
			transmitRadioHeader(rfHeader);
		}
	}

	return true;
}

void CSoundCardRepeaterTRXThread::processNetworkHeader(CHeaderData* header)
{
	wxASSERT(header != NULL);

	// If shutdown we ignore incoming headers
	if (m_rptState == DSRS_SHUTDOWN) {
		delete header;
		return;
	}

	wxLogMessage(wxT("Network header received - My: %s/%s  Your: %s  Rpt1: %s  Rpt2: %s  Flags: %02X %02X %02X"), header->getMyCall1().c_str(), header->getMyCall2().c_str(), header->getYourCall().c_str(), header->getRptCall1().c_str(), header->getRptCall2().c_str(), header->getFlag1(), header->getFlag2(), header->getFlag3());

	// Is it for us?
	if (!header->getRptCall2().IsSameAs(m_rptCallsign)) {
		wxLogMessage(wxT("Invalid network RPT2 value, ignoring"));
		delete header;
		return;
	}

	bool res = setRepeaterState(DSRS_NETWORK);
	if (!res) {
		delete header;
		return;
	}

	delete m_header;
	m_header = header;

	if (m_mode == MODE_GATEWAY) {
		// If in gateway mode, set the repeater bit , set flag 2 to 0x01,
		// and change RPT1 & RPT2 just for transmission
		CHeaderData txHeader(*m_header);
		txHeader.setRepeaterMode(true);
		txHeader.setFlag2(0x01U);
		txHeader.setRptCall1(m_rptCallsign);
		txHeader.setRptCall2(m_gwyCallsign);
		transmitNetworkHeader(txHeader);
	} else {
		CHeaderData header(*m_header);
		transmitNetworkHeader(header);
	}
}

void CSoundCardRepeaterTRXThread::processRadioFrame(FRAME_TYPE type)
{
	bool bits[DV_FRAME_LENGTH_BITS];
	m_bitBuffer.getData(bits, DV_FRAME_LENGTH_BITS);

	m_ambeFrames++;

	// If a sync frame, regenerate the sync bytes (bits)
	if (type == FRAME_SYNC)
		::memcpy(bits + VOICE_FRAME_LENGTH_BITS, DATA_SYNC_BITS, DATA_FRAME_LENGTH_BITS * sizeof(bool));

	// Only regenerate the AMBE on received radio data
	unsigned int errors = 0U;
	if (type != FRAME_END) {
		errors = m_ambe.regenerate(bits);
		m_ambeErrors += errors;
		m_ambeBits   += 48U;		// Only count the bits with FEC added
	}

	if (::memcmp(bits, NULL_AMBE_DATA_BITS, VOICE_FRAME_LENGTH_BITS * sizeof(bool)) == 0)
		m_ambeSilence++;

	// If this is deleting an announcement, ignore the audio
	if (m_deleting) {
		if (type == FRAME_END) {
			m_deleting  = false;
			m_recording = false;
		}
		return;
	}

	// If this is recording an announcement, send the audio to the announcement unit and then stop
	if (m_recording) {
		// Convert the bits to bytes for recording
		unsigned char bytes[DV_FRAME_LENGTH_BYTES];
		unsigned int n = 0U;
		for (unsigned int i = 0U; i < DV_FRAME_LENGTH_BYTES; i++, n += 8U)
			bytes[i] = CUtils::bitsToByteRev(bits + n);

		m_announcement->writeData(bytes, DV_FRAME_LENGTH_BYTES, type == FRAME_END);

		if (type == FRAME_END) {
			m_deleting  = false;
			m_recording = false;
		}
		return;
	}

	// Pass background AMBE data to the network
	if (m_busyData) {
		if (type == FRAME_END) {
			if (!m_blocked && m_protocolHandler != NULL) {
				unsigned char bytes[DV_FRAME_MAX_LENGTH_BYTES];
				::memcpy(bytes, NULL_AMBE_DATA_BYTES, VOICE_FRAME_LENGTH_BYTES);
				::memcpy(bytes + VOICE_FRAME_LENGTH_BYTES, END_PATTERN_BYTES, END_PATTERN_LENGTH_BYTES);
				m_protocolHandler->writeBusyData(bytes, DV_FRAME_MAX_LENGTH_BYTES, 0U, true);
			}

			m_busyData = false;
		} else {
			if (!m_blocked && m_protocolHandler != NULL) {
				// Convert the bits to bytes for the network
				unsigned char bytes[DV_FRAME_LENGTH_BYTES];
				unsigned int n = 0U;
				for (unsigned int i = 0U; i < DV_FRAME_LENGTH_BYTES; i++, n += 8U)
					bytes[i] = CUtils::bitsToByteRev(bits + n);
				m_protocolHandler->writeBusyData(bytes, DV_FRAME_LENGTH_BYTES, errors, false);
			}
		}

		return;
	}

	// Don't pass through the frame of an invalid transmission
	if (m_rptState != DSRS_VALID)
		return;

	if (type == FRAME_END) {
		if (m_logging != NULL)
			m_logging->close();

		// Transmit the end sync on the radio, but only in duplex mode
		if (m_mode == MODE_DUPLEX) {
			m_radioBuffer.addData(bits, DV_FRAME_LENGTH_BITS);
			m_radioBuffer.addData(END_PATTERN_BITS, END_PATTERN_LENGTH_BITS);
			m_radioBuffer.addData(END_PATTERN_BITS, END_PATTERN_LENGTH_BITS);
			m_radioBuffer.addData(END_PATTERN_BITS, END_PATTERN_LENGTH_BITS);
		}

		// Send null data and the end marker over the network
		if (!m_blocked && m_protocolHandler != NULL) {
			unsigned char bytes[DV_FRAME_MAX_LENGTH_BYTES];
			::memcpy(bytes, NULL_AMBE_DATA_BYTES, VOICE_FRAME_LENGTH_BYTES);
			::memcpy(bytes + VOICE_FRAME_LENGTH_BYTES, END_PATTERN_BYTES, END_PATTERN_LENGTH_BYTES);
			m_protocolHandler->writeData(bytes, DV_FRAME_MAX_LENGTH_BYTES, 0U, true);
		}
	} else {
		// Convert the bits to bytes for the network
		unsigned char bytes[DV_FRAME_LENGTH_BYTES];
		unsigned int n = 0U;
		for (unsigned int i = 0U; i < DV_FRAME_LENGTH_BYTES; i++, n += 8U)
			bytes[i] = CUtils::bitsToByteRev(bits + n);

		// Send the data to the network
		if (!m_blocked && m_protocolHandler != NULL)
			m_protocolHandler->writeData(bytes, DV_FRAME_LENGTH_BYTES, errors, false);

		if (m_logging != NULL)
			m_logging->write(bytes, DV_FRAME_LENGTH_BYTES);

		// Send the data for transmission, but only in duplex mode
		if (m_mode == MODE_DUPLEX) {
			if (m_blanking)
				blankDTMF(bits);
			m_radioBuffer.addData(bits, DV_FRAME_LENGTH_BITS);
		}
	}
}

unsigned int CSoundCardRepeaterTRXThread::processNetworkFrame(unsigned char* data, unsigned int length, unsigned char seqNo)
{
	wxASSERT(data != NULL);
	wxASSERT(length > 0U);

	if (m_rptState != DSRS_NETWORK)
		return 0U;

	bool end = (seqNo & 0x40U) == 0x40U;
	if (end) {
		// Convert the bytes to bits for transmission
		bool bits[DV_FRAME_LENGTH_BITS * 2U];
		unsigned int n = 0U;
		for (unsigned int i = 0U; i < length; i++, n += 8U)
			CUtils::byteToBitsRev(data[i], bits + n);

		m_networkBuffer.addData(bits, length * 8U);
		m_networkBuffer.addData(END_PATTERN_BITS, END_PATTERN_LENGTH_BITS);
		m_networkBuffer.addData(END_PATTERN_BITS, END_PATTERN_LENGTH_BITS);
		m_networkBuffer.addData(END_PATTERN_BITS, END_PATTERN_LENGTH_BITS);

		endOfNetworkData();
		return 1U;
	}

	// Mask out the control bits of the sequence number
	seqNo &= 0x1FU;

	// Count the number of silence frames to insert
	unsigned int tempSeqNo = m_networkSeqNo;
	unsigned int count = 0U;
	while (seqNo != tempSeqNo) {
		count++;

		tempSeqNo++;
		if (tempSeqNo >= 21U)
			tempSeqNo = 0U;
	}

	// If the number is too high, then it probably means an out-of-order frame, ignore it
	if (count > 18U)
		return 0U;

	unsigned int packetCount = 0U;

	// Insert missing frames
	while (seqNo != m_networkSeqNo) {
		if (count > SILENCE_THRESHOLD) {
			m_networkBuffer.addData(NULL_AMBE_DATA_BITS, VOICE_FRAME_LENGTH_BITS);
		} else {
			unsigned char buffer[DV_FRAME_LENGTH_BYTES];
			::memcpy(buffer, m_lastData, DV_FRAME_LENGTH_BYTES);

			m_ambe.regenerate(buffer);
			blankDTMF(buffer);

			// Convert the bytes to bits for transmission
			bool bits[VOICE_FRAME_LENGTH_BITS * 2U];
			unsigned int n = 0U;
			for (unsigned int i = 0U; i < VOICE_FRAME_LENGTH_BYTES; i++, n += 8U)
				CUtils::byteToBitsRev(buffer[i], bits + n);

			m_networkBuffer.addData(bits, VOICE_FRAME_LENGTH_BITS);
		}

		if (m_networkSeqNo == 0U)
			m_networkBuffer.addData(DATA_SYNC_BITS, DATA_FRAME_LENGTH_BITS);
		else
			m_networkBuffer.addData(NULL_SLOW_DATA_BITS, DATA_FRAME_LENGTH_BITS);

		packetCount++;
		m_networkRun++;
		m_networkSeqNo++;
		m_packetSilence++;
		if (m_networkSeqNo >= 21U)
			m_networkSeqNo = 0U;
	}

	// Regenerate the sync bytes
	if (m_networkSeqNo == 0U) {
		::memcpy(data + VOICE_FRAME_LENGTH_BYTES, DATA_SYNC_BYTES, DATA_FRAME_LENGTH_BYTES);
		m_headerEncoder.sync();
	} else {
		// If in Gateway mode, replace the slow data with our modified header
		if (m_mode == MODE_GATEWAY) {
			unsigned char slowData[DATA_FRAME_LENGTH_BYTES];
			m_headerEncoder.getHeaderData(slowData);
			::memcpy(data + VOICE_FRAME_LENGTH_BYTES, slowData, DATA_FRAME_LENGTH_BYTES);
		}
	}

	packetCount++;
	m_networkRun++;
	m_networkSeqNo++;
	if (m_networkSeqNo >= 21U)
		m_networkSeqNo = 0U;

	m_ambe.regenerate(data);

	// Convert the bytes to bits for transmission
	bool bits[DV_FRAME_LENGTH_BITS * 2U];
	unsigned int n = 0U;
	for (unsigned int i = 0U; i < length; i++, n += 8U)
		CUtils::byteToBitsRev(data[i], bits + n);

	blankDTMF(bits);

	m_networkBuffer.addData(bits, length * 8U);

	return packetCount;
}

CHeaderData* CSoundCardRepeaterTRXThread::processSlowData(bool sync)
{
	bool bits[DV_FRAME_LENGTH_BITS];
	m_bitBuffer.getData(bits, DV_FRAME_LENGTH_BITS);

	if (sync) {
		m_slowDataDecoder.sync();
		return NULL;
	}

	unsigned char bytes[DATA_FRAME_LENGTH_BYTES];

	// Convert the bits to bytes
	unsigned int n = 0U;
	for (unsigned int i = 0U; i < DATA_FRAME_LENGTH_BYTES; i++, n += 8U)
		bytes[i] = CUtils::bitsToByteRev(bits + VOICE_FRAME_LENGTH_BITS + n);

	m_slowDataDecoder.addData(bytes);

	CHeaderData* header = m_slowDataDecoder.getHeaderData();
	if (header == NULL)
		return NULL;

	wxLogMessage(wxT("Radio header from slow data - My: %s/%s  Your: %s  Rpt1: %s  Rpt2: %s  Flags: %02X %02X %02X  BER: 0%%"), header->getMyCall1().c_str(), header->getMyCall2().c_str(), header->getYourCall().c_str(), header->getRptCall1().c_str(), header->getRptCall2().c_str(), header->getFlag1(), header->getFlag2(), header->getFlag3());

	m_headerBER = 0U;

	return header;
}

void CSoundCardRepeaterTRXThread::endOfRadioData()
{
	switch (m_rptState) {
		case DSRS_VALID:
			wxLogMessage(wxT("AMBE for %s  Frames: %.1fs, Silence: %.1f%%, BER: %.1f%%"), m_header->getMyCall1().c_str(), float(m_ambeFrames) / 50.0F, float(m_ambeSilence * 100U) / float(m_ambeFrames), float(m_ambeErrors * 100U) / float(m_ambeBits));

			if (m_tempAckText.IsEmpty()) {
				if (m_ack == AT_BER) {
					// Create the ack text with the linked reflector and BER
					wxString ackText;
					if (m_linkStatus == LS_LINKED_DEXTRA || m_linkStatus == LS_LINKED_DPLUS || m_linkStatus == LS_LINKED_DCS || m_linkStatus == LS_LINKED_CCS || m_linkStatus == LS_LINKED_LOOPBACK)
						ackText.Printf(wxT("%-8s  BER: %.1f%%   "), m_reflector.c_str(), float(m_ambeErrors * 100U) / float(m_ambeBits));
					else
						ackText.Printf(wxT("BER: %.1f%%            "), float(m_ambeErrors * 100U) / float(m_ambeBits));
					m_ackEncoder.setTextData(ackText);
				} else {
					m_ackEncoder.setTextData(m_ackText);
				}
			} else {
				m_ackEncoder.setTextData(m_tempAckText);
				m_tempAckText.Clear();
			}

			if (m_ack != AT_NONE || m_mode == MODE_GATEWAY) {
				setRepeaterState(DSRS_VALID_WAIT);
			} else {
				setRepeaterState(DSRS_LISTENING);
				m_activeHangTimer.start();
			}
			break;

		case DSRS_INVALID:
			wxLogMessage(wxT("AMBE for %s  Frames: %.1fs, Silence: %.1f%%, BER: %.1f%%"), m_header->getMyCall1().c_str(), float(m_ambeFrames) / 50.0F, float(m_ambeSilence * 100U) / float(m_ambeFrames), float(m_ambeErrors * 100U) / float(m_ambeBits));

			if (m_ack != AT_NONE || m_mode == MODE_GATEWAY) {
				setRepeaterState(DSRS_INVALID_WAIT);
			} else {
				setRepeaterState(DSRS_LISTENING);
				m_activeHangTimer.start();
			}
			break;

		case DSRS_TIMEOUT:
			wxLogMessage(wxT("AMBE for %s  Frames: %.1fs, Silence: %.1f%%, BER: %.1f%%"), m_header->getMyCall1().c_str(), float(m_ambeFrames) / 50.0F, float(m_ambeSilence * 100U) / float(m_ambeFrames), float(m_ambeErrors * 100U) / float(m_ambeBits));

			if (m_tempAckText.IsEmpty()) {
				if (m_ack == AT_BER) {
					// Create the ack text with the linked reflector and BER
					wxString ackText;
					if (m_linkStatus == LS_LINKED_DEXTRA || m_linkStatus == LS_LINKED_DPLUS || m_linkStatus == LS_LINKED_DCS || m_linkStatus == LS_LINKED_CCS || m_linkStatus == LS_LINKED_LOOPBACK)
						ackText.Printf(wxT("%-8s  BER: %.1f%%   "), m_reflector.c_str(), float(m_ambeErrors * 100U) / float(m_ambeBits));
					else
						ackText.Printf(wxT("BER: %.1f%%            "), float(m_ambeErrors * 100U) / float(m_ambeBits));
					m_ackEncoder.setTextData(ackText);
				} else {
					m_ackEncoder.setTextData(m_ackText);
				}
			} else {
				m_ackEncoder.setTextData(m_tempAckText);
				m_tempAckText.Clear();
			}

			if (m_ack != AT_NONE || m_mode == MODE_GATEWAY) {
				setRepeaterState(DSRS_TIMEOUT_WAIT);
			} else {
				setRepeaterState(DSRS_LISTENING);
				m_activeHangTimer.start();
			}
			break;

		default:
			break;
	}
}

void CSoundCardRepeaterTRXThread::endOfNetworkData()
{
	if (m_watchdogTimer.hasExpired()) {
		m_packetCount   -= 77U;
		m_packetSilence -= 77U;
	}

	float loss = 0.0F;
	if (m_packetCount != 0U)
		loss = float(m_packetSilence) / float(m_packetCount);

	wxLogMessage(wxT("Stats for %s  Frames: %.1fs, Loss: %.1f%%, Packets: %u/%u"), m_header->getMyCall1().c_str(), float(m_packetCount) / 50.0F, loss * 100.0F, m_packetSilence, m_packetCount);

	setRepeaterState(DSRS_LISTENING);

	m_activeHangTimer.start();

	m_networkRun = NETWORK_RUN_FRAME_COUNT;
}

void CSoundCardRepeaterTRXThread::callback(const wxFloat32* input, wxFloat32* output, unsigned int nSamples, int id)
{
	::memset(output, 0x00, nSamples * sizeof(wxFloat32));

	if (!m_stopped) {
		readRadioData(input, nSamples);

		m_outBuffer.getData(output, nSamples);

		if (m_audioDelay != NULL)
			m_audioDelay->delay(output, nSamples);

		// Scale the output audio level
		for (unsigned int i = 0U; i < nSamples; i++)
			output[i] *= m_txLevel;
	}
}

void CSoundCardRepeaterTRXThread::setRadioState(DSTAR_RX_STATE state)
{
	// This is the too state
	switch (state) {
		case DSRXS_LISTENING:
			m_frameMatcher.reset();
			m_dataMatcher.reset();
			m_rxState = DSRXS_LISTENING;
			break;

		case DSRXS_PROCESS_HEADER:
			m_bitBuffer.setMaxLength(FEC_SECTION_LENGTH_BITS);
			m_rxState = DSRXS_PROCESS_HEADER;
			break;

		case DSRXS_PROCESS_DATA:
			m_bitBuffer.setMaxLength(DV_FRAME_LENGTH_BITS);
			m_endMatcher.reset();
			m_dataMatcher.reset();
			m_squelchCount   = 0U;
			m_ambeFrames     = 0U;
			m_ambeSilence    = 0U;
			m_ambeBits       = 1U;
			m_ambeErrors     = 0U;
			m_lastAMBEBits   = 0U;
			m_lastAMBEErrors = 0U;
			m_rxState        = DSRXS_PROCESS_DATA;
			break;

		case DSRXS_PROCESS_SLOW_DATA:
			m_bitBuffer.setMaxLength(DV_FRAME_LENGTH_BITS);
			m_squelchCount   = 0U;
			m_radioSeqNo     = 0U;
			m_radioSyncsLost = 0U;
			m_slowDataDecoder.reset();
			m_endMatcher.reset();
			m_dataMatcher.reset();
			m_rxState = DSRXS_PROCESS_SLOW_DATA;
			break;
	}
}

void CSoundCardRepeaterTRXThread::readRadioData(const wxFloat32* input, unsigned int nSamples)
{
	wxASSERT(input != NULL);
	wxASSERT(nSamples > 0U);

	if (m_reader != NULL) {		// Are we reading from a WAV file?
		wxFloat32 buffer[DSTAR_RADIO_BLOCK_SIZE * 2U];
		unsigned int n = m_reader->read(buffer, nSamples);
		if (n == 0U) {				// EOF on the input WAV file?
			CWAVFileReader* reader = m_reader;
			m_reader = NULL;

			reader->close();
			delete reader;

			wxLogInfo(wxT("End of the input file"));

			// Send data from the sound card as the file has ended
			m_inBuffer.addData(input, nSamples);
		} else {
			// Send data from the sound file
			m_inBuffer.addData(buffer, n);
		}
	} else {
		// Send data from the sound card
		m_inBuffer.addData(input, nSamples);
	}
}

CSoundCardRepeaterStatusData* CSoundCardRepeaterTRXThread::getStatus()
{
	float noise = 0.0F;
	if (m_noiseCount > 0U) {
		noise = m_noise / float(m_noiseCount);
		m_noiseCount = 0U;
		m_noise      = 0.0F;
	}

	if (m_rptState == DSRS_SHUTDOWN || m_rptState == DSRS_LISTENING) {
		return new CSoundCardRepeaterStatusData(wxEmptyString, wxEmptyString, wxEmptyString, wxEmptyString, wxEmptyString,
				0x00, 0x00, 0x00, m_tx, m_squelch, m_rxState, m_rptState, m_timeoutTimer.getTimer(),
				m_timeoutTimer.getTimeout(), m_beaconTimer.getTimer(), m_beaconTimer.getTimeout(),
				m_announcementTimer.getTimer(), m_announcementTimer.getTimeout(), 0.0F, noise, m_ackText, m_status1Text,
				m_status2Text, m_status3Text, m_status4Text, m_status5Text);
	} else if (m_rptState == DSRS_NETWORK) {
		float loss = 0.0F;
		if (m_packetCount != 0U)
			loss = float(m_packetSilence) / float(m_packetCount);

		return new CSoundCardRepeaterStatusData(m_header->getMyCall1(), m_header->getMyCall2(),
				m_header->getYourCall(), m_header->getRptCall1(), m_header->getRptCall2(), 
				m_header->getFlag1(), m_header->getFlag2(), m_header->getFlag3(), m_tx, m_squelch,
				m_rxState, m_rptState, m_timeoutTimer.getTimer(), m_timeoutTimer.getTimeout(),
				m_beaconTimer.getTimer(), m_beaconTimer.getTimeout(), m_announcementTimer.getTimer(),
				m_announcementTimer.getTimeout(), loss * 100.0F, noise, m_ackText, m_status1Text, m_status2Text,
				m_status3Text, m_status4Text, m_status5Text);
	} else {
		float   bits = float(m_ambeBits - m_lastAMBEBits);
		float errors = float(m_ambeErrors - m_lastAMBEErrors);
		if (bits == 0.0F)
			bits = 1.0F;

		m_lastAMBEBits   = m_ambeBits;
		m_lastAMBEErrors = m_ambeErrors;

		return new CSoundCardRepeaterStatusData(m_header->getMyCall1(), m_header->getMyCall2(),
				m_header->getYourCall(), m_header->getRptCall1(), m_header->getRptCall2(), 
				m_header->getFlag1(), m_header->getFlag2(), m_header->getFlag3(), m_tx, m_squelch,
				m_rxState, m_rptState, m_timeoutTimer.getTimer(), m_timeoutTimer.getTimeout(),
				m_beaconTimer.getTimer(), m_beaconTimer.getTimeout(), m_announcementTimer.getTimer(),
				m_announcementTimer.getTimeout(), (errors * 100.0F) / bits, noise, m_ackText, m_status1Text,
				m_status2Text, m_status3Text, m_status4Text, m_status5Text);
	}
}

void CSoundCardRepeaterTRXThread::clock(unsigned int ms)
{
	m_pollTimer.clock(ms);
	m_timeoutTimer.clock(ms);
	m_watchdogTimer.clock(ms);
	m_activeHangTimer.clock(ms);
	m_ackTimer.clock(ms);
	m_status1Timer.clock(ms);
	m_status2Timer.clock(ms);
	m_status3Timer.clock(ms);
	m_status4Timer.clock(ms);
	m_status5Timer.clock(ms);
	m_hangTimer.clock(ms);
	m_beaconTimer.clock(ms);
	m_announcementTimer.clock(ms);
	if (m_beacon != NULL)
		m_beacon->clock();
	if (m_announcement != NULL)
		m_announcement->clock();
}

void CSoundCardRepeaterTRXThread::shutdown()
{
	m_shutdown = true;
}

void CSoundCardRepeaterTRXThread::startup()
{
	m_shutdown = false;
}

void CSoundCardRepeaterTRXThread::command1()
{
	if (!m_controlCommand1Line.IsEmpty())
		::wxShell(m_controlCommand1Line);
}

void CSoundCardRepeaterTRXThread::command2()
{
	if (!m_controlCommand2Line.IsEmpty())
		::wxShell(m_controlCommand2Line);
}

void CSoundCardRepeaterTRXThread::command3()
{
	if (!m_controlCommand3Line.IsEmpty())
		::wxShell(m_controlCommand3Line);
}

void CSoundCardRepeaterTRXThread::command4()
{
	if (!m_controlCommand4Line.IsEmpty())
		::wxShell(m_controlCommand4Line);
}

bool CSoundCardRepeaterTRXThread::checkControl(const CHeaderData& header)
{
	if (!m_controlEnabled)
		return false;

	if (!m_controlRPT1.IsSameAs(header.getRptCall1()) || !m_controlRPT2.IsSameAs(header.getRptCall2()))
		return false;

	if (m_controlShutdown.IsSameAs(header.getYourCall())) {
		wxLogMessage(wxT("Shutdown requested by %s/%s"), header.getMyCall1().c_str(), header.getMyCall2().c_str());
		shutdown();
	} else if (m_controlStartup.IsSameAs(header.getYourCall())) {
		wxLogMessage(wxT("Startup requested by %s/%s"), header.getMyCall1().c_str(), header.getMyCall2().c_str());
		startup();
	} else if (m_controlStatus1.IsSameAs(header.getYourCall())) {
		wxLogMessage(wxT("Status 1 requested by %s/%s"), header.getMyCall1().c_str(), header.getMyCall2().c_str());
		m_status1Timer.start();
	} else if (m_controlStatus2.IsSameAs(header.getYourCall())) {
		wxLogMessage(wxT("Status 2 requested by %s/%s"), header.getMyCall1().c_str(), header.getMyCall2().c_str());
		m_status2Timer.start();
	} else if (m_controlStatus3.IsSameAs(header.getYourCall())) {
		wxLogMessage(wxT("Status 3 requested by %s/%s"), header.getMyCall1().c_str(), header.getMyCall2().c_str());
		m_status3Timer.start();
	} else if (m_controlStatus4.IsSameAs(header.getYourCall())) {
		wxLogMessage(wxT("Status 4 requested by %s/%s"), header.getMyCall1().c_str(), header.getMyCall2().c_str());
		m_status4Timer.start();
	} else if (m_controlStatus5.IsSameAs(header.getYourCall())) {
		wxLogMessage(wxT("Status 5 requested by %s/%s"), header.getMyCall1().c_str(), header.getMyCall2().c_str());
		m_status5Timer.start();
	} else if (m_controlCommand1.IsSameAs(header.getYourCall())) {
		wxLogMessage(wxT("Command 1 requested by %s/%s"), header.getMyCall1().c_str(), header.getMyCall2().c_str());
		command1();
	} else if (m_controlCommand2.IsSameAs(header.getYourCall())) {
		wxLogMessage(wxT("Command 2 requested by %s/%s"), header.getMyCall1().c_str(), header.getMyCall2().c_str());
		command2();
	} else if (m_controlCommand3.IsSameAs(header.getYourCall())) {
		wxLogMessage(wxT("Command 3 requested by %s/%s"), header.getMyCall1().c_str(), header.getMyCall2().c_str());
		command3();
	} else if (m_controlCommand4.IsSameAs(header.getYourCall())) {
		wxLogMessage(wxT("Command 4 requested by %s/%s"), header.getMyCall1().c_str(), header.getMyCall2().c_str());
		command4();
	} else if (m_controlOutput1.IsSameAs(header.getYourCall())) {
		wxLogMessage(wxT("Output 1 requested by %s/%s"), header.getMyCall1().c_str(), header.getMyCall2().c_str());
		m_output1 = !m_output1;
		m_controller->setOutput1(m_output1);
	} else if (m_controlOutput2.IsSameAs(header.getYourCall())) {
		wxLogMessage(wxT("Output 2 requested by %s/%s"), header.getMyCall1().c_str(), header.getMyCall2().c_str());
		m_output2 = !m_output2;
		m_controller->setOutput2(m_output2);
	} else if (m_controlOutput3.IsSameAs(header.getYourCall())) {
		wxLogMessage(wxT("Output 3 requested by %s/%s"), header.getMyCall1().c_str(), header.getMyCall2().c_str());
		m_output3 = !m_output3;
		m_controller->setOutput3(m_output3);
	} else if (m_controlOutput4.IsSameAs(header.getYourCall())) {
		wxLogMessage(wxT("Output 4 requested by %s/%s"), header.getMyCall1().c_str(), header.getMyCall2().c_str());
		m_output4 = !m_output4;
		m_controller->setOutput4(m_output4);
	} else {
		wxLogMessage(wxT("Invalid command of %s sent by %s/%s"), header.getYourCall().c_str(), header.getMyCall1().c_str(), header.getMyCall2().c_str());
	}

	return true;
}

bool CSoundCardRepeaterTRXThread::checkAnnouncements(const CHeaderData& header)
{
	if (m_announcement == NULL)
		return false;

	if (m_recordRPT1.IsSameAs(header.getRptCall1()) && m_recordRPT2.IsSameAs(header.getRptCall2())) {
		wxLogMessage(wxT("Announcement creation requested by %s/%s"), header.getMyCall1().c_str(), header.getMyCall2().c_str());
		m_announcement->writeHeader(header);
		m_recording = true;
		return true;
	}

	if (m_deleteRPT1.IsSameAs(header.getRptCall1()) && m_deleteRPT2.IsSameAs(header.getRptCall2())) {
		wxLogMessage(wxT("Announcement deletion requested by %s/%s"), header.getMyCall1().c_str(), header.getMyCall2().c_str());
		m_announcement->deleteAnnouncement();
		m_deleting = true;
		return true;
	}

	return false;
}

TRISTATE CSoundCardRepeaterTRXThread::checkHeader(CHeaderData& header)
{
	// If not in RPT1 validation mode, then a simplex header is converted to a proper repeater header
	if (!m_rpt1Validation) {
		if (!header.isRepeaterMode()) {
			// Convert to a properly addressed repeater packet
			header.setRepeaterMode(true);
			header.setRptCall1(m_rptCallsign);
			header.setRptCall2(m_gwyCallsign);
		}
	}

	// The repeater bit must be set when not in gateway mode
	if (m_mode != MODE_GATEWAY) {
		if (!header.isRepeaterMode()) {
			wxLogMessage(wxT("Received a non-repeater packet, ignoring"));
			return STATE_FALSE;
		}
	} else {
		// Quietly reject acks when in gateway mode
		if (header.isAck() || header.isNoResponse() || header.isRelayUnavailable())
			return STATE_UNKNOWN;

		// As a second check, reject on own UR call
		wxString ur = header.getYourCall();
		if (ur.IsSameAs(m_rptCallsign) || ur.IsSameAs(m_gwyCallsign))
			return STATE_UNKNOWN;

		// Change RPT2 to be the gateway callsign in gateway mode
		header.setRptCall2(m_gwyCallsign);
	}

	wxString my = header.getMyCall1();

	// Make sure MyCall is not empty, a silly value, or the repeater or gateway callsigns, STN* is a special case
	if (!my.Left(3U).IsSameAs(wxT("STN"))) {
		if (my.IsSameAs(m_rptCallsign) ||
			my.IsSameAs(m_gwyCallsign) ||
			my.IsSameAs(wxT("        ")) ||
			my.Left(6U).IsSameAs(wxT("NOCALL")) ||
			my.Left(6U).IsSameAs(wxT("N0CALL")) ||
			my.Left(6U).IsSameAs(wxT("MYCALL"))) {
			wxLogMessage(wxT("Invalid MYCALL value of %s, ignoring"), my.c_str());
			return STATE_UNKNOWN;
		}
	}

	// Check for a French class 3 novice callsign, and reject
	// Of the form F0xxx
	if (my.Left(2U).IsSameAs(wxT("F0"))) {
		wxLogMessage(wxT("French novice class licence callsign found, %s, ignoring"), my.c_str());
		return STATE_UNKNOWN;
	}

	// Check for an Australian foundation class licence callsign, and reject
	// Of the form VKnFxxx
	if (my.Left(2U).IsSameAs(wxT("VK")) && my.GetChar(3U) == wxT('F') && my.GetChar(6U) != wxT(' ')) {
		wxLogMessage(wxT("Australian foundation class licence callsign found, %s, ignoring"), my.c_str());
		return STATE_UNKNOWN;
	}

	// Check the MyCall value against the regular expression
	bool ok = m_regEx.Matches(my);
	if (!ok) {
		wxLogMessage(wxT("Invalid MYCALL value of %s, ignoring"), my.c_str());
		return STATE_UNKNOWN;
	}

	// Is it for us?
	if (!header.getRptCall1().IsSameAs(m_rptCallsign)) {
		wxLogMessage(wxT("Invalid RPT1 value %s, ignoring"), header.getRptCall1().c_str());
		return STATE_FALSE;
	}

	// If using callsign restriction, validate the my callsign
	if (m_restriction) {
		if (!my.Left(LONG_CALLSIGN_LENGTH - 1U).IsSameAs(m_rptCallsign.Left(LONG_CALLSIGN_LENGTH - 1U))) {
			wxLogMessage(wxT("Unauthorised user %s tried to access the repeater"), my.c_str());
			return STATE_UNKNOWN;
		}
	}

	return STATE_TRUE;
}

void CSoundCardRepeaterTRXThread::getStatistics()
{
	if (m_rptState == DSRS_VALID)
		m_radioCount++;

	if (m_rptState == DSRS_NETWORK)
		m_networkCount++;

	if (m_tx)
		m_transmitCount++;

	m_timeCount++;
	if ((m_timeCount % DSTAR_TICKS_PER_SEC) == 0U) {
		wxDateTime dateTime = wxDateTime::Now();
		unsigned int hour = dateTime.GetHour();

		if (hour != m_lastHour) {
			writeStatistics();
			m_lastHour = hour;
		}
	}
}

void CSoundCardRepeaterTRXThread::writeStatistics()
{
	wxLogInfo(wxT("STATS: Radio input: %u%%"),    (m_radioCount * 100U) / m_timeCount);
	wxLogInfo(wxT("STATS: Network input: %u%%"),  (m_networkCount * 100U) / m_timeCount);
	wxLogInfo(wxT("STATS: Total transmit: %u%%"), (m_transmitCount * 100U) / m_timeCount);

	m_radioCount    = 0U;
	m_networkCount  = 0U;
	m_transmitCount = 0U;
	m_timeCount     = 0U;
}

void CSoundCardRepeaterTRXThread::blankDTMF(bool* data)
{
	wxASSERT(data != NULL);

	if (data[1U] && data[7U] && data[11U] && data[21U] && data[25U] && data[31U] && !data[49U] && !data[55U])
		::memcpy(data, NULL_AMBE_DATA_BITS, VOICE_FRAME_LENGTH_BITS * sizeof(bool));
}

void CSoundCardRepeaterTRXThread::blankDTMF(unsigned char* data)
{
	wxASSERT(data != NULL);

	// DTMF begins with these byte values
	if ((data[0] & DTMF_MASK[0]) == DTMF_SIG[0] && (data[1] & DTMF_MASK[1]) == DTMF_SIG[1] &&
		(data[2] & DTMF_MASK[2]) == DTMF_SIG[2] && (data[3] & DTMF_MASK[3]) == DTMF_SIG[3] &&
		(data[4] & DTMF_MASK[4]) == DTMF_SIG[4] && (data[5] & DTMF_MASK[5]) == DTMF_SIG[5] &&
		(data[6] & DTMF_MASK[6]) == DTMF_SIG[6] && (data[7] & DTMF_MASK[7]) == DTMF_SIG[7] &&
		(data[8] & DTMF_MASK[8]) == DTMF_SIG[8])
		::memcpy(data, NULL_AMBE_DATA_BYTES, VOICE_FRAME_LENGTH_BYTES);
}
