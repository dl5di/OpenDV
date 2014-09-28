/*
 *   Copyright (C) 2009,2010,2012 by Jonathan Naylor G4KLX
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

#include "RadioHeaderDecoder.h"
#include "RadioHeaderEncoder.h"
#include "SoundCardClientThread.h"
#include "SoundCardClientApp.h"
#include "DStarDefines.h"
#include "MessageData.h"
#include "BusyData.h"

// #define	TX_TO_WAV_FILE

CSoundCardClientThread::CSoundCardClientThread() :
wxThread(wxTHREAD_JOINABLE),
m_audioSoundcard(NULL),
m_radioSoundcard(NULL),
m_dongle(NULL),
m_header(NULL),
m_controller(NULL),
m_pttEnable(false),
m_txInvert(false),
m_txDelay(NULL),
m_slowDataEncoder(),
m_slowDataDecoder(),
m_stopped(true),
m_radioBuffer(DSTAR_RADIO_BLOCK_SIZE * 30U),
m_decodeAudio(DSTAR_AUDIO_BLOCK_SIZE * 30U),
m_decodeData(VOICE_FRAME_LENGTH_BYTES * 30U),
m_encodeAudio(DSTAR_AUDIO_BLOCK_SIZE * 30U),
m_encodeData(VOICE_FRAME_LENGTH_BYTES * 30U),
m_transmitBits(DSTAR_GMSK_SYMBOL_RATE * 1U),		// 1s of transmit data
m_transmit(CLIENT_RECEIVE),
m_record(false),
m_recorder(NULL),
m_callsign1(),
m_callsign2(),
m_your(),
m_rpt1(),
m_rpt2(),
m_message(),
m_reader(NULL),
m_demodulator(),
m_modulator(),
m_bitBuffer(DV_FRAME_LENGTH_BITS),
m_frameCount(0U),
m_state(RX_STATE_LISTENING),
m_frameMatcher(FRAME_SYNC_LENGTH_BITS, FRAME_SYNC_BITS, 1U),
m_dataMatcher1(DATA_FRAME_LENGTH_BITS, DATA_SYNC_BITS, 1U),
m_dataMatcher2(DATA_FRAME_LENGTH_BITS, DATA_SYNC_BITS, 1U, DATA_BLOCK_SIZE_BITS * 2U),
m_endMatcher(END_PATTERN_LENGTH_BITS, END_PATTERN_BITS, 1U),
m_headerSeen(false),
m_writer(NULL),
m_killed(false),
m_tx(false)
{
	setState(RX_STATE_LISTENING);

	CDVTOOLFileWriter::setDirectory(::wxGetHomeDir());
}

CSoundCardClientThread::~CSoundCardClientThread()
{
}

void* CSoundCardClientThread::Entry()
{
	// Wait here until we have the essentials to run
	while (!m_killed && (m_dongle == NULL || m_audioSoundcard == NULL || m_radioSoundcard == NULL || m_controller == NULL ||
			m_callsign1.IsEmpty() || m_callsign1.IsSameAs(wxT("        "))))
		Sleep(500UL);		// 1/2 sec

	if (m_killed)
		return NULL;

	m_stopped = false;

	m_dongle->Create();
	m_dongle->SetPriority(100U);
	m_dongle->Run();

	if (m_txInvert)
		m_controller->setTransmit(true);
	else
		m_controller->setTransmit(false);

	while (!m_killed) {
		if (m_transmit == CLIENT_RECEIVE)
			receive();
		else
			transmit();
	}

	if (m_txInvert)
		m_controller->setTransmit(true);
	else
		m_controller->setTransmit(false);

	m_dongle->kill();
	m_dongle->Wait();

	m_audioSoundcard->close();
	m_radioSoundcard->close();

	delete m_audioSoundcard;
	delete m_radioSoundcard;

	m_controller->close();

	delete m_txDelay;

	if (m_reader != NULL) {
		m_reader->close();
		delete m_reader;
	}

#if defined(TX_TO_WAV_FILE)
	if (m_writer != NULL) {
		m_writer->close();
		delete m_writer;
	}
#endif

	return NULL;
}

void CSoundCardClientThread::kill()
{
	m_killed = true;
}

void CSoundCardClientThread::setReader(CWAVFileReader* reader)
{
	wxASSERT(reader != NULL);

	// Already busy?
	if (m_reader != NULL) {
		reader->close();
		delete reader;
		return;
	}

	resetReceiver();

	wxLogInfo(wxT("Reading from WAV file - %s"), reader->getFilename().c_str());

	m_reader = reader;
}

void CSoundCardClientThread::setCallsign(const wxString& callsign1, const wxString& callsign2)
{
	m_callsign1 = callsign1;
	m_callsign2 = callsign2;
}

void CSoundCardClientThread::setAudioSoundCard(CSoundCardReaderWriter* soundcard)
{
	wxASSERT(soundcard != NULL);

	if (!m_stopped) {
		soundcard->close();
		delete soundcard;
		return;
	}

	if (m_audioSoundcard != NULL) {
		m_audioSoundcard->close();
		delete m_audioSoundcard;
	}

	m_audioSoundcard = soundcard;
}

void CSoundCardClientThread::setRadioSoundCard(CSoundCardReaderWriter* soundcard)
{
	wxASSERT(soundcard != NULL);

	if (!m_stopped) {
		soundcard->close();
		delete soundcard;
		return;
	}

	if (m_radioSoundcard != NULL) {
		m_radioSoundcard->close();
		delete m_radioSoundcard;
	}

	m_radioSoundcard = soundcard;
}

void CSoundCardClientThread::setInversion(bool rxInvert, bool txInvert)
{
	m_demodulator.setInvert(rxInvert);
	m_modulator.setInvert(txInvert);
}

void CSoundCardClientThread::setDVDongle(CDVDongleController* dongle)
{
	wxASSERT(dongle != NULL);

	if (!m_stopped) {
		delete dongle;
		return;
	}

	if (m_dongle != NULL) {
		m_dongle->kill();
		m_dongle->Wait();
	}

	m_dongle = new CDVDongleThread(dongle, &m_decodeAudio, &m_decodeData, &m_encodeAudio, &m_encodeData);
}

void CSoundCardClientThread::setController(IExternalController* controller, bool pttEnable, unsigned int txDelay, bool txInvert)
{
	wxASSERT(controller != NULL);

	if (!m_stopped) {
		controller->close();
		return;
	}

	if (m_controller != NULL) {
		m_controller->close();
		delete m_txDelay;
		m_txDelay = NULL;
	}

	m_controller = controller;
	m_pttEnable  = pttEnable;
	m_txInvert   = txInvert;

	if (txDelay > 0U)
		m_txDelay = new CPTTDelay(txDelay);
}

void CSoundCardClientThread::setMessage(const wxString& text)
{
	m_message = text;
}

void CSoundCardClientThread::setRecording(bool record)
{
	m_record = record;

	if (!record && m_recorder != NULL) {
		m_recorder->close();
		delete m_recorder;
		m_recorder = NULL;
	}
}

void CSoundCardClientThread::setBleep(bool on)
{
	if (m_dongle != NULL)
		m_dongle->setBleep(on);
}

void CSoundCardClientThread::setYour(const wxString& your)
{
	m_your = your;
}

void CSoundCardClientThread::setRpt1(const wxString& rpt1)
{
	// An empty RPT1 callsign also implies an empty RPT2
	if (rpt1.IsSameAs(UNUSED)) {
		m_rpt1 = wxT("        ");
		m_rpt2 = wxT("        ");
	} else {
		m_rpt1 = rpt1;
	}
}

void CSoundCardClientThread::setRpt2(const wxString& rpt2)
{
	// An empty RPT2 callsign
	if (rpt2.IsSameAs(UNUSED))
		m_rpt2 = wxT("        ");
	else
		m_rpt2 = rpt2;
}

bool CSoundCardClientThread::setTransmit(bool transmit)
{
	if (m_stopped)
		return false;

	if (transmit) {
		delete m_header;		// The previous value, if any

		m_header = new CHeaderData(m_callsign1, m_callsign2, m_your, m_rpt1, m_rpt2);
		if (m_rpt1.IsEmpty() || m_rpt1.IsSameAs(wxT("        ")) || m_rpt1.IsSameAs(wxT("DIRECT  ")))
			m_header->setRepeaterMode(false);
		else
			m_header->setRepeaterMode(true);

		m_slowDataEncoder.reset();
		m_slowDataEncoder.setHeaderData(*m_header);

		if (!m_message.IsEmpty())
			m_slowDataEncoder.setMessageData(m_message);

		m_transmit = CLIENT_TRANSMIT;
	} else {
		resetReceiver();

		m_transmit = CLIENT_WANT_RECEIVE;

		m_tx = false;
	}

	return true;
}

void CSoundCardClientThread::receive()
{
	resetReceiver();

	// While receiving and not exitting
	while (m_transmit == CLIENT_RECEIVE && !m_killed) {
		// Get the audio from the RX
		wxFloat32 audio[DSTAR_RADIO_BLOCK_SIZE];

		unsigned int length = 0U;
		for (unsigned int n = 0U; length < DSTAR_RADIO_BLOCK_SIZE && n < 20U; n++) {
			length += m_radioBuffer.getData(audio + length, DSTAR_RADIO_BLOCK_SIZE - length);

			if (length < DSTAR_RADIO_BLOCK_SIZE)
				Sleep(FRAME_TIME_MS / 4UL);
		}

		if (length < DSTAR_RADIO_BLOCK_SIZE)
			wxLogError(wxT("No audio is being received"));

		for (unsigned int i = 0U; i < length; i++) {
			DEMOD_STATE state = m_demodulator.decode(audio[i]);

			switch (state) {
				case DEMOD_TRUE:
					stateMachine(true);
					break;
				case DEMOD_FALSE:
					stateMachine(false);
					break;
				case DEMOD_UNKNOWN:
					break;
			}
		}

		// Check the transmit button, if it's enabled
		if (m_pttEnable) {
			if (m_controller->getSquelch())
				m_tx = setTransmit(true);
		}

		// Feed the PTT delay with transmit off entries
		bool tx = (m_txInvert) ? true : false;

		if (m_txDelay != NULL)
			m_controller->setTransmit(m_txDelay->delay(tx));
		else
			m_controller->setTransmit(tx);
	}
}

void CSoundCardClientThread::transmit()
{
	m_radioBuffer.clear();
	m_transmitBits.clear();
	m_encodeAudio.clear();

	m_encodeData.clear();
	m_dongle->setMode(DVDMODE_ENCODE);

	wxASSERT(m_header != NULL);
	wxLogMessage(wxT("Transmitting to - My: %s/%s  Your: %s  Rpt1: %s  Rpt2: %s  Flags: %02X %02X %02X"), m_header->getMyCall1().c_str(), m_header->getMyCall2().c_str(), m_header->getYourCall().c_str(), m_header->getRptCall1().c_str(), m_header->getRptCall2().c_str(), m_header->getFlag1(), m_header->getFlag2(), m_header->getFlag3());

#if defined(TX_TO_WAV_FILE)
	if (m_writer != NULL) {
		m_writer->close();
		delete m_writer;
		m_writer = NULL;
	}

	wxDateTime time;
	time.SetToCurrent();

	// This is for logging the output audio to a WAV file
	wxString fileName = time.Format(wxT("%Y%m%d-%H%M%S.wav"));
	CWAVFileWriter* writer = new CWAVFileWriter(fileName, DSTAR_RADIO_SAMPLE_RATE, 1U, 16U, DSTAR_RADIO_BLOCK_SAMPLES);

	bool res = writer->open();
	if (!res) {
		wxLogError(wxT("Cannot open the output WAV file - %s"), fileName.c_str());
		delete writer;
	} else {
		wxLogMessage(wxT("Opened the output WAV file - %s"), fileName.c_str());
		m_writer = writer;
	}
#endif

	// Pause until all the silence data has been processed by the AMBE2020
	unsigned int startCount = 30U;
	while (startCount > 0U) {
		unsigned char frame[DV_FRAME_LENGTH_BYTES];
		unsigned int n = m_encodeData.getData(frame, VOICE_FRAME_LENGTH_BYTES);
		if (n > 0U)
			startCount--;

		Sleep(FRAME_TIME_MS / 4UL);
	}

	// Send the bit sync, 700 bits in 4 bit chunks
	for (unsigned int i = 0U; i < 175U; i++)
		m_transmitBits.addData(BIT_SYNC_BITS, BIT_SYNC_LENGTH_BITS);

	// Send the frame sync, 15 bits
	m_transmitBits.addData(FRAME_SYNC_BITS, FRAME_SYNC_LENGTH_BITS);

	// Send the header, 660 bits
	bool headerBits[FEC_SECTION_LENGTH_BITS];
	CRadioHeaderEncoder encoder(*m_header);
	encoder.getRadioData(headerBits, FEC_SECTION_LENGTH_BITS);
	m_transmitBits.addData(headerBits, FEC_SECTION_LENGTH_BITS);

	transmitBits();

	unsigned int frameCount = 20U;

	unsigned int endCount = 30U;

	// While transmitting and not exiting
	while (endCount > 0U && !m_killed) {
		unsigned char frame[DV_FRAME_LENGTH_BYTES];
		unsigned int n = m_encodeData.getData(frame, VOICE_FRAME_LENGTH_BYTES);

		if (n == VOICE_FRAME_LENGTH_BYTES) {
			if (frameCount == 20U) {
				// Put in the data resync pattern
				::memcpy(frame + VOICE_FRAME_LENGTH_BYTES, DATA_SYNC_BYTES, DATA_FRAME_LENGTH_BYTES);
				frameCount = 0U;
			} else {
				// Tack the slow data on the end
				m_slowDataEncoder.getData(frame + VOICE_FRAME_LENGTH_BYTES);
				frameCount++;
			}

			// Send the AMBE and slow data frame
			for (unsigned int i = 0U; i < DV_FRAME_LENGTH_BYTES; i++) {
				bool bits[8U];
				byteToBits(frame[i], bits);
				m_transmitBits.addData(bits, 8U);
			}

			transmitBits();

			// Check for the release of the transmit button, if it's enabled
			if (m_tx && m_pttEnable) {
				if (!m_controller->getSquelch()) {
					setTransmit(false);
					m_tx = false;
				}
			}

			// Make transmit hang so that all the audio is flushed
			if (m_transmit != CLIENT_TRANSMIT)
				endCount--;
		}

		Sleep(FRAME_TIME_MS / 4UL);
	}

	m_dongle->setMode(DVDMODE_IDLE);

	// Send the end pattern
	m_transmitBits.addData(END_PATTERN_BITS, END_PATTERN_LENGTH_BITS);
	m_transmitBits.addData(END_PATTERN_BITS, END_PATTERN_LENGTH_BITS);
	m_transmitBits.addData(END_PATTERN_BITS, END_PATTERN_LENGTH_BITS);

	// Wait until all the GMSK has been transmitted
	while (!m_transmitBits.isEmpty() && !m_radioBuffer.isEmpty()) {
		transmitBits();
		Sleep(FRAME_TIME_MS);
	}

	// Wait for all the GMSK audio to leave
	Sleep(FRAME_TIME_MS * 10U);

#if defined(TX_TO_WAV_FILE)
	if (m_writer != NULL) {
		wxLogMessage(wxT("Closing the output WAV file"));
		m_writer->close();
		delete m_writer;
		m_writer = NULL;
	}
#endif

	wxLogMessage(wxT("Finished transmitting"));

	m_transmit = CLIENT_RECEIVE;
}

void CSoundCardClientThread::transmitBits()
{
	bool tx = false;

	if (!m_transmitBits.isEmpty() && m_radioBuffer.isEmpty()) {
		// Switch on the transmitter
		tx = true;

		// Put quite a lot of data into the buffer to allow for slow downs
		bool bits[DV_FRAME_LENGTH_BITS * 12U];
		unsigned int length = m_transmitBits.getData(bits, DV_FRAME_LENGTH_BITS * 12U);

		for (unsigned int i = 0U; i < length; i++) {
			wxFloat32 buffer[DSTAR_RADIO_BIT_LENGTH];
			unsigned int len = m_modulator.code(bits[i], buffer, DSTAR_RADIO_BIT_LENGTH);

			m_radioBuffer.addData(buffer, len);
		}
	} else if (!m_transmitBits.isEmpty() && !m_radioBuffer.isEmpty()) {
		// Switch on the transmitter
		tx = true;

		bool bits[DV_FRAME_LENGTH_BITS];
		unsigned int length = m_transmitBits.getData(bits, DV_FRAME_LENGTH_BITS);

		for (unsigned int i = 0U; i < length; i++) {
			wxFloat32 buffer[DSTAR_RADIO_BIT_LENGTH];
			unsigned int len = m_modulator.code(bits[i], buffer, DSTAR_RADIO_BIT_LENGTH);

			m_radioBuffer.addData(buffer, len);
		}
	} else if (m_transmitBits.isEmpty() && !m_radioBuffer.isEmpty()) {
		// Switch on the transmitter
		tx = true;
	}

	// Invert the PTT?
	if (m_txInvert)
		tx = !tx;

	if (m_txDelay != NULL)
		m_controller->setTransmit(m_txDelay->delay(tx));
	else
		m_controller->setTransmit(tx);
}

// XXX Some of this should go into EndOfData()
void CSoundCardClientThread::resetReceiver()
{
	setState(RX_STATE_LISTENING);

	m_demodulator.reset();

	endOfData();
}

void CSoundCardClientThread::stateMachine(bool bit)
{
	switch (m_state) {
		case RX_STATE_LISTENING: {
				bool res1 = m_frameMatcher.add(bit);
				bool res2 = m_dataMatcher1.add(bit);

				// The frame sync has been seen
				if (res1) {
					// wxLogInfo(wxT("Found frame sync"));
					setState(RX_STATE_PROCESS_HEADER);
				}

				// This may cause R2D2, but that's how it is
				if (res2) {
					wxLogInfo(wxT("Found data sync"));
					setState(RX_STATE_PROCESSING);
				}
			}
			break;

		case RX_STATE_PROCESS_HEADER:
			m_bitBuffer.add(bit);
			if (m_bitBuffer.getLength() == FEC_SECTION_LENGTH_BITS) {
				bool res = processHeader();
				if (!res) {
					// Either the checksum failed, or this is a DD packet
					// wxLogInfo(wxT("Radio header invalid"));
					setState(RX_STATE_LISTENING);
				} else {
					// A valid header and is a DV packet
					// wxLogInfo(wxT("Found valid radio header"));
					setState(RX_STATE_PROCESSING);
				}
			}
			break;

		case RX_STATE_PROCESSING: {
				m_bitBuffer.add(bit);
				bool res1       = m_endMatcher.add(bit);
				LPM_STATUS res2 = m_dataMatcher2.add(bit);

				if (res1) {
					wxLogInfo(wxT("Found end sync"));
					setState(RX_STATE_LISTENING);
					break;
				}

				if (m_bitBuffer.getLength() == DV_FRAME_LENGTH_BITS) {
					if (res2 == LPM_FOUND) {
						// wxLogInfo(wxT("Found data re-sync at %u"), m_frameCount);
						processFrame(true);
						m_dataMatcher2.resetAll();
						m_frameCount = 0U;
					} else {
						// Allow a long grace period
						if (m_frameCount > 51U) {	// Just over 1s
							// wxLogInfo(wxT("Data re-sync not found in time at %u"), m_frameCount);
							setState(RX_STATE_LISTENING);
						} else {
							processFrame(false);
							m_frameCount++;
						}
					}

					m_bitBuffer.clear();
				}
			}
			break;
	}
}

bool CSoundCardClientThread::processHeader()
{
	bool buffer[FEC_SECTION_LENGTH_BITS];
	m_bitBuffer.getData(buffer, FEC_SECTION_LENGTH_BITS);

	CRadioHeaderDecoder decoder(buffer, FEC_SECTION_LENGTH_BITS);

	CHeaderData* header = decoder.getHeaderData();
	if (header == NULL) {
		wxLogMessage(wxT("Header checksum failure"));
		return false;
	}

	unsigned int ber = decoder.getBER();

	wxLogMessage(wxT("Header decoded - My: %s/%s  Your: %s  Rpt1: %s  Rpt2: %s  Flags: %02X %02X %02X  BER: %u%%"), header->getMyCall1().c_str(), header->getMyCall2().c_str(), header->getYourCall().c_str(), header->getRptCall1().c_str(), header->getRptCall2().c_str(), header->getFlag1(), header->getFlag2(), header->getFlag3(), ber);

	// We don't handle DD data packets
	if (header->isDataPacket()) {
		wxLogMessage(wxT("Received a DD packet, ignoring"));
		delete header;
		return false;
	}

	// To gate data going to the GUI
	m_headerSeen = true;

	if (m_record) {
		if (m_recorder != NULL) {
			m_recorder->close();
			delete m_recorder;
		}

		m_recorder = new CDVTOOLFileWriter;
		bool res = m_recorder->open(*header);

		if (res) {
			// Write the raw header data
			wxLogMessage(wxT("Recording to %s"), m_recorder->getFileName().c_str());
			unsigned char buffer[RADIO_HEADER_LENGTH_BYTES];
			decoder.getHeaderBytes(buffer, RADIO_HEADER_LENGTH_BYTES);
			m_recorder->writeHeader(buffer, RADIO_HEADER_LENGTH_BYTES);
		} else {
			// Can't open the file
			wxLogError(wxT("Couldn't open the recording file - %s"), m_recorder->getFileName().c_str());
			::wxGetApp().error(_("Can't open the recording file"));
			delete m_recorder;
			m_recorder = NULL;
		}
	}

	::wxGetApp().showHeader(header);

	return true;
}

void CSoundCardClientThread::processFrame(bool sync)
{
	bool bitBuffer[DV_FRAME_LENGTH_BITS];
	m_bitBuffer.getData(bitBuffer, DV_FRAME_LENGTH_BITS);

	// Convert the data in Icom bit order
	unsigned int n = 0U;
	unsigned char byteBuffer[DV_FRAME_LENGTH_BYTES];
	for (unsigned int i = 0U; i < DV_FRAME_LENGTH_BYTES; i++, n += 8U)
		byteBuffer[i] = bitsToByte(bitBuffer + n);

	if (m_recorder != NULL)
		m_recorder->writeFrame(byteBuffer, DV_FRAME_LENGTH_BYTES);

	// Only process the slow data if this isn't a data sync frame
	if (!sync)
		processSlowData(byteBuffer + VOICE_FRAME_LENGTH_BYTES);
	else
		m_slowDataDecoder.sync();

	m_decodeData.addData(byteBuffer, VOICE_FRAME_LENGTH_BYTES);
}

void CSoundCardClientThread::processSlowData(const unsigned char* buffer)
{
	m_slowDataDecoder.addData(buffer);

	// If the header was missed for some reason, use the one from the slow data instead
	if (!m_headerSeen) {
		CHeaderData* header = m_slowDataDecoder.getHeaderData();
		if (header != NULL) {
			wxLogMessage(wxT("Header decoded from slow data - My: %s/%s  Your: %s  Rpt1: %s  Rpt2: %s  Flags: %02X %02X %02X"), header->getMyCall1().c_str(), header->getMyCall2().c_str(), header->getYourCall().c_str(), header->getRptCall1().c_str(), header->getRptCall2().c_str(), header->getFlag1(), header->getFlag2(), header->getFlag3());

			if (m_record) {
				if (m_recorder != NULL) {
					m_recorder->close();
					delete m_recorder;
				}

				m_recorder = new CDVTOOLFileWriter;
				bool res = m_recorder->open(*header);

				if (res) {
					// Write the raw header data
					wxLogMessage(wxT("Recording to %s"), m_recorder->getFileName().c_str());
					unsigned char buffer[RADIO_HEADER_LENGTH_BYTES];
					m_slowDataDecoder.getHeader(buffer, RADIO_HEADER_LENGTH_BYTES);
					m_recorder->writeFrame(buffer, RADIO_HEADER_LENGTH_BYTES);
				} else {
					// Can't open the file
					wxLogError(wxT("Couldn't open the recording file - %s"), m_recorder->getFileName().c_str());
					::wxGetApp().error(_("Can't open the recording file"));
					delete m_recorder;
					m_recorder = NULL;
				}
			}

			m_headerSeen = true;

			::wxGetApp().showHeader(header);
		}
	}

	// Only tell the GUI if it has the header already
	if (m_headerSeen) {
		CMessageData* message = m_slowDataDecoder.getMessageData();
		if (message != NULL)
			::wxGetApp().showMessage(message);
	}
}

void CSoundCardClientThread::callback(const wxFloat32* input, wxFloat32* output, unsigned int nSamples, int id)
{
	::memset(output, 0x00, nSamples * sizeof(wxFloat32));

	if (m_transmit != CLIENT_RECEIVE) {
		switch (id) {
			case USER_AUDIO_ID:
				m_encodeAudio.addData(input, nSamples);
				break;
			case RADIO_AUDIO_ID:
				m_radioBuffer.getData(output, nSamples);

#if defined(TX_TO_WAV_FILE)
				// If writing the transmit audio to a WAV file
				if (m_writer != NULL)
					m_writer->write(output, nSamples);
#endif
				break;
		}
	} else {
		if (id == RADIO_AUDIO_ID)
			readRadioData(input, nSamples);
	}

	if (id == USER_AUDIO_ID)
		m_decodeAudio.getData(output, nSamples);
}

void CSoundCardClientThread::endOfData()
{
	// Tell the GUI
	::wxGetApp().showBusy(new CBusyData(false));

	// Close the recording file, if open
	if (m_recorder != NULL) {
		m_recorder->close();
		delete m_recorder;
		m_recorder = NULL;
	}

	m_slowDataDecoder.reset();

	m_headerSeen = false;
}

unsigned char CSoundCardClientThread::bitsToByte(const bool* bits) const
{
	wxASSERT(bits != NULL);

	unsigned char val = 0x00;

	for (unsigned int i = 0U; i < 8U; i++) {
		val >>= 1;

		if (bits[i])
			val |= 0x80;
	}

	return val;
}

void CSoundCardClientThread::byteToBits(unsigned char byte, bool* data) const
{
	wxASSERT(data != NULL);

	unsigned char mask = 0x01U;
	for (unsigned int i = 0U; i < 8U; i++, mask <<= 1)
		data[i] = (byte & mask) ? true : false;
}

void CSoundCardClientThread::setState(RX_STATE state)
{
	// This is the from state
	switch (m_state) {
		case RX_STATE_LISTENING:
			m_frameCount = 0U;
			break;

		case RX_STATE_PROCESS_HEADER:
			m_frameCount = 20U;
			break;

		case RX_STATE_PROCESSING:
			endOfData();
			m_dongle->setMode(DVDMODE_IDLE);
			break;
	}

	// This is the too state
	switch (state) {
		case RX_STATE_LISTENING:
			m_frameMatcher.reset();
			m_dataMatcher1.reset();
			m_state = RX_STATE_LISTENING;
			break;

		case RX_STATE_PROCESS_HEADER:
			m_bitBuffer.setMaxLength(FEC_SECTION_LENGTH_BITS);
			m_state = RX_STATE_PROCESS_HEADER;
			break;

		case RX_STATE_PROCESSING:
			m_bitBuffer.setMaxLength(DV_FRAME_LENGTH_BITS);
			::wxGetApp().showBusy(new CBusyData(true));
			m_endMatcher.reset();
			m_dataMatcher2.resetAll();
			m_state = RX_STATE_PROCESSING;
			m_slowDataDecoder.reset();
			m_decodeData.clear();
			m_decodeAudio.clear();
			m_dongle->setMode(DVDMODE_DECODE);
			break;
	}
}

void CSoundCardClientThread::readRadioData(const wxFloat32* input, unsigned int nSamples)
{
	wxASSERT(input != NULL);
	wxASSERT(nSamples > 0U);

	if (m_reader != NULL) {		// Are we reading from a WAV file?
		wxFloat32 buffer[DSTAR_RADIO_BLOCK_SIZE];
		unsigned int n = m_reader->read(buffer, DSTAR_RADIO_BLOCK_SIZE);
		if (n == 0U) {				// EOF on the input WAV file?
			CWAVFileReader* reader = m_reader;
			m_reader = NULL;

			reader->close();
			delete reader;

			wxLogInfo(wxT("End of the input file"));
			setState(RX_STATE_LISTENING);

			// Send data from the sound card as the file has ended
			m_radioBuffer.addData(input, nSamples);

			return;
		}

		// Send data from the sound file
		m_radioBuffer.addData(buffer, n);
	} else {
		// Send data from the sound card
		m_radioBuffer.addData(input, nSamples);
	}
}
