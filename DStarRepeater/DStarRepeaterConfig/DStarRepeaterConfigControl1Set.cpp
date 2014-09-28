/*
 *   Copyright (C) 2009-2012,2014 by Jonathan Naylor G4KLX
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

#include "DStarRepeaterConfigControl1Set.h"
#include "DStarDefines.h"

const unsigned int BORDER_SIZE    = 5U;
const unsigned int CONTROL_WIDTH1 = 120U;
const unsigned int CONTROL_WIDTH2 = 200U;

CDStarRepeaterConfigControl1Set::CDStarRepeaterConfigControl1Set(wxWindow* parent, int id, const wxString& title, bool enabled, const wxString& rpt1Callsign, const wxString& rpt2Callsign, const wxString& shutdown, const wxString& startup, const wxString& status1, const wxString& status2, const wxString& status3, const wxString& status4, const wxString& status5, const wxString& output1, const wxString& output2, const wxString& output3, const wxString& output4) :
wxPanel(parent, id),
m_title(title),
m_enabled(NULL),
m_rpt1Callsign(NULL),
m_rpt2Callsign(NULL),
m_shutdown(NULL),
m_startup(NULL),
m_status1(NULL),
m_status2(NULL),
m_status3(NULL),
m_status4(NULL),
m_status5(NULL),
m_output1(NULL),
m_output2(NULL),
m_output3(NULL),
m_output4(NULL)
{
	wxFlexGridSizer* sizer = new wxFlexGridSizer(2);

	wxStaticText* controlLabel = new wxStaticText(this, -1, _("Control"));
	sizer->Add(controlLabel, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	m_enabled = new wxChoice(this, -1, wxDefaultPosition, wxSize(CONTROL_WIDTH1, -1));
	m_enabled->Append(_("Disabled"));
	m_enabled->Append(_("Enabled"));
	sizer->Add(m_enabled, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);
	m_enabled->SetSelection(enabled ? 1 : 0);

	wxStaticText* rpt1CallsignLabel = new wxStaticText(this, -1, _("RPT1 Callsign"));
	sizer->Add(rpt1CallsignLabel, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	m_rpt1Callsign = new CCallsignTextCtrl(this, -1, rpt1Callsign, wxDefaultPosition, wxSize(CONTROL_WIDTH1, -1));
	m_rpt1Callsign->SetMaxLength(LONG_CALLSIGN_LENGTH);
	sizer->Add(m_rpt1Callsign, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* rpt2CallsignLabel = new wxStaticText(this, -1, _("RPT2 Callsign"));
	sizer->Add(rpt2CallsignLabel, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	m_rpt2Callsign = new CCallsignTextCtrl(this, -1, rpt2Callsign, wxDefaultPosition, wxSize(CONTROL_WIDTH1, -1));
	m_rpt2Callsign->SetMaxLength(LONG_CALLSIGN_LENGTH);
	sizer->Add(m_rpt2Callsign, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* shutdownLabel = new wxStaticText(this, -1, _("Shutdown"));
	sizer->Add(shutdownLabel, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	m_shutdown = new CCallsignTextCtrl(this, -1, shutdown, wxDefaultPosition, wxSize(CONTROL_WIDTH1, -1));
	m_shutdown->SetMaxLength(LONG_CALLSIGN_LENGTH);
	sizer->Add(m_shutdown, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* startupLabel = new wxStaticText(this, -1, _("Startup"));
	sizer->Add(startupLabel, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	m_startup = new CCallsignTextCtrl(this, -1, startup, wxDefaultPosition, wxSize(CONTROL_WIDTH1, -1));
	m_startup->SetMaxLength(LONG_CALLSIGN_LENGTH);
	sizer->Add(m_startup, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* status1Label = new wxStaticText(this, -1, _("Status 1"));
	sizer->Add(status1Label, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	m_status1 = new CCallsignTextCtrl(this, -1, status1, wxDefaultPosition, wxSize(CONTROL_WIDTH1, -1));
	m_status1->SetMaxLength(LONG_CALLSIGN_LENGTH);
	sizer->Add(m_status1, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* status2Label = new wxStaticText(this, -1, _("Status 2"));
	sizer->Add(status2Label, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	m_status2 = new CCallsignTextCtrl(this, -1, status2, wxDefaultPosition, wxSize(CONTROL_WIDTH1, -1));
	m_status2->SetMaxLength(LONG_CALLSIGN_LENGTH);
	sizer->Add(m_status2, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* status3Label = new wxStaticText(this, -1, _("Status 3"));
	sizer->Add(status3Label, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	m_status3 = new CCallsignTextCtrl(this, -1, status3, wxDefaultPosition, wxSize(CONTROL_WIDTH1, -1));
	m_status3->SetMaxLength(LONG_CALLSIGN_LENGTH);
	sizer->Add(m_status3, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* status4Label = new wxStaticText(this, -1, _("Status 4"));
	sizer->Add(status4Label, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	m_status4 = new CCallsignTextCtrl(this, -1, status4, wxDefaultPosition, wxSize(CONTROL_WIDTH1, -1));
	m_status4->SetMaxLength(LONG_CALLSIGN_LENGTH);
	sizer->Add(m_status4, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* status5Label = new wxStaticText(this, -1, _("Status 5"));
	sizer->Add(status5Label, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	m_status5 = new CCallsignTextCtrl(this, -1, status5, wxDefaultPosition, wxSize(CONTROL_WIDTH1, -1));
	m_status5->SetMaxLength(LONG_CALLSIGN_LENGTH);
	sizer->Add(m_status5, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* output1Label = new wxStaticText(this, -1, _("Output 1"));
	sizer->Add(output1Label, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	m_output1 = new CCallsignTextCtrl(this, -1, output1, wxDefaultPosition, wxSize(CONTROL_WIDTH1, -1));
	m_output1->SetMaxLength(LONG_CALLSIGN_LENGTH);
	sizer->Add(m_output1, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* output2Label = new wxStaticText(this, -1, _("Output 2"));
	sizer->Add(output2Label, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	m_output2 = new CCallsignTextCtrl(this, -1, output2, wxDefaultPosition, wxSize(CONTROL_WIDTH1, -1));
	m_output2->SetMaxLength(LONG_CALLSIGN_LENGTH);
	sizer->Add(m_output2, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* output3Label = new wxStaticText(this, -1, _("Output 3"));
	sizer->Add(output3Label, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	m_output3 = new CCallsignTextCtrl(this, -1, output3, wxDefaultPosition, wxSize(CONTROL_WIDTH1, -1));
	m_output3->SetMaxLength(LONG_CALLSIGN_LENGTH);
	sizer->Add(m_output3, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* output4Label = new wxStaticText(this, -1, _("Output 4"));
	sizer->Add(output4Label, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	m_output4 = new CCallsignTextCtrl(this, -1, output4, wxDefaultPosition, wxSize(CONTROL_WIDTH1, -1));
	m_output4->SetMaxLength(LONG_CALLSIGN_LENGTH);
	sizer->Add(m_output4, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	SetAutoLayout(true);

	sizer->Fit(this);
	sizer->SetSizeHints(this);

	SetSizer(sizer);
}


CDStarRepeaterConfigControl1Set::~CDStarRepeaterConfigControl1Set()
{
}

bool CDStarRepeaterConfigControl1Set::Validate()
{
	return m_enabled->GetCurrentSelection() != wxNOT_FOUND;
}

bool CDStarRepeaterConfigControl1Set::getEnabled() const
{
	int n = m_enabled->GetCurrentSelection();

	return n == 1;
}

wxString CDStarRepeaterConfigControl1Set::getRPT1Callsign() const
{
	return m_rpt1Callsign->GetValue().MakeUpper();
}

wxString CDStarRepeaterConfigControl1Set::getRPT2Callsign() const
{
	return m_rpt2Callsign->GetValue().MakeUpper();
}

wxString CDStarRepeaterConfigControl1Set::getShutdown() const
{
	return m_shutdown->GetValue().MakeUpper();
}

wxString CDStarRepeaterConfigControl1Set::getStartup() const
{
	return m_startup->GetValue().MakeUpper();
}

wxString CDStarRepeaterConfigControl1Set::getStatus1() const
{
	return m_status1->GetValue().MakeUpper();
}

wxString CDStarRepeaterConfigControl1Set::getStatus2() const
{
	return m_status2->GetValue().MakeUpper();
}

wxString CDStarRepeaterConfigControl1Set::getStatus3() const
{
	return m_status3->GetValue().MakeUpper();
}

wxString CDStarRepeaterConfigControl1Set::getStatus4() const
{
	return m_status4->GetValue().MakeUpper();
}

wxString CDStarRepeaterConfigControl1Set::getStatus5() const
{
	return m_status5->GetValue().MakeUpper();
}

wxString CDStarRepeaterConfigControl1Set::getOutput1() const
{
	return m_output1->GetValue().MakeUpper();
}

wxString CDStarRepeaterConfigControl1Set::getOutput2() const
{
	return m_output2->GetValue().MakeUpper();
}

wxString CDStarRepeaterConfigControl1Set::getOutput3() const
{
	return m_output3->GetValue().MakeUpper();
}

wxString CDStarRepeaterConfigControl1Set::getOutput4() const
{
	return m_output4->GetValue().MakeUpper();
}
