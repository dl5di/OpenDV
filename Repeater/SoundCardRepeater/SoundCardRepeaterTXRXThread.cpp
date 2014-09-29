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
#include "RadioHeaderEncoder.h"
#include "RadioHeaderDecoder.h"
#include "SoundCardRepeaterApp.h"
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

CSoundCardRepeaterTXRXThread::CSoundCardRepeaterTXRXThread() :
m_soundcard(NULL),
m_protocolHandler(NULL),
m_controller(NULL),
m_goertzel(DSTAR_RADIO_SAMPLE_RATE, GOERTZEL_FREQ, GOERTZEL_N, 0.1F),
m_pttDelay(NULL),
m_audioDelay(NULL),
m_stopped(true),
m_inBuffer(DSTAR_RADIO_SAMPLE_RATE * 1U),			// One second of data
m_outBuffer(DSTAR_RADIO_SAMPLE_RATE * 1U),			// One second of data
m_networkBuffer(DSTAR_GMSK_SYMBOL_RATE * 2U),		// Two seconds of data
m_networkRun(0U),
m_networkStarted(false),
m_rptCallsign(),
m_reader(NULL),
m_demodulator(),
m_modulator(),
m_bitBuffer(DV_FRAME_LENGTH_BITS),
m_rxState(DSRXS_LISTENING),
m_frameMatcher(FRAME_SYNC_LENGTH_BITS, FRAME_SYNC_BITS),
m_dataMatcher(DATA_FRAME_LENGTH_BITS, DATA_SYNC_BITS),
m_endMatcher(END_PATTERN_LENGTH_BITS, END_PATTERN_BITS),
m_writer(NULL),
m_rxHeader(NULL),
m_txHeader(NULL),
m_headerBER(0U),
m_radioSeqNo(0U),
m_radioSyncsLost(0U),
m_networkSeqNo(0U),
m_watchdogTimer(1000U, 2U),		// 2s
m_pollTimer(1000U, 60U),		// 60s
m_hangTimer(1000U, 0U, 0U),		// 0ms
m_rptState(DSRS_LISTENING),
m_slowDataDecoder(),
m_tx(false),
m_squelch(false),
m_squelchCount(0U),
m_noise(0.0F),
m_noiseCount(0U),
m_killed(false),
m_rxLevel(1.0F),
m_txLevel(1.0F),
m_squelchMode(SM_NORMAL),
m_activeHangTimer(1000U),
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
m_headerTime(),
m_packetTime(),
m_packetCount(0U),
m_packetSilence(0U)
{
	m_lastData = new unsigned char[DV_FRAME_MAX_LENGTH_BYTES];

	setRadioState(DSRXS_LISTENING);
	setRepeaterState(DSRS_LISTENING);
}

CSoundCardRepeaterTXRXThread::~CSoundCardRepeaterTXRXThread()
{
	delete[] m_lastData;
	delete   m_rxHeader;
	delete   m_txHeader;
}

