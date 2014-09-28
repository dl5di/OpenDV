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

#include "DStarRepeaterFrame.h"
#include "DStarRepeaterApp.h"
#include "DStarDefines.h"
#include "LogEvent.h"
#include "Version.h"

#include <wx/aboutdlg.h>

DEFINE_EVENT_TYPE(LOG_EVENT)

enum {
	Menu_File_Logging = 6000,

	Menu_View_Updates,

	Menu_Action_Shutdown,
	Menu_Action_Startup,
	Menu_Action_Command1,
	Menu_Action_Command2,
	Menu_Action_Command3,
	Menu_Action_Command4,
	Menu_Action_Command5,
	Menu_Action_Command6,

	Menu_Output_1,
	Menu_Output_2,
	Menu_Output_3,
	Menu_Output_4
};

BEGIN_EVENT_TABLE(CDStarRepeaterFrame, wxFrame)
	EVT_MENU(Menu_File_Logging,     CDStarRepeaterFrame::onLogging)
	EVT_MENU(wxID_EXIT,             CDStarRepeaterFrame::onQuit)
	EVT_MENU(Menu_View_Updates,     CDStarRepeaterFrame::onUpdates)
	EVT_MENU(wxID_ABOUT,            CDStarRepeaterFrame::onAbout)

	EVT_MENU(Menu_Action_Startup,   CDStarRepeaterFrame::onActions)
	EVT_MENU(Menu_Action_Shutdown,  CDStarRepeaterFrame::onActions)
	EVT_MENU(Menu_Action_Command1,  CDStarRepeaterFrame::onActions)
	EVT_MENU(Menu_Action_Command2,  CDStarRepeaterFrame::onActions)
	EVT_MENU(Menu_Action_Command3,  CDStarRepeaterFrame::onActions)
	EVT_MENU(Menu_Action_Command4,  CDStarRepeaterFrame::onActions)
	EVT_MENU(Menu_Action_Command5,  CDStarRepeaterFrame::onActions)
	EVT_MENU(Menu_Action_Command6,  CDStarRepeaterFrame::onActions)

	EVT_MENU(Menu_Output_1,         CDStarRepeaterFrame::onOutputs)
	EVT_MENU(Menu_Output_2,         CDStarRepeaterFrame::onOutputs)
	EVT_MENU(Menu_Output_3,         CDStarRepeaterFrame::onOutputs)
	EVT_MENU(Menu_Output_4,         CDStarRepeaterFrame::onOutputs)

	EVT_CLOSE(CDStarRepeaterFrame::onClose)

	EVT_TIMER(-1, CDStarRepeaterFrame::onTimer)

	EVT_CUSTOM(LOG_EVENT, wxID_ANY, CDStarRepeaterFrame::onLog)
END_EVENT_TABLE()

#if defined(__WINDOWS__)
const unsigned int BORDER_SIZE    = 5U;
const unsigned int CONTROL_WIDTH  = 100U;
const unsigned int LABEL_WIDTH    = 70U;
const unsigned int LOGTEXT_WIDTH  = 560U;
#else
const unsigned int BORDER_SIZE    = 5U;
const unsigned int CONTROL_WIDTH  = 150U;
const unsigned int LABEL_WIDTH    = 70U;
const unsigned int LOGTEXT_WIDTH  = 710U;
#endif

