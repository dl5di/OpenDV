/*
 *   Copyright (C) 2010-2014 by Jonathan Naylor G4KLX
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

#ifndef	IRCDDBGatewayConfig_H
#define	IRCDDBGatewayConfig_H

#include "Defs.h"

#include <wx/wx.h>
#include <wx/config.h>
#include <wx/filename.h>

class CIRCDDBGatewayConfig {
public:
#if defined(__WINDOWS__)
	CIRCDDBGatewayConfig(wxConfigBase* config, const wxString& dir, const wxString& configName, const wxString& name);
#else
	CIRCDDBGatewayConfig(const wxString& dir, const wxString& configName, const wxString& name);
#endif
	~CIRCDDBGatewayConfig();

	void getGateway(GATEWAY_TYPE& type, wxString& callsign, wxString& address, wxString& icomAddress, unsigned int& icomPort, wxString& hbAddress, unsigned int& hbPort, double& latitude, double& longitude, wxString& description1, wxString& description2, wxString& url) const;
	void setGateway(GATEWAY_TYPE type, const wxString& callsign, const wxString& address, const wxString& icomAddress, unsigned int icomPort, const wxString& hbAddress, unsigned int hbPort, double latitude, double longitude, const wxString& description1, const wxString& description2, const wxString& url);

	void getRepeater1(wxString& callsign, wxString& band, HW_TYPE& type, wxString& address, unsigned int& port, unsigned char& band1, unsigned char& band2, unsigned char& band3, wxString& reflector, bool& atStartup, RECONNECT& reconnect, double& frequency, double& offset, double& range, double& latitude, double& longitude, double& agl, wxString& description1, wxString& description2, wxString& url) const;
	void setRepeater1(const wxString& band, HW_TYPE type, const wxString& address, unsigned int port, unsigned char band1, unsigned char band2, unsigned char band3, const wxString& reflector, bool atStartup, RECONNECT reconnect, double frequency, double offset, double range, double latitude, double longitude, double agl, const wxString& description1, const wxString& description2, const wxString& url);

	void getRepeater2(wxString& callsign, wxString& band, HW_TYPE& type, wxString& address, unsigned int& port, unsigned char& band1, unsigned char& band2, unsigned char& band3, wxString& reflector, bool& atStartup, RECONNECT& reconnect, double& frequency, double& offset, double& range, double& latitude, double& longitude, double& agl, wxString& description1, wxString& description2, wxString& url) const;
	void setRepeater2(const wxString& band, HW_TYPE type, const wxString& address, unsigned int port, unsigned char band1, unsigned char band2, unsigned char band3, const wxString& reflector, bool atStartup, RECONNECT reconnect, double frequency, double offset, double range, double latitude, double longitude, double agl, const wxString& description1, const wxString& description2, const wxString& url);

	void getRepeater3(wxString& callsign, wxString& band, HW_TYPE& type, wxString& address, unsigned int& port, unsigned char& band1, unsigned char& band2, unsigned char& band3, wxString& reflector, bool& atStartup, RECONNECT& reconnect, double& frequency, double& offset, double& range, double& latitude, double& longitude, double& agl, wxString& description1, wxString& description2, wxString& url) const;
	void setRepeater3(const wxString& band, HW_TYPE type, const wxString& address, unsigned int port, unsigned char band1, unsigned char band2, unsigned char band3, const wxString& reflector, bool atStartup, RECONNECT reconnect, double frequency, double offset, double range, double latitude, double longitude, double agl, const wxString& description1, const wxString& description2, const wxString& url);

	void getRepeater4(wxString& callsign, wxString& band, HW_TYPE& type, wxString& address, unsigned int& port, unsigned char& band1, unsigned char& band2, unsigned char& band3, wxString& reflector, bool& atStartup, RECONNECT& reconnect, double& frequency, double& offset, double& range, double& latitude, double& longitude, double& agl, wxString& description1, wxString& description2, wxString& url) const;
	void setRepeater4(const wxString& band, HW_TYPE type, const wxString& address, unsigned int port, unsigned char band1, unsigned char band2, unsigned char band3, const wxString& reflector, bool atStartup, RECONNECT reconnect, double frequency, double offset, double range, double latitude, double longitude, double agl, const wxString& description1, const wxString& description2, const wxString& url);

	void getIrcDDB(bool& enabled, wxString& hostname, wxString& username, wxString& password) const;
	void setIrcDDB(bool enabled, const wxString& hostname, const wxString& username, const wxString& password);

	void getDPRS(bool& enabled, wxString& hostname, unsigned int& port) const;
	void setDPRS(bool enabled, const wxString& hostname, unsigned int port);

	void getDExtra(bool& enabled, unsigned int& maxDongles) const;
	void setDExtra(bool enabled, unsigned int maxDongles);

	void getDPlus(bool& enabled, unsigned int& maxDongles, wxString& login) const;
	void setDPlus(bool enabled, unsigned int maxDongles, const wxString& login);

	void getDCS(bool& dcsEnabled, bool& ccsEnabled, wxString& ccsHost) const;
	void setDCS(bool dcsEnabled, bool ccsEnabled, const wxString& ccsHost);

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
	void getStarNet1(wxString& band, wxString& callsign, wxString& logoff, wxString& info, wxString& permanent, unsigned int& userTimeout, unsigned int& groupTimeout, STARNET_CALLSIGN_SWITCH& callsignSwitch, bool& txMsgSwitch, wxString& reflector) const;
	void setStarNet1(const wxString& band, const wxString& callsign, const wxString& logoff, const wxString& info, const wxString& permanent, unsigned int userTimeout, unsigned int groupTimeout, STARNET_CALLSIGN_SWITCH callsignSwitch, bool txMsgSwitch, const wxString& reflector);

	void getStarNet2(wxString& band, wxString& callsign, wxString& logoff, wxString& info, wxString& permanent, unsigned int& userTimeout, unsigned int& groupTimeout, STARNET_CALLSIGN_SWITCH& callsignSwitch, bool& txMsgSwitch, wxString& reflector) const;
	void setStarNet2(const wxString& band, const wxString& callsign, const wxString& logoff, const wxString& info, const wxString& permanent, unsigned int userTimeout, unsigned int groupTimeout, STARNET_CALLSIGN_SWITCH callsignSwitch, bool txMsgSwitch, const wxString& reflector);

	void getStarNet3(wxString& band, wxString& callsign, wxString& logoff, wxString& info, wxString& permanent, unsigned int& userTimeout, unsigned int& groupTimeout, STARNET_CALLSIGN_SWITCH& callsignSwitch, bool& txMsgSwitch, wxString& reflector) const;
	void setStarNet3(const wxString& band, const wxString& callsign, const wxString& logoff, const wxString& info, const wxString& permanent, unsigned int userTimeout, unsigned int groupTimeout, STARNET_CALLSIGN_SWITCH callsignSwitch, bool txMsgSwitch, const wxString& reflector);

	void getStarNet4(wxString& band, wxString& callsign, wxString& logoff, wxString& info, wxString& permanent, unsigned int& userTimeout, unsigned int& groupTimeout, STARNET_CALLSIGN_SWITCH& callsignSwitch, bool& txMsgSwitch, wxString& reflector) const;
	void setStarNet4(const wxString& band, const wxString& callsign, const wxString& logoff, const wxString& info, const wxString& permanent, unsigned int userTimeout, unsigned int groupTimeout, STARNET_CALLSIGN_SWITCH callsignSwitch, bool txMsgSwitch, const wxString& reflector);

	void getStarNet5(wxString& band, wxString& callsign, wxString& logoff, wxString& info, wxString& permanent, unsigned int& userTimeout, unsigned int& groupTimeout, STARNET_CALLSIGN_SWITCH& callsignSwitch, bool& txMsgSwitch, wxString& reflector) const;
	void setStarNet5(const wxString& band, const wxString& callsign, const wxString& logoff, const wxString& info, const wxString& permanent, unsigned int userTimeout, unsigned int groupTimeout, STARNET_CALLSIGN_SWITCH callsignSwitch, bool txMsgSwitch, const wxString& reflector);
#else
	void getStarNet1(wxString& band, wxString& callsign, wxString& logoff, wxString& info, wxString& permanent, unsigned int& userTimeout, unsigned int& groupTimeout, STARNET_CALLSIGN_SWITCH& callsignSwitch, bool& txMsgSwitch) const;
	void setStarNet1(const wxString& band, const wxString& callsign, const wxString& logoff, const wxString& info, const wxString& permanent, unsigned int userTimeout, unsigned int groupTimeout, STARNET_CALLSIGN_SWITCH callsignSwitch, bool txMsgSwitch);

	void getStarNet2(wxString& band, wxString& callsign, wxString& logoff, wxString& info, wxString& permanent, unsigned int& userTimeout, unsigned int& groupTimeout, STARNET_CALLSIGN_SWITCH& callsignSwitch, bool& txMsgSwitch) const;
	void setStarNet2(const wxString& band, const wxString& callsign, const wxString& logoff, const wxString& info, const wxString& permanent, unsigned int userTimeout, unsigned int groupTimeout, STARNET_CALLSIGN_SWITCH callsignSwitch, bool txMsgSwitch);

	void getStarNet3(wxString& band, wxString& callsign, wxString& logoff, wxString& info, wxString& permanent, unsigned int& userTimeout, unsigned int& groupTimeout, STARNET_CALLSIGN_SWITCH& callsignSwitch, bool& txMsgSwitch) const;
	void setStarNet3(const wxString& band, const wxString& callsign, const wxString& logoff, const wxString& info, const wxString& permanent, unsigned int userTimeout, unsigned int groupTimeout, STARNET_CALLSIGN_SWITCH callsignSwitch, bool txMsgSwitch);

	void getStarNet4(wxString& band, wxString& callsign, wxString& logoff, wxString& info, wxString& permanent, unsigned int& userTimeout, unsigned int& groupTimeout, STARNET_CALLSIGN_SWITCH& callsignSwitch, bool& txMsgSwitch) const;
	void setStarNet4(const wxString& band, const wxString& callsign, const wxString& logoff, const wxString& info, const wxString& permanent, unsigned int userTimeout, unsigned int groupTimeout, STARNET_CALLSIGN_SWITCH callsignSwitch, bool txMsgSwitch);

	void getStarNet5(wxString& band, wxString& callsign, wxString& logoff, wxString& info, wxString& permanent, unsigned int& userTimeout, unsigned int& groupTimeout, STARNET_CALLSIGN_SWITCH& callsignSwitch, bool& txMsgSwitch) const;
	void setStarNet5(const wxString& band, const wxString& callsign, const wxString& logoff, const wxString& info, const wxString& permanent, unsigned int userTimeout, unsigned int groupTimeout, STARNET_CALLSIGN_SWITCH callsignSwitch, bool txMsgSwitch);
#endif

	void getRemote(bool& enabled, wxString& password, unsigned int& port) const;
	void setRemote(bool enabled, const wxString& password, unsigned int port);

	void getMiscellaneous(TEXT_LANG& language, bool& infoEnabled, bool& echoEnabled, bool& logEnabled, bool& dratsEnabled, bool& dtmfEnabled) const;
	void setMiscellaneous(TEXT_LANG language, bool infoEnabled, bool echoEnabled, bool logEnabled, bool dratsEnabled, bool dtmfEnabled);

	void getPosition(int& x, int& y) const;
	void setPosition(int x, int y);

	bool write();

private:
#if defined(__WINDOWS__)
	wxConfigBase* m_config;
	wxString      m_name;
#endif
	wxFileName    m_fileName;
	GATEWAY_TYPE  m_type;
	wxString      m_callsign;
	wxString      m_address;
	wxString      m_icomAddress;
	unsigned int  m_icomPort;
	wxString      m_hbAddress;
	unsigned int  m_hbPort;
	double        m_latitude;
	double        m_longitude;
	wxString      m_description1;
	wxString      m_description2;
	wxString      m_url;
	wxString      m_repeater1Callsign;
	wxString      m_repeater1Band;
	HW_TYPE       m_repeater1Type;
	wxString      m_repeater1Address;
	unsigned int  m_repeater1Port;
	wxString      m_repeater1Reflector;
	bool          m_repeater1AtStartup;
	RECONNECT     m_repeater1Reconnect;
	double        m_repeater1Frequency;
	double        m_repeater1Offset;
	double        m_repeater1Range;
	double        m_repeater1Latitude;
	double        m_repeater1Longitude;
	double        m_repeater1Agl;
	wxString      m_repeater1Description1;
	wxString      m_repeater1Description2;
	wxString      m_repeater1URL;
	unsigned char m_repeater1Band1;
	unsigned char m_repeater1Band2;
	unsigned char m_repeater1Band3;
	wxString      m_repeater2Callsign;
	wxString      m_repeater2Band;
	HW_TYPE       m_repeater2Type;
	wxString      m_repeater2Address;
	unsigned int  m_repeater2Port;
	wxString      m_repeater2Reflector;
	bool          m_repeater2AtStartup;
	RECONNECT     m_repeater2Reconnect;
	double        m_repeater2Frequency;
	double        m_repeater2Offset;
	double        m_repeater2Range;
	double        m_repeater2Latitude;
	double        m_repeater2Longitude;
	double        m_repeater2Agl;
	wxString      m_repeater2Description1;
	wxString      m_repeater2Description2;
	wxString      m_repeater2URL;
	unsigned char m_repeater2Band1;
	unsigned char m_repeater2Band2;
	unsigned char m_repeater2Band3;
	wxString      m_repeater3Callsign;
	wxString      m_repeater3Band;
	HW_TYPE       m_repeater3Type;
	wxString      m_repeater3Address;
	unsigned int  m_repeater3Port;
	wxString      m_repeater3Reflector;
	bool          m_repeater3AtStartup;
	RECONNECT     m_repeater3Reconnect;
	double        m_repeater3Frequency;
	double        m_repeater3Offset;
	double        m_repeater3Range;
	double        m_repeater3Latitude;
	double        m_repeater3Longitude;
	double        m_repeater3Agl;
	wxString      m_repeater3Description1;
	wxString      m_repeater3Description2;
	wxString      m_repeater3URL;
	unsigned char m_repeater3Band1;
	unsigned char m_repeater3Band2;
	unsigned char m_repeater3Band3;
	wxString      m_repeater4Callsign;
	wxString      m_repeater4Band;
	HW_TYPE       m_repeater4Type;
	wxString      m_repeater4Address;
	unsigned int  m_repeater4Port;
	wxString      m_repeater4Reflector;
	bool          m_repeater4AtStartup;
	RECONNECT     m_repeater4Reconnect;
	double        m_repeater4Frequency;
	double        m_repeater4Offset;
	double        m_repeater4Range;
	double        m_repeater4Latitude;
	double        m_repeater4Longitude;
	double        m_repeater4Agl;
	wxString      m_repeater4Description1;
	wxString      m_repeater4Description2;
	wxString      m_repeater4URL;
	unsigned char m_repeater4Band1;
	unsigned char m_repeater4Band2;
	unsigned char m_repeater4Band3;
	bool          m_ircddbEnabled;
	wxString      m_ircddbHostname;
	wxString      m_ircddbUsername;
	wxString      m_ircddbPassword;
	bool          m_aprsEnabled;
	wxString      m_aprsHostname;
	unsigned int  m_aprsPort;
	bool          m_dextraEnabled;
	unsigned int  m_dextraMaxDongles;
	bool          m_dplusEnabled;
	unsigned int  m_dplusMaxDongles;
	wxString      m_dplusLogin;
	bool          m_dcsEnabled;
	bool          m_ccsEnabled;
	wxString      m_ccsHost;
	wxString      m_starNet1Band;
	wxString      m_starNet1Callsign;
	wxString      m_starNet1Logoff;
	wxString      m_starNet1Info;
	wxString      m_starNet1Permanent;
	unsigned int  m_starNet1UserTimeout;
	unsigned int  m_starNet1GroupTimeout;
	STARNET_CALLSIGN_SWITCH m_starNet1CallsignSwitch;
	bool          m_starNet1TxMsgSwitch;
	wxString      m_starNet1Reflector;
	wxString      m_starNet2Band;
	wxString      m_starNet2Callsign;
	wxString      m_starNet2Logoff;
	wxString      m_starNet2Info;
	wxString      m_starNet2Permanent;
	unsigned int  m_starNet2UserTimeout;
	unsigned int  m_starNet2GroupTimeout;
	STARNET_CALLSIGN_SWITCH m_starNet2CallsignSwitch;
	bool          m_starNet2TxMsgSwitch;
	wxString      m_starNet2Reflector;
	wxString      m_starNet3Band;
	wxString      m_starNet3Callsign;
	wxString      m_starNet3Logoff;
	wxString      m_starNet3Info;
	wxString      m_starNet3Permanent;
	unsigned int  m_starNet3UserTimeout;
	unsigned int  m_starNet3GroupTimeout;
	STARNET_CALLSIGN_SWITCH m_starNet3CallsignSwitch;
	bool          m_starNet3TxMsgSwitch;
	wxString      m_starNet3Reflector;
	wxString      m_starNet4Band;
	wxString      m_starNet4Callsign;
	wxString      m_starNet4Logoff;
	wxString      m_starNet4Info;
	wxString      m_starNet4Permanent;
	unsigned int  m_starNet4UserTimeout;
	unsigned int  m_starNet4GroupTimeout;
	STARNET_CALLSIGN_SWITCH m_starNet4CallsignSwitch;
	bool          m_starNet4TxMsgSwitch;
	wxString      m_starNet4Reflector;
	wxString      m_starNet5Band;
	wxString      m_starNet5Callsign;
	wxString      m_starNet5Logoff;
	wxString      m_starNet5Info;
	wxString      m_starNet5Permanent;
	unsigned int  m_starNet5UserTimeout;
	unsigned int  m_starNet5GroupTimeout;
	STARNET_CALLSIGN_SWITCH m_starNet5CallsignSwitch;
	bool          m_starNet5TxMsgSwitch;
	wxString      m_starNet5Reflector;
	bool          m_remoteEnabled;
	wxString      m_remotePassword;
	unsigned int  m_remotePort;
	TEXT_LANG     m_language;
	bool          m_infoEnabled;
	bool          m_echoEnabled;
	bool          m_logEnabled;
	bool          m_dratsEnabled;
	bool          m_dtmfEnabled;
	int           m_x;
	int           m_y;
};

#endif
