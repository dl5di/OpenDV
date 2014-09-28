/*
 *   Copyright (C) 2010,2012 by Jonathan Naylor G4KLX
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

#include "DVAPClientThread.h"
#include "DVAPClientApp.h"
#include "DStarDefines.h"
#include "MessageData.h"
#include "BusyData.h"

CDVAPClientThread::CDVAPClientThread() :
wxThread(wxTHREAD_JOINABLE),
m_soundcard(NULL),
m_dongle(NULL),
m_header(NULL),
m_dvap(NULL),
m_slowDataDecoder(),
m_slowDataEncoder(),
m_stopped(true),
m_decodeAudio(DSTAR_AUDIO_BLOCK_SIZE * 30U),
m_decodeData(VOICE_FRAME_LENGTH_BYTES * 30U),
m_encodeAudio(DSTAR_AUDIO_BLOCK_SIZE * 30U),
m_encodeData(VOICE_FRAME_LENGTH_BYTES * 30U),
m_transmit(CS_RECEIVE),
m_rxPollTimer(TICKS_PER_SEC * 2U, 2U),
m_txPollTimer(TICKS_PER_SEC * 4U, 2U),
m_record(false),
m_recorder(NULL),
m_callsign1(),
m_callsign2(),
m_your(),
m_rpt1(),
m_rpt2(),
m_message(),
m_syncCount(0U),
m_state(RXS_LISTENING),
m_killed(false),
m_tx(false)
{
	setState(RXS_LISTENING);

	CDVTOOLFileWriter::setDirectory(::wxGetHomeDir());
}

CDVAPClientThread::~CDVAPClientThread()
{
}

void* CDVAPClientThread::Entry()
{
	// Wait here until we have the essentials to run
	while (!m_killed && (m_dongle == NULL || m_soundcard == NULL || m_dvap == NULL || m_callsign1.IsEmpty() || m_callsign1.IsSameAs(wxT("        "))))
		Sleep(500UL);		// 1/2 sec

	if (m_killed)
		return NULL;

	m_stopped = false;

	m_dongle->Create();
	m_dongle->SetPriority(100U);
	m_dongle->Run();

	while (!m_killed) {
		if (m_transmit == CS_RECEIVE)
			receive();
		else
			transmit();
	}

	m_dongle->kill();
	m_dongle->Wait();

	m_soundcard->close();
	delete m_soundcard;

	m_dvap->close();

	return NULL;
}

void CDVAPClientThread::kill()
{
	m_killed = true;
}

void CDVAPClientThread::setCallsign(const wxString& callsign1, const wxString& callsign2)
{
	m_callsign1 = callsign1;
	m_callsign2 = callsign2;
}

void CDVAPClientThread::setSoundCard(CSoundCardReaderWriter* soundcard)
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

void CDVAPClientThread::setDVDongle(CDVDongleController* dongle)
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

void CDVAPClientThread::setDVAP(CDVAPController* dvap)
{
	wxASSERT(dvap != NULL);

	if (!m_stopped) {
		dvap->close();
		return;
	}

	if (m_dvap != NULL)
		m_dvap->close();

	m_dvap = dvap;
}

void CDVAPClientThread::setMessage(const wxString& text)
{
	m_message = text;
}

void CDVAPClientThread::setRecording(bool record)
{
	m_record = record;

	if (!record && m_recorder != NULL) {
		m_recorder->close();
		delete m_recorder;
		m_recorder = NULL;
	}
}

void CDVAPClientThread::setBleep(bool on)
{
	if (m_dongle != NULL)
		m_dongle->setBleep(on);
}

void CDVAPClientThread::setYour(const wxString& your)
{
	m_your = your;
}

void CDVAPClientThread::setRpt1(const wxString& rpt1)
{
	// An empty RPT1 callsign also implies an empty RPT2
	if (rpt1.IsSameAs(UNUSED)) {
		m_rpt1 = wxT("        ");
		m_rpt2 = wxT("        ");
	} else {
		m_rpt1 = rpt1;
	}
}

void CDVAPClientThread::setRpt2(const wxString& rpt2)
{
	// An empty RPT2 callsign
	if (rpt2.IsSameAs(UNUSED))
		m_rpt2 = wxT("        ");
	else
		m_rpt2 = rpt2;
}

bool CDVAPClientThread::setTransmit(bool transmit)
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

		m_transmit = CS_TRANSMIT;
	} else {
		resetReceiver();

		m_transmit = CS_WANT_RECEIVE;

		m_tx = false;
	}

	return true;
}

void CDVAPClientThread::receive()
{
	m_rxPollTimer.start();

	resetReceiver();

	// While receiving and not exitting
	while (m_transmit == CS_RECEIVE && !m_killed) {
		switch (m_state) {
			case RXS_LISTENING: {
					CHeaderData* header = m_dvap->readHeader();
					if (header != NULL) {
						bool res = processHeader(header);
						if (!res) {
							// Either the checksum failed, or this is a DD packet
							// wxLogInfo(wxT("Radio header invalid"));
							setState(RXS_LISTENING);
						} else {
							// A valid header and is a DV packet
							// wxLogInfo(wxT("Found valid radio header"));
							setState(RXS_PROCESS_DATA);
						}
					}
				}
				break;

			case RXS_PROCESS_DATA: {
					unsigned char data[15U];
					bool end;
					int length = m_dvap->readData(data, 15U, end);
					if (length == -1)
						break;

					if (end) {
						wxLogMessage(wxT("Found end sync"));
						setState(RXS_LISTENING);
						break;
					}

					bool res = ::memcmp(data + VOICE_FRAME_LENGTH_BYTES, DATA_SYNC_BYTES, DATA_FRAME_LENGTH_BYTES) == 0;
					if (res) {
						wxLogMessage(wxT("Found data sync at frame %u"), m_syncCount);
						m_syncCount = 0U;
						processFrame(data, true);
					} else if (m_syncCount == 20U) {
						wxLogMessage(wxT("Regenerating data sync"));
						m_syncCount = 0U;
						processFrame(data, true);
					} else {
						m_syncCount++;
						processFrame(data, false);
					}
				}
				break;

			default:
				break;
		}

		if (m_rxPollTimer.hasExpired()) {
			m_dvap->writePoll();
			m_rxPollTimer.reset();
		}

		Sleep(FRAME_TIME_MS / 2UL);

		m_rxPollTimer.clock();
	}

	m_rxPollTimer.stop();
}

void CDVAPClientThread::transmit()
{
	m_txPollTimer.start();

	m_encodeAudio.clear();

	m_encodeData.clear();
	m_dongle->setMode(DVDMODE_ENCODE);

	wxASSERT(m_header != NULL);
	wxLogMessage(wxT("Transmitting to - My: %s/%s  Your: %s  Rpt1: %s  Rpt2: %s  Flags: %02X %02X %02X"), m_header->getMyCall1().c_str(), m_header->getMyCall2().c_str(), m_header->getYourCall().c_str(), m_header->getRptCall1().c_str(), m_header->getRptCall2().c_str(), m_header->getFlag1(), m_header->getFlag2(), m_header->getFlag3());

	// Pause until all the silence data has been processed by the AMBE2020
	unsigned int startCount = 30U;
	while (startCount > 0U) {
		unsigned char frame[DV_FRAME_LENGTH_BYTES];
		unsigned int n = m_encodeData.getData(frame, VOICE_FRAME_LENGTH_BYTES);
		if (n > 0U)
			startCount--;

		Sleep(FRAME_TIME_MS / 4UL);
	}

	m_dvap->writeHeader(*m_header);

	unsigned int frameCount = 20U;

	unsigned int endCount = 30U;

	// While transmitting and not exiting
	while (endCount > 0U && !m_killed) {
		bool hasSpace = m_dvap->hasSpace();
		if (hasSpace) {
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

				m_dvap->writeData(frame, DV_FRAME_LENGTH_BYTES, false);

				// Make transmit hang so that all the audio is flushed
				if (m_transmit != CS_TRANSMIT)
					endCount--;
			}
		}

		if (m_txPollTimer.hasExpired()) {
			m_dvap->writePoll();
			m_txPollTimer.reset();
		}

		Sleep(FRAME_TIME_MS / 4UL);

		m_txPollTimer.clock();
	}

	// Send the end pattern
	m_dvap->writeData(END_PATTERN_BYTES, END_PATTERN_LENGTH_BYTES, true);

	m_dongle->setMode(DVDMODE_IDLE);

	wxLogMessage(wxT("Finished transmitting"));

	m_transmit = CS_RECEIVE;

	m_txPollTimer.stop();
}

// XXX Some of this should go into EndOfData()
void CDVAPClientThread::resetReceiver()
{
	setState(RXS_LISTENING);

	endOfData();
}

bool CDVAPClientThread::processHeader(CHeaderData* header)
{
	wxLogMessage(wxT("Header decoded - My: %s/%s  Your: %s  Rpt1: %s  Rpt2: %s  Flags: %02X %02X %02X"), header->getMyCall1().c_str(), header->getMyCall2().c_str(), header->getYourCall().c_str(), header->getRptCall1().c_str(), header->getRptCall2().c_str(), header->getFlag1(), header->getFlag2(), header->getFlag3());

	// We don't handle DD data packets
	if (header->isDataPacket()) {
		wxLogMessage(wxT("Received a DD packet, ignoring"));
		delete header;
		return false;
	}

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

	::wxGetApp().showHeader(header);

	return true;
}

void CDVAPClientThread::processFrame(const unsigned char* buffer, bool sync)
{
	if (m_recorder != NULL)
		m_recorder->writeFrame(buffer, DV_FRAME_LENGTH_BYTES);

	// Only process the slow data if this isn't a data sync frame
	if (!sync)
		processSlowData(buffer + VOICE_FRAME_LENGTH_BYTES);
	else
		m_slowDataDecoder.sync();

	m_decodeData.addData(buffer, VOICE_FRAME_LENGTH_BYTES);
}

void CDVAPClientThread::processSlowData(const unsigned char* buffer)
{
	m_slowDataDecoder.addData(buffer);

	CMessageData* message = m_slowDataDecoder.getMessageData();
	if (message != NULL)
		::wxGetApp().showMessage(message);
}

void CDVAPClientThread::callback(const wxFloat32* input, wxFloat32* output, unsigned int nSamples, int id)
{
	::memset(output, 0x00, nSamples * sizeof(wxFloat32));

	if (m_transmit != CS_RECEIVE)
		m_encodeAudio.addData(input, nSamples);

	m_decodeAudio.getData(output, nSamples);
}

void CDVAPClientThread::endOfData()
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
}

void CDVAPClientThread::setState(RX_STATE state)
{
	// This is the from state
	switch (m_state) {
		case RXS_LISTENING:
			m_syncCount = 0U;
			break;

		case RXS_PROCESS_DATA:
			endOfData();
			m_dongle->setMode(DVDMODE_IDLE);
			break;
	}

	// This is the too state
	switch (state) {
		case RXS_LISTENING:
			m_state = RXS_LISTENING;
			break;

		case RXS_PROCESS_DATA:
			::wxGetApp().showBusy(new CBusyData(true));
			m_slowDataDecoder.reset();
			m_decodeData.clear();
			m_decodeAudio.clear();
			m_dongle->setMode(DVDMODE_DECODE);
			m_syncCount = 20U;
			m_state = RXS_PROCESS_DATA;
			break;
	}
}
