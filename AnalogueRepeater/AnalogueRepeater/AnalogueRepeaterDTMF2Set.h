/*
 *   Copyright (C) 2009,2010 by Jonathan Naylor G4KLX
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

#ifndef	AnalogueRepeaterDTMF2Set_H
#define	AnalogueRepeaterDTMF2Set_H

#include "DTMFTextCtrl.h"

#include <wx/wx.h>

class CAnalogueRepeaterDTMF2Set : public wxPanel {
public:
	CAnalogueRepeaterDTMF2Set(wxWindow* parent, int id, const wxString& title, const wxString& command1,
		const wxString& command1Line, const wxString& command2, const wxString& command2Line);
	virtual ~CAnalogueRepeaterDTMF2Set();

	virtual bool Validate();

	virtual wxString getCommand1() const;
	virtual wxString getCommand1Line() const;
	virtual wxString getCommand2() const;
	virtual wxString getCommand2Line() const;

private:
	wxString       m_title;
	CDTMFTextCtrl* m_command1;
	wxTextCtrl*    m_command1Line;
	CDTMFTextCtrl* m_command2;
	wxTextCtrl*    m_command2Line;
};

#endif
