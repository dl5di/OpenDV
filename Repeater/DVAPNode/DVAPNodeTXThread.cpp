/*
 *   Copyright (C) 2011,2012,2013 by Jonathan Naylor G4KLX
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

#include "DVAPNodeTXThread.h"
#include "DStarDefines.h"
#include "DVAPNodeApp.h"
#include "HeaderData.h"
#include "Version.h"

const unsigned char DTMF_MASK[] = {0x82U, 0x08U, 0x20U, 0x82U, 0x00U, 0x00U, 0x82U, 0x00U, 0x00U};
const unsigned char DTMF_SIG[]  = {0x82U, 0x08U, 0x20U, 0x82U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U};

const unsigned int NETWORK_QUEUE_COUNT = 2U;

const unsigned int SILENCE_THRESHOLD = 2U;

const unsigned int CYCLE_TIME = 9U;

CDVAPNodeTXThread::CDVAPNodeTXThread() :
m_dvap(NULL),
m_protocolHandler(NULL),
m_rptCallsign(),
m_txHeader(NULL),
m_networkQueue(NULL),
m_writeNum(0U),
m_readNum(0U),
m_networkSeqNo(0U),
m_watchdogTimer(1000U, 2U),			// 2s
m_pollTimer(1000U, 60U),			// 60s
m_dvapPollTimer(1000U, 2U),			// 2s
m_state(DSRS_LISTENING),
m_tx(false),
m_killed(false),
m_lastData(NULL),
m_ambe(),
m_headerTime(),
m_packetTime(),
m_packetCount(0U),
m_packetSilence(0U)
{
	m_networkQueue = new COutputQueue*[NETWORK_QUEUE_COUNT];
	for (unsigned int i = 0U; i < NETWORK_QUEUE_COUNT; i++)
		m_networkQueue[i] = new COutputQueue((DV_FRAME_LENGTH_BYTES + 2U) * 200U, NETWORK_RUN_FRAME_COUNT);		// 4s worth of data);

	m_lastData = new unsigned char[DV_FRAME_MAX_LENGTH_BYTES];
}

CDVAPNodeTXThread::~CDVAPNodeTXThread()
{
	for (unsigned int i = 0U; i < NETWORK_QUEUE_COUNT; i++)
		delete m_networkQueue[i];
	delete[] m_networkQueue;
	delete[] m_lastData;
	delete   m_txHeader;
}

void CDVAPNodeTXThread::run()
{
	// Wait here until we have the essentials to run
	while (!m_killed && (m_dvap == NULL || m_protocolHandler == NULL || m_rptCallsign.IsEmpty() || m_rptCallsign.IsSameAs(wxT("        "))))
		::wxMilliSleep(500UL);		// 1/2 sec

	if (m_killed)
		return;

	m_dvapPollTimer.start();
	m_pollTimer.start();

	wxLogMessage(wxT("Starting the DVAP transmitter thread"));

	wxStopWatch stopWatch;

	while (!m_killed) {
		stopWatch.Start();

		// Remove any received data from the DVAP
		m_dvap->purgeRX();

		// Listen all the time on the network for status packets at least
		receiveNetwork();

		m_tx = m_dvap->getPTT();

		if (m_state == DSRS_NETWORK) {
			if (m_watchdogTimer.hasExpired()) {
				wxLogMessage(wxT("Network watchdog has expired"));
				// Send end of transmission data to the radio
				m_networkQueue[m_writeNum]->addData(END_PATTERN_BYTES, DV_FRAME_LENGTH_BYTES, true);
				endOfNetworkData();
			}
		}

		// Send the network poll if needed and restart the timer
		if (m_pollTimer.hasExpired()) {
#if defined(__WINDOWS__)
			m_protocolHandler->writePoll(wxT("win_dvap-") + VERSION);
#else
			m_protocolHandler->writePoll(wxT("linux_dvap-") + VERSION);
#endif
			m_pollTimer.reset();
		}

		if (m_dvapPollTimer.hasExpired()) {
			m_dvap->writePoll();
			m_dvapPollTimer.reset();
		}

		if (m_networkQueue[m_readNum]->dataReady())
			transmitNetworkData();
		else if (m_networkQueue[m_readNum]->headerReady())
			transmitNetworkHeader();

		unsigned long ms = stopWatch.Time();
		if (ms < CYCLE_TIME) {
			::wxMilliSleep(CYCLE_TIME - ms);
			clock(CYCLE_TIME);
		} else {
			clock(ms);
		}
	}

	wxLogMessage(wxT("Stopping the DVAP transmitter thread"));

	m_dvap->close();

	m_protocolHandler->close();
	delete m_protocolHandler;
}

void CDVAPNodeTXThread::kill()
{
	m_killed = true;
}

void CDVAPNodeTXThread::setCallsign(const wxString& callsign, const wxString& gateway, DSTAR_MODE mode, ACK_TYPE ack, bool restriction, bool rpt1Validation)
{
	// Pad the callsign up to eight characters
	m_rptCallsign = callsign;
	m_rptCallsign.resize(LONG_CALLSIGN_LENGTH, wxT(' '));
}

void CDVAPNodeTXThread::setProtocolHandler(CRepeaterProtocolHandler* handler)
{
	wxASSERT(handler != NULL);

	m_protocolHandler = handler;
}

void CDVAPNodeTXThread::setDVAP(CDVAPController* dvap)
{
	wxASSERT(dvap != NULL);

	m_dvap = dvap;
}

void CDVAPNodeTXThread::setTimes(unsigned int timeout, unsigned int ackTime)
{
}

void CDVAPNodeTXThread::setBeacon(unsigned int time, const wxString& text, bool voice, TEXT_LANG language)
{
}

void CDVAPNodeTXThread::setAnnouncement(bool enabled, unsigned int time, const wxString& recordRPT1, const wxString& recordRPT2, const wxString& deleteRPT1, const wxString& deleteRPT2)
{
}

void CDVAPNodeTXThread::setLogging(bool logging, const wxString& dir)
{
}

void CDVAPNodeTXThread::setBlackList(CCallsignList* list)
{
}

void CDVAPNodeTXThread::setGreyList(CCallsignList* list)
{
}

void CDVAPNodeTXThread::receiveNetwork()
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
				count -= 2U;

				// wxLogMessage(wxT("Inserting %u silence packets into the network data stream"), count);

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

void CDVAPNodeTXThread::transmitNetworkHeader(CHeaderData* header)
{
	wxLogMessage(wxT("Transmitting to - My: %s/%s  Your: %s  Rpt1: %s  Rpt2: %s  Flags: %02X %02X %02X"), header->getMyCall1().c_str(), header->getMyCall2().c_str(), header->getYourCall().c_str(), header->getRptCall1().c_str(), header->getRptCall2().c_str(), header->getFlag1(), header->getFlag2(), header->getFlag3());

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
			m_dvap->purgeTX();
			m_networkQueue[m_readNum]->reset();
			m_networkQueue[m_readNum]->addData(END_PATTERN_BYTES, DV_FRAME_LENGTH_BYTES, true);
		}
	}

	m_networkQueue[m_writeNum]->reset();
	m_networkQueue[m_writeNum]->setHeader(header);
}

void CDVAPNodeTXThread::transmitNetworkHeader()
{
	// Don't send a header until the TX has stopped
	if (m_tx)
		return;

	CHeaderData* header = m_networkQueue[m_readNum]->getHeader();
	if (header == NULL)
		return;

	m_dvap->writeHeader(*header);
	delete header;

	m_dvapPollTimer.reset();
}

void CDVAPNodeTXThread::transmitNetworkData()
{
	bool space = m_dvap->hasSpace();
	if (!space)
		return;

	unsigned char buffer[DV_FRAME_LENGTH_BYTES];
	bool end;

	unsigned int length = m_networkQueue[m_readNum]->getData(buffer, DV_FRAME_LENGTH_BYTES, end);

	if (length > 0U) {
		m_dvap->writeData(buffer, length, end);
		m_dvapPollTimer.reset();

		if (end) {
			m_networkQueue[m_readNum]->reset();

			m_readNum++;
			if (m_readNum >= NETWORK_QUEUE_COUNT)
				m_readNum = 0U;
		}
	}
}

void CDVAPNodeTXThread::processNetworkHeader(CHeaderData* header)
{
	wxASSERT(header != NULL);

	wxLogMessage(wxT("Network header received - My: %s/%s  Your: %s  Rpt1: %s  Rpt2: %s  Flags: %02X %02X %02X"), header->getMyCall1().c_str(), header->getMyCall2().c_str(), header->getYourCall().c_str(), header->getRptCall1().c_str(), header->getRptCall2().c_str(), header->getFlag1(), header->getFlag2(), header->getFlag3());

	// Is it for us?
	if (!header->getRptCall2().IsSameAs(m_rptCallsign)) {
		wxLogMessage(wxT("Invalid network RPT2 value, ignoring"));
		delete header;
		return;
	}

	m_state = DSRS_NETWORK;
	m_networkSeqNo = 0U;
	m_watchdogTimer.start();

	delete m_txHeader;
	m_txHeader = header;

	transmitNetworkHeader(new CHeaderData(*header));
}

unsigned int CDVAPNodeTXThread::processNetworkFrame(unsigned char* data, unsigned int length, unsigned char seqNo)
{
	wxASSERT(data != NULL);
	wxASSERT(length > 0U);

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

void CDVAPNodeTXThread::endOfNetworkData()
{
	if (m_watchdogTimer.hasExpired()) {
		m_packetCount   -= 77U;
		m_packetSilence -= 77U;
	}

	float loss = 0.0F;
	if (m_packetCount != 0U)
		loss = float(m_packetSilence) / float(m_packetCount);

	wxLogMessage(wxT("Stats for %s  Frames: %.1fs, Loss: %.1f%%, Packets: %u/%u"), m_txHeader->getMyCall1().c_str(), float(m_packetCount) / 50.0F, loss * 100.0F, m_packetSilence, m_packetCount);

	m_state = DSRS_LISTENING;
	m_watchdogTimer.stop();
	m_protocolHandler->reset();

	m_writeNum++;
	if (m_writeNum >= NETWORK_QUEUE_COUNT)
		m_writeNum = 0U;
}

CDVAPNodeStatusData* CDVAPNodeTXThread::getStatus()
{
	float loss = 0.0F;
	if (m_packetCount != 0U)
		loss = float(m_packetSilence) / float(m_packetCount);

	if (m_state == DSRS_LISTENING)
		return new CDVAPNodeStatusData(wxEmptyString, wxEmptyString, wxEmptyString, wxEmptyString,
				wxEmptyString, 0x00, 0x00, 0x00, m_tx, false, 0, m_state, 0U, 0U, 0U, 0U, 0U, 0U, 0.0F, wxEmptyString);
	else
		return new CDVAPNodeStatusData(m_txHeader->getMyCall1(), m_txHeader->getMyCall2(),
				m_txHeader->getYourCall(), m_txHeader->getRptCall1(), m_txHeader->getRptCall2(), 
				m_txHeader->getFlag1(), m_txHeader->getFlag2(), m_txHeader->getFlag3(), m_tx, false, 0,
				m_state, 0U, 0U, 0U, 0U, 0U, 0U, loss * 100.0F, wxEmptyString);
}

void CDVAPNodeTXThread::clock(unsigned int ms)
{
	m_pollTimer.clock(ms);
	m_watchdogTimer.clock(ms);
	m_dvapPollTimer.clock(ms);
}
