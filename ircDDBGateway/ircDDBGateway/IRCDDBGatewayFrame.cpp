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

#include "IRCDDBGatewayStatusData.h"
#include "IRCDDBGatewayFrame.h"
#include "IRCDDBGatewayDefs.h"
#include "IRCDDBGatewayApp.h"
#include "LogEvent.h"
#include "Version.h"

const unsigned int BORDER_SIZE = 5U;

#if defined(__WINDOWS__)
const unsigned int IRCDDB_WIDTH   = 230U;
const unsigned int LINKED_WIDTH   = 160U;
const unsigned int INCOMING_WIDTH = 320U;
const unsigned int DONGLES_WIDTH  = 560U;
const unsigned int LOGTEXT_WIDTH  = 560U;
#else
const unsigned int IRCDDB_WIDTH   = 288U;
const unsigned int LINKED_WIDTH   = 220U;
const unsigned int INCOMING_WIDTH = 385U;
const unsigned int DONGLES_WIDTH  = 700U;
const unsigned int LOGTEXT_WIDTH  = 700U;
#endif

#include <wx/gbsizer.h>
#include <wx/aboutdlg.h>

DEFINE_EVENT_TYPE(LOG_EVENT)

enum {
	Menu_Edit_Preferences = 6000,
	Menu_View_Updates
};

BEGIN_EVENT_TABLE(CIRCDDBGatewayFrame, wxFrame)
	EVT_MENU(wxID_EXIT, CIRCDDBGatewayFrame::onQuit)
	EVT_MENU(Menu_View_Updates, CIRCDDBGatewayFrame::onUpdates)
	EVT_MENU(wxID_ABOUT, CIRCDDBGatewayFrame::onAbout)

	EVT_TIMER(-1, CIRCDDBGatewayFrame::onTimer)

	EVT_CLOSE(CIRCDDBGatewayFrame::onClose)

	EVT_CUSTOM(LOG_EVENT, wxID_ANY, CIRCDDBGatewayFrame::onLog)
END_EVENT_TABLE()

CIRCDDBGatewayFrame::CIRCDDBGatewayFrame(const wxString& title, const wxPoint& position, bool gui) :
wxFrame(NULL, -1, title, position),
m_timer(this),
m_ircDDBStatus(NULL),
m_dprsStatus(NULL),
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

	wxStaticBoxSizer* status1Sizer = new wxStaticBoxSizer(new wxStaticBox(panel, -1, _("Status")), wxVERTICAL);
	wxFlexGridSizer* status2Sizer = new wxFlexGridSizer(6);

	wxStaticText* ircDDBStatusLabel = new wxStaticText(panel, -1, _("ircDDB:"));
	status2Sizer->Add(ircDDBStatusLabel, 0, wxALL, BORDER_SIZE);

	m_ircDDBStatus = new wxStaticText(panel, -1, wxEmptyString, wxDefaultPosition, wxSize(IRCDDB_WIDTH, -1));
	status2Sizer->Add(m_ircDDBStatus, 0, wxALL, BORDER_SIZE);

	wxStaticText* dprsStatusLabel = new wxStaticText(panel, -1, _("D-PRS:"));
	status2Sizer->Add(dprsStatusLabel, 0, wxALL, BORDER_SIZE);

	m_dprsStatus = new wxStaticText(panel, -1, wxEmptyString, wxDefaultPosition, wxSize(IRCDDB_WIDTH, -1));
	status2Sizer->Add(m_dprsStatus, 0, wxALL, BORDER_SIZE);

	status1Sizer->Add(status2Sizer);
	panelSizer->Add(status1Sizer, 0, wxALL, BORDER_SIZE);

	wxStaticBoxSizer* links1Sizer = new wxStaticBoxSizer(new wxStaticBox(panel, -1, _("Links")), wxVERTICAL);
	wxFlexGridSizer* links2Sizer = new wxFlexGridSizer(3);

	for (unsigned int i = 0U; i < 4U; i++) {
		wxString text;
		text.Printf(_("Repeater %u:"), i + 1U);

		wxStaticText* repeaterLabel = new wxStaticText(panel, -1, text);
		links2Sizer->Add(repeaterLabel, 0, wxALL, BORDER_SIZE);

		m_linkStatus[i] = new wxStaticText(panel, -1, wxEmptyString, wxDefaultPosition, wxSize(LINKED_WIDTH, -1));
		links2Sizer->Add(m_linkStatus[i], 0, wxALL, BORDER_SIZE);

		m_incoming[i] = new wxStaticText(panel, -1, wxEmptyString, wxDefaultPosition, wxSize(INCOMING_WIDTH, -1));
		links2Sizer->Add(m_incoming[i], 0, wxALL, BORDER_SIZE);
	}

	links1Sizer->Add(links2Sizer);
	panelSizer->Add(links1Sizer, 0, wxALL, BORDER_SIZE);

	wxStaticBoxSizer* dongles1Sizer = new wxStaticBoxSizer(new wxStaticBox(panel, -1, _("Dongles")), wxVERTICAL);
	wxFlexGridSizer* dongles2Sizer = new wxFlexGridSizer(5);

	m_dongles = new wxStaticText(panel, -1, wxEmptyString, wxDefaultPosition, wxSize(DONGLES_WIDTH, -1));
	dongles2Sizer->Add(m_dongles, 0, wxALL, BORDER_SIZE);

	dongles1Sizer->Add(dongles2Sizer);
	panelSizer->Add(dongles1Sizer, 0, wxALL, BORDER_SIZE);

	wxStaticBoxSizer* log1Sizer = new wxStaticBoxSizer(new wxStaticBox(panel, -1, _("Log")), wxVERTICAL);
	wxBoxSizer* log2Sizer = new wxBoxSizer(wxVERTICAL);

	for (unsigned int i = 0U; i < 7U; i++) {
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

	m_timer.Start(5000);
}

