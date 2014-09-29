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

#include "CacheManager.h"
#include "DStarDefines.h"

CCacheManager::CCacheManager() :
m_mutex(),
m_userCache(),
m_gatewayCache(),
m_repeaterCache()
{
}

CCacheManager::~CCacheManager()
{
}

CUserData* CCacheManager::findUser(const wxString& user)
{
	wxMutexLocker locker(m_mutex);

	CUserRecord* ur = m_userCache.find(user);
	if (ur == NULL)
		return NULL;

	CRepeaterRecord* rr = m_repeaterCache.find(ur->getRepeater());
	wxString gateway;
	if (rr == NULL) {
		gateway = ur->getRepeater();
		gateway.Append(wxT("        "));
		gateway.Truncate(LONG_CALLSIGN_LENGTH - 1U);
		gateway.Append(wxT("G"));
	} else {
		gateway = rr->getGateway();
	}

	CGatewayRecord* gr = m_gatewayCache.find(gateway);
	if (gr == NULL)
		return NULL;

	return new CUserData(user, ur->getRepeater(), gr->getGateway(), gr->getAddress());
}

CGatewayData* CCacheManager::findGateway(const wxString& gateway)
{
	wxMutexLocker locker(m_mutex);

	CGatewayRecord* gr = m_gatewayCache.find(gateway);
	if (gr == NULL)
		return NULL;

	return new CGatewayData(gateway, gr->getAddress(), gr->getProtocol());
}

CRepeaterData* CCacheManager::findRepeater(const wxString& repeater)
{
	wxMutexLocker locker(m_mutex);

	CRepeaterRecord* rr = m_repeaterCache.find(repeater);
	wxString gateway;
	if (rr == NULL) {
		gateway = repeater;
		gateway.Append(wxT("        "));
		gateway.Truncate(LONG_CALLSIGN_LENGTH - 1U);
		gateway.Append(wxT("G"));
	} else {
		gateway = rr->getGateway();
	}

	CGatewayRecord* gr = m_gatewayCache.find(gateway);
	if (gr == NULL)
		return NULL;

	return new CRepeaterData(repeater, gr->getGateway(), gr->getAddress(), gr->getProtocol());
}

void CCacheManager::updateUser(const wxString& user, const wxString& repeater, const wxString& gateway, const wxString& address, DSTAR_PROTOCOL protocol, bool addrLock, bool protoLock)
{
	wxMutexLocker locker(m_mutex);

	wxString repeater7 = repeater.Left(LONG_CALLSIGN_LENGTH - 1U);
	wxString gateway7  = gateway.Left(LONG_CALLSIGN_LENGTH - 1U);

	m_userCache.update(user, repeater);

	// Only store non-standard repeater-gateway pairs
	if (!repeater7.IsSameAs(gateway7))
		m_repeaterCache.update(repeater, gateway);

	m_gatewayCache.update(gateway, address, protocol, addrLock, protoLock);
}

void CCacheManager::updateRepeater(const wxString& repeater, const wxString& gateway, const wxString& address, DSTAR_PROTOCOL protocol, bool addrLock, bool protoLock)
{
	wxMutexLocker locker(m_mutex);

	wxString repeater7 = repeater.Left(LONG_CALLSIGN_LENGTH - 1U);
	wxString gateway7  = gateway.Left(LONG_CALLSIGN_LENGTH - 1U);

	// Only store non-standard repeater-gateway pairs
	if (!repeater7.IsSameAs(gateway7))
		m_repeaterCache.update(repeater, gateway);

	m_gatewayCache.update(gateway, address, protocol, addrLock, protoLock);
}

void CCacheManager::updateGateway(const wxString& gateway, const wxString& address, DSTAR_PROTOCOL protocol, bool addrLock, bool protoLock)
{
	wxMutexLocker locker(m_mutex);

	m_gatewayCache.update(gateway, address, protocol, addrLock, protoLock);
}
