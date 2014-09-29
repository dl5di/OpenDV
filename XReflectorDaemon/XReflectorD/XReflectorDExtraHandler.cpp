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

#include "XReflectorReflectorHandler.h"
#include "XReflectorDExtraHandler.h"
#include "DStarDefines.h"
#include "Utils.h"

unsigned int               CXReflectorDExtraHandler::m_maxReflectors = 0U;
CXReflectorDExtraHandler** CXReflectorDExtraHandler::m_reflectors = NULL;

wxString                   CXReflectorDExtraHandler::m_callsign;
CDExtraProtocolHandler*    CXReflectorDExtraHandler::m_handler = NULL;

bool                       CXReflectorDExtraHandler::m_stateChange = false;

CXReflectorUserLog*        CXReflectorDExtraHandler::m_userLogger = NULL;


CXReflectorDExtraHandler::CXReflectorDExtraHandler(IXReflectorReflectorCallback* handler, const wxString& reflector, const wxString& repeater, const in_addr& address, unsigned int port) :
m_reflector(reflector),
m_repeater(repeater),
m_address(address),
m_port(port),
m_linked(false),
m_destination(handler),
m_time(),
m_pollTimer(1000U, 10U),			// 10s
m_pollInactivityTimer(1000U, 60U),	// 60s
m_dExtraId(0x00),
m_rptrId(0x00),
m_inactivityTimer(1000U, 2U)		// 2s
{
	wxASSERT(handler != NULL);
	wxASSERT(port > 0U);

	m_pollTimer.start();
	m_pollInactivityTimer.start();

	m_time = ::time(NULL);

	m_stateChange = true;
	m_linked      = true;
}

CXReflectorDExtraHandler::CXReflectorDExtraHandler(const wxString& reflector, const in_addr& address, unsigned int port) :
m_reflector(reflector),
m_repeater(),
m_address(address),
m_port(port),
m_linked(false),
m_destination(NULL),
m_time(),
m_pollTimer(1000U, 10U),
m_pollInactivityTimer(1000U, 60U),
m_dExtraId(0x00),
m_rptrId(0x00),
m_inactivityTimer(1000U, 2U)
{
	wxASSERT(port > 0U);

	m_pollTimer.start();
	m_pollInactivityTimer.start();

	m_time = ::time(NULL);

	m_stateChange = true;
	m_linked      = true;
}

CXReflectorDExtraHandler::~CXReflectorDExtraHandler()
{
}

void CXReflectorDExtraHandler::initialise(unsigned int maxReflectors)
{
	wxASSERT(maxReflectors > 0U);

	m_maxReflectors = maxReflectors;

	m_reflectors = new CXReflectorDExtraHandler*[m_maxReflectors];
	for (unsigned int i = 0U; i < m_maxReflectors; i++)
		m_reflectors[i] = NULL;
}

void CXReflectorDExtraHandler::setCallsign(const wxString& callsign)
{
	m_callsign = callsign;
	m_callsign.Append(wxT("        "));
	m_callsign.Truncate(LONG_CALLSIGN_LENGTH - 1U);
	m_callsign.Append(wxT(" "));
}

void CXReflectorDExtraHandler::setDExtraProtocolHandler(CDExtraProtocolHandler* handler)
{
	wxASSERT(handler != NULL);

	m_handler = handler;
}

void CXReflectorDExtraHandler::setUserLogger(CXReflectorUserLog* logger)
{
	m_userLogger = logger;
}

void CXReflectorDExtraHandler::process(CHeaderData& header)
{
	for (unsigned int i = 0U; i < m_maxReflectors; i++) {
		CXReflectorDExtraHandler* reflector = m_reflectors[i];
		if (reflector != NULL) {
			if (reflector->m_address.s_addr == header.getYourAddress().s_addr && reflector->m_port == header.getYourPort())
				reflector->processInt(header);
		}
	}	
}

void CXReflectorDExtraHandler::process(CAMBEData& data)
{
	for (unsigned int i = 0U; i < m_maxReflectors; i++) {
		CXReflectorDExtraHandler* reflector = m_reflectors[i];
		if (reflector != NULL) {
			if (reflector->m_address.s_addr == data.getYourAddress().s_addr && reflector->m_port == data.getYourPort())
				reflector->processInt(data);
		}
	}	
}

