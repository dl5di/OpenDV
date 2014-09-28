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

#include "IRCDDBGatewayStatusData.h"

CIRCDDBGatewayStatusData::CIRCDDBGatewayStatusData(IRCDDB_STATUS ircDDBStatus, bool dprsStatus) :
m_ircDDBStatus(ircDDBStatus),
m_dprsStatus(dprsStatus)
{
}

CIRCDDBGatewayStatusData::~CIRCDDBGatewayStatusData()
{
}

void CIRCDDBGatewayStatusData::setRepeater(unsigned int n, const wxString& callsign, LINK_STATUS linkStatus, const wxString& linkCallsign, const wxString& incoming)
{
	wxASSERT(n < 4U);

	m_callsign[n]     = callsign;
	m_linkStatus[n]   = linkStatus;
	m_linkCallsign[n] = linkCallsign;
	m_incoming[n]     = incoming;
}

void CIRCDDBGatewayStatusData::setDongles(const wxString& dongles)
{
	m_dongles = dongles;
}

IRCDDB_STATUS CIRCDDBGatewayStatusData::getIrcDDBStatus() const
{
	return m_ircDDBStatus;
}

bool CIRCDDBGatewayStatusData::getDPRSStatus() const
{
	return m_dprsStatus;
}

wxString CIRCDDBGatewayStatusData::getCallsign(unsigned int n) const
{
	wxASSERT(n < 4U);

	return m_callsign[n];
}

LINK_STATUS CIRCDDBGatewayStatusData::getLinkStatus(unsigned int n) const
{
	wxASSERT(n < 4U);

	return m_linkStatus[n];
}

wxString CIRCDDBGatewayStatusData::getLinkCallsign(unsigned int n) const
{
	wxASSERT(n < 4U);

	return m_linkCallsign[n];
}

wxString CIRCDDBGatewayStatusData::getIncoming(unsigned int n) const
{
	wxASSERT(n < 4U);

	return m_incoming[n];
}

wxString CIRCDDBGatewayStatusData::getDongles() const
{
	return m_dongles;
}
