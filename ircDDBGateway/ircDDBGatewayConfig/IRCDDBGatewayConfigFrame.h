/*
 *   Copyright (C) 2010-2014 by Jonathan Naylor G4KLX
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

#ifndef	IRCDDBGatewayConfigFrame_H
#define	IRCDDBGatewayConfigFrame_H

#include "IRCDDBGatewayConfigMiscellaneousSet.h"
#include "IRCDDBGatewayConfigGatewaySet.h"
#include "IRCDDBGatewayConfigIrcDDBSet.h"
#include "IRCDDBGatewayConfig.h"
#include "RepeaterInfoSet.h"
#include "RepeaterDataSet.h"
#include "StarNetSet.h"
#include "RemoteSet.h"
#include "DExtraSet.h"
#include "DPlusSet.h"
#include "DPRSSet.h"
#include "DCSSet.h"
#include "Defs.h"

#include <wx/wx.h>

class CIRCDDBGatewayConfigFrame : public wxFrame {
public:
	CIRCDDBGatewayConfigFrame(const wxString& title, const wxString& confDir, const wxString& name);
	virtual ~CIRCDDBGatewayConfigFrame();

	virtual void onQuit(wxCommandEvent& event);
	virtual void onSave(wxCommandEvent& event);
	virtual void onAbout(wxCommandEvent& event);
	virtual void onClose(wxCloseEvent& event);

private:
	CIRCDDBGatewayConfig*                 m_config;
	CIRCDDBGatewayConfigGatewaySet*       m_gateway;
	CRepeaterDataSet*                     m_repeaterData1;
	CRepeaterInfoSet*                     m_repeaterInfo1;
	CRepeaterDataSet*                     m_repeaterData2;
	CRepeaterInfoSet*                     m_repeaterInfo2;
	CRepeaterDataSet*                     m_repeaterData3;
	CRepeaterInfoSet*                     m_repeaterInfo3;
	CRepeaterDataSet*                     m_repeaterData4;
	CRepeaterInfoSet*                     m_repeaterInfo4;
	CIRCDDBGatewayConfigIrcDDBSet*        m_ircDDB;
	CDPRSSet*                             m_dprs;
	CDExtraSet*                           m_dextra;
	CDPlusSet*                            m_dplus;
	CDCSSet*                              m_dcs;
	CStarNetSet*                          m_starNet1;
	CStarNetSet*                          m_starNet2;
	CStarNetSet*                          m_starNet3;
	CStarNetSet*                          m_starNet4;
	CStarNetSet*                          m_starNet5;
	CRemoteSet*                           m_remote;
	CIRCDDBGatewayConfigMiscellaneousSet* m_miscellaneous;

	DECLARE_EVENT_TABLE()

	wxMenuBar* createMenuBar();
};

#endif
