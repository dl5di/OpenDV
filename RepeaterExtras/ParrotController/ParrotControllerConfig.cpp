/*
 *   Copyright (C) 2009,2011,2012 by Jonathan Naylor G4KLX
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

#include "ParrotControllerConfig.h"

const wxString KEY_MODE             = wxT("mode");
const wxString KEY_BEACON_TIME      = wxT("beaconTime");
const wxString KEY_BEACON_FILENAME  = wxT("beaconFileName");
const wxString KEY_TURNAROUND_TIME  = wxT("turnaroundTime");
const wxString KEY_KEEP_FILE        = wxT("keepFile");
const wxString KEY_REPEATER_ADDRESS = wxT("repeaterAddress");
const wxString KEY_REPEATER_PORT    = wxT("repeaterPort");
const wxString KEY_LOCAL_ADDRESS    = wxT("localAddress");
const wxString KEY_LOCAL_PORT       = wxT("localPort");
const wxString KEY_WINDOW_X         = wxT("windowX");
const wxString KEY_WINDOW_Y         = wxT("windowY");

const PARROT_MODE  DEFAULT_MODE             = PM_PARROT;
const unsigned int DEFAULT_BEACON_TIME      = 60U;
const wxString     DEFAULT_BEACON_FILENAME  = wxEmptyString;
const unsigned int DEFAULT_TURNAROUND_TIME  = 2U;
const bool         DEFAULT_KEEP_FILE        = false;
const wxString     DEFAULT_REPEATER_ADDRESS = wxT("127.0.0.1");
const unsigned int DEFAULT_REPEATER_PORT    = 20011U;
const wxString     DEFAULT_LOCAL_ADDRESS    = wxT("127.0.0.1");
const unsigned int DEFAULT_LOCAL_PORT       = 20010U;
const int          DEFAULT_WINDOW_X         = -1;
const int          DEFAULT_WINDOW_Y         = -1;


#if defined(__WINDOWS__)

CParrotControllerConfig::CParrotControllerConfig(wxConfigBase* config, const wxString& name) :
m_config(config),
m_name(wxT("/")),
m_mode(DEFAULT_MODE),
m_beaconTime(DEFAULT_BEACON_TIME),
m_beaconFileName(DEFAULT_BEACON_FILENAME),
m_turnaroundTime(DEFAULT_TURNAROUND_TIME),
m_keepFile(DEFAULT_KEEP_FILE),
m_repeaterAddress(DEFAULT_REPEATER_ADDRESS),
m_repeaterPort(DEFAULT_REPEATER_PORT),
m_localAddress(DEFAULT_LOCAL_ADDRESS),
m_localPort(DEFAULT_LOCAL_PORT),
m_x(DEFAULT_WINDOW_X),
m_y(DEFAULT_WINDOW_Y)
{
	wxASSERT(config != NULL);

	if (!name.IsEmpty())
		m_name = wxT("/") + name;

	long temp;
	m_config->Read(m_name + KEY_MODE, &temp, long(DEFAULT_MODE));
	m_mode = PARROT_MODE(temp);

	m_config->Read(m_name + KEY_BEACON_TIME, &temp, long(DEFAULT_BEACON_TIME));
	m_beaconTime = (unsigned int)temp;

	m_config->Read(m_name + KEY_BEACON_FILENAME, &m_beaconFileName, DEFAULT_BEACON_FILENAME);

	m_config->Read(m_name + KEY_TURNAROUND_TIME, &temp, long(DEFAULT_TURNAROUND_TIME));
	m_turnaroundTime = (unsigned int)temp;

	m_config->Read(m_name + KEY_KEEP_FILE, &m_keepFile, DEFAULT_KEEP_FILE);

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

CParrotControllerConfig::~CParrotControllerConfig()
{
	delete m_config;
}

#else

CParrotControllerConfig::CParrotControllerConfig(const wxString& dir, const wxString& name) :
m_fileName(),
m_mode(DEFAULT_MODE),
m_beaconTime(DEFAULT_BEACON_TIME),
m_beaconFileName(DEFAULT_BEACON_FILENAME),
m_turnaroundTime(DEFAULT_TURNAROUND_TIME),
m_keepFile(DEFAULT_KEEP_FILE),
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

		if (key.IsSameAs(KEY_MODE)) {
			val.ToLong(&temp1);
			m_mode = PARROT_MODE(temp1);
		} else if (key.IsSameAs(KEY_BEACON_TIME)) {
			val.ToULong(&temp2);
			m_beaconTime = (unsigned int)temp2;
		} else if (key.IsSameAs(KEY_BEACON_FILENAME)) {
			m_beaconFileName = val;
		} else if (key.IsSameAs(KEY_TURNAROUND_TIME)) {
			val.ToULong(&temp2);
			m_turnaroundTime = (unsigned int)temp2;
		} else if (key.IsSameAs(KEY_KEEP_FILE)) {
			val.ToLong(&temp1);
			m_keepFile = temp1 == 1L;
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

CParrotControllerConfig::~CParrotControllerConfig()
{
}

#endif

void CParrotControllerConfig::getMode(PARROT_MODE& mode, unsigned int& beaconTime, wxString& beaconFileName, unsigned int& turnaroundTime, bool& keepFile) const
{
	mode           = m_mode;
	beaconTime     = m_beaconTime;
	beaconFileName = m_beaconFileName;
	turnaroundTime = m_turnaroundTime;
	keepFile       = m_keepFile;
}

void CParrotControllerConfig::setMode(PARROT_MODE mode, unsigned int beaconTime, const wxString& beaconFileName, unsigned int turnaroundTime, bool keepFile)
{
	m_mode           = mode;
	m_beaconTime     = beaconTime;
	m_beaconFileName = beaconFileName;
	m_turnaroundTime = turnaroundTime;
	m_keepFile       = keepFile;
}

void CParrotControllerConfig::getNetwork(wxString& repeaterAddress, unsigned int& repeaterPort, wxString& localAddress, unsigned int& localPort) const
{
	repeaterAddress = m_repeaterAddress;
	repeaterPort    = m_repeaterPort;
	localAddress    = m_localAddress;
	localPort       = m_localPort;
}

void CParrotControllerConfig::setNetwork(const wxString& repeaterAddress, unsigned int repeaterPort, const wxString& localAddress, unsigned int localPort)
{
	m_repeaterAddress = repeaterAddress;
	m_repeaterPort    = repeaterPort;
	m_localAddress    = localAddress;
	m_localPort       = localPort;
}

void CParrotControllerConfig::getPosition(int& x, int& y) const
{
	x = m_x;
	y = m_y;
}

void CParrotControllerConfig::setPosition(int x, int y)
{
	m_x = x;
	m_y = y;
}

#if defined(__WINDOWS__)

bool CParrotControllerConfig::write()
{
	m_config->Write(m_name + KEY_MODE, long(m_mode));
	m_config->Write(m_name + KEY_BEACON_TIME, long(m_beaconTime));
	m_config->Write(m_name + KEY_BEACON_FILENAME, m_beaconFileName);
	m_config->Write(m_name + KEY_TURNAROUND_TIME, long(m_turnaroundTime));
	m_config->Write(m_name + KEY_KEEP_FILE, m_keepFile);
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

bool CParrotControllerConfig::write()
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
	buffer.Printf(wxT("%s=%d"), KEY_MODE.c_str(), int(m_mode)); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%u"), KEY_BEACON_TIME.c_str(), m_beaconTime); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_BEACON_FILENAME.c_str(), m_beaconFileName.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%u"), KEY_TURNAROUND_TIME.c_str(), m_turnaroundTime); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"), KEY_KEEP_FILE.c_str(), m_keepFile ? 1 : 0); file.AddLine(buffer);
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
