/*
 *   Copyright (C) 2011 by Jonathan Naylor G4KLX
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

#include "RemoteControlStarNetPanel.h"
#include "RemoteControlApp.h"

enum {
	Button_Refresh = 7100,
	Button_LogoffAll,
	List_StarNets,
	Menu_Logoff
};

BEGIN_EVENT_TABLE(CRemoteControlStarNetPanel, wxPanel)
	EVT_BUTTON(Button_Refresh,               CRemoteControlStarNetPanel::onRefresh)
	EVT_BUTTON(Button_LogoffAll,             CRemoteControlStarNetPanel::onLogoffAll)
	EVT_LIST_ITEM_RIGHT_CLICK(List_StarNets, CRemoteControlStarNetPanel::onClick)
	EVT_MENU(Menu_Logoff,                    CRemoteControlStarNetPanel::onLogoff)
END_EVENT_TABLE()

#if defined(__WINDOWS__)
const unsigned int LIST_HEIGHT    = 350U;
const unsigned int LIST_WIDTH     = 350U;
const unsigned int CALLSIGN_WIDTH = 100U;
const unsigned int TIMER_WIDTH    = 145U;
const unsigned int BUTTON_WIDTH   = 75U;
#else
const unsigned int LIST_HEIGHT    = 350U;
const unsigned int LIST_WIDTH     = 350U;
const unsigned int CALLSIGN_WIDTH = 100U;
const unsigned int TIMER_WIDTH    = 145U;
const unsigned int BUTTON_WIDTH   = 75U;
#endif

const unsigned int BORDER_SIZE = 5U;

CRemoteControlStarNetPanel::CRemoteControlStarNetPanel(wxWindow* parent, int id, const wxString& callsign) :
wxPanel(parent, id),
m_callsign(callsign),
m_list(NULL),
m_logoff(NULL),
m_timer(NULL),
m_logoffAll(NULL),
m_chosen(),
m_menu(NULL)
{
	wxBoxSizer* sizer1 = new wxBoxSizer(wxHORIZONTAL);

	m_list = new wxListCtrl(this, List_StarNets, wxDefaultPosition, wxSize(LIST_WIDTH, LIST_HEIGHT), wxLC_REPORT | wxLC_SINGLE_SEL);
	m_list->InsertColumn(0L, _("Callsign"));
	m_list->SetColumnWidth(0L, CALLSIGN_WIDTH);
	m_list->InsertColumn(1L, _("User Timer"));
	m_list->SetColumnWidth(1L, TIMER_WIDTH);
	sizer1->Add(m_list, 0, wxTOP | wxBOTTOM | wxLEFT | wxEXPAND, BORDER_SIZE);

	wxBoxSizer* sizer2 = new wxBoxSizer(wxVERTICAL);

	wxButton* refreshButton = new wxButton(this, Button_Refresh, _("Refresh"), wxDefaultPosition, wxSize(BUTTON_WIDTH, -1));
	sizer2->Add(refreshButton, 0, wxALL, BORDER_SIZE);

	wxStaticText* dummy1Label = new wxStaticText(this, -1, wxEmptyString);
	sizer2->Add(dummy1Label, 0, wxALL, BORDER_SIZE);

	wxStaticText* logoffLabel = new wxStaticText(this, -1, _("Logoff Callsign"));
	sizer2->Add(logoffLabel, 0, wxALL, BORDER_SIZE);

	m_logoff = new wxStaticText(this, -1, wxEmptyString);
	sizer2->Add(m_logoff, 0, wxALL, BORDER_SIZE);

	wxStaticText* dummy3Label = new wxStaticText(this, -1, wxEmptyString);
	sizer2->Add(dummy3Label, 0, wxALL, BORDER_SIZE);

	wxStaticText* timerLabel = new wxStaticText(this, -1, _("Group Timer"));
	sizer2->Add(timerLabel, 0, wxALL, BORDER_SIZE);

	m_timer = new wxStaticText(this, -1, wxEmptyString);
	sizer2->Add(m_timer, 0, wxALL, BORDER_SIZE);

	wxStaticText* dummy5Label = new wxStaticText(this, -1, wxEmptyString);
	sizer2->Add(dummy5Label, 0, wxALL, BORDER_SIZE);

	m_logoffAll = new wxButton(this, Button_LogoffAll, _("Logoff All"), wxDefaultPosition, wxSize(BUTTON_WIDTH, -1));
	m_logoffAll->Disable();
	sizer2->Add(m_logoffAll, 0, wxALL, BORDER_SIZE);

	sizer1->Add(sizer2);

	SetAutoLayout(true);

	SetSizer(sizer1);

	m_menu = new wxMenu;
	m_menu->Append(Menu_Logoff, _("Logoff"));
}


CRemoteControlStarNetPanel::~CRemoteControlStarNetPanel()
{
}

void CRemoteControlStarNetPanel::add(const CRemoteControlStarNetGroup& data)
{
	m_list->DeleteAllItems();

	wxString logoff = data.getLogoff();
	if (logoff.IsEmpty())
		m_logoff->SetLabel(_("None"));
	else
		m_logoff->SetLabel(logoff);

	unsigned int   timer = data.getTimer();
	unsigned int timeout = data.getTimeout();

	if (timeout == 0U) {
		m_timer->SetLabel(_("None"));
	} else {
		wxString text;
		text.Printf(wxT("%u/%u"), timer, timeout);
		m_timer->SetLabel(text);
	}

	unsigned int count = data.getUserCount();
	for (unsigned int i = 0U; i < count; i++) {
		CRemoteControlStarNetUser& user = data.getUser(i);

		wxString    callsign = user.getCallsign();
		unsigned int   timer = user.getTimer();
		unsigned int timeout = user.getTimeout();

		m_list->InsertItem(0L, callsign);

		if (timeout == 0U) {
			m_list->SetItem(0L, 1, _("None"));
		} else {
			wxString text;
			text.Printf(wxT("%u/%u"), timer, timeout);
			m_list->SetItem(0L, 1, text);
		}
	}

	if (count > 0U)
		m_logoffAll->Enable();
	else
		m_logoffAll->Disable();
}

void CRemoteControlStarNetPanel::onClick(wxListEvent& event)
{
	long n = event.GetIndex();
	if (n < 0) {
		m_chosen.Clear();
		return;
	}

	m_chosen = m_list->GetItemText(n);

	PopupMenu(m_menu);
}

void CRemoteControlStarNetPanel::onRefresh(wxCommandEvent& event)
{
	::wxGetApp().starNetRefresh(m_callsign);
}

void CRemoteControlStarNetPanel::onLogoff(wxCommandEvent& event)
{
	::wxGetApp().starNetLogoff(m_callsign, m_chosen);
}

void CRemoteControlStarNetPanel::onLogoffAll(wxCommandEvent& event)
{
	::wxGetApp().starNetLogoff(m_callsign, wxT("ALL     "));
}