void CSoundCardRepeaterTXRXThread::run()
{
	// Wait here until we have the essentials to run
	while (!m_killed && (m_soundcard == NULL  || m_protocolHandler == NULL || m_rptCallsign.IsEmpty() || m_rptCallsign.IsSameAs(wxT("        ")) || m_controller == NULL))
		::wxMilliSleep(500UL);		// 1/2 sec

	if (m_killed)
		return;

	m_stopped = false;

	m_controller->setActive(false);
	m_controller->setRadioTransmit(false);

	m_pollTimer.start();

	wxDateTime dateTime = wxDateTime::Now();
	m_lastHour = dateTime.GetHour();

	m_inBuffer.clear();

	wxLogMessage(wxT("Starting the sound card transmitter and receiver thread"));

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

		// Clock the heartbeat output every one second
		count++;
		if (count == 50U) {
			m_controller->setHeartbeat();
			count = 0U;
		}

		// Set the output state
		if (m_tx || (m_activeHangTimer.isRunning() && !m_activeHangTimer.hasExpired())) {
			m_controller->setActive(true);
		} else {
			m_controller->setActive(false);
			m_activeHangTimer.stop();
		}

		// Check the shutdown state, state changes are done here to bypass the state machine which is
		// frozen when m_disable is asserted
		m_disable = m_controller->getDisable();
		if (m_disable) {
			if (m_rptState != DSRS_SHUTDOWN) {
				m_watchdogTimer.stop();
				m_activeHangTimer.stop();
				m_hangTimer.stop();
				m_networkBuffer.clear();
				m_bitBuffer.clear();
				m_networkRun = 0U;
				m_networkStarted = false;
				m_controller->setActive(false);
				m_controller->setRadioTransmit(false);
				m_rptState = DSRS_SHUTDOWN;
			}
		} else {
			if (m_rptState == DSRS_SHUTDOWN) {
				m_watchdogTimer.stop();
				m_hangTimer.stop();
				m_rptState = DSRS_LISTENING;
				m_protocolHandler->reset();
			}
		}

		// Send the output data
		if (m_networkStarted)
			transmitNetwork();
		else if (m_networkRun >= NETWORK_RUN_FRAME_COUNT)
			transmitNetwork();
		else
			transmit();

		getStatistics();

		unsigned int ms = timer.Time();
		clock(ms);
	}

	writeStatistics();

	wxLogMessage(wxT("Stopping the sound card transmitter and receiver thread"));

	m_controller->setActive(false);
	m_controller->setRadioTransmit(false);
	m_controller->close();
	delete m_controller;

	m_soundcard->close();
	delete m_soundcard;

	delete m_audioDelay;
	delete m_pttDelay;

	if (m_reader != NULL) {
		m_reader->close();
		delete m_reader;
	}

	m_protocolHandler->close();
	delete m_protocolHandler;

#if defined(TX_TO_WAV_FILE)
	if (m_writer != NULL) {
		m_writer->close();
		delete m_writer;
	}
#endif
}

void CSoundCardRepeaterTXRXThread::kill()
{
	m_killed = true;
}

void CSoundCardRepeaterTXRXThread::setReader(CWAVFileReader* reader)
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

void CSoundCardRepeaterTXRXThread::setCallsign(const wxString& callsign, const wxString& gateway, DSTAR_MODE mode, ACK_TYPE ack, bool restriction, bool rpt1Validation, bool dtmfBlanking)
{
	// Pad the callsign up to eight characters
	m_rptCallsign = callsign;
	m_rptCallsign.resize(LONG_CALLSIGN_LENGTH, wxT(' '));
}

void CSoundCardRepeaterTXRXThread::setProtocolHandler(CRepeaterProtocolHandler* handler)
{
	m_protocolHandler = handler;
}

void CSoundCardRepeaterTXRXThread::setSoundCard(CSoundCardReaderWriter* soundcard, wxFloat32 rxLevel, wxFloat32 txLevel, SQUELCH_MODE squelchMode, wxFloat32 squelchLevel, bool rxInvert, bool txInvert)
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

void CSoundCardRepeaterTXRXThread::setTimes(unsigned int timeout, unsigned int ackTime, unsigned int hangTime)
{
	m_hangTimer.setTimeout(0U, hangTime);
}

void CSoundCardRepeaterTXRXThread::setBeacon(unsigned int time, const wxString& text, bool voice, TEXT_LANG language)
{
}

void CSoundCardRepeaterTXRXThread::setAnnouncement(bool enabled, unsigned int time, const wxString& recordRPT1, const wxString& recordRPT2, const wxString& deleteRPT1, const wxString& deleteRPT2)
{
}

void CSoundCardRepeaterTXRXThread::setController(CExternalController* controller, int pttDelay)
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

void CSoundCardRepeaterTXRXThread::setControl(bool enabled, const wxString& rpt1Callsign, const wxString& rpt2Callsign, const wxString& shutdown, const wxString& startup, const wxString& status1, const wxString& status2, const wxString& status3, const wxString& status4, const wxString& status5, const wxString& command1, const wxString& command1Line, const wxString& command2, const wxString& command2Line, const wxString& command3, const wxString& command3Line, const wxString& command4, const wxString& command4Line, const wxString& output1, const wxString& output2, const wxString& output3, const wxString& output4)
{
}

