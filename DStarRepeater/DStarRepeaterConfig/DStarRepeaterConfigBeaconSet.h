/*
 *   Copyright (C) 2012,2014 by Jonathan Naylor G4KLX
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

#ifndef	DStarRepeaterConfigBeaconSet_H
#define	DStarRepeaterConfigBeaconSet_H

#include "MessageTextCtrl.h"
#include "DStarDefines.h"

#include <wx/wx.h>

class CDStarRepeaterConfigBeaconSet : public wxPanel {
public:
	CDStarRepeaterConfigBeaconSet(wxWindow* parent, int id, const wxString& title, unsigned int time, const wxString& text, bool voice, TEXT_LANG language);
	virtual ~CDStarRepeaterConfigBeaconSet();

	virtual bool Validate();

	virtual unsigned int getTime() const;
	virtual wxString     getText() const;
	virtual bool         getVoice() const;
	virtual TEXT_LANG    getLanguage() const;

private:
	wxString          m_title;
	wxSlider*         m_time;
	CMessageTextCtrl* m_text;
	wxChoice*         m_voice;
	wxChoice*         m_language;
};

#endif
