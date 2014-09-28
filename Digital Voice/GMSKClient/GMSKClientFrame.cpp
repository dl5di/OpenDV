/*
 *   Copyright (C) 2010,2012 by Jonathan Naylor G4KLX
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

#include "GMSKClientPreferences.h"
#include "GMSKClientFrame.h"
#include "GMSKClientApp.h"
#include "MessageEvent.h"
#include "DStarDefines.h"
#include "HeaderEvent.h"
#include "BusyEvent.h"
#include "Version.h"

#if defined(__WXGTK__) || defined(__WXMAC__)
#include "GMSKClient.xpm"
#endif

#include <wx/gbsizer.h>
#include <wx/aboutdlg.h>

enum {
	Menu_File_Recording = 6000,

	Menu_Edit_Preferences,

	Combo_Your,
	Combo_Rpt1,
	Combo_Rpt2,

	Button_Transmit,
	Button_OneTouchReply
};

DEFINE_EVENT_TYPE(MESSAGE_EVENT)
DEFINE_EVENT_TYPE(HEADER_EVENT)
DEFINE_EVENT_TYPE(ERROR_EVENT)
DEFINE_EVENT_TYPE(BUSY_EVENT)

BEGIN_EVENT_TABLE(CGMSKClientFrame, wxFrame)
	EVT_MENU(Menu_File_Recording, CGMSKClientFrame::onRecording)
	EVT_MENU(wxID_EXIT, CGMSKClientFrame::onQuit)
	EVT_MENU(Menu_Edit_Preferences, CGMSKClientFrame::onPreferences)
	EVT_MENU(wxID_ABOUT, CGMSKClientFrame::onAbout)

	EVT_CLOSE(CGMSKClientFrame::onClose)

	EVT_TEXT_ENTER(Combo_Your, CGMSKClientFrame::onYour)
	EVT_COMBOBOX(Combo_Your, CGMSKClientFrame::onYour)

	EVT_TEXT_ENTER(Combo_Rpt1, CGMSKClientFrame::onRpt1)
	EVT_COMBOBOX(Combo_Rpt1, CGMSKClientFrame::onRpt1)

	EVT_TEXT_ENTER(Combo_Rpt2, CGMSKClientFrame::onRpt2)
	EVT_COMBOBOX(Combo_Rpt2, CGMSKClientFrame::onRpt2)

	EVT_TOGGLEBUTTON(Button_Transmit, CGMSKClientFrame::onTransmit)

	EVT_BUTTON(Button_OneTouchReply, CGMSKClientFrame::onOneTouchReply)

	EVT_CUSTOM(MESSAGE_EVENT, wxID_ANY, CGMSKClientFrame::onMessage)
	EVT_CUSTOM(HEADER_EVENT,  wxID_ANY, CGMSKClientFrame::onHeader)
	EVT_CUSTOM(ERROR_EVENT,   wxID_ANY, CGMSKClientFrame::onError)
	EVT_CUSTOM(BUSY_EVENT,    wxID_ANY, CGMSKClientFrame::onBusy)
END_EVENT_TABLE()

CGMSKClientFrame::CGMSKClientFrame(const wxString& title) :
wxFrame(NULL, -1, title),
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
m_heard(NULL)
{
	SetIcon(wxICON(GMSKClient));

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

	wxStaticBoxSizer* info1Sizer = new wxStaticBoxSizer(new wxStaticBox(panel, -1, _("Current/Last Heard"), wxDefaultPosition, wxSize(INFO_WIDTH, INFO_HEIGHT)), wxVERTICAL);

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

	m_hrdFlags = new wxStaticText(panel, -1, wxEmptyString, wxDefaultPosition, wxSize(LABEL_WIDTH * 3U, -1), wxALIGN_RIGHT);
	info2Sizer->Add(m_hrdFlags, wxGBPosition(1, 3), wxGBSpan(1, 3), wxALL, BORDER_SIZE);

	wxStaticText* hrdMessageLabel = new wxStaticText(panel, -1, _("Message:"), wxDefaultPosition, wxSize(LABEL_WIDTH, -1), wxALIGN_RIGHT);
	info2Sizer->Add(hrdMessageLabel, wxGBPosition(2, 0), wxDefaultSpan, wxALL, BORDER_SIZE);

	m_hrdMessage = new wxStaticText(panel, -1, wxEmptyString, wxDefaultPosition, wxSize(LABEL_WIDTH * 3U, -1), wxALIGN_RIGHT);
	info2Sizer->Add(m_hrdMessage, wxGBPosition(2, 1), wxGBSpan(1, 3), wxALL, BORDER_SIZE);

	info1Sizer->Add(info2Sizer);

	panelSizer->Add(info1Sizer, wxGBPosition(2, 0), wxGBSpan(5, 6), wxALL | wxEXPAND, BORDER_SIZE);

	m_heard = new wxListCtrl(panel, -1, wxDefaultPosition, wxSize(HEARD_WIDTH, HEARD_HEIGHT), wxLC_REPORT | wxLC_SINGLE_SEL);
	m_heard->InsertColumn(0L, _("Date/Time"));
	m_heard->SetColumnWidth(0L, DATETIME_WIDTH);
	m_heard->InsertColumn(1L, _("Your"));
	m_heard->SetColumnWidth(1L, CALLSIGN_WIDTH);
	m_heard->InsertColumn(2L, _("My"));
	m_heard->SetColumnWidth(2L, CALLSIGN_WIDTH);
	m_heard->InsertColumn(3L, _("RPT1"));
	m_heard->SetColumnWidth(3L, CALLSIGN_WIDTH);
	m_heard->InsertColumn(4L, _("RPT2"));
	m_heard->SetColumnWidth(4L, CALLSIGN_WIDTH);
	m_heard->InsertColumn(5L, _("Message"));
	m_heard->SetColumnWidth(5L, MESSAGE_WIDTH);
	panelSizer->Add(m_heard, wxGBPosition(7, 0), wxGBSpan(11, 6), wxALL | wxEXPAND, BORDER_SIZE);

	panel->SetSizer(panelSizer);
	panelSizer->SetSizeHints(panel);

	mainSizer->Add(panel);

	SetSizer(mainSizer);
	mainSizer->SetSizeHints(this);
}

CGMSKClientFrame::~CGMSKClientFrame()
{
}

wxMenuBar* CGMSKClientFrame::createMenuBar()
{
	wxMenu* fileMenu = new wxMenu();
	fileMenu->AppendCheckItem(Menu_File_Recording, _("&Record data\tCtrl+R"));
	fileMenu->AppendSeparator();
	fileMenu->Append(wxID_EXIT, _("Exit"));

	wxMenu* editMenu = new wxMenu();
	editMenu->Append(Menu_Edit_Preferences, _("Preferences..."));

	wxMenu* helpMenu = new wxMenu();
	helpMenu->Append(wxID_ABOUT, _("About GMSK Client"));

	wxMenuBar* menuBar = new wxMenuBar();
	menuBar->Append(fileMenu, _("File"));
	menuBar->Append(editMenu, _("Edit"));
	menuBar->Append(helpMenu, _("Help"));

	return menuBar;
}

void CGMSKClientFrame::showMessage(CMessageData* message)
{
	wxASSERT(message != NULL);

	CMessageEvent event(message, MESSAGE_EVENT);

	AddPendingEvent(event);
}

void CGMSKClientFrame::showHeader(CHeaderData* header)
{
	wxASSERT(header != NULL);

	CHeaderEvent event(header, HEADER_EVENT);

	AddPendingEvent(event);
}

void CGMSKClientFrame::showBusy(CBusyData* busy)
{
	wxASSERT(busy != NULL);

	CBusyEvent event(busy, BUSY_EVENT);

	AddPendingEvent(event);
}

void CGMSKClientFrame::error(const wxString& error)
{
	CErrorEvent event(error, ERROR_EVENT);

	AddPendingEvent(event);
}

void CGMSKClientFrame::onRecording(wxCommandEvent& event)
{
	bool recording = event.IsChecked();

	::wxGetApp().setRecording(recording);
}

void CGMSKClientFrame::onQuit(wxCommandEvent& event)
{
	Close(false);
}

void CGMSKClientFrame::onClose(wxCloseEvent& event)
{
	if (!event.CanVeto()) {
		saveCalls();
		Destroy();
		return;
	}

	int reply = ::wxMessageBox(_("Do you want to exit GMSK Client"), _("Exit GMSK Client"), wxOK | wxCANCEL | wxICON_QUESTION);
	switch (reply) {
		case wxOK:
			saveCalls();
			Destroy();
			break;
		case wxCANCEL:
			event.Veto();
			break;
	}
}

void CGMSKClientFrame::onPreferences(wxCommandEvent& event)
{
	wxString oldCallsign1, oldCallsign2;
	::wxGetApp().getCallsign(oldCallsign1, oldCallsign2);

	wxString oldReadDevice, oldWriteDevice;
	::wxGetApp().getSoundcard(oldReadDevice, oldWriteDevice);

	GMSK_MODEM_TYPE oldModemType;
	unsigned int oldModemAddress;
	::wxGetApp().getModem(oldModemType, oldModemAddress);

	wxString oldDVDDevice;
	::wxGetApp().getDVDongle(oldDVDDevice);

	wxString oldMessage;
	::wxGetApp().getMessage(oldMessage);

	bool oldBleep;
	::wxGetApp().getBleep(oldBleep);

	CGMSKClientPreferences dialog(this, -1, oldCallsign1, oldCallsign2, oldReadDevice, oldWriteDevice, oldModemType,
								   oldModemAddress, oldDVDDevice, oldMessage, oldBleep);
	if (dialog.ShowModal() != wxID_OK)
		return;

	wxString newCallsign1        = dialog.getCallsign1();
	wxString newCallsign2        = dialog.getCallsign2();
	wxString newReadDevice       = dialog.getSoundcardReadDevice();
	wxString newWriteDevice      = dialog.getSoundcardWriteDevice();
	GMSK_MODEM_TYPE newModemType = dialog.getModemType();
	unsigned int newModemAddress = dialog.getModemAddress();
	wxString newDVDDevice        = dialog.getDVDDevice();
	wxString newMessage          = dialog.getMessage();
	bool     newBleep            = dialog.getBleep();

	bool changed = false;

	if (!newCallsign1.IsSameAs(oldCallsign1) || !newCallsign2.IsSameAs(oldCallsign2))
		::wxGetApp().setCallsign(newCallsign1, newCallsign2);

	if (!newReadDevice.IsSameAs(oldReadDevice) || !newWriteDevice.IsSameAs(oldWriteDevice)) {
		::wxGetApp().setSoundcard(newReadDevice, newWriteDevice);
		changed = true;
	}

	if (newModemType != oldModemType || newModemAddress != oldModemAddress) {
		::wxGetApp().setModem(newModemType, newModemAddress);
		changed = true;
	}

	if (!newDVDDevice.IsSameAs(oldDVDDevice)) {
		::wxGetApp().setDVDongle(newDVDDevice);
		changed = true;
	}

	if (!newMessage.IsSameAs(oldMessage))
		::wxGetApp().setMessage(newMessage);

	if (newBleep != oldBleep)
		::wxGetApp().setBleep(newBleep);

	if (changed) {
		wxMessageDialog dialog(this, _("The changes made will not take effect\nuntil the application is restarted"), _("GMSK Client Information"), wxICON_INFORMATION);
		dialog.ShowModal();
	}
}

void CGMSKClientFrame::onAbout(wxCommandEvent& event)
{
	wxAboutDialogInfo info;
	info.AddDeveloper(wxT("Jonathan Naylor, G4KLX"));
	info.SetName(APPLICATION_NAME);
	info.SetVersion(VERSION);
	info.SetDescription(_("This program allows a computer with a DV Dongle\na GMSK Modem and a 9600Bd capable radio to work D-Star stations\nwith or without a repeater."));

	::wxAboutBox(info);
}

void CGMSKClientFrame::onYour(wxCommandEvent& event)
{
	wxString your = m_your->GetValue();

	processYour(your);
}

bool CGMSKClientFrame::processYour(wxString& your)
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

void CGMSKClientFrame::onRpt1(wxCommandEvent& event)
{
	wxString rpt1 = m_rpt1->GetValue();

	processRpt1(rpt1);
}

bool CGMSKClientFrame::processRpt1(wxString& rpt1)
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

void CGMSKClientFrame::onRpt2(wxCommandEvent& event)
{
	wxString rpt2 = m_rpt2->GetValue();

	processRpt2(rpt2);
}

bool CGMSKClientFrame::processRpt2(wxString& rpt2)
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

void CGMSKClientFrame::onTransmit(wxCommandEvent& event)
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
		} else {
			// Transmit failed
			m_transmit->SetValue(false);
			wxLogError(wxT("Unable to transmit"));
			error(_("Unable to transmit"));
		}
	} else {
		m_status->SetBackgroundColour(*wxLIGHT_GREY);
		m_status->SetLabel(wxEmptyString);

		::wxGetApp().setTransmit(false);
	}
}

void CGMSKClientFrame::onOneTouchReply(wxCommandEvent& event)
{
	wxString my12 = m_hrdMy->GetLabel();

	if (my12.IsEmpty() || my12.Left(LONG_CALLSIGN_LENGTH).IsSameAs(wxT("        ")))
		return;

	wxString my1  = my12.Left(LONG_CALLSIGN_LENGTH);
	wxString rpt1 = m_hrdRpt1->GetLabel();
	wxString rpt2 = m_hrdRpt2->GetLabel();

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

void CGMSKClientFrame::onMessage(wxEvent& event)
{
	CMessageEvent& msgEvent = dynamic_cast<CMessageEvent&>(event);

	CMessageData* message = msgEvent.getMessageData();
	wxASSERT(message != NULL);

	wxString msg = message->getMsgText();

	m_hrdMessage->SetLabel(msg);
	m_heard->SetItem(0L, 5, msg);

	delete message;
}

void CGMSKClientFrame::onHeader(wxEvent& event)
{
	CHeaderEvent& hdrEvent = dynamic_cast<CHeaderEvent&>(event);

	CHeaderData* header = hdrEvent.getHeaderData();
	wxASSERT(header != NULL);

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

	wxString rpt1Call = header->getRptCall2();
	m_hrdRpt1->SetLabel(rpt1Call);
	m_heard->SetItem(0L, 3, rpt1Call);

	wxString rpt2Call = header->getRptCall1();
	m_hrdRpt2->SetLabel(rpt2Call);
	m_heard->SetItem(0L, 4, rpt2Call);

	wxString flags;
	flags.Printf(wxT("%02X %02X %02X"), header->getFlag1(), header->getFlag2(), header->getFlag3());
	m_hrdFlags->SetLabel(flags);

	m_hrdMessage->SetLabel(wxEmptyString);

	delete header;
}

void CGMSKClientFrame::onBusy(wxEvent& event)
{
	CBusyEvent& busyEvent = dynamic_cast<CBusyEvent&>(event);

	CBusyData* busy = busyEvent.getBusyData();
	wxASSERT(busy != NULL);

	if (busy->isBusy()) {
		m_status->SetBackgroundColour(*wxCYAN);
		m_status->SetLabel(_("Busy"));
	} else {
		m_status->SetBackgroundColour(*wxLIGHT_GREY);
		m_status->SetLabel(wxEmptyString);
	}

	delete busy;
}

void CGMSKClientFrame::onError(wxEvent& event)
{
	CErrorEvent& errEvent = dynamic_cast<CErrorEvent&>(event);

	wxString text = errEvent.getText();

	wxMessageDialog dialog(this, text, _("GMSK Client Error"), wxICON_ERROR);
	dialog.ShowModal();
}

void CGMSKClientFrame::saveCalls() const
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
