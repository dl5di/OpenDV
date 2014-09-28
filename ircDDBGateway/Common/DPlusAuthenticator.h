/*
 *   Copyright (C) 2010-2013 by Jonathan Naylor G4KLX
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

#ifndef	DPlusAuthenticator_H
#define	DPlusAuthenticator_H

#include "TCPReaderWriterClient.h"
#include "CacheManager.h"
#include "Timer.h"

#include <wx/wx.h>

#if defined(__WINDOWS__)
#include "Inaddr.h"
#else
#include <netinet/in.h>
#endif

class CDPlusAuthenticator : public wxThread {
public:
	CDPlusAuthenticator(const wxString& loginCallsign, const wxString& gatewayCallsign, const wxString& address, CCacheManager* cache);
	virtual ~CDPlusAuthenticator();

	virtual void  start();

	virtual void* Entry();

	virtual void stop();

private:
	wxString       m_loginCallsign;
	wxString       m_gatewayCallsign;
	wxString       m_address;
	CCacheManager* m_cache;
	CTimer         m_timer;
	CTimer         m_pollTimer;
	bool           m_killed;

	bool poll(const wxString& callsign, const wxString& hostname, unsigned int port, unsigned char id);
	bool authenticate(const wxString& callsign, const wxString& hostname, unsigned int port, unsigned char id, bool writeToCache);
	bool read(CTCPReaderWriterClient& socket, unsigned char* buffer, unsigned int len) const;
};

#endif
