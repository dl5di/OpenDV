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

#ifndef	RepeaterHandler_H
#define	RepeaterHandler_H

#include "RepeaterProtocolHandler.h"
#include "DExtraProtocolHandler.h"
#include "DPlusProtocolHandler.h"
#include "RemoteRepeaterData.h"
#include "G2ProtocolHandler.h"
#include "ReflectorCallback.h"
#include "RepeaterCallback.h"
#include "StarNetHandler.h"
#include "TextCollector.h"
#include "CacheManager.h"
#include "HeaderLogger.h"
#include "CallsignList.h"
#include "DRATSServer.h"
#include "CCSCallback.h"
#include "VersionUnit.h"
#include "CCSHandler.h"
#include "StatusData.h"
#include "APRSWriter.h"
#include "HeardData.h"
#include "AudioUnit.h"
#include "EchoUnit.h"
#include "PollData.h"
#include "DDData.h"
#include "IRCDDB.h"
#include "Timer.h"
#include "DTMF.h"
#include "Defs.h"

#if defined(__WINDOWS__)
#include "Inaddr.h"
#else
#include <netinet/in.h>
#endif

#include <wx/wx.h>

class CRepeaterHandler : public IRepeaterCallback, public IReflectorCallback, public ICCSCallback {
public:
	static void initialise(unsigned int maxRepeaters);

	static void add(const wxString& callsign, const wxString& band, const wxString& address, unsigned int port, HW_TYPE hwType, const wxString& reflector, bool atStartup, RECONNECT reconnect, bool dratsEnabled, double frequency, double offset, double range, double latitude, double longitude, double agl, const wxString& description1, const wxString& description2, const wxString& url, IRepeaterProtocolHandler* handler, unsigned char band1, unsigned char band2, unsigned char band3);

	static void setLocalAddress(const wxString& address);
	static void setG2Handler(CG2ProtocolHandler* handler);
	static void setIRC(CIRCDDB* irc);
	static void setCache(CCacheManager* cache);
	static void setGateway(const wxString& gateway);
	static void setLanguage(TEXT_LANG language);
	static void setDExtraEnabled(bool enabled);
	static void setDPlusEnabled(bool enabled);
	static void setDCSEnabled(bool enabled);
	static void setHeaderLogger(CHeaderLogger* logger);
	static void setAPRSWriter(CAPRSWriter* writer);
	static void setInfoEnabled(bool enabled);
	static void setEchoEnabled(bool enabled);
	static void setDTMFEnabled(bool enabled);
	static void setRestrictList(CCallsignList* list);

	static void startup();

	static bool getRepeater(unsigned int n, wxString& callsign, LINK_STATUS& linkStatus, wxString& linkCallsign);

	static wxArrayString     listDVRepeaters();

	static CRepeaterHandler* findDVRepeater(const CHeaderData& header);
	static CRepeaterHandler* findDVRepeater(const CAMBEData& data, bool busy);
	static CRepeaterHandler* findDVRepeater(const wxString& callsign);

	static CRepeaterHandler* findRepeater(const CPollData& data);

	static CRepeaterHandler* findDDRepeater(const CDDData& data);
	static CRepeaterHandler* findDDRepeater();

	static void pollAllIcom(CPollData& data);

	static void writeStatus(CStatusData& statusData);

	static void resolveUser(const wxString& user, const wxString& repeater, const wxString& gateway, const wxString& address);
	static void resolveRepeater(const wxString& repeater, const wxString& gateway, const wxString& address, DSTAR_PROTOCOL protocol);

	static void finalise();

	static void clock(unsigned int ms);

	void processRepeater(CHeaderData& header);
	void processRepeater(CHeardData& heard);
	void processRepeater(CAMBEData& data);
	void processRepeater(CPollData& data);
	void processRepeater(CDDData& data);

	void processBusy(CHeaderData& header);
	void processBusy(CAMBEData& data);

	void link(RECONNECT reconnect, const wxString& reflector);
	void unlink(PROTOCOL protocol, const wxString& reflector);

	CRemoteRepeaterData* getInfo() const;

	virtual bool process(CHeaderData& header, DIRECTION direction, AUDIO_SOURCE source);
	virtual bool process(CAMBEData& data, DIRECTION direction, AUDIO_SOURCE source);
	virtual bool process(CDDData& data);

	virtual void linkUp(DSTAR_PROTOCOL protocol, const wxString& callsign);
	virtual void linkRefused(DSTAR_PROTOCOL protocol, const wxString& callsign);
	virtual bool linkFailed(DSTAR_PROTOCOL protocol, const wxString& callsign, bool isRecoverable);

	virtual void ccsLinkMade(const wxString& callsign, DIRECTION direction);
	virtual void ccsLinkFailed(const wxString& dtmf, DIRECTION direction);
	virtual void ccsLinkEnded(const wxString& callsign, DIRECTION direction);

protected:
	CRepeaterHandler(const wxString& callsign, const wxString& band, const wxString& address, unsigned int port, HW_TYPE hwType, const wxString& reflector, bool atStartup, RECONNECT reconnect, bool dratsEnabled, double frequency, double offset, double range, double latitude, double longitude, double agl, const wxString& description1, const wxString& description2, const wxString& url, IRepeaterProtocolHandler* handler, unsigned char band1, unsigned char band2, unsigned char band3);
	virtual ~CRepeaterHandler();

