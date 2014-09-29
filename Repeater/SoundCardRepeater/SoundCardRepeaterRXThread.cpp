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

#include "SoundCardRepeaterRXThread.h"
#include "SoundCardRepeaterApp.h"
#include "RadioHeaderDecoder.h"
#include "RadioHeaderEncoder.h"
#include "DStarDefines.h"
#include "HeaderData.h"
#include "Version.h"
#include "Utils.h"

const wxFloat32 GOERTZEL_FREQ = 4000.0F; 
const unsigned int GOERTZEL_N = DSTAR_RADIO_SAMPLE_RATE / 500U;

const unsigned int MAX_DATA_SYNC_BIT_ERRS   = 2U;
const unsigned int MAX_END_PATTERN_BIT_ERRS = 4U;
const unsigned int MAX_SQUELCH_COUNT = 5U;
const unsigned int MAX_LOST_SYNCS    = 3U;

CSoundCardRepeaterRXThread::CSoundCardRepeaterRXThread() :
m_soundcard(NULL),
m_protocolHandler(NULL),
m_controller(NULL),
m_goertzel(DSTAR_RADIO_SAMPLE_RATE, GOERTZEL_FREQ, GOERTZEL_N, 0.1F),
m_inBuffer(DSTAR_RADIO_SAMPLE_RATE * 1U),			// One second of data
m_reader(NULL),
m_demodulator(),
m_bitBuffer(DV_FRAME_LENGTH_BITS),
m_rxState(DSRXS_LISTENING),
m_frameMatcher(FRAME_SYNC_LENGTH_BITS, FRAME_SYNC_BITS),
m_dataMatcher(DATA_FRAME_LENGTH_BITS, DATA_SYNC_BITS),
m_endMatcher(END_PATTERN_LENGTH_BITS, END_PATTERN_BITS),
m_header(NULL),
m_headerBER(0U),
m_radioSeqNo(0U),
m_radioSyncsLost(0U),
m_pollTimer(1000U, 60U),		// 60s
m_rptState(DSRS_LISTENING),
m_slowDataDecoder(),
m_squelch(false),
m_squelchCount(0U),
m_noise(0.0F),
m_noiseCount(0U),
m_killed(false),
m_squelchMode(SM_NORMAL),
m_ambe(),
m_ambeFrames(0U),
m_ambeSilence(0U),
m_ambeBits(1U),
m_ambeErrors(0U),
m_lastAMBEBits(0U),
m_lastAMBEErrors(0U)
{
	setRadioState(DSRXS_LISTENING);
}

CSoundCardRepeaterRXThread::~CSoundCardRepeaterRXThread()
{
	delete m_header;
}

void CSoundCardRepeaterRXThread::run()
{
	// Wait here until we have the essentials to run
	while (!m_killed && (m_soundcard == NULL || m_protocolHandler == NULL || m_controller == NULL))
		::wxMilliSleep(500UL);		// 1/2 sec

	if (m_killed)
		return;

	m_controller->setActive(false);

	m_pollTimer.start();

	m_inBuffer.clear();

	wxLogMessage(wxT("Starting the sound card receiver thread"));

	unsigned int count = 0U;

	wxStopWatch timer;

	while (!m_killed) {
		timer.Start();

		// Process the incoming D-Star transmission
		receiveRadio();

		// Process network traffic
		receiveNetwork();

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

		unsigned int ms = timer.Time();
		clock(ms);
	}

	wxLogMessage(wxT("Stopping the sound card receiver thread"));

	m_controller->setActive(false);
	m_controller->close();
	delete m_controller;

	m_soundcard->close();
	delete m_soundcard;

	if (m_reader != NULL) {
		m_reader->close();
		delete m_reader;
	}

	m_protocolHandler->close();
	delete m_protocolHandler;
}

void CSoundCardRepeaterRXThread::kill()
{
	m_killed = true;
}

void CSoundCardRepeaterRXThread::setReader(CWAVFileReader* reader)
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

void CSoundCardRepeaterRXThread::setCallsign(const wxString& callsign, const wxString& gateway, DSTAR_MODE mode, ACK_TYPE ack, bool restriction, bool rpt1Validation, bool dtmfBlanking)
{
}

void CSoundCardRepeaterRXThread::setProtocolHandler(CRepeaterProtocolHandler* handler)
{
	m_protocolHandler = handler;
}

void CSoundCardRepeaterRXThread::setSoundCard(CSoundCardReaderWriter* soundcard, wxFloat32 rxLevel, wxFloat32 txLevel, SQUELCH_MODE squelchMode, wxFloat32 squelchLevel, bool rxInvert, bool txInvert)
{
	wxASSERT(soundcard != NULL);

	m_soundcard = soundcard;

	m_squelchMode = squelchMode;
	m_goertzel.setThreshold(squelchLevel);

	m_demodulator.setInvert(rxInvert);
}

void CSoundCardRepeaterRXThread::setTimes(unsigned int timeout, unsigned int ackTime, unsigned int hangTime)
{
}

void CSoundCardRepeaterRXThread::setBeacon(unsigned int time, const wxString& text, bool voice, TEXT_LANG language)
{
}

void CSoundCardRepeaterRXThread::setAnnouncement(bool enabled, unsigned int time, const wxString& recordRPT1, const wxString& recordRPT2, const wxString& deleteRPT1, const wxString& deleteRPT2)
{
}

void CSoundCardRepeaterRXThread::setController(CExternalController* controller, int pttDelay)
{
	wxASSERT(controller != NULL);

	m_controller = controller;
}

