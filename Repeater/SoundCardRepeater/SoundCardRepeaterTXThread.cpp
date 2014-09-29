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

#include "SoundCardRepeaterTXThread.h"
#include "RadioHeaderEncoder.h"
#include "SoundCardRepeaterApp.h"
#include "DStarDefines.h"
#include "HeaderData.h"
#include "Version.h"
#include "Utils.h"

const unsigned char DTMF_MASK[] = {0x82U, 0x08U, 0x20U, 0x82U, 0x00U, 0x00U, 0x82U, 0x00U, 0x00U};
const unsigned char DTMF_SIG[]  = {0x82U, 0x08U, 0x20U, 0x82U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U};

const unsigned int SILENCE_THRESHOLD = 2U;

// #define	TX_TO_WAV_FILE

CSoundCardRepeaterTXThread::CSoundCardRepeaterTXThread() :
m_soundcard(NULL),
m_protocolHandler(NULL),
m_controller(NULL),
m_pttDelay(NULL),
m_audioDelay(NULL),
m_outBuffer(DSTAR_RADIO_SAMPLE_RATE * 1U),			// One second of data
m_networkBuffer(DSTAR_GMSK_SYMBOL_RATE * 2U),		// Two seconds of data
m_networkRun(0U),
m_networkStarted(false),
m_rptCallsign(),
m_modulator(),
m_writer(NULL),
m_header(NULL),
m_networkSeqNo(0U),
m_watchdogTimer(1000U, 2U),		// 2s
m_pollTimer(1000U, 60U),		// 60s
m_hangTimer(1000U, 0U, 0U),		// 0ms
m_rptState(DSRS_LISTENING),
m_tx(false),
m_killed(false),
m_activeHangTimer(1000U),
m_lastData(NULL),
m_ambe(),
m_headerTime(),
m_packetTime(),
m_packetCount(0U),
m_packetSilence(0U)
{
	m_lastData = new unsigned char[DV_FRAME_MAX_LENGTH_BYTES];
}

CSoundCardRepeaterTXThread::~CSoundCardRepeaterTXThread()
{
	delete[] m_lastData;
	delete   m_header;
}

void CSoundCardRepeaterTXThread::run()
{
	// Wait here until we have the essentials to run
	while (!m_killed && (m_soundcard == NULL || m_protocolHandler == NULL || m_rptCallsign.IsEmpty() || m_rptCallsign.IsSameAs(wxT("        ")) || m_controller == NULL))
		::wxMilliSleep(500UL);		// 1/2 sec

	if (m_killed)
		return;

	m_controller->setActive(false);
	m_controller->setRadioTransmit(false);

	m_pollTimer.start();

	wxLogMessage(wxT("Starting the sound card transmitter thread"));

	unsigned int count = 0U;

	wxStopWatch timer;

	while (!m_killed) {
		timer.Start();

		// Process network traffic
		receiveNetwork();

		if (m_rptState == DSRS_NETWORK) {
			if (m_watchdogTimer.hasExpired()) {
				wxLogMessage(wxT("Network watchdog has expired"));
				// Send end of transmission data to the radio
				m_networkBuffer.addData(END_PATTERN_BITS, END_PATTERN_LENGTH_BITS);
				m_networkBuffer.addData(END_PATTERN_BITS, END_PATTERN_LENGTH_BITS);
				m_networkBuffer.addData(END_PATTERN_BITS, END_PATTERN_LENGTH_BITS);

				endOfNetworkData();
			}
		}

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
		if (m_rptState == DSRS_NETWORK    || (m_activeHangTimer.isRunning() && !m_activeHangTimer.hasExpired())) {
			m_controller->setActive(true);
		} else {
			m_controller->setActive(false);
			m_activeHangTimer.stop();
		}

		// Send the output data
		if (m_networkStarted)
			transmitNetwork();
		else if (m_networkRun >= NETWORK_RUN_FRAME_COUNT)
			transmitNetwork();
		else
			transmit();

		unsigned int ms = timer.Time();
		clock(ms);
	}

	wxLogMessage(wxT("Stopping the sound card transmitter thread"));

	m_controller->setActive(false);
	m_controller->setRadioTransmit(false);
	m_controller->close();
	delete m_controller;

	m_soundcard->close();
	delete m_soundcard;

	delete m_audioDelay;
	delete m_pttDelay;

	m_protocolHandler->close();
	delete m_protocolHandler;

#if defined(TX_TO_WAV_FILE)
	if (m_writer != NULL) {
		m_writer->close();
		delete m_writer;
	}
#endif
}

