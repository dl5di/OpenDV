/*
 *   Copyright (C) 2011,2012,2013 by Jonathan Naylor G4KLX
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

#include "RemoteControlPreferences.h"
#include "RemoteControlFrame.h"
#include "RemoteControlDefs.h"
#include "RemoteControlApp.h"
#include "Version.h"
#include "SHA256.h"

const unsigned int BORDER_SIZE = 5U;

#if defined(__WINDOWS__)
const unsigned int MAIN_HEIGHT    = 350U;
const unsigned int MAIN_WIDTH     = 500U;
#else
const unsigned int MAIN_HEIGHT    = 350U;
const unsigned int MAIN_WIDTH     = 520U;
#endif

#include <wx/aboutdlg.h>

enum {
	Menu_Edit_Preferences = 7000
};

BEGIN_EVENT_TABLE(CRemoteControlFrame, wxFrame)
	EVT_MENU(wxID_EXIT, CRemoteControlFrame::onQuit)
	EVT_MENU(Menu_Edit_Preferences, CRemoteControlFrame::onPreferences)
	EVT_MENU(wxID_ABOUT, CRemoteControlFrame::onAbout)

	EVT_TIMER(-1, CRemoteControlFrame::onTimer)

	EVT_CLOSE(CRemoteControlFrame::onClose)
END_EVENT_TABLE()

CRemoteControlFrame::CRemoteControlFrame(const wxString& title, const wxPoint& position) :
wxFrame(NULL, -1, title, position),
m_state(RCFS_NORMAL),
m_timer(this),
m_noteBook(NULL),
m_handler(NULL),
m_password(),
m_repeaters(),
m_starNets(),
m_it1(),
m_it2()
{
	SetMenuBar(createMenuBar());

	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

	wxPanel* panel = new wxPanel(this, -1);

	wxBoxSizer* panelSizer = new wxBoxSizer(wxVERTICAL);

	m_noteBook = new wxNotebook(panel, -1, wxDefaultPosition, wxSize(MAIN_WIDTH, MAIN_HEIGHT));
	panelSizer->Add(m_noteBook, 0, wxALL | wxGROW, BORDER_SIZE);

	panel->SetSizer(panelSizer);

	panelSizer->SetSizeHints(panel);

	mainSizer->Add(panel);

	SetSizer(mainSizer);
	mainSizer->SetSizeHints(this);

	wxString address;
	unsigned int port;
	::wxGetApp().getConfig(address, port, m_password);

	if (address.IsEmpty() || port == 0U || m_password.IsEmpty())
		return;

	m_handler = new CRemoteControlRemoteControlHandler(address, port);

	bool ret = m_handler->open();
	if (!ret) {
		delete m_handler;
		m_handler = NULL;
		return;
	}

	ret = m_handler->login();
	if (!ret) {
		m_handler->close();
		delete m_handler;
		m_handler = NULL;
		return;
	}

	m_state = RCFS_LOGIN;

	m_timer.Start(100);
}

CRemoteControlFrame::~CRemoteControlFrame()
{
}

wxMenuBar* CRemoteControlFrame::createMenuBar()
{
	wxMenu* fileMenu = new wxMenu();
	fileMenu->Append(wxID_EXIT, _("Exit"));

	wxMenu* editMenu = new wxMenu();
	editMenu->Append(Menu_Edit_Preferences, _("Preferences..."));

	wxMenu* helpMenu = new wxMenu();
	helpMenu->Append(wxID_ABOUT, _("About Remote Control"));

	wxMenuBar* menuBar = new wxMenuBar();
	menuBar->Append(fileMenu, _("File"));
	menuBar->Append(editMenu, _("Edit"));
	menuBar->Append(helpMenu, _("Help"));

	return menuBar;
}

void CRemoteControlFrame::onQuit(wxCommandEvent& event)
{
	Close(false);
}

void CRemoteControlFrame::onClose(wxCloseEvent& event)
{
	int x, y;
	GetPosition(&x, &y);
	if (x >= 0 && y >= 0)
		::wxGetApp().setPosition(x, y);

	m_timer.Stop();

	if (m_handler != NULL) {
		m_handler->logout();
		m_handler->close();
	}

	Destroy();
}

void CRemoteControlFrame::onPreferences(wxCommandEvent& event)
{
	wxString address, password;
	unsigned int port;
	::wxGetApp().getConfig(address, port, password);

	CRemoteControlPreferences dialog1(this, -1, address, port, password);
	if (dialog1.ShowModal() != wxID_OK)
		return;

	address   = dialog1.getAddress();
	port      = dialog1.getPort();
	password  = dialog1.getPassword();

	::wxGetApp().setConfig(address, port, password);

	wxMessageDialog dialog2(this, _("The changes made will not take effect\nuntil the application is restarted"), _("Remote Control Information"), wxICON_INFORMATION);
	dialog2.ShowModal();
}

void CRemoteControlFrame::onAbout(wxCommandEvent& event)
{
	wxAboutDialogInfo info;
	info.AddDeveloper(wxT("Jonathan Naylor, G4KLX"));
	info.SetCopyright(wxT("(C) 2011-2013 using GPL v2 or later"));
	info.SetName(APPLICATION_NAME);
	info.SetVersion(VERSION);
	info.SetDescription(_("This program allows for the controlling of\nircDDB Gateway and STARnet Digital Server."));

	::wxAboutBox(info);
}

void CRemoteControlFrame::onTimer(wxTimerEvent& event)
{
	RC_TYPE type = m_handler->readType();

	switch (type) {
		case RCT_NONE:
			m_handler->retry();
			break;

		case RCT_ACK:
			if (m_state == RCFS_HASH) {
				m_handler->setLoggedIn(true);
				m_handler->getCallsigns();
				m_state = RCFS_NORMAL;
			}
			break;

		case RCT_NAK: {
				if (m_state == RCFS_HASH || m_state == RCFS_LOGIN)
					m_handler->setLoggedIn(false);

				wxString text = m_handler->readNAK();
				m_state = RCFS_NORMAL;
				::wxMessageBox(text);
			}
			break;

		case RCT_RANDOM: {
				unsigned int rnd = m_handler->readRandom();
				sendHash(rnd);
				m_state = RCFS_HASH;
			}
			break;

		case RCT_CALLSIGNS: {
				m_noteBook->DeleteAllPages();

				CRemoteControlCallsignData* data = m_handler->readCallsigns();
				for (unsigned int i = 0U; i < data->getRepeaterCount(); i++)
					addRepeater(data->getRepeater(i));
				for (unsigned int i = 0U; i < data->getStarNetCount(); i++)
					addStarNet(data->getStarNet(i));
				delete data;

				m_state = RCFS_GET_REPEATERS;
				m_it1   = m_repeaters.begin();
				m_it2   = m_starNets.begin();
				getNextRepeater();
			}
			break;

		case RCT_REPEATER: {
				CRemoteControlRepeaterData* data = m_handler->readRepeater();
				wxString callsign = data->getCallsign();
				m_repeaters[callsign]->add(*data);
				delete data;

				if (m_state == RCFS_GET_REPEATERS || m_state == RCFS_GET_STARNETS)
					getNextRepeater();
			}
			break;

		case RCT_STARNET: {
				CRemoteControlStarNetGroup* data = m_handler->readStarNetGroup();
				wxString callsign = data->getCallsign();
				m_starNets[callsign]->add(*data);
				delete data;

				if (m_state == RCFS_GET_REPEATERS || m_state == RCFS_GET_STARNETS)
					getNextStarNet();
			}
			break;
	}
}

void CRemoteControlFrame::addRepeater(const wxString& callsign)
{
	CRemoteControlRepeaterPanel* repeater = new CRemoteControlRepeaterPanel(m_noteBook, -1, callsign);

	m_noteBook->AddPage(repeater, callsign, false);

	m_repeaters[callsign] = repeater;
}

void CRemoteControlFrame::addStarNet(const wxString& callsign)
{
	CRemoteControlStarNetPanel* starNet = new CRemoteControlStarNetPanel(m_noteBook, -1, callsign);

	m_noteBook->AddPage(starNet, callsign, false);

	m_starNets[callsign] = starNet;
}

void CRemoteControlFrame::repeaterRefresh(const wxString& callsign)
{
	m_handler->getRepeater(callsign);
}

void CRemoteControlFrame::starNetRefresh(const wxString& callsign)
{
	m_handler->getStarNet(callsign);
}

void CRemoteControlFrame::link(const wxString& callsign, RECONNECT reconnect, const wxString& reflector)
{
	m_handler->link(callsign, reconnect, reflector);
}

void CRemoteControlFrame::starNetLogoff(const wxString& callsign, const wxString& user)
{
	m_handler->logoff(callsign, user);
}

void CRemoteControlFrame::sendHash(unsigned int rnd)
{
	unsigned int len = m_password.Len() + sizeof(unsigned int);
	unsigned char*  in = new unsigned char[len];
	unsigned char* out = new unsigned char[32U];

	::memcpy(in, &rnd, sizeof(unsigned int));
	for (unsigned int i = 0U; i < m_password.Len(); i++)
		in[i + sizeof(unsigned int)] = m_password.GetChar(i);

	CSHA256 sha256;
	sha256.buffer(in, len, out);

	m_handler->sendHash(out, 32U);

	delete[] in;
	delete[] out;
}

void CRemoteControlFrame::getNextRepeater()
{
	if (m_it1 == m_repeaters.end()) {
		m_state = RCFS_GET_STARNETS;
		getNextStarNet();
		return;
	}

	m_handler->getRepeater(m_it1->first);
	++m_it1;
}
	
void CRemoteControlFrame::getNextStarNet()
{
	if (m_it2 == m_starNets.end()) {
		m_state = RCFS_NORMAL;
		return;
	}

	m_handler->getStarNet(m_it2->first);
	++m_it2;
}