void CSoundCardRepeaterTXRXThread::setActiveHang(unsigned int time)
{
	m_activeHangTimer.setTimeout(time);
}

void CSoundCardRepeaterTXRXThread::setOutputs(bool out1, bool out2, bool out3, bool out4)
{
}

void CSoundCardRepeaterTXRXThread::setRXLevel(wxFloat32 level)
{
	m_rxLevel = level;
}

void CSoundCardRepeaterTXRXThread::setTXLevel(wxFloat32 level)
{
	m_txLevel = level;
}

void CSoundCardRepeaterTXRXThread::setSquelchLevel(wxFloat32 level)
{
	m_goertzel.setThreshold(level);
}

void CSoundCardRepeaterTXRXThread::setLogging(bool logging, const wxString& dir)
{
}

void CSoundCardRepeaterTXRXThread::setBlackList(CCallsignList* list)
{
}

void CSoundCardRepeaterTXRXThread::setGreyList(CCallsignList* list)
{
}

void CSoundCardRepeaterTXRXThread::receiveRadio()
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

void CSoundCardRepeaterTXRXThread::receiveNetwork()
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

void CSoundCardRepeaterTXRXThread::transmitNetworkHeader(CHeaderData& header)
{
	wxLogMessage(wxT("Transmitting to - My: %s/%s  Your: %s  Rpt1: %s  Rpt2: %s  Flags: %02X %02X %02X"), header.getMyCall1().c_str(), header.getMyCall2().c_str(), header.getYourCall().c_str(), header.getRptCall1().c_str(), header.getRptCall2().c_str(), header.getFlag1(), header.getFlag2(), header.getFlag3());

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

void CSoundCardRepeaterTXRXThread::transmitNetwork()
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

void CSoundCardRepeaterTXRXThread::transmit()
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

void CSoundCardRepeaterTXRXThread::transmitBits(const bool* bits, unsigned int length)
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

void CSoundCardRepeaterTXRXThread::radioStateMachine(bool bit)
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

void CSoundCardRepeaterTXRXThread::repeaterStateMachine()
{
	if (m_watchdogTimer.isRunning() && m_watchdogTimer.hasExpired()) {
		wxLogMessage(wxT("Network watchdog has expired"));

		// Send end of transmission data to the radio
		m_networkBuffer.addData(END_PATTERN_BITS, END_PATTERN_LENGTH_BITS);
		m_networkBuffer.addData(END_PATTERN_BITS, END_PATTERN_LENGTH_BITS);
		m_networkBuffer.addData(END_PATTERN_BITS, END_PATTERN_LENGTH_BITS);

		endOfNetworkData();
	}
}

bool CSoundCardRepeaterTXRXThread::setRepeaterState(DSTAR_RPT_STATE state)
{
	// Can't change state when shutdown
	if (m_disable)
		return false;

	// The "to" state
	switch (state) {
		case DSRS_SHUTDOWN:
			m_watchdogTimer.stop();
			m_activeHangTimer.stop();
			m_networkBuffer.clear();
			m_networkRun = 0U;
			m_networkStarted = false;
			m_controller->setActive(false);
			m_controller->setRadioTransmit(false);
			m_rptState = DSRS_SHUTDOWN;
			break;

		case DSRS_LISTENING:
			m_rptState = DSRS_LISTENING;
			break;

		case DSRS_VALID:
			if (m_rptState != DSRS_LISTENING)
				return false;

			m_rptState = DSRS_VALID;
			break;

		default:
			break;
	}

	return true;
}

CHeaderData* CSoundCardRepeaterTXRXThread::processFECHeader()
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

bool CSoundCardRepeaterTXRXThread::processRadioHeader(CHeaderData* header)
{
	wxASSERT(header != NULL);

	// If shutdown we ignore incoming headers
	if (m_rptState == DSRS_SHUTDOWN) {
		delete header;
		return true;
	}

	// We don't handle DD data packets
	if (header->isDataPacket()) {
		wxLogMessage(wxT("Received a DD packet, ignoring"));
		delete header;
		return false;
	}

	setRepeaterState(DSRS_VALID);

	// Send the valid header to the gateway if we are accepted
	if (m_rptState == DSRS_VALID) {
		delete m_rxHeader;
		m_rxHeader = header;

		CHeaderData netHeader(*m_rxHeader);
		netHeader.setRptCall1(m_rxHeader->getRptCall2());
		netHeader.setRptCall2(m_rxHeader->getRptCall1());

		m_protocolHandler->writeHeader(netHeader);
	} else {
		delete header;
	}

	return true;
}

void CSoundCardRepeaterTXRXThread::processNetworkHeader(CHeaderData* header)
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

	delete m_txHeader;
	m_txHeader = header;

	m_networkSeqNo = 0U;
	m_watchdogTimer.start();
	m_activeHangTimer.stop();

	transmitNetworkHeader(*header);
}

