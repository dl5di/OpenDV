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

#include "AnalogueRepeaterStatusData.h"

CAnalogueRepeaterStatusData::CAnalogueRepeaterStatusData(bool radioTransmit, ANALOGUE_SQUELCH squelch, wxFloat32 level,
		ANALOGUE_RPT_STATE state, wxFloat32 radioVOGAD, wxFloat32 extVOGAD, wxString radioDTMF, wxString extDTMF,
		unsigned int timeoutTimer, unsigned int timeoutExpiry, unsigned int lockoutTimer,
		unsigned int lockoutExpiry, unsigned int hangTimer, unsigned int hangExpiry, unsigned int latchTimer,
		unsigned int latchExpiry, unsigned int ackTimer, unsigned int ackExpiry, unsigned int minimumTimer,
		unsigned int minimumExpiry, unsigned int callsignTimer, unsigned int callsignExpiry,
		unsigned int activeTimer, unsigned int activeExpiry, bool extTransmit, bool battery) :
m_radioTransmit(radioTransmit),
m_extTransmit(extTransmit),
m_battery(battery),
m_squelch(squelch),
m_level(level),
m_state(state),
m_radioVOGAD(radioVOGAD),
m_extVOGAD(extVOGAD),
m_radioDTMF(radioDTMF),
m_extDTMF(extDTMF),
m_timeoutTimer(timeoutTimer),
m_timeoutExpiry(timeoutExpiry),
m_lockoutTimer(lockoutTimer),
m_lockoutExpiry(lockoutExpiry),
m_hangTimer(hangTimer),
m_hangExpiry(hangExpiry),
m_latchTimer(latchTimer),
m_latchExpiry(latchExpiry),
m_ackTimer(ackTimer),
m_ackExpiry(ackExpiry),
m_minimumTimer(minimumTimer),
m_minimumExpiry(minimumExpiry),
m_callsignTimer(callsignTimer),
m_callsignExpiry(callsignExpiry),
m_activeTimer(activeTimer),
m_activeExpiry(activeExpiry)
{
}

CAnalogueRepeaterStatusData::~CAnalogueRepeaterStatusData()
{
}

bool CAnalogueRepeaterStatusData::isRadioTransmit() const
{
	return m_radioTransmit;
}

bool CAnalogueRepeaterStatusData::isExternalTransmit() const
{
	return m_extTransmit;
}

bool CAnalogueRepeaterStatusData::isBattery() const
{
	return m_battery;
}

ANALOGUE_SQUELCH CAnalogueRepeaterStatusData::getSquelch() const
{
	return m_squelch;
}

wxFloat32 CAnalogueRepeaterStatusData::getLevel() const
{
	return m_level;
}

ANALOGUE_RPT_STATE CAnalogueRepeaterStatusData::getState() const
{
	return m_state;
}

wxFloat32 CAnalogueRepeaterStatusData::getRadioVOGAD() const
{
	return m_radioVOGAD;
}

wxFloat32 CAnalogueRepeaterStatusData::getExternalVOGAD() const
{
	return m_extVOGAD;
}

wxString CAnalogueRepeaterStatusData::getRadioDTMF() const
{
	return m_radioDTMF;
}

wxString CAnalogueRepeaterStatusData::getExternalDTMF() const
{
	return m_extDTMF;
}

unsigned int CAnalogueRepeaterStatusData::getTimeoutTimer() const
{
	return m_timeoutTimer;
}

unsigned int CAnalogueRepeaterStatusData::getTimeoutExpiry() const
{
	return m_timeoutExpiry;
}

unsigned int CAnalogueRepeaterStatusData::getLockoutTimer() const
{
	return m_lockoutTimer;
}

unsigned int CAnalogueRepeaterStatusData::getLockoutExpiry() const
{
	return m_lockoutExpiry;
}

unsigned int CAnalogueRepeaterStatusData::getHangTimer() const
{
	return m_hangTimer;
}

unsigned int CAnalogueRepeaterStatusData::getHangExpiry() const
{
	return m_hangExpiry;
}

unsigned int CAnalogueRepeaterStatusData::getLatchTimer() const
{
	return m_latchTimer;
}

unsigned int CAnalogueRepeaterStatusData::getLatchExpiry() const
{
	return m_latchExpiry;
}

unsigned int CAnalogueRepeaterStatusData::getAckTimer() const
{
	return m_ackTimer;
}

unsigned int CAnalogueRepeaterStatusData::getAckExpiry() const
{
	return m_ackExpiry;
}

unsigned int CAnalogueRepeaterStatusData::getMinimumTimer() const
{
	return m_minimumTimer;
}

unsigned int CAnalogueRepeaterStatusData::getMinimumExpiry() const
{
	return m_minimumExpiry;
}

unsigned int CAnalogueRepeaterStatusData::getCallsignTimer() const
{
	return m_callsignTimer;
}

unsigned int CAnalogueRepeaterStatusData::getCallsignExpiry() const
{
	return m_callsignExpiry;
}

unsigned int CAnalogueRepeaterStatusData::getActiveTimer() const
{
	return m_activeTimer;
}

unsigned int CAnalogueRepeaterStatusData::getActiveExpiry() const
{
	return m_activeExpiry;
}
