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

#include "StarNetServerThread.h"
#include "StarNetServerDefs.h"
#include "StarNetHandler.h"
#include "DExtraHandler.h"			// DEXTRA LINK
#include "DCSHandler.h"				// DCS LINK
#include "HeaderData.h"
#include "G2Handler.h"
#include "AMBEData.h"
#include "HostFile.h"				// DEXTRA_LINK || DCS_LINK
#include "Utils.h"

#include <wx/filename.h>
#include <wx/ffile.h>

const unsigned int REMOTE_DUMMY_PORT = 65015U;

CStarNetServerThread::CStarNetServerThread(bool nolog, const wxString& logDir) :
m_nolog(nolog),
m_logDir(logDir),
m_killed(false),
m_stopped(true),
m_callsign(),
m_address(),
#if defined(DEXTRA_LINK)
m_dextraPool(NULL),
#endif
#if defined(DCS_LINK)
m_dcsPool(NULL),
#endif
m_g2Handler(NULL),
m_irc(NULL),
m_cache(),
m_logEnabled(false),
m_statusTimer(1000U, 1U),		// 1 second
m_lastStatus(IS_DISCONNECTED),
m_remoteEnabled(false),
m_remotePassword(),
m_remotePort(0U),
m_remote(NULL)
{
	CHeaderData::initialise();
	CG2Handler::initialise(MAX_ROUTES);
	CStarNetHandler::initialise(MAX_STARNETS);
#if defined(DEXTRA_LINK)
	CDExtraHandler::initialise(MAX_DEXTRA_LINKS);
#endif
#if defined(DCS_LINK)
	CDCSHandler::initialise(MAX_DCS_LINKS);
#endif
}

CStarNetServerThread::~CStarNetServerThread()
{
	CHeaderData::finalise();
	CG2Handler::finalise();
	CStarNetHandler::finalise();
#if defined(DEXTRA_LINK)
	CDExtraHandler::finalise();
#endif
#if defined(DCS_LINK)
	CDCSHandler::finalise();
#endif
}

