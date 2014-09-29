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

#ifndef	XReflectorPreferences_H
#define	XReflectorPreferences_H

#include <wx/wx.h>
#include <wx/notebook.h>

#include "XReflectorMiscellaneousSet.h"
#include "XReflectorReflectorSet.h"
#include "XReflectorDPlusSet.h"

class CXReflectorPreferences : public wxDialog {
public:
	CXReflectorPreferences(wxWindow* parent, int id, const wxString& callsign,
				const wxString& address, bool dplusEnabled, bool logEnabled);
	virtual ~CXReflectorPreferences();

	virtual bool Validate();

	virtual wxString getCallsign() const;
	virtual wxString getAddress() const;

	virtual bool     getDPlusEnabled() const;

	virtual bool     getLogEnabled() const;

private:
	CXReflectorReflectorSet*     m_reflector;
	CXReflectorDPlusSet*         m_dplus;
	CXReflectorMiscellaneousSet* m_miscellaneous;
};

#endif
