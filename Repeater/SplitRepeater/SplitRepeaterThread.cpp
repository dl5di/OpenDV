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

#include "SplitRepeaterThread.h"
#include "SplitRepeaterApp.h"
#include "DStarDefines.h"
#include "Version.h"

const unsigned char DTMF_MASK[] = {0x82U, 0x08U, 0x20U, 0x82U, 0x00U, 0x00U, 0x82U, 0x00U, 0x00U};
const unsigned char DTMF_SIG[]  = {0x82U, 0x08U, 0x20U, 0x82U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U};

const unsigned int CYCLE_TIME    = 5U;
const unsigned int TICKS_PER_SEC = 200U;

CSplitRepeaterThread::CSplitRepeaterThread() :
m_protocolHandler(NULL),
m_gatewayAddress(),
m_gatewayPort(0U),
m_receiver1Address(),
m_receiver1Port(0U),
m_receiver2Address(),
m_receiver2Port(0U),
m_transmitter1Address(),
m_transmitter1Port(0U),
m_transmitter2Address(),
m_transmitter2Port(0U),
m_frameWaitTime(0U),
m_radioId(0U),
m_networkId(0U),
m_receiver1Id(0U),
m_receiver2Id(0U),
m_receiver1Header(NULL),
m_receiver2Header(NULL),
m_receiver1Data(NULL),
m_receiver2Data(NULL),
m_stopped(true),
m_rptCallsign(),
m_gwyCallsign(),
m_beacon(NULL),
m_beaconId(0x00U),
m_beaconSeq(0U),
m_announcement(NULL),
m_announcementId(0x00U),
m_announcementSeq(0U),
m_recordRPT1(),
m_recordRPT2(),
m_deleteRPT1(),
m_deleteRPT2(),
m_rxHeader(NULL),
m_txHeader(NULL),
m_radioSeqNo(0U),
m_timeoutTimer(1000U, 180U),		// 180s
m_radio1WatchdogTimer(1000U, 2U),	// 2s
m_radio2WatchdogTimer(1000U, 2U),	// 2s
m_networkWatchdogTimer(1000U, 2U),	// 2s
m_pollTimer(1000U, 60U),			// 60s
m_ackTimer(1000U, 0U, 500U),		// 0.5s
m_status1Timer(1000U, 3U),			// 3s
m_status2Timer(1000U, 3U),			// 3s
m_status3Timer(1000U, 3U),			// 3s
m_status4Timer(1000U, 3U),			// 3s
m_status5Timer(1000U, 3U),			// 3s
m_beaconTimer(1000U, 600U),			// 10 mins
m_announcementTimer(1000U, 0U),		// not running
m_state(DSRS_LISTENING),
m_ackEncoder(),
m_linkEncoder(),
m_status1Encoder(),
m_status2Encoder(),
m_status3Encoder(),
m_status4Encoder(),
m_status5Encoder(),
m_killed(false),
m_mode(MODE_DUPLEX),
m_ack(AT_BER),
m_restriction(false),
m_rpt1Validation(true),
m_controlEnabled(false),
m_controlRPT1(),
m_controlRPT2(),
m_controlShutdown(),
m_controlStartup(),
m_controlStatus1(),
m_controlStatus2(),
m_controlStatus3(),
m_controlStatus4(),
m_controlStatus5(),
m_controlCommand1(),
m_controlCommand1Line(),
m_controlCommand2(),
m_controlCommand2Line(),
m_controlCommand3(),
m_controlCommand3Line(),
m_controlCommand4(),
m_controlCommand4Line(),
m_shutdown(false),
m_ambeFrames(0U),
m_ambeSilence(0U),
m_ambeBits(1U),
m_ambeErrors(0U),
m_lastAMBEBits(0U),
m_lastAMBEErrors(0U),
m_ackText(),
m_tempAckText(),
m_linkStatus(LS_NONE),
m_reflector(),
m_status1Text(),
m_status2Text(),
m_status3Text(),
m_status4Text(),
m_status5Text(),
m_regEx(wxT("^[A-Z0-9]{1}[A-Z0-9]{0,1}[0-9]{1,2}[A-Z]{1,4} {0,4}[ A-Z]{1}$")),
m_blackList(NULL),
m_greyList(NULL),
m_blocked(false),
m_busyData(false),
m_packetTime(),
m_totalPackets(0U),
m_receiver1Packets(0U),
m_receiver2Packets(0U),
m_receiver1Choice(0U),
m_receiver2Choice(0U),
m_receiver1Rejected(0U),
m_receiver2Rejected(0U),
m_blanking(true),
m_recording(false),
m_deleting(false)
{
	m_receiver1Address.s_addr    = INADDR_NONE;
	m_receiver2Address.s_addr    = INADDR_NONE;
	m_transmitter1Address.s_addr = INADDR_NONE;
	m_transmitter2Address.s_addr = INADDR_NONE;

	m_receiver1Data = new CSplitRepeaterAMBEData*[21U];
	m_receiver2Data = new CSplitRepeaterAMBEData*[21U];

	::memset(m_receiver1Data, 0x00U, 21U * sizeof(CSplitRepeaterAMBEData*));
	::memset(m_receiver2Data, 0x00U, 21U * sizeof(CSplitRepeaterAMBEData*));

	setRepeaterState(DSRS_LISTENING);
}

CSplitRepeaterThread::~CSplitRepeaterThread()
{
	delete m_rxHeader;
	delete m_txHeader;

	delete m_receiver1Header;
	delete m_receiver2Header;

	clearQueue(1U);
	clearQueue(2U);

	delete[] m_receiver1Data;
	delete[] m_receiver2Data;
}

void CSplitRepeaterThread::run()
{
	// Wait here until we have the essentials to run
	while (!m_killed && (m_protocolHandler == NULL || m_rptCallsign.IsEmpty() || m_rptCallsign.IsSameAs(wxT("        "))))
		::wxMilliSleep(500UL);		// 1/2 sec

	if (m_killed)
		return;

	CSplitRepeaterHeaderData::initialise();

	m_stopped = false;

	m_announcementTimer.start();
	m_beaconTimer.start();
	m_pollTimer.start();

	wxLogMessage(wxT("Starting the Split repeater thread"));

	m_packetTime.Start();
	wxStopWatch timer;

	while (!m_killed) {
		timer.Start();

		receiveNetwork();

		chooseReceiver();

		repeaterStateMachine();

		// Send the network poll if needed and restart the timer
		if (m_pollTimer.hasExpired()) {
#if defined(__WINDOWS__)
			m_protocolHandler->writePoll(wxT("win_split-") + VERSION, m_gatewayAddress, m_gatewayPort);
#else
			m_protocolHandler->writePoll(wxT("linux_split-") + VERSION, m_gatewayAddress, m_gatewayPort);
#endif
			m_pollTimer.reset();
		}

		// Send the beacon and restart the timer
		if (m_beaconTimer.isRunning() && m_beaconTimer.hasExpired()) {
			m_beacon->sendBeacon();
			m_beaconTimer.reset();
		}

		// Send the announcement and restart the timer
		if (m_announcementTimer.isRunning() && m_announcementTimer.hasExpired()) {
			m_announcement->startAnnouncement();
			m_announcementTimer.reset();
		}

		// Send the status 1 message after a few seconds
		if (m_status1Timer.isRunning() && m_status1Timer.hasExpired()) {
			m_status1Timer.stop();
			if (m_state == DSRS_LISTENING)
				transmitUserStatus(0U);
		}

		// Send the status 2 message after a few seconds
		if (m_status2Timer.isRunning() && m_status2Timer.hasExpired()) {
			m_status2Timer.stop();
			if (m_state == DSRS_LISTENING)
				transmitUserStatus(1U);
		}

		// Send the status 3 message after a few seconds
		if (m_status3Timer.isRunning() && m_status3Timer.hasExpired()) {
			m_status3Timer.stop();
			if (m_state == DSRS_LISTENING)
				transmitUserStatus(2U);
		}

		// Send the status 4 message after a few seconds
		if (m_status4Timer.isRunning() && m_status4Timer.hasExpired()) {
			m_status4Timer.stop();
			if (m_state == DSRS_LISTENING)
				transmitUserStatus(3U);
		}

		// Send the status 5 message after a few seconds
		if (m_status5Timer.isRunning() && m_status5Timer.hasExpired()) {
			m_status5Timer.stop();
			if (m_state == DSRS_LISTENING)
				transmitUserStatus(4U);
		}

		// Check the shutdown state, state changes are done here to bypass the state machine which is
		// frozen when m_disable or m_shutdown are asserted
		if (m_shutdown) {
			if (m_state != DSRS_SHUTDOWN) {
				m_timeoutTimer.stop();
				m_radio1WatchdogTimer.stop();
				m_radio2WatchdogTimer.stop();
				m_networkWatchdogTimer.stop();
				m_ackTimer.stop();
				m_beaconTimer.stop();
				m_announcementTimer.stop();
				m_state = DSRS_SHUTDOWN;
			}
		} else {
			if (m_state == DSRS_SHUTDOWN) {
				m_timeoutTimer.stop();
				m_radio1WatchdogTimer.stop();
				m_radio2WatchdogTimer.stop();
				m_networkWatchdogTimer.stop();
				m_ackTimer.stop();
				m_beaconTimer.start();
				m_announcementTimer.start();
				m_state = DSRS_LISTENING;
			}
		}

		::wxMilliSleep(CYCLE_TIME);

		unsigned long ms = timer.Time();

		clock(ms);
	}

	wxLogMessage(wxT("Stopping the Split repeater thread"));

	CSplitRepeaterHeaderData::finalise();

	delete m_beacon;
	delete m_announcement;

	delete m_blackList;
	delete m_greyList;

	m_protocolHandler->close();
	delete m_protocolHandler;
}

void CSplitRepeaterThread::kill()
{
	m_killed = true;
}