void CSoundCardRepeaterTXRXThread::processRadioFrame(FRAME_TYPE type)
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

	// Don't pass through the frame of an invalid transmission
	if (m_rptState != DSRS_VALID)
		return;

	if (type == FRAME_END) {
		// Send null data and the end marker over the network
		unsigned char bytes[DV_FRAME_MAX_LENGTH_BYTES];
		::memcpy(bytes, NULL_AMBE_DATA_BYTES, VOICE_FRAME_LENGTH_BYTES);
		::memcpy(bytes + VOICE_FRAME_LENGTH_BYTES, END_PATTERN_BYTES, END_PATTERN_LENGTH_BYTES);
		m_protocolHandler->writeData(bytes, DV_FRAME_MAX_LENGTH_BYTES, 0U, true);
	} else {
		// Convert the bits to bytes for the network
		unsigned char bytes[DV_FRAME_LENGTH_BYTES];
		unsigned int n = 0U;
		for (unsigned int i = 0U; i < DV_FRAME_LENGTH_BYTES; i++, n += 8U)
			bytes[i] = CUtils::bitsToByteRev(bits + n);

		// Send the data to the network
		m_protocolHandler->writeData(bytes, DV_FRAME_LENGTH_BYTES, errors, false);
	}
}

unsigned int CSoundCardRepeaterTXRXThread::processNetworkFrame(unsigned char* data, unsigned int length, unsigned char seqNo)
{
	wxASSERT(data != NULL);
	wxASSERT(length > 0U);

	if (m_rptState == DSRS_SHUTDOWN)
		return 1U;

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
	if (m_networkSeqNo == 0U)
		::memcpy(data + VOICE_FRAME_LENGTH_BYTES, DATA_SYNC_BYTES, DATA_FRAME_LENGTH_BYTES);

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

	m_networkBuffer.addData(bits, length * 8U);

	return packetCount;
}

CHeaderData* CSoundCardRepeaterTXRXThread::processSlowData(bool sync)
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

void CSoundCardRepeaterTXRXThread::endOfRadioData()
{
	wxLogMessage(wxT("AMBE for %s  Frames: %.1fs, Silence: %.1f%%, BER: %.1f%%"), m_rxHeader->getMyCall1().c_str(), float(m_ambeFrames) / 50.0F, float(m_ambeSilence * 100U) / float(m_ambeFrames), float(m_ambeErrors * 100U) / float(m_ambeBits));

	setRepeaterState(DSRS_LISTENING);
}

void CSoundCardRepeaterTXRXThread::callback(const wxFloat32* input, wxFloat32* output, unsigned int nSamples, int id)
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

void CSoundCardRepeaterTXRXThread::setRadioState(DSTAR_RX_STATE state)
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

