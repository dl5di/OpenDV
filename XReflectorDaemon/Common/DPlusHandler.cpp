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

#include "RepeaterHandler.h"
#include "DPlusHandler.h"
#include "DStarDefines.h"
#include "Utils.h"

#include <wx/filename.h>

unsigned int               CDPlusHandler::m_maxReflectors = 0U;
unsigned int               CDPlusHandler::m_maxDongles = 0U;
CDPlusHandler**            CDPlusHandler::m_reflectors = NULL;

wxString                   CDPlusHandler::m_gatewayCallsign;
wxString                   CDPlusHandler::m_dplusLogin;
CDPlusProtocolHandlerPool* CDPlusHandler::m_pool = NULL;
CDPlusProtocolHandler*     CDPlusHandler::m_incoming = NULL;

bool                       CDPlusHandler::m_stateChange = false;

CDPlusAuthenticator*       CDPlusHandler::m_authenticator = NULL;
CHeaderLogger*             CDPlusHandler::m_headerLogger = NULL;


CDPlusHandler::CDPlusHandler(IReflectorCallback* handler, const wxString& repeater, const wxString& reflector, CDPlusProtocolHandler* protoHandler, const in_addr& address, unsigned int port) :
m_repeater(repeater),
m_callsign(m_dplusLogin),
m_reflector(reflector),
m_handler(protoHandler),
m_yourAddress(address),
m_yourPort(port),
m_myPort(0U),
m_direction(DIR_OUTGOING),
m_linkState(DPLUS_LINKING),
m_destination(handler),
m_time(),
m_pollTimer(1000U, 1U),			// 1s
m_pollInactivityTimer(1000U, 30U),
m_tryTimer(1000U, 1U),
m_tryCount(0U),
m_dPlusId(0x00U),
m_dPlusSeq(0x00U),
m_inactivityTimer(1000U, 2U),
m_header(NULL)
{
	wxASSERT(protoHandler != NULL);
	wxASSERT(handler != NULL);
	wxASSERT(port > 0U);

	m_myPort = protoHandler->getPort();

	m_pollInactivityTimer.start();
	m_tryTimer.start();

	m_time = ::time(NULL);

	wxChar band = repeater.GetChar(LONG_CALLSIGN_LENGTH - 1U);
	m_callsign.SetChar(LONG_CALLSIGN_LENGTH - 1U, band);
}

CDPlusHandler::CDPlusHandler(CDPlusProtocolHandler* protoHandler, const in_addr& address, unsigned int port) :
m_repeater(),
m_callsign(),
m_reflector(),
m_handler(protoHandler),
m_yourAddress(address),
m_yourPort(port),
m_myPort(0U),
m_direction(DIR_INCOMING),
m_linkState(DPLUS_LINKING),
m_destination(NULL),
m_time(),
m_pollTimer(1000U, 1U),					// 1s
m_pollInactivityTimer(1000U, 10U),		// 10s
m_tryTimer(1000U),
m_tryCount(0U),
m_dPlusId(0x00U),
m_dPlusSeq(0x00U),
m_inactivityTimer(1000U, 2U),
m_header(NULL)
{
	wxASSERT(protoHandler != NULL);
	wxASSERT(port > 0U);

	m_myPort = protoHandler->getPort();

	m_pollTimer.start();
	m_pollInactivityTimer.start();

	m_time = ::time(NULL);
}

CDPlusHandler::~CDPlusHandler()
{
	if (m_direction == DIR_OUTGOING)
		m_pool->release(m_handler);

	delete m_header;
}

void CDPlusHandler::initialise(unsigned int maxReflectors)
{
	wxASSERT(maxReflectors > 0U);

	m_maxReflectors = maxReflectors;

	m_reflectors = new CDPlusHandler*[m_maxReflectors];
	for (unsigned int i = 0U; i < m_maxReflectors; i++)
		m_reflectors[i] = NULL;
}

void CDPlusHandler::startAuthenticator(const wxString& address, CCacheManager* cache)
{
	wxASSERT(cache != NULL);

	m_authenticator = new CDPlusAuthenticator(m_dplusLogin, m_gatewayCallsign, address, cache);
	m_authenticator->start();
}

