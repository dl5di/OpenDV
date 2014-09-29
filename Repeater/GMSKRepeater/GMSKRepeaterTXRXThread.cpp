/*
 *   Copyright (C) 2010-2013 by Jonathan Naylor G4KLX
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

#include "GMSKRepeaterTXRXThread.h"
#include "GMSKRepeaterApp.h"
#include "DStarDefines.h"
#include "Version.h"

const unsigned char DTMF_MASK[] = {0x82U, 0x08U, 0x20U, 0x82U, 0x00U, 0x00U, 0x82U, 0x00U, 0x00U};
const unsigned char DTMF_SIG[]  = {0x82U, 0x08U, 0x20U, 0x82U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U};

const unsigned int CYCLE_TIME = 15U;

const unsigned int NETWORK_QUEUE_COUNT = 2U;

const unsigned int SILENCE_THRESHOLD = 2U;

CGMSKRepeaterTXRXThread::CGMSKRepeaterTXRXThread() :
m_modem(NULL),
m_protocolHandler(NULL),
m_controller(NULL),
m_ambeBuffer(NULL),
m_ambeLength(0U),
m_rptCallsign(),
m_rxHeader(NULL),
m_txHeader(NULL),
m_networkQueue(NULL),
m_writeNum(0U),
m_readNum(0U),
m_radioSeqNo(0U),
m_networkSeqNo(0U),
m_watchdogTimer(1000U, 2U),			// 2s
m_pollTimer(1000U, 60U),			// 60s
m_headerReadTimer(1000U, 0U, 50U),	// 50ms
m_dataWaitTimer(1000U, 0U, 100U),	// 100ms
m_state(DSRS_LISTENING),
m_tx(false),
m_killed(false),
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
m_headerTime(),
m_packetTime(),
m_packetCount(0U),
m_modemBuffer(NULL),
m_modemLength(0U),
m_modemEnd(false),
m_packetSilence(0U)
{
	m_networkQueue = new COutputQueue*[NETWORK_QUEUE_COUNT];
	for (unsigned int i = 0U; i < NETWORK_QUEUE_COUNT; i++)
		m_networkQueue[i] = new COutputQueue((DV_FRAME_LENGTH_BYTES + 2U) * 200U, NETWORK_RUN_FRAME_COUNT);		// 4s worth of data);

	m_modemBuffer = new unsigned char[DV_FRAME_LENGTH_BYTES];
	m_ambeBuffer  = new unsigned char[DV_FRAME_LENGTH_BYTES];
	m_lastData    = new unsigned char[DV_FRAME_MAX_LENGTH_BYTES];

	setRepeaterState(DSRS_LISTENING);
}

CGMSKRepeaterTXRXThread::~CGMSKRepeaterTXRXThread()
{
	for (unsigned int i = 0U; i < NETWORK_QUEUE_COUNT; i++)
		delete m_networkQueue[i];
	delete[] m_networkQueue;
	delete[] m_modemBuffer;
	delete[] m_ambeBuffer;
	delete[] m_lastData;
	delete   m_rxHeader;
	delete   m_txHeader;
}

void CGMSKRepeaterTXRXThread::run()
{
	// Wait here until we have the essentials to run
	while (!m_killed && (m_modem == NULL  || m_controller == NULL || m_protocolHandler == NULL || m_rptCallsign.IsEmpty() || m_rptCallsign.IsSameAs(wxT("        "))))
		::wxMilliSleep(500UL);		// 1/2 sec

	if (m_killed)
		return;

	m_controller->setActive(false);
	m_controller->setRadioTransmit(false);

	m_pollTimer.start();

	wxLogMessage(wxT("Starting the GMSK transmitter and receiver thread"));

	unsigned int count = 0U;

	wxStopWatch stopWatch;

	while (!m_killed) {
		stopWatch.Start();

		switch (m_state) {
			case DSRS_SHUTDOWN:
			case DSRS_LISTENING:
				// Only check for the RF header every 100ms or so
				if (m_headerReadTimer.hasExpired()) {
					bool error = receiveRadioHeader();
					if (error)
						reopenModem();

					m_headerReadTimer.reset();
				}
				break;

			case DSRS_VALID: {
					bool error = receiveRadioData();
					if (error)
						reopenModem();
				}
				break;

			default:
				break;
		}

		if (m_killed)
			break;

		// Listen all the time on the network for status packets at least
		receiveNetwork();

		repeaterStateMachine();

		// Send the network poll if needed and restart the timer
		if (m_pollTimer.hasExpired()) {
#if defined(__WINDOWS__)
			m_protocolHandler->writePoll(wxT("win_gmsk-") + VERSION);
#else
			m_protocolHandler->writePoll(wxT("linux_gmsk-") + VERSION);
#endif
			m_pollTimer.reset();
		}

		// Clock the heartbeat output every one second
		count++;
		if (count == 60U) {
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
			if (m_state != DSRS_SHUTDOWN) {
				m_watchdogTimer.stop();
				m_activeHangTimer.stop();
				for (unsigned int i = 0U; i < NETWORK_QUEUE_COUNT; i++)
					m_networkQueue[i]->reset();
				m_tx = false;
				m_controller->setActive(false);
				m_controller->setRadioTransmit(false);
				m_state = DSRS_SHUTDOWN;
			}
		} else {
			if (m_state == DSRS_SHUTDOWN) {
				m_watchdogTimer.stop();
				m_state = DSRS_LISTENING;
				m_protocolHandler->reset();
			}
		}

		if (m_networkQueue[m_readNum]->dataReady()) {
			bool ret = transmitNetworkData();
			if (!ret)
				reopenModem();
		} else if (m_networkQueue[m_readNum]->headerReady()) {
			bool ret = transmitNetworkHeader();
			if (!ret)
				reopenModem();
		}

		m_controller->setRadioTransmit(m_tx);

		unsigned long ms = stopWatch.Time();

		if (m_state != DSRS_VALID) {
			if (ms < CYCLE_TIME)
				::wxMilliSleep(CYCLE_TIME - ms);

			ms = stopWatch.Time();
		}

		// Catch up with the clock
		clock(ms);
	}

	wxLogMessage(wxT("Stopping the GMSK transmitter and receiver thread"));

	if (m_modem != NULL) {
		m_modem->close();
		delete m_modem;
	}

	m_controller->setActive(false);
	m_controller->setRadioTransmit(false);
	m_controller->close();
	delete m_controller;

	m_protocolHandler->close();
	delete m_protocolHandler;
}

void CGMSKRepeaterTXRXThread::kill()
{
	m_killed = true;
}

void CGMSKRepeaterTXRXThread::setCallsign(const wxString& callsign, const wxString& gateway, DSTAR_MODE mode, ACK_TYPE ack, bool restriction, bool rpt1Validation, bool dtmfBlanking)
{
	// Pad the callsign up to eight characters
	m_rptCallsign = callsign;
	m_rptCallsign.resize(LONG_CALLSIGN_LENGTH, wxT(' '));
}

void CGMSKRepeaterTXRXThread::setProtocolHandler(CRepeaterProtocolHandler* handler)
{
	wxASSERT(handler != NULL);

	m_protocolHandler = handler;
}

void CGMSKRepeaterTXRXThread::setModem(IGMSKModem* modem)
{
	wxASSERT(modem != NULL);

	m_modem = modem;
}

void CGMSKRepeaterTXRXThread::setTimes(unsigned int timeout, unsigned int ackTime)
{
}

void CGMSKRepeaterTXRXThread::setBeacon(unsigned int time, const wxString& text, bool voice, TEXT_LANG language)
{
}

void CGMSKRepeaterTXRXThread::setAnnouncement(bool enabled, unsigned int time, const wxString& recordRPT1, const wxString& recordRPT2, const wxString& deleteRPT1, const wxString& deleteRPT2)
{
}

void CGMSKRepeaterTXRXThread::setController(CExternalController* controller, unsigned int activeHangTime)
{
	wxASSERT(controller != NULL);

	m_controller = controller;
	m_activeHangTimer.setTimeout(activeHangTime);
}

void CGMSKRepeaterTXRXThread::setControl(bool enabled, const wxString& rpt1Callsign, const wxString& rpt2Callsign, const wxString& shutdown, const wxString& startup, const wxString& status1, const wxString& status2, const wxString& status3, const wxString& status4, const wxString& status5, const wxString& command1, const wxString& command1Line, const wxString& command2, const wxString& command2Line, const wxString& command3, const wxString& command3Line, const wxString& command4, const wxString& command4Line, const wxString& output1, const wxString& output2, const wxString& output3, const wxString& output4)
{
}

void CGMSKRepeaterTXRXThread::setOutputs(bool out1, bool out2, bool out3, bool out4)
{
}

void CGMSKRepeaterTXRXThread::setLogging(bool logging, const wxString& dir)
{
}

void CGMSKRepeaterTXRXThread::setBlackList(CCallsignList* list)
{
}

void CGMSKRepeaterTXRXThread::setGreyList(CCallsignList* list)
{
}

bool CGMSKRepeaterTXRXThread::receiveRadioHeader()
{
	bool error;
	CHeaderData* header = m_modem->readHeader(error);
	if (header == NULL)
		return error;

	wxLogMessage(wxT("Header decoded - My: %s/%s  Your: %s  Rpt1: %s  Rpt2: %s  Flags: %02X %02X %02X"), header->getMyCall1().c_str(), header->getMyCall2().c_str(), header->getYourCall().c_str(), header->getRptCall1().c_str(), header->getRptCall2().c_str(), header->getFlag1(), header->getFlag2(), header->getFlag3());

	bool res = processRadioHeader(header);
	if (res) {
		// A valid header and is a DV packet
		m_radioSeqNo     = 20U;
		m_ambeFrames     = 0U;
		m_ambeSilence    = 0U;
		m_ambeBits       = 1U;
		m_ambeErrors     = 0U;
		m_ambeLength     = 0U;
		m_lastAMBEBits   = 0U;
		m_lastAMBEErrors = 0U;
	} else {
		// This is a DD packet or some other problem
		// wxLogMessage(wxT("Invalid header"));
	}

	return false;
}

bool CGMSKRepeaterTXRXThread::receiveRadioData()
{
	int length;

	do {
		unsigned char data[GMSK_MODEM_DATA_LENGTH];
		bool end;

		length = m_modem->readData(data, GMSK_MODEM_DATA_LENGTH, end);
		if (length == -1)
			return true;

		if (end) {
			// wxLogMessage(wxT("Found end sync"));
			processRadioFrame(m_ambeBuffer, FRAME_END);
			endOfRadioData();
			return false;
		}

		for (int i = 0; i < length; i++) {
			m_ambeBuffer[m_ambeLength] = data[i];
			m_ambeLength++;

			if (m_ambeLength >= DV_FRAME_LENGTH_BYTES) {
				m_ambeLength = 0U;

				bool res = ::memcmp(m_ambeBuffer + VOICE_FRAME_LENGTH_BYTES, DATA_SYNC_BYTES, DATA_FRAME_LENGTH_BYTES) == 0;
				if (res) {
					// wxLogMessage(wxT("Found data sync at frame %u"), m_radioSyncCounter);
					m_radioSeqNo = 0U;
					processRadioFrame(m_ambeBuffer, FRAME_SYNC);
				} else if (m_radioSeqNo == 20U) {
					// wxLogMessage(wxT("Regenerating data sync"));
					m_radioSeqNo = 0U;
					processRadioFrame(m_ambeBuffer, FRAME_SYNC);
				} else {
					m_radioSeqNo++;
					processRadioFrame(m_ambeBuffer, FRAME_NORMAL);
				}
			}
		}
	} while (length == int(GMSK_MODEM_DATA_LENGTH));

	return false;
}

void CGMSKRepeaterTXRXThread::receiveNetwork()
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
			unsigned char data[2U * DV_FRAME_MAX_LENGTH_BYTES];
			::memset(data, 0x00U, 2U * DV_FRAME_MAX_LENGTH_BYTES);

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
	if (m_state == DSRS_NETWORK && m_packetTime.Time() > 200L) {
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

void CGMSKRepeaterTXRXThread::transmitNetworkHeader(const CHeaderData& header)
{
	wxLogMessage(wxT("Transmitting to - My: %s/%s  Your: %s  Rpt1: %s  Rpt2: %s  Flags: %02X %02X %02X"), header.getMyCall1().c_str(), header.getMyCall2().c_str(), header.getYourCall().c_str(), header.getRptCall1().c_str(), header.getRptCall2().c_str(), header.getFlag1(), header.getFlag2(), header.getFlag3());

	bool empty = m_networkQueue[m_readNum]->isEmpty();
	if (!empty) {
		bool headerReady = m_networkQueue[m_readNum]->headerReady();
		if (headerReady) {
			// Transmission has never started, so just purge the queue
			m_networkQueue[m_readNum]->reset();

			m_readNum++;
			if (m_readNum >= NETWORK_QUEUE_COUNT)
				m_readNum = 0U;
		} else {
			// Purge the currently transmitting buffer and append an end of stream
			m_networkQueue[m_readNum]->reset();
			m_networkQueue[m_readNum]->addData(END_PATTERN_BYTES, DV_FRAME_LENGTH_BYTES, true);
		}
	}

	m_networkQueue[m_writeNum]->reset();
	m_networkQueue[m_writeNum]->setHeader(new CHeaderData(header));
}

bool CGMSKRepeaterTXRXThread::transmitNetworkHeader()
{
	TRISTATE ptt = m_modem->getPTT();
	if (ptt == STATE_UNKNOWN)
		return false;

	if (ptt == STATE_TRUE)
		return true;

	CHeaderData* header = m_networkQueue[m_readNum]->getHeader();
	if (header == NULL)
		return true;

	m_modem->writeHeader(*header);
	delete header;

	m_tx = true;

	m_modemLength = 0U;
	m_modemEnd    = false;

	m_dataWaitTimer.start();

	return true;
}

bool CGMSKRepeaterTXRXThread::transmitNetworkData()
{
	if (m_dataWaitTimer.isRunning() && !m_dataWaitTimer.hasExpired())
		return true;

	m_dataWaitTimer.stop();

	TRISTATE space = m_modem->hasSpace();
	if (space == STATE_UNKNOWN)
		return false;

	if (space == STATE_FALSE)
		return true;

	if (m_modemLength == 0U)
		m_modemLength = m_networkQueue[m_readNum]->getData(m_modemBuffer, DV_FRAME_LENGTH_BYTES, m_modemEnd);

	// If nothing to do then leave
	if (m_modemLength == 0U)
		return true;

	int length = m_modem->writeData(m_modemBuffer, m_modemLength);
	if (length == -1)
		return false;

	if (int(m_modemLength) == length) {
		if (m_modemEnd) {
			m_modem->setPTT(false);

			m_networkQueue[m_readNum]->reset();

			m_readNum++;
			if (m_readNum >= NETWORK_QUEUE_COUNT)
				m_readNum = 0U;

			m_tx = false;

			return true;
		} else {
			m_modemLength = 0U;

			return true;
		}
	} else {
		m_modemLength -= length;

		for (unsigned int i = 0U; i < m_modemLength; i++)
			m_modemBuffer[i + 0U] = m_modemBuffer[i + length];

		return true;
	}
}

void CGMSKRepeaterTXRXThread::repeaterStateMachine()
{
	if (m_watchdogTimer.isRunning() && m_watchdogTimer.hasExpired()) {
		wxLogMessage(wxT("Network watchdog has expired"));
		// Send end of transmission data to the radio
		m_networkQueue[m_writeNum]->addData(END_PATTERN_BYTES, DV_FRAME_LENGTH_BYTES, true);
		endOfNetworkData();
		m_tx = false;
	}
}

bool CGMSKRepeaterTXRXThread::setRepeaterState(DSTAR_RPT_STATE state)
{
	// Can't change state when shutdown
	if (m_disable)
		return false;

	// The "to" state
	switch (state) {
		case DSRS_SHUTDOWN:
			m_watchdogTimer.stop();
			m_activeHangTimer.stop();
			m_controller->setActive(false);
			m_controller->setRadioTransmit(false);
			m_state = DSRS_SHUTDOWN;
			break;

		case DSRS_LISTENING:
			m_state = DSRS_LISTENING;
			break;

		case DSRS_VALID:
			if (m_state != DSRS_LISTENING)
				return false;

			m_state = DSRS_VALID;
			break;

		default:
			break;
	}

	return true;
}

bool CGMSKRepeaterTXRXThread::processRadioHeader(CHeaderData* header)
{
	wxASSERT(header != NULL);

	// We don't handle DD data packets
	if (header->isDataPacket()) {
		wxLogMessage(wxT("Received a DD packet, ignoring"));
		delete header;
		return false;
	}

	setRepeaterState(DSRS_VALID);

	if (m_state == DSRS_VALID) {
		// Send the valid header to the gateway if we are accepted
		delete m_rxHeader;
		m_rxHeader = header;

		CHeaderData netHeader(*m_rxHeader);
		netHeader.setRptCall1(m_rxHeader->getRptCall2());
		netHeader.setRptCall2(m_rxHeader->getRptCall1());

		m_protocolHandler->writeHeader(netHeader);
	}

	return true;
}

void CGMSKRepeaterTXRXThread::processNetworkHeader(CHeaderData* header)
{
	wxASSERT(header != NULL);

	// If shutdown we ignore incoming headers
	if (m_state == DSRS_SHUTDOWN) {
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

void CGMSKRepeaterTXRXThread::processRadioFrame(unsigned char* data, FRAME_TYPE type)
{
	m_ambeFrames++;

	// If a sync frame, regenerate the sync bytes
	if (type == FRAME_SYNC)
		::memcpy(data + VOICE_FRAME_LENGTH_BYTES, DATA_SYNC_BYTES, DATA_FRAME_LENGTH_BYTES);

	// Only regenerate the AMBE on received radio data
	unsigned int errors = 0U;
	if (type != FRAME_END) {
		errors = m_ambe.regenerate(data);
		m_ambeErrors += errors;
		m_ambeBits   += 48U;		// Only count the bits with FEC added
	}

	if (::memcmp(data, NULL_AMBE_DATA_BYTES, VOICE_FRAME_LENGTH_BYTES) == 0)
		m_ambeSilence++;

	// Don't pass through the frame of an invalid transmission
	if (m_state != DSRS_VALID)
		return;

	if (type == FRAME_END) {
		// Send null data and the end marker over the network, and the statistics
			unsigned char bytes[DV_FRAME_MAX_LENGTH_BYTES];
		::memcpy(bytes, NULL_AMBE_DATA_BYTES, VOICE_FRAME_LENGTH_BYTES);
		::memcpy(bytes + VOICE_FRAME_LENGTH_BYTES, END_PATTERN_BYTES, END_PATTERN_LENGTH_BYTES);
		m_protocolHandler->writeData(bytes, DV_FRAME_MAX_LENGTH_BYTES, 0U, true);
	} else {
		// Send the data to the network
		m_protocolHandler->writeData(data, DV_FRAME_LENGTH_BYTES, errors, false);
	}
}

unsigned int CGMSKRepeaterTXRXThread::processNetworkFrame(unsigned char* data, unsigned int length, unsigned char seqNo)
{
	wxASSERT(data != NULL);
	wxASSERT(length > 0U);

	// If shutdown we ignore incoming data
	if (m_state == DSRS_SHUTDOWN)
		return 1U;

	bool end = (seqNo & 0x40U) == 0x40U;
	if (end) {
		if (length > DV_FRAME_LENGTH_BYTES) {
			m_networkQueue[m_writeNum]->addData(data, DV_FRAME_LENGTH_BYTES, false);
			m_networkQueue[m_writeNum]->addData(data + DV_FRAME_LENGTH_BYTES, DV_FRAME_LENGTH_BYTES, true);
		} else {
			m_networkQueue[m_writeNum]->addData(data, DV_FRAME_LENGTH_BYTES, true);
		}

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

	// If the number is too high, then it probably means an old out-of-order frame, ignore it
	if (count > 18U)
		return 0U;

	unsigned int packetCount = 0U;

	// Insert missing frames
	while (seqNo != m_networkSeqNo) {
		unsigned char buffer[DV_FRAME_LENGTH_BYTES];
		if (count > SILENCE_THRESHOLD) {
			::memcpy(buffer, NULL_FRAME_DATA_BYTES, DV_FRAME_LENGTH_BYTES);
		} else {
			::memcpy(buffer, m_lastData, DV_FRAME_LENGTH_BYTES);
			m_ambe.regenerate(buffer);
		}

		if (m_networkSeqNo == 0U)
			::memcpy(buffer + VOICE_FRAME_LENGTH_BYTES, DATA_SYNC_BYTES, DATA_FRAME_LENGTH_BYTES);

		m_networkQueue[m_writeNum]->addData(buffer, DV_FRAME_LENGTH_BYTES, false);

		packetCount++;
		m_networkSeqNo++;
		m_packetSilence++;
		if (m_networkSeqNo >= 21U)
			m_networkSeqNo = 0U;
	}

	// Regenerate the sync bytes
	if (m_networkSeqNo == 0U)
		::memcpy(data + VOICE_FRAME_LENGTH_BYTES, DATA_SYNC_BYTES, DATA_FRAME_LENGTH_BYTES);

	packetCount++;
	m_networkSeqNo++;
	if (m_networkSeqNo >= 21U)
		m_networkSeqNo = 0U;

	m_ambe.regenerate(data);

	m_networkQueue[m_writeNum]->addData(data, DV_FRAME_LENGTH_BYTES, false);

	return packetCount;
}

void CGMSKRepeaterTXRXThread::endOfRadioData()
{
	wxLogMessage(wxT("AMBE for %s  Frames: %.1fs, Silence: %.1f%%, BER: %.1f%%"), m_rxHeader->getMyCall1().c_str(), float(m_ambeFrames) / 50.0F, float(m_ambeSilence * 100U) / float(m_ambeFrames), float(m_ambeErrors * 100U) / float(m_ambeBits));

	setRepeaterState(DSRS_LISTENING);
}

void CGMSKRepeaterTXRXThread::endOfNetworkData()
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

	m_writeNum++;
	if (m_writeNum >= NETWORK_QUEUE_COUNT)
		m_writeNum = 0U;
}

CGMSKRepeaterStatusData* CGMSKRepeaterTXRXThread::getStatus()
{
	float   bits = float(m_ambeBits - m_lastAMBEBits);
	float errors = float(m_ambeErrors - m_lastAMBEErrors);
	if (bits == 0.0F)
		bits = 1.0F;

	m_lastAMBEBits   = m_ambeBits;
	m_lastAMBEErrors = m_ambeErrors;

	if (m_state == DSRS_SHUTDOWN || m_state == DSRS_LISTENING)
		return new CGMSKRepeaterStatusData(wxEmptyString, wxEmptyString, wxEmptyString, wxEmptyString,
				wxEmptyString, 0x00, 0x00, 0x00, m_tx, m_state, 0U, 0U, 0U, 0U, 0U, 0U, 0.0F,
				wxEmptyString, wxEmptyString, wxEmptyString, wxEmptyString, wxEmptyString, wxEmptyString);
	else
		return new CGMSKRepeaterStatusData(m_rxHeader->getMyCall1(), m_rxHeader->getMyCall2(),
				m_rxHeader->getYourCall(), m_rxHeader->getRptCall1(), m_rxHeader->getRptCall2(), 
				m_rxHeader->getFlag1(), m_rxHeader->getFlag2(), m_rxHeader->getFlag3(), m_tx, m_state,
				0U, 0U, 0U, 0U, 0U, 0U, (errors * 100.0F) / bits, wxEmptyString, wxEmptyString, wxEmptyString,
				wxEmptyString, wxEmptyString, wxEmptyString);
}

void CGMSKRepeaterTXRXThread::clock(unsigned int ms)
{
	m_pollTimer.clock(ms);
	m_watchdogTimer.clock(ms);
	m_activeHangTimer.clock(ms);
	m_headerReadTimer.clock(ms);
	m_dataWaitTimer.clock(ms);
}

void CGMSKRepeaterTXRXThread::shutdown()
{
}

void CGMSKRepeaterTXRXThread::startup()
{
}

void CGMSKRepeaterTXRXThread::command1()
{
}

void CGMSKRepeaterTXRXThread::command2()
{
}

void CGMSKRepeaterTXRXThread::command3()
{
}

void CGMSKRepeaterTXRXThread::command4()
{
}

bool CGMSKRepeaterTXRXThread::reopenModem()
{
	m_modem->close();

	while (!m_killed) {
		bool ret = m_modem->open();
		if (ret) {
			wxLogMessage(wxT("Reopened the GMSK modem"));
			return true;
		}

		// Reset the repeaters state
		setRepeaterState(DSRS_LISTENING);
		m_tx = false;
		for (unsigned int j = 0U; j < NETWORK_QUEUE_COUNT; j++)
			m_networkQueue[j]->reset();

		::wxMilliSleep(1000UL);
	}

	delete m_modem;
	m_modem = NULL;

	return false;
}
