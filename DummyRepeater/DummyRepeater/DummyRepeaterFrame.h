/*
 *   Copyright (C) 2010,2011,2012,2014 by Jonathan Naylor G4KLX
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

#ifndef	DummyRepeaterFrame_H
#define	DummyRepeaterFrame_H

#include "DummyRepeaterMessageEvent.h"
#include "DummyRepeaterErrorEvent.h"
#include "DummyRepeaterDefs.h"
#include "CallsignTextCtrl.h"
#include "MessageData.h"
#include "HeaderData.h"

#include <wx/wx.h>
#include <wx/tglbtn.h>
#include <wx/listctrl.h>

class CDummyRepeaterFrame : public wxFrame {
public:
	CDummyRepeaterFrame(const wxString& title, const wxPoint& position);
	virtual ~CDummyRepeaterFrame();

	virtual void onQuit(wxCommandEvent& event);

	virtual void onPreferences(wxCommandEvent& event);

	virtual void onAbout(wxCommandEvent& event);

	virtual void onClose(wxCloseEvent& event);

	virtual void onYour(wxCommandEvent& event);
	virtual void onRpt1(wxCommandEvent& event);
	virtual void onRpt2(wxCommandEvent& event);

	virtual void onTransmit(wxCommandEvent& event);
	virtual void onOneTouchReply(wxCommandEvent& event);

	virtual void onTimeout(wxTimerEvent& event);

	virtual void onMessage(wxEvent& event);
	virtual void onHeader(wxEvent& event);
	virtual void onSlowData(wxEvent& event);
	virtual void onStatus(wxEvent& event);
	virtual void onError(wxEvent& event);

	virtual void showMessage(CMessageData* message);
	virtual void showHeader(CHeaderData* header);
	virtual void showSlowData(const wxString& text);
	virtual void showStatus1(const wxString& text);
	virtual void showStatus2(const wxString& text);
	virtual void showStatus3(const wxString& text);
	virtual void showStatus4(const wxString& text);
	virtual void showStatus5(const wxString& text);

	virtual void error(const wxString& error);

private:
	wxComboBox*     m_your;
	wxComboBox*     m_rpt1;
	wxComboBox*     m_rpt2;
	wxToggleButton* m_transmit;
	wxStaticText*   m_status;
	wxStaticText*   m_hrdYour;
	wxStaticText*   m_hrdMy;
	wxStaticText*   m_hrdRpt1;
	wxStaticText*   m_hrdRpt2;
	wxStaticText*   m_hrdFlags;
	wxStaticText*   m_hrdMessage;
	wxStaticText*   m_slowData;
	wxStaticText*   m_status1;
	wxStaticText*   m_status2;
	wxStaticText*   m_status3;
	wxStaticText*   m_status4;
	wxStaticText*   m_status5;
	wxListCtrl*     m_heard;
	wxTimer         m_timer;
	unsigned int    m_timeout;

	DECLARE_EVENT_TABLE()

	wxMenuBar* createMenuBar();
	void saveCalls() const;
	void savePosition() const;
	bool processYour(wxString& your);
	bool processRpt1(wxString& rpt1);
	bool processRpt2(wxString& rpt2);
};

#endif
