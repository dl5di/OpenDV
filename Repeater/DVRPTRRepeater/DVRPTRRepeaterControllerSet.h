/*
 *   Copyright (C) 2012 by Jonathan Naylor G4KLX
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

#ifndef	DVRPTRRepeaterControllerSet_H
#define	DVRPTRRepeaterControllerSet_H

#include <wx/wx.h>

class CDVRPTRRepeaterControllerSet : public wxPanel {
public:
	CDVRPTRRepeaterControllerSet(wxWindow* parent, int id, const wxString& title, const wxString& type, unsigned int time);
	virtual ~CDVRPTRRepeaterControllerSet();

	virtual bool Validate();

	virtual wxString getType() const;

	virtual unsigned int getTime() const;

private:
	wxString  m_title;
	wxChoice* m_type;
	wxSlider* m_time;
};

#endif