void CSplitRepeaterThread::setCallsign(const wxString& callsign, const wxString& gateway, DSTAR_MODE mode, ACK_TYPE ack, bool restriction, bool rpt1Validation, bool dtmfBlanking)
{
	// Pad the callsign up to eight characters
	m_rptCallsign = callsign;
	m_rptCallsign.resize(LONG_CALLSIGN_LENGTH, wxT(' '));

	if (gateway.IsEmpty()) {
		m_gwyCallsign = callsign;
		m_gwyCallsign.resize(LONG_CALLSIGN_LENGTH - 1U, wxT(' '));
		m_gwyCallsign.Append(wxT("G"));
	} else {
		m_gwyCallsign = gateway;
		m_gwyCallsign.resize(LONG_CALLSIGN_LENGTH, wxT(' '));
	}

	m_mode           = mode;
	m_ack            = ack;
	m_restriction    = restriction;
	m_rpt1Validation = rpt1Validation;
	m_blanking       = dtmfBlanking;
}

void CSplitRepeaterThread::setProtocolHandler(CSplitRepeaterProtocolHandler* handler)
{
	wxASSERT(handler != NULL);

	m_protocolHandler = handler;
}

bool CSplitRepeaterThread::setGateway(const wxString& address, unsigned int port)
{
	in_addr addr;
	addr.s_addr = ::inet_addr(address.mb_str());

	if (addr.s_addr == INADDR_NONE)
		return false;

	::memcpy(&m_gatewayAddress, &addr, sizeof(in_addr));
	m_gatewayPort = port;

	return true;
}

bool CSplitRepeaterThread::setReceiver1(const wxString& address, unsigned int port)
{
	in_addr addr;
	addr.s_addr = ::inet_addr(address.mb_str());

	if (addr.s_addr == INADDR_NONE)
		return false;

	::memcpy(&m_receiver1Address, &addr, sizeof(in_addr));
	m_receiver1Port = port;

	return true;
}

bool CSplitRepeaterThread::setReceiver2(const wxString& address, unsigned int port)
{
	in_addr addr;
	addr.s_addr = ::inet_addr(address.mb_str());

	if (addr.s_addr == INADDR_NONE)
		return false;

	::memcpy(&m_receiver2Address, &addr, sizeof(in_addr));
	m_receiver2Port = port;

	return true;
}

bool CSplitRepeaterThread::setTransmitter1(const wxString& address, unsigned int port)
{
	in_addr addr;
	addr.s_addr = ::inet_addr(address.mb_str());

	if (addr.s_addr == INADDR_NONE)
		return false;

	::memcpy(&m_transmitter1Address, &addr, sizeof(in_addr));
	m_transmitter1Port = port;

	return true;
}

bool CSplitRepeaterThread::setTransmitter2(const wxString& address, unsigned int port)
{
	in_addr addr;
	addr.s_addr = ::inet_addr(address.mb_str());

	if (addr.s_addr == INADDR_NONE)
		return false;

	::memcpy(&m_transmitter2Address, &addr, sizeof(in_addr));
	m_transmitter2Port = port;

	return true;
}

void CSplitRepeaterThread::setTimes(unsigned int timeout, unsigned int ackTime, unsigned int frameWaitTime)
{
	m_timeoutTimer.setTimeout(timeout);
	m_ackTimer.setTimeout(0U, ackTime + 1000U);
	m_frameWaitTime = frameWaitTime;
}

void CSplitRepeaterThread::setBeacon(unsigned int time, const wxString& text, bool voice, TEXT_LANG language)
{
	m_beaconTimer.setTimeout(time);

	if (time > 0U)
		m_beacon = new CBeaconUnit(this, m_rptCallsign, text, voice, language);
}

void CSplitRepeaterThread::setAnnouncement(bool enabled, unsigned int time, const wxString& recordRPT1, const wxString& recordRPT2, const wxString& deleteRPT1, const wxString& deleteRPT2)
{
	if (enabled && time > 0U) {
		m_announcement = new CAnnouncementUnit(this, m_rptCallsign);

		m_announcementTimer.setTimeout(time);

		m_recordRPT1 = recordRPT1;
		m_recordRPT2 = recordRPT2;
		m_deleteRPT1 = deleteRPT1;
		m_deleteRPT2 = deleteRPT2;

		m_recordRPT1.Append(wxT(' '), LONG_CALLSIGN_LENGTH);
		m_recordRPT2.Append(wxT(' '), LONG_CALLSIGN_LENGTH);
		m_deleteRPT1.Append(wxT(' '), LONG_CALLSIGN_LENGTH);
		m_deleteRPT2.Append(wxT(' '), LONG_CALLSIGN_LENGTH);

		m_recordRPT1.Truncate(LONG_CALLSIGN_LENGTH);
		m_recordRPT2.Truncate(LONG_CALLSIGN_LENGTH);
		m_deleteRPT1.Truncate(LONG_CALLSIGN_LENGTH);
		m_deleteRPT2.Truncate(LONG_CALLSIGN_LENGTH);
	}
}

void CSplitRepeaterThread::setControl(bool enabled, const wxString& rpt1Callsign, const wxString& rpt2Callsign, const wxString& shutdown, const wxString& startup, const wxString& status1, const wxString& status2, const wxString& status3, const wxString& status4, const wxString& status5, const wxString& command1, const wxString& command1Line, const wxString& command2, const wxString& command2Line, const wxString& command3, const wxString& command3Line, const wxString& command4, const wxString& command4Line)
{
	m_controlEnabled      = enabled;
	m_controlRPT1         = rpt1Callsign;
	m_controlRPT2         = rpt2Callsign;
	m_controlShutdown     = shutdown;
	m_controlStartup      = startup;
	m_controlStatus1      = status1;
	m_controlStatus2      = status2;
	m_controlStatus3      = status3;
	m_controlStatus4      = status4;
	m_controlStatus5      = status5;
	m_controlCommand1     = command1;
	m_controlCommand1Line = command1Line;
	m_controlCommand2     = command2;
	m_controlCommand2Line = command2Line;
	m_controlCommand3     = command3;
	m_controlCommand3Line = command3Line;
	m_controlCommand4     = command4;
	m_controlCommand4Line = command4Line;

	m_controlRPT1.resize(LONG_CALLSIGN_LENGTH, wxT(' '));
	m_controlRPT2.resize(LONG_CALLSIGN_LENGTH, wxT(' '));
	m_controlShutdown.resize(LONG_CALLSIGN_LENGTH, wxT(' '));
	m_controlStartup.resize(LONG_CALLSIGN_LENGTH, wxT(' '));
	m_controlStatus1.resize(LONG_CALLSIGN_LENGTH, wxT(' '));
	m_controlStatus2.resize(LONG_CALLSIGN_LENGTH, wxT(' '));
	m_controlStatus3.resize(LONG_CALLSIGN_LENGTH, wxT(' '));
	m_controlStatus4.resize(LONG_CALLSIGN_LENGTH, wxT(' '));
	m_controlStatus5.resize(LONG_CALLSIGN_LENGTH, wxT(' '));
	m_controlCommand1.resize(LONG_CALLSIGN_LENGTH, wxT(' '));
	m_controlCommand2.resize(LONG_CALLSIGN_LENGTH, wxT(' '));
	m_controlCommand3.resize(LONG_CALLSIGN_LENGTH, wxT(' '));
	m_controlCommand4.resize(LONG_CALLSIGN_LENGTH, wxT(' '));
}

void CSplitRepeaterThread::setBlackList(CCallsignList* list)
{
	wxASSERT(list != NULL);

	m_blackList = list;
}

void CSplitRepeaterThread::setGreyList(CCallsignList* list)
{
	wxASSERT(list != NULL);

	m_greyList = list;
}

void CSplitRepeaterThread::processReceiver1Header(CSplitRepeaterHeaderData* header)
{
	wxASSERT(header != NULL);

	wxLogMessage(wxT("Receiver 1 header - My: %s/%s  Your: %s  Rpt1: %s  Rpt2: %s  Flags: %02X %02X %02X"), header->getMyCall1().c_str(), header->getMyCall2().c_str(), header->getYourCall().c_str(), header->getRptCall1().c_str(), header->getRptCall2().c_str(), header->getFlag1(), header->getFlag2(), header->getFlag3());

	if (m_receiver2Id != 0x00U) {
		// Make sure it's the same header as receiver 2
		if (m_receiver2Header->getMyCall1().IsSameAs(header->getMyCall1())) {
			m_receiver1Id = header->getId();

			delete m_receiver1Header;
			m_receiver1Header = new CSplitRepeaterHeaderData(*header);

			m_radio1WatchdogTimer.start();

			clearQueue(1U);
		} else {
			wxLogMessage(wxT("Receiver 1 header does not match that of receiver 2, rejecting"));
		}
	} else {
		bool res = processRadioHeader(header);
		if (res) {
			// A valid header and is a DV packet
			m_receiver1Id = header->getId();

			delete m_receiver1Header;
			m_receiver1Header = new CSplitRepeaterHeaderData(*header);

			m_radio1WatchdogTimer.start();

			clearQueue(1U);
			clearQueue(2U);

			m_radioSeqNo     = 0U;
			m_ambeFrames     = 0U;
			m_ambeSilence    = 0U;
			m_ambeBits       = 1U;
			m_ambeErrors     = 0U;
			m_lastAMBEBits   = 0U;
			m_lastAMBEErrors = 0U;

			m_totalPackets      = 0U;
			m_receiver1Packets  = 0U;
			m_receiver2Packets  = 0U;
			m_receiver1Choice   = 0U;
			m_receiver2Choice   = 0U;
			m_receiver1Rejected = 0U;
			m_receiver2Rejected = 0U;
		} else {
			// This is a DD packet or some other problem
			// wxLogMessage(wxT("Invalid header"));
		}
	}
}

