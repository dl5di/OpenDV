/*
 *   Copyright (C) 2011,2012,2013 by Jonathan Naylor G4KLX
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

#include "DStarRepeaterStatusData.h"

CDStarRepeaterStatusData::CDStarRepeaterStatusData(const wxString& myCall1, const wxString& myCall2,
													 const wxString& yourCall, const wxString& rptCall1,
													 const wxString& rptCall2, unsigned char flag1,
													 unsigned char flag2, unsigned char flag3, bool tx,
													 DSTAR_RX_STATE rxState, DSTAR_RPT_STATE rptState,
													 unsigned int timeoutTimer, unsigned int timeoutExpiry,
													 unsigned int beaconTimer, unsigned int beaconExpiry,
													 unsigned int announceTimer, unsigned int announceExpiry,
													 float percent, const wxString& text, const wxString& status1,
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
m_rxState(rxState),
m_rptState(rptState),
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
m_status5(status5),
m_squelch(false),
m_signal(0)
{
}

CDStarRepeaterStatusData::~CDStarRepeaterStatusData()
{
}

void CDStarRepeaterStatusData::setDVAP(bool squelch, int signal)
{
	m_squelch = squelch;
	m_signal  = signal;
}

wxString CDStarRepeaterStatusData::getMyCall1() const
{
	return m_myCall1;
}

wxString CDStarRepeaterStatusData::getMyCall2() const
{
	return m_myCall2;
}

wxString CDStarRepeaterStatusData::getYourCall() const
{
	return m_yourCall;
}

wxString CDStarRepeaterStatusData::getRptCall1() const
{
	return m_rptCall1;
}

wxString CDStarRepeaterStatusData::getRptCall2() const
{
	return m_rptCall2;
}

unsigned char CDStarRepeaterStatusData::getFlag1() const
{
	return m_flag1;
}

unsigned char CDStarRepeaterStatusData::getFlag2() const
{
	return m_flag2;
}

unsigned char CDStarRepeaterStatusData::getFlag3() const
{
	return m_flag3;
}

bool CDStarRepeaterStatusData::getTX() const
{
	return m_tx;
}

bool CDStarRepeaterStatusData::getSquelch() const
{
	return m_squelch;
}

int CDStarRepeaterStatusData::getSignal() const
{
	return m_signal;
}

DSTAR_RX_STATE CDStarRepeaterStatusData::getRxState() const
{
	return m_rxState;
}

DSTAR_RPT_STATE CDStarRepeaterStatusData::getRptState() const
{
	return m_rptState;
}

unsigned int CDStarRepeaterStatusData::getTimeoutTimer() const
{
	return m_timeoutTimer;
}

unsigned int CDStarRepeaterStatusData::getTimeoutExpiry() const
{
	return m_timeoutExpiry;
}

unsigned int CDStarRepeaterStatusData::getBeaconTimer() const
{
	return m_beaconTimer;
}

unsigned int CDStarRepeaterStatusData::getBeaconExpiry() const
{
	return m_beaconExpiry;
}

unsigned int CDStarRepeaterStatusData::getAnnounceTimer() const
{
	return m_announceTimer;
}

unsigned int CDStarRepeaterStatusData::getAnnounceExpiry() const
{
	return m_announceExpiry;
}

float CDStarRepeaterStatusData::getPercent() const
{
	return m_percent;
}

wxString CDStarRepeaterStatusData::getText() const
{
	return m_text;
}

wxString CDStarRepeaterStatusData::getStatus1() const
{
	return m_status1;
}

wxString CDStarRepeaterStatusData::getStatus2() const
{
	return m_status2;
}

wxString CDStarRepeaterStatusData::getStatus3() const
{
	return m_status3;
}

wxString CDStarRepeaterStatusData::getStatus4() const
{
	return m_status4;
}

wxString CDStarRepeaterStatusData::getStatus5() const
{
	return m_status5;
}
