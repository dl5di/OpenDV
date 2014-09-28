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

#include "IRCDDBGatewayConfig.h"

#include <wx/textfile.h>

const wxString  KEY_GATEWAY_TYPE         = wxT("gatewayType");
const wxString  KEY_GATEWAY_CALLSIGN     = wxT("gatewayCallsign");
const wxString  KEY_GATEWAY_ADDRESS      = wxT("gatewayAddress");
const wxString  KEY_ICOM_ADDRESS         = wxT("icomAddress");
const wxString  KEY_ICOM_PORT            = wxT("icomPort");
const wxString  KEY_HB_ADDRESS           = wxT("hbAddress");
const wxString  KEY_HB_PORT              = wxT("hbPort");
const wxString  KEY_LATITUDE             = wxT("latitude");
const wxString  KEY_LONGITUDE            = wxT("longitude");
const wxString  KEY_DESCRIPTION1         = wxT("description1");
const wxString  KEY_DESCRIPTION2         = wxT("description2");
const wxString  KEY_URL                  = wxT("url");
const wxString  KEY_REPEATER_CALL1       = wxT("repeaterCall1");
const wxString  KEY_REPEATER_BAND1       = wxT("repeaterBand1");
const wxString  KEY_REPEATER_TYPE1       = wxT("repeaterType1");
const wxString  KEY_REPEATER_ADDRESS1    = wxT("repeaterAddress1");
const wxString  KEY_REPEATER_PORT1       = wxT("repeaterPort1");
const wxString  KEY_REFLECTOR1           = wxT("reflector1");
const wxString  KEY_ATSTARTUP1           = wxT("atStartup1");
const wxString  KEY_RECONNECT1           = wxT("reconnect1");
const wxString  KEY_FREQUENCY1           = wxT("frequency1");
const wxString  KEY_OFFSET1              = wxT("offset1");
const wxString  KEY_RANGE1               = wxT("rangeKms1");
const wxString  KEY_LATITUDE1            = wxT("latitude1");
const wxString  KEY_LONGITUDE1           = wxT("longitude1");
const wxString  KEY_AGL1                 = wxT("agl1");
const wxString  KEY_DESCRIPTION11        = wxT("description1_1");
const wxString  KEY_DESCRIPTION12        = wxT("description1_2");
const wxString  KEY_URL1                 = wxT("url1");
const wxString  KEY_BAND11               = wxT("band1_1");
const wxString  KEY_BAND12               = wxT("band1_2");
const wxString  KEY_BAND13               = wxT("band1_3");
const wxString  KEY_REPEATER_CALL2       = wxT("repeaterCall2");
const wxString  KEY_REPEATER_BAND2       = wxT("repeaterBand2");
const wxString  KEY_REPEATER_TYPE2       = wxT("repeaterType2");
const wxString  KEY_REPEATER_ADDRESS2    = wxT("repeaterAddress2");
const wxString  KEY_REPEATER_PORT2       = wxT("repeaterPort2");
const wxString  KEY_REFLECTOR2           = wxT("reflector2");
const wxString  KEY_ATSTARTUP2           = wxT("atStartup2");
const wxString  KEY_RECONNECT2           = wxT("reconnect2");
const wxString  KEY_FREQUENCY2           = wxT("frequency2");
const wxString  KEY_OFFSET2              = wxT("offset2");
const wxString  KEY_RANGE2               = wxT("rangeKms2");
const wxString  KEY_LATITUDE2            = wxT("latitude2");
const wxString  KEY_LONGITUDE2           = wxT("longitude2");
const wxString  KEY_AGL2                 = wxT("agl2");
const wxString  KEY_DESCRIPTION21        = wxT("description2_1");
const wxString  KEY_DESCRIPTION22        = wxT("description2_2");
const wxString  KEY_URL2                 = wxT("url2");
const wxString  KEY_BAND21               = wxT("band2_1");
const wxString  KEY_BAND22               = wxT("band2_2");
const wxString  KEY_BAND23               = wxT("band2_3");
const wxString  KEY_REPEATER_CALL3       = wxT("repeaterCall3");
const wxString  KEY_REPEATER_BAND3       = wxT("repeaterBand3");
const wxString  KEY_REPEATER_TYPE3       = wxT("repeaterType3");
const wxString  KEY_REPEATER_ADDRESS3    = wxT("repeaterAddress3");
const wxString  KEY_REPEATER_PORT3       = wxT("repeaterPort3");
const wxString  KEY_REFLECTOR3           = wxT("reflector3");
const wxString  KEY_ATSTARTUP3           = wxT("atStartup3");
const wxString  KEY_RECONNECT3           = wxT("reconnect3");
const wxString  KEY_FREQUENCY3           = wxT("frequency3");
const wxString  KEY_OFFSET3              = wxT("offset3");
const wxString  KEY_RANGE3               = wxT("rangeKms3");
const wxString  KEY_LATITUDE3            = wxT("latitude3");
const wxString  KEY_LONGITUDE3           = wxT("longitude3");
const wxString  KEY_AGL3                 = wxT("agl3");
const wxString  KEY_DESCRIPTION31        = wxT("description3_1");
const wxString  KEY_DESCRIPTION32        = wxT("description3_2");
const wxString  KEY_URL3                 = wxT("url3");
const wxString  KEY_BAND31               = wxT("band3_1");
const wxString  KEY_BAND32               = wxT("band3_2");
const wxString  KEY_BAND33               = wxT("band3_3");
const wxString  KEY_REPEATER_CALL4       = wxT("repeaterCall4");
const wxString  KEY_REPEATER_BAND4       = wxT("repeaterBand4");
const wxString  KEY_REPEATER_TYPE4       = wxT("repeaterType4");
const wxString  KEY_REPEATER_ADDRESS4    = wxT("repeaterAddress4");
const wxString  KEY_REPEATER_PORT4       = wxT("repeaterPort4");
const wxString  KEY_REFLECTOR4           = wxT("reflector4");
const wxString  KEY_ATSTARTUP4           = wxT("atStartup4");
const wxString  KEY_RECONNECT4           = wxT("reconnect4");
const wxString  KEY_FREQUENCY4           = wxT("frequency4");
const wxString  KEY_OFFSET4              = wxT("offset4");
const wxString  KEY_RANGE4               = wxT("rangeKms4");
const wxString  KEY_LATITUDE4            = wxT("latitude4");
const wxString  KEY_LONGITUDE4           = wxT("longitude4");
const wxString  KEY_AGL4                 = wxT("agl4");
const wxString  KEY_DESCRIPTION41        = wxT("description4_1");
const wxString  KEY_DESCRIPTION42        = wxT("description4_2");
const wxString  KEY_URL4                 = wxT("url4");
const wxString  KEY_BAND41               = wxT("band4_1");
const wxString  KEY_BAND42               = wxT("band4_2");
const wxString  KEY_BAND43               = wxT("band4_3");
const wxString  KEY_IRCDDB_ENABLED       = wxT("ircddbEnabled");
const wxString  KEY_IRCDDB_HOSTNAME      = wxT("ircddbHostname");
const wxString  KEY_IRCDDB_USERNAME      = wxT("ircddbUsername");
const wxString  KEY_IRCDDB_PASSWORD      = wxT("ircddbPassword");
const wxString  KEY_APRS_ENABLED         = wxT("aprsEnabled");
const wxString  KEY_APRS_HOSTNAME        = wxT("aprsHostname");
const wxString  KEY_APRS_PORT            = wxT("aprsPort");
const wxString  KEY_DEXTRA_ENABLED       = wxT("dextraEnabled");
const wxString  KEY_DEXTRA_MAXDONGLES    = wxT("dextraMaxDongles");
const wxString  KEY_DPLUS_ENABLED        = wxT("dplusEnabled");
const wxString  KEY_DPLUS_MAXDONGLES     = wxT("dplusMaxDongles");
const wxString  KEY_DPLUS_LOGIN          = wxT("dplusLogin");
const wxString  KEY_DCS_ENABLED          = wxT("dcsEnabled");
const wxString  KEY_CCS_ENABLED          = wxT("ccsEnabled");
const wxString  KEY_CCS_HOST             = wxT("ccsHost");
const wxString  KEY_STARNET_BAND1            = wxT("starNetBand1");
const wxString  KEY_STARNET_CALLSIGN1        = wxT("starNetCallsign1");
const wxString  KEY_STARNET_LOGOFF1          = wxT("starNetLogoff1");
const wxString  KEY_STARNET_INFO1            = wxT("starNetInfo1");
const wxString  KEY_STARNET_PERMANENT1       = wxT("starNetPermanent1");
const wxString  KEY_STARNET_USER_TIMEOUT1    = wxT("starNetUserTimeout1");
const wxString  KEY_STARNET_GROUP_TIMEOUT1   = wxT("starNetGroupTimeout1");
const wxString  KEY_STARNET_CALLSIGN_SWITCH1 = wxT("starNetCallsignSwitch1");
const wxString  KEY_STARNET_TXMSG_SWITCH1    = wxT("starNetTXMsgSwitch1");
const wxString  KEY_STARNET_REFLECTOR1       = wxT("starNetReflector1");		// DEXTRA_LINK
const wxString  KEY_STARNET_BAND2            = wxT("starNetBand2");
const wxString  KEY_STARNET_CALLSIGN2        = wxT("starNetCallsign2");
const wxString  KEY_STARNET_LOGOFF2          = wxT("starNetLogoff2");
const wxString  KEY_STARNET_INFO2            = wxT("starNetInfo2");
const wxString  KEY_STARNET_PERMANENT2       = wxT("starNetPermanent2");
const wxString  KEY_STARNET_USER_TIMEOUT2    = wxT("starNetUserTimeout2");
const wxString  KEY_STARNET_GROUP_TIMEOUT2   = wxT("starNetGroupTimeout2");
const wxString  KEY_STARNET_CALLSIGN_SWITCH2 = wxT("starNetCallsignSwitch2");
const wxString  KEY_STARNET_TXMSG_SWITCH2    = wxT("starNetTXMsgSwitch2");
const wxString  KEY_STARNET_REFLECTOR2       = wxT("starNetReflector2");		// DEXTRA_LINK
const wxString  KEY_STARNET_BAND3            = wxT("starNetBand3");
const wxString  KEY_STARNET_CALLSIGN3        = wxT("starNetCallsign3");
const wxString  KEY_STARNET_LOGOFF3          = wxT("starNetLogoff3");
const wxString  KEY_STARNET_INFO3            = wxT("starNetInfo3");
const wxString  KEY_STARNET_PERMANENT3       = wxT("starNetPermanent3");
const wxString  KEY_STARNET_USER_TIMEOUT3    = wxT("starNetUserTimeout3");
const wxString  KEY_STARNET_GROUP_TIMEOUT3   = wxT("starNetGroupTimeout3");
const wxString  KEY_STARNET_CALLSIGN_SWITCH3 = wxT("starNetCallsignSwitch3");
const wxString  KEY_STARNET_TXMSG_SWITCH3    = wxT("starNetTXMsgSwitch3");
const wxString  KEY_STARNET_REFLECTOR3       = wxT("starNetReflector3");		// DEXTRA_LINK
const wxString  KEY_STARNET_BAND4            = wxT("starNetBand4");
const wxString  KEY_STARNET_CALLSIGN4        = wxT("starNetCallsign4");
const wxString  KEY_STARNET_LOGOFF4          = wxT("starNetLogoff4");
const wxString  KEY_STARNET_INFO4            = wxT("starNetInfo4");
const wxString  KEY_STARNET_PERMANENT4       = wxT("starNetPermanent4");
const wxString  KEY_STARNET_USER_TIMEOUT4    = wxT("starNetUserTimeout4");
const wxString  KEY_STARNET_GROUP_TIMEOUT4   = wxT("starNetGroupTimeout4");
const wxString  KEY_STARNET_CALLSIGN_SWITCH4 = wxT("starNetCallsignSwitch4");
const wxString  KEY_STARNET_TXMSG_SWITCH4    = wxT("starNetTXMsgSwitch4");
const wxString  KEY_STARNET_REFLECTOR4       = wxT("starNetReflector4");		// DEXTRA_LINK
const wxString  KEY_STARNET_BAND5            = wxT("starNetBand5");
const wxString  KEY_STARNET_CALLSIGN5        = wxT("starNetCallsign5");
const wxString  KEY_STARNET_LOGOFF5          = wxT("starNetLogoff5");
const wxString  KEY_STARNET_INFO5            = wxT("starNetInfo5");
const wxString  KEY_STARNET_PERMANENT5       = wxT("starNetPermanent5");
const wxString  KEY_STARNET_USER_TIMEOUT5    = wxT("starNetUserTimeout5");
const wxString  KEY_STARNET_GROUP_TIMEOUT5   = wxT("starNetGroupTimeout5");
const wxString  KEY_STARNET_CALLSIGN_SWITCH5 = wxT("starNetCallsignSwitch5");
const wxString  KEY_STARNET_TXMSG_SWITCH5    = wxT("starNetTXMsgSwitch5");
const wxString  KEY_STARNET_REFLECTOR5       = wxT("starNetReflector5");		// DEXTRA_LINK
const wxString  KEY_REMOTE_ENABLED           = wxT("remoteEnabled");
const wxString  KEY_REMOTE_PASSWORD          = wxT("remotePassword");
const wxString  KEY_REMOTE_PORT              = wxT("remotePort");
const wxString  KEY_LANGUAGE                 = wxT("language");
const wxString  KEY_INFO_ENABLED             = wxT("infoEnabled");
const wxString  KEY_ECHO_ENABLED             = wxT("echoEnabled");
const wxString  KEY_LOG_ENABLED              = wxT("logEnabled");
const wxString  KEY_DRATS_ENABLED            = wxT("dratsEnabled");
const wxString  KEY_DTMF_ENABLED             = wxT("dtmfEnabled");
const wxString  KEY_WINDOW_X                 = wxT("windowX");
const wxString  KEY_WINDOW_Y                 = wxT("windowY");

const GATEWAY_TYPE DEFAULT_GATEWAY_TYPE          = GT_REPEATER;
const wxString     DEFAULT_GATEWAY_CALLSIGN      = wxEmptyString;
const wxString     DEFAULT_GATEWAY_ADDRESS       = wxEmptyString;
const wxString     DEFAULT_ICOM_ADDRESS          = wxT("172.16.0.20");
const unsigned int DEFAULT_ICOM_PORT             = 20000U;
const wxString     DEFAULT_HB_ADDRESS            = wxT("127.0.0.1");
const unsigned int DEFAULT_HB_PORT               = 20010U;
const double       DEFAULT_LATITUDE              = 0.0;
const double       DEFAULT_LONGITUDE             = 0.0;
const wxString     DEFAULT_DESCRIPTION1          = wxEmptyString;
const wxString     DEFAULT_DESCRIPTION2          = wxEmptyString;
const wxString     DEFAULT_URL                   = wxEmptyString;
const wxString     DEFAULT_REPEATER_CALL         = wxEmptyString;
const wxString     DEFAULT_REPEATER_BAND         = wxT(" ");
const HW_TYPE      DEFAULT_REPEATER_TYPE         = HW_HOMEBREW;
const wxString     DEFAULT_REPEATER_ADDRESS      = wxT("127.0.0.1");
const wxString     DEFAULT_REFLECTOR             = wxEmptyString;
const bool         DEFAULT_ATSTARTUP             = false;
const RECONNECT    DEFAULT_RECONNECT             = RECONNECT_NEVER;
const double       DEFAULT_FREQUENCY             = 0.0;
const double       DEFAULT_OFFSET                = 0.0;
const double       DEFAULT_RANGE                 = 0.0;
const double       DEFAULT_AGL                   = 0.0;
const unsigned char DEFAULT_BAND1                = 0x00U;
const unsigned char DEFAULT_BAND2                = 0x00U;
const unsigned char DEFAULT_BAND3                = 0x00U;
const unsigned int DEFAULT_REPEATER_PORT1        = 20011U;
const unsigned int DEFAULT_REPEATER_PORT2        = 20012U;
const unsigned int DEFAULT_REPEATER_PORT3        = 20013U;
const unsigned int DEFAULT_REPEATER_PORT4        = 20014U;
const bool         DEFAULT_IRCDDB_ENABLED        = true;
const wxString     DEFAULT_IRCDDB_HOSTNAME       = wxT("group1-irc.ircddb.net");
const wxString     DEFAULT_IRCDDB_USERNAME       = wxEmptyString;
const wxString     DEFAULT_IRCDDB_PASSWORD       = wxEmptyString;
const bool         DEFAULT_APRS_ENABLED          = true;
const wxString     DEFAULT_APRS_HOSTNAME         = wxT("rotate.aprs2.net");
const unsigned int DEFAULT_APRS_PORT             = 14580U;
const bool         DEFAULT_DEXTRA_ENABLED        = true;
const unsigned int DEFAULT_DEXTRA_MAXDONGLES     = 5U;
const bool         DEFAULT_DPLUS_ENABLED         = false;
const unsigned int DEFAULT_DPLUS_MAXDONGLES      = 5U;
const wxString     DEFAULT_DPLUS_LOGIN           = wxEmptyString;
const bool         DEFAULT_DCS_ENABLED           = true;
const bool         DEFAULT_CCS_ENABLED           = true;
const wxString     DEFAULT_CCS_HOST              = wxT("CCS004  ");
const wxString     DEFAULT_STARNET_BAND          = wxEmptyString;
const wxString     DEFAULT_STARNET_CALLSIGN      = wxEmptyString;
const wxString     DEFAULT_STARNET_LOGOFF        = wxEmptyString;
const wxString     DEFAULT_STARNET_INFO          = wxEmptyString;
const wxString     DEFAULT_STARNET_PERMANENT     = wxEmptyString;
const unsigned int DEFAULT_STARNET_USER_TIMEOUT  = 300U;
const unsigned int DEFAULT_STARNET_GROUP_TIMEOUT = 300U;
const STARNET_CALLSIGN_SWITCH DEFAULT_STARNET_CALLSIGN_SWITCH = SCS_GROUP_CALLSIGN;
const bool         DEFAULT_STARNET_TXMSG_SWITCH  = true;
const wxString     DEFAULT_STARNET_REFLECTOR     = wxEmptyString;
const bool         DEFAULT_REMOTE_ENABLED        = false;
const wxString     DEFAULT_REMOTE_PASSWORD       = wxEmptyString;
const unsigned int DEFAULT_REMOTE_PORT           = 0U;
const TEXT_LANG    DEFAULT_LANGUAGE              = TL_ENGLISH_UK;
const bool         DEFAULT_LOG_ENABLED           = false;
const bool         DEFAULT_INFO_ENABLED          = true;
const bool         DEFAULT_ECHO_ENABLED          = true;
const bool         DEFAULT_DRATS_ENABLED         = false;
const bool         DEFAULT_DTMF_ENABLED          = true;
const int          DEFAULT_WINDOW_X              = -1;
const int          DEFAULT_WINDOW_Y              = -1;


