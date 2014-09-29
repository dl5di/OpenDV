/*
 *   Copyright (C) 2010,2011,2012 by Jonathan Naylor G4KLX
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

#ifndef	XReflectorDExtraHandler_H
#define	XReflectorDExtraHandler_H

#include "XReflectorReflectorCallback.h"
#include "DExtraProtocolHandler.h"
#include "XReflectorUserLog.h"
#include "DStarDefines.h"
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

class CXReflectorDExtraHandler {
public:
	static void initialise(unsigned int maxReflectors);

	static void setCallsign(const wxString& callsign);
	static void setDExtraProtocolHandler(CDExtraProtocolHandler* handler);
	static void setUserLogger(CXReflectorUserLog* logger);

	static void unlink();

	static void writeHeader(const wxString& callsign, CHeaderData& header);
	static void writeAMBE(CAMBEData& data);

	static void writeDongleHeader(CHeaderData& header);
	static void writeDongleAMBE(CAMBEData& data);

	static void process(CHeaderData& header);
	static void process(CAMBEData& header);
	static void process(const CPollData& header);
	static void process(CConnectData& process);

	static void clock(unsigned long ms);

	static bool stateChange();
	static void writeStatus(wxFFile& file);

	static void finalise();

protected:
	CXReflectorDExtraHandler(IXReflectorReflectorCallback* handler, const wxString& reflector, const wxString& repeater, const in_addr& address, unsigned int port);
	CXReflectorDExtraHandler(const wxString& reflector, const in_addr& address, unsigned int port);
	~CXReflectorDExtraHandler();

	void processInt(CHeaderData& header);
	void processInt(CAMBEData& data);
	bool processInt(CConnectData& connect, CD_TYPE type);

	void writeHeaderInt(const wxString& callsign, CHeaderData& header);
	void writeDongleHeaderInt(CHeaderData& header);

	void writeAMBEInt(CAMBEData& data);
	void writeDongleAMBEInt(CAMBEData& data);

	bool clockInt(unsigned long ms);

private:
	static unsigned int               m_maxReflectors;
	static CXReflectorDExtraHandler** m_reflectors;

	static wxString                   m_callsign;
	static CDExtraProtocolHandler*    m_handler;

	static bool                       m_stateChange;

	static CXReflectorUserLog*        m_userLogger;

	wxString                      m_reflector;
	wxString                      m_repeater;
	in_addr                       m_address;
	unsigned int                  m_port;
	bool                          m_linked;
	IXReflectorReflectorCallback* m_destination;
	time_t                        m_time;
	CTimer                        m_pollTimer;
	CTimer                        m_pollInactivityTimer;
	unsigned int                  m_dExtraId;
	unsigned int                  m_rptrId;
	CTimer                        m_inactivityTimer;
};

#endif