void CSoundCardRepeaterRXThread::setControl(bool enabled, const wxString& rpt1Callsign, const wxString& rpt2Callsign, const wxString& shutdown, const wxString& startup, const wxString& status1, const wxString& status2, const wxString& status3, const wxString& status4, const wxString& status5, const wxString& command1, const wxString& command1Line, const wxString& command2, const wxString& command2Line, const wxString& command3, const wxString& command3Line, const wxString& command4, const wxString& command4Line, const wxString& output1, const wxString& output2, const wxString& output3, const wxString& output4)
{
}

void CSoundCardRepeaterRXThread::setActiveHang(unsigned int time)
{
}

void CSoundCardRepeaterRXThread::setOutputs(bool out1, bool out2, bool out3, bool out4)
{
}

void CSoundCardRepeaterRXThread::setRXLevel(wxFloat32 level)
{
}

void CSoundCardRepeaterRXThread::setTXLevel(wxFloat32 level)
{
}

void CSoundCardRepeaterRXThread::setSquelchLevel(wxFloat32 level)
{
	m_goertzel.setThreshold(level);
}

void CSoundCardRepeaterRXThread::setLogging(bool logging, const wxString& dir)
{
}

void CSoundCardRepeaterRXThread::setBlackList(CCallsignList* list)
{
}

void CSoundCardRepeaterRXThread::setGreyList(CCallsignList* list)
{
}

void CSoundCardRepeaterRXThread::receiveRadio()
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
		TRISTATE state = m_demodulator.decode(audio[i]);

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

void CSoundCardRepeaterRXThread::receiveNetwork()
{
	NETWORK_TYPE type;

	do {
		type = m_protocolHandler->read();
	} while (type != NETWORK_NONE);
}

void CSoundCardRepeaterRXThread::radioStateMachine(bool bit)
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

CHeaderData* CSoundCardRepeaterRXThread::processFECHeader()
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

bool CSoundCardRepeaterRXThread::processRadioHeader(CHeaderData* header)
{
	wxASSERT(header != NULL);

	// We don't handle DD data packets
	if (header->isDataPacket()) {
		wxLogMessage(wxT("Received a DD packet, ignoring"));
		delete header;
		return false;
	}

	m_rptState = DSRS_VALID;

	// Send the valid header to the gateway if we are accepted
	delete m_header;
	m_header = header;

	CHeaderData netHeader(*m_header);
	netHeader.setRptCall1(m_header->getRptCall2());
	netHeader.setRptCall2(m_header->getRptCall1());

	m_protocolHandler->writeHeader(netHeader);

	return true;
}

void CSoundCardRepeaterRXThread::processRadioFrame(FRAME_TYPE type)
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

CHeaderData* CSoundCardRepeaterRXThread::processSlowData(bool sync)
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

void CSoundCardRepeaterRXThread::endOfRadioData()
{
	wxLogMessage(wxT("AMBE for %s  Frames: %.1fs, Silence: %.1f%%, BER: %.1f%%"), m_header->getMyCall1().c_str(), float(m_ambeFrames) / 50.0F, float(m_ambeSilence * 100U) / float(m_ambeFrames), float(m_ambeErrors * 100U) / float(m_ambeBits));

	m_rptState = DSRS_LISTENING;

	m_protocolHandler->reset();
}

void CSoundCardRepeaterRXThread::callback(const wxFloat32* input, wxFloat32* output, unsigned int nSamples, int id)
{
	::memset(output, 0x00, nSamples * sizeof(wxFloat32));

	readRadioData(input, nSamples);
}

void CSoundCardRepeaterRXThread::setRadioState(DSTAR_RX_STATE state)
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

void CSoundCardRepeaterRXThread::readRadioData(const wxFloat32* input, unsigned int nSamples)
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

CSoundCardRepeaterStatusData* CSoundCardRepeaterRXThread::getStatus()
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

	if (m_rptState == DSRS_LISTENING)
		return new CSoundCardRepeaterStatusData(wxEmptyString, wxEmptyString, wxEmptyString, wxEmptyString,
				wxEmptyString, 0x00, 0x00, 0x00, false, m_squelch, m_rxState, m_rptState, 0U, 0U, 0U, 0U,
				0U, 0U, 0.0F, noise, wxEmptyString, wxEmptyString, wxEmptyString, wxEmptyString, wxEmptyString,
				wxEmptyString);
	else
		return new CSoundCardRepeaterStatusData(m_header->getMyCall1(), m_header->getMyCall2(),
				m_header->getYourCall(), m_header->getRptCall1(), m_header->getRptCall2(), 
				m_header->getFlag1(), m_header->getFlag2(), m_header->getFlag3(), false, m_squelch,
				m_rxState, m_rptState, 0U, 0U, 0U, 0U, 0U, 0U, (errors * 100.0F) / bits, noise,
				wxEmptyString, wxEmptyString, wxEmptyString, wxEmptyString, wxEmptyString, wxEmptyString);
}

void CSoundCardRepeaterRXThread::clock(unsigned int ms)
{
	m_pollTimer.clock(ms);
}

void CSoundCardRepeaterRXThread::shutdown()
{
}

void CSoundCardRepeaterRXThread::startup()
{
}

void CSoundCardRepeaterRXThread::command1()
{
}

void CSoundCardRepeaterRXThread::command2()
{
}

void CSoundCardRepeaterRXThread::command3()
{
}

void CSoundCardRepeaterRXThread::command4()
{
}
