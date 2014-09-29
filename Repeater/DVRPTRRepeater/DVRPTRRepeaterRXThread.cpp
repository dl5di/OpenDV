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

#include "DVRPTRRepeaterStatusData.h"
#include "DVRPTRRepeaterRXThread.h"
#include "DVRPTRRepeaterApp.h"
#include "DStarDefines.h"
#include "HeaderData.h"
#include "Version.h"
#include "Utils.h"

const unsigned char DTMF_MASK[] = {0x82U, 0x08U, 0x20U, 0x82U, 0x00U, 0x00U, 0x82U, 0x00U, 0x00U};
const unsigned char DTMF_SIG[]  = {0x82U, 0x08U, 0x20U, 0x82U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U};

const unsigned int MAX_DATA_SYNC_BIT_ERRS  = 2U;

const unsigned int NETWORK_QUEUE_COUNT = 2U;

const unsigned int CYCLE_TIME = 9U;

CDVRPTRRepeaterRXThread::CDVRPTRRepeaterRXThread() :
m_dvrptr(NULL),
m_protocolHandler(NULL),
m_rxHeader(NULL),
m_radioSeqNo(0U),
m_pollTimer(1000U, 60U),			// 60s
m_rptState(DSRS_LISTENING),
m_rxState(DSRXS_LISTENING),
m_slowDataDecoder(),
m_killed(false),
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

CDVRPTRRepeaterRXThread::~CDVRPTRRepeaterRXThread()
{
	delete  m_rxHeader;
}

void CDVRPTRRepeaterRXThread::run()
{
	// Wait here until we have the essentials to run
	while (!m_killed && (m_dvrptr == NULL  || m_protocolHandler == NULL))
		::wxMilliSleep(500UL);		// 1/2 sec

	if (m_killed)
		return;

	m_pollTimer.start();

	wxLogMessage(wxT("Starting the DV-RPTR receiver thread"));

	wxStopWatch stopWatch;

	while (!m_killed) {
		stopWatch.Start();

		receiveRadio();

		receiveNetwork();

		// Send the network poll if needed and restart the timer
		if (m_pollTimer.hasExpired()) {
#if defined(__WINDOWS__)
			m_protocolHandler->writePoll(wxT("win_dvrptr-") + VERSION);
#else
			m_protocolHandler->writePoll(wxT("linux_dvrptr-") + VERSION);
#endif
			m_pollTimer.reset();
		}

		unsigned long ms = stopWatch.Time();
		if (ms < CYCLE_TIME) {
			::wxMilliSleep(CYCLE_TIME - ms);
			clock(CYCLE_TIME);
		} else {
			clock(ms);
		}
	}

	wxLogMessage(wxT("Stopping the DV-RPTR receiver thread"));

	m_dvrptr->close();

	m_protocolHandler->close();
	delete m_protocolHandler;
}

void CDVRPTRRepeaterRXThread::kill()
{
	m_killed = true;
}

void CDVRPTRRepeaterRXThread::setCallsign(const wxString& callsign, const wxString& gateway, DSTAR_MODE mode, ACK_TYPE ack, bool restriction, bool rpt1Validation, bool dtmfBlanking)
{
}

void CDVRPTRRepeaterRXThread::setProtocolHandler(CRepeaterProtocolHandler* handler)
{
	wxASSERT(handler != NULL);

	m_protocolHandler = handler;
}

void CDVRPTRRepeaterRXThread::setModem(IDVRPTRController* controller)
{
	wxASSERT(controller != NULL);

	m_dvrptr = controller;
}

void CDVRPTRRepeaterRXThread::setTimes(unsigned int timeout, unsigned int ackTime)
{
}

void CDVRPTRRepeaterRXThread::setBeacon(unsigned int time, const wxString& text, bool voice, TEXT_LANG language)
{
}

void CDVRPTRRepeaterRXThread::setAnnouncement(bool enabled, unsigned int time, const wxString& recordRPT1, const wxString& recordRPT2, const wxString& deleteRPT1, const wxString& deleteRPT2)
{
}

