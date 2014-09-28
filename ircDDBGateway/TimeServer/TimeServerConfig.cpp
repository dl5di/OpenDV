/*
 *   Copyright (C) 2012 by Jonathan Naylor G4KLX
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

#include "TimeServerConfig.h"

const wxString KEY_CALLSIGN = wxT("callsign");
const wxString KEY_SENDA    = wxT("sendA");
const wxString KEY_SENDB    = wxT("sendB");
const wxString KEY_SENDC    = wxT("sendC");
const wxString KEY_SENDD    = wxT("sendD");
const wxString KEY_SENDE    = wxT("sendE");
const wxString KEY_ADDRESS  = wxT("address");
const wxString KEY_LANGUAGE = wxT("language");
const wxString KEY_FORMAT   = wxT("format");
const wxString KEY_INTERVAL = wxT("interval");
const wxString KEY_WINDOW_X = wxT("windowX");
const wxString KEY_WINDOW_Y = wxT("windowY");


const wxString DEFAULT_CALLSIGN = wxEmptyString;
const bool     DEFAULT_SENDA    = false;
const bool     DEFAULT_SENDB    = false;
const bool     DEFAULT_SENDC    = false;
const bool     DEFAULT_SENDD    = false;
const bool     DEFAULT_SENDE    = false;
const wxString DEFAULT_ADDRESS  = wxT("127.0.0.1");
const LANGUAGE DEFAULT_LANGUAGE = LANG_ENGLISH_UK_1;
const FORMAT   DEFAULT_FORMAT   = FORMAT_VOICE_TIME;
const INTERVAL DEFAULT_INTERVAL = INTERVAL_15MINS;
const int      DEFAULT_WINDOW_X = -1;
const int      DEFAULT_WINDOW_Y = -1;


#if defined(__WINDOWS__)

CTimeServerConfig::CTimeServerConfig(wxConfigBase* config, const wxString& name) :
m_config(config),
m_name(wxT("/")),
m_callsign(DEFAULT_CALLSIGN),
m_sendA(DEFAULT_SENDA),
m_sendB(DEFAULT_SENDB),
m_sendC(DEFAULT_SENDC),
m_sendD(DEFAULT_SENDD),
m_sendE(DEFAULT_SENDE),
m_address(DEFAULT_ADDRESS),
m_language(DEFAULT_LANGUAGE),
m_format(DEFAULT_FORMAT),
m_interval(DEFAULT_INTERVAL),
m_x(DEFAULT_WINDOW_X),
m_y(DEFAULT_WINDOW_Y)
{
	wxASSERT(config != NULL);

	if (!name.IsEmpty())
		m_name = wxT("/") + name + wxT("/");

	m_config->Read(m_name + KEY_CALLSIGN, &m_callsign, DEFAULT_CALLSIGN);

	m_config->Read(m_name + KEY_SENDA, &m_sendA, DEFAULT_SENDA);

	m_config->Read(m_name + KEY_SENDB, &m_sendB, DEFAULT_SENDB);

	m_config->Read(m_name + KEY_SENDC, &m_sendC, DEFAULT_SENDC);

	m_config->Read(m_name + KEY_SENDD, &m_sendD, DEFAULT_SENDD);

	m_config->Read(m_name + KEY_SENDE, &m_sendE, DEFAULT_SENDE);

	m_config->Read(m_name + KEY_ADDRESS, &m_address, DEFAULT_ADDRESS);

	long temp;
	m_config->Read(m_name + KEY_LANGUAGE, &temp, long(DEFAULT_LANGUAGE));
	m_language = LANGUAGE(temp);

	m_config->Read(m_name + KEY_FORMAT, &temp, long(DEFAULT_FORMAT));
	m_format = FORMAT(temp);

	m_config->Read(m_name + KEY_INTERVAL, &temp, long(DEFAULT_INTERVAL));
	m_interval = INTERVAL(temp);

	m_config->Read(m_name + KEY_WINDOW_X, &temp, long(DEFAULT_WINDOW_X));
	m_x = int(temp);

	m_config->Read(m_name + KEY_WINDOW_Y, &temp, long(DEFAULT_WINDOW_Y));
	m_y = int(temp);
}

CTimeServerConfig::~CTimeServerConfig()
{
	delete m_config;
}

#else

CTimeServerConfig::CTimeServerConfig(const wxString& dir, const wxString& name) :
m_fileName(),
m_callsign(DEFAULT_CALLSIGN),
m_sendA(DEFAULT_SENDA),
m_sendB(DEFAULT_SENDB),
m_sendC(DEFAULT_SENDC),
m_sendD(DEFAULT_SENDD),
m_sendE(DEFAULT_SENDE),
m_address(DEFAULT_ADDRESS),
m_language(DEFAULT_LANGUAGE),
m_format(DEFAULT_FORMAT),
m_interval(DEFAULT_INTERVAL),
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
		} else if (key.IsSameAs(KEY_SENDA)) {
			val.ToLong(&temp);
			m_sendA = temp == 1L;
		} else if (key.IsSameAs(KEY_SENDB)) {
			val.ToLong(&temp);
			m_sendB = temp == 1L;
		} else if (key.IsSameAs(KEY_SENDC)) {
			val.ToLong(&temp);
			m_sendC = temp == 1L;
		} else if (key.IsSameAs(KEY_SENDD)) {
			val.ToLong(&temp);
			m_sendD = temp == 1L;
		} else if (key.IsSameAs(KEY_SENDE)) {
			val.ToLong(&temp);
			m_sendE = temp == 1L;
		} else if (key.IsSameAs(KEY_ADDRESS)) {
			m_address = val;
		} else if (key.IsSameAs(KEY_LANGUAGE)) {
			val.ToLong(&temp);
			m_language = LANGUAGE(temp);
		} else if (key.IsSameAs(KEY_FORMAT)) {
			val.ToLong(&temp);
			m_format = FORMAT(temp);
		} else if (key.IsSameAs(KEY_INTERVAL)) {
			val.ToLong(&temp);
			m_interval = INTERVAL(temp);
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

CTimeServerConfig::~CTimeServerConfig()
{
}

#endif

void CTimeServerConfig::getGateway(wxString& callsign, bool& sendA, bool& sendB, bool& sendC, bool& sendD, bool& sendE, wxString& address) const
{
	callsign  = m_callsign;
	sendA     = m_sendA;
	sendB     = m_sendB;
	sendC     = m_sendC;
	sendD     = m_sendD;
	sendE     = m_sendE;
	address   = m_address;
}

void CTimeServerConfig::setGateway(const wxString& callsign, bool sendA, bool sendB, bool sendC, bool sendD, bool sendE, const wxString& address)
{
	m_callsign  = callsign;
	m_sendA     = sendA;
	m_sendB     = sendB;
	m_sendC     = sendC;
	m_sendD     = sendD;
	m_sendE     = sendE;
	m_address   = address;
}

void CTimeServerConfig::getAnnouncements(LANGUAGE& language, FORMAT& format, INTERVAL& interval) const
{
	language  = m_language;
	format    = m_format;
	interval  = m_interval;
}

void CTimeServerConfig::setAnnouncements(LANGUAGE language, FORMAT format, INTERVAL interval)
{
	m_language  = language;
	m_format    = format;
	m_interval  = interval;
}

void CTimeServerConfig::getPosition(int& x, int& y) const
{
	x = m_x;
	y = m_y;
}

void CTimeServerConfig::setPosition(int x, int y)
{
	m_x = x;
	m_y = y;
}

#if defined(__WINDOWS__)

bool CTimeServerConfig::write()
{
	m_config->Write(m_name + KEY_CALLSIGN, m_callsign);
	m_config->Write(m_name + KEY_SENDA, m_sendA);
	m_config->Write(m_name + KEY_SENDB, m_sendB);
	m_config->Write(m_name + KEY_SENDC, m_sendC);
	m_config->Write(m_name + KEY_SENDD, m_sendD);
	m_config->Write(m_name + KEY_SENDE, m_sendE);
	m_config->Write(m_name + KEY_ADDRESS, m_address);
	m_config->Write(m_name + KEY_LANGUAGE, long(m_language));
	m_config->Write(m_name + KEY_FORMAT,   long(m_format));
	m_config->Write(m_name + KEY_INTERVAL, long(m_interval));
	m_config->Write(m_name + KEY_WINDOW_X, long(m_x));
	m_config->Write(m_name + KEY_WINDOW_Y, long(m_y));
	m_config->Flush();

	return true;
}

#else

bool CTimeServerConfig::write()
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
	buffer.Printf(wxT("%s=%d"), KEY_SENDA.c_str(), m_sendA ? 1 : 0); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"), KEY_SENDB.c_str(), m_sendB ? 1 : 0); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"), KEY_SENDC.c_str(), m_sendC ? 1 : 0); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"), KEY_SENDD.c_str(), m_sendD ? 1 : 0); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"), KEY_SENDE.c_str(), m_sendE ? 1 : 0); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_ADDRESS.c_str(), m_address.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"), KEY_LANGUAGE.c_str(), int(m_language)); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"), KEY_FORMAT.c_str(),   int(m_format)); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"), KEY_INTERVAL.c_str(), int(m_interval)); file.AddLine(buffer);
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
