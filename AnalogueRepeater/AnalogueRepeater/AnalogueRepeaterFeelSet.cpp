/*
 *   Copyright (C) 2009,2010,2013 by Jonathan Naylor G4KLX
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

#include "AnalogueRepeaterFeelSet.h"

const unsigned int CONTROL_SIZE = 175U;
const unsigned int TIMES_WIDTH  = 300U;

const unsigned int BORDER_SIZE = 5U;

CAnalogueRepeaterFeelSet::CAnalogueRepeaterFeelSet(wxWindow* parent, int id, const wxString& title, ANALOGUE_CALLSIGN_START callAtStart, unsigned int startDelay, bool callAtEnd, ANALOGUE_TIMEOUT_TYPE timeoutType, ANALOGUE_CALLSIGN_HOLDOFF holdoff) :
wxPanel(parent, id),
m_title(title),
m_callAtStart(NULL),
m_startDelay(NULL),
m_callAtEnd(NULL),
m_timeoutType(NULL),
m_holdoff(NULL)
{
	wxFlexGridSizer* sizer = new wxFlexGridSizer(2);

	wxStaticText* callAtStartLabel = new wxStaticText(this, -1, _("Start Callsign"));
	sizer->Add(callAtStartLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_callAtStart = new wxChoice(this, -1, wxDefaultPosition, wxSize(CONTROL_SIZE, -1));
	m_callAtStart->Append(_("Disabled"));
	m_callAtStart->Append(_("At Opening"));
	m_callAtStart->Append(_("On Latch"));
	sizer->Add(m_callAtStart, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);
	m_callAtStart->SetSelection(int(callAtStart));

	wxStaticText* startDelayLabel = new wxStaticText(this, -1, _("Start Delay (s)"));
	sizer->Add(startDelayLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_startDelay = new wxSlider(this, -1, startDelay, 0, 5, wxDefaultPosition, wxSize(TIMES_WIDTH, -1), wxSL_HORIZONTAL | wxSL_LABELS);
	sizer->Add(m_startDelay, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* callAtEndLabel = new wxStaticText(this, -1, _("End Callsign"));
	sizer->Add(callAtEndLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_callAtEnd = new wxChoice(this, -1, wxDefaultPosition, wxSize(CONTROL_SIZE, -1));
	m_callAtEnd->Append(_("Disabled"));
	m_callAtEnd->Append(_("Enabled"));
	sizer->Add(m_callAtEnd, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);
	m_callAtEnd->SetSelection(callAtEnd ? 1 : 0);

	wxStaticText* timeoutTypeLabel = new wxStaticText(this, -1, _("Timeout Type"));
	sizer->Add(timeoutTypeLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_timeoutType = new wxChoice(this, -1, wxDefaultPosition, wxSize(CONTROL_SIZE, -1));
	m_timeoutType->Append(_("German"));
	m_timeoutType->Append(_("UK"));
	m_timeoutType->Append(_("USA"));
	sizer->Add(m_timeoutType, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);
	m_timeoutType->SetSelection(int(timeoutType));

	wxStaticText* holdoffLabel = new wxStaticText(this, -1, _("Callsign Holdoff"));
	sizer->Add(holdoffLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_holdoff = new wxChoice(this, -1, wxDefaultPosition, wxSize(CONTROL_SIZE, -1));
	m_holdoff->Append(_("None"));
	m_holdoff->Append(_("1/4 of Callsign Timer"));
	m_holdoff->Append(_("1/2 of Callsign Timer"));
	m_holdoff->Append(_("3/4 of Callsign Timer"));
	sizer->Add(m_holdoff, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);
	m_holdoff->SetSelection(int(holdoff));

	SetAutoLayout(true);

	sizer->Fit(this);
	sizer->SetSizeHints(this);

	SetSizer(sizer);
}


CAnalogueRepeaterFeelSet::~CAnalogueRepeaterFeelSet()
{
}

bool CAnalogueRepeaterFeelSet::Validate()
{
	if (m_callAtStart->GetCurrentSelection() == wxNOT_FOUND)
		return false;

	if (m_callAtEnd->GetCurrentSelection() == wxNOT_FOUND)
		return false;

	if (m_timeoutType->GetCurrentSelection() == wxNOT_FOUND)
		return false;

	return m_holdoff->GetCurrentSelection() != wxNOT_FOUND;
}

ANALOGUE_CALLSIGN_START CAnalogueRepeaterFeelSet::getCallAtStart() const
{
	return ANALOGUE_CALLSIGN_START(m_callAtStart->GetCurrentSelection());
}

unsigned int CAnalogueRepeaterFeelSet::getStartDelay() const
{
	return m_startDelay->GetValue();
}

bool CAnalogueRepeaterFeelSet::getCallAtEnd() const
{
	return m_callAtEnd->GetCurrentSelection() == 1;
}

ANALOGUE_TIMEOUT_TYPE CAnalogueRepeaterFeelSet::getTimeoutType() const
{
	return ANALOGUE_TIMEOUT_TYPE(m_timeoutType->GetCurrentSelection());
}

ANALOGUE_CALLSIGN_HOLDOFF CAnalogueRepeaterFeelSet::getHoldoff() const
{
	return ANALOGUE_CALLSIGN_HOLDOFF(m_holdoff->GetCurrentSelection());
}
