/*
 *   Copyright (C) 2011,2012 by Jonathan Naylor G4KLX
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

#ifndef	TimerControlThread_H
#define	TimerControlThread_H

#include "TimerControlRemoteControlHandler.h"
#include "TimerControlItem.h"
#include "Defs.h"

#include <wx/wx.h>

class CTimerControlThread {
public:
	CTimerControlThread();
	virtual ~CTimerControlThread();

	virtual void setGateway(const wxString& address, unsigned int port, const wxString& password);
	virtual void setDelay(bool enabled);
	virtual void setFileName(const wxString& fileName);

	virtual void reload();

	virtual void run();
	virtual void kill();

private:
	wxString                           m_password;
	wxString                           m_fileName;
	bool                               m_delay;
	unsigned int                       m_count;
	CTimerControlItem**                m_items;
	wxMutex                            m_mutex;
	CTimerControlRemoteControlHandler* m_handler;
	bool                               m_killed;

	bool login();
	bool link(const wxString& repeater, RECONNECT reconnect, const wxString& reflector);
	void logoff();

	bool sendHash(unsigned int rnd);
};

#endif
