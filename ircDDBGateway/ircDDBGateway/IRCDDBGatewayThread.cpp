/*
 *   Copyright (C) 2010-2014 by Jonathan Naylor G4KLX
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

#include "IRCDDBGatewayThread.h"
#include "IRCDDBGatewayDefs.h"
#include "RepeaterHandler.h"
#include "StarNetHandler.h"
#include "CallsignServer.h"
#include "DExtraHandler.h"
#include "DPlusHandler.h"
#include "HeaderLogger.h"
#include "ConnectData.h"
#include "CCSHandler.h"
#include "HeaderData.h"
#include "StatusData.h"
#include "DCSHandler.h"
#include "DDHandler.h"
#include "G2Handler.h"
#include "HeardData.h"
#include "PollData.h"
#include "AMBEData.h"
#include "HostFile.h"
#include "CCSData.h"
#include "DDData.h"
#include "Utils.h"
#include "Defs.h"

#include <wx/filename.h>
#include <wx/textfile.h>
#include <wx/ffile.h>

const wxString LOOPBACK_ADDRESS = wxT("127.0.0.1");

const unsigned int REMOTE_DUMMY_PORT = 65016U;

CIRCDDBGatewayThread::CIRCDDBGatewayThread(const wxString& logDir, const wxString& name) :
m_logDir(logDir),
m_name(name),
m_killed(false),
m_stopped(true),
m_gatewayType(GT_REPEATER),
m_gatewayCallsign(),
m_gatewayAddress(),
m_icomRepeaterHandler(NULL),
m_hbRepeaterHandler(NULL),
m_dummyRepeaterHandler(NULL),
m_dextraPool(NULL),
m_dplusPool(NULL),
m_dcsPool(NULL),
m_g2Handler(NULL),
m_aprsWriter(NULL),
m_irc(NULL),
m_cache(),
m_language(TL_ENGLISH_UK),
m_dextraEnabled(true),
m_dextraMaxDongles(0U),
m_dplusEnabled(false),
m_dplusMaxDongles(0U),
m_dplusLogin(),
m_dcsEnabled(true),
m_ccsEnabled(true),
m_ccsHost(),
m_infoEnabled(true),
m_echoEnabled(true),
m_dtmfEnabled(true),
m_logEnabled(false),
m_ddModeEnabled(false),
m_lastStatus(IS_DISABLED),
m_statusTimer1(1000U, 1U),		// 1 second
m_statusTimer2(1000U, 1U),		// 1 second
m_remoteEnabled(false),
m_remotePassword(),
m_remotePort(0U),
m_remote(NULL),
m_statusFileTimer(1000U, 2U * 60U),		// 2 minutes
m_status1(),
m_status2(),
m_status3(),
m_status4(),
m_status5(),
m_latitude(0.0),
m_longitude(0.0),
m_restrictList(NULL)
{
	CHeaderData::initialise();
	CG2Handler::initialise(MAX_ROUTES);
	CDExtraHandler::initialise(MAX_DEXTRA_LINKS);
	CDPlusHandler::initialise(MAX_DPLUS_LINKS);
	CDCSHandler::initialise(MAX_DCS_LINKS);
	CRepeaterHandler::initialise(MAX_REPEATERS);
	CStarNetHandler::initialise(MAX_STARNETS, m_name);
	CCCSHandler::initialise(MAX_REPEATERS);
	CAudioUnit::initialise();
}

CIRCDDBGatewayThread::~CIRCDDBGatewayThread()
{
	CHeaderData::finalise();
	CG2Handler::finalise();
	CDExtraHandler::finalise();
	CDPlusHandler::finalise();
	CDCSHandler::finalise();
	CRepeaterHandler::finalise();
	CStarNetHandler::finalise();
	CCCSHandler::finalise();
	CAudioUnit::finalise();
}

void CIRCDDBGatewayThread::run()
{
	// Truncate the old Links.log file
	wxString fullName = LINKS_BASE_NAME;

	if (!m_name.IsEmpty()) {
		fullName.Append(wxT("_"));
		fullName.Append(m_name);
	}

	wxFileName fileName1(m_logDir, fullName, wxT("log"));
	wxLogMessage(wxT("Truncating %s"), fileName1.GetFullPath().c_str());

	wxFFile file;
	bool ret = file.Open(fileName1.GetFullPath(), wxT("wt"));
	if (ret)
		file.Close();

	// Truncate the old StarNet.log file
	fullName = STARNET_BASE_NAME;

	if (!m_name.IsEmpty()) {
		fullName.Append(wxT("_"));
		fullName.Append(m_name);
	}

	wxFileName fileName2(m_logDir, fullName, wxT("log"));
	wxLogMessage(wxT("Truncating %s"), fileName2.GetFullPath().c_str());

	ret = file.Open(fileName2.GetFullPath(), wxT("wt"));
	if (ret)
		file.Close();

	wxString dextraAddress = m_dextraEnabled ? m_gatewayAddress : LOOPBACK_ADDRESS;
	m_dextraPool = new CDExtraProtocolHandlerPool(MAX_OUTGOING + 1U, DEXTRA_PORT, dextraAddress);
	ret = m_dextraPool->open();
	if (!ret) {
		wxLogError(wxT("Could not open the DExtra protocol pool"));
		delete m_dextraPool;
		m_dextraPool = NULL;
	} else {
		// Allocate the incoming port
		CDExtraProtocolHandler* handler = m_dextraPool->getHandler(DEXTRA_PORT);
		CDExtraHandler::setDExtraProtocolIncoming(handler);
		CDExtraHandler::setDExtraProtocolHandlerPool(m_dextraPool);
	}

	wxString dplusAddress = m_dplusEnabled ? m_gatewayAddress : LOOPBACK_ADDRESS;
	m_dplusPool = new CDPlusProtocolHandlerPool(MAX_OUTGOING + 1U, DPLUS_PORT, dplusAddress);
	ret = m_dplusPool->open();
	if (!ret) {
		wxLogError(wxT("Could not open the D-Plus protocol pool"));
		delete m_dplusPool;
		m_dplusPool = NULL;
	} else {
		// Allocate the incoming port
		CDPlusProtocolHandler* handler = m_dplusPool->getHandler(DPLUS_PORT);
		CDPlusHandler::setDPlusProtocolIncoming(handler);
		CDPlusHandler::setDPlusProtocolHandlerPool(m_dplusPool);
	}

	wxString dcsAddress = m_dcsEnabled ? m_gatewayAddress : LOOPBACK_ADDRESS;
	m_dcsPool = new CDCSProtocolHandlerPool(MAX_OUTGOING + 1U, DCS_PORT, dcsAddress);
	ret = m_dcsPool->open();
	if (!ret) {
		wxLogError(wxT("Could not open the DCS protocol pool"));
		delete m_dcsPool;
		m_dcsPool = NULL;
	} else {
		// Allocate the incoming port
		CDCSProtocolHandler* handler = m_dcsPool->getHandler(DCS_PORT);
		CDCSHandler::setDCSProtocolIncoming(handler);
		CDCSHandler::setDCSProtocolHandlerPool(m_dcsPool);
	}

	m_g2Handler = new CG2ProtocolHandler(G2_DV_PORT, m_gatewayAddress);
	ret = m_g2Handler->open();
	if (!ret) {
		wxLogError(wxT("Could not open the G2 protocol handler"));
		delete m_g2Handler;
		m_g2Handler = NULL;
	}

	// Wait here until we have the essentials to run
	while (!m_killed && (m_dextraPool == NULL || m_dplusPool == NULL || m_dcsPool == NULL || m_g2Handler == NULL || (m_icomRepeaterHandler == NULL && m_hbRepeaterHandler == NULL && m_dummyRepeaterHandler == NULL) || m_gatewayCallsign.IsEmpty()))
		::wxMilliSleep(500UL);		// 1/2 sec

	if (m_killed)
		return;

	m_stopped = false;

	wxLogMessage(wxT("Starting the ircDDB Gateway thread"));

	CHeaderLogger* headerLogger = NULL;
	if (m_logEnabled) {
		m_statusTimer1.start();

		headerLogger = new CHeaderLogger(m_logDir, m_name);
		bool ret = headerLogger->open();
		if (!ret) {
			delete headerLogger;
			headerLogger = NULL;
		}
	}

	loadGateways();
	loadReflectors();

	CG2Handler::setG2ProtocolHandler(m_g2Handler);
	CG2Handler::setHeaderLogger(headerLogger);

	CDExtraHandler::setCallsign(m_gatewayCallsign);
	CDExtraHandler::setHeaderLogger(headerLogger);
	CDExtraHandler::setMaxDongles(m_dextraMaxDongles);

	CDPlusHandler::setCallsign(m_gatewayCallsign);
	CDPlusHandler::setDPlusLogin(m_dplusLogin);
	CDPlusHandler::setHeaderLogger(headerLogger);
	CDPlusHandler::setMaxDongles(m_dplusMaxDongles);
	if (m_dplusEnabled)
		CDPlusHandler::startAuthenticator(m_gatewayAddress, &m_cache);

	CDCSHandler::setGatewayType(m_gatewayType);
	CDCSHandler::setHeaderLogger(headerLogger);

	CRepeaterHandler::setLocalAddress(m_gatewayAddress);
	CRepeaterHandler::setG2Handler(m_g2Handler);

	if (m_irc != NULL)
		CRepeaterHandler::setIRC(m_irc);

	CRepeaterHandler::setCache(&m_cache);
	CRepeaterHandler::setGateway(m_gatewayCallsign);
	CRepeaterHandler::setLanguage(m_language);
	CRepeaterHandler::setDExtraEnabled(m_dextraEnabled);
	CRepeaterHandler::setDPlusEnabled(m_dplusEnabled);
	CRepeaterHandler::setDCSEnabled(m_dcsEnabled);
	CRepeaterHandler::setHeaderLogger(headerLogger);
	CRepeaterHandler::setAPRSWriter(m_aprsWriter);
	CRepeaterHandler::setInfoEnabled(m_infoEnabled);
	CRepeaterHandler::setEchoEnabled(m_echoEnabled);
	CRepeaterHandler::setDTMFEnabled(m_dtmfEnabled);
	if (m_restrictList != NULL)
		CRepeaterHandler::setRestrictList(m_restrictList);

	CAudioUnit::setLanguage(m_language);

	CStarNetHandler::setCache(&m_cache);
	CStarNetHandler::setGateway(m_gatewayCallsign);
	CStarNetHandler::setG2Handler(m_g2Handler);

	if (m_irc != NULL)
		CStarNetHandler::setIRC(m_irc);

	CStarNetHandler::setLogging(m_logEnabled, m_logDir);
#if defined(DEXTRA_LINK) || defined(DCS_LINK)
	CStarNetHandler::link();
#endif

	if (m_ddModeEnabled) {
		CDDHandler::initialise(MAX_DD_ROUTES, m_name);
		CDDHandler::setLogging(m_logEnabled, m_logDir);
		CDDHandler::setHeaderLogger(headerLogger);

		if (m_irc != NULL)
			CDDHandler::setIRC(m_irc);
	}

	wxString ccsAddress = m_ccsEnabled ? m_gatewayAddress : LOOPBACK_ADDRESS;
	CCCSHandler::setLocalAddress(ccsAddress);
	CCCSHandler::setHeaderLogger(headerLogger);
	CCCSHandler::setHost(m_ccsHost);

	// If no ircDDB then APRS is started immediately
	if (m_aprsWriter != NULL && m_irc == NULL)
		m_aprsWriter->setEnabled(true);

	if (m_remoteEnabled && !m_remotePassword.IsEmpty() && m_remotePort > 0U) {
		m_remote = new CRemoteHandler(m_remotePassword, m_remotePort, m_gatewayAddress);
		bool res = m_remote->open();
		if (!res) {
			delete m_remote;
			m_remote = NULL;
		}
	}

	CRepeaterHandler::startup();

	m_statusFileTimer.start();

	CCallsignServer* server = NULL;
	if (m_dextraEnabled || m_dcsEnabled) {
		server = new CCallsignServer(m_gatewayCallsign, m_gatewayAddress, &m_cache);
		server->start();
	}

	wxStopWatch stopWatch;
	stopWatch.Start();

	m_statusTimer2.start();

	try {
		while (!m_killed) {
			if (m_icomRepeaterHandler != NULL)
				processRepeater(m_icomRepeaterHandler);

			if (m_hbRepeaterHandler != NULL)
				processRepeater(m_hbRepeaterHandler);

			if (m_dummyRepeaterHandler != NULL)
				processRepeater(m_dummyRepeaterHandler);

			if (m_irc != NULL)
				processIrcDDB();

			processDExtra();
			processDPlus();
			processDCS();
			processG2();
			CCCSHandler::process();

			if (m_ddModeEnabled)
				processDD();

			if (m_remote != NULL)
				m_remote->process();

			unsigned long ms = stopWatch.Time();
			stopWatch.Start();

			CRepeaterHandler::clock(ms);
			CG2Handler::clock(ms);
			CDExtraHandler::clock(ms);
			CDPlusHandler::clock(ms);
			CDCSHandler::clock(ms);
			CStarNetHandler::clock(ms);
			CDDHandler::clock(ms);
	 		CCCSHandler::clock(ms);

			m_statusTimer2.clock(ms);

			m_statusFileTimer.clock(ms);
			if (m_statusFileTimer.hasExpired()) {
				readStatusFiles();
				m_statusFileTimer.start();
			}

			if (m_aprsWriter != NULL)
				m_aprsWriter->clock(ms);

			if (m_logEnabled) {
				m_statusTimer1.clock(ms);
				if (m_statusTimer1.hasExpired()) {
					bool ret1 = CDExtraHandler::stateChange();
					bool ret2 = CDPlusHandler::stateChange();
					bool ret3 = CDCSHandler::stateChange();
					bool ret4 = CCCSHandler::stateChange();
					if (ret1 || ret2 || ret3 || ret4)
						writeStatus();
		
					m_statusTimer1.start();
				}
			}

			::wxMilliSleep(TIME_PER_TIC_MS);
		}
	}
	catch (std::exception& e) {
		wxString message(e.what(), wxConvLocal);
		wxLogError(wxT("Exception raised - \"%s\""), message.c_str());
	}
	catch (...) {
		wxLogError(wxT("Unknown exception raised"));
	}

	wxLogMessage(wxT("Stopping the ircDDB Gateway thread"));

	// Unlink from all reflectors
	CDExtraHandler::unlink();
	CDPlusHandler::unlink();
	CDCSHandler::unlink();
	CCCSHandler::disconnect();

	if (m_ddModeEnabled)
		CDDHandler::finalise();

	if (server != NULL)
		server->stop();

	m_dextraPool->close();
	delete m_dextraPool;

	m_dplusPool->close();
	delete m_dplusPool;

	m_dcsPool->close();
	delete m_dcsPool;

	m_g2Handler->close();
	delete m_g2Handler;

	if (m_irc != NULL) {
		m_irc->close();
		delete m_irc;
	}

	if (m_icomRepeaterHandler != NULL) {
		m_icomRepeaterHandler->close();
		delete m_icomRepeaterHandler;
	}

	if (m_hbRepeaterHandler != NULL) {
		m_hbRepeaterHandler->close();
		delete m_hbRepeaterHandler;
	}

	if (m_dummyRepeaterHandler != NULL) {
		m_dummyRepeaterHandler->close();
		delete m_dummyRepeaterHandler;
	}

	if (m_remote != NULL) {
		m_remote->close();
		delete m_remote;
	}

	if (headerLogger != NULL) {
		headerLogger->close();
		delete headerLogger;
	}
}

void CIRCDDBGatewayThread::kill()
{
	m_killed = true;
}

void CIRCDDBGatewayThread::setGateway(GATEWAY_TYPE gatewayType, const wxString& gatewayCallsign, const wxString& gatewayAddress)
{
	if (!m_stopped)
		return;

	m_gatewayType     = gatewayType;
	m_gatewayCallsign = gatewayCallsign;
	m_gatewayAddress  = gatewayAddress;
}

void CIRCDDBGatewayThread::addRepeater(const wxString& callsign, const wxString& band, const wxString& address, unsigned int port, HW_TYPE hwType, const wxString& reflector, bool atStartup, RECONNECT reconnect, bool dratsEnabled, double frequency, double offset, double range, double latitude, double longitude, double agl, const wxString& description1, const wxString& description2, const wxString& url, IRepeaterProtocolHandler* handler, unsigned char band1, unsigned char band2, unsigned char band3)
{
	CRepeaterHandler::add(callsign, band, address, port, hwType, reflector, atStartup, reconnect, dratsEnabled, frequency, offset, range, latitude, longitude, agl, description1, description2, url, handler, band1, band2, band3);

	wxString repeater = callsign;
	repeater.Truncate(LONG_CALLSIGN_LENGTH - 1U);
	repeater.Append(band);

	// Add a fixed address and protocol for the local repeaters
	m_cache.updateRepeater(repeater, m_gatewayCallsign, wxT("127.0.0.1"), DP_LOOPBACK, true, true);

	wxLogMessage(wxT("Adding %s to the cache as a local repeater"), repeater.c_str());
}

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
void CIRCDDBGatewayThread::addStarNet(const wxString& callsign, const wxString& logoff, const wxString& repeater, const wxString& infoText, const wxString& permanent, unsigned int userTimeout, unsigned int groupTimeout, STARNET_CALLSIGN_SWITCH callsignSwitch, bool txMsgSwitch, const wxString& reflector)
{
	CStarNetHandler::add(callsign, logoff, repeater, infoText, permanent, userTimeout, groupTimeout, callsignSwitch, txMsgSwitch, reflector);
}
#else
void CIRCDDBGatewayThread::addStarNet(const wxString& callsign, const wxString& logoff, const wxString& repeater, const wxString& infoText, const wxString& permanent, unsigned int userTimeout, unsigned int groupTimeout, STARNET_CALLSIGN_SWITCH callsignSwitch, bool txMsgSwitch)
{
	CStarNetHandler::add(callsign, logoff, repeater, infoText, permanent, userTimeout, groupTimeout, callsignSwitch, txMsgSwitch);
}
#endif

void CIRCDDBGatewayThread::setIcomRepeaterHandler(CIcomRepeaterProtocolHandler* handler)
{
	m_icomRepeaterHandler = handler;
}

void CIRCDDBGatewayThread::setHBRepeaterHandler(CHBRepeaterProtocolHandler* handler)
{
	m_hbRepeaterHandler = handler;
}

void CIRCDDBGatewayThread::setDummyRepeaterHandler(CDummyRepeaterProtocolHandler* handler)
{
	m_dummyRepeaterHandler = handler;
}

void CIRCDDBGatewayThread::setIRC(CIRCDDB* irc)
{
	wxASSERT(irc != NULL);

	m_irc = irc;

	m_lastStatus = IS_DISCONNECTED;
}

void CIRCDDBGatewayThread::setLanguage(TEXT_LANG language)
{
	m_language = language;
}

void CIRCDDBGatewayThread::setDExtra(bool enabled, unsigned int maxDongles)
{
	if (enabled) {
		m_dextraEnabled    = true;
		m_dextraMaxDongles = maxDongles;
	} else {
		m_dextraEnabled    = false;
		m_dextraMaxDongles = 0U;
	}
}

void CIRCDDBGatewayThread::setDPlus(bool enabled, unsigned int maxDongles, const wxString& login)
{
	if (enabled) {
		m_dplusEnabled    = true;
		m_dplusMaxDongles = maxDongles;
	} else {
		m_dplusEnabled    = false;
		m_dplusMaxDongles = 0U;
	}

	m_dplusLogin = login;
}

void CIRCDDBGatewayThread::setDCS(bool enabled)
{
	m_dcsEnabled = enabled;
}

void CIRCDDBGatewayThread::setCCS(bool enabled, const wxString& host)
{
	m_ccsEnabled = enabled;

	wxFileName fileName(wxFileName::GetHomeDir(), CCS_HOSTS_FILE_NAME);

	if (!fileName.IsFileReadable()) {
		wxLogMessage(wxT("File %s not readable"), fileName.GetFullPath().c_str());
#if defined(__WINDOWS__)
		fileName.Assign(::wxGetCwd(), CCS_HOSTS_FILE_NAME);
#else
		fileName.Assign(wxT(DATA_DIR), CCS_HOSTS_FILE_NAME);
#endif
		if (!fileName.IsFileReadable()) {
			wxLogMessage(wxT("File %s not readable"), fileName.GetFullPath().c_str());
			m_ccsEnabled = false;
			return;
		}
	}

	CHostFile hostFile(fileName.GetFullPath(), true);

	m_ccsHost = hostFile.getAddress(host);
}

void CIRCDDBGatewayThread::setLog(bool enabled)
{
	m_logEnabled = enabled;
}

void CIRCDDBGatewayThread::setAPRSWriter(CAPRSWriter* writer)
{
	m_aprsWriter = writer;
}

void CIRCDDBGatewayThread::setInfoEnabled(bool enabled)
{
	m_infoEnabled = enabled;
}

void CIRCDDBGatewayThread::setEchoEnabled(bool enabled)
{
	m_echoEnabled = enabled;
}

void CIRCDDBGatewayThread::setDTMFEnabled(bool enabled)
{
	m_dtmfEnabled = enabled;
}

void CIRCDDBGatewayThread::setDDModeEnabled(bool enabled)
{
	m_ddModeEnabled = enabled;
}

void CIRCDDBGatewayThread::setLocation(double latitude, double longitude)
{
	m_latitude  = latitude;
	m_longitude = longitude;
}

void CIRCDDBGatewayThread::setRemote(bool enabled, const wxString& password, unsigned int port)
{
	if (enabled) {
		m_remoteEnabled  = true;
		m_remotePassword = password;
		m_remotePort     = port;
	} else {
		m_remoteEnabled  = false;
		m_remotePassword = password;
		m_remotePort     = REMOTE_DUMMY_PORT;
	}
}

void CIRCDDBGatewayThread::setRestrictList(CCallsignList* list)
{
	wxASSERT(list != NULL);

	m_restrictList = list;
}

void CIRCDDBGatewayThread::processIrcDDB()
{
	// Once per second
	if (m_statusTimer2.hasExpired()) {
		int status = m_irc->getConnectionState();
		switch (status) {
			case 0:
			case 10:
				if (m_lastStatus != IS_DISCONNECTED) {
					wxLogInfo(wxT("Disconnected from ircDDB"));
					m_lastStatus = IS_DISCONNECTED;
					if (m_aprsWriter != NULL)
						m_aprsWriter->setEnabled(false);
				}
				break;
			case 7:
				if (m_lastStatus != IS_CONNECTED) {
					wxLogInfo(wxT("Connected to ircDDB"));
					m_lastStatus = IS_CONNECTED;
					if (m_aprsWriter != NULL)
						m_aprsWriter->setEnabled(true);
				}
				break;
			default:
				if (m_lastStatus != IS_CONNECTING) {
					wxLogInfo(wxT("Connecting to ircDDB"));
					m_lastStatus = IS_CONNECTING;
					if (m_aprsWriter != NULL)
						m_aprsWriter->setEnabled(false);
				}
				break;
		}

		m_statusTimer2.start();
	}

	// Process incoming ircDDB messages, updating the caches
	for (;;) {
		IRCDDB_RESPONSE_TYPE type = m_irc->getMessageType();

		switch (type) {
			case IDRT_USER: {
					wxString user, repeater, gateway, address;
					bool res = m_irc->receiveUser(user, repeater, gateway, address);
					if (!res)
						break;

					CRepeaterHandler::resolveUser(user, repeater, gateway, address);
					if (!address.IsEmpty()) {
						wxLogMessage(wxT("USER: %s %s %s %s"), user.c_str(), repeater.c_str(), gateway.c_str(), address.c_str());
						m_cache.updateUser(user, repeater, gateway, address, DP_DEXTRA, false, false);
					} else {
						wxLogMessage(wxT("USER: %s NOT FOUND"), user.c_str());
					}
				}
				break;

			case IDRT_REPEATER: {
					wxString repeater, gateway, address;
					bool res = m_irc->receiveRepeater(repeater, gateway, address);
					if (!res)
						break;

					CRepeaterHandler::resolveRepeater(repeater, gateway, address, DP_DEXTRA);
					if (!address.IsEmpty()) {
						wxLogMessage(wxT("REPEATER: %s %s %s"), repeater.c_str(), gateway.c_str(), address.c_str());
						m_cache.updateRepeater(repeater, gateway, address, DP_DEXTRA, false, false);
					} else {
						wxLogMessage(wxT("REPEATER: %s NOT FOUND"), repeater.c_str());
					}
				}
				break;

			case IDRT_GATEWAY: {
					wxString gateway, address;
					bool res = m_irc->receiveGateway(gateway, address);
					if (!res)
						break;

					CDExtraHandler::gatewayUpdate(gateway, address);
					CDPlusHandler::gatewayUpdate(gateway, address);
					if (!address.IsEmpty()) {
						wxLogMessage(wxT("GATEWAY: %s %s"), gateway.c_str(), address.c_str());
						m_cache.updateGateway(gateway, address, DP_DEXTRA, false, false);
					} else {
						wxLogMessage(wxT("GATEWAY: %s NOT FOUND"), gateway.c_str());
					}
				}
				break;

			default:
				return;
		}
	}
}

void CIRCDDBGatewayThread::processRepeater(IRepeaterProtocolHandler* handler)
{
	for (;;) {
		REPEATER_TYPE type = handler->read();

		switch (type) {
			case RT_POLL: {
					CPollData* poll = handler->readPoll();
					if (poll != NULL) {
						CRepeaterHandler* handler = CRepeaterHandler::findRepeater(*poll);
						if (handler != NULL)
							handler->processRepeater(*poll);
						else
							CRepeaterHandler::pollAllIcom(*poll);

						delete poll;
					}
				}
				break;

			case RT_HEARD: {
					CHeardData* heard = handler->readHeard();
					if (heard != NULL) {
						wxString user = heard->getUser();
						wxString repeater = heard->getRepeater();

						// Internal controller heard have identical user and repeater values
						if (!repeater.IsSameAs(user)) {
							CRepeaterHandler* handler = CRepeaterHandler::findDVRepeater(repeater);
							if (handler == NULL)
								wxLogMessage(wxT("Heard received from unknown repeater, %s"), repeater.c_str());
							else
								handler->processRepeater(*heard);

							delete heard;
						}
					}
				}
				break;

			case RT_HEADER: {
					CHeaderData* header = handler->readHeader();
					if (header != NULL) {
						// wxLogMessage(wxT("Repeater header - My: %s/%s  Your: %s  Rpt1: %s  Rpt2: %s  Flags: %02X %02X %02X"), header->getMyCall1().c_str(), header->getMyCall2().c_str(), header->getYourCall().c_str(), header->getRptCall1().c_str(), header->getRptCall2().c_str(), header->getFlag1(), header->getFlag2(), header->getFlag3());

						CRepeaterHandler* repeater = CRepeaterHandler::findDVRepeater(*header);
						if (repeater == NULL)
							wxLogMessage(wxT("Header received from unknown repeater, %s"), header->getRptCall1().c_str());
						else
							repeater->processRepeater(*header);

						delete header;
					}
				}
				break;

			case RT_AMBE: {
					CAMBEData* data = handler->readAMBE();
					if (data != NULL) {
						CRepeaterHandler* repeater = CRepeaterHandler::findDVRepeater(*data, false);
						if (repeater != NULL)
							repeater->processRepeater(*data);

						delete data;
					}
				}
				break;

			case RT_BUSY_HEADER: {
					CHeaderData* header = handler->readBusyHeader();
					if (header != NULL) {
						// wxLogMessage(wxT("Repeater busy header - My: %s/%s  Your: %s  Rpt1: %s  Rpt2: %s  Flags: %02X %02X %02X"), header->getMyCall1().c_str(), header->getMyCall2().c_str(), header->getYourCall().c_str(), header->getRptCall1().c_str(), header->getRptCall2().c_str(), header->getFlag1(), header->getFlag2(), header->getFlag3());

						CRepeaterHandler* repeater = CRepeaterHandler::findDVRepeater(*header);
						if (repeater == NULL)
							wxLogMessage(wxT("Busy header received from unknown repeater, %s"), header->getRptCall1().c_str());
						else
							repeater->processBusy(*header);

						delete header;
					}
				}
				break;

			case RT_BUSY_AMBE: {
					CAMBEData* data = handler->readBusyAMBE();
					if (data != NULL) {
						CRepeaterHandler* repeater = CRepeaterHandler::findDVRepeater(*data, true);
						if (repeater != NULL)
							repeater->processBusy(*data);

						delete data;
					}
				}
				break;

			case RT_DD: {
					CDDData* data = handler->readDD();
					if (data != NULL) {
						// wxLogMessage(wxT("DD header - My: %s/%s  Your: %s  Rpt1: %s  Rpt2: %s  Flags: %02X %02X %02X"), data->getMyCall1().c_str(), data->getMyCall2().c_str(), data->getYourCall().c_str(), data->getRptCall1().c_str(), data->getRptCall2().c_str(), data->getFlag1(), data->getFlag2(), data->getFlag3());

						CRepeaterHandler* repeater = CRepeaterHandler::findDDRepeater();
						if (repeater == NULL)
							wxLogMessage(wxT("DD data received from unknown DD repeater, %s"), data->getRptCall1().c_str());
						else
							repeater->processRepeater(*data);

						delete data;
					}
				}
				break;

			default:
				return;
		}
	}
}

void CIRCDDBGatewayThread::processDExtra()
{
	for (;;) {
		DEXTRA_TYPE type = m_dextraPool->read();

		switch (type) {
			case DE_POLL: {
					CPollData* poll = m_dextraPool->readPoll();
					if (poll != NULL) {
						CDExtraHandler::process(*poll);
						delete poll;
					}
				}
				break;

			case DE_CONNECT: {
					CConnectData* connect = m_dextraPool->readConnect();
					if (connect != NULL) {
						CDExtraHandler::process(*connect);
						delete connect;
					}
				}
				break;

			case DE_HEADER: {
					CHeaderData* header = m_dextraPool->readHeader();
					if (header != NULL) {
						// wxLogMessage(wxT("DExtra header - My: %s/%s  Your: %s  Rpt1: %s  Rpt2: %s"), header->getMyCall1().c_str(), header->getMyCall2().c_str(), header->getYourCall().c_str(), header->getRptCall1().c_str(), header->getRptCall2().c_str());
						CDExtraHandler::process(*header);
						delete header;
					}
				}
				break;

			case DE_AMBE: {
					CAMBEData* data = m_dextraPool->readAMBE();
					if (data != NULL) {
						CDExtraHandler::process(*data);
						delete data;
					}
				}
				break;

			default:
				return;
		}
	}
}

void CIRCDDBGatewayThread::processDPlus()
{
	for (;;) {
		DPLUS_TYPE type = m_dplusPool->read();

		switch (type) {
			case DP_POLL: {
					CPollData* poll = m_dplusPool->readPoll();
					if (poll != NULL) {
						CDPlusHandler::process(*poll);
						delete poll;
					}
				}
				break;

			case DP_CONNECT: {
					CConnectData* connect = m_dplusPool->readConnect();
					if (connect != NULL) {
						CDPlusHandler::process(*connect);
						delete connect;
					}
				}
				break;

			case DP_HEADER: {
					CHeaderData* header = m_dplusPool->readHeader();
					if (header != NULL) {
						// wxLogMessage(wxT("D-Plus header - My: %s/%s  Your: %s  Rpt1: %s  Rpt2: %s"), header->getMyCall1().c_str(), header->getMyCall2().c_str(), header->getYourCall().c_str(), header->getRptCall1().c_str(), header->getRptCall2().c_str());
						CDPlusHandler::process(*header);
						delete header;
					}
				}
				break;

			case DP_AMBE: {
					CAMBEData* data = m_dplusPool->readAMBE();
					if (data != NULL) {
						CDPlusHandler::process(*data);
						delete data;
					}
				}
				break;

			default:
				return;
		}
	}
}

void CIRCDDBGatewayThread::processDCS()
{
	for (;;) {
		DCS_TYPE type = m_dcsPool->read();

		switch (type) {
			case DC_POLL: {
					CPollData* poll = m_dcsPool->readPoll();
					if (poll != NULL) {
						CDCSHandler::process(*poll);
						delete poll;
					}
				}
				break;

			case DC_CONNECT: {
					CConnectData* connect = m_dcsPool->readConnect();
					if (connect != NULL) {
						CDCSHandler::process(*connect);
						delete connect;
					}
				}
				break;

			case DC_DATA: {
					CAMBEData* data = m_dcsPool->readData();
					if (data != NULL) {
						// wxLogMessage(wxT("DCS header - My: %s/%s  Your: %s  Rpt1: %s  Rpt2: %s"), header->getMyCall1().c_str(), header->getMyCall2().c_str(), header->getYourCall().c_str(), header->getRptCall1().c_str(), header->getRptCall2().c_str());
						CDCSHandler::process(*data);
						delete data;
					}
				}
				break;

			default:
				return;
		}
	}
}

void CIRCDDBGatewayThread::processG2()
{
	for (;;) {
		G2_TYPE type = m_g2Handler->read();

		switch (type) {
			case GT_HEADER: {
					CHeaderData* header = m_g2Handler->readHeader();
					if (header != NULL) {
						// wxLogMessage(wxT("G2 header - My: %s/%s  Your: %s  Rpt1: %s  Rpt2: %s  Flags: %02X %02X %02X"), header->getMyCall1().c_str(), header->getMyCall2().c_str(), header->getYourCall().c_str(), header->getRptCall1().c_str(), header->getRptCall2().c_str(), header->getFlag1(), header->getFlag2(), header->getFlag3());
						CG2Handler::process(*header);
						delete header;
					}
				}
				break;

			case GT_AMBE: {
					CAMBEData* data = m_g2Handler->readAMBE();
					if (data != NULL) {
						CG2Handler::process(*data);
						delete data;
					}
				}
				break;

			default:
				return;
		}
	}
}

void CIRCDDBGatewayThread::processDD()
{
	for (;;) {
		CDDData* data = CDDHandler::read();
		if (data == NULL)
			return;

		// wxLogMessage(wxT("DD header - My: %s/%s  Your: %s  Rpt1: %s  Rpt2: %s  Flags: %02X %02X %02X"), data->getMyCall1().c_str(), data->getMyCall2().c_str(), data->getYourCall().c_str(), data->getRptCall1().c_str(), data->getRptCall2().c_str(), data->getFlag1(), data->getFlag2(), data->getFlag3());

		delete data;
	}
}

void CIRCDDBGatewayThread::loadGateways()
{
	wxFileName fileName(wxFileName::GetHomeDir(), GATEWAY_HOSTS_FILE_NAME);
	if (!fileName.IsFileReadable())
		return;

	unsigned int count = 0U;

	CHostFile hostFile(fileName.GetFullPath(), false);
	for (unsigned int i = 0U; i < hostFile.getCount(); i++) {
		wxString gateway = hostFile.getName(i);
		in_addr address  = CUDPReaderWriter::lookup(hostFile.getAddress(i));

		if (address.s_addr != INADDR_NONE) {
			unsigned char* ucp = (unsigned char*)&address;

			wxString addrText;
			addrText.Printf(wxT("%u.%u.%u.%u"), ucp[0U] & 0xFFU, ucp[1U] & 0xFFU, ucp[2U] & 0xFFU, ucp[3U] & 0xFFU);

			wxLogMessage(wxT("Locking %s to %s"), gateway.c_str(), addrText.c_str());

			gateway.Append(wxT("        "));
			gateway.Truncate(LONG_CALLSIGN_LENGTH - 1U);
			gateway.Append(wxT("G"));
			m_cache.updateGateway(gateway, addrText, DP_DEXTRA, true, false);

			count++;
		}
	}

	wxLogMessage(wxT("Loaded %u of %u gateways from %s"), count, hostFile.getCount(), fileName.GetFullPath().c_str());
}

void CIRCDDBGatewayThread::loadReflectors()
{
	if (m_dplusEnabled) {
		wxFileName fileName(wxFileName::GetHomeDir(), DPLUS_HOSTS_FILE_NAME);
		if (fileName.IsFileReadable())
			loadDPlusReflectors(fileName.GetFullPath());

#if defined(__WINDOWS__)
		fileName.Assign(::wxGetCwd(), DPLUS_HOSTS_FILE_NAME);
#else
		fileName.Assign(wxT(DATA_DIR), DPLUS_HOSTS_FILE_NAME);
#endif
		if (fileName.IsFileReadable())
			loadDPlusReflectors(fileName.GetFullPath());
	}

	if (m_dextraEnabled) {
		wxFileName fileName(wxFileName::GetHomeDir(), DEXTRA_HOSTS_FILE_NAME);
		if (fileName.IsFileReadable())
			loadDExtraReflectors(fileName.GetFullPath());

#if defined(__WINDOWS__)
		fileName.Assign(::wxGetCwd(), DEXTRA_HOSTS_FILE_NAME);
#else
		fileName.Assign(wxT(DATA_DIR), DEXTRA_HOSTS_FILE_NAME);
#endif
		if (fileName.IsFileReadable())
			loadDExtraReflectors(fileName.GetFullPath());
	}

	if (m_dcsEnabled) {
		wxFileName fileName(wxFileName::GetHomeDir(), DCS_HOSTS_FILE_NAME);
		if (fileName.IsFileReadable())
			loadDCSReflectors(fileName.GetFullPath());

#if defined(__WINDOWS__)
		fileName.Assign(::wxGetCwd(), DCS_HOSTS_FILE_NAME);
#else
		fileName.Assign(wxT(DATA_DIR), DCS_HOSTS_FILE_NAME);
#endif
		if (fileName.IsFileReadable())
			loadDCSReflectors(fileName.GetFullPath());
	}
}

void CIRCDDBGatewayThread::loadDExtraReflectors(const wxString& fileName)
{
	unsigned int count = 0U;

	CHostFile hostFile(fileName, false);
	for (unsigned int i = 0U; i < hostFile.getCount(); i++) {
		wxString reflector = hostFile.getName(i);
		in_addr address    = CUDPReaderWriter::lookup(hostFile.getAddress(i));
		bool lock          = hostFile.getLock(i);

		if (address.s_addr != INADDR_NONE) {
			unsigned char* ucp = (unsigned char*)&address;

			wxString addrText;
			addrText.Printf(wxT("%u.%u.%u.%u"), ucp[0U] & 0xFFU, ucp[1U] & 0xFFU, ucp[2U] & 0xFFU, ucp[3U] & 0xFFU);

			if (lock)
				wxLogMessage(wxT("Locking %s to %s"), reflector.c_str(), addrText.c_str());

			reflector.Append(wxT("        "));
			reflector.Truncate(LONG_CALLSIGN_LENGTH - 1U);
			reflector.Append(wxT("G"));
			m_cache.updateGateway(reflector, addrText, DP_DEXTRA, lock, true);

			count++;
		}
	}

	wxLogMessage(wxT("Loaded %u of %u DExtra reflectors from %s"), count, hostFile.getCount(), fileName.c_str());
}

void CIRCDDBGatewayThread::loadDPlusReflectors(const wxString& fileName)
{
	unsigned int count = 0U;

	CHostFile hostFile(fileName, false);
	for (unsigned int i = 0U; i < hostFile.getCount(); i++) {
		wxString reflector = hostFile.getName(i);
		in_addr address    = CUDPReaderWriter::lookup(hostFile.getAddress(i));
		bool lock          = hostFile.getLock(i);

		if (address.s_addr != INADDR_NONE) {
			unsigned char* ucp = (unsigned char*)&address;

			wxString addrText;
			addrText.Printf(wxT("%u.%u.%u.%u"), ucp[0U] & 0xFFU, ucp[1U] & 0xFFU, ucp[2U] & 0xFFU, ucp[3U] & 0xFFU);

			if (lock)
				wxLogMessage(wxT("Locking %s to %s"), reflector.c_str(), addrText.c_str());

			reflector.Append(wxT("        "));
			reflector.Truncate(LONG_CALLSIGN_LENGTH - 1U);
			reflector.Append(wxT("G"));
			m_cache.updateGateway(reflector, addrText, DP_DPLUS, lock, true);

			count++;
		}
	}

	wxLogMessage(wxT("Loaded %u of %u D-Plus reflectors from %s"), count, hostFile.getCount(), fileName.c_str());
}

void CIRCDDBGatewayThread::loadDCSReflectors(const wxString& fileName)
{
	unsigned int count = 0U;

	CHostFile hostFile(fileName, false);
	for (unsigned int i = 0U; i < hostFile.getCount(); i++) {
		wxString reflector = hostFile.getName(i);
		in_addr address    = CUDPReaderWriter::lookup(hostFile.getAddress(i));
		bool lock          = hostFile.getLock(i);

		if (address.s_addr != INADDR_NONE) {
			unsigned char* ucp = (unsigned char*)&address;

			wxString addrText;
			addrText.Printf(wxT("%u.%u.%u.%u"), ucp[0U] & 0xFFU, ucp[1U] & 0xFFU, ucp[2U] & 0xFFU, ucp[3U] & 0xFFU);

			if (lock)
				wxLogMessage(wxT("Locking %s to %s"), reflector.c_str(), addrText.c_str());

			reflector.Append(wxT("        "));
			reflector.Truncate(LONG_CALLSIGN_LENGTH - 1U);
			reflector.Append(wxT("G"));
			m_cache.updateGateway(reflector, addrText, DP_DCS, lock, true);

			count++;
		}
	}

	wxLogMessage(wxT("Loaded %u of %u DCS reflectors from %s"), count, hostFile.getCount(), fileName.c_str());
}
	
void CIRCDDBGatewayThread::writeStatus()
{
	wxString fullName = LINKS_BASE_NAME;

	if (!m_name.IsEmpty()) {
		fullName.Append(wxT("_"));
		fullName.Append(m_name);
	}

	wxFileName fileName(m_logDir, fullName, wxT("log"));

	wxFFile file;
	bool ret = file.Open(fileName.GetFullPath(), wxT("wt"));
	if (!ret) {
		wxLogError(wxT("Unable to open %s for writing"), fileName.GetFullPath().c_str());
		return;
	}

	CDExtraHandler::writeStatus(file);
	CDPlusHandler::writeStatus(file);
	CDCSHandler::writeStatus(file);
	CCCSHandler::writeStatus(file);

	file.Close();
}

CIRCDDBGatewayStatusData* CIRCDDBGatewayThread::getStatus() const
{
	bool aprsStatus = false;
	if (m_aprsWriter != NULL)
		aprsStatus = m_aprsWriter->isConnected();

	CIRCDDBGatewayStatusData* status = new CIRCDDBGatewayStatusData(m_lastStatus, aprsStatus);

	for (unsigned int i = 0U; i < 4U; i++) {
		wxString callsign, linkCallsign;
		LINK_STATUS linkStatus;
		bool ret = CRepeaterHandler::getRepeater(i, callsign, linkStatus, linkCallsign);
		if (ret) {
			wxString incoming1 = CDExtraHandler::getIncoming(callsign);
			wxString incoming2 = CDCSHandler::getIncoming(callsign);
			wxString incoming3 = CCCSHandler::getIncoming(callsign);

			wxString incoming;
			if (!incoming1.IsEmpty()) {
				incoming.Append(incoming1);
				incoming.Append(wxT(" "));
			}
			if (!incoming2.IsEmpty()) {
				incoming.Append(incoming2);
				incoming.Append(wxT(" "));
			}
			if (!incoming3.IsEmpty()) {
				incoming.Append(incoming3);
				incoming.Append(wxT(" "));
			}

			status->setRepeater(i, callsign, linkStatus, linkCallsign, incoming);
		}
	}

	wxString dongles;
	dongles.Append(CDExtraHandler::getDongles());
	dongles.Append(CDPlusHandler::getDongles());
	status->setDongles(dongles);

	return status;
}

void CIRCDDBGatewayThread::readStatusFiles()
{
	readStatusFile(STATUS1_FILE_NAME, 0U, m_status1);
	readStatusFile(STATUS2_FILE_NAME, 1U, m_status2);
	readStatusFile(STATUS3_FILE_NAME, 2U, m_status3);
	readStatusFile(STATUS4_FILE_NAME, 3U, m_status4);
	readStatusFile(STATUS5_FILE_NAME, 4U, m_status5);
}

void CIRCDDBGatewayThread::readStatusFile(const wxString& filename, unsigned int n, wxString& var)
{
	wxFileName name(wxFileName::GetHomeDir(), filename);

	wxString text;

	bool res = wxFile::Exists(name.GetFullPath());
	if (res) {
		wxTextFile textFile(name.GetFullPath());
		bool res = textFile.Open();
		if (res) {
			text = textFile.GetFirstLine();
			textFile.Close();
		}
	}

	if (!var.IsSameAs(text)) {
		wxLogMessage(wxT("Status %u message set to \"%s\""), n + 1U, text.c_str());
		CStatusData statusData(text, n);
		CRepeaterHandler::writeStatus(statusData);
		var = text;
	}
}
