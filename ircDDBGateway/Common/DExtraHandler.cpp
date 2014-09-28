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

#include "RepeaterHandler.h"
#include "DExtraHandler.h"
#include "DStarDefines.h"
#include "Utils.h"

unsigned int                CDExtraHandler::m_maxReflectors = 0U;
unsigned int                CDExtraHandler::m_maxDongles = 0U;
CDExtraHandler**            CDExtraHandler::m_reflectors = NULL;

wxString                    CDExtraHandler::m_callsign;
CDExtraProtocolHandlerPool* CDExtraHandler::m_pool = NULL;
CDExtraProtocolHandler*     CDExtraHandler::m_incoming = NULL;

bool                        CDExtraHandler::m_stateChange = false;

CHeaderLogger*              CDExtraHandler::m_headerLogger = NULL;


CDExtraHandler::CDExtraHandler(IReflectorCallback* handler, const wxString& reflector, const wxString& repeater, CDExtraProtocolHandler* protoHandler, const in_addr& address, unsigned int port, DIRECTION direction) :
m_reflector(reflector),
m_repeater(repeater),
m_handler(protoHandler),
m_yourAddress(address),
m_yourPort(port),
m_direction(direction),
m_linkState(DEXTRA_LINKING),
m_destination(handler),
m_time(),
m_pollTimer(1000U, 10U),
m_pollInactivityTimer(1000U, 60U),
m_tryTimer(1000U, 1U),
m_tryCount(0U),
m_dExtraId(0x00U),
m_dExtraSeq(0x00U),
m_inactivityTimer(1000U, NETWORK_TIMEOUT),
m_header(NULL)
{
	wxASSERT(protoHandler != NULL);
	wxASSERT(handler != NULL);
	wxASSERT(port > 0U);

	m_pollInactivityTimer.start();

	m_time = ::time(NULL);

	if (direction == DIR_INCOMING) {
		m_pollTimer.start();
		m_stateChange = true;
		m_linkState = DEXTRA_LINKED;
	} else {
		m_linkState = DEXTRA_LINKING;
		m_tryTimer.start();
	}
}

CDExtraHandler::CDExtraHandler(CDExtraProtocolHandler* protoHandler, const wxString& reflector, const in_addr& address, unsigned int port, DIRECTION direction) :
m_reflector(reflector),
m_repeater(),
m_handler(protoHandler),
m_yourAddress(address),
m_yourPort(port),
m_direction(direction),
m_linkState(DEXTRA_LINKING),
m_destination(NULL),
m_time(),
m_pollTimer(1000U, 10U),
m_pollInactivityTimer(1000U, 60U),
m_tryTimer(1000U, 1U),
m_tryCount(0U),
m_dExtraId(0x00U),
m_dExtraSeq(0x00U),
m_inactivityTimer(1000U, NETWORK_TIMEOUT),
m_header(NULL)
{
	wxASSERT(protoHandler != NULL);
	wxASSERT(port > 0U);

	m_pollInactivityTimer.start();

	m_time = ::time(NULL);

	if (direction == DIR_INCOMING) {
		m_pollTimer.start();
		m_stateChange = true;
		m_linkState = DEXTRA_LINKED;
	} else {
		m_linkState = DEXTRA_LINKING;
		m_tryTimer.start();
	}
}

CDExtraHandler::~CDExtraHandler()
{
	if (m_direction == DIR_OUTGOING)
		m_pool->release(m_handler);

	delete m_header;
}

void CDExtraHandler::initialise(unsigned int maxReflectors)
{
	wxASSERT(maxReflectors > 0U);

	m_maxReflectors = maxReflectors;

	m_reflectors = new CDExtraHandler*[m_maxReflectors];
	for (unsigned int i = 0U; i < m_maxReflectors; i++)
		m_reflectors[i] = NULL;
}

void CDExtraHandler::setCallsign(const wxString& callsign)
{
	m_callsign = callsign;
	m_callsign.SetChar(LONG_CALLSIGN_LENGTH - 1U, wxT(' '));
}

