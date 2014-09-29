/*
 *   Copyright (C) 2010,2011,2012,2013 by Jonathan Naylor G4KLX
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

#ifndef	DPlusHandler_H
#define	DPlusHandler_H

#include "DPlusProtocolHandlerPool.h"
#include "DPlusAuthenticator.h"
#include "ReflectorCallback.h"
#include "CacheManager.h"
#include "DStarDefines.h"
#include "HeaderLogger.h"
#include "ConnectData.h"
#include "HeaderData.h"
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

enum DPLUS_STATE {
	DPLUS_LINKING,
	DPLUS_LINKED,
	DPLUS_UNLINKING
};

class CDPlusHandler {
public:
	static void initialise(unsigned int maxReflectors);

	static void setCallsign(const wxString& callsign);
	static void setDPlusProtocolHandlerPool(CDPlusProtocolHandlerPool* pool);
	static void setDPlusProtocolIncoming(CDPlusProtocolHandler* handler);
	static void setDPlusLogin(const wxString& dplusLogin);
	static void setHeaderLogger(CHeaderLogger* logger);
	static void setMaxDongles(unsigned int maxDongles);

	static void startAuthenticator(const wxString& address, CCacheManager* cache);

	static void link(IReflectorCallback* handler, const wxString& repeater, const wxString& reflector, const in_addr& address);
	static void relink(IReflectorCallback* handler, const wxString& reflector);
	static void unlink(IReflectorCallback* handler, const wxString& exclude = wxEmptyString);
	static void unlink();

	static void writeHeader(IReflectorCallback* handler, CHeaderData& header, DIRECTION direction);
	static void writeAMBE(IReflectorCallback* handler, CAMBEData& data, DIRECTION direction);

	static void process(CHeaderData& header);
	static void process(CAMBEData& header);
	static void process(const CPollData& header);
	static void process(CConnectData& process);

	static void gatewayUpdate(const wxString& gateway, const wxString& address);
	static void clock(unsigned int ms);

	static bool stateChange();
	static void writeStatus(wxFFile& file);

	static void finalise();

	static void getInfo(IReflectorCallback* handler, CRemoteRepeaterData& data);

	static wxString getDongles();

protected:
	CDPlusHandler(IReflectorCallback* handler, const wxString& repeater, const wxString& reflector, CDPlusProtocolHandler* protoHandler, const in_addr& address, unsigned int port);
	CDPlusHandler(CDPlusProtocolHandler* protoHandler, const in_addr& address, unsigned int port);
	~CDPlusHandler();

	void processInt(CHeaderData& header);
	void processInt(CAMBEData& data);
	bool processInt(CConnectData& connect, CD_TYPE type);

	void writeHeaderInt(IReflectorCallback* handler, CHeaderData& header, DIRECTION direction);
	void writeAMBEInt(IReflectorCallback* handler, CAMBEData& data, DIRECTION direction);

	bool clockInt(unsigned int ms);

private:
	static unsigned int               m_maxReflectors;
	static unsigned int               m_maxDongles;
	static CDPlusHandler**            m_reflectors;

	static wxString                   m_gatewayCallsign;
	static wxString                   m_dplusLogin;
	static CDPlusProtocolHandlerPool* m_pool;
	static CDPlusProtocolHandler*     m_incoming;

	static bool                       m_stateChange;

	static CHeaderLogger*             m_headerLogger;
	static CDPlusAuthenticator*       m_authenticator;

	wxString               m_repeater;
	wxString               m_callsign;
	wxString               m_reflector;
	CDPlusProtocolHandler* m_handler;
	in_addr                m_yourAddress;
	unsigned int           m_yourPort;
	unsigned int           m_myPort;
	DIRECTION              m_direction;
	DPLUS_STATE            m_linkState;
	IReflectorCallback*    m_destination;
	time_t                 m_time;
	CTimer                 m_pollTimer;
	CTimer                 m_pollInactivityTimer;
	CTimer                 m_tryTimer;
	unsigned int           m_tryCount;
	unsigned int           m_dPlusId;
	unsigned int           m_dPlusSeq;
	CTimer                 m_inactivityTimer;
	CHeaderData*           m_header;

	unsigned int calcBackoff();
};

#endif
