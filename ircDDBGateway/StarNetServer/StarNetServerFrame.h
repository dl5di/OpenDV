/*
 *   Copyright (C) 2010,2011,2012 by Jonathan Naylor G4KLX
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

#ifndef	StarNetServerFrame_H
#define	StarNetServerFrame_H

#include "Defs.h"

#include <wx/wx.h>
#include <wx/tglbtn.h>

class CStarNetServerFrame : public wxFrame {
public:
	CStarNetServerFrame(const wxString& title, const wxPoint& position, bool gui);
	virtual ~CStarNetServerFrame();

	virtual void onQuit(wxCommandEvent& event);
	virtual void onPreferences(wxCommandEvent& event);
	virtual void onUpdates(wxCommandEvent& event);
	virtual void onAbout(wxCommandEvent& event);
	virtual void onClose(wxCloseEvent& event);
	virtual void onLog(wxEvent& event);

	virtual void showLog(const wxString& text);

private:
	wxStaticText* m_logLine[20];
	bool          m_updates;

	DECLARE_EVENT_TABLE()

	wxMenuBar* createMenuBar();
};

#endif
