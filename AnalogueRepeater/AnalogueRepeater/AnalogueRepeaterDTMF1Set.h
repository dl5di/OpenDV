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

#ifndef	AnalogueRepeaterDTMF1Set_H
#define	AnalogueRepeaterDTMF1Set_H

#include "DTMFTextCtrl.h"

#include <wx/wx.h>

class CAnalogueRepeaterDTMF1Set : public wxPanel {
public:
	CAnalogueRepeaterDTMF1Set(wxWindow* parent, int id, const wxString& title, bool radio, bool external,
		const wxString& shutdown, const wxString& startup, const wxString& timeout, const wxString& timeReset,
		const wxString& output1, const wxString& output2, const wxString& output3, const wxString& output4,
		wxFloat32 threshold);
	virtual ~CAnalogueRepeaterDTMF1Set();

	virtual bool Validate();

	virtual bool      getRadio() const;
	virtual bool      getExternal() const;

	virtual wxString  getShutdown() const;
	virtual wxString  getStartup() const;

	virtual wxString  getTimeout() const;
	virtual wxString  getTimeReset() const;

	virtual wxString  getOutput1() const;
	virtual wxString  getOutput2() const;
	virtual wxString  getOutput3() const;
	virtual wxString  getOutput4() const;

	virtual wxFloat32 getThreshold() const;

	virtual void onThreshold(wxScrollEvent& event);

private:
	wxString       m_title;
	wxChoice*      m_radio;
	wxChoice*      m_external;
	CDTMFTextCtrl* m_shutdown;
	CDTMFTextCtrl* m_startup;
	CDTMFTextCtrl* m_timeout;
	CDTMFTextCtrl* m_timeReset;
	CDTMFTextCtrl* m_output1;
	CDTMFTextCtrl* m_output2;
	CDTMFTextCtrl* m_output3;
	CDTMFTextCtrl* m_output4;
	wxSlider*      m_threshold;

	DECLARE_EVENT_TABLE()
};

#endif
