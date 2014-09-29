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

#include "DVAPNodeRXThread.h"
#include "DStarDefines.h"
#include "DVAPNodeApp.h"
#include "HeaderData.h"
#include "Version.h"

const unsigned int NETWORK_QUEUE_COUNT = 2U;

const unsigned int CYCLE_TIME = 9U;

CDVAPNodeRXThread::CDVAPNodeRXThread() :
m_dvap(NULL),
m_protocolHandler(NULL),
m_rxHeader(NULL),
m_radioSeqNo(0U),
m_pollTimer(1000U, 60U),			// 60s
m_dvapPollTimer(1000U, 2U),			// 2s
m_state(DSRS_LISTENING),
m_squelch(false),
m_signal(0),
m_killed(false),
m_ambe(),
m_ambeFrames(0U),
m_ambeSilence(0U),
m_ambeBits(1U),
m_ambeErrors(0U),
m_lastAMBEBits(0U),
m_lastAMBEErrors(0U)
{
}

CDVAPNodeRXThread::~CDVAPNodeRXThread()
{
	delete m_rxHeader;
}

void CDVAPNodeRXThread::run()
{
	// Wait here until we have the essentials to run
	while (!m_killed && (m_dvap == NULL  || m_protocolHandler == NULL))
		::wxMilliSleep(500UL);		// 1/2 sec

	if (m_killed)
		return;

	m_dvapPollTimer.start();
	m_pollTimer.start();

	wxLogMessage(wxT("Starting the DVAP receiver thread"));

	wxStopWatch stopWatch;

	while (!m_killed) {
		stopWatch.Start();

		if (m_state == DSRS_LISTENING)
			receiveRadioHeader();
		else
			receiveRadioData();

		// Listen all the time on the network for status packets at least
		receiveNetwork();

		m_squelch = m_dvap->getSquelch();
		m_signal  = m_dvap->getSignal();

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

		unsigned long ms = stopWatch.Time();
		if (ms < CYCLE_TIME) {
			::wxMilliSleep(CYCLE_TIME - ms);
			clock(CYCLE_TIME);
		} else {
			clock(ms);
		}
	}

	wxLogMessage(wxT("Stopping the DVAP receiver thread"));

	m_dvap->close();

	m_protocolHandler->close();
	delete m_protocolHandler;
}

void CDVAPNodeRXThread::kill()
{
	m_killed = true;
}

void CDVAPNodeRXThread::setCallsign(const wxString& callsign, const wxString& gateway, DSTAR_MODE mode, ACK_TYPE ack, bool restriction, bool rpt1Validation)
{
}

void CDVAPNodeRXThread::setProtocolHandler(CRepeaterProtocolHandler* handler)
{
	wxASSERT(handler != NULL);

	m_protocolHandler = handler;
}

void CDVAPNodeRXThread::setDVAP(CDVAPController* dvap)
{
	wxASSERT(dvap != NULL);

	m_dvap = dvap;
}

void CDVAPNodeRXThread::setTimes(unsigned int timeout, unsigned int ackTime)
{
}

void CDVAPNodeRXThread::setBeacon(unsigned int time, const wxString& text, bool voice, TEXT_LANG language)
{
}

void CDVAPNodeRXThread::setAnnouncement(bool enabled, unsigned int time, const wxString& recordRPT1, const wxString& recordRPT2, const wxString& deleteRPT1, const wxString& deleteRPT2)
{
}

void CDVAPNodeRXThread::setLogging(bool logging, const wxString& dir)
{
}

void CDVAPNodeRXThread::setBlackList(CCallsignList* list)
{
}

void CDVAPNodeRXThread::setGreyList(CCallsignList* list)
{
}

void CDVAPNodeRXThread::receiveRadioHeader()
{
	CHeaderData* header = m_dvap->readHeader();
	if (header == NULL)
		return;

	wxLogMessage(wxT("Header decoded - My: %s/%s  Your: %s  Rpt1: %s  Rpt2: %s  Flags: %02X %02X %02X"), header->getMyCall1().c_str(), header->getMyCall2().c_str(), header->getYourCall().c_str(), header->getRptCall1().c_str(), header->getRptCall2().c_str(), header->getFlag1(), header->getFlag2(), header->getFlag3());

	bool res = processRadioHeader(header);
	if (res) {
		// A valid header and is a DV packet
		m_radioSeqNo     = 20U;
		m_ambeFrames     = 0U;
		m_ambeSilence    = 0U;
		m_ambeBits       = 1U;
		m_ambeErrors     = 0U;
		m_lastAMBEBits   = 0U;
		m_lastAMBEErrors = 0U;
	} else {
		// This is a DD packet or some other problem
		// wxLogMessage(wxT("Invalid header"));
	}
}

