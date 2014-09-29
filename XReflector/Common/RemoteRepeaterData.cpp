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

#include "RemoteRepeaterData.h"

#include <wx/arrimpl.cpp>
WX_DEFINE_OBJARRAY(CLinkData_t);

CRemoteRepeaterData::CRemoteRepeaterData(const wxString& callsign, RECONNECT reconnect, const wxString& reflector) :
m_callsign(callsign),
m_reconnect(reconnect),
m_reflector(reflector),
m_links()
{
}

CRemoteRepeaterData::~CRemoteRepeaterData()
{
	m_links.Clear();
}

void CRemoteRepeaterData::addLink(const wxString& callsign, PROTOCOL protocol, bool linked, DIRECTION direction, bool dongle)
{
	CRemoteLinkData data(callsign, protocol, linked, direction, dongle);

	m_links.Add(data);
}

wxString CRemoteRepeaterData::getCallsign() const
{
	return m_callsign;
}

wxInt32 CRemoteRepeaterData::getReconnect() const
{
	return wxInt32(m_reconnect);
}

wxString CRemoteRepeaterData::getReflector() const
{
	return m_reflector;
}

unsigned int CRemoteRepeaterData::getLinkCount() const
{
	return m_links.GetCount();
}

CRemoteLinkData& CRemoteRepeaterData::getLink(unsigned int n) const
{
	return m_links.Item(n);
}
