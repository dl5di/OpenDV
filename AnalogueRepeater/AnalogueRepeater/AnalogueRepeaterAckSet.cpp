/*
 *   Copyright (C) 2009-2013 by Jonathan Naylor G4KLX
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

#include "AnalogueRepeaterAckSet.h"
#include "AnalogueRepeaterApp.h"

#include <wx/filename.h>

enum {
	Slider_Level = 7000,

	Choice_RadioAck = 8006,
	Choice_ExternalAck,
	Choice_BatteryAck,

	Button_RadioAck,
	Button_ExternalAck,
	Button_BatteryAck
};

BEGIN_EVENT_TABLE(CAnalogueRepeaterAckSet, wxPanel)
	EVT_COMMAND_SCROLL_THUMBTRACK(Slider_Level, CAnalogueRepeaterAckSet::onLevel)

	EVT_CHOICE(Choice_RadioAck,    CAnalogueRepeaterAckSet::onRadioAckChoice)
	EVT_CHOICE(Choice_ExternalAck, CAnalogueRepeaterAckSet::onExternalAckChoice)
	EVT_CHOICE(Choice_BatteryAck,  CAnalogueRepeaterAckSet::onBatteryAckChoice)

	EVT_BUTTON(Button_RadioAck,    CAnalogueRepeaterAckSet::onRadioAckButton)
	EVT_BUTTON(Button_ExternalAck, CAnalogueRepeaterAckSet::onExternalAckButton)
	EVT_BUTTON(Button_BatteryAck,  CAnalogueRepeaterAckSet::onBatteryAckButton)
END_EVENT_TABLE()

const unsigned int ACK_WIDTH1 = 300U;
const unsigned int ACK_WIDTH2 = 110U;

const unsigned int BORDER_SIZE = 5U;

CAnalogueRepeaterAckSet::CAnalogueRepeaterAckSet(wxWindow* parent, int id, const wxString& title, AUDIO_SOURCE radioAckSource, const wxString& radioAck, AUDIO_SOURCE externalAckSource, const wxString& externalAck, AUDIO_SOURCE batteryAckSource, const wxString& batteryAck, unsigned int speed, unsigned int freq, wxFloat32 level, unsigned int delay, unsigned int minimum) :
wxPanel(parent, id),
m_title(title),
m_radioAck(NULL),
m_cwRadioAck(NULL),
m_wavRadioAck(NULL),
m_buttonRadioAck(NULL),
m_externalAck(NULL),
m_cwExternalAck(NULL),
m_wavExternalAck(NULL),
m_buttonExternalAck(NULL),
m_batteryAck(NULL),
m_cwBatteryAck(NULL),
m_wavBatteryAck(NULL),
m_buttonBatteryAck(NULL),
m_speed(NULL),
m_freq(NULL),
m_level(NULL),
m_delay(NULL),
m_minimum(NULL)
{
	wxFlexGridSizer* sizer = new wxFlexGridSizer(3);

	m_radioAck = new wxChoice(this, Choice_RadioAck, wxDefaultPosition, wxSize(ACK_WIDTH2, -1));
	m_radioAck->Append(_("CW Radio Ack"));
	m_radioAck->Append(_("WAV Radio Ack"));
	sizer->Add(m_radioAck, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_cwRadioAck = new CCWTextCtrl(this, -1, wxEmptyString, wxDefaultPosition, wxSize(ACK_WIDTH1, -1));
	m_cwRadioAck->SetMaxLength(20U);
	sizer->Add(m_cwRadioAck, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* dummy1Label = new wxStaticText(this, -1, wxEmptyString);
	sizer->Add(dummy1Label, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	wxStaticText* dummy2Label = new wxStaticText(this, -1, wxEmptyString);
	sizer->Add(dummy2Label, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_wavRadioAck = new wxTextCtrl(this, -1, wxEmptyString, wxDefaultPosition, wxSize(ACK_WIDTH1, -1));
	sizer->Add(m_wavRadioAck, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	m_buttonRadioAck = new wxButton(this, Button_RadioAck, _("Choose..."));
	sizer->Add(m_buttonRadioAck, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	m_externalAck = new wxChoice(this, Choice_ExternalAck, wxDefaultPosition, wxSize(ACK_WIDTH2, -1));
	m_externalAck->Append(_("CW External Ack"));
	m_externalAck->Append(_("WAV External Ack"));
	sizer->Add(m_externalAck, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_cwExternalAck = new CCWTextCtrl(this, -1, wxEmptyString, wxDefaultPosition, wxSize(ACK_WIDTH1, -1));
	m_cwExternalAck->SetMaxLength(20U);
	sizer->Add(m_cwExternalAck, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* dummy3Label = new wxStaticText(this, -1, wxEmptyString);
	sizer->Add(dummy3Label, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	wxStaticText* dummy4Label = new wxStaticText(this, -1, wxEmptyString);
	sizer->Add(dummy4Label, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_wavExternalAck = new wxTextCtrl(this, -1, wxEmptyString, wxDefaultPosition, wxSize(ACK_WIDTH1, -1));
	sizer->Add(m_wavExternalAck, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	m_buttonExternalAck = new wxButton(this, Button_ExternalAck, _("Choose..."));
	sizer->Add(m_buttonExternalAck, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	m_batteryAck = new wxChoice(this, Choice_BatteryAck, wxDefaultPosition, wxSize(ACK_WIDTH2, -1));
	m_batteryAck->Append(_("CW Battery Ack"));
	m_batteryAck->Append(_("WAV Battery Ack"));
	sizer->Add(m_batteryAck, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_cwBatteryAck = new CCWTextCtrl(this, -1, wxEmptyString, wxDefaultPosition, wxSize(ACK_WIDTH1, -1));
	m_cwBatteryAck->SetMaxLength(20U);
	sizer->Add(m_cwBatteryAck, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* dummy5Label = new wxStaticText(this, -1, wxEmptyString);
	sizer->Add(dummy5Label, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	wxStaticText* dummy6Label = new wxStaticText(this, -1, wxEmptyString);
	sizer->Add(dummy6Label, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_wavBatteryAck = new wxTextCtrl(this, -1, wxEmptyString, wxDefaultPosition, wxSize(ACK_WIDTH1, -1));
	sizer->Add(m_wavBatteryAck, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	m_buttonBatteryAck = new wxButton(this, Button_BatteryAck, _("Choose..."));
	sizer->Add(m_buttonBatteryAck, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* speedLabel = new wxStaticText(this, -1, _("Speed (WPM)"));
	sizer->Add(speedLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_speed = new wxSlider(this, -1, speed, 8, 25, wxDefaultPosition, wxSize(ACK_WIDTH1, -1), wxSL_HORIZONTAL | wxSL_LABELS);
	sizer->Add(m_speed, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* dummy7Label = new wxStaticText(this, -1, wxEmptyString);
	sizer->Add(dummy7Label, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	wxStaticText* freqLabel = new wxStaticText(this, -1, _("Frequency (Hz)"));
	sizer->Add(freqLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_freq = new wxSlider(this, -1, freq, 400, 2000, wxDefaultPosition, wxSize(ACK_WIDTH1, -1), wxSL_HORIZONTAL | wxSL_LABELS);
	sizer->Add(m_freq, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* dummy8Label = new wxStaticText(this, -1, wxEmptyString);
	sizer->Add(dummy8Label, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	wxStaticText* levelLabel = new wxStaticText(this, -1, _("Level (%)"));
	sizer->Add(levelLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_level = new wxSlider(this, Slider_Level, int(level * 100.0F + 0.5F), 0, 100, wxDefaultPosition, wxSize(ACK_WIDTH1, -1), wxSL_HORIZONTAL | wxSL_LABELS);
	sizer->Add(m_level, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* dummy9Label = new wxStaticText(this, -1, wxEmptyString);
	sizer->Add(dummy9Label, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	wxStaticText* delayLabel = new wxStaticText(this, -1, _("Delay"));
	sizer->Add(delayLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_delay = new wxChoice(this, -1, wxDefaultPosition, wxSize(ACK_WIDTH2, -1));
	m_delay->Append(wxT("100 ms"));
	m_delay->Append(wxT("300 ms"));
	m_delay->Append(wxT("500 ms"));
	m_delay->Append(wxT("1000 ms"));
	m_delay->Append(wxT("1500 ms"));
	m_delay->Append(wxT("2000 ms"));
	m_delay->Append(wxT("2500 ms"));
	m_delay->Append(wxT("3000 ms"));
	sizer->Add(m_delay, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	switch (delay) {
		case 3U:
		case 3000U:
			m_delay->SetSelection(7); break;
		case 2500U:
			m_delay->SetSelection(6); break;
		case 2U:
		case 2000U:
			m_delay->SetSelection(5); break;
		case 1500U:
			m_delay->SetSelection(4); break;
		case 1U:
		case 1000U:
			m_delay->SetSelection(3); break;
		case 500U:
			m_delay->SetSelection(2); break;
		case 300U:
			m_delay->SetSelection(1); break;
		case 100U:
			m_delay->SetSelection(0); break;
		default:
			m_delay->SetSelection(3); break;
	}

	wxStaticText* dummy10Label = new wxStaticText(this, -1, wxEmptyString);
	sizer->Add(dummy10Label, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	wxStaticText* minimumLabel = new wxStaticText(this, -1, _("Minimum (s)"));
	sizer->Add(minimumLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_minimum = new wxChoice(this, -1, wxDefaultPosition, wxSize(ACK_WIDTH2, -1));
	m_minimum->Append(wxT("0 ms"));
	m_minimum->Append(wxT("100 ms"));
	m_minimum->Append(wxT("300 ms"));
	m_minimum->Append(wxT("500 ms"));
	m_minimum->Append(wxT("1000 ms"));
	m_minimum->Append(wxT("2000 ms"));
	m_minimum->Append(wxT("3000 ms"));
	m_minimum->Append(wxT("4000 ms"));
	m_minimum->Append(wxT("5000 ms"));
	m_minimum->Append(wxT("6000 ms"));
	m_minimum->Append(wxT("7000 ms"));
	m_minimum->Append(wxT("8000 ms"));
	m_minimum->Append(wxT("9000 ms"));
	m_minimum->Append(wxT("10000 ms"));
	sizer->Add(m_minimum, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	switch (minimum) {
		case 0U:
			m_minimum->SetSelection(0); break;
		case 100U:
			m_minimum->SetSelection(1); break;
		case 300U:
			m_minimum->SetSelection(2); break;
		case 500U:
			m_minimum->SetSelection(3); break;
		case 1U:
		case 1000U:
			m_minimum->SetSelection(4); break;
		case 2U:
		case 2000U:
			m_minimum->SetSelection(5); break;
		case 3U:
		case 3000U:
			m_minimum->SetSelection(6); break;
		case 4U:
		case 4000U:
			m_minimum->SetSelection(7); break;
		case 5U:
		case 5000U:
			m_minimum->SetSelection(8); break;
		case 6U:
		case 6000U:
			m_minimum->SetSelection(9); break;
		case 7U:
		case 7000U:
			m_minimum->SetSelection(10); break;
		case 8U:
		case 8000U:
			m_minimum->SetSelection(11); break;
		case 9U:
		case 9000U:
			m_minimum->SetSelection(12); break;
		case 10U:
		case 10000U:
			m_minimum->SetSelection(13); break;
		default:
			m_minimum->SetSelection(4); break;
	}

	SetAutoLayout(true);

	// sizer->Fit(this);
	// sizer->SetSizeHints(this);

	SetSizer(sizer);

	switch (radioAckSource) {
		case AS_CW_KEYER:
			m_cwRadioAck->Enable();
			m_wavRadioAck->Disable();
			m_buttonRadioAck->Disable();
			m_radioAck->SetSelection(0);
			m_cwRadioAck->SetValue(radioAck);
			break;
		case AS_WAV_FILE:
			m_cwRadioAck->Disable();
			m_wavRadioAck->Enable();
			m_buttonRadioAck->Enable();
			m_radioAck->SetSelection(1);
			m_wavRadioAck->SetValue(radioAck);
			break;
	}

	switch (externalAckSource) {
		case AS_CW_KEYER:
			m_cwExternalAck->Enable();
			m_wavExternalAck->Disable();
			m_buttonExternalAck->Disable();
			m_externalAck->SetSelection(0);
			m_cwExternalAck->SetValue(externalAck);
			break;
		case AS_WAV_FILE:
			m_cwExternalAck->Disable();
			m_wavExternalAck->Enable();
			m_buttonExternalAck->Enable();
			m_externalAck->SetSelection(1);
			m_wavExternalAck->SetValue(externalAck);
			break;
	}

	switch (batteryAckSource) {
		case AS_CW_KEYER:
			m_cwBatteryAck->Enable();
			m_wavBatteryAck->Disable();
			m_buttonBatteryAck->Disable();
			m_batteryAck->SetSelection(0);
			m_cwBatteryAck->SetValue(batteryAck);
			break;
		case AS_WAV_FILE:
			m_cwBatteryAck->Disable();
			m_wavBatteryAck->Enable();
			m_buttonBatteryAck->Enable();
			m_batteryAck->SetSelection(1);
			m_wavBatteryAck->SetValue(batteryAck);
			break;
	}
}

CAnalogueRepeaterAckSet::~CAnalogueRepeaterAckSet()
{
}

bool CAnalogueRepeaterAckSet::Validate()
{
	int n = m_radioAck->GetSelection();
	if (n == wxNOT_FOUND) {
		wxMessageDialog dialog(this, _("The Radio Ack Choice must be made"), m_title + _(" Error"), wxICON_ERROR);
		dialog.ShowModal();
		return false;
	}

	bool res = !getRadioAck().IsEmpty();
	if (!res) {
		wxMessageDialog dialog(this, _("The Radio Ack may not be empty"), m_title + _(" Error"), wxICON_ERROR);
		dialog.ShowModal();
		return false;
	}

	n = m_externalAck->GetSelection();
	if (n == wxNOT_FOUND) {
		wxMessageDialog dialog(this, _("The External Ack Choice must be made"), m_title + _(" Error"), wxICON_ERROR);
		dialog.ShowModal();
		return false;
	}

	res = !getExternalAck().IsEmpty();
	if (!res) {
		wxMessageDialog dialog(this, _("The External Ack may not be empty"), m_title + _(" Error"), wxICON_ERROR);
		dialog.ShowModal();
		return false;
	}

	n = m_batteryAck->GetSelection();
	if (n == wxNOT_FOUND) {
		wxMessageDialog dialog(this, _("The Battery Ack Choice must be made"), m_title + _(" Error"), wxICON_ERROR);
		dialog.ShowModal();
		return false;
	}

	res = !getBatteryAck().IsEmpty();
	if (!res) {
		wxMessageDialog dialog(this, _("The Battery Ack may not be empty"), m_title + _(" Error"), wxICON_ERROR);
		dialog.ShowModal();
		return false;
	}

	n = m_delay->GetSelection();
	if (!res) {
		wxMessageDialog dialog(this, _("The Ack Delay may not be empty"), m_title + _(" Error"), wxICON_ERROR);
		dialog.ShowModal();
		return false;
	}

	return true;
}

AUDIO_SOURCE CAnalogueRepeaterAckSet::getRadioAckSource() const
{
	int n = m_radioAck->GetSelection();

	switch (n) {
		case 1:
			return AS_WAV_FILE;
		default:
			return AS_CW_KEYER;
	}
}

wxString CAnalogueRepeaterAckSet::getRadioAck() const
{
	AUDIO_SOURCE source = getRadioAckSource();

	switch (source) {
		case AS_CW_KEYER:
			return m_cwRadioAck->GetValue().MakeUpper();
		case AS_WAV_FILE:
			return m_wavRadioAck->GetValue();
		default:
			return wxEmptyString;
	}
}

AUDIO_SOURCE CAnalogueRepeaterAckSet::getExternalAckSource() const
{
	int n = m_externalAck->GetSelection();

	switch (n) {
		case 1:
			return AS_WAV_FILE;
		default:
			return AS_CW_KEYER;
	}
}

wxString CAnalogueRepeaterAckSet::getExternalAck() const
{
	AUDIO_SOURCE source = getExternalAckSource();

	switch (source) {
		case AS_CW_KEYER:
			return m_cwExternalAck->GetValue().MakeUpper();
		case AS_WAV_FILE:
			return m_wavExternalAck->GetValue();
		default:
			return wxEmptyString;
	}
}

AUDIO_SOURCE CAnalogueRepeaterAckSet::getBatteryAckSource() const
{
	int n = m_batteryAck->GetSelection();

	switch (n) {
		case 1:
			return AS_WAV_FILE;
		default:
			return AS_CW_KEYER;
	}
}

wxString CAnalogueRepeaterAckSet::getBatteryAck() const
{
	AUDIO_SOURCE source = getBatteryAckSource();

	switch (source) {
		case AS_CW_KEYER:
			return m_cwBatteryAck->GetValue().MakeUpper();
		case AS_WAV_FILE:
			return m_wavBatteryAck->GetValue();
		default:
			return wxEmptyString;
	}
}

unsigned int CAnalogueRepeaterAckSet::getSpeed() const
{
	return m_speed->GetValue();
}

unsigned int CAnalogueRepeaterAckSet::getFreq() const
{
	return m_freq->GetValue();
}

wxFloat32 CAnalogueRepeaterAckSet::getLevel() const
{
	return wxFloat32(m_level->GetValue()) / 100.0F;
}

unsigned int CAnalogueRepeaterAckSet::getDelay() const
{
	int n = m_delay->GetSelection();
	switch (n) {
		case 0:
			return 100U;
		case 1:
			return 300U;
		case 2:
			return 500U;
		case 3:
			return 1000U;
		case 4:
			return 1500U;
		case 5:
			return 2000U;
		case 6:
			return 2500U;
		case 7:
			return 3000U;
		default:
			return 1000U;
	}
}

unsigned int CAnalogueRepeaterAckSet::getMinimum() const
{
	int n = m_minimum->GetSelection();
	switch (n) {
		case 0:
			return 0U;
		case 1:
			return 100U;
		case 2:
			return 300U;
		case 3:
			return 500U;
		case 4:
			return 1000U;
		case 5:
			return 2000U;
		case 6:
			return 3000U;
		case 7:
			return 4000U;
		case 8:
			return 5000U;
		case 9:
			return 6000U;
		case 10:
			return 7000U;
		case 11:
			return 8000U;
		case 12:
			return 9000U;
		case 13:
			return 10000U;
		default:
			return 1000U;
	}
}

void CAnalogueRepeaterAckSet::onLevel(wxScrollEvent& event)
{
	int n = event.GetPosition();

	wxFloat32 level = wxFloat32(n) / 100.0F;

	::wxGetApp().setAckLevel(level);
}

void CAnalogueRepeaterAckSet::onRadioAckChoice(wxCommandEvent&)
{
	int n = m_radioAck->GetSelection();

	switch (n) {
		case 0:
			m_cwRadioAck->Enable();
			m_wavRadioAck->Disable();
			m_buttonRadioAck->Disable();
			break;
		case 1:
			m_cwRadioAck->Disable();
			m_wavRadioAck->Enable();
			m_buttonRadioAck->Enable();
			break;
		default:
			break;
	}
}

void CAnalogueRepeaterAckSet::onRadioAckButton(wxCommandEvent&)
{
	wxString file = m_wavRadioAck->GetValue();

	wxString path;
	wxString name;

	if (!file.IsEmpty()) {
		wxFileName fileName(file);
		path = fileName.GetPath();
		name = fileName.GetFullName();
	}

	wxFileDialog dialog(this, _("Select a WAV File"), path, name, _("WAV file (*.wav)|*.wav|All files (*.*)|*.*"));
	if (dialog.ShowModal() != wxID_OK)
		return;

	wxString fileName = dialog.GetPath();

	m_wavRadioAck->SetValue(fileName);
}

void CAnalogueRepeaterAckSet::onExternalAckChoice(wxCommandEvent&)
{
	int n = m_externalAck->GetSelection();

	switch (n) {
		case 0:
			m_cwExternalAck->Enable();
			m_wavExternalAck->Disable();
			m_buttonExternalAck->Disable();
			break;
		case 1:
			m_cwExternalAck->Disable();
			m_wavExternalAck->Enable();
			m_buttonExternalAck->Enable();
			break;
		default:
			break;
	}
}

void CAnalogueRepeaterAckSet::onExternalAckButton(wxCommandEvent&)
{
	wxString file = m_wavExternalAck->GetValue();

	wxString path;
	wxString name;

	if (!file.IsEmpty()) {
		wxFileName fileName(file);
		path = fileName.GetPath();
		name = fileName.GetFullName();
	}

	wxFileDialog dialog(this, _("Select a WAV File"), path, name, _("WAV file (*.wav)|*.wav|All files (*.*)|*.*"));
	if (dialog.ShowModal() != wxID_OK)
		return;

	wxString fileName = dialog.GetPath();

	m_wavExternalAck->SetValue(fileName);
}

void CAnalogueRepeaterAckSet::onBatteryAckChoice(wxCommandEvent&)
{
	int n = m_batteryAck->GetSelection();

	switch (n) {
		case 0:
			m_cwBatteryAck->Enable();
			m_wavBatteryAck->Disable();
			m_buttonBatteryAck->Disable();
			break;
		case 1:
			m_cwBatteryAck->Disable();
			m_wavBatteryAck->Enable();
			m_buttonBatteryAck->Enable();
			break;
		default:
			break;
	}
}

void CAnalogueRepeaterAckSet::onBatteryAckButton(wxCommandEvent&)
{
	wxString file = m_wavBatteryAck->GetValue();

	wxString path;
	wxString name;

	if (!file.IsEmpty()) {
		wxFileName fileName(file);
		path = fileName.GetPath();
		name = fileName.GetFullName();
	}

	wxFileDialog dialog(this, _("Select a WAV File"), path, name, _("WAV file (*.wav)|*.wav|All files (*.*)|*.*"));
	if (dialog.ShowModal() != wxID_OK)
		return;

	wxString fileName = dialog.GetPath();

	m_wavBatteryAck->SetValue(fileName);
}
