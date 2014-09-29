/*
 *   Copyright (C) 2010,2011,2012 by Jonathan Naylor G4KLX
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

#ifndef	GMSKRepeaterModemSet_H
#define	GMSKRepeaterModemSet_H

#include "GMSKRepeaterDefs.h"
#include "DStarDefines.h"

#include <wx/wx.h>

class CGMSKRepeaterModemSet : public wxPanel {
public:
	CGMSKRepeaterModemSet(wxWindow* parent, int id, const wxString& title, GMSK_MODEM_TYPE type, unsigned int address);
	virtual ~CGMSKRepeaterModemSet();

	virtual bool Validate();

	virtual GMSK_MODEM_TYPE getType() const;
	virtual unsigned int getAddress() const;

private:
	wxString  m_title;
#if defined(WIN32)
	wxChoice* m_type;
#endif
	wxChoice* m_address;
};

#endif
