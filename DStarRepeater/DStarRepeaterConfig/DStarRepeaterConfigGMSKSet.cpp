/*
 *   Copyright (C) 2010,2011,2012,2014 by Jonathan Naylor G4KLX
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

#include "DStarRepeaterConfigGMSKSet.h"

const unsigned int BORDER_SIZE   = 5U;
const unsigned int CONTROL_WIDTH = 150U;

CDStarRepeaterConfigGMSKSet::CDStarRepeaterConfigGMSKSet(wxWindow* parent, int id, USB_INTERFACE type, unsigned int address) :
wxDialog(parent, id, wxString(_("GMSK Modem Settings"))),
#if defined(WIN32)
m_type(NULL),
#endif
m_address(NULL)
{
	wxBoxSizer* topSizer = new wxBoxSizer(wxVERTICAL);

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
	m_address->Append(wxT("0x304"));
	m_address->Append(wxT("0x305"));
	m_address->Append(wxT("0x306"));
	m_address->Append(wxT("0x307"));
	m_address->Append(wxT("0x308"));
	m_address->Append(wxT("0x309"));
	m_address->Append(wxT("0x30A"));
	m_address->Append(wxT("0x30B"));
	m_address->Append(wxT("0x30C"));
	m_address->Append(wxT("0x30D"));
	m_address->Append(wxT("0x30E"));
	m_address->Append(wxT("0x30F"));
	m_address->Append(wxT("0x310"));
	m_address->Append(wxT("0x311"));
	m_address->Append(wxT("0x312"));
	m_address->Append(wxT("0x313"));
	m_address->Append(wxT("0x314"));
	m_address->Append(wxT("0x315"));
	m_address->Append(wxT("0x316"));
	m_address->Append(wxT("0x317"));
	m_address->Append(wxT("0x318"));
	m_address->Append(wxT("0x319"));
	m_address->Append(wxT("0x31A"));
	m_address->Append(wxT("0x31B"));
	m_address->Append(wxT("0x31C"));
	m_address->Append(wxT("0x31D"));
	m_address->Append(wxT("0x31E"));
	m_address->Append(wxT("0x31F"));
	sizer->Add(m_address, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);
	m_address->SetSelection(address - 0x0300U);

	topSizer->Add(sizer);

	topSizer->Add(CreateButtonSizer(wxOK | wxCANCEL), 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	SetAutoLayout(true);

	topSizer->Fit(this);
	topSizer->SetSizeHints(this);

	SetSizer(topSizer);
}

CDStarRepeaterConfigGMSKSet::~CDStarRepeaterConfigGMSKSet()
{
}

bool CDStarRepeaterConfigGMSKSet::Validate()
{
#if defined(WIN32)
	if (m_type->GetCurrentSelection() == wxNOT_FOUND)
		return false;
#endif

	return m_address->GetCurrentSelection() != wxNOT_FOUND;
}

USB_INTERFACE CDStarRepeaterConfigGMSKSet::getType() const
{
#if defined(WIN32)
	int n = m_type->GetCurrentSelection();

	if (n == wxNOT_FOUND)
		return UI_WINUSB;
	else
		return USB_INTERFACE(n);
#else
	return UI_LIBUSB;
#endif
}

unsigned int CDStarRepeaterConfigGMSKSet::getAddress() const
{
	int n = m_address->GetCurrentSelection();

	if (n == wxNOT_FOUND)
		return 0x0300U;
	else
		return n + 0x0300U;
}