CDStarRepeaterFrame::CDStarRepeaterFrame(const wxString& title, const wxString& type, const wxPoint& position, bool gui) :
wxFrame(NULL, -1, title, position),
m_logging(NULL),
m_rxState(NULL),
m_rptState(NULL),
m_tx(NULL),
m_squelch(NULL),
m_signal(NULL),
m_your(NULL),
m_my(NULL),
m_rpt1(NULL),
m_rpt2(NULL),
m_flags(NULL),
m_percent(NULL),
m_timeout(NULL),
m_beacon(NULL),
m_announce(NULL),
m_text(NULL),
m_status1(NULL),
m_status2(NULL),
m_status3(NULL),
m_status4(NULL),
m_status5(NULL),
m_logLine1(NULL),
m_logLine2(NULL),
m_logLine3(NULL),
m_timer(this),
m_outputMenu(NULL),
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
	wxFlexGridSizer* stat2Sizer = new wxFlexGridSizer(6);

	wxStaticText* rxStateLabel = new wxStaticText(panel, -1, wxT("RX State:"), wxDefaultPosition, wxSize(LABEL_WIDTH, -1), wxALIGN_RIGHT);
	stat2Sizer->Add(rxStateLabel, 0, wxALL, BORDER_SIZE);

	m_rxState = new wxStaticText(panel, -1, wxEmptyString, wxDefaultPosition, wxSize(CONTROL_WIDTH, -1));
	stat2Sizer->Add(m_rxState, 0, wxALL, BORDER_SIZE);

	wxStaticText* rptStateLabel = new wxStaticText(panel, -1, wxT("Rpt State:"), wxDefaultPosition, wxSize(LABEL_WIDTH, -1), wxALIGN_RIGHT);
	stat2Sizer->Add(rptStateLabel, 0, wxALL, BORDER_SIZE);

	m_rptState = new wxStaticText(panel, -1, wxEmptyString, wxDefaultPosition, wxSize(CONTROL_WIDTH, -1));
	stat2Sizer->Add(m_rptState, 0, wxALL, BORDER_SIZE);

	wxStaticText* txLabel = new wxStaticText(panel, -1, wxT("TX:"), wxDefaultPosition, wxSize(LABEL_WIDTH, -1), wxALIGN_RIGHT);
	stat2Sizer->Add(txLabel, 0, wxALL, BORDER_SIZE);

	m_tx = new wxStaticText(panel, -1, wxEmptyString, wxDefaultPosition, wxSize(CONTROL_WIDTH, -1));
	stat2Sizer->Add(m_tx, 0, wxALL, BORDER_SIZE);

	stat1Sizer->Add(stat2Sizer);
	panelSizer->Add(stat1Sizer, 0, wxALL, BORDER_SIZE);

	if (type.IsSameAs(wxT("DVAP"))) {
		wxStaticBoxSizer* rx1Sizer = new wxStaticBoxSizer(new wxStaticBox(panel, -1, _("Receiver")), wxVERTICAL);
		wxFlexGridSizer* rx2Sizer = new wxFlexGridSizer(6);

		wxStaticText* squelchLabel = new wxStaticText(panel, -1, wxT("Squelch:"), wxDefaultPosition, wxSize(LABEL_WIDTH, -1), wxALIGN_RIGHT);
		rx2Sizer->Add(squelchLabel, 0, wxALL, BORDER_SIZE);

		m_squelch = new wxStaticText(panel, -1, wxEmptyString, wxDefaultPosition, wxSize(CONTROL_WIDTH, -1));
		rx2Sizer->Add(m_squelch, 0, wxALL, BORDER_SIZE);

		wxStaticText* signalLabel = new wxStaticText(panel, -1, wxT("Signal:"), wxDefaultPosition, wxSize(LABEL_WIDTH, -1), wxALIGN_RIGHT);
		rx2Sizer->Add(signalLabel, 0, wxALL, BORDER_SIZE);

		m_signal = new wxStaticText(panel, -1, wxEmptyString, wxDefaultPosition, wxSize(CONTROL_WIDTH, -1));
		rx2Sizer->Add(m_signal, 0, wxALL, BORDER_SIZE);

		wxStaticText* dummy20Label = new wxStaticText(panel, -1, wxEmptyString, wxDefaultPosition, wxSize(LABEL_WIDTH, -1));
		rx2Sizer->Add(dummy20Label, 0, wxALL, BORDER_SIZE);

		wxStaticText* dummy21Label = new wxStaticText(panel, -1, wxEmptyString, wxDefaultPosition, wxSize(CONTROL_WIDTH, -1));
		rx2Sizer->Add(dummy21Label, 0, wxALL, BORDER_SIZE);

		rx1Sizer->Add(rx2Sizer);
		panelSizer->Add(rx1Sizer, 0, wxALL, BORDER_SIZE);
	}

	wxStaticBoxSizer* info1Sizer = new wxStaticBoxSizer(new wxStaticBox(panel, -1, _("Header")), wxVERTICAL);
	wxFlexGridSizer* info2Sizer = new wxFlexGridSizer(6);

	wxStaticText* yourLabel = new wxStaticText(panel, -1, wxT("UR:"), wxDefaultPosition, wxSize(LABEL_WIDTH, -1), wxALIGN_RIGHT);
	info2Sizer->Add(yourLabel, 0, wxALL, BORDER_SIZE);

	m_your = new wxStaticText(panel, -1, wxEmptyString, wxDefaultPosition, wxSize(CONTROL_WIDTH, -1));
	info2Sizer->Add(m_your, 0, wxALL, BORDER_SIZE);

	wxStaticText* rpt1Label = new wxStaticText(panel, -1, wxT("RPT1:"), wxDefaultPosition, wxSize(LABEL_WIDTH, -1), wxALIGN_RIGHT);
	info2Sizer->Add(rpt1Label, 0, wxALL, BORDER_SIZE);

	m_rpt1 = new wxStaticText(panel, -1, wxEmptyString, wxDefaultPosition, wxSize(CONTROL_WIDTH, -1));
	info2Sizer->Add(m_rpt1, 0, wxALL, BORDER_SIZE);

	wxStaticText* rpt2Label = new wxStaticText(panel, -1, wxT("RPT2:"), wxDefaultPosition, wxSize(LABEL_WIDTH, -1), wxALIGN_RIGHT);
	info2Sizer->Add(rpt2Label, 0, wxALL, BORDER_SIZE);

	m_rpt2 = new wxStaticText(panel, -1, wxEmptyString, wxDefaultPosition, wxSize(CONTROL_WIDTH, -1));
	info2Sizer->Add(m_rpt2, 0, wxALL, BORDER_SIZE);

	wxStaticText* myLabel = new wxStaticText(panel, -1, wxT("MY:"), wxDefaultPosition, wxSize(LABEL_WIDTH, -1), wxALIGN_RIGHT);
	info2Sizer->Add(myLabel, 0, wxALL, BORDER_SIZE);

	m_my = new wxStaticText(panel, -1, wxEmptyString, wxDefaultPosition, wxSize(CONTROL_WIDTH, -1));
	info2Sizer->Add(m_my, 0, wxALL, BORDER_SIZE);

	wxStaticText* flagsLabel = new wxStaticText(panel, -1, _("Flags:"), wxDefaultPosition, wxSize(LABEL_WIDTH, -1), wxALIGN_RIGHT);
	info2Sizer->Add(flagsLabel, 0, wxALL, BORDER_SIZE);

	m_flags = new wxStaticText(panel, -1, wxEmptyString, wxDefaultPosition, wxSize(LABEL_WIDTH, -1));
	info2Sizer->Add(m_flags, 0, wxALL, BORDER_SIZE);

	wxStaticText* percentLabel = new wxStaticText(panel, -1, _("Loss/BER:"), wxDefaultPosition, wxSize(LABEL_WIDTH, -1), wxALIGN_RIGHT);
	info2Sizer->Add(percentLabel, 0, wxALL, BORDER_SIZE);

	m_percent = new wxStaticText(panel, -1, wxEmptyString, wxDefaultPosition, wxSize(LABEL_WIDTH, -1));
	info2Sizer->Add(m_percent, 0, wxALL, BORDER_SIZE);

	info1Sizer->Add(info2Sizer);
	panelSizer->Add(info1Sizer, 0, wxALL, BORDER_SIZE);

	wxStaticBoxSizer* timer1Sizer = new wxStaticBoxSizer(new wxStaticBox(panel, -1, _("Timers")), wxVERTICAL);
	wxFlexGridSizer* timer2Sizer = new wxFlexGridSizer(6);

	wxStaticText* timeoutLabel = new wxStaticText(panel, -1, _("Timeout:"), wxDefaultPosition, wxSize(LABEL_WIDTH, -1), wxALIGN_RIGHT);
	timer2Sizer->Add(timeoutLabel, 0, wxALL, BORDER_SIZE);

	m_timeout = new wxStaticText(panel, -1, wxEmptyString, wxDefaultPosition, wxSize(CONTROL_WIDTH, -1));
	timer2Sizer->Add(m_timeout, 0, wxALL, BORDER_SIZE);

	wxStaticText* beaconLabel = new wxStaticText(panel, -1, _("Beacon:"), wxDefaultPosition, wxSize(LABEL_WIDTH, -1), wxALIGN_RIGHT);
	timer2Sizer->Add(beaconLabel, 0, wxALL, BORDER_SIZE);

	m_beacon = new wxStaticText(panel, -1, wxEmptyString, wxDefaultPosition, wxSize(CONTROL_WIDTH, -1));
	timer2Sizer->Add(m_beacon, 0, wxALL, BORDER_SIZE);

	wxStaticText* announceLabel = new wxStaticText(panel, -1, _("Announce:"), wxDefaultPosition, wxSize(LABEL_WIDTH, -1), wxALIGN_RIGHT);
	timer2Sizer->Add(announceLabel, 0, wxALL, BORDER_SIZE);

	m_announce = new wxStaticText(panel, -1, wxEmptyString, wxDefaultPosition, wxSize(CONTROL_WIDTH, -1));
	timer2Sizer->Add(m_announce, 0, wxALL, BORDER_SIZE);

	timer1Sizer->Add(timer2Sizer);
	panelSizer->Add(timer1Sizer, 0, wxALL, BORDER_SIZE);

	wxStaticBoxSizer* text1Sizer = new wxStaticBoxSizer(new wxStaticBox(panel, -1, _("Gateway")), wxVERTICAL);
	wxFlexGridSizer* text2Sizer = new wxFlexGridSizer(6);

	wxStaticText* textLabel = new wxStaticText(panel, -1, _("Ack Text:"), wxDefaultPosition, wxSize(LABEL_WIDTH, -1), wxALIGN_RIGHT);
	text2Sizer->Add(textLabel, 0, wxALL, BORDER_SIZE);

	m_text = new wxStaticText(panel, -1, wxEmptyString, wxDefaultPosition, wxSize(CONTROL_WIDTH, -1));
	text2Sizer->Add(m_text, 0, wxALL, BORDER_SIZE);

	wxStaticText* status1Label = new wxStaticText(panel, -1, _("Status 1:"), wxDefaultPosition, wxSize(LABEL_WIDTH, -1), wxALIGN_RIGHT);
	text2Sizer->Add(status1Label, 0, wxALL, BORDER_SIZE);

	m_status1 = new wxStaticText(panel, -1, wxEmptyString, wxDefaultPosition, wxSize(CONTROL_WIDTH, -1));
	text2Sizer->Add(m_status1, 0, wxALL, BORDER_SIZE);

	wxStaticText* status2Label = new wxStaticText(panel, -1, _("Status 2:"), wxDefaultPosition, wxSize(LABEL_WIDTH, -1), wxALIGN_RIGHT);
	text2Sizer->Add(status2Label, 0, wxALL, BORDER_SIZE);

	m_status2 = new wxStaticText(panel, -1, wxEmptyString, wxDefaultPosition, wxSize(CONTROL_WIDTH, -1));
	text2Sizer->Add(m_status2, 0, wxALL, BORDER_SIZE);

	wxStaticText* status3Label = new wxStaticText(panel, -1, _("Status 3:"), wxDefaultPosition, wxSize(LABEL_WIDTH, -1), wxALIGN_RIGHT);
	text2Sizer->Add(status3Label, 0, wxALL, BORDER_SIZE);

	m_status3 = new wxStaticText(panel, -1, wxEmptyString, wxDefaultPosition, wxSize(CONTROL_WIDTH, -1));
	text2Sizer->Add(m_status3, 0, wxALL, BORDER_SIZE);

	wxStaticText* status4Label = new wxStaticText(panel, -1, _("Status 4:"), wxDefaultPosition, wxSize(LABEL_WIDTH, -1), wxALIGN_RIGHT);
	text2Sizer->Add(status4Label, 0, wxALL, BORDER_SIZE);

	m_status4 = new wxStaticText(panel, -1, wxEmptyString, wxDefaultPosition, wxSize(CONTROL_WIDTH, -1));
	text2Sizer->Add(m_status4, 0, wxALL, BORDER_SIZE);

	wxStaticText* status5Label = new wxStaticText(panel, -1, _("Status 5:"), wxDefaultPosition, wxSize(LABEL_WIDTH, -1), wxALIGN_RIGHT);
	text2Sizer->Add(status5Label, 0, wxALL, BORDER_SIZE);

	m_status5 = new wxStaticText(panel, -1, wxEmptyString, wxDefaultPosition, wxSize(CONTROL_WIDTH, -1));
	text2Sizer->Add(m_status5, 0, wxALL, BORDER_SIZE);

	text1Sizer->Add(text2Sizer);
	panelSizer->Add(text1Sizer, 0, wxALL, BORDER_SIZE);

	wxStaticBoxSizer* log1Sizer = new wxStaticBoxSizer(new wxStaticBox(panel, -1, _("Log")), wxVERTICAL);
	wxBoxSizer* log2Sizer = new wxBoxSizer(wxVERTICAL);

	m_logLine1 = new wxStaticText(panel, -1, wxEmptyString, wxDefaultPosition, wxSize(LOGTEXT_WIDTH, -1));
	m_logLine1->Wrap(LOGTEXT_WIDTH);
	log2Sizer->Add(m_logLine1, 0, wxTOP | wxLEFT | wxRIGHT, BORDER_SIZE);

	m_logLine2 = new wxStaticText(panel, -1, wxEmptyString, wxDefaultPosition, wxSize(LOGTEXT_WIDTH, -1));
	m_logLine2->Wrap(LOGTEXT_WIDTH);
	log2Sizer->Add(m_logLine2, 0, wxLEFT | wxRIGHT, BORDER_SIZE);

	m_logLine3 = new wxStaticText(panel, -1, wxEmptyString, wxDefaultPosition, wxSize(LOGTEXT_WIDTH, -1));
	m_logLine3->Wrap(LOGTEXT_WIDTH);
	log2Sizer->Add(m_logLine3, 0, wxLEFT | wxRIGHT | wxBOTTOM, BORDER_SIZE);

	log1Sizer->Add(log2Sizer);
	panelSizer->Add(log1Sizer, 0, wxALL, BORDER_SIZE);

	panel->SetSizer(panelSizer);
	panelSizer->SetSizeHints(panel);

	mainSizer->Add(panel);

	SetSizer(mainSizer);
	mainSizer->SetSizeHints(this);

	m_timer.Start(1000);
}

