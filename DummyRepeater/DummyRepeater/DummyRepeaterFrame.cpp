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

#include "DummyRepeaterSlowDataEvent.h"
#include "DummyRepeaterMessageEvent.h"
#include "DummyRepeaterHeaderEvent.h"
#include "DummyRepeaterStatusEvent.h"
#include "DummyRepeaterPreferences.h"
#include "DummyRepeaterFrame.h"
#include "DummyRepeaterApp.h"
#include "DStarDefines.h"
#include "Version.h"

#include <wx/gbsizer.h>
#include <wx/aboutdlg.h>

enum {
	Menu_Edit_Preferences = 6000,

	Combo_Your,
	Combo_Rpt1,
	Combo_Rpt2,

	Button_Transmit,
	Button_OneTouchReply,

	Timer_Timeout
};

DEFINE_EVENT_TYPE(MESSAGE_EVENT)
DEFINE_EVENT_TYPE(HEADER_EVENT)
DEFINE_EVENT_TYPE(SLOW_DATA_EVENT)
DEFINE_EVENT_TYPE(STATUS_EVENT)
DEFINE_EVENT_TYPE(ERROR_EVENT)

BEGIN_EVENT_TABLE(CDummyRepeaterFrame, wxFrame)
	EVT_MENU(wxID_EXIT, CDummyRepeaterFrame::onQuit)
	EVT_MENU(Menu_Edit_Preferences, CDummyRepeaterFrame::onPreferences)
	EVT_MENU(wxID_ABOUT, CDummyRepeaterFrame::onAbout)

	EVT_CLOSE(CDummyRepeaterFrame::onClose)

	EVT_TEXT_ENTER(Combo_Your, CDummyRepeaterFrame::onYour)
	EVT_COMBOBOX(Combo_Your, CDummyRepeaterFrame::onYour)

	EVT_TEXT_ENTER(Combo_Rpt1, CDummyRepeaterFrame::onRpt1)
	EVT_COMBOBOX(Combo_Rpt1, CDummyRepeaterFrame::onRpt1)

	EVT_TEXT_ENTER(Combo_Rpt2, CDummyRepeaterFrame::onRpt2)
	EVT_COMBOBOX(Combo_Rpt2, CDummyRepeaterFrame::onRpt2)

	EVT_TOGGLEBUTTON(Button_Transmit, CDummyRepeaterFrame::onTransmit)

	EVT_BUTTON(Button_OneTouchReply, CDummyRepeaterFrame::onOneTouchReply)

	EVT_TIMER(Timer_Timeout, CDummyRepeaterFrame::onTimeout)	

	EVT_CUSTOM(MESSAGE_EVENT,   wxID_ANY, CDummyRepeaterFrame::onMessage)
	EVT_CUSTOM(HEADER_EVENT,    wxID_ANY, CDummyRepeaterFrame::onHeader)
	EVT_CUSTOM(SLOW_DATA_EVENT, wxID_ANY, CDummyRepeaterFrame::onSlowData)
	EVT_CUSTOM(STATUS_EVENT,    wxID_ANY, CDummyRepeaterFrame::onStatus)
	EVT_CUSTOM(ERROR_EVENT,     wxID_ANY, CDummyRepeaterFrame::onError)
END_EVENT_TABLE()

