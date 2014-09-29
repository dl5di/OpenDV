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

#include "DVAPNodePreferences.h"
#include "DVAPNodeFrame.h"
#include "DVAPNodeApp.h"
#include "DStarDefines.h"
#include "LogEvent.h"
#include "Version.h"

#include <wx/aboutdlg.h>

DEFINE_EVENT_TYPE(LOG_EVENT)

enum {
	Menu_File_Logging = 6000,

	Menu_Edit_Preferences,

	Menu_View_Updates
};

BEGIN_EVENT_TABLE(CDVAPNodeFrame, wxFrame)
	EVT_MENU(Menu_File_Logging,     CDVAPNodeFrame::onLogging)
	EVT_MENU(wxID_EXIT,             CDVAPNodeFrame::onQuit)
	EVT_MENU(Menu_View_Updates,     CDVAPNodeFrame::onUpdates)
	EVT_MENU(Menu_Edit_Preferences, CDVAPNodeFrame::onPreferences)
	EVT_MENU(wxID_ABOUT,            CDVAPNodeFrame::onAbout)

	EVT_CLOSE(CDVAPNodeFrame::onClose)

	EVT_TIMER(-1, CDVAPNodeFrame::onTimer)

	EVT_CUSTOM(LOG_EVENT, wxID_ANY, CDVAPNodeFrame::onLog)
END_EVENT_TABLE()

const unsigned int BORDER_SIZE    = 5U;
const unsigned int CONTROL_WIDTH  = 100U;
const unsigned int LABEL_WIDTH    = 70U;
const unsigned int LOGTEXT_WIDTH  = 560U;

CDVAPNodeFrame::CDVAPNodeFrame(const wxString& title, const wxPoint& position, bool gui) :
wxFrame(NULL, -1, title, position),
m_logging(NULL),
m_state(NULL),
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
m_logLine1(NULL),
m_logLine2(NULL),
m_logLine3(NULL),
m_timer(this),
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

	wxStaticText* stateLabel = new wxStaticText(panel, -1, wxT("State:"), wxDefaultPosition, wxSize(LABEL_WIDTH, -1), wxALIGN_RIGHT);
	stat2Sizer->Add(stateLabel, 0, wxALL, BORDER_SIZE);

	m_state = new wxStaticText(panel, -1, wxEmptyString, wxDefaultPosition, wxSize(CONTROL_WIDTH, -1));
	stat2Sizer->Add(m_state, 0, wxALL, BORDER_SIZE);

	wxStaticText* txLabel = new wxStaticText(panel, -1, wxT("TX:"), wxDefaultPosition, wxSize(LABEL_WIDTH, -1), wxALIGN_RIGHT);
	stat2Sizer->Add(txLabel, 0, wxALL, BORDER_SIZE);

	m_tx = new wxStaticText(panel, -1, wxEmptyString, wxDefaultPosition, wxSize(CONTROL_WIDTH, -1));
	stat2Sizer->Add(m_tx, 0, wxALL, BORDER_SIZE);

	wxStaticText* dummy7Label = new wxStaticText(panel, -1, wxEmptyString, wxDefaultPosition, wxSize(LABEL_WIDTH, -1));
	stat2Sizer->Add(dummy7Label, 0, wxALL, BORDER_SIZE);

	wxStaticText* dummy8Label = new wxStaticText(panel, -1, wxEmptyString, wxDefaultPosition, wxSize(CONTROL_WIDTH, -1));
	stat2Sizer->Add(dummy8Label, 0, wxALL, BORDER_SIZE);

	stat1Sizer->Add(stat2Sizer);
	panelSizer->Add(stat1Sizer, 0, wxALL, BORDER_SIZE);

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

	wxStaticText* dummy3Label = new wxStaticText(panel, -1, wxEmptyString, wxDefaultPosition, wxSize(LABEL_WIDTH, -1));
	text2Sizer->Add(dummy3Label, 0, wxALL, BORDER_SIZE);

	wxStaticText* dummy4Label = new wxStaticText(panel, -1, wxEmptyString, wxDefaultPosition, wxSize(CONTROL_WIDTH, -1));
	text2Sizer->Add(dummy4Label, 0, wxALL, BORDER_SIZE);

	wxStaticText* dummy5Label = new wxStaticText(panel, -1, wxEmptyString, wxDefaultPosition, wxSize(LABEL_WIDTH, -1));
	text2Sizer->Add(dummy5Label, 0, wxALL, BORDER_SIZE);

	wxStaticText* dummy6Label = new wxStaticText(panel, -1, wxEmptyString, wxDefaultPosition, wxSize(CONTROL_WIDTH, -1));
	text2Sizer->Add(dummy6Label, 0, wxALL, BORDER_SIZE);

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

