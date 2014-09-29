/*
 *   Copyright (C) 2010,2012 by Jonathan Naylor G4KLX
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

#include "DExtraGatewayConfig.h"

const wxString KEY_CALLSIGN          = wxT("callsign");
const wxString KEY_REFLECTOR         = wxT("reflector");
const wxString KEY_ATSTARTUP         = wxT("atStartup");
const wxString KEY_RECONNECT         = wxT("reconnect");
const wxString KEY_LANGUAGE          = wxT("language");
const wxString KEY_REPEATER_CALLSIGN = wxT("repeaterCallsign");
const wxString KEY_REPEATER_ADDRESS  = wxT("repeaterAddress");
const wxString KEY_REPEATER_PORT     = wxT("repeaterPort");
const wxString KEY_LOCAL_ADDRESS     = wxT("localAddress");
const wxString KEY_LOCAL_PORT        = wxT("localPort");
const wxString KEY_WINDOW_X          = wxT("windowX");
const wxString KEY_WINDOW_Y          = wxT("windowY");


const wxString     DEFAULT_CALLSIGN          = wxEmptyString;
const wxString     DEFAULT_REFLECTOR         = wxEmptyString;
const bool         DEFAULT_ATSTARTUP         = false;
const RECONNECT    DEFAULT_RECONNECT         = RECONNECT_FIXED;
const TEXT_LANG    DEFAULT_LANGUAGE          = TL_ENGLISH_UK;
const wxString     DEFAULT_REPEATER_CALLSIGN = wxEmptyString;
const wxString     DEFAULT_REPEATER_ADDRESS  = wxT("127.0.0.1");
const unsigned int DEFAULT_REPEATER_PORT     = 20011L;
const wxString     DEFAULT_LOCAL_ADDRESS     = wxT("127.0.0.1");
const unsigned int DEFAULT_LOCAL_PORT        = 20010L;
const int          DEFAULT_WINDOW_X          = -1;
const int          DEFAULT_WINDOW_Y          = -1;


#if defined(__WINDOWS__)

CDExtraGatewayConfig::CDExtraGatewayConfig(wxConfigBase* config, const wxString& name) :
m_config(config),
m_name(wxT("/")),
m_callsign(DEFAULT_CALLSIGN),
m_reflector(DEFAULT_REFLECTOR),
m_atStartup(DEFAULT_ATSTARTUP),
m_reconnect(DEFAULT_RECONNECT),
m_language(DEFAULT_LANGUAGE),
m_repeaterCallsign(DEFAULT_REPEATER_CALLSIGN),
m_repeaterAddress(DEFAULT_REPEATER_ADDRESS),
m_repeaterPort(DEFAULT_REPEATER_PORT),
m_localAddress(DEFAULT_LOCAL_ADDRESS),
m_localPort(DEFAULT_LOCAL_PORT),
m_x(DEFAULT_WINDOW_X),
m_y(DEFAULT_WINDOW_Y)
{
	wxASSERT(config != NULL);

	if (!name.IsEmpty())
		m_name = wxT("/") + name + wxT("/");

	m_config->Read(m_name + KEY_CALLSIGN, &m_callsign, DEFAULT_CALLSIGN);

	m_config->Read(m_name + KEY_REFLECTOR, &m_reflector, DEFAULT_REFLECTOR);

	m_config->Read(m_name + KEY_ATSTARTUP, &m_atStartup, DEFAULT_ATSTARTUP);

	long temp;
	m_config->Read(m_name + KEY_RECONNECT, &temp, long(DEFAULT_RECONNECT));
	m_reconnect = RECONNECT(temp);

	m_config->Read(m_name + KEY_LANGUAGE, &temp, long(DEFAULT_LANGUAGE));
	m_language = TEXT_LANG(temp);

	m_config->Read(m_name + KEY_REPEATER_CALLSIGN, &m_repeaterCallsign, DEFAULT_REPEATER_CALLSIGN);

	m_config->Read(m_name + KEY_REPEATER_ADDRESS, &m_repeaterAddress, DEFAULT_REPEATER_ADDRESS);

	m_config->Read(m_name + KEY_REPEATER_PORT, &temp, long(DEFAULT_REPEATER_PORT));
	m_repeaterPort = (unsigned int)temp;

	m_config->Read(m_name + KEY_LOCAL_ADDRESS, &m_localAddress, DEFAULT_LOCAL_ADDRESS);

	m_config->Read(m_name + KEY_LOCAL_PORT, &temp, long(DEFAULT_LOCAL_PORT));
	m_localPort = (unsigned int)temp;

	m_config->Read(m_name + KEY_WINDOW_X, &temp, long(DEFAULT_WINDOW_X));
	m_x = int(temp);

	m_config->Read(m_name + KEY_WINDOW_Y, &temp, long(DEFAULT_WINDOW_Y));
	m_y = int(temp);
}

CDExtraGatewayConfig::~CDExtraGatewayConfig()
{
	delete m_config;
}

#else

CDExtraGatewayConfig::CDExtraGatewayConfig(const wxString& dir, const wxString& name) :
m_fileName(),
m_callsign(DEFAULT_CALLSIGN),
m_reflector(DEFAULT_REFLECTOR),
m_atStartup(DEFAULT_ATSTARTUP),
m_reconnect(DEFAULT_RECONNECT),
m_language(DEFAULT_LANGUAGE),
m_repeaterCallsign(DEFAULT_REPEATER_CALLSIGN),
m_repeaterAddress(DEFAULT_REPEATER_ADDRESS),
m_repeaterPort(DEFAULT_REPEATER_PORT),
m_localAddress(DEFAULT_LOCAL_ADDRESS),
m_localPort(DEFAULT_LOCAL_PORT),
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

	long temp1;
	unsigned long temp2;

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
		} else if (key.IsSameAs(KEY_REFLECTOR)) {
			m_reflector = val;
		} else if (key.IsSameAs(KEY_ATSTARTUP)) {
			val.ToLong(&temp1);
			m_atStartup = temp1 == 1L;
		} else if (key.IsSameAs(KEY_RECONNECT)) {
			val.ToLong(&temp1);
			m_reconnect = RECONNECT(temp1);
		} else if (key.IsSameAs(KEY_LANGUAGE)) {
			val.ToLong(&temp1);
			m_language = TEXT_LANG(temp1);
		} else if (key.IsSameAs(KEY_REPEATER_CALLSIGN)) {
			m_repeaterCallsign = val;
		} else if (key.IsSameAs(KEY_REPEATER_ADDRESS)) {
			m_repeaterAddress = val;
		} else if (key.IsSameAs(KEY_REPEATER_PORT)) {
			val.ToULong(&temp2);
			m_repeaterPort = (unsigned int)temp2;
		} else if (key.IsSameAs(KEY_LOCAL_ADDRESS)) {
			m_localAddress = val;
		} else if (key.IsSameAs(KEY_LOCAL_PORT)) {
			val.ToULong(&temp2);
			m_localPort = (unsigned int)temp2;
		} else if (key.IsSameAs(KEY_WINDOW_X)) {
			val.ToLong(&temp1);
			m_x = int(temp1);
		} else if (key.IsSameAs(KEY_WINDOW_Y)) {
			val.ToLong(&temp1);
			m_y = int(temp1);
		}

		str = file.GetNextLine();
	}

	file.Close();
}

CDExtraGatewayConfig::~CDExtraGatewayConfig()
{
}

#endif

void CDExtraGatewayConfig::getReflector(wxString& callsign, wxString& reflector, bool& atStartup, RECONNECT& reconnect, TEXT_LANG& language) const
{
	callsign  = m_callsign;
	reflector = m_reflector;
	atStartup = m_atStartup;
	reconnect = m_reconnect;
	language  = m_language;
}

void CDExtraGatewayConfig::setReflector(const wxString& callsign, const wxString& reflector, bool atStartup, RECONNECT reconnect, TEXT_LANG language)
{
	m_callsign  = callsign;
	m_reflector = reflector;
	m_atStartup = atStartup;
	m_reconnect = reconnect;
	m_language  = language;
}

void CDExtraGatewayConfig::getRepeater(wxString& repeaterCallsign, wxString& repeaterAddress, unsigned int& repeaterPort, wxString& localAddress, unsigned int& localPort) const
{
	repeaterCallsign = m_repeaterCallsign;
	repeaterAddress  = m_repeaterAddress;
	repeaterPort     = m_repeaterPort;
	localAddress     = m_localAddress;
	localPort        = m_localPort;
}

void CDExtraGatewayConfig::setRepeater(const wxString& repeaterCallsign, const wxString& repeaterAddress, unsigned int repeaterPort, const wxString& localAddress, unsigned int localPort)
{
	m_repeaterCallsign = repeaterCallsign;
	m_repeaterAddress  = repeaterAddress;
	m_repeaterPort     = repeaterPort;
	m_localAddress     = localAddress;
	m_localPort        = localPort;
}

void CDExtraGatewayConfig::getPosition(int& x, int& y) const
{
	x = m_x;
	y = m_y;
}

void CDExtraGatewayConfig::setPosition(int x, int y)
{
	m_x = x;
	m_y = y;
}

#if defined(__WINDOWS__)

bool CDExtraGatewayConfig::write()
{
	m_config->Write(m_name + KEY_CALLSIGN, m_callsign);
	m_config->Write(m_name + KEY_REFLECTOR, m_reflector);
	m_config->Write(m_name + KEY_ATSTARTUP, m_atStartup);
	m_config->Write(m_name + KEY_RECONNECT, long(m_reconnect));
	m_config->Write(m_name + KEY_LANGUAGE, long(m_language));
	m_config->Write(m_name + KEY_REPEATER_CALLSIGN, m_repeaterCallsign);
	m_config->Write(m_name + KEY_REPEATER_ADDRESS, m_repeaterAddress);
	m_config->Write(m_name + KEY_REPEATER_PORT, long(m_repeaterPort));
	m_config->Write(m_name + KEY_LOCAL_ADDRESS, m_localAddress);
	m_config->Write(m_name + KEY_LOCAL_PORT, long(m_localPort));
	m_config->Write(m_name + KEY_WINDOW_X, long(m_x));
	m_config->Write(m_name + KEY_WINDOW_Y, long(m_y));
	m_config->Flush();

	return true;
}

#else

bool CDExtraGatewayConfig::write()
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
	buffer.Printf(wxT("%s=%s"), KEY_REFLECTOR.c_str(), m_reflector.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"), KEY_ATSTARTUP.c_str(), m_atStartup ? 1 : 0); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"), KEY_RECONNECT.c_str(), int(m_reconnect)); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"), KEY_LANGUAGE.c_str(), int(m_language)); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_REPEATER_CALLSIGN.c_str(), m_repeaterCallsign.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_REPEATER_ADDRESS.c_str(), m_repeaterAddress.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%u"), KEY_REPEATER_PORT.c_str(), m_repeaterPort); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_LOCAL_ADDRESS.c_str(), m_localAddress.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%u"), KEY_LOCAL_PORT.c_str(), m_localPort); file.AddLine(buffer);
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