CDummyRepeaterFrame::CDummyRepeaterFrame(const wxString& title, const wxPoint& position) :
wxFrame(NULL, -1, title, position),
m_your(NULL),
m_rpt1(NULL),
m_rpt2(NULL),
m_transmit(NULL),
m_status(NULL),
m_hrdYour(NULL),
m_hrdMy(NULL),
m_hrdRpt1(NULL),
m_hrdRpt2(NULL),
m_hrdFlags(NULL),
m_hrdMessage(NULL),
m_slowData(NULL),
m_status1(NULL),
m_status2(NULL),
m_status3(NULL),
m_status4(NULL),
m_status5(NULL),
m_heard(NULL),
m_timer(this),
m_timeout(0U)
{
	SetMenuBar(createMenuBar());

	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

	wxPanel* panel = new wxPanel(this);

	wxGridBagSizer* panelSizer = new wxGridBagSizer();

	wxStaticText* yourLabel = new wxStaticText(panel, -1, wxT("UR"), wxDefaultPosition, wxSize(LABEL_WIDTH, -1), wxALIGN_RIGHT);
	panelSizer->Add(yourLabel, wxGBPosition(0, 0), wxDefaultSpan, wxALL, BORDER_SIZE);

	wxString call;
	wxSortedArrayString list;

	::wxGetApp().getYourCalls(call, list);
	m_your = new wxComboBox(panel, Combo_Your, wxEmptyString, wxDefaultPosition, wxSize(CONTROL_WIDTH, -1), 0, NULL, wxTE_PROCESS_ENTER);
	m_your->Append(CQCQCQ);
	m_your->Append(list);
	if (call.IsEmpty())
		m_your->SetSelection(0);
	else
		m_your->SetValue(call);
	panelSizer->Add(m_your, wxGBPosition(0, 1), wxDefaultSpan, wxALL, BORDER_SIZE);

	wxStaticText* rpt1Label = new wxStaticText(panel, -1, wxT("RPT1"), wxDefaultPosition, wxSize(LABEL_WIDTH, -1), wxALIGN_RIGHT);
	panelSizer->Add(rpt1Label, wxGBPosition(0, 2), wxDefaultSpan, wxALL, BORDER_SIZE);

	::wxGetApp().getRpt1Calls(call, list);
	m_rpt1 = new wxComboBox(panel, Combo_Rpt1, wxEmptyString, wxDefaultPosition, wxSize(CONTROL_WIDTH, -1), 0, NULL, wxTE_PROCESS_ENTER);
	m_rpt1->Append(UNUSED);
	m_rpt1->Append(list);
	if (call.IsEmpty())
		m_rpt1->SetSelection(0);
	else
		m_rpt1->SetValue(call);
	panelSizer->Add(m_rpt1, wxGBPosition(0, 3), wxDefaultSpan, wxALL, BORDER_SIZE);

	wxStaticText* rpt2Label = new wxStaticText(panel, -1, wxT("RPT2"), wxDefaultPosition, wxSize(LABEL_WIDTH, -1), wxALIGN_RIGHT);
	panelSizer->Add(rpt2Label, wxGBPosition(0, 4), wxDefaultSpan, wxALL, BORDER_SIZE);

	::wxGetApp().getRpt2Calls(call, list);
	m_rpt2 = new wxComboBox(panel, Combo_Rpt2, wxEmptyString, wxDefaultPosition, wxSize(CONTROL_WIDTH, -1), 0, NULL, wxTE_PROCESS_ENTER);
	m_rpt2->Append(UNUSED);
	m_rpt2->Append(list);
	if (call.IsEmpty())
		m_rpt2->SetSelection(0);
	else
		m_rpt2->SetValue(call);
	panelSizer->Add(m_rpt2, wxGBPosition(0, 5), wxDefaultSpan, wxALL, BORDER_SIZE);

	m_transmit = new wxToggleButton(panel, Button_Transmit, _("Transmit"), wxDefaultPosition, wxSize(CONTROL_WIDTH * 2, -1));
	panelSizer->Add(m_transmit, wxGBPosition(1, 0), wxGBSpan(1, 2), wxALL, BORDER_SIZE);

	m_status = new wxStaticText(panel, -1, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE);
	panelSizer->Add(m_status, wxGBPosition(1, 3), wxDefaultSpan, wxALL, BORDER_SIZE);

	wxButton* button = new wxButton(panel, Button_OneTouchReply, _("One-Touch Reply"), wxDefaultPosition, wxSize(CONTROL_WIDTH, -1));
	panelSizer->Add(button, wxGBPosition(1, 5), wxDefaultSpan, wxALL, BORDER_SIZE);

	wxStaticBoxSizer* info1Sizer = new wxStaticBoxSizer(new wxStaticBox(panel, -1, _("Current")), wxVERTICAL);

	wxGridBagSizer* info2Sizer = new wxGridBagSizer(BORDER_SIZE, BORDER_SIZE);

	wxStaticText* hrdYourLabel = new wxStaticText(panel, -1, _("Your:"), wxDefaultPosition, wxSize(LABEL_WIDTH, -1), wxALIGN_RIGHT);
	info2Sizer->Add(hrdYourLabel, wxGBPosition(0, 0), wxDefaultSpan, wxALL, BORDER_SIZE);

	m_hrdYour = new wxStaticText(panel, -1, wxEmptyString, wxDefaultPosition, wxSize(CONTROL_WIDTH, -1));
	info2Sizer->Add(m_hrdYour, wxGBPosition(0, 1), wxDefaultSpan, wxALL, BORDER_SIZE);

	wxStaticText* hrdRpt1Label = new wxStaticText(panel, -1, _("RPT1:"), wxDefaultPosition, wxSize(LABEL_WIDTH, -1), wxALIGN_RIGHT);
	info2Sizer->Add(hrdRpt1Label, wxGBPosition(0, 2), wxDefaultSpan, wxALL, BORDER_SIZE);

	m_hrdRpt1 = new wxStaticText(panel, -1, wxEmptyString, wxDefaultPosition, wxSize(CONTROL_WIDTH, -1));
	info2Sizer->Add(m_hrdRpt1, wxGBPosition(0, 3), wxDefaultSpan, wxALL, BORDER_SIZE);

	wxStaticText* hrdRpt2Label = new wxStaticText(panel, -1, _("RPT2:"), wxDefaultPosition, wxSize(LABEL_WIDTH, -1), wxALIGN_RIGHT);
	info2Sizer->Add(hrdRpt2Label, wxGBPosition(0, 4), wxDefaultSpan, wxALL, BORDER_SIZE);

	m_hrdRpt2 = new wxStaticText(panel, -1, wxEmptyString, wxDefaultPosition, wxSize(CONTROL_WIDTH, -1));
	info2Sizer->Add(m_hrdRpt2, wxGBPosition(0, 5), wxDefaultSpan, wxALL, BORDER_SIZE);

	wxStaticText* hrdMyLabel = new wxStaticText(panel, -1, _("My:"), wxDefaultPosition, wxSize(LABEL_WIDTH, -1), wxALIGN_RIGHT);
	info2Sizer->Add(hrdMyLabel, wxGBPosition(1, 0), wxDefaultSpan, wxALL, BORDER_SIZE);

	m_hrdMy = new wxStaticText(panel, -1, wxEmptyString, wxDefaultPosition, wxSize(CONTROL_WIDTH, -1));
	info2Sizer->Add(m_hrdMy, wxGBPosition(1, 1), wxDefaultSpan, wxALL, BORDER_SIZE);

	wxStaticText* hrdFlagsLabel = new wxStaticText(panel, -1, _("Flags:"), wxDefaultPosition, wxSize(LABEL_WIDTH, -1), wxALIGN_RIGHT);
	info2Sizer->Add(hrdFlagsLabel, wxGBPosition(1, 2), wxDefaultSpan, wxALL, BORDER_SIZE);

	m_hrdFlags = new wxStaticText(panel, -1, wxEmptyString, wxDefaultPosition, wxSize(LABEL_WIDTH * 3U, -1));
	info2Sizer->Add(m_hrdFlags, wxGBPosition(1, 3), wxGBSpan(1, 3), wxALL, BORDER_SIZE);

	wxStaticText* hrdMessageLabel = new wxStaticText(panel, -1, _("Message:"), wxDefaultPosition, wxSize(LABEL_WIDTH, -1), wxALIGN_RIGHT);
	info2Sizer->Add(hrdMessageLabel, wxGBPosition(2, 0), wxDefaultSpan, wxALL, BORDER_SIZE);

	m_hrdMessage = new wxStaticText(panel, -1, wxEmptyString, wxDefaultPosition, wxSize(LABEL_WIDTH * 3U, -1));
	info2Sizer->Add(m_hrdMessage, wxGBPosition(2, 1), wxGBSpan(1, 3), wxALL, BORDER_SIZE);

	info1Sizer->Add(info2Sizer);

	panelSizer->Add(info1Sizer, wxGBPosition(2, 0), wxGBSpan(5, 6), wxALL | wxEXPAND, BORDER_SIZE);

	wxStaticBoxSizer* slowData1Sizer = new wxStaticBoxSizer(new wxStaticBox(panel, -1, _("Status")), wxVERTICAL);

	wxGridBagSizer* slowData2Sizer = new wxGridBagSizer(BORDER_SIZE, BORDER_SIZE);

	wxStaticText* slowDataLabel = new wxStaticText(panel, -1, _("Message:"), wxDefaultPosition, wxSize(LABEL_WIDTH, -1), wxALIGN_RIGHT);
	slowData2Sizer->Add(slowDataLabel, wxGBPosition(0, 0), wxDefaultSpan, wxALL, BORDER_SIZE);

	m_slowData = new wxStaticText(panel, -1, wxEmptyString, wxDefaultPosition, wxSize(CONTROL_WIDTH, -1));
	slowData2Sizer->Add(m_slowData, wxGBPosition(0, 1), wxDefaultSpan, wxALL, BORDER_SIZE);

	wxStaticText* status1Label = new wxStaticText(panel, -1, _("Status 1:"), wxDefaultPosition, wxSize(LABEL_WIDTH, -1), wxALIGN_RIGHT);
	slowData2Sizer->Add(status1Label, wxGBPosition(0, 2), wxDefaultSpan, wxALL, BORDER_SIZE);

	m_status1 = new wxStaticText(panel, -1, wxEmptyString, wxDefaultPosition, wxSize(CONTROL_WIDTH, -1));
	slowData2Sizer->Add(m_status1, wxGBPosition(0, 3), wxDefaultSpan, wxALL, BORDER_SIZE);

	wxStaticText* status2Label = new wxStaticText(panel, -1, _("Status 2:"), wxDefaultPosition, wxSize(LABEL_WIDTH, -1), wxALIGN_RIGHT);
	slowData2Sizer->Add(status2Label, wxGBPosition(0, 4), wxDefaultSpan, wxALL, BORDER_SIZE);

	m_status2 = new wxStaticText(panel, -1, wxEmptyString, wxDefaultPosition, wxSize(CONTROL_WIDTH, -1));
	slowData2Sizer->Add(m_status2, wxGBPosition(0, 5), wxDefaultSpan, wxALL, BORDER_SIZE);

	wxStaticText* status3Label = new wxStaticText(panel, -1, _("Status 3:"), wxDefaultPosition, wxSize(LABEL_WIDTH, -1), wxALIGN_RIGHT);
	slowData2Sizer->Add(status3Label, wxGBPosition(1, 0), wxDefaultSpan, wxALL, BORDER_SIZE);

	m_status3 = new wxStaticText(panel, -1, wxEmptyString, wxDefaultPosition, wxSize(CONTROL_WIDTH, -1));
	slowData2Sizer->Add(m_status3, wxGBPosition(1, 1), wxDefaultSpan, wxALL, BORDER_SIZE);

	wxStaticText* status4Label = new wxStaticText(panel, -1, _("Status 4:"), wxDefaultPosition, wxSize(LABEL_WIDTH, -1), wxALIGN_RIGHT);
	slowData2Sizer->Add(status4Label, wxGBPosition(1, 2), wxDefaultSpan, wxALL, BORDER_SIZE);

	m_status4 = new wxStaticText(panel, -1, wxEmptyString, wxDefaultPosition, wxSize(CONTROL_WIDTH, -1));
	slowData2Sizer->Add(m_status4, wxGBPosition(1, 3), wxDefaultSpan, wxALL, BORDER_SIZE);

	wxStaticText* status5Label = new wxStaticText(panel, -1, _("Status 5:"), wxDefaultPosition, wxSize(LABEL_WIDTH, -1), wxALIGN_RIGHT);
	slowData2Sizer->Add(status5Label, wxGBPosition(1, 4), wxDefaultSpan, wxALL, BORDER_SIZE);

	m_status5 = new wxStaticText(panel, -1, wxEmptyString, wxDefaultPosition, wxSize(CONTROL_WIDTH, -1));
	slowData2Sizer->Add(m_status5, wxGBPosition(1, 5), wxDefaultSpan, wxALL, BORDER_SIZE);

	slowData1Sizer->Add(slowData2Sizer);

	panelSizer->Add(slowData1Sizer, wxGBPosition(7, 0), wxGBSpan(2, 6), wxALL | wxEXPAND, BORDER_SIZE);

	m_heard = new wxListCtrl(panel, -1, wxDefaultPosition, wxSize(HEARD_WIDTH, HEARD_HEIGHT), wxLC_REPORT | wxLC_SINGLE_SEL);
	m_heard->InsertColumn(0L, _("Date/Time"));
	m_heard->SetColumnWidth(0L, DATETIME_WIDTH);
	m_heard->InsertColumn(1L, _("Your"));
	m_heard->SetColumnWidth(1L, CALLSIGN_WIDTH);
	m_heard->InsertColumn(2L, _("My"));
	m_heard->SetColumnWidth(2L, MYCALLSIGN_WIDTH);
	m_heard->InsertColumn(3L, _("RPT1"));
	m_heard->SetColumnWidth(3L, CALLSIGN_WIDTH);
	m_heard->InsertColumn(4L, _("RPT2"));
	m_heard->SetColumnWidth(4L, CALLSIGN_WIDTH);
	m_heard->InsertColumn(5L, _("Message"));
	m_heard->SetColumnWidth(5L, MESSAGE_WIDTH);
	panelSizer->Add(m_heard, wxGBPosition(9, 0), wxGBSpan(11, 6), wxALL | wxEXPAND, BORDER_SIZE);

	panel->SetSizer(panelSizer);
	panelSizer->SetSizeHints(panel);

	mainSizer->Add(panel);

	SetSizer(mainSizer);
	mainSizer->SetSizeHints(this);

	::wxGetApp().getTimeout(m_timeout);
}

