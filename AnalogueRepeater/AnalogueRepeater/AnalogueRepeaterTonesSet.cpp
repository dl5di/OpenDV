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

#include "AnalogueRepeaterTonesSet.h"
#include "AnalogueRepeaterApp.h"

enum {
	Slider_Tb_Threshold = 7004,
	Slider_CTCSS_Threshold,
	Slider_CTCSS_Level
};

BEGIN_EVENT_TABLE(CAnalogueRepeaterTonesSet, wxPanel)
	EVT_COMMAND_SCROLL_THUMBTRACK(Slider_Tb_Threshold,    CAnalogueRepeaterTonesSet::onTbThreshold)
	EVT_COMMAND_SCROLL_THUMBTRACK(Slider_CTCSS_Threshold, CAnalogueRepeaterTonesSet::onCTCSSThreshold)
	EVT_COMMAND_SCROLL_THUMBTRACK(Slider_CTCSS_Level,     CAnalogueRepeaterTonesSet::onCTCSSLevel)
END_EVENT_TABLE()

const unsigned int TONES_WIDTH1 = 300U;
const unsigned int TONES_WIDTH2 = 125U;

const unsigned int BORDER_SIZE = 5U;

CAnalogueRepeaterTonesSet::CAnalogueRepeaterTonesSet(wxWindow* parent, int id, const wxString& title, bool tbEnable, wxFloat32 tbThreshold, wxFloat32 ctcssFreq, bool ctcssInternal, wxFloat32 ctcssThresh, wxFloat32 ctcssLevel, unsigned int ctcssHangTime, ANALOGUE_CTCSS_OUTPUT ctcssOutput) :
wxPanel(parent, id),
m_title(title),
m_tbEnable(NULL),
m_tbThreshold(NULL),
m_ctcssFreq(NULL),
m_ctcssInternal(NULL),
m_ctcssThresh(NULL),
m_ctcssLevel(NULL),
m_ctcssHangTime(NULL),
m_ctcssOutput(NULL)
{
	wxFlexGridSizer* sizer = new wxFlexGridSizer(2);

	wxStaticText* tbEnableLabel = new wxStaticText(this, -1, _("1750Hz Toneburst"));
	sizer->Add(tbEnableLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_tbEnable = new wxChoice(this, -1, wxDefaultPosition, wxSize(TONES_WIDTH2, -1));
	m_tbEnable->Append(_("Disabled"));
	m_tbEnable->Append(_("Enabled"));
	sizer->Add(m_tbEnable, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	if (tbEnable)
		m_tbEnable->SetSelection(1);
	else
		m_tbEnable->SetSelection(0);

	wxStaticText* tbThresholdLabel = new wxStaticText(this, -1, _("1750Hz Threshold"));
	sizer->Add(tbThresholdLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_tbThreshold = new wxSlider(this, Slider_Tb_Threshold, int(tbThreshold * 100.0F + 0.5F), 0, 100, wxDefaultPosition, wxSize(TONES_WIDTH1, -1), wxSL_HORIZONTAL | wxSL_LABELS);
	sizer->Add(m_tbThreshold, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* ctcssFreqLabel = new wxStaticText(this, -1, _("CTCSS Frequency"));
	sizer->Add(ctcssFreqLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_ctcssFreq = new wxChoice(this, -1, wxDefaultPosition, wxSize(TONES_WIDTH2, -1));
	m_ctcssFreq->Append(_("None"));
	m_ctcssFreq->Append(_("67.0 Hz (A)"));
	m_ctcssFreq->Append(_("71.9 Hz (B)"));
	m_ctcssFreq->Append(_("77.0 Hz (C)"));
	m_ctcssFreq->Append(_("79.7 Hz"));
	m_ctcssFreq->Append(_("82.5 Hz (D)"));
	m_ctcssFreq->Append(_("85.4 Hz"));
	m_ctcssFreq->Append(_("88.5 Hz (E)"));
	m_ctcssFreq->Append(_("91.5 Hz"));
	m_ctcssFreq->Append(_("94.8 Hz (F)"));
	m_ctcssFreq->Append(_("100.0 Hz"));
	m_ctcssFreq->Append(_("103.5 Hz (G)"));
	m_ctcssFreq->Append(_("107.2 Hz"));
	m_ctcssFreq->Append(_("110.9 Hz (H)"));
	m_ctcssFreq->Append(_("114.4 Hz"));
	m_ctcssFreq->Append(_("118.8 Hz (J)"));
	m_ctcssFreq->Append(_("123.0 Hz"));
	m_ctcssFreq->Append(_("127.3 Hz"));
	m_ctcssFreq->Append(_("131.8 Hz"));
	m_ctcssFreq->Append(_("136.5 Hz"));
	m_ctcssFreq->Append(_("141.3 Hz"));
	m_ctcssFreq->Append(_("146.2 Hz"));
	m_ctcssFreq->Append(_("150.0 Hz"));
	m_ctcssFreq->Append(_("151.4 Hz"));
	m_ctcssFreq->Append(_("156.7 Hz"));
	m_ctcssFreq->Append(_("162.2 Hz"));
	m_ctcssFreq->Append(_("167.9 Hz"));
	m_ctcssFreq->Append(_("173.8 Hz"));
	m_ctcssFreq->Append(_("179.9 Hz"));
	m_ctcssFreq->Append(_("186.2 Hz"));
	m_ctcssFreq->Append(_("192.8 Hz"));
	m_ctcssFreq->Append(_("203.5 Hz"));
	m_ctcssFreq->Append(_("210.7 Hz"));
	m_ctcssFreq->Append(_("218.1 Hz"));
	m_ctcssFreq->Append(_("225.7 Hz"));
	sizer->Add(m_ctcssFreq, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	if (ctcssFreq == 0.0F)
		m_ctcssFreq->SetSelection(0);
	else if (ctcssFreq == 67.0F)
		m_ctcssFreq->SetSelection(1);
	else if (ctcssFreq == 71.9F)
		m_ctcssFreq->SetSelection(2);
	else if (ctcssFreq == 77.0F)
		m_ctcssFreq->SetSelection(3);
	else if (ctcssFreq == 79.7F)
		m_ctcssFreq->SetSelection(4);
	else if (ctcssFreq == 82.5F)
		m_ctcssFreq->SetSelection(5);
	else if (ctcssFreq == 85.4F)
		m_ctcssFreq->SetSelection(6);
	else if (ctcssFreq == 88.5F)
		m_ctcssFreq->SetSelection(7);
	else if (ctcssFreq == 91.5F)
		m_ctcssFreq->SetSelection(8);
	else if (ctcssFreq == 94.8F)
		m_ctcssFreq->SetSelection(9);
	else if (ctcssFreq == 100.0F)
		m_ctcssFreq->SetSelection(10);
	else if (ctcssFreq == 103.5F)
		m_ctcssFreq->SetSelection(11);
	else if (ctcssFreq == 107.2F)
		m_ctcssFreq->SetSelection(12);
	else if (ctcssFreq == 110.9F)
		m_ctcssFreq->SetSelection(13);
	else if (ctcssFreq == 114.4F)
		m_ctcssFreq->SetSelection(14);
	else if (ctcssFreq == 118.8F)
		m_ctcssFreq->SetSelection(15);
	else if (ctcssFreq == 123.0F)
		m_ctcssFreq->SetSelection(16);
	else if (ctcssFreq == 127.3F)
		m_ctcssFreq->SetSelection(17);
	else if (ctcssFreq == 131.8F)
		m_ctcssFreq->SetSelection(18);
	else if (ctcssFreq == 136.5F)
		m_ctcssFreq->SetSelection(19);
	else if (ctcssFreq == 141.3F)
		m_ctcssFreq->SetSelection(20);
	else if (ctcssFreq == 146.2F)
		m_ctcssFreq->SetSelection(21);
	else if (ctcssFreq == 150.0F)
		m_ctcssFreq->SetSelection(22);
	else if (ctcssFreq == 151.4F)
		m_ctcssFreq->SetSelection(23);
	else if (ctcssFreq == 156.7F)
		m_ctcssFreq->SetSelection(24);
	else if (ctcssFreq == 162.2F)
		m_ctcssFreq->SetSelection(25);
	else if (ctcssFreq == 167.9F)
		m_ctcssFreq->SetSelection(26);
	else if (ctcssFreq == 173.8F)
		m_ctcssFreq->SetSelection(27);
	else if (ctcssFreq == 179.9F)
		m_ctcssFreq->SetSelection(28);
	else if (ctcssFreq == 186.2F)
		m_ctcssFreq->SetSelection(29);
	else if (ctcssFreq == 192.8F)
		m_ctcssFreq->SetSelection(30);
	else if (ctcssFreq == 203.5F)
		m_ctcssFreq->SetSelection(31);
	else if (ctcssFreq == 210.7F)
		m_ctcssFreq->SetSelection(32);
	else if (ctcssFreq == 218.1F)
		m_ctcssFreq->SetSelection(33);
	else if (ctcssFreq == 225.7F)
		m_ctcssFreq->SetSelection(34);
	else
		m_ctcssFreq->SetSelection(0);

	wxStaticText* ctcssInternalLabel = new wxStaticText(this, -1, _("CTCSS Source"));
	sizer->Add(ctcssInternalLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_ctcssInternal = new wxChoice(this, -1, wxDefaultPosition, wxSize(TONES_WIDTH2, -1));
	m_ctcssInternal->Append(_("Internal"));
	m_ctcssInternal->Append(_("External"));
	sizer->Add(m_ctcssInternal, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	if (ctcssInternal)
		m_ctcssInternal->SetSelection(0);
	else
		m_ctcssInternal->SetSelection(1);

	wxStaticText* ctcssThreshLabel = new wxStaticText(this, -1, _("CTCSS Threshold"));
	sizer->Add(ctcssThreshLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_ctcssThresh = new wxSlider(this, Slider_CTCSS_Threshold, int(ctcssThresh * 100.0F + 0.5F), 0, 100, wxDefaultPosition, wxSize(TONES_WIDTH1, -1), wxSL_HORIZONTAL | wxSL_LABELS);
	sizer->Add(m_ctcssThresh, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* ctcssLevelLabel = new wxStaticText(this, -1, _("CTCSS Level (%)"));
	sizer->Add(ctcssLevelLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_ctcssLevel = new wxSlider(this, Slider_CTCSS_Level, int(ctcssLevel * 100.0F + 0.5F), 0, 100, wxDefaultPosition, wxSize(TONES_WIDTH1, -1), wxSL_HORIZONTAL | wxSL_LABELS);
	sizer->Add(m_ctcssLevel, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* ctcssHangLabel = new wxStaticText(this, -1, _("CTCSS Hang"));
	sizer->Add(ctcssHangLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_ctcssHangTime = new wxChoice(this, -1, wxDefaultPosition, wxSize(TONES_WIDTH2, -1));
	m_ctcssHangTime->Append(_("None"));
	m_ctcssHangTime->Append(wxT("20 ms"));
	m_ctcssHangTime->Append(wxT("40 ms"));
	m_ctcssHangTime->Append(wxT("60 ms"));
	m_ctcssHangTime->Append(wxT("80 ms"));
	m_ctcssHangTime->Append(wxT("100 ms"));
	sizer->Add(m_ctcssHangTime, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	m_ctcssHangTime->SetSelection(ctcssHangTime);

	wxStaticText* ctcssOutputLabel = new wxStaticText(this, -1, _("CTCSS Output"));
	sizer->Add(ctcssOutputLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_ctcssOutput = new wxChoice(this, -1, wxDefaultPosition, wxSize(TONES_WIDTH2, -1));
	m_ctcssOutput->Append(_("None"));
	m_ctcssOutput->Append(_("When Open"));
	m_ctcssOutput->Append(_("On Audio"));
	m_ctcssOutput->Append(_("Always"));
	sizer->Add(m_ctcssOutput, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	m_ctcssOutput->SetSelection(int(ctcssOutput));

	SetAutoLayout(true);

	sizer->Fit(this);
	sizer->SetSizeHints(this);

	SetSizer(sizer);
}


CAnalogueRepeaterTonesSet::~CAnalogueRepeaterTonesSet()
{
}

bool CAnalogueRepeaterTonesSet::Validate()
{
	if (m_tbEnable->GetSelection() == wxNOT_FOUND)
		return false;

	if (m_ctcssFreq->GetSelection() == wxNOT_FOUND)
		return false;

	if (m_ctcssInternal->GetSelection() == wxNOT_FOUND)
		return false;

	if (m_ctcssHangTime->GetSelection() == wxNOT_FOUND)
		return false;

	return m_ctcssOutput->GetSelection() != wxNOT_FOUND;
}

bool CAnalogueRepeaterTonesSet::getTbEnable() const
{
	int n = m_tbEnable->GetSelection();

	switch (n) {
		case 0:
			return false;
		case 1:
			return true;
		default:
			return false;
	}
}

wxFloat32 CAnalogueRepeaterTonesSet::getTbThreshold() const
{
	return wxFloat32(m_tbThreshold->GetValue()) / 100.0F;
}

wxFloat32 CAnalogueRepeaterTonesSet::getCTCSSFreq() const
{
	int n = m_ctcssFreq->GetSelection();

	switch (n) {
		case 0:
			return 0.0F;
		case 1:
			return 67.0F;
		case 2:
			return 71.9F;
		case 3:
			return 77.0F;
		case 4:
			return 79.7F;
		case 5:
			return 82.5F;
		case 6:
			return 85.4F;
		case 7:
			return 88.5F;
		case 8:
			return 91.5F;
		case 9:
			return 94.8F;
		case 10:
			return 100.0F;
		case 11:
			return 103.5F;
		case 12:
			return 107.2F;
		case 13:
			return 110.9F;
		case 14:
			return 114.4F;
		case 15:
			return 118.8F;
		case 16:
			return 123.0F;
		case 17:
			return 127.3F;
		case 18:
			return 131.8F;
		case 19:
			return 136.5F;
		case 20:
			return 141.3F;
		case 21:
			return 146.2F;
		case 22:
			return 150.0F;
		case 23:
			return 151.4F;
		case 24:
			return 156.7F;
		case 25:
			return 162.2F;
		case 26:
			return 167.9F;
		case 27:
			return 173.8F;
		case 28:
			return 179.9F;
		case 29:
			return 186.2F;
		case 30:
			return 192.8F;
		case 31:
			return 203.5F;
		case 32:
			return 210.7F;
		case 33:
			return 218.1F;
		case 34:
			return 225.7F;
		default:
			return 0.0F;
	}
}

bool CAnalogueRepeaterTonesSet::getCTCSSInternal() const
{
	int n = m_ctcssInternal->GetSelection();

	switch (n) {
		case 0:
			return true;
		case 1:
			return false;
		default:
			return true;
	}
}

wxFloat32 CAnalogueRepeaterTonesSet::getCTCSSThresh() const
{
	return wxFloat32(m_ctcssThresh->GetValue()) / 100.0F;
}

wxFloat32 CAnalogueRepeaterTonesSet::getCTCSSLevel() const
{
	return wxFloat32(m_ctcssLevel->GetValue()) / 100.0F;
}

unsigned int CAnalogueRepeaterTonesSet::getCTCSSHangTime() const
{
	int n = m_ctcssHangTime->GetSelection();

	if (n == wxNOT_FOUND)
		return 0U;

	return (unsigned int)n;
}

ANALOGUE_CTCSS_OUTPUT CAnalogueRepeaterTonesSet::getCTCSSOutput() const
{
	int n = m_ctcssOutput->GetSelection();
	if (n == wxNOT_FOUND)
		return ACO_NONE;

	return ANALOGUE_CTCSS_OUTPUT(n);
}

void CAnalogueRepeaterTonesSet::onTbThreshold(wxScrollEvent &event)
{
	int n = event.GetPosition();

	wxFloat32 level = wxFloat32(n) / 100.0F;

	::wxGetApp().setTbThreshold(level);
}

void CAnalogueRepeaterTonesSet::onCTCSSThreshold(wxScrollEvent &event)
{
	int n = event.GetPosition();

	wxFloat32 level = wxFloat32(n) / 100.0F;

	::wxGetApp().setCTCSSThreshold(level);
}

void CAnalogueRepeaterTonesSet::onCTCSSLevel(wxScrollEvent &event)
{
	int n = event.GetPosition();

	wxFloat32 level = wxFloat32(n) / 100.0F;

	::wxGetApp().setCTCSSLevel(level);
}