void CDVRPTRRepeaterRXThread::setController(CExternalController* controller, unsigned int activeHangTime)
{
}

void CDVRPTRRepeaterRXThread::setControl(bool enabled, const wxString& rpt1Callsign, const wxString& rpt2Callsign, const wxString& shutdown, const wxString& startup, const wxString& status1, const wxString& status2, const wxString& status3, const wxString& status4, const wxString& status5, const wxString& command1, const wxString& command1Line, const wxString& command2, const wxString& command2Line, const wxString& command3, const wxString& command3Line, const wxString& command4, const wxString& command4Line, const wxString& output1, const wxString& output2, const wxString& output3, const wxString& output4)
{
}

void CDVRPTRRepeaterRXThread::setOutputs(bool out1, bool out2, bool out3, bool out4)
{
}

void CDVRPTRRepeaterRXThread::setLogging(bool logging, const wxString& dir)
{
}

void CDVRPTRRepeaterRXThread::setBlackList(CCallsignList* list)
{
}

void CDVRPTRRepeaterRXThread::setGreyList(CCallsignList* list)
{
}

void CDVRPTRRepeaterRXThread::receiveRadio()
{
	for (;;) {
		unsigned char data[50U];
		unsigned int length;
		DATA_QUEUE_TYPE type = m_dvrptr->readQueue(data, length);

		switch (type) {
			case DQT_NONE:
				return;
			case DQT_HEADER:
				// CUtils::dump(wxT("DQT_HEADER"), data, length);
				break;
			case DQT_DATA:
				// CUtils::dump(wxT("DQT_DATA"), data, length);
				break;
			case DQT_EOT:
				// wxLogMessage(wxT("DQT_EOT"));
				break;
			case DQT_LOST:
				// wxLogMessage(wxT("DQT_LOST"));
				break;
			default:
				wxLogMessage(wxT("type=%d"), int(type));
				CUtils::dump(wxT("DQT_???"), data, length);
				break;
		}

		switch (m_rxState) {
			case DSRXS_LISTENING:
				if (type == DQT_HEADER) {
					receiveHeader(data, length);
				} else if (type == DQT_DATA) {
					setRadioState(DSRXS_PROCESS_SLOW_DATA);
				}
				break;

			case DSRXS_PROCESS_SLOW_DATA:
				if (type == DQT_DATA) {
					receiveSlowData(data, length);
				} else if (type == DQT_EOT) {
					setRadioState(DSRXS_LISTENING);
				} else if (type == DQT_LOST) {
					setRadioState(DSRXS_LISTENING);
				}
				break;

			case DSRXS_PROCESS_DATA:
				if (type == DQT_DATA) {
					receiveRadioData(data, length);
				} else if (type == DQT_EOT) {
					processRadioFrame(data, FRAME_END);
					setRadioState(DSRXS_LISTENING);
					endOfRadioData();
				} else if (type == DQT_LOST) {
					::memcpy(data, NULL_FRAME_DATA_BYTES, DV_FRAME_LENGTH_BYTES);
					processRadioFrame(data, FRAME_END);
					setRadioState(DSRXS_LISTENING);
					endOfRadioData();
				}
				break;
		}
	}
}

void CDVRPTRRepeaterRXThread::receiveHeader(unsigned char* data, unsigned int length)
{
	CHeaderData* header = new CHeaderData(data, length, false);

	wxLogMessage(wxT("Radio header decoded - My: %s/%s  Your: %s  Rpt1: %s  Rpt2: %s  Flags: %02X %02X %02X"), header->getMyCall1().c_str(), header->getMyCall2().c_str(), header->getYourCall().c_str(), header->getRptCall1().c_str(), header->getRptCall2().c_str(), header->getFlag1(), header->getFlag2(), header->getFlag3());

	bool res = processRadioHeader(header);
	if (res) {
		// A valid header and is a DV packet
		m_radioSeqNo = 20U;
		setRadioState(DSRXS_PROCESS_DATA);
	} else {
		// This is a DD packet or some other problem
		// wxLogMessage(wxT("Invalid header"));
	}
}

