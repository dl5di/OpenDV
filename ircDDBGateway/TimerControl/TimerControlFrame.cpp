/*
 *   Copyright (C) 2011-2014 by Jonathan Naylor G4KLX
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

#include "TimerControlPreferences.h"
#include "TimerControlItemFile.h"
#include "TimerControlFrame.h"
#include "TimerControlItem.h"
#include "TimerControlDefs.h"
#include "TimerControlApp.h"
#include "Version.h"
#include "SHA256.h"

const unsigned int BORDER_SIZE = 5U;

#if defined(__WINDOWS__)
const unsigned int MAIN_HEIGHT    = 350U;
const unsigned int MAIN_WIDTH     = 500U;
#else
const unsigned int MAIN_HEIGHT    = 400U;
const unsigned int MAIN_WIDTH     = 590U;
#endif

#include <wx/aboutdlg.h>

enum {
	Menu_Edit_Preferences = 7000,

	Timer_Timer1,
	Timer_Timer2
};

BEGIN_EVENT_TABLE(CTimerControlFrame, wxFrame)
	EVT_MENU(wxID_EXIT, CTimerControlFrame::onQuit)
	EVT_MENU(Menu_Edit_Preferences, CTimerControlFrame::onPreferences)
	EVT_MENU(wxID_ABOUT, CTimerControlFrame::onAbout)

	EVT_TIMER(Timer_Timer1, CTimerControlFrame::onTimer1)
	EVT_TIMER(Timer_Timer2, CTimerControlFrame::onTimer2)

	EVT_CLOSE(CTimerControlFrame::onClose)
END_EVENT_TABLE()

CTimerControlFrame::CTimerControlFrame(const wxString& title, const wxPoint& position, bool delay) :
wxFrame(NULL, -1, title, position),
m_state(TCFS_NORMAL),
m_timer1(this, Timer_Timer1),
m_timer2(this, Timer_Timer2),
m_noteBook(NULL),
m_handler(NULL),
m_password(),
m_fileName(),
m_repeaters()
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

	m_timer1.Start(delay ? 20000 : 100, wxTIMER_ONE_SHOT);
}

CTimerControlFrame::~CTimerControlFrame()
{
}

wxMenuBar* CTimerControlFrame::createMenuBar()
{
	wxMenu* fileMenu = new wxMenu();
	fileMenu->Append(wxID_EXIT, _("Exit"));

	wxMenu* editMenu = new wxMenu();
	editMenu->Append(Menu_Edit_Preferences, _("Preferences..."));

	wxMenu* helpMenu = new wxMenu();
	helpMenu->Append(wxID_ABOUT, _("About Timer Control"));

	wxMenuBar* menuBar = new wxMenuBar();
	menuBar->Append(fileMenu, _("File"));
	menuBar->Append(editMenu, _("Edit"));
	menuBar->Append(helpMenu, _("Help"));

	return menuBar;
}

void CTimerControlFrame::onQuit(wxCommandEvent&)
{
	Close(false);
}

void CTimerControlFrame::onClose(wxCloseEvent&)
{
	int x, y;
	GetPosition(&x, &y);
	if (x >= 0 && y >= 0) {
		::wxGetApp().setPosition(x, y);
		::wxGetApp().writeConfig();
	}

	m_timer1.Stop();
	m_timer2.Stop();

	if (m_handler != NULL) {
		m_handler->logout();
		m_handler->close();
	}

	Destroy();
}

void CTimerControlFrame::onPreferences(wxCommandEvent&)
{
	wxString address, password;
	unsigned int port;
	::wxGetApp().getGateway(address, port, password);

	bool delay;
	::wxGetApp().getDelay(delay);

	CTimerControlPreferences dialog1(this, -1, address, port, password, delay);
	if (dialog1.ShowModal() != wxID_OK)
		return;

	address   = dialog1.getAddress();
	port      = dialog1.getPort();
	password  = dialog1.getPassword();

	delay     = dialog1.getDelay();

	::wxGetApp().setGateway(address, port, password);
	::wxGetApp().setDelay(delay);
	::wxGetApp().writeConfig();

	wxMessageDialog dialog2(this, _("The changes made will not take effect\nuntil the application is restarted"), _("Timer Control Information"), wxICON_INFORMATION);
	dialog2.ShowModal();
}

void CTimerControlFrame::onAbout(wxCommandEvent&)
{
	wxAboutDialogInfo info;
	info.AddDeveloper(wxT("Jonathan Naylor, G4KLX"));
	info.SetCopyright(wxT("(C) 2011-2014 using GPL v2 or later"));
	info.SetName(APPLICATION_NAME);
	info.SetVersion(VERSION);
	info.SetDescription(_("This program allows for the controlling of\nthe ircDDB Gateway."));

	::wxAboutBox(info);
}

void CTimerControlFrame::onTimer1(wxTimerEvent&)
{
	startup();
}

void CTimerControlFrame::onTimer2(wxTimerEvent&)
{
	TC_TYPE type = m_handler->readType();

	switch (type) {
		case TCT_NONE:
			m_handler->retry();
			break;

		case TCT_ACK:
			if (m_state == TCFS_HASH) {
				m_handler->setLoggedIn(true);
				m_handler->getCallsigns();
				m_state = TCFS_NORMAL;
			}
			break;

		case TCT_NAK: {
				if (m_state == TCFS_HASH || m_state == TCFS_LOGIN)
					m_handler->setLoggedIn(false);
				m_handler->logout();

				wxString text = m_handler->readNAK();
				m_state = TCFS_NORMAL;
				::wxMessageBox(text);
			}
			break;

		case TCT_RANDOM: {
				unsigned int rnd = m_handler->readRandom();
				sendHash(rnd);
				m_state = TCFS_HASH;
			}
			break;

		case TCT_CALLSIGNS: {
				m_noteBook->DeleteAllPages();

				wxArrayString data = m_handler->readCallsigns();

				for (unsigned int i = 0U; i < data.GetCount(); i++)
					addRepeater(data.Item(i));

				m_handler->logout();

				loadItems();
			}
			break;
	}
}

void CTimerControlFrame::startup()
{
	wxString address;
	unsigned int port;
	::wxGetApp().getGateway(address, port, m_password);

	if (address.IsEmpty() || port == 0U || m_password.IsEmpty())
		return;

	m_handler = new CTimerControlRemoteControlHandler(address, port);

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

	m_state = TCFS_LOGIN;

	m_timer2.Start(100, wxTIMER_CONTINUOUS);
}

void CTimerControlFrame::addRepeater(const wxString& callsign)
{
	CTimerControlRepeaterPanel* repeater = new CTimerControlRepeaterPanel(m_noteBook, -1, callsign);

	m_noteBook->AddPage(repeater, callsign, false);

	m_repeaters[callsign] = repeater;
}

void CTimerControlFrame::setFileName(const wxString& fileName)
{
	m_fileName = fileName;
}

void CTimerControlFrame::loadItems()
{
	CTimerControlItemFile file(m_fileName);

	unsigned int count;
	CTimerControlItem** items = file.readItems(count);

	if (count == 0U)
		return;

	for (CRepeater_t::iterator it = m_repeaters.begin(); it != m_repeaters.end(); ++it)
		(*it).second->load(items, count);

	delete[] items;
}

void CTimerControlFrame::writeItems()
{
	// Count the total number of items from all the panels
	unsigned int count = 0U;
	for (CRepeater_t::iterator it = m_repeaters.begin(); it != m_repeaters.end(); ++it)
		count += (*it).second->getCount();

	if (count == 0U)
		return;

	CTimerControlItem** items = new CTimerControlItem*[count];

	unsigned int n = 0U;
	for (CRepeater_t::iterator it = m_repeaters.begin(); it != m_repeaters.end(); ++it) {
		unsigned int count1 = (*it).second->getCount();
		CTimerControlItem** items1 = (*it).second->getItems();

		for (unsigned int i = 0U; i < count1; i++, n++)
			items[n] = items1[i];

		delete[] items1;
	}

	CTimerControlItemFile file(m_fileName);

	file.writeItems(items, count);

	delete[] items;
}

void CTimerControlFrame::sendHash(unsigned int rnd)
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
