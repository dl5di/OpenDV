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

#include "StarNetServerPreferences.h"
#include "StarNetServerFrame.h"
#include "StarNetServerDefs.h"
#include "StarNetServerApp.h"
#include "LogEvent.h"
#include "Version.h"

const unsigned int BORDER_SIZE = 5U;

#if defined(__WINDOWS__)
const unsigned int LOGTEXT_WIDTH  = 560U;
#else
const unsigned int LOGTEXT_WIDTH  = 700U;
#endif

#include <wx/gbsizer.h>
#include <wx/aboutdlg.h>

DEFINE_EVENT_TYPE(LOG_EVENT)

enum {
	Menu_Edit_Preferences = 6000,
	Menu_View_Updates
};

BEGIN_EVENT_TABLE(CStarNetServerFrame, wxFrame)
	EVT_MENU(wxID_EXIT, CStarNetServerFrame::onQuit)
	EVT_MENU(Menu_Edit_Preferences, CStarNetServerFrame::onPreferences)
	EVT_MENU(Menu_View_Updates, CStarNetServerFrame::onUpdates)
	EVT_MENU(wxID_ABOUT, CStarNetServerFrame::onAbout)

	EVT_CLOSE(CStarNetServerFrame::onClose)

	EVT_CUSTOM(LOG_EVENT, wxID_ANY, CStarNetServerFrame::onLog)
END_EVENT_TABLE()

CStarNetServerFrame::CStarNetServerFrame(const wxString& title, const wxPoint& position, bool gui) :
wxFrame(NULL, -1, title, position),
#if defined(__WXDEBUG__)
m_updates(true)
#else
m_updates(gui)
#endif
{
	SetMenuBar(createMenuBar());

	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

	wxPanel* panel = new wxPanel(this);

	wxBoxSizer* panelSizer = new wxBoxSizer(wxVERTICAL);

	wxStaticBoxSizer* log1Sizer = new wxStaticBoxSizer(new wxStaticBox(panel, -1, _("Log")), wxVERTICAL);
	wxBoxSizer* log2Sizer = new wxBoxSizer(wxVERTICAL);

	for (unsigned int i = 0U; i < 20U; i++) {
		m_logLine[i] = new wxStaticText(panel, -1, wxEmptyString, wxDefaultPosition, wxSize(LOGTEXT_WIDTH, -1));
		m_logLine[i]->Wrap(LOGTEXT_WIDTH);
		log2Sizer->Add(m_logLine[i], 0, wxTOP | wxLEFT | wxRIGHT, BORDER_SIZE);
	}

	log1Sizer->Add(log2Sizer);
	panelSizer->Add(log1Sizer, 0, wxALL, BORDER_SIZE);

	panel->SetSizer(panelSizer);
	panelSizer->SetSizeHints(panel);

	mainSizer->Add(panel);

	SetSizer(mainSizer);
	mainSizer->SetSizeHints(this);
}

CStarNetServerFrame::~CStarNetServerFrame()
{
}

wxMenuBar* CStarNetServerFrame::createMenuBar()
{
	wxMenu* fileMenu = new wxMenu();
	fileMenu->Append(wxID_EXIT, _("Exit"));

	wxMenu* editMenu = new wxMenu();
	editMenu->Append(Menu_Edit_Preferences, _("Preferences..."));

	wxMenu* viewMenu = new wxMenu();
	viewMenu->AppendCheckItem(Menu_View_Updates, _("GUI Updates"));
	viewMenu->Check(Menu_View_Updates, m_updates);

	wxMenu* helpMenu = new wxMenu();
	helpMenu->Append(wxID_ABOUT, _("About StarNet Server"));

	wxMenuBar* menuBar = new wxMenuBar();
	menuBar->Append(fileMenu, _("File"));
	menuBar->Append(editMenu, _("Edit"));
	menuBar->Append(viewMenu, _("View"));
	menuBar->Append(helpMenu, _("Help"));

	return menuBar;
}

void CStarNetServerFrame::onQuit(wxCommandEvent&)
{
	Close(false);
}

void CStarNetServerFrame::onClose(wxCloseEvent&)
{
	int x, y;
	GetPosition(&x, &y);
	if (x >= 0 && y >= 0) {
		::wxGetApp().setPosition(x, y);
		::wxGetApp().writeConfig();
	}

	Destroy();
}

