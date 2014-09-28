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

#ifndef	TimerControlFrame_H
#define	TimerControlFrame_H

#include "TimerControlRemoteControlHandler.h"
#include "TimerControlRepeaterPanel.h"
#include "Defs.h"

#include <wx/wx.h>
#include <wx/notebook.h>

WX_DECLARE_STRING_HASH_MAP(CTimerControlRepeaterPanel*, CRepeater_t);

enum TCF_STATE {
	TCFS_NORMAL,
	TCFS_LOGIN,
	TCFS_HASH
};

class CTimerControlFrame : public wxFrame {
public:
	CTimerControlFrame(const wxString& title, const wxPoint& position, bool delay);
	virtual ~CTimerControlFrame();

	virtual void onQuit(wxCommandEvent& event);
	virtual void onPreferences(wxCommandEvent& event);
	virtual void onAbout(wxCommandEvent& event);
	virtual void onClose(wxCloseEvent& event);
	virtual void onTimer1(wxTimerEvent& event);
	virtual void onTimer2(wxTimerEvent& event);

	virtual void setFileName(const wxString& fileName);

	virtual void writeItems();

private:
	TCF_STATE                          m_state;
	wxTimer                            m_timer1;
	wxTimer                            m_timer2;
	wxNotebook*                        m_noteBook;
	CTimerControlRemoteControlHandler* m_handler;
	wxString                           m_password;
	wxString                           m_fileName;
	CRepeater_t                        m_repeaters;

	DECLARE_EVENT_TABLE()

	wxMenuBar* createMenuBar();
	void startup();
	void addRepeater(const wxString& callsign);
	void sendHash(unsigned int rnd);
	void loadItems();
};

#endif
