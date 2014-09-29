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

#ifndef	ParrotControllerStatusData_H
#define	ParrotControllerStatusData_H

#include "ParrotControllerDefs.h"

#include <wx/wx.h>

class CParrotControllerStatusData {
public:
	CParrotControllerStatusData(PARROT_STATE state, const wxString& myCall1, const wxString myCall2,
		const wxString& yourCall, const wxString& rptCall1, const wxString& rptCall2, unsigned char flag1,
		unsigned char flag2, unsigned char flag3, unsigned int dataTimer, unsigned int dataTimeout,
		unsigned int turnaroundTimer, unsigned int turnaroundExpiry, unsigned int beaconTimer,
		unsigned int beaconExpiry);
	~CParrotControllerStatusData();

	PARROT_STATE  getState() const;

	wxString      getMyCall1() const;
	wxString      getMyCall2() const;
	wxString      getYourCall() const;
	wxString      getRptCall1() const;
	wxString      getRptCall2() const;
	unsigned char getFlag1() const;
	unsigned char getFlag2() const;
	unsigned char getFlag3() const;

	unsigned int  getDataTimer() const;
	unsigned int  getDataTimeout() const;

	unsigned int  getTurnaroundTimer() const;
	unsigned int  getTurnaroundExpiry() const;

	unsigned int  getBeaconTimer() const;
	unsigned int  getBeaconExpiry() const;

private:
	PARROT_STATE  m_state;
	wxString      m_myCall1;
	wxString      m_myCall2;
	wxString      m_yourCall;
	wxString      m_rptCall1;
	wxString      m_rptCall2;
	unsigned char m_flag1;
	unsigned char m_flag2;
	unsigned char m_flag3;
	unsigned int  m_dataTimer;
	unsigned int  m_dataTimeout;
	unsigned int  m_turnaroundTimer;
	unsigned int  m_turnaroundExpiry;
	unsigned int  m_beaconTimer;
	unsigned int  m_beaconExpiry;
};

#endif
