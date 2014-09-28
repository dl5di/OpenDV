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

#include "SlowDataEncoder.h"
#include "RepeaterHandler.h"
#include "StarNetHandler.h"
#include "DExtraHandler.h"		// DEXTRA_LINK
#include "DStarDefines.h"
#include "DCSHandler.h"			// DCS_LINK

#include <wx/filename.h>
#include <wx/tokenzr.h>
#include <vector>

const unsigned int MESSAGE_DELAY = 4U;

unsigned int        CStarNetHandler::m_maxStarNets = 0U;
CStarNetHandler**   CStarNetHandler::m_starNets = NULL;

CG2ProtocolHandler* CStarNetHandler::m_g2Handler = NULL;
CIRCDDB*            CStarNetHandler::m_irc = NULL;
CCacheManager*      CStarNetHandler::m_cache = NULL;
wxString            CStarNetHandler::m_gateway;

wxString            CStarNetHandler::m_name;
wxFFile*            CStarNetHandler::m_logFile = NULL;


CStarNetUser::CStarNetUser(const wxString &callsign, unsigned int timeout) :
m_callsign(callsign),
m_timer(1000U, timeout)
{
	m_timer.start();
}

CStarNetUser::~CStarNetUser()
{
}

bool CStarNetUser::clock(unsigned int ms)
{
	m_timer.clock(ms);

	return m_timer.isRunning() && m_timer.hasExpired();
}

bool CStarNetUser::hasExpired()
{
	return m_timer.isRunning() && m_timer.hasExpired();
}

void CStarNetUser::reset()
{
	m_timer.start();
}

wxString CStarNetUser::getCallsign() const
{
	return m_callsign;
}

CTimer CStarNetUser::getTimer() const
{
	return m_timer;
}

CStarNetId::CStarNetId(unsigned int id, unsigned int timeout, CStarNetUser* user) :
m_id(id),
m_timer(1000U, timeout),
m_login(false),
m_info(false),
m_logoff(false),
m_end(false),
m_user(user),
m_textCollector()
{
	wxASSERT(user != NULL);

	m_timer.start();
}

CStarNetId::~CStarNetId()
{
}

unsigned int CStarNetId::getId() const
{
	return m_id;
}

void CStarNetId::reset()
{
	m_timer.start();
}

void CStarNetId::setLogin()
{
	m_login = true;
}

void CStarNetId::setInfo()
{
	if (!m_login && !m_logoff)
		m_info = true;
}

void CStarNetId::setLogoff()
{
	if (!m_login && !m_info)
		m_logoff = true;
}

void CStarNetId::setEnd()
{
	m_end = true;
}

bool CStarNetId::clock(unsigned int ms)
{
	m_timer.clock(ms);

	return m_timer.isRunning() && m_timer.hasExpired();
}

bool CStarNetId::hasExpired()
{
	return m_timer.isRunning() && m_timer.hasExpired();
}

bool CStarNetId::isLogin() const
{
	return m_login;
}

bool CStarNetId::isInfo() const
{
	return m_info;
}

bool CStarNetId::isLogoff() const
{
	return m_logoff;
}

bool CStarNetId::isEnd() const
{
	return m_end;
}

CStarNetUser* CStarNetId::getUser() const
{
	return m_user;
}

CTextCollector& CStarNetId::getTextCollector()
{
	return m_textCollector;
}

void CStarNetHandler::initialise(unsigned int maxStarNets, const wxString& name)
{
	wxASSERT(maxStarNets > 0U);

	m_maxStarNets = maxStarNets;
	m_name        = name;

	m_starNets = new CStarNetHandler*[maxStarNets];

	for (unsigned int i = 0U; i < maxStarNets; i++)
		m_starNets[i] = NULL;
}

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
void CStarNetHandler::add(const wxString& callsign, const wxString& logoff, const wxString& repeater, const wxString& infoText, const wxString& permanent, unsigned int userTimeout, unsigned int groupTimeout, STARNET_CALLSIGN_SWITCH callsignSwitch, bool txMsgSwitch, const wxString& reflector)
{
	CStarNetHandler* starNet = new CStarNetHandler(callsign, logoff, repeater, infoText, permanent, userTimeout, groupTimeout, callsignSwitch, txMsgSwitch, reflector);

	for (unsigned int i = 0U; i < m_maxStarNets; i++) {
		if (m_starNets[i] == NULL) {
			m_starNets[i] = starNet;
			return;
		}
	}

	wxLogError(wxT("Cannot add StarNet group with callsign %s, no space"), callsign.c_str());

	delete starNet;
}
#else
void CStarNetHandler::add(const wxString& callsign, const wxString& logoff, const wxString& repeater, const wxString& infoText, const wxString& permanent, unsigned int userTimeout, unsigned int groupTimeout, STARNET_CALLSIGN_SWITCH callsignSwitch, bool txMsgSwitch)
{
	CStarNetHandler* starNet = new CStarNetHandler(callsign, logoff, repeater, infoText, permanent, userTimeout, groupTimeout, callsignSwitch, txMsgSwitch);

	for (unsigned int i = 0U; i < m_maxStarNets; i++) {
		if (m_starNets[i] == NULL) {
			m_starNets[i] = starNet;
			return;
		}
	}

	wxLogError(wxT("Cannot add StarNet group with callsign %s, no space"), callsign.c_str());

	delete starNet;
}
#endif

