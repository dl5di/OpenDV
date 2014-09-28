/*
 *   Copyright (C) 2012 by Jonathan Naylor G4KLX
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

#ifndef	VersionUnit_H
#define	VersionUnit_H

#include "RepeaterCallback.h"
#include "AMBEData.h"
#include "Timer.h"
#include "Defs.h"

#include <wx/wx.h>

enum VERSION_STATUS {
	VS_IDLE,
	VS_WAIT,
	VS_TRANSMIT
};

class CVersionUnit {
public:
	CVersionUnit(IRepeaterCallback* handler, const wxString& callsign);
	~CVersionUnit();

	void sendVersion();

	void cancel();

	void clock(unsigned int ms);

private:
	IRepeaterCallback* m_handler;
	wxString           m_callsign;
	VERSION_STATUS     m_status;
	CTimer             m_timer;
	CAMBEData**        m_data;
	unsigned int       m_id;
	unsigned int       m_out;
	wxStopWatch        m_time;
};

#endif