void CSplitRepeaterThread::processReceiver2Header(CSplitRepeaterHeaderData* header)
{
	wxASSERT(header != NULL);

	wxLogMessage(wxT("Receiver 2 header - My: %s/%s  Your: %s  Rpt1: %s  Rpt2: %s  Flags: %02X %02X %02X"), header->getMyCall1().c_str(), header->getMyCall2().c_str(), header->getYourCall().c_str(), header->getRptCall1().c_str(), header->getRptCall2().c_str(), header->getFlag1(), header->getFlag2(), header->getFlag3());

	if (m_receiver1Id != 0x00U) {
		// Make sure it's the same header as receiver 1
		if (m_receiver1Header->getMyCall1().IsSameAs(header->getMyCall1())) {
			m_receiver2Id = header->getId();

			delete m_receiver2Header;
			m_receiver2Header = new CSplitRepeaterHeaderData(*header);

			m_radio2WatchdogTimer.start();

			clearQueue(2U);
		} else {
			wxLogMessage(wxT("Receiver 2 header does not match that of receiver 1, rejecting"));
		}
	} else {
		bool res = processRadioHeader(header);
		if (res) {
			// A valid header and is a DV packet
			m_receiver2Id = header->getId();

			delete m_receiver2Header;
			m_receiver2Header = new CSplitRepeaterHeaderData(*header);

			m_radio2WatchdogTimer.start();

			clearQueue(1U);
			clearQueue(2U);

			m_radioSeqNo     = 0U;
			m_ambeFrames     = 0U;
			m_ambeSilence    = 0U;
			m_ambeBits       = 1U;
			m_ambeErrors     = 0U;
			m_lastAMBEBits   = 0U;
			m_lastAMBEErrors = 0U;

			m_totalPackets      = 0U;
			m_receiver1Packets  = 0U;
			m_receiver2Packets  = 0U;
			m_receiver1Choice   = 0U;
			m_receiver2Choice   = 0U;
			m_receiver1Rejected = 0U;
			m_receiver2Rejected = 0U;
		} else {
			// This is a DD packet or some other problem
			// wxLogMessage(wxT("Invalid header"));
		}
	}
}

void CSplitRepeaterThread::receiveNetwork()
{
	NETWORK_TYPE type;

	for (;;) {
		type = m_protocolHandler->read();

		// Get the data from the network
		if (type == NETWORK_NONE) {					// Nothing received
			break;
		} else if (type == NETWORK_HEADER) {		// A header
			CSplitRepeaterHeaderData* header = m_protocolHandler->readHeader();
			if (header != NULL) {
				in_addr   address = header->getAddress();
				unsigned int port = header->getPort();
				unsigned int   id = header->getId();

				if (address.s_addr == m_gatewayAddress.s_addr && port == m_gatewayPort) {
					if (m_networkId == 0x00U)
						processNetworkHeader(header);
					else if (id == m_networkId)
						transmitHeader(header);
				} else if (address.s_addr == m_receiver1Address.s_addr && port == m_receiver1Port) {
					if (m_receiver1Id == 0x00U)
						processReceiver1Header(header);
				} else if (address.s_addr == m_receiver2Address.s_addr && port == m_receiver2Port) {
					if (m_receiver2Id == 0x00U)
						processReceiver2Header(header);
				} else {
					unsigned long addr = ntohl(address.s_addr);
					wxLogMessage(wxT("Header received from an unknown source - %lu.%lu.%lu.%lu:%u"), (addr >> 24) & 0xFFU, (addr >> 16) & 0xFFU, (addr >> 8) & 0xFFU, (addr >> 0) & 0xFFU, port);
				}

				delete header;
			}
		} else if (type == NETWORK_DATA) {			// AMBE data and slow data
			CSplitRepeaterAMBEData* data = m_protocolHandler->readData();
			if (data != NULL) {
				in_addr   address = data->getAddress();
				unsigned int port = data->getPort();
				unsigned int   id = data->getId();

				if (address.s_addr == m_gatewayAddress.s_addr && port == m_gatewayPort) {
					if (id == m_networkId) {
						processNetworkFrame(data);
						m_networkWatchdogTimer.reset();
					}
				} else if (address.s_addr == m_receiver1Address.s_addr && port == m_receiver1Port) {
					if (id == m_receiver1Id) {
						processReceiver1Frame(data);
						m_radio1WatchdogTimer.reset();
					}
				} else if (address.s_addr == m_receiver2Address.s_addr && port == m_receiver2Port) {
					if (id == m_receiver2Id) {
						processReceiver2Frame(data);
						m_radio2WatchdogTimer.reset();
					}
				} else {
					unsigned long addr = ntohl(address.s_addr);
					wxLogMessage(wxT("Data received from an unknown source - %lu.%lu.%lu.%lu:%u"), (addr >> 24) & 0xFFU, (addr >> 16) & 0xFFU, (addr >> 8) & 0xFFU, (addr >> 0) & 0xFFU, port);
				}

				delete data;
			}
		} else if (type == NETWORK_TEXT) {			// Slow data text for the Ack
			m_protocolHandler->readText(m_ackText, m_linkStatus, m_reflector);
			m_linkEncoder.setTextData(m_ackText);
			wxLogMessage(wxT("Slow data set to \"%s\""), m_ackText.c_str());
		} else if (type == NETWORK_TEMPTEXT) {			// Temporary slow data text for the Ack
			m_protocolHandler->readTempText(m_tempAckText);
			wxLogMessage(wxT("Temporary slow data set to \"%s\""), m_tempAckText.c_str());
		} else if (type == NETWORK_STATUS1) {		// Status 1 data text
			m_status1Text = m_protocolHandler->readStatus1();
			m_status1Encoder.setTextData(m_status1Text);
			wxLogMessage(wxT("Status 1 data set to \"%s\""), m_status1Text.c_str());
		} else if (type == NETWORK_STATUS2) {		// Status 2 data text
			m_status2Text = m_protocolHandler->readStatus2();
			m_status2Encoder.setTextData(m_status2Text);
			wxLogMessage(wxT("Status 2 data set to \"%s\""), m_status2Text.c_str());
		} else if (type == NETWORK_STATUS3) {		// Status 3 data text
			m_status3Text = m_protocolHandler->readStatus3();
			m_status3Encoder.setTextData(m_status3Text);
			wxLogMessage(wxT("Status 3 data set to \"%s\""), m_status3Text.c_str());
		} else if (type == NETWORK_STATUS4) {		// Status 4 data text
			m_status4Text = m_protocolHandler->readStatus4();
			m_status4Encoder.setTextData(m_status4Text);
			wxLogMessage(wxT("Status 4 data set to \"%s\""), m_status4Text.c_str());
		} else if (type == NETWORK_STATUS5) {		// Status 5 data text
			m_status5Text = m_protocolHandler->readStatus5();
			m_status5Encoder.setTextData(m_status5Text);
			wxLogMessage(wxT("Status 5 data set to \"%s\""), m_status5Text.c_str());
		}
	}
}

void CSplitRepeaterThread::transmitHeader(CSplitRepeaterHeaderData* header)
{
	wxASSERT(header != NULL);

	if (m_transmitter1Port != 0U) {
		header->setDestination(m_transmitter1Address, m_transmitter1Port);

		m_protocolHandler->writeHeader(*header);
		m_protocolHandler->writeHeader(*header);
	}

	if (m_transmitter2Port != 0U) {
		header->setDestination(m_transmitter2Address, m_transmitter2Port);

		m_protocolHandler->writeHeader(*header);
		m_protocolHandler->writeHeader(*header);
	}
}

void CSplitRepeaterThread::transmitFrame(CSplitRepeaterAMBEData* data)
{
	wxASSERT(data != NULL);

	if (m_transmitter1Port != 0U) {
		data->setDestination(m_transmitter1Address, m_transmitter1Port);
		m_protocolHandler->writeData(*data);
	}

	if (m_transmitter2Port != 0U) {
		data->setDestination(m_transmitter2Address, m_transmitter2Port);
		m_protocolHandler->writeData(*data);
	}
}

void CSplitRepeaterThread::transmitStatus()
{
	unsigned int id = CSplitRepeaterHeaderData::createId();

	CSplitRepeaterHeaderData header(m_rptCallsign, wxT("    "), m_rxHeader->getMyCall1(), m_gwyCallsign, m_rptCallsign, RELAY_UNAVAILABLE);
	header.setId(id);

	wxLogMessage(wxT("Transmitting to - My: %s/%s  Your: %s  Rpt1: %s  Rpt2: %s  Flags: %02X %02X %02X"), header.getMyCall1().c_str(), header.getMyCall2().c_str(), header.getYourCall().c_str(), header.getRptCall1().c_str(), header.getRptCall2().c_str(), header.getFlag1(), header.getFlag2(), header.getFlag3());
	transmitHeader(&header);

	CSplitRepeaterAMBEData data;

	// Filler data
	for (unsigned int i = 0U; i < 21U; i++) {
		unsigned char buffer[DV_FRAME_LENGTH_BYTES];

		if (i == 0U) {
			m_ackEncoder.sync();

			::memcpy(buffer + 0U, NULL_AMBE_DATA_BYTES, VOICE_FRAME_LENGTH_BYTES);
			::memcpy(buffer + VOICE_FRAME_LENGTH_BYTES, DATA_SYNC_BYTES, DATA_FRAME_LENGTH_BYTES);
		} else {
			unsigned char text[DATA_FRAME_LENGTH_BYTES];
			m_ackEncoder.getTextData(text);

			::memcpy(buffer + 0U, NULL_AMBE_DATA_BYTES, VOICE_FRAME_LENGTH_BYTES);
			::memcpy(buffer + VOICE_FRAME_LENGTH_BYTES, text, DATA_FRAME_LENGTH_BYTES);
		}

		data.setData(buffer, DV_FRAME_LENGTH_BYTES);
		data.setSeq(i);
		data.setId(id);
		transmitFrame(&data);
	}

	data.setData(END_PATTERN_BYTES, DV_FRAME_LENGTH_BYTES);
	data.setSeq(0U);
	data.setId(id);
	data.setEnd(true);
	transmitFrame(&data);
}

