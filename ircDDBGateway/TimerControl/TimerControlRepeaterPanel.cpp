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

#include "TimerControlRepeaterPanel.h"
#include "TimerControlApp.h"
#include "DStarDefines.h"
#include "HostFile.h"

#include <cstdlib>

#include <wx/filename.h>

enum {
	List_Select = 7100,
	Button_Add,
	Button_Modify,
	Button_Delete
};

BEGIN_EVENT_TABLE(CTimerControlRepeaterPanel, wxPanel)
	EVT_LIST_ITEM_SELECTED(List_Select, CTimerControlRepeaterPanel::onSelect)

	EVT_BUTTON(Button_Add,    CTimerControlRepeaterPanel::onAdd)
	EVT_BUTTON(Button_Modify, CTimerControlRepeaterPanel::onModify)
	EVT_BUTTON(Button_Delete, CTimerControlRepeaterPanel::onDelete)
END_EVENT_TABLE()

#if defined(__WINDOWS__)
const unsigned int LIST_HEIGHT     = 350U;
const unsigned int LIST_WIDTH      = 350U;
const unsigned int DAY_WIDTH       = 75U;
const unsigned int TIME_WIDTH      = 60U;
const unsigned int CALLSIGN_WIDTH  = 100U;
const unsigned int TYPE_WIDTH      = 85U;
const unsigned int BUTTON_WIDTH    = 75U;
const unsigned int HOUR_WIDTH      = 40U;
const unsigned int MINUTE_WIDTH    = 40U;
const unsigned int REFLECTOR_WIDTH = 80U;
const unsigned int CHANNEL_WIDTH   = 40U;
const unsigned int RECONNECT_WIDTH = 75U;
#else
const unsigned int LIST_HEIGHT     = 350U;
const unsigned int LIST_WIDTH      = 350U;
const unsigned int DAY_WIDTH       = 100U;
const unsigned int TIME_WIDTH      = 100U;
const unsigned int CALLSIGN_WIDTH  = 100U;
const unsigned int TYPE_WIDTH      = 65U;
const unsigned int BUTTON_WIDTH    = 75U;
const unsigned int HOUR_WIDTH      = 50U;
const unsigned int MINUTE_WIDTH    = 50U;
const unsigned int REFLECTOR_WIDTH = 90U;
const unsigned int CHANNEL_WIDTH   = 50U;
const unsigned int RECONNECT_WIDTH = 100U;
#endif

const unsigned int BORDER_SIZE = 5U;

int itemCompare(const void* a, const void* b)
{
	const CTimerControlItem** first  = (const CTimerControlItem**)a;
	const CTimerControlItem** second = (const CTimerControlItem**)b;

	if ((*first)->m_day != (*second)->m_day)
		return int((*second)->m_day) - int((*first)->m_day);

	if ((*first)->m_hour != (*second)->m_hour)
		return int((*second)->m_hour) - int((*first)->m_hour);

	return int((*second)->m_minute) - int((*first)->m_minute);
}

