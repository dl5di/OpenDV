/*
 *   Copyright (C) 2009-2013,2015 by Jonathan Naylor G4KLX
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

#include "AnalogueRepeaterExternalSet.h"
#include "SoundCardReaderWriter.h"
#include "SerialPortSelector.h"

const unsigned int BORDER_SIZE    = 5U;
const unsigned int CONTROL_WIDTH1 = 300U;
const unsigned int CONTROL_WIDTH2 = 100U;

CAnalogueRepeaterExternalSet::CAnalogueRepeaterExternalSet(wxWindow* parent, int id, const wxString& title, ANALOGUE_EXTERNAL_MODE mode, const wxString& readDevice, const wxString& writeDevice, unsigned int delay, bool deEmphasis, bool preEmphasis, bool vogad, const wxString& device, SERIALPIN txPin, SERIALPIN rxPin, bool background) :
wxPanel(parent, id),
m_title(title),
m_mode(NULL),
m_readDevice(NULL),
m_writeDevice(NULL),
m_audioDelay(NULL),
m_deEmphasis(NULL),
m_preEmphasis(NULL),
m_vogad(NULL),
m_device(NULL),
m_txPin(NULL),
m_rxPin(NULL),
m_background(NULL)
{
	wxFlexGridSizer* sizer = new wxFlexGridSizer(2);

	wxStaticText* enableLabel = new wxStaticText(this, -1, _("External"));
	sizer->Add(enableLabel, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	m_mode = new wxChoice(this, -1, wxDefaultPosition, wxSize(CONTROL_WIDTH2, -1));
	m_mode->Append(_("Disabled"));
	m_mode->Append(_("Enabled"));
	m_mode->Append(_("Control Only"));
	sizer->Add(m_mode, 0, wxALL, BORDER_SIZE);
	m_mode->SetSelection(int(mode));

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

	wxStaticText* delayLabel = new wxStaticText(this, -1, _("Audio Delay"));
	sizer->Add(delayLabel, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	m_audioDelay = new wxChoice(this, -1, wxDefaultPosition, wxSize(CONTROL_WIDTH2, -1));
	m_audioDelay->Append(_("None"));
	m_audioDelay->Append(wxT("20 ms"));
	m_audioDelay->Append(wxT("40 ms"));
	m_audioDelay->Append(wxT("60 ms"));
	m_audioDelay->Append(wxT("80 ms"));
	m_audioDelay->Append(wxT("100 ms"));
	m_audioDelay->Append(wxT("120 ms"));
	m_audioDelay->Append(wxT("140 ms"));
	m_audioDelay->Append(wxT("160 ms"));
	m_audioDelay->Append(wxT("180 ms"));
	m_audioDelay->Append(wxT("200 ms"));
	m_audioDelay->Append(wxT("220 ms"));
	m_audioDelay->Append(wxT("240 ms"));
	m_audioDelay->Append(wxT("260 ms"));
	m_audioDelay->Append(wxT("280 ms"));
	m_audioDelay->Append(wxT("300 ms"));
	m_audioDelay->Append(wxT("320 ms"));
	m_audioDelay->Append(wxT("340 ms"));
	m_audioDelay->Append(wxT("360 ms"));
	m_audioDelay->Append(wxT("380 ms"));
	m_audioDelay->Append(wxT("400 ms"));
	m_audioDelay->Append(wxT("420 ms"));
	m_audioDelay->Append(wxT("440 ms"));
	m_audioDelay->Append(wxT("460 ms"));
	m_audioDelay->Append(wxT("480 ms"));
	m_audioDelay->Append(wxT("500 ms"));
	m_audioDelay->Append(wxT("520 ms"));
	m_audioDelay->Append(wxT("540 ms"));
	m_audioDelay->Append(wxT("560 ms"));
	m_audioDelay->Append(wxT("580 ms"));
	m_audioDelay->Append(wxT("600 ms"));
	m_audioDelay->Append(wxT("620 ms"));
	m_audioDelay->Append(wxT("640 ms"));
	m_audioDelay->Append(wxT("660 ms"));
	m_audioDelay->Append(wxT("680 ms"));
	m_audioDelay->Append(wxT("700 ms"));
	sizer->Add(m_audioDelay, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);
	m_audioDelay->SetSelection(delay);

	wxStaticText* deEmphasisLabel = new wxStaticText(this, -1, _("De-Emphasis"));
	sizer->Add(deEmphasisLabel, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	m_deEmphasis = new wxChoice(this, -1, wxDefaultPosition, wxSize(CONTROL_WIDTH2, -1));
	m_deEmphasis->Append(_("Off"));
	m_deEmphasis->Append(_("On"));
	sizer->Add(m_deEmphasis, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);
	m_deEmphasis->SetSelection(deEmphasis ? 1 : 0);

	wxStaticText* preEmphasisLabel = new wxStaticText(this, -1, _("Pre-Emphasis"));
	sizer->Add(preEmphasisLabel, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	m_preEmphasis = new wxChoice(this, -1, wxDefaultPosition, wxSize(CONTROL_WIDTH2, -1));
	m_preEmphasis->Append(_("Off"));
	m_preEmphasis->Append(_("On"));
	sizer->Add(m_preEmphasis, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);
	m_preEmphasis->SetSelection(preEmphasis ? 1 : 0);

	wxStaticText* vogadLabel = new wxStaticText(this, -1, _("VOGAD"));
	sizer->Add(vogadLabel, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	m_vogad = new wxChoice(this, -1, wxDefaultPosition, wxSize(CONTROL_WIDTH2, -1));
	m_vogad->Append(_("Off"));
	m_vogad->Append(_("On"));
	sizer->Add(m_vogad, 0, wxALL, BORDER_SIZE);
	m_vogad->SetSelection(vogad ? 1 : 0);

	wxArrayString devices = CSerialPortSelector::getDevices();

	wxStaticText* deviceLabel = new wxStaticText(this, -1, _("Device"));
	sizer->Add(deviceLabel, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	m_device = new wxChoice(this, -1, wxDefaultPosition, wxSize(CONTROL_WIDTH2, -1), devices);
	sizer->Add(m_device, 0, wxALL, BORDER_SIZE);

	m_device->Insert(_("None"), 0U);

	if (devices.GetCount() == 0U) {
		m_device->SetSelection(0);
	} else {
		bool res = m_device->SetStringSelection(device);
		if (!res)
			m_device->SetSelection(0);
	}

	wxStaticText* txPinLabel = new wxStaticText(this, -1, _("Transmit"));
	sizer->Add(txPinLabel, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	m_txPin = new wxChoice(this, -1, wxDefaultPosition, wxSize(CONTROL_WIDTH2, -1));
	m_txPin->Append(wxT("DTR"));
	m_txPin->Append(wxT("RTS"));
//	m_txPin->Append(wxT("EchoLink ASCII"));
	sizer->Add(m_txPin, 0, wxALL, BORDER_SIZE);

	switch (txPin) {
		case SERIAL_DTR:
			m_txPin->SetSelection(0);
			break;
		case SERIAL_RTS:
			m_txPin->SetSelection(1);
			break;
		case SERIAL_ECHOLINK:
			m_txPin->SetSelection(2);
			break;
		default:
			m_txPin->SetSelection(0);
			break;
	}

	wxStaticText* rxPinLabel = new wxStaticText(this, -1, _("Squelch"));
	sizer->Add(rxPinLabel, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	m_rxPin = new wxChoice(this, -1, wxDefaultPosition, wxSize(CONTROL_WIDTH2, -1));
	m_rxPin->Append(wxT("CD"));
	m_rxPin->Append(wxT("CTS"));
	m_rxPin->Append(wxT("DSR"));
	sizer->Add(m_rxPin, 0, wxALL, BORDER_SIZE);

	switch (rxPin) {
		case SERIAL_CD:
			m_rxPin->SetSelection(0);
			break;
		case SERIAL_CTS:
			m_rxPin->SetSelection(1);
			break;
		case SERIAL_DSR:
			m_rxPin->SetSelection(2);
			break;
		default:
			m_rxPin->SetSelection(0);
			break;
	}

	wxStaticText* backgroundLabel = new wxStaticText(this, -1, _("Background"));
	sizer->Add(backgroundLabel, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	m_background = new wxChoice(this, -1, wxDefaultPosition, wxSize(CONTROL_WIDTH2, -1));
	m_background->Append(_("Off"));
	m_background->Append(_("On"));
	sizer->Add(m_background, 0, wxALL, BORDER_SIZE);
	m_background->SetSelection(background ? 1 : 0);

	SetAutoLayout(true);

	sizer->Fit(this);
	sizer->SetSizeHints(this);

	SetSizer(sizer);
}


CAnalogueRepeaterExternalSet::~CAnalogueRepeaterExternalSet()
{
}

bool CAnalogueRepeaterExternalSet::Validate()
{
	if (m_mode->GetCurrentSelection() == wxNOT_FOUND)
		return false;

	if (m_readDevice->GetCurrentSelection() == wxNOT_FOUND)
		return false;

	if (m_writeDevice->GetCurrentSelection() == wxNOT_FOUND)
		return false;

	if (m_audioDelay->GetCurrentSelection() == wxNOT_FOUND)
		return false;

	if (m_deEmphasis->GetCurrentSelection() == wxNOT_FOUND)
		return false;

	if (m_preEmphasis->GetCurrentSelection() == wxNOT_FOUND)
		return false;

	if (m_vogad->GetCurrentSelection() == wxNOT_FOUND)
		return false;

	if (m_device->GetCurrentSelection() == wxNOT_FOUND)
		return false;

	if (m_txPin->GetCurrentSelection() == wxNOT_FOUND)
		return false;

	if (m_rxPin->GetCurrentSelection() == wxNOT_FOUND)
		return false;

	return m_background->GetCurrentSelection() != wxNOT_FOUND;
}

ANALOGUE_EXTERNAL_MODE CAnalogueRepeaterExternalSet::getMode() const
{
	return ANALOGUE_EXTERNAL_MODE(m_mode->GetCurrentSelection());
}

wxString CAnalogueRepeaterExternalSet::getReadDevice() const
{
	wxString device = m_readDevice->GetStringSelection();

	if (device.IsSameAs(_("None")))
		return wxEmptyString;
	else
		return device;
}

wxString CAnalogueRepeaterExternalSet::getWriteDevice() const
{
	wxString device = m_writeDevice->GetStringSelection();

	if (device.IsSameAs(_("None")))
		return wxEmptyString;
	else
		return device;
}

bool CAnalogueRepeaterExternalSet::getDeEmphasis() const
{
	int n = m_deEmphasis->GetCurrentSelection();

	switch (n) {
		case 0:
			return false;
		case 1:
			return true;
		default:
			return false;
	}
}

bool CAnalogueRepeaterExternalSet::getPreEmphasis() const
{
	int n = m_preEmphasis->GetCurrentSelection();

	switch (n) {
		case 0:
			return false;
		case 1:
			return true;
		default:
			return false;
	}
}

unsigned int CAnalogueRepeaterExternalSet::getAudioDelay() const
{
	int n = m_audioDelay->GetCurrentSelection();

	if (n == wxNOT_FOUND)
		return 0U;
	else
		return n;
}

bool CAnalogueRepeaterExternalSet::getVOGAD() const
{
	int n = m_vogad->GetCurrentSelection();

	switch (n) {
		case 0:
			return false;
		case 1:
			return true;
		default:
			return false;
	}
}

wxString CAnalogueRepeaterExternalSet::getDevice() const
{
	wxString device = m_device->GetStringSelection();

	if (device.IsSameAs(_("None")))
		return wxEmptyString;
	else
		return device;
}

SERIALPIN CAnalogueRepeaterExternalSet::getTXPin() const
{
	int n = m_txPin->GetCurrentSelection();

	switch (n) {
		case 0:
			return SERIAL_DTR;
		case 1:
			return SERIAL_RTS;
		case 2:
			return SERIAL_ECHOLINK;
		default:
			return SERIAL_DTR;
	}
}

SERIALPIN CAnalogueRepeaterExternalSet::getRXPin() const
{
	int n = m_rxPin->GetCurrentSelection();

	switch (n) {
		case 0:
			return SERIAL_CD;
		case 1:
			return SERIAL_CTS;
		case 2:
			return SERIAL_DSR;
		default:
			return SERIAL_CD;
	}
}

bool CAnalogueRepeaterExternalSet::getBackground() const
{
	int n = m_background->GetCurrentSelection();

	switch (n) {
		case 0:
			return false;
		case 1:
			return true;
		default:
			return false;
	}
}