void CStarNetServerThread::run()
{
	// Truncate the old StarNet.log file
	wxFileName fileName(m_logDir, STARNET_BASE_NAME, wxT("log"));
	wxLogMessage(wxT("Truncating %s"), fileName.GetFullPath().c_str());

	wxFFile file;
	bool ret = file.Open(fileName.GetFullPath(), wxT("wt"));
	if (ret)
		file.Close();

#if defined(DEXTRA_LINK)
	m_dextraPool = new CDExtraProtocolHandlerPool(MAX_DEXTRA_LINKS, DEXTRA_PORT, m_address);
	ret = m_dextraPool->open();
	if (!ret) {
		wxLogError(wxT("Could not open the DExtra protocol pool"));
		delete m_dextraPool;
		m_dextraPool = NULL;
	}
#endif

#if defined(DCS_LINK)
	m_dcsPool = new CDCSProtocolHandlerPool(MAX_DCS_LINKS, DCS_PORT, m_address);
	ret = m_dcsPool->open();
	if (!ret) {
		wxLogError(wxT("Could not open the DCS protocol pool"));
		delete m_dcsPool;
		m_dcsPool = NULL;
	}
#endif

	m_g2Handler = new CG2ProtocolHandler(G2_DV_PORT, m_address);
	ret = m_g2Handler->open();
	if (!ret) {
		wxLogError(wxT("Could not open the G2 protocol handler"));
		delete m_g2Handler;
		m_g2Handler = NULL;
	}

	// Wait here until we have the essentials to run
#if defined(DEXTRA_LINK)
	while (!m_killed && (m_g2Handler == NULL || m_dextraPool == NULL || m_irc == NULL || m_callsign.IsEmpty()))
		::wxMilliSleep(500UL);		// 1/2 sec
#elif defined(DCS_LINK)
	while (!m_killed && (m_g2Handler == NULL || m_dcsPool == NULL || m_irc == NULL || m_callsign.IsEmpty()))
		::wxMilliSleep(500UL);		// 1/2 sec
#else
	while (!m_killed && (m_g2Handler == NULL || m_irc == NULL || m_callsign.IsEmpty()))
		::wxMilliSleep(500UL);		// 1/2 sec
#endif

	if (m_killed)
		return;

	m_stopped = false;

	wxLogMessage(wxT("Starting the StarNet Server thread"));

	CHeaderLogger* headerLogger = NULL;
	if (m_logEnabled) {
		headerLogger = new CHeaderLogger(m_logDir);
		bool ret = headerLogger->open();
		if (!ret) {
			delete headerLogger;
			headerLogger = NULL;
		}
	}

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
	loadReflectors();
#endif

	CG2Handler::setG2ProtocolHandler(m_g2Handler);
	CG2Handler::setHeaderLogger(headerLogger);

#if defined(DEXTRA_LINK)
	CDExtraHandler::setCallsign(m_callsign);
	CDExtraHandler::setDExtraProtocolHandlerPool(m_dextraPool);
	CDExtraHandler::setHeaderLogger(headerLogger);
#endif
#if defined(DCS_LINK)
	CDCSHandler::setDCSProtocolHandlerPool(m_dcsPool);
	CDCSHandler::setHeaderLogger(headerLogger);
	CDCSHandler::setGatewayType(GT_STARNET);
#endif

	CStarNetHandler::setCache(&m_cache);
	CStarNetHandler::setGateway(m_callsign);
	CStarNetHandler::setG2Handler(m_g2Handler);
	CStarNetHandler::setIRC(m_irc);
	CStarNetHandler::setLogging(m_logEnabled, m_logDir);
#if defined(DEXTRA_LINK) || defined(DCS_LINK)
	CStarNetHandler::link();
#endif

	if (m_remoteEnabled && !m_remotePassword.IsEmpty() && m_remotePort > 0U) {
		m_remote = new CRemoteHandler(m_remotePassword, m_remotePort);
		bool res = m_remote->open();
		if (!res) {
			delete m_remote;
			m_remote = NULL;
		}
	}

	wxStopWatch stopWatch;
	stopWatch.Start();

	m_statusTimer.start();

	try {
		while (!m_killed) {
			processIrcDDB();
			processG2();
#if defined(DEXTRA_LINK)
			processDExtra();
#endif
#if defined(DCS_LINK)
			processDCS();
#endif
			if (m_remote != NULL)
				m_remote->process();

			unsigned long ms = stopWatch.Time();
			stopWatch.Start();

			m_statusTimer.clock(ms);

			CG2Handler::clock(ms);
			CStarNetHandler::clock(ms);
#if defined(DEXTRA_LINK)
			CDExtraHandler::clock(ms);
#endif
#if defined(DCS_LINK)
			CDCSHandler::clock(ms);
#endif

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

	wxLogMessage(wxT("Stopping the StarNet Server thread"));

#if defined(DEXTRA_LINK)
	// Unlink from all reflectors
	CDExtraHandler::unlink();

	m_dextraPool->close();
	delete m_dextraPool;
#endif

#if defined(DCS_LINK)
	// Unlink from all reflectors
	CDCSHandler::unlink();

	m_dcsPool->close();
	delete m_dcsPool;
#endif

	m_g2Handler->close();
	delete m_g2Handler;

	m_irc->close();
	delete m_irc;

	if (m_remote != NULL) {
		m_remote->close();
		delete m_remote;
	}

	if (headerLogger != NULL) {
		headerLogger->close();
		delete headerLogger;
	}
}

void CStarNetServerThread::kill()
{
	m_killed = true;
}

void CStarNetServerThread::setCallsign(const wxString& callsign)
{
	if (!m_stopped)
		return;

	m_callsign = callsign;
}

void CStarNetServerThread::setAddress(const wxString& address)
{
	m_address = address;
}

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
void CStarNetServerThread::addStarNet(const wxString& callsign, const wxString& logoff, const wxString& repeater, const wxString& infoText, const wxString& permanent, unsigned int userTimeout, unsigned int groupTimeout, STARNET_CALLSIGN_SWITCH callsignSwitch, bool txMsgSwitch, const wxString& reflector)
{
	CStarNetHandler::add(callsign, logoff, repeater, infoText, permanent, userTimeout, groupTimeout, callsignSwitch, txMsgSwitch, reflector);
}
#else
void CStarNetServerThread::addStarNet(const wxString& callsign, const wxString& logoff, const wxString& repeater, const wxString& infoText, const wxString& permanent, unsigned int userTimeout, unsigned int groupTimeout, STARNET_CALLSIGN_SWITCH callsignSwitch, bool txMsgSwitch)
{
	CStarNetHandler::add(callsign, logoff, repeater, infoText, permanent, userTimeout, groupTimeout, callsignSwitch, txMsgSwitch);
}
#endif

void CStarNetServerThread::setIRC(CIRCDDB* irc)
{
	wxASSERT(irc != NULL);

	m_irc = irc;
}

void CStarNetServerThread::setLog(bool enabled)
{
	m_logEnabled = enabled;
}

void CStarNetServerThread::setRemote(bool enabled, const wxString& password, unsigned int port)
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

void CStarNetServerThread::processIrcDDB()
{
	// Once per second
	if (m_statusTimer.hasExpired()) {
		int status = m_irc->getConnectionState();
		switch (status) {
			case 0:
			case 10:
				if (m_lastStatus != IS_DISCONNECTED) {
					wxLogInfo(wxT("Disconnected from ircDDB"));
					m_lastStatus = IS_DISCONNECTED;
				}
				break;
			case 7:
				if (m_lastStatus != IS_CONNECTED) {
					wxLogInfo(wxT("Connected to ircDDB"));
					m_lastStatus = IS_CONNECTED;
				}
				break;
			default:
				if (m_lastStatus != IS_CONNECTING) {
					wxLogInfo(wxT("Connecting to ircDDB"));
					m_lastStatus = IS_CONNECTING;
				}
				break;
		}

		m_statusTimer.start();
	}

	// Process all incoming ircDDB messages, updating the caches
	for (;;) {
		IRCDDB_RESPONSE_TYPE type = m_irc->getMessageType();

		switch (type) {
			case IDRT_NONE:
				return;

			case IDRT_USER: {
					wxString user, repeater, gateway, address;
					bool res = m_irc->receiveUser(user, repeater, gateway, address);
					if (!res)
						break;

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

#if defined(DEXTRA_LINK)
					CDExtraHandler::gatewayUpdate(gateway, address);
#endif
#if defined(DCS_LINK)
					CDCSHandler::gatewayUpdate(gateway, address);
#endif

					if (!address.IsEmpty()) {
						wxLogMessage(wxT("GATEWAY: %s %s"), gateway.c_str(), address.c_str());
						m_cache.updateGateway(gateway, address, DP_DEXTRA, false, false);
					} else {
						wxLogMessage(wxT("GATEWAY: %s NOT FOUND"), gateway.c_str());
					}
				}
				break;
		}
	}
}

#if defined(DEXTRA_LINK)
void CStarNetServerThread::processDExtra()
{
	for (;;) {
		DEXTRA_TYPE type = m_dextraPool->read();

		switch (type) {
			case DE_NONE:
				return;

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
		}
	}
}
#endif

#if defined(DCS_LINK)
void CStarNetServerThread::processDCS()
{
	for (;;) {
		DCS_TYPE type = m_dcsPool->read();

		switch (type) {
			case DC_NONE:
				return;

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
		}
	}
}
#endif

void CStarNetServerThread::processG2()
{
	for (;;) {
		G2_TYPE type = m_g2Handler->read();

		switch (type) {
			case GT_NONE:
				return;

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
		}
	}
}

#if defined(DEXTRA_LINK)
void CStarNetServerThread::loadReflectors()
{
	wxFileName fileName(wxFileName::GetHomeDir(), DEXTRA_HOSTS_FILE_NAME);

	if (!fileName.IsFileReadable()) {
		wxLogMessage(wxT("File %s not readable"), fileName.GetFullPath().c_str());
#if defined(__WINDOWS__)
		fileName.Assign(::wxGetCwd(), DEXTRA_HOSTS_FILE_NAME);
#else
		fileName.Assign(wxT(DATA_DIR), DEXTRA_HOSTS_FILE_NAME);
#endif
		if (!fileName.IsFileReadable())
			wxLogMessage(wxT("File %s not readable"), fileName.GetFullPath().c_str());
	}

	unsigned int count = 0U;

	CHostFile hostFile(fileName.GetFullPath(), true);
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

	wxLogMessage(wxT("Loaded %u of %u DExtra reflectors"), count, hostFile.getCount());
}
#endif

#if defined(DCS_LINK)
void CStarNetServerThread::loadReflectors()
{
	wxFileName fileName(wxFileName::GetHomeDir(), DCS_HOSTS_FILE_NAME);

	if (!fileName.IsFileReadable()) {
		wxLogMessage(wxT("File %s not readable"), fileName.GetFullPath().c_str());
#if defined(__WINDOWS__)
		fileName.Assign(::wxGetCwd(), DCS_HOSTS_FILE_NAME);
#else
		fileName.Assign(wxT(DATA_DIR), DCS_HOSTS_FILE_NAME);
#endif
		if (!fileName.IsFileReadable())
			wxLogMessage(wxT("File %s not readable"), fileName.GetFullPath().c_str());
	}

	unsigned int count = 0U;

	CHostFile hostFile(fileName.GetFullPath(), true);
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

	wxLogMessage(wxT("Loaded %u of %u DCS reflectors"), count, hostFile.getCount());
}
#endif
