/*
 *   Copyright (C) 2013,2014 by Jonathan Naylor G4KLX
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

#include "DStarRepeaterConfigAnnouncementSet.h"

const unsigned int CONTROL_SIZE = 100U;
const unsigned int TIMES_WIDTH  = 300U;
const unsigned int BORDER_SIZE  = 5U;

CDStarRepeaterConfigAnnouncementSet::CDStarRepeaterConfigAnnouncementSet(wxWindow* parent, int id, const wxString& title, bool enabled, unsigned int time, const wxString& recordRPT1, const wxString& recordRPT2, const wxString& deleteRPT1, const wxString& deleteRPT2) :
wxPanel(parent, id),
m_title(title),
m_enabled(NULL),
m_time(NULL),
m_recordRPT1(NULL),
m_recordRPT2(NULL),
m_deleteRPT1(NULL),
m_deleteRPT2(NULL)
{
	wxFlexGridSizer* sizer = new wxFlexGridSizer(2);

	wxStaticText* enabledText = new wxStaticText(this, -1, _("Enabled"));
	sizer->Add(enabledText, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	m_enabled = new wxChoice(this, -1, wxDefaultPosition, wxSize(CONTROL_SIZE, -1));
	m_enabled->Append(_("Disabled"));
	m_enabled->Append(_("Enabled"));
	sizer->Add(m_enabled, 0, wxALL, BORDER_SIZE);
	m_enabled->SetSelection(enabled ? 1 : 0);

	wxStaticText* timeLabel = new wxStaticText(this, -1, _("Time (mins)"));
	sizer->Add(timeLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_time = new wxSlider(this, -1, time / 60U, 0, 30, wxDefaultPosition, wxSize(TIMES_WIDTH, -1), wxSL_HORIZONTAL | wxSL_LABELS);
	sizer->Add(m_time, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* recordRPT1Text = new wxStaticText(this, -1, _("Record RPT1"));
	sizer->Add(recordRPT1Text, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	m_recordRPT1 = new CCallsignTextCtrl(this, -1, recordRPT1, wxDefaultPosition, wxSize(CONTROL_SIZE, -1));
	m_recordRPT1->SetMaxLength(LONG_CALLSIGN_LENGTH);
	sizer->Add(m_recordRPT1, 0, wxALL, BORDER_SIZE);

	wxStaticText* recordRPT2Text = new wxStaticText(this, -1, _("Record RPT2"));
	sizer->Add(recordRPT2Text, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	m_recordRPT2 = new CCallsignTextCtrl(this, -1, recordRPT2, wxDefaultPosition, wxSize(CONTROL_SIZE, -1));
	m_recordRPT2->SetMaxLength(LONG_CALLSIGN_LENGTH);
	sizer->Add(m_recordRPT2, 0, wxALL, BORDER_SIZE);

	wxStaticText* deleteRPT1Text = new wxStaticText(this, -1, _("Delete RPT1"));
	sizer->Add(deleteRPT1Text, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	m_deleteRPT1 = new CCallsignTextCtrl(this, -1, deleteRPT1, wxDefaultPosition, wxSize(CONTROL_SIZE, -1));
	m_deleteRPT1->SetMaxLength(LONG_CALLSIGN_LENGTH);
	sizer->Add(m_deleteRPT1, 0, wxALL, BORDER_SIZE);

	wxStaticText* deleteRPT2Text = new wxStaticText(this, -1, _("Delete RPT2"));
	sizer->Add(deleteRPT2Text, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	m_deleteRPT2 = new CCallsignTextCtrl(this, -1, deleteRPT2, wxDefaultPosition, wxSize(CONTROL_SIZE, -1));
	m_deleteRPT2->SetMaxLength(LONG_CALLSIGN_LENGTH);
	sizer->Add(m_deleteRPT2, 0, wxALL, BORDER_SIZE);

	SetAutoLayout(true);

	sizer->Fit(this);
	sizer->SetSizeHints(this);

	SetSizer(sizer);
}


CDStarRepeaterConfigAnnouncementSet::~CDStarRepeaterConfigAnnouncementSet()
{
}

bool CDStarRepeaterConfigAnnouncementSet::Validate()
{
	return m_enabled->GetCurrentSelection() != wxNOT_FOUND;
}

bool CDStarRepeaterConfigAnnouncementSet::getEnabled() const
{
	int n = m_enabled->GetCurrentSelection();
	if (n == wxNOT_FOUND)
		return false;

	return n == 1;
}

unsigned int CDStarRepeaterConfigAnnouncementSet::getTime() const
{
	return m_time->GetValue() * 60U;
}

wxString CDStarRepeaterConfigAnnouncementSet::getRecordRPT1() const
{
	return m_recordRPT1->GetValue().MakeUpper();
}

wxString CDStarRepeaterConfigAnnouncementSet::getRecordRPT2() const
{
	return m_recordRPT2->GetValue().MakeUpper();
}

wxString CDStarRepeaterConfigAnnouncementSet::getDeleteRPT1() const
{
	return m_deleteRPT1->GetValue().MakeUpper();
}

wxString CDStarRepeaterConfigAnnouncementSet::getDeleteRPT2() const
{
	return m_deleteRPT2->GetValue().MakeUpper();
}