CDVAPNodeFrame::~CDVAPNodeFrame()
{
}

wxMenuBar* CDVAPNodeFrame::createMenuBar()
{
	wxMenu* fileMenu = new wxMenu();
	m_logging = fileMenu->AppendCheckItem(Menu_File_Logging, _("Logging"));
	fileMenu->AppendSeparator();
	fileMenu->Append(wxID_EXIT, _("Exit"));

	wxMenu* editMenu = new wxMenu();
	editMenu->Append(Menu_Edit_Preferences, _("Preferences..."));

	wxMenu* viewMenu = new wxMenu();
	viewMenu->AppendCheckItem(Menu_View_Updates, _("GUI Updates"));
	viewMenu->Check(Menu_View_Updates, m_updates);

	wxMenu* helpMenu = new wxMenu();
	helpMenu->Append(wxID_ABOUT, _("About DVAP Node"));

	wxMenuBar* menuBar = new wxMenuBar();
	menuBar->Append(fileMenu,   _("File"));
	menuBar->Append(editMenu,   _("Edit"));
	menuBar->Append(viewMenu,   _("View"));
	menuBar->Append(helpMenu,   _("Help"));

	return menuBar;
}

void CDVAPNodeFrame::setLogging(bool logging)
{
	m_logging->Check(logging);
}

void CDVAPNodeFrame::onLogging(wxCommandEvent& event)
{
	::wxGetApp().setLogging(event.IsChecked());
}

void CDVAPNodeFrame::onQuit(wxCommandEvent& event)
{
	Close(false);
}

void CDVAPNodeFrame::onClose(wxCloseEvent& event)
{
	int x, y;
	GetPosition(&x, &y);
	if (x >= 0 && y >= 0) {
		::wxGetApp().setPosition(x, y);
		::wxGetApp().writeConfig();
	}

	Destroy();
}

void CDVAPNodeFrame::onUpdates(wxCommandEvent& event)
{
	m_updates = event.IsChecked();
}

void CDVAPNodeFrame::onAbout(wxCommandEvent& event)
{
	wxAboutDialogInfo info;
	info.AddDeveloper(wxT("Jonathan Naylor, G4KLX"));
	info.SetCopyright(wxT("(C) 2011-2014 using GPL v2 or later"));
	info.SetName(APPLICATION_NAME);
	info.SetVersion(VERSION);
	info.SetDescription(_("This program allows a computer with a DVAP\nand 9600Bd capable radios to act as a D-Star node."));

	::wxAboutBox(info);
}