void CStarNetHandler::setG2Handler(CG2ProtocolHandler* handler)
{
	wxASSERT(handler != NULL);

	m_g2Handler = handler;
}

void CStarNetHandler::setIRC(CIRCDDB* irc)
{
	wxASSERT(irc != NULL);

	m_irc = irc;
}

void CStarNetHandler::setCache(CCacheManager* cache)
{
	wxASSERT(cache != NULL);

	m_cache = cache;
}

void CStarNetHandler::setGateway(const wxString& gateway)
{
	m_gateway = gateway;
}

void CStarNetHandler::setLogging(bool enable, const wxString& dir)
{
	if (!enable)
		return;

	wxString fullName = STARNET_BASE_NAME;

	if (!m_name.IsEmpty()) {
		fullName.Append(wxT("_"));
		fullName.Append(m_name);
	}

	wxFileName fileName(dir, fullName, wxT("log"));

	m_logFile = new wxFFile;
	bool ret = m_logFile->Open(fileName.GetFullPath(), wxT("wt"));
	if (!ret) {
		wxLogError(wxT("Unable to open %s for writing"), fileName.GetFullPath().c_str());
		delete m_logFile;
		m_logFile = NULL;
	}
}

CStarNetHandler* CStarNetHandler::findStarNet(const wxString& callsign)
{
	for (unsigned int i = 0U; i < m_maxStarNets; i++) {
		CStarNetHandler* starNet = m_starNets[i];
		if (starNet != NULL) {
			if (starNet->m_groupCallsign.IsSameAs(callsign))
				return starNet;
		}
	}

	return NULL;
}

CStarNetHandler* CStarNetHandler::findStarNet(const CHeaderData& header)
{
	wxString your = header.getYourCall();

	for (unsigned int i = 0U; i < m_maxStarNets; i++) {
		CStarNetHandler* starNet = m_starNets[i];
		if (starNet != NULL) {
			if (starNet->m_groupCallsign.IsSameAs(your))
				return starNet;
			if (starNet->m_offCallsign.IsSameAs(your))
				return starNet;
		}
	}

	return NULL;
}

CStarNetHandler* CStarNetHandler::findStarNet(const CAMBEData& data)
{
	unsigned int id = data.getId();

	for (unsigned int i = 0U; i < m_maxStarNets; i++) {
		CStarNetHandler* starNet = m_starNets[i];
		if (starNet != NULL) {
			if (starNet->m_id == id)
				return starNet;
		}
	}

	return NULL;
}

wxArrayString CStarNetHandler::listStarNets()
{
	wxArrayString starNets;

	for (unsigned int i = 0U; i < m_maxStarNets; i++) {
		CStarNetHandler* starNet = m_starNets[i];
		if (starNet != NULL)
			starNets.Add(starNet->m_groupCallsign);
	}

	return starNets;
}

CRemoteStarNetGroup* CStarNetHandler::getInfo() const
{
	CRemoteStarNetGroup* data = new CRemoteStarNetGroup(m_groupCallsign, m_offCallsign, m_groupTimer.getTimer(), m_groupTimer.getTimeout());

	for (CStarNetUsersHashMap::const_iterator it = m_users.begin(); it != m_users.end(); ++it) {
		CStarNetUser* user = it->second;
		data->addUser(user->getCallsign(), user->getTimer().getTimer(), user->getTimer().getTimeout());
	}
	
	return data;
}
														  
void CStarNetHandler::finalise()
{
	for (unsigned int i = 0U; i < m_maxStarNets; i++)
		delete m_starNets[i];

	delete[] m_starNets;

	if (m_logFile != NULL) {
		m_logFile->Close();
		delete m_logFile;
	}
}

