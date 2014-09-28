/*
 *   Copyright (C) 2011-2014 by Jonathan Naylor G4KLX
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

#include "DStarRepeaterConfigDVRPTR2Set.h"
#include "SerialDataController.h"

const unsigned int BORDER_SIZE    = 5U;
const unsigned int CONTROL_WIDTH1 = 150U;
const unsigned int CONTROL_WIDTH2 = 300U;

const unsigned int ADDRESS_LENGTH  = 15U;
const unsigned int PORT_LENGTH     = 5U;

const int CHOICE_TYPE = 8750;

BEGIN_EVENT_TABLE(CDStarRepeaterConfigDVRPTR2Set, wxDialog)
	EVT_CHOICE(CHOICE_TYPE, CDStarRepeaterConfigDVRPTR2Set::onConnectionType)
END_EVENT_TABLE()


CDStarRepeaterConfigDVRPTR2Set::CDStarRepeaterConfigDVRPTR2Set(wxWindow* parent, int id, CONNECTION_TYPE connectionType, const wxString& usbPort, const wxString& address, unsigned int port, bool txInvert, unsigned int modLevel, unsigned int txDelay) :
wxDialog(parent, id, wxString(_("DV-RPTR V2 Settings"))),
m_connectionType(NULL),
m_usbPort(NULL),
m_address(NULL),
m_port(NULL),
m_txInvert(NULL),
m_modLevel(NULL),
m_txDelay(NULL)
{
	wxBoxSizer* topSizer = new wxBoxSizer(wxVERTICAL);

	wxFlexGridSizer* sizer = new wxFlexGridSizer(2);

	wxStaticText* connectionLabel = new wxStaticText(this, -1, _("Connection"));
	sizer->Add(connectionLabel, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	m_connectionType = new wxChoice(this, CHOICE_TYPE, wxDefaultPosition, wxSize(CONTROL_WIDTH1, -1));
	m_connectionType->Append(wxT("USB"));
	m_connectionType->Append(_("Network"));
	sizer->Add(m_connectionType, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);
	m_connectionType->SetSelection(connectionType == CT_NETWORK ? 1 : 0);

	wxStaticText* usbPortLabel = new wxStaticText(this, -1, _("USB Port"));
	sizer->Add(usbPortLabel, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	m_usbPort = new wxChoice(this, -1, wxDefaultPosition, wxSize(CONTROL_WIDTH1, -1));
	sizer->Add(m_usbPort, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxArrayString ports = CSerialDataController::getDevices();
	for (unsigned int i = 0U; i < ports.GetCount(); i++)
		m_usbPort->Append(ports.Item(i));

	bool found = m_usbPort->SetStringSelection(usbPort);
	if (!found)
		m_usbPort->SetSelection(0);

	wxStaticText* addressLabel = new wxStaticText(this, -1, _("Address"));
	sizer->Add(addressLabel, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	m_address = new CAddressTextCtrl(this, -1, address, wxDefaultPosition, wxSize(CONTROL_WIDTH1, -1));
	m_address->SetMaxLength(ADDRESS_LENGTH);
	sizer->Add(m_address, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* portLabel = new wxStaticText(this, -1, _("Port"));
	sizer->Add(portLabel, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxString buffer;
	buffer.Printf(wxT("%u"), port);

	m_port = new CPortTextCtrl(this, -1, buffer, wxDefaultPosition, wxSize(CONTROL_WIDTH1, -1));
	m_port->SetMaxLength(PORT_LENGTH);
	sizer->Add(m_port, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* txInvertLabel = new wxStaticText(this, -1, _("TX Inversion"));
	sizer->Add(txInvertLabel, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	m_txInvert = new wxChoice(this, -1, wxDefaultPosition, wxSize(CONTROL_WIDTH1, -1));
	m_txInvert->Append(_("Off"));
	m_txInvert->Append(_("On"));
	sizer->Add(m_txInvert, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);
	m_txInvert->SetSelection(txInvert ? 1 : 0);

	wxStaticText* modLevelLabel = new wxStaticText(this, -1, _("TX Level (%)"));
	sizer->Add(modLevelLabel, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	m_modLevel = new wxSlider(this, -1, modLevel, 0, 100, wxDefaultPosition, wxSize(CONTROL_WIDTH2, -1), wxSL_HORIZONTAL | wxSL_LABELS);
	sizer->Add(m_modLevel, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* txDelayLabel = new wxStaticText(this, -1, _("TX Delay (ms)"));
	sizer->Add(txDelayLabel, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	m_txDelay = new wxSlider(this, -1, txDelay, 100, 850, wxDefaultPosition, wxSize(CONTROL_WIDTH2, -1), wxSL_HORIZONTAL | wxSL_LABELS);
	sizer->Add(m_txDelay, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	switch (connectionType) {
		case CT_NETWORK:
			m_usbPort->Disable();
			m_address->Enable();
			m_port->Enable();
			break;
		default:
			m_usbPort->Enable();
			m_address->Disable();
			m_port->Disable();
			break;
	}

	topSizer->Add(sizer);

	topSizer->Add(CreateButtonSizer(wxOK | wxCANCEL), 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	SetAutoLayout(true);

	topSizer->Fit(this);
	topSizer->SetSizeHints(this);

	SetSizer(topSizer);
}

CDStarRepeaterConfigDVRPTR2Set::~CDStarRepeaterConfigDVRPTR2Set()
{
}

bool CDStarRepeaterConfigDVRPTR2Set::Validate()
{
	if (m_connectionType->GetCurrentSelection() == wxNOT_FOUND)
		return false;

	CONNECTION_TYPE type = getConnectionType();
	if (type == CT_USB) {
		if (m_usbPort->GetCurrentSelection() == wxNOT_FOUND)
			return false;
	} else {
		unsigned int port = getPort();

		if (port == 0U || port > 65535U) {
			wxMessageDialog dialog(this, _("The Port is not valid"), _("DV-RPTR V2 Error"), wxICON_ERROR);
			dialog.ShowModal();
			return false;
		}
	}

	 return m_txInvert->GetCurrentSelection() != wxNOT_FOUND;
}

CONNECTION_TYPE CDStarRepeaterConfigDVRPTR2Set::getConnectionType() const
{
	int n = m_connectionType->GetCurrentSelection();

	if (n == wxNOT_FOUND)
		return CT_USB;

	return n == 1 ? CT_NETWORK : CT_USB;
}

wxString CDStarRepeaterConfigDVRPTR2Set::getUSBPort() const
{
	int n = m_usbPort->GetCurrentSelection();

	if (n == wxNOT_FOUND)
		return wxEmptyString;

	return m_usbPort->GetStringSelection();
}

wxString CDStarRepeaterConfigDVRPTR2Set::getAddress() const
{
	return m_address->GetValue();
}

unsigned int CDStarRepeaterConfigDVRPTR2Set::getPort() const
{
	unsigned long n;

	m_port->GetValue().ToULong(&n);

	return n;
}

bool CDStarRepeaterConfigDVRPTR2Set::getTXInvert() const
{
	int n = m_txInvert->GetCurrentSelection();

	if (n == wxNOT_FOUND)
		return false;

	return n == 1;
}

unsigned int CDStarRepeaterConfigDVRPTR2Set::getModLevel() const
{
	return (unsigned int)m_modLevel->GetValue();
}

unsigned int CDStarRepeaterConfigDVRPTR2Set::getTXDelay() const
{
	return (unsigned int)m_txDelay->GetValue();
}

void CDStarRepeaterConfigDVRPTR2Set::onConnectionType(wxCommandEvent &event)
{
	int n = event.GetSelection();

	switch (n) {
		case 1:		// CT_NETWORK
			m_usbPort->Disable();
			m_address->Enable();
			m_port->Enable();
			break;
		default:	// CT_USB
			m_usbPort->Enable();
			m_address->Disable();
			m_port->Disable();
			break;
	}
}
