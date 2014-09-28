/*
 *   Copyright (C) 2009-2014 by Jonathan Naylor G4KLX
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

#ifndef	DStarRepeaterConfigSoundCardSet_H
#define	DStarRepeaterConfigSoundCardSet_H

#include "DStarRepeaterConfigDefs.h"
#include "DStarDefines.h"

#include <wx/wx.h>

class CDStarRepeaterConfigSoundCardSet : public wxDialog {
public:
	CDStarRepeaterConfigSoundCardSet(wxWindow* parent, int id, const wxString& rxDevice, const wxString& txDevice, bool rxInvert, bool txInvert, wxFloat32 rxLevel, wxFloat32 txLevel, unsigned int txDelay, unsigned int txTail);
	virtual ~CDStarRepeaterConfigSoundCardSet();

	virtual bool Validate();

	virtual wxString     getRXDevice() const;
	virtual wxString     getTXDevice() const;
	virtual bool         getRXInvert() const;
	virtual bool         getTXInvert() const;
	virtual wxFloat32    getRXLevel() const;
	virtual wxFloat32    getTXLevel() const;
	virtual unsigned int getTXDelay() const;
	virtual unsigned int getTXTail() const;

private:
	wxChoice* m_rxDevice;
	wxChoice* m_txDevice;
	wxChoice* m_rxInvert;
	wxChoice* m_txInvert;
	wxSlider* m_rxLevel;
	wxSlider* m_txLevel;
	wxSlider* m_txDelay;
	wxSlider* m_txTail;
};

#endif
