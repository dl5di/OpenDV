/*
 *   Copyright (C) 2010 by Jonathan Naylor G4KLX
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

#ifndef UserCache_H
#define	UserCache_H

#include <wx/wx.h>
#include <wx/ffile.h>

class CUserRecord {
public:
	CUserRecord(const wxString& user, const wxString& repeater) :
	m_user(user),
	m_repeater(repeater)
	{
	}

	wxString getUser() const
	{
		return m_user;
	}

	wxString getRepeater() const
	{
		return m_repeater;
	}

	void setRepeater(const wxString& repeater)
	{
		m_repeater = repeater;
	}

private:
	wxString m_user;
	wxString m_repeater;
};

WX_DECLARE_STRING_HASH_MAP(CUserRecord*, CUserCache_t);

class CUserCache {
public:
	CUserCache();
	~CUserCache();

	CUserRecord* find(const wxString& user);

	void update(const wxString& user, const wxString& repeater);

	unsigned int getCount() const;

private:
	CUserCache_t m_cache;
};

#endif