CIRCDDBGatewayFrame::~CIRCDDBGatewayFrame()
{
}

wxMenuBar* CIRCDDBGatewayFrame::createMenuBar()
{
	wxMenu* fileMenu = new wxMenu();
	fileMenu->Append(wxID_EXIT, _("Exit"));

	wxMenu* viewMenu = new wxMenu();
	viewMenu->AppendCheckItem(Menu_View_Updates, _("GUI Updates"));
	viewMenu->Check(Menu_View_Updates, m_updates);

	wxMenu* helpMenu = new wxMenu();
	helpMenu->Append(wxID_ABOUT, _("About ircDDB Gateway"));

	wxMenuBar* menuBar = new wxMenuBar();
	menuBar->Append(fileMenu, _("File"));
	menuBar->Append(viewMenu, _("View"));
	menuBar->Append(helpMenu, _("Help"));

	return menuBar;
}

void CIRCDDBGatewayFrame::onQuit(wxCommandEvent&)
{
	Close(false);
}

void CIRCDDBGatewayFrame::onClose(wxCloseEvent&)
{
	int x, y;
	GetPosition(&x, &y);
	if (x >= 0 && y >= 0)
		::wxGetApp().setPosition(x, y);

	Destroy();
}

void CIRCDDBGatewayFrame::onUpdates(wxCommandEvent& event)
{
	m_updates = event.IsChecked();
}

void CIRCDDBGatewayFrame::onAbout(wxCommandEvent&)
{
	wxAboutDialogInfo info;
	info.AddDeveloper(wxT("Jonathan Naylor, G4KLX"));
	info.AddDeveloper(wxT("Michael Dirska, DL1BFF"));
	info.SetCopyright(wxT("(C) 2010-2014 using GPL v2 or later"));
	info.SetName(APPLICATION_NAME);
	info.SetVersion(VERSION);
	info.SetDescription(_("This program allows a computer running homebrew repeaters\nto access the ircDDB network for G2 callsign and repeater routing,\nas well as D-Plus and DExtra reflectors. It includes a StarNet\nDigital server."));

	::wxAboutBox(info);
}

void CIRCDDBGatewayFrame::onLog(wxEvent& event)
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

	text = logEvent.getText();
	m_logLine[6U]->SetLabel(text);
}

void CIRCDDBGatewayFrame::showLog(const wxString& text)
{
	if (!m_updates)
		return;

	CLogEvent event(text, LOG_EVENT);

	AddPendingEvent(event);
}

void CIRCDDBGatewayFrame::onTimer(wxTimerEvent&)
{
	if (!m_updates)
		return;

	CIRCDDBGatewayStatusData* status = ::wxGetApp().getStatus();
	if (status == NULL)
		return;

	IRCDDB_STATUS ircDDBStatus = status->getIrcDDBStatus();
	switch (ircDDBStatus) {
		case IS_DISABLED:
			m_ircDDBStatus->SetLabel(_("Disabled"));
			break;
		case IS_DISCONNECTED:
			m_ircDDBStatus->SetLabel(_("Disconnected"));
			break;
		case IS_CONNECTING:
			m_ircDDBStatus->SetLabel(_("Connecting"));
			break;
		default:
			m_ircDDBStatus->SetLabel(_("Connected"));
			break;
	}

	bool dprsStatus = status->getDPRSStatus();
	if (dprsStatus) {
		if (ircDDBStatus == IS_CONNECTED || ircDDBStatus == IS_DISABLED)
			m_dprsStatus->SetLabel(_("Active"));
		else
			m_dprsStatus->SetLabel(_("Waiting"));
	} else {
		m_dprsStatus->SetLabel(_("Inactive"));
	}

	for (unsigned int i = 0U; i < 4U; i++) {
		wxString callsign = status->getCallsign(i);
		if (!callsign.IsEmpty()) {
			wxString text = callsign;

			wxString linkCallsign = status->getLinkCallsign(i);

			switch (status->getLinkStatus(i)) {
				case LS_NONE:
					text.Append(_("  Not linked"));
					break;
				case LS_LINKED_LOOPBACK:
				case LS_LINKED_DEXTRA:
				case LS_LINKED_DPLUS:
				case LS_LINKED_DCS:
				case LS_LINKED_CCS:
					text.Append(_("  Linked to  "));
					text.Append(linkCallsign);
					break;
				default:
					text.Append(_("  Linking to  "));
					text.Append(linkCallsign);
					break;
			}
			m_linkStatus[i]->SetLabel(text);

			wxString incoming = status->getIncoming(i);
			if (!incoming.IsEmpty()) {
				incoming.Prepend(wxT("[  "));
				incoming.Append(wxT("]"));
			}
			m_incoming[i]->SetLabel(incoming);
		}
	}

	wxString dongles = status->getDongles();
	m_dongles->SetLabel(dongles);

	delete status;
}
