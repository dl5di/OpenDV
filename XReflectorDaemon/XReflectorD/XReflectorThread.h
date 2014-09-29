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

#ifndef	XReflectorThread_H
#define	XReflectorThread_H

#include "DExtraProtocolHandler.h"
#include "DPlusProtocolHandler.h"
#include "Timer.h"
#include "Defs.h"

#include <wx/wx.h>

class CXReflectorThread {
public:
	CXReflectorThread(const wxString& name, const wxString& lopgDir);
	virtual ~CXReflectorThread();

	virtual void setReflector(const wxString& callsign, const wxString& address);
	virtual void setDPlus(bool enabled);
	virtual void setMiscellaneous(bool logEnabled);

	virtual void run();
	virtual void kill();

private:
	wxString                  m_name;
	wxString                  m_logDir;
	bool                      m_killed;
	bool                      m_stopped;
	wxString                  m_callsign;
	wxString                  m_address;
	CDExtraProtocolHandler*   m_dextraHandler;
	CDPlusProtocolHandler*    m_dplusHandler;
	bool                      m_dplusEnabled;
	unsigned int              m_dplusPort;
	bool                      m_logEnabled;
	CTimer                    m_statusTimer;

	void processDExtra();
	void processDPlus();

	void writeStatus();
	void writeUserLog();
};

#endif