void CDExtraHandler::setDExtraProtocolIncoming(CDExtraProtocolHandler* incoming)
{
	wxASSERT(incoming != NULL);

	m_incoming = incoming;
}

void CDExtraHandler::setDExtraProtocolHandlerPool(CDExtraProtocolHandlerPool* pool)
{
	wxASSERT(pool != NULL);

	m_pool = pool;
}

void CDExtraHandler::setHeaderLogger(CHeaderLogger* logger)
{
	m_headerLogger = logger;
}

void CDExtraHandler::setMaxDongles(unsigned int maxDongles)
{
	m_maxDongles = maxDongles;
}

wxString CDExtraHandler::getIncoming(const wxString& callsign)
{
	wxString incoming;

	for (unsigned int i = 0U; i < m_maxReflectors; i++) {
		CDExtraHandler* reflector = m_reflectors[i];
		if (reflector != NULL && reflector->m_direction == DIR_INCOMING && reflector->m_repeater.IsSameAs(callsign)) {
			incoming.Append(reflector->m_reflector);
			incoming.Append(wxT("  "));
		}
	}

	return incoming;
}

void CDExtraHandler::getInfo(IReflectorCallback* handler, CRemoteRepeaterData& data)
{
	wxASSERT(handler != NULL);

	for (unsigned int i = 0U; i < m_maxReflectors; i++) {
		CDExtraHandler* reflector = m_reflectors[i];
		if (reflector != NULL) {
			if (reflector->m_destination == handler) {
				if (reflector->m_direction == DIR_INCOMING && reflector->m_repeater.IsEmpty()) {
					if (reflector->m_linkState != DEXTRA_UNLINKING)
						data.addLink(reflector->m_reflector, PROTO_DEXTRA, reflector->m_linkState == DEXTRA_LINKED, DIR_INCOMING, true);
				} else {
					if (reflector->m_linkState != DEXTRA_UNLINKING)
						data.addLink(reflector->m_reflector, PROTO_DEXTRA, reflector->m_linkState == DEXTRA_LINKED, reflector->m_direction, false);
				}
			}
		}
	}
}

wxString CDExtraHandler::getDongles()
{
	wxString dongles;

	for (unsigned int i = 0U; i < m_maxReflectors; i++) {
		CDExtraHandler* reflector = m_reflectors[i];
		if (reflector != NULL && reflector->m_direction == DIR_INCOMING && reflector->m_repeater.IsEmpty()) {
			dongles.Append(wxT("X:"));
			dongles.Append(reflector->m_reflector);
			dongles.Append(wxT("  "));
		}
	}

	return dongles;
}

void CDExtraHandler::process(CHeaderData& header)
{
	in_addr   yourAddress = header.getYourAddress();
	unsigned int yourPort = header.getYourPort();

	for (unsigned int i = 0U; i < m_maxReflectors; i++) {
		CDExtraHandler* reflector = m_reflectors[i];
		if (reflector != NULL) {
			if (reflector->m_yourAddress.s_addr == yourAddress.s_addr &&
				reflector->m_yourPort           == yourPort)
				reflector->processInt(header);
		}
	}	
}

void CDExtraHandler::process(CAMBEData& data)
{
	in_addr   yourAddress = data.getYourAddress();
	unsigned int yourPort = data.getYourPort();

	for (unsigned int i = 0U; i < m_maxReflectors; i++) {
		CDExtraHandler* reflector = m_reflectors[i];
		if (reflector != NULL) {
			if (reflector->m_yourAddress.s_addr == yourAddress.s_addr &&
				reflector->m_yourPort           == yourPort)
				reflector->processInt(data);
		}
	}	
}

