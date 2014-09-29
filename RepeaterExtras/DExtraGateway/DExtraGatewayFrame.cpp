/*
 *   Copyright (C) 2010,2012,2013 by Jonathan Naylor G4KLX
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

#include "DExtraGatewayPreferences.h"
#include "DExtraGatewayFrame.h"
#include "DExtraGatewayApp.h"
#include "Version.h"

const unsigned int BORDER_SIZE   = 5U;
const unsigned int LABEL_WIDTH   = 80U;
const unsigned int CONTROL_WIDTH = 100U;

#include <wx/gbsizer.h>
#include <wx/aboutdlg.h>

enum {
	Menu_Edit_Preferences = 6000,
	Menu_View_Updates
};

BEGIN_EVENT_TABLE(CDExtraGatewayFrame, wxFrame)
	EVT_MENU(wxID_EXIT, CDExtraGatewayFrame::onQuit)
	EVT_MENU(Menu_Edit_Preferences, CDExtraGatewayFrame::onPreferences)
	EVT_MENU(Menu_View_Updates, CDExtraGatewayFrame::onUpdates)
	EVT_MENU(wxID_ABOUT, CDExtraGatewayFrame::onAbout)

	EVT_CLOSE(CDExtraGatewayFrame::onClose)

	EVT_TIMER(-1, CDExtraGatewayFrame::onTimer)
END_EVENT_TABLE()

CDExtraGatewayFrame::CDExtraGatewayFrame(const wxString& title, const wxPoint& position, bool gui) :
wxFrame(NULL, -1, title, position),
m_timer(this),
m_state(NULL),
m_your(NULL),
m_rpt1(NULL),
m_my(NULL),
m_rpt2(NULL),
m_reconnect(NULL),
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

	wxStaticBoxSizer* stat1Sizer = new wxStaticBoxSizer(new wxStaticBox(panel, -1, _("Status")), wxVERTICAL);
	wxFlexGridSizer* stat2Sizer = new wxFlexGridSizer(4);

	wxStaticText* stateLabel = new wxStaticText(panel, -1, _("State:"), wxDefaultPosition, wxSize(LABEL_WIDTH, -1), wxALIGN_RIGHT);
	stat2Sizer->Add(stateLabel, 0, wxALL, BORDER_SIZE);

	m_state = new wxStaticText(panel, -1, wxEmptyString, wxDefaultPosition, wxSize(CONTROL_WIDTH, -1));
	stat2Sizer->Add(m_state, 0, wxALL, BORDER_SIZE);

	wxStaticText* dummy1 = new wxStaticText(panel, -1, wxEmptyString, wxDefaultPosition, wxSize(LABEL_WIDTH, -1));
	stat2Sizer->Add(dummy1, 0, wxALL, BORDER_SIZE);

	wxStaticText* dummy2 = new wxStaticText(panel, -1, wxEmptyString, wxDefaultPosition, wxSize(CONTROL_WIDTH, -1));
	stat2Sizer->Add(dummy2, 0, wxALL, BORDER_SIZE);

	stat1Sizer->Add(stat2Sizer);
	panelSizer->Add(stat1Sizer, 0, wxALL, BORDER_SIZE);

	wxStaticBoxSizer* info1Sizer = new wxStaticBoxSizer(new wxStaticBox(panel, -1, _("Header")), wxVERTICAL);
	wxFlexGridSizer* info2Sizer = new wxFlexGridSizer(4);

	wxStaticText* yourLabel = new wxStaticText(panel, -1, wxT("UR:"), wxDefaultPosition, wxSize(LABEL_WIDTH, -1), wxALIGN_RIGHT);
	info2Sizer->Add(yourLabel, 0, wxALL, BORDER_SIZE);

	m_your = new wxStaticText(panel, -1, wxEmptyString, wxDefaultPosition, wxSize(CONTROL_WIDTH, -1));
	info2Sizer->Add(m_your, 0, wxALL, BORDER_SIZE);

	wxStaticText* rpt1Label = new wxStaticText(panel, -1, wxT("RPT1:"), wxDefaultPosition, wxSize(LABEL_WIDTH, -1), wxALIGN_RIGHT);
	info2Sizer->Add(rpt1Label, 0, wxALL, BORDER_SIZE);

	m_rpt1 = new wxStaticText(panel, -1, wxEmptyString, wxDefaultPosition, wxSize(CONTROL_WIDTH, -1));
	info2Sizer->Add(m_rpt1, 0, wxALL, BORDER_SIZE);

	wxStaticText* myLabel = new wxStaticText(panel, -1, wxT("MY:"), wxDefaultPosition, wxSize(LABEL_WIDTH, -1), wxALIGN_RIGHT);
	info2Sizer->Add(myLabel, 0, wxALL, BORDER_SIZE);

	m_my = new wxStaticText(panel, -1, wxEmptyString, wxDefaultPosition, wxSize(CONTROL_WIDTH, -1));
	info2Sizer->Add(m_my, 0, wxALL, BORDER_SIZE);

	wxStaticText* rpt2Label = new wxStaticText(panel, -1, wxT("RPT2:"), wxDefaultPosition, wxSize(LABEL_WIDTH, -1), wxALIGN_RIGHT);
	info2Sizer->Add(rpt2Label, 0, wxALL, BORDER_SIZE);

	m_rpt2 = new wxStaticText(panel, -1, wxEmptyString, wxDefaultPosition, wxSize(CONTROL_WIDTH, -1));
	info2Sizer->Add(m_rpt2, 0, wxALL, BORDER_SIZE);

	info1Sizer->Add(info2Sizer);
	panelSizer->Add(info1Sizer, 0, wxALL, BORDER_SIZE);

	wxStaticBoxSizer* timer1Sizer = new wxStaticBoxSizer(new wxStaticBox(panel, -1, _("Timers")), wxVERTICAL);
	wxFlexGridSizer* timer2Sizer = new wxFlexGridSizer(4);

	wxStaticText* reconnectLabel = new wxStaticText(panel, -1, _("Reconnect:"), wxDefaultPosition, wxSize(LABEL_WIDTH, -1), wxALIGN_RIGHT);
	timer2Sizer->Add(reconnectLabel, 0, wxALL, BORDER_SIZE);

	m_reconnect = new wxStaticText(panel, -1, wxEmptyString, wxDefaultPosition, wxSize(CONTROL_WIDTH, -1));
	timer2Sizer->Add(m_reconnect, 0, wxALL, BORDER_SIZE);

	wxStaticText* dummy3 = new wxStaticText(panel, -1, wxEmptyString, wxDefaultPosition, wxSize(LABEL_WIDTH, -1));
	timer2Sizer->Add(dummy3, 0, wxALL, BORDER_SIZE);

	wxStaticText* dummy4 = new wxStaticText(panel, -1, wxEmptyString, wxDefaultPosition, wxSize(CONTROL_WIDTH, -1));
	timer2Sizer->Add(dummy4, 0, wxALL, BORDER_SIZE);

	timer1Sizer->Add(timer2Sizer);
	panelSizer->Add(timer1Sizer, 0, wxALL, BORDER_SIZE);

	panel->SetSizer(panelSizer);
	panelSizer->SetSizeHints(panel);

	mainSizer->Add(panel);

	SetSizer(mainSizer);
	mainSizer->SetSizeHints(this);

	m_timer.Start(1000);
}

CDExtraGatewayFrame::~CDExtraGatewayFrame()
{
}

wxMenuBar* CDExtraGatewayFrame::createMenuBar()
{
	wxMenu* fileMenu = new wxMenu();
	fileMenu->Append(wxID_EXIT, _("Exit"));

	wxMenu* editMenu = new wxMenu();
	editMenu->Append(Menu_Edit_Preferences, _("Preferences..."));

	wxMenu* viewMenu = new wxMenu();
	viewMenu->AppendCheckItem(Menu_View_Updates, _("GUI Updates"));
	viewMenu->Check(Menu_View_Updates, m_updates);

	wxMenu* helpMenu = new wxMenu();
	helpMenu->Append(wxID_ABOUT, _("About DExtra Gateway"));

	wxMenuBar* menuBar = new wxMenuBar();
	menuBar->Append(fileMenu, _("File"));
	menuBar->Append(editMenu, _("Edit"));
	menuBar->Append(viewMenu, _("View"));
	menuBar->Append(helpMenu, _("Help"));

	return menuBar;
}

void CDExtraGatewayFrame::onQuit(wxCommandEvent& event)
{
	Close(false);
}

void CDExtraGatewayFrame::onClose(wxCloseEvent& event)
{
	int x, y;
	GetPosition(&x, &y);
	if (x >= 0 && y >= 0) {
		::wxGetApp().setPosition(x, y);
		::wxGetApp().writeConfig();
	}

	Destroy();
}

void CDExtraGatewayFrame::onPreferences(wxCommandEvent& event)
{
	wxString callsign, reflector;
	bool atStartup;
	RECONNECT reconnect;
	TEXT_LANG language;
	::wxGetApp().getReflector(callsign, reflector, atStartup, reconnect, language);

	wxString repeaterCallsign, repeaterAddress, localAddress;
	unsigned int repeaterPort, localPort;
	::wxGetApp().getRepeater(repeaterCallsign, repeaterAddress, repeaterPort, localAddress, localPort);

	CDExtraGatewayPreferences dialog1(this, -1, callsign, reflector, atStartup, reconnect, language, repeaterCallsign, repeaterAddress, repeaterPort, localAddress, localPort);
	if (dialog1.ShowModal() != wxID_OK)
		return;

	callsign  = dialog1.getCallsign();
	reflector = dialog1.getReflector();
	atStartup = dialog1.atStartup();
	reconnect = dialog1.getReconnect();
	language  = dialog1.getLanguage();

	repeaterCallsign = dialog1.getRepeaterCallsign();
	repeaterAddress  = dialog1.getRepeaterAddress();
	repeaterPort     = dialog1.getRepeaterPort();
	localAddress     = dialog1.getLocalAddress();
	localPort        = dialog1.getLocalPort();

	::wxGetApp().setReflector(callsign, reflector, atStartup, reconnect, language);
	::wxGetApp().setRepeater(repeaterCallsign, repeaterAddress, repeaterPort, localAddress, localPort);
	::wxGetApp().writeConfig();

	wxMessageDialog dialog2(this, _("The changes made will not take effect\nuntil the application is restarted"), _("DExtra Gateway Information"), wxICON_INFORMATION);
	dialog2.ShowModal();
}

void CDExtraGatewayFrame::onUpdates(wxCommandEvent& event)
{
	m_updates = event.IsChecked();
}

void CDExtraGatewayFrame::onAbout(wxCommandEvent& event)
{
	wxAboutDialogInfo info;
	info.AddDeveloper(wxT("Jonathan Naylor, G4KLX"));
	info.SetCopyright(wxT("(C) 2010-2013 using GPL v2 or later"));
	info.SetName(APPLICATION_NAME);
	info.SetVersion(VERSION);
	info.SetDescription(_("This program allows a computer running a homebrew\nrepeater to access DExtra reflectors."));

	::wxAboutBox(info);
}

void CDExtraGatewayFrame::onTimer(wxTimerEvent& event)
{
	if (!m_updates)
		return;

	CDExtraGatewayStatusData* status = ::wxGetApp().getStatus();
	if (status == NULL)
		return;

	wxString text;
	switch (status->getState()) {
		case LINK_NONE:
			m_state->SetLabel(_("Not linked"));
			break;
		case LINK_LINKING:
			text.Printf(_("Linking to %s"), status->getReflector().c_str());
			m_state->SetLabel(text);
			break;
		case LINK_LINKED:
			text.Printf(_("Linked to %s"), status->getReflector().c_str());
			m_state->SetLabel(text);
			break;
	}

	m_your->SetLabel(status->getYourCall());

	m_my->SetLabel(status->getMyCall());

	m_rpt1->SetLabel(status->getRptCall1());

	m_rpt2->SetLabel(status->getRptCall2());

	text.Printf(wxT("%u/%u"), status->getReconnectTimer(), status->getReconnectExpiry());
	m_reconnect->SetLabel(text);

	delete status;
}