void CDVRPTRRepeaterRXThread::receiveSlowData(unsigned char* data, unsigned int length)
{
	unsigned int errs;
	errs  = countBits(data[VOICE_FRAME_LENGTH_BYTES + 0U] ^ DATA_SYNC_BYTES[0U]);
	errs += countBits(data[VOICE_FRAME_LENGTH_BYTES + 1U] ^ DATA_SYNC_BYTES[1U]);
	errs += countBits(data[VOICE_FRAME_LENGTH_BYTES + 2U] ^ DATA_SYNC_BYTES[2U]);

	// The data sync has been seen, a fuzzy match is used, two bit errors or less
	if (errs <= MAX_DATA_SYNC_BIT_ERRS) {
		// wxLogMessage(wxT("Found data sync at frame %u, errs: %u"), m_radioSeqNo, errs);
		m_radioSeqNo     = 0U;
		m_slowDataDecoder.sync();
	} else if (m_radioSeqNo == 20U) {
		// wxLogMessage(wxT("Assuming data sync"));
		m_radioSeqNo = 0U;
		m_slowDataDecoder.sync();
	} else {
		m_radioSeqNo++;
		m_slowDataDecoder.addData(data + VOICE_FRAME_LENGTH_BYTES);

		CHeaderData* header = m_slowDataDecoder.getHeaderData();
		if (header == NULL)
			return;

		wxLogMessage(wxT("Radio header from slow data - My: %s/%s  Your: %s  Rpt1: %s  Rpt2: %s  Flags: %02X %02X %02X  BER: 0%%"), header->getMyCall1().c_str(), header->getMyCall2().c_str(), header->getYourCall().c_str(), header->getRptCall1().c_str(), header->getRptCall2().c_str(), header->getFlag1(), header->getFlag2(), header->getFlag3());

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
}

void CDVRPTRRepeaterRXThread::receiveRadioData(unsigned char* data, unsigned int length)
{
	unsigned int errs;
	errs  = countBits(data[VOICE_FRAME_LENGTH_BYTES + 0U] ^ DATA_SYNC_BYTES[0U]);
	errs += countBits(data[VOICE_FRAME_LENGTH_BYTES + 1U] ^ DATA_SYNC_BYTES[1U]);
	errs += countBits(data[VOICE_FRAME_LENGTH_BYTES + 2U] ^ DATA_SYNC_BYTES[2U]);

	// The data sync has been seen, a fuzzy match is used, two bit errors or less
	if (errs <= MAX_DATA_SYNC_BIT_ERRS) {
		// wxLogMessage(wxT("Found data sync at frame %u, errs: %u"), m_radioSeqNo, errs);
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

void CDVRPTRRepeaterRXThread::receiveNetwork()
{
	NETWORK_TYPE type;

	do {
		type = m_protocolHandler->read();
	} while (type != NETWORK_NONE);
}

void CDVRPTRRepeaterRXThread::setRadioState(DSTAR_RX_STATE state)
{
	// This is the too state
	switch (state) {
		case DSRXS_LISTENING:
			m_rxState = DSRXS_LISTENING;
			break;

		case DSRXS_PROCESS_DATA:
			m_ambeFrames     = 0U;
			m_ambeSilence    = 0U;
			m_ambeBits       = 1U;
			m_ambeErrors     = 0U;
			m_lastAMBEBits   = 0U;
			m_lastAMBEErrors = 0U;
			m_rxState        = DSRXS_PROCESS_DATA;
			break;

		case DSRXS_PROCESS_SLOW_DATA:
			m_radioSeqNo = 0U;
			m_slowDataDecoder.reset();
			m_rxState = DSRXS_PROCESS_SLOW_DATA;
			break;
	}
}

bool CDVRPTRRepeaterRXThread::processRadioHeader(CHeaderData* header)
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
	delete m_rxHeader;
	m_rxHeader = header;

	CHeaderData netHeader(*m_rxHeader);
	netHeader.setRptCall1(m_rxHeader->getRptCall2());
	netHeader.setRptCall2(m_rxHeader->getRptCall1());

	m_protocolHandler->writeHeader(netHeader);

	return true;
}

void CDVRPTRRepeaterRXThread::processRadioFrame(unsigned char* data, FRAME_TYPE type)
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

void CDVRPTRRepeaterRXThread::endOfRadioData()
{
	wxLogMessage(wxT("AMBE for %s  Frames: %.1fs, Silence: %.1f%%, BER: %.1f%%"), m_rxHeader->getMyCall1().c_str(), float(m_ambeFrames) / 50.0F, float(m_ambeSilence * 100U) / float(m_ambeFrames), float(m_ambeErrors * 100U) / float(m_ambeBits));

	m_rptState = DSRS_LISTENING;

	m_protocolHandler->reset();
}

CDVRPTRRepeaterStatusData* CDVRPTRRepeaterRXThread::getStatus()
{
	float   bits = float(m_ambeBits - m_lastAMBEBits);
	float errors = float(m_ambeErrors - m_lastAMBEErrors);
	if (bits == 0.0F)
		bits = 1.0F;

	m_lastAMBEBits   = m_ambeBits;
	m_lastAMBEErrors = m_ambeErrors;

	if (m_rptState == DSRS_SHUTDOWN || m_rptState == DSRS_LISTENING)
		return new CDVRPTRRepeaterStatusData(wxEmptyString, wxEmptyString, wxEmptyString, wxEmptyString,
				wxEmptyString, 0x00, 0x00, 0x00, false, m_rxState, m_rptState, 0U, 0U, 0U, 0U, 0U, 0U, 0.0F,
				wxEmptyString, wxEmptyString, wxEmptyString, wxEmptyString, wxEmptyString, wxEmptyString);
	else
		return new CDVRPTRRepeaterStatusData(m_rxHeader->getMyCall1(), m_rxHeader->getMyCall2(),
				m_rxHeader->getYourCall(), m_rxHeader->getRptCall1(), m_rxHeader->getRptCall2(), 
				m_rxHeader->getFlag1(), m_rxHeader->getFlag2(), m_rxHeader->getFlag3(), false, m_rxState,
				m_rptState, 0U, 0U, 0U, 0U, 0U, 0U, (errors * 100.0F) / bits, wxEmptyString, wxEmptyString,
				wxEmptyString, wxEmptyString, wxEmptyString, wxEmptyString);
}

void CDVRPTRRepeaterRXThread::clock(unsigned int ms)
{
	m_pollTimer.clock(ms);
}

void CDVRPTRRepeaterRXThread::shutdown()
{
}

void CDVRPTRRepeaterRXThread::startup()
{
}

void CDVRPTRRepeaterRXThread::command1()
{
}

void CDVRPTRRepeaterRXThread::command2()
{
}

void CDVRPTRRepeaterRXThread::command3()
{
}

void CDVRPTRRepeaterRXThread::command4()
{
}

unsigned int CDVRPTRRepeaterRXThread::countBits(unsigned char byte)
{
	unsigned int bits = 0U;

	if ((byte & 0x01U) == 0x01U)
		bits++;
	if ((byte & 0x02U) == 0x02U)
		bits++;
	if ((byte & 0x04U) == 0x04U)
		bits++;
	if ((byte & 0x08U) == 0x08U)
		bits++;
	if ((byte & 0x10U) == 0x10U)
		bits++;
	if ((byte & 0x20U) == 0x20U)
		bits++;
	if ((byte & 0x40U) == 0x40U)
		bits++;
	if ((byte & 0x80U) == 0x80U)
		bits++;

	return bits;
}