CTimerControlRepeaterPanel::CTimerControlRepeaterPanel(wxWindow* parent, int id, const wxString& callsign) :
wxPanel(parent, id),
m_callsign(callsign),
m_list(NULL),
m_day(NULL),
m_hour(NULL),
m_minute(NULL),
m_reflector(NULL),
m_channel(NULL),
m_reconnect(NULL),
m_item(NULL),
m_n(0)
{
	wxBoxSizer* sizer1 = new wxBoxSizer(wxHORIZONTAL);

	m_list = new wxListCtrl(this, List_Select, wxDefaultPosition, wxSize(LIST_WIDTH, LIST_HEIGHT), wxLC_REPORT | wxLC_SINGLE_SEL);
	m_list->InsertColumn(0L, _("Day"));
	m_list->SetColumnWidth(0L, DAY_WIDTH);
	m_list->InsertColumn(1L, _("Time"));
	m_list->SetColumnWidth(1L, TIME_WIDTH);
	m_list->InsertColumn(2L, _("Type"));
	m_list->SetColumnWidth(2L, TYPE_WIDTH);
	m_list->InsertColumn(3L, _("Reflector"));
	m_list->SetColumnWidth(3L, REFLECTOR_WIDTH);
	sizer1->Add(m_list, 0, wxTOP | wxBOTTOM | wxLEFT | wxEXPAND, BORDER_SIZE);

	wxBoxSizer* sizer2 = new wxBoxSizer(wxVERTICAL);

	m_day = new wxChoice(this, -1, wxDefaultPosition, wxSize(DAY_WIDTH, -1));
	m_day->Append(_("Sunday"));
	m_day->Append(_("Monday"));
	m_day->Append(_("Tuesday"));
	m_day->Append(_("Wednesday"));
	m_day->Append(_("Thursday"));
	m_day->Append(_("Friday"));
	m_day->Append(_("Saturday"));
	m_day->Append(_("Every day"));
	m_day->Append(_("Mon-Fri"));
	m_day->Append(_("Sat-Sun"));
	sizer2->Add(m_day, 0, wxALL, BORDER_SIZE);
	m_day->SetSelection(0);

	wxBoxSizer* sizer2a = new wxBoxSizer(wxHORIZONTAL);

	m_hour = new wxChoice(this, -1, wxDefaultPosition, wxSize(HOUR_WIDTH, -1));
	m_hour->Append(wxT("00"));
	m_hour->Append(wxT("01"));
	m_hour->Append(wxT("02"));
	m_hour->Append(wxT("03"));
	m_hour->Append(wxT("04"));
	m_hour->Append(wxT("05"));
	m_hour->Append(wxT("06"));
	m_hour->Append(wxT("07"));
	m_hour->Append(wxT("08"));
	m_hour->Append(wxT("09"));
	m_hour->Append(wxT("10"));
	m_hour->Append(wxT("11"));
	m_hour->Append(wxT("12"));
	m_hour->Append(wxT("13"));
	m_hour->Append(wxT("14"));
	m_hour->Append(wxT("15"));
	m_hour->Append(wxT("16"));
	m_hour->Append(wxT("17"));
	m_hour->Append(wxT("18"));
	m_hour->Append(wxT("19"));
	m_hour->Append(wxT("20"));
	m_hour->Append(wxT("21"));
	m_hour->Append(wxT("22"));
	m_hour->Append(wxT("23"));
	sizer2a->Add(m_hour, 0, wxALL, BORDER_SIZE);
	m_hour->SetSelection(0);

	wxStaticText* dummy1Label = new wxStaticText(this, -1, wxT(":"));
	sizer2a->Add(dummy1Label, 0, wxALL, BORDER_SIZE);

	m_minute = new wxChoice(this, -1, wxDefaultPosition, wxSize(MINUTE_WIDTH, -1));
	m_minute->Append(wxT("00"));
	m_minute->Append(wxT("05"));
	m_minute->Append(wxT("10"));
	m_minute->Append(wxT("15"));
	m_minute->Append(wxT("20"));
	m_minute->Append(wxT("25"));
	m_minute->Append(wxT("30"));
	m_minute->Append(wxT("35"));
	m_minute->Append(wxT("40"));
	m_minute->Append(wxT("45"));
	m_minute->Append(wxT("50"));
	m_minute->Append(wxT("55"));
	sizer2a->Add(m_minute, 0, wxALL, BORDER_SIZE);
	m_minute->SetSelection(0);

	sizer2->Add(sizer2a);

	wxStaticText* dummy2Label = new wxStaticText(this, -1, wxEmptyString);
	sizer2->Add(dummy2Label, 0, wxALL, BORDER_SIZE);

	wxBoxSizer* sizer3 = new wxBoxSizer(wxHORIZONTAL);

	m_reflector = new wxChoice(this, -1, wxDefaultPosition, wxSize(REFLECTOR_WIDTH, -1));
	m_reflector->Append(_("None"));

	wxFileName fileName1(wxFileName::GetHomeDir(), DPLUS_HOSTS_FILE_NAME);
	if (fileName1.IsFileReadable()) {
		CHostFile file(fileName1.GetFullPath(), false);

		for (unsigned int i = 0U; i < file.getCount(); i++)
			m_reflector->Append(file.getName(i).Trim());
	}

#if defined(__WINDOWS__)
	wxFileName fileName4(::wxGetCwd(), DPLUS_HOSTS_FILE_NAME);
#else
	wxFileName fileName4(wxT(DATA_DIR), DPLUS_HOSTS_FILE_NAME);
#endif
	if (fileName4.IsFileReadable()) {
		CHostFile file(fileName4.GetFullPath(), false);

		for (unsigned int i = 0U; i < file.getCount(); i++) {
			wxString name = file.getName(i).Trim();
			if (m_reflector->FindString(name) == wxNOT_FOUND)
				m_reflector->Append(name);
		}
	}

	wxFileName fileName2(wxFileName::GetHomeDir(), DEXTRA_HOSTS_FILE_NAME);
	if (fileName2.IsFileReadable()) {
		CHostFile file(fileName2.GetFullPath(), false);

		for (unsigned int i = 0U; i < file.getCount(); i++)
			m_reflector->Append(file.getName(i).Trim());
	}

#if defined(__WINDOWS__)
	wxFileName fileName5(::wxGetCwd(), DEXTRA_HOSTS_FILE_NAME);
#else
	wxFileName fileName5(wxT(DATA_DIR), DEXTRA_HOSTS_FILE_NAME);
#endif
	if (fileName5.IsFileReadable()) {
		CHostFile file(fileName5.GetFullPath(), false);

		for (unsigned int i = 0U; i < file.getCount(); i++) {
			wxString name = file.getName(i).Trim();
			if (m_reflector->FindString(name) == wxNOT_FOUND)
				m_reflector->Append(name);
		}
	}

	wxFileName fileName3(wxFileName::GetHomeDir(), DCS_HOSTS_FILE_NAME);
	if (fileName3.IsFileReadable()) {
		CHostFile file(fileName3.GetFullPath(), false);

		for (unsigned int i = 0U; i < file.getCount(); i++)
			m_reflector->Append(file.getName(i).Trim());
	}

#if defined(__WINDOWS__)
	wxFileName fileName6(::wxGetCwd(), DCS_HOSTS_FILE_NAME);
#else
	wxFileName fileName6(wxT(DATA_DIR), DCS_HOSTS_FILE_NAME);
#endif
	if (fileName6.IsFileReadable()) {
		CHostFile file(fileName6.GetFullPath(), false);

		for (unsigned int i = 0U; i < file.getCount(); i++) {
			wxString name = file.getName(i).Trim();
			if (m_reflector->FindString(name) == wxNOT_FOUND)
				m_reflector->Append(name);
		}
	}

	sizer3->Add(m_reflector, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);
	m_reflector->SetSelection(0);

	m_channel = new wxChoice(this, -1, wxDefaultPosition, wxSize(CHANNEL_WIDTH, -1));
	m_channel->Append(wxT("A"));
	m_channel->Append(wxT("B"));
	m_channel->Append(wxT("C"));
	m_channel->Append(wxT("D"));
	m_channel->Append(wxT("E"));
	m_channel->Append(wxT("F"));
	m_channel->Append(wxT("G"));
	m_channel->Append(wxT("H"));
	m_channel->Append(wxT("I"));
	m_channel->Append(wxT("J"));
	m_channel->Append(wxT("K"));
	m_channel->Append(wxT("L"));
	m_channel->Append(wxT("M"));
	m_channel->Append(wxT("N"));
	m_channel->Append(wxT("O"));
	m_channel->Append(wxT("P"));
	m_channel->Append(wxT("Q"));
	m_channel->Append(wxT("R"));
	m_channel->Append(wxT("S"));
	m_channel->Append(wxT("T"));
	m_channel->Append(wxT("U"));
	m_channel->Append(wxT("V"));
	m_channel->Append(wxT("W"));
	m_channel->Append(wxT("X"));
	m_channel->Append(wxT("Y"));
	m_channel->Append(wxT("Z"));
	sizer3->Add(m_channel, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);
	m_channel->SetSelection(0);

	sizer2->Add(sizer3);

	m_reconnect = new wxChoice(this, -1, wxDefaultPosition, wxSize(RECONNECT_WIDTH, -1));
	m_reconnect->Append(_("Never"));
	m_reconnect->Append(_("Fixed"));
	m_reconnect->Append(_("5 minutes"));
	m_reconnect->Append(_("10 minutes"));
	m_reconnect->Append(_("15 minutes"));
	m_reconnect->Append(_("20 minutes"));
	m_reconnect->Append(_("25 minutes"));
	m_reconnect->Append(_("30 minutes"));
	m_reconnect->Append(_("60 minutes"));
	m_reconnect->Append(_("90 minutes"));
	m_reconnect->Append(_("120 minutes"));
	m_reconnect->Append(_("180 minutes"));
	sizer2->Add(m_reconnect, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);
	m_reconnect->SetSelection(0);

	wxStaticText* dummy3Label = new wxStaticText(this, -1, wxEmptyString);
	sizer2->Add(dummy3Label, 0, wxALL, BORDER_SIZE);

	wxButton* addButton = new wxButton(this, Button_Add, _("Add"), wxDefaultPosition, wxSize(BUTTON_WIDTH, -1));
	sizer2->Add(addButton, 0, wxALL, BORDER_SIZE);

	wxButton* modButton = new wxButton(this, Button_Modify, _("Modify"), wxDefaultPosition, wxSize(BUTTON_WIDTH, -1));
	sizer2->Add(modButton, 0, wxALL, BORDER_SIZE);

	wxButton* delButton = new wxButton(this, Button_Delete, _("Delete"), wxDefaultPosition, wxSize(BUTTON_WIDTH, -1));
	sizer2->Add(delButton, 0, wxALL, BORDER_SIZE);

	sizer1->Add(sizer2);

	SetAutoLayout(true);

	SetSizer(sizer1);
}


