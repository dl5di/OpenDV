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

#include "RemoteStarNetUser.h"

CRemoteStarNetUser::CRemoteStarNetUser(const wxString& callsign, unsigned int timer, unsigned int timeout) :
m_callsign(callsign),
m_timer(timer),
m_timeout(timeout)
{
}

CRemoteStarNetUser::~CRemoteStarNetUser()
{
}

wxString CRemoteStarNetUser::getCallsign() const
{
	return m_callsign;
}

wxUint32 CRemoteStarNetUser::getTimer() const
{
	return wxUint32(m_timer);
}

wxUint32 CRemoteStarNetUser::getTimeout() const
{
	return wxUint32(m_timeout);
}
