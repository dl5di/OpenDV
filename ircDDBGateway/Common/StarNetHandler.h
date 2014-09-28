/*
 *   Copyright (C) 2011-2014 by Jonathan Naylor G4KLX
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

#ifndef	StarNetHandler_H
#define	StarNetHandler_H

#include "RemoteStarNetGroup.h"
#include "G2ProtocolHandler.h"
#include "ReflectorCallback.h"		// DEXTRA_LINK || DCS_LINK
#include "RepeaterCallback.h"
#include "TextCollector.h"
#include "CacheManager.h"
#include "HeaderData.h"
#include "AMBEData.h"
#include "IRCDDB.h"
#include "Timer.h"

#if defined(__WINDOWS__)
#include "Inaddr.h"
#else
#include <netinet/in.h>
#endif

#include <wx/wx.h>

class CStarNetUser {
public:
	CStarNetUser(const wxString& callsign, unsigned int timeout);
	~CStarNetUser();

	void reset();

	bool clock(unsigned int ms);
	bool hasExpired();

	wxString getCallsign() const;
	CTimer   getTimer() const;

private:
	wxString m_callsign;
	CTimer   m_timer;
};

class CStarNetId {
public:
	CStarNetId(unsigned int id, unsigned int timeout, CStarNetUser* user);
	~CStarNetId();

	unsigned int getId() const;

	void reset();

	void setLogin();
	void setInfo();
	void setLogoff();
	void setEnd();

	bool clock(unsigned int ms);
	bool hasExpired();

	bool isLogin() const;
	bool isInfo() const;
	bool isLogoff() const;
	bool isEnd() const;

	CStarNetUser* getUser() const;

	CTextCollector& getTextCollector();

private:
	unsigned int   m_id;
	CTimer         m_timer;
	bool           m_login;
	bool           m_info;
	bool           m_logoff;
	bool           m_end;
	CStarNetUser*  m_user;
	CTextCollector m_textCollector;
};

class CStarNetRepeater {
public:
	wxString           m_destination;
	wxString           m_repeater;
	wxString           m_gateway;
	in_addr            m_address;
	IRepeaterCallback* m_local;
};

WX_DECLARE_HASH_MAP(unsigned int, CStarNetId*, wxIntegerHash, wxIntegerEqual, CStarNetIdsHashMap);
WX_DECLARE_STRING_HASH_MAP(CStarNetUser*,     CStarNetUsersHashMap);
WX_DECLARE_STRING_HASH_MAP(CStarNetRepeater*, CStarNetRepeatersHashMap);

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
class CStarNetHandler : public IReflectorCallback {
#else
class CStarNetHandler {
#endif
public:
	static void initialise(unsigned int maxStarNets, const wxString& name = wxEmptyString);

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
	static void add(const wxString& callsign, const wxString& logoff, const wxString& repeater, const wxString& infoText, const wxString& permanent, unsigned int userTimeout, unsigned int groupTimeout, STARNET_CALLSIGN_SWITCH callsignSwitch, bool txMsgSwitch, const wxString& reflector);
#else
	static void add(const wxString& callsign, const wxString& logoff, const wxString& repeater, const wxString& infoText, const wxString& permanent, unsigned int userTimeout, unsigned int groupTimeout, STARNET_CALLSIGN_SWITCH callsignSwitch, bool txMsgSwitch);
#endif

	static void setG2Handler(CG2ProtocolHandler* handler);
	static void setIRC(CIRCDDB* irc);
	static void setCache(CCacheManager* cache);
	static void setGateway(const wxString& gateway);
	static void setLogging(bool enable, const wxString& dir);

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
	static void link();
#endif

	static wxArrayString    listStarNets();

	static CStarNetHandler* findStarNet(const wxString& callsign);
	static CStarNetHandler* findStarNet(const CHeaderData& header);
	static CStarNetHandler* findStarNet(const CAMBEData& data);

	static void finalise();

	static void clock(unsigned int ms);

	void process(CHeaderData& header);
	void process(CAMBEData& data);

	CRemoteStarNetGroup* getInfo() const;

	bool logoff(const wxString& callsign);

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
	virtual bool process(CHeaderData& header, DIRECTION direction, AUDIO_SOURCE source);
	virtual bool process(CAMBEData& data, DIRECTION direction, AUDIO_SOURCE source);

	virtual void linkUp(DSTAR_PROTOCOL protocol, const wxString& callsign);
	virtual void linkRefused(DSTAR_PROTOCOL protocol, const wxString& callsign);
	virtual bool linkFailed(DSTAR_PROTOCOL protocol, const wxString& callsign, bool isRecoverable);

	virtual bool singleHeader();
#endif

protected:
#if defined(DEXTRA_LINK) || defined(DCS_LINK)
	CStarNetHandler(const wxString& callsign, const wxString& logoff, const wxString& repeater, const wxString& infoText, const wxString& permanent, unsigned int userTimeout, unsigned int groupTimeout, STARNET_CALLSIGN_SWITCH callsignSwitch, bool txMsgSwitch, const wxString& reflector);
#else
	CStarNetHandler(const wxString& callsign, const wxString& logoff, const wxString& repeater, const wxString& infoText, const wxString& permanent, unsigned int userTimeout, unsigned int groupTimeout, STARNET_CALLSIGN_SWITCH callsignSwitch, bool txMsgSwitch);
#endif
	~CStarNetHandler();

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
	void linkInt();
#endif

	void clockInt(unsigned int ms);

private:
	static unsigned int        m_maxStarNets;
	static CStarNetHandler**   m_starNets;

	static CG2ProtocolHandler* m_g2Handler;
	static CIRCDDB*            m_irc;
	static CCacheManager*      m_cache;
	static wxString            m_gateway;

	static wxString            m_name;
	static wxFFile*            m_logFile;

	// Group info
	wxString       m_groupCallsign;
	wxString       m_offCallsign;
	wxString       m_shortCallsign;
	wxString       m_repeater;
	wxString       m_infoText;
	wxArrayString  m_permanent;
#if defined(DEXTRA_LINK) || defined(DCS_LINK)
	wxString       m_linkReflector;
	wxString       m_linkGateway;
	LINK_STATUS    m_linkStatus;
	CTimer         m_linkTimer;
#endif

	unsigned int   m_id;

	CTimer         m_groupTimer;
	CTimer         m_announceTimer;

	unsigned int   m_userTimeout;

	STARNET_CALLSIGN_SWITCH  m_callsignSwitch;
	bool                     m_txMsgSwitch;

	CStarNetIdsHashMap       m_ids;
	CStarNetUsersHashMap     m_users;
	CStarNetRepeatersHashMap m_repeaters;

	void sendFromText(const wxString& text) const;
	void sendToRepeaters(CHeaderData& header) const;
	void sendToRepeaters(CAMBEData& data) const;
	void sendAck(const CUserData& user, const wxString& text) const;
};

#endif
