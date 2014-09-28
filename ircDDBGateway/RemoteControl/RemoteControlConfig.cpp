/*
 *   Copyright (C) 2011,2012 by Jonathan Naylor G4KLX
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

#include "RemoteControlConfig.h"

const wxString  KEY_ADDRESS  = wxT("/address");
const wxString  KEY_PORT     = wxT("/port");
const wxString  KEY_PASSWORD = wxT("/password");
const wxString  KEY_WINDOW_X = wxT("/windowX");
const wxString  KEY_WINDOW_Y = wxT("/windowY");

const wxString  DEFAULT_ADDRESS  = wxEmptyString;
const long      DEFAULT_PORT     = 0L;
const wxString  DEFAULT_PASSWORD = wxEmptyString;
const long      DEFAULT_WINDOW_X = -1L;
const long      DEFAULT_WINDOW_Y = -1L;


CRemoteControlConfig::CRemoteControlConfig(wxConfigBase* config, const wxString& name) :
m_config(config),
m_name()
{
	wxASSERT(config != NULL);

	if (!name.IsEmpty())
		m_name = wxT("/") + name;
}

CRemoteControlConfig::~CRemoteControlConfig()
{
	delete m_config;
}

void CRemoteControlConfig::getConfig(wxString& address, unsigned int& port, wxString& password) const
{
	wxString addressKey  = m_name + KEY_ADDRESS;
	wxString portKey     = m_name + KEY_PORT;
	wxString passwordKey = m_name + KEY_PASSWORD;

	m_config->Read(addressKey,  &address,  DEFAULT_ADDRESS);

	long temp;
	m_config->Read(portKey,     &temp,     DEFAULT_PORT);
	port = (unsigned int)temp;

	m_config->Read(passwordKey, &password, DEFAULT_PASSWORD);
}

void CRemoteControlConfig::setConfig(const wxString& address, unsigned int port, const wxString& password) const
{
	wxString addressKey  = m_name + KEY_ADDRESS;
	wxString portKey     = m_name + KEY_PORT;
	wxString passwordKey = m_name + KEY_PASSWORD;

	m_config->Write(addressKey,  address);
	m_config->Write(portKey,     long(port));
	m_config->Write(passwordKey, password);
	m_config->Flush();
}


void CRemoteControlConfig::getPosition(int& x, int& y) const
{
	wxString xKey = m_name + KEY_WINDOW_X;
	wxString yKey = m_name + KEY_WINDOW_Y;

	long temp;
	m_config->Read(xKey, &temp, DEFAULT_WINDOW_X);
	x = (unsigned int)temp;

	m_config->Read(yKey, &temp, DEFAULT_WINDOW_Y);
	y = (unsigned int)temp;
}

void CRemoteControlConfig::setPosition(int x, int y)
{
	wxString xKey = m_name + KEY_WINDOW_X;
	wxString yKey = m_name + KEY_WINDOW_Y;

	m_config->Write(xKey, long(x));
	m_config->Write(yKey, long(y));
	m_config->Flush();
}
