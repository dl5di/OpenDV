/*
 *   Copyright (C) 2009 by Jonathan Naylor G4KLX
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

#include "ParrotControllerParrotSet.h"

const unsigned int CONTROL_WIDTH = 125U;

const unsigned int BORDER_SIZE   = 5U;

CParrotControllerParrotSet::CParrotControllerParrotSet(wxWindow* parent, int id, const wxString& title, unsigned int turnaroundTime, bool keepFile) :
wxPanel(parent, id),
m_title(title),
m_turnaroundTime(NULL),
m_keepFile(NULL)
{
	wxFlexGridSizer* sizer = new wxFlexGridSizer(2);

	wxStaticText* turnaroundTimeLabel = new wxStaticText(this, -1, _("Turnaround Time"));
	sizer->Add(turnaroundTimeLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_turnaroundTime = new wxChoice(this, -1, wxDefaultPosition, wxSize(CONTROL_WIDTH, -1));
	m_turnaroundTime->Append(_("2 seconds"));
	m_turnaroundTime->Append(_("3 seconds"));
	m_turnaroundTime->Append(_("4 seconds"));
	m_turnaroundTime->Append(_("5 seconds"));
	sizer->Add(m_turnaroundTime, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);
	m_turnaroundTime->SetSelection(turnaroundTime - 2U);

	wxStaticText* keepFileLabel = new wxStaticText(this, -1, _("Keep File"));
	sizer->Add(keepFileLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_keepFile = new wxChoice(this, -1, wxDefaultPosition, wxSize(CONTROL_WIDTH, -1));
	m_keepFile->Append(_("No"));
	m_keepFile->Append(_("Yes"));
	sizer->Add(m_keepFile, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);
	m_keepFile->SetSelection(keepFile ? 1 : 0);

	SetAutoLayout(true);

	sizer->Fit(this);
	sizer->SetSizeHints(this);

	SetSizer(sizer);
}


CParrotControllerParrotSet::~CParrotControllerParrotSet()
{
}

bool CParrotControllerParrotSet::Validate()
{
	if (m_turnaroundTime->GetCurrentSelection() == wxNOT_FOUND)
		return false;

	return m_keepFile->GetCurrentSelection() != wxNOT_FOUND;
}

unsigned int CParrotControllerParrotSet::getTurnaroundTime() const
{
	return (unsigned int)(m_turnaroundTime->GetCurrentSelection() + 2);
}

bool CParrotControllerParrotSet::getKeepFile() const
{
	return m_keepFile->GetCurrentSelection() == 1;
}
