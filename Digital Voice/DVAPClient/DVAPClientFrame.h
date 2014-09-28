/*
 *   Copyright (C) 2010,2012 by Jonathan Naylor G4KLX
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

#ifndef	DVAPClientFrame_H
#define	DVAPClientFrame_H

#include "CallsignTextCtrl.h"
#include "DVAPClientDefs.h"
#include "MessageEvent.h"
#include "MessageData.h"
#include "HeaderData.h"
#include "ErrorEvent.h"
#include "BusyData.h"

#include <wx/wx.h>
#include <wx/tglbtn.h>
#include <wx/listctrl.h>

class CDVAPClientFrame : public wxFrame {
public:
	CDVAPClientFrame(const wxString& title = wxEmptyString);
	virtual ~CDVAPClientFrame();

	virtual void onRecording(wxCommandEvent& event);
	virtual void onQuit(wxCommandEvent& event);

	virtual void onPreferences(wxCommandEvent& event);

	virtual void onAbout(wxCommandEvent& event);

	virtual void onClose(wxCloseEvent& event);

	virtual void onYour(wxCommandEvent& event);
	virtual void onRpt1(wxCommandEvent& event);
	virtual void onRpt2(wxCommandEvent& event);

	virtual void onTransmit(wxCommandEvent& event);
	virtual void onOneTouchReply(wxCommandEvent& event);

	virtual void onMessage(wxEvent& event);
	virtual void onHeader(wxEvent& event);
	virtual void onError(wxEvent& event);
	virtual void onBusy(wxEvent& event);

	virtual void showMessage(CMessageData* message);
	virtual void showHeader(CHeaderData* header);
	virtual void showBusy(CBusyData* busy);

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
	wxListCtrl*     m_heard;

	DECLARE_EVENT_TABLE()

	wxMenuBar* createMenuBar();
	void saveCalls() const;
	bool processYour(wxString& your);
	bool processRpt1(wxString& rpt1);
	bool processRpt2(wxString& rpt2);
};

#endif