#if defined(__WINDOWS__)

CIRCDDBGatewayConfig::CIRCDDBGatewayConfig(wxConfigBase* config, const wxString& dir, const wxString& configName, const wxString& name) :
m_config(config),
m_name(wxT("/")),
m_fileName(),
m_type(DEFAULT_GATEWAY_TYPE),
m_callsign(DEFAULT_GATEWAY_CALLSIGN),
m_address(DEFAULT_GATEWAY_ADDRESS),
m_icomAddress(DEFAULT_ICOM_ADDRESS),
m_icomPort(DEFAULT_ICOM_PORT),
m_hbAddress(DEFAULT_HB_ADDRESS),
m_hbPort(DEFAULT_HB_PORT),
m_latitude(DEFAULT_LATITUDE),
m_longitude(DEFAULT_LONGITUDE),
m_description1(DEFAULT_DESCRIPTION1),
m_description2(DEFAULT_DESCRIPTION2),
m_url(DEFAULT_URL),
m_repeater1Callsign(DEFAULT_REPEATER_CALL),
m_repeater1Band(DEFAULT_REPEATER_BAND),
m_repeater1Type(DEFAULT_REPEATER_TYPE),
m_repeater1Address(DEFAULT_REPEATER_ADDRESS),
m_repeater1Port(DEFAULT_REPEATER_PORT1),
m_repeater1Reflector(DEFAULT_REFLECTOR),
m_repeater1AtStartup(DEFAULT_ATSTARTUP),
m_repeater1Reconnect(DEFAULT_RECONNECT),
m_repeater1Frequency(DEFAULT_FREQUENCY),
m_repeater1Offset(DEFAULT_OFFSET),
m_repeater1Range(DEFAULT_RANGE),
m_repeater1Latitude(DEFAULT_LATITUDE),
m_repeater1Longitude(DEFAULT_LONGITUDE),
m_repeater1Agl(DEFAULT_AGL),
m_repeater1Description1(DEFAULT_DESCRIPTION1),
m_repeater1Description2(DEFAULT_DESCRIPTION2),
m_repeater1URL(DEFAULT_URL),
m_repeater1Band1(DEFAULT_BAND1),
m_repeater1Band2(DEFAULT_BAND2),
m_repeater1Band3(DEFAULT_BAND2),
m_repeater2Callsign(DEFAULT_REPEATER_CALL),
m_repeater2Band(DEFAULT_REPEATER_BAND),
m_repeater2Type(DEFAULT_REPEATER_TYPE),
m_repeater2Address(DEFAULT_REPEATER_ADDRESS),
m_repeater2Port(DEFAULT_REPEATER_PORT2),
m_repeater2Reflector(DEFAULT_REFLECTOR),
m_repeater2AtStartup(DEFAULT_ATSTARTUP),
m_repeater2Reconnect(DEFAULT_RECONNECT),
m_repeater2Frequency(DEFAULT_FREQUENCY),
m_repeater2Offset(DEFAULT_OFFSET),
m_repeater2Range(DEFAULT_RANGE),
m_repeater2Latitude(DEFAULT_LATITUDE),
m_repeater2Longitude(DEFAULT_LONGITUDE),
m_repeater2Agl(DEFAULT_AGL),
m_repeater2Description1(DEFAULT_DESCRIPTION1),
m_repeater2Description2(DEFAULT_DESCRIPTION2),
m_repeater2URL(DEFAULT_URL),
m_repeater2Band1(DEFAULT_BAND1),
m_repeater2Band2(DEFAULT_BAND2),
m_repeater2Band3(DEFAULT_BAND3),
m_repeater3Callsign(DEFAULT_REPEATER_CALL),
m_repeater3Band(DEFAULT_REPEATER_BAND),
m_repeater3Type(DEFAULT_REPEATER_TYPE),
m_repeater3Address(DEFAULT_REPEATER_ADDRESS),
m_repeater3Port(DEFAULT_REPEATER_PORT3),
m_repeater3Reflector(DEFAULT_REFLECTOR),
m_repeater3AtStartup(DEFAULT_ATSTARTUP),
m_repeater3Reconnect(DEFAULT_RECONNECT),
m_repeater3Frequency(DEFAULT_FREQUENCY),
m_repeater3Offset(DEFAULT_OFFSET),
m_repeater3Range(DEFAULT_RANGE),
m_repeater3Latitude(DEFAULT_LATITUDE),
m_repeater3Longitude(DEFAULT_LONGITUDE),
m_repeater3Agl(DEFAULT_AGL),
m_repeater3Description1(DEFAULT_DESCRIPTION1),
m_repeater3Description2(DEFAULT_DESCRIPTION2),
m_repeater3URL(DEFAULT_URL),
m_repeater3Band1(DEFAULT_BAND1),
m_repeater3Band2(DEFAULT_BAND2),
m_repeater3Band3(DEFAULT_BAND3),
m_repeater4Callsign(DEFAULT_REPEATER_CALL),
m_repeater4Band(DEFAULT_REPEATER_BAND),
m_repeater4Type(DEFAULT_REPEATER_TYPE),
m_repeater4Address(DEFAULT_REPEATER_ADDRESS),
m_repeater4Port(DEFAULT_REPEATER_PORT4),
m_repeater4Reflector(DEFAULT_REFLECTOR),
m_repeater4AtStartup(DEFAULT_ATSTARTUP),
m_repeater4Reconnect(DEFAULT_RECONNECT),
m_repeater4Frequency(DEFAULT_FREQUENCY),
m_repeater4Offset(DEFAULT_OFFSET),
m_repeater4Range(DEFAULT_RANGE),
m_repeater4Latitude(DEFAULT_LATITUDE),
m_repeater4Longitude(DEFAULT_LONGITUDE),
m_repeater4Agl(DEFAULT_AGL),
m_repeater4Description1(DEFAULT_DESCRIPTION1),
m_repeater4Description2(DEFAULT_DESCRIPTION2),
m_repeater4URL(DEFAULT_URL),
m_repeater4Band1(DEFAULT_BAND1),
m_repeater4Band2(DEFAULT_BAND2),
m_repeater4Band3(DEFAULT_BAND3),
m_ircddbEnabled(DEFAULT_IRCDDB_ENABLED),
m_ircddbHostname(DEFAULT_IRCDDB_HOSTNAME),
m_ircddbUsername(DEFAULT_IRCDDB_USERNAME),
m_ircddbPassword(DEFAULT_IRCDDB_PASSWORD),
m_aprsEnabled(DEFAULT_APRS_ENABLED),
m_aprsHostname(DEFAULT_APRS_HOSTNAME),
m_aprsPort(DEFAULT_APRS_PORT),
m_dextraEnabled(DEFAULT_DEXTRA_ENABLED),
m_dextraMaxDongles(DEFAULT_DEXTRA_MAXDONGLES),
m_dplusEnabled(DEFAULT_DPLUS_ENABLED),
m_dplusMaxDongles(DEFAULT_DPLUS_MAXDONGLES),
m_dplusLogin(DEFAULT_DPLUS_LOGIN),
m_dcsEnabled(DEFAULT_DCS_ENABLED),
m_ccsEnabled(DEFAULT_CCS_ENABLED),
m_ccsHost(DEFAULT_CCS_HOST),
m_starNet1Band(DEFAULT_STARNET_BAND),
m_starNet1Callsign(DEFAULT_STARNET_CALLSIGN),
m_starNet1Logoff(DEFAULT_STARNET_LOGOFF),
m_starNet1Info(DEFAULT_STARNET_INFO),
m_starNet1Permanent(DEFAULT_STARNET_PERMANENT),
m_starNet1UserTimeout(DEFAULT_STARNET_USER_TIMEOUT),
m_starNet1GroupTimeout(DEFAULT_STARNET_GROUP_TIMEOUT),
m_starNet1CallsignSwitch(DEFAULT_STARNET_CALLSIGN_SWITCH),
m_starNet1TxMsgSwitch(DEFAULT_STARNET_TXMSG_SWITCH),
m_starNet1Reflector(DEFAULT_STARNET_REFLECTOR),
m_starNet2Band(DEFAULT_STARNET_BAND),
m_starNet2Callsign(DEFAULT_STARNET_CALLSIGN),
m_starNet2Logoff(DEFAULT_STARNET_LOGOFF),
m_starNet2Info(DEFAULT_STARNET_INFO),
m_starNet2Permanent(DEFAULT_STARNET_PERMANENT),
m_starNet2UserTimeout(DEFAULT_STARNET_USER_TIMEOUT),
m_starNet2GroupTimeout(DEFAULT_STARNET_GROUP_TIMEOUT),
m_starNet2CallsignSwitch(DEFAULT_STARNET_CALLSIGN_SWITCH),
m_starNet2TxMsgSwitch(DEFAULT_STARNET_TXMSG_SWITCH),
m_starNet2Reflector(DEFAULT_STARNET_REFLECTOR),
m_starNet3Band(DEFAULT_STARNET_BAND),
m_starNet3Callsign(DEFAULT_STARNET_CALLSIGN),
m_starNet3Logoff(DEFAULT_STARNET_LOGOFF),
m_starNet3Info(DEFAULT_STARNET_INFO),
m_starNet3Permanent(DEFAULT_STARNET_PERMANENT),
m_starNet3UserTimeout(DEFAULT_STARNET_USER_TIMEOUT),
m_starNet3GroupTimeout(DEFAULT_STARNET_GROUP_TIMEOUT),
m_starNet3CallsignSwitch(DEFAULT_STARNET_CALLSIGN_SWITCH),
m_starNet3TxMsgSwitch(DEFAULT_STARNET_TXMSG_SWITCH),
m_starNet3Reflector(DEFAULT_STARNET_REFLECTOR),
m_starNet4Band(DEFAULT_STARNET_BAND),
m_starNet4Callsign(DEFAULT_STARNET_CALLSIGN),
m_starNet4Logoff(DEFAULT_STARNET_LOGOFF),
m_starNet4Info(DEFAULT_STARNET_INFO),
m_starNet4Permanent(DEFAULT_STARNET_PERMANENT),
m_starNet4UserTimeout(DEFAULT_STARNET_USER_TIMEOUT),
m_starNet4GroupTimeout(DEFAULT_STARNET_GROUP_TIMEOUT),
m_starNet4CallsignSwitch(DEFAULT_STARNET_CALLSIGN_SWITCH),
m_starNet4TxMsgSwitch(DEFAULT_STARNET_TXMSG_SWITCH),
m_starNet4Reflector(DEFAULT_STARNET_REFLECTOR),
m_starNet5Band(DEFAULT_STARNET_BAND),
m_starNet5Callsign(DEFAULT_STARNET_CALLSIGN),
m_starNet5Logoff(DEFAULT_STARNET_LOGOFF),
m_starNet5Info(DEFAULT_STARNET_INFO),
m_starNet5Permanent(DEFAULT_STARNET_PERMANENT),
m_starNet5UserTimeout(DEFAULT_STARNET_USER_TIMEOUT),
m_starNet5GroupTimeout(DEFAULT_STARNET_GROUP_TIMEOUT),
m_starNet5CallsignSwitch(DEFAULT_STARNET_CALLSIGN_SWITCH),
m_starNet5TxMsgSwitch(DEFAULT_STARNET_TXMSG_SWITCH),
m_starNet5Reflector(DEFAULT_STARNET_REFLECTOR),
m_remoteEnabled(DEFAULT_REMOTE_ENABLED),
m_remotePassword(DEFAULT_REMOTE_PASSWORD),
m_remotePort(DEFAULT_REMOTE_PORT),
m_language(DEFAULT_LANGUAGE),
m_infoEnabled(DEFAULT_INFO_ENABLED),
m_echoEnabled(DEFAULT_ECHO_ENABLED),
m_logEnabled(DEFAULT_LOG_ENABLED),
m_dratsEnabled(DEFAULT_DRATS_ENABLED),
m_dtmfEnabled(DEFAULT_DTMF_ENABLED),
m_x(DEFAULT_WINDOW_X),
m_y(DEFAULT_WINDOW_Y)
{
	wxASSERT(config != NULL);
	wxASSERT(!dir.IsEmpty());

	wxString fileName = configName;
	if (!name.IsEmpty()) {
		fileName = configName + wxT("_") + name;
		m_name = wxT("/") + name + wxT("/");
	}

	m_fileName.Assign(dir, fileName);

	long temp;

	m_config->Read(m_name + KEY_GATEWAY_TYPE, &temp, long(DEFAULT_GATEWAY_TYPE));
	m_type = GATEWAY_TYPE(temp);

	m_config->Read(m_name + KEY_GATEWAY_CALLSIGN, &m_callsign, DEFAULT_GATEWAY_CALLSIGN);

	m_config->Read(m_name + KEY_GATEWAY_ADDRESS, &m_address, DEFAULT_GATEWAY_ADDRESS);

	m_config->Read(m_name + KEY_ICOM_ADDRESS, &m_icomAddress, DEFAULT_ICOM_ADDRESS);

	m_config->Read(m_name + KEY_ICOM_PORT, &temp, long(DEFAULT_ICOM_PORT));
	m_icomPort = (unsigned int)temp;

	m_config->Read(m_name + KEY_HB_ADDRESS, &m_hbAddress, DEFAULT_HB_ADDRESS);

	m_config->Read(m_name + KEY_HB_PORT, &temp, long(DEFAULT_HB_PORT));
	m_hbPort = (unsigned int)temp;

	m_config->Read(m_name + KEY_LATITUDE, &m_latitude, DEFAULT_LATITUDE);

	m_config->Read(m_name + KEY_LONGITUDE, &m_longitude, DEFAULT_LONGITUDE);

	m_config->Read(m_name + KEY_DESCRIPTION1, &m_description1, DEFAULT_DESCRIPTION1);

	m_config->Read(m_name + KEY_DESCRIPTION2, &m_description2, DEFAULT_DESCRIPTION2);

	m_config->Read(m_name + KEY_URL, &m_url, DEFAULT_URL);

	m_config->Read(m_name + KEY_REPEATER_CALL1, &m_repeater1Callsign, DEFAULT_REPEATER_CALL);

	m_config->Read(m_name + KEY_REPEATER_BAND1, &m_repeater1Band, DEFAULT_REPEATER_BAND);

	m_config->Read(m_name + KEY_REPEATER_TYPE1, &temp, long(DEFAULT_REPEATER_TYPE));
	m_repeater1Type = HW_TYPE(temp);

	m_config->Read(m_name + KEY_REPEATER_ADDRESS1, &m_repeater1Address, DEFAULT_REPEATER_ADDRESS);

	m_config->Read(m_name + KEY_REPEATER_PORT1, &temp, long(DEFAULT_REPEATER_PORT1));
	m_repeater1Port = (unsigned int)temp;

	m_config->Read(m_name + KEY_REFLECTOR1, &m_repeater1Reflector, DEFAULT_REFLECTOR);

	m_config->Read(m_name + KEY_ATSTARTUP1, &m_repeater1AtStartup, DEFAULT_ATSTARTUP);

	m_config->Read(m_name + KEY_RECONNECT1, &temp, long(DEFAULT_RECONNECT));
	m_repeater1Reconnect = RECONNECT(temp);

	m_config->Read(m_name + KEY_FREQUENCY1, &m_repeater1Frequency, DEFAULT_FREQUENCY);

	m_config->Read(m_name + KEY_OFFSET1, &m_repeater1Offset, DEFAULT_OFFSET);

	m_config->Read(m_name + KEY_RANGE1, &m_repeater1Range, DEFAULT_RANGE);

	m_config->Read(m_name + KEY_LATITUDE1, &m_repeater1Latitude, DEFAULT_LATITUDE);

	m_config->Read(m_name + KEY_LONGITUDE1, &m_repeater1Longitude, DEFAULT_LONGITUDE);

	m_config->Read(m_name + KEY_AGL1, &m_repeater1Agl, DEFAULT_AGL);

	m_config->Read(m_name + KEY_DESCRIPTION11, &m_repeater1Description1, DEFAULT_DESCRIPTION1);

	m_config->Read(m_name + KEY_DESCRIPTION12, &m_repeater1Description2, DEFAULT_DESCRIPTION2);

	m_config->Read(m_name + KEY_URL1, &m_repeater1URL, DEFAULT_URL);

	m_config->Read(m_name + KEY_BAND11, &temp, long(DEFAULT_BAND1));
	m_repeater1Band1 = (unsigned char)temp;

	m_config->Read(m_name + KEY_BAND12, &temp, long(DEFAULT_BAND2));
	m_repeater1Band2 = (unsigned char)temp;

	m_config->Read(m_name + KEY_BAND13, &temp, long(DEFAULT_BAND3));
	m_repeater1Band3 = (unsigned char)temp;

	m_config->Read(m_name + KEY_REPEATER_CALL2, &m_repeater2Callsign, DEFAULT_REPEATER_CALL);

	m_config->Read(m_name + KEY_REPEATER_BAND2, &m_repeater2Band, DEFAULT_REPEATER_BAND);

	m_config->Read(m_name + KEY_REPEATER_TYPE2, &temp, long(DEFAULT_REPEATER_TYPE));
	m_repeater2Type = HW_TYPE(temp);

	m_config->Read(m_name + KEY_REPEATER_ADDRESS2, &m_repeater2Address, DEFAULT_REPEATER_ADDRESS);

	m_config->Read(m_name + KEY_REPEATER_PORT2, &temp, long(DEFAULT_REPEATER_PORT2));
	m_repeater2Port = (unsigned int)temp;

	m_config->Read(m_name + KEY_REFLECTOR2, &m_repeater2Reflector, DEFAULT_REFLECTOR);

	m_config->Read(m_name + KEY_ATSTARTUP2, &m_repeater2AtStartup, DEFAULT_ATSTARTUP);

	m_config->Read(m_name + KEY_RECONNECT2, &temp, long(DEFAULT_RECONNECT));
	m_repeater2Reconnect = RECONNECT(temp);

	m_config->Read(m_name + KEY_FREQUENCY2, &m_repeater2Frequency, DEFAULT_FREQUENCY);

	m_config->Read(m_name + KEY_OFFSET2, &m_repeater2Offset, DEFAULT_OFFSET);

	m_config->Read(m_name + KEY_RANGE2, &m_repeater2Range, DEFAULT_RANGE);

	m_config->Read(m_name + KEY_LATITUDE2, &m_repeater2Latitude, DEFAULT_LATITUDE);

	m_config->Read(m_name + KEY_LONGITUDE2, &m_repeater2Longitude, DEFAULT_LONGITUDE);

	m_config->Read(m_name + KEY_AGL2, &m_repeater2Agl, DEFAULT_AGL);

	m_config->Read(m_name + KEY_DESCRIPTION21, &m_repeater2Description1, DEFAULT_DESCRIPTION1);

	m_config->Read(m_name + KEY_DESCRIPTION22, &m_repeater2Description2, DEFAULT_DESCRIPTION2);

	m_config->Read(m_name + KEY_URL2, &m_repeater2URL, DEFAULT_URL);

	m_config->Read(m_name + KEY_BAND21, &temp, long(DEFAULT_BAND1));
	m_repeater2Band1 = (unsigned char)temp;

	m_config->Read(m_name + KEY_BAND22, &temp, long(DEFAULT_BAND2));
	m_repeater2Band2 = (unsigned char)temp;

	m_config->Read(m_name + KEY_BAND23, &temp, long(DEFAULT_BAND3));
	m_repeater2Band3 = (unsigned char)temp;

	m_config->Read(m_name + KEY_REPEATER_CALL3, &m_repeater3Callsign, DEFAULT_REPEATER_CALL);

	m_config->Read(m_name + KEY_REPEATER_BAND3, &m_repeater3Band, DEFAULT_REPEATER_BAND);

	m_config->Read(m_name + KEY_REPEATER_TYPE3, &temp, long(DEFAULT_REPEATER_TYPE));
	m_repeater3Type = HW_TYPE(temp);

	m_config->Read(m_name + KEY_REPEATER_ADDRESS3, &m_repeater3Address, DEFAULT_REPEATER_ADDRESS);

	m_config->Read(m_name + KEY_REPEATER_PORT3, &temp, long(DEFAULT_REPEATER_PORT3));
	m_repeater3Port = (unsigned int)temp;

	m_config->Read(m_name + KEY_REFLECTOR3, &m_repeater3Reflector, DEFAULT_REFLECTOR);

	m_config->Read(m_name + KEY_ATSTARTUP3, &m_repeater3AtStartup, DEFAULT_ATSTARTUP);

	m_config->Read(m_name + KEY_RECONNECT3, &temp, long(DEFAULT_RECONNECT));
	m_repeater3Reconnect = RECONNECT(temp);

	m_config->Read(m_name + KEY_FREQUENCY3, &m_repeater3Frequency, DEFAULT_FREQUENCY);

	m_config->Read(m_name + KEY_OFFSET3, &m_repeater3Offset, DEFAULT_OFFSET);

	m_config->Read(m_name + KEY_RANGE3, &m_repeater3Range, DEFAULT_RANGE);

	m_config->Read(m_name + KEY_LATITUDE3, &m_repeater3Latitude, DEFAULT_LATITUDE);

	m_config->Read(m_name + KEY_LONGITUDE3, &m_repeater3Longitude, DEFAULT_LONGITUDE);

	m_config->Read(m_name + KEY_AGL3, &m_repeater3Agl, DEFAULT_AGL);

	m_config->Read(m_name + KEY_DESCRIPTION31, &m_repeater3Description1, DEFAULT_DESCRIPTION1);

	m_config->Read(m_name + KEY_DESCRIPTION32, &m_repeater3Description2, DEFAULT_DESCRIPTION2);

	m_config->Read(m_name + KEY_URL3, &m_repeater3URL, DEFAULT_URL);

	m_config->Read(m_name + KEY_BAND31, &temp, long(DEFAULT_BAND1));
	m_repeater3Band1 = (unsigned char)temp;

	m_config->Read(m_name + KEY_BAND32, &temp, long(DEFAULT_BAND2));
	m_repeater3Band2 = (unsigned char)temp;

	m_config->Read(m_name + KEY_BAND33, &temp, long(DEFAULT_BAND3));
	m_repeater3Band3 = (unsigned char)temp;

	m_config->Read(m_name + KEY_REPEATER_CALL4, &m_repeater4Callsign, DEFAULT_REPEATER_CALL);

	m_config->Read(m_name + KEY_REPEATER_BAND4, &m_repeater4Band, DEFAULT_REPEATER_BAND);

	m_config->Read(m_name + KEY_REPEATER_TYPE4, &temp, long(DEFAULT_REPEATER_TYPE));
	m_repeater4Type = HW_TYPE(temp);

	m_config->Read(m_name + KEY_REPEATER_ADDRESS4, &m_repeater4Address, DEFAULT_REPEATER_ADDRESS);

	m_config->Read(m_name + KEY_REPEATER_PORT4, &temp, long(DEFAULT_REPEATER_PORT4));
	m_repeater4Port = (unsigned int)temp;

	m_config->Read(m_name + KEY_REFLECTOR4, &m_repeater4Reflector, DEFAULT_REFLECTOR);

	m_config->Read(m_name + KEY_ATSTARTUP4, &m_repeater4AtStartup, DEFAULT_ATSTARTUP);

	m_config->Read(m_name + KEY_RECONNECT4, &temp, long(DEFAULT_RECONNECT));
	m_repeater4Reconnect = RECONNECT(temp);

	m_config->Read(m_name + KEY_FREQUENCY4, &m_repeater4Frequency, DEFAULT_FREQUENCY);

	m_config->Read(m_name + KEY_OFFSET4, &m_repeater4Offset, DEFAULT_OFFSET);

	m_config->Read(m_name + KEY_RANGE4, &m_repeater4Range, DEFAULT_RANGE);

	m_config->Read(m_name + KEY_LATITUDE4, &m_repeater4Latitude, DEFAULT_LATITUDE);

	m_config->Read(m_name + KEY_LONGITUDE4, &m_repeater4Longitude, DEFAULT_LONGITUDE);

	m_config->Read(m_name + KEY_AGL4, &m_repeater4Agl, DEFAULT_AGL);

	m_config->Read(m_name + KEY_DESCRIPTION41, &m_repeater4Description1, DEFAULT_DESCRIPTION1);

	m_config->Read(m_name + KEY_DESCRIPTION42, &m_repeater4Description2, DEFAULT_DESCRIPTION2);

	m_config->Read(m_name + KEY_URL4, &m_repeater4URL, DEFAULT_URL);

	m_config->Read(m_name + KEY_BAND41, &temp, long(DEFAULT_BAND1));
	m_repeater4Band1 = (unsigned char)temp;

	m_config->Read(m_name + KEY_BAND42, &temp, long(DEFAULT_BAND2));
	m_repeater4Band2 = (unsigned char)temp;

	m_config->Read(m_name + KEY_BAND43, &temp, long(DEFAULT_BAND3));
	m_repeater4Band3 = (unsigned char)temp;

	m_config->Read(m_name + KEY_IRCDDB_ENABLED, &m_ircddbEnabled, DEFAULT_IRCDDB_ENABLED);

	m_config->Read(m_name + KEY_IRCDDB_HOSTNAME, &m_ircddbHostname, DEFAULT_IRCDDB_HOSTNAME);

	m_config->Read(m_name + KEY_IRCDDB_USERNAME, &m_ircddbUsername, DEFAULT_IRCDDB_USERNAME);

	m_config->Read(m_name + KEY_IRCDDB_PASSWORD, &m_ircddbPassword, DEFAULT_IRCDDB_PASSWORD);

	m_config->Read(m_name + KEY_APRS_ENABLED, &m_aprsEnabled, DEFAULT_APRS_ENABLED);

	m_config->Read(m_name + KEY_APRS_HOSTNAME, &m_aprsHostname, DEFAULT_APRS_HOSTNAME);

	m_config->Read(m_name + KEY_APRS_PORT, &temp, long(DEFAULT_APRS_PORT));
	m_aprsPort = (unsigned int)temp;

	m_config->Read(m_name + KEY_DEXTRA_ENABLED, &m_dextraEnabled, DEFAULT_DEXTRA_ENABLED);

	m_config->Read(m_name + KEY_DEXTRA_MAXDONGLES, &temp, long(DEFAULT_DEXTRA_MAXDONGLES));
	m_dextraMaxDongles = (unsigned int)temp;

	m_config->Read(m_name + KEY_DPLUS_ENABLED, &m_dplusEnabled, DEFAULT_DPLUS_ENABLED);

	m_config->Read(m_name + KEY_DPLUS_MAXDONGLES, &temp, long(DEFAULT_DPLUS_MAXDONGLES));
	m_dplusMaxDongles = (unsigned int)temp;

	m_config->Read(m_name + KEY_DPLUS_LOGIN, &m_dplusLogin, DEFAULT_DPLUS_LOGIN);

	m_config->Read(m_name + KEY_DCS_ENABLED, &m_dcsEnabled, DEFAULT_DCS_ENABLED);

	m_config->Read(m_name + KEY_CCS_ENABLED, &m_ccsEnabled, DEFAULT_CCS_ENABLED);

	m_config->Read(m_name + KEY_CCS_HOST, &m_ccsHost, DEFAULT_CCS_HOST);

	m_config->Read(m_name + KEY_STARNET_BAND1, &m_starNet1Band, DEFAULT_STARNET_BAND);

	m_config->Read(m_name + KEY_STARNET_CALLSIGN1, &m_starNet1Callsign, DEFAULT_STARNET_CALLSIGN);

	m_config->Read(m_name + KEY_STARNET_LOGOFF1, &m_starNet1Logoff, DEFAULT_STARNET_LOGOFF);

	m_config->Read(m_name + KEY_STARNET_INFO1, &m_starNet1Info, DEFAULT_STARNET_INFO);

	m_config->Read(m_name + KEY_STARNET_PERMANENT1, &m_starNet1Permanent, DEFAULT_STARNET_PERMANENT);

	m_config->Read(m_name + KEY_STARNET_USER_TIMEOUT1, &temp, long(DEFAULT_STARNET_USER_TIMEOUT));
	m_starNet1UserTimeout = (unsigned int)temp;

	m_config->Read(m_name + KEY_STARNET_GROUP_TIMEOUT1, &temp, long(DEFAULT_STARNET_GROUP_TIMEOUT));
	m_starNet1GroupTimeout = (unsigned int)temp;

	m_config->Read(m_name + KEY_STARNET_CALLSIGN_SWITCH1, &temp, long(DEFAULT_STARNET_CALLSIGN_SWITCH));
	m_starNet1CallsignSwitch = STARNET_CALLSIGN_SWITCH(temp);

	m_config->Read(m_name + KEY_STARNET_TXMSG_SWITCH1, &m_starNet1TxMsgSwitch, DEFAULT_STARNET_TXMSG_SWITCH);

	m_config->Read(m_name + KEY_STARNET_REFLECTOR1, &m_starNet1Reflector, DEFAULT_STARNET_REFLECTOR);

	m_config->Read(m_name + KEY_STARNET_BAND2, &m_starNet2Band, DEFAULT_STARNET_BAND);

	m_config->Read(m_name + KEY_STARNET_CALLSIGN2, &m_starNet2Callsign, DEFAULT_STARNET_CALLSIGN);

	m_config->Read(m_name + KEY_STARNET_LOGOFF2, &m_starNet2Logoff, DEFAULT_STARNET_LOGOFF);

	m_config->Read(m_name + KEY_STARNET_INFO2, &m_starNet2Info, DEFAULT_STARNET_INFO);

	m_config->Read(m_name + KEY_STARNET_PERMANENT2, &m_starNet2Permanent, DEFAULT_STARNET_PERMANENT);

	m_config->Read(m_name + KEY_STARNET_USER_TIMEOUT2, &temp, long(DEFAULT_STARNET_USER_TIMEOUT));
	m_starNet2UserTimeout = (unsigned int)temp;

	m_config->Read(m_name + KEY_STARNET_GROUP_TIMEOUT2, &temp, long(DEFAULT_STARNET_GROUP_TIMEOUT));
	m_starNet2GroupTimeout = (unsigned int)temp;

	m_config->Read(m_name + KEY_STARNET_CALLSIGN_SWITCH2, &temp, long(DEFAULT_STARNET_CALLSIGN_SWITCH));
	m_starNet2CallsignSwitch = STARNET_CALLSIGN_SWITCH(temp);

	m_config->Read(m_name + KEY_STARNET_TXMSG_SWITCH2, &m_starNet2TxMsgSwitch, DEFAULT_STARNET_TXMSG_SWITCH);

	m_config->Read(m_name + KEY_STARNET_REFLECTOR2, &m_starNet2Reflector, DEFAULT_STARNET_REFLECTOR);

	m_config->Read(m_name + KEY_STARNET_BAND3, &m_starNet3Band, DEFAULT_STARNET_BAND);

	m_config->Read(m_name + KEY_STARNET_CALLSIGN3, &m_starNet3Callsign, DEFAULT_STARNET_CALLSIGN);

	m_config->Read(m_name + KEY_STARNET_LOGOFF3, &m_starNet3Logoff, DEFAULT_STARNET_LOGOFF);

	m_config->Read(m_name + KEY_STARNET_INFO3, &m_starNet3Info, DEFAULT_STARNET_INFO);

	m_config->Read(m_name + KEY_STARNET_PERMANENT3, &m_starNet3Permanent, DEFAULT_STARNET_PERMANENT);

	m_config->Read(m_name + KEY_STARNET_USER_TIMEOUT3, &temp, long(DEFAULT_STARNET_USER_TIMEOUT));
	m_starNet3UserTimeout = (unsigned int)temp;

	m_config->Read(m_name + KEY_STARNET_GROUP_TIMEOUT3, &temp, long(DEFAULT_STARNET_GROUP_TIMEOUT));
	m_starNet3GroupTimeout = (unsigned int)temp;

	m_config->Read(m_name + KEY_STARNET_CALLSIGN_SWITCH3, &temp, long(DEFAULT_STARNET_CALLSIGN_SWITCH));
	m_starNet3CallsignSwitch = STARNET_CALLSIGN_SWITCH(temp);

	m_config->Read(m_name + KEY_STARNET_TXMSG_SWITCH3, &m_starNet3TxMsgSwitch, DEFAULT_STARNET_TXMSG_SWITCH);

	m_config->Read(m_name + KEY_STARNET_REFLECTOR3, &m_starNet3Reflector, DEFAULT_STARNET_REFLECTOR);

	m_config->Read(m_name + KEY_STARNET_BAND4, &m_starNet4Band, DEFAULT_STARNET_BAND);

	m_config->Read(m_name + KEY_STARNET_CALLSIGN4, &m_starNet4Callsign, DEFAULT_STARNET_CALLSIGN);

	m_config->Read(m_name + KEY_STARNET_LOGOFF4, &m_starNet4Logoff, DEFAULT_STARNET_LOGOFF);

	m_config->Read(m_name + KEY_STARNET_INFO4, &m_starNet4Info, DEFAULT_STARNET_INFO);

	m_config->Read(m_name + KEY_STARNET_PERMANENT4, &m_starNet4Permanent, DEFAULT_STARNET_PERMANENT);

	m_config->Read(m_name + KEY_STARNET_USER_TIMEOUT4, &temp, long(DEFAULT_STARNET_USER_TIMEOUT));
	m_starNet4UserTimeout = (unsigned int)temp;

	m_config->Read(m_name + KEY_STARNET_GROUP_TIMEOUT4, &temp, long(DEFAULT_STARNET_GROUP_TIMEOUT));
	m_starNet4GroupTimeout = (unsigned int)temp;

	m_config->Read(m_name + KEY_STARNET_CALLSIGN_SWITCH4, &temp, long(DEFAULT_STARNET_CALLSIGN_SWITCH));
	m_starNet4CallsignSwitch = STARNET_CALLSIGN_SWITCH(temp);

	m_config->Read(m_name + KEY_STARNET_TXMSG_SWITCH4, &m_starNet4TxMsgSwitch, DEFAULT_STARNET_TXMSG_SWITCH);

	m_config->Read(m_name + KEY_STARNET_REFLECTOR4, &m_starNet4Reflector, DEFAULT_STARNET_REFLECTOR);

	m_config->Read(m_name + KEY_STARNET_BAND5, &m_starNet5Band, DEFAULT_STARNET_BAND);

	m_config->Read(m_name + KEY_STARNET_CALLSIGN5, &m_starNet5Callsign, DEFAULT_STARNET_CALLSIGN);

	m_config->Read(m_name + KEY_STARNET_LOGOFF5, &m_starNet5Logoff, DEFAULT_STARNET_LOGOFF);

	m_config->Read(m_name + KEY_STARNET_INFO5, &m_starNet5Info, DEFAULT_STARNET_INFO);

	m_config->Read(m_name + KEY_STARNET_PERMANENT5, &m_starNet5Permanent, DEFAULT_STARNET_PERMANENT);

	m_config->Read(m_name + KEY_STARNET_USER_TIMEOUT5, &temp, long(DEFAULT_STARNET_USER_TIMEOUT));
	m_starNet5UserTimeout = (unsigned int)temp;

	m_config->Read(m_name + KEY_STARNET_GROUP_TIMEOUT5, &temp, long(DEFAULT_STARNET_GROUP_TIMEOUT));
	m_starNet5GroupTimeout = (unsigned int)temp;

	m_config->Read(m_name + KEY_STARNET_CALLSIGN_SWITCH5, &temp, long(DEFAULT_STARNET_CALLSIGN_SWITCH));
	m_starNet5CallsignSwitch = STARNET_CALLSIGN_SWITCH(temp);

	m_config->Read(m_name + KEY_STARNET_TXMSG_SWITCH5, &m_starNet5TxMsgSwitch, DEFAULT_STARNET_TXMSG_SWITCH);

	m_config->Read(m_name + KEY_STARNET_REFLECTOR5, &m_starNet5Reflector, DEFAULT_STARNET_REFLECTOR);

	m_config->Read(m_name + KEY_REMOTE_ENABLED, &m_remoteEnabled, DEFAULT_REMOTE_ENABLED);

	m_config->Read(m_name + KEY_REMOTE_PASSWORD, &m_remotePassword, DEFAULT_REMOTE_PASSWORD);

	m_config->Read(m_name + KEY_REMOTE_PORT, &temp, long(DEFAULT_REMOTE_PORT));
	m_remotePort = (unsigned int)temp;

	m_config->Read(m_name + KEY_LANGUAGE, &temp, long(DEFAULT_LANGUAGE));
	m_language = TEXT_LANG(temp);

	m_config->Read(m_name + KEY_INFO_ENABLED, &m_infoEnabled, DEFAULT_INFO_ENABLED);

	m_config->Read(m_name + KEY_ECHO_ENABLED, &m_echoEnabled, DEFAULT_ECHO_ENABLED);

	m_config->Read(m_name + KEY_LOG_ENABLED, &m_logEnabled, DEFAULT_LOG_ENABLED);

	m_config->Read(m_name + KEY_DRATS_ENABLED, &m_dratsEnabled, DEFAULT_DRATS_ENABLED);

	m_config->Read(m_name + KEY_DTMF_ENABLED, &m_dtmfEnabled, DEFAULT_DTMF_ENABLED);

	m_config->Read(m_name + KEY_WINDOW_X, &temp, long(DEFAULT_WINDOW_X));
	m_x = int(temp);

	m_config->Read(m_name + KEY_WINDOW_Y, &temp, long(DEFAULT_WINDOW_Y));
	m_y = int(temp);
}

