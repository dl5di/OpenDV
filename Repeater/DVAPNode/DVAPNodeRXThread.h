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

#ifndef	DVAPNodeRXThread_H
#define	DVAPNodeRXThread_H

#include "DVAPNodeThread.h"
#include "HeaderData.h"
#include "AMBEFEC.h"
#include "Timer.h"
#include "Utils.h"

#include <wx/wx.h>
#include <wx/regex.h>

class CDVAPNodeRXThread : public IDVAPNodeThread {
public:
	CDVAPNodeRXThread();
	virtual ~CDVAPNodeRXThread();

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
	CHeaderData*               m_rxHeader;
	unsigned char              m_radioSeqNo;
	CTimer                     m_pollTimer;
	CTimer                     m_dvapPollTimer;
	DSTAR_RPT_STATE            m_state;
	bool                       m_squelch;
	int                        m_signal;
	bool                       m_killed;
	CAMBEFEC                   m_ambe;
	unsigned int               m_ambeFrames;
	unsigned int               m_ambeSilence;
	unsigned int               m_ambeBits;
	unsigned int               m_ambeErrors;
	unsigned int               m_lastAMBEBits;
	unsigned int               m_lastAMBEErrors;

	void receiveRadioHeader();
	void receiveRadioData();

	void receiveNetwork();
	bool processRadioHeader(CHeaderData* header);
	void processRadioFrame(unsigned char* data, FRAME_TYPE type);
	void endOfRadioData();
	bool setRepeaterState(DSTAR_RPT_STATE state);
	void clock(unsigned int ms);
};

#endif
