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

#ifndef	DVAPNodeTXThread_H
#define	DVAPNodeTXThread_H

#include "DVAPNodeThread.h"
#include "OutputQueue.h"
#include "HeaderData.h"
#include "AMBEFEC.h"
#include "Timer.h"
#include "Utils.h"

#include <wx/wx.h>

class CDVAPNodeTXThread : public IDVAPNodeThread {
public:
	CDVAPNodeTXThread();
	virtual ~CDVAPNodeTXThread();

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

private:
	CDVAPController*           m_dvap;
	CRepeaterProtocolHandler*  m_protocolHandler;
	wxString                   m_rptCallsign;
	CHeaderData*               m_txHeader;
	COutputQueue**             m_networkQueue;
	unsigned int               m_writeNum;
	unsigned int               m_readNum;
	unsigned char              m_networkSeqNo;
	CTimer                     m_watchdogTimer;
	CTimer                     m_pollTimer;
	CTimer                     m_dvapPollTimer;
	DSTAR_RPT_STATE            m_state;
	bool                       m_tx;
	bool                       m_killed;
	unsigned char*             m_lastData;
	CAMBEFEC                   m_ambe;
	wxStopWatch                m_headerTime;
	wxStopWatch                m_packetTime;
	unsigned int               m_packetCount;
	unsigned int               m_packetSilence;

	void transmitNetworkHeader(CHeaderData* header);
	void transmitNetworkData();
	void transmitNetworkHeader();

	void receiveNetwork();
	void processNetworkHeader(CHeaderData* header);
	unsigned int processNetworkFrame(unsigned char* data, unsigned int length, unsigned char seqNo);
	void endOfNetworkData();
	void clock(unsigned int ms);
};

#endif
