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
#include "XReflectorDPlusHandler.h"
#include "DStarDefines.h"
#include "Utils.h"

#include <wx/filename.h>

unsigned int              CXReflectorDPlusHandler::m_maxReflectors = 0U;
CXReflectorDPlusHandler** CXReflectorDPlusHandler::m_reflectors = NULL;

wxString                  CXReflectorDPlusHandler::m_dplusCallsign;
CDPlusProtocolHandler*    CXReflectorDPlusHandler::m_handler = NULL;

bool                      CXReflectorDPlusHandler::m_stateChange = false;

CXReflectorUserLog*       CXReflectorDPlusHandler::m_userLogger = NULL;


CXReflectorDPlusHandler::CXReflectorDPlusHandler(const in_addr& address, unsigned int port) :
m_reflector(),
m_address(address),
m_port(port),
m_linked(false),
m_destination(NULL),
m_time(),
m_pollTimer(1000U, 1U),					// 1s
m_pollInactivityTimer(1000U, 30U),		// 30s
m_dPlusId(0x00),
m_inactivityTimer(1000U, 2U)			// 2s
{
	wxASSERT(port > 0U);

	m_pollInactivityTimer.start();

	m_time = ::time(NULL);
}

CXReflectorDPlusHandler::~CXReflectorDPlusHandler()
{
}

void CXReflectorDPlusHandler::initialise(unsigned int maxReflectors)
{
	wxASSERT(maxReflectors > 0U);

	m_maxReflectors = maxReflectors;

	m_reflectors = new CXReflectorDPlusHandler*[m_maxReflectors];
	for (unsigned int i = 0U; i < m_maxReflectors; i++)
		m_reflectors[i] = NULL;
}

void CXReflectorDPlusHandler::setCallsign(const wxString& callsign)
{
	m_dplusCallsign = callsign;
	m_dplusCallsign.Append(wxT("        "));
	m_dplusCallsign.Truncate(LONG_CALLSIGN_LENGTH - 1U);
	m_dplusCallsign.Append(wxT(" "));
}

void CXReflectorDPlusHandler::setDPlusProtocolHandler(CDPlusProtocolHandler* handler)
{
	wxASSERT(handler != NULL);

	m_handler = handler;
}

void CXReflectorDPlusHandler::setUserLogger(CXReflectorUserLog* logger)
{
	m_userLogger = logger;
}

void CXReflectorDPlusHandler::process(CHeaderData& header)
{
	in_addr address = header.getYourAddress();

	for (unsigned int i = 0U; i < m_maxReflectors; i++) {
		CXReflectorDPlusHandler* reflector = m_reflectors[i];
		if (reflector != NULL) {
			if (reflector->m_address.s_addr == address.s_addr)
				reflector->processInt(header);
		}
	}	
}

void CXReflectorDPlusHandler::process(CAMBEData& data)
{
	in_addr address = data.getYourAddress();

	for (unsigned int i = 0U; i < m_maxReflectors; i++) {
		CXReflectorDPlusHandler* reflector = m_reflectors[i];
		if (reflector != NULL) {
			if (reflector->m_address.s_addr == address.s_addr)
				reflector->processInt(data);
		}
	}	
}

void CXReflectorDPlusHandler::process(const CPollData& poll)
{
	in_addr address = poll.getYourAddress();

	for (unsigned int i = 0U; i < m_maxReflectors; i++) {
		if (m_reflectors[i] != NULL) {
			if (m_reflectors[i]->m_address.s_addr == address.s_addr) {
				m_reflectors[i]->m_pollInactivityTimer.reset();
				return;
			}
		}
	}	

	// If we cannot find an existing link, we ignore the poll
	wxLogMessage(wxT("Incoming poll from unknown reflector/dongle"));
}

