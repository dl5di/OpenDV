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

#include "InversionSet.h"

const unsigned int BORDER_SIZE   = 5U;
const unsigned int CONTROL_WIDTH = 200U;

CInversionSet::CInversionSet(wxWindow* parent, int id, const wxString& title, bool rxInvert, bool txInvert) :
wxPanel(parent, id),
m_title(title),
m_rxInvert(NULL),
m_txInvert(NULL)
{
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	wxStaticBoxSizer* readSizer = new wxStaticBoxSizer(new wxStaticBox(this, -1, _("Input")), wxVERTICAL);

	m_rxInvert = new wxCheckBox(this, -1, _("Invert"));
	m_rxInvert->SetValue(rxInvert);
	readSizer->Add(m_rxInvert, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	sizer->Add(readSizer, 0, wxALL, BORDER_SIZE);

	wxStaticBoxSizer* writeSizer = new wxStaticBoxSizer(new wxStaticBox(this, -1, _("Output")), wxVERTICAL);

	m_txInvert = new wxCheckBox(this, -1, _("Invert"));
	m_txInvert->SetValue(txInvert);
	writeSizer->Add(m_txInvert, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	sizer->Add(writeSizer, 0, wxALL, BORDER_SIZE);

	SetAutoLayout(true);

	sizer->Fit(this);
	sizer->SetSizeHints(this);

	SetSizer(sizer);
}


CInversionSet::~CInversionSet()
{
}

bool CInversionSet::getRXInvert() const
{
	return m_rxInvert->GetValue();
}

bool CInversionSet::getTXInvert() const
{
	return m_txInvert->GetValue();
}
