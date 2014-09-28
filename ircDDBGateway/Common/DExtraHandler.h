/*
 *   Copyright (C) 2010-2013 by Jonathan Naylor G4KLX
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

#ifndef	DExtraHander_H
#define	DExtraHander_H

#include "DExtraProtocolHandlerPool.h"
#include "ReflectorCallback.h"
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

enum DEXTRA_STATE {
	DEXTRA_LINKING,
	DEXTRA_LINKED,
	DEXTRA_UNLINKING
};

class CDExtraHandler {
public:
	static void initialise(unsigned int maxReflectors);

	static void setCallsign(const wxString& callsign);
	static void setDExtraProtocolHandlerPool(CDExtraProtocolHandlerPool* pool);
	static void setDExtraProtocolIncoming(CDExtraProtocolHandler* handler);
	static void setHeaderLogger(CHeaderLogger* logger);
	static void setMaxDongles(unsigned int maxDongles);

	static void link(IReflectorCallback* handler, const wxString& repeater, const wxString& reflector, const in_addr& address);
	static void unlink(IReflectorCallback* handler, const wxString& reflector = wxEmptyString, bool exclude = true);
	static void unlink();

	static void writeHeader(IReflectorCallback* handler, CHeaderData& header, DIRECTION direction);
	static void writeAMBE(IReflectorCallback* handler, CAMBEData& data, DIRECTION direction);

	static void process(CHeaderData& header);
	static void process(CAMBEData& data);
	static void process(const CPollData& poll);
	static void process(CConnectData& connect);

	static void gatewayUpdate(const wxString& reflector, const wxString& address);
	static void clock(unsigned int ms);

	static bool stateChange();
	static void writeStatus(wxFFile& file);

	static void finalise();

	static void getInfo(IReflectorCallback* handler, CRemoteRepeaterData& data);

	static wxString getIncoming(const wxString& callsign);
	static wxString getDongles();

protected:
	CDExtraHandler(IReflectorCallback* handler, const wxString& reflector, const wxString& repeater, CDExtraProtocolHandler* protoHandler, const in_addr& address, unsigned int port, DIRECTION direction);
	CDExtraHandler(CDExtraProtocolHandler* protoHandler, const wxString& reflector, const in_addr& address, unsigned int port, DIRECTION direction);
	~CDExtraHandler();

	void processInt(CHeaderData& header);
	void processInt(CAMBEData& data);
	bool processInt(CConnectData& connect, CD_TYPE type);

	void writeHeaderInt(IReflectorCallback* handler, CHeaderData& header, DIRECTION direction);
	void writeAMBEInt(IReflectorCallback* handler, CAMBEData& data, DIRECTION direction);

	bool clockInt(unsigned int ms);

private:
	static unsigned int                m_maxReflectors;
	static unsigned int                m_maxDongles;
	static CDExtraHandler**            m_reflectors;

	static wxString                    m_callsign;
	static CDExtraProtocolHandlerPool* m_pool;
	static CDExtraProtocolHandler*     m_incoming;

	static bool                        m_stateChange;

	static CHeaderLogger*              m_headerLogger;

	wxString                m_reflector;
	wxString                m_repeater;
	CDExtraProtocolHandler* m_handler;
	in_addr                 m_yourAddress;
	unsigned int            m_yourPort;
	DIRECTION               m_direction;
	DEXTRA_STATE            m_linkState;
	IReflectorCallback*     m_destination;
	time_t                  m_time;
	CTimer                  m_pollTimer;
	CTimer                  m_pollInactivityTimer;
	CTimer                  m_tryTimer;
	unsigned int            m_tryCount;
	unsigned int            m_dExtraId;
	unsigned int            m_dExtraSeq;
	CTimer                  m_inactivityTimer;
	CHeaderData*            m_header;

	unsigned int calcBackoff();
};

#endif
