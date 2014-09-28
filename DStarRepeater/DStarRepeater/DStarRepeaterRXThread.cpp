/*
 *   Copyright (C) 2011-2014 by Jonathan Naylor G4KLX
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

#include "DStarRepeaterStatusData.h"
#include "DStarRepeaterRXThread.h"
#include "DStarRepeaterApp.h"
#include "DVAPController.h"
#include "DStarDefines.h"
#include "HeaderData.h"
#include "Version.h"
#include "Utils.h"

const unsigned char DTMF_MASK[] = {0x82U, 0x08U, 0x20U, 0x82U, 0x00U, 0x00U, 0x82U, 0x00U, 0x00U};
const unsigned char DTMF_SIG[]  = {0x82U, 0x08U, 0x20U, 0x82U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U};

const unsigned int MAX_DATA_SYNC_BIT_ERRS  = 2U;

const unsigned int NETWORK_QUEUE_COUNT = 2U;

const unsigned int CYCLE_TIME = 9U;

CDStarRepeaterRXThread::CDStarRepeaterRXThread(const wxString& type) :
m_type(type),
m_modem(NULL),
m_protocolHandler(NULL),
m_rxHeader(NULL),
m_radioSeqNo(0U),
m_registerTimer(1000U),
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

CDStarRepeaterRXThread::~CDStarRepeaterRXThread()
{
	delete  m_rxHeader;
}

void CDStarRepeaterRXThread::run()
{
	// Wait here until we have the essentials to run
	while (!m_killed && (m_modem == NULL  || m_protocolHandler == NULL))
		::wxMilliSleep(500UL);		// 1/2 sec

	if (m_killed)
		return;

	m_registerTimer.start(10U);

	wxString hardware = m_type;
	int n = hardware.Find(wxT(' '));
	if (n != wxNOT_FOUND)
		hardware = m_type.Left(n);

	wxLogMessage(wxT("Starting the D-Star receiver thread"));

	wxStopWatch stopWatch;

	try {
		while (!m_killed) {
			stopWatch.Start();

			receiveModem();

			receiveNetwork();

			// Send the register packet if needed and restart the timer
			if (m_registerTimer.hasExpired()) {
				m_protocolHandler->writeRegister();
				m_registerTimer.start(30U);
			}

			unsigned long ms = stopWatch.Time();
			if (ms < CYCLE_TIME) {
				::wxMilliSleep(CYCLE_TIME - ms);
				clock(CYCLE_TIME);
			} else {
				clock(ms);
			}
		}
	}
	catch (std::exception& e) {
		wxString message(e.what(), wxConvLocal);
		wxLogError(wxT("Exception raised - \"%s\""), message.c_str());
	}
	catch (...) {
		wxLogError(wxT("Unknown exception raised"));
	}

	wxLogMessage(wxT("Stopping the D-Star receiver thread"));

	m_modem->stop();

	m_protocolHandler->close();
	delete m_protocolHandler;
}

void CDStarRepeaterRXThread::kill()
{
	m_killed = true;
}

void CDStarRepeaterRXThread::setCallsign(const wxString&, const wxString&, DSTAR_MODE, ACK_TYPE, bool, bool, bool, bool)
{
}

void CDStarRepeaterRXThread::setProtocolHandler(CRepeaterProtocolHandler* handler, bool local)
{
	wxASSERT(handler != NULL);

	m_protocolHandler = handler;
}

void CDStarRepeaterRXThread::setModem(CModem* modem)
{
	wxASSERT(modem != NULL);

	m_modem = modem;
}

void CDStarRepeaterRXThread::setTimes(unsigned int, unsigned int)
{
}

void CDStarRepeaterRXThread::setBeacon(unsigned int, const wxString&, bool, TEXT_LANG)
{
}

void CDStarRepeaterRXThread::setAnnouncement(bool, unsigned int, const wxString&, const wxString&, const wxString&, const wxString&)
{
}

void CDStarRepeaterRXThread::setController(CExternalController*, unsigned int)
{
}

void CDStarRepeaterRXThread::setControl(bool, const wxString&, const wxString&, const wxString&, const wxString&, const wxString&, const wxString&, const wxString&, const wxString&, const wxString&, const wxString&, const wxString&, const wxString&, const wxString&, const wxString&, const wxString&, const wxString&, const wxString&, const wxString&, const wxString&, const wxString&, const wxString&, const wxString&, const wxString&, const wxString&, const wxString&)
{
}

void CDStarRepeaterRXThread::setOutputs(bool, bool, bool, bool)
{
}

void CDStarRepeaterRXThread::setLogging(bool, const wxString&)
{
}

void CDStarRepeaterRXThread::setBlackList(CCallsignList*)
{
}

void CDStarRepeaterRXThread::setGreyList(CCallsignList*)
{
}

void CDStarRepeaterRXThread::receiveModem()
{
	for (;;) {
		DSMT_TYPE type = m_modem->read();
		if (type == DSMTT_NONE)
			return;

		switch (m_rxState) {
			case DSRXS_LISTENING:
				if (type == DSMTT_HEADER) {
					CHeaderData* header = m_modem->readHeader();
					receiveHeader(header);
				} else if (type == DSMTT_DATA) {
					unsigned char data[20U];
					unsigned int length = m_modem->readData(data, 20U);
					setRadioState(DSRXS_PROCESS_SLOW_DATA);
					receiveSlowData(data, length);
				}
				break;

			case DSRXS_PROCESS_SLOW_DATA:
				if (type == DSMTT_DATA) {
					unsigned char data[20U];
					unsigned int length = m_modem->readData(data, 20U);
					receiveSlowData(data, length);
				} else if (type == DSMTT_EOT || type == DSMTT_LOST) {
					setRadioState(DSRXS_LISTENING);
				}
				break;

			case DSRXS_PROCESS_DATA:
				if (type == DSMTT_DATA) {
					unsigned char data[20U];
					unsigned int length = m_modem->readData(data, 20U);
					receiveRadioData(data, length);
				} else if (type == DSMTT_EOT || type == DSMTT_LOST) {
					unsigned char data[20U];
					::memcpy(data, END_PATTERN_BYTES, DV_FRAME_LENGTH_BYTES);
					processRadioFrame(data, FRAME_END);
					setRadioState(DSRXS_LISTENING);
					endOfRadioData();
				}
				break;
		}
	}
}

void CDStarRepeaterRXThread::receiveHeader(CHeaderData* header)
{
	wxASSERT(header != NULL);

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

void CDStarRepeaterRXThread::receiveSlowData(unsigned char* data, unsigned int)
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

void CDStarRepeaterRXThread::receiveRadioData(unsigned char* data, unsigned int)
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

void CDStarRepeaterRXThread::receiveNetwork()
{
	NETWORK_TYPE type;

	do {
		type = m_protocolHandler->read();
	} while (type != NETWORK_NONE);
}

void CDStarRepeaterRXThread::setRadioState(DSTAR_RX_STATE state)
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

bool CDStarRepeaterRXThread::processRadioHeader(CHeaderData* header)
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

void CDStarRepeaterRXThread::processRadioFrame(unsigned char* data, FRAME_TYPE type)
{
	m_ambeFrames++;

	// If a sync frame, regenerate the sync bytes
	if (type == FRAME_SYNC)
		::memcpy(data + VOICE_FRAME_LENGTH_BYTES, DATA_SYNC_BYTES, DATA_FRAME_LENGTH_BYTES);

	// Only regenerate the AMBE on received radio data
	unsigned int errors = 0U;
	if (type != FRAME_END) {
		errors = m_ambe.count(data);
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

void CDStarRepeaterRXThread::endOfRadioData()
{
	wxLogMessage(wxT("AMBE for %s  Frames: %.1fs, Silence: %.1f%%, BER: %.1f%%"), m_rxHeader->getMyCall1().c_str(), float(m_ambeFrames) / 50.0F, float(m_ambeSilence * 100U) / float(m_ambeFrames), float(m_ambeErrors * 100U) / float(m_ambeBits));

	m_rptState = DSRS_LISTENING;

	m_protocolHandler->reset();
}

CDStarRepeaterStatusData* CDStarRepeaterRXThread::getStatus()
{
	float   bits = float(m_ambeBits - m_lastAMBEBits);
	float errors = float(m_ambeErrors - m_lastAMBEErrors);
	if (bits == 0.0F)
		bits = 1.0F;

	m_lastAMBEBits   = m_ambeBits;
	m_lastAMBEErrors = m_ambeErrors;

	CDStarRepeaterStatusData* status;
	if (m_rptState == DSRS_SHUTDOWN || m_rptState == DSRS_LISTENING)
		status = new CDStarRepeaterStatusData(wxEmptyString, wxEmptyString, wxEmptyString, wxEmptyString,
					wxEmptyString, 0x00, 0x00, 0x00, false, m_rxState, m_rptState, 0U, 0U, 0U, 0U, 0U, 0U, 0.0F,
					wxEmptyString, wxEmptyString, wxEmptyString, wxEmptyString, wxEmptyString, wxEmptyString);
	else
		status = new CDStarRepeaterStatusData(m_rxHeader->getMyCall1(), m_rxHeader->getMyCall2(),
					m_rxHeader->getYourCall(), m_rxHeader->getRptCall1(), m_rxHeader->getRptCall2(), 
					m_rxHeader->getFlag1(), m_rxHeader->getFlag2(), m_rxHeader->getFlag3(), false, m_rxState,
					m_rptState, 0U, 0U, 0U, 0U, 0U, 0U, (errors * 100.0F) / bits, wxEmptyString, wxEmptyString,
					wxEmptyString, wxEmptyString, wxEmptyString, wxEmptyString);

	if (m_type.IsSameAs(wxT("DVAP")) && m_modem != NULL) {
		CDVAPController* dvap = static_cast<CDVAPController*>(m_modem);
		bool squelch = dvap->getSquelch();
		int signal   = dvap->getSignal();
		status->setDVAP(squelch, signal);
	}

	return status;
}

void CDStarRepeaterRXThread::clock(unsigned int ms)
{
	m_registerTimer.clock(ms);
}

void CDStarRepeaterRXThread::shutdown()
{
}

void CDStarRepeaterRXThread::startup()
{
}

void CDStarRepeaterRXThread::command1()
{
}

void CDStarRepeaterRXThread::command2()
{
}

void CDStarRepeaterRXThread::command3()
{
}

void CDStarRepeaterRXThread::command4()
{
}

void CDStarRepeaterRXThread::command5()
{
}

void CDStarRepeaterRXThread::command6()
{
}

unsigned int CDStarRepeaterRXThread::countBits(unsigned char byte)
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
