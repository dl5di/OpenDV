/*
 *   Copyright (C) 2013 by Jonathan Naylor G4KLX
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
#include "DStarDefines.h"
#include "CCSHandler.h"
#include "Utils.h"

const unsigned int MAX_CCS_INCOMING = 5U;
const unsigned int MAX_CCS_OUTGOING = 5U;

CCS_STATUS               CCCSHandler::m_state = CS_DISABLED;

wxString                 CCCSHandler::m_callsign = wxEmptyString;

CCCSProtocolHandler*     CCCSHandler::m_handler = NULL;

in_addr                  CCCSHandler::m_address;

CHeaderLogger*           CCCSHandler::m_headerLogger = NULL;

CTimer                   CCCSHandler::m_pollInactivityTimer(1000U, 60U);
CTimer                   CCCSHandler::m_pollTimer(1000U, 10U);
CTimer                   CCCSHandler::m_tryTimer(1000U, 1U);
unsigned int             CCCSHandler::m_tryCount = 0U;

wxString                 CCCSHandler::m_locator = wxEmptyString;

CCCSAudioIncoming**      CCCSHandler::m_incoming = NULL;
CCCSAudioOutgoing**      CCCSHandler::m_outgoing = NULL;

void CCCSHandler::initialise()
{
	m_incoming = new CCCSAudioIncoming*[MAX_CCS_INCOMING];
	m_outgoing = new CCCSAudioOutgoing*[MAX_CCS_OUTGOING];

	for (unsigned int i = 0U; i < MAX_CCS_INCOMING; i++)
		m_incoming[i] = NULL;

	for (unsigned int i = 0U; i < MAX_CCS_OUTGOING; i++)
		m_outgoing[i] = NULL;
}

void CCCSHandler::setCCSProtocolHandler(CCCSProtocolHandler* handler)
{
	wxASSERT(handler != NULL);

	m_handler = handler;
}

void CCCSHandler::setHeaderLogger(CHeaderLogger* logger)
{
	m_headerLogger = logger;
}

void CCCSHandler::setCallsign(const wxString& callsign)
{
	m_callsign = callsign;
}

void CCCSHandler::setLocation(double latitude, double longitude)
{
	if (latitude != 0.0 && longitude != 0.0)
		m_locator = CUtils::latLonToLoc(latitude, longitude);
}

void CCCSHandler::addRepeater(IRepeaterCallback* handler)
{
	wxASSERT(handler != NULL);

	for (unsigned int i = 0U; i < MAX_CCS_OUTGOING; i++) {
		if (m_outgoing[i] == NULL) {
			m_outgoing[i] = new CCCSAudioOutgoing(handler);
			return;
		}
	}
}

void CCCSHandler::process(CAMBEData& data)
{
	CHeaderData& header = data.getHeader();
	wxString yourCall = header.getYourCall();
	unsigned int seqNo = data.getSeq();
	unsigned int id = data.getId();

	for (unsigned int i = 0U; i < MAX_CCS_INCOMING; i++) {
		if (m_incoming[i] != NULL) {
			if (m_incoming[i]->m_id == id) {
				m_incoming[i]->m_timer.reset();

				if (seqNo == 0U && !m_incoming[i]->m_busy) {
					if (yourCall.Left(1U).IsSameAs(wxT("*")))
						header.setCQCQCQ();
					m_incoming[i]->m_handler->process(header, DIR_INCOMING, AS_DUP);
				}

				if (!m_incoming[i]->m_busy)
					m_incoming[i]->m_handler->process(data, DIR_INCOMING, AS_CCS);

				if (data.isEnd()) {
					delete m_incoming[i];
					m_incoming[i] = NULL;
				}

				return;
			}
		}
	}

	// A new incoming CCS transmission
	for (unsigned int i = 0U; i < MAX_CCS_INCOMING; i++) {
		if (m_incoming[i] == NULL) {
			IRepeaterCallback* handler = CRepeaterHandler::findDVRepeater(header.getRptCall1());
			if (handler == NULL)
				return;

			// Write to Header.log if it's enabled
			if (m_headerLogger != NULL)
				m_headerLogger->write(wxT("CCS"), header);

			if (yourCall.Left(1U).IsSameAs(wxT("*")))
				header.setCQCQCQ();

			bool busy = !handler->process(header, DIR_INCOMING, AS_CCS);

			if (busy)
				m_handler->writeBusy(wxT("We're busy!"), m_address, CCS_PORT);
			else
				handler->process(data, DIR_INCOMING, AS_CCS);

			m_incoming[i] = new CCCSAudioIncoming(id, handler, busy);

			return;
		}
	}

	wxLogWarning(wxT("No space to add new incoming CCS audio"));
}

void CCCSHandler::process(CPollData& poll)
{
	m_pollInactivityTimer.reset();
}

void CCCSHandler::process(CConnectData& connect)
{
	CD_TYPE type = connect.getType();

	if (type == CT_ACK && m_state == CS_CONNECTING) {
		wxLogMessage(wxT("Connected to CCS"));
		m_pollInactivityTimer.start();
		m_pollTimer.start();
		m_tryTimer.stop();
		m_state = CS_CONNECTED;
		return;
	}

	if (type == CT_NAK && m_state == CS_CONNECTING) {
		wxLogMessage(wxT("Connection refused by CCS"));
		m_tryTimer.stop();
		m_state = CS_DISABLED;
		return;
	}
}

bool CCCSHandler::connect()
{
	m_address = CUDPReaderWriter::lookup(CCS_HOSTNAME);

	CConnectData connect(m_callsign, CT_LINK1, m_address, CCS_PORT);

	if (!m_locator.IsEmpty())
		connect.setLocator(m_locator);

	m_handler->writeConnect(connect);

	wxLogMessage(wxT("Connecting to CCS on host %s"), CCS_HOSTNAME.c_str());

	m_tryTimer.setTimeout(1U);
	m_tryTimer.start();

	m_tryCount = 1U;

	m_state = CS_CONNECTING;

	return true;
}

void CCCSHandler::disconnect()
{
	CConnectData connect(m_callsign, CT_UNLINK, m_address, CCS_PORT);
	m_handler->writeConnect(connect);

	m_pollInactivityTimer.stop();
	m_pollTimer.stop();

	m_state = CS_DISABLED;

	wxLogMessage(wxT("Disconnecting from CCS"));
}

void CCCSHandler::writeEnd(const wxString& local, const wxString& remote)
{
	if (m_state != CS_CONNECTED)
		return;

	m_handler->writeEnd(local, remote, m_address, CCS_PORT);
}

void CCCSHandler::writeHeard(const CHeaderData& data, const wxString& repeater, const wxString& reflector)
{
	if (m_state != CS_CONNECTED)
		return;

	CHeardData heard(data, repeater, reflector);
	heard.setDestination(m_address, CCS_PORT);

	m_handler->writeHeard(heard);
}

void CCCSHandler::writeHeader(IRepeaterCallback* handler, CHeaderData& header)
{
	if (m_state != CS_CONNECTED)
		return;

	for (unsigned int i = 0U; i < MAX_CCS_OUTGOING; i++) {
		if (m_outgoing[i] != NULL) {
			if (m_outgoing[i]->m_handler == handler) {
				m_outgoing[i]->m_rptCall1 = header.getRptCall1();
				m_outgoing[i]->m_rptCall2 = header.getRptCall2();
				m_outgoing[i]->m_yourCall = header.getYourCall();
				m_outgoing[i]->m_myCall1  = header.getMyCall1();
				m_outgoing[i]->m_myCall2  = header.getMyCall2();
				m_outgoing[i]->m_seqNo = 0U;
				return;
			}
		}
	}
}

void CCCSHandler::writeAMBE(IRepeaterCallback* handler, CAMBEData& data)
{
	if (m_state != CS_CONNECTED)
		return;

	for (unsigned int i = 0U; i < MAX_CCS_OUTGOING; i++) {
		if (m_outgoing[i] != NULL) {
			if (m_outgoing[i]->m_handler == handler) {
				CHeaderData& header = data.getHeader();
				header.setMyCall1(m_outgoing[i]->m_myCall1);
				header.setMyCall2(m_outgoing[i]->m_myCall2);
				header.setYourCall(m_outgoing[i]->m_yourCall);
				header.setRptCall1(m_outgoing[i]->m_rptCall1);
				header.setRptCall2(m_outgoing[i]->m_rptCall2);

				data.setRptSeq(m_outgoing[i]->m_seqNo++);
				data.setDestination(m_address, CCS_PORT);
				m_handler->writeData(data);
				return;
			}
		}
	}
}

void CCCSHandler::clock(unsigned int ms)
{
	m_pollInactivityTimer.clock(ms);
	m_pollTimer.clock(ms);
	m_tryTimer.clock(ms);

	if (m_pollInactivityTimer.isRunning() && m_pollInactivityTimer.hasExpired()) {
		wxLogMessage(wxT("CCS connection has failed (poll inactivity), reconnecting"));

		m_pollInactivityTimer.stop();
		m_pollTimer.stop();

		m_tryTimer.setTimeout(1U);
		m_tryTimer.start();

		m_tryCount = 1U;

		m_state = CS_CONNECTING;
		return;
	}

	if (m_tryTimer.isRunning() && m_tryTimer.hasExpired()) {
		CConnectData connect(m_callsign, CT_LINK1, m_address, CCS_PORT);
		m_handler->writeConnect(connect);

		unsigned int t = calcBackoff();
		m_tryTimer.setTimeout(t);
		m_tryTimer.reset();
	}

	if (m_pollTimer.isRunning() && m_pollTimer.hasExpired()) {
		CPollData poll(m_callsign, m_address, CCS_PORT);
		m_handler->writePoll(poll);

		m_pollTimer.reset();
	}

	// Handle inactivity on incoming audio
	for (unsigned int i = 0U; i < MAX_CCS_INCOMING; i++) {
		if (m_incoming[i] != NULL) {
			m_incoming[i]->m_timer.clock(ms);
			if (m_incoming[i]->m_timer.isRunning() && m_incoming[i]->m_timer.hasExpired()) {
				delete m_incoming[i];
				m_incoming[i] = NULL;
			}
		}
	}
}

void CCCSHandler::finalise()
{
	for (unsigned int i = 0U; i < MAX_CCS_INCOMING; i++)
		delete m_incoming[i];

	for (unsigned int i = 0U; i < MAX_CCS_OUTGOING; i++)
		delete m_outgoing[i];

	delete[] m_incoming;
	delete[] m_outgoing;
}

CCS_STATUS CCCSHandler::getState()
{
	return m_state;
}

unsigned int CCCSHandler::calcBackoff()
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
