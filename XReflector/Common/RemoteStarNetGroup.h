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

#ifndef	RemoteStarNetGroup_H
#define	RemoteStarNetGroup_H

#include "RemoteStarNetUser.h"

#include <wx/wx.h>

#include <wx/dynarray.h>
WX_DECLARE_OBJARRAY(CRemoteStarNetUser, CUserData_t);

class CRemoteStarNetGroup {
public:
	CRemoteStarNetGroup(const wxString& callsign, const wxString& logoff, unsigned int timer, unsigned int timeout);
	~CRemoteStarNetGroup();

	void addUser(const wxString& callsign, unsigned int timer, unsigned int timeout);

	wxString getCallsign() const;
	wxString getLogoff() const;
	wxUint32 getTimer() const;
	wxUint32 getTimeout() const;

	unsigned int getUserCount() const;
	CRemoteStarNetUser& getUser(unsigned int n) const;

private:
	wxString     m_callsign;
	wxString     m_logoff;
	unsigned int m_timer;
	unsigned int m_timeout;
	CUserData_t  m_users;
};

#endif