void CDPlusHandler::setCallsign(const wxString& callsign)
{
	m_gatewayCallsign = callsign;
}

void CDPlusHandler::setDPlusProtocolHandlerPool(CDPlusProtocolHandlerPool* pool)
{
	wxASSERT(pool != NULL);

	m_pool = pool;
}

void CDPlusHandler::setDPlusProtocolIncoming(CDPlusProtocolHandler* handler)
{
	wxASSERT(handler != NULL);

	m_incoming = handler;
}

void CDPlusHandler::setDPlusLogin(const wxString& dplusLogin)
{
	m_dplusLogin = dplusLogin;
}

void CDPlusHandler::setHeaderLogger(CHeaderLogger* logger)
{
	m_headerLogger = logger;
}

void CDPlusHandler::setMaxDongles(unsigned int maxDongles)
{
	m_maxDongles = maxDongles;
}

void CDPlusHandler::getInfo(IReflectorCallback* handler, CRemoteRepeaterData& data)
{
	wxASSERT(handler != NULL);

	for (unsigned int i = 0U; i < m_maxReflectors; i++) {
		CDPlusHandler* reflector = m_reflectors[i];
		if (reflector != NULL) {
			if (reflector->m_destination == handler)
				data.addLink(reflector->m_reflector, PROTO_DPLUS, reflector->m_linkState == DPLUS_LINKED, reflector->m_direction, true);
		}
	}
}

wxString CDPlusHandler::getDongles()
{
	wxString dongles;

	for (unsigned int i = 0U; i < m_maxReflectors; i++) {
		CDPlusHandler* reflector = m_reflectors[i];

		if (reflector != NULL && reflector->m_direction == DIR_INCOMING) {
			dongles.Append(wxT("P:"));
			dongles.Append(reflector->m_reflector);
			dongles.Append(wxT("  "));
		}
	}

	return dongles;
}

void CDPlusHandler::process(CHeaderData& header)
{
	in_addr   yourAddress = header.getYourAddress();
	unsigned int yourPort = header.getYourPort();
	unsigned int   myPort = header.getMyPort();

	for (unsigned int i = 0U; i < m_maxReflectors; i++) {
		CDPlusHandler* reflector = m_reflectors[i];

		if (reflector != NULL) {
			if (reflector->m_yourAddress.s_addr == yourAddress.s_addr &&
				reflector->m_yourPort           == yourPort &&
				reflector->m_myPort             == myPort) {
				reflector->processInt(header);
				return;
			}
		}
	}	
}

void CDPlusHandler::process(CAMBEData& data)
{
	in_addr   yourAddress = data.getYourAddress();
	unsigned int yourPort = data.getYourPort();
	unsigned int   myPort = data.getMyPort();

	for (unsigned int i = 0U; i < m_maxReflectors; i++) {
		CDPlusHandler* reflector = m_reflectors[i];

		if (reflector != NULL) {
			if (reflector->m_yourAddress.s_addr == yourAddress.s_addr &&
				reflector->m_yourPort           == yourPort &&
				reflector->m_myPort             == myPort) {
				reflector->processInt(data);
				return;
			}
		}
	}	
}

void CDPlusHandler::process(const CPollData& poll)
{
	in_addr   yourAddress = poll.getYourAddress();
	unsigned int yourPort = poll.getYourPort();
	unsigned int   myPort = poll.getMyPort();

	for (unsigned int i = 0U; i < m_maxReflectors; i++) {
		CDPlusHandler* reflector = m_reflectors[i];

		if (reflector != NULL) {
			if (reflector->m_yourAddress.s_addr == yourAddress.s_addr &&
				reflector->m_yourPort           == yourPort &&
				reflector->m_myPort             == myPort) {
				reflector->m_pollInactivityTimer.reset();
				return;
			}
		}
	}	

	// If we cannot find an existing link, we ignore the poll
	wxLogMessage(wxT("Incoming poll from unknown D-Plus dongle"));
}