void CDExtraHandler::process(const CPollData& poll)
{
	bool found = false;

	wxString    reflector = poll.getData1();
	in_addr   yourAddress = poll.getYourAddress();
	unsigned int yourPort = poll.getYourPort();

	// Check to see if we already have a link
	for (unsigned int i = 0U; i < m_maxReflectors; i++) {
		if (m_reflectors[i] != NULL) {
			if (m_reflectors[i]->m_reflector.Left(LONG_CALLSIGN_LENGTH - 1U).IsSameAs(reflector.Left(LONG_CALLSIGN_LENGTH - 1U)) &&
				m_reflectors[i]->m_yourAddress.s_addr == yourAddress.s_addr &&
				m_reflectors[i]->m_yourPort           == yourPort &&
				m_reflectors[i]->m_linkState          == DEXTRA_LINKED) {
				m_reflectors[i]->m_pollInactivityTimer.start();
				found = true;
			}
		}
	}	

	if (found)
		return;

	// A repeater poll arriving here is an error
	if (!poll.isDongle())
		return;

	// Check to see if we are allowed to accept it
	unsigned int count = 0U;
	for (unsigned int i = 0U; i < m_maxReflectors; i++) {
		if (m_reflectors[i] != NULL &&
			m_reflectors[i]->m_direction == DIR_INCOMING &&
			m_reflectors[i]->m_repeater.IsEmpty())
			count++;
	}

	if (count >= m_maxDongles)
		return;

	// An unmatched poll indicates the need for a new entry
	wxLogMessage(wxT("New incoming DExtra Dongle from %s"), reflector.c_str());

	CDExtraHandler* handler = new CDExtraHandler(m_incoming, reflector, yourAddress, yourPort, DIR_INCOMING);

	found = false;

	for (unsigned int i = 0U; i < m_maxReflectors; i++) {
		if (m_reflectors[i] == NULL) {
			m_reflectors[i] = handler;
			found = true;
			break;
		}
	}

	if (found) {
		// Return the poll
		CPollData poll(m_callsign, yourAddress, yourPort);
		m_incoming->writePoll(poll);
	} else {
		wxLogError(wxT("No space to add new DExtra Dongle, ignoring"));
		delete handler;
	}
}

void CDExtraHandler::process(CConnectData& connect)
{
	CD_TYPE type = connect.getType();

	if (type == CT_ACK || type == CT_NAK || type == CT_UNLINK) {
		for (unsigned int i = 0U; i < m_maxReflectors; i++) {
			if (m_reflectors[i] != NULL) {
				bool res = m_reflectors[i]->processInt(connect, type);
				if (res) {
					delete m_reflectors[i];
					m_reflectors[i] = NULL;
				}
			}
		}

		return;
	}

	// else if type == CT_LINK1 or type == CT_LINK2
	in_addr   yourAddress = connect.getYourAddress();
	unsigned int yourPort = connect.getYourPort();

	wxString repeaterCallsign = connect.getRepeater();

	wxChar band = connect.getReflector().GetChar(LONG_CALLSIGN_LENGTH - 1U);

	wxString reflectorCallsign = m_callsign;
	reflectorCallsign.SetChar(LONG_CALLSIGN_LENGTH - 1U, band);

	// Check that it isn't a duplicate
	for (unsigned int i = 0U; i < m_maxReflectors; i++) {
		if (m_reflectors[i] != NULL) {
			if (m_reflectors[i]->m_direction          == DIR_INCOMING &&
			    m_reflectors[i]->m_yourAddress.s_addr == yourAddress.s_addr &&
			    m_reflectors[i]->m_yourPort           == yourPort &&
			    m_reflectors[i]->m_repeater.IsSameAs(reflectorCallsign) &&
			    m_reflectors[i]->m_reflector.IsSameAs(repeaterCallsign))
				return;
		}
	}

	// Check the validity of our repeater callsign
	IReflectorCallback* handler = CRepeaterHandler::findDVRepeater(reflectorCallsign);
	if (handler == NULL) {
		wxLogMessage(wxT("DExtra connect to unknown reflector %s from %s"), reflectorCallsign.c_str(), repeaterCallsign.c_str());
		CConnectData reply(repeaterCallsign, reflectorCallsign, CT_NAK, yourAddress, yourPort);
		m_incoming->writeConnect(reply);
		return;
	}

	// A new connect packet indicates the need for a new entry
	wxLogMessage(wxT("New incoming DExtra link to %s from %s"), reflectorCallsign.c_str(), repeaterCallsign.c_str());

	CDExtraHandler* dextra = new CDExtraHandler(handler, repeaterCallsign, reflectorCallsign, m_incoming, yourAddress, yourPort, DIR_INCOMING);

	bool found = false;
	for (unsigned int i = 0U; i < m_maxReflectors; i++) {
		if (m_reflectors[i] == NULL) {
			m_reflectors[i] = dextra;
			found = true;
			break;
		}
	}

	if (found) {
		CConnectData reply(repeaterCallsign, reflectorCallsign, CT_ACK, yourAddress, yourPort);
		m_incoming->writeConnect(reply);

		wxString callsign = repeaterCallsign;
		callsign.SetChar(LONG_CALLSIGN_LENGTH - 1U, wxT(' '));
		CPollData poll(callsign, yourAddress, yourPort);
		m_incoming->writePoll(poll);
	} else {
		CConnectData reply(repeaterCallsign, reflectorCallsign, CT_NAK, yourAddress, yourPort);
		m_incoming->writeConnect(reply);

		wxLogError(wxT("No space to add new DExtra repeater, ignoring"));
		delete dextra;
	}
}