CTimerControlRepeaterPanel::~CTimerControlRepeaterPanel()
{
}

void CTimerControlRepeaterPanel::load(CTimerControlItem** items, unsigned int count)
{
	for (unsigned int i = 0U; i < count; i++) {
		if (items[i]->m_repeater.IsSameAs(m_callsign))
			insertItem(items[i]);
	}

	reload();
}

unsigned int CTimerControlRepeaterPanel::getCount() const
{
	return (unsigned int)m_list->GetItemCount();
}

CTimerControlItem** CTimerControlRepeaterPanel::getItems() const
{
	unsigned int count = getCount();

	CTimerControlItem** items = new CTimerControlItem*[count];

	for (unsigned int i = 0U; i < count; i++)
		items[i] = (CTimerControlItem*)m_list->GetItemData(i);

	return items;
}

void CTimerControlRepeaterPanel::insertItem(CTimerControlItem* item)
{
	switch (item->m_day) {
		case 0U: m_list->InsertItem(0L, _("Sunday")); break;
		case 1U: m_list->InsertItem(0L, _("Monday")); break;
		case 2U: m_list->InsertItem(0L, _("Tuesday")); break;
		case 3U: m_list->InsertItem(0L, _("Wednesday")); break;
		case 4U: m_list->InsertItem(0L, _("Thursday")); break;
		case 5U: m_list->InsertItem(0L, _("Friday")); break;
		case 6U: m_list->InsertItem(0L, _("Saturday")); break;
		case 7U: m_list->InsertItem(0L, _("Every day")); break;
		case 8U: m_list->InsertItem(0L, _("Mon-Fri")); break;
		case 9U: m_list->InsertItem(0L, _("Sat-Sun")); break;
		default: m_list->InsertItem(0L, wxT("??????")); break;
	}

	wxString text;
	text.Printf(wxT("%02u:%02u"), item->m_hour, item->m_minute);
	m_list->SetItem(0L, 1, text);

	switch (item->m_reconnect) {
		case RECONNECT_NEVER:   m_list->SetItem(0L, 2, _("Never")); break;
		case RECONNECT_FIXED:   m_list->SetItem(0L, 2, _("Fixed")); break;
		case RECONNECT_5MINS:   m_list->SetItem(0L, 2, _("5 minutes")); break;
		case RECONNECT_10MINS:  m_list->SetItem(0L, 2, _("10 minutes")); break;
		case RECONNECT_15MINS:  m_list->SetItem(0L, 2, _("15 minutes")); break;
		case RECONNECT_20MINS:  m_list->SetItem(0L, 2, _("20 minutes")); break;
		case RECONNECT_25MINS:  m_list->SetItem(0L, 2, _("25 minutes")); break;
		case RECONNECT_30MINS:  m_list->SetItem(0L, 2, _("30 minutes")); break;
		case RECONNECT_60MINS:  m_list->SetItem(0L, 2, _("60 minutes")); break;
		case RECONNECT_90MINS:  m_list->SetItem(0L, 2, _("90 minutes")); break;
		case RECONNECT_120MINS: m_list->SetItem(0L, 2, _("120 minutes")); break;
		case RECONNECT_180MINS: m_list->SetItem(0L, 2, _("180 minutes")); break;
		default:                m_list->SetItem(0L, 2, wxT("????????")); break;
	}

	if (item->m_reflector.IsEmpty())
		m_list->SetItem(0L, 3, _("None"));
	else
		m_list->SetItem(0L, 3, item->m_reflector);

	m_list->SetItemPtrData(0L, wxUIntPtr(item));
}