CDStarRepeaterFrame::~CDStarRepeaterFrame()
{
}

wxMenuBar* CDStarRepeaterFrame::createMenuBar()
{
	wxMenu* fileMenu = new wxMenu();
	m_logging = fileMenu->AppendCheckItem(Menu_File_Logging, _("Logging"));
	fileMenu->AppendSeparator();
	fileMenu->Append(wxID_EXIT, _("Exit"));

	wxMenu* viewMenu = new wxMenu();
	viewMenu->AppendCheckItem(Menu_View_Updates, _("GUI Updates"));
	viewMenu->Check(Menu_View_Updates, m_updates);

	wxMenu* actionMenu = new wxMenu();
	actionMenu->Append(Menu_Action_Shutdown, _("Shutdown"));
	actionMenu->Append(Menu_Action_Startup,  _("Startup"));
	actionMenu->Append(Menu_Action_Command1, _("Command 1"));
	actionMenu->Append(Menu_Action_Command2, _("Command 2"));
	actionMenu->Append(Menu_Action_Command3, _("Command 3"));
	actionMenu->Append(Menu_Action_Command4, _("Command 4"));
	actionMenu->Append(Menu_Action_Command5, _("Command 5"));
	actionMenu->Append(Menu_Action_Command6, _("Command 6"));

	m_outputMenu = new wxMenu();
	m_outputMenu->AppendCheckItem(Menu_Output_1, _("Output 1"));
	m_outputMenu->AppendCheckItem(Menu_Output_2, _("Output 2"));
	m_outputMenu->AppendCheckItem(Menu_Output_3, _("Output 3"));
	m_outputMenu->AppendCheckItem(Menu_Output_4, _("Output 4"));

	wxMenu* helpMenu = new wxMenu();
	helpMenu->Append(wxID_ABOUT, _("About D-Star Repeater"));

	wxMenuBar* menuBar = new wxMenuBar();
	menuBar->Append(fileMenu,     _("File"));
	menuBar->Append(viewMenu,     _("View"));
	menuBar->Append(actionMenu,   _("Action"));
	menuBar->Append(m_outputMenu, _("Outputs"));
	menuBar->Append(helpMenu,     _("Help"));

	return menuBar;
}