void CDExtraHandler::link(IReflectorCallback* handler, const wxString& repeater, const wxString &gateway, const in_addr& address)
{
	CDExtraProtocolHandler* protoHandler = m_pool->getHandler();
	if (protoHandler == NULL)
		return;

	CDExtraHandler* dextra = new CDExtraHandler(handler, gateway, repeater, protoHandler, address, DEXTRA_PORT, DIR_OUTGOING);

	bool found = false;

	for (unsigned int i = 0U; i < m_maxReflectors; i++) {
		if (m_reflectors[i] == NULL) {
			m_reflectors[i] = dextra;
			found = true;
			break;
		}
	}

	if (found) {
		CConnectData reply(repeater, gateway, CT_LINK1, address, DEXTRA_PORT);
		protoHandler->writeConnect(reply);
	} else {
		wxLogError(wxT("No space to add new DExtra link, ignoring"));
		delete dextra;
	}
}

void CDExtraHandler::unlink(IReflectorCallback* handler, const wxString& callsign, bool exclude)
{
	for (unsigned int i = 0U; i < m_maxReflectors; i++) {
		CDExtraHandler* reflector = m_reflectors[i];

		if (reflector != NULL) {
			bool found = false;

			if (exclude) {
				if (reflector->m_direction == DIR_OUTGOING && reflector->m_destination == handler && !reflector->m_reflector.IsSameAs(callsign)) {
					wxLogMessage(wxT("Removing outgoing DExtra link %s, %s"), reflector->m_repeater.c_str(), reflector->m_reflector.c_str());

					if (reflector->m_linkState == DEXTRA_LINKING || reflector->m_linkState == DEXTRA_LINKED) {
						CConnectData connect(reflector->m_repeater, reflector->m_yourAddress, reflector->m_yourPort);
						reflector->m_handler->writeConnect(connect);

						reflector->m_linkState = DEXTRA_UNLINKING;

						reflector->m_destination->linkFailed(DP_DEXTRA, reflector->m_reflector, false);
					}

					found = true;
				}
			} else {
				if (reflector->m_destination == handler && reflector->m_reflector.IsSameAs(callsign)) {
					wxLogMessage(wxT("Removing DExtra link %s, %s"), reflector->m_repeater.c_str(), reflector->m_reflector.c_str());

					if (reflector->m_linkState == DEXTRA_LINKING || reflector->m_linkState == DEXTRA_LINKED) {
						CConnectData connect(reflector->m_repeater, reflector->m_yourAddress, reflector->m_yourPort);
						reflector->m_handler->writeConnect(connect);

						reflector->m_linkState = DEXTRA_UNLINKING;

						reflector->m_destination->linkFailed(DP_DEXTRA, reflector->m_reflector, false);
					}

					found = true;
				}
			}

				// If an active link with incoming traffic, send an EOT to the repeater
			if (found) {
				if (reflector->m_dExtraId != 0x00U) {
					unsigned int seq = reflector->m_dExtraSeq + 1U;
					if (seq == 21U)
						seq = 0U;

					CAMBEData data;
					data.setData(END_PATTERN_BYTES, DV_FRAME_LENGTH_BYTES);
					data.setSeq(seq);
					data.setEnd(true);
					data.setId(reflector->m_dExtraId);

					reflector->m_destination->process(data, reflector->m_direction, AS_DEXTRA);
				}

				m_stateChange = true;

				delete m_reflectors[i];
				m_reflectors[i] = NULL;
			}
		}
	}	
}

