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

#include "DVRPTRClientThread.h"
#include "DVRPTRClientApp.h"
#include "DStarDefines.h"
#include "MessageData.h"
#include "BusyData.h"

CDVRPTRClientThread::CDVRPTRClientThread() :
wxThread(wxTHREAD_JOINABLE),
m_soundcard(NULL),
m_dongle(NULL),
m_header(NULL),
m_modem(NULL),
m_slowDataDecoder(),
m_slowDataEncoder(),
m_stopped(true),
m_decodeAudio(DSTAR_AUDIO_BLOCK_SIZE * 30U),
m_decodeData(VOICE_FRAME_LENGTH_BYTES * 30U),
m_encodeAudio(DSTAR_AUDIO_BLOCK_SIZE * 30U),
m_encodeData(VOICE_FRAME_LENGTH_BYTES * 30U),
m_transmit(CS_RECEIVE),
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

CDVRPTRClientThread::~CDVRPTRClientThread()
{
}

void* CDVRPTRClientThread::Entry()
{
	// Wait here until we have the essentials to run
	while (!m_killed && (m_dongle == NULL || m_soundcard == NULL || m_modem == NULL || m_callsign1.IsEmpty() || m_callsign1.IsSameAs(wxT("        "))))
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

	m_modem->close();

	return NULL;
}

void CDVRPTRClientThread::kill()
{
	m_killed = true;
}

void CDVRPTRClientThread::setCallsign(const wxString& callsign1, const wxString& callsign2)
{
	m_callsign1 = callsign1;
	m_callsign2 = callsign2;
}

void CDVRPTRClientThread::setSoundCard(CSoundCardReaderWriter* soundcard)
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

void CDVRPTRClientThread::setDVDongle(CDVDongleController* dongle)
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

void CDVRPTRClientThread::setModem(IDVRPTRController* modem)
{
	wxASSERT(modem != NULL);

	if (!m_stopped) {
		modem->close();
		return;
	}

	if (m_modem != NULL)
		m_modem->close();

	m_modem = modem;
}

void CDVRPTRClientThread::setMessage(const wxString& text)
{
	m_message = text;
}

void CDVRPTRClientThread::setRecording(bool record)
{
	m_record = record;

	if (!record && m_recorder != NULL) {
		m_recorder->close();
		delete m_recorder;
		m_recorder = NULL;
	}
}

void CDVRPTRClientThread::setBleep(bool on)
{
	if (m_dongle != NULL)
		m_dongle->setBleep(on);
}

void CDVRPTRClientThread::setYour(const wxString& your)
{
	m_your = your;
}

void CDVRPTRClientThread::setRpt1(const wxString& rpt1)
{
	// An empty RPT1 callsign also implies an empty RPT2
	if (rpt1.IsSameAs(UNUSED)) {
		m_rpt1 = wxT("        ");
		m_rpt2 = wxT("        ");
	} else {
		m_rpt1 = rpt1;
	}
}

void CDVRPTRClientThread::setRpt2(const wxString& rpt2)
{
	// An empty RPT2 callsign
	if (rpt2.IsSameAs(UNUSED))
		m_rpt2 = wxT("        ");
	else
		m_rpt2 = rpt2;
}

bool CDVRPTRClientThread::setTransmit(bool transmit)
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

void CDVRPTRClientThread::receive()
{
	resetReceiver();

	// While receiving and not exitting
	while (m_transmit == CS_RECEIVE && !m_killed) {
		unsigned char data[50U];
		unsigned int length;
		DATA_QUEUE_TYPE type = m_modem->readQueue(data, length);

		switch (type) {
			case DQT_HEADER: {
					CHeaderData* header = new CHeaderData(data, length, false);
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
				break;

			case DQT_DATA: {
					if (m_state != RXS_PROCESS_DATA)
						setState(RXS_PROCESS_DATA);

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

			case DQT_EOT:
				wxLogMessage(wxT("Found end sync"));
				setState(RXS_LISTENING);
				break;

			case DQT_LOST:
				wxLogMessage(wxT("Lost sync"));
				setState(RXS_LISTENING);
				break;

			default:
				break;
		}

		Sleep(FRAME_TIME_MS / 2UL);
	}
}

void CDVRPTRClientThread::transmit()
{
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

	m_modem->writeHeader(*m_header);

	unsigned int frameCount = 20U;

	unsigned int endCount = 30U;

	// While transmitting and not exiting
	while (endCount > 0U && !m_killed) {
		bool hasSpace = m_modem->hasSpace();
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

				m_modem->writeData(frame, DV_FRAME_LENGTH_BYTES, endCount == 1U);

				// Make transmit hang so that all the audio is flushed
				if (m_transmit != CS_TRANSMIT)
					endCount--;
			}
		}

		Sleep(FRAME_TIME_MS / 4UL);
	}

	m_dongle->setMode(DVDMODE_IDLE);

	wxLogMessage(wxT("Finished transmitting"));

	m_transmit = CS_RECEIVE;
}

// XXX Some of this should go into EndOfData()
void CDVRPTRClientThread::resetReceiver()
{
	setState(RXS_LISTENING);

	endOfData();
}

bool CDVRPTRClientThread::processHeader(CHeaderData* header)
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

void CDVRPTRClientThread::processFrame(const unsigned char* buffer, bool sync)
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

void CDVRPTRClientThread::processSlowData(const unsigned char* buffer)
{
	m_slowDataDecoder.addData(buffer);

	CMessageData* message = m_slowDataDecoder.getMessageData();
	if (message != NULL)
		::wxGetApp().showMessage(message);
}

void CDVRPTRClientThread::callback(const wxFloat32* input, wxFloat32* output, unsigned int nSamples, int id)
{
	::memset(output, 0x00, nSamples * sizeof(wxFloat32));

	if (m_transmit != CS_RECEIVE)
		m_encodeAudio.addData(input, nSamples);

	m_decodeAudio.getData(output, nSamples);
}

void CDVRPTRClientThread::endOfData()
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

void CDVRPTRClientThread::setState(RX_STATE state)
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
