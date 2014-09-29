/*
 *   Copyright (C) 2011,2012,2013 by Jonathan Naylor G4KLX
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

#include "XReflectorPreferences.h"
#include "XReflectorFrame.h"
#include "XReflectorDefs.h"
#include "XReflectorApp.h"
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

BEGIN_EVENT_TABLE(CXReflectorFrame, wxFrame)
	EVT_MENU(wxID_EXIT, CXReflectorFrame::onQuit)
	EVT_MENU(Menu_Edit_Preferences, CXReflectorFrame::onPreferences)
	EVT_MENU(Menu_View_Updates, CXReflectorFrame::onUpdates)
	EVT_MENU(wxID_ABOUT, CXReflectorFrame::onAbout)

	EVT_CLOSE(CXReflectorFrame::onClose)

	EVT_CUSTOM(LOG_EVENT, wxID_ANY, CXReflectorFrame::onLog)
END_EVENT_TABLE()

CXReflectorFrame::CXReflectorFrame(const wxString& title, const wxPoint& position, bool gui) :
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

CXReflectorFrame::~CXReflectorFrame()
{
}

wxMenuBar* CXReflectorFrame::createMenuBar()
{
	wxMenu* fileMenu = new wxMenu();
	fileMenu->Append(wxID_EXIT, _("Exit"));

	wxMenu* editMenu = new wxMenu();
	editMenu->Append(Menu_Edit_Preferences, _("Preferences..."));

	wxMenu* viewMenu = new wxMenu();
	viewMenu->AppendCheckItem(Menu_View_Updates, _("GUI Updates"));
	viewMenu->Check(Menu_View_Updates, m_updates);

	wxMenu* helpMenu = new wxMenu();
	helpMenu->Append(wxID_ABOUT, _("About XReflector"));

	wxMenuBar* menuBar = new wxMenuBar();
	menuBar->Append(fileMenu, _("File"));
	menuBar->Append(editMenu, _("Edit"));
	menuBar->Append(viewMenu, _("View"));
	menuBar->Append(helpMenu, _("Help"));

	return menuBar;
}

void CXReflectorFrame::onQuit(wxCommandEvent& event)
{
	Close(false);
}

void CXReflectorFrame::onClose(wxCloseEvent& event)
{
	int x, y;
	GetPosition(&x, &y);
	if (x >= 0 && y >= 0) {
		::wxGetApp().setPosition(x, y);
		::wxGetApp().writeConfig();
	}

	Destroy();
}

void CXReflectorFrame::onPreferences(wxCommandEvent& event)
{
	wxString callsign, address;
	::wxGetApp().getReflector(callsign, address);

	bool dplusEnabled;
	::wxGetApp().getDPlus(dplusEnabled);

	bool logEnabled;
	::wxGetApp().getMiscellaneous(logEnabled);

	CXReflectorPreferences dialog1(this, -1, callsign, address, dplusEnabled, logEnabled);
	if (dialog1.ShowModal() != wxID_OK)
		return;

	callsign     = dialog1.getCallsign();
	address      = dialog1.getAddress();

	dplusEnabled = dialog1.getDPlusEnabled();

	logEnabled   = dialog1.getLogEnabled();

	::wxGetApp().setReflector(callsign, address);
	::wxGetApp().setDPlus(dplusEnabled);
	::wxGetApp().setMiscellaneous(logEnabled);
	::wxGetApp().writeConfig();

	wxMessageDialog dialog2(this, _("The changes made will not take effect\nuntil the application is restarted"), _("XReflector Information"), wxICON_INFORMATION);
	dialog2.ShowModal();
}

void CXReflectorFrame::onUpdates(wxCommandEvent& event)
{
	m_updates = event.IsChecked();
}

void CXReflectorFrame::onAbout(wxCommandEvent& event)
{
	wxAboutDialogInfo info;
	info.AddDeveloper(wxT("Jonathan Naylor, G4KLX"));
	info.SetCopyright(wxT("(C) 2011-2013 using GPL v2 or later"));
	info.SetName(APPLICATION_NAME);
	info.SetVersion(VERSION);
	info.SetDescription(_("This program allows an Internet connected computer \nto become a DExtra and D-Plus Reflector."));

	::wxAboutBox(info);
}

void CXReflectorFrame::onLog(wxEvent& event)
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

void CXReflectorFrame::showLog(const wxString& text)
{
	if (!m_updates)
		return;

	CLogEvent event(text, LOG_EVENT);

	AddPendingEvent(event);
}