void CTimerControlRepeaterPanel::onSelect(wxListEvent& event)
{
	m_n    = event.GetIndex();
	m_item = (CTimerControlItem*)m_list->GetItemData(m_n);
	if (m_item == NULL)
		return;

	m_day->SetSelection(int(m_item->m_day));
	m_hour->SetSelection(int(m_item->m_hour));
	m_minute->SetSelection(int(m_item->m_minute / 5U));

	if (m_item->m_reflector.IsEmpty()) {
		m_reflector->SetSelection(0);
		m_channel->SetSelection(0);
	} else {
		m_reflector->SetStringSelection(m_item->m_reflector.Left(7U).Trim());
		m_channel->SetStringSelection(m_item->m_reflector.Mid(7U).Trim());
	}

	m_reconnect->SetSelection(int(m_item->m_reconnect));
}

void CTimerControlRepeaterPanel::onAdd(wxCommandEvent&)
{
	int day       = m_day->GetSelection();
	int hour      = m_hour->GetSelection();
	int minute    = m_minute->GetSelection();
	int reflector = m_reflector->GetSelection();
	int channel   = m_channel->GetSelection();
	int reconnect = m_reconnect->GetSelection();

	if (day == wxNOT_FOUND || hour == wxNOT_FOUND || minute == wxNOT_FOUND || reflector == wxNOT_FOUND || channel == wxNOT_FOUND || reconnect == wxNOT_FOUND)
		return;

	CTimerControlItem* item = new CTimerControlItem;

	item->m_repeater  = m_callsign;
	item->m_day       = (unsigned int)day;
	item->m_hour      = (unsigned int)hour;
	item->m_minute    = (unsigned int)minute * 5U;

	if (reflector != 0) {
		wxString callsign = m_reflector->GetStringSelection();
		callsign.Append(wxT("        "));
		callsign.Truncate(7U);
		callsign.Append(m_channel->GetStringSelection());
		item->m_reflector = callsign;
	}

	item->m_reconnect = RECONNECT(reconnect);

	m_day->SetSelection(0);
	m_hour->SetSelection(0);
	m_minute->SetSelection(0);
	m_reflector->SetSelection(0);
	m_channel->SetSelection(0);
	m_reconnect->SetSelection(0);

	reload(item);

	::wxGetApp().writeItems();
}