CDummyRepeaterFrame::~CDummyRepeaterFrame()
{
}

wxMenuBar* CDummyRepeaterFrame::createMenuBar()
{
	wxMenu* fileMenu = new wxMenu();
	fileMenu->Append(wxID_EXIT, _("Exit"));

	wxMenu* editMenu = new wxMenu();
	editMenu->Append(Menu_Edit_Preferences, _("Preferences..."));

	wxMenu* helpMenu = new wxMenu();
	helpMenu->Append(wxID_ABOUT, _("About Dummy Repeater"));

	wxMenuBar* menuBar = new wxMenuBar();
	menuBar->Append(fileMenu, _("File"));
	menuBar->Append(editMenu, _("Edit"));
	menuBar->Append(helpMenu, _("Help"));

	return menuBar;
}

void CDummyRepeaterFrame::showMessage(CMessageData* message)
{
	wxASSERT(message != NULL);

	CDummyRepeaterMessageEvent event(message, MESSAGE_EVENT);

	AddPendingEvent(event);
}

void CDummyRepeaterFrame::showHeader(CHeaderData* header)
{
	CDummyRepeaterHeaderEvent event(header, HEADER_EVENT);

	AddPendingEvent(event);
}

void CDummyRepeaterFrame::showSlowData(const wxString& text)
{
	CDummyRepeaterSlowDataEvent event(text, SLOW_DATA_EVENT);

	AddPendingEvent(event);
}

