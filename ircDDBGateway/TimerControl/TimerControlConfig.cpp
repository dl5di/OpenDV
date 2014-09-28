/*
 *   Copyright (C) 2011,2012,2013 by Jonathan Naylor G4KLX
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

#include "TimerControlConfig.h"
#include "TimerControlDefs.h"

const wxString  KEY_ADDRESS  = wxT("address");
const wxString  KEY_PORT     = wxT("port");
const wxString  KEY_PASSWORD = wxT("password");
const wxString  KEY_DELAY    = wxT("delay");
const wxString  KEY_WINDOW_X = wxT("windowX");
const wxString  KEY_WINDOW_Y = wxT("windowY");

const wxString  DEFAULT_ADDRESS  = wxEmptyString;
const long      DEFAULT_PORT     = 0L;
const wxString  DEFAULT_PASSWORD = wxEmptyString;
const bool      DEFAULT_DELAY    = false;
const long      DEFAULT_WINDOW_X = -1L;
const long      DEFAULT_WINDOW_Y = -1L;


#if defined(__WINDOWS__)

CTimerControlConfig::CTimerControlConfig(wxConfigBase* config, const wxString& name) :
m_config(config),
m_name(wxT("/")),
m_address(DEFAULT_ADDRESS),
m_port(DEFAULT_PORT),
m_password(DEFAULT_PASSWORD),
m_delay(DEFAULT_DELAY),
m_x(DEFAULT_WINDOW_X),
m_y(DEFAULT_WINDOW_Y)
{
	wxASSERT(config != NULL);

	if (!name.IsEmpty())
		m_name = wxT("/") + name;

	m_config->Read(m_name + KEY_ADDRESS, &m_address, DEFAULT_ADDRESS);

	long temp;
	m_config->Read(m_name + KEY_PORT, &temp, DEFAULT_PORT);
	m_port = (unsigned int)temp;

	m_config->Read(m_name + KEY_PASSWORD, &m_password, DEFAULT_PASSWORD);

	m_config->Read(m_name + KEY_DELAY, &m_delay, DEFAULT_DELAY);

	m_config->Read(m_name + KEY_WINDOW_X, &temp, DEFAULT_WINDOW_X);
	m_x = (unsigned int)temp;

	m_config->Read(m_name + KEY_WINDOW_Y, &temp, DEFAULT_WINDOW_Y);
	m_y = (unsigned int)temp;
}

CTimerControlConfig::~CTimerControlConfig()
{
	delete m_config;
}

#else

CTimerControlConfig::CTimerControlConfig(const wxString& dir, const wxString& name) :
m_fileName(),
m_address(DEFAULT_ADDRESS),
m_port(DEFAULT_PORT),
m_password(DEFAULT_PASSWORD),
m_delay(DEFAULT_DELAY),
m_x(DEFAULT_WINDOW_X),
m_y(DEFAULT_WINDOW_Y)
{
	wxASSERT(!dir.IsEmpty());

	wxString fileName = CONFIG_FILE_NAME;
	if (!name.IsEmpty())
		fileName = CONFIG_FILE_NAME + wxT("_") + name;

	m_fileName.Assign(dir, fileName);

	wxTextFile file(m_fileName.GetFullPath());

	bool exists = file.Exists();
	if (!exists)
		return;

	bool ret = file.Open();
	if (!ret) {
		wxLogError(wxT("Cannot open the config file - %s"), m_fileName.GetFullPath().c_str());
		return;
	}

	long temp;

	wxString str = file.GetFirstLine();

	while (!file.Eof()) {
		if (str.GetChar(0U) == wxT('#')) {
			str = file.GetNextLine();
			continue;
		}

		int n = str.Find(wxT('='));
		if (n == wxNOT_FOUND) {
			str = file.GetNextLine();
			continue;
		}

		wxString key = str.Left(n);
		wxString val = str.Mid(n + 1U);

		if (key.IsSameAs(KEY_ADDRESS)) {
			m_address = val;
		} else if (key.IsSameAs(KEY_PORT)) {
			val.ToLong(&temp);
			m_port = (unsigned int)temp;
		} else if (key.IsSameAs(KEY_PASSWORD)) {
			m_password = val;
		} else if (key.IsSameAs(KEY_DELAY)) {
			val.ToLong(&temp);
			m_delay = temp == 1L;
		} else if (key.IsSameAs(KEY_WINDOW_X)) {
			val.ToLong(&temp);
			m_x = int(temp);
		} else if (key.IsSameAs(KEY_WINDOW_Y)) {
			val.ToLong(&temp);
			m_y = int(temp);
		}

		str = file.GetNextLine();
	}

	file.Close();
}

CTimerControlConfig::~CTimerControlConfig()
{
}

#endif

void CTimerControlConfig::getGateway(wxString& address, unsigned int& port, wxString& password) const
{
	address  = m_address;
	port     = m_port;
	password = m_password;
}

void CTimerControlConfig::setGateway(const wxString& address, unsigned int port, const wxString& password)
{
	m_address  = address;
	m_port     = port;
	m_password = password;
}

void CTimerControlConfig::getDelay(bool& delay) const
{
	delay = m_delay;
}

void CTimerControlConfig::setDelay(bool delay)
{
	m_delay = delay;
}

void CTimerControlConfig::getPosition(int& x, int& y) const
{
	x = m_x;
	y = m_y;
}

void CTimerControlConfig::setPosition(int x, int y)
{
	m_x = x;
	m_y = y;
}

#if defined(__WINDOWS__)

bool CTimerControlConfig::write()
{
	m_config->Write(m_name + KEY_ADDRESS, m_address);
	m_config->Write(m_name + KEY_PORT, long(m_port));
	m_config->Write(m_name + KEY_PASSWORD, m_password);
	m_config->Write(m_name + KEY_DELAY, m_delay);
	m_config->Write(m_name + KEY_WINDOW_X, long(m_x));
	m_config->Write(m_name + KEY_WINDOW_Y, long(m_y));
	m_config->Flush();

	return true;
}

#else

bool CTimerControlConfig::write()
{
	wxTextFile file(m_fileName.GetFullPath());

	bool exists = file.Exists();
	if (exists) {
		bool ret = file.Open();
		if (!ret) {
			wxLogError(wxT("Cannot open the config file - %s"), m_fileName.GetFullPath().c_str());
			return false;
		}

		// Remove the existing file entries
		file.Clear();
	} else {
		bool ret = file.Create();
		if (!ret) {
			wxLogError(wxT("Cannot create the config file - %s"), m_fileName.GetFullPath().c_str());
			return false;
		}
	}

	wxString buffer;
	buffer.Printf(wxT("%s=%s"), KEY_ADDRESS.c_str(), m_address.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%u"), KEY_PORT.c_str(), m_port); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_PASSWORD.c_str(), m_password.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"), KEY_DELAY.c_str(), m_delay ? 1 : 0); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"), KEY_WINDOW_X.c_str(), m_x); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"), KEY_WINDOW_Y.c_str(), m_y); file.AddLine(buffer);

	bool ret = file.Write();
	if (!ret) {
		file.Close();
		wxLogError(wxT("Cannot write the config file - %s"), m_fileName.GetFullPath().c_str());
		return false;
	}

	file.Close();

	return true;
}

#endif
