/*
 *   Copyright (C) 2009,2011 by Jonathan Naylor G4KLX
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

#include "ParrotControllerBeaconSet.h"

const unsigned int CONTROL_WIDTH1 = 200U;
const unsigned int CONTROL_WIDTH2 = 300U;

const unsigned int BORDER_SIZE    = 5U;

const int BROWSE_BUTTON = 7000;

BEGIN_EVENT_TABLE(CParrotControllerBeaconSet, wxPanel)
	EVT_BUTTON(BROWSE_BUTTON, CParrotControllerBeaconSet::onBrowse)
END_EVENT_TABLE()

CParrotControllerBeaconSet::CParrotControllerBeaconSet(wxWindow* parent, int id, const wxString& title, unsigned int beaconTime, const wxString& beaconFileName) :
wxPanel(parent, id),
m_title(title),
m_beaconFileName(NULL),
m_beaconTime(NULL)
{
	wxFlexGridSizer* sizer = new wxFlexGridSizer(2);

	wxStaticText* beaconFileNameLabel = new wxStaticText(this, -1, _("File Name"));
	sizer->Add(beaconFileNameLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	wxBoxSizer* fileNameSizer = new wxBoxSizer(wxHORIZONTAL);

	m_beaconFileName = new wxTextCtrl(this, -1, beaconFileName, wxDefaultPosition, wxSize(CONTROL_WIDTH1, -1));
	fileNameSizer->Add(m_beaconFileName, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxButton* browseButton = new wxButton(this, BROWSE_BUTTON, _("Browse..."));
	fileNameSizer->Add(browseButton, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	sizer->Add(fileNameSizer, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	wxStaticText* beaconTimeLabel = new wxStaticText(this, -1, _("Beacon Time (mins)"));
	sizer->Add(beaconTimeLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_beaconTime = new wxSlider(this, -1, beaconTime / 60U, 0, 20, wxDefaultPosition, wxSize(CONTROL_WIDTH2, -1), wxSL_HORIZONTAL | wxSL_LABELS);
	sizer->Add(m_beaconTime, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	SetAutoLayout(true);

	sizer->Fit(this);
	sizer->SetSizeHints(this);

	SetSizer(sizer);
}


CParrotControllerBeaconSet::~CParrotControllerBeaconSet()
{
}

bool CParrotControllerBeaconSet::Validate()
{
	return true;
}

unsigned int CParrotControllerBeaconSet::getBeaconTime() const
{
	return (unsigned int)m_beaconTime->GetValue() * 60U;
}

wxString CParrotControllerBeaconSet::getBeaconFileName() const
{
	return m_beaconFileName->GetValue();
}

void CParrotControllerBeaconSet::onBrowse(wxCommandEvent& event)
{
	wxFileDialog dialog(this, _("Select a DVTOOL File"), wxEmptyString, wxEmptyString, _("DVTOOL file (*.dvtool)|*.dvtool|All files (*.*)|*.*"));
	if (dialog.ShowModal() != wxID_OK)
		return;

	wxString fileName = dialog.GetPath();

	m_beaconFileName->SetValue(fileName);
}
