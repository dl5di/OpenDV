/*
 *   Copyright (C) 2010,2012 by Jonathan Naylor G4KLX
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

#include "GMSKClientModemSet.h"

CGMSKClientModemSet::CGMSKClientModemSet(wxWindow* parent, int id, const wxString& title, GMSK_MODEM_TYPE type, unsigned int address) :
wxPanel(parent, id),
m_title(title),
#if defined(WIN32)
m_type(NULL),
#endif
m_address(NULL)
{
	wxFlexGridSizer* sizer = new wxFlexGridSizer(2);

#if defined(WIN32)
	wxStaticText* typeLabel = new wxStaticText(this, -1, _("Type"));
	sizer->Add(typeLabel, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	m_type = new wxChoice(this, -1, wxDefaultPosition, wxSize(CONTROL_WIDTH, -1));
	m_type->Append(wxT("LibUSB"));
	m_type->Append(wxT("WinUSB"));
	sizer->Add(m_type, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);
	m_type->SetSelection(int(type));
#endif

	wxStaticText* addressLabel = new wxStaticText(this, -1, _("Address"));
	sizer->Add(addressLabel, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	m_address = new wxChoice(this, -1, wxDefaultPosition, wxSize(CONTROL_WIDTH, -1));
	m_address->Append(wxT("0x300"));
	m_address->Append(wxT("0x301"));
	m_address->Append(wxT("0x302"));
	m_address->Append(wxT("0x303"));
	sizer->Add(m_address, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);
	m_address->SetSelection(address - 0x0300U);

	SetAutoLayout(true);

	sizer->Fit(this);
	sizer->SetSizeHints(this);

	SetSizer(sizer);
}

CGMSKClientModemSet::~CGMSKClientModemSet()
{
}

bool CGMSKClientModemSet::Validate()
{
#if defined(WIN32)
	if (m_type->GetCurrentSelection() == wxNOT_FOUND)
		return false;
#endif

	return m_address->GetCurrentSelection() != wxNOT_FOUND;
}

GMSK_MODEM_TYPE CGMSKClientModemSet::getType() const
{
#if defined(WIN32)
	int n = m_type->GetCurrentSelection();

	if (n == wxNOT_FOUND)
		return GMT_WINUSB;
	else
		return GMSK_MODEM_TYPE(n);
#else
	return GMT_LIBUSB;
#endif
}

unsigned int CGMSKClientModemSet::getAddress() const
{
	int n = m_address->GetCurrentSelection();

	if (n == wxNOT_FOUND)
		return 0x0300U;
	else
		return n + 0x0300U;
}
