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

#include "DVToolReaderUserSoundcardSet.h"

#include "SoundCardReaderWriter.h"

const unsigned int BORDER_SIZE   = 5U;
const unsigned int CONTROL_WIDTH = 200U;

CDVToolReaderUserSoundcardSet::CDVToolReaderUserSoundcardSet(wxWindow* parent, int id, const wxString& title, const wxString& device) :
wxPanel(parent, id),
m_title(title),
m_device(NULL)
{
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	wxArrayString devices = CSoundCardReaderWriter::getWriteDevices();

	m_device = new wxChoice(this, -1, wxDefaultPosition, wxSize(CONTROL_WIDTH, -1), devices);
	sizer->Add(m_device, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	if (devices.GetCount() == 0U) {
		m_device->Append(_("<none>"));
		m_device->SetSelection(0);
	} else {
		bool res = m_device->SetStringSelection(device);
		if (!res)
			m_device->SetSelection(0);
	}

	SetAutoLayout(true);

	sizer->Fit(this);
	sizer->SetSizeHints(this);

	SetSizer(sizer);
}


CDVToolReaderUserSoundcardSet::~CDVToolReaderUserSoundcardSet()
{
}

bool CDVToolReaderUserSoundcardSet::Validate()
{
	bool res = !getDevice().IsEmpty();
	if (!res) {
		wxMessageDialog dialog(this, _("No sound card selected"), m_title + _(" Error"), wxICON_ERROR);
		dialog.ShowModal();
	}

	return res;
}

wxString CDVToolReaderUserSoundcardSet::getDevice() const
{
	wxString device = m_device->GetStringSelection();

	if (device.IsSameAs(_("<none>")))
		return wxEmptyString;
	else
		return device;
}
