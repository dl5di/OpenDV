/*
 *   Copyright (C) 2011-2015 by Jonathan Naylor G4KLX
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

#include "DStarRepeaterConfigMMDVMSet.h"
#include "SerialPortSelector.h"

const unsigned int BORDER_SIZE    = 5U;
const unsigned int CONTROL_WIDTH1 = 150U;
const unsigned int CONTROL_WIDTH2 = 300U;

const unsigned int PORT_LENGTH     = 5U;

CDStarRepeaterConfigMMDVMSet::CDStarRepeaterConfigMMDVMSet(wxWindow* parent, int id, const wxString& port, bool rxInvert, bool txInvert, bool pttInvert, unsigned int txDelay, unsigned int rxLevel, unsigned int txLevel) :
wxDialog(parent, id, wxString(_("MMDVM Settings"))),
m_port(NULL),
m_txInvert(NULL),
m_rxInvert(NULL),
m_pttInvert(NULL),
m_txDelay(NULL),
m_rxLevel(NULL),
m_txLevel(NULL)
{
	wxBoxSizer* topSizer = new wxBoxSizer(wxVERTICAL);

	wxFlexGridSizer* sizer = new wxFlexGridSizer(2);

	wxStaticText* portLabel = new wxStaticText(this, -1, _("Port"));
	sizer->Add(portLabel, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	m_port = new wxChoice(this, -1, wxDefaultPosition, wxSize(CONTROL_WIDTH1, -1));
	sizer->Add(m_port, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxArrayString ports = CSerialPortSelector::getDevices();
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

	wxStaticText* pttInvertLabel = new wxStaticText(this, -1, _("PTT Inversion"));
	sizer->Add(pttInvertLabel, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	m_pttInvert = new wxChoice(this, -1, wxDefaultPosition, wxSize(CONTROL_WIDTH1, -1));
	m_pttInvert->Append(_("Off"));
	m_pttInvert->Append(_("On"));
	sizer->Add(m_pttInvert, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);
	m_pttInvert->SetSelection(pttInvert ? 1 : 0);

	wxStaticText* txDelayLabel = new wxStaticText(this, -1, _("TX Delay (ms)"));
	sizer->Add(txDelayLabel, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	m_txDelay = new wxSlider(this, -1, txDelay, 0, 500, wxDefaultPosition, wxSize(CONTROL_WIDTH2, -1), wxSL_HORIZONTAL | wxSL_LABELS);
	sizer->Add(m_txDelay, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* rxLevelLabel = new wxStaticText(this, -1, _("RX Level (%)"));
	sizer->Add(rxLevelLabel, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	m_rxLevel = new wxSlider(this, -1, rxLevel, 0, 100, wxDefaultPosition, wxSize(CONTROL_WIDTH2, -1), wxSL_HORIZONTAL | wxSL_LABELS);
	sizer->Add(m_rxLevel, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* txLevelLabel = new wxStaticText(this, -1, _("TX Level (%)"));
	sizer->Add(txLevelLabel, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	m_txLevel = new wxSlider(this, -1, txLevel, 0, 100, wxDefaultPosition, wxSize(CONTROL_WIDTH2, -1), wxSL_HORIZONTAL | wxSL_LABELS);
	sizer->Add(m_txLevel, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	topSizer->Add(sizer);

	topSizer->Add(CreateButtonSizer(wxOK | wxCANCEL), 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	SetAutoLayout(true);

	topSizer->Fit(this);
	topSizer->SetSizeHints(this);

	SetSizer(topSizer);
}

CDStarRepeaterConfigMMDVMSet::~CDStarRepeaterConfigMMDVMSet()
{
}

bool CDStarRepeaterConfigMMDVMSet::Validate()
{
	if (m_port->GetCurrentSelection() == wxNOT_FOUND)
		return false;

	if (m_txInvert->GetCurrentSelection() == wxNOT_FOUND)
		return false;

	if (m_rxInvert->GetCurrentSelection() == wxNOT_FOUND)
		return false;

	if (m_pttInvert->GetCurrentSelection() == wxNOT_FOUND)
		return false;

	return true;
}

wxString CDStarRepeaterConfigMMDVMSet::getPort() const
{
	int n = m_port->GetCurrentSelection();

	if (n == wxNOT_FOUND)
		return wxEmptyString;

	return m_port->GetStringSelection();
}

bool CDStarRepeaterConfigMMDVMSet::getRXInvert() const
{
	int n = m_rxInvert->GetCurrentSelection();

	if (n == wxNOT_FOUND)
		return false;

	return n == 1;
}

bool CDStarRepeaterConfigMMDVMSet::getTXInvert() const
{
	int n = m_txInvert->GetCurrentSelection();

	if (n == wxNOT_FOUND)
		return false;

	return n == 1;
}

bool CDStarRepeaterConfigMMDVMSet::getPTTInvert() const
{
	int n = m_pttInvert->GetCurrentSelection();

	if (n == wxNOT_FOUND)
		return false;

	return n == 1;
}

unsigned int CDStarRepeaterConfigMMDVMSet::getTXDelay() const
{
	return (unsigned int)m_txDelay->GetValue();
}

unsigned int CDStarRepeaterConfigMMDVMSet::getRXLevel() const
{
	return (unsigned int)m_rxLevel->GetValue();
}

unsigned int CDStarRepeaterConfigMMDVMSet::getTXLevel() const
{
	return (unsigned int)m_txLevel->GetValue();
}
