/*
 *   Copyright (C) 2009-2012 by Jonathan Naylor G4KLX
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

#include "SoundCardRepeaterRadioSet.h"
#include "SoundCardReaderWriter.h"
#include "SoundCardRepeaterApp.h"

enum {
	Slider_Squelch = 7000,
	Slider_RX,
	Slider_TX
};

BEGIN_EVENT_TABLE(CSoundCardRepeaterRadioSet, wxPanel)
	EVT_COMMAND_SCROLL_THUMBTRACK(Slider_RX,      CSoundCardRepeaterRadioSet::onRXLevel)
	EVT_COMMAND_SCROLL_THUMBTRACK(Slider_TX,      CSoundCardRepeaterRadioSet::onTXLevel)
	EVT_COMMAND_SCROLL_THUMBTRACK(Slider_Squelch, CSoundCardRepeaterRadioSet::onSquelchLevel)
END_EVENT_TABLE()

const unsigned int BORDER_SIZE    = 5U;
const unsigned int CONTROL_WIDTH1 = 300U;
const unsigned int CONTROL_WIDTH2 = 100U;

CSoundCardRepeaterRadioSet::CSoundCardRepeaterRadioSet(wxWindow* parent, int id, const wxString& title, const wxString& readDevice, const wxString& writeDevice, wxFloat32 rxLevel, wxFloat32 txLevel, SQUELCH_MODE squelchMode, wxFloat32 squelchLevel, bool rxInvert, bool txInvert) :
wxPanel(parent, id),
m_title(title),
m_readDevice(NULL),
m_writeDevice(NULL),
m_rxLevel(NULL),
m_txLevel(NULL),
m_squelchMode(NULL),
m_squelchLevel(NULL),
m_rxInvert(NULL),
m_txInvert(NULL)
{
	wxFlexGridSizer* sizer = new wxFlexGridSizer(2);

	wxArrayString readDevices = CSoundCardReaderWriter::getReadDevices();
	wxArrayString writeDevices = CSoundCardReaderWriter::getWriteDevices();

	wxStaticText* readLabel = new wxStaticText(this, -1, _("Input"));
	sizer->Add(readLabel, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	m_readDevice = new wxChoice(this, -1, wxDefaultPosition, wxSize(CONTROL_WIDTH1, -1), readDevices);
	sizer->Add(m_readDevice, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* writeLabel = new wxStaticText(this, -1, _("Output"));
	sizer->Add(writeLabel, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	m_writeDevice = new wxChoice(this, -1, wxDefaultPosition, wxSize(CONTROL_WIDTH1, -1), writeDevices);
	sizer->Add(m_writeDevice, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* rxLevelLabel = new wxStaticText(this, -1, _("RX Level (%)"));
	sizer->Add(rxLevelLabel, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	m_rxLevel = new wxSlider(this, Slider_RX, int(rxLevel * 100.0F + 0.5F), 0, 100, wxDefaultPosition, wxSize(CONTROL_WIDTH1, -1), wxSL_HORIZONTAL | wxSL_LABELS);
	sizer->Add(m_rxLevel, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* txLevelLabel = new wxStaticText(this, -1, _("TX Level (%)"));
	sizer->Add(txLevelLabel, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	m_txLevel = new wxSlider(this, Slider_TX, int(txLevel * 100.0F + 0.5F), 0, 100, wxDefaultPosition, wxSize(CONTROL_WIDTH1, -1), wxSL_HORIZONTAL | wxSL_LABELS);
	sizer->Add(m_txLevel, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* squelchLevelLabel = new wxStaticText(this, -1, _("Squelch Level (%)"));
	sizer->Add(squelchLevelLabel, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	m_squelchLevel = new wxSlider(this, Slider_Squelch, int(squelchLevel * 100.0F + 0.5F), 0, 100, wxDefaultPosition, wxSize(CONTROL_WIDTH1, -1), wxSL_HORIZONTAL | wxSL_LABELS);
	sizer->Add(m_squelchLevel, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* squelchModeLabel = new wxStaticText(this, -1, _("Squelch Mode"));
	sizer->Add(squelchModeLabel, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	m_squelchMode = new wxChoice(this, -1, wxDefaultPosition, wxSize(CONTROL_WIDTH2, -1));
	m_squelchMode->Append(_("Normal"));
	m_squelchMode->Append(_("Inverted"));
	m_squelchMode->Append(_("Open"));
	sizer->Add(m_squelchMode, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* rxInvertLabel = new wxStaticText(this, -1, _("RX Inversion"));
	sizer->Add(rxInvertLabel, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	m_rxInvert = new wxChoice(this, -1, wxDefaultPosition, wxSize(CONTROL_WIDTH2, -1));
	m_rxInvert->Append(_("Off"));
	m_rxInvert->Append(_("On"));
	sizer->Add(m_rxInvert, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* txInvertLabel = new wxStaticText(this, -1, _("TX Inversion"));
	sizer->Add(txInvertLabel, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	m_txInvert = new wxChoice(this, -1, wxDefaultPosition, wxSize(CONTROL_WIDTH2, -1));
	m_txInvert->Append(_("Off"));
	m_txInvert->Append(_("On"));
	sizer->Add(m_txInvert, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	if (readDevices.GetCount() == 0U) {
		m_readDevice->Append(_("None"));
		m_readDevice->SetSelection(0);
	} else {
		bool res = m_readDevice->SetStringSelection(readDevice);
		if (!res)
			m_readDevice->SetSelection(0);
	}

	if (writeDevices.GetCount() == 0U) {
		m_writeDevice->Append(_("None"));
		m_writeDevice->SetSelection(0);
	} else {
		bool res = m_writeDevice->SetStringSelection(writeDevice);
		if (!res)
			m_writeDevice->SetSelection(0);
	}

	m_squelchMode->SetSelection(int(squelchMode));
	m_rxInvert->SetSelection(rxInvert ? 1 : 0);
	m_txInvert->SetSelection(txInvert ? 1 : 0);

	SetAutoLayout(true);

	sizer->Fit(this);
	sizer->SetSizeHints(this);

	SetSizer(sizer);
}


CSoundCardRepeaterRadioSet::~CSoundCardRepeaterRadioSet()
{
}

bool CSoundCardRepeaterRadioSet::Validate()
{
	if (m_readDevice->GetCurrentSelection() == wxNOT_FOUND)
		return false;

	if (m_writeDevice->GetCurrentSelection() == wxNOT_FOUND)
		return false;

	if (m_squelchMode->GetCurrentSelection() == wxNOT_FOUND)
		return false;

	if (m_rxInvert->GetCurrentSelection() == wxNOT_FOUND)
		return false;

	return m_txInvert->GetCurrentSelection() != wxNOT_FOUND;
}

wxString CSoundCardRepeaterRadioSet::getReadDevice() const
{
	wxString device = m_readDevice->GetStringSelection();

	if (device.IsSameAs(_("None")))
		return wxEmptyString;
	else
		return device;
}

wxString CSoundCardRepeaterRadioSet::getWriteDevice() const
{
	wxString device = m_writeDevice->GetStringSelection();

	if (device.IsSameAs(_("None")))
		return wxEmptyString;
	else
		return device;
}

wxFloat32 CSoundCardRepeaterRadioSet::getRXLevel() const
{
	int n = m_rxLevel->GetValue();

	return wxFloat32(n) / 100.0F;
}

wxFloat32 CSoundCardRepeaterRadioSet::getTXLevel() const
{
	int n = m_txLevel->GetValue();

	return wxFloat32(n) / 100.0F;
}

SQUELCH_MODE CSoundCardRepeaterRadioSet::getSquelchMode() const
{
	int n = m_squelchMode->GetCurrentSelection();

	switch (n) {
		case 0:
			return SM_NORMAL;
		case 1:
			return SM_INVERTED;
		case 2:
			return SM_OPEN;
		default:
			return SM_NORMAL;
	}
}

wxFloat32 CSoundCardRepeaterRadioSet::getSquelchLevel() const
{
	int n = m_squelchLevel->GetValue();

	return wxFloat32(n) / 100.0F;
}

bool CSoundCardRepeaterRadioSet::getRXInvert() const
{
	int n = m_rxInvert->GetCurrentSelection();

	switch (n) {
		case 0:
			return false;
		case 1:
			return true;
		default:
			return false;
	}
}

bool CSoundCardRepeaterRadioSet::getTXInvert() const
{
	int n = m_txInvert->GetCurrentSelection();

	switch (n) {
		case 0:
			return false;
		case 1:
			return true;
		default:
			return false;
	}
}

void CSoundCardRepeaterRadioSet::onRXLevel(wxScrollEvent &event)
{
	int n = event.GetPosition();

	wxFloat32 level = wxFloat32(n) / 100.0F;

	::wxGetApp().setRXLevel(level);
}

void CSoundCardRepeaterRadioSet::onTXLevel(wxScrollEvent &event)
{
	int n = event.GetPosition();

	wxFloat32 level = wxFloat32(n) / 100.0F;

	::wxGetApp().setTXLevel(level);
}

void CSoundCardRepeaterRadioSet::onSquelchLevel(wxScrollEvent &event)
{
	int n = event.GetPosition();

	wxFloat32 level = wxFloat32(n) / 100.0F;

	::wxGetApp().setSquelchLevel(level);
}
