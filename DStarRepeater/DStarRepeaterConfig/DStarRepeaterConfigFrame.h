/*
 *   Copyright (C) 2011-2014 by Jonathan Naylor G4KLX
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

#ifndef	DStarRepeaterConfigFrame_H
#define	DStarRepeaterConfigFrame_H

#include "DStarRepeaterConfigAnnouncementSet.h"
#include "DStarRepeaterConfigControllerSet.h"
#include "DStarRepeaterConfigCallsignSet.h"
#include "DStarRepeaterConfigControl1Set.h"
#include "DStarRepeaterConfigControl2Set.h"
#include "DStarRepeaterConfigNetworkSet.h"
#include "DStarRepeaterConfigBeaconSet.h"
#include "DStarRepeaterConfigModemSet.h"
#include "DStarRepeaterConfigTimesSet.h"
#include "DStarRepeaterConfigDefs.h"
#include "DStarRepeaterConfig.h"
#include "DStarDefines.h"

#include <wx/wx.h>

class CDStarRepeaterConfigFrame : public wxFrame {
public:
	CDStarRepeaterConfigFrame(const wxString& title, const wxString& confDir, const wxString& name);
	virtual ~CDStarRepeaterConfigFrame();

	virtual void onQuit(wxCommandEvent& event);
	virtual void onSave(wxCommandEvent& event);
	virtual void onAbout(wxCommandEvent& event);
	virtual void onClose(wxCloseEvent& event);

private:
	CDStarRepeaterConfig*                m_config;
	CDStarRepeaterConfigCallsignSet*     m_callsign;
	CDStarRepeaterConfigNetworkSet*      m_network;
	CDStarRepeaterConfigTimesSet*        m_times;
	CDStarRepeaterConfigBeaconSet*       m_beacon;
	CDStarRepeaterConfigAnnouncementSet* m_announcement;
	CDStarRepeaterConfigModemSet*        m_modem;
	CDStarRepeaterConfigControl1Set*     m_control1;
	CDStarRepeaterConfigControl2Set*     m_control2;
	CDStarRepeaterConfigControllerSet*   m_controller;

	DECLARE_EVENT_TABLE()

	wxMenuBar* createMenuBar();
};

#endif