void CDStarRepeaterFrame::setLogging(bool logging)
{
	m_logging->Check(logging);
}

void CDStarRepeaterFrame::onLogging(wxCommandEvent& event)
{
	::wxGetApp().setLogging(event.IsChecked());
}

void CDStarRepeaterFrame::onQuit(wxCommandEvent&)
{
	Close(false);
}

void CDStarRepeaterFrame::onClose(wxCloseEvent&)
{
	int x, y;
	GetPosition(&x, &y);
	if (x >= 0 && y >= 0)
		::wxGetApp().setPosition(x, y);

	Destroy();
}

void CDStarRepeaterFrame::onActions(wxCommandEvent& event)
{
	int id = event.GetId();

	switch (id) {
		case Menu_Action_Shutdown:
			::wxGetApp().shutdown();
			break;
		case Menu_Action_Startup:
			::wxGetApp().startup();
			break;
		case Menu_Action_Command1:
			::wxGetApp().command1();
			break;
		case Menu_Action_Command2:
			::wxGetApp().command2();
			break;
		case Menu_Action_Command3:
			::wxGetApp().command3();
			break;
		case Menu_Action_Command4:
			::wxGetApp().command4();
			break;
		case Menu_Action_Command5:
			::wxGetApp().command5();
			break;
		case Menu_Action_Command6:
			::wxGetApp().command6();
			break;
	}
}

