/*
 *   Copyright (C) 2010,2011,2012 by Jonathan Naylor G4KLX
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

#include "XReflectorPreferences.h"
#include "XReflectorDefs.h"

const unsigned int BORDER_SIZE = 5U;

CXReflectorPreferences::CXReflectorPreferences(wxWindow* parent, int id, const wxString& callsign,
						   const wxString& address, bool dplusEnabled, bool logEnabled) :
wxDialog(parent, id, wxString(_("XReflector Preferences"))),
m_reflector(NULL),
m_dplus(NULL),
m_miscellaneous(NULL)
{
	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

	wxNotebook* noteBook = new wxNotebook(this, -1);

	m_reflector = new CXReflectorReflectorSet(noteBook, -1, APPLICATION_NAME, callsign, address);
	noteBook->AddPage(m_reflector, _("Reflector"), true);

	m_dplus = new CXReflectorDPlusSet(noteBook, -1, APPLICATION_NAME, dplusEnabled);
	noteBook->AddPage(m_dplus, wxT("D-Plus"), false);

	m_miscellaneous = new CXReflectorMiscellaneousSet(noteBook, -1, APPLICATION_NAME, logEnabled);
	noteBook->AddPage(m_miscellaneous, wxT("Misc"), false);

	mainSizer->Add(noteBook, 1, wxALL | wxGROW, BORDER_SIZE);

	mainSizer->Add(CreateButtonSizer(wxOK | wxCANCEL), 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	SetAutoLayout(true);
	Layout();

	mainSizer->Fit(this);
	mainSizer->SetSizeHints(this);

	SetSizer(mainSizer);
}

CXReflectorPreferences::~CXReflectorPreferences()
{
}

bool CXReflectorPreferences::Validate()
{
	if (!m_reflector->Validate())
		return false;

	if (!m_dplus->Validate())
		return false;

	return m_miscellaneous->Validate();
}

wxString CXReflectorPreferences::getCallsign() const
{
	return m_reflector->getCallsign();
}

wxString CXReflectorPreferences::getAddress() const
{
	return m_reflector->getAddress();
}

bool CXReflectorPreferences::getDPlusEnabled() const
{
	return m_dplus->getEnabled();
}

bool CXReflectorPreferences::getLogEnabled() const
{
	return m_miscellaneous->getLogEnabled();
}
