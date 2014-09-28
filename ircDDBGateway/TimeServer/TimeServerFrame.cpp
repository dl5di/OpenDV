/*
 *   Copyright (C) 2012-2014 by Jonathan Naylor G4KLX
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

#include "TimeServerPreferences.h"
#include "TimeServerFrame.h"
#include "TimeServerApp.h"
#include "LogEvent.h"
#include "Version.h"

#if defined(__WINDOWS__)
const unsigned int LOGTEXT_WIDTH  = 560U;
#else
const unsigned int LOGTEXT_WIDTH  = 700U;
#endif

const unsigned int BORDER_SIZE   = 5U;

#include <wx/gbsizer.h>
#include <wx/aboutdlg.h>

DEFINE_EVENT_TYPE(LOG_EVENT)

enum {
	Menu_Edit_Preferences = 6000,
	Menu_View_Updates
};

BEGIN_EVENT_TABLE(CTimeServerFrame, wxFrame)
	EVT_MENU(wxID_EXIT, CTimeServerFrame::onQuit)
	EVT_MENU(Menu_Edit_Preferences, CTimeServerFrame::onPreferences)
	EVT_MENU(Menu_View_Updates, CTimeServerFrame::onUpdates)
	EVT_MENU(wxID_ABOUT, CTimeServerFrame::onAbout)

	EVT_CLOSE(CTimeServerFrame::onClose)

	EVT_CUSTOM(LOG_EVENT, wxID_ANY, CTimeServerFrame::onLog)
END_EVENT_TABLE()

CTimeServerFrame::CTimeServerFrame(const wxString& title, const wxPoint& position, bool gui) :
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

	for (unsigned int i = 0U; i < 10U; i++) {
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

CTimeServerFrame::~CTimeServerFrame()
{
}

wxMenuBar* CTimeServerFrame::createMenuBar()
{
	wxMenu* fileMenu = new wxMenu();
	fileMenu->Append(wxID_EXIT, _("Exit"));

	wxMenu* editMenu = new wxMenu();
	editMenu->Append(Menu_Edit_Preferences, _("Preferences..."));

	wxMenu* viewMenu = new wxMenu();
	viewMenu->AppendCheckItem(Menu_View_Updates, _("GUI Updates"));
	viewMenu->Check(Menu_View_Updates, m_updates);

	wxMenu* helpMenu = new wxMenu();
	helpMenu->Append(wxID_ABOUT, _("About Time Server"));

	wxMenuBar* menuBar = new wxMenuBar();
	menuBar->Append(fileMenu, _("File"));
	menuBar->Append(editMenu, _("Edit"));
	menuBar->Append(viewMenu, _("View"));
	menuBar->Append(helpMenu, _("Help"));

	return menuBar;
}

void CTimeServerFrame::onQuit(wxCommandEvent&)
{
	Close(false);
}

void CTimeServerFrame::onClose(wxCloseEvent&)
{
	int x, y;
	GetPosition(&x, &y);
	if (x >= 0 && y >= 0) {
		::wxGetApp().setPosition(x, y);
		::wxGetApp().writeConfig();
	}

	Destroy();
}

void CTimeServerFrame::onPreferences(wxCommandEvent&)
{
	wxString callsign, address;
	bool sendA, sendB, sendC, sendD, sendE;
	::wxGetApp().getGateway(callsign, sendA, sendB, sendC, sendD, sendE, address);

	LANGUAGE language;
	FORMAT format;
	INTERVAL interval;
	::wxGetApp().getAnnouncements(language, format, interval);

	CTimeServerPreferences dialog1(this, -1, callsign, sendA, sendB, sendC, sendD, sendE, address, language, format, interval);
	if (dialog1.ShowModal() != wxID_OK)
		return;

	callsign  = dialog1.getCallsign();
	sendA     = dialog1.getSendA();
	sendB     = dialog1.getSendB();
	sendC     = dialog1.getSendC();
	sendD     = dialog1.getSendD();
	sendE     = dialog1.getSendE();
	address   = dialog1.getAddress();
	language  = dialog1.getLanguage();
	format    = dialog1.getFormat();
	interval  = dialog1.getInterval();

	::wxGetApp().setGateway(callsign, sendA, sendB, sendC, sendD, sendE, address);
	::wxGetApp().setAnnouncements(language, format, interval);
	::wxGetApp().writeConfig();

	wxMessageDialog dialog2(this, _("The changes made will not take effect\nuntil the application is restarted"), _("DCS Gateway Information"), wxICON_INFORMATION);
	dialog2.ShowModal();
}

void CTimeServerFrame::onUpdates(wxCommandEvent& event)
{
	m_updates = event.IsChecked();
}

void CTimeServerFrame::onAbout(wxCommandEvent&)
{
	wxAboutDialogInfo info;
	info.AddDeveloper(wxT("Jonathan Naylor, G4KLX"));
	info.SetCopyright(wxT("(C) 2012-2014 using GPL v2 or later"));
	info.SetName(APPLICATION_NAME);
	info.SetVersion(VERSION);
	info.SetDescription(_("This program allows a computer running a gateway\nto have the time announced."));

	::wxAboutBox(info);
}

void CTimeServerFrame::onLog(wxEvent& event)
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

	text = logEvent.getText();
	m_logLine[9U]->SetLabel(text);
}

void CTimeServerFrame::showLog(const wxString& text)
{
	if (!m_updates)
		return;

	CLogEvent event(text, LOG_EVENT);

	AddPendingEvent(event);
}