void CSoundCardRepeaterTXThread::kill()
{
	m_killed = true;
}

void CSoundCardRepeaterTXThread::setReader(CWAVFileReader* reader)
{
}

void CSoundCardRepeaterTXThread::setCallsign(const wxString& callsign, const wxString& gateway, DSTAR_MODE mode, ACK_TYPE ack, bool restriction, bool rpt1Validation, bool dtmfBlanking)
{
	// Pad the callsign up to eight characters
	m_rptCallsign = callsign;
	m_rptCallsign.resize(LONG_CALLSIGN_LENGTH, wxT(' '));
}

void CSoundCardRepeaterTXThread::setProtocolHandler(CRepeaterProtocolHandler* handler)
{
	m_protocolHandler = handler;
}

void CSoundCardRepeaterTXThread::setSoundCard(CSoundCardReaderWriter* soundcard, wxFloat32 rxLevel, wxFloat32 txLevel, SQUELCH_MODE squelchMode, wxFloat32 squelchLevel, bool rxInvert, bool txInvert)
{
	wxASSERT(soundcard != NULL);

	m_soundcard = soundcard;

	m_modulator.setInvert(txInvert);
}

void CSoundCardRepeaterTXThread::setTimes(unsigned int timeout, unsigned int ackTime, unsigned int hangTime)
{
	m_hangTimer.setTimeout(0U, hangTime);
}

void CSoundCardRepeaterTXThread::setBeacon(unsigned int time, const wxString& text, bool voice, TEXT_LANG language)
{
}

void CSoundCardRepeaterTXThread::setAnnouncement(bool enabled, unsigned int time, const wxString& recordRPT1, const wxString& recordRPT2, const wxString& deleteRPT1, const wxString& deleteRPT2)
{
}

void CSoundCardRepeaterTXThread::setController(CExternalController* controller, int pttDelay)
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

void CSoundCardRepeaterTXThread::setControl(bool enabled, const wxString& rpt1Callsign, const wxString& rpt2Callsign, const wxString& shutdown, const wxString& startup, const wxString& status1, const wxString& status2, const wxString& status3, const wxString& status4, const wxString& status5, const wxString& command1, const wxString& command1Line, const wxString& command2, const wxString& command2Line, const wxString& command3, const wxString& command3Line, const wxString& command4, const wxString& command4Line, const wxString& output1, const wxString& output2, const wxString& output3, const wxString& output4)
{
}

void CSoundCardRepeaterTXThread::setActiveHang(unsigned int time)
{
	m_activeHangTimer.setTimeout(time);
}

void CSoundCardRepeaterTXThread::setOutputs(bool out1, bool out2, bool out3, bool out4)
{
}

void CSoundCardRepeaterTXThread::setRXLevel(wxFloat32 level)
{
}

void CSoundCardRepeaterTXThread::setTXLevel(wxFloat32 level)
{
}

void CSoundCardRepeaterTXThread::setSquelchLevel(wxFloat32 level)
{
}

void CSoundCardRepeaterTXThread::setLogging(bool logging, const wxString& dir)
{
}

void CSoundCardRepeaterTXThread::setBlackList(CCallsignList* list)
{
}

void CSoundCardRepeaterTXThread::setGreyList(CCallsignList* list)
{
}