void CStarNetHandler::clock(unsigned int ms)
{
	for (unsigned int i = 0U; i < m_maxStarNets; i++) {
		if (m_starNets[i] != NULL)
			m_starNets[i]->clockInt(ms);
	}
}

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
void CStarNetHandler::link()
{
	for (unsigned int i = 0U; i < m_maxStarNets; i++) {
		if (m_starNets[i] != NULL)
			m_starNets[i]->linkInt();
	}
}
#endif

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
CStarNetHandler::CStarNetHandler(const wxString& callsign, const wxString& logoff, const wxString& repeater, const wxString& infoText, const wxString& permanent, unsigned int userTimeout, unsigned int groupTimeout, STARNET_CALLSIGN_SWITCH callsignSwitch, bool txMsgSwitch, const wxString& reflector) :
m_groupCallsign(callsign),
m_offCallsign(logoff),
m_shortCallsign(wxT("SNET")),
m_repeater(repeater),
m_infoText(infoText),
m_permanent(),
m_linkReflector(reflector),
m_linkGateway(),
m_linkStatus(LS_NONE),
m_linkTimer(1000U, NETWORK_TIMEOUT),
m_id(0x00U),
m_groupTimer(1000U, groupTimeout * 60U),
m_announceTimer(1000U, 2U * 60U),		// 2 minutes
m_userTimeout(userTimeout),
m_callsignSwitch(callsignSwitch),
m_txMsgSwitch(txMsgSwitch),
m_ids(),
m_users(),
m_repeaters()
{
	m_announceTimer.start();

	// Create the short version of the STARnet Group callsign
	wxString rest;
	if (m_groupCallsign.StartsWith(wxT("STN"), &rest)) {
		wxChar c = m_groupCallsign.GetChar(7U);
		if (c == wxT(' '))
			m_shortCallsign.Printf(wxT("S%s"), rest.Left(3U).c_str());
		else
			m_shortCallsign.Printf(wxT("%s%c"), rest.Left(3U).c_str(), c);
	}

	wxStringTokenizer tkn(permanent, wxT(","));
	while (tkn.HasMoreTokens()) {
		wxString callsign = tkn.GetNextToken();
		callsign.resize(LONG_CALLSIGN_LENGTH, wxT(' '));
		m_permanent.Add(callsign);
	}
}
#else
CStarNetHandler::CStarNetHandler(const wxString& callsign, const wxString& logoff, const wxString& repeater, const wxString& infoText, const wxString& permanent, unsigned int userTimeout, unsigned int groupTimeout, STARNET_CALLSIGN_SWITCH callsignSwitch, bool txMsgSwitch) :
m_groupCallsign(callsign),
m_offCallsign(logoff),
m_shortCallsign(wxT("SNET")),
m_repeater(repeater),
m_infoText(infoText),
m_permanent(),
m_id(0x00U),
m_groupTimer(1000U, groupTimeout * 60U),
m_announceTimer(1000U, 2U * 60U),		// 2 minutes
m_userTimeout(userTimeout),
m_callsignSwitch(callsignSwitch),
m_txMsgSwitch(txMsgSwitch),
m_ids(),
m_users(),
m_repeaters()
{
	m_announceTimer.start();

	// Create the short version of the STARnet Group callsign
	wxString rest;
	if (m_groupCallsign.StartsWith(wxT("STN"), &rest)) {
		wxChar c = m_groupCallsign.GetChar(7U);
		if (c == wxT(' '))
			m_shortCallsign.Printf(wxT("S%s"), rest.Left(3U).c_str());
		else
			m_shortCallsign.Printf(wxT("%s%c"), rest.Left(3U).c_str(), c);
	}

	wxStringTokenizer tkn(permanent, wxT(","));
	while (tkn.HasMoreTokens()) {
		wxString callsign = tkn.GetNextToken();
		callsign.resize(LONG_CALLSIGN_LENGTH, wxT(' '));
		m_permanent.Add(callsign);
	}
}
#endif

CStarNetHandler::~CStarNetHandler()
{
	for (CStarNetUsersHashMap::iterator it = m_users.begin(); it != m_users.end(); ++it)
		delete it->second;

	for (CStarNetRepeatersHashMap::iterator it = m_repeaters.begin(); it != m_repeaters.end(); ++it)
		delete it->second;

	m_users.clear();
	m_repeaters.clear();
}