void CDPlusHandler::process(CConnectData& connect)
{
	CD_TYPE          type = connect.getType();
	in_addr   yourAddress = connect.getYourAddress();
	unsigned int yourPort = connect.getYourPort();
	unsigned int   myPort = connect.getMyPort();

	for (unsigned int i = 0U; i < m_maxReflectors; i++) {
		CDPlusHandler* reflector = m_reflectors[i];

		if (reflector != NULL) {
			if (reflector->m_yourAddress.s_addr == yourAddress.s_addr &&
				reflector->m_yourPort           == yourPort &&
				reflector->m_myPort             == myPort) {
				bool res = m_reflectors[i]->processInt(connect, type);
				if (res) {
					delete m_reflectors[i];
					m_reflectors[i] = NULL;
				}
			}
		}
	}

	// Check that it isn't a duplicate
	for (unsigned int i = 0U; i < m_maxReflectors; i++) {
		CDPlusHandler* reflector = m_reflectors[i];

		if (reflector != NULL) {
			if (reflector->m_yourAddress.s_addr == yourAddress.s_addr &&
				reflector->m_yourPort           == yourPort &&
				reflector->m_myPort             == myPort)
				return;
		}
	}

	if (type == CT_UNLINK)
		return;

	if (type != CT_LINK1) {
		wxLogMessage(wxT("Incoming D-Plus message from unknown source"));
		return;
	}

	// Check to see if we are allowed to accept it
	unsigned int count = 0U;
	for (unsigned int i = 0U; i < m_maxReflectors; i++) {
		if (m_reflectors[i] != NULL &&
			m_reflectors[i]->m_direction == DIR_INCOMING)
			count++;
	}

	if (count >= m_maxDongles)
		return;

	CDPlusHandler* dplus = new CDPlusHandler(m_incoming, yourAddress, yourPort);

	bool found = false;

	for (unsigned int i = 0U; i < m_maxReflectors; i++) {
		if (m_reflectors[i] == NULL) {
			m_reflectors[i] = dplus;
			found = true;
			break;
		}
	}

	if (found) {
		CConnectData connect(CT_LINK1, yourAddress, yourPort);
		m_incoming->writeConnect(connect);
	} else {
		wxLogError(wxT("No space to add new D-Plus dongle, ignoring"));
		delete dplus;
	}
}

void CDPlusHandler::link(IReflectorCallback* handler, const wxString& repeater, const wxString &gateway, const in_addr& address)
{
	CDPlusProtocolHandler* protoHandler = m_pool->getHandler();
	if (protoHandler == NULL)
		return;

	CDPlusHandler* dplus = new CDPlusHandler(handler, repeater, gateway, protoHandler, address, DPLUS_PORT);

	bool found = false;

	for (unsigned int i = 0U; i < m_maxReflectors; i++) {
		if (m_reflectors[i] == NULL) {
			m_reflectors[i] = dplus;
			found = true;
			break;
		}
	}

	if (found) {
		CConnectData connect(CT_LINK1, address, DPLUS_PORT);
		protoHandler->writeConnect(connect);
		m_stateChange = true;
	} else {
		wxLogError(wxT("No space to add new D-Plus reflector, ignoring"));
		delete dplus;
	}
}

void CDPlusHandler::relink(IReflectorCallback* handler, const wxString &gateway)
{
	for (unsigned int i = 0U; i < m_maxReflectors; i++) {
		if (m_reflectors[i] != NULL && m_reflectors[i]->m_direction == DIR_OUTGOING) {
			if (m_reflectors[i]->m_destination == handler) {
				m_reflectors[i]->m_reflector = gateway;
				m_reflectors[i]->m_dPlusId   = 0x00U;
				m_reflectors[i]->m_dPlusSeq  = 0x00U;
				m_stateChange = true;
				return;
			}
		}
	}	
}

