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

#include "StarNetServerPreferences.h"
#include "StarNetServerDefs.h"

const unsigned int BORDER_SIZE = 5U;

CStarNetServerPreferences::CStarNetServerPreferences(wxWindow* parent, int id,
						   const wxString& callsign, const wxString& address,
						   const wxString& hostname, const wxString& username,
						   const wxString& password,
						   bool logEnabled,
#if defined(DEXTRA_LINK) || defined(DCS_LINK)
						   const wxString& starNetBand1, const wxString& callsign1, const wxString& logoff1, const wxString& info1, const wxString& permanent1, unsigned int userTimeout1, unsigned int groupTimeout1, STARNET_CALLSIGN_SWITCH callsignSwitch1, bool txMsgSwitch1, const wxString& link1,
						   const wxString& starNetBand2, const wxString& callsign2, const wxString& logoff2, const wxString& info2, const wxString& permanent2, unsigned int userTimeout2, unsigned int groupTimeout2, STARNET_CALLSIGN_SWITCH callsignSwitch2, bool txMsgSwitch2, const wxString& link2,
						   const wxString& starNetBand3, const wxString& callsign3, const wxString& logoff3, const wxString& info3, const wxString& permanent3, unsigned int userTimeout3, unsigned int groupTimeout3, STARNET_CALLSIGN_SWITCH callsignSwitch3, bool txMsgSwitch3, const wxString& link3,
						   const wxString& starNetBand4, const wxString& callsign4, const wxString& logoff4, const wxString& info4, const wxString& permanent4, unsigned int userTimeout4, unsigned int groupTimeout4, STARNET_CALLSIGN_SWITCH callsignSwitch4, bool txMsgSwitch4, const wxString& link4,
						   const wxString& starNetBand5, const wxString& callsign5, const wxString& logoff5, const wxString& info5, const wxString& permanent5, unsigned int userTimeout5, unsigned int groupTimeout5, STARNET_CALLSIGN_SWITCH callsignSwitch5, bool txMsgSwitch5, const wxString& link5,
						   const wxString& starNetBand6, const wxString& callsign6, const wxString& logoff6, const wxString& info6, const wxString& permanent6, unsigned int userTimeout6, unsigned int groupTimeout6, STARNET_CALLSIGN_SWITCH callsignSwitch6, bool txMsgSwitch6, const wxString& link6,
						   const wxString& starNetBand7, const wxString& callsign7, const wxString& logoff7, const wxString& info7, const wxString& permanent7, unsigned int userTimeout7, unsigned int groupTimeout7, STARNET_CALLSIGN_SWITCH callsignSwitch7, bool txMsgSwitch7, const wxString& link7,
						   const wxString& starNetBand8, const wxString& callsign8, const wxString& logoff8, const wxString& info8, const wxString& permanent8, unsigned int userTimeout8, unsigned int groupTimeout8, STARNET_CALLSIGN_SWITCH callsignSwitch8, bool txMsgSwitch8, const wxString& link8,
						   const wxString& starNetBand9, const wxString& callsign9, const wxString& logoff9, const wxString& info9, const wxString& permanent9, unsigned int userTimeout9, unsigned int groupTimeout9, STARNET_CALLSIGN_SWITCH callsignSwitch9, bool txMsgSwitch9, const wxString& link9,
						   const wxString& starNetBand10, const wxString& callsign10, const wxString& logoff10, const wxString& info10, const wxString& permanent10, unsigned int userTimeout10, unsigned int groupTimeout10, STARNET_CALLSIGN_SWITCH callsignSwitch10, bool txMsgSwitch10, const wxString& link10,
						   const wxString& starNetBand11, const wxString& callsign11, const wxString& logoff11, const wxString& info11, const wxString& permanent11, unsigned int userTimeout11, unsigned int groupTimeout11, STARNET_CALLSIGN_SWITCH callsignSwitch11, bool txMsgSwitch11, const wxString& link11,
						   const wxString& starNetBand12, const wxString& callsign12, const wxString& logoff12, const wxString& info12, const wxString& permanent12, unsigned int userTimeout12, unsigned int groupTimeout12, STARNET_CALLSIGN_SWITCH callsignSwitch12, bool txMsgSwitch12, const wxString& link12,
						   const wxString& starNetBand13, const wxString& callsign13, const wxString& logoff13, const wxString& info13, const wxString& permanent13, unsigned int userTimeout13, unsigned int groupTimeout13, STARNET_CALLSIGN_SWITCH callsignSwitch13, bool txMsgSwitch13, const wxString& link13,
						   const wxString& starNetBand14, const wxString& callsign14, const wxString& logoff14, const wxString& info14, const wxString& permanent14, unsigned int userTimeout14, unsigned int groupTimeout14, STARNET_CALLSIGN_SWITCH callsignSwitch14, bool txMsgSwitch14, const wxString& link14,
						   const wxString& starNetBand15, const wxString& callsign15, const wxString& logoff15, const wxString& info15, const wxString& permanent15, unsigned int userTimeout15, unsigned int groupTimeout15, STARNET_CALLSIGN_SWITCH callsignSwitch15, bool txMsgSwitch15, const wxString& link15,
#else
						   const wxString& starNetBand1, const wxString& callsign1, const wxString& logoff1, const wxString& info1, const wxString& permanent1, unsigned int userTimeout1, unsigned int groupTimeout1, STARNET_CALLSIGN_SWITCH callsignSwitch1, bool txMsgSwitch1,
						   const wxString& starNetBand2, const wxString& callsign2, const wxString& logoff2, const wxString& info2, const wxString& permanent2, unsigned int userTimeout2, unsigned int groupTimeout2, STARNET_CALLSIGN_SWITCH callsignSwitch2, bool txMsgSwitch2,
						   const wxString& starNetBand3, const wxString& callsign3, const wxString& logoff3, const wxString& info3, const wxString& permanent3, unsigned int userTimeout3, unsigned int groupTimeout3, STARNET_CALLSIGN_SWITCH callsignSwitch3, bool txMsgSwitch3,
						   const wxString& starNetBand4, const wxString& callsign4, const wxString& logoff4, const wxString& info4, const wxString& permanent4, unsigned int userTimeout4, unsigned int groupTimeout4, STARNET_CALLSIGN_SWITCH callsignSwitch4, bool txMsgSwitch4,
						   const wxString& starNetBand5, const wxString& callsign5, const wxString& logoff5, const wxString& info5, const wxString& permanent5, unsigned int userTimeout5, unsigned int groupTimeout5, STARNET_CALLSIGN_SWITCH callsignSwitch5, bool txMsgSwitch5,
						   const wxString& starNetBand6, const wxString& callsign6, const wxString& logoff6, const wxString& info6, const wxString& permanent6, unsigned int userTimeout6, unsigned int groupTimeout6, STARNET_CALLSIGN_SWITCH callsignSwitch6, bool txMsgSwitch6,
						   const wxString& starNetBand7, const wxString& callsign7, const wxString& logoff7, const wxString& info7, const wxString& permanent7, unsigned int userTimeout7, unsigned int groupTimeout7, STARNET_CALLSIGN_SWITCH callsignSwitch7, bool txMsgSwitch7,
						   const wxString& starNetBand8, const wxString& callsign8, const wxString& logoff8, const wxString& info8, const wxString& permanent8, unsigned int userTimeout8, unsigned int groupTimeout8, STARNET_CALLSIGN_SWITCH callsignSwitch8, bool txMsgSwitch8,
						   const wxString& starNetBand9, const wxString& callsign9, const wxString& logoff9, const wxString& info9, const wxString& permanent9, unsigned int userTimeout9, unsigned int groupTimeout9, STARNET_CALLSIGN_SWITCH callsignSwitch9, bool txMsgSwitch9,
						   const wxString& starNetBand10, const wxString& callsign10, const wxString& logoff10, const wxString& info10, const wxString& permanent10, unsigned int userTimeout10, unsigned int groupTimeout10, STARNET_CALLSIGN_SWITCH callsignSwitch10, bool txMsgSwitch10,
						   const wxString& starNetBand11, const wxString& callsign11, const wxString& logoff11, const wxString& info11, const wxString& permanent11, unsigned int userTimeout11, unsigned int groupTimeout11, STARNET_CALLSIGN_SWITCH callsignSwitch11, bool txMsgSwitch11,
						   const wxString& starNetBand12, const wxString& callsign12, const wxString& logoff12, const wxString& info12, const wxString& permanent12, unsigned int userTimeout12, unsigned int groupTimeout12, STARNET_CALLSIGN_SWITCH callsignSwitch12, bool txMsgSwitch12,
						   const wxString& starNetBand13, const wxString& callsign13, const wxString& logoff13, const wxString& info13, const wxString& permanent13, unsigned int userTimeout13, unsigned int groupTimeout13, STARNET_CALLSIGN_SWITCH callsignSwitch13, bool txMsgSwitch13,
						   const wxString& starNetBand14, const wxString& callsign14, const wxString& logoff14, const wxString& info14, const wxString& permanent14, unsigned int userTimeout14, unsigned int groupTimeout14, STARNET_CALLSIGN_SWITCH callsignSwitch14, bool txMsgSwitch14,
						   const wxString& starNetBand15, const wxString& callsign15, const wxString& logoff15, const wxString& info15, const wxString& permanent15, unsigned int userTimeout15, unsigned int groupTimeout15, STARNET_CALLSIGN_SWITCH callsignSwitch15, bool txMsgSwitch15,
#endif
						   bool remoteEnabled, const wxString& remotePassword, unsigned int remotePort) :
