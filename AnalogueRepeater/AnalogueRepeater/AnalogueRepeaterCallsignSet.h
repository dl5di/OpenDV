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

#ifndef	AnalogueRepeaterCallsignSet_H
#define	AnalogueRepeaterCallsignSet_H

#include "AnalogueDefines.h"
#include "CWTextCtrl.h"

#include <wx/wx.h>

class CAnalogueRepeaterCallsignSet : public wxPanel {
public:
	CAnalogueRepeaterCallsignSet(wxWindow* parent, int id, const wxString& title, AUDIO_SOURCE openIdSource, const wxString& openId, AUDIO_SOURCE closeIdSource, const wxString& closeId, AUDIO_SOURCE beacon1Source, const wxString& beacon1, AUDIO_SOURCE beacon2Source, const wxString& beacon2, unsigned int speed, unsigned int freq, wxFloat32 level1, wxFloat32 level2);
	virtual ~CAnalogueRepeaterCallsignSet();

	virtual bool Validate();

	virtual AUDIO_SOURCE getOpenIdSource() const;
	virtual wxString     getOpenId() const;

	virtual AUDIO_SOURCE getCloseIdSource() const;
	virtual wxString     getCloseId() const;

	virtual AUDIO_SOURCE getBeacon1Source() const;
	virtual wxString     getBeacon1() const;

	virtual AUDIO_SOURCE getBeacon2Source() const;
	virtual wxString     getBeacon2() const;

	virtual unsigned int getSpeed() const;
	virtual unsigned int getFreq() const;
	virtual wxFloat32    getLevel1() const;
	virtual wxFloat32    getLevel2() const;

	virtual void onLevel1(wxScrollEvent& event);
	virtual void onLevel2(wxScrollEvent& event);

	virtual void onOpenIdChoice(wxCommandEvent& event);
	virtual void onCloseIdChoice(wxCommandEvent& event);
	virtual void onBeacon1Choice(wxCommandEvent& event);
	virtual void onBeacon2Choice(wxCommandEvent& event);

	virtual void onOpenIdButton(wxCommandEvent& event);
	virtual void onCloseIdButton(wxCommandEvent& event);
	virtual void onBeacon1Button(wxCommandEvent& event);
	virtual void onBeacon2Button(wxCommandEvent& event);

private:
	wxString     m_title;
	wxChoice*    m_openId;
	CCWTextCtrl* m_cwOpenId;
	wxTextCtrl*  m_wavOpenId;
	wxButton*    m_buttonOpenId;
	wxChoice*    m_closeId;
	CCWTextCtrl* m_cwCloseId;
	wxTextCtrl*  m_wavCloseId;
	wxButton*    m_buttonCloseId;
	wxChoice*    m_beacon1;
	CCWTextCtrl* m_cwBeacon1;
	wxTextCtrl*  m_wavBeacon1;
	wxButton*    m_buttonBeacon1;
	wxChoice*    m_beacon2;
	CCWTextCtrl* m_cwBeacon2;
	wxTextCtrl*  m_wavBeacon2;
	wxButton*    m_buttonBeacon2;
	wxSlider*    m_speed;
	wxSlider*    m_freq;
	wxSlider*    m_level1;
	wxSlider*    m_level2;

	DECLARE_EVENT_TABLE()
};

#endif