	void resolveUserInt(const wxString& user, const wxString& repeater, const wxString& gateway, const wxString& address);
	void resolveRepeaterInt(const wxString& repeater, const wxString& gateway, const wxString& address, DSTAR_PROTOCOL protocol);

	void startupInt();

	void setIndex(unsigned int index);

	void clockInt(unsigned int ms);

private:
	static unsigned int       m_maxRepeaters;
	static CRepeaterHandler** m_repeaters;

	static wxString  m_localAddress;
	static CG2ProtocolHandler* m_g2Handler;
	static CCacheManager* m_cache;
	static wxString  m_gateway;
	static CIRCDDB*  m_irc;
	static TEXT_LANG m_language;
	static bool      m_dextraEnabled;
	static bool      m_dplusEnabled;
	static bool      m_dcsEnabled;
	static bool      m_infoEnabled;
	static bool      m_echoEnabled;
	static bool      m_dtmfEnabled;

	static CHeaderLogger*   m_headerLogger;

	static CAPRSWriter*     m_aprsWriter;

	static CCallsignList*   m_restrictList;

	// Repeater info
	unsigned int              m_index;
	wxString                  m_rptCallsign;
	wxString                  m_gwyCallsign;
	unsigned char             m_band;
	in_addr                   m_address;
	unsigned int              m_port;
	HW_TYPE                   m_hwType;
	IRepeaterProtocolHandler* m_repeaterHandler;
	double                    m_frequency;
	double                    m_offset;
	double                    m_range;
	double                    m_latitude;
	double                    m_longitude;
	double                    m_agl;
	wxString                  m_description1;
	wxString                  m_description2;
	wxString                  m_url;
	unsigned char             m_band1;
	unsigned char             m_band2;
	unsigned char             m_band3;
	unsigned int              m_repeaterId;
	unsigned int              m_busyId;
	CTimer                    m_watchdogTimer;
	bool                      m_ddMode;
	wxString                  m_ddCallsign;
	CTimer                    m_queryTimer;

	// User details
	wxString                  m_myCall1;
	wxString                  m_myCall2;
	wxString                  m_yourCall;
	wxString                  m_rptCall1;
	wxString                  m_rptCall2;
	unsigned char             m_flag1;
	unsigned char             m_flag2;
	unsigned char             m_flag3;
	bool                      m_restricted;

	// Statistics
	unsigned int              m_frames;
	unsigned int              m_silence;
	unsigned int              m_errors;

	// Slow data handling
	CTextCollector            m_textCollector;
	wxString                  m_text;

	// Cross-band repeating
	CRepeaterHandler*         m_xBandRptr;

	// StarNet
	CStarNetHandler*          m_starNet;

	// G2 info
	G2_STATUS                 m_g2Status;
	wxString                  m_g2User;
	wxString                  m_g2Repeater;
	wxString                  m_g2Gateway;
	CHeaderData*              m_g2Header;
	in_addr                   m_g2Address;

	// Link info
	LINK_STATUS               m_linkStatus;
	wxString                  m_linkRepeater;
	wxString                  m_linkGateway;
	RECONNECT                 m_linkReconnect;
	bool                      m_linkAtStartup;
	wxString                  m_linkStartup;
	CTimer                    m_linkReconnectTimer;
	bool                      m_linkRelink;

	// Echoing
	CEchoUnit*                m_echo;

	// Voice messages
	CAudioUnit*               m_audio;
	bool                      m_infoNeeded;

	// Version information
	CVersionUnit*             m_version;

	// D-RATS handler
	CDRATSServer*             m_drats;

	// DTMF commands
	CDTMF                     m_dtmf;

	// Poll timer
	CTimer                    m_pollTimer;

	// CCS
	CCCSHandler*              m_ccsHandler;

	// Reflector restoration
	wxString                  m_lastReflector;

	// Icom heard data
	wxString                  m_heardUser;
	wxString                  m_heardRepeater;
	CTimer                    m_heardTimer;

	void g2CommandHandler(const wxString& callsign, const wxString& user, CHeaderData& header);
	void ccsCommandHandler(const wxString& callsign, const wxString& user, const wxString& type);
	void reflectorCommandHandler(const wxString& callsign, const wxString& user, const wxString& type);
	void sendToOutgoing(const CHeaderData& header);
	void sendToOutgoing(const CAMBEData& data);
	void sendToIncoming(const CHeaderData& header);
	void sendToIncoming(const CAMBEData& data);

	void writeIsBusy(const wxString& callsign);
	void writeLinkedTo(const wxString& callsign);
	void writeLinkingTo(const wxString& callsign);
	void writeNotLinked();

	void sendHeard(const wxString& text = wxEmptyString);
	void sendStats();

	void linkInt(const wxString& callsign);

	void suspendLinks();
	bool restoreLinks();

	void triggerInfo();
};

#endif
