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

#include "AnalogueRepeaterCallsignSet.h"
#include "AnalogueRepeaterApp.h"

#include <wx/filename.h>

enum {
	Slider_Level1 = 7001,
	Slider_Level2,

	Choice_OpenId = 8000,
	Choice_CloseId,
	Choice_Beacon1,
	Choice_Beacon2,

	Button_OpenId,
	Button_CloseId,
	Button_Beacon1,
	Button_Beacon2
};

BEGIN_EVENT_TABLE(CAnalogueRepeaterCallsignSet, wxPanel)
	EVT_COMMAND_SCROLL_THUMBTRACK(Slider_Level1, CAnalogueRepeaterCallsignSet::onLevel1)
	EVT_COMMAND_SCROLL_THUMBTRACK(Slider_Level2, CAnalogueRepeaterCallsignSet::onLevel2)

	EVT_CHOICE(Choice_OpenId,  CAnalogueRepeaterCallsignSet::onOpenIdChoice)
	EVT_CHOICE(Choice_CloseId, CAnalogueRepeaterCallsignSet::onCloseIdChoice)
	EVT_CHOICE(Choice_Beacon1, CAnalogueRepeaterCallsignSet::onBeacon1Choice)
	EVT_CHOICE(Choice_Beacon2, CAnalogueRepeaterCallsignSet::onBeacon2Choice)

	EVT_BUTTON(Button_OpenId,  CAnalogueRepeaterCallsignSet::onOpenIdButton)
	EVT_BUTTON(Button_CloseId, CAnalogueRepeaterCallsignSet::onCloseIdButton)
	EVT_BUTTON(Button_Beacon1, CAnalogueRepeaterCallsignSet::onBeacon1Button)
	EVT_BUTTON(Button_Beacon2, CAnalogueRepeaterCallsignSet::onBeacon2Button)
END_EVENT_TABLE()

const unsigned int ID_WIDTH1 = 300U;
const unsigned int ID_WIDTH2 = 110U;

const unsigned int BORDER_SIZE = 5U;

