/*
 *   Copyright (C) 2010,2011,2012,2014 by Jonathan Naylor G4KLX
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

#ifndef	StarNetServerConfig_H
#define	StarNetServerConfig_H

#include "Defs.h"

#include <wx/wx.h>
#include <wx/config.h>
#include <wx/filename.h>

class CStarNetServerConfig {
public:
#if defined(__WINDOWS__)
	CStarNetServerConfig(wxConfigBase* config, const wxString& dir);
#else
	CStarNetServerConfig(const wxString& dir);
#endif
	~CStarNetServerConfig();

	void getGateway(wxString& callsign, wxString& address) const;
	void setGateway(const wxString& callsign, const wxString& address);

	void getIrcDDB(wxString& hostname, wxString& username, wxString& password) const;
	void setIrcDDB(const wxString& hostname, const wxString& username, const wxString& password);

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

	void getStarNet6(wxString& band, wxString& callsign, wxString& logoff, wxString& info, wxString& permanent, unsigned int& userTimeout, unsigned int& groupTimeout, STARNET_CALLSIGN_SWITCH& callsignSwitch, bool& txMsgSwitch, wxString& reflector) const;
	void setStarNet6(const wxString& band, const wxString& callsign, const wxString& logoff, const wxString& info, const wxString& permanent, unsigned int userTimeout, unsigned int groupTimeout, STARNET_CALLSIGN_SWITCH callsignSwitch, bool txMsgSwitch, const wxString& reflector);

	void getStarNet7(wxString& band, wxString& callsign, wxString& logoff, wxString& info, wxString& permanent, unsigned int& userTimeout, unsigned int& groupTimeout, STARNET_CALLSIGN_SWITCH& callsignSwitch, bool& txMsgSwitch, wxString& reflector) const;
	void setStarNet7(const wxString& band, const wxString& callsign, const wxString& logoff, const wxString& info, const wxString& permanent, unsigned int userTimeout, unsigned int groupTimeout, STARNET_CALLSIGN_SWITCH callsignSwitch, bool txMsgSwitch, const wxString& reflector);

	void getStarNet8(wxString& band, wxString& callsign, wxString& logoff, wxString& info, wxString& permanent, unsigned int& userTimeout, unsigned int& groupTimeout, STARNET_CALLSIGN_SWITCH& callsignSwitch, bool& txMsgSwitch, wxString& reflector) const;
	void setStarNet8(const wxString& band, const wxString& callsign, const wxString& logoff, const wxString& info, const wxString& permanent, unsigned int userTimeout, unsigned int groupTimeout, STARNET_CALLSIGN_SWITCH callsignSwitch, bool txMsgSwitch, const wxString& reflector);

	void getStarNet9(wxString& band, wxString& callsign, wxString& logoff, wxString& info, wxString& permanent, unsigned int& userTimeout, unsigned int& groupTimeout, STARNET_CALLSIGN_SWITCH& callsignSwitch, bool& txMsgSwitch, wxString& reflector) const;
	void setStarNet9(const wxString& band, const wxString& callsign, const wxString& logoff, const wxString& info, const wxString& permanent, unsigned int userTimeout, unsigned int groupTimeout, STARNET_CALLSIGN_SWITCH callsignSwitch, bool txMsgSwitch, const wxString& reflector);

	void getStarNet10(wxString& band, wxString& callsign, wxString& logoff, wxString& info, wxString& permanent, unsigned int& userTimeout, unsigned int& groupTimeout, STARNET_CALLSIGN_SWITCH& callsignSwitch, bool& txMsgSwitch, wxString& reflector) const;
	void setStarNet10(const wxString& band, const wxString& callsign, const wxString& logoff, const wxString& info, const wxString& permanent, unsigned int userTimeout, unsigned int groupTimeout, STARNET_CALLSIGN_SWITCH callsignSwitch, bool txMsgSwitch, const wxString& reflector);

	void getStarNet11(wxString& band, wxString& callsign, wxString& logoff, wxString& info, wxString& permanent, unsigned int& userTimeout, unsigned int& groupTimeout, STARNET_CALLSIGN_SWITCH& callsignSwitch, bool& txMsgSwitch, wxString& reflector) const;
	void setStarNet11(const wxString& band, const wxString& callsign, const wxString& logoff, const wxString& info, const wxString& permanent, unsigned int userTimeout, unsigned int groupTimeout, STARNET_CALLSIGN_SWITCH callsignSwitch, bool txMsgSwitch, const wxString& reflector);

	void getStarNet12(wxString& band, wxString& callsign, wxString& logoff, wxString& info, wxString& permanent, unsigned int& userTimeout, unsigned int& groupTimeout, STARNET_CALLSIGN_SWITCH& callsignSwitch, bool& txMsgSwitch, wxString& reflector) const;
	void setStarNet12(const wxString& band, const wxString& callsign, const wxString& logoff, const wxString& info, const wxString& permanent, unsigned int userTimeout, unsigned int groupTimeout, STARNET_CALLSIGN_SWITCH callsignSwitch, bool txMsgSwitch, const wxString& reflector);

	void getStarNet13(wxString& band, wxString& callsign, wxString& logoff, wxString& info, wxString& permanent, unsigned int& userTimeout, unsigned int& groupTimeout, STARNET_CALLSIGN_SWITCH& callsignSwitch, bool& txMsgSwitch, wxString& reflector) const;
	void setStarNet13(const wxString& band, const wxString& callsign, const wxString& logoff, const wxString& info, const wxString& permanent, unsigned int userTimeout, unsigned int groupTimeout, STARNET_CALLSIGN_SWITCH callsignSwitch, bool txMsgSwitch, const wxString& reflector);

	void getStarNet14(wxString& band, wxString& callsign, wxString& logoff, wxString& info, wxString& permanent, unsigned int& userTimeout, unsigned int& groupTimeout, STARNET_CALLSIGN_SWITCH& callsignSwitch, bool& txMsgSwitch, wxString& reflector) const;
	void setStarNet14(const wxString& band, const wxString& callsign, const wxString& logoff, const wxString& info, const wxString& permanent, unsigned int userTimeout, unsigned int groupTimeout, STARNET_CALLSIGN_SWITCH callsignSwitch, bool txMsgSwitch, const wxString& reflector);

	void getStarNet15(wxString& band, wxString& callsign, wxString& logoff, wxString& info, wxString& permanent, unsigned int& userTimeout, unsigned int& groupTimeout, STARNET_CALLSIGN_SWITCH& callsignSwitch, bool& txMsgSwitch, wxString& reflector) const;
	void setStarNet15(const wxString& band, const wxString& callsign, const wxString& logoff, const wxString& info, const wxString& permanent, unsigned int userTimeout, unsigned int groupTimeout, STARNET_CALLSIGN_SWITCH callsignSwitch, bool txMsgSwitch, const wxString& reflector);
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

	void getStarNet6(wxString& band, wxString& callsign, wxString& logoff, wxString& info, wxString& permanent, unsigned int& userTimeout, unsigned int& groupTimeout, STARNET_CALLSIGN_SWITCH& callsignSwitch, bool& txMsgSwitch) const;
	void setStarNet6(const wxString& band, const wxString& callsign, const wxString& logoff, const wxString& info, const wxString& permanent, unsigned int userTimeout, unsigned int groupTimeout, STARNET_CALLSIGN_SWITCH callsignSwitch, bool txMsgSwitch);

	void getStarNet7(wxString& band, wxString& callsign, wxString& logoff, wxString& info, wxString& permanent, unsigned int& userTimeout, unsigned int& groupTimeout, STARNET_CALLSIGN_SWITCH& callsignSwitch, bool& txMsgSwitch) const;
	void setStarNet7(const wxString& band, const wxString& callsign, const wxString& logoff, const wxString& info, const wxString& permanent, unsigned int userTimeout, unsigned int groupTimeout, STARNET_CALLSIGN_SWITCH callsignSwitch, bool txMsgSwitch);

	void getStarNet8(wxString& band, wxString& callsign, wxString& logoff, wxString& info, wxString& permanent, unsigned int& userTimeout, unsigned int& groupTimeout, STARNET_CALLSIGN_SWITCH& callsignSwitch, bool& txMsgSwitch) const;
	void setStarNet8(const wxString& band, const wxString& callsign, const wxString& logoff, const wxString& info, const wxString& permanent, unsigned int userTimeout, unsigned int groupTimeout, STARNET_CALLSIGN_SWITCH callsignSwitch, bool txMsgSwitch);

	void getStarNet9(wxString& band, wxString& callsign, wxString& logoff, wxString& info, wxString& permanent, unsigned int& userTimeout, unsigned int& groupTimeout, STARNET_CALLSIGN_SWITCH& callsignSwitch, bool& txMsgSwitch) const;
	void setStarNet9(const wxString& band, const wxString& callsign, const wxString& logoff, const wxString& info, const wxString& permanent, unsigned int userTimeout, unsigned int groupTimeout, STARNET_CALLSIGN_SWITCH callsignSwitch, bool txMsgSwitch);

	void getStarNet10(wxString& band, wxString& callsign, wxString& logoff, wxString& info, wxString& permanent, unsigned int& userTimeout, unsigned int& groupTimeout, STARNET_CALLSIGN_SWITCH& callsignSwitch, bool& txMsgSwitch) const;
	void setStarNet10(const wxString& band, const wxString& callsign, const wxString& logoff, const wxString& info, const wxString& permanent, unsigned int userTimeout, unsigned int groupTimeout, STARNET_CALLSIGN_SWITCH callsignSwitch, bool txMsgSwitch);

	void getStarNet11(wxString& band, wxString& callsign, wxString& logoff, wxString& info, wxString& permanent, unsigned int& userTimeout, unsigned int& groupTimeout, STARNET_CALLSIGN_SWITCH& callsignSwitch, bool& txMsgSwitch) const;
	void setStarNet11(const wxString& band, const wxString& callsign, const wxString& logoff, const wxString& info, const wxString& permanent, unsigned int userTimeout, unsigned int groupTimeout, STARNET_CALLSIGN_SWITCH callsignSwitch, bool txMsgSwitch);

	void getStarNet12(wxString& band, wxString& callsign, wxString& logoff, wxString& info, wxString& permanent, unsigned int& userTimeout, unsigned int& groupTimeout, STARNET_CALLSIGN_SWITCH& callsignSwitch, bool& txMsgSwitch) const;
	void setStarNet12(const wxString& band, const wxString& callsign, const wxString& logoff, const wxString& info, const wxString& permanent, unsigned int userTimeout, unsigned int groupTimeout, STARNET_CALLSIGN_SWITCH callsignSwitch, bool txMsgSwitch);

	void getStarNet13(wxString& band, wxString& callsign, wxString& logoff, wxString& info, wxString& permanent, unsigned int& userTimeout, unsigned int& groupTimeout, STARNET_CALLSIGN_SWITCH& callsignSwitch, bool& txMsgSwitch) const;
	void setStarNet13(const wxString& band, const wxString& callsign, const wxString& logoff, const wxString& info, const wxString& permanent, unsigned int userTimeout, unsigned int groupTimeout, STARNET_CALLSIGN_SWITCH callsignSwitch, bool txMsgSwitch);

	void getStarNet14(wxString& band, wxString& callsign, wxString& logoff, wxString& info, wxString& permanent, unsigned int& userTimeout, unsigned int& groupTimeout, STARNET_CALLSIGN_SWITCH& callsignSwitch, bool& txMsgSwitch) const;
	void setStarNet14(const wxString& band, const wxString& callsign, const wxString& logoff, const wxString& info, const wxString& permanent, unsigned int userTimeout, unsigned int groupTimeout, STARNET_CALLSIGN_SWITCH callsignSwitch, bool txMsgSwitch);

	void getStarNet15(wxString& band, wxString& callsign, wxString& logoff, wxString& info, wxString& permanent, unsigned int& userTimeout, unsigned int& groupTimeout, STARNET_CALLSIGN_SWITCH& callsignSwitch, bool& txMsgSwitch) const;
	void setStarNet15(const wxString& band, const wxString& callsign, const wxString& logoff, const wxString& info, const wxString& permanent, unsigned int userTimeout, unsigned int groupTimeout, STARNET_CALLSIGN_SWITCH callsignSwitch, bool txMsgSwitch);
#endif

	void getRemote(bool& enabled, wxString& password, unsigned int& port) const;
	void setRemote(bool enabled, const wxString& password, unsigned int port);

	void getMiscellaneous(bool& enabled) const;
	void setMiscellaneous(bool enabled);

	void getPosition(int& x, int& y) const;
	void setPosition(int x, int y);

	bool write();

private:
#if defined(__WINDOWS__)
	wxConfigBase* m_config;
#endif
	wxFileName    m_fileName;
	wxString      m_callsign;
	wxString      m_address;
	wxString      m_ircddbHostname;
	wxString      m_ircddbUsername;
	wxString      m_ircddbPassword;
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
	wxString      m_starNet6Band;
	wxString      m_starNet6Callsign;
	wxString      m_starNet6Logoff;
	wxString      m_starNet6Info;
	wxString      m_starNet6Permanent;
	unsigned int  m_starNet6UserTimeout;
	unsigned int  m_starNet6GroupTimeout;
	STARNET_CALLSIGN_SWITCH m_starNet6CallsignSwitch;
	bool          m_starNet6TxMsgSwitch;
	wxString      m_starNet6Reflector;
	wxString      m_starNet7Band;
	wxString      m_starNet7Callsign;
	wxString      m_starNet7Logoff;
	wxString      m_starNet7Info;
	wxString      m_starNet7Permanent;
	unsigned int  m_starNet7UserTimeout;
	unsigned int  m_starNet7GroupTimeout;
	STARNET_CALLSIGN_SWITCH m_starNet7CallsignSwitch;
	bool          m_starNet7TxMsgSwitch;
	wxString      m_starNet7Reflector;
	wxString      m_starNet8Band;
	wxString      m_starNet8Callsign;
	wxString      m_starNet8Logoff;
	wxString      m_starNet8Info;
	wxString      m_starNet8Permanent;
	unsigned int  m_starNet8UserTimeout;
	unsigned int  m_starNet8GroupTimeout;
	STARNET_CALLSIGN_SWITCH m_starNet8CallsignSwitch;
	bool          m_starNet8TxMsgSwitch;
	wxString      m_starNet8Reflector;
	wxString      m_starNet9Band;
	wxString      m_starNet9Callsign;
	wxString      m_starNet9Logoff;
	wxString      m_starNet9Info;
	wxString      m_starNet9Permanent;
	unsigned int  m_starNet9UserTimeout;
	unsigned int  m_starNet9GroupTimeout;
	STARNET_CALLSIGN_SWITCH m_starNet9CallsignSwitch;
	bool          m_starNet9TxMsgSwitch;
	wxString      m_starNet9Reflector;
	wxString      m_starNet10Band;
	wxString      m_starNet10Callsign;
	wxString      m_starNet10Logoff;
	wxString      m_starNet10Info;
	wxString      m_starNet10Permanent;
	unsigned int  m_starNet10UserTimeout;
	unsigned int  m_starNet10GroupTimeout;
	STARNET_CALLSIGN_SWITCH m_starNet10CallsignSwitch;
	bool          m_starNet10TxMsgSwitch;
	wxString      m_starNet10Reflector;
	wxString      m_starNet11Band;
	wxString      m_starNet11Callsign;
	wxString      m_starNet11Logoff;
	wxString      m_starNet11Info;
	wxString      m_starNet11Permanent;
	unsigned int  m_starNet11UserTimeout;
	unsigned int  m_starNet11GroupTimeout;
	STARNET_CALLSIGN_SWITCH m_starNet11CallsignSwitch;
	bool          m_starNet11TxMsgSwitch;
	wxString      m_starNet11Reflector;
	wxString      m_starNet12Band;
	wxString      m_starNet12Callsign;
	wxString      m_starNet12Logoff;
	wxString      m_starNet12Info;
	wxString      m_starNet12Permanent;
	unsigned int  m_starNet12UserTimeout;
	unsigned int  m_starNet12GroupTimeout;
	STARNET_CALLSIGN_SWITCH m_starNet12CallsignSwitch;
	bool          m_starNet12TxMsgSwitch;
	wxString      m_starNet12Reflector;
	wxString      m_starNet13Band;
	wxString      m_starNet13Callsign;
	wxString      m_starNet13Logoff;
	wxString      m_starNet13Info;
	wxString      m_starNet13Permanent;
	unsigned int  m_starNet13UserTimeout;
	unsigned int  m_starNet13GroupTimeout;
	STARNET_CALLSIGN_SWITCH m_starNet13CallsignSwitch;
	bool          m_starNet13TxMsgSwitch;
	wxString      m_starNet13Reflector;
	wxString      m_starNet14Band;
	wxString      m_starNet14Callsign;
	wxString      m_starNet14Logoff;
	wxString      m_starNet14Info;
	wxString      m_starNet14Permanent;
	unsigned int  m_starNet14UserTimeout;
	unsigned int  m_starNet14GroupTimeout;
	STARNET_CALLSIGN_SWITCH m_starNet14CallsignSwitch;
	bool          m_starNet14TxMsgSwitch;
	wxString      m_starNet14Reflector;
	wxString      m_starNet15Band;
	wxString      m_starNet15Callsign;
	wxString      m_starNet15Logoff;
	wxString      m_starNet15Info;
	wxString      m_starNet15Permanent;
	unsigned int  m_starNet15UserTimeout;
	unsigned int  m_starNet15GroupTimeout;
	STARNET_CALLSIGN_SWITCH m_starNet15CallsignSwitch;
	bool          m_starNet15TxMsgSwitch;
	wxString      m_starNet15Reflector;
	bool          m_remoteEnabled;
	wxString      m_remotePassword;
	unsigned int  m_remotePort;
	bool          m_logEnabled;
	int           m_x;
	int           m_y;
};

#endif
