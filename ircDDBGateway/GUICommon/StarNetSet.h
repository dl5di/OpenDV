/*
 *   Copyright (C) 2011,2012 by Jonathan Naylor G4KLX
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

#ifndef	StarNetSet_H
#define	StarNetSet_H

#include "CallsignTextCtrl.h"
#include "Defs.h"

#include <wx/wx.h>

class CStarNetSet : public wxPanel {
public:
#if defined(DEXTRA_LINK) || defined(DCS_LINK)
	CStarNetSet(wxWindow* parent, int id, const wxString& title, const wxString& band, const wxString& callsign, const wxString& logoff, const wxString& info, const wxString& permanent, unsigned int userTimeout, unsigned int groupTimeout, STARNET_CALLSIGN_SWITCH callsignSwitch, bool txMsgSwitch, const wxString& reflector);
#else
	CStarNetSet(wxWindow* parent, int id, const wxString& title, const wxString& band, const wxString& callsign, const wxString& logoff, const wxString& info, const wxString& permanent, unsigned int userTimeout, unsigned int groupTimeout, STARNET_CALLSIGN_SWITCH callsignSwitch, bool txMsgSwitch);
#endif
	virtual ~CStarNetSet();

	virtual bool Validate();

	virtual wxString                getBand() const;
	virtual wxString                getCallsign() const;
	virtual wxString                getLogoff() const;
	virtual wxString                getInfo() const;
	virtual wxString                getPermanent() const;
	virtual unsigned int            getGroupTimeout() const;
	virtual unsigned int            getUserTimeout() const;
	virtual STARNET_CALLSIGN_SWITCH getCallsignSwitch() const;
	virtual bool                    getTXMsgSwitch() const;
#if defined(DEXTRA_LINK) || defined(DCS_LINK)
	virtual wxString                getReflector() const;
#endif

private:
	wxString           m_title;
	wxChoice*          m_band;
	CCallsignTextCtrl* m_callsign;
	CCallsignTextCtrl* m_logoff;
	wxTextCtrl*        m_info;
	CCallsignTextCtrl* m_permanent;
	wxChoice*          m_userTimeout;
	wxChoice*          m_groupTimeout;
	wxChoice*          m_callsignSwitch;
	wxChoice*          m_txMsgSwitch;
#if defined(DEXTRA_LINK) || defined(DCS_LINK)
	wxChoice*          m_reflector;
	wxChoice*          m_channel;
#endif
};

#endif
