/*
 *   Copyright (C) 2011-2015 by Jonathan Naylor G4KLX
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

#ifndef	DStarRepeaterTRXThread_H
#define	DStarRepeaterTRXThread_H

#include "DStarRepeaterStatusData.h"
#include "RepeaterProtocolHandler.h"
#include "AnnouncementCallback.h"
#include "DStarRepeaterThread.h"
#include "DStarRepeaterWindowsefs.h"
#include "DVTOOLFileWriter.h"
#include "AnnouncementUnit.h"
#include "SlowDataDecoder.h"
#include "SlowDataEncoder.h"
#include "BeaconCallback.h"
#include "CallsignList.h"
#include "OutputQueue.h"
#include "BeaconUnit.h"
#include "HeaderData.h"
#include "AMBEFEC.h"
#include "Timer.h"
#include "Utils.h"

#include <string>

class CDStarRepeaterTRXThread : public IDStarRepeaterThread, public IBeaconCallback, public IAnnouncementCallback {
public:
	CDStarRepeaterTRXThread(const std::string& type);
	virtual ~CDStarRepeaterTRXThread();

	virtual void setCallsign(const std::string& callsign, const std::string& gateway, DSTAR_MODE mode, ACK_TYPE ack, bool restriction, bool rpt1Validation, bool dtmfBlanking, bool errorReply);
	virtual void setProtocolHandler(CRepeaterProtocolHandler* handler, bool local);
	virtual void setModem(CModem* modem);
	virtual void setController(CExternalController* controller, unsigned int activeHangTime);
	virtual void setTimes(unsigned int timeout, unsigned int ackTime);
	virtual void setBeacon(unsigned int time, const std::string& text, bool voice, TEXT_LANG language);
	virtual void setAnnouncement(bool enabled, unsigned int time, const std::string& recordRPT1, const std::string& recordRPT2, const std::string& deleteRPT1, const std::string& deleteRPT2);
	virtual void setControl(bool enabled, const std::string& rpt1Callsign, const std::string& rpt2Callsign, const std::string& shutdown, const std::string& startup, const std::string& status1, const std::string& status2, const std::string& status3, const std::string& status4, const std::string& status5, const std::string& command1, const std::string& command1Line, const std::string& command2, const std::string& command2Line, const std::string& command3, const std::string& command3Line, const std::string& command4, const std::string& command4Line, const std::string& command5, const std::string& command5Line, const std::string& command6, const std::string& command6Line, const std::string& output1, const std::string& output2, const std::string& output3, const std::string& output4);
	virtual void setOutputs(bool out1, bool out2, bool out3, bool out4);
	virtual void setLogging(bool logging, const std::string& dir);
	virtual void setWhiteList(CCallsignList* list);
	virtual void setBlackList(CCallsignList* list);
	virtual void setGreyList(CCallsignList* list);

	virtual void shutdown();
	virtual void startup();

	virtual void command1();
	virtual void command2();
	virtual void command3();
	virtual void command4();
	virtual void command5();
	virtual void command6();

	virtual CDStarRepeaterStatusData* getStatus();

	virtual void run();
	virtual void kill();

	virtual void transmitBeaconHeader();
	virtual void transmitBeaconData(const unsigned char* data, unsigned int length, bool end);

	virtual void transmitAnnouncementHeader(CHeaderData* header);
	virtual void transmitAnnouncementData(const unsigned char* data, unsigned int length, bool end);

private:
	std::string                m_type;
	CModem*                    m_modem;
	CRepeaterProtocolHandler*  m_protocolHandler;
	CExternalController*       m_controller;
	bool                       m_stopped;
	std::string                m_rptCallsign;
	std::string                m_gwyCallsign;
	CBeaconUnit*               m_beacon;
	CAnnouncementUnit*         m_announcement;
	std::string                m_recordRPT1;
	std::string                m_recordRPT2;
	std::string                m_deleteRPT1;
	std::string                m_deleteRPT2;
	CHeaderData*               m_rxHeader;
	COutputQueue               m_localQueue;
	COutputQueue               m_radioQueue;
	COutputQueue**             m_networkQueue;
	unsigned int               m_writeNum;
	unsigned int               m_readNum;
	unsigned char              m_radioSeqNo;
	unsigned char              m_networkSeqNo;
	CTimer                     m_timeoutTimer;
	CTimer                     m_watchdogTimer;
	CTimer                     m_pollTimer;
	CTimer                     m_ackTimer;
	CTimer                     m_status1Timer;
	CTimer                     m_status2Timer;
	CTimer                     m_status3Timer;
	CTimer                     m_status4Timer;
	CTimer                     m_status5Timer;
	CTimer                     m_beaconTimer;
	CTimer                     m_announcementTimer;
	CTimer                     m_statusTimer;
	CTimer                     m_heartbeatTimer;
	DSTAR_RPT_STATE            m_rptState;
	DSTAR_RX_STATE             m_rxState;
	CSlowDataDecoder           m_slowDataDecoder;
	CSlowDataEncoder           m_ackEncoder;
	CSlowDataEncoder           m_linkEncoder;
	CSlowDataEncoder           m_headerEncoder;
	CSlowDataEncoder           m_status1Encoder;
	CSlowDataEncoder           m_status2Encoder;
	CSlowDataEncoder           m_status3Encoder;
	CSlowDataEncoder           m_status4Encoder;
	CSlowDataEncoder           m_status5Encoder;
	bool                       m_tx;
	unsigned int               m_space;
	bool                       m_killed;
	DSTAR_MODE                 m_mode;
	ACK_TYPE                   m_ack;
	bool                       m_restriction;
	bool                       m_rpt1Validation;
	bool                       m_errorReply;
	bool                       m_controlEnabled;
	std::string                m_controlRPT1;
	std::string                m_controlRPT2;
	std::string                m_controlShutdown;
	std::string                m_controlStartup;
	std::string                m_controlStatus1;
	std::string                m_controlStatus2;
	std::string                m_controlStatus3;
	std::string                m_controlStatus4;
	std::string                m_controlStatus5;
	std::string                m_controlCommand1;
	std::string                m_controlCommand1Line;
	std::string                m_controlCommand2;
	std::string                m_controlCommand2Line;
	std::string                m_controlCommand3;
	std::string                m_controlCommand3Line;
	std::string                m_controlCommand4;
	std::string                m_controlCommand4Line;
	std::string                m_controlCommand5;
	std::string                m_controlCommand5Line;
	std::string                m_controlCommand6;
	std::string                m_controlCommand6Line;
	std::string                m_controlOutput1;
	std::string                m_controlOutput2;
	std::string                m_controlOutput3;
	std::string                m_controlOutput4;
	bool                       m_output1;
	bool                       m_output2;
	bool                       m_output3;
	bool                       m_output4;
	CTimer                     m_activeHangTimer;
	bool                       m_shutdown;
	bool                       m_disable;
	CDVTOOLFileWriter*         m_logging;
	unsigned char*             m_lastData;
	CAMBEFEC                   m_ambe;
	unsigned int               m_ambeFrames;
	unsigned int               m_ambeSilence;
	unsigned int               m_ambeBits;
	unsigned int               m_ambeErrors;
	unsigned int               m_lastAMBEBits;
	unsigned int               m_lastAMBEErrors;
	std::string                m_ackText;
	std::string                m_tempAckText;
	LINK_STATUS                m_linkStatus;
	std::string                m_reflector;
	std::string                m_status1Text;
	std::string                m_status2Text;
	std::string                m_status3Text;
	std::string                m_status4Text;
	std::string                m_status5Text;
	wxRegEx                    m_regEx;
	CStopWatch                 m_headerTime;
	CStopWatch                 m_packetTime;
	unsigned int               m_packetCount;
	unsigned int               m_packetSilence;
	CCallsignList*             m_whiteList;
	CCallsignList*             m_blackList;
	CCallsignList*             m_greyList;
	bool                       m_blocked;
	bool                       m_busyData;
	bool                       m_blanking;
	bool                       m_recording;
	bool                       m_deleting;

	void receiveHeader(CHeaderData* header);
	void receiveRadioData(unsigned char* data, unsigned int length);
	void receiveSlowData(unsigned char* data, unsigned int length);
	void transmitRadioHeader(CHeaderData* header);
	void transmitLocalHeader(CHeaderData* header);
	void transmitNetworkHeader(CHeaderData* header);
	void transmitStatus();
	void transmitErrorStatus();
	void transmitUserStatus(unsigned int n);
	void transmitLocalData();
	void transmitRadioData();
	void transmitNetworkData();
	void transmitLocalHeader();
	void transmitRadioHeader();
	void transmitNetworkHeader();

	void repeaterStateMachine();
	void receiveModem();
	void receiveNetwork();
	bool processRadioHeader(CHeaderData* header);
	void processNetworkHeader(CHeaderData* header);
	void processRadioFrame(unsigned char* data, FRAME_TYPE type);
	unsigned int processNetworkFrame(unsigned char* data, unsigned int length, unsigned char seqNo);
	void endOfRadioData();
	void endOfNetworkData();
	void setRadioState(DSTAR_RX_STATE state);
	bool setRepeaterState(DSTAR_RPT_STATE state);
	bool checkControl(const CHeaderData& header);
	bool checkAnnouncements(const CHeaderData& header);
	TRISTATE checkHeader(CHeaderData& header);
	unsigned int countBits(unsigned char byte);
	void clock(unsigned int ms);
	void blankDTMF(unsigned char* data);
};

#endif
