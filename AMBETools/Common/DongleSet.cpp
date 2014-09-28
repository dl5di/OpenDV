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

#include "SerialDataController.h"
#include "DongleSet.h"

const unsigned int BORDER_SIZE   = 5U;
const unsigned int CONTROL_WIDTH = 150U;

const unsigned int ADDRESS_LENGTH  = 15U;
const unsigned int PORT_LENGTH     = 5U;

const int CHOICE_TYPE = 8750;

BEGIN_EVENT_TABLE(CDongleSet, wxPanel)
	EVT_CHOICE(CHOICE_TYPE, CDongleSet::onType)
END_EVENT_TABLE()

CDongleSet::CDongleSet(wxWindow* parent, int id, const wxString& title, DONGLE_TYPE type, const wxString& device, const wxString& address, unsigned int port) :
wxPanel(parent, id),
m_title(title),
m_type(NULL),
m_device(NULL),
m_address(NULL),
m_port(NULL)
{
	wxFlexGridSizer* sizer = new wxFlexGridSizer(2);

	wxStaticText* typeLabel = new wxStaticText(this, -1, _("Type"));
	sizer->Add(typeLabel, 0, wxALL, BORDER_SIZE);

	m_type = new wxChoice(this, CHOICE_TYPE, wxDefaultPosition, wxSize(CONTROL_WIDTH, -1));
	m_type->Append(wxT("DV-Dongle"));
	m_type->Append(wxT("DV3000"));
	sizer->Add(m_type, 0, wxALL, BORDER_SIZE);
	m_type->SetSelection(int(type));

	wxArrayString devices = CSerialDataController::getDevices();

	wxStaticText* deviceLabel = new wxStaticText(this, -1, _("Device"));
	sizer->Add(deviceLabel, 0, wxALL, BORDER_SIZE);

	m_device = new wxChoice(this, -1, wxDefaultPosition, wxSize(CONTROL_WIDTH, -1), devices);
	m_device->Insert(_("<none>"), 0U);
	sizer->Add(m_device, 0, wxALL, BORDER_SIZE);

	if (devices.GetCount() == 0U) {
		m_device->SetSelection(0);
	} else {
		bool res = m_device->SetStringSelection(device);
		if (!res)
			m_device->SetSelection(0);
	}

	wxStaticText* addressLabel = new wxStaticText(this, -1, _("Address"));
	sizer->Add(addressLabel, 0, wxALL, BORDER_SIZE);

	m_address = new CAddressTextCtrl(this, -1, address, wxDefaultPosition, wxSize(CONTROL_WIDTH, -1));
	m_address->SetMaxLength(ADDRESS_LENGTH);
	sizer->Add(m_address, 0, wxALL, BORDER_SIZE);

	wxStaticText* portLabel = new wxStaticText(this, -1, _("Port"));
	sizer->Add(portLabel, 0, wxALL, BORDER_SIZE);

	wxString text;
	text.Printf(wxT("%u"), port);

	m_port = new CPortTextCtrl(this, -1, text, wxDefaultPosition, wxSize(CONTROL_WIDTH, -1));
	m_port->SetMaxLength(PORT_LENGTH);
	sizer->Add(m_port, 0, wxALL, BORDER_SIZE);

	if (type == DT_DV3000) {
		m_device->Disable();
	} else {
		m_address->Disable();
		m_port->Disable();
	}

	SetAutoLayout(true);

	//sizer->Fit(this);
	//sizer->SetSizeHints(this);

	SetSizer(sizer);
}


CDongleSet::~CDongleSet()
{
}

bool CDongleSet::Validate()
{
	int n = m_type->GetSelection();

	if (n == wxNOT_FOUND) {
		wxMessageDialog dialog(this, _("The AMBE Dongle type is not set"), m_title + _(" Error"), wxICON_ERROR);
		dialog.ShowModal();
		return false;
	}

	if (n == 0) {
		n = m_device->GetSelection();

		if (n == wxNOT_FOUND) {
			wxMessageDialog dialog(this, _("The AMBE Dongle device is not set"), m_title + _(" Error"), wxICON_ERROR);
			dialog.ShowModal();
			return false;
		}
	} else {
		unsigned int port = getPort();

		if (port == 0U || port > 65535U) {
			wxMessageDialog dialog(this, _("The Dongle Port is not valid"), m_title + _(" Error"), wxICON_ERROR);
			dialog.ShowModal();
			return false;
		}
	}

	return true;
}

DONGLE_TYPE CDongleSet::getType() const
{
	int n = m_type->GetSelection();

	if (n == wxNOT_FOUND)
		return DT_DVDONGLE;

	return DONGLE_TYPE(n);
}

wxString CDongleSet::getDevice() const
{
	wxString device = m_device->GetStringSelection();

	if (device.IsSameAs(_("<none>")))
		return wxEmptyString;
	else
		return device;
}

wxString CDongleSet::getAddress() const
{
	return m_address->GetValue();
}

unsigned int CDongleSet::getPort() const
{
	unsigned long n;

	m_port->GetValue().ToULong(&n);

	return n;
}

void CDongleSet::onType(wxCommandEvent& event)
{
	int n = event.GetSelection();

	switch (n) {
		default:	// DV-Dongle
			m_device->Enable();
			m_address->Disable();
			m_port->Disable();
			break;

		case 1:		// DV3000
			m_device->Disable();
			m_address->Enable();
			m_port->Enable();
			break;
	}
}
