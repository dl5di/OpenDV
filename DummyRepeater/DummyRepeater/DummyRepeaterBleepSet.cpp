/*
 *   Copyright (C) 2010 by Jonathan Naylor G4KLX
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

#include "DummyRepeaterBleepSet.h"

const unsigned int BORDER_SIZE   = 5U;
const unsigned int CONTROL_WIDTH = 200U;

CDummyRepeaterBleepSet::CDummyRepeaterBleepSet(wxWindow* parent, int id, const wxString& title, bool bleep) :
wxPanel(parent, id),
m_title(title),
m_bleep(NULL)
{
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	m_bleep = new wxCheckBox(this, -1, _("End Bleep"));
	m_bleep->SetValue(bleep);
	sizer->Add(m_bleep, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	SetAutoLayout(true);

	sizer->Fit(this);
	sizer->SetSizeHints(this);

	SetSizer(sizer);
}


CDummyRepeaterBleepSet::~CDummyRepeaterBleepSet()
{
}

bool CDummyRepeaterBleepSet::getBleep() const
{
	return m_bleep->GetValue();
}
