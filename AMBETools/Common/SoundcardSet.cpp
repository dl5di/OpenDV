/*
 *   Copyright (C) 2009,2014 by Jonathan Naylor G4KLX
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

#include "SoundcardSet.h"

#include "SoundCardReaderWriter.h"

const unsigned int BORDER_SIZE   = 5U;
const unsigned int CONTROL_WIDTH = 200U;

CSoundcardSet::CSoundcardSet(wxWindow* parent, int id, const wxString& title, const wxString& device, bool read, bool write) :
wxPanel(parent, id),
m_title(title),
m_readDevice(NULL),
m_writeDevice(NULL)
{
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	if (write) {
		wxArrayString devices = CSoundCardReaderWriter::getWriteDevices();

		m_writeDevice = new wxChoice(this, -1, wxDefaultPosition, wxSize(CONTROL_WIDTH, -1), devices);
		sizer->Add(m_writeDevice, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

		if (devices.GetCount() == 0U) {
			m_writeDevice->Append(_("<none>"));
			m_writeDevice->SetSelection(0);
		} else {
			bool res = m_writeDevice->SetStringSelection(device);
			if (!res)
				m_writeDevice->SetSelection(0);
		}
	}

	if (read) {
		wxArrayString devices = CSoundCardReaderWriter::getReadDevices();

		m_readDevice = new wxChoice(this, -1, wxDefaultPosition, wxSize(CONTROL_WIDTH, -1), devices);
		sizer->Add(m_readDevice, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

		if (devices.GetCount() == 0U) {
			m_readDevice->Append(_("<none>"));
			m_readDevice->SetSelection(0);
		} else {
			bool res = m_readDevice->SetStringSelection(device);
			if (!res)
				m_readDevice->SetSelection(0);
		}
	}

	SetAutoLayout(true);

	sizer->Fit(this);
	sizer->SetSizeHints(this);

	SetSizer(sizer);
}


CSoundcardSet::~CSoundcardSet()
{
}

bool CSoundcardSet::Validate()
{
	if (m_readDevice != NULL) {
		int n = m_readDevice->GetSelection();
		if (n == wxNOT_FOUND) {
			wxMessageDialog dialog(this, _("No sound card selected"), m_title + _(" Error"), wxICON_ERROR);
			dialog.ShowModal();
			return false;
		}
	}

	if (m_writeDevice != NULL) {
		int n = m_writeDevice->GetSelection();
		if (n == wxNOT_FOUND) {
			wxMessageDialog dialog(this, _("No sound card selected"), m_title + _(" Error"), wxICON_ERROR);
			dialog.ShowModal();
			return false;
		}
	}

	return true;
}

wxString CSoundcardSet::getReadDevice() const
{
	wxASSERT(m_readDevice != NULL);

	wxString device = m_readDevice->GetStringSelection();

	if (device.IsSameAs(_("<none>")))
		return wxEmptyString;
	else
		return device;
}

wxString CSoundcardSet::getWriteDevice() const
{
	wxASSERT(m_writeDevice != NULL);

	wxString device = m_writeDevice->GetStringSelection();

	if (device.IsSameAs(_("<none>")))
		return wxEmptyString;
	else
		return device;
}