void CSplitRepeaterThread::transmitErrorStatus()
{
	unsigned int id = CSplitRepeaterHeaderData::createId();

	CSplitRepeaterHeaderData header(m_rptCallsign, wxT("    "), m_rxHeader->getMyCall1(), m_rptCallsign, m_rptCallsign, RELAY_UNAVAILABLE);
	header.setId(id);

	wxLogMessage(wxT("Transmitting to - My: %s/%s  Your: %s  Rpt1: %s  Rpt2: %s  Flags: %02X %02X %02X"), header.getMyCall1().c_str(), header.getMyCall2().c_str(), header.getYourCall().c_str(), header.getRptCall1().c_str(), header.getRptCall2().c_str(), header.getFlag1(), header.getFlag2(), header.getFlag3());
	transmitHeader(&header);

	CSplitRepeaterAMBEData data;

	// Filler data
	for (unsigned int i = 0U; i < 21U; i++) {
		unsigned char buffer[DV_FRAME_LENGTH_BYTES];

		if (i == 0U) {
			m_linkEncoder.sync();

			::memcpy(buffer + 0U, NULL_AMBE_DATA_BYTES, VOICE_FRAME_LENGTH_BYTES);
			::memcpy(buffer + VOICE_FRAME_LENGTH_BYTES, DATA_SYNC_BYTES, DATA_FRAME_LENGTH_BYTES);
		} else {
			unsigned char text[DATA_FRAME_LENGTH_BYTES];
			m_linkEncoder.getTextData(text);

			::memcpy(buffer + 0U, NULL_AMBE_DATA_BYTES, VOICE_FRAME_LENGTH_BYTES);
			::memcpy(buffer + VOICE_FRAME_LENGTH_BYTES, text, DATA_FRAME_LENGTH_BYTES);
		}

		data.setData(buffer, DV_FRAME_LENGTH_BYTES);
		data.setSeq(i);
		data.setId(id);
		transmitFrame(&data);
	}

	data.setData(END_PATTERN_BYTES, DV_FRAME_LENGTH_BYTES);
	data.setSeq(0U);
	data.setId(id);
	data.setEnd(true);
	transmitFrame(&data);
}

void CSplitRepeaterThread::transmitBeaconHeader()
{
	m_beaconId  = CSplitRepeaterHeaderData::createId();
	m_beaconSeq = 0U;

	CSplitRepeaterHeaderData header(m_rptCallsign, wxT("RPTR"), wxT("CQCQCQ  "), m_gwyCallsign, m_rptCallsign);
	header.setId(m_beaconId);

	wxLogMessage(wxT("Transmitting to - My: %s/%s  Your: %s  Rpt1: %s  Rpt2: %s  Flags: %02X %02X %02X"), header.getMyCall1().c_str(), header.getMyCall2().c_str(), header.getYourCall().c_str(), header.getRptCall1().c_str(), header.getRptCall2().c_str(), header.getFlag1(), header.getFlag2(), header.getFlag3());
	transmitHeader(&header);
}

void CSplitRepeaterThread::transmitBeaconData(const unsigned char* data, unsigned int length, bool end)
{
	CSplitRepeaterAMBEData ambe;

	ambe.setData(data, length);
	ambe.setSeq(m_beaconSeq);
	ambe.setId(m_beaconId);
	ambe.setEnd(end);

	m_beaconSeq++;
	if (m_beaconSeq >= 21U)
		m_beaconSeq = 0U;

	transmitFrame(&ambe);
}

void CSplitRepeaterThread::transmitAnnouncementHeader(CHeaderData* header1)
{
	m_announcementId  = CSplitRepeaterHeaderData::createId();
	m_announcementSeq = 0U;

	CSplitRepeaterHeaderData header2;
	header2.setFlag1(header1->getFlag1());
	header2.setFlag2(header1->getFlag2());
	header2.setFlag3(header1->getFlag3());
	header2.setMyCall1(header1->getMyCall1());
	header2.setMyCall2(header1->getMyCall2());
	header2.setYourCall(header1->getYourCall());
	header2.setRptCall1(m_gwyCallsign);
	header2.setRptCall2(m_rptCallsign);
	header2.setId(m_beaconId);

	delete header1;

	wxLogMessage(wxT("Transmitting to - My: %s/%s  Your: %s  Rpt1: %s  Rpt2: %s  Flags: %02X %02X %02X"), header2.getMyCall1().c_str(), header2.getMyCall2().c_str(), header2.getYourCall().c_str(), header2.getRptCall1().c_str(), header2.getRptCall2().c_str(), header2.getFlag1(), header2.getFlag2(), header2.getFlag3());
	transmitHeader(&header2);
}

void CSplitRepeaterThread::transmitAnnouncementData(const unsigned char* data, unsigned int length, bool end)
{
	CSplitRepeaterAMBEData ambe;

	ambe.setData(data, length);
	ambe.setSeq(m_announcementSeq);
	ambe.setId(m_announcementId);
	ambe.setEnd(end);

	m_announcementSeq++;
	if (m_announcementSeq >= 21U)
		m_announcementSeq = 0U;

	transmitFrame(&ambe);
}

void CSplitRepeaterThread::transmitUserStatus(unsigned int n)
{
	unsigned int id = CSplitRepeaterHeaderData::createId();

	CSlowDataEncoder* encoder = NULL;
	CSplitRepeaterHeaderData* header = NULL;
	switch (n) {
		case 0U:
			header = new CSplitRepeaterHeaderData(m_rptCallsign, wxT("    "), wxT("STATUS 1"), m_gwyCallsign, m_rptCallsign);
			encoder = &m_status1Encoder;
			break;
		case 1U:
			header = new CSplitRepeaterHeaderData(m_rptCallsign, wxT("    "), wxT("STATUS 2"), m_gwyCallsign, m_rptCallsign);
			encoder = &m_status2Encoder;
			break;
		case 2U:
			header = new CSplitRepeaterHeaderData(m_rptCallsign, wxT("    "), wxT("STATUS 3"), m_gwyCallsign, m_rptCallsign);
			encoder = &m_status3Encoder;
			break;
		case 3U:
			header = new CSplitRepeaterHeaderData(m_rptCallsign, wxT("    "), wxT("STATUS 4"), m_gwyCallsign, m_rptCallsign);
			encoder = &m_status4Encoder;
			break;
		case 4U:
			header = new CSplitRepeaterHeaderData(m_rptCallsign, wxT("    "), wxT("STATUS 5"), m_gwyCallsign, m_rptCallsign);
			encoder = &m_status5Encoder;
			break;
		default:
			wxLogWarning(wxT("Invalid status number - %u"), n);
			return;
	}

	header->setId(id);

	wxLogMessage(wxT("Transmitting to - My: %s/%s  Your: %s  Rpt1: %s  Rpt2: %s  Flags: %02X %02X %02X"), header->getMyCall1().c_str(), header->getMyCall2().c_str(), header->getYourCall().c_str(), header->getRptCall1().c_str(), header->getRptCall2().c_str(), header->getFlag1(), header->getFlag2(), header->getFlag3());
	transmitHeader(header);

	delete header;

	CSplitRepeaterAMBEData data;

	// Filler data
	for (unsigned int i = 0U; i < 21U; i++) {
		unsigned char buffer[DV_FRAME_LENGTH_BYTES];

		if (i == 0U) {
			encoder->sync();

			::memcpy(buffer + 0U, NULL_AMBE_DATA_BYTES, VOICE_FRAME_LENGTH_BYTES);
			::memcpy(buffer + VOICE_FRAME_LENGTH_BYTES, DATA_SYNC_BYTES, DATA_FRAME_LENGTH_BYTES);
		} else {
			unsigned char text[DATA_FRAME_LENGTH_BYTES];
			encoder->getTextData(text);

			::memcpy(buffer + 0U, NULL_AMBE_DATA_BYTES, VOICE_FRAME_LENGTH_BYTES);
			::memcpy(buffer + VOICE_FRAME_LENGTH_BYTES, text, DATA_FRAME_LENGTH_BYTES);
		}

		data.setData(buffer, DV_FRAME_LENGTH_BYTES);
		data.setSeq(i);
		data.setId(id);
		transmitFrame(&data);
	}

	data.setData(END_PATTERN_BYTES, DV_FRAME_LENGTH_BYTES);
	data.setSeq(0U);
	data.setId(id);
	data.setEnd(true);
	transmitFrame(&data);
}

void CSplitRepeaterThread::repeaterStateMachine()
{
	if (m_radio1WatchdogTimer.isRunning() && m_radio1WatchdogTimer.hasExpired()) {
		m_radio1WatchdogTimer.stop();
		wxLogMessage(wxT("Receiver 1 watchdog has expired"));
		m_receiver1Id = 0x00U;
		clearQueue(1U);

		if (m_receiver2Id == 0x00U) {
			m_radioId = 0x00U;
			endOfRadioData();
			m_recording = false;
			m_deleting  = false;
			m_busyData  = false;
		}
	}

	if (m_radio2WatchdogTimer.isRunning() && m_radio2WatchdogTimer.hasExpired()) {
		m_radio2WatchdogTimer.stop();
		wxLogMessage(wxT("Receiver 2 watchdog has expired"));
		m_receiver2Id = 0x00U;
		clearQueue(2U);

		if (m_receiver1Id == 0x00U) {
			m_radioId = 0x00U;
			endOfRadioData();
			m_recording = false;
			m_deleting  = false;
			m_busyData  = false;
		}
	}

	switch (m_state) {
		case DSRS_VALID:
			if (m_timeoutTimer.isRunning() && m_timeoutTimer.hasExpired()) {
				wxLogMessage(wxT("User has timed out"));
				setRepeaterState(DSRS_TIMEOUT);
			}
			break;

		case DSRS_VALID_WAIT:
			if (m_ackTimer.hasExpired()) {
				transmitStatus();
				setRepeaterState(DSRS_LISTENING);
			}
			break;

		case DSRS_INVALID_WAIT:
			if (m_ackTimer.hasExpired()) {
				transmitErrorStatus();
				setRepeaterState(DSRS_LISTENING);
			}
			break;

		case DSRS_TIMEOUT_WAIT:
			if (m_ackTimer.hasExpired()) {
				transmitStatus();
				setRepeaterState(DSRS_LISTENING);
			}
			break;

		case DSRS_NETWORK:
			if (m_networkWatchdogTimer.hasExpired()) {
				wxLogMessage(wxT("Network watchdog has expired"));
				setRepeaterState(DSRS_LISTENING);
			}
			break;

		default:
			break;
	}
}