void CXReflectorDExtraHandler::process(const CPollData& poll)
{
	bool found = false;

	wxString reflector = poll.getData1();
	in_addr  address   = poll.getYourAddress();
	unsigned int port  = poll.getYourPort();

	// Check to see if we already have a link
	for (unsigned int i = 0U; i < m_maxReflectors; i++) {
		if (m_reflectors[i] != NULL) {
			if (m_reflectors[i]->m_reflector.Left(LONG_CALLSIGN_LENGTH - 1U).IsSameAs(reflector.Left(LONG_CALLSIGN_LENGTH - 1U)) &&
				m_reflectors[i]->m_address.s_addr == address.s_addr && m_reflectors[i]->m_port == port) {
				m_reflectors[i]->m_pollInactivityTimer.reset();
				found = true;
			}
		}
	}	

	if (found)
		return;

	// A repeater poll arriving here is an error
	if (!poll.isDongle()) {
		wxLogMessage(wxT("%s is trying to connect as a DExtra Dongle, ignoring"), reflector.c_str());
		return;
	}

	// An unmatched poll indicates the need for a new entry
	wxLogMessage(wxT("New incoming DExtra Dongle from %s"), reflector.c_str());

	CXReflectorDExtraHandler* handler = new CXReflectorDExtraHandler(reflector, address, port);

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
		CPollData poll(m_callsign, address, port, 0U);
		m_handler->writePoll(poll);
	} else {
		wxLogError(wxT("No space to add new DExtra Dongle, ignoring"));
		delete handler;
	}
}

void CXReflectorDExtraHandler::process(CConnectData& connect)
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
	in_addr   address = connect.getYourAddress();
	unsigned int port = connect.getYourPort();

	wxString repeaterCallsign = connect.getRepeater();

	wxChar band = connect.getReflector().GetChar(LONG_CALLSIGN_LENGTH - 1U);

	wxString reflectorCallsign = m_callsign;
	reflectorCallsign.SetChar(LONG_CALLSIGN_LENGTH - 1U, band);

	// Check that it isn't a duplicate
	for (unsigned int i = 0U; i < m_maxReflectors; i++) {
		if (m_reflectors[i] != NULL) {
			if (m_reflectors[i]->m_address.s_addr == address.s_addr &&
			    m_reflectors[i]->m_port == port &&
			    m_reflectors[i]->m_repeater.IsSameAs(reflectorCallsign) &&
			    m_reflectors[i]->m_reflector.IsSameAs(repeaterCallsign))
				return;
		}
	}

	// Check the validity of our repeater callsign
	IXReflectorReflectorCallback* handler = CXReflectorReflectorHandler::findReflector(reflectorCallsign);
	if (handler == NULL) {
		wxLogMessage(wxT("DExtra connect to unknown reflector %s from %s"), reflectorCallsign.c_str(), repeaterCallsign.c_str());
		CConnectData reply(repeaterCallsign, reflectorCallsign, CT_NAK, address, port, 0U);
		m_handler->writeConnect(reply);
		return;
	}

	// A new connect packet indicates the need for a new entry
	wxLogMessage(wxT("New incoming DExtra link to %s from %s"), reflectorCallsign.c_str(), repeaterCallsign.c_str());

	CXReflectorDExtraHandler* dextra = new CXReflectorDExtraHandler(handler, repeaterCallsign, reflectorCallsign, address, port);

	bool found = false;
	for (unsigned int i = 0U; i < m_maxReflectors; i++) {
		if (m_reflectors[i] == NULL) {
			m_reflectors[i] = dextra;
			found = true;
			break;
		}
	}

	if (found) {
		CConnectData reply(repeaterCallsign, reflectorCallsign, CT_ACK, address, port, 0U);
		m_handler->writeConnect(reply);

		CPollData poll(m_callsign, address, port, 0U);
		m_handler->writePoll(poll);
	} else {
		CConnectData reply(repeaterCallsign, reflectorCallsign, CT_NAK, address, port, 0U);
		m_handler->writeConnect(reply);

		wxLogError(wxT("No space to add new reflector, ignoring"));
		delete dextra;
	}
}

void CXReflectorDExtraHandler::unlink()
{
	for (unsigned int i = 0U; i < m_maxReflectors; i++) {
		CXReflectorDExtraHandler* handler = m_reflectors[i];
		if (handler != NULL) {
			if (!handler->m_repeater.IsEmpty()) {
				wxLogMessage(wxT("Unlinking from DExtra reflector %s"), handler->m_reflector.c_str());
				CConnectData connect(handler->m_repeater, handler->m_address, handler->m_port, 0U);
				m_handler->writeConnect(connect);
			}
		}
	}	
}

