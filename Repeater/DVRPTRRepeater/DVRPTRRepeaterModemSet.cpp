/*
 *   Copyright (C) 2011,2012,2013 by Jonathan Naylor G4KLX
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

#include "DVRPTRRepeaterModemSet.h"
#include "DVRPTRControllerV1.h"
#include "DVRPTRControllerV2.h"
#include "DVRPTRControllerV3.h"

const unsigned int BORDER_SIZE    = 5U;
const unsigned int CONTROL_WIDTH1 = 150U;
const unsigned int CONTROL_WIDTH2 = 300U;

const unsigned int ADDRESS_LENGTH  = 15U;
const unsigned int PORT_LENGTH     = 5U;

const int CHOICE_VERSION = 8749;
const int CHOICE_TYPE    = 8750;

BEGIN_EVENT_TABLE(CDVRPTRRepeaterModemSet, wxPanel)
	EVT_CHOICE(CHOICE_VERSION, CDVRPTRRepeaterModemSet::onVersion)
	EVT_CHOICE(CHOICE_TYPE,    CDVRPTRRepeaterModemSet::onConnectionType)
END_EVENT_TABLE()


CDVRPTRRepeaterModemSet::CDVRPTRRepeaterModemSet(wxWindow* parent, int id, const wxString& title, DVRPTR_VERSION version, CONNECTION_TYPE connectionType, const wxString& usbPort, const wxString& address, unsigned int port, bool rxInvert, bool txInvert, bool channel, unsigned int modLevel, unsigned int txDelay) :
wxPanel(parent, id),
m_title(title),
m_version(NULL),
m_connectionType(NULL),
m_usbPort(NULL),
m_address(NULL),
m_port(NULL),
m_txInvert(NULL),
m_rxInvert(NULL),
m_channel(NULL),
m_modLevel(NULL),
m_txDelay(NULL)
{
	wxFlexGridSizer* sizer = new wxFlexGridSizer(2);

	wxStaticText* versionLabel = new wxStaticText(this, -1, _("Version"));
	sizer->Add(versionLabel, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	m_version = new wxChoice(this, CHOICE_VERSION, wxDefaultPosition, wxSize(CONTROL_WIDTH1, -1));
	m_version->Append(wxT("1"));
	m_version->Append(wxT("2"));
	m_version->Append(wxT("3"));
	sizer->Add(m_version, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);
	m_version->SetSelection(int(version) - 1);

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

	wxArrayString ports;

	switch (version) {
		case DVRPTR_V2:
			ports = CDVRPTRControllerV2::getDevices();
			break;
		case DVRPTR_V3:
			ports = CDVRPTRControllerV3::getDevices();
			break;
		default:
			ports = CDVRPTRControllerV1::getDevices();
			break;
	}

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

	wxStaticText* rxInvertLabel = new wxStaticText(this, -1, _("RX Inversion"));
	sizer->Add(rxInvertLabel, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	m_rxInvert = new wxChoice(this, -1, wxDefaultPosition, wxSize(CONTROL_WIDTH1, -1));
	m_rxInvert->Append(_("Off"));
	m_rxInvert->Append(_("On"));
	sizer->Add(m_rxInvert, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);
	m_rxInvert->SetSelection(rxInvert ? 1 : 0);

	wxStaticText* channelLabel = new wxStaticText(this, -1, _("Channel"));
	sizer->Add(channelLabel, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	m_channel = new wxChoice(this, -1, wxDefaultPosition, wxSize(CONTROL_WIDTH1, -1));
	m_channel->Append(_("A / FSK Pin"));
	m_channel->Append(_("B / AFSK Pin"));
	sizer->Add(m_channel, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);
	m_channel->SetSelection(channel ? 1 : 0);

	wxStaticText* modLevelLabel = new wxStaticText(this, -1, _("TX Level (%)"));
	sizer->Add(modLevelLabel, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	m_modLevel = new wxSlider(this, -1, modLevel, 0, 100, wxDefaultPosition, wxSize(CONTROL_WIDTH2, -1), wxSL_HORIZONTAL | wxSL_LABELS);
	sizer->Add(m_modLevel, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* txDelayLabel = new wxStaticText(this, -1, _("TX Delay (ms)"));
	sizer->Add(txDelayLabel, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	m_txDelay = new wxSlider(this, -1, txDelay, 0, 500, wxDefaultPosition, wxSize(CONTROL_WIDTH2, -1), wxSL_HORIZONTAL | wxSL_LABELS);
	sizer->Add(m_txDelay, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	switch (version) {
		case DVRPTR_V2:
		case DVRPTR_V3:
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

			m_connectionType->Enable();
			m_rxInvert->Enable();
			m_txDelay->Enable();
			m_channel->Enable();
			break;

		default:
			m_connectionType->SetSelection(0);		// USB
			m_connectionType->Disable();
			m_usbPort->Enable();
			m_address->Disable();
			m_port->Disable();
			m_rxInvert->Enable();
			m_txDelay->Enable();
			m_channel->Enable();
	}

	SetAutoLayout(true);

	sizer->Fit(this);
	sizer->SetSizeHints(this);

	SetSizer(sizer);
}

CDVRPTRRepeaterModemSet::~CDVRPTRRepeaterModemSet()
{
}

bool CDVRPTRRepeaterModemSet::Validate()
{
	if (m_version->GetCurrentSelection() == wxNOT_FOUND)
		return false;

	if (m_connectionType->GetCurrentSelection() == wxNOT_FOUND)
		return false;

	CONNECTION_TYPE type = getConnectionType();
	if (type == CT_USB) {
		if (m_usbPort->GetCurrentSelection() == wxNOT_FOUND)
			return false;
	} else {
		unsigned int port = getPort();

		if (port == 0U || port > 65535U) {
			wxMessageDialog dialog(this, _("The Port is not valid"), m_title + _(" Error"), wxICON_ERROR);
			dialog.ShowModal();
			return false;
		}
	}

	if (m_txInvert->GetCurrentSelection() == wxNOT_FOUND)
		return false;

	if (m_rxInvert->GetCurrentSelection() == wxNOT_FOUND)
		return false;

	return m_channel->GetCurrentSelection() != wxNOT_FOUND;
}

DVRPTR_VERSION CDVRPTRRepeaterModemSet::getVersion() const
{
	int n = m_version->GetCurrentSelection();

	switch (n) {
		case 1:
			return DVRPTR_V2;
		case 2:
			return DVRPTR_V3;
		default:
			return DVRPTR_V1;
	}
}

CONNECTION_TYPE CDVRPTRRepeaterModemSet::getConnectionType() const
{
	int n = m_connectionType->GetCurrentSelection();

	if (n == wxNOT_FOUND)
		return CT_USB;

	return n == 1 ? CT_NETWORK : CT_USB;
}

wxString CDVRPTRRepeaterModemSet::getUSBPort() const
{
	int n = m_usbPort->GetCurrentSelection();

	if (n == wxNOT_FOUND)
		return wxEmptyString;

	return m_usbPort->GetStringSelection();
}

wxString CDVRPTRRepeaterModemSet::getAddress() const
{
	return m_address->GetValue();
}

unsigned int CDVRPTRRepeaterModemSet::getPort() const
{
	unsigned long n;

	m_port->GetValue().ToULong(&n);

	return n;
}

bool CDVRPTRRepeaterModemSet::getRXInvert() const
{
	int n = m_rxInvert->GetCurrentSelection();

	if (n == wxNOT_FOUND)
		return false;

	return n == 1;
}

bool CDVRPTRRepeaterModemSet::getTXInvert() const
{
	int n = m_txInvert->GetCurrentSelection();

	if (n == wxNOT_FOUND)
		return false;

	return n == 1;
}

bool CDVRPTRRepeaterModemSet::getChannel() const
{
	int n = m_channel->GetCurrentSelection();

	if (n == wxNOT_FOUND)
		return false;

	return n == 1;
}

unsigned int CDVRPTRRepeaterModemSet::getModLevel() const
{
	return (unsigned int)m_modLevel->GetValue();
}

unsigned int CDVRPTRRepeaterModemSet::getTXDelay() const
{
	return (unsigned int)m_txDelay->GetValue();
}

void CDVRPTRRepeaterModemSet::onVersion(wxCommandEvent &event)
{
	int n = event.GetSelection();

	wxString curr = m_port->GetStringSelection();

	wxArrayString ports;

	switch (n) {
		case 1:
		case 2:
			ports = CDVRPTRControllerV2::getDevices();
			m_connectionType->SetSelection(0);		// USB
			m_connectionType->Enable();
			m_usbPort->Enable();
			m_address->Disable();
			m_port->Disable();
			m_rxInvert->Disable();
			m_txDelay->Disable();
			m_channel->Disable();
			break;
		default:
			ports = CDVRPTRControllerV1::getDevices();
			m_connectionType->SetSelection(0);		// USB
			m_connectionType->Disable();
			m_usbPort->Enable();
			m_address->Disable();
			m_port->Disable();
			m_rxInvert->Enable();
			m_txDelay->Enable();
			m_channel->Enable();
			break;
	}

	m_usbPort->Clear();
	for (unsigned int i = 0U; i < ports.GetCount(); i++)
		m_usbPort->Append(ports.Item(i));

	bool found = m_usbPort->SetStringSelection(curr);
	if (!found)
		m_usbPort->SetSelection(0);
}

void CDVRPTRRepeaterModemSet::onConnectionType(wxCommandEvent &event)
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