void CDVAPNodeFrame::onPreferences(wxCommandEvent& event)
{
	wxString callsign, gateway;
	DSTAR_MODE mode;
	ACK_TYPE ack;
	bool restriction, rpt1Validation;
	::wxGetApp().getCallsign(callsign, gateway, mode, ack, restriction, rpt1Validation);

	wxString gatewayAddress, localAddress;
	unsigned int gatewayPort, localPort;
	::wxGetApp().getNetwork(gatewayAddress, gatewayPort, localAddress, localPort);

	unsigned int timeout, ackTime;
	::wxGetApp().getTimes(timeout, ackTime);

	unsigned int beaconTime;
	wxString beaconText;
	bool beaconVoice;
	TEXT_LANG language;
	::wxGetApp().getBeacon(beaconTime, beaconText, beaconVoice, language);

	bool announcementEnabled;
	unsigned int announcementTime;
	wxString announcementRecordRPT1, announcementRecordRPT2;
	wxString announcementDeleteRPT1, announcementDeleteRPT2;
	::wxGetApp().getAnnouncement(announcementEnabled, announcementTime, announcementRecordRPT1, announcementRecordRPT2, announcementDeleteRPT1, announcementDeleteRPT2);

	wxString port;
	unsigned int frequency;
	int power, squelch;
	::wxGetApp().getDVAP(port, frequency, power, squelch);

	CDVAPNodePreferences dialog1(this, -1, callsign, gateway, mode, ack, restriction, rpt1Validation, gatewayAddress,
		gatewayPort, localAddress, localPort, timeout, ackTime, beaconTime, beaconText, beaconVoice, language,
		announcementEnabled, announcementTime, announcementRecordRPT1, announcementRecordRPT2, announcementDeleteRPT1,
		announcementDeleteRPT2, port, frequency, power, squelch);
	if (dialog1.ShowModal() != wxID_OK)
		return;

	callsign       = dialog1.getCallsign();
	gateway        = dialog1.getGateway();
	mode           = dialog1.getMode();
	ack            = dialog1.getAck();
	restriction    = dialog1.getRestriction();
	rpt1Validation = dialog1.getRPT1Validation();
	::wxGetApp().setCallsign(callsign, gateway, mode, ack, restriction, rpt1Validation);

	gatewayAddress = dialog1.getGatewayAddress();
	gatewayPort    = dialog1.getGatewayPort();
	localAddress   = dialog1.getLocalAddress();
	localPort      = dialog1.getLocalPort();
	::wxGetApp().setNetwork(gatewayAddress, gatewayPort, localAddress, localPort);

	timeout    = dialog1.getTimeout();
	ackTime    = dialog1.getAckTime();
	::wxGetApp().setTimes(timeout, ackTime);

	beaconTime  = dialog1.getBeaconTime();
	beaconText  = dialog1.getBeaconText();
	beaconVoice = dialog1.getBeaconVoice();
	language    = dialog1.getLanguage();
	::wxGetApp().setBeacon(beaconTime, beaconText, beaconVoice, language);

	announcementEnabled    = dialog1.getAnnouncementEnabled();
	announcementTime       = dialog1.getAnnouncementTime();
	announcementRecordRPT1 = dialog1.getAnnouncementRecordRPT1();
	announcementRecordRPT2 = dialog1.getAnnouncementRecordRPT2();
	announcementDeleteRPT1 = dialog1.getAnnouncementDeleteRPT1();
	announcementDeleteRPT2 = dialog1.getAnnouncementDeleteRPT2();
	::wxGetApp().setAnnouncement(announcementEnabled, announcementTime, announcementRecordRPT1, announcementRecordRPT2, announcementDeleteRPT1, announcementDeleteRPT2);

	port      = dialog1.getPort();
	frequency = dialog1.getFrequency();
	power     = dialog1.getPower();
	squelch   = dialog1.getSquelch();
	::wxGetApp().setDVAP(port, frequency, power, squelch);

	::wxGetApp().writeConfig();

	wxMessageDialog dialog2(this, _("The changes made will not take effect\nuntil the application is restarted"), _("DVAP Node Information"), wxICON_INFORMATION);
	dialog2.ShowModal();
}

void CDVAPNodeFrame::onTimer(wxTimerEvent& event)
{
	if (!m_updates)
		return;

	CDVAPNodeStatusData* status = ::wxGetApp().getStatus();
	if (status == NULL)
		return;

	switch (status->getState()) {
		case DSRS_SHUTDOWN:
			m_state->SetLabel(_("Shutdown"));
			break;
		case DSRS_LISTENING:
			m_state->SetLabel(_("Listening"));
			break;
		case DSRS_VALID:
			m_state->SetLabel(_("Valid"));
			break;
		case DSRS_VALID_WAIT:
		case DSRS_INVALID_WAIT:
		case DSRS_TIMEOUT_WAIT:
			m_state->SetLabel(_("Wait"));
			break;
		case DSRS_INVALID:
			m_state->SetLabel(_("Invalid"));
			break;
		case DSRS_TIMEOUT:
			m_state->SetLabel(_("Timeout"));
			break;
		case DSRS_NETWORK:
			m_state->SetLabel(_("Network"));
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

	if (tx) {
		m_squelch->SetLabel(wxEmptyString);
		m_signal->SetLabel(wxEmptyString);
	} else {
		bool squelch = status->getSquelch();
		m_squelch->SetLabel(squelch ? _("Open") : _("Closed"));

		text.Printf(wxT("%d dBm"), status->getSignal());
		m_signal->SetLabel(text);
	}

	text.Printf(wxT("%u/%u"), status->getTimeoutTimer(), status->getTimeoutExpiry());
	m_timeout->SetLabel(text);

	text.Printf(wxT("%u/%u"), status->getBeaconTimer(), status->getBeaconExpiry());
	m_beacon->SetLabel(text);

	text.Printf(wxT("%u/%u"), status->getAnnounceTimer(), status->getAnnounceExpiry());
	m_announce->SetLabel(text);

	m_text->SetLabel(status->getText());

	delete status;
}

void CDVAPNodeFrame::onLog(wxEvent& event)
{
	CLogEvent& logEvent = dynamic_cast<CLogEvent&>(event);

	wxString text = logEvent.getText();

	m_logLine1->SetLabel(m_logLine2->GetLabel());
	m_logLine2->SetLabel(m_logLine3->GetLabel());
	m_logLine3->SetLabel(text);
}

void CDVAPNodeFrame::showLog(const wxString& text)
{
	if (!m_updates)
		return;

	CLogEvent event(text, LOG_EVENT);

	AddPendingEvent(event);
}
