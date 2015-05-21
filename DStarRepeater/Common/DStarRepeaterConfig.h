/*
 *   Copyright (C) 2011-2015 by Jonathan Naylor G4KLX
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

#ifndef	DStarRepeaterConfig_H
#define	DStarRepeaterConfig_H

#include "DStarDefines.h"

#include <string>
#include <vector>

class CDStarRepeaterConfig {
public:
#if defined(WIN32)
	CDStarRepeaterConfig(wxConfigBase* config, const std::string& dir, const std::string& configName, const std::string& name);
#else
	CDStarRepeaterConfig(const std::string& dir, const std::string& configName, const std::string& name);
#endif
	~CDStarRepeaterConfig();

	void getCallsign(std::string& callsign, std::string& gateway, DSTAR_MODE& mode, ACK_TYPE& ack, bool& restriction, bool& rpt1Validation, bool& dtmfBlanking, bool& errorReply) const;
	void setCallsign(const std::string& callsign, const std::string& gateway, DSTAR_MODE mode, ACK_TYPE ack, bool restriction, bool rpt1Validation, bool dtmfBlanking, bool errorReply);

	void getNetwork(std::string& gatewayAddress, unsigned int& gatewayPort, std::string& localAddress, unsigned int& localPort, std::string& name) const;
	void setNetwork(const std::string& gatewayAddress, unsigned int gatewayPort, const std::string& localAddress, unsigned int localPort, const std::string& name);

	void getModem(std::string& type) const;
	void setModem(const std::string& type);

	void getTimes(unsigned int& timeout, unsigned int& ackTime) const;
	void setTimes(unsigned int timeout, unsigned int ackTime);

	void getBeacon(unsigned int& time, std::string& text, bool& voice, TEXT_LANG& language) const;
	void setBeacon(unsigned int time, const std::string& text, bool voice, TEXT_LANG language);

	void getAnnouncement(bool& enabled, unsigned int& time, std::string& recordRPT1, std::string& recordRPT2, std::string& deleteRPT1, std::string& deleteRPT2) const;
	void setAnnouncement(bool enabled, unsigned int time, const std::string& recordRPT1, const std::string& recordRPT2, const std::string& deleteRPT1, const std::string& deleteRPT2);

	void getControl(bool& enabled, std::string& rpt1Callsign, std::string& rpt2Callsign, std::string& shutdown, std::string& startup, std::string& status1, std::string& status2, std::string& status3, std::string& status4, std::string& status5, std::string& command1, std::string& command1Line, std::string& command2, std::string& command2Line, std::string& command5, std::string& command5Line, std::string& command6, std::string& command6Line, std::string& command3, std::string& command3Line, std::string& command4, std::string& command4Line, std::string& output1, std::string& output2, std::string& output3, std::string& output4) const;
	void setControl(bool enabled, const std::string& rpt1Callsign, const std::string& rpt2Callsign, const std::string& shutdown, const std::string& startup, const std::string& status1, const std::string& status2, const std::string& status3, const std::string& status4, const std::string& status5, const std::string& command1, const std::string& command1Line, const std::string& command2, const std::string& command2Line, const std::string& command3, const std::string& command3Line, const std::string& command4, const std::string& command4Line, const std::string& command5, const std::string& command5Line, const std::string& command6, const std::string& command6Line, const std::string& output1, const std::string& output2, const std::string& output3, const std::string& output4);

	void getController(std::string& type, unsigned int& serialConfig, bool& pttInvert, unsigned int& activeHangTime) const;
	void setController(const std::string& type, unsigned int serialConfig, bool pttInvert, unsigned int activeHangTime);

	void getOutputs(bool& out1, bool& out2, bool& out3, bool& out4) const;
	void setOutputs(bool out1, bool out2, bool out3, bool out4);

	void getLogging(bool& logging) const;
	void setLogging(bool logging);

	void getPosition(int& x, int& y) const;
	void setPosition(int x, int y);

	void getDVAP(std::string& port, unsigned int& frequency, int& power, int& squelch) const;
	void setDVAP(const std::string& port, unsigned int frequency, int power, int squelch);

	void getGMSK(USB_INTERFACE& type, unsigned int& address) const;
	void setGMSK(USB_INTERFACE type, unsigned int address);

	void getDVRPTR1(std::string& port, bool& rxInvert, bool& txInvert, bool& channel, unsigned int& modLevel, unsigned int& txDelay) const;
	void setDVRPTR1(const std::string& port, bool rxInvert, bool txInvert, bool channel, unsigned int modLevel, unsigned int txDelay);

	void getDVRPTR2(CONNECTION_TYPE& connectionType, std::string& usbPort, std::string& address, unsigned int& port, bool& txInvert, unsigned int& modLevel, unsigned int& txDelay) const;
	void setDVRPTR2(CONNECTION_TYPE connectionType, const std::string& usbPort, const std::string& address, unsigned int port, bool txInvert, unsigned int modLevel, unsigned int txDelay);

	void getDVRPTR3(CONNECTION_TYPE& connectionType, std::string& usbPort, std::string& address, unsigned int& port, bool& txInvert, unsigned int& modLevel, unsigned int& txDelay) const;
	void setDVRPTR3(CONNECTION_TYPE connectionType, const std::string& usbPort, const std::string& address, unsigned int port, bool txInvert, unsigned int modLevel, unsigned int txDelay);

	void getDVMEGA(std::string& port, DVMEGA_VARIANT& variant, bool& rxInvert, bool& txInvert, unsigned int& txDelay, unsigned int& rxFrequency, unsigned int& txFrequency, unsigned int& power) const;
	void setDVMEGA(const std::string& port, DVMEGA_VARIANT variant, bool rxInvert, bool txInvert, unsigned int txDelay, unsigned int rxFrequency, unsigned int txFrequency, unsigned int power);

	void getSoundCard(std::string& rxDevice, std::string& txDevice, bool& rxInvert, bool& txInvert, float& rxLevel, float& txLevel, unsigned int& txDelay, unsigned int& txTail) const;
	void setSoundCard(const std::string& rxDevice, const std::string& txDevice, bool rxInvert, bool txInvert, float rxLevel, float txLevel, unsigned int txDelay, unsigned int txTail);

	void getSplit(std::string& localAddress, unsigned int& localPort, std::vector<std::string>& transmitterNames, std::vector<std::string>& receiverNames, unsigned int& timeout) const;
	void setSplit(const std::string& localAddress, unsigned int localPort, const std::vector<std::string>& transmitterNames, const std::vector<std::string>& receiverNames, unsigned int timeout);

	bool write();

private:
#if defined(WIN32)
	wxConfigBase* m_config;
	std::string   m_name;
#endif
	wxFileName    m_fileName;
	std::string   m_callsign;
	std::string   m_gateway;
	DSTAR_MODE    m_mode;
	ACK_TYPE      m_ack;
	bool          m_restriction;
	bool          m_rpt1Validation;
	bool          m_dtmfBlanking;
	bool          m_errorReply;
	std::string   m_gatewayAddress;
	unsigned int  m_gatewayPort;
	std::string   m_localAddress;
	unsigned int  m_localPort;
	std::string   m_networkName;
	std::string   m_modemType;
	unsigned int  m_timeout;
	unsigned int  m_ackTime;
	unsigned int  m_beaconTime;
	std::string   m_beaconText;
	bool          m_beaconVoice;
	TEXT_LANG     m_language;
	bool          m_announcementEnabled;
	unsigned int  m_announcementTime;
	std::string   m_announcementRecordRPT1;
	std::string   m_announcementRecordRPT2;
	std::string   m_announcementDeleteRPT1;
	std::string   m_announcementDeleteRPT2;
	bool          m_controlEnabled;
	std::string   m_controlRpt1Callsign;
	std::string   m_controlRpt2Callsign;
	std::string   m_controlShutdown;
	std::string   m_controlStartup;
	std::string   m_controlStatus1;
	std::string   m_controlStatus2;
	std::string   m_controlStatus3;
	std::string   m_controlStatus4;
	std::string   m_controlStatus5;
	std::string   m_controlCommand1;
	std::string   m_controlCommand1Line;
	std::string   m_controlCommand2;
	std::string   m_controlCommand2Line;
	std::string   m_controlCommand3;
	std::string   m_controlCommand3Line;
	std::string   m_controlCommand4;
	std::string   m_controlCommand4Line;
	std::string   m_controlCommand5;
	std::string   m_controlCommand5Line;
	std::string   m_controlCommand6;
	std::string   m_controlCommand6Line;
	std::string   m_controlOutput1;
	std::string   m_controlOutput2;
	std::string   m_controlOutput3;
	std::string   m_controlOutput4;
	std::string   m_controllerType;
	unsigned int  m_serialConfig;
	bool          m_pttInvert;
	unsigned int  m_activeHangTime;
	bool          m_output1;
	bool          m_output2;
	bool          m_output3;
	bool          m_output4;
	bool          m_logging;
	int           m_x;
	int           m_y;

	// DVAP
	std::string   m_dvapPort;
	unsigned int  m_dvapFrequency;
	int           m_dvapPower;
	int           m_dvapSquelch;

	// GMSK
	USB_INTERFACE m_gmskInterface;
	unsigned int  m_gmskAddress;

	// DV-RPTR 1
	std::string   m_dvrptr1Port;
	bool          m_dvrptr1RXInvert;
	bool          m_dvrptr1TXInvert;
	bool          m_dvrptr1Channel;
	unsigned int  m_dvrptr1ModLevel;
	unsigned int  m_dvrptr1TXDelay;

	// DV-RPTR 2
	CONNECTION_TYPE m_dvrptr2Connection;
	std::string     m_dvrptr2USBPort;
	std::string     m_dvrptr2Address;
	unsigned int    m_dvrptr2Port;
	bool            m_dvrptr2TXInvert;
	unsigned int    m_dvrptr2ModLevel;
	unsigned int    m_dvrptr2TXDelay;

	// DV-RPTR 3
	CONNECTION_TYPE m_dvrptr3Connection;
	std::string     m_dvrptr3USBPort;
	std::string     m_dvrptr3Address;
	unsigned int    m_dvrptr3Port;
	bool            m_dvrptr3TXInvert;
	unsigned int    m_dvrptr3ModLevel;
	unsigned int    m_dvrptr3TXDelay;

	// DVMEGA
	std::string    m_dvmegaPort;
	DVMEGA_VARIANT m_dvmegaVariant;
	bool           m_dvmegaRXInvert;
	bool           m_dvmegaTXInvert;
	unsigned int   m_dvmegaTXDelay;
	unsigned int   m_dvmegaRXFrequency;
	unsigned int   m_dvmegaTXFrequency;
	unsigned int   m_dvmegaPower;

	// Sound Card
	std::string   m_soundCardRXDevice;
	std::string   m_soundCardTXDevice;
	bool          m_soundCardRXInvert;
	bool          m_soundCardTXInvert;
	float         m_soundCardRXLevel;
	float         m_soundCardTXLevel;
	unsigned int  m_soundCardTXDelay;
	unsigned int  m_soundCardTXTail;

	// Split
	std::string              m_splitLocalAddress;
	unsigned int             m_splitLocalPort;
	std::vector<std::string> m_splitTXNames;
	std::vector<std::string> m_splitRXNames;
	unsigned int             m_splitTimeout;
};

#endif
