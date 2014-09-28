/*
 *   Copyright (C) 2009,2010,2011 by Jonathan Naylor G4KLX
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

#include "AnalogueRepeaterDTMF1Set.h"
#include "AnalogueRepeaterApp.h"

enum {
	Slider_Threshold = 7003
};

BEGIN_EVENT_TABLE(CAnalogueRepeaterDTMF1Set, wxPanel)
	EVT_COMMAND_SCROLL_THUMBTRACK(Slider_Threshold, CAnalogueRepeaterDTMF1Set::onThreshold)
END_EVENT_TABLE()

const unsigned int DTMF_WIDTH1 = 125U;
const unsigned int DTMF_WIDTH2 = 125U;
const unsigned int DTMF_WIDTH3 = 300U;
const unsigned int DTMF_WIDTH4 = 200U;

const unsigned int BORDER_SIZE = 5U;

CAnalogueRepeaterDTMF1Set::CAnalogueRepeaterDTMF1Set(wxWindow* parent, int id, const wxString& title,
															   bool radio, bool external, const wxString& shutdown,
															   const wxString& startup, const wxString& timeout,
															   const wxString& timeReset, const wxString& output1,
															   const wxString& output2, const wxString& output3,
															   const wxString& output4, wxFloat32 threshold) :
wxPanel(parent, id),
m_title(title),
m_radio(NULL),
m_external(NULL),
m_shutdown(NULL),
m_startup(NULL),
m_timeout(NULL),
m_timeReset(NULL),
m_output1(NULL),
m_output2(NULL),
m_output3(NULL),
m_output4(NULL),
m_threshold(NULL)
{
	wxFlexGridSizer* sizer = new wxFlexGridSizer(2);

	wxStaticText* radioLabel = new wxStaticText(this, -1, _("Radio"));
	sizer->Add(radioLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_radio = new wxChoice(this, -1, wxDefaultPosition, wxSize(DTMF_WIDTH1, -1));
	m_radio->Append(_("Disabled"));
	m_radio->Append(_("Enabled"));
	sizer->Add(m_radio, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);
	m_radio->SetSelection(radio ? 1 : 0);

	wxStaticText* externalLabel = new wxStaticText(this, -1, _("External"));
	sizer->Add(externalLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_external = new wxChoice(this, -1, wxDefaultPosition, wxSize(DTMF_WIDTH1, -1));
	m_external->Append(_("Disabled"));
	m_external->Append(_("Enabled"));
	sizer->Add(m_external, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);
	m_external->SetSelection(external ? 1 : 0);

	wxStaticText* shutdownLabel = new wxStaticText(this, -1, _("Shutdown"));
	sizer->Add(shutdownLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_shutdown = new CDTMFTextCtrl(this, -1, shutdown, wxDefaultPosition, wxSize(DTMF_WIDTH2, -1));
	m_shutdown->SetMaxLength(10U);
	sizer->Add(m_shutdown, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* startupLabel = new wxStaticText(this, -1, _("Startup"));
	sizer->Add(startupLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_startup = new CDTMFTextCtrl(this, -1, startup, wxDefaultPosition, wxSize(DTMF_WIDTH2, -1));
	m_startup->SetMaxLength(10U);
	sizer->Add(m_startup, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* timeoutLabel = new wxStaticText(this, -1, _("Timeout"));
	sizer->Add(timeoutLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_timeout = new CDTMFTextCtrl(this, -1, timeout, wxDefaultPosition, wxSize(DTMF_WIDTH2, -1));
	m_timeout->SetMaxLength(10U);
	sizer->Add(m_timeout, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* timeResetLabel = new wxStaticText(this, -1, _("Time Reset"));
	sizer->Add(timeResetLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_timeReset = new CDTMFTextCtrl(this, -1, timeReset, wxDefaultPosition, wxSize(DTMF_WIDTH2, -1));
	m_timeReset->SetMaxLength(10U);
	sizer->Add(m_timeReset, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* output1Label = new wxStaticText(this, -1, _("Output 1"));
	sizer->Add(output1Label, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_output1 = new CDTMFTextCtrl(this, -1, output1, wxDefaultPosition, wxSize(DTMF_WIDTH2, -1));
	m_output1->SetMaxLength(10U);
	sizer->Add(m_output1, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* output2Label = new wxStaticText(this, -1, _("Output 2"));
	sizer->Add(output2Label, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_output2 = new CDTMFTextCtrl(this, -1, output2, wxDefaultPosition, wxSize(DTMF_WIDTH2, -1));
	m_output2->SetMaxLength(10U);
	sizer->Add(m_output2, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* output3Label = new wxStaticText(this, -1, _("Output 3"));
	sizer->Add(output3Label, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_output3 = new CDTMFTextCtrl(this, -1, output3, wxDefaultPosition, wxSize(DTMF_WIDTH2, -1));
	m_output3->SetMaxLength(10U);
	sizer->Add(m_output3, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* output4Label = new wxStaticText(this, -1, _("Output 4"));
	sizer->Add(output4Label, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_output4 = new CDTMFTextCtrl(this, -1, output4, wxDefaultPosition, wxSize(DTMF_WIDTH2, -1));
	m_output4->SetMaxLength(10U);
	sizer->Add(m_output4, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* thresholdLabel = new wxStaticText(this, -1, _("Threshold"));
	sizer->Add(thresholdLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_threshold = new wxSlider(this, Slider_Threshold, int(threshold * 100.0F + 0.5F), 0, 100, wxDefaultPosition, wxSize(DTMF_WIDTH3, -1), wxSL_HORIZONTAL | wxSL_LABELS);
	sizer->Add(m_threshold, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	SetAutoLayout(true);

	sizer->Fit(this);
	sizer->SetSizeHints(this);

	SetSizer(sizer);
}


CAnalogueRepeaterDTMF1Set::~CAnalogueRepeaterDTMF1Set()
{
}

bool CAnalogueRepeaterDTMF1Set::Validate()
{
	if (m_radio->GetCurrentSelection() == wxNOT_FOUND)
		return false;

	return m_external->GetCurrentSelection() != wxNOT_FOUND;
}

bool CAnalogueRepeaterDTMF1Set::getRadio() const
{
	return m_radio->GetCurrentSelection() == 1;
}

bool CAnalogueRepeaterDTMF1Set::getExternal() const
{
	return m_external->GetCurrentSelection() == 1;
}

wxString CAnalogueRepeaterDTMF1Set::getShutdown() const
{
	return m_shutdown->GetValue().MakeUpper();
}

wxString CAnalogueRepeaterDTMF1Set::getStartup() const
{
	return m_startup->GetValue().MakeUpper();
}

wxString CAnalogueRepeaterDTMF1Set::getTimeout() const
{
	return m_timeout->GetValue().MakeUpper();
}

wxString CAnalogueRepeaterDTMF1Set::getTimeReset() const
{
	return m_timeReset->GetValue().MakeUpper();
}

wxString CAnalogueRepeaterDTMF1Set::getOutput1() const
{
	return m_output1->GetValue().MakeUpper();
}

wxString CAnalogueRepeaterDTMF1Set::getOutput2() const
{
	return m_output2->GetValue().MakeUpper();
}

wxString CAnalogueRepeaterDTMF1Set::getOutput3() const
{
	return m_output3->GetValue().MakeUpper();
}

wxString CAnalogueRepeaterDTMF1Set::getOutput4() const
{
	return m_output4->GetValue().MakeUpper();
}

wxFloat32 CAnalogueRepeaterDTMF1Set::getThreshold() const
{
	return wxFloat32(m_threshold->GetValue()) / 100.0F;
}

void CAnalogueRepeaterDTMF1Set::onThreshold(wxScrollEvent &event)
{
	int n = event.GetPosition();

	wxFloat32 level = wxFloat32(n) / 100.0F;

	::wxGetApp().setDTMFThreshold(level);
}
