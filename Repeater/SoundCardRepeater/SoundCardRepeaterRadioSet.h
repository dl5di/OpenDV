/*
 *   Copyright (C) 2009-2012 by Jonathan Naylor G4KLX
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

#ifndef	SoundCardRepeaterRadioSet_H
#define	SoundCardRepeaterRadioSet_H

#include "SoundCardRepeaterDefs.h"
#include "DStarDefines.h"

#include <wx/wx.h>

class CSoundCardRepeaterRadioSet : public wxPanel {
public:
	CSoundCardRepeaterRadioSet(wxWindow* parent, int id, const wxString& title, const wxString& readDevice, const wxString& writeDevice, wxFloat32 rxLevel, wxFloat32 txLevel, SQUELCH_MODE squelchMode, wxFloat32 squelchLevel, bool rxInvert, bool txInvert);
	virtual ~CSoundCardRepeaterRadioSet();

	virtual bool Validate();

	virtual wxString getReadDevice() const;
	virtual wxString getWriteDevice() const;

	virtual wxFloat32 getRXLevel() const;
	virtual wxFloat32 getTXLevel() const;

	virtual SQUELCH_MODE getSquelchMode() const;
	virtual wxFloat32    getSquelchLevel() const;

	virtual bool getRXInvert() const;
	virtual bool getTXInvert() const;

	virtual void onRXLevel(wxScrollEvent& event);
	virtual void onTXLevel(wxScrollEvent& event);
	virtual void onSquelchLevel(wxScrollEvent& event);

private:
	wxString  m_title;
	wxChoice* m_readDevice;
	wxChoice* m_writeDevice;
	wxSlider* m_rxLevel;
	wxSlider* m_txLevel;
	wxChoice* m_squelchMode;
	wxSlider* m_squelchLevel;
	wxChoice* m_rxInvert;
	wxChoice* m_txInvert;

	DECLARE_EVENT_TABLE()
};

#endif