void CDPlusHandler::unlink(IReflectorCallback* handler, const wxString& exclude)
{
	for (unsigned int i = 0U; i < m_maxReflectors; i++) {
		CDPlusHandler* reflector = m_reflectors[i];

		if (reflector != NULL) {
			if (reflector->m_direction == DIR_OUTGOING && reflector->m_destination == handler && !reflector->m_reflector.IsSameAs(exclude)) {
				wxLogMessage(wxT("Removing outgoing D-Plus link %s, %s"), reflector->m_repeater.c_str(), reflector->m_reflector.c_str());

				if (reflector->m_linkState == DPLUS_LINKING || reflector->m_linkState == DPLUS_LINKED) {
					CConnectData connect(CT_UNLINK, reflector->m_yourAddress, DPLUS_PORT);
					reflector->m_handler->writeConnect(connect);
					reflector->m_handler->writeConnect(connect);

					reflector->m_linkState = DPLUS_UNLINKING;
					reflector->m_tryTimer.setTimeout(1U);
					reflector->m_tryTimer.start();
					reflector->m_pollTimer.stop();
					reflector->m_pollInactivityTimer.stop();
					reflector->m_tryCount = 0U;
				}

				// If an active link with incoming traffic, send an EOT to the repeater
				if (reflector->m_dPlusId != 0x00U) {
					unsigned int seq = reflector->m_dPlusSeq + 1U;
					if (seq == 21U)
						seq = 0U;

					CAMBEData data;
					data.setData(END_PATTERN_BYTES, DV_FRAME_LENGTH_BYTES);
					data.setSeq(seq);
					data.setEnd(true);
					data.setId(reflector->m_dPlusId);

					reflector->m_destination->process(data, reflector->m_direction, AS_DPLUS);
				}

				m_stateChange = true;
			}
		}
	}
}

void CDPlusHandler::unlink()
{
	for (unsigned int i = 0U; i < m_maxReflectors; i++) {
		CDPlusHandler* reflector = m_reflectors[i];
		if (reflector != NULL) {
			if (!reflector->m_reflector.IsEmpty())
				wxLogMessage(wxT("Unlinking from D-Plus reflector or dongle %s"), reflector->m_reflector.c_str());

			CConnectData connect(CT_UNLINK, reflector->m_yourAddress, reflector->m_yourPort);
			reflector->m_handler->writeConnect(connect);
			reflector->m_handler->writeConnect(connect);
			reflector->m_tryTimer.setTimeout(1U);
			reflector->m_tryTimer.start();
			reflector->m_pollTimer.stop();
			reflector->m_pollInactivityTimer.stop();
			reflector->m_tryCount = 0U;
		}
	}	
}

void CDPlusHandler::writeHeader(IReflectorCallback* handler, CHeaderData& header, DIRECTION direction)
{
	for (unsigned int i = 0U; i < m_maxReflectors; i++) {
		if (m_reflectors[i] != NULL)
			m_reflectors[i]->writeHeaderInt(handler, header, direction);
	}	
}

void CDPlusHandler::writeAMBE(IReflectorCallback* handler, CAMBEData& data, DIRECTION direction)
{
	for (unsigned int i = 0U; i < m_maxReflectors; i++) {
		if (m_reflectors[i] != NULL)
			m_reflectors[i]->writeAMBEInt(handler, data, direction);
	}	
}

void CDPlusHandler::gatewayUpdate(const wxString& gateway, const wxString& address)
{
	wxString gatewayBase = gateway;
	gatewayBase.Truncate(LONG_CALLSIGN_LENGTH - 1U);

	for (unsigned int i = 0U; i < m_maxReflectors; i++) {
		CDPlusHandler* reflector = m_reflectors[i];
		if (reflector != NULL) {
			if (!reflector->m_reflector.IsEmpty() && reflector->m_reflector.Left(LONG_CALLSIGN_LENGTH - 1U).IsSameAs(gatewayBase)) {
				if (!address.IsEmpty()) {
					// A new address, change the value
					wxLogMessage(wxT("Changing IP address of D-Plus gateway or reflector %s to %s"), gatewayBase.c_str(), address.c_str());
					reflector->m_yourAddress.s_addr = ::inet_addr(address.mb_str());
				} else {
					wxLogMessage(wxT("IP address for D-Plus gateway or reflector %s has been removed"), gatewayBase.c_str());

					// No address, this probably shouldn't happen....
					if (reflector->m_direction == DIR_OUTGOING && reflector->m_destination != NULL)
						reflector->m_destination->linkFailed(DP_DPLUS, reflector->m_reflector, false);

					m_stateChange = true;

					delete m_reflectors[i];
					m_reflectors[i] = NULL;
				}
			}
		}
	}
}

