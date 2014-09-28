/*
 *   Copyright (C) 2011 by Jonathan Naylor G4KLX
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

#ifndef	RemoteSet_H
#define	RemoteSet_H

#include "PortTextCtrl.h"

#include <wx/wx.h>

class CRemoteSet : public wxPanel {
public:
	CRemoteSet(wxWindow* parent, int id, const wxString& title, bool enabled, const wxString& password, unsigned int port);
	virtual ~CRemoteSet();

	virtual bool Validate();

	virtual bool         getEnabled() const;
	virtual wxString     getPassword() const;
	virtual unsigned int getPort() const;

private:
	wxString       m_title;
	wxChoice*      m_enabled;
	wxTextCtrl*    m_password;
	CPortTextCtrl* m_port;
};

#endif
