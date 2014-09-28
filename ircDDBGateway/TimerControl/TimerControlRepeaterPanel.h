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

#ifndef	TimerControlRepeaterPanel_H
#define	TimerControlRepeaterPanel_H

#include "TimerControlItem.h"

#include <wx/wx.h>
#include <wx/listctrl.h>

class CTimerControlRepeaterPanel : public wxPanel {
public:
	CTimerControlRepeaterPanel(wxWindow* parent, int id, const wxString& callsign);
	virtual ~CTimerControlRepeaterPanel();

	virtual void onSelect(wxListEvent& event);

	virtual void onAdd(wxCommandEvent& event);
	virtual void onModify(wxCommandEvent& event);
	virtual void onDelete(wxCommandEvent& event);

	virtual void load(CTimerControlItem** items, unsigned int count);

	virtual unsigned int getCount() const;
	virtual CTimerControlItem** getItems() const;

private:
	wxString           m_callsign;
	wxListCtrl*        m_list;
	wxChoice*          m_day;
	wxChoice*          m_hour;
	wxChoice*          m_minute;
	wxChoice*          m_reflector;
	wxChoice*          m_channel;
	wxChoice*          m_reconnect;
	CTimerControlItem* m_item;
	int                m_n;

	DECLARE_EVENT_TABLE()

	void insertItem(CTimerControlItem* item);
	void reload(CTimerControlItem* item = NULL);
};

#endif
