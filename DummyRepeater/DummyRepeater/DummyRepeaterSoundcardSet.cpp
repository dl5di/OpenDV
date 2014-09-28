/*
 *   Copyright (C) 2010 by Jonathan Naylor G4KLX
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

#include "DummyRepeaterSoundcardSet.h"

#include "SoundCardReaderWriter.h"

const unsigned int BORDER_SIZE   = 5U;
const unsigned int CONTROL_WIDTH = 300U;

CDummyRepeaterSoundcardSet::CDummyRepeaterSoundcardSet(wxWindow* parent, int id, const wxString& title, const wxString& readDevice, const wxString& writeDevice) :
wxPanel(parent, id),
m_title(title),
m_readDevice(NULL),
m_writeDevice(NULL)
{
	wxFlexGridSizer* sizer = new wxFlexGridSizer(2);

	wxArrayString readDevices = CSoundCardReaderWriter::getReadDevices();
	wxArrayString writeDevices = CSoundCardReaderWriter::getWriteDevices();

	wxStaticText* readLabel = new wxStaticText(this, -1, _("Input"));
	sizer->Add(readLabel, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	m_readDevice = new wxChoice(this, -1, wxDefaultPosition, wxSize(CONTROL_WIDTH, -1), readDevices);
	sizer->Add(m_readDevice, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* writeLabel = new wxStaticText(this, -1, _("Output"));
	sizer->Add(writeLabel, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	m_writeDevice = new wxChoice(this, -1, wxDefaultPosition, wxSize(CONTROL_WIDTH, -1), writeDevices);
	sizer->Add(m_writeDevice, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	if (readDevices.GetCount() == 0U) {
		m_readDevice->Append(_("<none>"));
		m_readDevice->SetSelection(0);
	} else {
		bool res = m_readDevice->SetStringSelection(readDevice);
		if (!res)
			m_readDevice->SetSelection(0);
	}

	if (writeDevices.GetCount() == 0U) {
		m_writeDevice->Append(_("<none>"));
		m_writeDevice->SetSelection(0);
	} else {
		bool res = m_writeDevice->SetStringSelection(writeDevice);
		if (!res)
			m_writeDevice->SetSelection(0);
	}

	SetAutoLayout(true);

	sizer->Fit(this);
	sizer->SetSizeHints(this);

	SetSizer(sizer);
}

CDummyRepeaterSoundcardSet::~CDummyRepeaterSoundcardSet()
{
}

bool CDummyRepeaterSoundcardSet::Validate()
{
	if (getReadDevice().IsEmpty() || getWriteDevice().IsEmpty()) {
		wxMessageDialog dialog(this, _("The soundcard is not set"), m_title + _(" Error"), wxICON_ERROR);
		dialog.ShowModal();
		return false;
	}

	return true;
}

wxString CDummyRepeaterSoundcardSet::getReadDevice() const
{
	wxString device = m_readDevice->GetStringSelection();

	if (device.IsSameAs(_("<none>")))
		return wxEmptyString;
	else
		return device;
}

wxString CDummyRepeaterSoundcardSet::getWriteDevice() const
{
	wxString device = m_writeDevice->GetStringSelection();

	if (device.IsSameAs(_("<none>")))
		return wxEmptyString;
	else
		return device;
}
