/*
 *   Copyright (C) 2010-2014 by Jonathan Naylor G4KLX
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

#include "DummyRepeaterThread.h"
#include "DummyRepeaterApp.h"
#include "DStarDefines.h"
#include "MessageData.h"
#include "Version.h"


CDummyRepeaterThread::CDummyRepeaterThread() :
wxThread(wxTHREAD_JOINABLE),
m_soundcard(NULL),
m_dongle(NULL),
m_controller(NULL),
m_slowDataEncoder(),
m_slowDataDecoder(),
m_protocol(NULL),
m_stopped(true),
m_decodeAudio(DSTAR_RADIO_BLOCK_SIZE * 30U),
m_encodeData(VOICE_FRAME_LENGTH_BYTES * 30U),
m_transmit(CLIENT_RECEIVE),
m_callsign1(),
m_callsign2(),
m_your(),
m_rpt1(),
m_rpt2(),
m_message(),
m_frameCount(0U),
m_networkSeqNo(0U),
m_killed(false),
m_started(false),
m_watchdog(DSTAR_TICKS_PER_SEC, 2U),
m_poll(DSTAR_TICKS_PER_SEC, 60U),
m_clockCount(0U),
m_busy(false),
m_localTX(false),
m_externalTX(false)
{
}

CDummyRepeaterThread::~CDummyRepeaterThread()
{
}

void* CDummyRepeaterThread::Entry()
{
	// Wait here until we have the essentials to run
	while (!m_killed && (m_dongle == NULL || m_controller == NULL || m_soundcard == NULL || m_protocol == NULL || m_callsign1.IsEmpty()))
		Sleep(500UL);		// 1/2 sec

	if (m_killed)
		return NULL;

	m_stopped = false;

	m_poll.start();

	m_dongle->Create();
	m_dongle->SetPriority(100U);
	m_dongle->Run();

	m_controller->setRadioTransmit(false);

	while (!m_killed) {
		if (m_transmit == CLIENT_RECEIVE)
			receive();
		else
			transmit();
	}

	m_dongle->kill();

	m_controller->setRadioTransmit(false);
	m_controller->close();

	m_protocol->close();
	delete m_protocol;

	m_soundcard->close();
	delete m_soundcard;

	return NULL;
}

void CDummyRepeaterThread::decodeCallback(const wxFloat32* audio, unsigned int length)
{
	m_decodeAudio.addData(audio, length);
}

void CDummyRepeaterThread::encodeCallback(const unsigned char* ambe, unsigned int length)
{
	m_encodeData.addData(ambe, length);
}

void CDummyRepeaterThread::kill()
{
	m_killed = true;
}

void CDummyRepeaterThread::setCallsign(const wxString& callsign1, const wxString& callsign2)
{
	m_callsign1 = callsign1;
	m_callsign2 = callsign2;
}

void CDummyRepeaterThread::setSoundCard(CSoundCardReaderWriter* soundcard)
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

void CDummyRepeaterThread::setDongle(CDongleThread* dongle)
{
	wxASSERT(dongle != NULL);

	if (!m_stopped) {
		dongle->kill();
		return;
	}

	if (m_dongle != NULL)
		m_dongle->kill();

	m_dongle = dongle;
}

void CDummyRepeaterThread::setController(CExternalController* controller)
{
	wxASSERT(controller != NULL);

	if (!m_stopped) {
		controller->close();
		return;
	}

	if (m_controller != NULL)
		m_controller->close();

	m_controller = controller;
}

void CDummyRepeaterThread::setProtocol(CRepeaterProtocolHandler* protocol)
{
	wxASSERT(protocol != NULL);

	if (!m_stopped) {
		protocol->close();
		delete protocol;
		return;
	}

	if (m_protocol != NULL) {
		m_protocol->close();
		delete m_protocol;
	}

	m_protocol = protocol;
}

void CDummyRepeaterThread::setMessage(const wxString& text)
{
	m_message = text;
}

void CDummyRepeaterThread::setBleep(bool on)
{
	if (m_dongle != NULL)
		m_dongle->setBleep(on);
}

void CDummyRepeaterThread::setYour(const wxString& your)
{
	m_your = your;
}

void CDummyRepeaterThread::setRpt1(const wxString& rpt1)
{
	// An empty RPT1 callsign also implies an empty RPT2
	if (rpt1.IsSameAs(UNUSED)) {
		m_rpt1 = wxT("        ");
		m_rpt2 = wxT("        ");
	} else {
		m_rpt1 = rpt1;
	}
}

void CDummyRepeaterThread::setRpt2(const wxString& rpt2)
{
	// An empty RPT2 callsign
	if (rpt2.IsSameAs(UNUSED))
		m_rpt2 = wxT("        ");
	else
		m_rpt2 = rpt2;
}

bool CDummyRepeaterThread::setTransmit(bool transmit)
{
	if (m_stopped)
		return false;

	m_localTX = transmit;

	if (m_localTX && m_transmit != CLIENT_TRANSMIT) {
		m_transmit = CLIENT_TRANSMIT;
	} else if (!m_localTX && m_transmit == CLIENT_TRANSMIT) {
		if (!m_externalTX) {
			resetReceiver();
			m_transmit = CLIENT_WANT_RECEIVE;
		}
	}

	return true;
}

void CDummyRepeaterThread::checkController()
{
	m_externalTX = m_controller->getRadioSquelch1();

	if (m_externalTX && m_transmit != CLIENT_TRANSMIT) {
		m_transmit = CLIENT_TRANSMIT;
	} else if (!m_externalTX && m_transmit == CLIENT_TRANSMIT) {
		if (!m_localTX) {
			resetReceiver();
			m_transmit = CLIENT_WANT_RECEIVE;
		}
	}

	m_controller->setRadioTransmit(m_busy);
}

void CDummyRepeaterThread::receive()
{
	m_clockCount = 0U;
	m_busy = false;

	unsigned int hangCount = 0U;

	// While receiving and not exitting
	while (m_transmit == CLIENT_RECEIVE && !m_killed) {
		// Get the audio from the RX
		NETWORK_TYPE type;

		for (;;) {
			type = m_protocol->read();

			if (type == NETWORK_NONE) {
				break;
			} else if (type == NETWORK_HEADER) {
				CHeaderData* header = m_protocol->readHeader();
				if (header != NULL) {
					processHeader(header);
					m_watchdog.start();
					m_clockCount = 0U;
					hangCount = 0U;
					m_busy = true;
				}
				break;
			} else if (type == NETWORK_DATA) {
				unsigned char buffer[DV_FRAME_LENGTH_BYTES];
				unsigned char seqNo;
				unsigned int length = m_protocol->readData(buffer, DV_FRAME_LENGTH_BYTES, seqNo);
				if (length != 0U) {
					bool end = processFrame(buffer, seqNo);
					if (end)
						hangCount = 30U;
					else
						hangCount = 0U;
					m_watchdog.reset();
					m_clockCount = 0U;
				}
				break;
			} else if (type == NETWORK_TEXT) {
				wxString text, reflector;
				LINK_STATUS status;
				m_protocol->readText(text, status, reflector);
				::wxGetApp().showSlowData(text);
			} else if (type == NETWORK_TEMPTEXT) {
				wxString text;
				m_protocol->readTempText(text);
			} else if (type == NETWORK_STATUS1) {
				wxString text = m_protocol->readStatus1();
				::wxGetApp().showStatus1(text);
			} else if (type == NETWORK_STATUS2) {
				wxString text = m_protocol->readStatus2();
				::wxGetApp().showStatus2(text);
			} else if (type == NETWORK_STATUS3) {
				wxString text = m_protocol->readStatus3();
				::wxGetApp().showStatus3(text);
			} else if (type == NETWORK_STATUS4) {
				wxString text = m_protocol->readStatus4();
				::wxGetApp().showStatus4(text);
			} else if (type == NETWORK_STATUS5) {
				wxString text = m_protocol->readStatus5();
				::wxGetApp().showStatus5(text);
			}
		}

		// Have we missed a data frame?
		if (type == NETWORK_NONE && m_busy) {
			m_clockCount++;
			if (m_clockCount == 8U) {
				// Create a silence frame
				unsigned char buffer[DV_FRAME_LENGTH_BYTES];
				::memcpy(buffer, NULL_FRAME_DATA_BYTES, DV_FRAME_LENGTH_BYTES);
				processFrame(buffer, m_networkSeqNo);

				m_clockCount = 0U;
			}
		}

		if (m_watchdog.isRunning() && m_watchdog.hasExpired()) {
			wxLogMessage(wxT("Network watchdog has expired"));
			m_dongle->setIdle();
			m_protocol->reset();
			resetReceiver();
		}

		if (hangCount > 0U) {
			hangCount--;
			if (hangCount == 0U) {
				m_dongle->setIdle();
				resetReceiver();
			}
		}

		checkController();

		Sleep(DSTAR_FRAME_TIME_MS / 4UL);
	}
}

void CDummyRepeaterThread::transmit()
{
	m_encodeData.clear();
	m_dongle->setEncode();

	// Pause until all the silence data has been processed by the AMBE2020
	for (unsigned int startCount = 30U; startCount > 0U; startCount--) {
		unsigned char frame[DV_FRAME_LENGTH_BYTES];
		unsigned int n = 0U;
		do {
			n += m_encodeData.getData(frame + n, VOICE_FRAME_LENGTH_BYTES - n);

			if (n < VOICE_FRAME_LENGTH_BYTES)
				Sleep(DSTAR_FRAME_TIME_MS / 4UL);
		} while (n < VOICE_FRAME_LENGTH_BYTES);

		serviceNetwork();
		checkController();
	}

	CHeaderData* header = new CHeaderData(m_callsign1, m_callsign2, m_your, m_rpt2, m_rpt1);

	wxLogMessage(wxT("Transmitting to - My: %s/%s  Your: %s  Rpt1: %s  Rpt2: %s"), header->getMyCall1().c_str(), header->getMyCall2().c_str(), header->getYourCall().c_str(), header->getRptCall1().c_str(), header->getRptCall2().c_str());

	m_slowDataEncoder.reset();
	m_slowDataEncoder.setHeaderData(*header);

	serviceNetwork();
	checkController();

	if (!m_message.IsEmpty())
		m_slowDataEncoder.setMessageData(m_message);

	m_protocol->writeHeader(*header);
	delete header;

	serviceNetwork();
	checkController();

	m_frameCount = 20U;

	unsigned int endCount = 30U;

	// While transmitting and not exiting
	for (;;) {
		unsigned char frame[DV_FRAME_LENGTH_BYTES];
		unsigned int n = 0U;
		do {
			n += m_encodeData.getData(frame + n, VOICE_FRAME_LENGTH_BYTES - n);

			if (n < VOICE_FRAME_LENGTH_BYTES)
				Sleep(DSTAR_FRAME_TIME_MS / 4UL);
		} while (n < VOICE_FRAME_LENGTH_BYTES);

		serviceNetwork();
		checkController();

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

		// Send the AMBE and slow data frame
		if (endCount == 0U || m_killed) {
			m_protocol->writeData(frame, DV_FRAME_LENGTH_BYTES, 0U, true);
			break;
		} else {
			m_protocol->writeData(frame, DV_FRAME_LENGTH_BYTES, 0U, false);
		}

		serviceNetwork();
		checkController();
	}

	m_dongle->setIdle();

	resetReceiver();

	m_transmit = CLIENT_RECEIVE;

	serviceNetwork();
	checkController();
}

void CDummyRepeaterThread::processHeader(CHeaderData* header)
{
	wxASSERT(header != NULL);

	wxLogMessage(wxT("Header decoded - My: %s/%s  Your: %s  Rpt1: %s  Rpt2: %s"), header->getMyCall1().c_str(), header->getMyCall2().c_str(), header->getYourCall().c_str(), header->getRptCall1().c_str(), header->getRptCall2().c_str());

	// Tell the GUI, this must be last
	::wxGetApp().showHeader(header);

	m_dongle->setDecode();

	// Put 60ms of silence into the buffer
	m_dongle->writeDecode(NULL_AMBE_DATA_BYTES, VOICE_FRAME_LENGTH_BYTES);
	m_dongle->writeDecode(NULL_AMBE_DATA_BYTES, VOICE_FRAME_LENGTH_BYTES);
	m_dongle->writeDecode(NULL_AMBE_DATA_BYTES, VOICE_FRAME_LENGTH_BYTES);

	m_slowDataDecoder.reset();

	m_networkSeqNo = 0U;
}

bool CDummyRepeaterThread::processFrame(const unsigned char* buffer, unsigned char seqNo)
{
	bool end = (seqNo & 0x40U) == 0x40U;
	if (end)
		return true;

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

	// If the number is too high, then it probably means an old out-of-order frame, ignore it
	if (count > 18U)
		return false;

	// Insert missing frames
	while (seqNo != m_networkSeqNo) {
		if (m_networkSeqNo == 0U) {
			m_slowDataDecoder.sync();
		} else {
			m_slowDataDecoder.addData(NULL_SLOW_DATA_BYTES);
			CMessageData* message = m_slowDataDecoder.getMessageData();
			if (message != NULL)
				::wxGetApp().showMessage(message);
		}

		// Write a silence frame
		m_dongle->writeDecode(NULL_AMBE_DATA_BYTES, VOICE_FRAME_LENGTH_BYTES);

		m_networkSeqNo++;
		if (m_networkSeqNo >= 21U)
			m_networkSeqNo = 0U;
	}

	if (seqNo == 0U) {
		m_slowDataDecoder.sync();
		m_networkSeqNo = 1U;
	} else {
		m_slowDataDecoder.addData(buffer + VOICE_FRAME_LENGTH_BYTES);
		CMessageData* message = m_slowDataDecoder.getMessageData();
		if (message != NULL)
			::wxGetApp().showMessage(message);

		m_networkSeqNo++;
		if (m_networkSeqNo >= 21U)
			m_networkSeqNo = 0U;
	}

	m_dongle->writeDecode(buffer, VOICE_FRAME_LENGTH_BYTES);

	return false;
}

void CDummyRepeaterThread::callback(const wxFloat32* input, wxFloat32* output, unsigned int nSamples, int)
{
	::memset(output, 0x00, nSamples * sizeof(wxFloat32));

	if (m_stopped)
		return;

	if (m_transmit != CLIENT_RECEIVE)
		m_dongle->writeEncode(input, nSamples);

	m_decodeAudio.getData(output, nSamples);

	m_poll.clock();
	m_watchdog.clock();

	// Send the network poll if needed and restart the timer
	if (m_poll.hasExpired()) {
#if defined(__WINDOWS__)
		m_protocol->writePoll(wxT("win_dummy-") + VERSION);
#else
		m_protocol->writePoll(wxT("linux_dummy-") + VERSION);
#endif
		m_poll.reset();
	}
}

void CDummyRepeaterThread::resetReceiver()
{
	// Tell the GUI
	::wxGetApp().showHeader(NULL);

	m_slowDataDecoder.reset();

	m_watchdog.stop();

	m_busy = false;
}

void CDummyRepeaterThread::serviceNetwork()
{
	for (;;) {
		NETWORK_TYPE type = m_protocol->read();

		if (type == NETWORK_NONE) {
			return;
		} else if (type == NETWORK_TEXT) {
			wxString text, reflector;
			LINK_STATUS status;
			m_protocol->readText(text, status, reflector);
			wxGetApp().showSlowData(text);
		} else if (type == NETWORK_STATUS1) {
			wxString text = m_protocol->readStatus1();
			::wxGetApp().showStatus1(text);
		} else if (type == NETWORK_STATUS2) {
			wxString text = m_protocol->readStatus2();
			::wxGetApp().showStatus2(text);
		} else if (type == NETWORK_STATUS3) {
			wxString text = m_protocol->readStatus3();
			::wxGetApp().showStatus3(text);
		} else if (type == NETWORK_STATUS4) {
			wxString text = m_protocol->readStatus4();
			::wxGetApp().showStatus4(text);
		} else if (type == NETWORK_STATUS5) {
			wxString text = m_protocol->readStatus5();
			::wxGetApp().showStatus5(text);
		}
	}
}