bool CSplitRepeaterThread::setRepeaterState(DSTAR_RPT_STATE state)
{
	// Can't change state when shutdown
	if (m_shutdown)
		return false;

	// The "from" state
	switch (m_state) {
		case DSRS_SHUTDOWN:
			m_beaconTimer.start();
			m_announcementTimer.start();
			break;

		case DSRS_LISTENING:
			m_beaconTimer.stop();
			break;

		default:
			break;
	}

	// The "to" state
	switch (state) {
		case DSRS_SHUTDOWN:
			m_timeoutTimer.stop();
			m_radio1WatchdogTimer.stop();
			m_radio2WatchdogTimer.stop();
			m_networkWatchdogTimer.stop();
			m_ackTimer.stop();
			m_beaconTimer.stop();
			m_announcementTimer.stop();
			m_state = DSRS_SHUTDOWN;
			break;

		case DSRS_LISTENING:
			m_timeoutTimer.stop();
			m_networkWatchdogTimer.stop();
			m_ackTimer.stop();
			m_beaconTimer.start();
			m_networkId = 0x00U;
			m_state = DSRS_LISTENING;
			break;

		case DSRS_VALID:
			if (m_state != DSRS_LISTENING && m_state != DSRS_VALID_WAIT)
				return false;

			if (m_state == DSRS_LISTENING)
				m_timeoutTimer.start();
			else
				m_ackTimer.stop();

			m_state = DSRS_VALID;
			break;

		case DSRS_VALID_WAIT:
			m_ackTimer.start();
			m_state = DSRS_VALID_WAIT;
			break;

		case DSRS_TIMEOUT: {
				CSplitRepeaterAMBEData data;
				data.setData(END_PATTERN_BYTES, DV_FRAME_LENGTH_BYTES);
				data.setId(m_radioId);
				data.setSeq(m_radioSeqNo);
				data.setEnd(true);

				// Send end of transmission data to the radio and the network
				if (m_mode == MODE_DUPLEX)
					transmitFrame(&data);

				m_protocolHandler->writeData(data);

				m_timeoutTimer.stop();
				m_state = DSRS_TIMEOUT;
			}
			break;

		case DSRS_TIMEOUT_WAIT:
			m_ackTimer.start();
			m_timeoutTimer.stop();
			m_state = DSRS_TIMEOUT_WAIT;
			break;

		case DSRS_INVALID:
			if (m_state != DSRS_LISTENING)
				return false;

			m_timeoutTimer.stop();
			m_state = DSRS_INVALID;
			break;

		case DSRS_INVALID_WAIT:
			m_ackTimer.start();
			m_timeoutTimer.stop();
			m_state = DSRS_INVALID_WAIT;
			break;

		case DSRS_NETWORK:
			if (m_state != DSRS_LISTENING && m_state != DSRS_VALID_WAIT && m_state != DSRS_INVALID_WAIT && m_state != DSRS_TIMEOUT_WAIT)
				return false;

			m_state = DSRS_NETWORK;
			m_timeoutTimer.stop();
			m_networkWatchdogTimer.start();
			m_ackTimer.stop();
			break;
	}

	return true;
}

bool CSplitRepeaterThread::processRadioHeader(CSplitRepeaterHeaderData* header)
{
	wxASSERT(header != NULL);

	// Check control messages
	bool res = checkControl(*header);
	if (res)
		return true;

	// Check announcement messages
	res = checkAnnouncements(*header);
	if (res) {
		bool res = setRepeaterState(DSRS_INVALID);
		if (res) {
			delete m_rxHeader;
			m_rxHeader = new CSplitRepeaterHeaderData(*header);
		}
		return true;
	}

	// If shutdown we ignore incoming headers
	if (m_state == DSRS_SHUTDOWN)
		return true;

	if (m_blackList != NULL) {
		bool res = m_blackList->isInList(header->getMyCall1());
		if (res) {
			wxLogMessage(wxT("%s rejected due to being in the black list"), header->getMyCall1().c_str());
			return true;
		}
	}

	m_blocked = false;
	if (m_greyList != NULL) {
		bool res = m_greyList->isInList(header->getMyCall1());
		if (res) {
			wxLogMessage(wxT("%s blocked from the network due to being in the grey list"), header->getMyCall1().c_str());
			m_blocked = true;
		}
	}

	// We don't handle DD data packets
	if (header->isDataPacket()) {
		wxLogMessage(wxT("Received a DD packet, ignoring"));
		return false;
	}

	TRISTATE valid = checkHeader(*header);
	switch (valid) {
		case STATE_FALSE: {
				bool res = setRepeaterState(DSRS_INVALID);
				if (res) {
					delete m_rxHeader;
					m_rxHeader = new CSplitRepeaterHeaderData(*header);
				}
			}
			return true;

		case STATE_UNKNOWN:
			return true;

		case STATE_TRUE:
			break;
	}

	res = setRepeaterState(DSRS_VALID);
	if (res) {
		delete m_rxHeader;
		m_rxHeader = new CSplitRepeaterHeaderData(*header);
	}

	// Save the header for later transmission
	delete m_txHeader;
	m_txHeader = new CSplitRepeaterHeaderData(*header);

	m_radioId = CSplitRepeaterHeaderData::createId();

	return true;
}

void CSplitRepeaterThread::processRadioHeader()
{
	// If m_txHeader is not NULL then it needs to be transmitted
	if (m_txHeader == NULL)
		return;

	// Send the valid header to the gateway if we are accepted
	if (m_state == DSRS_VALID) {
		// Only send on the network if the user isn't blocked and we have one and RPT2 is not blank or the repeater callsign
		if (!m_blocked && !m_txHeader->getRptCall2().IsSameAs(wxT("        ")) && !m_txHeader->getRptCall2().IsSameAs(m_rptCallsign)) {
			CSplitRepeaterHeaderData netHeader(*m_txHeader);
			netHeader.setFlag1(m_txHeader->getFlag1() & ~REPEATER_MASK);
			netHeader.setDestination(m_gatewayAddress, m_gatewayPort);
			netHeader.setId(m_radioId);
			m_protocolHandler->writeHeader(netHeader);
		}

		// Create the new radio header but only in duplex mode
		if (m_mode == MODE_DUPLEX) {
			CSplitRepeaterHeaderData rfHeader(*m_txHeader);
			rfHeader.setRptCall1(m_rptCallsign);
			rfHeader.setRptCall2(m_rptCallsign);
			rfHeader.setFlag1(m_txHeader->getFlag1() & ~REPEATER_MASK);
			rfHeader.setId(m_radioId);

			wxLogMessage(wxT("Transmitting to - My: %s/%s  Your: %s  Rpt1: %s  Rpt2: %s  Flags: %02X %02X %02X"), rfHeader.getMyCall1().c_str(), rfHeader.getMyCall2().c_str(), rfHeader.getYourCall().c_str(), rfHeader.getRptCall1().c_str(), rfHeader.getRptCall2().c_str(), rfHeader.getFlag1(), rfHeader.getFlag2(), rfHeader.getFlag3());
			transmitHeader(&rfHeader);
		}
	}

	// If we're in network mode, send the header as a busy header to the gateway in case it's an unlink
	// command
	if (m_state == DSRS_NETWORK) {
		// Only send on the network if we have one and RPT2 is not blank or the repeater callsign
		if (!m_txHeader->getRptCall2().IsSameAs(wxT("        ")) && !m_txHeader->getRptCall2().IsSameAs(m_rptCallsign)) {
			if (!m_blocked) {
				CSplitRepeaterHeaderData netHeader(*m_txHeader);
				netHeader.setRptCall1(m_txHeader->getRptCall2());
				netHeader.setRptCall2(m_txHeader->getRptCall1());
				netHeader.setFlag1(m_txHeader->getFlag1() & ~REPEATER_MASK);
				netHeader.setDestination(m_gatewayAddress, m_gatewayPort);
				netHeader.setId(m_radioId);
				m_protocolHandler->writeBusyHeader(netHeader);
			}

			m_busyData = true;
		}
	}

	delete m_txHeader;
	m_txHeader = NULL;
}

void CSplitRepeaterThread::processReceiver1Frame(CSplitRepeaterAMBEData* data)
{
	wxASSERT(data != NULL);

	unsigned int seqNo = data->getSeq();

	// Count the number of frames between
	unsigned int tempSeqNo = m_radioSeqNo;
	unsigned int count = 0U;
	while (seqNo != tempSeqNo) {
		count++;

		tempSeqNo++;
		if (tempSeqNo >= 21U)
			tempSeqNo = 0U;
	}

	// If the number is too high, then it probably means an old out-of-order frame, ignore it
	if (count > 18U) {
		m_receiver1Rejected++;
		return;
	}

	delete m_receiver1Data[seqNo];

	data->setTime(m_packetTime.Time());

	m_receiver1Data[seqNo] = new CSplitRepeaterAMBEData(*data);
}

