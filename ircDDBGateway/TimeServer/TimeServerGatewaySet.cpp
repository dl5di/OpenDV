/*
 *   Copyright (C) 2012 by Jonathan Naylor G4KLX
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

#include "TimeServerGatewaySet.h"
#include "DStarDefines.h"

const unsigned int BORDER_SIZE   = 5U;
const unsigned int CONTROL_WIDTH = 150U;

CTimeServerGatewaySet::CTimeServerGatewaySet(wxWindow* parent, int id, const wxString& title, const wxString& callsign, bool sendA, bool sendB, bool sendC, bool sendD, bool sendE, const wxString& address) :
wxPanel(parent, id),
m_title(title),
m_callsign(NULL),
m_address(NULL),
m_sendA(NULL),
m_sendB(NULL),
m_sendC(NULL),
m_sendD(NULL),
m_sendE(NULL)
{
	wxFlexGridSizer* sizer = new wxFlexGridSizer(2);

	wxStaticText* callsignLabel = new wxStaticText(this, -1, _("Callsign"));
	sizer->Add(callsignLabel, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	m_callsign = new CCallsignTextCtrl(this, -1, callsign, wxDefaultPosition, wxSize(CONTROL_WIDTH, -1));
	m_callsign->SetMaxLength(LONG_CALLSIGN_LENGTH - 1U);
	sizer->Add(m_callsign, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* addressLabel = new wxStaticText(this, -1, _("Address"));
	sizer->Add(addressLabel, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	m_address = new CAddressTextCtrl(this, -1, address, wxDefaultPosition, wxSize(CONTROL_WIDTH, -1));
	sizer->Add(m_address, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* sendALabel = new wxStaticText(this, -1, _("Module A"));
	sizer->Add(sendALabel, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	m_sendA = new wxChoice(this, -1, wxDefaultPosition, wxSize(CONTROL_WIDTH, -1));
	m_sendA->Append(_("No"));
	m_sendA->Append(_("Yes"));
	sizer->Add(m_sendA, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);
	m_sendA->SetSelection(sendA ? 1 : 0);

	wxStaticText* sendBLabel = new wxStaticText(this, -1, _("Module B"));
	sizer->Add(sendBLabel, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	m_sendB = new wxChoice(this, -1, wxDefaultPosition, wxSize(CONTROL_WIDTH, -1));
	m_sendB->Append(_("No"));
	m_sendB->Append(_("Yes"));
	sizer->Add(m_sendB, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);
	m_sendB->SetSelection(sendB ? 1 : 0);

	wxStaticText* sendCLabel = new wxStaticText(this, -1, _("Module C"));
	sizer->Add(sendCLabel, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	m_sendC = new wxChoice(this, -1, wxDefaultPosition, wxSize(CONTROL_WIDTH, -1));
	m_sendC->Append(_("No"));
	m_sendC->Append(_("Yes"));
	sizer->Add(m_sendC, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);
	m_sendC->SetSelection(sendC ? 1 : 0);

	wxStaticText* sendDLabel = new wxStaticText(this, -1, _("Module D"));
	sizer->Add(sendDLabel, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	m_sendD = new wxChoice(this, -1, wxDefaultPosition, wxSize(CONTROL_WIDTH, -1));
	m_sendD->Append(_("No"));
	m_sendD->Append(_("Yes"));
	sizer->Add(m_sendD, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);
	m_sendD->SetSelection(sendD ? 1 : 0);

	wxStaticText* sendELabel = new wxStaticText(this, -1, _("Module E"));
	sizer->Add(sendELabel, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	m_sendE = new wxChoice(this, -1, wxDefaultPosition, wxSize(CONTROL_WIDTH, -1));
	m_sendE->Append(_("No"));
	m_sendE->Append(_("Yes"));
	sizer->Add(m_sendE, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);
	m_sendE->SetSelection(sendE ? 1 : 0);

	SetAutoLayout(true);

	SetSizer(sizer);
}


CTimeServerGatewaySet::~CTimeServerGatewaySet()
{
}

bool CTimeServerGatewaySet::Validate()
{
	bool res = getCallsign().IsEmpty();
	if (res) {
		wxMessageDialog dialog(this, _("The Callsign may not be empty"), _("Time Server Error"), wxICON_ERROR);
		dialog.ShowModal();
		return false;
	}

	res = getAddress().IsEmpty();
	if (res) {
		wxMessageDialog dialog(this, _("The Address may not be empty"), _("Time Server Error"), wxICON_ERROR);
		dialog.ShowModal();
		return false;
	}

	if (m_sendA->GetCurrentSelection() == wxNOT_FOUND)
		return false;

	if (m_sendB->GetCurrentSelection() == wxNOT_FOUND)
		return false;

	if (m_sendC->GetCurrentSelection() == wxNOT_FOUND)
		return false;

	if (m_sendD->GetCurrentSelection() == wxNOT_FOUND)
		return false;

	return m_sendE->GetCurrentSelection() != wxNOT_FOUND;
}

wxString CTimeServerGatewaySet::getCallsign() const
{
	wxString callsign = m_callsign->GetValue();

	callsign.MakeUpper();

	return callsign;
}

wxString CTimeServerGatewaySet::getAddress() const
{
	return m_address->GetValue();
}

bool CTimeServerGatewaySet::getSendA() const
{
	int n = m_sendA->GetCurrentSelection();

	return n == 1;
}

bool CTimeServerGatewaySet::getSendB() const
{
	int n = m_sendB->GetCurrentSelection();

	return n == 1;
}

bool CTimeServerGatewaySet::getSendC() const
{
	int n = m_sendC->GetCurrentSelection();

	return n == 1;
}

bool CTimeServerGatewaySet::getSendD() const
{
	int n = m_sendD->GetCurrentSelection();

	return n == 1;
}

bool CTimeServerGatewaySet::getSendE() const
{
	int n = m_sendE->GetCurrentSelection();

	return n == 1;
}