void CDummyRepeaterFrame::showStatus1(const wxString& text)
{
	CDummyRepeaterStatusEvent event(text, 1U, STATUS_EVENT);

	AddPendingEvent(event);
}

void CDummyRepeaterFrame::showStatus2(const wxString& text)
{
	CDummyRepeaterStatusEvent event(text, 2U, STATUS_EVENT);

	AddPendingEvent(event);
}

void CDummyRepeaterFrame::showStatus3(const wxString& text)
{
	CDummyRepeaterStatusEvent event(text, 3U, STATUS_EVENT);

	AddPendingEvent(event);
}

void CDummyRepeaterFrame::showStatus4(const wxString& text)
{
	CDummyRepeaterStatusEvent event(text, 4U, STATUS_EVENT);

	AddPendingEvent(event);
}

void CDummyRepeaterFrame::showStatus5(const wxString& text)
{
	CDummyRepeaterStatusEvent event(text, 5U, STATUS_EVENT);

	AddPendingEvent(event);
}

void CDummyRepeaterFrame::error(const wxString& error)
{
	CDummyRepeaterErrorEvent event(error, ERROR_EVENT);

	AddPendingEvent(event);
}

void CDummyRepeaterFrame::onQuit(wxCommandEvent&)
{
	Close(false);
}