void CStarNetServerFrame::onPreferences(wxCommandEvent&)
{
	wxString callsign, address;
	::wxGetApp().getGateway(callsign, address);

	wxString hostname, username, password;
	::wxGetApp().getIrcDDB(hostname, username, password);

	unsigned int remotePort;
	wxString remotePassword;
	bool remoteEnabled;
	::wxGetApp().getRemote(remoteEnabled, remotePassword, remotePort);

	bool logEnabled;
	::wxGetApp().getMiscellaneous(logEnabled);

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
	wxString starNetBand1, starNetCallsign1, starNetLogoff1, starNetInfo1, starNetLink1, starNetPermanent1;
	unsigned int starNetUserTimeout1, starNetGroupTimeout1;
	STARNET_CALLSIGN_SWITCH starNetCallsignSwitch1;
	bool starNetTXMsgSwitch1;
	::wxGetApp().getStarNet1(starNetBand1, starNetCallsign1, starNetLogoff1, starNetInfo1, starNetPermanent1, starNetUserTimeout1, starNetGroupTimeout1, starNetCallsignSwitch1, starNetTXMsgSwitch1, starNetLink1);

	wxString starNetBand2, starNetCallsign2, starNetLogoff2, starNetInfo2, starNetLink2, starNetPermanent2;
	unsigned int starNetUserTimeout2, starNetGroupTimeout2;
	STARNET_CALLSIGN_SWITCH starNetCallsignSwitch2;
	bool starNetTXMsgSwitch2;
	::wxGetApp().getStarNet2(starNetBand2, starNetCallsign2, starNetLogoff2, starNetInfo2, starNetPermanent2, starNetUserTimeout2, starNetGroupTimeout2, starNetCallsignSwitch2, starNetTXMsgSwitch2, starNetLink2);

	wxString starNetBand3, starNetCallsign3, starNetLogoff3, starNetInfo3, starNetLink3, starNetPermanent3;
	unsigned int starNetUserTimeout3, starNetGroupTimeout3;
	STARNET_CALLSIGN_SWITCH starNetCallsignSwitch3;
	bool starNetTXMsgSwitch3;
	::wxGetApp().getStarNet3(starNetBand3, starNetCallsign3, starNetLogoff3, starNetInfo3, starNetPermanent3, starNetUserTimeout3, starNetGroupTimeout3, starNetCallsignSwitch3, starNetTXMsgSwitch3, starNetLink3);

	wxString starNetBand4, starNetCallsign4, starNetLogoff4, starNetInfo4, starNetLink4, starNetPermanent4;
	unsigned int starNetUserTimeout4, starNetGroupTimeout4;
	STARNET_CALLSIGN_SWITCH starNetCallsignSwitch4;
	bool starNetTXMsgSwitch4;
	::wxGetApp().getStarNet4(starNetBand4, starNetCallsign4, starNetLogoff4, starNetInfo4, starNetPermanent4, starNetUserTimeout4, starNetGroupTimeout4, starNetCallsignSwitch4, starNetTXMsgSwitch4, starNetLink4);

	wxString starNetBand5, starNetCallsign5, starNetLogoff5, starNetInfo5, starNetLink5, starNetPermanent5;
	unsigned int starNetUserTimeout5, starNetGroupTimeout5;
	STARNET_CALLSIGN_SWITCH starNetCallsignSwitch5;
	bool starNetTXMsgSwitch5;
	::wxGetApp().getStarNet5(starNetBand5, starNetCallsign5, starNetLogoff5, starNetInfo5, starNetPermanent5, starNetUserTimeout5, starNetGroupTimeout5, starNetCallsignSwitch5, starNetTXMsgSwitch5, starNetLink5);

	wxString starNetBand6, starNetCallsign6, starNetLogoff6, starNetInfo6, starNetLink6, starNetPermanent6;
	unsigned int starNetUserTimeout6, starNetGroupTimeout6;
	STARNET_CALLSIGN_SWITCH starNetCallsignSwitch6;
	bool starNetTXMsgSwitch6;
	::wxGetApp().getStarNet6(starNetBand6, starNetCallsign6, starNetLogoff6, starNetInfo6, starNetPermanent6, starNetUserTimeout6, starNetGroupTimeout6, starNetCallsignSwitch6, starNetTXMsgSwitch6, starNetLink6);

	wxString starNetBand7, starNetCallsign7, starNetLogoff7, starNetInfo7, starNetLink7, starNetPermanent7;
	unsigned int starNetUserTimeout7, starNetGroupTimeout7;
	STARNET_CALLSIGN_SWITCH starNetCallsignSwitch7;
	bool starNetTXMsgSwitch7;
	::wxGetApp().getStarNet7(starNetBand7, starNetCallsign7, starNetLogoff7, starNetInfo7, starNetPermanent7, starNetUserTimeout7, starNetGroupTimeout7, starNetCallsignSwitch7, starNetTXMsgSwitch7, starNetLink7);

	wxString starNetBand8, starNetCallsign8, starNetLogoff8, starNetInfo8, starNetLink8, starNetPermanent8;
	unsigned int starNetUserTimeout8, starNetGroupTimeout8;
	STARNET_CALLSIGN_SWITCH starNetCallsignSwitch8;
	bool starNetTXMsgSwitch8;
	::wxGetApp().getStarNet8(starNetBand8, starNetCallsign8, starNetLogoff8, starNetInfo8, starNetPermanent8, starNetUserTimeout8, starNetGroupTimeout8, starNetCallsignSwitch8, starNetTXMsgSwitch8, starNetLink8);

	wxString starNetBand9, starNetCallsign9, starNetLogoff9, starNetInfo9, starNetLink9, starNetPermanent9;
	unsigned int starNetUserTimeout9, starNetGroupTimeout9;
	STARNET_CALLSIGN_SWITCH starNetCallsignSwitch9;
	bool starNetTXMsgSwitch9;
	::wxGetApp().getStarNet9(starNetBand9, starNetCallsign9, starNetLogoff9, starNetInfo9, starNetPermanent9, starNetUserTimeout9, starNetGroupTimeout9, starNetCallsignSwitch9, starNetTXMsgSwitch9, starNetLink9);

	wxString starNetBand10, starNetCallsign10, starNetInfo10, starNetLogoff10, starNetLink10, starNetPermanent10;
	unsigned int starNetUserTimeout10, starNetGroupTimeout10;
	STARNET_CALLSIGN_SWITCH starNetCallsignSwitch10;
	bool starNetTXMsgSwitch10;
	::wxGetApp().getStarNet10(starNetBand10, starNetCallsign10, starNetLogoff10, starNetInfo10, starNetPermanent10, starNetUserTimeout10, starNetGroupTimeout10, starNetCallsignSwitch10, starNetTXMsgSwitch10, starNetLink10);

	wxString starNetBand11, starNetCallsign11, starNetLogoff11, starNetInfo11, starNetLink11, starNetPermanent11;
	unsigned int starNetUserTimeout11, starNetGroupTimeout11;
	STARNET_CALLSIGN_SWITCH starNetCallsignSwitch11;
	bool starNetTXMsgSwitch11;
	::wxGetApp().getStarNet11(starNetBand11, starNetCallsign11, starNetLogoff11, starNetInfo11, starNetPermanent11, starNetUserTimeout11, starNetGroupTimeout11, starNetCallsignSwitch11, starNetTXMsgSwitch11, starNetLink11);

	wxString starNetBand12, starNetCallsign12, starNetLogoff12, starNetInfo12, starNetLink12, starNetPermanent12;
	unsigned int starNetUserTimeout12, starNetGroupTimeout12;
	STARNET_CALLSIGN_SWITCH starNetCallsignSwitch12;
	bool starNetTXMsgSwitch12;
	::wxGetApp().getStarNet12(starNetBand12, starNetCallsign12, starNetLogoff12, starNetInfo12, starNetPermanent12, starNetUserTimeout12, starNetGroupTimeout12, starNetCallsignSwitch12, starNetTXMsgSwitch12, starNetLink12);

	wxString starNetBand13, starNetCallsign13, starNetLogoff13, starNetInfo13, starNetLink13, starNetPermanent13;
	unsigned int starNetUserTimeout13, starNetGroupTimeout13;
	STARNET_CALLSIGN_SWITCH starNetCallsignSwitch13;
	bool starNetTXMsgSwitch13;
	::wxGetApp().getStarNet13(starNetBand13, starNetCallsign13, starNetLogoff13, starNetInfo13, starNetPermanent13, starNetUserTimeout13, starNetGroupTimeout13, starNetCallsignSwitch13, starNetTXMsgSwitch13, starNetLink13);

	wxString starNetBand14, starNetCallsign14, starNetLogoff14, starNetInfo14, starNetLink14, starNetPermanent14;
	unsigned int starNetUserTimeout14, starNetGroupTimeout14;
	STARNET_CALLSIGN_SWITCH starNetCallsignSwitch14;
	bool starNetTXMsgSwitch14;
	::wxGetApp().getStarNet14(starNetBand14, starNetCallsign14, starNetLogoff14, starNetInfo14, starNetPermanent14, starNetUserTimeout14, starNetGroupTimeout14, starNetCallsignSwitch14, starNetTXMsgSwitch14, starNetLink14);

	wxString starNetBand15, starNetCallsign15, starNetLogoff15, starNetInfo15, starNetLink15, starNetPermanent15;
	unsigned int starNetUserTimeout15, starNetGroupTimeout15;
	STARNET_CALLSIGN_SWITCH starNetCallsignSwitch15;
	bool starNetTXMsgSwitch15;
	::wxGetApp().getStarNet15(starNetBand15, starNetCallsign15, starNetLogoff15, starNetInfo15, starNetPermanent15, starNetUserTimeout15, starNetGroupTimeout15, starNetCallsignSwitch15, starNetTXMsgSwitch15, starNetLink15);

	CStarNetServerPreferences dialog1(this, -1, callsign, address,
		hostname, username, password, logEnabled,
		starNetBand1,  starNetCallsign1,  starNetLogoff1,  starNetInfo1,  starNetPermanent1,  starNetUserTimeout1,  starNetGroupTimeout1,  starNetCallsignSwitch1,  starNetTXMsgSwitch1,  starNetLink1,
		starNetBand2,  starNetCallsign2,  starNetLogoff2,  starNetInfo2,  starNetPermanent2,  starNetUserTimeout2,  starNetGroupTimeout2,  starNetCallsignSwitch2,  starNetTXMsgSwitch2,  starNetLink2,
		starNetBand3,  starNetCallsign3,  starNetLogoff3,  starNetInfo3,  starNetPermanent3,  starNetUserTimeout3,  starNetGroupTimeout3,  starNetCallsignSwitch3,  starNetTXMsgSwitch3,  starNetLink3,
		starNetBand4,  starNetCallsign4,  starNetLogoff4,  starNetInfo4,  starNetPermanent4,  starNetUserTimeout4,  starNetGroupTimeout4,  starNetCallsignSwitch4,  starNetTXMsgSwitch4,  starNetLink4,
		starNetBand5,  starNetCallsign5,  starNetLogoff5,  starNetInfo5,  starNetPermanent5,  starNetUserTimeout5,  starNetGroupTimeout5,  starNetCallsignSwitch5,  starNetTXMsgSwitch5,  starNetLink5,
		starNetBand6,  starNetCallsign6,  starNetLogoff6,  starNetInfo6,  starNetPermanent6,  starNetUserTimeout6,  starNetGroupTimeout6,  starNetCallsignSwitch6,  starNetTXMsgSwitch6,  starNetLink6,
		starNetBand7,  starNetCallsign7,  starNetLogoff7,  starNetInfo7,  starNetPermanent7,  starNetUserTimeout7,  starNetGroupTimeout7,  starNetCallsignSwitch7,  starNetTXMsgSwitch7,  starNetLink7,
		starNetBand8,  starNetCallsign8,  starNetLogoff8,  starNetInfo8,  starNetPermanent8,  starNetUserTimeout8,  starNetGroupTimeout8,  starNetCallsignSwitch8,  starNetTXMsgSwitch8,  starNetLink8,
		starNetBand9,  starNetCallsign9,  starNetLogoff9,  starNetInfo9,  starNetPermanent9,  starNetUserTimeout9,  starNetGroupTimeout9,  starNetCallsignSwitch9,  starNetTXMsgSwitch9,  starNetLink9,
		starNetBand10, starNetCallsign10, starNetLogoff10, starNetInfo10, starNetPermanent10, starNetUserTimeout10, starNetGroupTimeout10, starNetCallsignSwitch10, starNetTXMsgSwitch10, starNetLink10,
		starNetBand11, starNetCallsign11, starNetLogoff11, starNetInfo11, starNetPermanent11, starNetUserTimeout11, starNetGroupTimeout11, starNetCallsignSwitch11, starNetTXMsgSwitch11, starNetLink11,
		starNetBand12, starNetCallsign12, starNetLogoff12, starNetInfo12, starNetPermanent12, starNetUserTimeout12, starNetGroupTimeout12, starNetCallsignSwitch12, starNetTXMsgSwitch12, starNetLink12,
		starNetBand13, starNetCallsign13, starNetLogoff13, starNetInfo13, starNetPermanent13, starNetUserTimeout13, starNetGroupTimeout13, starNetCallsignSwitch13, starNetTXMsgSwitch13, starNetLink13,
		starNetBand14, starNetCallsign14, starNetLogoff14, starNetInfo14, starNetPermanent14, starNetUserTimeout14, starNetGroupTimeout14, starNetCallsignSwitch14, starNetTXMsgSwitch14, starNetLink14,
		starNetBand15, starNetCallsign15, starNetLogoff15, starNetInfo15, starNetPermanent15, starNetUserTimeout15, starNetGroupTimeout15, starNetCallsignSwitch15, starNetTXMsgSwitch15, starNetLink15,
		remoteEnabled, remotePassword, remotePort);
#else
	wxString starNetBand1, starNetCallsign1, starNetLogoff1, starNetInfo1, starNetPermanent1;
	unsigned int starNetUserTimeout1, starNetGroupTimeout1;
	STARNET_CALLSIGN_SWITCH starNetCallsignSwitch1;
	bool starNetTXMsgSwitch1;
	::wxGetApp().getStarNet1(starNetBand1, starNetCallsign1, starNetLogoff1, starNetInfo1, starNetPermanent1, starNetUserTimeout1, starNetGroupTimeout1, starNetCallsignSwitch1, starNetTXMsgSwitch1);

	wxString starNetBand2, starNetCallsign2, starNetLogoff2, starNetInfo2, starNetPermanent2;
	unsigned int starNetUserTimeout2, starNetGroupTimeout2;
	STARNET_CALLSIGN_SWITCH starNetCallsignSwitch2;
	bool starNetTXMsgSwitch2;
	::wxGetApp().getStarNet2(starNetBand2, starNetCallsign2, starNetLogoff2, starNetInfo2, starNetPermanent2, starNetUserTimeout2, starNetGroupTimeout2, starNetCallsignSwitch2, starNetTXMsgSwitch2);

	wxString starNetBand3, starNetCallsign3, starNetLogoff3, starNetInfo3, starNetPermanent3;
	unsigned int starNetUserTimeout3, starNetGroupTimeout3;
	STARNET_CALLSIGN_SWITCH starNetCallsignSwitch3;
	bool starNetTXMsgSwitch3;
	::wxGetApp().getStarNet3(starNetBand3, starNetCallsign3, starNetLogoff3, starNetInfo3, starNetPermanent3, starNetUserTimeout3, starNetGroupTimeout3, starNetCallsignSwitch3, starNetTXMsgSwitch3);

	wxString starNetBand4, starNetCallsign4, starNetLogoff4, starNetInfo4, starNetPermanent4;
	unsigned int starNetUserTimeout4, starNetGroupTimeout4;
	STARNET_CALLSIGN_SWITCH starNetCallsignSwitch4;
	bool starNetTXMsgSwitch4;
	::wxGetApp().getStarNet4(starNetBand4, starNetCallsign4, starNetLogoff4, starNetInfo4, starNetPermanent4, starNetUserTimeout4, starNetGroupTimeout4, starNetCallsignSwitch4, starNetTXMsgSwitch4);

	wxString starNetBand5, starNetCallsign5, starNetLogoff5, starNetInfo5, starNetPermanent5;
	unsigned int starNetUserTimeout5, starNetGroupTimeout5;
	STARNET_CALLSIGN_SWITCH starNetCallsignSwitch5;
	bool starNetTXMsgSwitch5;
	::wxGetApp().getStarNet5(starNetBand5, starNetCallsign5, starNetLogoff5, starNetInfo5, starNetPermanent5, starNetUserTimeout5, starNetGroupTimeout5, starNetCallsignSwitch5, starNetTXMsgSwitch5);

	wxString starNetBand6, starNetCallsign6, starNetLogoff6, starNetInfo6, starNetPermanent6;
	unsigned int starNetUserTimeout6, starNetGroupTimeout6;
	STARNET_CALLSIGN_SWITCH starNetCallsignSwitch6;
	bool starNetTXMsgSwitch6;
	::wxGetApp().getStarNet6(starNetBand6, starNetCallsign6, starNetLogoff6, starNetInfo6, starNetPermanent6, starNetUserTimeout6, starNetGroupTimeout6, starNetCallsignSwitch6, starNetTXMsgSwitch6);

	wxString starNetBand7, starNetCallsign7, starNetLogoff7, starNetInfo7, starNetPermanent7;
	unsigned int starNetUserTimeout7, starNetGroupTimeout7;
	STARNET_CALLSIGN_SWITCH starNetCallsignSwitch7;
	bool starNetTXMsgSwitch7;
	::wxGetApp().getStarNet7(starNetBand7, starNetCallsign7, starNetLogoff7, starNetInfo7, starNetPermanent7, starNetUserTimeout7, starNetGroupTimeout7, starNetCallsignSwitch7, starNetTXMsgSwitch7);

	wxString starNetBand8, starNetCallsign8, starNetLogoff8, starNetInfo8, starNetPermanent8;
	unsigned int starNetUserTimeout8, starNetGroupTimeout8;
	STARNET_CALLSIGN_SWITCH starNetCallsignSwitch8;
	bool starNetTXMsgSwitch8;
	::wxGetApp().getStarNet8(starNetBand8, starNetCallsign8, starNetLogoff8, starNetInfo8, starNetPermanent8, starNetUserTimeout8, starNetGroupTimeout8, starNetCallsignSwitch8, starNetTXMsgSwitch8);

	wxString starNetBand9, starNetCallsign9, starNetLogoff9, starNetInfo9, starNetPermanent9;
	unsigned int starNetUserTimeout9, starNetGroupTimeout9;
	STARNET_CALLSIGN_SWITCH starNetCallsignSwitch9;
	bool starNetTXMsgSwitch9;
	::wxGetApp().getStarNet9(starNetBand9, starNetCallsign9, starNetLogoff9, starNetInfo9, starNetPermanent9, starNetUserTimeout9, starNetGroupTimeout9, starNetCallsignSwitch9, starNetTXMsgSwitch9);

	wxString starNetBand10, starNetCallsign10, starNetLogoff10, starNetInfo10, starNetPermanent10;
	unsigned int starNetUserTimeout10, starNetGroupTimeout10;
	STARNET_CALLSIGN_SWITCH starNetCallsignSwitch10;
	bool starNetTXMsgSwitch10;
	::wxGetApp().getStarNet10(starNetBand10, starNetCallsign10, starNetLogoff10, starNetInfo10, starNetPermanent10, starNetUserTimeout10, starNetGroupTimeout10, starNetCallsignSwitch10, starNetTXMsgSwitch10);

	wxString starNetBand11, starNetCallsign11, starNetLogoff11, starNetInfo11, starNetPermanent11;
	unsigned int starNetUserTimeout11, starNetGroupTimeout11;
	STARNET_CALLSIGN_SWITCH starNetCallsignSwitch11;
	bool starNetTXMsgSwitch11;
	::wxGetApp().getStarNet11(starNetBand11, starNetCallsign11, starNetLogoff11, starNetInfo11, starNetPermanent11, starNetUserTimeout11, starNetGroupTimeout11, starNetCallsignSwitch11, starNetTXMsgSwitch11);

	wxString starNetBand12, starNetCallsign12, starNetLogoff12, starNetInfo12, starNetPermanent12;
	unsigned int starNetUserTimeout12, starNetGroupTimeout12;
	STARNET_CALLSIGN_SWITCH starNetCallsignSwitch12;
	bool starNetTXMsgSwitch12;
	::wxGetApp().getStarNet12(starNetBand12, starNetCallsign12, starNetLogoff12, starNetInfo12, starNetPermanent12, starNetUserTimeout12, starNetGroupTimeout12, starNetCallsignSwitch12, starNetTXMsgSwitch12);

	wxString starNetBand13, starNetCallsign13, starNetLogoff13, starNetInfo13, starNetPermanent13;
	unsigned int starNetUserTimeout13, starNetGroupTimeout13;
	STARNET_CALLSIGN_SWITCH starNetCallsignSwitch13;
	bool starNetTXMsgSwitch13;
	::wxGetApp().getStarNet13(starNetBand13, starNetCallsign13, starNetLogoff13, starNetInfo13, starNetPermanent13, starNetUserTimeout13, starNetGroupTimeout13, starNetCallsignSwitch13, starNetTXMsgSwitch13);

	wxString starNetBand14, starNetCallsign14, starNetLogoff14, starNetInfo14, starNetPermanent14;
	unsigned int starNetUserTimeout14, starNetGroupTimeout14;
	STARNET_CALLSIGN_SWITCH starNetCallsignSwitch14;
	bool starNetTXMsgSwitch14;
	::wxGetApp().getStarNet14(starNetBand14, starNetCallsign14, starNetLogoff14, starNetInfo14, starNetPermanent14, starNetUserTimeout14, starNetGroupTimeout14, starNetCallsignSwitch14, starNetTXMsgSwitch14);

	wxString starNetBand15, starNetCallsign15, starNetLogoff15, starNetInfo15, starNetPermanent15;
	unsigned int starNetUserTimeout15, starNetGroupTimeout15;
	STARNET_CALLSIGN_SWITCH starNetCallsignSwitch15;
	bool starNetTXMsgSwitch15;
	::wxGetApp().getStarNet15(starNetBand15, starNetCallsign15, starNetLogoff15, starNetInfo15, starNetPermanent15, starNetUserTimeout15, starNetGroupTimeout15, starNetCallsignSwitch15, starNetTXMsgSwitch15);

	CStarNetServerPreferences dialog1(this, -1, callsign, address,
		hostname, username, password, logEnabled,
		starNetBand1,  starNetCallsign1,  starNetLogoff1,  starNetInfo1,  starNetPermanent1,  starNetUserTimeout1,  starNetGroupTimeout1,  starNetCallsignSwitch1,  starNetTXMsgSwitch1,
		starNetBand2,  starNetCallsign2,  starNetLogoff2,  starNetInfo2,  starNetPermanent2,  starNetUserTimeout2,  starNetGroupTimeout2,  starNetCallsignSwitch2,  starNetTXMsgSwitch2,
		starNetBand3,  starNetCallsign3,  starNetLogoff3,  starNetInfo3,  starNetPermanent3,  starNetUserTimeout3,  starNetGroupTimeout3,  starNetCallsignSwitch3,  starNetTXMsgSwitch3,
		starNetBand4,  starNetCallsign4,  starNetLogoff4,  starNetInfo4,  starNetPermanent4,  starNetUserTimeout4,  starNetGroupTimeout4,  starNetCallsignSwitch4,  starNetTXMsgSwitch4,
		starNetBand5,  starNetCallsign5,  starNetLogoff5,  starNetInfo5,  starNetPermanent5,  starNetUserTimeout5,  starNetGroupTimeout5,  starNetCallsignSwitch5,  starNetTXMsgSwitch5,
		starNetBand6,  starNetCallsign6,  starNetLogoff6,  starNetInfo6,  starNetPermanent6,  starNetUserTimeout6,  starNetGroupTimeout6,  starNetCallsignSwitch6,  starNetTXMsgSwitch6,
		starNetBand7,  starNetCallsign7,  starNetLogoff7,  starNetInfo7,  starNetPermanent7,  starNetUserTimeout7,  starNetGroupTimeout7,  starNetCallsignSwitch7,  starNetTXMsgSwitch7,
		starNetBand8,  starNetCallsign8,  starNetLogoff8,  starNetInfo8,  starNetPermanent8,  starNetUserTimeout8,  starNetGroupTimeout8,  starNetCallsignSwitch8,  starNetTXMsgSwitch8,
		starNetBand9,  starNetCallsign9,  starNetLogoff9,  starNetInfo9,  starNetPermanent9,  starNetUserTimeout9,  starNetGroupTimeout9,  starNetCallsignSwitch9,  starNetTXMsgSwitch9,
		starNetBand10, starNetCallsign10, starNetLogoff10, starNetInfo10, starNetPermanent10, starNetUserTimeout10, starNetGroupTimeout10, starNetCallsignSwitch10, starNetTXMsgSwitch10,
		starNetBand11, starNetCallsign11, starNetLogoff11, starNetInfo11, starNetPermanent11, starNetUserTimeout11, starNetGroupTimeout11, starNetCallsignSwitch11, starNetTXMsgSwitch11,
		starNetBand12, starNetCallsign12, starNetLogoff12, starNetInfo12, starNetPermanent12, starNetUserTimeout12, starNetGroupTimeout12, starNetCallsignSwitch12, starNetTXMsgSwitch12,
		starNetBand13, starNetCallsign13, starNetLogoff13, starNetInfo13, starNetPermanent13, starNetUserTimeout13, starNetGroupTimeout13, starNetCallsignSwitch13, starNetTXMsgSwitch13,
		starNetBand14, starNetCallsign14, starNetLogoff14, starNetInfo14, starNetPermanent14, starNetUserTimeout14, starNetGroupTimeout14, starNetCallsignSwitch14, starNetTXMsgSwitch14,
		starNetBand15, starNetCallsign15, starNetLogoff15, starNetInfo15, starNetPermanent15, starNetUserTimeout15, starNetGroupTimeout15, starNetCallsignSwitch15, starNetTXMsgSwitch15,
		remoteEnabled, remotePassword, remotePort);
#endif
	if (dialog1.ShowModal() != wxID_OK)
		return;

	callsign         = dialog1.getCallsign();
	address          = dialog1.getAddress();

	hostname         = dialog1.getHostname();
	username         = dialog1.getUsername();
	password         = dialog1.getPassword();

	starNetBand1         = dialog1.getStarNetBand1();
	starNetCallsign1     = dialog1.getStarNetCallsign1();
	starNetLogoff1       = dialog1.getStarNetLogoff1();
	starNetInfo1         = dialog1.getStarNetInfo1();
	starNetPermanent1    = dialog1.getStarNetPermanent1();
	starNetUserTimeout1  = dialog1.getStarNetUserTimeout1();
	starNetGroupTimeout1 = dialog1.getStarNetGroupTimeout1();
	starNetCallsignSwitch1 = dialog1.getStarNetCallsignSwitch1();
	starNetTXMsgSwitch1  = dialog1.getStarNetTXMsgSwitch1();
#if defined(DEXTRA_LINK) || defined(DCS_LINK)
	starNetLink1         = dialog1.getStarNetLink1();
#endif

	starNetBand2         = dialog1.getStarNetBand2();
	starNetCallsign2     = dialog1.getStarNetCallsign2();
	starNetLogoff2       = dialog1.getStarNetLogoff2();
	starNetInfo2         = dialog1.getStarNetInfo2();
	starNetPermanent2    = dialog1.getStarNetPermanent2();
	starNetUserTimeout2  = dialog1.getStarNetUserTimeout2();
	starNetGroupTimeout2 = dialog1.getStarNetGroupTimeout2();
	starNetCallsignSwitch2 = dialog1.getStarNetCallsignSwitch2();
	starNetTXMsgSwitch2  = dialog1.getStarNetTXMsgSwitch2();
#if defined(DEXTRA_LINK) || defined(DCS_LINK)
	starNetLink2         = dialog1.getStarNetLink2();
#endif

	starNetBand3         = dialog1.getStarNetBand3();
	starNetCallsign3     = dialog1.getStarNetCallsign3();
	starNetLogoff3       = dialog1.getStarNetLogoff3();
	starNetInfo3         = dialog1.getStarNetInfo3();
	starNetPermanent3    = dialog1.getStarNetPermanent3();
	starNetUserTimeout3  = dialog1.getStarNetUserTimeout3();
	starNetGroupTimeout3 = dialog1.getStarNetGroupTimeout3();
	starNetCallsignSwitch3 = dialog1.getStarNetCallsignSwitch3();
	starNetTXMsgSwitch3  = dialog1.getStarNetTXMsgSwitch3();
#if defined(DEXTRA_LINK) || defined(DCS_LINK)
	starNetLink3         = dialog1.getStarNetLink3();
#endif

	starNetBand4         = dialog1.getStarNetBand4();
	starNetCallsign4     = dialog1.getStarNetCallsign4();
	starNetLogoff4       = dialog1.getStarNetLogoff4();
	starNetInfo4         = dialog1.getStarNetInfo4();
	starNetPermanent4    = dialog1.getStarNetPermanent4();
	starNetUserTimeout4  = dialog1.getStarNetUserTimeout4();
	starNetGroupTimeout4 = dialog1.getStarNetGroupTimeout4();
	starNetCallsignSwitch4 = dialog1.getStarNetCallsignSwitch4();
	starNetTXMsgSwitch4  = dialog1.getStarNetTXMsgSwitch4();
#if defined(DEXTRA_LINK) || defined(DCS_LINK)
	starNetLink4         = dialog1.getStarNetLink4();
#endif

	starNetBand5         = dialog1.getStarNetBand5();
	starNetCallsign5     = dialog1.getStarNetCallsign5();
	starNetLogoff5       = dialog1.getStarNetLogoff5();
	starNetInfo5         = dialog1.getStarNetInfo5();
	starNetPermanent5    = dialog1.getStarNetPermanent5();
	starNetUserTimeout5  = dialog1.getStarNetUserTimeout5();
	starNetGroupTimeout5 = dialog1.getStarNetGroupTimeout5();
	starNetCallsignSwitch5 = dialog1.getStarNetCallsignSwitch5();
	starNetTXMsgSwitch5  = dialog1.getStarNetTXMsgSwitch5();
#if defined(DEXTRA_LINK) || defined(DCS_LINK)
	starNetLink5         = dialog1.getStarNetLink5();
#endif

	starNetBand6         = dialog1.getStarNetBand6();
	starNetCallsign6     = dialog1.getStarNetCallsign6();
	starNetLogoff6       = dialog1.getStarNetLogoff6();
	starNetInfo6         = dialog1.getStarNetInfo6();
	starNetPermanent6    = dialog1.getStarNetPermanent6();
	starNetUserTimeout6  = dialog1.getStarNetUserTimeout6();
	starNetGroupTimeout6 = dialog1.getStarNetGroupTimeout6();
	starNetCallsignSwitch6 = dialog1.getStarNetCallsignSwitch6();
	starNetTXMsgSwitch6  = dialog1.getStarNetTXMsgSwitch6();
#if defined(DEXTRA_LINK) || defined(DCS_LINK)
	starNetLink6         = dialog1.getStarNetLink6();
#endif

	starNetBand7         = dialog1.getStarNetBand7();
	starNetCallsign7     = dialog1.getStarNetCallsign7();
	starNetLogoff7       = dialog1.getStarNetLogoff7();
	starNetInfo7         = dialog1.getStarNetInfo7();
	starNetPermanent7    = dialog1.getStarNetPermanent7();
	starNetUserTimeout7  = dialog1.getStarNetUserTimeout7();
	starNetGroupTimeout7 = dialog1.getStarNetGroupTimeout7();
	starNetCallsignSwitch7 = dialog1.getStarNetCallsignSwitch7();
	starNetTXMsgSwitch7  = dialog1.getStarNetTXMsgSwitch7();
#if defined(DEXTRA_LINK) || defined(DCS_LINK)
	starNetLink7         = dialog1.getStarNetLink7();
#endif

	starNetBand8         = dialog1.getStarNetBand8();
	starNetCallsign8     = dialog1.getStarNetCallsign8();
	starNetLogoff8       = dialog1.getStarNetLogoff8();
	starNetInfo8         = dialog1.getStarNetInfo8();
	starNetPermanent8    = dialog1.getStarNetPermanent8();
	starNetUserTimeout8  = dialog1.getStarNetUserTimeout8();
	starNetGroupTimeout8 = dialog1.getStarNetGroupTimeout8();
	starNetCallsignSwitch8 = dialog1.getStarNetCallsignSwitch8();
	starNetTXMsgSwitch8  = dialog1.getStarNetTXMsgSwitch8();
#if defined(DEXTRA_LINK) || defined(DCS_LINK)
	starNetLink8         = dialog1.getStarNetLink8();
#endif

	starNetBand9         = dialog1.getStarNetBand9();
	starNetCallsign9     = dialog1.getStarNetCallsign9();
	starNetLogoff9       = dialog1.getStarNetLogoff9();
	starNetInfo9         = dialog1.getStarNetInfo9();
	starNetPermanent9    = dialog1.getStarNetPermanent9();
	starNetUserTimeout9  = dialog1.getStarNetUserTimeout9();
	starNetGroupTimeout9 = dialog1.getStarNetGroupTimeout9();
	starNetCallsignSwitch9 = dialog1.getStarNetCallsignSwitch9();
	starNetTXMsgSwitch9  = dialog1.getStarNetTXMsgSwitch9();
#if defined(DEXTRA_LINK) || defined(DCS_LINK)
	starNetLink9         = dialog1.getStarNetLink9();
#endif

	starNetBand10         = dialog1.getStarNetBand10();
	starNetCallsign10     = dialog1.getStarNetCallsign10();
	starNetLogoff10       = dialog1.getStarNetLogoff10();
	starNetInfo10         = dialog1.getStarNetInfo10();
	starNetPermanent10    = dialog1.getStarNetPermanent10();
	starNetUserTimeout10  = dialog1.getStarNetUserTimeout10();
	starNetGroupTimeout10 = dialog1.getStarNetGroupTimeout10();
	starNetCallsignSwitch10 = dialog1.getStarNetCallsignSwitch10();
	starNetTXMsgSwitch10  = dialog1.getStarNetTXMsgSwitch10();
#if defined(DEXTRA_LINK) || defined(DCS_LINK)
	starNetLink10         = dialog1.getStarNetLink10();
#endif

	starNetBand11         = dialog1.getStarNetBand11();
	starNetCallsign11     = dialog1.getStarNetCallsign11();
	starNetLogoff11       = dialog1.getStarNetLogoff11();
	starNetInfo11         = dialog1.getStarNetInfo11();
	starNetPermanent11    = dialog1.getStarNetPermanent11();
	starNetUserTimeout11  = dialog1.getStarNetUserTimeout11();
	starNetGroupTimeout11 = dialog1.getStarNetGroupTimeout11();
	starNetCallsignSwitch11 = dialog1.getStarNetCallsignSwitch11();
	starNetTXMsgSwitch11  = dialog1.getStarNetTXMsgSwitch11();
#if defined(DEXTRA_LINK) || defined(DCS_LINK)
	starNetLink11         = dialog1.getStarNetLink11();
#endif

	starNetBand12         = dialog1.getStarNetBand12();
	starNetCallsign12     = dialog1.getStarNetCallsign12();
	starNetLogoff12       = dialog1.getStarNetLogoff12();
	starNetInfo12         = dialog1.getStarNetInfo12();
	starNetPermanent12    = dialog1.getStarNetPermanent12();
	starNetUserTimeout12  = dialog1.getStarNetUserTimeout12();
	starNetGroupTimeout12 = dialog1.getStarNetGroupTimeout12();
	starNetCallsignSwitch12 = dialog1.getStarNetCallsignSwitch12();
	starNetTXMsgSwitch12  = dialog1.getStarNetTXMsgSwitch12();
#if defined(DEXTRA_LINK) || defined(DCS_LINK)
	starNetLink12         = dialog1.getStarNetLink12();
#endif

	starNetBand13         = dialog1.getStarNetBand13();
	starNetCallsign13     = dialog1.getStarNetCallsign13();
	starNetLogoff13       = dialog1.getStarNetLogoff13();
	starNetInfo13         = dialog1.getStarNetInfo13();
	starNetPermanent13    = dialog1.getStarNetPermanent13();
	starNetUserTimeout13  = dialog1.getStarNetUserTimeout13();
	starNetGroupTimeout13 = dialog1.getStarNetGroupTimeout13();
	starNetCallsignSwitch13 = dialog1.getStarNetCallsignSwitch13();
	starNetTXMsgSwitch13  = dialog1.getStarNetTXMsgSwitch13();
#if defined(DEXTRA_LINK) || defined(DCS_LINK)
	starNetLink13         = dialog1.getStarNetLink13();
#endif

	starNetBand14         = dialog1.getStarNetBand14();
	starNetCallsign14     = dialog1.getStarNetCallsign14();
	starNetLogoff14       = dialog1.getStarNetLogoff14();
	starNetInfo14         = dialog1.getStarNetInfo14();
	starNetPermanent14    = dialog1.getStarNetPermanent14();
	starNetUserTimeout14  = dialog1.getStarNetUserTimeout14();
	starNetGroupTimeout14 = dialog1.getStarNetGroupTimeout14();
	starNetCallsignSwitch14 = dialog1.getStarNetCallsignSwitch14();
	starNetTXMsgSwitch14  = dialog1.getStarNetTXMsgSwitch14();
#if defined(DEXTRA_LINK) || defined(DCS_LINK)
	starNetLink14         = dialog1.getStarNetLink14();
#endif

	starNetBand15         = dialog1.getStarNetBand15();
	starNetCallsign15     = dialog1.getStarNetCallsign15();
	starNetLogoff15       = dialog1.getStarNetLogoff15();
	starNetInfo15         = dialog1.getStarNetInfo15();
	starNetPermanent15    = dialog1.getStarNetPermanent15();
	starNetUserTimeout15  = dialog1.getStarNetUserTimeout15();
	starNetGroupTimeout15 = dialog1.getStarNetGroupTimeout15();
	starNetCallsignSwitch15 = dialog1.getStarNetCallsignSwitch15();
	starNetTXMsgSwitch15  = dialog1.getStarNetTXMsgSwitch15();
#if defined(DEXTRA_LINK) || defined(DCS_LINK)
	starNetLink15         = dialog1.getStarNetLink15();
#endif

	remoteEnabled       = dialog1.getRemoteEnabled();
	remotePassword      = dialog1.getRemotePassword();
	remotePort          = dialog1.getRemotePort();

	logEnabled          = dialog1.getLogEnabled();

	::wxGetApp().setGateway(callsign, address);
	::wxGetApp().setIrcDDB(hostname, username, password);
#if defined(DEXTRA_LINK) || defined(DCS_LINK)
	::wxGetApp().setStarNet1(starNetBand1,   starNetCallsign1,  starNetLogoff1,  starNetInfo1,  starNetPermanent1,  starNetUserTimeout1,  starNetGroupTimeout1,  starNetCallsignSwitch1,  starNetTXMsgSwitch1,  starNetLink1);
	::wxGetApp().setStarNet2(starNetBand2,   starNetCallsign2,  starNetLogoff2,  starNetInfo2,  starNetPermanent2,  starNetUserTimeout2,  starNetGroupTimeout2,  starNetCallsignSwitch2,  starNetTXMsgSwitch2,  starNetLink2);
	::wxGetApp().setStarNet3(starNetBand3,   starNetCallsign3,  starNetLogoff3,  starNetInfo3,  starNetPermanent3,  starNetUserTimeout3,  starNetGroupTimeout3,  starNetCallsignSwitch3,  starNetTXMsgSwitch3,  starNetLink3);
	::wxGetApp().setStarNet4(starNetBand4,   starNetCallsign4,  starNetLogoff4,  starNetInfo4,  starNetPermanent4,  starNetUserTimeout4,  starNetGroupTimeout4,  starNetCallsignSwitch4,  starNetTXMsgSwitch4,  starNetLink4);
	::wxGetApp().setStarNet5(starNetBand5,   starNetCallsign5,  starNetLogoff5,  starNetInfo5,  starNetPermanent5,  starNetUserTimeout5,  starNetGroupTimeout5,  starNetCallsignSwitch5,  starNetTXMsgSwitch5,  starNetLink5);
	::wxGetApp().setStarNet6(starNetBand6,   starNetCallsign6,  starNetLogoff6,  starNetInfo6,  starNetPermanent6,  starNetUserTimeout6,  starNetGroupTimeout6,  starNetCallsignSwitch6,  starNetTXMsgSwitch6,  starNetLink6);
	::wxGetApp().setStarNet7(starNetBand7,   starNetCallsign7,  starNetLogoff7,  starNetInfo7,  starNetPermanent7,  starNetUserTimeout7,  starNetGroupTimeout7,  starNetCallsignSwitch7,  starNetTXMsgSwitch7,  starNetLink7);
	::wxGetApp().setStarNet8(starNetBand8,   starNetCallsign8,  starNetLogoff8,  starNetInfo8,  starNetPermanent8,  starNetUserTimeout8,  starNetGroupTimeout8,  starNetCallsignSwitch8,  starNetTXMsgSwitch8,  starNetLink8);
	::wxGetApp().setStarNet9(starNetBand9,   starNetCallsign9,  starNetLogoff9,  starNetInfo9,  starNetPermanent9,  starNetUserTimeout9,  starNetGroupTimeout9,  starNetCallsignSwitch9,  starNetTXMsgSwitch9,  starNetLink9);
	::wxGetApp().setStarNet10(starNetBand10, starNetCallsign10, starNetLogoff10, starNetInfo10, starNetPermanent10, starNetUserTimeout10, starNetGroupTimeout10, starNetCallsignSwitch10, starNetTXMsgSwitch10, starNetLink10);
	::wxGetApp().setStarNet11(starNetBand11, starNetCallsign11, starNetLogoff11, starNetInfo11, starNetPermanent11, starNetUserTimeout11, starNetGroupTimeout11, starNetCallsignSwitch11, starNetTXMsgSwitch11, starNetLink11);
	::wxGetApp().setStarNet12(starNetBand12, starNetCallsign12, starNetLogoff12, starNetInfo12, starNetPermanent12, starNetUserTimeout12, starNetGroupTimeout12, starNetCallsignSwitch12, starNetTXMsgSwitch12, starNetLink12);
	::wxGetApp().setStarNet13(starNetBand13, starNetCallsign13, starNetLogoff13, starNetInfo13, starNetPermanent13, starNetUserTimeout13, starNetGroupTimeout13, starNetCallsignSwitch13, starNetTXMsgSwitch13, starNetLink13);
	::wxGetApp().setStarNet14(starNetBand14, starNetCallsign14, starNetLogoff14, starNetInfo14, starNetPermanent14, starNetUserTimeout14, starNetGroupTimeout14, starNetCallsignSwitch14, starNetTXMsgSwitch14, starNetLink14);
	::wxGetApp().setStarNet15(starNetBand15, starNetCallsign15, starNetLogoff15, starNetInfo15, starNetPermanent15, starNetUserTimeout15, starNetGroupTimeout15, starNetCallsignSwitch15, starNetTXMsgSwitch15, starNetLink15);
#else
	::wxGetApp().setStarNet1(starNetBand1,   starNetCallsign1,  starNetLogoff1,  starNetInfo1,  starNetPermanent1,  starNetUserTimeout1,  starNetGroupTimeout1,  starNetCallsignSwitch1,  starNetTXMsgSwitch1);
	::wxGetApp().setStarNet2(starNetBand2,   starNetCallsign2,  starNetLogoff2,  starNetInfo2,  starNetPermanent2,  starNetUserTimeout2,  starNetGroupTimeout2,  starNetCallsignSwitch2,  starNetTXMsgSwitch2);
	::wxGetApp().setStarNet3(starNetBand3,   starNetCallsign3,  starNetLogoff3,  starNetInfo3,  starNetPermanent3,  starNetUserTimeout3,  starNetGroupTimeout3,  starNetCallsignSwitch3,  starNetTXMsgSwitch3);
	::wxGetApp().setStarNet4(starNetBand4,   starNetCallsign4,  starNetLogoff4,  starNetInfo4,  starNetPermanent4,  starNetUserTimeout4,  starNetGroupTimeout4,  starNetCallsignSwitch4,  starNetTXMsgSwitch4);
	::wxGetApp().setStarNet5(starNetBand5,   starNetCallsign5,  starNetLogoff5,  starNetInfo5,  starNetPermanent5,  starNetUserTimeout5,  starNetGroupTimeout5,  starNetCallsignSwitch5,  starNetTXMsgSwitch5);
	::wxGetApp().setStarNet6(starNetBand6,   starNetCallsign6,  starNetLogoff6,  starNetInfo6,  starNetPermanent6,  starNetUserTimeout6,  starNetGroupTimeout6,  starNetCallsignSwitch6,  starNetTXMsgSwitch6);
	::wxGetApp().setStarNet7(starNetBand7,   starNetCallsign7,  starNetLogoff7,  starNetInfo7,  starNetPermanent7,  starNetUserTimeout7,  starNetGroupTimeout7,  starNetCallsignSwitch7,  starNetTXMsgSwitch7);
	::wxGetApp().setStarNet8(starNetBand8,   starNetCallsign8,  starNetLogoff8,  starNetInfo8,  starNetPermanent8,  starNetUserTimeout8,  starNetGroupTimeout8,  starNetCallsignSwitch8,  starNetTXMsgSwitch8);
	::wxGetApp().setStarNet9(starNetBand9,   starNetCallsign9,  starNetLogoff9,  starNetInfo9,  starNetPermanent9,  starNetUserTimeout9,  starNetGroupTimeout9,  starNetCallsignSwitch9,  starNetTXMsgSwitch9);
	::wxGetApp().setStarNet10(starNetBand10, starNetCallsign10, starNetLogoff10, starNetInfo10, starNetPermanent10, starNetUserTimeout10, starNetGroupTimeout10, starNetCallsignSwitch10, starNetTXMsgSwitch10);
	::wxGetApp().setStarNet11(starNetBand11, starNetCallsign11, starNetLogoff11, starNetInfo11, starNetPermanent11, starNetUserTimeout11, starNetGroupTimeout11, starNetCallsignSwitch11, starNetTXMsgSwitch11);
	::wxGetApp().setStarNet12(starNetBand12, starNetCallsign12, starNetLogoff12, starNetInfo12, starNetPermanent12, starNetUserTimeout12, starNetGroupTimeout12, starNetCallsignSwitch12, starNetTXMsgSwitch12);
	::wxGetApp().setStarNet13(starNetBand13, starNetCallsign13, starNetLogoff13, starNetInfo13, starNetPermanent13, starNetUserTimeout13, starNetGroupTimeout13, starNetCallsignSwitch13, starNetTXMsgSwitch13);
	::wxGetApp().setStarNet14(starNetBand14, starNetCallsign14, starNetLogoff14, starNetInfo14, starNetPermanent14, starNetUserTimeout14, starNetGroupTimeout14, starNetCallsignSwitch14, starNetTXMsgSwitch14);
	::wxGetApp().setStarNet15(starNetBand15, starNetCallsign15, starNetLogoff15, starNetInfo15, starNetPermanent15, starNetUserTimeout15, starNetGroupTimeout15, starNetCallsignSwitch15, starNetTXMsgSwitch15);
#endif
	::wxGetApp().setRemote(remoteEnabled, remotePassword, remotePort);
	::wxGetApp().setMiscellaneous(logEnabled);
	::wxGetApp().writeConfig();

	wxMessageDialog dialog2(this, _("The changes made will not take effect\nuntil the application is restarted"), _("StarNET Server Information"), wxICON_INFORMATION);
	dialog2.ShowModal();
}

