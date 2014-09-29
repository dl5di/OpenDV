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

#include "RemoteStarNetGroup.h"

#include <wx/arrimpl.cpp>
WX_DEFINE_OBJARRAY(CUserData_t);

CRemoteStarNetGroup::CRemoteStarNetGroup(const wxString& callsign, const wxString& logoff, unsigned int timer, unsigned int timeout) :
m_callsign(callsign),
m_logoff(logoff),
m_timer(timer),
m_timeout(timeout),
m_users()
{
	if (m_logoff.IsSameAs(wxT("        ")))
		m_logoff.Clear();
}

CRemoteStarNetGroup::~CRemoteStarNetGroup()
{
	m_users.Clear();
}

void CRemoteStarNetGroup::addUser(const wxString& callsign, unsigned int timer, unsigned int timeout)
{
	CRemoteStarNetUser user(callsign, timer, timeout);

	m_users.Add(user);
}

wxString CRemoteStarNetGroup::getCallsign() const
{
	return m_callsign;
}

wxString CRemoteStarNetGroup::getLogoff() const
{
	return m_logoff;
}

wxUint32 CRemoteStarNetGroup::getTimer() const
{
	return wxUint32(m_timer);
}

wxUint32 CRemoteStarNetGroup::getTimeout() const
{
	return wxUint32(m_timeout);
}

unsigned int CRemoteStarNetGroup::getUserCount() const
{
	return m_users.GetCount();
}

CRemoteStarNetUser& CRemoteStarNetGroup::getUser(unsigned int n) const
{
	return m_users.Item(n);
}