void CDExtraHandler::unlink()
{
	for (unsigned int i = 0U; i < m_maxReflectors; i++) {
		CDExtraHandler* reflector = m_reflectors[i];

		if (reflector != NULL) {
			if (!reflector->m_repeater.IsEmpty()) {
				wxLogMessage(wxT("Unlinking from DExtra reflector %s"), reflector->m_reflector.c_str());

				CConnectData connect(reflector->m_repeater, reflector->m_yourAddress, reflector->m_yourPort);
				reflector->m_handler->writeConnect(connect);

				reflector->m_linkState = DEXTRA_UNLINKING;
			}
		}
	}	
}

void CDExtraHandler::writeHeader(IReflectorCallback* handler, CHeaderData& header, DIRECTION direction)
{
	for (unsigned int i = 0U; i < m_maxReflectors; i++) {
		if (m_reflectors[i] != NULL)
			m_reflectors[i]->writeHeaderInt(handler, header, direction);
	}	
}

void CDExtraHandler::writeAMBE(IReflectorCallback* handler, CAMBEData& data, DIRECTION direction)
{
	for (unsigned int i = 0U; i < m_maxReflectors; i++) {
		if (m_reflectors[i] != NULL)
			m_reflectors[i]->writeAMBEInt(handler, data, direction);
	}	
}

void CDExtraHandler::gatewayUpdate(const wxString& reflector, const wxString& address)
{
	wxString gateway = reflector;
	gateway.Truncate(LONG_CALLSIGN_LENGTH - 1U);

	for (unsigned int i = 0U; i < m_maxReflectors; i++) {
		CDExtraHandler* reflector = m_reflectors[i];
		if (reflector != NULL) {
			if (reflector->m_reflector.Left(LONG_CALLSIGN_LENGTH - 1U).IsSameAs(gateway)) {
				if (!address.IsEmpty()) {
					// A new address, change the value
					wxLogMessage(wxT("Changing IP address of DExtra gateway or reflector %s to %s"), reflector->m_reflector.c_str(), address.c_str());
					reflector->m_yourAddress.s_addr = ::inet_addr(address.mb_str());
				} else {
					wxLogMessage(wxT("IP address for DExtra gateway or reflector %s has been removed"), reflector->m_reflector.c_str());

					// No address, this probably shouldn't happen....
					if (reflector->m_direction == DIR_OUTGOING && reflector->m_destination != NULL)
						reflector->m_destination->linkFailed(DP_DEXTRA, reflector->m_reflector, false);

					m_stateChange = true;

					delete m_reflectors[i];
					m_reflectors[i] = NULL;
				}
			}
		}
	}
}

void CDExtraHandler::clock(unsigned int ms)
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

void CDExtraHandler::finalise()
{
	for (unsigned int i = 0U; i < m_maxReflectors; i++)
		delete m_reflectors[i];

	delete[] m_reflectors;
}