void CDummyRepeaterFrame::onClose(wxCloseEvent& event)
{
	if (!event.CanVeto()) {
		saveCalls();
		Destroy();
		return;
	}

	int reply = ::wxMessageBox(_("Do you want to exit Dummy Repeater"), _("Exit Dummy Repeater"), wxOK | wxCANCEL | wxICON_QUESTION);
	switch (reply) {
		case wxOK:
			saveCalls();
			savePosition();
			Destroy();
			break;
		case wxCANCEL:
			event.Veto();
			break;
	}
}

void CDummyRepeaterFrame::onPreferences(wxCommandEvent&)
{
	wxString callsign1, callsign2;
	::wxGetApp().getCallsign(callsign1, callsign2);

	wxString readDevice, writeDevice;
	::wxGetApp().getSoundcard(readDevice, writeDevice);

	DONGLE_TYPE dongleType;
	wxString dongleDevice, dongleAddress;
	unsigned int donglePort;
	::wxGetApp().getDongle(dongleType, dongleDevice, dongleAddress, donglePort);

	wxString gatewayAddress, localAddress;
	unsigned int gatewayPort, localPort;
	::wxGetApp().getNetwork(gatewayAddress, gatewayPort, localAddress, localPort);

	wxString type;
	unsigned int config;
	bool pttInvert, squelchInvert;
	::wxGetApp().getController(type, config, pttInvert, squelchInvert);

	unsigned int timeout;
	::wxGetApp().getTimeout(timeout);

	wxString message;
	::wxGetApp().getMessage(message);

	bool bleep;
	::wxGetApp().getBleep(bleep);

	CDummyRepeaterPreferences dialog1(this, -1, callsign1, callsign2, readDevice, writeDevice, dongleType, dongleDevice,
		dongleAddress, donglePort, gatewayAddress, gatewayPort, localAddress, localPort, type, config, pttInvert,
		squelchInvert, timeout, message, bleep);
	if (dialog1.ShowModal() != wxID_OK)
		return;

	callsign1       = dialog1.getCallsign1();
	callsign2       = dialog1.getCallsign2();

	readDevice      = dialog1.getSoundcardReadDevice();
	writeDevice     = dialog1.getSoundcardWriteDevice();

	gatewayAddress  = dialog1.getGatewayAddress();
	gatewayPort     = dialog1.getGatewayPort();
	localAddress    = dialog1.getLocalAddress();
	localPort       = dialog1.getLocalPort();

	type            = dialog1.getType();
	config          = dialog1.getConfig();
	pttInvert       = dialog1.getPTTInvert();
	squelchInvert   = dialog1.getSquelchInvert();

	dongleType      = dialog1.getDongleType();
	dongleDevice    = dialog1.getDongleDevice();
	dongleAddress   = dialog1.getDongleAddress();
	donglePort      = dialog1.getDonglePort();

	timeout         = dialog1.getTimeout();

	message         = dialog1.getMessage();

	bleep           = dialog1.getBleep();

	::wxGetApp().setCallsign(callsign1, callsign2);
	::wxGetApp().setSoundcard(readDevice, writeDevice);
	::wxGetApp().setDongle(dongleType, dongleDevice, dongleAddress, donglePort);
	::wxGetApp().setNetwork(gatewayAddress, gatewayPort, localAddress, localPort);
	::wxGetApp().setController(type, config, pttInvert, squelchInvert);
	::wxGetApp().setTimeout(timeout);
	::wxGetApp().setMessage(message);
	::wxGetApp().setBleep(bleep);

	wxMessageDialog dialog2(this, _("The changes made will not take effect\nuntil the application is restarted"), _("Dummy Repeater Information"), wxICON_INFORMATION);
	dialog2.ShowModal();
}

