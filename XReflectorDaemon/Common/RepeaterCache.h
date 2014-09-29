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

#ifndef RepeaterCache_H
#define	RepeaterCache_H

#include <wx/wx.h>
#include <wx/ffile.h>

class CRepeaterRecord {
public:
	CRepeaterRecord(const wxString& repeater, const wxString& gateway) :
	m_repeater(repeater),
	m_gateway(gateway)
	{
	}

	wxString getRepeater() const
	{
		return m_repeater;
	}

	wxString getGateway() const
	{
		return m_gateway;
	}

	void setGateway(const wxString& gateway)
	{
		m_gateway = gateway;
	}

private:
	wxString m_repeater;
	wxString m_gateway;
};

WX_DECLARE_STRING_HASH_MAP(CRepeaterRecord*, CRepeaterCache_t);

class CRepeaterCache {
public:
	CRepeaterCache();
	~CRepeaterCache();

	CRepeaterRecord* find(const wxString& repeater);

	void update(const wxString& repeater, const wxString& gateway);

	unsigned int getCount() const;

private:
	CRepeaterCache_t m_cache;
};

#endif