void CSplitRepeaterThread::processReceiver2Frame(CSplitRepeaterAMBEData* data)
{
	wxASSERT(data != NULL);

	unsigned int seqNo = data->getSeq();

	// Count the number of frames between
	unsigned int tempSeqNo = m_radioSeqNo;
	unsigned int count = 0U;
	while (seqNo != tempSeqNo) {
		count++;

		tempSeqNo++;
		if (tempSeqNo >= 21U)
			tempSeqNo = 0U;
	}

	// If the number is too high, then it probably means an old out-of-order frame, ignore it
	if (count > 18U) {
		m_receiver2Rejected++;
		return;
	}

	delete m_receiver2Data[seqNo];

	data->setTime(m_packetTime.Time());

	m_receiver2Data[seqNo] = new CSplitRepeaterAMBEData(*data);
}

void CSplitRepeaterThread::processNetworkHeader(CSplitRepeaterHeaderData* header)
{
	wxASSERT(header != NULL);

	// If shutdown we ignore incoming headers
	if (m_state == DSRS_SHUTDOWN)
		return;

	wxLogMessage(wxT("Network header received - My: %s/%s  Your: %s  Rpt1: %s  Rpt2: %s  Flags: %02X %02X %02X"), header->getMyCall1().c_str(), header->getMyCall2().c_str(), header->getYourCall().c_str(), header->getRptCall1().c_str(), header->getRptCall2().c_str(), header->getFlag1(), header->getFlag2(), header->getFlag3());

	// Is it for us?
	if (!header->getRptCall2().IsSameAs(m_rptCallsign)) {
		wxLogMessage(wxT("Invalid network RPT2 value, ignoring"));
		return;
	}

	bool res = setRepeaterState(DSRS_NETWORK);
	if (res) {
		delete m_rxHeader;
		m_rxHeader = new CSplitRepeaterHeaderData(*header);

		m_networkId = header->getId();

		wxLogMessage(wxT("Transmitting to - My: %s/%s  Your: %s  Rpt1: %s  Rpt2: %s  Flags: %02X %02X %02X"), header->getMyCall1().c_str(), header->getMyCall2().c_str(), header->getYourCall().c_str(), header->getRptCall1().c_str(), header->getRptCall2().c_str(), header->getFlag1(), header->getFlag2(), header->getFlag3());
		transmitHeader(header);
	}
}

void CSplitRepeaterThread::processRadioFrame(CSplitRepeaterAMBEData* data)
{
	wxASSERT(data != NULL);

	// Transmit the header if it hasn't already been transmitted
	processRadioHeader();

	m_ambeFrames++;

	unsigned char bytes[DV_FRAME_LENGTH_BYTES];
	data->getData(bytes, DV_FRAME_LENGTH_BYTES);

	bool end = data->isEnd();

	// Only regenerate the AMBE on received radio data
	unsigned int errors = 0U;
	if (!end) {
		errors = data->getErrors();
		m_ambeErrors += errors;
		m_ambeBits   += 48U;		// Only count the bits with FEC added
	}

	if (::memcmp(bytes, NULL_AMBE_DATA_BYTES, VOICE_FRAME_LENGTH_BYTES) == 0)
		m_ambeSilence++;

	// If this is deleting an announcement, ignore the audio
	if (m_deleting)
		return;

	// If this is recording an announcement, send the audio to the announcement unit and then stop
	if (m_recording) {
		m_announcement->writeData(bytes, DV_FRAME_LENGTH_BYTES, end);
		return;
	}

	if (m_busyData) {
		if (!m_blocked) {
			// Pass background AMBE data to the network if the user isn't blocked
			data->setDestination(m_gatewayAddress, m_gatewayPort);
			data->setId(m_radioId);
			m_protocolHandler->writeBusyData(*data);
		}
	} else if (m_state == DSRS_VALID) {
		// Don't pass through the frame of an invalid transmission
		data->setId(m_radioId);

		// Transmit the data on the radio, but only in duplex mode
		if (m_mode == MODE_DUPLEX) {
			if (m_blanking)
				blankDTMF(*data);
			transmitFrame(data);
		}

		// Send data over the network if the user is not blocked
		if (!m_blocked) {
			data->setDestination(m_gatewayAddress, m_gatewayPort);
			m_protocolHandler->writeData(*data);
		}
	}

	if (end) {
		m_radio1WatchdogTimer.stop();
		m_radio2WatchdogTimer.stop();

		m_receiver1Id = 0x00U;
		m_receiver2Id = 0x00U;
		m_radioId = 0x00U;

		clearQueue(1U);
		clearQueue(2U);

		endOfRadioData();

		m_busyData  = false;
		m_recording = false;
		m_deleting  = false;
	}
}

void CSplitRepeaterThread::processNetworkFrame(CSplitRepeaterAMBEData* data)
{
	wxASSERT(data != NULL);

	if (m_state != DSRS_NETWORK)
		return;

	bool end = data->isEnd();
	if (end) {
		blankDTMF(*data);
		transmitFrame(data);
		setRepeaterState(DSRS_LISTENING);
		return;
	}

	blankDTMF(*data);
	transmitFrame(data);
}

void CSplitRepeaterThread::chooseReceiver()
{
	unsigned long t = m_packetTime.Time();

	unsigned int seqNo = m_radioSeqNo;

	for (unsigned int i = 0U; i < 21U; i++) {
		CSplitRepeaterAMBEData* rx1 = m_receiver1Data[seqNo];
		CSplitRepeaterAMBEData* rx2 = m_receiver2Data[seqNo];

		if (rx1 != NULL && m_receiver2Id == 0x00U) {
			// Data from receiver 1, and receiver 2 is not active
			m_totalPackets++;
			m_receiver1Packets++;
			m_receiver1Choice++;

			processRadioFrame(rx1);

			delete m_receiver1Data[seqNo];
			m_receiver1Data[seqNo] = NULL;

			m_radioSeqNo = seqNo + 1U;
			if (m_radioSeqNo >= 21U)
				m_radioSeqNo = 0U;
		} else if (rx2 != NULL && m_receiver1Id == 0x00U) {
			// Data from receiver 2, and receiver 1 is not active
			m_totalPackets++;
			m_receiver2Packets++;
			m_receiver2Choice++;

			processRadioFrame(rx2);

			delete m_receiver2Data[seqNo];
			m_receiver2Data[seqNo] = NULL;

			m_radioSeqNo = seqNo + 1U;
			if (m_radioSeqNo >= 21U)
				m_radioSeqNo = 0U;
		} else if (rx1 != NULL && rx2 != NULL) {
			m_totalPackets++;
			m_receiver1Packets++;
			m_receiver2Packets++;

			bool end1 = rx1->isEnd();
			bool end2 = rx2->isEnd();

			if (!end1 && !end2) {
				// Normal data, choose the one with the lowest errors
				unsigned int err1 = rx1->getErrors();
				unsigned int err2 = rx2->getErrors();

				if (err1 <= err2) {
					processRadioFrame(rx1);
					m_receiver1Choice++;
				} else {
					processRadioFrame(rx2);
					m_receiver2Choice++;
				}
			} else if (end1 && !end2) {
				// Receiver 1 has given up, but receiver 2 is still OK
				wxLogMessage(wxT("Receiver 1 has ended"));

				processRadioFrame(rx2);
				m_receiver2Choice++;

				m_receiver1Id = 0x00U;
				m_radio1WatchdogTimer.stop();
				clearQueue(1U);
			} else if (!end1 && end2) {
				// Receiver 2 has given up, but receiver 1 is still OK
				wxLogMessage(wxT("Receiver 2 has ended"));

				processRadioFrame(rx1);
				m_receiver1Choice++;

				m_receiver2Id = 0x00U;
				m_radio2WatchdogTimer.stop();
				clearQueue(2U);
			} else {	// (end1 && end2)
				wxLogMessage(wxT("Receiver 1 and receiver 2 have ended"));
				// It really is the end of the radio transmission
				processRadioFrame(rx1);
				m_receiver1Choice++;
			}

			delete m_receiver1Data[seqNo];
			delete m_receiver2Data[seqNo];

			m_receiver1Data[seqNo] = NULL;
			m_receiver2Data[seqNo] = NULL;

			m_radioSeqNo = seqNo + 1U;
			if (m_radioSeqNo >= 21U)
				m_radioSeqNo = 0U;

			if (end1 && end2)
				return;
		} else if (rx1 != NULL && m_receiver2Id != 0x00U) {
			// Data from receiver 1, and receiver 2 is active
			unsigned long t1 = t - rx1->getTime();
			if (t1 < m_frameWaitTime) {
				m_radioSeqNo = seqNo;
				return;
			}

			bool end = rx1->isEnd();
			if (end) {
				wxLogMessage(wxT("Receiver 1 has ended"));

				// We have the end on receiver 1 and receiver 2 is active
				m_radioSeqNo = seqNo;

				m_receiver1Id = 0x00U;
				m_radio1WatchdogTimer.stop();
				clearQueue(1U);

				return;
			} else {
				m_totalPackets++;
				m_receiver1Packets++;
				m_receiver1Choice++;

				processRadioFrame(rx1);

				m_radioSeqNo = seqNo + 1U;
				if (m_radioSeqNo >= 21U)
					m_radioSeqNo = 0U;
			}

			delete m_receiver1Data[seqNo];
			m_receiver1Data[seqNo] = NULL;
		} else if (rx2 != NULL && m_receiver1Id != 0x00U) {
			// Data from receiver 1, and receiver 2 is active
			unsigned long t2 = t - rx2->getTime();
			if (t2 < m_frameWaitTime) {
				m_radioSeqNo = seqNo;
				return;
			}

			bool end = rx2->isEnd();
			if (end) {
				wxLogMessage(wxT("Receiver 2 has ended"));

				// We have the end on receiver 2 and receiver 1 is active
				m_radioSeqNo = seqNo;

				m_receiver2Id = 0x00U;
				m_radio2WatchdogTimer.stop();
				clearQueue(2U);

				return;
			} else {
				m_totalPackets++;
				m_receiver2Packets++;
				m_receiver2Choice++;

				processRadioFrame(rx2);

				m_radioSeqNo = seqNo + 1U;
				if (m_radioSeqNo >= 21U)
					m_radioSeqNo = 0U;
			}

			delete m_receiver2Data[seqNo];
			m_receiver2Data[seqNo] = NULL;
		}

		seqNo++;
		if (seqNo >= 21U)
			seqNo = 0U;
	}
}