void CTimerControlRepeaterPanel::onModify(wxCommandEvent&)
{
	if (m_item == NULL)
		return;

	int day       = m_day->GetSelection();
	int hour      = m_hour->GetSelection();
	int minute    = m_minute->GetSelection();
	int reflector = m_reflector->GetSelection();
	int channel   = m_channel->GetSelection();
	int reconnect = m_reconnect->GetSelection();

	if (day == wxNOT_FOUND || hour == wxNOT_FOUND || minute == wxNOT_FOUND || reflector == wxNOT_FOUND || channel == wxNOT_FOUND || reconnect == wxNOT_FOUND)
		return;

	CTimerControlItem* item = new CTimerControlItem;

	item->m_repeater  = m_callsign;
	item->m_day       = (unsigned int)day;
	item->m_hour      = (unsigned int)hour;
	item->m_minute    = (unsigned int)minute * 5U;

	if (reflector != 0) {
		wxString callsign = m_reflector->GetStringSelection();
		callsign.Append(wxT("        "));
		callsign.Truncate(7U);
		callsign.Append(m_channel->GetStringSelection());
		item->m_reflector = callsign;
	}

	item->m_reconnect = RECONNECT(reconnect);

	m_list->DeleteItem(m_n);

	delete m_item;
	m_item = NULL;

	m_day->SetSelection(0);
	m_hour->SetSelection(0);
	m_minute->SetSelection(0);
	m_reflector->SetSelection(0);
	m_channel->SetSelection(0);
	m_reconnect->SetSelection(0);

	reload(item);

	::wxGetApp().writeItems();
}

