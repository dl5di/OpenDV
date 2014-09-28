/*
 *   Copyright (C) 2013 by Jonathan Naylor G4KLX
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

#ifndef	CCSData_H
#define	CCSData_H

#include <wx/wx.h>

#if defined(__WINDOWS__)
#include "Inaddr.h"
#else
#include <netinet/in.h>
#endif

enum CC_TYPE {
	CT_TERMINATE,
	CT_DTMFNOTFOUND,
	CT_DTMFFOUND,
	CT_INFO
};

class CCCSData {
public:
	CCCSData(const wxString& local, double latitude, double longitude, double frequency, double offset, const wxString& description1, const wxString& description2, const wxString& url, CC_TYPE type);
	CCCSData(const wxString& local, const wxString& remote, CC_TYPE type);
	CCCSData();
	~CCCSData();

	bool setCCSData(const unsigned char* data, unsigned int length, const in_addr& yourAddress, unsigned int yourPort, unsigned int myPort);

	unsigned int getCCSData(unsigned char* data, unsigned int length) const;

	void setDestination(const in_addr& address, unsigned int port);

	wxString     getLocal() const;
	wxString     getRemote() const;
	CC_TYPE      getType() const;

	in_addr      getYourAddress() const;
	unsigned int getYourPort() const;
	unsigned int getMyPort() const;

private:
	wxString     m_local;
	wxString     m_remote;
	double       m_latitude;
	double       m_longitude;
	double       m_frequency;
	double       m_offset;
	wxString     m_description1;
	wxString     m_description2;
	wxString     m_url;
	CC_TYPE      m_type;
	in_addr      m_yourAddress;
	unsigned int m_yourPort;
	unsigned int m_myPort;
};

#endif