void CSplitRepeaterThread::endOfRadioData()
{
	switch (m_state) {
		case DSRS_VALID:
			wxLogMessage(wxT("AMBE for %s  Frames: %.1fs, Silence: %.1f%%, BER: %.1f%%"), m_rxHeader->getMyCall1().c_str(), float(m_ambeFrames) / 50.0F, float(m_ambeSilence * 100U) / float(m_ambeFrames), float(m_ambeErrors * 100U) / float(m_ambeBits));

			if (m_totalPackets > 0U)
				wxLogMessage(wxT("Diversity for %s  Frames: %u/%u/%u, Rejected: %u/%u, Chosen: %u/%u, %.1f%%/%.1f%%"), m_rxHeader->getMyCall1().c_str(), m_totalPackets, m_receiver1Packets, m_receiver2Packets, m_receiver1Rejected, m_receiver2Rejected, m_receiver1Choice, m_receiver2Choice, float(m_receiver1Choice * 100U) / float(m_totalPackets), float(m_receiver2Choice * 100U) / float(m_totalPackets));

			if (m_tempAckText.IsEmpty()) {
				if (m_ack == AT_BER) {
					// Create the ack text with the linked reflector and BER
					wxString ackText;
					if (m_linkStatus == LS_LINKED_DEXTRA || m_linkStatus == LS_LINKED_DPLUS || m_linkStatus == LS_LINKED_DCS || m_linkStatus == LS_LINKED_CCS || m_linkStatus == LS_LINKED_LOOPBACK)
						ackText.Printf(wxT("%-8s  BER: %.1f%%   "), m_reflector.c_str(), float(m_ambeErrors * 100U) / float(m_ambeBits));
					else
						ackText.Printf(wxT("BER: %.1f%%            "), float(m_ambeErrors * 100U) / float(m_ambeBits));
					m_ackEncoder.setTextData(ackText);
				} else {
					m_ackEncoder.setTextData(m_ackText);
				}
			} else {
				m_ackEncoder.setTextData(m_tempAckText);
				m_tempAckText.Clear();
			}

			if (m_ack != AT_NONE || m_mode == MODE_GATEWAY)
				setRepeaterState(DSRS_VALID_WAIT);
			else
				setRepeaterState(DSRS_LISTENING);
			break;

		case DSRS_INVALID:
			wxLogMessage(wxT("AMBE for %s  Frames: %.1fs, Silence: %.1f%%, BER: %.1f%%"), m_rxHeader->getMyCall1().c_str(), float(m_ambeFrames) / 50.0F, float(m_ambeSilence * 100U) / float(m_ambeFrames), float(m_ambeErrors * 100U) / float(m_ambeBits));

			if (m_totalPackets > 0U)
				wxLogMessage(wxT("Diversity for %s  Frames: %u/%u/%u, Rejected: %u/%u, Chosen: %u/%u, %.1f%%/%.1f%%"), m_rxHeader->getMyCall1().c_str(), m_totalPackets, m_receiver1Packets, m_receiver2Packets, m_receiver1Rejected, m_receiver2Rejected, m_receiver1Choice, m_receiver2Choice, float(m_receiver1Choice * 100U) / float(m_totalPackets), float(m_receiver2Choice * 100U) / float(m_totalPackets));

			if (m_ack != AT_NONE || m_mode == MODE_GATEWAY)
				setRepeaterState(DSRS_INVALID_WAIT);
			else
				setRepeaterState(DSRS_LISTENING);
			break;

		case DSRS_TIMEOUT:
			wxLogMessage(wxT("AMBE for %s  Frames: %.1fs, Silence: %.1f%%, BER: %.1f%%"), m_rxHeader->getMyCall1().c_str(), float(m_ambeFrames) / 50.0F, float(m_ambeSilence * 100U) / float(m_ambeFrames), float(m_ambeErrors * 100U) / float(m_ambeBits));

			if (m_totalPackets > 0U)
				wxLogMessage(wxT("Diversity for %s  Frames: %u/%u/%u, Rejected: %u/%u, Chosen: %u/%u, %.1f%%/%.1f%%"), m_rxHeader->getMyCall1().c_str(), m_totalPackets, m_receiver1Packets, m_receiver2Packets, m_receiver1Rejected, m_receiver2Rejected, m_receiver1Choice, m_receiver2Choice, float(m_receiver1Choice * 100U) / float(m_totalPackets), float(m_receiver2Choice * 100U) / float(m_totalPackets));

			if (m_tempAckText.IsEmpty()) {
				if (m_ack == AT_BER) {
					// Create the ack text with the linked reflector and BER
					wxString ackText;
					if (m_linkStatus == LS_LINKED_DEXTRA || m_linkStatus == LS_LINKED_DPLUS || m_linkStatus == LS_LINKED_DCS || m_linkStatus == LS_LINKED_CCS || m_linkStatus == LS_LINKED_LOOPBACK)
						ackText.Printf(wxT("%-8s  BER: %.1f%%   "), m_reflector.c_str(), float(m_ambeErrors * 100U) / float(m_ambeBits));
					else
						ackText.Printf(wxT("BER: %.1f%%            "), float(m_ambeErrors * 100U) / float(m_ambeBits));
					m_ackEncoder.setTextData(ackText);
				} else {
					m_ackEncoder.setTextData(m_ackText);
				}
			} else {
				m_ackEncoder.setTextData(m_tempAckText);
				m_tempAckText.Clear();
			}

			if (m_ack != AT_NONE || m_mode == MODE_GATEWAY)
				setRepeaterState(DSRS_TIMEOUT_WAIT);
			else
				setRepeaterState(DSRS_LISTENING);
			break;

		default:
			break;
	}
}

CSplitRepeaterStatusData* CSplitRepeaterThread::getStatus()
{
	float   bits = float(m_ambeBits - m_lastAMBEBits);
	float errors = float(m_ambeErrors - m_lastAMBEErrors);
	if (bits == 0.0F)
		bits = 1.0F;

	m_lastAMBEBits   = m_ambeBits;
	m_lastAMBEErrors = m_ambeErrors;

	if (m_state == DSRS_SHUTDOWN || m_state == DSRS_LISTENING)
		return new CSplitRepeaterStatusData(wxEmptyString, wxEmptyString, wxEmptyString, wxEmptyString,
				wxEmptyString, 0x00, 0x00, 0x00, m_state, m_timeoutTimer.getTimer(),
				m_timeoutTimer.getTimeout(), m_beaconTimer.getTimer(), m_beaconTimer.getTimeout(),
				m_announcementTimer.getTimer(), m_announcementTimer.getTimeout(), 0.0F,
				m_ackText, m_status1Text, m_status2Text, m_status3Text, m_status4Text, m_status5Text);
	else
		return new CSplitRepeaterStatusData(m_rxHeader->getMyCall1(), m_rxHeader->getMyCall2(),
				m_rxHeader->getYourCall(), m_rxHeader->getRptCall1(), m_rxHeader->getRptCall2(), 
				m_rxHeader->getFlag1(), m_rxHeader->getFlag2(), m_rxHeader->getFlag3(), m_state,
				m_timeoutTimer.getTimer(), m_timeoutTimer.getTimeout(), m_beaconTimer.getTimer(),
				m_beaconTimer.getTimeout(), m_announcementTimer.getTimer(), m_announcementTimer.getTimeout(),
				(errors * 100.0F) / bits, m_ackText, m_status1Text, m_status2Text, m_status3Text, m_status4Text,
				m_status5Text);
}

void CSplitRepeaterThread::clock(unsigned long ms)
{
	m_pollTimer.clock(ms);
	m_timeoutTimer.clock(ms);
	m_radio1WatchdogTimer.clock(ms);
	m_radio2WatchdogTimer.clock(ms);
	m_networkWatchdogTimer.clock(ms);
	m_ackTimer.clock(ms);
	m_status1Timer.clock(ms);
	m_status2Timer.clock(ms);
	m_status3Timer.clock(ms);
	m_status4Timer.clock(ms);
	m_status5Timer.clock(ms);
	m_beaconTimer.clock(ms);
	m_announcementTimer.clock(ms);
	if (m_beacon != NULL)
		m_beacon->clock();
	if (m_announcement != NULL)
		m_announcement->clock();
}

void CSplitRepeaterThread::shutdown()
{
	m_shutdown = true;
}

void CSplitRepeaterThread::startup()
{
	m_shutdown = false;
}

void CSplitRepeaterThread::command1()
{
	if (!m_controlCommand1Line.IsEmpty())
		::wxShell(m_controlCommand1Line);
}

void CSplitRepeaterThread::command2()
{
	if (!m_controlCommand2Line.IsEmpty())
		::wxShell(m_controlCommand2Line);
}

void CSplitRepeaterThread::command3()
{
	if (!m_controlCommand3Line.IsEmpty())
		::wxShell(m_controlCommand3Line);
}

void CSplitRepeaterThread::command4()
{
	if (!m_controlCommand4Line.IsEmpty())
		::wxShell(m_controlCommand4Line);
}