void CDExtraHandler::processInt(CHeaderData& header)
{
	unsigned int id = header.getId();
	wxString rpt1 = header.getRptCall1();
	wxString rpt2 = header.getRptCall2();

	if (m_linkState != DEXTRA_LINKED)
		return;

	switch (m_direction) {
		case DIR_OUTGOING: {
				// Always a repeater connection
				if (!m_reflector.IsSameAs(rpt2) && !m_reflector.IsSameAs(rpt1))
					return;

				// If we're already processing, ignore the new header
				if (m_dExtraId != 0x00U)
					return;

				// Write to Header.log if it's enabled
				if (m_headerLogger != NULL)
					m_headerLogger->write(wxT("DExtra"), header);

				m_dExtraId  = id;
				m_dExtraSeq = 0x00U;
				m_inactivityTimer.start();

				delete m_header;

				m_header = new CHeaderData(header);
				m_header->setCQCQCQ();
				m_header->setFlags(0x00U, 0x00U, 0x00U);

				m_destination->process(*m_header, m_direction, AS_DEXTRA);
			}
			break;

		case DIR_INCOMING:
			if (!m_repeater.IsEmpty()) {
				// A repeater connection
				if (!m_repeater.IsSameAs(rpt2) && !m_repeater.IsSameAs(rpt1))
					return;

				// If we're already processing, ignore the new header
				if (m_dExtraId != 0x00U)
					return;

				// Write to Header.log if it's enabled
				if (m_headerLogger != NULL)
					m_headerLogger->write(wxT("DExtra"), header);

				m_dExtraId  = id;
				m_dExtraSeq = 0x00U;
				m_inactivityTimer.start();

				delete m_header;

				m_header = new CHeaderData(header);
				m_header->setCQCQCQ();
				m_header->setFlags(0x00U, 0x00U, 0x00U);

				m_destination->process(*m_header, m_direction, AS_DEXTRA);
			} else {
				// A Dongle connection
				// Check the destination callsign
				m_destination = CRepeaterHandler::findDVRepeater(rpt2);
				if (m_destination == NULL) {
					m_destination = CRepeaterHandler::findDVRepeater(rpt1);
					if (m_destination == NULL)
						return;
				}

				// If we're already processing, ignore the new header
				if (m_dExtraId != 0x00U)
					return;

				// Write to Header.log if it's enabled
				if (m_headerLogger != NULL)
					m_headerLogger->write(wxT("DExtra"), header);

				m_dExtraId  = id;
				m_dExtraSeq = 0x00U;
				m_inactivityTimer.start();

				delete m_header;

				m_header = new CHeaderData(header);
				m_header->setCQCQCQ();
				m_header->setFlags(0x00U, 0x00U, 0x00U);

				m_destination->process(*m_header, m_direction, AS_DEXTRA);
			}
			break;
	}
}

void CDExtraHandler::processInt(CAMBEData& data)
{
	if (m_linkState != DEXTRA_LINKED)
		return;

	if (m_dExtraId != data.getId())
		return;

	m_pollInactivityTimer.start();
	m_inactivityTimer.start();

	m_dExtraSeq = data.getSeq();

	// Send the header every 21 frames
	if (m_dExtraSeq == 0U)
		m_destination->process(*m_header, m_direction, AS_DUP);

	// Copy the data to ensure it remains unchanged
	CAMBEData temp(data);

	m_destination->process(temp, m_direction, AS_DEXTRA);

	if (temp.isEnd()) {
		delete m_header;
		m_header = NULL;

		m_dExtraId  = 0x00U;
		m_dExtraSeq = 0x00U;

		m_inactivityTimer.stop();
	}
}