void CDummyRepeaterFrame::onAbout(wxCommandEvent&)
{
	wxAboutDialogInfo info;
	info.AddDeveloper(wxT("Jonathan Naylor, G4KLX"));
	info.SetCopyright(wxT("(C) 2010-2014 using GPL v2 or later"));
	info.SetName(APPLICATION_NAME);
	info.SetVersion(VERSION);
	info.SetDescription(_("This program allows a computer with an AMBE Dongle\nto behave as a D-Star Repeater."));

	::wxAboutBox(info);
}

void CDummyRepeaterFrame::onYour(wxCommandEvent&)
{
	wxString your = m_your->GetValue();

	processYour(your);
}

bool CDummyRepeaterFrame::processYour(wxString& your)
{
	your.MakeUpper();

	// Check for the right length
	size_t len = your.Length();
	if (len > LONG_CALLSIGN_LENGTH) {
		error(_("The YOUR callsign is too long"));
		return false;
	}

	// Check for invalid characters
	size_t pos = your.find_first_not_of(wxT("ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789/ "));
	if (pos != wxString::npos) {
		error(_("Invalid character in the YOUR callsign"));
		return false;
	}

	// Add the Your callsign to the list
	if (!your.IsSameAs(CQCQCQ)) {
		int index = m_your->FindString(your);
		if (index == wxNOT_FOUND)
			m_your->Insert(your, 0U);	// Insert at the top
	}

	// Replace value with possibly modified versions
	m_your->SetValue(your);

	::wxGetApp().setYour(your);

	return true;
}

void CDummyRepeaterFrame::onRpt1(wxCommandEvent&)
{
	wxString rpt1 = m_rpt1->GetValue();

	processRpt1(rpt1);
}

bool CDummyRepeaterFrame::processRpt1(wxString& rpt1)
{
	if (!rpt1.IsSameAs(UNUSED)) {
		rpt1.MakeUpper();

		// Check for the right length
		size_t len = rpt1.Length();
		if (len > LONG_CALLSIGN_LENGTH) {
			error(_("The RPT1 callsign is too long"));
			return false;
		}

		// Check for invalid characters
		size_t pos = rpt1.find_first_not_of(wxT("ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789/ "));
		if (pos != wxString::npos) {
			error(_("Invalid character in the RPT1 callsign"));
			return false;
		}

		// Add the RPT1 callsign to the list
		int index = m_rpt1->FindString(rpt1);
		if (index == wxNOT_FOUND)
			m_rpt1->Insert(rpt1, 0U);	// Insert at the top

		// Replace value with possibly modified versions
		m_rpt1->SetValue(rpt1);
	}

	::wxGetApp().setRpt1(rpt1);

	return true;
}

