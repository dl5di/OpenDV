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

#ifndef	StarNetServerThread_H
#define	StarNetServerThread_H

#include "DExtraProtocolHandlerPool.h"		// DEXTRA_LINK
#include "DCSProtocolHandlerPool.h"			// DCS_LINK
#include "G2ProtocolHandler.h"
#include "RemoteHandler.h"
#include "CacheManager.h"
#include "IRCDDB.h"
#include "Timer.h"
#include "Defs.h"

#include <wx/wx.h>

class CStarNetServerThread {
public:
	CStarNetServerThread(bool nolog, const wxString& logDir);
	virtual ~CStarNetServerThread();

	virtual void setCallsign(const wxString& callsign);
	virtual void setAddress(const wxString& address);
#if defined(DEXTRA_LINK) || defined(DCS_LINK)
	virtual void addStarNet(const wxString& callsign, const wxString& logoff, const wxString& repeater, const wxString& infoText, const wxString& permanent, unsigned int userTimeout, unsigned int groupTimeout, STARNET_CALLSIGN_SWITCH callsignSwitch, bool txMsgSwitch, const wxString& reflector);
#else
	virtual void addStarNet(const wxString& callsign, const wxString& logoff, const wxString& repeater, const wxString& infoText, const wxString& permanent, unsigned int userTimeout, unsigned int groupTimeout, STARNET_CALLSIGN_SWITCH callsignSwitch, bool txMsgSwitch);
#endif
	virtual void setRemote(bool enabled, const wxString& password, unsigned int port);
	virtual void setIRC(CIRCDDB* irc);
	virtual void setLog(bool enabled);

	virtual void run();
	virtual void kill();

private:
	bool                    m_nolog;
	wxString                m_logDir;
	bool                    m_killed;
	bool                    m_stopped;
	wxString                m_callsign;
	wxString                m_address;
#if defined(DEXTRA_LINK)
	CDExtraProtocolHandlerPool* m_dextraPool;
#endif
#if defined(DCS_LINK)
	CDCSProtocolHandlerPool*    m_dcsPool;
#endif
	CG2ProtocolHandler*     m_g2Handler;
	CIRCDDB*                m_irc;
	CCacheManager           m_cache;
	bool                    m_logEnabled;
	CTimer                  m_statusTimer;
	IRCDDB_STATUS           m_lastStatus;
	bool                    m_remoteEnabled;
	wxString                m_remotePassword;
	unsigned int            m_remotePort;
	CRemoteHandler*         m_remote;

	void processIrcDDB();
	void processG2();
#if defined(DEXTRA_LINK)
	void processDExtra();
	void loadReflectors();
#endif
#if defined(DCS_LINK)
	void processDCS();
	void loadReflectors();
#endif
};

#endif
