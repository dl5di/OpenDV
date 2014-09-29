/*
 *   Copyright (C) 2009,2010,2012,2013 by Jonathan Naylor G4KLX
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

#include "ParrotControllerPreferences.h"
#include "ParrotControllerFrame.h"
#include "ParrotControllerApp.h"
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

BEGIN_EVENT_TABLE(CParrotControllerFrame, wxFrame)
	EVT_MENU(wxID_EXIT, CParrotControllerFrame::onQuit)
	EVT_MENU(Menu_Edit_Preferences, CParrotControllerFrame::onPreferences)
	EVT_MENU(Menu_View_Updates, CParrotControllerFrame::onUpdates)
	EVT_MENU(wxID_ABOUT, CParrotControllerFrame::onAbout)

	EVT_CLOSE(CParrotControllerFrame::onClose)

	EVT_TIMER(-1, CParrotControllerFrame::onTimer)
END_EVENT_TABLE()

CParrotControllerFrame::CParrotControllerFrame(const wxString& title, const wxPoint& position, bool gui) :
wxFrame(NULL, -1, title, position),
m_timer(this),
m_state(NULL),
m_your(NULL),
m_rpt1(NULL),
m_my(NULL),
m_rpt2(NULL),
m_flags(NULL),
m_data(NULL),
m_turnaround(NULL),
m_beacon(NULL),
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

	wxStaticText* flagsLabel = new wxStaticText(panel, -1, wxT("Flags:"), wxDefaultPosition, wxSize(LABEL_WIDTH, -1), wxALIGN_RIGHT);
	info2Sizer->Add(flagsLabel, 0, wxALL, BORDER_SIZE);

	m_flags = new wxStaticText(panel, -1, wxEmptyString, wxDefaultPosition, wxSize(CONTROL_WIDTH, -1));
	info2Sizer->Add(m_flags, 0, wxALL, BORDER_SIZE);

	info1Sizer->Add(info2Sizer);
	panelSizer->Add(info1Sizer, 0, wxALL, BORDER_SIZE);

	wxStaticBoxSizer* timer1Sizer = new wxStaticBoxSizer(new wxStaticBox(panel, -1, _("Timers")), wxVERTICAL);
	wxFlexGridSizer* timer2Sizer = new wxFlexGridSizer(4);

	wxStaticText* dataLabel = new wxStaticText(panel, -1, _("Data:"), wxDefaultPosition, wxSize(LABEL_WIDTH, -1), wxALIGN_RIGHT);
	timer2Sizer->Add(dataLabel, 0, wxALL, BORDER_SIZE);

	m_data = new wxStaticText(panel, -1, wxEmptyString, wxDefaultPosition, wxSize(CONTROL_WIDTH, -1));
	timer2Sizer->Add(m_data, 0, wxALL, BORDER_SIZE);

	wxStaticText* turnaroundLabel = new wxStaticText(panel, -1, _("Turnaround:"), wxDefaultPosition, wxSize(LABEL_WIDTH, -1), wxALIGN_RIGHT);
	timer2Sizer->Add(turnaroundLabel, 0, wxALL, BORDER_SIZE);

	m_turnaround = new wxStaticText(panel, -1, wxEmptyString, wxDefaultPosition, wxSize(CONTROL_WIDTH, -1));
	timer2Sizer->Add(m_turnaround, 0, wxALL, BORDER_SIZE);

	wxStaticText* beaconLabel = new wxStaticText(panel, -1, _("Beacon:"), wxDefaultPosition, wxSize(LABEL_WIDTH, -1), wxALIGN_RIGHT);
	timer2Sizer->Add(beaconLabel, 0, wxALL, BORDER_SIZE);

	m_beacon = new wxStaticText(panel, -1, wxEmptyString, wxDefaultPosition, wxSize(CONTROL_WIDTH, -1));
	timer2Sizer->Add(m_beacon, 0, wxALL, BORDER_SIZE);

	timer1Sizer->Add(timer2Sizer);
	panelSizer->Add(timer1Sizer, 0, wxALL, BORDER_SIZE);

	panel->SetSizer(panelSizer);
	panelSizer->SetSizeHints(panel);

	mainSizer->Add(panel);

	SetSizer(mainSizer);
	mainSizer->SetSizeHints(this);

	m_timer.Start(1000);
}

CParrotControllerFrame::~CParrotControllerFrame()
{
}

wxMenuBar* CParrotControllerFrame::createMenuBar()
{
	wxMenu* fileMenu = new wxMenu();
	fileMenu->Append(wxID_EXIT, _("Exit"));

	wxMenu* editMenu = new wxMenu();
	editMenu->Append(Menu_Edit_Preferences, _("Preferences..."));

	wxMenu* viewMenu = new wxMenu();
	viewMenu->AppendCheckItem(Menu_View_Updates, _("GUI Updates"));
	viewMenu->Check(Menu_View_Updates, m_updates);

	wxMenu* helpMenu = new wxMenu();
	helpMenu->Append(wxID_ABOUT, _("About Parrot Controller"));

	wxMenuBar* menuBar = new wxMenuBar();
	menuBar->Append(fileMenu, _("File"));
	menuBar->Append(editMenu, _("Edit"));
	menuBar->Append(viewMenu, _("View"));
	menuBar->Append(helpMenu, _("Help"));

	return menuBar;
}

void CParrotControllerFrame::onQuit(wxCommandEvent& event)
{
	Close(false);
}

void CParrotControllerFrame::onClose(wxCloseEvent& event)
{
	int x, y;
	GetPosition(&x, &y);
	if (x >= 0 && y >= 0) {
		::wxGetApp().setPosition(x, y);
		::wxGetApp().writeConfig();
	}

	Destroy();
}

void CParrotControllerFrame::onPreferences(wxCommandEvent& event)
{
	PARROT_MODE mode;
	unsigned int beaconTime, turnaroundTime;
	wxString beaconFileName;
	bool keepFile;
	::wxGetApp().getMode(mode, beaconTime, beaconFileName, turnaroundTime, keepFile);

	wxString repeaterAddress, localAddress;
	unsigned int repeaterPort, localPort;
	::wxGetApp().getNetwork(repeaterAddress, repeaterPort, localAddress, localPort);

	CParrotControllerPreferences dialog1(this, -1, mode, beaconTime, beaconFileName, turnaroundTime, keepFile,
										 repeaterAddress, repeaterPort, localAddress, localPort);
	if (dialog1.ShowModal() != wxID_OK)
		return;

	mode           = dialog1.getMode();
	beaconTime     = dialog1.getBeaconTime();
	beaconFileName = dialog1.getBeaconFileName();
	turnaroundTime = dialog1.getTurnaroundTime();
	keepFile       = dialog1.getKeepFile();

	repeaterAddress = dialog1.getRepeaterAddress();
	repeaterPort    = dialog1.getRepeaterPort();
	localAddress    = dialog1.getLocalAddress();
	localPort       = dialog1.getLocalPort();

	::wxGetApp().setMode(mode, beaconTime, beaconFileName, turnaroundTime, keepFile);
	::wxGetApp().setNetwork(repeaterAddress, repeaterPort, localAddress, localPort);
	::wxGetApp().writeConfig();

	wxMessageDialog dialog2(this, _("The changes made will not take effect\nuntil the application is restarted"), _("Parrot Controller Information"), wxICON_INFORMATION);
	dialog2.ShowModal();
}

void CParrotControllerFrame::onUpdates(wxCommandEvent& event)
{
	m_updates = event.IsChecked();
}

void CParrotControllerFrame::onAbout(wxCommandEvent& event)
{
	wxAboutDialogInfo info;
	info.AddDeveloper(wxT("Jonathan Naylor, G4KLX"));
	info.SetCopyright(wxT("(C) 2009-2013 using GPL v2 or later"));
	info.SetName(APPLICATION_NAME);
	info.SetVersion(VERSION);
	info.SetDescription(_("This program allows a computer running a homebrew\nrepeater to act as a D-Star Parrot or Beacon."));

	::wxAboutBox(info);
}

void CParrotControllerFrame::onTimer(wxTimerEvent& event)
{
	if (!m_updates)
		return;

	CParrotControllerStatusData* status = ::wxGetApp().getStatus();
	if (status == NULL)
		return;

	switch (status->getState()) {
		case PS_WAITING:
			m_state->SetLabel(_("Waiting"));
			break;
		case PS_RECEIVING:
			m_state->SetLabel(_("Receiving"));
			break;
		case PS_TRANSMITTING:
			m_state->SetLabel(_("Transmitting"));
			break;
	}

	m_your->SetLabel(status->getYourCall());

	wxString myCall  = status->getMyCall1();
	wxString myCall2 = status->getMyCall2();
	if (!myCall2.IsEmpty() && !myCall2.IsSameAs(wxT("    "))) {
		myCall.Append(wxT("/"));
		myCall.Append(myCall2);
	}
	m_my->SetLabel(myCall);

	m_rpt1->SetLabel(status->getRptCall1());

	m_rpt2->SetLabel(status->getRptCall2());

	wxString flagsText;
	flagsText.Printf(wxT("%02X %02X %02X"), status->getFlag1(), status->getFlag2(), status->getFlag3());
	m_flags->SetLabel(flagsText);

	wxString dataText;
	dataText.Printf(wxT("%u/%u"), status->getDataTimer() / DSTAR_TICKS_PER_SEC, status->getDataTimeout() / DSTAR_TICKS_PER_SEC);
	m_data->SetLabel(dataText);

	wxString turnaroundText;
	turnaroundText.Printf(wxT("%u/%u"), status->getTurnaroundTimer(), status->getTurnaroundExpiry());
	m_turnaround->SetLabel(turnaroundText);

	wxString beaconText;
	beaconText.Printf(wxT("%u/%u"), status->getBeaconTimer(), status->getBeaconExpiry());
	m_beacon->SetLabel(beaconText);

	delete status;
}
