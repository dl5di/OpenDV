/*
 *   Copyright (C) 2013 by Jonathan Naylor G4KLX
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

#ifndef	CCSHandler_H
#define	CCSHandler_H

#include "CCSProtocolHandler.h"
#include "RepeaterCallback.h"
#include "DStarDefines.h"
#include "HeaderLogger.h"
#include "ConnectData.h"
#include "AMBEData.h"
#include "PollData.h"
#include "Timer.h"
#include "Defs.h"

#if defined(__WINDOWS__)
#include "Inaddr.h"
#else
#include <netinet/in.h>
#endif

#include <wx/wx.h>
#include <wx/ffile.h>

class CCCSAudioIncoming {
public:
	CCCSAudioIncoming(unsigned int id, IRepeaterCallback* handler, bool busy) :
	m_id(id),
	m_handler(handler),
	m_busy(busy),
	m_timer(1000U, 2U)
	{
		wxASSERT(handler != NULL);

		m_timer.start();
	}

	~CCCSAudioIncoming()
	{
	}

	unsigned int       m_id;
	IRepeaterCallback* m_handler;
	bool               m_busy;
	CTimer             m_timer;
};

class CCCSAudioOutgoing {
public:
	CCCSAudioOutgoing(IRepeaterCallback* handler) :
	m_handler(handler),
	m_seqNo(0U),
	m_rptCall1(),
	m_rptCall2(),
	m_yourCall(),
	m_myCall1(),
	m_myCall2()
	{
		wxASSERT(handler != NULL);
	}

	~CCCSAudioOutgoing()
	{
	}

	IRepeaterCallback* m_handler;
	unsigned int       m_seqNo;
	wxString           m_rptCall1;
	wxString           m_rptCall2;
	wxString           m_yourCall;
	wxString           m_myCall1;
	wxString           m_myCall2;
};

class CCCSHandler {
public:
	static void initialise();

	static void setLocation(double latitude, double longitude);
	static void setCCSProtocolHandler(CCCSProtocolHandler* handler);
	static void setHeaderLogger(CHeaderLogger* logger);
	static void setCallsign(const wxString& callsign);

	static void addRepeater(IRepeaterCallback* handler);

	static bool connect();
	static void disconnect();

	static void writeHeard(const CHeaderData& data, const wxString& repeater, const wxString& reflector = wxEmptyString);
	static void writeEnd(const wxString& local, const wxString& remote);
	static void writeHeader(IRepeaterCallback* handler, CHeaderData& header);
	static void writeAMBE(IRepeaterCallback* handler, CAMBEData& data);

	static void process(CAMBEData& header);
	static void process(CPollData& data);
	static void process(CConnectData& connect);

	static void clock(unsigned int ms);

	static void finalise();

	static CCS_STATUS getState();

private:
	static CCS_STATUS               m_state;

	static wxString                 m_callsign;

	static CCCSProtocolHandler*     m_handler;

	static in_addr                  m_address;

	static CHeaderLogger*           m_headerLogger;

	static CTimer                   m_pollInactivityTimer;
	static CTimer                   m_pollTimer;
	static CTimer                   m_tryTimer;
	static unsigned int             m_tryCount;

	static wxString                 m_locator;

	static CCCSAudioIncoming**      m_incoming;
	static CCCSAudioOutgoing**      m_outgoing;

	static unsigned int calcBackoff();
};

#endif
