/*
 *   Copyright (C) 2010-2013 by Jonathan Naylor G4KLX
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

#include "GMSKRepeaterStatusData.h"

CGMSKRepeaterStatusData::CGMSKRepeaterStatusData(const wxString& myCall1, const wxString& myCall2,
												 const wxString& yourCall, const wxString& rptCall1,
												 const wxString& rptCall2, unsigned char flag1,
												 unsigned char flag2, unsigned char flag3, bool tx,
												 DSTAR_RPT_STATE state, unsigned int timeoutTimer,
												 unsigned int timeoutExpiry, unsigned int beaconTimer,
												 unsigned int beaconExpiry, unsigned int announceTimer,
												 unsigned int announceExpiry, float percent,
												 const wxString& text, const wxString& status1,
												 const wxString& status2, const wxString& status3,
												 const wxString& status4, const wxString& status5) :
m_myCall1(myCall1),
m_myCall2(myCall2),
m_yourCall(yourCall),
m_rptCall1(rptCall1),
m_rptCall2(rptCall2),
m_flag1(flag1),
m_flag2(flag2),
m_flag3(flag3),
m_tx(tx),
m_state(state),
m_timeoutTimer(timeoutTimer),
m_timeoutExpiry(timeoutExpiry),
m_beaconTimer(beaconTimer),
m_beaconExpiry(beaconExpiry),
m_announceTimer(announceTimer),
m_announceExpiry(announceExpiry),
m_percent(percent),
m_text(text),
m_status1(status1),
m_status2(status2),
m_status3(status3),
m_status4(status4),
m_status5(status5)
{
}

CGMSKRepeaterStatusData::~CGMSKRepeaterStatusData()
{
}

wxString CGMSKRepeaterStatusData::getMyCall1() const
{
	return m_myCall1;
}

wxString CGMSKRepeaterStatusData::getMyCall2() const
{
	return m_myCall2;
}

wxString CGMSKRepeaterStatusData::getYourCall() const
{
	return m_yourCall;
}

wxString CGMSKRepeaterStatusData::getRptCall1() const
{
	return m_rptCall1;
}

wxString CGMSKRepeaterStatusData::getRptCall2() const
{
	return m_rptCall2;
}

unsigned char CGMSKRepeaterStatusData::getFlag1() const
{
	return m_flag1;
}

unsigned char CGMSKRepeaterStatusData::getFlag2() const
{
	return m_flag2;
}

unsigned char CGMSKRepeaterStatusData::getFlag3() const
{
	return m_flag3;
}

bool CGMSKRepeaterStatusData::getTX() const
{
	return m_tx;
}

DSTAR_RPT_STATE CGMSKRepeaterStatusData::getState() const
{
	return m_state;
}

unsigned int CGMSKRepeaterStatusData::getTimeoutTimer() const
{
	return m_timeoutTimer;
}

unsigned int CGMSKRepeaterStatusData::getTimeoutExpiry() const
{
	return m_timeoutExpiry;
}

unsigned int CGMSKRepeaterStatusData::getBeaconTimer() const
{
	return m_beaconTimer;
}

unsigned int CGMSKRepeaterStatusData::getBeaconExpiry() const
{
	return m_beaconExpiry;
}

unsigned int CGMSKRepeaterStatusData::getAnnounceTimer() const
{
	return m_announceTimer;
}

unsigned int CGMSKRepeaterStatusData::getAnnounceExpiry() const
{
	return m_announceExpiry;
}

float CGMSKRepeaterStatusData::getPercent() const
{
	return m_percent;
}

wxString CGMSKRepeaterStatusData::getText() const
{
	return m_text;
}

wxString CGMSKRepeaterStatusData::getStatus1() const
{
	return m_status1;
}

wxString CGMSKRepeaterStatusData::getStatus2() const
{
	return m_status2;
}

wxString CGMSKRepeaterStatusData::getStatus3() const
{
	return m_status3;
}

wxString CGMSKRepeaterStatusData::getStatus4() const
{
	return m_status4;
}

wxString CGMSKRepeaterStatusData::getStatus5() const
{
	return m_status5;
}
