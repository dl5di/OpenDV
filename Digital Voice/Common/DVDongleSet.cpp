/*
 *   Copyright (C) 2009,2010,2012 by Jonathan Naylor G4KLX
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

#include "DVDongleSet.h"

#include "SerialDataController.h"

const unsigned int BORDER_SIZE   = 5U;
const unsigned int CONTROL_WIDTH = 150U;

CDVDongleSet::CDVDongleSet(wxWindow* parent, int id, const wxString& title, const wxString& device) :
wxPanel(parent, id),
m_title(title),
m_device(NULL)
{
	wxFlexGridSizer* sizer = new wxFlexGridSizer(2);

	wxArrayString devices = CSerialDataController::getDevices();

	wxStaticText* deviceLabel = new wxStaticText(this, -1, _("Device"));
	sizer->Add(deviceLabel, 0, wxALL, BORDER_SIZE);

	m_device = new wxChoice(this, -1, wxDefaultPosition, wxSize(CONTROL_WIDTH, -1), devices);
	m_device->Insert(_("<none>"), 0U);
	sizer->Add(m_device, 0, wxALL, BORDER_SIZE);

	if (devices.GetCount() == 0U) {
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


CDVDongleSet::~CDVDongleSet()
{
}

bool CDVDongleSet::Validate()
{
	int n = m_device->GetSelection();

	if (n == wxNOT_FOUND) {
		wxMessageDialog dialog(this, _("The DV Dongle device is not set"), m_title + _(" Error"), wxICON_ERROR);
		dialog.ShowModal();
		return false;
	}

	return true;
}

wxString CDVDongleSet::getDevice() const
{
	wxString device = m_device->GetStringSelection();

	if (device.IsSameAs(_("<none>")))
		return wxEmptyString;
	else
		return device;
}
