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

#include "RemoteControlRepeaterData.h"

#include <wx/arrimpl.cpp>
WX_DEFINE_OBJARRAY(CRemoteLinkData_t);

CRemoteControlRepeaterData::CRemoteControlRepeaterData(const wxString& callsign, wxInt32 reconnect, const wxString& reflector) :
m_callsign(callsign),
m_reconnect(RECONNECT(reconnect)),
m_reflector(reflector),
m_links()
{
}

CRemoteControlRepeaterData::~CRemoteControlRepeaterData()
{
	m_links.Clear();
}

void CRemoteControlRepeaterData::addLink(const wxString& callsign, wxInt32 protocol, wxInt32 linked, wxInt32 direction, wxInt32 dongle)
{
	CRemoteControlLinkData data(callsign, protocol, linked, direction, dongle);

	m_links.Add(data);
}

wxString CRemoteControlRepeaterData::getCallsign() const
{
	return m_callsign;
}

RECONNECT CRemoteControlRepeaterData::getReconnect() const
{
	return m_reconnect;
}

wxString CRemoteControlRepeaterData::getReflector() const
{
	return m_reflector;
}

unsigned int CRemoteControlRepeaterData::getLinkCount() const
{
	return m_links.GetCount();
}

CRemoteControlLinkData& CRemoteControlRepeaterData::getLink(unsigned int n) const
{
	return m_links.Item(n);
}
