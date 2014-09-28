/*
 *   Copyright (C) 2012 by Jonathan Naylor G4KLX
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

#include "DCSClientHostFile.h"
#include "DCSClientPanel.h"
#include "DCSClientApp.h"
#include "DStarDefines.h"

#include <wx/filename.h>
#include <wx/gbsizer.h>

enum {
	Button_Connect = 7000,
	Button_Transmit,

	Timer_Connected
};

BEGIN_EVENT_TABLE(CDCSClientPanel, wxPanel)
	EVT_TOGGLEBUTTON(Button_Connect,  CDCSClientPanel::onConnect)
	EVT_TOGGLEBUTTON(Button_Transmit, CDCSClientPanel::onTransmit)

	EVT_TIMER(Timer_Connected, CDCSClientPanel::onTimer)
END_EVENT_TABLE()

const unsigned int BORDER_SIZE    = 5U;
const unsigned int LABEL_WIDTH1   = 80U;
const unsigned int LABEL_WIDTH2   = 60U;
const unsigned int CONTROL_WIDTH1 = 100U;
const unsigned int CONTROL_WIDTH2 = 50U;

const unsigned int INFO_WIDTH    = 690U;
const unsigned int INFO_HEIGHT   = 160U;

const unsigned int HEARD_WIDTH   = 690U;
const unsigned int HEARD_HEIGHT  = 300U;

const unsigned int DATETIME_WIDTH = 125U;
const unsigned int CALLSIGN_WIDTH = 100U;
const unsigned int MESSAGE_WIDTH  = 260U;

const unsigned int LONG_CALLSIGN_WIDTH   = 80U;
const unsigned int SHORT_CALLSIGN_WIDTH  = 50U;

CDCSClientPanel::CDCSClientPanel(wxWindow* parent, int id) :
wxPanel(parent, id),
m_timer(this, Timer_Connected),
m_reflector(NULL),
m_connect(NULL),
m_module(NULL),
m_status1(NULL),
m_transmit(NULL),
m_status2(NULL),
m_hrdYour(NULL),
m_hrdMy(NULL),
m_hrdReflector(NULL),
m_hrdMessage(NULL),
m_heard(NULL),
m_reflectorName(),
m_connected(false),
m_busy(false),
m_hosts(),
m_initialised(false)
{
	wxGridBagSizer* panelSizer = new wxGridBagSizer();

	wxStaticText* reflectorLabel = new wxStaticText(this, -1, _("Reflector"), wxDefaultPosition, wxSize(LABEL_WIDTH2, -1), wxALIGN_RIGHT);
	panelSizer->Add(reflectorLabel, wxGBPosition(0, 0), wxDefaultSpan, wxALL, BORDER_SIZE);

	wxFileName fileName(wxFileName::GetHomeDir(), HOSTS_FILE_NAME);

	if (!fileName.IsFileReadable()) {
		wxLogMessage(wxT("File %s not readable"), fileName.GetFullPath().c_str());
#if defined(__WINDOWS__)
		fileName.Assign(::wxGetCwd(), HOSTS_FILE_NAME);
#else
		fileName.Assign(wxT(DATA_DIR), HOSTS_FILE_NAME);
#endif
		if (!fileName.IsFileReadable())
			wxLogMessage(wxT("File %s not readable"), fileName.GetFullPath().c_str());
	}

	CDCSClientHostFile file(fileName.GetFullPath());
	unsigned int hostCount = file.getCount();

	m_reflector = new wxChoice(this, -1, wxDefaultPosition, wxSize(CONTROL_WIDTH1, -1));
	for (unsigned int i = 0U; i < hostCount; i++) {
		m_hosts.Add(file.getHost(i));
		m_reflector->Append(file.getName(i));
	}
	panelSizer->Add(m_reflector, wxGBPosition(0, 1), wxDefaultSpan, wxALL, BORDER_SIZE);

	m_module = new wxChoice(this, -1, wxDefaultPosition, wxSize(CONTROL_WIDTH2, -1));
	m_module->Append(wxT("A"));
	m_module->Append(wxT("B"));
	m_module->Append(wxT("C"));
	m_module->Append(wxT("D"));
	m_module->Append(wxT("E"));
	m_module->Append(wxT("F"));
	m_module->Append(wxT("G"));
	m_module->Append(wxT("H"));
	m_module->Append(wxT("I"));
	m_module->Append(wxT("J"));
	m_module->Append(wxT("K"));
	m_module->Append(wxT("L"));
	m_module->Append(wxT("M"));
	m_module->Append(wxT("N"));
	m_module->Append(wxT("O"));
	m_module->Append(wxT("P"));
	m_module->Append(wxT("Q"));
	m_module->Append(wxT("R"));
	m_module->Append(wxT("S"));
	m_module->Append(wxT("T"));
	m_module->Append(wxT("U"));
	m_module->Append(wxT("V"));
	m_module->Append(wxT("W"));
	m_module->Append(wxT("X"));
	m_module->Append(wxT("Y"));
	m_module->Append(wxT("Z"));
	panelSizer->Add(m_module, wxGBPosition(0, 2), wxDefaultSpan, wxALL, BORDER_SIZE);

	wxString reflector;
	::wxGetApp().getLastReflector(reflector);

	wxString module = reflector.Mid(LONG_CALLSIGN_LENGTH - 1U, 1U);
	wxString ref    = reflector.Left(LONG_CALLSIGN_LENGTH - 1U);
	ref.Trim();

	bool res = m_reflector->SetStringSelection(ref);
	if (!res)
		m_reflector->SetSelection(0);

	res = m_module->SetStringSelection(module);
	if (!res)
		m_module->SetSelection(0U);

	m_connect = new wxToggleButton(this, Button_Connect, _("Connect"), wxDefaultPosition, wxSize(CONTROL_WIDTH1, -1));
	panelSizer->Add(m_connect, wxGBPosition(0, 3), wxDefaultSpan, wxALL, BORDER_SIZE);

	m_status1 = new wxStaticText(this, -1, _("Not connected"));
	panelSizer->Add(m_status1, wxGBPosition(0, 4), wxDefaultSpan, wxALL, BORDER_SIZE);

	m_transmit = new wxToggleButton(this, Button_Transmit, _("Transmit"), wxDefaultPosition, wxSize(CONTROL_WIDTH1 * 2 + BORDER_SIZE * 2, -1));
	panelSizer->Add(m_transmit, wxGBPosition(1, 1), wxGBSpan(1, 2), wxALL, BORDER_SIZE);

	m_status2 = new wxStaticText(this, -1, wxEmptyString);
	panelSizer->Add(m_status2, wxGBPosition(1, 4), wxDefaultSpan, wxALL, BORDER_SIZE);

	wxStaticBoxSizer* info1Sizer = new wxStaticBoxSizer(new wxStaticBox(this, -1, _("Current"), wxDefaultPosition, wxSize(INFO_WIDTH, INFO_HEIGHT)), wxVERTICAL);

	wxGridBagSizer* info2Sizer = new wxGridBagSizer(BORDER_SIZE, BORDER_SIZE);

	wxStaticText* hrdYourLabel = new wxStaticText(this, -1, _("Your:"), wxDefaultPosition, wxSize(LABEL_WIDTH1, -1), wxALIGN_RIGHT);
	info2Sizer->Add(hrdYourLabel, wxGBPosition(0, 0), wxDefaultSpan, wxALL, BORDER_SIZE);

	m_hrdYour = new wxStaticText(this, -1, wxEmptyString, wxDefaultPosition, wxSize(CONTROL_WIDTH1, -1));
	info2Sizer->Add(m_hrdYour, wxGBPosition(0, 1), wxDefaultSpan, wxALL, BORDER_SIZE);

	wxStaticText* hrdMyLabel = new wxStaticText(this, -1, _("My:"), wxDefaultPosition, wxSize(LABEL_WIDTH1, -1), wxALIGN_RIGHT);
	info2Sizer->Add(hrdMyLabel, wxGBPosition(0, 2), wxDefaultSpan, wxALL, BORDER_SIZE);

	m_hrdMy = new wxStaticText(this, -1, wxEmptyString, wxDefaultPosition, wxSize(CONTROL_WIDTH1, -1));
	info2Sizer->Add(m_hrdMy, wxGBPosition(0, 3), wxDefaultSpan, wxALL, BORDER_SIZE);

	wxStaticText* hrdReflectorLabel = new wxStaticText(this, -1, wxT("Reflector:"), wxDefaultPosition, wxSize(LABEL_WIDTH1, -1), wxALIGN_RIGHT);
	info2Sizer->Add(hrdReflectorLabel, wxGBPosition(0, 4), wxDefaultSpan, wxALL, BORDER_SIZE);

	m_hrdReflector = new wxStaticText(this, -1, wxEmptyString, wxDefaultPosition, wxSize(CONTROL_WIDTH1, -1));
	info2Sizer->Add(m_hrdReflector, wxGBPosition(0, 5), wxDefaultSpan, wxALL, BORDER_SIZE);

	wxStaticText* hrdMessageLabel = new wxStaticText(this, -1, _("Message:"), wxDefaultPosition, wxSize(LABEL_WIDTH1, -1), wxALIGN_RIGHT);
	info2Sizer->Add(hrdMessageLabel, wxGBPosition(1, 0), wxDefaultSpan, wxALL, BORDER_SIZE);

	m_hrdMessage = new wxStaticText(this, -1, wxEmptyString, wxDefaultPosition, wxSize(LABEL_WIDTH1 * 3U, -1), wxALIGN_RIGHT);
	info2Sizer->Add(m_hrdMessage, wxGBPosition(1, 1), wxGBSpan(1, 3), wxALL, BORDER_SIZE);

	info1Sizer->Add(info2Sizer);

	panelSizer->Add(info1Sizer, wxGBPosition(2, 0), wxGBSpan(5, 6), wxALL | wxEXPAND, BORDER_SIZE);

	m_heard = new wxListCtrl(this, -1, wxDefaultPosition, wxSize(HEARD_WIDTH, HEARD_HEIGHT), wxLC_REPORT | wxLC_SINGLE_SEL);
	m_heard->InsertColumn(0L, _("Date/Time"));
	m_heard->SetColumnWidth(0L, DATETIME_WIDTH);
	m_heard->InsertColumn(1L, _("Your"));
	m_heard->SetColumnWidth(1L, CALLSIGN_WIDTH);
	m_heard->InsertColumn(2L, _("My"));
	m_heard->SetColumnWidth(2L, CALLSIGN_WIDTH);
	m_heard->InsertColumn(3L, _("Reflector"));
	m_heard->SetColumnWidth(3L, CALLSIGN_WIDTH);
	m_heard->InsertColumn(4L, _("Message"));
	m_heard->SetColumnWidth(4L, MESSAGE_WIDTH);
	panelSizer->Add(m_heard, wxGBPosition(7, 0), wxGBSpan(11, 6), wxALL | wxEXPAND, BORDER_SIZE);

	SetSizer(panelSizer);
	panelSizer->SetSizeHints(this);

	m_timer.Start(5000);
}

CDCSClientPanel::~CDCSClientPanel()
{
}

void CDCSClientPanel::onConnect(wxCommandEvent& event)
{
	bool state = event.IsChecked();

	if (state) {
		int n1 = m_reflector->GetSelection();
		if (n1 == wxNOT_FOUND) {
			onError(_("No Reflector name selected"));
			return;
		}

		int n2 = m_module->GetSelection();
		if (n2 == wxNOT_FOUND) {
			onError(_("No Module selected"));
			return;
		}

		m_reflectorName = m_reflector->GetString(n1);
		wxString module = m_module->GetString(n2);

		wxString address = m_hosts.Item(n1);

		m_reflectorName.resize(LONG_CALLSIGN_LENGTH - 1U, wxT(' '));
		m_reflectorName.Append(module);

		bool res = ::wxGetApp().connect(true, m_reflectorName, address);
		if (res) {
			m_connect->SetLabel(_("Disconnect"));
			::wxGetApp().setLastReflector(m_reflectorName);
		} else {
			// Connect failed
			m_connect->SetValue(false);
			wxLogError(wxT("Unable to connect"));
			onError(_("Unable to connect"));
		}
	} else {
		m_status1->SetLabel(_("Not connected"));
		m_connect->SetLabel(_("Connect"));
		::wxGetApp().connect(false);
	}
}

void CDCSClientPanel::onTransmit(wxCommandEvent& event)
{
	bool state = event.IsChecked();

	if (m_busy) {
		m_transmit->SetValue(false);
		wxLogError(wxT("Unable to transmit"));
		onError(_("Unable to transmit"));
		return;
	}

	if (state) {
		bool res = ::wxGetApp().transmit(true);
		if (res) {
			m_status2->SetBackgroundColour(*wxRED);
			m_status2->SetLabel(_("TRANSMIT"));
		} else {
			// Transmit failed
			m_transmit->SetValue(false);
			wxLogError(wxT("Unable to transmit"));
			onError(_("Unable to transmit"));
		}
	} else {
		m_status2->SetBackgroundColour(*wxLIGHT_GREY);
		m_status2->SetLabel(wxEmptyString);

		::wxGetApp().transmit(false);
	}
}

void CDCSClientPanel::onMessage(CMessageData* message)
{
	wxASSERT(message != NULL);

	wxString msg = message->getMsgText();

	m_hrdMessage->SetLabel(msg);
	m_heard->SetItem(0L, 4, msg);

	delete message;
}

void CDCSClientPanel::onHeader(CHeaderData* header)
{
	m_busy = header != NULL;

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

		wxString reflectorCall = header->getRptCall2();
		m_hrdReflector->SetLabel(reflectorCall);
		m_heard->SetItem(0L, 3, reflectorCall);

		m_hrdMessage->SetLabel(wxEmptyString);

		m_status2->SetBackgroundColour(*wxCYAN);
		m_status2->SetLabel(_("Busy"));

		delete header;
	} else {
		m_hrdYour->SetLabel(wxEmptyString);
		m_hrdMy->SetLabel(wxEmptyString);
		m_hrdReflector->SetLabel(wxEmptyString);
		m_hrdMessage->SetLabel(wxEmptyString);

		m_status2->SetBackgroundColour(*wxLIGHT_GREY);
		m_status2->SetLabel(wxEmptyString);
	}
}

void CDCSClientPanel::onError(const wxString& error)
{
	wxMessageDialog dialog(this, error, _("DExtra Client Error"), wxICON_ERROR);
	dialog.ShowModal();
}

void CDCSClientPanel::onTimer(wxTimerEvent& event)
{
	m_connected = ::wxGetApp().isConnected();
	if (m_connected)
		m_status1->SetLabel(_("Connected to ") + m_reflectorName);
	else
		m_status1->SetLabel(_("Not connected"));

	if (!m_initialised) {
		setStartupReflector();
		m_initialised = true;
	}
}

void CDCSClientPanel::setStartupReflector()
{
	wxString reflector;
	::wxGetApp().getStartup(reflector);

	// Anything to do?
	if (reflector.IsEmpty())
		return;

	wxString module = reflector.Mid(LONG_CALLSIGN_LENGTH - 1U, 1U);
	wxString    ref = reflector.Left(LONG_CALLSIGN_LENGTH - 1U).Trim();

	int n1 = m_reflector->FindString(ref);
	if (n1 == wxNOT_FOUND) {
		wxLogError(wxT("Cannot find the startup reflector in the DCS list"));
		return;
	}

	m_reflector->SetSelection(n1);

	int n2 = m_module->FindString(module);
	if (n2 == wxNOT_FOUND) {
		wxLogError(wxT("Cannot find the startup module in the module list"));
		return;
	}

	m_module->SetSelection(n2);

	// Set the reflector
	m_reflectorName = reflector;

	wxString address = m_hosts.Item(n1);

	bool res = ::wxGetApp().connect(true, m_reflectorName, address);
	if (res) {
		m_connect->SetValue(true);
		m_connect->SetLabel(_("Disconnect"));
		::wxGetApp().setLastReflector(m_reflectorName);
	} else {
		// Connect failed
		wxLogError(wxT("Unable to connect to startup reflector: %s"), reflector.c_str());
		onError(_("Unable to connect"));
	}
}
