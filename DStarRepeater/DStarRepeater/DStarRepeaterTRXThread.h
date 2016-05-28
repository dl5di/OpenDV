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
#include "DStarRepeaterDefs.h"
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

#include <wx/wx.h>
#include <wx/regex.h>

class CDStarRepeaterTRXThread : public IDStarRepeaterThread, public IBeaconCallback, public IAnnouncementCallback {
public:
	CDStarRepeaterTRXThread(const wxString& type);
	virtual ~CDStarRepeaterTRXThread();

	virtual void setCallsign(const wxString& callsign, const wxString& gateway, DSTAR_MODE mode, ACK_TYPE ack, bool restriction, bool rpt1Validation, bool dtmfBlanking, bool errorReply);
	virtual void setProtocolHandler(CRepeaterProtocolHandler* handler, bool local);
	virtual void setModem(CModem* modem);
	virtual void setController(CExternalController* controller, unsigned int activeHangTime);
	virtual void setTimes(unsigned int timeout, unsigned int ackTime);
	virtual void setBeacon(unsigned int time, const wxString& text, bool voice, TEXT_LANG language);
	virtual void setAnnouncement(bool enabled, unsigned int time, const wxString& recordRPT1, const wxString& recordRPT2, const wxString& deleteRPT1, const wxString& deleteRPT2);

	virtual void setControl(bool enabled, const wxString& rpt1Callsign,
		const wxString& rpt2Callsign, const wxString& shutdown,
		const wxString& startup, const wxArrayString& command,
		const wxArrayString& status, const wxArrayString& outputs
	);

	virtual void setOutputs(bool out1, bool out2, bool out3, bool out4);
	virtual void setLogging(bool logging, const wxString& dir);
	virtual void setWhiteList(CCallsignList* list);
	virtual void setBlackList(CCallsignList* list);
	virtual void setGreyList(CCallsignList* list);

	virtual void shutdown();
	virtual void startup();

	virtual CDStarRepeaterStatusData* getStatus();

	virtual void *Entry();

	virtual void kill();

	virtual void transmitBeaconHeader();
	virtual void transmitBeaconData(const unsigned char* data, unsigned int length, bool end);

	virtual void transmitAnnouncementHeader(CHeaderData* header);
	virtual void transmitAnnouncementData(const unsigned char* data, unsigned int length, bool end);

private:
	wxString                   m_type;
	CModem*                    m_modem;
	CRepeaterProtocolHandler*  m_protocolHandler;
	CExternalController*       m_controller;
	bool                       m_stopped;
	wxString                   m_rptCallsign;
	wxString                   m_gwyCallsign;
	CBeaconUnit*               m_beacon;
	CAnnouncementUnit*         m_announcement;
	wxString                   m_recordRPT1;
	wxString                   m_recordRPT2;
	wxString                   m_deleteRPT1;
	wxString                   m_deleteRPT2;
	CHeaderData*               m_rxHeader;
	COutputQueue               m_localQueue;
	COutputQueue               m_radioQueue;
	COutputQueue**             m_networkQueue;
	unsigned int               m_writeNum;
	unsigned int               m_readNum;
	unsigned char              m_radioSeqNo;
	unsigned char              m_networkSeqNo;
	unsigned char              m_lastSlowDataType;
	CTimer                     m_timeoutTimer;
	CTimer                     m_watchdogTimer;
	CTimer                     m_pollTimer;
	CTimer                     m_ackTimer;

	CTimer                     m_statusAnnounceTimer[5];

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

	// XXX ARRAY!
	CSlowDataEncoder           m_status1Encoder;
	CSlowDataEncoder           m_status2Encoder;
	CSlowDataEncoder           m_status3Encoder;
	CSlowDataEncoder           m_status4Encoder;
	CSlowDataEncoder           m_status5Encoder;

	// XXX ARRAY!
	wxArrayString		   m_statusText;

	bool                       m_tx;
	unsigned int               m_space;
	bool                       m_killed;
	DSTAR_MODE                 m_mode;
	ACK_TYPE                   m_ack;
	bool                       m_restriction;
	bool                       m_rpt1Validation;
	bool                       m_errorReply;
	bool                       m_controlEnabled;
	wxString                   m_controlRPT1;
	wxString                   m_controlRPT2;
	wxString                   m_controlShutdown;
	wxString                   m_controlStartup;

	wxArrayString              m_controlStatus;
	wxArrayString              m_controlCommand;

	wxArrayString		   m_controlOutput;

	bool			   m_output[4];

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
	wxString                   m_ackText;
	wxString                   m_tempAckText;
	LINK_STATUS                m_linkStatus;
	wxString                   m_reflector;

	wxRegEx                    m_regEx;
	wxStopWatch                m_headerTime;
	wxStopWatch                m_packetTime;
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
