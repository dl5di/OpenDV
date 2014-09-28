/*
 *   Copyright (C) 2009,2010,2011 by Jonathan Naylor G4KLX
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

#ifndef	AnalogueRepeaterTonesSet_H
#define	AnalogueRepeaterTonesSet_H

#include "AnalogueDefines.h"

#include <wx/wx.h>

class CAnalogueRepeaterTonesSet : public wxPanel {
public:
	CAnalogueRepeaterTonesSet(wxWindow* parent, int id, const wxString& title, bool tbEnable, wxFloat32 tbThreshold, wxFloat32 ctcssFreq, bool ctcssInternal, wxFloat32 ctcssThresh, wxFloat32 ctcssLevel, unsigned int ctcssHangTime, ANALOGUE_CTCSS_OUTPUT ctcssOutput);
	virtual ~CAnalogueRepeaterTonesSet();

	virtual bool Validate();

	virtual bool                  getTbEnable() const;
	virtual wxFloat32             getTbThreshold() const;
	virtual wxFloat32             getCTCSSFreq() const;
	virtual bool                  getCTCSSInternal() const;
	virtual wxFloat32             getCTCSSThresh() const;
	virtual wxFloat32             getCTCSSLevel() const;
	virtual unsigned int          getCTCSSHangTime() const;
	virtual ANALOGUE_CTCSS_OUTPUT getCTCSSOutput() const;

	virtual void onTbThreshold(wxScrollEvent& event);
	virtual void onCTCSSThreshold(wxScrollEvent& event);
	virtual void onCTCSSLevel(wxScrollEvent& event);

private:
	wxString  m_title;
	wxChoice* m_tbEnable;
	wxSlider* m_tbThreshold;
	wxChoice* m_ctcssFreq;
	wxChoice* m_ctcssInternal;
	wxSlider* m_ctcssThresh;
	wxSlider* m_ctcssLevel;
	wxChoice* m_ctcssHangTime;
	wxChoice* m_ctcssOutput;

	DECLARE_EVENT_TABLE()
};

#endif