void CStarNetServerFrame::onUpdates(wxCommandEvent& event)
{
	m_updates = event.IsChecked();
}

void CStarNetServerFrame::onAbout(wxCommandEvent&)
{
	wxAboutDialogInfo info;
	info.AddDeveloper(wxT("Jonathan Naylor, G4KLX"));
	info.AddDeveloper(wxT("Michael Dirska, DL1BFF"));
	info.SetCopyright(wxT("(C) 2011-2014 using GPL v2 or later"));
	info.SetName(APPLICATION_NAME);
	info.SetVersion(VERSION);
	info.SetDescription(_("This program allows an Internet connected computer \nto become a StarNet Digital Server."));

	::wxAboutBox(info);
}

void CStarNetServerFrame::onLog(wxEvent& event)
{
	CLogEvent& logEvent = dynamic_cast<CLogEvent&>(event);

	wxString text;

	text = m_logLine[1U]->GetLabel();
	m_logLine[0U]->SetLabel(text);

	text = m_logLine[2U]->GetLabel();
	m_logLine[1U]->SetLabel(text);

	text = m_logLine[3U]->GetLabel();
	m_logLine[2U]->SetLabel(text);

	text = m_logLine[4U]->GetLabel();
	m_logLine[3U]->SetLabel(text);

	text = m_logLine[5U]->GetLabel();
	m_logLine[4U]->SetLabel(text);

	text = m_logLine[6U]->GetLabel();
	m_logLine[5U]->SetLabel(text);

	text = m_logLine[7U]->GetLabel();
	m_logLine[6U]->SetLabel(text);

	text = m_logLine[8U]->GetLabel();
	m_logLine[7U]->SetLabel(text);

	text = m_logLine[9U]->GetLabel();
	m_logLine[8U]->SetLabel(text);

	text = m_logLine[10U]->GetLabel();
	m_logLine[9U]->SetLabel(text);

	text = m_logLine[11U]->GetLabel();
	m_logLine[10U]->SetLabel(text);

	text = m_logLine[12U]->GetLabel();
	m_logLine[11U]->SetLabel(text);

	text = m_logLine[13U]->GetLabel();
	m_logLine[12U]->SetLabel(text);

	text = m_logLine[14U]->GetLabel();
	m_logLine[13U]->SetLabel(text);

	text = m_logLine[15U]->GetLabel();
	m_logLine[14U]->SetLabel(text);

	text = m_logLine[16U]->GetLabel();
	m_logLine[15U]->SetLabel(text);

	text = m_logLine[17U]->GetLabel();
	m_logLine[16U]->SetLabel(text);

	text = m_logLine[18U]->GetLabel();
	m_logLine[17U]->SetLabel(text);

	text = m_logLine[19U]->GetLabel();
	m_logLine[18U]->SetLabel(text);

	text = logEvent.getText();
	m_logLine[19U]->SetLabel(text);
}

void CStarNetServerFrame::showLog(const wxString& text)
{
	if (!m_updates)
		return;

	CLogEvent event(text, LOG_EVENT);

	AddPendingEvent(event);
}