void CDPlusHandler::clock(unsigned int ms)
{
	for (unsigned int i = 0U; i < m_maxReflectors; i++) {
		if (m_reflectors[i] != NULL) {
			bool ret = m_reflectors[i]->clockInt(ms);
			if (ret) {
				delete m_reflectors[i];
				m_reflectors[i] = NULL;
			}
		}
	}
}

void CDPlusHandler::finalise()
{
	if (m_authenticator != NULL)
		m_authenticator->stop();

	for (unsigned int i = 0U; i < m_maxReflectors; i++)
		delete m_reflectors[i];

	delete[] m_reflectors;
}

void CDPlusHandler::processInt(CHeaderData& header)
{
	wxString   rpt1 = header.getRptCall1();
	wxString   rpt2 = header.getRptCall2();
	unsigned int id = header.getId();

	if (m_linkState != DPLUS_LINKED)
		return;

	switch (m_direction) {
		case DIR_OUTGOING:
			if (m_reflector.IsSameAs(rpt1) || m_reflector.IsSameAs(rpt2)) {
				// If we're already processing, ignore the new header
				if (m_dPlusId != 0x00U)
					return;

				// Write to Header.log if it's enabled
				if (m_headerLogger != NULL)
					m_headerLogger->write(wxT("DPlus"), header);

				m_dPlusId  = id;
				m_dPlusSeq = 0x00U;
				m_inactivityTimer.start();
				m_pollInactivityTimer.reset();

				delete m_header;

				m_header = new CHeaderData(header);
				m_header->setCQCQCQ();
				m_header->setFlags(0x00U, 0x00U, 0x00U);

				m_destination->process(*m_header, m_direction, AS_DPLUS);
			}
			break;

		case DIR_INCOMING: {
				m_destination = CRepeaterHandler::findDVRepeater(rpt1);
				if (m_destination == NULL) {
					m_destination = CRepeaterHandler::findDVRepeater(rpt2);
					if (m_destination == NULL)
						return;
				}

				if (m_dPlusId != 0x00U)
					return;

				// Write to Header.log if it's enabled
				if (m_headerLogger != NULL)
					m_headerLogger->write(wxT("DPlus"), header);

				m_dPlusId  = id;
				m_dPlusSeq = 0x00U;
				m_inactivityTimer.start();
				m_pollInactivityTimer.reset();

				delete m_header;

				m_header = new CHeaderData(header);
				m_header->setCQCQCQ();
				m_header->setFlags(0x00U, 0x00U, 0x00U);

				m_destination->process(*m_header, m_direction, AS_DPLUS);
			}
			break;
	}
}

void CDPlusHandler::processInt(CAMBEData& data)
{
	unsigned int id = data.getId();

	if (m_dPlusId != id)
		return;

	m_dPlusSeq = data.getSeq();

	// Send the header every 21 frames
	if (m_dPlusSeq == 0U)
		m_destination->process(*m_header, m_direction, AS_DUP);

	m_inactivityTimer.reset();
	m_pollInactivityTimer.reset();

	m_destination->process(data, m_direction, AS_DPLUS);

	if (data.isEnd()) {
		m_dPlusId  = 0x00U;
		m_dPlusSeq = 0x00U;

		delete m_header;
		m_header = NULL;

		m_inactivityTimer.stop();
	}
}

