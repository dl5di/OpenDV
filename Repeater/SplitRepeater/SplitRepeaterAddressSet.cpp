/*
 *   Copyright (C) 2012 by Jonathan Naylor G4KLX
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

#include "SplitRepeaterAddressSet.h"

const unsigned int ADDRESS_WIDTH   = 120U;
const unsigned int PORT_WIDTH      = 80U;

const unsigned int ADDRESS_LENGTH  = 15U;
const unsigned int PORT_LENGTH     = 5U;

const unsigned int BORDER_SIZE = 5U;

CSplitRepeaterAddressSet::CSplitRepeaterAddressSet(wxWindow* parent, int id, const wxString& title, const wxString& address, unsigned int port) :
wxPanel(parent, id),
m_title(title),
m_address(NULL),
m_port(NULL)
{
	wxFlexGridSizer* sizer = new wxFlexGridSizer(2);

	wxStaticText* addressLabel = new wxStaticText(this, -1, _("Address"));
	sizer->Add(addressLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_address = new CAddressTextCtrl(this, -1, address, wxDefaultPosition, wxSize(ADDRESS_WIDTH, -1));
	m_address->SetMaxLength(ADDRESS_LENGTH);
	sizer->Add(m_address, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* portLabel = new wxStaticText(this, -1, _("Port"));
	sizer->Add(portLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	wxString buffer;
	buffer.Printf(wxT("%u"), port);

	m_port = new CPortTextCtrl(this, -1, buffer, wxDefaultPosition, wxSize(PORT_WIDTH, -1));
	m_port->SetMaxLength(PORT_LENGTH);
	sizer->Add(m_port, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	SetAutoLayout(true);

	sizer->Fit(this);
	sizer->SetSizeHints(this);

	SetSizer(sizer);
}


CSplitRepeaterAddressSet::~CSplitRepeaterAddressSet()
{
}

bool CSplitRepeaterAddressSet::Validate()
{
	unsigned int port = getPort();

	if (port > 65535U) {
		wxMessageDialog dialog(this, _("The Port is not valid"), m_title + _(" Error"), wxICON_ERROR);
		dialog.ShowModal();
		return false;
	}

	return true;
}

wxString CSplitRepeaterAddressSet::getAddress() const
{
	return m_address->GetValue();
}

unsigned int CSplitRepeaterAddressSet::getPort() const
{
	unsigned long n;

	m_port->GetValue().ToULong(&n);

	return n;
}
