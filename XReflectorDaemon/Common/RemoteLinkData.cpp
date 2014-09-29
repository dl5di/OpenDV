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

#include "RemoteLinkData.h"

CRemoteLinkData::CRemoteLinkData(const wxString& callsign, PROTOCOL protocol, bool linked, DIRECTION direction, bool dongle) :
m_callsign(callsign),
m_protocol(protocol),
m_linked(linked),
m_direction(direction),
m_dongle(dongle)
{
}

CRemoteLinkData::~CRemoteLinkData()
{
}

wxString CRemoteLinkData::getCallsign() const
{
	return m_callsign;
}

wxInt32 CRemoteLinkData::getProtocol() const
{
	return wxInt32(m_protocol);
}

wxInt32 CRemoteLinkData::isLinked() const
{
	return m_linked ? 1 : 0;
}

wxInt32 CRemoteLinkData::getDirection() const
{
	return wxInt32(m_direction);
}

wxInt32 CRemoteLinkData::isDongle() const
{
	return m_dongle ? 1 : 0;
}