void CDummyRepeaterFrame::onRpt2(wxCommandEvent&)
{
	wxString rpt2 = m_rpt2->GetValue();

	processRpt2(rpt2);
}

bool CDummyRepeaterFrame::processRpt2(wxString& rpt2)
{
	if (!rpt2.IsSameAs(UNUSED)) {
		rpt2.MakeUpper();

		// Check for the right length
		size_t len = rpt2.Length();
		if (len > LONG_CALLSIGN_LENGTH) {
			error(_("The RPT2 callsign is too long"));
			return false;
		}

		// Check for invalid characters
		size_t pos = rpt2.find_first_not_of(wxT("ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789/ "));
		if (pos != wxString::npos) {
			error(_("Invalid character in the RPT2 callsign"));
			return false;
		}

		// Add the RPT2 callsign to the list
		int index = m_rpt2->FindString(rpt2);
		if (index == wxNOT_FOUND)
			m_rpt2->Insert(rpt2, 0U);	// Insert at the top

		// Replace value with possibly modified versions
		m_rpt2->SetValue(rpt2);
	}

	::wxGetApp().setRpt2(rpt2);

	return true;
}

void CDummyRepeaterFrame::onTransmit(wxCommandEvent& event)
{
	bool state = event.IsChecked();

	if (state) {
		wxString your = m_your->GetValue();
		bool ret = processYour(your);
		if (!ret) {
			m_transmit->SetValue(false);
			return;
		}

		wxString rpt1 = m_rpt1->GetValue();
		ret = processRpt1(rpt1);
		if (!ret) {
			m_transmit->SetValue(false);
			return;
		}

		wxString rpt2 = m_rpt2->GetValue();
		ret = processRpt2(rpt2);
		if (!ret) {
			m_transmit->SetValue(false);
			return;
		}

		ret = ::wxGetApp().setTransmit(true);
		if (ret) {
			m_status->SetBackgroundColour(*wxRED);
			m_status->SetLabel(_("TRANSMIT"));
			if (m_timeout > 0U)
				m_timer.Start(m_timeout * 1000, wxTIMER_ONE_SHOT);
		} else {
			// Transmit failed
			m_transmit->SetValue(false);
			wxLogError(wxT("Unable to transmit"));
			error(_("Unable to transmit"));
		}
	} else {
		m_status->SetBackgroundColour(*wxLIGHT_GREY);
		m_status->SetLabel(wxEmptyString);
		m_timer.Stop();

		::wxGetApp().setTransmit(false);
	}
}

void CDummyRepeaterFrame::onOneTouchReply(wxCommandEvent&)
{
	wxString my12 = m_hrdMy->GetLabel();

	if (my12.IsEmpty() || my12.Left(LONG_CALLSIGN_LENGTH).IsSameAs(wxT("        ")))
		return;

	wxString my1  = my12.Left(LONG_CALLSIGN_LENGTH);
	wxString rpt1 = m_hrdRpt2->GetLabel();
	wxString rpt2 = m_hrdRpt1->GetLabel();

	if (rpt1.IsSameAs(DIRECT))
		rpt1 = UNUSED;
	if (rpt2.IsSameAs(DIRECT))
		rpt2 = UNUSED;

	// Add the YOUR callsign to the list and select it
	int index = m_your->FindString(my1);
	if (index == wxNOT_FOUND)
		m_your->Insert(my1, 0U);	// Insert at the top

	m_your->SetValue(my1);

	// Add the RPT1 callsign to the list and select it
	index = m_rpt1->FindString(rpt1);
	if (index == wxNOT_FOUND)
		m_rpt1->Insert(rpt1, 0U);	// Insert at the top

	m_rpt1->SetValue(rpt1);

	// Add the RPT2 callsign to the list and select it
	index = m_rpt2->FindString(rpt2);
	if (index == wxNOT_FOUND)
		m_rpt2->Insert(rpt2, 0U);	// Insert at the top

	m_rpt2->SetValue(rpt2);
}

void CDummyRepeaterFrame::onTimeout(wxTimerEvent&)
{
	m_status->SetBackgroundColour(*wxLIGHT_GREY);
	m_status->SetLabel(wxEmptyString);

	::wxGetApp().setTransmit(false);

	error(_("Timeout!"));
}

void CDummyRepeaterFrame::onMessage(wxEvent& event)
{
	CDummyRepeaterMessageEvent& msgEvent = dynamic_cast<CDummyRepeaterMessageEvent&>(event);

	CMessageData* message = msgEvent.getMessageData();
	wxASSERT(message != NULL);

	wxString msg = message->getText();

	m_hrdMessage->SetLabel(msg);
	m_heard->SetItem(0L, 5, msg);

	delete message;
}

