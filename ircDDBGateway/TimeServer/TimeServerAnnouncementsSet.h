/*
 *   Copyright (C) 2012 by Jonathan Naylor G4KLX
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

#ifndef	TimeServerAnnouncementsSet_H
#define	TimeServerAnnouncementsSet_H

#include "TimeServerDefs.h"

#include <wx/wx.h>

class CTimeServerAnnouncementsSet : public wxPanel {
public:
	CTimeServerAnnouncementsSet(wxWindow* parent, int id, const wxString& title, LANGUAGE language, FORMAT format, INTERVAL interval);
	virtual ~CTimeServerAnnouncementsSet();

	virtual bool Validate();

	virtual LANGUAGE getLanguage() const;

	virtual FORMAT   getFormat() const;

	virtual INTERVAL getInterval() const;

private:
	wxString  m_title;
	wxChoice* m_language;
	wxChoice* m_format;
	wxChoice* m_interval;
};

#endif