bool CDPlusHandler::processInt(CConnectData& connect, CD_TYPE type)
{
	switch (m_direction) {
		case DIR_OUTGOING:
			switch (type) {
				case CT_ACK:
					if (m_linkState == DPLUS_LINKING) {
						wxLogMessage(wxT("D-Plus ACK message received from %s"), m_reflector.c_str());
						m_destination->linkUp(DP_DPLUS, m_reflector);
						m_stateChange = true;
						m_linkState   = DPLUS_LINKED;
						m_tryTimer.stop();
						m_pollTimer.start();
						m_pollInactivityTimer.start();
					}
					return false;

				case CT_NAK:
					if (m_linkState == DPLUS_LINKING) {
						wxLogMessage(wxT("D-Plus NAK message received from %s"), m_reflector.c_str());
						m_destination->linkRefused(DP_DPLUS, m_reflector);
						CConnectData reply(CT_UNLINK, connect.getYourAddress(), connect.getYourPort());
						m_handler->writeConnect(reply);
						m_tryTimer.stop();
					}
					return true;

				case CT_UNLINK:
					if (m_linkState == DPLUS_UNLINKING) {
						wxLogMessage(wxT("D-Plus disconnect acknowledgement received from %s"), m_reflector.c_str());
						m_destination->linkFailed(DP_DPLUS, m_reflector, false);
						m_stateChange = true;
						m_tryTimer.stop();
					}
					return true;

				case CT_LINK1: {
						CConnectData reply(m_dplusLogin, CT_LINK2, connect.getYourAddress(), connect.getYourPort());
						m_handler->writeConnect(reply);
						m_tryTimer.stop();
					}
					return false;

				default:
					return false;
			}
			break;

		case DIR_INCOMING:
			switch (type) {
				case CT_LINK2: {
						m_reflector = connect.getRepeater();
						wxLogMessage(wxT("D-Plus dongle link to %s has started"), m_reflector.c_str());
						CConnectData reply(CT_ACK, m_yourAddress, m_yourPort);
						m_handler->writeConnect(reply);
						m_linkState   = DPLUS_LINKED;
						m_stateChange = true;
					}
					return false;

				case CT_UNLINK:
					if (m_linkState == DPLUS_LINKED) {
						wxLogMessage(wxT("D-Plus dongle link to %s has ended (unlinked)"), m_reflector.c_str());
						m_stateChange = true;
						m_handler->writeConnect(connect);
					}
					return true;

				default:
					return false;
			}
			break;
	}

	return false;
}

bool CDPlusHandler::clockInt(unsigned int ms)
{
	m_tryTimer.clock(ms);
	m_pollTimer.clock(ms);
	m_inactivityTimer.clock(ms);
	m_pollInactivityTimer.clock(ms);

	if (m_pollInactivityTimer.isRunning() && m_pollInactivityTimer.hasExpired()) {
		m_pollInactivityTimer.reset();

		delete m_header;
		m_header = NULL;

		m_stateChange = true;
		m_dPlusId     = 0x00U;
		m_dPlusSeq    = 0x00U;

		if (!m_reflector.IsEmpty()) {
			switch (m_linkState) {
				case DPLUS_LINKING:
					wxLogMessage(wxT("D-Plus link to %s has failed to connect"), m_reflector.c_str());
					break;
				case DPLUS_LINKED:
					wxLogMessage(wxT("D-Plus link to %s has failed (poll inactivity)"), m_reflector.c_str());
					break;
				case DPLUS_UNLINKING:
					wxLogMessage(wxT("D-Plus link to %s has failed to disconnect cleanly"), m_reflector.c_str());
					break;
				default:
					break;
			}
		}

		if (m_direction == DIR_OUTGOING) {
			bool reconnect = m_destination->linkFailed(DP_DPLUS, m_reflector, true);
			if (reconnect) {
				CConnectData connect(CT_LINK1, m_yourAddress, DPLUS_PORT);
				m_handler->writeConnect(connect);
				m_linkState = DPLUS_LINKING;
				m_tryTimer.setTimeout(1U);
				m_tryTimer.reset();
				m_tryCount = 0U;
				return false;
			}
		}

		return true;
	}

	if (m_pollTimer.isRunning() && m_pollTimer.hasExpired()) {
		CPollData poll(m_yourAddress, m_yourPort);
		m_handler->writePoll(poll);

		m_pollTimer.reset();
	}

	if (m_tryTimer.isRunning() && m_tryTimer.hasExpired()) {
		switch (m_linkState) {
			case DPLUS_LINKING: {
					CConnectData connect(CT_LINK1, m_yourAddress, DPLUS_PORT);
					m_handler->writeConnect(connect);
				}
				break;

			case DPLUS_UNLINKING: {
					CConnectData connect(CT_UNLINK, m_yourAddress, m_yourPort);
					m_handler->writeConnect(connect);
					m_handler->writeConnect(connect);
				}
				break;

			default:
				break;
		}

		unsigned int timeout = calcBackoff();
		m_tryTimer.setTimeout(timeout);
		m_tryTimer.reset();
	}

	if (m_inactivityTimer.isRunning() && m_inactivityTimer.hasExpired()) {
		delete m_header;
		m_header = NULL;

		m_dPlusId  = 0x00U;
		m_dPlusSeq = 0x00U;

		m_inactivityTimer.stop();
	}

	return false;
}

