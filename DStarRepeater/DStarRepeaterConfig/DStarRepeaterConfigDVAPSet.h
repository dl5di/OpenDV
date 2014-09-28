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

#ifndef	DStarRepeaterConfigDVAPSet_H
#define	DStarRepeaterConfigDVAPSet_H

#include "DStarRepeaterConfigDefs.h"
#include "DStarDefines.h"

#include <wx/wx.h>
#include <wx/spinctrl.h>

class CDStarRepeaterConfigDVAPSet : public wxDialog {
public:
	CDStarRepeaterConfigDVAPSet(wxWindow* parent, int id, const wxString& port, unsigned int frequency, int power, int squelch);
	virtual ~CDStarRepeaterConfigDVAPSet();

	virtual bool Validate();

	virtual wxString     getPort() const;
	virtual unsigned int getFrequency() const;
	virtual int          getPower() const;
	virtual int          getSquelch() const;

private:
	wxChoice*   m_port;
	wxChoice*   m_band;
	wxTextCtrl* m_frequency;
	wxSpinCtrl* m_power;
	wxSpinCtrl* m_squelch;
};

#endif