void CSoundCardRepeaterTXRXThread::endOfNetworkData()
{
	if (m_watchdogTimer.hasExpired()) {
		m_packetCount   -= 77U;
		m_packetSilence -= 77U;
	}

	float loss = 0.0F;
	if (m_packetCount != 0U)
		loss = float(m_packetSilence) / float(m_packetCount);

	wxLogMessage(wxT("Stats for %s  Frames: %.1fs, Loss: %.1f%%, Packets: %u/%u"), m_txHeader->getMyCall1().c_str(), float(m_packetCount) / 50.0F, loss * 100.0F, m_packetSilence, m_packetCount);

	m_watchdogTimer.stop();
	m_activeHangTimer.start();
	m_protocolHandler->reset();

	m_networkRun = NETWORK_RUN_FRAME_COUNT;
}

void CSoundCardRepeaterTXRXThread::readRadioData(const wxFloat32* input, unsigned int nSamples)
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

CSoundCardRepeaterStatusData* CSoundCardRepeaterTXRXThread::getStatus()
{
	float   bits = float(m_ambeBits - m_lastAMBEBits);
	float errors = float(m_ambeErrors - m_lastAMBEErrors);
	if (bits == 0.0F)
		bits = 1.0F;

	float noise = 0.0F;
	if (m_noiseCount > 0U) {
		noise = m_noise / float(m_noiseCount);
		m_noiseCount = 0U;
		m_noise      = 0.0F;
	}

	m_lastAMBEBits   = m_ambeBits;
	m_lastAMBEErrors = m_ambeErrors;

	if (m_rptState == DSRS_SHUTDOWN || m_rptState == DSRS_LISTENING)
		return new CSoundCardRepeaterStatusData(wxEmptyString, wxEmptyString, wxEmptyString, wxEmptyString,
				wxEmptyString, 0x00, 0x00, 0x00, m_tx, m_squelch, m_rxState, m_rptState, 0U, 0U, 0U, 0U,
				0U, 0U, 0.0F, noise, wxEmptyString, wxEmptyString, wxEmptyString, wxEmptyString, wxEmptyString,
				wxEmptyString);
	else
		return new CSoundCardRepeaterStatusData(m_rxHeader->getMyCall1(), m_rxHeader->getMyCall2(),
				m_rxHeader->getYourCall(), m_rxHeader->getRptCall1(), m_rxHeader->getRptCall2(), 
				m_rxHeader->getFlag1(), m_rxHeader->getFlag2(), m_rxHeader->getFlag3(), m_tx, m_squelch,
				m_rxState, m_rptState, 0U, 0U, 0U, 0U, 0U, 0U, (errors * 100.0F) / bits, noise, wxEmptyString,
				wxEmptyString, wxEmptyString, wxEmptyString, wxEmptyString, wxEmptyString);
}

void CSoundCardRepeaterTXRXThread::clock(unsigned int ms)
{
	m_pollTimer.clock(ms);
	m_watchdogTimer.clock(ms);
	m_activeHangTimer.clock(ms);
	m_hangTimer.clock(ms);
}

void CSoundCardRepeaterTXRXThread::shutdown()
{
}

void CSoundCardRepeaterTXRXThread::startup()
{
}

void CSoundCardRepeaterTXRXThread::command1()
{
}

void CSoundCardRepeaterTXRXThread::command2()
{
}

void CSoundCardRepeaterTXRXThread::command3()
{
}

void CSoundCardRepeaterTXRXThread::command4()
{
}

void CSoundCardRepeaterTXRXThread::getStatistics()
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

void CSoundCardRepeaterTXRXThread::writeStatistics()
{
	wxLogInfo(wxT("STATS: Radio input: %u%%"),    (m_radioCount * 100U) / m_timeCount);
	wxLogInfo(wxT("STATS: Network input: %u%%"),  (m_networkCount * 100U) / m_timeCount);
	wxLogInfo(wxT("STATS: Total transmit: %u%%"), (m_transmitCount * 100U) / m_timeCount);

	m_radioCount    = 0U;
	m_networkCount  = 0U;
	m_transmitCount = 0U;
	m_timeCount     = 0U;
}