bool CDExtraHandler::processInt(CConnectData& connect, CD_TYPE type)
{
	in_addr yourAddress   = connect.getYourAddress();
	unsigned int yourPort = connect.getYourPort();
	wxString     repeater = connect.getRepeater();

	if (m_yourAddress.s_addr != yourAddress.s_addr || m_yourPort != yourPort)
		return false;

	switch (type) {
		case CT_ACK:
			if (!m_repeater.IsSameAs(repeater))
				return false;

			if (m_linkState == DEXTRA_LINKING) {
				wxLogMessage(wxT("DExtra ACK message received from %s"), m_reflector.c_str());

				if (m_direction == DIR_OUTGOING && m_destination != NULL)
					m_destination->linkUp(DP_DEXTRA, m_reflector);

				m_tryTimer.stop();
				m_pollTimer.start();
				m_stateChange = true;
				m_linkState   = DEXTRA_LINKED;
			}

			return false;

		case CT_NAK:
			if (!m_repeater.IsSameAs(repeater))
				return false;

			if (m_linkState == DEXTRA_LINKING) {
				wxLogMessage(wxT("DExtra NAK message received from %s"), m_reflector.c_str());

				if (m_direction == DIR_OUTGOING && m_destination != NULL)
					m_destination->linkRefused(DP_DEXTRA, m_reflector);

				return true;
			}

			return false;

		case CT_UNLINK:
			if (!m_reflector.IsSameAs(repeater))
				return false;

			if (m_linkState == DEXTRA_LINKED) {
				wxLogMessage(wxT("DExtra disconnect message received from %s"), m_reflector.c_str());

				if (m_direction == DIR_OUTGOING && m_destination != NULL)
					m_destination->linkFailed(DP_DEXTRA, m_reflector, false);

				m_stateChange = true;
			}

			return true;

		default:
			return false;
	}
}

bool CDExtraHandler::clockInt(unsigned int ms)
{
	m_tryTimer.clock(ms);
	m_pollTimer.clock(ms);
	m_inactivityTimer.clock(ms);
	m_pollInactivityTimer.clock(ms);

	if (m_pollInactivityTimer.isRunning() && m_pollInactivityTimer.hasExpired()) {
		m_pollInactivityTimer.start();

		delete m_header;
		m_header = NULL;

		m_stateChange = true;
		m_dExtraId    = 0x00U;
		m_dExtraSeq   = 0x00U;

		switch (m_linkState) {
			case DEXTRA_LINKING:
				wxLogMessage(wxT("DExtra link to %s has failed to connect"), m_reflector.c_str());
				break;
			case DEXTRA_LINKED:
				wxLogMessage(wxT("DExtra link to %s has failed (poll inactivity)"), m_reflector.c_str());
				break;
			case DEXTRA_UNLINKING:
				wxLogMessage(wxT("DExtra link to %s has failed to disconnect cleanly"), m_reflector.c_str());
				break;
			default:
				break;
		}

		if (m_direction == DIR_OUTGOING) {
			bool reconnect = m_destination->linkFailed(DP_DEXTRA, m_reflector, true);
			if (reconnect) {
				CConnectData reply(m_repeater, m_reflector, CT_LINK1, m_yourAddress, m_yourPort);
				m_handler->writeConnect(reply);
				m_linkState = DEXTRA_LINKING;
				m_tryTimer.start(1U);
				m_tryCount = 0U;
				return false;
			}
		}

		return true;
	}

	if (m_pollTimer.isRunning() && m_pollTimer.hasExpired()) {
		if (m_linkState == DEXTRA_LINKED) {
			if (!m_repeater.IsEmpty()) {
				wxString callsign = m_repeater;
				callsign.SetChar(LONG_CALLSIGN_LENGTH - 1U, wxT(' '));
				CPollData poll(callsign, m_yourAddress, m_yourPort);
				m_handler->writePoll(poll);
			} else {
				CPollData poll(m_callsign, m_yourAddress, m_yourPort);
				m_handler->writePoll(poll);
			}
		}

		m_pollTimer.start();
	}

	if (m_inactivityTimer.isRunning() && m_inactivityTimer.hasExpired()) {
		delete m_header;
		m_header = NULL;

		m_dExtraId  = 0x00U;
		m_dExtraSeq = 0x00U;

		m_inactivityTimer.stop();
	}

	if (m_linkState == DEXTRA_LINKING) {
		if (m_tryTimer.isRunning() && m_tryTimer.hasExpired()) {
			CConnectData reply(m_repeater, m_reflector, CT_LINK1, m_yourAddress, m_yourPort);
			m_handler->writeConnect(reply);

			unsigned int timeout = calcBackoff();
			m_tryTimer.start(timeout);
		}
	}

	return false;
}