void CStarNetHandler::process(CHeaderData &header)
{
	wxString my   = header.getMyCall1();
	wxString your = header.getYourCall();

	unsigned int id = header.getId();

	CStarNetUser* user = m_users[my];

	// Ensure that this user is in the cache
	CUserData* userData = m_cache->findUser(my);
	if (userData == NULL)
		m_irc->findUser(my);

	if (your.IsSameAs(m_groupCallsign)) {
		// This is a normal message for logging in/relaying
		if (user == NULL) {
			// This is a new user, add them to the list
			if (m_logFile != NULL) {
				time_t timeNow = ::time(NULL);
				struct tm* tm = ::gmtime(&timeNow);

				wxString text;
				text.Printf(wxT("%04d-%02d-%02d %02d:%02d:%02d: Adding %s to StarNet group %s\n"),
					tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec,
					my.c_str(), m_groupCallsign.c_str());

				m_logFile->Write(text);
				m_logFile->Flush();
			}

			// Start the StarNet group timer if not already running
			if (!m_groupTimer.isRunning())
				m_groupTimer.start();

			user = new CStarNetUser(my, m_userTimeout * 60U);
			m_users[my] = user;

			CStarNetId* tx = new CStarNetId(id, MESSAGE_DELAY, user);
			tx->setLogin();
			m_ids[id] = tx;
		} else {
			user->reset();

			// Check that it isn't a duplicate header
			CStarNetId* tx = m_ids[id];
			if (tx != NULL) {
				delete userData;
				return;
			}

			m_ids[id] = new CStarNetId(id, MESSAGE_DELAY, user);
		}
	} else {
		delete userData;
		userData = NULL;

		// This is a logoff message
		if (user == NULL)				// Not a known user, ignore
			return;

		if (m_logFile != NULL) {
			time_t timeNow = ::time(NULL);
			struct tm* tm = ::gmtime(&timeNow);

			wxString text;
			text.Printf(wxT("%04d-%02d-%02d %02d:%02d:%02d: Removing %s from StarNet group %s, logged off\n"),
				tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec,
				user->getCallsign().c_str(), m_groupCallsign.c_str());

			m_logFile->Write(text);
			m_logFile->Flush();
		}

		// Remove the user from the user list
		m_users.erase(my);

		CStarNetId* tx = new CStarNetId(id, MESSAGE_DELAY, user);
		tx->setLogoff();
		m_ids[id] = tx;

		return;
	}

	m_groupTimer.start();

	if (m_id != 0x00U) {
		delete userData;
		return;
	}

	m_id = id;

	// Change the Your callsign to CQCQCQ
	header.setCQCQCQ();

	header.setFlag1(0x00);
	header.setFlag2(0x00);
	header.setFlag3(0x00);

#if defined(DEXTRA_LINK)
	header.setRepeaters(m_linkGateway, m_linkReflector);
	CDExtraHandler::writeHeader(this, header, DIR_OUTGOING);
#endif
#if defined(DCS_LINK)
	header.setRepeaters(m_linkGateway, m_linkReflector);
	CDCSHandler::writeHeader(this, header, DIR_OUTGOING);
#endif

	// Get the home repeater of the user
	wxString exclude;
	if (userData != NULL) {
		exclude = userData->getRepeater();
		delete userData;
		userData = NULL;
	}

	// Build new repeater list
	for (CStarNetUsersHashMap::const_iterator it = m_users.begin(); it != m_users.end(); ++it) {
		CStarNetUser* user = it->second;
		if (user != NULL) {
			// Find the user in the cache
			CUserData* userData = m_cache->findUser(user->getCallsign());

			if (userData != NULL) {
				// Check for the excluded repeater
				if (!userData->getRepeater().IsSameAs(exclude)) {
					// Find the users repeater in the repeater list, add it otherwise
					CStarNetRepeater* repeater = m_repeaters[userData->getRepeater()];
					if (repeater == NULL) {
						// Add a new repeater entry
						repeater = new CStarNetRepeater;
						repeater->m_destination = wxT("/") + userData->getRepeater().Left(6U) + userData->getRepeater().Right(1U);
						repeater->m_repeater    = userData->getRepeater();
						repeater->m_gateway     = userData->getGateway();
						repeater->m_address     = userData->getAddress();
						repeater->m_local       = CRepeaterHandler::findDVRepeater(userData->getRepeater());
						m_repeaters[userData->getRepeater()] = repeater;
					}
				}

				delete userData;
				userData = NULL;
			}
		}
	}

	switch (m_callsignSwitch) {
		case SCS_GROUP_CALLSIGN:
			// Change the My Callsign 1 to be that of the StarNet group
			header.setMyCall1(m_groupCallsign);
			header.setMyCall2(wxT("SNET"));
			break;
		case SCS_USER_CALLSIGN:
			// Change the My Callsign 2 to be that of the StarNet group
			header.setMyCall1(my);
			header.setMyCall2(m_shortCallsign);
			break;
		default:
			break;
	}

	sendToRepeaters(header);

	if (m_txMsgSwitch)
		sendFromText(my);
}

void CStarNetHandler::process(CAMBEData &data)
{
	unsigned int id = data.getId();

	CStarNetId* tx = m_ids[id];
	if (tx == NULL)
		return;

	tx->reset();

	CStarNetUser* user = tx->getUser();
	user->reset();

	m_groupTimer.start();

	// If we've just logged in, the LOGOFF and INFO commands are disabled
	if (!tx->isLogin()) {
		// If we've already found some slow data, then don't look again
		if (!tx->isLogoff() && !tx->isInfo()) {
			tx->getTextCollector().writeData(data);
			bool hasText = tx->getTextCollector().hasData();
			if (hasText) {
				wxString text = tx->getTextCollector().getData();

				if (text.Left(6U).IsSameAs(wxT("LOGOFF"), false)) {
					if (m_logFile != NULL) {
						time_t timeNow = ::time(NULL);
						struct tm* tm = ::gmtime(&timeNow);

						wxString text;
						text.Printf(wxT("%04d-%02d-%02d %02d:%02d:%02d: Removing %s from StarNet group %s, logged off\n"),
							tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec,
							user->getCallsign().c_str(), m_groupCallsign.c_str());

						m_logFile->Write(text);
						m_logFile->Flush();
					}

					tx->setLogoff();

					// Ensure that this user is in the cache in time for the logoff ack
					CUserData* cacheUser = m_cache->findUser(user->getCallsign());
					if (cacheUser == NULL)
						m_irc->findUser(user->getCallsign());

					delete cacheUser;
					cacheUser = NULL;
				}

				if (text.Left(4U).IsSameAs(wxT("INFO"), false)) {
					tx->setInfo();

					// Ensure that this user is in the cache in time for the info text
					CUserData* cacheUser = m_cache->findUser(user->getCallsign());
					if (cacheUser == NULL)
						m_irc->findUser(user->getCallsign());

					delete cacheUser;
					cacheUser = NULL;
				}
			}
		}
	}

	if (id == m_id) {
#if defined(DEXTRA_LINK)
		CDExtraHandler::writeAMBE(this, data, DIR_OUTGOING);
#endif
#if defined(DCS_LINK)
		CDCSHandler::writeAMBE(this, data, DIR_OUTGOING);
#endif
		sendToRepeaters(data);
	}

	if (data.isEnd()) {
		if (id == m_id) {
			// Clear the repeater list if we're the relayed id
			for (CStarNetRepeatersHashMap::iterator it = m_repeaters.begin(); it != m_repeaters.end(); ++it)
				delete it->second;
			m_repeaters.clear();
			m_id = 0x00U;
		}

		if (tx->isLogin()) {
			tx->reset();
			tx->setEnd();
		} else if (tx->isLogoff()) {
			m_users.erase(user->getCallsign());
			tx->reset();
			tx->setEnd();
		} else if (tx->isInfo()) {
			tx->reset();
			tx->setEnd();
		} else {
			m_ids.erase(tx->getId());
			delete tx;
		}
	}
}

