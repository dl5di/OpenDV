/*
 *   Copyright (C) 2010 by Jonathan Naylor G4KLX
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

#ifndef	DummyRepeaterSlowDataEvent_H
#define	DummyRepeaterSlowDataEvent_H

#include <wx/wx.h>

class CDummyRepeaterSlowDataEvent : public wxEvent {
public:
	CDummyRepeaterSlowDataEvent(const wxString& slowData, wxEventType type, int id = 0);
	virtual ~CDummyRepeaterSlowDataEvent();

	virtual wxString getSlowData() const;

	virtual wxEvent* Clone() const;

protected:
	CDummyRepeaterSlowDataEvent(const CDummyRepeaterSlowDataEvent& event);

private:
	wxString m_slowData;
};

#endif