void CDVAPNodeRXThread::receiveRadioData()
{
	unsigned char data[DV_FRAME_MAX_LENGTH_BYTES];
	bool end;

	int length = m_dvap->readData(data, DV_FRAME_MAX_LENGTH_BYTES, end);
	if (length == -1)
		return;

	if (end) {
		// wxLogMessage(wxT("Found end sync"));
		processRadioFrame(data, FRAME_END);
		endOfRadioData();
		return;
	}

	bool res = ::memcmp(data + VOICE_FRAME_LENGTH_BYTES, DATA_SYNC_BYTES, DATA_FRAME_LENGTH_BYTES) == 0;
	if (res) {
		// wxLogMessage(wxT("Found data sync at frame %u"), m_radioSyncCounter);
		m_radioSeqNo = 0U;
		processRadioFrame(data, FRAME_SYNC);
	} else if (m_radioSeqNo == 20U) {
		// wxLogMessage(wxT("Regenerating data sync"));
		m_radioSeqNo = 0U;
		processRadioFrame(data, FRAME_SYNC);
	} else {
		m_radioSeqNo++;
		processRadioFrame(data, FRAME_NORMAL);
	}
}

void CDVAPNodeRXThread::receiveNetwork()
{
	NETWORK_TYPE type;

	do {
		type = m_protocolHandler->read();
	} while (type != NETWORK_NONE);
}

bool CDVAPNodeRXThread::processRadioHeader(CHeaderData* header)
{
	wxASSERT(header != NULL);

	// We don't handle DD data packets
	if (header->isDataPacket()) {
		wxLogMessage(wxT("Received a DD packet, ignoring"));
		delete header;
		return false;
	}

	m_state = DSRS_VALID;

	// Send the valid header to the gateway if we are accepted
	delete m_rxHeader;
	m_rxHeader = header;

	CHeaderData netHeader(*m_rxHeader);
	netHeader.setRptCall1(m_rxHeader->getRptCall2());
	netHeader.setRptCall2(m_rxHeader->getRptCall1());

	m_protocolHandler->writeHeader(netHeader);

	return true;
}

void CDVAPNodeRXThread::processRadioFrame(unsigned char* data, FRAME_TYPE type)
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

void CDVAPNodeRXThread::endOfRadioData()
{
	wxLogMessage(wxT("AMBE for %s  Frames: %.1fs, Silence: %.1f%%, BER: %.1f%%"), m_rxHeader->getMyCall1().c_str(), float(m_ambeFrames) / 50.0F, float(m_ambeSilence * 100U) / float(m_ambeFrames), float(m_ambeErrors * 100U) / float(m_ambeBits));

	m_state = DSRS_LISTENING;

	m_protocolHandler->reset();
}

CDVAPNodeStatusData* CDVAPNodeRXThread::getStatus()
{
	float   bits = float(m_ambeBits - m_lastAMBEBits);
	float errors = float(m_ambeErrors - m_lastAMBEErrors);
	if (bits == 0.0F)
		bits = 1.0F;

	m_lastAMBEBits   = m_ambeBits;
	m_lastAMBEErrors = m_ambeErrors;

	if (m_state == DSRS_LISTENING)
		return new CDVAPNodeStatusData(wxEmptyString, wxEmptyString, wxEmptyString, wxEmptyString,
				wxEmptyString, 0x00, 0x00, 0x00, false, m_squelch, m_signal, m_state, 0U, 0U, 0U, 0U, 0U, 0U, 0.0F,
				wxEmptyString);
	else
		return new CDVAPNodeStatusData(m_rxHeader->getMyCall1(), m_rxHeader->getMyCall2(),
				m_rxHeader->getYourCall(), m_rxHeader->getRptCall1(), m_rxHeader->getRptCall2(), 
				m_rxHeader->getFlag1(), m_rxHeader->getFlag2(), m_rxHeader->getFlag3(), false, m_squelch,
				m_signal, m_state, 0U, 0U, 0U, 0U, 0U, 0U, (errors * 100.0F) / bits, wxEmptyString);
}

void CDVAPNodeRXThread::clock(unsigned int ms)
{
	m_pollTimer.clock(ms);
	m_dvapPollTimer.clock(ms);
}
