/*
 *   Copyright (C) 2009 by Jonathan Naylor G4KLX
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

#include "ParrotControllerStatusData.h"

CParrotControllerStatusData::CParrotControllerStatusData(PARROT_STATE state, const wxString& myCall1,
														 const wxString myCall2, const wxString& yourCall,
														 const wxString& rptCall1, const wxString& rptCall2,
														 unsigned char flag1, unsigned char flag2,
														 unsigned char flag3, unsigned int dataTimer,
														 unsigned int dataTimeout, unsigned int turnaroundTimer,
														 unsigned int turnaroundExpiry, unsigned int beaconTimer,
														 unsigned int beaconExpiry) :
m_state(state),
m_myCall1(myCall1),
m_myCall2(myCall2),
m_yourCall(yourCall),
m_rptCall1(rptCall1),
m_rptCall2(rptCall2),
m_flag1(flag1),
m_flag2(flag2),
m_flag3(flag3),
m_dataTimer(dataTimer),
m_dataTimeout(dataTimeout),
m_turnaroundTimer(turnaroundTimer),
m_turnaroundExpiry(turnaroundExpiry),
m_beaconTimer(beaconTimer),
m_beaconExpiry(beaconExpiry)
{
}

CParrotControllerStatusData::~CParrotControllerStatusData()
{
}

PARROT_STATE CParrotControllerStatusData::getState() const
{
	return m_state;
}

wxString CParrotControllerStatusData::getMyCall1() const
{
	return m_myCall1;
}

wxString CParrotControllerStatusData::getMyCall2() const
{
	return m_myCall2;
}

wxString CParrotControllerStatusData::getYourCall() const
{
	return m_yourCall;
}

wxString CParrotControllerStatusData::getRptCall1() const
{
	return m_rptCall1;
}

wxString CParrotControllerStatusData::getRptCall2() const
{
	return m_rptCall2;
}

unsigned char CParrotControllerStatusData::getFlag1() const
{
	return m_flag1;
}

unsigned char CParrotControllerStatusData::getFlag2() const
{
	return m_flag2;
}

unsigned char CParrotControllerStatusData::getFlag3() const
{
	return m_flag3;
}

unsigned int CParrotControllerStatusData::getDataTimer() const
{
	return m_dataTimer;
}

unsigned int CParrotControllerStatusData::getDataTimeout() const
{
	return m_dataTimeout;
}

unsigned int CParrotControllerStatusData::getTurnaroundTimer() const
{
	return m_turnaroundTimer;
}

unsigned int CParrotControllerStatusData::getTurnaroundExpiry() const
{
	return m_turnaroundExpiry;
}

unsigned int CParrotControllerStatusData::getBeaconTimer() const
{
	return m_beaconTimer;
}

unsigned int CParrotControllerStatusData::getBeaconExpiry() const
{
	return m_beaconExpiry;
}