void CSoundCardRepeaterTXThread::receiveNetwork()
{
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

void CSoundCardRepeaterTXThread::transmitNetworkHeader(CHeaderData& header)
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

void CSoundCardRepeaterTXThread::transmitNetwork()
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

void CSoundCardRepeaterTXThread::transmit()
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

void CSoundCardRepeaterTXThread::transmitBits(const bool* bits, unsigned int length)
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

void CSoundCardRepeaterTXThread::processNetworkHeader(CHeaderData* header)
{
	wxASSERT(header != NULL);

	wxLogMessage(wxT("Network header received - My: %s/%s  Your: %s  Rpt1: %s  Rpt2: %s  Flags: %02X %02X %02X"), header->getMyCall1().c_str(), header->getMyCall2().c_str(), header->getYourCall().c_str(), header->getRptCall1().c_str(), header->getRptCall2().c_str(), header->getFlag1(), header->getFlag2(), header->getFlag3());

	// Is it for us?
	if (!header->getRptCall2().IsSameAs(m_rptCallsign)) {
		wxLogMessage(wxT("Invalid network RPT2 value, ignoring"));
		delete header;
		return;
	}

	m_rptState = DSRS_NETWORK;
	m_networkSeqNo = 0U;
	m_watchdogTimer.start();
	m_activeHangTimer.stop();

	delete m_header;
	m_header = header;

	transmitNetworkHeader(*header);
}

unsigned int CSoundCardRepeaterTXThread::processNetworkFrame(unsigned char* data, unsigned int length, unsigned char seqNo)
{
	wxASSERT(data != NULL);
	wxASSERT(length > 0U);

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

void CSoundCardRepeaterTXThread::endOfNetworkData()
{
	if (m_watchdogTimer.hasExpired()) {
		m_packetCount   -= 77U;
		m_packetSilence -= 77U;
	}

	float loss = 0.0F;
	if (m_packetCount != 0U)
		loss = float(m_packetSilence) / float(m_packetCount);

	wxLogMessage(wxT("Stats for %s  Frames: %.1fs, Loss: %.1f%%, Packets: %u/%u"), m_header->getMyCall1().c_str(), float(m_packetCount) / 50.0F, loss * 100.0F, m_packetSilence, m_packetCount);

	m_rptState = DSRS_LISTENING;
	m_watchdogTimer.stop();
	m_activeHangTimer.start();

	m_networkRun = NETWORK_RUN_FRAME_COUNT;

	m_protocolHandler->reset();
}

void CSoundCardRepeaterTXThread::callback(const wxFloat32* input, wxFloat32* output, unsigned int nSamples, int id)
{
	::memset(output, 0x00, nSamples * sizeof(wxFloat32));

	m_outBuffer.getData(output, nSamples);

	if (m_audioDelay != NULL)
		m_audioDelay->delay(output, nSamples);
}

CSoundCardRepeaterStatusData* CSoundCardRepeaterTXThread::getStatus()
{
	float loss = 0.0F;
	if (m_packetCount != 0U)
		loss = float(m_packetSilence) / float(m_packetCount);

	if (m_rptState == DSRS_LISTENING)
		return new CSoundCardRepeaterStatusData(wxEmptyString, wxEmptyString, wxEmptyString, wxEmptyString,
				wxEmptyString, 0x00, 0x00, 0x00, m_tx, false, DSRXS_LISTENING, m_rptState, 0U, 0U, 0U, 0U, 0U,
				0U, 0.0F, 0.0F, wxEmptyString, wxEmptyString, wxEmptyString, wxEmptyString, wxEmptyString, wxEmptyString);
	else
		return new CSoundCardRepeaterStatusData(m_header->getMyCall1(), m_header->getMyCall2(),
				m_header->getYourCall(), m_header->getRptCall1(), m_header->getRptCall2(), 
				m_header->getFlag1(), m_header->getFlag2(), m_header->getFlag3(), m_tx, false,
				DSRXS_LISTENING, m_rptState, 0U, 0U, 0U, 0U, 0U, 0U, loss * 100.0F, 0.0F,
				wxEmptyString, wxEmptyString, wxEmptyString, wxEmptyString, wxEmptyString, wxEmptyString);
}

void CSoundCardRepeaterTXThread::clock(unsigned int ms)
{
	m_watchdogTimer.clock(ms);
	m_activeHangTimer.clock(ms);
	m_hangTimer.clock(ms);
}

void CSoundCardRepeaterTXThread::shutdown()
{
}

void CSoundCardRepeaterTXThread::startup()
{
}

void CSoundCardRepeaterTXThread::command1()
{
}

void CSoundCardRepeaterTXThread::command2()
{
}

void CSoundCardRepeaterTXThread::command3()
{
}

void CSoundCardRepeaterTXThread::command4()
{
}