bool CStarNetHandler::logoff(const wxString &callsign)
{
	if (callsign.IsSameAs(wxT("ALL     "))) {
		for (CStarNetUsersHashMap::iterator it = m_users.begin(); it != m_users.end(); ++it) {
			CStarNetUser* user = it->second;

			if (user != NULL && m_logFile != NULL) {
				time_t timeNow = ::time(NULL);
				struct tm* tm = ::gmtime(&timeNow);

				wxString text;
				text.Printf(wxT("%04d-%02d-%02d %02d:%02d:%02d: Removing %s from StarNet group %s, logged off by remote control\n"),
					tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec,
					user->getCallsign().c_str(), m_groupCallsign.c_str());

				m_logFile->Write(text);
			}

			delete user;
		}

		if (m_logFile != NULL)
			m_logFile->Flush();

		for (CStarNetIdsHashMap::iterator it = m_ids.begin(); it != m_ids.end(); ++it)
			delete it->second;

		for (CStarNetRepeatersHashMap::iterator it = m_repeaters.begin(); it != m_repeaters.end(); ++it)
			delete it->second;

		m_users.clear();
		m_ids.clear();
		m_repeaters.end();

		m_groupTimer.stop();
		m_id = 0x00U;

		return true;
	} else {
		CStarNetUser* user = m_users[callsign];
		if (user == NULL) {
			wxLogMessage(wxT("Invalid callsign asked to logoff"));
			return false;
		}

		if (m_logFile != NULL) {
			time_t timeNow = ::time(NULL);
			struct tm* tm = ::gmtime(&timeNow);

			wxString text;
			text.Printf(wxT("%04d-%02d-%02d %02d:%02d:%02d: Removing %s from StarNet group %s, logged off by remote control\n"),
				tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec,
				user->getCallsign().c_str(), m_groupCallsign.c_str());

			m_logFile->Write(text);
			m_logFile->Flush();
		}

		// Find any associated id structure associated with this use, and the logged off user is the
		// currently relayed one, remove his id.
		for (CStarNetIdsHashMap::iterator it = m_ids.begin(); it != m_ids.end(); ++it) {
			CStarNetId* id = it->second;
			if (id != NULL && id->getUser() == user) {
				if (id->getId() == m_id)
					m_id = 0x00U;

				m_ids.erase(it);
				delete id;
				break;
			}
		}

		m_users.erase(callsign);
		delete user;

		// Check to see if we have any users left
		unsigned int count = 0U;
		for (CStarNetUsersHashMap::iterator it = m_users.begin(); it != m_users.end(); ++it) {
			if (it->second != NULL)
				count++;
		}

		// If none then clear all the data structures
		if (count == 0U) {
			for (CStarNetUsersHashMap::iterator it = m_users.begin(); it != m_users.end(); ++it)
				delete it->second;
			for (CStarNetIdsHashMap::iterator it = m_ids.begin(); it != m_ids.end(); ++it)
				delete it->second;
			for (CStarNetRepeatersHashMap::iterator it = m_repeaters.begin(); it != m_repeaters.end(); ++it)
				delete it->second;

			m_users.clear();
			m_ids.clear();
			m_repeaters.end();

			m_groupTimer.stop();
			m_id = 0x00U;
		}

		return true;
	}
}

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
bool CStarNetHandler::process(CHeaderData &header, DIRECTION, AUDIO_SOURCE)
{
	if (m_id != 0x00U)
		return false;

	wxString my = header.getMyCall1();
	m_id = header.getId();

	m_linkTimer.start();

	// Change the Your callsign to CQCQCQ
	header.setCQCQCQ();

	header.setFlag1(0x00);
	header.setFlag2(0x00);
	header.setFlag3(0x00);

	// Build new repeater list
	for (CStarNetUsersHashMap::const_iterator it = m_users.begin(); it != m_users.end(); ++it) {
		CStarNetUser* user = it->second;
		if (user != NULL) {
			// Find the user in the cache
			CUserData* userData = m_cache->findUser(user->getCallsign());

			if (userData != NULL) {
				// Find the users repeater in the repeater list, add it otherwise
				CStarNetRepeater* repeater = m_repeaters[userData->getRepeater()];
				if (repeater == NULL) {
					// Add a new repeater entry
					repeater = new CStarNetRepeater;
					repeater->m_destination = wxT("/") + userData->getRepeater().Left(6U) + userData->getRepeater().Right(1U);
					repeater->m_repeater    = userData->getRepeater();
					repeater->m_gateway     = userData->getGateway();
					repeater->m_address     = userData->getAddress();
					repeater->m_local       = CRepeaterHandler::findDVRepeater(userData->getRepeater());
					m_repeaters[userData->getRepeater()] = repeater;
				}

				delete userData;
				userData = NULL;
			}
		}
	}

	switch (m_callsignSwitch) {
		case SCS_GROUP_CALLSIGN:
			// Change the My Callsign 1 to be that of the StarNet group
			header.setMyCall1(m_groupCallsign);
			header.setMyCall2(wxT("SNET"));
			break;
		case SCS_USER_CALLSIGN:
			// Change the My Callsign 2 to be that of the StarNet group
			header.setMyCall1(my);
			header.setMyCall2(m_shortCallsign);
			break;
		default:
			break;
	}

	sendToRepeaters(header);

	if (m_txMsgSwitch)
		sendFromText(my);

	return true;
}

