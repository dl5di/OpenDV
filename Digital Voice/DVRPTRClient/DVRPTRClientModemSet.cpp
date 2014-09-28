/*
 *   Copyright (C) 2011,2012 by Jonathan Naylor G4KLX
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

#include "DVRPTRClientModemSet.h"
#include "DVRPTRControllerV1.h"
#include "DVRPTRControllerV2.h"

const unsigned int CONTROL_WIDTH1 = 150U;
const unsigned int CONTROL_WIDTH2 = 300U;

const int CHOICE_VERSION = 8749;

BEGIN_EVENT_TABLE(CDVRPTRClientModemSet, wxPanel)
	EVT_CHOICE(CHOICE_VERSION, CDVRPTRClientModemSet::onVersion)
END_EVENT_TABLE()


CDVRPTRClientModemSet::CDVRPTRClientModemSet(wxWindow* parent, int id, const wxString& title, DVRPTR_VERSION version, const wxString& port, bool rxInvert, bool txInvert, bool channel, unsigned int modLevel, unsigned int txDelay) :
wxPanel(parent, id),
m_title(title),
m_version(NULL),
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
	sizer->Add(m_version, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);
	m_version->SetSelection(version == DVRPTR_V2 ? 1 : 0);

	wxStaticText* portLabel = new wxStaticText(this, -1, _("Port"));
	sizer->Add(portLabel, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	m_port = new wxChoice(this, -1, wxDefaultPosition, wxSize(CONTROL_WIDTH1, -1));
	sizer->Add(m_port, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxArrayString ports;

	switch (version) {
		case DVRPTR_V2:
			ports = CDVRPTRControllerV2::getDevices();
			break;
		default:
			ports = CDVRPTRControllerV1::getDevices();
			break;
	}

	for (unsigned int i = 0U; i < ports.GetCount(); i++)
		m_port->Append(ports.Item(i));

	bool found = m_port->SetStringSelection(port);
	if (!found)
		m_port->SetSelection(0);

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

	SetAutoLayout(true);

	sizer->Fit(this);
	sizer->SetSizeHints(this);

	SetSizer(sizer);
}

CDVRPTRClientModemSet::~CDVRPTRClientModemSet()
{
}

bool CDVRPTRClientModemSet::Validate()
{
	if (m_version->GetCurrentSelection() == wxNOT_FOUND)
		return false;

	if (m_port->GetCurrentSelection() == wxNOT_FOUND)
		return false;

	if (m_txInvert->GetCurrentSelection() == wxNOT_FOUND)
		return false;

	if (m_rxInvert->GetCurrentSelection() == wxNOT_FOUND)
		return false;

	return m_channel->GetCurrentSelection() != wxNOT_FOUND;
}

DVRPTR_VERSION CDVRPTRClientModemSet::getVersion() const
{
	int n = m_version->GetCurrentSelection();

	if (n == wxNOT_FOUND)
		return DVRPTR_V1;

	return n == 1 ? DVRPTR_V2 : DVRPTR_V1;
}

wxString CDVRPTRClientModemSet::getPort() const
{
	int n = m_port->GetCurrentSelection();

	if (n == wxNOT_FOUND)
		return wxEmptyString;

	return m_port->GetStringSelection();
}

bool CDVRPTRClientModemSet::getRXInvert() const
{
	int n = m_rxInvert->GetCurrentSelection();

	if (n == wxNOT_FOUND)
		return false;

	return n == 1;
}

bool CDVRPTRClientModemSet::getTXInvert() const
{
	int n = m_txInvert->GetCurrentSelection();

	if (n == wxNOT_FOUND)
		return false;

	return n == 1;
}

bool CDVRPTRClientModemSet::getChannel() const
{
	int n = m_channel->GetCurrentSelection();

	if (n == wxNOT_FOUND)
		return false;

	return n == 1;
}

unsigned int CDVRPTRClientModemSet::getModLevel() const
{
	return (unsigned int)m_modLevel->GetValue();
}

unsigned int CDVRPTRClientModemSet::getTXDelay() const
{
	return (unsigned int)m_txDelay->GetValue();
}

void CDVRPTRClientModemSet::onVersion(wxCommandEvent &event)
{
	int n = event.GetSelection();

	wxString curr = m_port->GetStringSelection();

	wxArrayString ports;

	switch (n) {
		case 1:
			ports = CDVRPTRControllerV2::getDevices();
			m_rxInvert->Disable();
			m_txDelay->Disable();
			m_channel->Disable();
			break;
		default:
			ports = CDVRPTRControllerV1::getDevices();
			m_rxInvert->Enable();
			m_txDelay->Enable();
			m_channel->Enable();
			break;
	}

	m_port->Clear();
	for (unsigned int i = 0U; i < ports.GetCount(); i++)
		m_port->Append(ports.Item(i));

	bool found = m_port->SetStringSelection(curr);
	if (!found)
		m_port->SetSelection(0);
}
