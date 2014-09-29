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

#ifndef	DExtraClientControllerSet_H
#define	DExtraClientControllerSet_H

#include <wx/wx.h>

class CDExtraClientControllerSet : public wxPanel {
public:
	CDExtraClientControllerSet(wxWindow* parent, int id, const wxString& title, const wxString& type, unsigned int delay, bool pttInvert, bool squelchInvert);
	virtual ~CDExtraClientControllerSet();

	virtual bool Validate();

	virtual wxString getType() const;

	virtual unsigned int getDelay() const;

	virtual bool getPTTInvert() const;
	virtual bool getSquelchInvert() const;

private:
	wxString  m_title;
	wxChoice* m_type;
	wxChoice* m_delay;
	wxChoice* m_pttInvert;
	wxChoice* m_squelchInvert;
};

#endif
