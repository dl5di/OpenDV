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

#ifndef	IRCDDBGatewayThread_H
#define	IRCDDBGatewayThread_H

#include "DummyRepeaterProtocolHandler.h"
#include "IcomRepeaterProtocolHandler.h"
#include "HBRepeaterProtocolHandler.h"
#include "DExtraProtocolHandlerPool.h"
#include "DPlusProtocolHandlerPool.h"
#include "RepeaterProtocolHandler.h"
#include "IRCDDBGatewayStatusData.h"
#include "DCSProtocolHandlerPool.h"
#include "G2ProtocolHandler.h"
#include "RemoteHandler.h"
#include "CacheManager.h"
#include "CallsignList.h"
#include "APRSWriter.h"
#include "IRCDDB.h"
#include "Timer.h"
#include "Defs.h"

#include <wx/wx.h>

class CIRCDDBGatewayThread {
public:
	CIRCDDBGatewayThread(const wxString& logDir, const wxString& name);
	virtual ~CIRCDDBGatewayThread();

	virtual void setGateway(GATEWAY_TYPE type, const wxString& callsign, const wxString& address);
	virtual void addRepeater(const wxString& callsign, const wxString& band, const wxString& address, unsigned int port, HW_TYPE hwType, const wxString& reflector, bool atStartup, RECONNECT reconnect, bool dratsEnabled, double frequency, double offset, double range, double latitude, double longitude, double agl, const wxString& description1, const wxString& description2, const wxString& url, IRepeaterProtocolHandler* handler, unsigned char band1 = 0x00U, unsigned char band2 = 0x00U, unsigned char band3 = 0x00U);
#if defined(DEXTRA_LINK) || defined(DCS_LINK)
	virtual void addStarNet(const wxString& callsign, const wxString& logoff, const wxString& repeater, const wxString& infoText, const wxString& permanent, unsigned int userTimeout, unsigned int groupTimeout, STARNET_CALLSIGN_SWITCH callsignSwitch, bool txMsgSwitch, const wxString& reflector);
#else
	virtual void addStarNet(const wxString& callsign, const wxString& logoff, const wxString& repeater, const wxString& infoText, const wxString& permanent, unsigned int userTimeout, unsigned int groupTimeout, STARNET_CALLSIGN_SWITCH callsignSwitch, bool txMsgSwitch);
#endif
	virtual void setIcomRepeaterHandler(CIcomRepeaterProtocolHandler* handler);
	virtual void setHBRepeaterHandler(CHBRepeaterProtocolHandler* handler);
	virtual void setDummyRepeaterHandler(CDummyRepeaterProtocolHandler* handler);
	virtual void setIRC(CIRCDDB* irc);
	virtual void setLanguage(TEXT_LANG language);
	virtual void setDExtra(bool enabled, unsigned int maxDongles);
	virtual void setDPlus(bool enabled, unsigned int maxDongles, const wxString& login);
	virtual void setDCS(bool enabled);
	virtual void setCCS(bool enabled, const wxString& host);
	virtual void setLog(bool enabled);
	virtual void setAPRSWriter(CAPRSWriter* writer);
	virtual void setInfoEnabled(bool enabled);
	virtual void setEchoEnabled(bool enabled);
	virtual void setDTMFEnabled(bool enabled);
	virtual void setDDModeEnabled(bool enabled);
	virtual void setRemote(bool enabled, const wxString& password, unsigned int port);
	virtual void setLocation(double latitude, double longitude);
	virtual void setRestrictList(CCallsignList* list);

	virtual CIRCDDBGatewayStatusData* getStatus() const;

	virtual void run();
	virtual void kill();

private:
	wxString                  m_logDir;
	wxString                  m_name;
	bool                      m_killed;
	bool                      m_stopped;
	GATEWAY_TYPE              m_gatewayType;
	wxString                  m_gatewayCallsign;
	wxString                  m_gatewayAddress;
	CIcomRepeaterProtocolHandler*  m_icomRepeaterHandler;
	CHBRepeaterProtocolHandler*    m_hbRepeaterHandler;
	CDummyRepeaterProtocolHandler* m_dummyRepeaterHandler;
	CDExtraProtocolHandlerPool*    m_dextraPool;
	CDPlusProtocolHandlerPool*     m_dplusPool;
	CDCSProtocolHandlerPool*       m_dcsPool;
	CG2ProtocolHandler*       m_g2Handler;
	CAPRSWriter*              m_aprsWriter;
	CIRCDDB*                  m_irc;
	CCacheManager             m_cache;
	TEXT_LANG                 m_language;
	bool                      m_dextraEnabled;
	unsigned int              m_dextraMaxDongles;
	bool                      m_dplusEnabled;
	unsigned int              m_dplusMaxDongles;
	wxString                  m_dplusLogin;
	bool                      m_dcsEnabled;
	bool                      m_ccsEnabled;
	wxString                  m_ccsHost;
	bool                      m_infoEnabled;
	bool                      m_echoEnabled;
	bool                      m_dtmfEnabled;
	bool                      m_logEnabled;
	bool                      m_ddModeEnabled;
	IRCDDB_STATUS             m_lastStatus;
	CTimer                    m_statusTimer1;
	CTimer                    m_statusTimer2;
	bool                      m_remoteEnabled;
	wxString                  m_remotePassword;
	unsigned int              m_remotePort;
	CRemoteHandler*           m_remote;
	CTimer                    m_statusFileTimer;
	wxString                  m_status1;
	wxString                  m_status2;
	wxString                  m_status3;
	wxString                  m_status4;
	wxString                  m_status5;
	double                    m_latitude;
	double                    m_longitude;
	CCallsignList*            m_restrictList;

	void processIrcDDB();
	void processRepeater(IRepeaterProtocolHandler* handler);
	void processDExtra();
	void processDPlus();
	void processDCS();
	void processG2();
	void processDD();

	void loadGateways();
	void loadReflectors();
	void loadDExtraReflectors(const wxString& fileName);
	void loadDPlusReflectors(const wxString& fileName);
	void loadDCSReflectors(const wxString& fileName);

	void writeStatus();

	void readStatusFiles();
	void readStatusFile(const wxString& filename, unsigned int n, wxString& var);
};

#endif