void CDExtraHandler::writeHeaderInt(IReflectorCallback* handler, CHeaderData& header, DIRECTION direction)
{
	if (m_linkState != DEXTRA_LINKED)
		return;

	// Is it link in the right direction
	if (m_direction != direction)
		return;

	// Already in use?
	if (m_dExtraId != 0x00)
		return;

	switch (m_direction) {
		case DIR_OUTGOING:
			if (m_destination == handler) {
				header.setDestination(m_yourAddress, m_yourPort);
				m_handler->writeHeader(header);
			}
			break;

		case DIR_INCOMING:
			if (m_repeater.IsEmpty() || m_destination == handler) {
				header.setDestination(m_yourAddress, m_yourPort);
				m_handler->writeHeader(header);
			}
			break;
	}
}

void CDExtraHandler::writeAMBEInt(IReflectorCallback* handler, CAMBEData& data, DIRECTION direction)
{
	if (m_linkState != DEXTRA_LINKED)
		return;

	// Is it link in the right direction
	if (m_direction != direction)
		return;

	// Already in use?
	if (m_dExtraId != 0x00)
		return;

	switch (m_direction) {
		case DIR_OUTGOING:
			if (m_destination == handler) {
				data.setDestination(m_yourAddress, m_yourPort);
				m_handler->writeAMBE(data);
			}
			break;

		case DIR_INCOMING:
			if (m_repeater.IsEmpty() || m_destination == handler) {
				data.setDestination(m_yourAddress, m_yourPort);
				m_handler->writeAMBE(data);
			}
			break;
	}
}

bool CDExtraHandler::stateChange()
{
	bool stateChange = m_stateChange;

	m_stateChange = false;

	return stateChange;
}

void CDExtraHandler::writeStatus(wxFFile& file)
{
	for (unsigned int i = 0U; i < m_maxReflectors; i++) {
		CDExtraHandler* reflector = m_reflectors[i];
		if (reflector != NULL) {
			wxString text;

			struct tm* tm = ::gmtime(&reflector->m_time);

			switch (reflector->m_direction) {
				case DIR_OUTGOING:
					if (reflector->m_linkState == DEXTRA_LINKED) {
						text.Printf(wxT("%04d-%02d-%02d %02d:%02d:%02d: DExtra link - Type: Repeater Rptr: %s Refl: %s Dir: Outgoing\n"),
							tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec, 
							reflector->m_repeater.c_str(), reflector->m_reflector.c_str());

						file.Write(text);
					}
					break;

				case DIR_INCOMING:
					if (reflector->m_linkState == DEXTRA_LINKED) {
						if (reflector->m_repeater.IsEmpty())
							text.Printf(wxT("%04d-%02d-%02d %02d:%02d:%02d: DExtra link - Type: Dongle User: %s Dir: Incoming\n"),
								tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec, 
								reflector->m_reflector.c_str());
						else
							text.Printf(wxT("%04d-%02d-%02d %02d:%02d:%02d: DExtra link - Type: Repeater Rptr: %s Refl: %s Dir: Incoming\n"),
								tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec, 
								reflector->m_repeater.c_str(), reflector->m_reflector.c_str());

						file.Write(text);
					}
					break;
			}

		}
	}
}

unsigned int CDExtraHandler::calcBackoff()
{
	if (m_tryCount >= 7U) {
		m_tryCount++;
		return 60U;
	}

	unsigned int timeout = 1U;

	for (unsigned int i = 0U; i < m_tryCount; i++)
		timeout *= 2U;

	m_tryCount++;

	if (timeout > 60U)
		return 60U;
	else
		return timeout;
}
