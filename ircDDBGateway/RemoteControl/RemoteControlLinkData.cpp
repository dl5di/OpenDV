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

#include "RemoteControlLinkData.h"

CRemoteControlLinkData::CRemoteControlLinkData(const wxString& callsign, wxInt32 protocol, wxInt32 linked, wxInt32 direction, wxInt32 dongle) :
m_callsign(callsign),
m_protocol(PROTOCOL(protocol)),
m_linked(false),
m_direction(DIRECTION(direction)),
m_dongle(false)
{
	m_linked = linked == 1;
	m_dongle = dongle == 1;
}

CRemoteControlLinkData::~CRemoteControlLinkData()
{
}

wxString CRemoteControlLinkData::getCallsign() const
{
	return m_callsign;
}

PROTOCOL CRemoteControlLinkData::getProtocol() const
{
	return m_protocol;
}

bool CRemoteControlLinkData::isLinked() const
{
	return m_linked;
}

DIRECTION CRemoteControlLinkData::getDirection() const
{
	return m_direction;
}

bool CRemoteControlLinkData::isDongle() const
{
	return m_dongle;
}
