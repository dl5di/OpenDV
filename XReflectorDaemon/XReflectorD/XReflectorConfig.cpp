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

#include "XReflectorConfig.h"
#include "XReflectorDefs.h"

const wxString  KEY_CALLSIGN      = wxT("callsign");
const wxString  KEY_ADDRESS       = wxT("address");
const wxString  KEY_DPLUS_ENABLED = wxT("dplusEnabled");
const wxString  KEY_LOG_ENABLED   = wxT("logEnabled");
const wxString  KEY_WINDOW_X      = wxT("windowX");
const wxString  KEY_WINDOW_Y      = wxT("windowY");

const wxString DEFAULT_CALLSIGN      = wxT("XRF999  ");
const wxString DEFAULT_ADDRESS       = wxEmptyString;
const bool     DEFAULT_DPLUS_ENABLED = false;
const bool     DEFAULT_LOG_ENABLED   = false;
const int      DEFAULT_WINDOW_X      = -1;
const int      DEFAULT_WINDOW_Y      = -1;


#if defined(__WINDOWS__)

CXReflectorConfig::CXReflectorConfig(wxConfigBase* config, const wxString& name) :
m_config(config),
m_name(wxT("/")),
m_callsign(DEFAULT_CALLSIGN),
m_address(DEFAULT_ADDRESS),
m_dplusEnabled(DEFAULT_DPLUS_ENABLED),
m_logEnabled(DEFAULT_LOG_ENABLED),
m_x(DEFAULT_WINDOW_X),
m_y(DEFAULT_WINDOW_Y)
{
	wxASSERT(config != NULL);

	if (!name.IsEmpty())
		m_name = wxT("/") + name + wxT("/");

	m_config->Read(m_name + KEY_CALLSIGN, &m_callsign, DEFAULT_CALLSIGN);
	m_config->Read(m_name + KEY_ADDRESS, &m_address, DEFAULT_ADDRESS);
	m_config->Read(m_name + KEY_DPLUS_ENABLED, &m_dplusEnabled, DEFAULT_DPLUS_ENABLED);
	m_config->Read(m_name + KEY_LOG_ENABLED, &m_logEnabled, DEFAULT_LOG_ENABLED);

	long temp;
	m_config->Read(m_name + KEY_WINDOW_X, &temp, long(DEFAULT_WINDOW_X));
	m_x = int(temp);

	m_config->Read(m_name + KEY_WINDOW_Y, &temp, long(DEFAULT_WINDOW_Y));
	m_y = int(temp);
}

CXReflectorConfig::~CXReflectorConfig()
{
	delete m_config;
}

#else

CXReflectorConfig::CXReflectorConfig(const wxString& dir, const wxString& name) :
m_fileName(),
m_callsign(DEFAULT_CALLSIGN),
m_address(DEFAULT_ADDRESS),
m_dplusEnabled(DEFAULT_DPLUS_ENABLED),
m_logEnabled(DEFAULT_LOG_ENABLED),
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

		if (key.IsSameAs(KEY_CALLSIGN)) {
			m_callsign = val;
		} else if (key.IsSameAs(KEY_ADDRESS)) {
			m_address = val;
		} else if (key.IsSameAs(KEY_DPLUS_ENABLED)) {
			val.ToLong(&temp);
			m_dplusEnabled = temp == 1L;
		} else if (key.IsSameAs(KEY_LOG_ENABLED)) {
			val.ToLong(&temp);
			m_logEnabled = temp == 1L;
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

CXReflectorConfig::~CXReflectorConfig()
{
}

#endif

void CXReflectorConfig::getReflector(wxString& callsign, wxString& address) const
{
	callsign = m_callsign;
	address  = m_address;
}

void CXReflectorConfig::setReflector(const wxString& callsign, const wxString& address)
{
	m_callsign = callsign;
	m_address  = address;
}

void CXReflectorConfig::getDPlus(bool& enabled) const
{
	enabled = m_dplusEnabled;
}

void CXReflectorConfig::setDPlus(bool enabled)
{
	m_dplusEnabled = enabled;
}

void CXReflectorConfig::getMiscellaneous(bool& enabled) const
{
	enabled = m_logEnabled;
}

void CXReflectorConfig::setMiscellaneous(bool enabled)
{
	m_logEnabled = enabled;
}

void CXReflectorConfig::getPosition(int& x, int& y) const
{
	x = m_x;
	y = m_y;
}

void CXReflectorConfig::setPosition(int x, int y)
{
	m_x = x;
	m_y = y;
}

#if defined(__WINDOWS__)

bool CXReflectorConfig::write()
{
	m_config->Write(m_name + KEY_CALLSIGN, m_callsign);
	m_config->Write(m_name + KEY_ADDRESS, m_address);
	m_config->Write(m_name + KEY_DPLUS_ENABLED, m_dplusEnabled);
	m_config->Write(m_name + KEY_LOG_ENABLED, m_logEnabled);
	m_config->Write(m_name + KEY_WINDOW_X, long(m_x));
	m_config->Write(m_name + KEY_WINDOW_Y, long(m_y));
	m_config->Flush();

	return true;
}

#else

bool CXReflectorConfig::write()
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
	buffer.Printf(wxT("%s=%s"), KEY_CALLSIGN.c_str(), m_callsign.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_ADDRESS.c_str(), m_address.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"), KEY_DPLUS_ENABLED.c_str(), m_dplusEnabled ? 1 : 0); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"), KEY_LOG_ENABLED.c_str(), m_logEnabled ? 1 : 0); file.AddLine(buffer);
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
