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

#ifndef	RemoteControlRepeaterPanel_H
#define	RemoteControlRepeaterPanel_H

#include "RemoteControlRepeaterData.h"

#include <wx/wx.h>
#include <wx/listctrl.h>

WX_DEFINE_ARRAY_INT(PROTOCOL, CArrayProtocol);

class CRemoteControlRepeaterPanel : public wxPanel {
public:
	CRemoteControlRepeaterPanel(wxWindow* parent, int id, const wxString& callsign);
	virtual ~CRemoteControlRepeaterPanel();

	virtual void add(const CRemoteControlRepeaterData& data);

	virtual void onRefresh(wxCommandEvent& event);
	virtual void onLink(wxCommandEvent& event);
	virtual void onUnlink(wxCommandEvent& event);
	virtual void onSelect(wxListEvent& event);

private:
	wxString       m_callsign;
	wxListCtrl*    m_list;
	wxChoice*      m_reflector;
	wxChoice*      m_channel;
	wxChoice*      m_reconnect;
	wxButton*      m_unlink;
	int            m_selected;
	wxArrayString  m_reflectors;
	CArrayProtocol m_protocols;

	DECLARE_EVENT_TABLE()
};

#endif
