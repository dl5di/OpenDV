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

#ifndef	DVAPNodeConfig_H
#define	DVAPNodeConfig_H

#include "DStarDefines.h"
#include "DVAPNodeDefs.h"

#include <wx/wx.h>
#include <wx/config.h>
#include <wx/filename.h>

class CDVAPNodeConfig {
public:
#if defined(__WINDOWS__)
	CDVAPNodeConfig(wxConfigBase* config, const wxString& name);
#else
	CDVAPNodeConfig(const wxString& dir, const wxString& name);
#endif
	~CDVAPNodeConfig();

	void getCallsign(wxString& callsign, wxString& gateway, DSTAR_MODE& mode, ACK_TYPE& ack, bool& restriction, bool& rpt1Validation) const;
	void setCallsign(const wxString& callsign, const wxString& gateway, DSTAR_MODE mode, ACK_TYPE ack, bool restriction, bool rpt1Validation);

	void getNetwork(wxString& gatewayAddress, unsigned int& gatewayPort, wxString& localAddress, unsigned int& localPort) const;
	void setNetwork(const wxString& gatewayAddress, unsigned int gatewayPort, const wxString& localAddress, unsigned int localPort);

	void getDVAP(wxString& port, unsigned int& frequency, int& power, int& squelch) const;
	void setDVAP(const wxString& port, unsigned int frequency, int power, int squelch);

	void getTimes(unsigned int& timeout, unsigned int& ackTime) const;
	void setTimes(unsigned int timeout, unsigned int ackTime);

	void getBeacon(unsigned int& time, wxString& text, bool& voice, TEXT_LANG& language) const;
	void setBeacon(unsigned int time, const wxString& text, bool voice, TEXT_LANG language);

	void getAnnouncement(bool& enabled, unsigned int& time, wxString& recordRPT1, wxString& recordRPT2, wxString& deleteRPT1, wxString& deleteRPT2) const;
	void setAnnouncement(bool enabled, unsigned int time, const wxString& recordRPT1, const wxString& recordRPT2, const wxString& deleteRPT1, const wxString& deleteRPT2);

	void getLogging(bool& logging) const;
	void setLogging(bool logging);

	void getPosition(int& x, int& y) const;
	void setPosition(int x, int y);

	bool write();

private:
#if defined(__WINDOWS__)
	wxConfigBase* m_config;
	wxString      m_name;
#else
	wxFileName    m_fileName;
#endif
	wxString      m_callsign;
	wxString      m_gateway;
	DSTAR_MODE    m_mode;
	ACK_TYPE      m_ack;
	bool          m_restriction;
	bool          m_rpt1Validation;
	wxString      m_gatewayAddress;
	unsigned int  m_gatewayPort;
	wxString      m_localAddress;
	unsigned int  m_localPort;
	wxString      m_port;
	unsigned int  m_frequency;
	int           m_power;
	int           m_squelch;
	unsigned int  m_timeout;
	unsigned int  m_ackTime;
	unsigned int  m_beaconTime;
	wxString      m_beaconText;
	bool          m_beaconVoice;
	TEXT_LANG     m_language;
	bool          m_announcementEnabled;
	unsigned int  m_announcementTime;
	wxString      m_announcementRecordRPT1;
	wxString      m_announcementRecordRPT2;
	wxString      m_announcementDeleteRPT1;
	wxString      m_announcementDeleteRPT2;
	bool          m_logging;
	int           m_x;
	int           m_y;
};

#endif
