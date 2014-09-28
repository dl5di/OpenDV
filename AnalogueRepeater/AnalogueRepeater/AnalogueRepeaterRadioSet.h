/*
 *   Copyright (C) 2009 by Jonathan Naylor G4KLX
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

#ifndef	AnalogueRepeaterRadioSet_H
#define	AnalogueRepeaterRadioSet_H

#include <wx/wx.h>

class CAnalogueRepeaterRadioSet : public wxPanel {
public:
	CAnalogueRepeaterRadioSet(wxWindow* parent, int id, const wxString& title, const wxString& readDevice, const wxString& writeDevice, unsigned int audioDelay, bool deEmphasis, bool preEmphasis, bool vogad);
	virtual ~CAnalogueRepeaterRadioSet();

	virtual bool Validate();

	virtual wxString     getReadDevice() const;
	virtual wxString     getWriteDevice() const;
	virtual unsigned int getAudioDelay() const;
	virtual bool         getDeEmphasis() const;
	virtual bool         getPreEmphasis() const;
	virtual bool         getVOGAD() const;

private:
	wxString  m_title;
	wxChoice* m_readDevice;
	wxChoice* m_writeDevice;
	wxChoice* m_audioDelay;
	wxChoice* m_deEmphasis;
	wxChoice* m_preEmphasis;
	wxChoice* m_vogad;
};

#endif