bool CStarNetHandler::process(CAMBEData &data, DIRECTION, AUDIO_SOURCE)
{
	unsigned int id = data.getId();
	if (id != m_id)
		return false;

	m_linkTimer.start();

	sendToRepeaters(data);

	if (data.isEnd()) {
		m_linkTimer.stop();
		m_id = 0x00U;

		// Clear the repeater list
		for (CStarNetRepeatersHashMap::iterator it = m_repeaters.begin(); it != m_repeaters.end(); ++it)
			delete it->second;
		m_repeaters.clear();
	}

	return true;
}
#endif

#if defined(DEXTRA_LINK)
void CStarNetHandler::linkInt()
{
	if (m_linkReflector.IsEmpty())
		return;

	wxLogMessage(wxT("Linking %s at startup to DExtra reflector %s"), m_repeater.c_str(), m_linkReflector.c_str());

	// Find the repeater to link to
	CRepeaterData* data = m_cache->findRepeater(m_linkReflector);
	if (data == NULL) {
		wxLogError(wxT("Cannot find the reflector in the cache, not linking"));
		return;
	}

	m_linkGateway = data->getGateway();
	m_linkStatus  = LS_LINKING_DEXTRA;

	CDExtraHandler::link(this, m_repeater, m_linkReflector, data->getAddress());

	delete data;
}
#endif

#if defined(DCS_LINK)
void CStarNetHandler::linkInt()
{
	if (m_linkReflector.IsEmpty())
		return;

	wxLogMessage(wxT("Linking %s at startup to DCS reflector %s"), m_repeater.c_str(), m_linkReflector.c_str());

	// Find the repeater to link to
	CRepeaterData* data = m_cache->findRepeater(m_linkReflector);
	if (data == NULL) {
		wxLogError(wxT("Cannot find the reflector in the cache, not linking"));
		return;
	}

	m_linkGateway = data->getGateway();
	m_linkStatus  = LS_LINKING_DCS;

	CDCSHandler::link(this, m_repeater, m_linkReflector, data->getAddress());

	delete data;
}
#endif

