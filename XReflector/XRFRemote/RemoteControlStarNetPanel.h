/*
 *   Copyright (C) 2011 by Jonathan Naylor G4KLX
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

#ifndef	RemoteControlStarNetPanel_H
#define	RemoteControlStarNetPanel_H

#include "RemoteControlStarNetGroup.h"

#include <wx/wx.h>
#include <wx/listctrl.h>

class CRemoteControlStarNetPanel : public wxPanel {
public:
	CRemoteControlStarNetPanel(wxWindow* parent, int id, const wxString& callsign);
	virtual ~CRemoteControlStarNetPanel();

	virtual void add(const CRemoteControlStarNetGroup& data);

	virtual void onClick(wxListEvent& event);
	virtual void onRefresh(wxCommandEvent& event);
	virtual void onLogoff(wxCommandEvent& event);
	virtual void onLogoffAll(wxCommandEvent& event);

private:
	wxString      m_callsign;
	wxListCtrl*   m_list;
	wxStaticText* m_logoff;
	wxStaticText* m_timer;
	wxButton*     m_logoffAll;
	wxString      m_chosen;
	wxMenu*       m_menu;

	DECLARE_EVENT_TABLE()
};

#endif
