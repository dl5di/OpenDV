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

#include "MessageSet.h"

const unsigned int MAX_MESSAGE_LENGTH = 20U;

const unsigned int CONTROL_SIZE = 200U;
const unsigned int BORDER_SIZE  = 5U;

CMessageSet::CMessageSet(wxWindow* parent, int id, const wxString& title, const wxString& message) :
wxPanel(parent, id),
m_title(title),
m_message(NULL)
{
	wxFlexGridSizer* sizer = new wxFlexGridSizer(2);

	wxStaticText* messageText = new wxStaticText(this, -1, _("Message"));
	sizer->Add(messageText, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	m_message = new CMessageTextCtrl(this, -1, message, wxDefaultPosition, wxSize(CONTROL_SIZE, -1));
	m_message->SetMaxLength(MAX_MESSAGE_LENGTH);
	sizer->Add(m_message, 0, wxALL, BORDER_SIZE);

	SetAutoLayout(true);

	sizer->Fit(this);
	sizer->SetSizeHints(this);

	SetSizer(sizer);
}


CMessageSet::~CMessageSet()
{
}

wxString CMessageSet::getMessage() const
{
	return m_message->GetValue();
}
