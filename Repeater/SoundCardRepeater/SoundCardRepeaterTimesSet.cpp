/*
 *   Copyright (C) 2009-2012 by Jonathan Naylor G4KLX
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

#include "SoundCardRepeaterTimesSet.h"

const unsigned int TIMES_WIDTH  = 300U;

const unsigned int BORDER_SIZE = 5U;

CSoundCardRepeaterTimesSet::CSoundCardRepeaterTimesSet(wxWindow* parent, int id, const wxString& title, unsigned int timeout, unsigned int ackTime, unsigned int hangTime) :
wxPanel(parent, id),
m_title(title),
m_timeout(NULL),
m_ackTime(NULL),
m_hangTime(NULL)
{
	wxFlexGridSizer* sizer = new wxFlexGridSizer(2);

	wxStaticText* timeoutLabel = new wxStaticText(this, -1, _("Timeout (secs)"));
	sizer->Add(timeoutLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_timeout = new wxSlider(this, -1, timeout, 0, 300, wxDefaultPosition, wxSize(TIMES_WIDTH, -1), wxSL_HORIZONTAL | wxSL_LABELS);
	sizer->Add(m_timeout, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* ackTimeLabel = new wxStaticText(this, -1, _("Ack Time (ms)"));
	sizer->Add(ackTimeLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	if (ackTime < 100U)
		ackTime = 500U;

	m_ackTime = new wxSlider(this, -1, ackTime, 100, 2000, wxDefaultPosition, wxSize(TIMES_WIDTH, -1), wxSL_HORIZONTAL | wxSL_LABELS);
	sizer->Add(m_ackTime, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* hangTimeLabel = new wxStaticText(this, -1, _("Hang Time (ms)"));
	sizer->Add(hangTimeLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_hangTime = new wxSlider(this, -1, hangTime, 0, 3000, wxDefaultPosition, wxSize(TIMES_WIDTH, -1), wxSL_HORIZONTAL | wxSL_LABELS);
	sizer->Add(m_hangTime, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	SetAutoLayout(true);

	sizer->Fit(this);
	sizer->SetSizeHints(this);

	SetSizer(sizer);
}


CSoundCardRepeaterTimesSet::~CSoundCardRepeaterTimesSet()
{
}

bool CSoundCardRepeaterTimesSet::Validate()
{
	return true;
}

unsigned int CSoundCardRepeaterTimesSet::getTimeout() const
{
	return m_timeout->GetValue();
}

unsigned int CSoundCardRepeaterTimesSet::getAckTime() const
{
	return m_ackTime->GetValue();
}

unsigned int CSoundCardRepeaterTimesSet::getHangTime() const
{
	return m_hangTime->GetValue();
}
