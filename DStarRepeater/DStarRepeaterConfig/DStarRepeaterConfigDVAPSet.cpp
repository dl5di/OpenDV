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

#include "DStarRepeaterConfigDVAPSet.h"
#include "SerialDataController.h"

const unsigned int BORDER_SIZE   = 5U;
const unsigned int CONTROL_WIDTH = 100U;
const unsigned int NAME_WIDTH    = 200U;

CDStarRepeaterConfigDVAPSet::CDStarRepeaterConfigDVAPSet(wxWindow* parent, int id, const wxString& port, unsigned int frequency, int power, int squelch) :
wxDialog(parent, id, wxString(_("DVAP Modem Settings"))),
m_port(NULL),
m_band(NULL),
m_frequency(NULL),
m_power(NULL),
m_squelch(NULL)
{
	wxBoxSizer* topSizer = new wxBoxSizer(wxVERTICAL);

	wxFlexGridSizer* sizer = new wxFlexGridSizer(2);

	wxStaticText* portLabel = new wxStaticText(this, -1, _("Port"));
	sizer->Add(portLabel, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	m_port = new wxChoice(this, -1, wxDefaultPosition, wxSize(NAME_WIDTH, -1));
	sizer->Add(m_port, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxArrayString ports = CSerialDataController::getDevices();
	for (unsigned int i = 0U; i < ports.GetCount(); i++)
		m_port->Append(ports.Item(i));
	bool res = m_port->SetStringSelection(port);
	if (!res)
		m_port->SetSelection(0);

	wxStaticText* bandLabel = new wxStaticText(this, -1, _("Band"));
	sizer->Add(bandLabel, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	m_band = new wxChoice(this, -1, wxDefaultPosition, wxSize(CONTROL_WIDTH, -1));
	m_band->Append(wxT("2m"));
	m_band->Append(wxT("70cms"));
	sizer->Add(m_band, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);
	m_band->SetSelection(frequency > 400000000U ? 1 : 0);

	wxStaticText* freqLabel = new wxStaticText(this, -1, _("Frequency (Hz)"));
	sizer->Add(freqLabel, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxString text;
	text.Printf(wxT("%u"), frequency);

	m_frequency = new wxTextCtrl(this, -1, text, wxDefaultPosition, wxSize(CONTROL_WIDTH, -1));
	sizer->Add(m_frequency, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* powerLabel = new wxStaticText(this, -1, _("Power (dBm)"));
	sizer->Add(powerLabel, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	m_power = new wxSpinCtrl(this, -1, wxEmptyString, wxDefaultPosition, wxSize(CONTROL_WIDTH, -1), wxSP_ARROW_KEYS, -12, 10, power);
	sizer->Add(m_power, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* squelchLabel = new wxStaticText(this, -1, _("Squelch (dBm)"));
	sizer->Add(squelchLabel, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	m_squelch = new wxSpinCtrl(this, -1, wxEmptyString, wxDefaultPosition, wxSize(CONTROL_WIDTH, -1), wxSP_ARROW_KEYS, -128, -45, squelch);
	sizer->Add(m_squelch, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	topSizer->Add(sizer);

	topSizer->Add(CreateButtonSizer(wxOK | wxCANCEL), 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	SetAutoLayout(true);

	topSizer->Fit(this);
	topSizer->SetSizeHints(this);

	SetSizer(topSizer);
}

CDStarRepeaterConfigDVAPSet::~CDStarRepeaterConfigDVAPSet()
{
}

bool CDStarRepeaterConfigDVAPSet::Validate()
{
	if (m_port->GetCurrentSelection() == wxNOT_FOUND)
		return false;

	unsigned int freq = getFrequency();

	int n = m_band->GetCurrentSelection();
	switch (n) {
		case 0:
			if (freq < 144000000U || freq > 148000000U) {
				wxMessageDialog dialog(this, _("The Frequency is out of range"), _("DVAP Modem Error"), wxICON_ERROR);
				dialog.ShowModal();
				return false;
			}
			break;

		case 1:
			if (freq < 420000000U || freq > 450000000U) {
				wxMessageDialog dialog(this, _("The Frequency is out of range"), _("DVAP Modem Error"), wxICON_ERROR);
				dialog.ShowModal();
				return false;
			}
			break;

		default:
			return false;
	}

	return true;
}

wxString CDStarRepeaterConfigDVAPSet::getPort() const
{
	return m_port->GetStringSelection();
}

unsigned int CDStarRepeaterConfigDVAPSet::getFrequency() const
{
	wxString hz = m_frequency->GetValue();

	unsigned long frequency;
	hz.ToULong(&frequency);

	return frequency;
}

int CDStarRepeaterConfigDVAPSet::getPower() const
{
	int power = m_power->GetValue();

	return power;
}

int CDStarRepeaterConfigDVAPSet::getSquelch() const
{
	int power = m_squelch->GetValue();

	return power;
}
