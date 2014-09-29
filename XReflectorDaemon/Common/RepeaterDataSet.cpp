/*
 *   Copyright (C) 2010,2011,2012,2013 by Jonathan Naylor G4KLX
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

#include "RepeaterDataSet.h"
#include "DStarDefines.h"
#include "HostFile.h"

#include <wx/filename.h>

const unsigned int CONTROL_WIDTH1 = 130U;
const unsigned int CONTROL_WIDTH2 = 80U;
const unsigned int CONTROL_WIDTH3 = 40U;

const unsigned int ADDRESS_LENGTH = 15U;
const unsigned int PORT_LENGTH    = 5U;

const unsigned int BORDER_SIZE = 5U;


const int CHOICE_BAND = 8745;
const int CHOICE_TYPE = 8746;

BEGIN_EVENT_TABLE(CRepeaterDataSet, wxPanel)
	EVT_CHOICE(CHOICE_BAND, CRepeaterDataSet::onBand)
	EVT_CHOICE(CHOICE_TYPE, CRepeaterDataSet::onType)
END_EVENT_TABLE()


CRepeaterDataSet::CRepeaterDataSet(wxWindow* parent, int id, const wxString& title, const wxString& band, HW_TYPE type, const wxString& address, unsigned int port, unsigned char band1, unsigned char band2, unsigned char band3, bool dplusEnabled, bool dExtraEnabled, bool dcsEnabled, const wxString& reflector, bool atStartup, RECONNECT reconnect) :
wxPanel(parent, id),
m_title(title),
m_band(NULL),
m_type(NULL),
m_address(NULL),
m_port(NULL),
m_band1(NULL),
m_band2(NULL),
m_band3(NULL),
m_reflector(NULL),
m_channel(NULL),
m_startup(NULL),
m_reconnect(NULL)
{
	wxFlexGridSizer* sizer = new wxFlexGridSizer(3);

	wxStaticText* bandLabel = new wxStaticText(this, -1, _("Band"));
	sizer->Add(bandLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_band = new wxChoice(this, CHOICE_BAND, wxDefaultPosition, wxSize(CONTROL_WIDTH2, -1));
	m_band->Append(_("None"));
	m_band->Append(wxT("A"));
	m_band->Append(wxT("B"));
	m_band->Append(wxT("C"));
	m_band->Append(wxT("D"));
	m_band->Append(wxT("E"));
	m_band->Append(wxT("AD"));
	m_band->Append(wxT("BD"));
	m_band->Append(wxT("CD"));
	m_band->Append(wxT("DD"));
	m_band->Append(wxT("ED"));
	sizer->Add(m_band, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);
	bool res = m_band->SetStringSelection(band);
	if (!res)
		m_band->SetSelection(0);

	wxStaticText* dummy1Label = new wxStaticText(this, -1, wxEmptyString);
	sizer->Add(dummy1Label, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	wxStaticText* hardwareTypeLabel = new wxStaticText(this, -1, _("Type"));
	sizer->Add(hardwareTypeLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_type = new wxChoice(this, CHOICE_TYPE, wxDefaultPosition, wxSize(CONTROL_WIDTH1, -1));
	m_type->Append(_("Homebrew"));
	m_type->Append(wxT("Icom"));
	m_type->Append(_("Dummy"));
	sizer->Add(m_type, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);
	m_type->SetSelection(int(type));

	wxStaticText* dummy2Label = new wxStaticText(this, -1, wxEmptyString);
	sizer->Add(dummy2Label, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	wxStaticText* addressLabel = new wxStaticText(this, -1, _("Address"));
	sizer->Add(addressLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_address = new CAddressTextCtrl(this, -1, address, wxDefaultPosition, wxSize(CONTROL_WIDTH1, -1));
	m_address->SetMaxLength(ADDRESS_LENGTH);
	sizer->Add(m_address, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* dummy3Label = new wxStaticText(this, -1, wxEmptyString);
	sizer->Add(dummy3Label, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	wxStaticText* portLabel = new wxStaticText(this, -1, _("Port"));
	sizer->Add(portLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	wxString buffer;
	buffer.Printf(wxT("%u"), port);

	m_port = new CPortTextCtrl(this, -1, buffer, wxDefaultPosition, wxSize(CONTROL_WIDTH2, -1));
	m_port->SetMaxLength(PORT_LENGTH);
	sizer->Add(m_port, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* dummy4Label = new wxStaticText(this, -1, wxEmptyString);
	sizer->Add(dummy4Label, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	wxStaticText* icomBandsLabel = new wxStaticText(this, -1, _("Bands"));
	sizer->Add(icomBandsLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	wxBoxSizer* bandSizer = new wxBoxSizer(wxHORIZONTAL);

	buffer.Printf(wxT("%u"), band1);
	m_band1 = new CPortTextCtrl(this, -1, buffer, wxDefaultPosition, wxSize(CONTROL_WIDTH3, -1));
	m_band1->SetMaxLength(2U);
	bandSizer->Add(m_band1, 0, wxALIGN_LEFT, BORDER_SIZE);

	buffer.Printf(wxT("%u"), band2);
	m_band2 = new CPortTextCtrl(this, -1, buffer, wxDefaultPosition, wxSize(CONTROL_WIDTH3, -1));
	m_band2->SetMaxLength(2U);
	bandSizer->Add(m_band2, 0, wxLEFT | wxALIGN_LEFT, BORDER_SIZE);

	buffer.Printf(wxT("%u"), band3);
	m_band3 = new CPortTextCtrl(this, -1, buffer, wxDefaultPosition, wxSize(CONTROL_WIDTH3, -1));
	m_band3->SetMaxLength(2U);
	bandSizer->Add(m_band3, 0, wxLEFT | wxALIGN_LEFT, BORDER_SIZE);

	sizer->Add(bandSizer, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	wxStaticText* dummy0Label = new wxStaticText(this, -1, wxEmptyString);
	sizer->Add(dummy0Label, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	wxStaticText* reflectorLabel = new wxStaticText(this, -1, _("Reflector"));
	sizer->Add(reflectorLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_reflector = new wxChoice(this, -1, wxDefaultPosition, wxSize(CONTROL_WIDTH1, -1));
	m_reflector->Append(_("None"));

	if (dplusEnabled) {
		wxFileName fileName(wxFileName::GetHomeDir(), DPLUS_HOSTS_FILE_NAME);
		if (!fileName.IsFileReadable()) {
#if defined(__WINDOWS__)
			fileName.Assign(::wxGetCwd(), DPLUS_HOSTS_FILE_NAME);
#else
			fileName.Assign(wxT(DATA_DIR), DPLUS_HOSTS_FILE_NAME);
#endif
		}

		CHostFile file(fileName.GetFullPath(), true);

		for (unsigned int i = 0U; i < file.getCount(); i++)
			m_reflector->Append(file.getName(i));
	}

	if (dExtraEnabled) {
		wxFileName fileName(wxFileName::GetHomeDir(), DEXTRA_HOSTS_FILE_NAME);
		if (!fileName.IsFileReadable()) {
#if defined(__WINDOWS__)
			fileName.Assign(::wxGetCwd(), DEXTRA_HOSTS_FILE_NAME);
#else
			fileName.Assign(wxT(DATA_DIR), DEXTRA_HOSTS_FILE_NAME);
#endif
		}

		CHostFile file(fileName.GetFullPath(), true);

		for (unsigned int i = 0U; i < file.getCount(); i++)
			m_reflector->Append(file.getName(i));
	}

	if (dcsEnabled) {
		wxFileName fileName(wxFileName::GetHomeDir(), DCS_HOSTS_FILE_NAME);
		if (!fileName.IsFileReadable()) {
#if defined(__WINDOWS__)
			fileName.Assign(::wxGetCwd(), DCS_HOSTS_FILE_NAME);
#else
			fileName.Assign(wxT(DATA_DIR), DCS_HOSTS_FILE_NAME);
#endif
		}

		CHostFile file(fileName.GetFullPath(), true);

		for (unsigned int i = 0U; i < file.getCount(); i++)
			m_reflector->Append(file.getName(i));
	}

	sizer->Add(m_reflector, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	if (reflector.IsEmpty()) {
		m_reflector->SetSelection(0);
	} else {
		wxString name = reflector;
		name.SetChar(LONG_CALLSIGN_LENGTH - 1U, wxT(' '));
		bool res = m_reflector->SetStringSelection(name);
		if (!res)
			m_reflector->SetSelection(0);
	}

	m_channel = new wxChoice(this, -1, wxDefaultPosition, wxSize(CONTROL_WIDTH2, -1));
	m_channel->Append(wxT("A"));
	m_channel->Append(wxT("B"));
	m_channel->Append(wxT("C"));
	m_channel->Append(wxT("D"));
	m_channel->Append(wxT("E"));
	m_channel->Append(wxT("F"));
	m_channel->Append(wxT("G"));
	m_channel->Append(wxT("H"));
	m_channel->Append(wxT("I"));
	m_channel->Append(wxT("J"));
	m_channel->Append(wxT("K"));
	m_channel->Append(wxT("L"));
	m_channel->Append(wxT("M"));
	m_channel->Append(wxT("N"));
	m_channel->Append(wxT("O"));
	m_channel->Append(wxT("P"));
	m_channel->Append(wxT("Q"));
	m_channel->Append(wxT("R"));
	m_channel->Append(wxT("S"));
	m_channel->Append(wxT("T"));
	m_channel->Append(wxT("U"));
	m_channel->Append(wxT("V"));
	m_channel->Append(wxT("W"));
	m_channel->Append(wxT("X"));
	m_channel->Append(wxT("Y"));
	m_channel->Append(wxT("Z"));
	sizer->Add(m_channel, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);
	res = m_channel->SetStringSelection(reflector.Right(1U));
	if (!res)
		m_channel->SetSelection(0);

	wxStaticText* startupLabel = new wxStaticText(this, -1, _("Startup"));
	sizer->Add(startupLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_startup = new wxChoice(this, -1, wxDefaultPosition, wxSize(CONTROL_WIDTH1, -1));
	m_startup->Append(_("No"));
	m_startup->Append(_("Yes"));
	sizer->Add(m_startup, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);
	m_startup->SetSelection(atStartup ? 1 : 0);

	wxStaticText* dummy5Label = new wxStaticText(this, -1, wxEmptyString);
	sizer->Add(dummy5Label, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	wxStaticText* reconnectLabel = new wxStaticText(this, -1, _("Reconnect"));
	sizer->Add(reconnectLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_reconnect = new wxChoice(this, -1, wxDefaultPosition, wxSize(CONTROL_WIDTH1, -1));
	m_reconnect->Append(_("Never"));
	m_reconnect->Append(_("Fixed"));
	m_reconnect->Append(_("5 minutes"));
	m_reconnect->Append(_("10 minutes"));
	m_reconnect->Append(_("15 minutes"));
	m_reconnect->Append(_("20 minutes"));
	m_reconnect->Append(_("25 minutes"));
	m_reconnect->Append(_("30 minutes"));
	m_reconnect->Append(_("60 minutes"));
	m_reconnect->Append(_("90 minutes"));
	m_reconnect->Append(_("120 minutes"));
	m_reconnect->Append(_("180 minutes"));
	sizer->Add(m_reconnect, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);
	m_reconnect->SetSelection(int(reconnect));

	if (isDDMode()) {
		m_type->SetSelection(1);
		m_reflector->SetSelection(0);
		m_channel->SetSelection(0);
		m_startup->SetSelection(0);
		m_reconnect->SetSelection(0);

		m_type->Disable();
		m_reflector->Disable();
		m_channel->Disable();
		m_startup->Disable();
		m_reconnect->Disable();
	} else {
		m_type->Enable();
		m_reflector->Enable();
		m_channel->Enable();
		m_startup->Enable();
		m_reconnect->Enable();
	}

	if (type == HW_ICOM) {
		m_band1->Enable();
		m_band2->Enable();
		m_band3->Enable();
	} else {
		m_band1->Disable();
		m_band2->Disable();
		m_band3->Disable();
	}

	SetAutoLayout(true);

	SetSizer(sizer);
}


CRepeaterDataSet::~CRepeaterDataSet()
{
}

bool CRepeaterDataSet::Validate()
{
	int band = m_band->GetCurrentSelection();
	if (band == wxNOT_FOUND)
		return false;

	if (band == 0)
		return true;

	if (m_type->GetCurrentSelection() == wxNOT_FOUND) {
		wxMessageDialog dialog(this, _("The Hardware Type is not set"), m_title + _(" Error"), wxICON_ERROR);
		dialog.ShowModal();
		return false;
	}

	wxString address = getAddress();

	if (address.IsEmpty()) {
		wxMessageDialog dialog(this, _("The Repeater Address is not valid"), m_title + _(" Error"), wxICON_ERROR);
		dialog.ShowModal();
		return false;
	}

	unsigned int port = getPort();

	if (port == 0U || port > 65535U) {
		wxMessageDialog dialog(this, _("The Repeater Port is not valid"), m_title + _(" Error"), wxICON_ERROR);
		dialog.ShowModal();
		return false;
	}

	if (m_reflector->GetCurrentSelection() == wxNOT_FOUND)
		return false;

	if (m_channel->GetCurrentSelection() == wxNOT_FOUND)
		return false;

	if (m_startup->GetCurrentSelection() == wxNOT_FOUND)
		return false;

	if (m_reconnect->GetCurrentSelection() == wxNOT_FOUND)
		return false;

	return true;
}

wxString CRepeaterDataSet::getBand() const
{
	int c = m_band->GetCurrentSelection();

	switch (c) {
		case 0:
			return wxT(" ");
		case 1:
			return wxT("A");
		case 2:
			return wxT("B");
		case 3:
			return wxT("C");
		case 4:
			return wxT("D");
		case 5:
			return wxT("E");
		case 6:
			return wxT("AD");
		case 7:
			return wxT("BD");
		case 8:
			return wxT("CD");
		case 9:
			return wxT("DD");
		case 10:
			return wxT("ED");
		default:
			return wxT(" ");
	}
}

wxString CRepeaterDataSet::getAddress() const
{
	return m_address->GetValue();
}

HW_TYPE CRepeaterDataSet::getType() const
{
	if (isDDMode())
		return HW_ICOM;

	int n = m_type->GetCurrentSelection();
	if (n == wxNOT_FOUND)
		n = 0;

	return HW_TYPE(n);
}

unsigned int CRepeaterDataSet::getPort() const
{
	unsigned long n;
	m_port->GetValue().ToULong(&n);

	return n;
}

unsigned char CRepeaterDataSet::getBand1() const
{
	unsigned long n;
	m_band1->GetValue().ToULong(&n);

	return n;
}

unsigned char CRepeaterDataSet::getBand2() const
{
	unsigned long n;
	m_band2->GetValue().ToULong(&n);

	return n;
}

unsigned char CRepeaterDataSet::getBand3() const
{
	unsigned long n;
	m_band3->GetValue().ToULong(&n);

	return n;
}

wxString CRepeaterDataSet::getReflector() const
{
	if (isDDMode())
		return wxEmptyString;

	int n = m_reflector->GetCurrentSelection();
	int c = m_channel->GetCurrentSelection();

	if (n == 0)
		return wxEmptyString;

	wxString reflector = m_reflector->GetStringSelection();

	reflector.Append(wxT("        "));
	reflector.Truncate(LONG_CALLSIGN_LENGTH - 1U);
	reflector.Append(wxT('A') + c);

	return reflector;
}

bool CRepeaterDataSet::atStartup() const
{
	if (isDDMode())
		return false;

	int n = m_startup->GetCurrentSelection();

	return n == 1;
}

RECONNECT CRepeaterDataSet::getReconnect() const
{
	if (isDDMode())
		return RECONNECT_NEVER;

	int n = m_reconnect->GetCurrentSelection();

	return RECONNECT(n);
}

void CRepeaterDataSet::onBand(wxCommandEvent &event)
{
	if (isDDMode()) {
		m_type->SetSelection(1);
		m_reflector->SetSelection(0);
		m_channel->SetSelection(0);
		m_startup->SetSelection(0);
		m_reconnect->SetSelection(0);

		m_type->Disable();
		m_reflector->Disable();
		m_channel->Disable();
		m_startup->Disable();
		m_reconnect->Disable();
	} else {
		m_type->Enable();
		m_reflector->Enable();
		m_channel->Enable();
		m_startup->Enable();
		m_reconnect->Enable();
	}
}

void CRepeaterDataSet::onType(wxCommandEvent &event)
{
	int n = m_type->GetCurrentSelection();
	if (n != 1) {
		m_band1->Disable();
		m_band2->Disable();
		m_band3->Disable();
	} else {
		m_band1->Enable();
		m_band2->Enable();
		m_band3->Enable();
	}
}

bool CRepeaterDataSet::isDDMode() const
{
	int c = m_band->GetCurrentSelection();

	switch (c) {
		case 5:
		case 6:
		case 7:
		case 8:
			return true;
		default:
			return false;
	}
}
