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

#ifndef	DVAPNodeTRXThread_H
#define	DVAPNodeTRXThread_H

#include "AnnouncementCallback.h"
#include "AnnouncementUnit.h"
#include "DVTOOLFileWriter.h"
#include "SlowDataEncoder.h"
#include "DVAPNodeThread.h"
#include "BeaconCallback.h"
#include "OutputQueue.h"
#include "HeaderData.h"
#include "BeaconUnit.h"
#include "AMBEFEC.h"
#include "Timer.h"
#include "Utils.h"

#include <wx/wx.h>
#include <wx/regex.h>

class CDVAPNodeTRXThread : public IDVAPNodeThread, public IBeaconCallback, public IAnnouncementCallback {
public:
	CDVAPNodeTRXThread();
	virtual ~CDVAPNodeTRXThread();

	virtual void setCallsign(const wxString& callsign, const wxString& gateway, DSTAR_MODE mode, ACK_TYPE ack, bool restriction, bool rpt1Validation);
	virtual void setProtocolHandler(CRepeaterProtocolHandler* handler);
	virtual void setDVAP(CDVAPController* dvap);
	virtual void setTimes(unsigned int timeout, unsigned int ackTime);
	virtual void setBeacon(unsigned int time, const wxString& text, bool voice, TEXT_LANG language);
	virtual void setAnnouncement(bool enabled, unsigned int time, const wxString& recordRPT1, const wxString& recordRPT2, const wxString& deleteRPT1, const wxString& deleteRPT2);
	virtual void setLogging(bool logging, const wxString& dir);
	virtual void setBlackList(CCallsignList* list);
	virtual void setGreyList(CCallsignList* list);

	virtual CDVAPNodeStatusData* getStatus();

	virtual void run();
	virtual void kill();

	virtual void transmitBeaconHeader();
	virtual void transmitBeaconData(const unsigned char* data, unsigned int length, bool end);

	virtual void transmitAnnouncementHeader(CHeaderData* header);
	virtual void transmitAnnouncementData(const unsigned char* data, unsigned int length, bool end);

private:
	CDVAPController*           m_dvap;
	CRepeaterProtocolHandler*  m_protocolHandler;
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
	COutputQueue**             m_networkQueue;
	unsigned int               m_writeNum;
	unsigned int               m_readNum;
	unsigned char              m_radioSeqNo;
	unsigned char              m_networkSeqNo;
	CTimer                     m_timeoutTimer;
	CTimer                     m_watchdogTimer;
	CTimer                     m_pollTimer;
	CTimer                     m_ackTimer;
	CTimer                     m_beaconTimer;
	CTimer                     m_announcementTimer;
	CTimer                     m_dvapPollTimer;
	DSTAR_RPT_STATE            m_state;
	CSlowDataEncoder           m_ackEncoder;
	CSlowDataEncoder           m_linkEncoder;
	bool                       m_tx;
	bool                       m_squelch;
	int                        m_signal;
	bool                       m_killed;
	DSTAR_MODE                 m_mode;
	ACK_TYPE                   m_ack;
	bool                       m_restriction;
	bool                       m_rpt1Validation;
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
	CCallsignList*             m_blackList;
	CCallsignList*             m_greyList;
	bool                       m_blocked;
	bool                       m_recording;
	bool                       m_deleting;

	void receiveRadioHeader();
	void receiveRadioData();
	void transmitLocalHeader(CHeaderData* header);
	void transmitNetworkHeader(CHeaderData* header);
	void transmitStatus();
	void transmitErrorStatus();
	void transmitLocalData();
	void transmitNetworkData();
	void transmitLocalHeader();
	void transmitNetworkHeader();

	void repeaterStateMachine();
	void receiveNetwork();
	bool processRadioHeader(CHeaderData* header);
	void processNetworkHeader(CHeaderData* header);
	void processRadioFrame(unsigned char* data, FRAME_TYPE type);
	unsigned int processNetworkFrame(unsigned char* data, unsigned int length, unsigned char seqNo);
	void endOfRadioData();
	void endOfNetworkData();
	bool setRepeaterState(DSTAR_RPT_STATE state);
	bool checkAnnouncements(const CHeaderData& header);
	TRISTATE checkHeader(CHeaderData& header);
	void clock(unsigned int ms);
	void blankDTMF(unsigned char* data);
};

#endif