void CXReflectorDPlusHandler::process(CConnectData& connect)
{
	CD_TYPE    type = connect.getType();
	in_addr address = connect.getYourAddress();

	for (unsigned int i = 0U; i < m_maxReflectors; i++) {
		CXReflectorDPlusHandler* reflector = m_reflectors[i];
		if (reflector != NULL) {
			if (reflector->m_address.s_addr == address.s_addr) {
				bool res = m_reflectors[i]->processInt(connect, type);
				if (res) {
					delete m_reflectors[i];
					m_reflectors[i] = NULL;
				}
			}
		}
	}

	unsigned int port = connect.getYourPort();

	// Check that it isn't a duplicate
	for (unsigned int i = 0U; i < m_maxReflectors; i++) {
		if (m_reflectors[i] != NULL) {
			if (m_reflectors[i]->m_address.s_addr == address.s_addr)
				return;
		}
	}

	if (type == CT_UNLINK)
		return;

	if (type != CT_LINK1) {
		wxLogMessage(wxT("Incoming D-Plus message from unknown source"));
		return;
	}

	CXReflectorDPlusHandler* dplus = new CXReflectorDPlusHandler(address, port);

	bool found = false;

	for (unsigned int i = 0U; i < m_maxReflectors; i++) {
		if (m_reflectors[i] == NULL) {
			m_reflectors[i] = dplus;
			found = true;
			break;
		}
	}

	if (found) {
		CConnectData connect(CT_LINK1, address, port, 0U);
		m_handler->writeConnect(connect);
	} else {
		wxLogError(wxT("No space to add new D-Plus dongle, ignoring"));
		delete dplus;
	}
}

void CXReflectorDPlusHandler::unlink()
{
	for (unsigned int i = 0U; i < m_maxReflectors; i++) {
		CXReflectorDPlusHandler* handler = m_reflectors[i];
		if (handler != NULL) {
			if (!handler->m_reflector.IsEmpty())
				wxLogMessage(wxT("Unlinking from D-Plus reflector/dongle %s"), handler->m_reflector.c_str());

			CConnectData connect(CT_UNLINK, handler->m_address, handler->m_port, 0U);
			m_handler->writeConnect(connect);
			m_handler->writeConnect(connect);
		}
	}	
}

void CXReflectorDPlusHandler::writeHeader(CHeaderData& header)
{
	for (unsigned int i = 0U; i < m_maxReflectors; i++) {
		if (m_reflectors[i] != NULL)
			m_reflectors[i]->writeHeaderInt(header);
	}	
}

void CXReflectorDPlusHandler::writeAMBE(CAMBEData& data)
{
	for (unsigned int i = 0U; i < m_maxReflectors; i++) {
		if (m_reflectors[i] != NULL)
			m_reflectors[i]->writeAMBEInt(data);
	}	
}

void CXReflectorDPlusHandler::clock(unsigned long ms)
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

void CXReflectorDPlusHandler::finalise()
{
	for (unsigned int i = 0U; i < m_maxReflectors; i++)
		delete m_reflectors[i];

	delete[] m_reflectors;
}

void CXReflectorDPlusHandler::processInt(CHeaderData& header)
{
	wxString   rpt1 = header.getRptCall1();
	wxString   rpt2 = header.getRptCall2();
	unsigned int id = header.getId();

	if (!m_linked)
		return;

	// Convert the callsign(s) from the DPlus call to the repeater call
	wxString rpt = m_dplusCallsign;

	wxChar band = rpt2.GetChar(LONG_CALLSIGN_LENGTH - 1U);
	rpt.SetChar(LONG_CALLSIGN_LENGTH - 1U, band);

	// Check the destination callsign
	m_destination = CXReflectorReflectorHandler::findReflector(rpt);
	if (m_destination == NULL) {
		band = rpt1.GetChar(LONG_CALLSIGN_LENGTH - 1U);
		rpt.SetChar(LONG_CALLSIGN_LENGTH - 1U, band);

		m_destination = CXReflectorReflectorHandler::findReflector(rpt);
		if (m_destination == NULL) {
			wxLogMessage(wxT("Incoming unmatched D-Plus header, RPT1=%s, RPT2=%s"), rpt1.c_str(), rpt2.c_str());
			return;
		}
	}

	if (m_dPlusId != 0x00)
		return;

	m_dPlusId = id;
	m_inactivityTimer.start();
	m_pollInactivityTimer.reset();

	header.setCQCQCQ();
	header.setFlag1(0x00U);
	bool ret = m_destination->process(m_dPlusId, header);

	// Write to Users.log if it's enabled
	if (ret && m_userLogger != NULL)
		m_userLogger->write(header.getMyCall1(), header.getRptCall2(), m_reflector, PROTO_DPLUS);
}

