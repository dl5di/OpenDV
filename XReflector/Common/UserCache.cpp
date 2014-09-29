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

#include "UserCache.h"

const unsigned int CACHE_SIZE = 1000U;

CUserCache::CUserCache() :
m_cache(CACHE_SIZE)
{
}

CUserCache::~CUserCache()
{
	for (CUserCache_t::iterator it = m_cache.begin(); it != m_cache.end(); ++it)
		delete it->second;
}

CUserRecord* CUserCache::find(const wxString& user)
{
	return m_cache[user];
}

void CUserCache::update(const wxString& user, const wxString& repeater)
{
	CUserRecord* rec = m_cache[user];

	if (rec == NULL)
		// A brand new record is needed
		m_cache[user] = new CUserRecord(user, repeater);
	else
		// Update an existing record
		rec->setRepeater(repeater);
}

unsigned int CUserCache::getCount() const
{
	return m_cache.size();
}
