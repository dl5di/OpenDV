/*
 *   Copyright (C) 2011,2012,2014 by Jonathan Naylor G4KLX
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

#include "DStarDefines.h"
#include "StarNetSet.h"
#include "HostFile.h"		// DEXTRA_LINK || DCS_LINK
#include "Defs.h"			// DEXTRA_LINK || DCS_LINK

#include <wx/filename.h>	// DEXTRA_LINK || DCS_LINK

const unsigned int BAND_WIDTH     = 60U;
const unsigned int CALLSIGN_WIDTH = 120U;
const unsigned int INFO_WIDTH     = 150U;
const unsigned int TIMEOUT_WIDTH  = 150U;

const unsigned int CONTROL_WIDTH1 = 150U;	// DEXTRA_LINK
const unsigned int CONTROL_WIDTH2 = 60U;	// DEXTRA_LINK

const unsigned int INFO_LENGTH    = 20U;

const unsigned int BORDER_SIZE = 5U;

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
CStarNetSet::CStarNetSet(wxWindow* parent, int id, const wxString& title, const wxString& band, const wxString& callsign, const wxString& logoff, const wxString& info, const wxString& permanent, unsigned int userTimeout, unsigned int groupTimeout, STARNET_CALLSIGN_SWITCH callsignSwitch, bool txMsgSwitch, const wxString& reflector) :
#else
CStarNetSet::CStarNetSet(wxWindow* parent, int id, const wxString& title, const wxString& band, const wxString& callsign, const wxString& logoff, const wxString& info, const wxString& permanent, unsigned int userTimeout, unsigned int groupTimeout, STARNET_CALLSIGN_SWITCH callsignSwitch, bool txMsgSwitch) :
#endif
wxPanel(parent, id),
m_title(title),
m_band(NULL),
m_callsign(NULL),
m_logoff(NULL),
m_info(NULL),
m_permanent(NULL),
m_userTimeout(NULL),
m_groupTimeout(NULL),
m_callsignSwitch(NULL),
m_txMsgSwitch(NULL)
#if defined(DEXTRA_LINK) || defined(DCS_LINK)
,m_reflector(NULL)
#endif
{
	wxFlexGridSizer* sizer = new wxFlexGridSizer(3);

	wxStaticText* bandLabel = new wxStaticText(this, -1, _("Band"));
	sizer->Add(bandLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_band = new wxChoice(this, -1, wxDefaultPosition, wxSize(BAND_WIDTH, -1));
	m_band->Append(wxT("A"));
	m_band->Append(wxT("B"));
	m_band->Append(wxT("C"));
	m_band->Append(wxT("D"));
	sizer->Add(m_band, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);
	bool res = m_band->SetStringSelection(band);
	if (!res)
		m_band->SetSelection(0);

	wxStaticText* dummy1Label = new wxStaticText(this, -1, wxEmptyString);
	sizer->Add(dummy1Label, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	wxStaticText* callsignLabel = new wxStaticText(this, -1, _("Group Call"));
	sizer->Add(callsignLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	wxString call = callsign;
	call.Truncate(LONG_CALLSIGN_LENGTH);

	m_callsign = new CCallsignTextCtrl(this, -1, call, wxDefaultPosition, wxSize(CALLSIGN_WIDTH, -1));
	m_callsign->SetMaxLength(LONG_CALLSIGN_LENGTH);
	sizer->Add(m_callsign, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* dummy2Label = new wxStaticText(this, -1, wxEmptyString);
	sizer->Add(dummy2Label, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	wxStaticText* logoffLabel = new wxStaticText(this, -1, _("Logoff Call"));
	sizer->Add(logoffLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	call = logoff;
	call.Truncate(LONG_CALLSIGN_LENGTH);

	m_logoff = new CCallsignTextCtrl(this, -1, call, wxDefaultPosition, wxSize(CALLSIGN_WIDTH, -1));
	m_logoff->SetMaxLength(LONG_CALLSIGN_LENGTH);
	sizer->Add(m_logoff, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* dummy3Label = new wxStaticText(this, -1, wxEmptyString);
	sizer->Add(dummy3Label, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	wxStaticText* infoLabel = new wxStaticText(this, -1, _("Information"));
	sizer->Add(infoLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_info = new wxTextCtrl(this, -1, info, wxDefaultPosition, wxSize(INFO_WIDTH, -1));
	m_info->SetMaxLength(INFO_LENGTH);
	sizer->Add(m_info, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* dummy4Label = new wxStaticText(this, -1, wxEmptyString);
	sizer->Add(dummy4Label, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	wxStaticText* permanentLabel = new wxStaticText(this, -1, _("Permanent Calls"));
	sizer->Add(permanentLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	call = permanent;
	call.Truncate(LONG_CALLSIGN_LENGTH);

	m_permanent = new CCallsignTextCtrl(this, -1, call, wxDefaultPosition, wxSize(INFO_WIDTH, -1));
	sizer->Add(m_permanent, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* dummy4ALabel = new wxStaticText(this, -1, wxEmptyString);
	sizer->Add(dummy4ALabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	wxStaticText* userTimeoutLabel = new wxStaticText(this, -1, _("User Timeout"));
	sizer->Add(userTimeoutLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_userTimeout = new wxChoice(this, -1, wxDefaultPosition, wxSize(TIMEOUT_WIDTH, -1));
	m_userTimeout->Append(_("Never"));
	m_userTimeout->Append(_("30 mins"));
	m_userTimeout->Append(_("60 mins"));
	m_userTimeout->Append(_("120 mins"));
	m_userTimeout->Append(_("180 mins"));
	m_userTimeout->Append(_("240 mins"));
	m_userTimeout->Append(_("300 mins"));
	sizer->Add(m_userTimeout, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	switch (userTimeout) {
		case 0U:   m_userTimeout->SetSelection(0); break;
		case 30U:  m_userTimeout->SetSelection(1); break;
		case 60U:  m_userTimeout->SetSelection(2); break;
		case 120U: m_userTimeout->SetSelection(3); break;
		case 180U: m_userTimeout->SetSelection(4); break;
		case 240U: m_userTimeout->SetSelection(5); break;
		default:   m_userTimeout->SetSelection(6); break;
	}

	wxStaticText* dummy5Label = new wxStaticText(this, -1, wxEmptyString);
	sizer->Add(dummy5Label, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	wxStaticText* groupTimeoutLabel = new wxStaticText(this, -1, _("Group Timeout"));
	sizer->Add(groupTimeoutLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_groupTimeout = new wxChoice(this, -1, wxDefaultPosition, wxSize(TIMEOUT_WIDTH, -1));
	m_groupTimeout->Append(_("Never"));
	m_groupTimeout->Append(_("30 mins"));
	m_groupTimeout->Append(_("60 mins"));
	m_groupTimeout->Append(_("120 mins"));
	m_groupTimeout->Append(_("180 mins"));
	m_groupTimeout->Append(_("240 mins"));
	m_groupTimeout->Append(_("300 mins"));
	sizer->Add(m_groupTimeout, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	switch (groupTimeout) {
		case 0U:   m_groupTimeout->SetSelection(0); break;
		case 30U:  m_groupTimeout->SetSelection(1); break;
		case 60U:  m_groupTimeout->SetSelection(2); break;
		case 120U: m_groupTimeout->SetSelection(3); break;
		case 180U: m_groupTimeout->SetSelection(4); break;
		case 240U: m_groupTimeout->SetSelection(5); break;
		default:   m_groupTimeout->SetSelection(6); break;
	}

	wxStaticText* dummy6Label = new wxStaticText(this, -1, wxEmptyString);
	sizer->Add(dummy6Label, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	wxStaticText* callsignSwitchLabel = new wxStaticText(this, -1, _("MYCALL Setting"));
	sizer->Add(callsignSwitchLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_callsignSwitch = new wxChoice(this, -1, wxDefaultPosition, wxSize(CALLSIGN_WIDTH, -1));
	m_callsignSwitch->Append(_("Group"));
	m_callsignSwitch->Append(_("User"));
	sizer->Add(m_callsignSwitch, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	switch (callsignSwitch) {
		case SCS_GROUP_CALLSIGN: m_callsignSwitch->SetSelection(0); break;
		case SCS_USER_CALLSIGN:  m_callsignSwitch->SetSelection(1); break;
		default:                 m_callsignSwitch->SetSelection(0); break;
	}

	wxStaticText* dummy7Label = new wxStaticText(this, -1, wxEmptyString);
	sizer->Add(dummy7Label, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	wxStaticText* txMsgSwitchLabel = new wxStaticText(this, -1, _("TX Message"));
	sizer->Add(txMsgSwitchLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_txMsgSwitch = new wxChoice(this, -1, wxDefaultPosition, wxSize(CALLSIGN_WIDTH, -1));
	m_txMsgSwitch->Append(_("Off"));
	m_txMsgSwitch->Append(_("On"));
	sizer->Add(m_txMsgSwitch, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);
	m_txMsgSwitch->SetSelection(txMsgSwitch ? 1 : 0);

	wxStaticText* dummy8Label = new wxStaticText(this, -1, wxEmptyString);
	sizer->Add(dummy8Label, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

#if defined(DEXTRA_LINK)
	wxStaticText* reflectorLabel = new wxStaticText(this, -1, _("Reflector"));
	sizer->Add(reflectorLabel, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	m_reflector = new wxChoice(this, -1, wxDefaultPosition, wxSize(CONTROL_WIDTH1, -1));
	m_reflector->Append(_("None"));

	wxFileName fileName(wxFileName::GetHomeDir(), DEXTRA_HOSTS_FILE_NAME);
	if (!fileName.IsFileReadable()) {
		wxLogMessage(wxT("File %s not readable"), fileName.GetFullPath().c_str());
#if defined(__WINDOWS__)
		fileName.Assign(::wxGetCwd(), DEXTRA_HOSTS_FILE_NAME);
#else
		fileName.Assign(wxT(DATA_DIR), DEXTRA_HOSTS_FILE_NAME);
#endif
		if (!fileName.IsFileReadable())
			wxLogMessage(wxT("File %s not readable"), fileName.GetFullPath().c_str());
	}

	CHostFile file(fileName.GetFullPath(), false);

	for (unsigned int i = 0U; i < file.getCount(); i++)
		m_reflector->Append(file.getName(i));

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
	sizer->Add(m_channel, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);
	res = m_channel->SetStringSelection(reflector.Right(1U));
	if (!res)
		m_channel->SetSelection(0);
#endif

#if defined(DCS_LINK)
	wxStaticText* reflectorLabel = new wxStaticText(this, -1, _("Reflector"));
	sizer->Add(reflectorLabel, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	m_reflector = new wxChoice(this, -1, wxDefaultPosition, wxSize(CONTROL_WIDTH1, -1));
	m_reflector->Append(_("None"));

	wxFileName fileName(wxFileName::GetHomeDir(), DCS_HOSTS_FILE_NAME);
	if (!fileName.IsFileReadable()) {
		wxLogMessage(wxT("File %s not readable"), fileName.GetFullPath().c_str());
#if defined(__WINDOWS__)
		fileName.Assign(::wxGetCwd(), DCS_HOSTS_FILE_NAME);
#else
		fileName.Assign(wxT(DATA_DIR), DCS_HOSTS_FILE_NAME);
#endif
		if (!fileName.IsFileReadable())
			wxLogMessage(wxT("File %s not readable"), fileName.GetFullPath().c_str());
	}

	CHostFile file(fileName.GetFullPath(), false);

	for (unsigned int i = 0U; i < file.getCount(); i++)
		m_reflector->Append(file.getName(i));

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
#endif

	SetAutoLayout(true);

	SetSizer(sizer);
}


CStarNetSet::~CStarNetSet()
{
}

bool CStarNetSet::Validate()
{
	int n = m_band->GetCurrentSelection();
	if (n == wxNOT_FOUND) {
		wxMessageDialog dialog(this, _("The StarNet Band is not set"), m_title + _(" Error"), wxICON_ERROR);
		dialog.ShowModal();
		return false;
	}

	n = m_userTimeout->GetCurrentSelection();
	if (n == wxNOT_FOUND) {
		wxMessageDialog dialog(this, _("The StarNet user timeout is not set"), m_title + _(" Error"), wxICON_ERROR);
		dialog.ShowModal();
		return false;
	}

	n = m_groupTimeout->GetCurrentSelection();
	if (n == wxNOT_FOUND) {
		wxMessageDialog dialog(this, _("The StarNet group timeout is not set"), m_title + _(" Error"), wxICON_ERROR);
		dialog.ShowModal();
		return false;
	}

	n = m_callsignSwitch->GetCurrentSelection();
	if (n == wxNOT_FOUND) {
		wxMessageDialog dialog(this, _("The MYCALL Setting is not set"), m_title + _(" Error"), wxICON_ERROR);
		dialog.ShowModal();
		return false;
	}

	n = m_txMsgSwitch->GetCurrentSelection();
	if (n == wxNOT_FOUND) {
		wxMessageDialog dialog(this, _("The TX Message switch is not set"), m_title + _(" Error"), wxICON_ERROR);
		dialog.ShowModal();
		return false;
	}

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
	n = m_reflector->GetCurrentSelection();
	if (n == wxNOT_FOUND) {
		wxMessageDialog dialog(this, _("The StarNet reflector link is not set"), m_title + _(" Error"), wxICON_ERROR);
		dialog.ShowModal();
		return false;
	}
#endif

	return true;
}

wxString CStarNetSet::getBand() const
{
	return m_band->GetStringSelection();
}

wxString CStarNetSet::getCallsign() const
{
	wxString callsign = m_callsign->GetValue();

	callsign.MakeUpper();
	callsign.Append(wxT("        "));
	callsign.Truncate(LONG_CALLSIGN_LENGTH);

	return callsign;
}

wxString CStarNetSet::getLogoff() const
{
	wxString callsign = m_logoff->GetValue();

	callsign.MakeUpper();
	callsign.Append(wxT("        "));
	callsign.Truncate(LONG_CALLSIGN_LENGTH);

	return callsign;
}

wxString CStarNetSet::getInfo() const
{
	return m_info->GetValue();
}

wxString CStarNetSet::getPermanent() const
{
	wxString callsign = m_permanent->GetValue();

	callsign.MakeUpper();
	callsign.Append(wxT("        "));
	callsign.Truncate(LONG_CALLSIGN_LENGTH);

	return callsign;
}

unsigned int CStarNetSet::getUserTimeout() const
{
	int n = m_userTimeout->GetCurrentSelection();
	switch (n) {
		case 0:
			return 0U;
		case 1:
			return 30U;
		case 2:
			return 60U;
		case 3:
			return 120U;
		case 4:
			return 180U;
		case 5:
			return 240U;
		default:
			return 300U;
	}
}

unsigned int CStarNetSet::getGroupTimeout() const
{
	int n = m_groupTimeout->GetCurrentSelection();
	switch (n) {
		case 0:
			return 0U;
		case 1:
			return 30U;
		case 2:
			return 60U;
		case 3:
			return 120U;
		case 4:
			return 180U;
		case 5:
			return 240U;
		default:
			return 300U;
	}
}

STARNET_CALLSIGN_SWITCH CStarNetSet::getCallsignSwitch() const
{
	int n = m_callsignSwitch->GetCurrentSelection();
	switch (n) {
		case 0:
			return SCS_GROUP_CALLSIGN;
		case 1:
			return SCS_USER_CALLSIGN;
		default:
			return SCS_GROUP_CALLSIGN;
	}
}

bool CStarNetSet::getTXMsgSwitch() const
{
	int n = m_txMsgSwitch->GetCurrentSelection();
	switch (n) {
		case 0:
			return false;
		case 1:
			return true;
		default:
			return false;
	}
}

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
wxString CStarNetSet::getReflector() const
{
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
#endif