bool CSplitRepeaterThread::checkControl(const CSplitRepeaterHeaderData& header)
{
	if (!m_controlEnabled)
		return false;

	if (!m_controlRPT1.IsSameAs(header.getRptCall1()) || !m_controlRPT2.IsSameAs(header.getRptCall2()))
		return false;

	if (m_controlShutdown.IsSameAs(header.getYourCall())) {
		wxLogMessage(wxT("Shutdown requested by %s/%s"), header.getMyCall1().c_str(), header.getMyCall2().c_str());
		shutdown();
	} else if (m_controlStartup.IsSameAs(header.getYourCall())) {
		wxLogMessage(wxT("Startup requested by %s/%s"), header.getMyCall1().c_str(), header.getMyCall2().c_str());
		startup();
	} else if (m_controlStatus1.IsSameAs(header.getYourCall())) {
		wxLogMessage(wxT("Status 1 requested by %s/%s"), header.getMyCall1().c_str(), header.getMyCall2().c_str());
		m_status1Timer.start();
	} else if (m_controlStatus2.IsSameAs(header.getYourCall())) {
		wxLogMessage(wxT("Status 2 requested by %s/%s"), header.getMyCall1().c_str(), header.getMyCall2().c_str());
		m_status2Timer.start();
	} else if (m_controlStatus3.IsSameAs(header.getYourCall())) {
		wxLogMessage(wxT("Status 3 requested by %s/%s"), header.getMyCall1().c_str(), header.getMyCall2().c_str());
		m_status3Timer.start();
	} else if (m_controlStatus4.IsSameAs(header.getYourCall())) {
		wxLogMessage(wxT("Status 4 requested by %s/%s"), header.getMyCall1().c_str(), header.getMyCall2().c_str());
		m_status4Timer.start();
	} else if (m_controlStatus5.IsSameAs(header.getYourCall())) {
		wxLogMessage(wxT("Status 5 requested by %s/%s"), header.getMyCall1().c_str(), header.getMyCall2().c_str());
		m_status5Timer.start();
	} else if (m_controlCommand1.IsSameAs(header.getYourCall())) {
		wxLogMessage(wxT("Command 1 requested by %s/%s"), header.getMyCall1().c_str(), header.getMyCall2().c_str());
		command1();
	} else if (m_controlCommand2.IsSameAs(header.getYourCall())) {
		wxLogMessage(wxT("Command 2 requested by %s/%s"), header.getMyCall1().c_str(), header.getMyCall2().c_str());
		command2();
	} else if (m_controlCommand3.IsSameAs(header.getYourCall())) {
		wxLogMessage(wxT("Command 3 requested by %s/%s"), header.getMyCall1().c_str(), header.getMyCall2().c_str());
		command3();
	} else if (m_controlCommand4.IsSameAs(header.getYourCall())) {
		wxLogMessage(wxT("Command 4 requested by %s/%s"), header.getMyCall1().c_str(), header.getMyCall2().c_str());
		command4();
	} else {
		wxLogMessage(wxT("Invalid command of %s sent by %s/%s"), header.getYourCall().c_str(), header.getMyCall1().c_str(), header.getMyCall2().c_str());
	}

	return true;
}

bool CSplitRepeaterThread::checkAnnouncements(const CSplitRepeaterHeaderData& header)
{
	if (m_announcement == NULL)
		return false;

	if (m_recordRPT1.IsSameAs(header.getRptCall1()) && m_recordRPT2.IsSameAs(header.getRptCall2())) {
		wxLogMessage(wxT("Announcement creation requested by %s/%s"), header.getMyCall1().c_str(), header.getMyCall2().c_str());
		CHeaderData header2;
		header2.setFlag1(header.getFlag1());
		header2.setFlag2(header.getFlag2());
		header2.setFlag3(header.getFlag3());
		header2.setMyCall1(header.getMyCall1());
		header2.setMyCall2(header.getMyCall2());
		header2.setYourCall(header.getYourCall());
		header2.setRptCall1(header.getRptCall1());
		header2.setRptCall2(header.getRptCall2());
		m_announcement->writeHeader(header2);
		m_recording = true;
		return true;
	}

	if (m_deleteRPT1.IsSameAs(header.getRptCall1()) && m_deleteRPT2.IsSameAs(header.getRptCall2())) {
		wxLogMessage(wxT("Announcement deletion requested by %s/%s"), header.getMyCall1().c_str(), header.getMyCall2().c_str());
		m_announcement->deleteAnnouncement();
		m_deleting = true;
		return true;
	}

	return false;
}

TRISTATE CSplitRepeaterThread::checkHeader(CSplitRepeaterHeaderData& header)
{
	// If not in RPT1 validation mode, then a simplex header is converted to a proper repeater header
	if (!m_rpt1Validation) {
		if (!header.isRepeaterMode()) {
			// Convert to a properly addressed repeater packet
			header.setRepeaterMode(true);
			header.setRptCall1(m_rptCallsign);
			header.setRptCall2(m_gwyCallsign);
		}
	}

	// The repeater bit must be set when not in gateway mode
	if (m_mode != MODE_GATEWAY) {
		if (!header.isRepeaterMode()) {
			wxLogMessage(wxT("Received a non-repeater packet, ignoring"));
			return STATE_FALSE;
		}
	} else {
		// Quietly reject acks when in gateway mode
		if (header.isAck() || header.isNoResponse() || header.isRelayUnavailable())
			return STATE_UNKNOWN;

		// As a second check, reject on own UR call
		wxString ur = header.getYourCall();
		if (ur.IsSameAs(m_rptCallsign) || ur.IsSameAs(m_gwyCallsign))
			return STATE_UNKNOWN;

		// Change RPT2 to be the gateway callsign in gateway mode
		header.setRptCall2(m_gwyCallsign);
	}

	wxString my = header.getMyCall1();

	// Make sure MyCall is not empty, a silly value, or the repeater or gateway callsigns, STN* is a special case
	if (!my.Left(3U).IsSameAs(wxT("STN"))) {
		if (my.IsSameAs(m_rptCallsign) ||
			my.IsSameAs(m_gwyCallsign) ||
			my.IsSameAs(wxT("        ")) ||
			my.Left(6U).IsSameAs(wxT("NOCALL")) ||
			my.Left(6U).IsSameAs(wxT("N0CALL")) ||
			my.Left(6U).IsSameAs(wxT("MYCALL"))) {
			wxLogMessage(wxT("Invalid MYCALL value of %s, ignoring"), my.c_str());
			return STATE_UNKNOWN;
		}
	}

	// Check for a French class 3 novice callsign, and reject
	// Of the form F0xxx
	if (my.Left(2U).IsSameAs(wxT("F0"))) {
		wxLogMessage(wxT("French novice class licence callsign found, %s, ignoring"), my.c_str());
		return STATE_UNKNOWN;
	}

	// Check for an Australian foundation class licence callsign, and reject
	// Of the form VKnFxxx
	if (my.Left(2U).IsSameAs(wxT("VK")) && my.GetChar(3U) == wxT('F') && my.GetChar(6U) != wxT(' ')) {
		wxLogMessage(wxT("Australian foundation class licence callsign found, %s, ignoring"), my.c_str());
		return STATE_UNKNOWN;
	}

	// Check the MyCall value against the regular expression
	bool ok = m_regEx.Matches(my);
	if (!ok) {
		wxLogMessage(wxT("Invalid MYCALL value of %s, ignoring"), my.c_str());
		return STATE_UNKNOWN;
	}

	// Is it for us?
	if (!header.getRptCall1().IsSameAs(m_rptCallsign)) {
		wxLogMessage(wxT("Invalid RPT1 value %s, ignoring"), header.getRptCall1().c_str());
		return STATE_FALSE;
	}

	// If using callsign restriction, validate the my callsign
	if (m_restriction) {
		if (!my.Left(LONG_CALLSIGN_LENGTH - 1U).IsSameAs(m_rptCallsign.Left(LONG_CALLSIGN_LENGTH - 1U))) {
			wxLogMessage(wxT("Unauthorised user %s tried to access the repeater"), my.c_str());
			return STATE_UNKNOWN;
		}
	}

	return STATE_TRUE;
}

void CSplitRepeaterThread::clearQueue(unsigned int n)
{
	unsigned int i;

	switch (n) {
		case 1U:
			for (i = 0U; i < 21U; i++) {
				delete m_receiver1Data[i];
				m_receiver1Data[i] = NULL;
			}
			break;

		case 2U:
			for (i = 0U; i < 21U; i++) {
				delete m_receiver2Data[i];
				m_receiver2Data[i] = NULL;
			}
			break;

		default:
			wxLogWarning(wxT("Unknown queue to clear - %u"), n);
			break;
	}
}

void CSplitRepeaterThread::blankDTMF(CSplitRepeaterAMBEData& data)
{
	unsigned char ambe[DV_FRAME_LENGTH_BYTES];
	data.getData(ambe, DV_FRAME_LENGTH_BYTES);

	// DTMF begins with these byte values
	if ((ambe[0] & DTMF_MASK[0]) == DTMF_SIG[0] && (ambe[1] & DTMF_MASK[1]) == DTMF_SIG[1] &&
		(ambe[2] & DTMF_MASK[2]) == DTMF_SIG[2] && (ambe[3] & DTMF_MASK[3]) == DTMF_SIG[3] &&
		(ambe[4] & DTMF_MASK[4]) == DTMF_SIG[4] && (ambe[5] & DTMF_MASK[5]) == DTMF_SIG[5] &&
		(ambe[6] & DTMF_MASK[6]) == DTMF_SIG[6] && (ambe[7] & DTMF_MASK[7]) == DTMF_SIG[7] &&
		(ambe[8] & DTMF_MASK[8]) == DTMF_SIG[8])
		::memcpy(ambe, NULL_AMBE_DATA_BYTES, VOICE_FRAME_LENGTH_BYTES);

	data.setData(ambe, DV_FRAME_LENGTH_BYTES);
}
