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

#ifndef	RepeaterInfoSet_H
#define	RepeaterInfoSet_H

#include <wx/wx.h>

class CRepeaterInfoSet : public wxPanel {
public:
	CRepeaterInfoSet(wxWindow* parent, int id, const wxString& title, double frequency, double offset, double range, double latitude, double longitude, double agl, const wxString& description1, const wxString& description2, const wxString& url);
	virtual ~CRepeaterInfoSet();

	virtual bool Validate();

	virtual double       getFrequency() const;
	virtual double       getOffset() const;
	virtual double       getRange() const;

	virtual double       getLatitude() const;
	virtual double       getLongitude() const;
	virtual double       getAGL() const;

	virtual wxString     getDescription1() const;
	virtual wxString     getDescription2() const;
	virtual wxString     getURL() const;

private:
	wxString           m_title;
	wxTextCtrl*        m_frequency;
	wxTextCtrl*        m_offset;
	wxTextCtrl*        m_range;
	wxTextCtrl*        m_latitude;
	wxTextCtrl*        m_longitude;
	wxTextCtrl*        m_agl;
	wxTextCtrl*        m_description1;
	wxTextCtrl*        m_description2;
	wxTextCtrl*        m_url;
};

#endif
