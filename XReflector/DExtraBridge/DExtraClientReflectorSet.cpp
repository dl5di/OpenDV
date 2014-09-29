/*
 *   Copyright (C) 2011,2012 by Jonathan Naylor G4KLX
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

#include "DExtraClientReflectorSet.h"
#include "DExtraClientHostFile.h"
#include "DExtraClientDefs.h"

#include <wx/filename.h>

const unsigned int REFLECTOR_WIDTH = 100U;
const unsigned int MODULE_WIDTH    = 50U;

const unsigned int BORDER_SIZE = 5U;

CDExtraClientReflectorSet::CDExtraClientReflectorSet(wxWindow* parent, int id, const wxString& title, const wxString& reflector, unsigned int module) :
wxPanel(parent, id),
m_title(title),
m_reflector(NULL),
m_module(NULL)
{
	wxFlexGridSizer* sizer = new wxFlexGridSizer(3);

	wxStaticText* reflectorText = new wxStaticText(this, -1, _("Reflector"));
	sizer->Add(reflectorText, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxFileName fileName(wxFileName::GetHomeDir(), HOSTS_FILE_NAME);

	if (!fileName.IsFileReadable()) {
		wxLogMessage(wxT("File %s not readable"), fileName.GetFullPath().c_str());
#if defined(__WINDOWS__)
		fileName.Assign(::wxGetCwd(), HOSTS_FILE_NAME);
#else
		fileName.Assign(wxT(DATA_DIR), HOSTS_FILE_NAME);
#endif
		if (!fileName.IsFileReadable())
			wxLogMessage(wxT("File %s not readable"), fileName.GetFullPath().c_str());
	}

	CDExtraClientHostFile file(fileName.GetFullPath());
	unsigned int hostCount = file.getCount();

	m_reflector = new wxChoice(this, -1, wxDefaultPosition, wxSize(REFLECTOR_WIDTH, -1));
	m_reflector->Append(_("None"));
	for (unsigned int i = 0U; i < hostCount; i++)
		m_reflector->Append(file.getName(i));
	sizer->Add(m_reflector, 0, wxALL, BORDER_SIZE);

	bool res = m_reflector->SetStringSelection(reflector);
	if (!res)
		m_reflector->SetSelection(0);

	m_module = new wxChoice(this, -1, wxDefaultPosition, wxSize(MODULE_WIDTH, -1));
	m_module->Append(wxT("A"));
	m_module->Append(wxT("B"));
	m_module->Append(wxT("C"));
	m_module->Append(wxT("D"));
	m_module->Append(wxT("E"));
	sizer->Add(m_module, 0, wxALL, BORDER_SIZE);

	m_module->SetSelection(module);

	SetAutoLayout(true);

	sizer->Fit(this);
	sizer->SetSizeHints(this);

	SetSizer(sizer);
}


CDExtraClientReflectorSet::~CDExtraClientReflectorSet()
{
}

bool CDExtraClientReflectorSet::Validate()
{
	if (m_reflector->GetSelection() == wxNOT_FOUND)
		return false;

	return m_module->GetSelection() != wxNOT_FOUND;
}

wxString CDExtraClientReflectorSet::getReflector() const
{
	wxString reflector = m_reflector->GetStringSelection();

	if (reflector.IsSameAs(_("None")))
		return wxEmptyString;

	return reflector;
}

unsigned int CDExtraClientReflectorSet::getModule() const
{
	int n = m_module->GetSelection();

	if (n == wxNOT_FOUND)
		return 0U;

	return (unsigned int)n;
}