wxDialog(parent, id, wxString(_("StarNet Server Preferences"))),
m_callsign(NULL),
m_ircDDB(NULL),
m_starNet1(NULL),
m_starNet2(NULL),
m_starNet3(NULL),
m_starNet4(NULL),
m_starNet5(NULL),
m_starNet6(NULL),
m_starNet7(NULL),
m_starNet8(NULL),
m_starNet9(NULL),
m_starNet10(NULL),
m_starNet11(NULL),
m_starNet12(NULL),
m_starNet13(NULL),
m_starNet14(NULL),
m_starNet15(NULL),
m_remote(NULL),
m_miscellaneous(NULL)
{
	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

	wxNotebook* noteBook = new wxNotebook(this, -1);

	m_callsign = new CStarNetServerCallsignSet(noteBook, -1, APPLICATION_NAME, callsign, address);
	noteBook->AddPage(m_callsign, _("Callsign"), true);

	m_ircDDB = new CStarNetServerIrcDDBSet(noteBook, -1, APPLICATION_NAME, hostname, username, password);
	noteBook->AddPage(m_ircDDB, wxT("ircDDB"), false);

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
	m_starNet1 = new CStarNetSet(noteBook, -1, APPLICATION_NAME, starNetBand1, callsign1, logoff1, info1, permanent1, userTimeout1, groupTimeout1, callsignSwitch1, txMsgSwitch1, link1);
	noteBook->AddPage(m_starNet1, wxT("StarNet 1"), false);

	m_starNet2 = new CStarNetSet(noteBook, -1, APPLICATION_NAME, starNetBand2, callsign2, logoff2, info2, permanent2, userTimeout2, groupTimeout2, callsignSwitch2, txMsgSwitch2, link2);
	noteBook->AddPage(m_starNet2, wxT("StarNet 2"), false);

	m_starNet3 = new CStarNetSet(noteBook, -1, APPLICATION_NAME, starNetBand3, callsign3, logoff3, info3, permanent3, userTimeout3, groupTimeout3, callsignSwitch3, txMsgSwitch3, link3);
	noteBook->AddPage(m_starNet3, wxT("StarNet 3"), false);

	m_starNet4 = new CStarNetSet(noteBook, -1, APPLICATION_NAME, starNetBand4, callsign4, logoff4, info4, permanent4, userTimeout4, groupTimeout4, callsignSwitch4, txMsgSwitch4, link4);
	noteBook->AddPage(m_starNet4, wxT("StarNet 4"), false);

	m_starNet5 = new CStarNetSet(noteBook, -1, APPLICATION_NAME, starNetBand5, callsign5, logoff5, info5, permanent5, userTimeout5, groupTimeout5, callsignSwitch5, txMsgSwitch5, link5);
	noteBook->AddPage(m_starNet5, wxT("StarNet 5"), false);

	m_starNet6 = new CStarNetSet(noteBook, -1, APPLICATION_NAME, starNetBand6, callsign6, logoff6, info6, permanent6, userTimeout6, groupTimeout6, callsignSwitch6, txMsgSwitch6, link6);
	noteBook->AddPage(m_starNet6, wxT("StarNet 6"), false);

	m_starNet7 = new CStarNetSet(noteBook, -1, APPLICATION_NAME, starNetBand7, callsign7, logoff7, info7, permanent7, userTimeout7, groupTimeout7, callsignSwitch7, txMsgSwitch7, link7);
	noteBook->AddPage(m_starNet7, wxT("StarNet 7"), false);

	m_starNet8 = new CStarNetSet(noteBook, -1, APPLICATION_NAME, starNetBand8, callsign8, logoff8, info8, permanent8, userTimeout8, groupTimeout8, callsignSwitch8, txMsgSwitch8, link8);
	noteBook->AddPage(m_starNet8, wxT("StarNet 8"), false);

	m_starNet9 = new CStarNetSet(noteBook, -1, APPLICATION_NAME, starNetBand9, callsign9, logoff9, info9, permanent9, userTimeout9, groupTimeout9, callsignSwitch9, txMsgSwitch9, link9);
	noteBook->AddPage(m_starNet9, wxT("StarNet 9"), false);

	m_starNet10 = new CStarNetSet(noteBook, -1, APPLICATION_NAME, starNetBand10, callsign10, logoff10, info10, permanent10, userTimeout10, groupTimeout10, callsignSwitch10, txMsgSwitch10, link10);
	noteBook->AddPage(m_starNet10, wxT("StarNet 10"), false);

	m_starNet11 = new CStarNetSet(noteBook, -1, APPLICATION_NAME, starNetBand11, callsign11, logoff11, info11, permanent11, userTimeout11, groupTimeout11, callsignSwitch11, txMsgSwitch11, link11);
	noteBook->AddPage(m_starNet11, wxT("StarNet 11"), false);

	m_starNet12 = new CStarNetSet(noteBook, -1, APPLICATION_NAME, starNetBand12, callsign12, logoff12, info12, permanent12, userTimeout12, groupTimeout12, callsignSwitch12, txMsgSwitch12, link12);
	noteBook->AddPage(m_starNet12, wxT("StarNet 12"), false);

	m_starNet13 = new CStarNetSet(noteBook, -1, APPLICATION_NAME, starNetBand13, callsign13, logoff13, info13, permanent13, userTimeout13, groupTimeout13, callsignSwitch13, txMsgSwitch13, link13);
	noteBook->AddPage(m_starNet13, wxT("StarNet 13"), false);

	m_starNet14 = new CStarNetSet(noteBook, -1, APPLICATION_NAME, starNetBand14, callsign14, logoff14, info14, permanent14, userTimeout14, groupTimeout14, callsignSwitch14, txMsgSwitch14, link14);
	noteBook->AddPage(m_starNet14, wxT("StarNet 14"), false);

	m_starNet15 = new CStarNetSet(noteBook, -1, APPLICATION_NAME, starNetBand15, callsign15, logoff15, info15, permanent15, userTimeout15, groupTimeout15, callsignSwitch15, txMsgSwitch15, link15);
	noteBook->AddPage(m_starNet15, wxT("StarNet 15"), false);
#else
	m_starNet1 = new CStarNetSet(noteBook, -1, APPLICATION_NAME, starNetBand1, callsign1, logoff1, info1, permanent1, userTimeout1, groupTimeout1, callsignSwitch1, txMsgSwitch1);
	noteBook->AddPage(m_starNet1, wxT("StarNet 1"), false);

	m_starNet2 = new CStarNetSet(noteBook, -1, APPLICATION_NAME, starNetBand2, callsign2, logoff2, info2, permanent2, userTimeout2, groupTimeout2, callsignSwitch2, txMsgSwitch2);
	noteBook->AddPage(m_starNet2, wxT("StarNet 2"), false);

	m_starNet3 = new CStarNetSet(noteBook, -1, APPLICATION_NAME, starNetBand3, callsign3, logoff3, info3, permanent3, userTimeout3, groupTimeout3, callsignSwitch3, txMsgSwitch3);
	noteBook->AddPage(m_starNet3, wxT("StarNet 3"), false);

	m_starNet4 = new CStarNetSet(noteBook, -1, APPLICATION_NAME, starNetBand4, callsign4, logoff4, info4, permanent4, userTimeout4, groupTimeout4, callsignSwitch4, txMsgSwitch4);
	noteBook->AddPage(m_starNet4, wxT("StarNet 4"), false);

	m_starNet5 = new CStarNetSet(noteBook, -1, APPLICATION_NAME, starNetBand5, callsign5, logoff5, info5, permanent5, userTimeout5, groupTimeout5, callsignSwitch5, txMsgSwitch5);
	noteBook->AddPage(m_starNet5, wxT("StarNet 5"), false);

	m_starNet6 = new CStarNetSet(noteBook, -1, APPLICATION_NAME, starNetBand6, callsign6, logoff6, info6, permanent6, userTimeout6, groupTimeout6, callsignSwitch6, txMsgSwitch6);
	noteBook->AddPage(m_starNet6, wxT("StarNet 6"), false);

	m_starNet7 = new CStarNetSet(noteBook, -1, APPLICATION_NAME, starNetBand7, callsign7, logoff7, info7, permanent7, userTimeout7, groupTimeout7, callsignSwitch7, txMsgSwitch7);
	noteBook->AddPage(m_starNet7, wxT("StarNet 7"), false);

	m_starNet8 = new CStarNetSet(noteBook, -1, APPLICATION_NAME, starNetBand8, callsign8, logoff8, info8, permanent8, userTimeout8, groupTimeout8, callsignSwitch8, txMsgSwitch8);
	noteBook->AddPage(m_starNet8, wxT("StarNet 8"), false);

	m_starNet9 = new CStarNetSet(noteBook, -1, APPLICATION_NAME, starNetBand9, callsign9, logoff9, info9, permanent9, userTimeout9, groupTimeout9, callsignSwitch9, txMsgSwitch9);
	noteBook->AddPage(m_starNet9, wxT("StarNet 9"), false);

	m_starNet10 = new CStarNetSet(noteBook, -1, APPLICATION_NAME, starNetBand10, callsign10, logoff10, info10, permanent10, userTimeout10, groupTimeout10, callsignSwitch10, txMsgSwitch10);
	noteBook->AddPage(m_starNet10, wxT("StarNet 10"), false);

	m_starNet11 = new CStarNetSet(noteBook, -1, APPLICATION_NAME, starNetBand11, callsign11, logoff11, info11, permanent11, userTimeout11, groupTimeout11, callsignSwitch11, txMsgSwitch11);
	noteBook->AddPage(m_starNet11, wxT("StarNet 11"), false);

	m_starNet12 = new CStarNetSet(noteBook, -1, APPLICATION_NAME, starNetBand12, callsign12, logoff12, info12, permanent12, userTimeout12, groupTimeout12, callsignSwitch12, txMsgSwitch12);
	noteBook->AddPage(m_starNet12, wxT("StarNet 12"), false);

	m_starNet13 = new CStarNetSet(noteBook, -1, APPLICATION_NAME, starNetBand13, callsign13, logoff13, info13, permanent13, userTimeout13, groupTimeout13, callsignSwitch13, txMsgSwitch13);
	noteBook->AddPage(m_starNet13, wxT("StarNet 13"), false);

	m_starNet14 = new CStarNetSet(noteBook, -1, APPLICATION_NAME, starNetBand14, callsign14, logoff14, info14, permanent14, userTimeout14, groupTimeout14, callsignSwitch14, txMsgSwitch14);
	noteBook->AddPage(m_starNet14, wxT("StarNet 14"), false);

	m_starNet15 = new CStarNetSet(noteBook, -1, APPLICATION_NAME, starNetBand15, callsign15, logoff15, info15, permanent15, userTimeout15, groupTimeout15, callsignSwitch15, txMsgSwitch15);
	noteBook->AddPage(m_starNet15, wxT("StarNet 15"), false);
#endif

	m_remote = new CRemoteSet(noteBook, -1, APPLICATION_NAME, remoteEnabled, remotePassword, remotePort);
	noteBook->AddPage(m_remote, wxT("Remote"), false);

	m_miscellaneous = new CStarNetServerMiscellaneousSet(noteBook, -1, APPLICATION_NAME, logEnabled);
	noteBook->AddPage(m_miscellaneous, wxT("Misc"), false);

	mainSizer->Add(noteBook, 1, wxALL | wxGROW, BORDER_SIZE);

	mainSizer->Add(CreateButtonSizer(wxOK | wxCANCEL), 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	SetAutoLayout(true);
	Layout();

	mainSizer->Fit(this);
	mainSizer->SetSizeHints(this);

	SetSizer(mainSizer);
}

CStarNetServerPreferences::~CStarNetServerPreferences()
{
}

bool CStarNetServerPreferences::Validate()
{
	if (!m_callsign->Validate())
		return false;

	if (!m_ircDDB->Validate())
		return false;

	if (!m_starNet1->Validate())
		return false;

	if (!m_starNet2->Validate())
		return false;

	if (!m_starNet3->Validate())
		return false;

	if (!m_starNet4->Validate())
		return false;

	if (!m_starNet5->Validate())
		return false;

	if (!m_starNet6->Validate())
		return false;

	if (!m_starNet7->Validate())
		return false;

	if (!m_starNet8->Validate())
		return false;

	if (!m_starNet9->Validate())
		return false;

	if (!m_starNet10->Validate())
		return false;

	if (!m_starNet11->Validate())
		return false;

	if (!m_starNet12->Validate())
		return false;

	if (!m_starNet13->Validate())
		return false;

	if (!m_starNet14->Validate())
		return false;

	if (!m_starNet15->Validate())
		return false;

	if (!m_remote->Validate())
		return false;

	return m_miscellaneous->Validate();
}

wxString CStarNetServerPreferences::getCallsign() const
{
	return m_callsign->getCallsign();
}

wxString CStarNetServerPreferences::getAddress() const
{
	return m_callsign->getAddress();
}

wxString CStarNetServerPreferences::getHostname() const
{
	return m_ircDDB->getHostname();
}

wxString CStarNetServerPreferences::getUsername() const
{
	return m_ircDDB->getUsername();
}

wxString CStarNetServerPreferences::getPassword() const
{
	return m_ircDDB->getPassword();
}

bool CStarNetServerPreferences::getLogEnabled() const
{
	return m_miscellaneous->getLogEnabled();
}

wxString CStarNetServerPreferences::getStarNetBand1() const
{
	return m_starNet1->getBand();
}

wxString CStarNetServerPreferences::getStarNetCallsign1() const
{
	return m_starNet1->getCallsign();
}

wxString CStarNetServerPreferences::getStarNetLogoff1() const
{
	return m_starNet1->getLogoff();
}

wxString CStarNetServerPreferences::getStarNetInfo1() const
{
	return m_starNet1->getInfo();
}

wxString CStarNetServerPreferences::getStarNetPermanent1() const
{
	return m_starNet1->getPermanent();
}

unsigned int CStarNetServerPreferences::getStarNetUserTimeout1() const
{
	return m_starNet1->getUserTimeout();
}

unsigned int CStarNetServerPreferences::getStarNetGroupTimeout1() const
{
	return m_starNet1->getGroupTimeout();
}

STARNET_CALLSIGN_SWITCH CStarNetServerPreferences::getStarNetCallsignSwitch1() const
{
	return m_starNet1->getCallsignSwitch();
}

bool CStarNetServerPreferences::getStarNetTXMsgSwitch1() const
{
	return m_starNet1->getTXMsgSwitch();
}

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
wxString CStarNetServerPreferences::getStarNetLink1() const
{
	return m_starNet1->getReflector();
}
#endif

wxString CStarNetServerPreferences::getStarNetBand2() const
{
	return m_starNet2->getBand();
}

wxString CStarNetServerPreferences::getStarNetCallsign2() const
{
	return m_starNet2->getCallsign();
}

wxString CStarNetServerPreferences::getStarNetLogoff2() const
{
	return m_starNet2->getLogoff();
}

wxString CStarNetServerPreferences::getStarNetInfo2() const
{
	return m_starNet2->getInfo();
}

wxString CStarNetServerPreferences::getStarNetPermanent2() const
{
	return m_starNet2->getPermanent();
}

unsigned int CStarNetServerPreferences::getStarNetUserTimeout2() const
{
	return m_starNet2->getUserTimeout();
}

unsigned int CStarNetServerPreferences::getStarNetGroupTimeout2() const
{
	return m_starNet2->getGroupTimeout();
}

STARNET_CALLSIGN_SWITCH CStarNetServerPreferences::getStarNetCallsignSwitch2() const
{
	return m_starNet2->getCallsignSwitch();
}

bool CStarNetServerPreferences::getStarNetTXMsgSwitch2() const
{
	return m_starNet2->getTXMsgSwitch();
}

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
wxString CStarNetServerPreferences::getStarNetLink2() const
{
	return m_starNet2->getReflector();
}
#endif

wxString CStarNetServerPreferences::getStarNetBand3() const
{
	return m_starNet3->getBand();
}

wxString CStarNetServerPreferences::getStarNetCallsign3() const
{
	return m_starNet3->getCallsign();
}

wxString CStarNetServerPreferences::getStarNetLogoff3() const
{
	return m_starNet3->getLogoff();
}

wxString CStarNetServerPreferences::getStarNetInfo3() const
{
	return m_starNet3->getInfo();
}

wxString CStarNetServerPreferences::getStarNetPermanent3() const
{
	return m_starNet3->getPermanent();
}

unsigned int CStarNetServerPreferences::getStarNetUserTimeout3() const
{
	return m_starNet3->getUserTimeout();
}

unsigned int CStarNetServerPreferences::getStarNetGroupTimeout3() const
{
	return m_starNet3->getGroupTimeout();
}

STARNET_CALLSIGN_SWITCH CStarNetServerPreferences::getStarNetCallsignSwitch3() const
{
	return m_starNet3->getCallsignSwitch();
}

bool CStarNetServerPreferences::getStarNetTXMsgSwitch3() const
{
	return m_starNet3->getTXMsgSwitch();
}

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
wxString CStarNetServerPreferences::getStarNetLink3() const
{
	return m_starNet3->getReflector();
}
#endif

wxString CStarNetServerPreferences::getStarNetBand4() const
{
	return m_starNet4->getBand();
}

wxString CStarNetServerPreferences::getStarNetCallsign4() const
{
	return m_starNet4->getCallsign();
}

wxString CStarNetServerPreferences::getStarNetLogoff4() const
{
	return m_starNet4->getLogoff();
}

wxString CStarNetServerPreferences::getStarNetInfo4() const
{
	return m_starNet4->getInfo();
}

wxString CStarNetServerPreferences::getStarNetPermanent4() const
{
	return m_starNet4->getPermanent();
}

unsigned int CStarNetServerPreferences::getStarNetUserTimeout4() const
{
	return m_starNet4->getUserTimeout();
}

unsigned int CStarNetServerPreferences::getStarNetGroupTimeout4() const
{
	return m_starNet4->getGroupTimeout();
}

STARNET_CALLSIGN_SWITCH CStarNetServerPreferences::getStarNetCallsignSwitch4() const
{
	return m_starNet4->getCallsignSwitch();
}

bool CStarNetServerPreferences::getStarNetTXMsgSwitch4() const
{
	return m_starNet4->getTXMsgSwitch();
}

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
wxString CStarNetServerPreferences::getStarNetLink4() const
{
	return m_starNet4->getReflector();
}
#endif

wxString CStarNetServerPreferences::getStarNetBand5() const
{
	return m_starNet5->getBand();
}

wxString CStarNetServerPreferences::getStarNetCallsign5() const
{
	return m_starNet5->getCallsign();
}

wxString CStarNetServerPreferences::getStarNetLogoff5() const
{
	return m_starNet5->getLogoff();
}

wxString CStarNetServerPreferences::getStarNetInfo5() const
{
	return m_starNet5->getInfo();
}

wxString CStarNetServerPreferences::getStarNetPermanent5() const
{
	return m_starNet5->getPermanent();
}

unsigned int CStarNetServerPreferences::getStarNetUserTimeout5() const
{
	return m_starNet5->getUserTimeout();
}

unsigned int CStarNetServerPreferences::getStarNetGroupTimeout5() const
{
	return m_starNet5->getGroupTimeout();
}

STARNET_CALLSIGN_SWITCH CStarNetServerPreferences::getStarNetCallsignSwitch5() const
{
	return m_starNet5->getCallsignSwitch();
}

bool CStarNetServerPreferences::getStarNetTXMsgSwitch5() const
{
	return m_starNet5->getTXMsgSwitch();
}

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
wxString CStarNetServerPreferences::getStarNetLink5() const
{
	return m_starNet5->getReflector();
}
#endif

wxString CStarNetServerPreferences::getStarNetBand6() const
{
	return m_starNet6->getBand();
}

wxString CStarNetServerPreferences::getStarNetCallsign6() const
{
	return m_starNet6->getCallsign();
}

wxString CStarNetServerPreferences::getStarNetLogoff6() const
{
	return m_starNet6->getLogoff();
}

wxString CStarNetServerPreferences::getStarNetInfo6() const
{
	return m_starNet6->getInfo();
}

wxString CStarNetServerPreferences::getStarNetPermanent6() const
{
	return m_starNet6->getPermanent();
}

unsigned int CStarNetServerPreferences::getStarNetUserTimeout6() const
{
	return m_starNet6->getUserTimeout();
}

unsigned int CStarNetServerPreferences::getStarNetGroupTimeout6() const
{
	return m_starNet6->getGroupTimeout();
}

STARNET_CALLSIGN_SWITCH CStarNetServerPreferences::getStarNetCallsignSwitch6() const
{
	return m_starNet6->getCallsignSwitch();
}

bool CStarNetServerPreferences::getStarNetTXMsgSwitch6() const
{
	return m_starNet6->getTXMsgSwitch();
}

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
wxString CStarNetServerPreferences::getStarNetLink6() const
{
	return m_starNet6->getReflector();
}
#endif

wxString CStarNetServerPreferences::getStarNetBand7() const
{
	return m_starNet7->getBand();
}

wxString CStarNetServerPreferences::getStarNetCallsign7() const
{
	return m_starNet7->getCallsign();
}

wxString CStarNetServerPreferences::getStarNetLogoff7() const
{
	return m_starNet7->getLogoff();
}

wxString CStarNetServerPreferences::getStarNetInfo7() const
{
	return m_starNet7->getInfo();
}

wxString CStarNetServerPreferences::getStarNetPermanent7() const
{
	return m_starNet7->getPermanent();
}

unsigned int CStarNetServerPreferences::getStarNetUserTimeout7() const
{
	return m_starNet7->getUserTimeout();
}

unsigned int CStarNetServerPreferences::getStarNetGroupTimeout7() const
{
	return m_starNet7->getGroupTimeout();
}

STARNET_CALLSIGN_SWITCH CStarNetServerPreferences::getStarNetCallsignSwitch7() const
{
	return m_starNet7->getCallsignSwitch();
}

bool CStarNetServerPreferences::getStarNetTXMsgSwitch7() const
{
	return m_starNet7->getTXMsgSwitch();
}

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
wxString CStarNetServerPreferences::getStarNetLink7() const
{
	return m_starNet7->getReflector();
}
#endif

wxString CStarNetServerPreferences::getStarNetBand8() const
{
	return m_starNet8->getBand();
}

wxString CStarNetServerPreferences::getStarNetCallsign8() const
{
	return m_starNet8->getCallsign();
}

wxString CStarNetServerPreferences::getStarNetLogoff8() const
{
	return m_starNet8->getLogoff();
}

wxString CStarNetServerPreferences::getStarNetInfo8() const
{
	return m_starNet8->getInfo();
}

wxString CStarNetServerPreferences::getStarNetPermanent8() const
{
	return m_starNet8->getPermanent();
}

unsigned int CStarNetServerPreferences::getStarNetUserTimeout8() const
{
	return m_starNet8->getUserTimeout();
}

unsigned int CStarNetServerPreferences::getStarNetGroupTimeout8() const
{
	return m_starNet8->getGroupTimeout();
}

STARNET_CALLSIGN_SWITCH CStarNetServerPreferences::getStarNetCallsignSwitch8() const
{
	return m_starNet8->getCallsignSwitch();
}

bool CStarNetServerPreferences::getStarNetTXMsgSwitch8() const
{
	return m_starNet8->getTXMsgSwitch();
}

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
wxString CStarNetServerPreferences::getStarNetLink8() const
{
	return m_starNet8->getReflector();
}
#endif

wxString CStarNetServerPreferences::getStarNetBand9() const
{
	return m_starNet9->getBand();
}

wxString CStarNetServerPreferences::getStarNetCallsign9() const
{
	return m_starNet9->getCallsign();
}

wxString CStarNetServerPreferences::getStarNetLogoff9() const
{
	return m_starNet9->getLogoff();
}

wxString CStarNetServerPreferences::getStarNetInfo9() const
{
	return m_starNet9->getInfo();
}

wxString CStarNetServerPreferences::getStarNetPermanent9() const
{
	return m_starNet9->getPermanent();
}

unsigned int CStarNetServerPreferences::getStarNetUserTimeout9() const
{
	return m_starNet9->getUserTimeout();
}

unsigned int CStarNetServerPreferences::getStarNetGroupTimeout9() const
{
	return m_starNet9->getGroupTimeout();
}

STARNET_CALLSIGN_SWITCH CStarNetServerPreferences::getStarNetCallsignSwitch9() const
{
	return m_starNet9->getCallsignSwitch();
}

bool CStarNetServerPreferences::getStarNetTXMsgSwitch9() const
{
	return m_starNet9->getTXMsgSwitch();
}

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
wxString CStarNetServerPreferences::getStarNetLink9() const
{
	return m_starNet9->getReflector();
}
#endif

wxString CStarNetServerPreferences::getStarNetBand10() const
{
	return m_starNet10->getBand();
}

wxString CStarNetServerPreferences::getStarNetCallsign10() const
{
	return m_starNet10->getCallsign();
}

wxString CStarNetServerPreferences::getStarNetLogoff10() const
{
	return m_starNet10->getLogoff();
}

wxString CStarNetServerPreferences::getStarNetInfo10() const
{
	return m_starNet10->getInfo();
}

wxString CStarNetServerPreferences::getStarNetPermanent10() const
{
	return m_starNet10->getPermanent();
}

unsigned int CStarNetServerPreferences::getStarNetUserTimeout10() const
{
	return m_starNet10->getUserTimeout();
}

unsigned int CStarNetServerPreferences::getStarNetGroupTimeout10() const
{
	return m_starNet10->getGroupTimeout();
}

STARNET_CALLSIGN_SWITCH CStarNetServerPreferences::getStarNetCallsignSwitch10() const
{
	return m_starNet10->getCallsignSwitch();
}

bool CStarNetServerPreferences::getStarNetTXMsgSwitch10() const
{
	return m_starNet10->getTXMsgSwitch();
}

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
wxString CStarNetServerPreferences::getStarNetLink10() const
{
	return m_starNet10->getReflector();
}
#endif

wxString CStarNetServerPreferences::getStarNetBand11() const
{
	return m_starNet11->getBand();
}

wxString CStarNetServerPreferences::getStarNetCallsign11() const
{
	return m_starNet11->getCallsign();
}

wxString CStarNetServerPreferences::getStarNetLogoff11() const
{
	return m_starNet11->getLogoff();
}

wxString CStarNetServerPreferences::getStarNetInfo11() const
{
	return m_starNet11->getInfo();
}

wxString CStarNetServerPreferences::getStarNetPermanent11() const
{
	return m_starNet11->getPermanent();
}

unsigned int CStarNetServerPreferences::getStarNetUserTimeout11() const
{
	return m_starNet11->getUserTimeout();
}

unsigned int CStarNetServerPreferences::getStarNetGroupTimeout11() const
{
	return m_starNet11->getGroupTimeout();
}

STARNET_CALLSIGN_SWITCH CStarNetServerPreferences::getStarNetCallsignSwitch11() const
{
	return m_starNet11->getCallsignSwitch();
}

bool CStarNetServerPreferences::getStarNetTXMsgSwitch11() const
{
	return m_starNet11->getTXMsgSwitch();
}

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
wxString CStarNetServerPreferences::getStarNetLink11() const
{
	return m_starNet11->getReflector();
}
#endif

wxString CStarNetServerPreferences::getStarNetBand12() const
{
	return m_starNet12->getBand();
}

wxString CStarNetServerPreferences::getStarNetCallsign12() const
{
	return m_starNet12->getCallsign();
}

wxString CStarNetServerPreferences::getStarNetLogoff12() const
{
	return m_starNet12->getLogoff();
}

wxString CStarNetServerPreferences::getStarNetInfo12() const
{
	return m_starNet12->getInfo();
}

wxString CStarNetServerPreferences::getStarNetPermanent12() const
{
	return m_starNet12->getPermanent();
}

unsigned int CStarNetServerPreferences::getStarNetUserTimeout12() const
{
	return m_starNet12->getUserTimeout();
}

unsigned int CStarNetServerPreferences::getStarNetGroupTimeout12() const
{
	return m_starNet12->getGroupTimeout();
}

STARNET_CALLSIGN_SWITCH CStarNetServerPreferences::getStarNetCallsignSwitch12() const
{
	return m_starNet12->getCallsignSwitch();
}

bool CStarNetServerPreferences::getStarNetTXMsgSwitch12() const
{
	return m_starNet12->getTXMsgSwitch();
}

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
wxString CStarNetServerPreferences::getStarNetLink12() const
{
	return m_starNet12->getReflector();
}
#endif

wxString CStarNetServerPreferences::getStarNetBand13() const
{
	return m_starNet13->getBand();
}

wxString CStarNetServerPreferences::getStarNetCallsign13() const
{
	return m_starNet13->getCallsign();
}

wxString CStarNetServerPreferences::getStarNetLogoff13() const
{
	return m_starNet13->getLogoff();
}

wxString CStarNetServerPreferences::getStarNetInfo13() const
{
	return m_starNet13->getInfo();
}

wxString CStarNetServerPreferences::getStarNetPermanent13() const
{
	return m_starNet13->getPermanent();
}

unsigned int CStarNetServerPreferences::getStarNetUserTimeout13() const
{
	return m_starNet13->getUserTimeout();
}

unsigned int CStarNetServerPreferences::getStarNetGroupTimeout13() const
{
	return m_starNet13->getGroupTimeout();
}

STARNET_CALLSIGN_SWITCH CStarNetServerPreferences::getStarNetCallsignSwitch13() const
{
	return m_starNet13->getCallsignSwitch();
}

bool CStarNetServerPreferences::getStarNetTXMsgSwitch13() const
{
	return m_starNet13->getTXMsgSwitch();
}

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
wxString CStarNetServerPreferences::getStarNetLink13() const
{
	return m_starNet13->getReflector();
}
#endif

wxString CStarNetServerPreferences::getStarNetBand14() const
{
	return m_starNet14->getBand();
}

wxString CStarNetServerPreferences::getStarNetCallsign14() const
{
	return m_starNet14->getCallsign();
}

wxString CStarNetServerPreferences::getStarNetLogoff14() const
{
	return m_starNet14->getLogoff();
}

wxString CStarNetServerPreferences::getStarNetInfo14() const
{
	return m_starNet14->getInfo();
}

wxString CStarNetServerPreferences::getStarNetPermanent14() const
{
	return m_starNet14->getPermanent();
}

unsigned int CStarNetServerPreferences::getStarNetUserTimeout14() const
{
	return m_starNet14->getUserTimeout();
}

unsigned int CStarNetServerPreferences::getStarNetGroupTimeout14() const
{
	return m_starNet14->getGroupTimeout();
}

STARNET_CALLSIGN_SWITCH CStarNetServerPreferences::getStarNetCallsignSwitch14() const
{
	return m_starNet14->getCallsignSwitch();
}

bool CStarNetServerPreferences::getStarNetTXMsgSwitch14() const
{
	return m_starNet14->getTXMsgSwitch();
}

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
wxString CStarNetServerPreferences::getStarNetLink14() const
{
	return m_starNet14->getReflector();
}
#endif

wxString CStarNetServerPreferences::getStarNetBand15() const
{
	return m_starNet15->getBand();
}

wxString CStarNetServerPreferences::getStarNetCallsign15() const
{
	return m_starNet15->getCallsign();
}

wxString CStarNetServerPreferences::getStarNetLogoff15() const
{
	return m_starNet15->getLogoff();
}

wxString CStarNetServerPreferences::getStarNetInfo15() const
{
	return m_starNet15->getInfo();
}

wxString CStarNetServerPreferences::getStarNetPermanent15() const
{
	return m_starNet15->getPermanent();
}

unsigned int CStarNetServerPreferences::getStarNetUserTimeout15() const
{
	return m_starNet15->getUserTimeout();
}

unsigned int CStarNetServerPreferences::getStarNetGroupTimeout15() const
{
	return m_starNet15->getGroupTimeout();
}

STARNET_CALLSIGN_SWITCH CStarNetServerPreferences::getStarNetCallsignSwitch15() const
{
	return m_starNet15->getCallsignSwitch();
}

bool CStarNetServerPreferences::getStarNetTXMsgSwitch15() const
{
	return m_starNet15->getTXMsgSwitch();
}

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
wxString CStarNetServerPreferences::getStarNetLink15() const
{
	return m_starNet15->getReflector();
}
#endif

bool CStarNetServerPreferences::getRemoteEnabled() const
{
	return m_remote->getEnabled();
}

wxString CStarNetServerPreferences::getRemotePassword() const
{
	return m_remote->getPassword();
}

unsigned int CStarNetServerPreferences::getRemotePort() const
{
	return m_remote->getPort();
}
