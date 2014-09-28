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

#include "DummyRepeaterTimeoutSet.h"

const unsigned int BORDER_SIZE   = 5U;
const unsigned int CONTROL_WIDTH = 200U;

CDummyRepeaterTimeoutSet::CDummyRepeaterTimeoutSet(wxWindow* parent, int id, const wxString& title, unsigned int timeout) :
wxPanel(parent, id),
m_title(title),
m_timeout(NULL)
{
	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText* label = new wxStaticText(this, -1, _("Timeout (secs)"));
	sizer->Add(label, 0, wxALL, BORDER_SIZE);

	m_timeout = new wxSlider(this, -1, timeout, 0, 240, wxDefaultPosition, wxSize(CONTROL_WIDTH, -1), wxSL_HORIZONTAL | wxSL_LABELS);
	sizer->Add(m_timeout, 0, wxALL, BORDER_SIZE);

	SetAutoLayout(true);

	sizer->Fit(this);
	sizer->SetSizeHints(this);

	SetSizer(sizer);
}


CDummyRepeaterTimeoutSet::~CDummyRepeaterTimeoutSet()
{
}

unsigned int CDummyRepeaterTimeoutSet::getTimeout() const
{
	return m_timeout->GetValue();
}
