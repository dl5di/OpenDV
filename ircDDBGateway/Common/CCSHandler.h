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
#include "DStarDefines.h"
#include "HeaderLogger.h"
#include "ConnectData.h"
#include "CCSCallback.h"
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

enum CCS_STATUS {
	CS_DISABLED,
	CS_CONNECTING,
	CS_CONNECTED,
	CS_ACTIVE
};

WX_DECLARE_STRING_HASH_MAP(wxString, CCCSCache_t);

class CCCSHandler {
public:
	CCCSHandler(ICCSCallback* handler, const wxString& callsign, unsigned int delay, double latitude, double longitude, double frequency, double offset, const wxString& description1, const wxString& description2, const wxString& url, unsigned int localPort);
	~CCCSHandler();

	bool connect();

	void writeHeard(CHeaderData& header);
	void writeHeader(CHeaderData& header);
	void writeAMBE(CAMBEData& data);

	void startLink(const wxString& dtmf, const wxString& user, const wxString& type);
	void stopLink(const wxString& user = wxEmptyString, const wxString& type = wxEmptyString);

	void unlink(const wxString& callsign);

	void setReflector(const wxString& callsign = wxEmptyString);

	CCS_STATUS getStatus() const;

	static void disconnect();

	static void initialise(unsigned int count);

	static void process();

	static void clock(unsigned int ms);

	static void setHeaderLogger(CHeaderLogger* logger);

	static void setLocalAddress(const wxString& address);

	static void setHost(const wxString& host);

	static bool stateChange();
	static void writeStatus(wxFFile& file);

	static void getInfo(ICCSCallback* handler, CRemoteRepeaterData& data);

	static wxString getIncoming(const wxString& callsign);

	static void finalise();

protected:
	void clockInt(unsigned int ms);

	void processInt();

	void disconnectInt();

private:
	static CCCSHandler**  m_handlers;
	static unsigned int   m_count;

	static wxString       m_localAddress;
	static CHeaderLogger* m_headerLogger;

	static wxString       m_ccsHost;

	static CCCSCache_t    m_cache;
	static wxMutex        m_mutex;

	static bool           m_stateChange;

	ICCSCallback*       m_handler;
	wxString            m_callsign;
	wxString            m_reflector;
	double              m_latitude;
	double              m_longitude;
	double              m_frequency;
	double              m_offset;
	wxString            m_description1;
	wxString            m_description2;
	wxString            m_url;
	in_addr             m_ccsAddress;
	CCCSProtocolHandler m_protocol;
	CCS_STATUS          m_state;
	wxString            m_local;
	CTimer              m_announceTimer;
	CTimer              m_inactivityTimer;
	CTimer              m_pollInactivityTimer;
	CTimer              m_pollTimer;
	CTimer              m_waitTimer;
	CTimer              m_tryTimer;
	unsigned int        m_tryCount;
	unsigned int        m_id;
	unsigned int        m_seqNo;
	time_t              m_time;
	DIRECTION           m_direction;
	wxString            m_yourCall;
	wxString            m_myCall1;
	wxString            m_myCall2;
	wxString            m_rptCall1;

	void process(CAMBEData& header);
	void process(CPollData& data);
	void process(CConnectData& connect);
	void process(CCCSData& data);

	unsigned int calcBackoff();

	static void     addToCache(const wxString& dtmf, const wxString& callsign);
	static wxString findInCache(const wxString& dtmf);
};

#endif
