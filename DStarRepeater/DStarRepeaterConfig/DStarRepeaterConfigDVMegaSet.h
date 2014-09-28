/*
 *   Copyright (C) 2011-2014 by Jonathan Naylor G4KLX
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

#ifndef	DStarRepeaterConfigDVMegaSet_H
#define	DStarRepeaterConfigDVMegaSet_H

#include "DStarRepeaterConfigDefs.h"
#include "DStarDefines.h"

#include <wx/wx.h>

class CDStarRepeaterConfigDVMegaSet : public wxDialog {
public:
	CDStarRepeaterConfigDVMegaSet(wxWindow* parent, int id, const wxString& port, DVMEGA_VARIANT variant, bool rxInvert, bool txInvert, unsigned int txDelay, unsigned int frequency, unsigned int power);
	virtual ~CDStarRepeaterConfigDVMegaSet();

	virtual bool Validate();

	virtual wxString       getPort() const;
	virtual DVMEGA_VARIANT getVariant() const;
	virtual bool           getRXInvert() const;
	virtual bool           getTXInvert() const;
	virtual unsigned int   getTXDelay() const;
	virtual unsigned int   getFrequency() const;
	virtual unsigned int   getPower() const;

	virtual void onVariant(wxCommandEvent& event);

private:
	wxChoice*   m_port;
	wxChoice*   m_variant;
	wxChoice*   m_txInvert;
	wxChoice*   m_rxInvert;
	wxSlider*   m_txDelay;
	wxChoice*   m_band;
	wxTextCtrl* m_frequency;
	wxSlider*   m_power;

	DECLARE_EVENT_TABLE()
};

#endif
