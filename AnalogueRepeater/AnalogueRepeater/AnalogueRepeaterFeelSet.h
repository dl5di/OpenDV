/*
 *   Copyright (C) 2009,2010,2013 by Jonathan Naylor G4KLX
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

#ifndef	AnalogueRepeaterFeelSet_H
#define	AnalogueRepeaterFeelSet_H

#include <wx/wx.h>

#include "AnalogueDefines.h"
#include "TimeoutTones.h"

class CAnalogueRepeaterFeelSet : public wxPanel {
public:
	CAnalogueRepeaterFeelSet(wxWindow* parent, int id, const wxString& title, ANALOGUE_CALLSIGN_START callAtStart, unsigned int startDelay, bool callAtEnd, ANALOGUE_TIMEOUT_TYPE timeoutType, ANALOGUE_CALLSIGN_HOLDOFF holdoff);
	virtual ~CAnalogueRepeaterFeelSet();

	virtual bool Validate();

	virtual ANALOGUE_CALLSIGN_START   getCallAtStart() const;
	virtual unsigned int              getStartDelay() const;
	virtual bool                      getCallAtEnd() const;
	virtual ANALOGUE_TIMEOUT_TYPE     getTimeoutType() const;
	virtual ANALOGUE_CALLSIGN_HOLDOFF getHoldoff() const;

private:
	wxString  m_title;
	wxChoice* m_callAtStart;
	wxSlider* m_startDelay;
	wxChoice* m_callAtEnd;
	wxChoice* m_timeoutType;
	wxChoice* m_holdoff;
};

#endif
