/*
 *   Copyright (C) 2010,2011,2013 by Jonathan Naylor G4KLX
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

#include "StarNetHandler.h"
#include "DStarDefines.h"
#include "G2Handler.h"
#include "Utils.h"
#include "Defs.h"

unsigned int        CG2Handler::m_maxRoutes = 0U;
CG2Handler**        CG2Handler::m_routes = NULL;

CG2ProtocolHandler* CG2Handler::m_handler = NULL;

CHeaderLogger*      CG2Handler::m_headerLogger = NULL;

CG2Handler::CG2Handler(CRepeaterHandler* repeater, const in_addr& address, unsigned int id) :
m_repeater(repeater),
m_address(address),
m_id(id),
m_inactivityTimer(1000U, 2U)
{
	m_inactivityTimer.start();
}

CG2Handler::~CG2Handler()
{
}

void CG2Handler::initialise(unsigned int maxRoutes)
{
	m_maxRoutes = maxRoutes;

	if (maxRoutes == 0U)
		return;

	m_routes = new CG2Handler*[m_maxRoutes];
	for (unsigned int i = 0U; i < m_maxRoutes; i++)
		m_routes[i] = NULL;
}

void CG2Handler::setG2ProtocolHandler(CG2ProtocolHandler* handler)
{
	wxASSERT(handler != NULL);

	m_handler = handler;
}

void CG2Handler::setHeaderLogger(CHeaderLogger* logger)
{
	m_headerLogger = logger;
}

void CG2Handler::process(CHeaderData& header)
{
	// Is this a busy reply?
	unsigned char flag1 = header.getFlag1();
	if (flag1 == 0x01) {
		// Don't check the incoming stream
		// wxLogMessage(wxT("G2 busy message received"));
		return;
	}

	// Check to see if this is for StarNet
	CStarNetHandler* handler = CStarNetHandler::findStarNet(header);
	if (handler != NULL) {
		// Write to Header.log if it's enabled
		if (m_headerLogger != NULL)
			m_headerLogger->write(wxT("StarNet"), header);

		handler->process(header);
		return;
	}

	// No need to go any further
	if (m_maxRoutes == 0U)
		return;

	in_addr address = header.getYourAddress();
	unsigned int id = header.getId();

	for (unsigned int i = 0U; i < m_maxRoutes; i++) {
		CG2Handler* route = m_routes[i];
		if (route != NULL) {
			// Is this a duplicate header, ignore it
			if (route->m_id == id)
				return;
		}
	}	

	// Find the destination repeater
	CRepeaterHandler* repeater = CRepeaterHandler::findDVRepeater(header.getRptCall2());
	if (repeater == NULL) {
		wxLogMessage(wxT("Incoming G2 header from %s to unknown repeater - %s"), header.getMyCall1().c_str(), header.getRptCall2().c_str());
		return;		// Not found, ignore
	}

	CG2Handler* route = new CG2Handler(repeater, address, id);

	for (unsigned int i = 0U; i < m_maxRoutes; i++) {
		if (m_routes[i] == NULL) {
			m_routes[i] = route;

			// Write to Header.log if it's enabled
			if (m_headerLogger != NULL)
				m_headerLogger->write(wxT("G2"), header);

			repeater->process(header, DIR_INCOMING, AS_G2);
			return;
		}
	}

	wxLogMessage(wxT("No space to add new G2 route, ignoring"));

	delete route;
}

void CG2Handler::process(CAMBEData& data)
{
	// Check to see if this is for StarNet
	CStarNetHandler* handler = CStarNetHandler::findStarNet(data);
	if (handler != NULL) {
		handler->process(data);
		return;
	}

	// No need to go any further
	if (m_maxRoutes == 0U)
		return;

	unsigned int id = data.getId();

	for (unsigned int i = 0U; i < m_maxRoutes; i++) {
		CG2Handler* route = m_routes[i];
		if (route != NULL) {
			if (route->m_id == id) {
				route->m_inactivityTimer.reset();
				route->m_repeater->process(data, DIR_INCOMING, AS_G2);

				if (data.isEnd()) {
					delete route;
					m_routes[i] = NULL;
				}

				return;
			}
		}
	}	
}

void CG2Handler::clock(unsigned int ms)
{
	for (unsigned int i = 0U; i < m_maxRoutes; i++) {
		CG2Handler* route = m_routes[i];
		if (route != NULL) {
			bool ret = route->clockInt(ms);
			if (ret) {
				delete route;
				m_routes[i] = NULL;
			}
		}
	}
}

void CG2Handler::finalise()
{
	for (unsigned int i = 0U; i < m_maxRoutes; i++)
		delete m_routes[i];

	delete[] m_routes;
}

bool CG2Handler::clockInt(unsigned int ms)
{
	m_inactivityTimer.clock(ms);

	if (m_inactivityTimer.isRunning() && m_inactivityTimer.hasExpired()) {
		wxLogMessage(wxT("Inactivity timeout for a G2 route has expired"));
		return true;
	}

	return false;
}
