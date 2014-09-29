/*
 *   Copyright (C) 2010 by Jonathan Naylor G4KLX
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

#include "DCSGatewayStatusData.h"

CDCSGatewayStatusData::CDCSGatewayStatusData(const wxString& myCall, const wxString& yourCall,
												   const wxString& rptCall1, const wxString& rptCall2,
												   LINK_STATE state, const wxString& reflector,
												   unsigned int reconnectTimer, unsigned int reconnectExpiry) :
m_myCall(myCall),
m_yourCall(yourCall),
m_rptCall1(rptCall1),
m_rptCall2(rptCall2),
m_state(state),
m_reflector(reflector),
m_reconnectTimer(reconnectTimer),
m_reconnectExpiry(reconnectExpiry)
{
}

CDCSGatewayStatusData::~CDCSGatewayStatusData()
{
}

wxString CDCSGatewayStatusData::getMyCall() const
{
	return m_myCall;
}

wxString CDCSGatewayStatusData::getYourCall() const
{
	return m_yourCall;
}

wxString CDCSGatewayStatusData::getRptCall1() const
{
	return m_rptCall1;
}

wxString CDCSGatewayStatusData::getRptCall2() const
{
	return m_rptCall2;
}

LINK_STATE CDCSGatewayStatusData::getState() const
{
	return m_state;
}

wxString CDCSGatewayStatusData::getReflector() const
{
	return m_reflector;
}

unsigned int CDCSGatewayStatusData::getReconnectTimer() const
{
	return m_reconnectTimer;
}

unsigned int CDCSGatewayStatusData::getReconnectExpiry() const
{
	return m_reconnectExpiry;
}