CAnalogueRepeaterCallsignSet::CAnalogueRepeaterCallsignSet(wxWindow* parent, int id, const wxString& title, AUDIO_SOURCE openIdSource, const wxString& openId, AUDIO_SOURCE closeIdSource, const wxString& closeId, AUDIO_SOURCE beacon1Source, const wxString& beacon1, AUDIO_SOURCE beacon2Source, const wxString& beacon2, unsigned int speed, unsigned int freq, wxFloat32 level1, wxFloat32 level2) :
wxPanel(parent, id),
m_title(title),
m_openId(NULL),
m_cwOpenId(NULL),
m_wavOpenId(NULL),
m_buttonOpenId(NULL),
m_closeId(NULL),
m_cwCloseId(NULL),
m_wavCloseId(NULL),
m_buttonCloseId(NULL),
m_beacon1(NULL),
m_cwBeacon1(NULL),
m_wavBeacon1(NULL),
m_buttonBeacon1(NULL),
m_beacon2(NULL),
m_cwBeacon2(NULL),
m_wavBeacon2(NULL),
m_buttonBeacon2(NULL),
m_speed(NULL),
m_freq(NULL),
m_level1(NULL),
m_level2(NULL)
{
	wxFlexGridSizer* sizer = new wxFlexGridSizer(3);

	m_openId = new wxChoice(this, Choice_OpenId, wxDefaultPosition, wxSize(ID_WIDTH2, -1));
	m_openId->Append(_("CW Open ID"));
	m_openId->Append(_("WAV Open ID"));
	sizer->Add(m_openId, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_cwOpenId = new CCWTextCtrl(this, -1, wxEmptyString, wxDefaultPosition, wxSize(ID_WIDTH1, -1));
	sizer->Add(m_cwOpenId, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* dummy1Label = new wxStaticText(this, -1, wxEmptyString);
	sizer->Add(dummy1Label, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	wxStaticText* dummy2Label = new wxStaticText(this, -1, wxEmptyString);
	sizer->Add(dummy2Label, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_wavOpenId = new wxTextCtrl(this, -1, wxEmptyString, wxDefaultPosition, wxSize(ID_WIDTH1, -1));
	sizer->Add(m_wavOpenId, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	m_buttonOpenId = new wxButton(this, Button_OpenId, _("Choose..."));
	sizer->Add(m_buttonOpenId, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	m_closeId = new wxChoice(this, Choice_CloseId, wxDefaultPosition, wxSize(ID_WIDTH2, -1));
	m_closeId->Append(_("CW Close ID"));
	m_closeId->Append(_("WAV Close ID"));
	sizer->Add(m_closeId, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_cwCloseId = new CCWTextCtrl(this, -1, wxEmptyString, wxDefaultPosition, wxSize(ID_WIDTH1, -1));
	sizer->Add(m_cwCloseId, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* dummy3Label = new wxStaticText(this, -1, wxEmptyString);
	sizer->Add(dummy3Label, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	wxStaticText* dummy4Label = new wxStaticText(this, -1, wxEmptyString);
	sizer->Add(dummy4Label, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_wavCloseId = new wxTextCtrl(this, -1, wxEmptyString, wxDefaultPosition, wxSize(ID_WIDTH1, -1));
	sizer->Add(m_wavCloseId, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	m_buttonCloseId = new wxButton(this, Button_CloseId, _("Choose..."));
	sizer->Add(m_buttonCloseId, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	m_beacon1 = new wxChoice(this, Choice_Beacon1, wxDefaultPosition, wxSize(ID_WIDTH2, -1));
	m_beacon1->Append(_("CW Beacon 1"));
	m_beacon1->Append(_("WAV Beacon 1"));
	sizer->Add(m_beacon1, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_cwBeacon1 = new CCWTextCtrl(this, -1, wxEmptyString, wxDefaultPosition, wxSize(ID_WIDTH1, -1));
	sizer->Add(m_cwBeacon1, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* dummy5Label = new wxStaticText(this, -1, wxEmptyString);
	sizer->Add(dummy5Label, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	wxStaticText* dummy6Label = new wxStaticText(this, -1, wxEmptyString);
	sizer->Add(dummy6Label, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_wavBeacon1 = new wxTextCtrl(this, -1, wxEmptyString, wxDefaultPosition, wxSize(ID_WIDTH1, -1));
	sizer->Add(m_wavBeacon1, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	m_buttonBeacon1 = new wxButton(this, Button_Beacon1, _("Choose..."));
	sizer->Add(m_buttonBeacon1, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	m_beacon2 = new wxChoice(this, Choice_Beacon2, wxDefaultPosition, wxSize(ID_WIDTH2, -1));
	m_beacon2->Append(_("CW Beacon 2"));
	m_beacon2->Append(_("WAV Beacon 2"));
	sizer->Add(m_beacon2, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_cwBeacon2 = new CCWTextCtrl(this, -1, wxEmptyString, wxDefaultPosition, wxSize(ID_WIDTH1, -1));
	sizer->Add(m_cwBeacon2, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* dummy7Label = new wxStaticText(this, -1, wxEmptyString);
	sizer->Add(dummy7Label, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	wxStaticText* dummy8Label = new wxStaticText(this, -1, wxEmptyString);
	sizer->Add(dummy8Label, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_wavBeacon2 = new wxTextCtrl(this, -1, wxEmptyString, wxDefaultPosition, wxSize(ID_WIDTH1, -1));
	sizer->Add(m_wavBeacon2, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	m_buttonBeacon2 = new wxButton(this, Button_Beacon2, _("Choose..."));
	sizer->Add(m_buttonBeacon2, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* speedLabel = new wxStaticText(this, -1, _("Speed (WPM)"));
	sizer->Add(speedLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_speed = new wxSlider(this, -1, speed, 8, 25, wxDefaultPosition, wxSize(ID_WIDTH1, -1), wxSL_HORIZONTAL | wxSL_LABELS);
	sizer->Add(m_speed, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* dummy9Label = new wxStaticText(this, -1, wxEmptyString);
	sizer->Add(dummy9Label, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	wxStaticText* freqLabel = new wxStaticText(this, -1, _("Frequency (Hz)"));
	sizer->Add(freqLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_freq = new wxSlider(this, -1, freq, 400, 2000, wxDefaultPosition, wxSize(ID_WIDTH1, -1), wxSL_HORIZONTAL | wxSL_LABELS);
	sizer->Add(m_freq, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* dummy10Label = new wxStaticText(this, -1, wxEmptyString);
	sizer->Add(dummy10Label, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	wxStaticText* level1Label = new wxStaticText(this, -1, _("Level 1 (%)"));
	sizer->Add(level1Label, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_level1 = new wxSlider(this, Slider_Level1, int(level1 * 100.0F + 0.5F), 0, 100, wxDefaultPosition, wxSize(ID_WIDTH1, -1), wxSL_HORIZONTAL | wxSL_LABELS);
	sizer->Add(m_level1, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* dummy11Label = new wxStaticText(this, -1, wxEmptyString);
	sizer->Add(dummy11Label, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	wxStaticText* level2Label = new wxStaticText(this, -1, _("Level 2 (%)"));
	sizer->Add(level2Label, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_level2 = new wxSlider(this, Slider_Level2, int(level2 * 100.0F + 0.5F), 0, 100, wxDefaultPosition, wxSize(ID_WIDTH1, -1), wxSL_HORIZONTAL | wxSL_LABELS);
	sizer->Add(m_level2, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	SetAutoLayout(true);

	// sizer->Fit(this);
	// sizer->SetSizeHints(this);

	SetSizer(sizer);

	switch (openIdSource) {
		case AS_CW_KEYER:
			m_cwOpenId->Enable();
			m_wavOpenId->Disable();
			m_buttonOpenId->Disable();
			m_openId->SetSelection(0);
			m_cwOpenId->SetValue(openId);
			break;
		case AS_WAV_FILE:
			m_cwOpenId->Disable();
			m_wavOpenId->Enable();
			m_buttonOpenId->Enable();
			m_openId->SetSelection(1);
			m_wavOpenId->SetValue(openId);
			break;
	}

	switch (closeIdSource) {
		case AS_CW_KEYER:
			m_cwCloseId->Enable();
			m_wavCloseId->Disable();
			m_buttonCloseId->Disable();
			m_closeId->SetSelection(0);
			m_cwCloseId->SetValue(closeId);
			break;
		case AS_WAV_FILE:
			m_cwCloseId->Disable();
			m_wavCloseId->Enable();
			m_buttonCloseId->Enable();
			m_closeId->SetSelection(1);
			m_wavCloseId->SetValue(closeId);
			break;
	}

	switch (beacon1Source) {
		case AS_CW_KEYER:
			m_cwBeacon1->Enable();
			m_wavBeacon1->Disable();
			m_buttonBeacon1->Disable();
			m_beacon1->SetSelection(0);
			m_cwBeacon1->SetValue(beacon1);
			break;
		case AS_WAV_FILE:
			m_cwBeacon1->Disable();
			m_wavBeacon1->Enable();
			m_buttonBeacon1->Enable();
			m_beacon1->SetSelection(1);
			m_wavBeacon1->SetValue(beacon1);
			break;
	}

	switch (beacon2Source) {
		case AS_CW_KEYER:
			m_cwBeacon2->Enable();
			m_wavBeacon2->Disable();
			m_buttonBeacon2->Disable();
			m_beacon2->SetSelection(0);
			m_cwBeacon2->SetValue(beacon2);
			break;
		case AS_WAV_FILE:
			m_cwBeacon2->Disable();
			m_wavBeacon2->Enable();
			m_buttonBeacon2->Enable();
			m_beacon2->SetSelection(1);
			m_wavBeacon2->SetValue(beacon2);
			break;
	}
}


CAnalogueRepeaterCallsignSet::~CAnalogueRepeaterCallsignSet()
{
}

bool CAnalogueRepeaterCallsignSet::Validate()
{
	int n = m_openId->GetSelection();
	if (n == wxNOT_FOUND) {
		wxMessageDialog dialog(this, _("The Open ID Choice must be made"), m_title + _(" Error"), wxICON_ERROR);
		dialog.ShowModal();
		return false;
	}

	n = m_closeId->GetSelection();
	if (n == wxNOT_FOUND) {
		wxMessageDialog dialog(this, _("The Close ID Choice must be made"), m_title + _(" Error"), wxICON_ERROR);
		dialog.ShowModal();
		return false;
	}

	n = m_beacon1->GetSelection();
	if (n == wxNOT_FOUND) {
		wxMessageDialog dialog(this, _("The Beacon 1 Choice must be made"), m_title + _(" Error"), wxICON_ERROR);
		dialog.ShowModal();
		return false;
	}

	n = m_beacon2->GetSelection();
	if (n == wxNOT_FOUND) {
		wxMessageDialog dialog(this, _("The Beacon 2 Choice must be made"), m_title + _(" Error"), wxICON_ERROR);
		dialog.ShowModal();
		return false;
	}

	return true;
}

AUDIO_SOURCE CAnalogueRepeaterCallsignSet::getOpenIdSource() const
{
	int n = m_openId->GetSelection();

	switch (n) {
		case 1:
			return AS_WAV_FILE;
		default:
			return AS_CW_KEYER;
	}
}

wxString CAnalogueRepeaterCallsignSet::getOpenId() const
{
	AUDIO_SOURCE source = getOpenIdSource();

	switch (source) {
		case AS_CW_KEYER:
			return m_cwOpenId->GetValue().MakeUpper();
		case AS_WAV_FILE:
			return m_wavOpenId->GetValue();
		default:
			return wxEmptyString;
	}
}

AUDIO_SOURCE CAnalogueRepeaterCallsignSet::getCloseIdSource() const
{
	int n = m_closeId->GetSelection();

	switch (n) {
		case 1:
			return AS_WAV_FILE;
		default:
			return AS_CW_KEYER;
	}
}

wxString CAnalogueRepeaterCallsignSet::getCloseId() const
{
	AUDIO_SOURCE source = getCloseIdSource();

	switch (source) {
		case AS_CW_KEYER:
			return m_cwCloseId->GetValue().MakeUpper();
		case AS_WAV_FILE:
			return m_wavCloseId->GetValue();
		default:
			return wxEmptyString;
	}
}

AUDIO_SOURCE CAnalogueRepeaterCallsignSet::getBeacon1Source() const
{
	int n = m_beacon1->GetSelection();

	switch (n) {
		case 1:
			return AS_WAV_FILE;
		default:
			return AS_CW_KEYER;
	}
}

wxString CAnalogueRepeaterCallsignSet::getBeacon1() const
{
	AUDIO_SOURCE source = getBeacon1Source();

	switch (source) {
		case AS_CW_KEYER:
			return m_cwBeacon1->GetValue().MakeUpper();
		case AS_WAV_FILE:
			return m_wavBeacon1->GetValue();
		default:
			return wxEmptyString;
	}
}

AUDIO_SOURCE CAnalogueRepeaterCallsignSet::getBeacon2Source() const
{
	int n = m_beacon2->GetSelection();

	switch (n) {
		case 1:
			return AS_WAV_FILE;
		default:
			return AS_CW_KEYER;
	}
}

wxString CAnalogueRepeaterCallsignSet::getBeacon2() const
{
	AUDIO_SOURCE source = getBeacon2Source();

	switch (source) {
		case AS_CW_KEYER:
			return m_cwBeacon2->GetValue().MakeUpper();
		case AS_WAV_FILE:
			return m_wavBeacon2->GetValue();
		default:
			return wxEmptyString;
	}
}

unsigned int CAnalogueRepeaterCallsignSet::getSpeed() const
{
	return m_speed->GetValue();
}

unsigned int CAnalogueRepeaterCallsignSet::getFreq() const
{
	return m_freq->GetValue();
}

wxFloat32 CAnalogueRepeaterCallsignSet::getLevel1() const
{
	return wxFloat32(m_level1->GetValue()) / 100.0F;
}

wxFloat32 CAnalogueRepeaterCallsignSet::getLevel2() const
{
	return wxFloat32(m_level2->GetValue()) / 100.0F;
}

void CAnalogueRepeaterCallsignSet::onLevel1(wxScrollEvent &event)
{
	int n = event.GetPosition();

	wxFloat32 level = wxFloat32(n) / 100.0F;

	::wxGetApp().setId1Level(level);
}

void CAnalogueRepeaterCallsignSet::onLevel2(wxScrollEvent &event)
{
	int n = event.GetPosition();

	wxFloat32 level = wxFloat32(n) / 100.0F;

	::wxGetApp().setId2Level(level);
}

void CAnalogueRepeaterCallsignSet::onOpenIdChoice(wxCommandEvent&)
{
	int n = m_openId->GetSelection();

	switch (n) {
		case 0:
			m_cwOpenId->Enable();
			m_wavOpenId->Disable();
			m_buttonOpenId->Disable();
			break;
		case 1:
			m_cwOpenId->Disable();
			m_wavOpenId->Enable();
			m_buttonOpenId->Enable();
			break;
		default:
			break;
	}
}

void CAnalogueRepeaterCallsignSet::onCloseIdChoice(wxCommandEvent&)
{
	int n = m_closeId->GetSelection();

	switch (n) {
		case 0:
			m_cwCloseId->Enable();
			m_wavCloseId->Disable();
			m_buttonCloseId->Disable();
			break;
		case 1:
			m_cwCloseId->Disable();
			m_wavCloseId->Enable();
			m_buttonCloseId->Enable();
			break;
		default:
			break;
	}
}

void CAnalogueRepeaterCallsignSet::onOpenIdButton(wxCommandEvent&)
{
	wxString file = m_wavOpenId->GetValue();

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

	m_wavOpenId->SetValue(fileName);
}

void CAnalogueRepeaterCallsignSet::onCloseIdButton(wxCommandEvent&)
{
	wxString file = m_wavCloseId->GetValue();

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

	m_wavCloseId->SetValue(fileName);
}

void CAnalogueRepeaterCallsignSet::onBeacon1Choice(wxCommandEvent&)
{
	int n = m_beacon1->GetSelection();

	switch (n) {
		case 0:
			m_cwBeacon1->Enable();
			m_wavBeacon1->Disable();
			m_buttonBeacon1->Disable();
			break;
		case 1:
			m_cwBeacon1->Disable();
			m_wavBeacon1->Enable();
			m_buttonBeacon1->Enable();
			break;
		default:
			break;
	}
}

void CAnalogueRepeaterCallsignSet::onBeacon1Button(wxCommandEvent&)
{
	wxString file = m_wavBeacon1->GetValue();

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

	m_wavBeacon1->SetValue(fileName);
}

void CAnalogueRepeaterCallsignSet::onBeacon2Choice(wxCommandEvent&)
{
	int n = m_beacon2->GetSelection();

	switch (n) {
		case 0:
			m_cwBeacon2->Enable();
			m_wavBeacon2->Disable();
			m_buttonBeacon2->Disable();
			break;
		case 1:
			m_cwBeacon2->Disable();
			m_wavBeacon2->Enable();
			m_buttonBeacon2->Enable();
			break;
		default:
			break;
	}
}

void CAnalogueRepeaterCallsignSet::onBeacon2Button(wxCommandEvent&)
{
	wxString file = m_wavBeacon2->GetValue();

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

	m_wavBeacon2->SetValue(fileName);
}
