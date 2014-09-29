/*
 *   Copyright (C) 2010,2011 by Jonathan Naylor G4KLX
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

#ifndef	DExtraClientPanel_H
#define	DExtraClientPanel_H

#include "DExtraClientDefs.h"
#include "MessageData.h"
#include "HeaderData.h"

#include <wx/wx.h>
#include <wx/tglbtn.h>
#include <wx/listctrl.h>

class CDExtraClientPanel : public wxPanel {
public:
	CDExtraClientPanel(wxWindow* parent, int id);
	virtual ~CDExtraClientPanel();

	virtual void onConnect(wxCommandEvent& event);
	virtual void onTransmit(wxCommandEvent& event);

	virtual void onModule(wxCommandEvent& event);

	virtual void onMessage(CMessageData* message);
	virtual void onHeader(CHeaderData* header);
	virtual void onError(const wxString& error);
	virtual void onTimer(wxTimerEvent& event);

private:
	wxTimer         m_timer;
	wxChoice*       m_reflector;
	wxToggleButton* m_connect;
	wxChoice*       m_module;
	wxStaticText*   m_status1;
	wxToggleButton* m_transmit;
	wxStaticText*   m_status2;
	wxStaticText*   m_hrdYour;
	wxStaticText*   m_hrdMy;
	wxStaticText*   m_hrdReflector;
	wxStaticText*   m_hrdMessage;
	wxListCtrl*     m_heard;
	wxString        m_reflectorName;
	bool            m_connected;
	bool            m_busy;
	wxArrayString   m_hosts;
	bool            m_initialised;

	DECLARE_EVENT_TABLE()

	void setModule();

	void setStartupReflector();
};

#endif
