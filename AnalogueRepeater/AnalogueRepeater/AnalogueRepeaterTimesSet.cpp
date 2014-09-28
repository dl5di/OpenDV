/*
 *   Copyright (C) 2009,2010 by Jonathan Naylor G4KLX
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

#include "AnalogueRepeaterTimesSet.h"

const unsigned int TIMES_WIDTH = 300U;

const unsigned int BORDER_SIZE = 5U;

CAnalogueRepeaterTimesSet::CAnalogueRepeaterTimesSet(wxWindow* parent, int id, const wxString& title, unsigned int callsignTime, unsigned int timeout, unsigned int lockoutTime, unsigned int hangTime, unsigned int latchTime) :
wxPanel(parent, id),
m_title(title),
m_callsignTime(NULL),
m_timeout(NULL),
m_lockoutTime(NULL),
m_hangTime(NULL),
m_latchTime(NULL)
{
	wxFlexGridSizer* sizer = new wxFlexGridSizer(2);

	wxStaticText* callsignTimeLabel = new wxStaticText(this, -1, _("Callsign Time (mins)"));
	sizer->Add(callsignTimeLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_callsignTime = new wxSlider(this, -1, callsignTime / 60U, 5, 30, wxDefaultPosition, wxSize(TIMES_WIDTH, -1), wxSL_HORIZONTAL | wxSL_LABELS);
	sizer->Add(m_callsignTime, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* timeoutLabel = new wxStaticText(this, -1, _("Timeout (secs)"));
	sizer->Add(timeoutLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_timeout = new wxSlider(this, -1, timeout, 0, 300, wxDefaultPosition, wxSize(TIMES_WIDTH, -1), wxSL_HORIZONTAL | wxSL_LABELS);
	sizer->Add(m_timeout, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* lockoutTimeLabel = new wxStaticText(this, -1, _("Lockout Time (secs)"));
	sizer->Add(lockoutTimeLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_lockoutTime = new wxSlider(this, -1, lockoutTime, 0, 180, wxDefaultPosition, wxSize(TIMES_WIDTH, -1), wxSL_HORIZONTAL | wxSL_LABELS);
	sizer->Add(m_lockoutTime, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* hangTimeLabel = new wxStaticText(this, -1, _("Hang Time (secs)"));
	sizer->Add(hangTimeLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_hangTime = new wxSlider(this, -1, hangTime, 1, 60, wxDefaultPosition, wxSize(TIMES_WIDTH, -1), wxSL_HORIZONTAL | wxSL_LABELS);
	sizer->Add(m_hangTime, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* latchTimeLabel = new wxStaticText(this, -1, _("Latch Time (secs)"));
	sizer->Add(latchTimeLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_latchTime = new wxSlider(this, -1, latchTime, 0, 10, wxDefaultPosition, wxSize(TIMES_WIDTH, -1), wxSL_HORIZONTAL | wxSL_LABELS);
	sizer->Add(m_latchTime, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	SetAutoLayout(true);

	sizer->Fit(this);
	sizer->SetSizeHints(this);

	SetSizer(sizer);
}


CAnalogueRepeaterTimesSet::~CAnalogueRepeaterTimesSet()
{
}

bool CAnalogueRepeaterTimesSet::Validate()
{
	return true;
}

unsigned int CAnalogueRepeaterTimesSet::getCallsignTime() const
{
	return m_callsignTime->GetValue() * 60U;
}

unsigned int CAnalogueRepeaterTimesSet::getTimeout() const
{
	return m_timeout->GetValue();
}

unsigned int CAnalogueRepeaterTimesSet::getLockoutTime() const
{
	return m_lockoutTime->GetValue();
}

unsigned int CAnalogueRepeaterTimesSet::getHangTime() const
{
	return m_hangTime->GetValue();
}

unsigned int CAnalogueRepeaterTimesSet::getLatchTime() const
{
	return m_latchTime->GetValue();
}