void CStarNetHandler::clockInt(unsigned int ms)
{
#if defined(DEXTRA_LINK) || defined(DCS_LINK)
	m_linkTimer.clock(ms);
	if (m_linkTimer.isRunning() && m_linkTimer.hasExpired()) {
		m_linkTimer.stop();
		m_id = 0x00U;

		// Clear the repeater list
		for (CStarNetRepeatersHashMap::iterator it = m_repeaters.begin(); it != m_repeaters.end(); ++it)
			delete it->second;
		m_repeaters.clear();
	}
#endif

	m_announceTimer.clock(ms);
	if (m_announceTimer.hasExpired()) {
		m_irc->sendHeardWithTXMsg(m_groupCallsign, wxT("    "), wxT("CQCQCQ  "), m_repeater, m_gateway, 0x00U, 0x00U, 0x00U, wxEmptyString, m_infoText);

		if (!m_offCallsign.IsEmpty() && !m_offCallsign.IsSameAs(wxT("        ")))
			m_irc->sendHeardWithTXMsg(m_offCallsign, wxT("    "), wxT("CQCQCQ  "), m_repeater, m_gateway, 0x00U, 0x00U, 0x00U, wxEmptyString, m_infoText);

		m_announceTimer.start(60U * 60U);		// 1 hour
	}

	// For each incoming id
	for (CStarNetIdsHashMap::iterator it = m_ids.begin(); it != m_ids.end(); ++it) {
		CStarNetId* tx = it->second;

		if (tx != NULL && tx->clock(ms)) {
			wxString callsign = tx->getUser()->getCallsign();

			if (tx->isEnd()) {
				CUserData* user = m_cache->findUser(callsign);
				if (user != NULL) {
					if (tx->isLogin())
						sendAck(*user, wxT("Logged in"));
					else if (tx->isInfo())
						sendAck(*user, m_infoText);
					else if (tx->isLogoff())
						sendAck(*user, wxT("Logged off"));

					delete user;
					user = NULL;
				} else {
					wxLogError(wxT("Cannot find %s in the cache"), callsign.c_str());
				}

				delete tx;
				m_ids.erase(it);

				// The iterator is now invalid, so we'll find the next expiry on the next clock tick with a
				// new iterator
				break;
			} else {
				if (tx->getId() == m_id) {
					// Clear the repeater list if we're the relayed id
					for (CStarNetRepeatersHashMap::iterator it = m_repeaters.begin(); it != m_repeaters.end(); ++it)
						delete it->second;
					m_repeaters.clear();
					m_id = 0x00U;
				}

				if (tx->isLogin()) {
					tx->reset();
					tx->setEnd();
				} else if (tx->isLogoff()) {
					m_users.erase(callsign);
					tx->reset();
					tx->setEnd();
				} else if (tx->isInfo()) {
					tx->reset();
					tx->setEnd();
				} else {
					delete tx;
					m_ids.erase(it);
					// The iterator is now invalid, so we'll find the next expiry on the next clock tick with a
					// new iterator
					break;
				}
			}
		}
	}

	// Individual user expiry, but not for the permanent entries
	for (CStarNetUsersHashMap::iterator it = m_users.begin(); it != m_users.end(); ++it) {
		CStarNetUser* user = it->second;
		if (user != NULL && m_permanent.Index(user->getCallsign()) == wxNOT_FOUND)
			user->clock(ms);
	}

	// Handle the group expiry timer
	m_groupTimer.clock(ms);

	// Don't do timeouts when relaying audio
	if (m_id != 0x00U)
		return;

	if (m_groupTimer.isRunning() && m_groupTimer.hasExpired()) {
		std::vector<CStarNetUser*> permanent;

		// Clear all the users, except the permenent one
		for (CStarNetUsersHashMap::iterator it = m_users.begin(); it != m_users.end(); ++it) {
			CStarNetUser* user = it->second;

			if (user != NULL) {
				if (m_permanent.Index(user->getCallsign()) != wxNOT_FOUND) {
					permanent.push_back(user);
				} else {
					if (m_logFile != NULL) {
						time_t timeNow = ::time(NULL);
						struct tm* tm = ::gmtime(&timeNow);

						wxString text;
						text.Printf(wxT("%04d-%02d-%02d %02d:%02d:%02d: Removing %s from StarNet group %s, group timeout\n"),
							tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec,
							user->getCallsign().c_str(), m_groupCallsign.c_str());

						m_logFile->Write(text);
						m_logFile->Flush();
					}

					delete user;
				}
			}
		}

		m_users.clear();

		// Re-insert the permenent users
		for (std::vector<CStarNetUser*>::const_iterator it = permanent.begin(); it != permanent.end(); ++it) {
			CStarNetUser* user = *it;
			wxString callsign = user->getCallsign();
			m_users[callsign] = user;
		}

		m_groupTimer.stop();
	}

	// Individual user expiry
	for (CStarNetUsersHashMap::iterator it = m_users.begin(); it != m_users.end(); ++it) {
		CStarNetUser* user = it->second;
		if (user != NULL && user->hasExpired()) {
			if (m_logFile != NULL) {
				time_t timeNow = ::time(NULL);
				struct tm* tm = ::gmtime(&timeNow);

				wxString text;
				text.Printf(wxT("%04d-%02d-%02d %02d:%02d:%02d: Removing %s from StarNet group %s, user timeout\n"),
					tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec,
					user->getCallsign().c_str(), m_groupCallsign.c_str());

				m_logFile->Write(text);
				m_logFile->Flush();
			}

			delete user;
			m_users.erase(it);
			// The iterator is now invalid, so we'll find the next expiry on the next clock tick with a
			// new iterator
			break;
		}
	}
}

void CStarNetHandler::sendToRepeaters(CHeaderData& header) const
{
	for (CStarNetRepeatersHashMap::const_iterator it = m_repeaters.begin(); it != m_repeaters.end(); ++it) {
		CStarNetRepeater* repeater = it->second;
		if (repeater != NULL) {
			header.setYourCall(repeater->m_destination);
			header.setDestination(repeater->m_address, G2_DV_PORT);
			header.setRepeaters(repeater->m_gateway, repeater->m_repeater);
			if (repeater->m_local != NULL)
				repeater->m_local->process(header, DIR_INCOMING, AS_G2);
			else
				m_g2Handler->writeHeader(header);
		}
	}
}

void CStarNetHandler::sendToRepeaters(CAMBEData& data) const
{
	for (CStarNetRepeatersHashMap::const_iterator it = m_repeaters.begin(); it != m_repeaters.end(); ++it) {
		CStarNetRepeater* repeater = it->second;
		if (repeater != NULL) {
			data.setDestination(repeater->m_address, G2_DV_PORT);
			if (repeater->m_local != NULL)
				repeater->m_local->process(data, DIR_INCOMING, AS_G2);
			else
				m_g2Handler->writeAMBE(data);
		}
	}
}

void CStarNetHandler::sendFromText(const wxString& my) const
{
	wxString text;
	switch (m_callsignSwitch) {
		case SCS_GROUP_CALLSIGN:
			text.Printf(wxT("FROM %s"), my.c_str());
			break;
		case SCS_USER_CALLSIGN:
			text.Printf(wxT("VIA STARnet %s"), m_groupCallsign.c_str());
			break;
		default:
			break;
	}

	CSlowDataEncoder slowData;
	slowData.setTextData(text);

	CAMBEData data;
	data.setId(m_id);

	unsigned char buffer[DV_FRAME_LENGTH_BYTES];
	::memcpy(buffer + 0U, NULL_AMBE_DATA_BYTES, VOICE_FRAME_LENGTH_BYTES);

	for (unsigned int i = 0U; i < 21U; i++) {
		if (i == 0U) {
			// The first AMBE packet is a sync
			::memcpy(buffer + VOICE_FRAME_LENGTH_BYTES, DATA_SYNC_BYTES, DATA_FRAME_LENGTH_BYTES);
			data.setData(buffer, DV_FRAME_LENGTH_BYTES);
			data.setSeq(i);
		} else {
			// The packets containing the text data
			unsigned char slowDataBuffer[DATA_FRAME_LENGTH_BYTES];
			slowData.getTextData(slowDataBuffer);
			::memcpy(buffer + VOICE_FRAME_LENGTH_BYTES, slowDataBuffer, DATA_FRAME_LENGTH_BYTES);
			data.setData(buffer, DV_FRAME_LENGTH_BYTES);
			data.setSeq(i);
		}

		sendToRepeaters(data);
	}
}