CIRCDDBGatewayConfig::~CIRCDDBGatewayConfig()
{
	delete m_config;
}

#else

CIRCDDBGatewayConfig::CIRCDDBGatewayConfig(const wxString& dir, const wxString& configName, const wxString& name) :
m_fileName(),
m_type(DEFAULT_GATEWAY_TYPE),
m_callsign(DEFAULT_GATEWAY_CALLSIGN),
m_address(DEFAULT_GATEWAY_ADDRESS),
m_icomAddress(DEFAULT_ICOM_ADDRESS),
m_icomPort(DEFAULT_ICOM_PORT),
m_hbAddress(DEFAULT_HB_ADDRESS),
m_hbPort(DEFAULT_HB_PORT),
m_latitude(DEFAULT_LATITUDE),
m_longitude(DEFAULT_LONGITUDE),
m_description1(DEFAULT_DESCRIPTION1),
m_description2(DEFAULT_DESCRIPTION2),
m_url(DEFAULT_URL),
m_repeater1Callsign(DEFAULT_REPEATER_CALL),
m_repeater1Band(DEFAULT_REPEATER_BAND),
m_repeater1Type(DEFAULT_REPEATER_TYPE),
m_repeater1Address(DEFAULT_REPEATER_ADDRESS),
m_repeater1Port(DEFAULT_REPEATER_PORT1),
m_repeater1Reflector(DEFAULT_REFLECTOR),
m_repeater1AtStartup(DEFAULT_ATSTARTUP),
m_repeater1Reconnect(DEFAULT_RECONNECT),
m_repeater1Frequency(DEFAULT_FREQUENCY),
m_repeater1Offset(DEFAULT_OFFSET),
m_repeater1Range(DEFAULT_RANGE),
m_repeater1Latitude(DEFAULT_LATITUDE),
m_repeater1Longitude(DEFAULT_LONGITUDE),
m_repeater1Agl(DEFAULT_AGL),
m_repeater1Description1(DEFAULT_DESCRIPTION1),
m_repeater1Description2(DEFAULT_DESCRIPTION2),
m_repeater1URL(DEFAULT_URL),
m_repeater1Band1(DEFAULT_BAND1),
m_repeater1Band2(DEFAULT_BAND2),
m_repeater1Band3(DEFAULT_BAND3),
m_repeater2Callsign(DEFAULT_REPEATER_CALL),
m_repeater2Band(DEFAULT_REPEATER_BAND),
m_repeater2Type(DEFAULT_REPEATER_TYPE),
m_repeater2Address(DEFAULT_REPEATER_ADDRESS),
m_repeater2Port(DEFAULT_REPEATER_PORT2),
m_repeater2Reflector(DEFAULT_REFLECTOR),
m_repeater2AtStartup(DEFAULT_ATSTARTUP),
m_repeater2Reconnect(DEFAULT_RECONNECT),
m_repeater2Frequency(DEFAULT_FREQUENCY),
m_repeater2Offset(DEFAULT_OFFSET),
m_repeater2Range(DEFAULT_RANGE),
m_repeater2Latitude(DEFAULT_LATITUDE),
m_repeater2Longitude(DEFAULT_LONGITUDE),
m_repeater2Agl(DEFAULT_AGL),
m_repeater2Description1(DEFAULT_DESCRIPTION1),
m_repeater2Description2(DEFAULT_DESCRIPTION2),
m_repeater2URL(DEFAULT_URL),
m_repeater2Band1(DEFAULT_BAND1),
m_repeater2Band2(DEFAULT_BAND2),
m_repeater2Band3(DEFAULT_BAND3),
m_repeater3Callsign(DEFAULT_REPEATER_CALL),
m_repeater3Band(DEFAULT_REPEATER_BAND),
m_repeater3Type(DEFAULT_REPEATER_TYPE),
m_repeater3Address(DEFAULT_REPEATER_ADDRESS),
m_repeater3Port(DEFAULT_REPEATER_PORT3),
m_repeater3Reflector(DEFAULT_REFLECTOR),
m_repeater3AtStartup(DEFAULT_ATSTARTUP),
m_repeater3Reconnect(DEFAULT_RECONNECT),
m_repeater3Frequency(DEFAULT_FREQUENCY),
m_repeater3Offset(DEFAULT_OFFSET),
m_repeater3Range(DEFAULT_RANGE),
m_repeater3Latitude(DEFAULT_LATITUDE),
m_repeater3Longitude(DEFAULT_LONGITUDE),
m_repeater3Agl(DEFAULT_AGL),
m_repeater3Description1(DEFAULT_DESCRIPTION1),
m_repeater3Description2(DEFAULT_DESCRIPTION2),
m_repeater3URL(DEFAULT_URL),
m_repeater3Band1(DEFAULT_BAND1),
m_repeater3Band2(DEFAULT_BAND2),
m_repeater3Band3(DEFAULT_BAND3),
m_repeater4Callsign(DEFAULT_REPEATER_CALL),
m_repeater4Band(DEFAULT_REPEATER_BAND),
m_repeater4Type(DEFAULT_REPEATER_TYPE),
m_repeater4Address(DEFAULT_REPEATER_ADDRESS),
m_repeater4Port(DEFAULT_REPEATER_PORT4),
m_repeater4Reflector(DEFAULT_REFLECTOR),
m_repeater4AtStartup(DEFAULT_ATSTARTUP),
m_repeater4Reconnect(DEFAULT_RECONNECT),
m_repeater4Frequency(DEFAULT_FREQUENCY),
m_repeater4Offset(DEFAULT_OFFSET),
m_repeater4Range(DEFAULT_RANGE),
m_repeater4Latitude(DEFAULT_LATITUDE),
m_repeater4Longitude(DEFAULT_LONGITUDE),
m_repeater4Agl(DEFAULT_AGL),
m_repeater4Description1(DEFAULT_DESCRIPTION1),
m_repeater4Description2(DEFAULT_DESCRIPTION2),
m_repeater4URL(DEFAULT_URL),
m_repeater4Band1(DEFAULT_BAND1),
m_repeater4Band2(DEFAULT_BAND2),
m_repeater4Band3(DEFAULT_BAND3),
m_ircddbEnabled(DEFAULT_IRCDDB_ENABLED),
m_ircddbHostname(DEFAULT_IRCDDB_HOSTNAME),
m_ircddbUsername(DEFAULT_IRCDDB_USERNAME),
m_ircddbPassword(DEFAULT_IRCDDB_PASSWORD),
m_aprsEnabled(DEFAULT_APRS_ENABLED),
m_aprsHostname(DEFAULT_APRS_HOSTNAME),
m_aprsPort(DEFAULT_APRS_PORT),
m_dextraEnabled(DEFAULT_DEXTRA_ENABLED),
m_dextraMaxDongles(DEFAULT_DEXTRA_MAXDONGLES),
m_dplusEnabled(DEFAULT_DPLUS_ENABLED),
m_dplusMaxDongles(DEFAULT_DPLUS_MAXDONGLES),
m_dplusLogin(DEFAULT_DPLUS_LOGIN),
m_dcsEnabled(DEFAULT_DCS_ENABLED),
m_ccsEnabled(DEFAULT_CCS_ENABLED),
m_ccsHost(DEFAULT_CCS_HOST),
m_starNet1Band(DEFAULT_STARNET_BAND),
m_starNet1Callsign(DEFAULT_STARNET_CALLSIGN),
m_starNet1Logoff(DEFAULT_STARNET_LOGOFF),
m_starNet1Info(DEFAULT_STARNET_INFO),
m_starNet1Permanent(DEFAULT_STARNET_PERMANENT),
m_starNet1UserTimeout(DEFAULT_STARNET_USER_TIMEOUT),
m_starNet1GroupTimeout(DEFAULT_STARNET_GROUP_TIMEOUT),
m_starNet1CallsignSwitch(DEFAULT_STARNET_CALLSIGN_SWITCH),
m_starNet1TxMsgSwitch(DEFAULT_STARNET_TXMSG_SWITCH),
m_starNet1Reflector(DEFAULT_STARNET_REFLECTOR),
m_starNet2Band(DEFAULT_STARNET_BAND),
m_starNet2Callsign(DEFAULT_STARNET_CALLSIGN),
m_starNet2Logoff(DEFAULT_STARNET_LOGOFF),
m_starNet2Info(DEFAULT_STARNET_INFO),
m_starNet2Permanent(DEFAULT_STARNET_PERMANENT),
m_starNet2UserTimeout(DEFAULT_STARNET_USER_TIMEOUT),
m_starNet2GroupTimeout(DEFAULT_STARNET_GROUP_TIMEOUT),
m_starNet2CallsignSwitch(DEFAULT_STARNET_CALLSIGN_SWITCH),
m_starNet2TxMsgSwitch(DEFAULT_STARNET_TXMSG_SWITCH),
m_starNet2Reflector(DEFAULT_STARNET_REFLECTOR),
m_starNet3Band(DEFAULT_STARNET_BAND),
m_starNet3Callsign(DEFAULT_STARNET_CALLSIGN),
m_starNet3Logoff(DEFAULT_STARNET_LOGOFF),
m_starNet3Info(DEFAULT_STARNET_INFO),
m_starNet3Permanent(DEFAULT_STARNET_PERMANENT),
m_starNet3UserTimeout(DEFAULT_STARNET_USER_TIMEOUT),
m_starNet3GroupTimeout(DEFAULT_STARNET_GROUP_TIMEOUT),
m_starNet3CallsignSwitch(DEFAULT_STARNET_CALLSIGN_SWITCH),
m_starNet3TxMsgSwitch(DEFAULT_STARNET_TXMSG_SWITCH),
m_starNet3Reflector(DEFAULT_STARNET_REFLECTOR),
m_starNet4Band(DEFAULT_STARNET_BAND),
m_starNet4Callsign(DEFAULT_STARNET_CALLSIGN),
m_starNet4Logoff(DEFAULT_STARNET_LOGOFF),
m_starNet4Info(DEFAULT_STARNET_INFO),
m_starNet4Permanent(DEFAULT_STARNET_PERMANENT),
m_starNet4UserTimeout(DEFAULT_STARNET_USER_TIMEOUT),
m_starNet4GroupTimeout(DEFAULT_STARNET_GROUP_TIMEOUT),
m_starNet4CallsignSwitch(DEFAULT_STARNET_CALLSIGN_SWITCH),
m_starNet4TxMsgSwitch(DEFAULT_STARNET_TXMSG_SWITCH),
m_starNet4Reflector(DEFAULT_STARNET_REFLECTOR),
m_starNet5Band(DEFAULT_STARNET_BAND),
m_starNet5Callsign(DEFAULT_STARNET_CALLSIGN),
m_starNet5Logoff(DEFAULT_STARNET_LOGOFF),
m_starNet5Info(DEFAULT_STARNET_INFO),
m_starNet5Permanent(DEFAULT_STARNET_PERMANENT),
m_starNet5UserTimeout(DEFAULT_STARNET_USER_TIMEOUT),
m_starNet5GroupTimeout(DEFAULT_STARNET_GROUP_TIMEOUT),
m_starNet5CallsignSwitch(DEFAULT_STARNET_CALLSIGN_SWITCH),
m_starNet5TxMsgSwitch(DEFAULT_STARNET_TXMSG_SWITCH),
m_starNet5Reflector(DEFAULT_STARNET_REFLECTOR),
m_remoteEnabled(DEFAULT_REMOTE_ENABLED),
m_remotePassword(DEFAULT_REMOTE_PASSWORD),
m_remotePort(DEFAULT_REMOTE_PORT),
m_language(DEFAULT_LANGUAGE),
m_infoEnabled(DEFAULT_INFO_ENABLED),
m_echoEnabled(DEFAULT_ECHO_ENABLED),
m_logEnabled(DEFAULT_LOG_ENABLED),
m_dratsEnabled(DEFAULT_DRATS_ENABLED),
m_dtmfEnabled(DEFAULT_DTMF_ENABLED),
m_x(DEFAULT_WINDOW_X),
m_y(DEFAULT_WINDOW_Y)
{
	wxASSERT(!dir.IsEmpty());

	wxString fileName = configName;
	if (!name.IsEmpty())
		fileName = configName + wxT("_") + name;

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

		if (key.IsSameAs(KEY_GATEWAY_TYPE)) {
			val.ToLong(&temp1);
			m_type = GATEWAY_TYPE(temp1);
		} else if (key.IsSameAs(KEY_GATEWAY_CALLSIGN)) {
			m_callsign = val;
		} else if (key.IsSameAs(KEY_GATEWAY_ADDRESS)) {
			m_address = val;
		} else if (key.IsSameAs(KEY_ICOM_ADDRESS)) {
			m_icomAddress = val;
		} else if (key.IsSameAs(KEY_ICOM_PORT)) {
			val.ToULong(&temp2);
			m_icomPort = (unsigned int)temp2;
		} else if (key.IsSameAs(KEY_HB_ADDRESS)) {
			m_hbAddress = val;
		} else if (key.IsSameAs(KEY_HB_PORT)) {
			val.ToULong(&temp2);
			m_hbPort = (unsigned int)temp2;
		} else if (key.IsSameAs(KEY_LATITUDE)) {
			val.ToDouble(&m_latitude);
		} else if (key.IsSameAs(KEY_LONGITUDE)) {
			val.ToDouble(&m_longitude);
		} else if (key.IsSameAs(KEY_DESCRIPTION1)) {
			m_description1 = val;
		} else if (key.IsSameAs(KEY_DESCRIPTION2)) {
			m_description2 = val;
		} else if (key.IsSameAs(KEY_URL)) {
			m_url = val;
		} else if (key.IsSameAs(KEY_REPEATER_CALL1)) {
			m_repeater1Callsign = val;
		} else if (key.IsSameAs(KEY_REPEATER_BAND1)) {
			m_repeater1Band = val;
		} else if (key.IsSameAs(KEY_REPEATER_TYPE1)) {
			val.ToLong(&temp1);
			m_repeater1Type = HW_TYPE(temp1);
		} else if (key.IsSameAs(KEY_REPEATER_ADDRESS1)) {
			m_repeater1Address = val;
		} else if (key.IsSameAs(KEY_REPEATER_PORT1)) {
			val.ToULong(&temp2);
			m_repeater1Port = (unsigned int)temp2;
		} else if (key.IsSameAs(KEY_REFLECTOR1)) {
			m_repeater1Reflector = val;
		} else if (key.IsSameAs(KEY_ATSTARTUP1)) {
			val.ToLong(&temp1);
			m_repeater1AtStartup = temp1 == 1L;
		} else if (key.IsSameAs(KEY_RECONNECT1)) {
			val.ToLong(&temp1);
			m_repeater1Reconnect = RECONNECT(temp1);
		} else if (key.IsSameAs(KEY_FREQUENCY1)) {
			val.ToDouble(&m_repeater1Frequency);
		} else if (key.IsSameAs(KEY_OFFSET1)) {
			val.ToDouble(&m_repeater1Offset);
		} else if (key.IsSameAs(KEY_RANGE1)) {
			val.ToDouble(&m_repeater1Range);
		} else if (key.IsSameAs(KEY_LATITUDE1)) {
			val.ToDouble(&m_repeater1Latitude);
		} else if (key.IsSameAs(KEY_LONGITUDE1)) {
			val.ToDouble(&m_repeater1Longitude);
		} else if (key.IsSameAs(KEY_AGL1)) {
			val.ToDouble(&m_repeater1Agl);
		} else if (key.IsSameAs(KEY_DESCRIPTION11)) {
			m_repeater1Description1 = val;
		} else if (key.IsSameAs(KEY_DESCRIPTION12)) {
			m_repeater1Description2 = val;
		} else if (key.IsSameAs(KEY_URL1)) {
			m_repeater1URL = val;
		} else if (key.IsSameAs(KEY_BAND11)) {
			val.ToULong(&temp2);
			m_repeater1Band1 = (unsigned char)temp2;
		} else if (key.IsSameAs(KEY_BAND12)) {
			val.ToULong(&temp2);
			m_repeater1Band2 = (unsigned char)temp2;
		} else if (key.IsSameAs(KEY_BAND13)) {
			val.ToULong(&temp2);
			m_repeater1Band3 = (unsigned char)temp2;
		} else if (key.IsSameAs(KEY_REPEATER_CALL2)) {
			m_repeater2Callsign = val;
		} else if (key.IsSameAs(KEY_REPEATER_BAND2)) {
			m_repeater2Band = val;
		} else if (key.IsSameAs(KEY_REPEATER_TYPE2)) {
			val.ToLong(&temp1);
			m_repeater2Type = HW_TYPE(temp1);
		} else if (key.IsSameAs(KEY_REPEATER_ADDRESS2)) {
			m_repeater2Address = val;
		} else if (key.IsSameAs(KEY_REPEATER_PORT2)) {
			val.ToULong(&temp2);
			m_repeater2Port = (unsigned int)temp2;
		} else if (key.IsSameAs(KEY_REFLECTOR2)) {
			m_repeater2Reflector = val;
		} else if (key.IsSameAs(KEY_ATSTARTUP2)) {
			val.ToLong(&temp1);
			m_repeater2AtStartup = temp1 == 1L;
		} else if (key.IsSameAs(KEY_RECONNECT2)) {
			val.ToLong(&temp1);
			m_repeater2Reconnect = RECONNECT(temp1);
		} else if (key.IsSameAs(KEY_FREQUENCY2)) {
			val.ToDouble(&m_repeater2Frequency);
		} else if (key.IsSameAs(KEY_OFFSET2)) {
			val.ToDouble(&m_repeater2Offset);
		} else if (key.IsSameAs(KEY_RANGE2)) {
			val.ToDouble(&m_repeater2Range);
		} else if (key.IsSameAs(KEY_LATITUDE2)) {
			val.ToDouble(&m_repeater2Latitude);
		} else if (key.IsSameAs(KEY_LONGITUDE2)) {
			val.ToDouble(&m_repeater2Longitude);
		} else if (key.IsSameAs(KEY_AGL2)) {
			val.ToDouble(&m_repeater2Agl);
		} else if (key.IsSameAs(KEY_DESCRIPTION21)) {
			m_repeater2Description1 = val;
		} else if (key.IsSameAs(KEY_DESCRIPTION22)) {
			m_repeater2Description2 = val;
		} else if (key.IsSameAs(KEY_URL2)) {
			m_repeater2URL = val;
		} else if (key.IsSameAs(KEY_BAND21)) {
			val.ToULong(&temp2);
			m_repeater2Band1 = (unsigned char)temp2;
		} else if (key.IsSameAs(KEY_BAND22)) {
			val.ToULong(&temp2);
			m_repeater2Band2 = (unsigned char)temp2;
		} else if (key.IsSameAs(KEY_BAND23)) {
			val.ToULong(&temp2);
			m_repeater2Band3 = (unsigned char)temp2;
		} else if (key.IsSameAs(KEY_REPEATER_CALL3)) {
			m_repeater3Callsign = val;
		} else if (key.IsSameAs(KEY_REPEATER_BAND3)) {
			m_repeater3Band = val;
		} else if (key.IsSameAs(KEY_REPEATER_TYPE3)) {
			val.ToLong(&temp1);
			m_repeater3Type = HW_TYPE(temp1);
		} else if (key.IsSameAs(KEY_REPEATER_ADDRESS3)) {
			m_repeater3Address = val;
		} else if (key.IsSameAs(KEY_REPEATER_PORT3)) {
			val.ToULong(&temp2);
			m_repeater3Port = (unsigned int)temp2;
		} else if (key.IsSameAs(KEY_REFLECTOR3)) {
			m_repeater3Reflector = val;
		} else if (key.IsSameAs(KEY_ATSTARTUP3)) {
			val.ToLong(&temp1);
			m_repeater3AtStartup = temp1 == 1L;
		} else if (key.IsSameAs(KEY_RECONNECT3)) {
			val.ToLong(&temp1);
			m_repeater3Reconnect = RECONNECT(temp1);
		} else if (key.IsSameAs(KEY_FREQUENCY3)) {
			val.ToDouble(&m_repeater3Frequency);
		} else if (key.IsSameAs(KEY_OFFSET3)) {
			val.ToDouble(&m_repeater3Offset);
		} else if (key.IsSameAs(KEY_RANGE3)) {
			val.ToDouble(&m_repeater3Range);
		} else if (key.IsSameAs(KEY_LATITUDE3)) {
			val.ToDouble(&m_repeater3Latitude);
		} else if (key.IsSameAs(KEY_LONGITUDE3)) {
			val.ToDouble(&m_repeater3Longitude);
		} else if (key.IsSameAs(KEY_AGL3)) {
			val.ToDouble(&m_repeater3Agl);
		} else if (key.IsSameAs(KEY_DESCRIPTION31)) {
			m_repeater3Description1 = val;
		} else if (key.IsSameAs(KEY_DESCRIPTION32)) {
			m_repeater3Description2 = val;
		} else if (key.IsSameAs(KEY_URL3)) {
			m_repeater3URL = val;
		} else if (key.IsSameAs(KEY_BAND31)) {
			val.ToULong(&temp2);
			m_repeater3Band1 = (unsigned char)temp2;
		} else if (key.IsSameAs(KEY_BAND32)) {
			val.ToULong(&temp2);
			m_repeater3Band2 = (unsigned char)temp2;
		} else if (key.IsSameAs(KEY_BAND33)) {
			val.ToULong(&temp2);
			m_repeater3Band3 = (unsigned char)temp2;
		} else if (key.IsSameAs(KEY_REPEATER_CALL4)) {
			m_repeater4Callsign = val;
		} else if (key.IsSameAs(KEY_REPEATER_BAND4)) {
			m_repeater4Band = val;
		} else if (key.IsSameAs(KEY_REPEATER_TYPE4)) {
			val.ToLong(&temp1);
			m_repeater4Type = HW_TYPE(temp1);
		} else if (key.IsSameAs(KEY_REPEATER_ADDRESS4)) {
			m_repeater4Address = val;
		} else if (key.IsSameAs(KEY_REPEATER_PORT4)) {
			val.ToULong(&temp2);
			m_repeater4Port = (unsigned int)temp2;
		} else if (key.IsSameAs(KEY_REFLECTOR4)) {
			m_repeater4Reflector = val;
		} else if (key.IsSameAs(KEY_ATSTARTUP4)) {
			val.ToLong(&temp1);
			m_repeater4AtStartup = temp1 == 1L;
		} else if (key.IsSameAs(KEY_RECONNECT4)) {
			val.ToLong(&temp1);
			m_repeater4Reconnect = RECONNECT(temp1);
		} else if (key.IsSameAs(KEY_FREQUENCY4)) {
			val.ToDouble(&m_repeater4Frequency);
		} else if (key.IsSameAs(KEY_OFFSET4)) {
			val.ToDouble(&m_repeater4Offset);
		} else if (key.IsSameAs(KEY_RANGE4)) {
			val.ToDouble(&m_repeater4Range);
		} else if (key.IsSameAs(KEY_LATITUDE4)) {
			val.ToDouble(&m_repeater4Latitude);
		} else if (key.IsSameAs(KEY_LONGITUDE4)) {
			val.ToDouble(&m_repeater4Longitude);
		} else if (key.IsSameAs(KEY_AGL4)) {
			val.ToDouble(&m_repeater4Agl);
		} else if (key.IsSameAs(KEY_DESCRIPTION41)) {
			m_repeater4Description1 = val;
		} else if (key.IsSameAs(KEY_DESCRIPTION42)) {
			m_repeater4Description2 = val;
		} else if (key.IsSameAs(KEY_URL4)) {
			m_repeater4URL = val;
		} else if (key.IsSameAs(KEY_BAND41)) {
			val.ToULong(&temp2);
			m_repeater4Band1 = (unsigned char)temp2;
		} else if (key.IsSameAs(KEY_BAND42)) {
			val.ToULong(&temp2);
			m_repeater4Band2 = (unsigned char)temp2;
		} else if (key.IsSameAs(KEY_BAND43)) {
			val.ToULong(&temp2);
			m_repeater4Band3 = (unsigned char)temp2;
		} else if (key.IsSameAs(KEY_IRCDDB_ENABLED)) {
			val.ToLong(&temp1);
			m_ircddbEnabled = temp1 == 1L;
		} else if (key.IsSameAs(KEY_IRCDDB_HOSTNAME)) {
			m_ircddbHostname = val;
		} else if (key.IsSameAs(KEY_IRCDDB_USERNAME)) {
			m_ircddbUsername = val;
		} else if (key.IsSameAs(KEY_IRCDDB_PASSWORD)) {
			m_ircddbPassword = val;
		} else if (key.IsSameAs(KEY_APRS_ENABLED)) {
			val.ToLong(&temp1);
			m_aprsEnabled = temp1 == 1L;
		} else if (key.IsSameAs(KEY_APRS_HOSTNAME)) {
			m_aprsHostname = val;
		} else if (key.IsSameAs(KEY_APRS_PORT)) {
			val.ToULong(&temp2);
			m_aprsPort = (unsigned int)temp2;
		} else if (key.IsSameAs(KEY_DEXTRA_ENABLED)) {
			val.ToLong(&temp1);
			m_dextraEnabled = temp1 == 1L;
		} else if (key.IsSameAs(KEY_DEXTRA_MAXDONGLES)) {
			val.ToULong(&temp2);
			m_dextraMaxDongles = (unsigned int)temp2;
		} else if (key.IsSameAs(KEY_DPLUS_ENABLED)) {
			val.ToLong(&temp1);
			m_dplusEnabled = temp1 == 1L;
		} else if (key.IsSameAs(KEY_DPLUS_MAXDONGLES)) {
			val.ToULong(&temp2);
			m_dplusMaxDongles = (unsigned int)temp2;
		} else if (key.IsSameAs(KEY_DPLUS_LOGIN)) {
			m_dplusLogin = val;
		} else if (key.IsSameAs(KEY_DCS_ENABLED)) {
			val.ToLong(&temp1);
			m_dcsEnabled = temp1 == 1L;
		} else if (key.IsSameAs(KEY_CCS_ENABLED)) {
			val.ToLong(&temp1);
			m_ccsEnabled = temp1 == 1L;
		} else if (key.IsSameAs(KEY_CCS_HOST)) {
			m_ccsHost = val;
		} else if (key.IsSameAs(KEY_STARNET_BAND1)) {
			m_starNet1Band = val;
		} else if (key.IsSameAs(KEY_STARNET_CALLSIGN1)) {
			m_starNet1Callsign = val;
		} else if (key.IsSameAs(KEY_STARNET_LOGOFF1)) {
			m_starNet1Logoff = val;
		} else if (key.IsSameAs(KEY_STARNET_INFO1)) {
			m_starNet1Info = val;
		} else if (key.IsSameAs(KEY_STARNET_PERMANENT1)) {
			m_starNet1Permanent = val;
		} else if (key.IsSameAs(KEY_STARNET_USER_TIMEOUT1)) {
			val.ToULong(&temp2);
			m_starNet1UserTimeout = (unsigned int)temp2;
		} else if (key.IsSameAs(KEY_STARNET_GROUP_TIMEOUT1)) {
			val.ToULong(&temp2);
			m_starNet1GroupTimeout = (unsigned int)temp2;
		} else if (key.IsSameAs(KEY_STARNET_CALLSIGN_SWITCH1)) {
			val.ToLong(&temp1);
			m_starNet1CallsignSwitch = STARNET_CALLSIGN_SWITCH(temp1);
		} else if (key.IsSameAs(KEY_STARNET_TXMSG_SWITCH1)) {
			val.ToLong(&temp1);
			m_starNet1TxMsgSwitch = temp1 == 1L;
		} else if (key.IsSameAs(KEY_STARNET_REFLECTOR1)) {
			m_starNet1Reflector = val;
		} else if (key.IsSameAs(KEY_STARNET_BAND2)) {
			m_starNet2Band = val;
		} else if (key.IsSameAs(KEY_STARNET_CALLSIGN2)) {
			m_starNet2Callsign = val;
		} else if (key.IsSameAs(KEY_STARNET_LOGOFF2)) {
			m_starNet2Logoff = val;
		} else if (key.IsSameAs(KEY_STARNET_INFO2)) {
			m_starNet2Info = val;
		} else if (key.IsSameAs(KEY_STARNET_PERMANENT2)) {
			m_starNet2Permanent = val;
		} else if (key.IsSameAs(KEY_STARNET_USER_TIMEOUT2)) {
			val.ToULong(&temp2);
			m_starNet2UserTimeout = (unsigned int)temp2;
		} else if (key.IsSameAs(KEY_STARNET_GROUP_TIMEOUT2)) {
			val.ToULong(&temp2);
			m_starNet2GroupTimeout = (unsigned int)temp2;
		} else if (key.IsSameAs(KEY_STARNET_CALLSIGN_SWITCH2)) {
			val.ToLong(&temp1);
			m_starNet2CallsignSwitch = STARNET_CALLSIGN_SWITCH(temp1);
		} else if (key.IsSameAs(KEY_STARNET_TXMSG_SWITCH2)) {
			val.ToLong(&temp1);
			m_starNet2TxMsgSwitch = temp1 == 1L;
		} else if (key.IsSameAs(KEY_STARNET_REFLECTOR2)) {
			m_starNet2Reflector = val;
		} else if (key.IsSameAs(KEY_STARNET_BAND3)) {
			m_starNet3Band = val;
		} else if (key.IsSameAs(KEY_STARNET_CALLSIGN3)) {
			m_starNet3Callsign = val;
		} else if (key.IsSameAs(KEY_STARNET_LOGOFF3)) {
			m_starNet3Logoff = val;
		} else if (key.IsSameAs(KEY_STARNET_INFO3)) {
			m_starNet3Info = val;
		} else if (key.IsSameAs(KEY_STARNET_PERMANENT3)) {
			m_starNet3Permanent = val;
		} else if (key.IsSameAs(KEY_STARNET_USER_TIMEOUT3)) {
			val.ToULong(&temp2);
			m_starNet3UserTimeout = (unsigned int)temp2;
		} else if (key.IsSameAs(KEY_STARNET_GROUP_TIMEOUT3)) {
			val.ToULong(&temp2);
			m_starNet3GroupTimeout = (unsigned int)temp2;
		} else if (key.IsSameAs(KEY_STARNET_CALLSIGN_SWITCH3)) {
			val.ToLong(&temp1);
			m_starNet3CallsignSwitch = STARNET_CALLSIGN_SWITCH(temp1);
		} else if (key.IsSameAs(KEY_STARNET_TXMSG_SWITCH3)) {
			val.ToLong(&temp1);
			m_starNet3TxMsgSwitch = temp1 == 1L;
		} else if (key.IsSameAs(KEY_STARNET_REFLECTOR3)) {
			m_starNet3Reflector = val;
		} else if (key.IsSameAs(KEY_STARNET_BAND4)) {
			m_starNet4Band = val;
		} else if (key.IsSameAs(KEY_STARNET_CALLSIGN4)) {
			m_starNet4Callsign = val;
		} else if (key.IsSameAs(KEY_STARNET_LOGOFF4)) {
			m_starNet4Logoff = val;
		} else if (key.IsSameAs(KEY_STARNET_INFO4)) {
			m_starNet4Info = val;
		} else if (key.IsSameAs(KEY_STARNET_PERMANENT4)) {
			m_starNet4Permanent = val;
		} else if (key.IsSameAs(KEY_STARNET_USER_TIMEOUT4)) {
			val.ToULong(&temp2);
			m_starNet4UserTimeout = (unsigned int)temp2;
		} else if (key.IsSameAs(KEY_STARNET_GROUP_TIMEOUT4)) {
			val.ToULong(&temp2);
			m_starNet4GroupTimeout = (unsigned int)temp2;
		} else if (key.IsSameAs(KEY_STARNET_CALLSIGN_SWITCH4)) {
			val.ToLong(&temp1);
			m_starNet4CallsignSwitch = STARNET_CALLSIGN_SWITCH(temp1);
		} else if (key.IsSameAs(KEY_STARNET_TXMSG_SWITCH4)) {
			val.ToLong(&temp1);
			m_starNet4TxMsgSwitch = temp1 == 1L;
		} else if (key.IsSameAs(KEY_STARNET_REFLECTOR4)) {
			m_starNet4Reflector = val;
		} else if (key.IsSameAs(KEY_STARNET_BAND5)) {
			m_starNet5Band = val;
		} else if (key.IsSameAs(KEY_STARNET_CALLSIGN5)) {
			m_starNet5Callsign = val;
		} else if (key.IsSameAs(KEY_STARNET_LOGOFF5)) {
			m_starNet5Logoff = val;
		} else if (key.IsSameAs(KEY_STARNET_INFO5)) {
			m_starNet5Info = val;
		} else if (key.IsSameAs(KEY_STARNET_PERMANENT5)) {
			m_starNet5Permanent = val;
		} else if (key.IsSameAs(KEY_STARNET_USER_TIMEOUT5)) {
			val.ToULong(&temp2);
			m_starNet5UserTimeout = (unsigned int)temp2;
		} else if (key.IsSameAs(KEY_STARNET_GROUP_TIMEOUT5)) {
			val.ToULong(&temp2);
			m_starNet5GroupTimeout = (unsigned int)temp2;
		} else if (key.IsSameAs(KEY_STARNET_CALLSIGN_SWITCH5)) {
			val.ToLong(&temp1);
			m_starNet5CallsignSwitch = STARNET_CALLSIGN_SWITCH(temp1);
		} else if (key.IsSameAs(KEY_STARNET_TXMSG_SWITCH5)) {
			val.ToLong(&temp1);
			m_starNet5TxMsgSwitch = temp1 == 1L;
		} else if (key.IsSameAs(KEY_STARNET_REFLECTOR5)) {
			m_starNet5Reflector = val;
		} else if (key.IsSameAs(KEY_REMOTE_ENABLED)) {
			val.ToLong(&temp1);
			m_remoteEnabled = temp1 == 1L;
		} else if (key.IsSameAs(KEY_REMOTE_PASSWORD)) {
			m_remotePassword = val;
		} else if (key.IsSameAs(KEY_REMOTE_PORT)) {
			val.ToULong(&temp2);
			m_remotePort = (unsigned int)temp2;
		} else if (key.IsSameAs(KEY_LANGUAGE)) {
			val.ToLong(&temp1);
			m_language = TEXT_LANG(temp1);
		} else if (key.IsSameAs(KEY_INFO_ENABLED)) {
			val.ToLong(&temp1);
			m_infoEnabled = temp1 == 1L;
		} else if (key.IsSameAs(KEY_ECHO_ENABLED)) {
			val.ToLong(&temp1);
			m_echoEnabled = temp1 == 1L;
		} else if (key.IsSameAs(KEY_LOG_ENABLED)) {
			val.ToLong(&temp1);
			m_logEnabled = temp1 == 1L;
		} else if (key.IsSameAs(KEY_DRATS_ENABLED)) {
			val.ToLong(&temp1);
			m_dratsEnabled = temp1 == 1L;
		} else if (key.IsSameAs(KEY_DTMF_ENABLED)) {
			val.ToLong(&temp1);
			m_dtmfEnabled = temp1 == 1L;
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

CIRCDDBGatewayConfig::~CIRCDDBGatewayConfig()
{
}

#endif

void CIRCDDBGatewayConfig::getGateway(GATEWAY_TYPE& type, wxString& callsign, wxString& address, wxString& icomAddress, unsigned int& icomPort, wxString& hbAddress, unsigned int& hbPort, double& latitude, double& longitude, wxString& description1, wxString& description2, wxString& url) const
{
	type         = m_type;
	callsign     = m_callsign;
	address      = m_address;
	icomAddress  = m_icomAddress;
	icomPort     = m_icomPort;
	hbAddress    = m_hbAddress;
	hbPort       = m_hbPort;
	latitude     = m_latitude;
	longitude    = m_longitude;
	description1 = m_description1;
	description2 = m_description2;
	url          = m_url;

	if (address.IsSameAs(wxT("127.0.0.1")))
		address.Clear();
}

void CIRCDDBGatewayConfig::setGateway(GATEWAY_TYPE type, const wxString& callsign, const wxString& address, const wxString& icomAddress, unsigned int icomPort, const wxString& hbAddress, unsigned int hbPort, double latitude, double longitude, const wxString& description1, const wxString& description2, const wxString& url)
{
	m_type         = type;
	m_callsign     = callsign;
	m_address      = address;
	m_icomAddress  = icomAddress;
	m_icomPort     = icomPort;
	m_hbAddress    = hbAddress;
	m_hbPort       = hbPort;
	m_latitude     = latitude;
	m_longitude    = longitude;
	m_description1 = description1;
	m_description2 = description2;
	m_url          = url;
}

void CIRCDDBGatewayConfig::getRepeater1(wxString& callsign, wxString& band, HW_TYPE& type, wxString& address, unsigned int& port, unsigned char& band1, unsigned char& band2, unsigned char& band3, wxString& reflector, bool& atStartup, RECONNECT& reconnect, double& frequency, double& offset, double& range, double& latitude, double& longitude, double& agl, wxString& description1, wxString& description2, wxString& url) const
{
	callsign  = m_repeater1Callsign;
	band      = m_repeater1Band;
	type      = m_repeater1Type;
	address   = m_repeater1Address;
	port      = m_repeater1Port;
	band1     = m_repeater1Band1;
	band2     = m_repeater1Band2;
	band3     = m_repeater1Band3;
	reflector = m_repeater1Reflector;
	atStartup = m_repeater1AtStartup;
	reconnect = m_repeater1Reconnect;
	frequency = m_repeater1Frequency;
	offset    = m_repeater1Offset;
	range     = m_repeater1Range;
	latitude  = m_repeater1Latitude;
	longitude = m_repeater1Longitude;
	agl       = m_repeater1Agl;
	description1 = m_repeater1Description1;
	description2 = m_repeater1Description2;
	url          = m_repeater1URL;
}

void CIRCDDBGatewayConfig::setRepeater1(const wxString& band, HW_TYPE type, const wxString& address, unsigned int port, unsigned char band1, unsigned char band2, unsigned char band3, const wxString& reflector, bool atStartup, RECONNECT reconnect, double frequency, double offset, double range, double latitude, double longitude, double agl, const wxString& description1, const wxString& description2, const wxString& url)
{
	m_repeater1Band      = band;
	m_repeater1Type      = type;
	m_repeater1Address   = address;
	m_repeater1Port      = port;
	m_repeater1Band1     = band1;
	m_repeater1Band2     = band2;
	m_repeater1Band3     = band3;
	m_repeater1Reflector = reflector;
	m_repeater1AtStartup = atStartup;
	m_repeater1Reconnect = reconnect;
	m_repeater1Frequency = frequency;
	m_repeater1Offset    = offset;
	m_repeater1Range     = range;
	m_repeater1Latitude  = latitude;
	m_repeater1Longitude = longitude;
	m_repeater1Agl       = agl;
	m_repeater1Description1 = description1;
	m_repeater1Description2 = description2;
	m_repeater1URL          = url;
}

void CIRCDDBGatewayConfig::getRepeater2(wxString& callsign, wxString& band, HW_TYPE& type, wxString& address, unsigned int& port, unsigned char& band1, unsigned char& band2, unsigned char& band3, wxString& reflector, bool& atStartup, RECONNECT& reconnect, double& frequency, double& offset, double& range, double& latitude, double& longitude, double& agl, wxString& description1, wxString& description2, wxString& url) const
{
	callsign  = m_repeater2Callsign;
	band      = m_repeater2Band;
	type      = m_repeater2Type;
	address   = m_repeater2Address;
	port      = m_repeater2Port;
	band1     = m_repeater2Band1;
	band2     = m_repeater2Band2;
	band3     = m_repeater2Band3;
	reflector = m_repeater2Reflector;
	atStartup = m_repeater2AtStartup;
	reconnect = m_repeater2Reconnect;
	frequency = m_repeater2Frequency;
	offset    = m_repeater2Offset;
	range     = m_repeater2Range;
	latitude  = m_repeater2Latitude;
	longitude = m_repeater2Longitude;
	agl       = m_repeater2Agl;
	description1 = m_repeater2Description1;
	description2 = m_repeater2Description2;
	url          = m_repeater2URL;
}

void CIRCDDBGatewayConfig::setRepeater2(const wxString& band, HW_TYPE type, const wxString& address, unsigned int port, unsigned char band1, unsigned char band2, unsigned char band3, const wxString& reflector, bool atStartup, RECONNECT reconnect, double frequency, double offset, double range, double latitude, double longitude, double agl, const wxString& description1, const wxString& description2, const wxString& url)
{
	m_repeater2Band      = band;
	m_repeater2Type      = type;
	m_repeater2Address   = address;
	m_repeater2Port      = port;
	m_repeater2Band1     = band1;
	m_repeater2Band2     = band2;
	m_repeater2Band3     = band3;
	m_repeater2Reflector = reflector;
	m_repeater2AtStartup = atStartup;
	m_repeater2Reconnect = reconnect;
	m_repeater2Frequency = frequency;
	m_repeater2Offset    = offset;
	m_repeater2Range     = range;
	m_repeater2Latitude  = latitude;
	m_repeater2Longitude = longitude;
	m_repeater2Agl       = agl;
	m_repeater2Description1 = description1;
	m_repeater2Description2 = description2;
	m_repeater2URL          = url;
}

void CIRCDDBGatewayConfig::getRepeater3(wxString& callsign, wxString& band, HW_TYPE& type, wxString& address, unsigned int& port, unsigned char& band1, unsigned char& band2, unsigned char& band3, wxString& reflector, bool& atStartup, RECONNECT& reconnect, double& frequency, double& offset, double& range, double& latitude, double& longitude, double& agl, wxString& description1, wxString& description2, wxString& url) const
{
	callsign  = m_repeater3Callsign;
	band      = m_repeater3Band;
	type      = m_repeater3Type;
	address   = m_repeater3Address;
	port      = m_repeater3Port;
	band1     = m_repeater3Band1;
	band2     = m_repeater3Band2;
	band3     = m_repeater3Band3;
	reflector = m_repeater3Reflector;
	atStartup = m_repeater3AtStartup;
	reconnect = m_repeater3Reconnect;
	frequency = m_repeater3Frequency;
	offset    = m_repeater3Offset;
	range     = m_repeater3Range;
	latitude  = m_repeater3Latitude;
	longitude = m_repeater3Longitude;
	agl       = m_repeater3Agl;
	description1 = m_repeater3Description1;
	description2 = m_repeater3Description2;
	url          = m_repeater3URL;
}

void CIRCDDBGatewayConfig::setRepeater3(const wxString& band, HW_TYPE type, const wxString& address, unsigned int port, unsigned char band1, unsigned char band2, unsigned char band3, const wxString& reflector, bool atStartup, RECONNECT reconnect, double frequency, double offset, double range, double latitude, double longitude, double agl, const wxString& description1, const wxString& description2, const wxString& url)
{
	m_repeater3Band      = band;
	m_repeater3Type      = type;
	m_repeater3Address   = address;
	m_repeater3Port      = port;
	m_repeater3Band1     = band1;
	m_repeater3Band2     = band2;
	m_repeater3Band3     = band3;
	m_repeater3Reflector = reflector;
	m_repeater3AtStartup = atStartup;
	m_repeater3Reconnect = reconnect;
	m_repeater3Frequency = frequency;
	m_repeater3Offset    = offset;
	m_repeater3Range     = range;
	m_repeater3Latitude  = latitude;
	m_repeater3Longitude = longitude;
	m_repeater3Agl       = agl;
	m_repeater3Description1 = description1;
	m_repeater3Description2 = description2;
	m_repeater3URL          = url;
}

void CIRCDDBGatewayConfig::getRepeater4(wxString& callsign, wxString& band, HW_TYPE& type, wxString& address, unsigned int& port, unsigned char& band1, unsigned char& band2, unsigned char& band3, wxString& reflector, bool& atStartup, RECONNECT& reconnect, double& frequency, double& offset, double& range, double& latitude, double& longitude, double& agl, wxString& description1, wxString& description2, wxString& url) const
{
	callsign  = m_repeater4Callsign;
	band      = m_repeater4Band;
	type      = m_repeater4Type;
	address   = m_repeater4Address;
	port      = m_repeater4Port;
	band1     = m_repeater4Band1;
	band2     = m_repeater4Band2;
	band3     = m_repeater4Band3;
	reflector = m_repeater4Reflector;
	atStartup = m_repeater4AtStartup;
	reconnect = m_repeater4Reconnect;
	frequency = m_repeater4Frequency;
	offset    = m_repeater4Offset;
	range     = m_repeater4Range;
	latitude  = m_repeater4Latitude;
	longitude = m_repeater4Longitude;
	agl       = m_repeater4Agl;
	description1 = m_repeater4Description1;
	description2 = m_repeater4Description2;
	url          = m_repeater4URL;
}

void CIRCDDBGatewayConfig::setRepeater4(const wxString& band, HW_TYPE type, const wxString& address, unsigned int port, unsigned char band1, unsigned char band2, unsigned char band3, const wxString& reflector, bool atStartup, RECONNECT reconnect, double frequency, double offset, double range, double latitude, double longitude, double agl, const wxString& description1, const wxString& description2, const wxString& url)
{
	m_repeater4Band      = band;
	m_repeater4Type      = type;
	m_repeater4Address   = address;
	m_repeater4Port      = port;
	m_repeater4Band1     = band1;
	m_repeater4Band2     = band2;
	m_repeater4Band3     = band3;
	m_repeater4Reflector = reflector;
	m_repeater4AtStartup = atStartup;
	m_repeater4Reconnect = reconnect;
	m_repeater4Frequency = frequency;
	m_repeater4Offset    = offset;
	m_repeater4Range     = range;
	m_repeater4Latitude  = latitude;
	m_repeater4Longitude = longitude;
	m_repeater4Agl       = agl;
	m_repeater4Description1 = description1;
	m_repeater4Description2 = description2;
	m_repeater4URL          = url;
}

void CIRCDDBGatewayConfig::getIrcDDB(bool& enabled, wxString& hostname, wxString& username, wxString& password) const
{
	enabled  = m_ircddbEnabled;
	hostname = m_ircddbHostname;
	username = m_ircddbUsername;
	password = m_ircddbPassword;
}

void CIRCDDBGatewayConfig::setIrcDDB(bool enabled, const wxString& hostname, const wxString& username, const wxString& password)
{
	m_ircddbEnabled  = enabled;
	m_ircddbHostname = hostname;
	m_ircddbUsername = username;
	m_ircddbPassword = password;
}

void CIRCDDBGatewayConfig::getDPRS(bool& enabled, wxString& hostname, unsigned int& port) const
{
	enabled  = m_aprsEnabled;
	hostname = m_aprsHostname;
	port     = m_aprsPort;
}

void CIRCDDBGatewayConfig::setDPRS(bool enabled, const wxString& hostname, unsigned int port)
{
	m_aprsEnabled  = enabled;
	m_aprsHostname = hostname;
	m_aprsPort     = port;
}

void CIRCDDBGatewayConfig::getDExtra(bool& enabled, unsigned int& maxDongles) const
{
	enabled    = m_dextraEnabled;
	maxDongles = m_dextraMaxDongles;
}

void CIRCDDBGatewayConfig::setDExtra(bool enabled, unsigned int maxDongles)
{
	m_dextraEnabled    = enabled;
	m_dextraMaxDongles = maxDongles;
}

void CIRCDDBGatewayConfig::getDPlus(bool& enabled, unsigned int& maxDongles, wxString& login) const
{
	enabled    = m_dplusEnabled;
	maxDongles = m_dplusMaxDongles;
	login      = m_dplusLogin;
}

void CIRCDDBGatewayConfig::setDPlus(bool enabled, unsigned int maxDongles, const wxString& login)
{
	m_dplusEnabled    = enabled;
	m_dplusMaxDongles = maxDongles;
	m_dplusLogin      = login;
}

void CIRCDDBGatewayConfig::getDCS(bool& dcsEnabled, bool& ccsEnabled, wxString& ccsHost) const
{
	dcsEnabled = m_dcsEnabled;
	ccsEnabled = m_ccsEnabled;
	ccsHost    = m_ccsHost;
}

void CIRCDDBGatewayConfig::setDCS(bool dcsEnabled, bool ccsEnabled, const wxString& ccsHost)
{
	m_dcsEnabled = dcsEnabled;
	m_ccsEnabled = ccsEnabled;
	m_ccsHost    = ccsHost;
}

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
void CIRCDDBGatewayConfig::getStarNet1(wxString& band, wxString& callsign, wxString& logoff, wxString& info, wxString& permanent, unsigned int& userTimeout, unsigned int& groupTimeout, STARNET_CALLSIGN_SWITCH& callsignSwitch, bool& txMsgSwitch, wxString& reflector) const
#else
void CIRCDDBGatewayConfig::getStarNet1(wxString& band, wxString& callsign, wxString& logoff, wxString& info, wxString& permanent, unsigned int& userTimeout, unsigned int& groupTimeout, STARNET_CALLSIGN_SWITCH& callsignSwitch, bool& txMsgSwitch) const
#endif
{
	band           = m_starNet1Band;
	callsign       = m_starNet1Callsign;
	logoff         = m_starNet1Logoff;
	info           = m_starNet1Info;
	permanent      = m_starNet1Permanent;
	userTimeout    = m_starNet1UserTimeout;
	groupTimeout   = m_starNet1GroupTimeout;
	callsignSwitch = m_starNet1CallsignSwitch;
	txMsgSwitch    = m_starNet1TxMsgSwitch;

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
	reflector      = m_starNet1Reflector;
#endif
}

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
void CIRCDDBGatewayConfig::setStarNet1(const wxString& band, const wxString& callsign, const wxString& logoff, const wxString& info, const wxString& permanent, unsigned int userTimeout, unsigned int groupTimeout, STARNET_CALLSIGN_SWITCH callsignSwitch, bool txMsgSwitch, const wxString& reflector)
#else
void CIRCDDBGatewayConfig::setStarNet1(const wxString& band, const wxString& callsign, const wxString& logoff, const wxString& info, const wxString& permanent, unsigned int userTimeout, unsigned int groupTimeout, STARNET_CALLSIGN_SWITCH callsignSwitch, bool txMsgSwitch)
#endif
{
	m_starNet1Band           = band;
	m_starNet1Callsign       = callsign;
	m_starNet1Logoff         = logoff;
	m_starNet1Info           = info;
	m_starNet1Permanent      = permanent;
	m_starNet1UserTimeout    = userTimeout;
	m_starNet1GroupTimeout   = groupTimeout;
	m_starNet1CallsignSwitch = callsignSwitch;
	m_starNet1TxMsgSwitch    = txMsgSwitch;

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
	m_starNet1Reflector      = reflector;
#endif
}

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
void CIRCDDBGatewayConfig::getStarNet2(wxString& band, wxString& callsign, wxString& logoff, wxString& info, wxString& permanent, unsigned int& userTimeout, unsigned int& groupTimeout, STARNET_CALLSIGN_SWITCH& callsignSwitch, bool& txMsgSwitch, wxString& reflector) const
#else
void CIRCDDBGatewayConfig::getStarNet2(wxString& band, wxString& callsign, wxString& logoff, wxString& info, wxString& permanent, unsigned int& userTimeout, unsigned int& groupTimeout, STARNET_CALLSIGN_SWITCH& callsignSwitch, bool& txMsgSwitch) const
#endif
{
	band           = m_starNet2Band;
	callsign       = m_starNet2Callsign;
	logoff         = m_starNet2Logoff;
	info           = m_starNet2Info;
	permanent      = m_starNet2Permanent;
	userTimeout    = m_starNet2UserTimeout;
	groupTimeout   = m_starNet2GroupTimeout;
	callsignSwitch = m_starNet2CallsignSwitch;
	txMsgSwitch    = m_starNet2TxMsgSwitch;

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
	reflector      = m_starNet2Reflector;
#endif
}

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
void CIRCDDBGatewayConfig::setStarNet2(const wxString& band, const wxString& callsign, const wxString& logoff, const wxString& info, const wxString& permanent, unsigned int userTimeout, unsigned int groupTimeout, STARNET_CALLSIGN_SWITCH callsignSwitch, bool txMsgSwitch, const wxString& reflector)
#else
void CIRCDDBGatewayConfig::setStarNet2(const wxString& band, const wxString& callsign, const wxString& logoff, const wxString& info, const wxString& permanent, unsigned int userTimeout, unsigned int groupTimeout, STARNET_CALLSIGN_SWITCH callsignSwitch, bool txMsgSwitch)
#endif
{
	m_starNet2Band           = band;
	m_starNet2Callsign       = callsign;
	m_starNet2Logoff         = logoff;
	m_starNet2Info           = info;
	m_starNet2Permanent      = permanent;
	m_starNet2UserTimeout    = userTimeout;
	m_starNet2GroupTimeout   = groupTimeout;
	m_starNet2CallsignSwitch = callsignSwitch;
	m_starNet2TxMsgSwitch    = txMsgSwitch;

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
	m_starNet2Reflector      = reflector;
#endif
}

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
void CIRCDDBGatewayConfig::getStarNet3(wxString& band, wxString& callsign, wxString& logoff, wxString& info, wxString& permanent, unsigned int& userTimeout, unsigned int& groupTimeout, STARNET_CALLSIGN_SWITCH& callsignSwitch, bool& txMsgSwitch, wxString& reflector) const
#else
void CIRCDDBGatewayConfig::getStarNet3(wxString& band, wxString& callsign, wxString& logoff, wxString& info, wxString& permanent, unsigned int& userTimeout, unsigned int& groupTimeout, STARNET_CALLSIGN_SWITCH& callsignSwitch, bool& txMsgSwitch) const
#endif
{
	band           = m_starNet3Band;
	callsign       = m_starNet3Callsign;
	logoff         = m_starNet3Logoff;
	info           = m_starNet3Info;
	permanent      = m_starNet3Permanent;
	userTimeout    = m_starNet3UserTimeout;
	groupTimeout   = m_starNet3GroupTimeout;
	callsignSwitch = m_starNet3CallsignSwitch;
	txMsgSwitch    = m_starNet3TxMsgSwitch;

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
	reflector      = m_starNet3Reflector;
#endif
}

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
void CIRCDDBGatewayConfig::setStarNet3(const wxString& band, const wxString& callsign, const wxString& logoff, const wxString& info, const wxString& permanent, unsigned int userTimeout, unsigned int groupTimeout, STARNET_CALLSIGN_SWITCH callsignSwitch, bool txMsgSwitch, const wxString& reflector)
#else
void CIRCDDBGatewayConfig::setStarNet3(const wxString& band, const wxString& callsign, const wxString& logoff, const wxString& info, const wxString& permanent, unsigned int userTimeout, unsigned int groupTimeout, STARNET_CALLSIGN_SWITCH callsignSwitch, bool txMsgSwitch)
#endif
{
	m_starNet3Band           = band;
	m_starNet3Callsign       = callsign;
	m_starNet3Logoff         = logoff;
	m_starNet3Info           = info;
	m_starNet3Permanent      = permanent;
	m_starNet3UserTimeout    = userTimeout;
	m_starNet3GroupTimeout   = groupTimeout;
	m_starNet3CallsignSwitch = callsignSwitch;
	m_starNet3TxMsgSwitch    = txMsgSwitch;

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
	m_starNet3Reflector      = reflector;
#endif
}

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
void CIRCDDBGatewayConfig::getStarNet4(wxString& band, wxString& callsign, wxString& logoff, wxString& info, wxString& permanent, unsigned int& userTimeout, unsigned int& groupTimeout, STARNET_CALLSIGN_SWITCH& callsignSwitch, bool& txMsgSwitch, wxString& reflector) const
#else
void CIRCDDBGatewayConfig::getStarNet4(wxString& band, wxString& callsign, wxString& logoff, wxString& info, wxString& permanent, unsigned int& userTimeout, unsigned int& groupTimeout, STARNET_CALLSIGN_SWITCH& callsignSwitch, bool& txMsgSwitch) const
#endif
{
	band           = m_starNet4Band;
	callsign       = m_starNet4Callsign;
	logoff         = m_starNet4Logoff;
	info           = m_starNet4Info;
	permanent      = m_starNet4Permanent;
	userTimeout    = m_starNet4UserTimeout;
	groupTimeout   = m_starNet4GroupTimeout;
	callsignSwitch = m_starNet4CallsignSwitch;
	txMsgSwitch    = m_starNet4TxMsgSwitch;

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
	reflector      = m_starNet4Reflector;
#endif
}

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
void CIRCDDBGatewayConfig::setStarNet4(const wxString& band, const wxString& callsign, const wxString& logoff, const wxString& info, const wxString& permanent, unsigned int userTimeout, unsigned int groupTimeout, STARNET_CALLSIGN_SWITCH callsignSwitch, bool txMsgSwitch, const wxString& reflector)
#else
void CIRCDDBGatewayConfig::setStarNet4(const wxString& band, const wxString& callsign, const wxString& logoff, const wxString& info, const wxString& permanent, unsigned int userTimeout, unsigned int groupTimeout, STARNET_CALLSIGN_SWITCH callsignSwitch, bool txMsgSwitch)
#endif
{
	m_starNet4Band           = band;
	m_starNet4Callsign       = callsign;
	m_starNet4Logoff         = logoff;
	m_starNet4Info           = info;
	m_starNet4Permanent      = permanent;
	m_starNet4UserTimeout    = userTimeout;
	m_starNet4GroupTimeout   = groupTimeout;
	m_starNet4CallsignSwitch = callsignSwitch;
	m_starNet4TxMsgSwitch    = txMsgSwitch;

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
	m_starNet4Reflector      = reflector;
#endif
}

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
void CIRCDDBGatewayConfig::getStarNet5(wxString& band, wxString& callsign, wxString& logoff, wxString& info, wxString& permanent, unsigned int& userTimeout, unsigned int& groupTimeout, STARNET_CALLSIGN_SWITCH& callsignSwitch, bool& txMsgSwitch, wxString& reflector) const
#else
void CIRCDDBGatewayConfig::getStarNet5(wxString& band, wxString& callsign, wxString& logoff, wxString& info, wxString& permanent, unsigned int& userTimeout, unsigned int& groupTimeout, STARNET_CALLSIGN_SWITCH& callsignSwitch, bool& txMsgSwitch) const
#endif
{
	band           = m_starNet5Band;
	callsign       = m_starNet5Callsign;
	logoff         = m_starNet5Logoff;
	info           = m_starNet5Info;
	permanent      = m_starNet5Permanent;
	userTimeout    = m_starNet5UserTimeout;
	groupTimeout   = m_starNet5GroupTimeout;
	callsignSwitch = m_starNet5CallsignSwitch;
	txMsgSwitch    = m_starNet5TxMsgSwitch;

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
	reflector      = m_starNet5Reflector;
#endif
}

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
void CIRCDDBGatewayConfig::setStarNet5(const wxString& band, const wxString& callsign, const wxString& logoff, const wxString& info, const wxString& permanent, unsigned int userTimeout, unsigned int groupTimeout, STARNET_CALLSIGN_SWITCH callsignSwitch, bool txMsgSwitch, const wxString& reflector)
#else
void CIRCDDBGatewayConfig::setStarNet5(const wxString& band, const wxString& callsign, const wxString& logoff, const wxString& info, const wxString& permanent, unsigned int userTimeout, unsigned int groupTimeout, STARNET_CALLSIGN_SWITCH callsignSwitch, bool txMsgSwitch)
#endif
{
	m_starNet5Band           = band;
	m_starNet5Callsign       = callsign;
	m_starNet5Logoff         = logoff;
	m_starNet5Info           = info;
	m_starNet5Permanent      = permanent;
	m_starNet5UserTimeout    = userTimeout;
	m_starNet5GroupTimeout   = groupTimeout;
	m_starNet5CallsignSwitch = callsignSwitch;
	m_starNet5TxMsgSwitch    = txMsgSwitch;

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
	m_starNet5Reflector      = reflector;
#endif
}

void CIRCDDBGatewayConfig::getRemote(bool& enabled, wxString& password, unsigned int& port) const
{
	enabled  = m_remoteEnabled;
	password = m_remotePassword;
	port     = m_remotePort;
}

void CIRCDDBGatewayConfig::setRemote(bool enabled, const wxString& password, unsigned int port)
{
	m_remoteEnabled  = enabled;
	m_remotePassword = password;
	m_remotePort     = port;
}

void CIRCDDBGatewayConfig::getMiscellaneous(TEXT_LANG& language, bool& infoEnabled, bool& echoEnabled, bool& logEnabled, bool& dratsEnabled, bool& dtmfEnabled) const
{
	language     = m_language;
	infoEnabled  = m_infoEnabled;
	echoEnabled  = m_echoEnabled;
	logEnabled   = m_logEnabled;
	dratsEnabled = m_dratsEnabled;
	dtmfEnabled  = m_dtmfEnabled;
}

void CIRCDDBGatewayConfig::setMiscellaneous(TEXT_LANG language, bool infoEnabled, bool echoEnabled, bool logEnabled, bool dratsEnabled, bool dtmfEnabled)
{
	m_language     = language;
	m_infoEnabled  = infoEnabled;
	m_echoEnabled  = echoEnabled;
	m_logEnabled   = logEnabled;
	m_dratsEnabled = dratsEnabled;
	m_dtmfEnabled  = dtmfEnabled;
}

void CIRCDDBGatewayConfig::getPosition(int& x, int& y) const
{
	x = m_x;
	y = m_y;
}

void CIRCDDBGatewayConfig::setPosition(int x, int y)
{
	m_x = x;
	m_y = y;
}

bool CIRCDDBGatewayConfig::write()
{
#if defined(__WINDOWS__)
	wxString text;

	m_config->Write(m_name + KEY_GATEWAY_TYPE, long(m_type));
	m_config->Write(m_name + KEY_GATEWAY_CALLSIGN, m_callsign);
	m_config->Write(m_name + KEY_GATEWAY_ADDRESS, m_address);
	m_config->Write(m_name + KEY_ICOM_ADDRESS, m_icomAddress);
	m_config->Write(m_name + KEY_ICOM_PORT, long(m_icomPort));
	m_config->Write(m_name + KEY_HB_ADDRESS, m_hbAddress);
	m_config->Write(m_name + KEY_HB_PORT, long(m_hbPort));

	text.Printf(wxT("%.6lf"), m_latitude);
	m_config->Write(m_name + KEY_LATITUDE, text);

	text.Printf(wxT("%.6lf"), m_longitude);
	m_config->Write(m_name + KEY_LONGITUDE, text);

	m_config->Write(m_name + KEY_DESCRIPTION1, m_description1);
	m_config->Write(m_name + KEY_DESCRIPTION2, m_description2);
	m_config->Write(m_name + KEY_URL, m_url);
	m_config->Write(m_name + KEY_REPEATER_BAND1, m_repeater1Band);
	m_config->Write(m_name + KEY_REPEATER_TYPE1, long(m_repeater1Type));
	m_config->Write(m_name + KEY_REPEATER_ADDRESS1, m_repeater1Address);
	m_config->Write(m_name + KEY_REPEATER_PORT1, long(m_repeater1Port));
	m_config->Write(m_name + KEY_REFLECTOR1, m_repeater1Reflector);
	m_config->Write(m_name + KEY_ATSTARTUP1, m_repeater1AtStartup);
	m_config->Write(m_name + KEY_RECONNECT1, long(m_repeater1Reconnect));

	text.Printf(wxT("%.5lf"), m_repeater1Frequency);
	m_config->Write(m_name + KEY_FREQUENCY1, text);

	text.Printf(wxT("%.4lf"), m_repeater1Offset);
	m_config->Write(m_name + KEY_OFFSET1, text);

	m_config->Write(m_name + KEY_RANGE1, m_repeater1Range);

	text.Printf(wxT("%.6lf"), m_repeater1Latitude);
	m_config->Write(m_name + KEY_LATITUDE1, text);

	text.Printf(wxT("%.6lf"), m_repeater1Longitude);
	m_config->Write(m_name + KEY_LONGITUDE1, text);

	m_config->Write(m_name + KEY_AGL1, m_repeater1Agl);
	m_config->Write(m_name + KEY_DESCRIPTION11, m_repeater1Description1);
	m_config->Write(m_name + KEY_DESCRIPTION12, m_repeater1Description2);
	m_config->Write(m_name + KEY_URL1, m_repeater1URL);
	m_config->Write(m_name + KEY_BAND11, long(m_repeater1Band1));
	m_config->Write(m_name + KEY_BAND12, long(m_repeater1Band2));
	m_config->Write(m_name + KEY_BAND13, long(m_repeater1Band3));
	m_config->Write(m_name + KEY_REPEATER_BAND2, m_repeater2Band);
	m_config->Write(m_name + KEY_REPEATER_TYPE2, long(m_repeater2Type));
	m_config->Write(m_name + KEY_REPEATER_ADDRESS2, m_repeater2Address);
	m_config->Write(m_name + KEY_REPEATER_PORT2, long(m_repeater2Port));
	m_config->Write(m_name + KEY_REFLECTOR2, m_repeater2Reflector);
	m_config->Write(m_name + KEY_ATSTARTUP2, m_repeater2AtStartup);
	m_config->Write(m_name + KEY_RECONNECT2, long(m_repeater2Reconnect));

	text.Printf(wxT("%.5lf"), m_repeater2Frequency);
	m_config->Write(m_name + KEY_FREQUENCY2, text);

	text.Printf(wxT("%.4lf"), m_repeater2Offset);
	m_config->Write(m_name + KEY_OFFSET2, text);

	m_config->Write(m_name + KEY_RANGE2, m_repeater2Range);

	text.Printf(wxT("%.6lf"), m_repeater2Latitude);
	m_config->Write(m_name + KEY_LATITUDE2, text);

	text.Printf(wxT("%.6lf"), m_repeater2Longitude);
	m_config->Write(m_name + KEY_LONGITUDE2, text);

	m_config->Write(m_name + KEY_AGL2, m_repeater2Agl);
	m_config->Write(m_name + KEY_DESCRIPTION21, m_repeater2Description1);
	m_config->Write(m_name + KEY_DESCRIPTION22, m_repeater2Description2);
	m_config->Write(m_name + KEY_URL2, m_repeater2URL);
	m_config->Write(m_name + KEY_BAND21, long(m_repeater2Band1));
	m_config->Write(m_name + KEY_BAND22, long(m_repeater2Band2));
	m_config->Write(m_name + KEY_BAND23, long(m_repeater2Band3));
	m_config->Write(m_name + KEY_REPEATER_BAND3, m_repeater3Band);
	m_config->Write(m_name + KEY_REPEATER_TYPE3, long(m_repeater3Type));
	m_config->Write(m_name + KEY_REPEATER_ADDRESS3, m_repeater3Address);
	m_config->Write(m_name + KEY_REPEATER_PORT3, long(m_repeater3Port));
	m_config->Write(m_name + KEY_REFLECTOR3, m_repeater3Reflector);
	m_config->Write(m_name + KEY_ATSTARTUP3, m_repeater3AtStartup);
	m_config->Write(m_name + KEY_RECONNECT3, long(m_repeater3Reconnect));

	text.Printf(wxT("%.5lf"), m_repeater3Frequency);
	m_config->Write(m_name + KEY_FREQUENCY3, text);

	text.Printf(wxT("%.4lf"), m_repeater3Offset);
	m_config->Write(m_name + KEY_OFFSET3, text);

	m_config->Write(m_name + KEY_RANGE3, m_repeater3Range);

	text.Printf(wxT("%.6lf"), m_repeater3Latitude);
	m_config->Write(m_name + KEY_LATITUDE3, text);

	text.Printf(wxT("%.6lf"), m_repeater3Longitude);
	m_config->Write(m_name + KEY_LONGITUDE3, text);

	m_config->Write(m_name + KEY_AGL3, m_repeater3Agl);
	m_config->Write(m_name + KEY_DESCRIPTION31, m_repeater3Description1);
	m_config->Write(m_name + KEY_DESCRIPTION32, m_repeater3Description2);
	m_config->Write(m_name + KEY_URL3, m_repeater3URL);
	m_config->Write(m_name + KEY_BAND31, long(m_repeater3Band1));
	m_config->Write(m_name + KEY_BAND32, long(m_repeater3Band2));
	m_config->Write(m_name + KEY_BAND33, long(m_repeater3Band3));
	m_config->Write(m_name + KEY_REPEATER_BAND4, m_repeater4Band);
	m_config->Write(m_name + KEY_REPEATER_TYPE4, long(m_repeater4Type));
	m_config->Write(m_name + KEY_REPEATER_ADDRESS4, m_repeater4Address);
	m_config->Write(m_name + KEY_REPEATER_PORT4, long(m_repeater4Port));
	m_config->Write(m_name + KEY_REFLECTOR4, m_repeater4Reflector);
	m_config->Write(m_name + KEY_ATSTARTUP4, m_repeater4AtStartup);
	m_config->Write(m_name + KEY_RECONNECT4, long(m_repeater4Reconnect));

	text.Printf(wxT("%.5lf"), m_repeater4Frequency);
	m_config->Write(m_name + KEY_FREQUENCY4, text);

	text.Printf(wxT("%.4lf"), m_repeater4Offset);
	m_config->Write(m_name + KEY_OFFSET4, text);

	m_config->Write(m_name + KEY_RANGE4, m_repeater4Range);

	text.Printf(wxT("%.6lf"), m_repeater4Latitude);
	m_config->Write(m_name + KEY_LATITUDE4, text);

	text.Printf(wxT("%.6lf"), m_repeater4Longitude);
	m_config->Write(m_name + KEY_LONGITUDE4, text);

	m_config->Write(m_name + KEY_AGL4, m_repeater4Agl);
	m_config->Write(m_name + KEY_DESCRIPTION41, m_repeater4Description1);
	m_config->Write(m_name + KEY_DESCRIPTION42, m_repeater4Description2);
	m_config->Write(m_name + KEY_URL4, m_repeater4URL);
	m_config->Write(m_name + KEY_BAND41, long(m_repeater4Band1));
	m_config->Write(m_name + KEY_BAND42, long(m_repeater4Band2));
	m_config->Write(m_name + KEY_BAND43, long(m_repeater4Band3));
	m_config->Write(m_name + KEY_IRCDDB_ENABLED, m_ircddbEnabled);
	m_config->Write(m_name + KEY_IRCDDB_HOSTNAME, m_ircddbHostname);
	m_config->Write(m_name + KEY_IRCDDB_USERNAME, m_ircddbUsername);
	m_config->Write(m_name + KEY_IRCDDB_PASSWORD, m_ircddbPassword);
	m_config->Write(m_name + KEY_APRS_ENABLED, m_aprsEnabled);
	m_config->Write(m_name + KEY_APRS_HOSTNAME, m_aprsHostname);
	m_config->Write(m_name + KEY_APRS_PORT, long(m_aprsPort));
	m_config->Write(m_name + KEY_DEXTRA_ENABLED, m_dextraEnabled);
	m_config->Write(m_name + KEY_DEXTRA_MAXDONGLES, long(m_dextraMaxDongles));
	m_config->Write(m_name + KEY_DPLUS_ENABLED, m_dplusEnabled);
	m_config->Write(m_name + KEY_DPLUS_MAXDONGLES, long(m_dplusMaxDongles));
	m_config->Write(m_name + KEY_DPLUS_LOGIN, m_dplusLogin);
	m_config->Write(m_name + KEY_DCS_ENABLED, m_dcsEnabled);
	m_config->Write(m_name + KEY_CCS_ENABLED, m_ccsEnabled);
	m_config->Write(m_name + KEY_CCS_HOST, m_ccsHost);
	m_config->Write(m_name + KEY_STARNET_BAND1, m_starNet1Band);
	m_config->Write(m_name + KEY_STARNET_CALLSIGN1, m_starNet1Callsign);
	m_config->Write(m_name + KEY_STARNET_LOGOFF1, m_starNet1Logoff);
	m_config->Write(m_name + KEY_STARNET_INFO1, m_starNet1Info);
	m_config->Write(m_name + KEY_STARNET_PERMANENT1, m_starNet1Permanent);
	m_config->Write(m_name + KEY_STARNET_USER_TIMEOUT1, long(m_starNet1UserTimeout));
	m_config->Write(m_name + KEY_STARNET_GROUP_TIMEOUT1, long(m_starNet1GroupTimeout));
	m_config->Write(m_name + KEY_STARNET_CALLSIGN_SWITCH1, long(m_starNet1CallsignSwitch));
	m_config->Write(m_name + KEY_STARNET_TXMSG_SWITCH1, long(m_starNet1TxMsgSwitch));
	m_config->Write(m_name + KEY_STARNET_REFLECTOR1, m_starNet1Reflector);
	m_config->Write(m_name + KEY_STARNET_BAND2, m_starNet2Band);
	m_config->Write(m_name + KEY_STARNET_CALLSIGN2, m_starNet2Callsign);
	m_config->Write(m_name + KEY_STARNET_LOGOFF2, m_starNet2Logoff);
	m_config->Write(m_name + KEY_STARNET_INFO2, m_starNet2Info);
	m_config->Write(m_name + KEY_STARNET_PERMANENT2, m_starNet2Permanent);
	m_config->Write(m_name + KEY_STARNET_USER_TIMEOUT2, long(m_starNet2UserTimeout));
	m_config->Write(m_name + KEY_STARNET_GROUP_TIMEOUT2, long(m_starNet2GroupTimeout));
	m_config->Write(m_name + KEY_STARNET_CALLSIGN_SWITCH2, long(m_starNet2CallsignSwitch));
	m_config->Write(m_name + KEY_STARNET_TXMSG_SWITCH2, long(m_starNet2TxMsgSwitch));
	m_config->Write(m_name + KEY_STARNET_REFLECTOR2, m_starNet2Reflector);
	m_config->Write(m_name + KEY_STARNET_BAND3, m_starNet3Band);
	m_config->Write(m_name + KEY_STARNET_CALLSIGN3, m_starNet3Callsign);
	m_config->Write(m_name + KEY_STARNET_LOGOFF3, m_starNet3Logoff);
	m_config->Write(m_name + KEY_STARNET_INFO3, m_starNet3Info);
	m_config->Write(m_name + KEY_STARNET_PERMANENT3, m_starNet3Permanent);
	m_config->Write(m_name + KEY_STARNET_USER_TIMEOUT3, long(m_starNet3UserTimeout));
	m_config->Write(m_name + KEY_STARNET_GROUP_TIMEOUT3, long(m_starNet3GroupTimeout));
	m_config->Write(m_name + KEY_STARNET_CALLSIGN_SWITCH3, long(m_starNet3CallsignSwitch));
	m_config->Write(m_name + KEY_STARNET_TXMSG_SWITCH3, long(m_starNet3TxMsgSwitch));
	m_config->Write(m_name + KEY_STARNET_REFLECTOR3, m_starNet3Reflector);
	m_config->Write(m_name + KEY_STARNET_BAND4, m_starNet4Band);
	m_config->Write(m_name + KEY_STARNET_CALLSIGN4, m_starNet4Callsign);
	m_config->Write(m_name + KEY_STARNET_LOGOFF4, m_starNet4Logoff);
	m_config->Write(m_name + KEY_STARNET_INFO4, m_starNet4Info);
	m_config->Write(m_name + KEY_STARNET_PERMANENT4, m_starNet4Permanent);
	m_config->Write(m_name + KEY_STARNET_USER_TIMEOUT4, long(m_starNet4UserTimeout));
	m_config->Write(m_name + KEY_STARNET_GROUP_TIMEOUT4, long(m_starNet4GroupTimeout));
	m_config->Write(m_name + KEY_STARNET_CALLSIGN_SWITCH4, long(m_starNet4CallsignSwitch));
	m_config->Write(m_name + KEY_STARNET_TXMSG_SWITCH4, long(m_starNet4TxMsgSwitch));
	m_config->Write(m_name + KEY_STARNET_REFLECTOR4, m_starNet4Reflector);
	m_config->Write(m_name + KEY_STARNET_BAND5, m_starNet5Band);
	m_config->Write(m_name + KEY_STARNET_CALLSIGN5, m_starNet5Callsign);
	m_config->Write(m_name + KEY_STARNET_LOGOFF5, m_starNet5Logoff);
	m_config->Write(m_name + KEY_STARNET_INFO5, m_starNet5Info);
	m_config->Write(m_name + KEY_STARNET_PERMANENT5, m_starNet5Permanent);
	m_config->Write(m_name + KEY_STARNET_USER_TIMEOUT5, long(m_starNet5UserTimeout));
	m_config->Write(m_name + KEY_STARNET_GROUP_TIMEOUT5, long(m_starNet5GroupTimeout));
	m_config->Write(m_name + KEY_STARNET_CALLSIGN_SWITCH5, long(m_starNet5CallsignSwitch));
	m_config->Write(m_name + KEY_STARNET_TXMSG_SWITCH5, long(m_starNet5TxMsgSwitch));
	m_config->Write(m_name + KEY_STARNET_REFLECTOR5, m_starNet5Reflector);
	m_config->Write(m_name + KEY_REMOTE_ENABLED, m_remoteEnabled);
	m_config->Write(m_name + KEY_REMOTE_PASSWORD, m_remotePassword);
	m_config->Write(m_name + KEY_REMOTE_PORT, long(m_remotePort));
	m_config->Write(m_name + KEY_LANGUAGE, long(m_language));
	m_config->Write(m_name + KEY_INFO_ENABLED, m_infoEnabled);
	m_config->Write(m_name + KEY_ECHO_ENABLED, m_echoEnabled);
	m_config->Write(m_name + KEY_LOG_ENABLED, m_logEnabled);
	m_config->Write(m_name + KEY_DRATS_ENABLED, m_dratsEnabled);
	m_config->Write(m_name + KEY_DTMF_ENABLED, m_dtmfEnabled);
	m_config->Write(m_name + KEY_WINDOW_X, long(m_x));
	m_config->Write(m_name + KEY_WINDOW_Y, long(m_y));
	m_config->Flush();
#endif

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
	buffer.Printf(wxT("%s=%d"), KEY_GATEWAY_TYPE.c_str(), int(m_type)); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_GATEWAY_CALLSIGN.c_str(), m_callsign.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_GATEWAY_ADDRESS.c_str(), m_address.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_ICOM_ADDRESS.c_str(), m_icomAddress.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%u"), KEY_ICOM_PORT.c_str(), m_icomPort); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_HB_ADDRESS.c_str(), m_hbAddress.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%u"), KEY_HB_PORT.c_str(), m_hbPort); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%.6lf"), KEY_LATITUDE.c_str(), m_latitude); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%.6lf"), KEY_LONGITUDE.c_str(), m_longitude); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_DESCRIPTION1.c_str(), m_description1.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_DESCRIPTION2.c_str(), m_description2.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_URL.c_str(), m_url.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_REPEATER_CALL1.c_str(), m_repeater1Callsign.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_REPEATER_BAND1.c_str(), m_repeater1Band.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"), KEY_REPEATER_TYPE1.c_str(), int(m_repeater1Type)); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_REPEATER_ADDRESS1.c_str(), m_repeater1Address.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%u"), KEY_REPEATER_PORT1.c_str(), m_repeater1Port); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_REFLECTOR1.c_str(), m_repeater1Reflector.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"), KEY_ATSTARTUP1.c_str(), m_repeater1AtStartup ? 1 : 0); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"), KEY_RECONNECT1.c_str(), int(m_repeater1Reconnect)); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%.5lf"), KEY_FREQUENCY1.c_str(), m_repeater1Frequency); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%.4lf"), KEY_OFFSET1.c_str(), m_repeater1Offset); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%.3lf"), KEY_RANGE1.c_str(), m_repeater1Range); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%.6lf"), KEY_LATITUDE1.c_str(), m_repeater1Latitude); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%.6lf"), KEY_LONGITUDE1.c_str(), m_repeater1Longitude); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%.3lf"), KEY_AGL1.c_str(), m_repeater1Agl); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_DESCRIPTION11.c_str(), m_repeater1Description1.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_DESCRIPTION12.c_str(), m_repeater1Description2.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_URL1.c_str(), m_repeater1URL.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%u"), KEY_BAND11.c_str(), m_repeater1Band1); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%u"), KEY_BAND12.c_str(), m_repeater1Band2); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%u"), KEY_BAND13.c_str(), m_repeater1Band3); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_REPEATER_CALL2.c_str(), m_repeater2Callsign.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_REPEATER_BAND2.c_str(), m_repeater2Band.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"), KEY_REPEATER_TYPE2.c_str(), int(m_repeater2Type)); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_REPEATER_ADDRESS2.c_str(), m_repeater2Address.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%u"), KEY_REPEATER_PORT2.c_str(), m_repeater2Port); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_REFLECTOR2.c_str(), m_repeater2Reflector.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"), KEY_ATSTARTUP2.c_str(), m_repeater2AtStartup ? 1 : 0); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"), KEY_RECONNECT2.c_str(), int(m_repeater2Reconnect)); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%.5lf"), KEY_FREQUENCY2.c_str(), m_repeater2Frequency); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%.4lf"), KEY_OFFSET2.c_str(), m_repeater2Offset); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%.3lf"), KEY_RANGE2.c_str(), m_repeater2Range); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%.6lf"), KEY_LATITUDE2.c_str(), m_repeater2Latitude); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%.6lf"), KEY_LONGITUDE2.c_str(), m_repeater2Longitude); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%.3lf"), KEY_AGL2.c_str(), m_repeater2Agl); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_DESCRIPTION21.c_str(), m_repeater2Description1.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_DESCRIPTION22.c_str(), m_repeater2Description2.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_URL2.c_str(), m_repeater2URL.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%u"), KEY_BAND21.c_str(), m_repeater2Band1); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%u"), KEY_BAND22.c_str(), m_repeater2Band2); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%u"), KEY_BAND23.c_str(), m_repeater2Band3); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_REPEATER_CALL3.c_str(), m_repeater3Callsign.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_REPEATER_BAND3.c_str(), m_repeater3Band.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"), KEY_REPEATER_TYPE3.c_str(), int(m_repeater3Type)); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_REPEATER_ADDRESS3.c_str(), m_repeater3Address.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%u"), KEY_REPEATER_PORT3.c_str(), m_repeater3Port); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_REFLECTOR3.c_str(), m_repeater3Reflector.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"), KEY_ATSTARTUP3.c_str(), m_repeater3AtStartup ? 1 : 0); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"), KEY_RECONNECT3.c_str(), int(m_repeater3Reconnect)); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%.5lf"), KEY_FREQUENCY3.c_str(), m_repeater3Frequency); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%.4lf"), KEY_OFFSET3.c_str(), m_repeater3Offset); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%.3lf"), KEY_RANGE3.c_str(), m_repeater3Range); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%.6lf"), KEY_LATITUDE3.c_str(), m_repeater3Latitude); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%.6lf"), KEY_LONGITUDE3.c_str(), m_repeater3Longitude); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%.3lf"), KEY_AGL3.c_str(), m_repeater3Agl); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_DESCRIPTION31.c_str(), m_repeater3Description1.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_DESCRIPTION32.c_str(), m_repeater3Description2.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_URL3.c_str(), m_repeater3URL.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%u"), KEY_BAND31.c_str(), m_repeater3Band1); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%u"), KEY_BAND32.c_str(), m_repeater3Band2); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%u"), KEY_BAND33.c_str(), m_repeater3Band3); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_REPEATER_CALL4.c_str(), m_repeater4Callsign.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_REPEATER_BAND4.c_str(), m_repeater4Band.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"), KEY_REPEATER_TYPE4.c_str(), int(m_repeater4Type)); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_REPEATER_ADDRESS4.c_str(), m_repeater4Address.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%u"), KEY_REPEATER_PORT4.c_str(), m_repeater4Port); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_REFLECTOR4.c_str(), m_repeater4Reflector.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"), KEY_ATSTARTUP4.c_str(), m_repeater4AtStartup ? 1 : 0); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"), KEY_RECONNECT4.c_str(), int(m_repeater4Reconnect)); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%.5lf"), KEY_FREQUENCY4.c_str(), m_repeater4Frequency); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%.4lf"), KEY_OFFSET4.c_str(), m_repeater4Offset); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%.3lf"), KEY_RANGE4.c_str(), m_repeater4Range); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%.6lf"), KEY_LATITUDE4.c_str(), m_repeater4Latitude); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%.6lf"), KEY_LONGITUDE4.c_str(), m_repeater4Longitude); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%.3lf"), KEY_AGL4.c_str(), m_repeater4Agl); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_DESCRIPTION41.c_str(), m_repeater4Description1.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_DESCRIPTION42.c_str(), m_repeater4Description2.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_URL4.c_str(), m_repeater4URL.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%u"), KEY_BAND41.c_str(), m_repeater4Band1); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%u"), KEY_BAND42.c_str(), m_repeater4Band2); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%u"), KEY_BAND43.c_str(), m_repeater4Band3); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"), KEY_IRCDDB_ENABLED.c_str(), m_ircddbEnabled ? 1 : 0); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_IRCDDB_HOSTNAME.c_str(), m_ircddbHostname.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_IRCDDB_USERNAME.c_str(), m_ircddbUsername.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_IRCDDB_PASSWORD.c_str(), m_ircddbPassword.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"), KEY_APRS_ENABLED.c_str(), m_aprsEnabled ? 1 : 0); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_APRS_HOSTNAME.c_str(), m_aprsHostname.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%u"), KEY_APRS_PORT.c_str(), m_aprsPort); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"), KEY_DEXTRA_ENABLED.c_str(), m_dextraEnabled ? 1 : 0); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%u"), KEY_DEXTRA_MAXDONGLES.c_str(), m_dextraMaxDongles); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"), KEY_DPLUS_ENABLED.c_str(), m_dplusEnabled ? 1 : 0); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%u"), KEY_DPLUS_MAXDONGLES.c_str(), m_dplusMaxDongles); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_DPLUS_LOGIN.c_str(), m_dplusLogin.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"), KEY_DCS_ENABLED.c_str(), m_dcsEnabled ? 1 : 0); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"), KEY_CCS_ENABLED.c_str(), m_ccsEnabled ? 1 : 0); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_CCS_HOST.c_str(), m_ccsHost.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_STARNET_BAND1.c_str(), m_starNet1Band.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_STARNET_CALLSIGN1.c_str(), m_starNet1Callsign.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_STARNET_LOGOFF1.c_str(), m_starNet1Logoff.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_STARNET_INFO1.c_str(), m_starNet1Info.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_STARNET_PERMANENT1.c_str(), m_starNet1Permanent.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%u"), KEY_STARNET_USER_TIMEOUT1.c_str(), m_starNet1UserTimeout); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%u"), KEY_STARNET_GROUP_TIMEOUT1.c_str(), m_starNet1GroupTimeout); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"), KEY_STARNET_CALLSIGN_SWITCH1.c_str(), int(m_starNet1CallsignSwitch)); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"), KEY_STARNET_TXMSG_SWITCH1.c_str(), m_starNet1TxMsgSwitch ? 1 : 0); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_STARNET_REFLECTOR1.c_str(), m_starNet1Reflector.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_STARNET_BAND2.c_str(), m_starNet2Band.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_STARNET_CALLSIGN2.c_str(), m_starNet2Callsign.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_STARNET_LOGOFF2.c_str(), m_starNet2Logoff.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_STARNET_INFO2.c_str(), m_starNet2Info.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_STARNET_PERMANENT2.c_str(), m_starNet2Permanent.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%u"), KEY_STARNET_USER_TIMEOUT2.c_str(), m_starNet2UserTimeout); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%u"), KEY_STARNET_GROUP_TIMEOUT2.c_str(), m_starNet2GroupTimeout); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"), KEY_STARNET_CALLSIGN_SWITCH2.c_str(), int(m_starNet2CallsignSwitch)); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"), KEY_STARNET_TXMSG_SWITCH2.c_str(), m_starNet2TxMsgSwitch ? 1 : 0); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_STARNET_REFLECTOR2.c_str(), m_starNet2Reflector.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_STARNET_BAND3.c_str(), m_starNet3Band.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_STARNET_CALLSIGN3.c_str(), m_starNet3Callsign.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_STARNET_LOGOFF3.c_str(), m_starNet3Logoff.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_STARNET_INFO3.c_str(), m_starNet3Info.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_STARNET_PERMANENT3.c_str(), m_starNet3Permanent.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%u"), KEY_STARNET_USER_TIMEOUT3.c_str(), m_starNet3UserTimeout); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%u"), KEY_STARNET_GROUP_TIMEOUT3.c_str(), m_starNet3GroupTimeout); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"), KEY_STARNET_CALLSIGN_SWITCH3.c_str(), int(m_starNet3CallsignSwitch)); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"), KEY_STARNET_TXMSG_SWITCH3.c_str(), m_starNet3TxMsgSwitch ? 1 : 0); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_STARNET_REFLECTOR3.c_str(), m_starNet3Reflector.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_STARNET_BAND4.c_str(), m_starNet4Band.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_STARNET_CALLSIGN4.c_str(), m_starNet4Callsign.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_STARNET_LOGOFF4.c_str(), m_starNet4Logoff.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_STARNET_INFO4.c_str(), m_starNet4Info.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_STARNET_PERMANENT4.c_str(), m_starNet4Permanent.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%u"), KEY_STARNET_USER_TIMEOUT4.c_str(), m_starNet4UserTimeout); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%u"), KEY_STARNET_GROUP_TIMEOUT4.c_str(), m_starNet4GroupTimeout); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"), KEY_STARNET_CALLSIGN_SWITCH4.c_str(), int(m_starNet4CallsignSwitch)); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"), KEY_STARNET_TXMSG_SWITCH4.c_str(), m_starNet4TxMsgSwitch ? 1 : 0); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_STARNET_REFLECTOR4.c_str(), m_starNet4Reflector.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_STARNET_BAND5.c_str(), m_starNet5Band.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_STARNET_CALLSIGN5.c_str(), m_starNet5Callsign.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_STARNET_LOGOFF5.c_str(), m_starNet5Logoff.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_STARNET_INFO5.c_str(), m_starNet5Info.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_STARNET_PERMANENT5.c_str(), m_starNet5Permanent.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%u"), KEY_STARNET_USER_TIMEOUT5.c_str(), m_starNet5UserTimeout); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%u"), KEY_STARNET_GROUP_TIMEOUT5.c_str(), m_starNet5GroupTimeout); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"), KEY_STARNET_CALLSIGN_SWITCH5.c_str(), int(m_starNet5CallsignSwitch)); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"), KEY_STARNET_TXMSG_SWITCH5.c_str(), m_starNet5TxMsgSwitch ? 1 : 0); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_STARNET_REFLECTOR5.c_str(), m_starNet5Reflector.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"), KEY_REMOTE_ENABLED.c_str(), m_remoteEnabled ? 1 : 0); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_REMOTE_PASSWORD.c_str(), m_remotePassword.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%u"), KEY_REMOTE_PORT.c_str(), m_remotePort); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"), KEY_LANGUAGE.c_str(), int(m_language)); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"), KEY_INFO_ENABLED.c_str(), m_infoEnabled ? 1 : 0); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"), KEY_ECHO_ENABLED.c_str(), m_echoEnabled ? 1 : 0); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"), KEY_LOG_ENABLED.c_str(), m_logEnabled ? 1 : 0); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"), KEY_DRATS_ENABLED.c_str(), m_dratsEnabled ? 1 : 0); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"), KEY_DTMF_ENABLED.c_str(), m_dtmfEnabled ? 1 : 0); file.AddLine(buffer);
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