void CXReflectorDExtraHandler::writeHeader(const wxString& callsign, CHeaderData& header)
{
	for (unsigned int i = 0U; i < m_maxReflectors; i++) {
		if (m_reflectors[i] != NULL)
			m_reflectors[i]->writeHeaderInt(callsign, header);
	}	
}

void CXReflectorDExtraHandler::writeAMBE(CAMBEData& data)
{
	for (unsigned int i = 0U; i < m_maxReflectors; i++) {
		if (m_reflectors[i] != NULL)
			m_reflectors[i]->writeAMBEInt(data);
	}	
}

void CXReflectorDExtraHandler::writeDongleHeader(CHeaderData& header)
{
	for (unsigned int i = 0U; i < m_maxReflectors; i++) {
		if (m_reflectors[i] != NULL)
			m_reflectors[i]->writeDongleHeaderInt(header);
	}	
}

void CXReflectorDExtraHandler::writeDongleAMBE(CAMBEData& data)
{
	for (unsigned int i = 0U; i < m_maxReflectors; i++) {
		if (m_reflectors[i] != NULL)
			m_reflectors[i]->writeDongleAMBEInt(data);
	}	
}

void CXReflectorDExtraHandler::clock(unsigned long ms)
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

void CXReflectorDExtraHandler::finalise()
{
	for (unsigned int i = 0U; i < m_maxReflectors; i++)
		delete m_reflectors[i];

	delete[] m_reflectors;
}

void CXReflectorDExtraHandler::processInt(CHeaderData& header)
{
	unsigned int id = header.getId();

	if (!m_repeater.IsEmpty()) {
		// A repeater connection
		if (!m_repeater.IsSameAs(header.getRptCall2()))
			return;

		// If we're already processing, ignore the new header
		if (m_dExtraId != 0x00)
			return;

		m_dExtraId = id;
		m_inactivityTimer.start();

		header.setCQCQCQ();
		header.setFlag1(0x00U);
		bool ret = m_destination->process(m_dExtraId, header);

		// Write to Users.log if it's enabled
		if (ret && m_userLogger != NULL)
			m_userLogger->write(header.getMyCall1(), m_repeater, m_reflector, PROTO_DEXTRA);
	} else {
		// A Dongle connection
		// Check the destination callsign
		m_destination = CXReflectorReflectorHandler::findReflector(header.getRptCall2());
		if (m_destination == NULL)
			return;

		// If we're already processing, ignore the new header
		if (m_dExtraId != 0x00)
			return;

		m_dExtraId = id;
		m_inactivityTimer.start();

		header.setCQCQCQ();
		header.setFlag1(0x00U);
		bool ret = m_destination->process(m_dExtraId, header);

		// Write to Users.log if it's enabled
		if (ret && m_userLogger != NULL)
			m_userLogger->write(header.getMyCall1(), header.getRptCall2(), header.getMyCall1(), PROTO_DEXTRA);
	}
}

void CXReflectorDExtraHandler::processInt(CAMBEData& data)
{
	unsigned int id = data.getId();
	if (m_dExtraId != id)
		return;

	m_inactivityTimer.reset();

	m_destination->process(m_dExtraId, data);

	if (data.isEnd()) {
		m_dExtraId = 0x00;
		m_inactivityTimer.stop();
	}
}

bool CXReflectorDExtraHandler::processInt(CConnectData& connect, CD_TYPE type)
{
	in_addr address   = connect.getYourAddress();
	unsigned int port = connect.getYourPort();
	wxString repeater = connect.getRepeater();

	if (m_address.s_addr != address.s_addr || m_port != port)
		return false;

	switch (type) {
		case CT_ACK:
			if (!m_repeater.IsSameAs(repeater))
				return false;

			if (!m_linked) {
				wxLogMessage(wxT("DExtra ACK message received from %s"), m_reflector.c_str());
				m_pollTimer.start();
				m_stateChange = true;
				m_linked      = true;
			}

			return false;

		case CT_NAK:
			if (!m_repeater.IsSameAs(repeater))
				return false;

			if (!m_linked)
				wxLogMessage(wxT("DExtra NAK message received from %s"), m_reflector.c_str());

			if (m_dExtraId != 0x00U && m_destination != NULL)
				m_destination->timeout(m_dExtraId);

			return true;

		case CT_UNLINK:
			if (!m_reflector.IsSameAs(repeater))
				return false;

			if (m_linked) {
				wxLogMessage(wxT("DExtra disconnect message received from %s"), m_reflector.c_str());
				m_stateChange = true;
			}

			if (m_dExtraId != 0x00U && m_destination != NULL)
				m_destination->timeout(m_dExtraId);

			return true;

		default:
			return false;
	}
}