void CDPlusHandler::writeHeaderInt(IReflectorCallback* handler, CHeaderData& header, DIRECTION direction)
{
	wxASSERT(handler != NULL);

	if (m_linkState != DPLUS_LINKED)
		return;

	if (direction != m_direction)
		return;

	// Already is use?
	if (m_dPlusId != 0x00U)
		return;

	switch (m_direction) {
		case DIR_OUTGOING:
			if (m_destination == handler) {
				header.setRepeaters(m_callsign, m_reflector);
				header.setDestination(m_yourAddress, m_yourPort);
				m_handler->writeHeader(header);
			}
			break;

		case DIR_INCOMING:
			header.setDestination(m_yourAddress, m_yourPort);
			m_handler->writeHeader(header);
			break;
	}
}

void CDPlusHandler::writeAMBEInt(IReflectorCallback* handler, CAMBEData& data, DIRECTION direction)
{
	if (m_linkState != DPLUS_LINKED)
		return;

	if (direction != m_direction)
		return;

	// Already in use?
	if (m_dPlusId != 0x00U)
		return;

	switch (m_direction) {
		case DIR_OUTGOING:
			if (m_destination == handler) {
				data.setDestination(m_yourAddress, m_yourPort);
				m_handler->writeAMBE(data);
			}
			break;

		case DIR_INCOMING:
			data.setDestination(m_yourAddress, m_yourPort);
			m_handler->writeAMBE(data);
			break;
	}
}

bool CDPlusHandler::stateChange()
{
	bool stateChange = m_stateChange;

	m_stateChange = false;

	return stateChange;
}

void CDPlusHandler::writeStatus(wxFFile& file)
{
	for (unsigned int i = 0U; i < m_maxReflectors; i++) {
		CDPlusHandler* reflector = m_reflectors[i];
		if (reflector != NULL) {
			wxString text;

			struct tm* tm = ::gmtime(&reflector->m_time);

			if (reflector->m_linkState == DPLUS_LINKED) {
				switch (reflector->m_direction) {
					case DIR_OUTGOING:
						text.Printf(wxT("%04d-%02d-%02d %02d:%02d:%02d: DPlus link - Type: Dongle Rptr: %s Refl: %s Dir: Outgoing\n"),
							tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec, 
							reflector->m_repeater.c_str(), reflector->m_reflector.c_str());
						break;

					case DIR_INCOMING:
						text.Printf(wxT("%04d-%02d-%02d %02d:%02d:%02d: DPlus link - Type: Dongle User: %s Dir: Incoming\n"),
							tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec, 
							reflector->m_reflector.c_str());
						break;
				}

				file.Write(text);
			}
		}
	}
}

unsigned int CDPlusHandler::calcBackoff()
{
	unsigned int timeout = 1U;

	for (unsigned int i = 0U; i < m_tryCount; i++)
		timeout *= 2U;

	m_tryCount++;

	if (timeout > 60U)
		return 60U;
	else
		return timeout;
}
