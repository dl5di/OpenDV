/*
 *   Copyright (C) 2015 by Jonathan Naylor G4KLX
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

#if !defined(CONF_H)
#define	CONF_H

#include <string>

class CConf
{
public:
  CConf(const std::string& file);
  ~CConf();

  bool read();

  // The General section
  unsigned int getDMRId() const;
  unsigned int getDMRColorCode() const;
  std::string  getDStarCallsign() const;
  std::string  getFusionCallsign() const;
  bool         getDuplex() const;
  unsigned int getModeHang() const;

  bool         getDStarEnabled() const;
  bool         getDMREnabled() const;
  bool         getFusionEnabled() const;

  std::string  getLogPath() const;
  std::string  getLogRoot() const;
  unsigned int getLogLevel() const;
  bool         getLogDisplay() const;
  bool         getSourceDump() const;
  bool         getSinkDump() const;

  unsigned int getRxFrequency() const;
  unsigned int getTxFrequency() const;
  unsigned int getPower() const;
  float        getLatitude() const;
  float        getLongitude() const;
  int          getHeight() const;
  std::string  getLocation() const;
  std::string  getDescription() const;
  std::string  getURL() const;

  // The D-Star Network section
  unsigned int getDStarLocalPort() const;
  std::string  getDStarRemoteAddress() const;
  unsigned int getDStarRemotePort() const;

  // The DMR Network section
  std::string  getDMRNetworkAddress() const;
  unsigned int getDMRNetworkPort() const;
  std::string  getDMRNetworkPassword() const;

  // The System Fusion Network section
  std::string  getFusionNetworkAddress() const;
  unsigned int getFusionNetworkPort() const;

  // The Modem section
  std::string  getModemPort() const;
  bool         getModemRXInvert() const;
  bool         getModemTXInvert() const;
  bool         getModemPTTInvert() const;
  unsigned int getModemTXDelay() const;
  unsigned int getModemRXLevel() const;
  unsigned int getModemTXLevel() const;
  bool         getModemDebug() const;

  // The TFTSERIAL section
  std::string  getTFTSerialPort() const;

private:
  std::string  m_file;
  unsigned int m_dmrId;
  unsigned int m_dmrColorCode;
  std::string  m_dstarCallsign;
  std::string  m_fusionCallsign;
  bool         m_duplex;
  unsigned int m_modeHang;

  bool         m_dstarEnabled;
  bool         m_dmrEnabled;
  bool         m_fusionEnabled;

  std::string  m_logPath;
  std::string  m_logRoot;
  unsigned int m_logLevel;
  bool         m_logDisplay;
  bool         m_sourceDump;
  bool         m_sinkDump;

  unsigned int m_rxFrequency;
  unsigned int m_txFrequency;
  unsigned int m_power;
  float        m_latitude;
  float        m_longitude;
  int          m_height;
  std::string  m_location;
  std::string  m_description;
  std::string  m_url;

  unsigned int m_dstarLocalPort;
  std::string  m_dstarRemoteAddress;
  unsigned int m_dstarRemotePort;

  std::string  m_dmrNetworkAddress;
  unsigned int m_dmrNetworkPort;
  std::string  m_dmrNetworkPassword;

  unsigned int m_fusionNetworkPort;
  std::string  m_fusionNetworkAddress;

  std::string  m_modemPort;
  bool         m_modemRXInvert;
  bool         m_modemTXInvert;
  bool         m_modemPTTInvert;
  unsigned int m_modemTXDelay;
  unsigned int m_modemRXLevel;
  unsigned int m_modemTXLevel;
  bool         m_modemDebug;

  std::string  m_tftSerialPort;
};

#endif