void CStarNetHandler::sendAck(const CUserData& user, const wxString& text) const
{
	unsigned int id = CHeaderData::createId();

	CHeaderData header(m_groupCallsign, wxT("    "), user.getUser(), user.getGateway(), user.getRepeater());
	header.setDestination(user.getAddress(), G2_DV_PORT);
	header.setId(id);
	m_g2Handler->writeHeader(header);

	CSlowDataEncoder slowData;
	slowData.setTextData(text);

	CAMBEData data;
	data.setId(id);
	data.setDestination(user.getAddress(), G2_DV_PORT);

	unsigned char buffer[DV_FRAME_MAX_LENGTH_BYTES];
	::memcpy(buffer + 0U, NULL_AMBE_DATA_BYTES, VOICE_FRAME_LENGTH_BYTES);

	for (unsigned int i = 0U; i < 20U; i++) {
		if (i == 0U) {
			// The first AMBE packet is a sync
			::memcpy(buffer + VOICE_FRAME_LENGTH_BYTES, DATA_SYNC_BYTES, DATA_FRAME_LENGTH_BYTES);
			data.setData(buffer, DV_FRAME_LENGTH_BYTES);
			data.setSeq(i);
		} else if (i == 19U) {
			// The last packet of the ack
			::memcpy(buffer + VOICE_FRAME_LENGTH_BYTES, END_PATTERN_BYTES, END_PATTERN_LENGTH_BYTES);
			data.setData(buffer, DV_FRAME_MAX_LENGTH_BYTES);
			data.setSeq(i);
			data.setEnd(true);
		} else {
			// The packets containing the text data
			unsigned char slowDataBuffer[DATA_FRAME_LENGTH_BYTES];
			slowData.getTextData(slowDataBuffer);
			::memcpy(buffer + VOICE_FRAME_LENGTH_BYTES, slowDataBuffer, DATA_FRAME_LENGTH_BYTES);
			data.setData(buffer, DV_FRAME_LENGTH_BYTES);
			data.setSeq(i);
		}

		m_g2Handler->writeAMBE(data);
	}
}

#if defined(DEXTRA_LINK)
void CStarNetHandler::linkUp(DSTAR_PROTOCOL, const wxString& callsign)
{
	wxLogMessage(wxT("DExtra link to %s established"), callsign.c_str());

	m_linkStatus = LS_LINKED_DEXTRA;
}

bool CStarNetHandler::linkFailed(DSTAR_PROTOCOL, const wxString& callsign, bool isRecoverable)
{
	if (!isRecoverable) {
		if (m_linkStatus != LS_NONE) {
			wxLogMessage(wxT("DExtra link to %s has failed"), callsign.c_str());
			m_linkStatus = LS_NONE;
		}

		return false;
	}

	if (m_linkStatus == LS_LINKING_DEXTRA || m_linkStatus == LS_LINKED_DEXTRA) {
		wxLogMessage(wxT("DExtra link to %s has failed, relinking"), callsign.c_str());
		m_linkStatus = LS_LINKING_DEXTRA;
		return true;
	}

	return false;
}

void CStarNetHandler::linkRefused(DSTAR_PROTOCOL, const wxString& callsign)
{
	if (m_linkStatus != LS_NONE) {
		wxLogMessage(wxT("DExtra link to %s was refused"), callsign.c_str());
		m_linkStatus = LS_NONE;
	}
}

bool CStarNetHandler::singleHeader()
{
	return true;
}
#endif

#if defined(DCS_LINK)
void CStarNetHandler::linkUp(DSTAR_PROTOCOL, const wxString& callsign)
{
	wxLogMessage(wxT("DCS link to %s established"), callsign.c_str());

	m_linkStatus = LS_LINKED_DCS;
}

void CStarNetHandler::linkRefused(DSTAR_PROTOCOL, const wxString& callsign)
{
	if (m_linkStatus != LS_NONE) {
		wxLogMessage(wxT("DCS link to %s was refused"), callsign.c_str());
		m_linkStatus = LS_NONE;
	}
}

bool CStarNetHandler::linkFailed(DSTAR_PROTOCOL, const wxString& callsign, bool isRecoverable)
{
	if (!isRecoverable) {
		if (m_linkStatus != LS_NONE) {
			wxLogMessage(wxT("DCS link to %s has failed"), callsign.c_str());
			m_linkStatus = LS_NONE;
		}

		return false;
	}

	if (m_linkStatus == LS_LINKING_DCS || m_linkStatus == LS_LINKED_DCS) {
		wxLogMessage(wxT("DCS link to %s has failed, relinking"), callsign.c_str());
		m_linkStatus = LS_LINKING_DCS;
		return true;
	}

	return false;
}

bool CStarNetHandler::singleHeader()
{
	return true;
}
#endif
