/*
 *   Copyright (C) 2009,2010,2012 by Jonathan Naylor G4KLX
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

#ifndef	AnalogueRepeaterFrame_H
#define	AnalogueRepeaterFrame_H

#include "AnalogueRepeaterDefs.h"

#include <wx/wx.h>

class CAnalogueRepeaterFrame : public wxFrame {
public:
	CAnalogueRepeaterFrame(const wxString& title, const wxPoint& position, bool gui);
	virtual ~CAnalogueRepeaterFrame();

#if defined(__WXDEBUG__)
	virtual void onOpen(wxCommandEvent& event);
#endif
	virtual void onQuit(wxCommandEvent& event);
	virtual void onPreferences(wxCommandEvent& event);
	virtual void onUpdates(wxCommandEvent& event);
	virtual void onAbout(wxCommandEvent& event);
	virtual void onClose(wxCloseEvent& event);
	virtual void onActions(wxCommandEvent& event);
	virtual void onOutputs(wxCommandEvent& event);
	virtual void onLog(wxEvent& event);

	virtual void onTimer(wxTimerEvent& event);

	virtual void showLog(const wxString& text);

	virtual void setOutputs(bool out1, bool out2, bool out3, bool out4);

private:
	wxStaticText* m_radioTransmit;
	wxStaticText* m_radioSquelch;
	wxStaticText* m_radioVOGAD;
	wxStaticText* m_toneburst;
	wxStaticText* m_ctcss;
	wxStaticText* m_radioDTMF;
	wxStaticText* m_extSquelch;
	wxStaticText* m_extTransmit;
	wxStaticText* m_extVOGAD;
	wxStaticText* m_extDTMF;
	wxStaticText* m_level;
	wxStaticText* m_state;
	wxStaticText* m_battery;
	wxStaticText* m_timeout;
	wxStaticText* m_lockout;
	wxStaticText* m_latch;
	wxStaticText* m_hang;
	wxStaticText* m_ack;
	wxStaticText* m_minimum;
	wxStaticText* m_callsign;
	wxStaticText* m_active;
	wxStaticText* m_logLine1;
	wxStaticText* m_logLine2;
	wxStaticText* m_logLine3;
	wxTimer       m_timer;
	wxMenu*       m_outputMenu;
	bool          m_updates;

	DECLARE_EVENT_TABLE()

	wxMenuBar* createMenuBar();
};

#endif
