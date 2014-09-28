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

#include "RemoteControlCallsignData.h"

CRemoteControlCallsignData::CRemoteControlCallsignData() :
m_repeaters(),
m_starNets()
{
}

CRemoteControlCallsignData::~CRemoteControlCallsignData()
{
}

void CRemoteControlCallsignData::addRepeater(const wxString& callsign)
{
	m_repeaters.Add(callsign);
}

void CRemoteControlCallsignData::addStarNet(const wxString& callsign)
{
	m_starNets.Add(callsign);
}

unsigned int CRemoteControlCallsignData::getRepeaterCount() const
{
	return m_repeaters.GetCount();
}

wxString CRemoteControlCallsignData::getRepeater(unsigned int n) const
{
	return m_repeaters.Item(n);
}

unsigned int CRemoteControlCallsignData::getStarNetCount() const
{
	return m_starNets.GetCount();
}

wxString CRemoteControlCallsignData::getStarNet(unsigned int n) const
{
	return m_starNets.Item(n);
}
