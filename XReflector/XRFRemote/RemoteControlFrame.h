/*
 *   Copyright (C) 2011,2012 by Jonathan Naylor G4KLX
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

#ifndef	RemoteControlFrame_H
#define	RemoteControlFrame_H

#include "RemoteControlRemoteControlHandler.h"
#include "RemoteControlRepeaterPanel.h"
#include "RemoteControlStarNetPanel.h"
#include "Defs.h"

#include <wx/wx.h>
#include <wx/notebook.h>

WX_DECLARE_STRING_HASH_MAP(CRemoteControlRepeaterPanel*, CRepeater_t);
WX_DECLARE_STRING_HASH_MAP(CRemoteControlStarNetPanel*, CStarNet_t);

enum RCF_STATE {
	RCFS_NORMAL,
	RCFS_LOGIN,
	RCFS_HASH,
	RCFS_GET_REPEATERS,
	RCFS_GET_STARNETS
};

class CRemoteControlFrame : public wxFrame {
public:
	CRemoteControlFrame(const wxString& title, const wxPoint& position);
	virtual ~CRemoteControlFrame();

	virtual void onQuit(wxCommandEvent& event);
	virtual void onPreferences(wxCommandEvent& event);
	virtual void onAbout(wxCommandEvent& event);
	virtual void onClose(wxCloseEvent& event);
	virtual void onTimer(wxTimerEvent& event);

	virtual void repeaterRefresh(const wxString& callsign);
	virtual void starNetRefresh(const wxString& callsign);
	virtual void link(const wxString& callsign, RECONNECT reconnect, const wxString& reflector);
	virtual void starNetLogoff(const wxString& callsign, const wxString& user);

private:
	RCF_STATE                           m_state;
	wxTimer                             m_timer;
	wxNotebook*                         m_noteBook;
	CRemoteControlRemoteControlHandler* m_handler;
	wxString                            m_password;
	CRepeater_t                         m_repeaters;
	CStarNet_t                          m_starNets;
	CRepeater_t::iterator               m_it1;
	CStarNet_t::iterator                m_it2;

	DECLARE_EVENT_TABLE()

	wxMenuBar* createMenuBar();
	void addRepeater(const wxString& callsign);
	void addStarNet(const wxString& callsign);
	void sendHash(unsigned int rnd);
	void getNextRepeater();
	void getNextStarNet();
};

#endif
