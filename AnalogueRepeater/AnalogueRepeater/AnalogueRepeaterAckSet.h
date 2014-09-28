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

#ifndef	AnalogueRepeaterAckSet_H
#define	AnalogueRepeaterAckSet_H

#include <wx/wx.h>

#include "AnalogueDefines.h"
#include "CWTextCtrl.h"

class CAnalogueRepeaterAckSet : public wxPanel {
public:
	CAnalogueRepeaterAckSet(wxWindow* parent, int id, const wxString& title, AUDIO_SOURCE radioAckSource, const wxString& radioAck, AUDIO_SOURCE externalAckSource, const wxString& externalAck, AUDIO_SOURCE batteryAckSource, const wxString& batteryAck, unsigned int speed, unsigned int freq, wxFloat32 level, unsigned int delay, unsigned int minimum);
	virtual ~CAnalogueRepeaterAckSet();

	virtual bool Validate();

	virtual AUDIO_SOURCE getRadioAckSource() const;
	virtual wxString     getRadioAck() const;

	virtual AUDIO_SOURCE getExternalAckSource() const;
	virtual wxString     getExternalAck() const;

	virtual AUDIO_SOURCE getBatteryAckSource() const;
	virtual wxString     getBatteryAck() const;

	virtual unsigned int getSpeed() const;
	virtual unsigned int getFreq() const;
	virtual wxFloat32    getLevel() const;
	virtual unsigned int getDelay() const;
	virtual unsigned int getMinimum() const;

	virtual void onLevel(wxScrollEvent& event);

	virtual void onRadioAckChoice(wxCommandEvent& event);
	virtual void onExternalAckChoice(wxCommandEvent& event);
	virtual void onBatteryAckChoice(wxCommandEvent& event);

	virtual void onRadioAckButton(wxCommandEvent& event);
	virtual void onExternalAckButton(wxCommandEvent& event);
	virtual void onBatteryAckButton(wxCommandEvent& event);

private:
	wxString     m_title;
	wxChoice*    m_radioAck;
	CCWTextCtrl* m_cwRadioAck;
	wxTextCtrl*  m_wavRadioAck;
	wxButton*    m_buttonRadioAck;
	wxChoice*    m_externalAck;
	CCWTextCtrl* m_cwExternalAck;
	wxTextCtrl*  m_wavExternalAck;
	wxButton*    m_buttonExternalAck;
	wxChoice*    m_batteryAck;
	CCWTextCtrl* m_cwBatteryAck;
	wxTextCtrl*  m_wavBatteryAck;
	wxButton*    m_buttonBatteryAck;
	wxSlider*    m_speed;
	wxSlider*    m_freq;
	wxSlider*    m_level;
	wxChoice*    m_delay;
	wxChoice*    m_minimum;

	DECLARE_EVENT_TABLE()
};

#endif