void CXReflectorDPlusHandler::processInt(CAMBEData& data)
{
	unsigned int id = data.getId();

	if (m_dPlusId == id) {
		m_inactivityTimer.reset();
		m_pollInactivityTimer.reset();
		m_destination->process(m_dPlusId, data);

		if (data.isEnd()) {
			m_dPlusId = 0x00;
			m_inactivityTimer.stop();
		}
	}
}

bool CXReflectorDPlusHandler::processInt(CConnectData& connect, CD_TYPE type)
{
	switch (type) {
		case CT_LINK2: {
				m_reflector = connect.getRepeater();
				wxLogMessage(wxT("D-Plus dongle link to %s has started"), m_reflector.c_str());
				CConnectData reply(CT_ACK, m_address, m_port, 0U);
				m_handler->writeConnect(reply);
				m_linked = true;
				m_stateChange = true;
				m_pollTimer.start();
			}
			return false;

		case CT_UNLINK:
			if (m_linked) {
				wxLogMessage(wxT("D-Plus dongle link to %s has ended (unlinked)"), m_reflector.c_str());

				if (m_dPlusId != 0x00U && m_destination != NULL)
					m_destination->timeout(m_dPlusId);

				m_stateChange = true;
				m_handler->writeConnect(connect);
			}
			return true;

		default:
			return false;
	}
}

bool CXReflectorDPlusHandler::clockInt(unsigned long ms)
{
	m_pollTimer.clock(ms);
	m_inactivityTimer.clock(ms);
	m_pollInactivityTimer.clock(ms);

	if (m_pollTimer.isRunning() && m_pollTimer.hasExpired()) {
		CPollData poll(m_address, m_port, 0U);
		m_handler->writePoll(poll);
		m_pollTimer.reset();
	}

	if (m_pollInactivityTimer.isRunning() && m_pollInactivityTimer.hasExpired()) {
		if (!m_reflector.IsEmpty())
			wxLogMessage(wxT("D-Plus dongle link to %s has ended (poll inactivity)"), m_reflector.c_str());

		if (m_dPlusId != 0x00U && m_destination != NULL)
			m_destination->timeout(m_dPlusId);

		m_stateChange = true;

		return true;
	}

	if (m_inactivityTimer.isRunning() && m_inactivityTimer.hasExpired()) {
		wxLogMessage(wxT("D-Plus inactivity timer expired"));

		if (m_dPlusId != 0x00U && m_destination != NULL)
			m_destination->timeout(m_dPlusId);

		m_dPlusId = 0x00;
		m_inactivityTimer.stop();
	}

	return false;
}

void CXReflectorDPlusHandler::writeHeaderInt(CHeaderData& header)
{
	if (!m_linked)
		return;

	// If there's an incoming data stream, don't send
	if (m_dPlusId != 0x00U)
		return;

	wxString rpt2 = m_dplusCallsign;
	wxChar band = header.getRptCall1().GetChar(LONG_CALLSIGN_LENGTH - 1U);
	rpt2.SetChar(LONG_CALLSIGN_LENGTH - 1U, band);

	wxString rpt1 = m_dplusCallsign;
	band = header.getRptCall2().GetChar(LONG_CALLSIGN_LENGTH - 1U);
	rpt1.SetChar(LONG_CALLSIGN_LENGTH - 1U, band);

	header.setRepeaters(rpt1, rpt2);
	header.setDestination(m_address, m_port);
	m_handler->writeHeader(header);
}

void CXReflectorDPlusHandler::writeAMBEInt(CAMBEData& data)
{
	if (!m_linked)
		return;

	// If there's an incoming data stream, don't send
	if (m_dPlusId != 0x00U)
		return;

	data.setDestination(m_address, m_port);
	m_handler->writeAMBE(data);
}

bool CXReflectorDPlusHandler::stateChange()
{
	bool stateChange = m_stateChange;

	m_stateChange = false;

	return stateChange;
}

void CXReflectorDPlusHandler::writeStatus(wxFFile& file)
{
	for (unsigned int i = 0U; i < m_maxReflectors; i++) {
		CXReflectorDPlusHandler* reflector = m_reflectors[i];
		if (reflector != NULL) {
			struct tm* tm = ::gmtime(&reflector->m_time);

			wxString text;
			text.Printf(wxT("%04d-%02d-%02d %02d:%02d:%02d: DPlus link - Type: Dongle User: %s Dir: Inbound\n"),
				tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec, 
				reflector->m_reflector.c_str());

			file.Write(text);
		}
	}
}
