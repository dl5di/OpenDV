/*
 *   Copyright (C) 2011 by Jonathan Naylor G4KLX
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

#include "RemoteReflectorData.h"

#include <wx/arrimpl.cpp>
WX_DEFINE_OBJARRAY(CLinkData_t);

CRemoteReflectorData::CRemoteReflectorData(const wxString& callsign, RECONNECT reconnect, const wxString& reflector) :
m_callsign(callsign),
m_reconnect(reconnect),
m_reflector(reflector),
m_links()
{
}

CRemoteReflectorData::~CRemoteReflectorData()
{
	m_links.Clear();
}

void CRemoteReflectorData::addLink(const wxString& callsign, PROTOCOL protocol, bool linked, DIRECTION direction, bool dongle)
{
	CRemoteLinkData data(callsign, protocol, linked, direction, dongle);

	m_links.Add(data);
}

wxString CRemoteReflectorData::getCallsign() const
{
	return m_callsign;
}

wxInt32 CRemoteReflectorData::getReconnect() const
{
	return wxInt32(m_reconnect);
}

wxString CRemoteReflectorData::getReflector() const
{
	return m_reflector;
}

unsigned int CRemoteReflectorData::getLinkCount() const
{
	return m_links.GetCount();
}

CRemoteLinkData& CRemoteReflectorData::getLink(unsigned int n) const
{
	return m_links.Item(n);
}
