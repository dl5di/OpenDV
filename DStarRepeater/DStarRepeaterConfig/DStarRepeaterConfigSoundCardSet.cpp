/*
 *   Copyright (C) 2009-2014 by Jonathan Naylor G4KLX
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

#include "DStarRepeaterConfigSoundCardSet.h"
#include "SoundCardReaderWriter.h"

const unsigned int BORDER_SIZE    = 5U;
const unsigned int CONTROL_WIDTH1 = 150U;
const unsigned int CONTROL_WIDTH2 = 300U;
const unsigned int CONTROL_WIDTH3 = 350U;

const unsigned int ADDRESS_LENGTH  = 15U;
const unsigned int PORT_LENGTH     = 5U;


CDStarRepeaterConfigSoundCardSet::CDStarRepeaterConfigSoundCardSet(wxWindow* parent, int id, const wxString& rxDevice, const wxString& txDevice, bool rxInvert, bool txInvert, wxFloat32 rxLevel, wxFloat32 txLevel, unsigned int txDelay, unsigned int txTail) :
wxDialog(parent, id, wxString(_("Sound Card Settings"))),
m_rxDevice(NULL),
m_txDevice(NULL),
m_rxInvert(NULL),
m_txInvert(NULL),
m_rxLevel(NULL),
m_txLevel(NULL),
m_txDelay(NULL),
m_txTail(NULL)
{
	wxBoxSizer* topSizer = new wxBoxSizer(wxVERTICAL);

	wxFlexGridSizer* sizer = new wxFlexGridSizer(2);

	wxStaticText* rxDeviceLabel = new wxStaticText(this, -1, _("RX Device"));
	sizer->Add(rxDeviceLabel, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	m_rxDevice = new wxChoice(this, -1, wxDefaultPosition, wxSize(CONTROL_WIDTH3, -1));
	sizer->Add(m_rxDevice, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxArrayString devs = CSoundCardReaderWriter::getReadDevices();
	for (unsigned int i = 0U; i < devs.GetCount(); i++)
		m_rxDevice->Append(devs.Item(i));

	bool found = m_rxDevice->SetStringSelection(rxDevice);
	if (!found)
		m_rxDevice->SetSelection(0);

	wxStaticText* txDeviceLabel = new wxStaticText(this, -1, _("TX Device"));
	sizer->Add(txDeviceLabel, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	m_txDevice = new wxChoice(this, -1, wxDefaultPosition, wxSize(CONTROL_WIDTH3, -1));
	sizer->Add(m_txDevice, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	devs = CSoundCardReaderWriter::getWriteDevices();
	for (unsigned int i = 0U; i < devs.GetCount(); i++)
		m_txDevice->Append(devs.Item(i));

	found = m_txDevice->SetStringSelection(txDevice);
	if (!found)
		m_txDevice->SetSelection(0);

	wxStaticText* rxInvertLabel = new wxStaticText(this, -1, _("RX Inversion"));
	sizer->Add(rxInvertLabel, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	m_rxInvert = new wxChoice(this, -1, wxDefaultPosition, wxSize(CONTROL_WIDTH1, -1));
	m_rxInvert->Append(_("Off"));
	m_rxInvert->Append(_("On"));
	sizer->Add(m_rxInvert, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);
	m_rxInvert->SetSelection(rxInvert ? 1 : 0);

	wxStaticText* txInvertLabel = new wxStaticText(this, -1, _("TX Inversion"));
	sizer->Add(txInvertLabel, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	m_txInvert = new wxChoice(this, -1, wxDefaultPosition, wxSize(CONTROL_WIDTH1, -1));
	m_txInvert->Append(_("Off"));
	m_txInvert->Append(_("On"));
	sizer->Add(m_txInvert, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);
	m_txInvert->SetSelection(txInvert ? 1 : 0);

	wxStaticText* rxLevelLabel = new wxStaticText(this, -1, _("RX Level (%)"));
	sizer->Add(rxLevelLabel, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	int level = int(rxLevel * 100.0F + 0.5F);
	m_rxLevel = new wxSlider(this, -1, level, 0, 100, wxDefaultPosition, wxSize(CONTROL_WIDTH2, -1), wxSL_HORIZONTAL | wxSL_LABELS);
	sizer->Add(m_rxLevel, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* txLevelLabel = new wxStaticText(this, -1, _("TX Level (%)"));
	sizer->Add(txLevelLabel, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	level = int(txLevel * 100.0F + 0.5F);
	m_txLevel = new wxSlider(this, -1, level, 0, 100, wxDefaultPosition, wxSize(CONTROL_WIDTH2, -1), wxSL_HORIZONTAL | wxSL_LABELS);
	sizer->Add(m_txLevel, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* txDelayLabel = new wxStaticText(this, -1, _("TX Delay (ms)"));
	sizer->Add(txDelayLabel, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	m_txDelay = new wxSlider(this, -1, txDelay, 0, 500, wxDefaultPosition, wxSize(CONTROL_WIDTH2, -1), wxSL_HORIZONTAL | wxSL_LABELS);
	sizer->Add(m_txDelay, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* txTailLabel = new wxStaticText(this, -1, _("TX Tail (ms)"));
	sizer->Add(txTailLabel, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	m_txTail = new wxSlider(this, -1, txTail, 0, 100, wxDefaultPosition, wxSize(CONTROL_WIDTH2, -1), wxSL_HORIZONTAL | wxSL_LABELS);
	sizer->Add(m_txTail, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	topSizer->Add(sizer);

	topSizer->Add(CreateButtonSizer(wxOK | wxCANCEL), 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	SetAutoLayout(true);

	topSizer->Fit(this);
	topSizer->SetSizeHints(this);

	SetSizer(topSizer);
}

CDStarRepeaterConfigSoundCardSet::~CDStarRepeaterConfigSoundCardSet()
{
}

bool CDStarRepeaterConfigSoundCardSet::Validate()
{
	if (m_rxDevice->GetCurrentSelection() == wxNOT_FOUND)
		return false;

	if (m_txDevice->GetCurrentSelection() == wxNOT_FOUND)
		return false;

	if (m_rxInvert->GetCurrentSelection() == wxNOT_FOUND)
		return false;

	return m_txInvert->GetCurrentSelection() != wxNOT_FOUND;
}

wxString CDStarRepeaterConfigSoundCardSet::getRXDevice() const
{
	int n = m_rxDevice->GetCurrentSelection();

	if (n == wxNOT_FOUND)
		return wxEmptyString;

	return m_rxDevice->GetStringSelection();
}

wxString CDStarRepeaterConfigSoundCardSet::getTXDevice() const
{
	int n = m_txDevice->GetCurrentSelection();

	if (n == wxNOT_FOUND)
		return wxEmptyString;

	return m_txDevice->GetStringSelection();
}

bool CDStarRepeaterConfigSoundCardSet::getRXInvert() const
{
	int n = m_rxInvert->GetCurrentSelection();

	if (n == wxNOT_FOUND)
		return false;

	return n == 1;
}

bool CDStarRepeaterConfigSoundCardSet::getTXInvert() const
{
	int n = m_txInvert->GetCurrentSelection();

	if (n == wxNOT_FOUND)
		return false;

	return n == 1;
}

wxFloat32 CDStarRepeaterConfigSoundCardSet::getRXLevel() const
{
	return wxFloat32(m_rxLevel->GetValue()) / 100.0F;
}

wxFloat32 CDStarRepeaterConfigSoundCardSet::getTXLevel() const
{
	return wxFloat32(m_txLevel->GetValue()) / 100.0F;
}

unsigned int CDStarRepeaterConfigSoundCardSet::getTXDelay() const
{
	return (unsigned int)m_txDelay->GetValue();
}

unsigned int CDStarRepeaterConfigSoundCardSet::getTXTail() const
{
	return (unsigned int)m_txTail->GetValue();
}