bool CXReflectorDExtraHandler::clockInt(unsigned long ms)
{
	m_pollTimer.clock(ms);
	m_inactivityTimer.clock(ms);
	m_pollInactivityTimer.clock(ms);

	if (m_pollTimer.isRunning() && m_pollTimer.hasExpired()) {
		if (m_linked) {
			CPollData poll(m_callsign, m_address, m_port, 0U);
			m_handler->writePoll(poll);
		} else {
			CConnectData reply(m_repeater, m_reflector, CT_LINK1, m_address, m_port, 0U);
			m_handler->writeConnect(reply);
		}

		m_pollTimer.reset();
	}

	if (m_pollInactivityTimer.isRunning() && m_pollInactivityTimer.hasExpired()) {
		wxLogMessage(wxT("Incoming DExtra link to %s has ended (poll inactivity)"), m_reflector.c_str());

		if (m_dExtraId != 0x00U && m_destination != NULL)
			m_destination->timeout(m_dExtraId);

		m_stateChange = true;
		return true;
	}

	if (m_inactivityTimer.isRunning() && m_inactivityTimer.hasExpired()) {
		wxLogMessage(wxT("DExtra inactivity timer expired"));

		if (m_dExtraId != 0x00U && m_destination != NULL)
			m_destination->timeout(m_dExtraId);

		m_dExtraId = 0x00;
		m_inactivityTimer.stop();
	}

	return false;
}

void CXReflectorDExtraHandler::writeHeaderInt(const wxString& callsign, CHeaderData& header)
{
	if (!m_linked)
		return;

	// Do the callsigns match, this also reject Dongle links
	if (!callsign.IsSameAs(m_repeater))
		return;

	// Already in use?
	if (m_dExtraId != 0x00)
		return;

	header.setDestination(m_address, m_port);
	m_handler->writeHeader(header);
	m_rptrId = header.getId();
}

void CXReflectorDExtraHandler::writeDongleHeaderInt(CHeaderData& header)
{
	if (!m_linked)
		return;

	// Make sure this is a Dongle link
	if (!m_repeater.IsEmpty())
		return;

	// Already in use?
	if (m_dExtraId != 0x00)
		return;

	header.setDestination(m_address, m_port);
	m_handler->writeHeader(header);
	m_rptrId = header.getId();
}

void CXReflectorDExtraHandler::writeAMBEInt(CAMBEData& data)
{
	if (!m_linked)
		return;

	// Make sure this is a repeater link
	if (m_repeater.IsEmpty())
		return;

	// If the ids don't match, reject
	if (data.getId() != m_rptrId)
		return;

	// Already in use?
	if (m_dExtraId != 0x00)
		return;

	data.setDestination(m_address, m_port);
	m_handler->writeAMBE(data);

	if (data.isEnd())
		m_rptrId = 0x00;
}

void CXReflectorDExtraHandler::writeDongleAMBEInt(CAMBEData& data)
{
	if (!m_linked)
		return;

	// Make sure this is a Dongle link
	if (!m_repeater.IsEmpty())
		return;

	// If the ids don't match, reject
	if (data.getId() != m_rptrId)
		return;

	// Already in use?
	if (m_dExtraId != 0x00)
		return;

	data.setDestination(m_address, m_port);
	m_handler->writeAMBE(data);

	if (data.isEnd())
		m_rptrId = 0x00;
}

bool CXReflectorDExtraHandler::stateChange()
{
	bool stateChange = m_stateChange;

	m_stateChange = false;

	return stateChange;
}

void CXReflectorDExtraHandler::writeStatus(wxFFile& file)
{
	for (unsigned int i = 0U; i < m_maxReflectors; i++) {
		CXReflectorDExtraHandler* reflector = m_reflectors[i];
		if (reflector != NULL) {
			struct tm* tm = ::gmtime(&reflector->m_time);

			wxString text;
			if (reflector->m_repeater.IsEmpty())
				text.Printf(wxT("%04d-%02d-%02d %02d:%02d:%02d: DExtra link - Type: Dongle User: %s Dir: Inbound\n"),
					tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec, 
					reflector->m_reflector.c_str());
			else
				text.Printf(wxT("%04d-%02d-%02d %02d:%02d:%02d: DExtra link - Type: Repeater Rptr: %s Refl: %s Dir: Inbound\n"),
					tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec, 
					reflector->m_repeater.c_str(), reflector->m_reflector.c_str());

			file.Write(text);
		}
	}
}
