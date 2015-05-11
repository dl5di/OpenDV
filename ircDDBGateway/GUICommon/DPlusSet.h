/*
 *   Copyright (C) 2010,2011,2013 by Jonathan Naylor G4KLX
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

#ifndef	DPlusSet_H
#define	DPlusSet_H

#include "CallsignTextCtrl.h"
#include "Defs.h"

#include <wx/wx.h>

class CDPlusSet : public wxPanel {
public:
	CDPlusSet(wxWindow* parent, int id, const wxString& title, bool enabled, unsigned int maxDongles, unsigned int maxLinks, const wxString& login);
	virtual ~CDPlusSet();

	virtual bool Validate();

	virtual bool         getEnabled() const;
	virtual unsigned int getMaxDongles() const;
	virtual wxString     getLogin() const;

private:
	wxString           m_title;
	wxChoice*          m_enabled;
	wxChoice*          m_maxDongles;
	CCallsignTextCtrl* m_login;
};

#endif
