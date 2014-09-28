/*
 *   Copyright (C) 2010,2011,2012,2013 by Jonathan Naylor G4KLX
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

#include "DExtraClientThread.h"
#include "DExtraClientApp.h"
#include "DStarDefines.h"
#include "MessageData.h"

// #define	DUMP_AMBE


CDExtraClientThread::CDExtraClientThread() :
wxThread(wxTHREAD_JOINABLE),
m_soundcard(NULL),
m_dongle(NULL),
m_controller(NULL),
m_pttInvert(false),
m_squelchInvert(false),
m_audioDelay(NULL),
m_slowDataEncoder(),
m_slowDataDecoder(),
m_protocol(),
m_stopped(true),
m_decodeAudio(DSTAR_AUDIO_BLOCK_SIZE * 30U),
m_decodeData(VOICE_FRAME_LENGTH_BYTES * 30U),
m_encodeAudio(DSTAR_AUDIO_BLOCK_SIZE * 30U),
m_encodeData(VOICE_FRAME_LENGTH_BYTES * 30U),
m_transmit(CLIENT_RECEIVE),
m_record(false),
m_recorder(NULL),
m_callsign(),
m_message(),
m_frameCount(0U),
m_killed(false),
m_reflector(),
m_module(wxT(' ')),
m_started(false),
m_watchdog(TICKS_PER_SEC, 1U),
m_dumper(NULL),
m_tx(false)
{
	CDVTOOLFileWriter::setDirectory(::wxGetHomeDir());
}

CDExtraClientThread::~CDExtraClientThread()
{
}

void* CDExtraClientThread::Entry()
{
	// Wait here until we have the essentials to run
	while (!m_killed && (m_dongle == NULL || m_soundcard == NULL || m_controller == NULL || m_callsign.IsEmpty()))
		Sleep(500UL);		// 1/2 sec

	if (m_killed)
		return NULL;

	m_stopped = false;

	m_dongle->Create();
	m_dongle->SetPriority(100U);
	m_dongle->Run();

	if (m_pttInvert)
		m_controller->setTransmit(true);
	else
		m_controller->setTransmit(false);

	while (!m_killed) {
		if (m_transmit == CLIENT_RECEIVE)
			receive();
		else
			transmit();
	}

	if (m_pttInvert)
		m_controller->setTransmit(true);
	else
		m_controller->setTransmit(false);

	m_dongle->kill();
	m_dongle->Wait();

	m_soundcard->close();
	delete m_soundcard;

	m_controller->close();

	delete m_audioDelay;

	return NULL;
}

void CDExtraClientThread::kill()
{
	m_killed = true;
}

void CDExtraClientThread::setCallsign(const wxString& callsign)
{
	m_callsign = callsign;

	m_protocol.setCallsign(callsign);
}

void CDExtraClientThread::setSoundCard(CSoundCardReaderWriter* soundcard)
{
	wxASSERT(soundcard != NULL);

	if (!m_stopped) {
		soundcard->close();
		delete soundcard;
		return;
	}

	if (m_soundcard != NULL) {
		m_soundcard->close();
		delete m_soundcard;
	}

	m_soundcard = soundcard;
}

void CDExtraClientThread::setDVDongle(CDVDongleController* dongle)
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

void CDExtraClientThread::setMessage(const wxString& text)
{
	m_message = text;
}

void CDExtraClientThread::setRecording(bool record)
{
	m_record = record;

	if (!record && m_recorder != NULL) {
		m_recorder->close();
		delete m_recorder;
		m_recorder = NULL;
	}
}

void CDExtraClientThread::setBleep(bool on)
{
	if (m_dongle != NULL)
		m_dongle->setBleep(on);
}

void CDExtraClientThread::setController(IExternalController* controller, unsigned int audioDelay, bool pttInvert, bool squelchInvert)
{
	wxASSERT(controller != NULL);

	if (!m_stopped) {
		controller->close();
		return;
	}

	if (m_controller != NULL) {
		m_controller->close();
		delete m_audioDelay;
		m_audioDelay = NULL;
	}

	m_controller    = controller;
	m_pttInvert     = pttInvert;
	m_squelchInvert = squelchInvert;

	if (audioDelay > 0U)
		m_audioDelay = new CAudioDelay(audioDelay * FRAME_TIME_MS * DSTAR_RADIO_SAMPLE_RATE / 1000U);
}

bool CDExtraClientThread::connect(bool connect, const wxString& reflector, const wxString& address)
{
	if (m_stopped)
		return false;

	m_reflector = reflector;

	if (m_protocol.isConnected())
		m_protocol.close();

	if (connect) {
		bool res = m_protocol.open(address, DEXTRA_PORT);
		if (!res)
			return false;

		wxLogMessage(wxT("Connected to %s on address %s"), reflector.c_str(), address.c_str());
	} else {
		m_protocol.close();

		wxLogMessage(wxT("Disconnected"));
	}

	return true;
}

bool CDExtraClientThread::transmit(bool transmit)
{
	if (m_stopped)
		return false;

	if (transmit) {
		if (!m_protocol.isConnected())
			return false;

		m_transmit = CLIENT_TRANSMIT;
	} else {
		resetReceiver();

		m_transmit = CLIENT_WANT_RECEIVE;

		m_tx = false;
	}

	return true;
}

void CDExtraClientThread::setModule(wxChar module)
{
	wxLogMessage(wxT("Module set to %c"), module);

	m_module = module;

	m_protocol.setModule(module);
}

bool CDExtraClientThread::isConnected()
{
	return m_protocol.isConnected();
}

void CDExtraClientThread::receive()
{
	// While receiving and not exitting
	while (m_transmit == CLIENT_RECEIVE && !m_killed) {
		// Get the audio from the RX
		NETWORK_TYPE type = m_protocol.read();
		switch (type) {
			case NETWORK_NONE:
				break;

			case NETWORK_HEADER: {
					CHeaderData* header = m_protocol.readHeader();
					if (header != NULL)
						processHeader(header);
				}
				break;

			case NETWORK_DATA: {
					unsigned char buffer[DV_FRAME_LENGTH_BYTES];
					unsigned int length;
					bool sync, end;
					length = m_protocol.readData(buffer, DV_FRAME_LENGTH_BYTES, sync, end);
					processFrame(buffer, sync, end);
				}
				break;
		}

		if (m_watchdog.hasExpired()) {
			m_dongle->setMode(DVDMODE_IDLE);
			m_protocol.reset();
			resetReceiver();
		}

		if (m_squelchInvert) {
			if (!m_controller->getSquelch())
				m_tx = transmit(true);
		} else {
			if (m_controller->getSquelch())
				m_tx = transmit(true);
		}

		Sleep(FRAME_TIME_MS / 4UL);
	}
}

void CDExtraClientThread::transmit()
{
	m_encodeData.clear();
	m_dongle->setMode(DVDMODE_ENCODE);

	// Pause until all the silence data has been processed by the AMBE2020
	unsigned int startCount = 30U;
	while (startCount > 0U) {
		unsigned char frame[DV_FRAME_LENGTH_BYTES];
		unsigned int n = m_encodeData.getData(frame, VOICE_FRAME_LENGTH_BYTES);
		if (n > 0U)
			startCount--;

		Sleep(FRAME_TIME_MS / 4UL);
	}

	wxString rpt1 = m_reflector;
	rpt1.Append(wxT("        "));
	rpt1.Truncate(LONG_CALLSIGN_LENGTH - 1U);
	rpt1.Append(m_module);

	wxString rpt2 = m_reflector;
	rpt2.Append(wxT("        "));
	rpt2.Truncate(LONG_CALLSIGN_LENGTH - 1U);
	rpt2.Append(wxT('G'));

	CHeaderData* header = new CHeaderData(m_callsign, wxT("DNGL"), wxT("CQCQCQ  "), rpt1, rpt2);

	m_slowDataEncoder.reset();
	m_slowDataEncoder.setHeaderData(*header);

#if defined(DUMP_AMBE)
	m_dumper = new CDVTOOLFileWriter;
	bool res = m_dumper->open(*header);
	if (!res) {
		delete m_dumper;
		m_dumper = NULL;
	} else {
		wxLogMessage(wxT("Dumping AMBE to %s"), m_dumper->getFileName().c_str());
		m_dumper->writeHeader(*header);
	}
#endif
	if (!m_message.IsEmpty())
		m_slowDataEncoder.setMessageData(m_message);

	m_protocol.writeHeader(*header);
	delete header;

	m_frameCount = 20U;

	unsigned int endCount = 30U;

	// While transmitting and not exiting
	for (;;) {
		unsigned char frame[DV_FRAME_LENGTH_BYTES];
		unsigned int n = m_encodeData.getData(frame, VOICE_FRAME_LENGTH_BYTES);
		if (n > 0U) {
			if (m_frameCount == 20U) {
				// Put in the data resync pattern
				::memcpy(frame + VOICE_FRAME_LENGTH_BYTES, DATA_SYNC_BYTES, DATA_FRAME_LENGTH_BYTES);
				m_frameCount = 0U;
			} else {
				// Tack the slow data on the end
				m_slowDataEncoder.getData(frame + VOICE_FRAME_LENGTH_BYTES);
				m_frameCount++;
			}

			if (m_transmit != CLIENT_TRANSMIT)
				endCount--;

#if defined(DUMP_AMBE)
			if (m_dumper != NULL)
				m_dumper->writeFrame(frame, DV_FRAME_LENGTH_BYTES);
#endif
			// Send the AMBE and slow data frame
			if (endCount == 0U || m_killed) {
				m_protocol.writeData(frame, DV_FRAME_LENGTH_BYTES, true);
				break;
			} else {
				m_protocol.writeData(frame, DV_FRAME_LENGTH_BYTES, false);
			}

			if (m_tx) {
				if (m_squelchInvert) {
					if (m_controller->getSquelch()) {
						transmit(false);
						m_tx = false;
					}
				} else {
					if (!m_controller->getSquelch()) {
						transmit(false);
						m_tx = false;
					}
				}
			}
		}

		Sleep(FRAME_TIME_MS / 4UL);
	}

	m_dongle->setMode(DVDMODE_IDLE);

#if defined(DUMP_AMBE)
	if (m_dumper != NULL) {
		m_dumper->close();
		delete m_dumper;
		m_dumper = NULL;
	}
#endif

	resetReceiver();

	m_transmit = CLIENT_RECEIVE;
}

void CDExtraClientThread::processHeader(CHeaderData* header)
{
	wxASSERT(header != NULL);

	wxLogMessage(wxT("Header decoded - My: %s/%s  Your: %s  Rpt1: %s  Rpt2: %s"), header->getMyCall1().c_str(), header->getMyCall2().c_str(), header->getYourCall().c_str(), header->getRptCall1().c_str(), header->getRptCall2().c_str());

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
			m_recorder->writeHeader(*header);
		} else {
			// Can't open the file
			wxLogError(wxT("Couldn't open the recording file - %s"), m_recorder->getFileName().c_str());
			::wxGetApp().error(_("Can't open the recording file"));
			delete m_recorder;
			m_recorder = NULL;
		}
	}

	// Tell the GUI, this must be last
	::wxGetApp().showHeader(header);

	m_watchdog.start();

	m_decodeData.clear();
	m_dongle->setMode(DVDMODE_DECODE);

	// Start transmitting
	if (m_pttInvert)
		m_controller->setTransmit(false);
	else
		m_controller->setTransmit(true);
}

void CDExtraClientThread::processFrame(const unsigned char* buffer, bool sync, bool end)
{
	m_watchdog.reset();

	if (m_recorder != NULL)
		m_recorder->writeFrame(buffer, DV_FRAME_LENGTH_BYTES);

	// Only process the slow data if this isn't a data sync frame
	if (!sync)
		processSlowData(buffer + VOICE_FRAME_LENGTH_BYTES);
	else
		m_slowDataDecoder.sync();

	m_decodeData.addData(buffer, VOICE_FRAME_LENGTH_BYTES);

	if (end) {
		m_dongle->setMode(DVDMODE_IDLE);
		resetReceiver();
	}
}

void CDExtraClientThread::processSlowData(const unsigned char* buffer)
{
	m_slowDataDecoder.addData(buffer);

	CMessageData* message = m_slowDataDecoder.getMessageData();
	if (message != NULL)
		::wxGetApp().showMessage(message);
}

void CDExtraClientThread::callback(const wxFloat32* input, wxFloat32* output, unsigned int nSamples, int id)
{
	::memset(output, 0x00, nSamples * sizeof(wxFloat32));

	if (m_stopped)
		return;

	if (m_transmit != CLIENT_RECEIVE)
		m_encodeAudio.addData(input, nSamples);

	m_decodeAudio.getData(output, nSamples);
	if (m_audioDelay != NULL)
		m_audioDelay->delay(output, nSamples);

	m_watchdog.clock();
	m_protocol.clock();
}

void CDExtraClientThread::resetReceiver()
{
	// Tell the GUI
	::wxGetApp().showHeader(NULL);

	m_slowDataDecoder.reset();

	m_watchdog.stop();

	// Switch off the transmitter
	if (m_pttInvert)
		m_controller->setTransmit(true);
	else
		m_controller->setTransmit(false);

	// Close the recording file, if open
	if (m_recorder != NULL) {
		m_recorder->close();
		delete m_recorder;
		m_recorder = NULL;
	}
}