void CDummyRepeaterFrame::onHeader(wxEvent& event)
{
	CDummyRepeaterHeaderEvent& hdrEvent = dynamic_cast<CDummyRepeaterHeaderEvent&>(event);

	CHeaderData* header = hdrEvent.getHeaderData();

	if (header != NULL) {
		wxDateTime dateTime = header->getTime();
		wxString hrdDateTime = dateTime.FormatISODate() + wxT(" ") + dateTime.FormatISOTime();
		m_heard->InsertItem(0L, hrdDateTime);

		wxString urCall = header->getYourCall();
		m_hrdYour->SetLabel(urCall);
		m_heard->SetItem(0L, 1, urCall);

		wxString myCall = header->getMyCall1();

		wxString myCall2 = header->getMyCall2();
		if (!myCall2.IsSameAs(wxT("    "))) {
			myCall.Append(wxT("/"));
			myCall.Append(myCall2);
		}

		m_hrdMy->SetLabel(myCall);
		m_heard->SetItem(0L, 2, myCall);

		wxString rpt1Call = header->getRptCall1();
		m_hrdRpt1->SetLabel(rpt1Call);
		m_heard->SetItem(0L, 3, rpt1Call);

		wxString rpt2Call = header->getRptCall2();
		m_hrdRpt2->SetLabel(rpt2Call);
		m_heard->SetItem(0L, 4, rpt2Call);

		wxString flags;
		flags.Printf(wxT("%02X %02X %02X"), header->getFlag1(), header->getFlag2(), header->getFlag3());
		m_hrdFlags->SetLabel(flags);

		m_hrdMessage->SetLabel(wxEmptyString);

		delete header;
	} else {
		m_hrdYour->SetLabel(wxEmptyString);
		m_hrdMy->SetLabel(wxEmptyString);
		m_hrdRpt1->SetLabel(wxEmptyString);
		m_hrdRpt2->SetLabel(wxEmptyString);
		m_hrdFlags->SetLabel(wxEmptyString);
		m_hrdMessage->SetLabel(wxEmptyString);
	}
}

void CDummyRepeaterFrame::onSlowData(wxEvent& event)
{
	CDummyRepeaterSlowDataEvent& hdrEvent = dynamic_cast<CDummyRepeaterSlowDataEvent&>(event);

	wxString text = hdrEvent.getSlowData();

	m_slowData->SetLabel(text);
}

void CDummyRepeaterFrame::onStatus(wxEvent& event)
{
	CDummyRepeaterStatusEvent& hdrEvent = dynamic_cast<CDummyRepeaterStatusEvent&>(event);

	wxString text = hdrEvent.getText();
	unsigned int n = hdrEvent.getN();

	switch (n) {
		case 1U:
			m_status1->SetLabel(text);
			break;
		case 2U:
			m_status2->SetLabel(text);
			break;
		case 3U:
			m_status3->SetLabel(text);
			break;
		case 4U:
			m_status4->SetLabel(text);
			break;
		case 5U:
			m_status5->SetLabel(text);
			break;
		default:
			break;
	}
}

void CDummyRepeaterFrame::onError(wxEvent& event)
{
	CDummyRepeaterErrorEvent& errEvent = dynamic_cast<CDummyRepeaterErrorEvent&>(event);

	wxString text = errEvent.getText();

	wxMessageDialog dialog(this, text, _("Dummy Repeater Error"), wxICON_ERROR);
	dialog.ShowModal();
}

void CDummyRepeaterFrame::saveCalls() const
{
	wxString      call = m_your->GetValue();
	wxArrayString list = m_your->GetStrings();
	int index = list.Index(CQCQCQ);		// Remove the fixed value
	if (index != wxNOT_FOUND)
		list.RemoveAt(index);
	::wxGetApp().setYourCalls(call, list);

	call = m_rpt1->GetValue();
	list = m_rpt1->GetStrings();
	index = list.Index(UNUSED);		// Remove the fixed value
	if (index != wxNOT_FOUND)
		list.RemoveAt(index);
	::wxGetApp().setRpt1Calls(call, list);

	call = m_rpt2->GetValue();
	list = m_rpt2->GetStrings();
	index = list.Index(UNUSED);		// Remove the fixed value
	if (index != wxNOT_FOUND)
		list.RemoveAt(index);
	::wxGetApp().setRpt2Calls(call, list);
}

void CDummyRepeaterFrame::savePosition() const
{
	int x, y;
	GetPosition(&x, &y);

	if (x >= 0 && y >= 0)
		::wxGetApp().setPosition(x, y);
}
