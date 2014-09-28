/*
 *   Copyright (C) 2010,2014 by Jonathan Naylor G4KLX
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

#include "DStarRepeaterConfigActiveHangSet.h"

const unsigned int TIMED_WIDTH = 300U;

const unsigned int BORDER_SIZE = 5U;

CDStarRepeaterConfigActiveHangSet::CDStarRepeaterConfigActiveHangSet(wxWindow* parent, int id, const wxString& title, unsigned int time) :
wxPanel(parent, id),
m_title(title),
m_time(NULL)
{
	wxFlexGridSizer* sizer = new wxFlexGridSizer(2);

	wxStaticText* timeLabel = new wxStaticText(this, -1, _("Time (secs)"));
	sizer->Add(timeLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_time = new wxSlider(this, -1, time, 0, 300, wxDefaultPosition, wxSize(TIMED_WIDTH, -1), wxSL_HORIZONTAL | wxSL_LABELS);
	sizer->Add(m_time, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	SetAutoLayout(true);

	sizer->Fit(this);
	sizer->SetSizeHints(this);

	SetSizer(sizer);
}


CDStarRepeaterConfigActiveHangSet::~CDStarRepeaterConfigActiveHangSet()
{
}

bool CDStarRepeaterConfigActiveHangSet::Validate()
{
	return true;
}

unsigned int CDStarRepeaterConfigActiveHangSet::getTime() const
{
	return m_time->GetValue();
}
