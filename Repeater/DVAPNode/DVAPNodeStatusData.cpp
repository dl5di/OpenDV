/*
 *   Copyright (C) 2011,2013 by Jonathan Naylor G4KLX
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

#include "DVAPNodeStatusData.h"

CDVAPNodeStatusData::CDVAPNodeStatusData(const wxString& myCall1, const wxString& myCall2,
												 const wxString& yourCall, const wxString& rptCall1,
												 const wxString& rptCall2, unsigned char flag1,
												 unsigned char flag2, unsigned char flag3, bool tx, bool squelch,
												 int signal, DSTAR_RPT_STATE state, unsigned int timeoutTimer,
												 unsigned int timeoutExpiry, unsigned int beaconTimer,
												 unsigned int beaconExpiry, unsigned int announceTimer,
												 unsigned int announceExpiry, float percent, const wxString& text) :
m_myCall1(myCall1),
m_myCall2(myCall2),
m_yourCall(yourCall),
m_rptCall1(rptCall1),
m_rptCall2(rptCall2),
m_flag1(flag1),
m_flag2(flag2),
m_flag3(flag3),
m_tx(tx),
m_squelch(squelch),
m_signal(signal),
m_state(state),
m_timeoutTimer(timeoutTimer),
m_timeoutExpiry(timeoutExpiry),
m_beaconTimer(beaconTimer),
m_beaconExpiry(beaconExpiry),
m_announceTimer(announceTimer),
m_announceExpiry(announceExpiry),
m_percent(percent),
m_text(text)
{
}

CDVAPNodeStatusData::~CDVAPNodeStatusData()
{
}

wxString CDVAPNodeStatusData::getMyCall1() const
{
	return m_myCall1;
}

wxString CDVAPNodeStatusData::getMyCall2() const
{
	return m_myCall2;
}

wxString CDVAPNodeStatusData::getYourCall() const
{
	return m_yourCall;
}

wxString CDVAPNodeStatusData::getRptCall1() const
{
	return m_rptCall1;
}

wxString CDVAPNodeStatusData::getRptCall2() const
{
	return m_rptCall2;
}

unsigned char CDVAPNodeStatusData::getFlag1() const
{
	return m_flag1;
}

unsigned char CDVAPNodeStatusData::getFlag2() const
{
	return m_flag2;
}

unsigned char CDVAPNodeStatusData::getFlag3() const
{
	return m_flag3;
}

bool CDVAPNodeStatusData::getTX() const
{
	return m_tx;
}

bool CDVAPNodeStatusData::getSquelch() const
{
	return m_squelch;
}

int CDVAPNodeStatusData::getSignal() const
{
	return m_signal;
}

DSTAR_RPT_STATE CDVAPNodeStatusData::getState() const
{
	return m_state;
}

unsigned int CDVAPNodeStatusData::getTimeoutTimer() const
{
	return m_timeoutTimer;
}

unsigned int CDVAPNodeStatusData::getTimeoutExpiry() const
{
	return m_timeoutExpiry;
}

unsigned int CDVAPNodeStatusData::getBeaconTimer() const
{
	return m_beaconTimer;
}

unsigned int CDVAPNodeStatusData::getBeaconExpiry() const
{
	return m_beaconExpiry;
}

unsigned int CDVAPNodeStatusData::getAnnounceTimer() const
{
	return m_announceTimer;
}

unsigned int CDVAPNodeStatusData::getAnnounceExpiry() const
{
	return m_announceExpiry;
}

float CDVAPNodeStatusData::getPercent() const
{
	return m_percent;
}

wxString CDVAPNodeStatusData::getText() const
{
	return m_text;
}
