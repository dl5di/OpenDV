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

#ifndef	DVRPTRRepeaterConfig_H
#define	DVRPTRRepeaterConfig_H

#include "DStarDefines.h"
#include "DVRPTRRepeaterDefs.h"

#include <wx/wx.h>
#include <wx/config.h>
#include <wx/filename.h>

class CDVRPTRRepeaterConfig {
public:
#if defined(__WINDOWS__)
	CDVRPTRRepeaterConfig(wxConfigBase* config, const wxString& name);
#else
	CDVRPTRRepeaterConfig(const wxString& dir, const wxString& name);
#endif
	~CDVRPTRRepeaterConfig();

	void getCallsign(wxString& callsign, wxString& gateway, DSTAR_MODE& mode, ACK_TYPE& ack, bool& restriction, bool& rpt1Validation, bool& dtmfBlanking) const;
	void setCallsign(const wxString& callsign, const wxString& gateway, DSTAR_MODE mode, ACK_TYPE ack, bool restriction, bool rpt1Validation, bool dtmfBlanking);

	void getNetwork(wxString& gatewayAddress, unsigned int& gatewayPort, wxString& localAddress, unsigned int& localPort) const;
	void setNetwork(const wxString& gatewayAddress, unsigned int gatewayPort, const wxString& localAddress, unsigned int localPort);

	void getModem(DVRPTR_VERSION& version, CONNECTION_TYPE& connectionType, wxString& usbPort, wxString& address, unsigned int& port, bool& rxInvert, bool& txInvert, bool& channel, unsigned int& modLevel, unsigned int& txDelay) const;
	void setModem(DVRPTR_VERSION version, CONNECTION_TYPE connectionType, const wxString& usbPort, const wxString& address, unsigned int port, bool rxInvert, bool txInvert, bool channel, unsigned int modLevel, unsigned int txDelay);

	void getModem(wxString& path) const;
	void setModem(const wxString& path);

	void getTimes(unsigned int& timeout, unsigned int& ackTime) const;
	void setTimes(unsigned int timeout, unsigned int ackTime);

	void getBeacon(unsigned int& time, wxString& text, bool& voice, TEXT_LANG& language) const;
	void setBeacon(unsigned int time, const wxString& text, bool voice, TEXT_LANG language);

	void getAnnouncement(bool& enabled, unsigned int& time, wxString& recordRPT1, wxString& recordRPT2, wxString& deleteRPT1, wxString& deleteRPT2) const;
	void setAnnouncement(bool enabled, unsigned int time, const wxString& recordRPT1, const wxString& recordRPT2, const wxString& deleteRPT1, const wxString& deleteRPT2);

	void getControl(bool& enabled, wxString& rpt1Callsign, wxString& rpt2Callsign, wxString& shutdown, wxString& startup, wxString& status1, wxString& status2, wxString& status3, wxString& status4, wxString& status5, wxString& command1, wxString& command1Line, wxString& command2, wxString& command2Line, wxString& command3, wxString& command3Line, wxString& command4, wxString& command4Line, wxString& output1, wxString& output2, wxString& output3, wxString& output4) const;
	void setControl(bool enabled, const wxString& rpt1Callsign, const wxString& rpt2Callsign, const wxString& shutdown, const wxString& startup, const wxString& status1, const wxString& status2, const wxString& status3, const wxString& status4, const wxString& status5, const wxString& command1, const wxString& command1Line, const wxString& command2, const wxString& command2Line, const wxString& command3, const wxString& command3Line, const wxString& command4, const wxString& command4Line, const wxString& output1, const wxString& output2, const wxString& output3, const wxString& output4);

	void getController(wxString& type, unsigned int& activeHangTime) const;
	void setController(const wxString& type, unsigned int activeHangTime);

	void getOutputs(bool& out1, bool& out2, bool& out3, bool& out4) const;
	void setOutputs(bool out1, bool out2, bool out3, bool out4);

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
	bool          m_dtmfBlanking;
	wxString      m_gatewayAddress;
	unsigned int  m_gatewayPort;
	wxString      m_localAddress;
	unsigned int  m_localPort;
	DVRPTR_VERSION  m_modemVersion;
	CONNECTION_TYPE m_modemConnection;
	wxString      m_modemUSBPort;
	wxString      m_modemUSBPath;
	wxString      m_modemAddress;
	unsigned int  m_modemPort;
	bool          m_rxInvert;
	bool          m_txInvert;
	bool          m_channel;
	unsigned int  m_modLevel;
	unsigned int  m_txDelay;
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
	bool          m_controlEnabled;
	wxString      m_controlRpt1Callsign;
	wxString      m_controlRpt2Callsign;
	wxString      m_controlShutdown;
	wxString      m_controlStartup;
	wxString      m_controlStatus1;
	wxString      m_controlStatus2;
	wxString      m_controlStatus3;
	wxString      m_controlStatus4;
	wxString      m_controlStatus5;
	wxString      m_controlCommand1;
	wxString      m_controlCommand1Line;
	wxString      m_controlCommand2;
	wxString      m_controlCommand2Line;
	wxString      m_controlCommand3;
	wxString      m_controlCommand3Line;
	wxString      m_controlCommand4;
	wxString      m_controlCommand4Line;
	wxString      m_controlOutput1;
	wxString      m_controlOutput2;
	wxString      m_controlOutput3;
	wxString      m_controlOutput4;
	wxString      m_controllerType;
	unsigned int  m_activeHangTime;
	bool          m_output1;
	bool          m_output2;
	bool          m_output3;
	bool          m_output4;
	bool          m_logging;
	int           m_x;
	int           m_y;
};

#endif