void CDStarRepeaterFrame::onOutputs(wxCommandEvent&)
{
	bool out1 = m_outputMenu->IsChecked(Menu_Output_1);
	bool out2 = m_outputMenu->IsChecked(Menu_Output_2);
	bool out3 = m_outputMenu->IsChecked(Menu_Output_3);
	bool out4 = m_outputMenu->IsChecked(Menu_Output_4);

	::wxGetApp().setOutputs(out1, out2, out3, out4);
}

void CDStarRepeaterFrame::onUpdates(wxCommandEvent& event)
{
	m_updates = event.IsChecked();
}

void CDStarRepeaterFrame::onAbout(wxCommandEvent&)
{
	wxAboutDialogInfo info;
	info.AddDeveloper(wxT("Jonathan Naylor, G4KLX"));
	info.SetCopyright(wxT("(C) 2013,2014 using GPL v2 or later"));
	info.SetName(APPLICATION_NAME);
	info.SetVersion(VERSION);
	info.SetDescription(_("This program allows a computer with a suitable\nmodem driver to act as a D-Star repeater."));

	::wxAboutBox(info);
}

void CDStarRepeaterFrame::onTimer(wxTimerEvent&)
{
	if (!m_updates)
		return;

	CDStarRepeaterStatusData* status = ::wxGetApp().getStatus();
	if (status == NULL)
		return;

	switch (status->getRxState()) {
		case DSRXS_LISTENING:
			m_rxState->SetLabel(_("Listening"));
			break;
		case DSRXS_PROCESS_DATA:
			m_rxState->SetLabel(_("Process Data"));
			break;
		case DSRXS_PROCESS_SLOW_DATA:
			m_rxState->SetLabel(_("Slow Data"));
			break;
	}

	switch (status->getRptState()) {
		case DSRS_SHUTDOWN:
			m_rptState->SetLabel(_("Shutdown"));
			break;
		case DSRS_LISTENING:
			m_rptState->SetLabel(_("Listening"));
			break;
		case DSRS_VALID:
			m_rptState->SetLabel(_("Valid"));
			break;
		case DSRS_VALID_WAIT:
		case DSRS_INVALID_WAIT:
		case DSRS_TIMEOUT_WAIT:
			m_rptState->SetLabel(_("Wait"));
			break;
		case DSRS_INVALID:
			m_rptState->SetLabel(_("Invalid"));
			break;
		case DSRS_TIMEOUT:
			m_rptState->SetLabel(_("Timeout"));
			break;
		case DSRS_NETWORK:
			m_rptState->SetLabel(_("Network"));
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

	wxString text;

	text.Printf(wxT("%02X %02X %02X"), status->getFlag1(), status->getFlag2(), status->getFlag3());
	m_flags->SetLabel(text);

	text.Printf(wxT("%.1f%%"), status->getPercent());
	m_percent->SetLabel(text);

	bool tx = status->getTX();
	m_tx->SetLabel(tx ? _("On") : _("Off"));

	text.Printf(wxT("%u/%u"), status->getTimeoutTimer(), status->getTimeoutExpiry());
	m_timeout->SetLabel(text);

	text.Printf(wxT("%u/%u"), status->getBeaconTimer(), status->getBeaconExpiry());
	m_beacon->SetLabel(text);

	text.Printf(wxT("%u/%u"), status->getAnnounceTimer(), status->getAnnounceExpiry());
	m_announce->SetLabel(text);

	m_text->SetLabel(status->getText());
	m_status1->SetLabel(status->getStatus1());
	m_status2->SetLabel(status->getStatus2());
	m_status3->SetLabel(status->getStatus3());
	m_status4->SetLabel(status->getStatus4());
	m_status5->SetLabel(status->getStatus5());

	// DVAP
	if (m_squelch != NULL) {
		if (tx) {
			m_squelch->SetLabel(wxEmptyString);
			m_signal->SetLabel(wxEmptyString);
		} else {
			bool squelch = status->getSquelch();
			m_squelch->SetLabel(squelch ? _("Open") : _("Closed"));

			text.Printf(wxT("%d dBm"), status->getSignal());
			m_signal->SetLabel(text);
		}
	}

	delete status;
}

void CDStarRepeaterFrame::onLog(wxEvent& event)
{
	CLogEvent& logEvent = dynamic_cast<CLogEvent&>(event);

	wxString text = logEvent.getText();

	m_logLine1->SetLabel(m_logLine2->GetLabel());
	m_logLine2->SetLabel(m_logLine3->GetLabel());
	m_logLine3->SetLabel(text);
}

void CDStarRepeaterFrame::showLog(const wxString& text)
{
	if (!m_updates)
		return;

	CLogEvent event(text, LOG_EVENT);

	AddPendingEvent(event);
}

void CDStarRepeaterFrame::setOutputs(bool out1, bool out2, bool out3, bool out4)
{
	m_outputMenu->Check(Menu_Output_1, out1);
	m_outputMenu->Check(Menu_Output_2, out2);
	m_outputMenu->Check(Menu_Output_3, out3);
	m_outputMenu->Check(Menu_Output_4, out4);
}