void CTimerControlRepeaterPanel::onDelete(wxCommandEvent&)
{
	if (m_item == NULL)
		return;

	m_list->DeleteItem(m_n);

	delete m_item;
	m_item = NULL;

	m_day->SetSelection(0);
	m_hour->SetSelection(0);
	m_minute->SetSelection(0);
	m_reflector->SetSelection(0);
	m_channel->SetSelection(0);
	m_reconnect->SetSelection(0);

	::wxGetApp().writeItems();
}

void CTimerControlRepeaterPanel::reload(CTimerControlItem* item)
{
	int n = m_list->GetItemCount();

	CTimerControlItem** items = new CTimerControlItem*[n + 1];

	unsigned int count = 0U;
	for (int i = 0; i < n; i++) {
		CTimerControlItem* listItem = (CTimerControlItem*)m_list->GetItemData(i);

		// Reject any that have the same day, hour and minute settings
		if (item != NULL) {
			// Match the exact day, hour, and minute
			if (listItem->m_day == item->m_day && listItem->m_hour == item->m_hour && listItem->m_minute == item->m_minute) {
				delete listItem;
			// Match every day, hour, and minute
			} else if (7U == item->m_day && listItem->m_hour == item->m_hour && listItem->m_minute == item->m_minute) {
				delete listItem;
			// Match every day, hour, and minute
			} else if (listItem->m_day == 7U && listItem->m_hour == item->m_hour && listItem->m_minute == item->m_minute) {
				delete listItem;
			// Match week day, hour, and minute
			} else if (8U == item->m_day && ((listItem->m_day >= 1U && listItem->m_day <= 5U) || listItem->m_day == 7U) && listItem->m_hour == item->m_hour && listItem->m_minute == item->m_minute) {
				delete listItem;
			// Match week day, hour, and minute
			} else if (listItem->m_day == 8U && ((item->m_day >= 1U && item->m_day <= 5U) || item->m_day == 7U) && listItem->m_hour == item->m_hour && listItem->m_minute == item->m_minute) {
				delete listItem;
			// Match weekend, hour, and minute
			} else if (9U == item->m_day && (listItem->m_day == 0U || listItem->m_day == 6U || listItem->m_day == 7U) && listItem->m_hour == item->m_hour && listItem->m_minute == item->m_minute) {
				delete listItem;
			// Match weekend, hour, and minute
			} else if (listItem->m_day == 9U && (item->m_day == 0U || item->m_day == 6U || item->m_day == 7U) && listItem->m_hour == item->m_hour && listItem->m_minute == item->m_minute) {
				delete listItem;
			} else {
				items[count] = listItem;
				count++;
			}
		} else {
			items[count] = listItem;
			count++;
		}
	}

	// Put the new one at the end of the list
	if (item != NULL) {
		items[count] = item;
		count++;
	}

	m_list->DeleteAllItems();

	::qsort(items, count, sizeof(CTimerControlItem*), itemCompare);

	for (unsigned int i = 0U; i < count; i++)
		insertItem(items[i]);

	delete[] items;
}
