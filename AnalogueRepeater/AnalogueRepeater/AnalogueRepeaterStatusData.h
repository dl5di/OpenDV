/*
 *   Copyright (C) 2009,2010 by Jonathan Naylor G4KLX
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

#ifndef	AnalogueRepeaterStatusData_H
#define	AnalogueRepeaterStatusData_H

#include "AnalogueDefines.h"

#include <wx/wx.h>

class CAnalogueRepeaterStatusData {
public:
	CAnalogueRepeaterStatusData(bool radioTransmit, ANALOGUE_SQUELCH squelch, wxFloat32 level,
		ANALOGUE_RPT_STATE state, wxFloat32 radioVOGAD, wxFloat32 extVOGAD, wxString radioDTMF,
		wxString extDTMF, unsigned int timeoutTimer, unsigned int timeoutExpiry, unsigned int lockoutTimer,
		unsigned int lockoutExpiry, unsigned int hangTimer, unsigned int hangExpiry, unsigned int latchTimer,
		unsigned int latchExpiry, unsigned int ackTimer, unsigned int ackExpiry, unsigned int minimumTimer,
		unsigned int minimumExpiry, unsigned int callsignTimer, unsigned int callsignExpiry,
		unsigned int activeTimer, unsigned int activeExpity, bool extTransmit, bool battery);
	~CAnalogueRepeaterStatusData();

	bool isRadioTransmit() const;
	bool isExternalTransmit() const;
	bool isBattery() const;

	ANALOGUE_SQUELCH   getSquelch() const;
	wxFloat32          getLevel() const;
	ANALOGUE_RPT_STATE getState() const;

	wxFloat32 getRadioVOGAD() const;
	wxFloat32 getExternalVOGAD() const;

	wxString getRadioDTMF() const;
	wxString getExternalDTMF() const;

	unsigned int getTimeoutTimer() const;
	unsigned int getTimeoutExpiry() const;

	unsigned int getLockoutTimer() const;
	unsigned int getLockoutExpiry() const;

	unsigned int getHangTimer() const;
	unsigned int getHangExpiry() const;

	unsigned int getLatchTimer() const;
	unsigned int getLatchExpiry() const;

	unsigned int getAckTimer() const;
	unsigned int getAckExpiry() const;

	unsigned int getMinimumTimer() const;
	unsigned int getMinimumExpiry() const;

	unsigned int getCallsignTimer() const;
	unsigned int getCallsignExpiry() const;

	unsigned int getActiveTimer() const;
	unsigned int getActiveExpiry() const;

private:
	bool               m_radioTransmit;
	bool               m_extTransmit;
	bool               m_battery;
	ANALOGUE_SQUELCH   m_squelch;
	wxFloat32          m_level;
	ANALOGUE_RPT_STATE m_state;
	wxFloat32          m_radioVOGAD;
	wxFloat32          m_extVOGAD;
	wxString           m_radioDTMF;
	wxString           m_extDTMF;
	unsigned int       m_timeoutTimer;
	unsigned int       m_timeoutExpiry;
	unsigned int       m_lockoutTimer;
	unsigned int       m_lockoutExpiry;
	unsigned int       m_hangTimer;
	unsigned int       m_hangExpiry;
	unsigned int       m_latchTimer;
	unsigned int       m_latchExpiry;
	unsigned int       m_ackTimer;
	unsigned int       m_ackExpiry;
	unsigned int       m_minimumTimer;
	unsigned int       m_minimumExpiry;
	unsigned int       m_callsignTimer;
	unsigned int       m_callsignExpiry;
	unsigned int       m_activeTimer;
	unsigned int       m_activeExpiry;
};

#endif
