/*
 *   Copyright (C) 2010,2011,2012 by Jonathan Naylor G4KLX
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

#include "StarNetServerConfig.h"
#include "StarNetServerDefs.h"

#include <wx/textfile.h>

const wxString  KEY_CALLSIGN                = wxT("callsign");
const wxString  KEY_ADDRESS                 = wxT("address");
const wxString  KEY_IRCDDB_HOSTNAME         = wxT("ircddbHostname");
const wxString  KEY_IRCDDB_USERNAME         = wxT("ircddbUsername");
const wxString  KEY_IRCDDB_PASSWORD         = wxT("ircddbPassword");
const wxString  KEY_STARNET_BAND1           = wxT("starNetBand1");
const wxString  KEY_STARNET_CALLSIGN1       = wxT("starNetCallsign1");
const wxString  KEY_STARNET_LOGOFF1         = wxT("starNetLogoff1");
const wxString  KEY_STARNET_INFO1           = wxT("starNetInfo1");
const wxString  KEY_STARNET_PERMANENT1      = wxT("starNetPermanent1");
const wxString  KEY_STARNET_USER_TIMEOUT1   = wxT("starNetUserTimeout1");
const wxString  KEY_STARNET_GROUP_TIMEOUT1  = wxT("starNetGroupTimeout1");
const wxString  KEY_STARNET_CALLSIGN_SWITCH1 = wxT("starNetCallsignSwitch1");
const wxString  KEY_STARNET_TXMSG_SWITCH1   = wxT("starNetTXMsgSwitch1");
const wxString  KEY_STARNET_REFLECTOR1      = wxT("starNetReflector1");		// DEXTRA_LINK
const wxString  KEY_STARNET_BAND2           = wxT("starNetBand2");
const wxString  KEY_STARNET_CALLSIGN2       = wxT("starNetCallsign2");
const wxString  KEY_STARNET_LOGOFF2         = wxT("starNetLogoff2");
const wxString  KEY_STARNET_INFO2           = wxT("starNetInfo2");
const wxString  KEY_STARNET_PERMANENT2      = wxT("starNetPermanent2");
const wxString  KEY_STARNET_USER_TIMEOUT2   = wxT("starNetUserTimeout2");
const wxString  KEY_STARNET_GROUP_TIMEOUT2  = wxT("starNetGroupTimeout2");
const wxString  KEY_STARNET_CALLSIGN_SWITCH2 = wxT("starNetCallsignSwitch2");
const wxString  KEY_STARNET_TXMSG_SWITCH2   = wxT("starNetTXMsgSwitch2");
const wxString  KEY_STARNET_REFLECTOR2      = wxT("starNetReflector2");		// DEXTRA_LINK
const wxString  KEY_STARNET_BAND3           = wxT("starNetBand3");
const wxString  KEY_STARNET_CALLSIGN3       = wxT("starNetCallsign3");
const wxString  KEY_STARNET_LOGOFF3         = wxT("starNetLogoff3");
const wxString  KEY_STARNET_INFO3           = wxT("starNetInfo3");
const wxString  KEY_STARNET_PERMANENT3      = wxT("starNetPermanent3");
const wxString  KEY_STARNET_USER_TIMEOUT3   = wxT("starNetUserTimeout3");
const wxString  KEY_STARNET_GROUP_TIMEOUT3  = wxT("starNetGroupTimeout3");
const wxString  KEY_STARNET_CALLSIGN_SWITCH3 = wxT("starNetCallsignSwitch3");
const wxString  KEY_STARNET_TXMSG_SWITCH3   = wxT("starNetTXMsgSwitch3");
const wxString  KEY_STARNET_REFLECTOR3      = wxT("starNetReflector3");		// DEXTRA_LINK
const wxString  KEY_STARNET_BAND4           = wxT("starNetBand4");
const wxString  KEY_STARNET_CALLSIGN4       = wxT("starNetCallsign4");
const wxString  KEY_STARNET_LOGOFF4         = wxT("starNetLogoff4");
const wxString  KEY_STARNET_INFO4           = wxT("starNetInfo4");
const wxString  KEY_STARNET_PERMANENT4      = wxT("starNetPermanent4");
const wxString  KEY_STARNET_USER_TIMEOUT4   = wxT("starNetUserTimeout4");
const wxString  KEY_STARNET_GROUP_TIMEOUT4  = wxT("starNetGroupTimeout4");
const wxString  KEY_STARNET_CALLSIGN_SWITCH4 = wxT("starNetCallsignSwitch4");
const wxString  KEY_STARNET_TXMSG_SWITCH4   = wxT("starNetTXMsgSwitch4");
const wxString  KEY_STARNET_REFLECTOR4      = wxT("starNetReflector4");		// DEXTRA_LINK
const wxString  KEY_STARNET_BAND5           = wxT("starNetBand5");
const wxString  KEY_STARNET_CALLSIGN5       = wxT("starNetCallsign5");
const wxString  KEY_STARNET_LOGOFF5         = wxT("starNetLogoff5");
const wxString  KEY_STARNET_INFO5           = wxT("starNetInfo5");
const wxString  KEY_STARNET_PERMANENT5      = wxT("starNetPermanent5");
const wxString  KEY_STARNET_USER_TIMEOUT5   = wxT("starNetUserTimeout5");
const wxString  KEY_STARNET_GROUP_TIMEOUT5  = wxT("starNetGroupTimeout5");
const wxString  KEY_STARNET_CALLSIGN_SWITCH5 = wxT("starNetCallsignSwitch5");
const wxString  KEY_STARNET_TXMSG_SWITCH5   = wxT("starNetTXMsgSwitch5");
const wxString  KEY_STARNET_REFLECTOR5      = wxT("starNetReflector5");		// DEXTRA_LINK
const wxString  KEY_STARNET_BAND6           = wxT("starNetBand6");
const wxString  KEY_STARNET_CALLSIGN6       = wxT("starNetCallsign6");
const wxString  KEY_STARNET_LOGOFF6         = wxT("starNetLogoff6");
const wxString  KEY_STARNET_INFO6           = wxT("starNetInfo6");
const wxString  KEY_STARNET_PERMANENT6      = wxT("starNetPermanent6");
const wxString  KEY_STARNET_USER_TIMEOUT6   = wxT("starNetUserTimeout6");
const wxString  KEY_STARNET_GROUP_TIMEOUT6  = wxT("starNetGroupTimeout6");
const wxString  KEY_STARNET_CALLSIGN_SWITCH6 = wxT("starNetCallsignSwitch6");
const wxString  KEY_STARNET_TXMSG_SWITCH6   = wxT("starNetTXMsgSwitch6");
const wxString  KEY_STARNET_REFLECTOR6      = wxT("starNetReflector6");		// DEXTRA_LINK
const wxString  KEY_STARNET_BAND7           = wxT("starNetBand7");
const wxString  KEY_STARNET_CALLSIGN7       = wxT("starNetCallsign7");
const wxString  KEY_STARNET_LOGOFF7         = wxT("starNetLogoff7");
const wxString  KEY_STARNET_INFO7           = wxT("starNetInfo7");
const wxString  KEY_STARNET_PERMANENT7      = wxT("starNetPermanent7");
const wxString  KEY_STARNET_USER_TIMEOUT7   = wxT("starNetUserTimeout7");
const wxString  KEY_STARNET_GROUP_TIMEOUT7  = wxT("starNetGroupTimeout7");
const wxString  KEY_STARNET_CALLSIGN_SWITCH7 = wxT("starNetCallsignSwitch7");
const wxString  KEY_STARNET_TXMSG_SWITCH7   = wxT("starNetTXMsgSwitch7");
const wxString  KEY_STARNET_REFLECTOR7      = wxT("starNetReflector7");		// DEXTRA_LINK
const wxString  KEY_STARNET_BAND8           = wxT("starNetBand8");
const wxString  KEY_STARNET_CALLSIGN8       = wxT("starNetCallsign8");
const wxString  KEY_STARNET_LOGOFF8         = wxT("starNetLogoff8");
const wxString  KEY_STARNET_INFO8           = wxT("starNetInfo8");
const wxString  KEY_STARNET_PERMANENT8      = wxT("starNetPermanent8");
const wxString  KEY_STARNET_USER_TIMEOUT8   = wxT("starNetUserTimeout8");
const wxString  KEY_STARNET_GROUP_TIMEOUT8  = wxT("starNetGroupTimeout8");
const wxString  KEY_STARNET_CALLSIGN_SWITCH8 = wxT("starNetCallsignSwitch8");
const wxString  KEY_STARNET_TXMSG_SWITCH8   = wxT("starNetTXMsgSwitch8");
const wxString  KEY_STARNET_REFLECTOR8      = wxT("starNetReflector8");		// DEXTRA_LINK
const wxString  KEY_STARNET_BAND9           = wxT("starNetBand9");
const wxString  KEY_STARNET_CALLSIGN9       = wxT("starNetCallsign9");
const wxString  KEY_STARNET_LOGOFF9         = wxT("starNetLogoff9");
const wxString  KEY_STARNET_INFO9           = wxT("starNetInfo9");
const wxString  KEY_STARNET_PERMANENT9      = wxT("starNetPermanent9");
const wxString  KEY_STARNET_USER_TIMEOUT9   = wxT("starNetUserTimeout9");
const wxString  KEY_STARNET_GROUP_TIMEOUT9  = wxT("starNetGroupTimeout9");
const wxString  KEY_STARNET_CALLSIGN_SWITCH9 = wxT("starNetCallsignSwitch9");
const wxString  KEY_STARNET_TXMSG_SWITCH9   = wxT("starNetTXMsgSwitch9");
const wxString  KEY_STARNET_REFLECTOR9      = wxT("starNetReflector9");		// DEXTRA_LINK
const wxString  KEY_STARNET_BAND10          = wxT("starNetBand10");
const wxString  KEY_STARNET_CALLSIGN10      = wxT("starNetCallsign10");
const wxString  KEY_STARNET_LOGOFF10        = wxT("starNetLogoff10");
const wxString  KEY_STARNET_INFO10          = wxT("starNetInfo10");
const wxString  KEY_STARNET_PERMANENT10     = wxT("starNetPermanent10");
const wxString  KEY_STARNET_USER_TIMEOUT10  = wxT("starNetUserTimeout10");
const wxString  KEY_STARNET_GROUP_TIMEOUT10 = wxT("starNetGroupTimeout10");
const wxString  KEY_STARNET_CALLSIGN_SWITCH10 = wxT("starNetCallsignSwitch10");
const wxString  KEY_STARNET_TXMSG_SWITCH10  = wxT("starNetTXMsgSwitch10");
const wxString  KEY_STARNET_REFLECTOR10     = wxT("starNetReflector10");	// DEXTRA_LINK
const wxString  KEY_STARNET_BAND11          = wxT("starNetBand11");
const wxString  KEY_STARNET_CALLSIGN11      = wxT("starNetCallsign11");
const wxString  KEY_STARNET_LOGOFF11        = wxT("starNetLogoff11");
const wxString  KEY_STARNET_INFO11          = wxT("starNetInfo11");
const wxString  KEY_STARNET_PERMANENT11     = wxT("starNetPermanent11");
const wxString  KEY_STARNET_USER_TIMEOUT11  = wxT("starNetUserTimeout11");
const wxString  KEY_STARNET_GROUP_TIMEOUT11 = wxT("starNetGroupTimeout11");
const wxString  KEY_STARNET_CALLSIGN_SWITCH11 = wxT("starNetCallsignSwitch11");
const wxString  KEY_STARNET_TXMSG_SWITCH11  = wxT("starNetTXMsgSwitch11");
const wxString  KEY_STARNET_REFLECTOR11     = wxT("starNetReflector11");		// DEXTRA_LINK
const wxString  KEY_STARNET_BAND12          = wxT("starNetBand12");
const wxString  KEY_STARNET_CALLSIGN12      = wxT("starNetCallsign12");
const wxString  KEY_STARNET_LOGOFF12        = wxT("starNetLogoff12");
const wxString  KEY_STARNET_INFO12          = wxT("starNetInfo12");
const wxString  KEY_STARNET_PERMANENT12     = wxT("starNetPermanent12");
const wxString  KEY_STARNET_USER_TIMEOUT12  = wxT("starNetUserTimeout12");
const wxString  KEY_STARNET_GROUP_TIMEOUT12 = wxT("starNetGroupTimeout12");
const wxString  KEY_STARNET_CALLSIGN_SWITCH12 = wxT("starNetCallsignSwitch12");
const wxString  KEY_STARNET_TXMSG_SWITCH12  = wxT("starNetTXMsgSwitch12");
const wxString  KEY_STARNET_REFLECTOR12     = wxT("starNetReflector12");		// DEXTRA_LINK
const wxString  KEY_STARNET_BAND13          = wxT("starNetBand13");
const wxString  KEY_STARNET_CALLSIGN13      = wxT("starNetCallsign13");
const wxString  KEY_STARNET_LOGOFF13        = wxT("starNetLogoff13");
const wxString  KEY_STARNET_INFO13          = wxT("starNetInfo13");
const wxString  KEY_STARNET_PERMANENT13     = wxT("starNetPermanent13");
const wxString  KEY_STARNET_USER_TIMEOUT13  = wxT("starNetUserTimeout13");
const wxString  KEY_STARNET_GROUP_TIMEOUT13 = wxT("starNetGroupTimeout13");
const wxString  KEY_STARNET_CALLSIGN_SWITCH13 = wxT("starNetCallsignSwitch13");
const wxString  KEY_STARNET_TXMSG_SWITCH13  = wxT("starNetTXMsgSwitch13");
const wxString  KEY_STARNET_REFLECTOR13     = wxT("starNetReflector13");		// DEXTRA_LINK
const wxString  KEY_STARNET_BAND14          = wxT("starNetBand14");
const wxString  KEY_STARNET_CALLSIGN14      = wxT("starNetCallsign14");
const wxString  KEY_STARNET_LOGOFF14        = wxT("starNetLogoff14");
const wxString  KEY_STARNET_INFO14          = wxT("starNetInfo14");
const wxString  KEY_STARNET_PERMANENT14     = wxT("starNetPermanent14");
const wxString  KEY_STARNET_USER_TIMEOUT14  = wxT("starNetUserTimeout14");
const wxString  KEY_STARNET_GROUP_TIMEOUT14 = wxT("starNetGroupTimeout14");
const wxString  KEY_STARNET_CALLSIGN_SWITCH14 = wxT("starNetCallsignSwitch14");
const wxString  KEY_STARNET_TXMSG_SWITCH14  = wxT("starNetTXMsgSwitch14");
const wxString  KEY_STARNET_REFLECTOR14     = wxT("starNetReflector14");		// DEXTRA_LINK
const wxString  KEY_STARNET_BAND15          = wxT("starNetBand15");
const wxString  KEY_STARNET_CALLSIGN15      = wxT("starNetCallsign15");
const wxString  KEY_STARNET_LOGOFF15        = wxT("starNetLogoff15");
const wxString  KEY_STARNET_INFO15          = wxT("starNetInfo15");
const wxString  KEY_STARNET_PERMANENT15     = wxT("starNetPermanent15");
const wxString  KEY_STARNET_USER_TIMEOUT15  = wxT("starNetUserTimeout15");
const wxString  KEY_STARNET_GROUP_TIMEOUT15 = wxT("starNetGroupTimeout15");
const wxString  KEY_STARNET_CALLSIGN_SWITCH15 = wxT("starNetCallsignSwitch15");
const wxString  KEY_STARNET_TXMSG_SWITCH15  = wxT("starNetTXMsgSwitch15");
const wxString  KEY_STARNET_REFLECTOR15     = wxT("starNetReflector15");		// DEXTRA_LINK
const wxString  KEY_REMOTE_ENABLED          = wxT("remoteEnabled");
const wxString  KEY_REMOTE_PASSWORD         = wxT("remotePassword");
const wxString  KEY_REMOTE_PORT             = wxT("remotePort");
const wxString  KEY_LOG_ENABLED             = wxT("logEnabled");
const wxString  KEY_WINDOW_X                = wxT("windowX");
const wxString  KEY_WINDOW_Y                = wxT("windowY");

const wxString     DEFAULT_CALLSIGN                = wxEmptyString;
const wxString     DEFAULT_ADDRESS                 = wxEmptyString;
const wxString     DEFAULT_IRCDDB_HOSTNAME         = wxT("group1-irc.ircddb.net");
const wxString     DEFAULT_IRCDDB_USERNAME         = wxEmptyString;
const wxString     DEFAULT_IRCDDB_PASSWORD         = wxEmptyString;
const wxString     DEFAULT_STARNET_BAND            = wxEmptyString;
const wxString     DEFAULT_STARNET_CALLSIGN        = wxEmptyString;
const wxString     DEFAULT_STARNET_LOGOFF          = wxEmptyString;
const wxString     DEFAULT_STARNET_INFO            = wxEmptyString;
const wxString     DEFAULT_STARNET_PERMANENT       = wxEmptyString;
const unsigned int DEFAULT_STARNET_USER_TIMEOUT    = 300U;
const unsigned int DEFAULT_STARNET_GROUP_TIMEOUT   = 300U;
const STARNET_CALLSIGN_SWITCH DEFAULT_STARNET_CALLSIGN_SWITCH = SCS_GROUP_CALLSIGN;
const bool         DEFAULT_STARNET_TXMSG_SWITCH    = true;
const wxString     DEFAULT_STARNET_REFLECTOR       = wxEmptyString;
const bool         DEFAULT_REMOTE_ENABLED          = false;
const wxString     DEFAULT_REMOTE_PASSWORD         = wxEmptyString;
const unsigned int DEFAULT_REMOTE_PORT             = 0U;
const bool         DEFAULT_LOG_ENABLED             = false;
const int          DEFAULT_WINDOW_X                = -1;
const int          DEFAULT_WINDOW_Y                = -1;

#if defined(__WINDOWS__)

CStarNetServerConfig::CStarNetServerConfig(wxConfigBase* config, const wxString& dir) :
m_config(config),
m_fileName(),
m_callsign(DEFAULT_CALLSIGN),
m_address(DEFAULT_ADDRESS),
m_ircddbHostname(DEFAULT_IRCDDB_HOSTNAME),
m_ircddbUsername(DEFAULT_IRCDDB_USERNAME),
m_ircddbPassword(DEFAULT_IRCDDB_PASSWORD),
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
m_starNet6Band(DEFAULT_STARNET_BAND),
m_starNet6Callsign(DEFAULT_STARNET_CALLSIGN),
m_starNet6Logoff(DEFAULT_STARNET_LOGOFF),
m_starNet6Info(DEFAULT_STARNET_INFO),
m_starNet6Permanent(DEFAULT_STARNET_PERMANENT),
m_starNet6UserTimeout(DEFAULT_STARNET_USER_TIMEOUT),
m_starNet6GroupTimeout(DEFAULT_STARNET_GROUP_TIMEOUT),
m_starNet6CallsignSwitch(DEFAULT_STARNET_CALLSIGN_SWITCH),
m_starNet6TxMsgSwitch(DEFAULT_STARNET_TXMSG_SWITCH),
m_starNet6Reflector(DEFAULT_STARNET_REFLECTOR),
m_starNet7Band(DEFAULT_STARNET_BAND),
m_starNet7Callsign(DEFAULT_STARNET_CALLSIGN),
m_starNet7Logoff(DEFAULT_STARNET_LOGOFF),
m_starNet7Info(DEFAULT_STARNET_INFO),
m_starNet7Permanent(DEFAULT_STARNET_PERMANENT),
m_starNet7UserTimeout(DEFAULT_STARNET_USER_TIMEOUT),
m_starNet7GroupTimeout(DEFAULT_STARNET_GROUP_TIMEOUT),
m_starNet7CallsignSwitch(DEFAULT_STARNET_CALLSIGN_SWITCH),
m_starNet7TxMsgSwitch(DEFAULT_STARNET_TXMSG_SWITCH),
m_starNet7Reflector(DEFAULT_STARNET_REFLECTOR),
m_starNet8Band(DEFAULT_STARNET_BAND),
m_starNet8Callsign(DEFAULT_STARNET_CALLSIGN),
m_starNet8Logoff(DEFAULT_STARNET_LOGOFF),
m_starNet8Info(DEFAULT_STARNET_INFO),
m_starNet8Permanent(DEFAULT_STARNET_PERMANENT),
m_starNet8UserTimeout(DEFAULT_STARNET_USER_TIMEOUT),
m_starNet8GroupTimeout(DEFAULT_STARNET_GROUP_TIMEOUT),
m_starNet8CallsignSwitch(DEFAULT_STARNET_CALLSIGN_SWITCH),
m_starNet8TxMsgSwitch(DEFAULT_STARNET_TXMSG_SWITCH),
m_starNet8Reflector(DEFAULT_STARNET_REFLECTOR),
m_starNet9Band(DEFAULT_STARNET_BAND),
m_starNet9Callsign(DEFAULT_STARNET_CALLSIGN),
m_starNet9Logoff(DEFAULT_STARNET_LOGOFF),
m_starNet9Info(DEFAULT_STARNET_INFO),
m_starNet9Permanent(DEFAULT_STARNET_PERMANENT),
m_starNet9UserTimeout(DEFAULT_STARNET_USER_TIMEOUT),
m_starNet9GroupTimeout(DEFAULT_STARNET_GROUP_TIMEOUT),
m_starNet9CallsignSwitch(DEFAULT_STARNET_CALLSIGN_SWITCH),
m_starNet9TxMsgSwitch(DEFAULT_STARNET_TXMSG_SWITCH),
m_starNet9Reflector(DEFAULT_STARNET_REFLECTOR),
m_starNet10Band(DEFAULT_STARNET_BAND),
m_starNet10Callsign(DEFAULT_STARNET_CALLSIGN),
m_starNet10Logoff(DEFAULT_STARNET_LOGOFF),
m_starNet10Info(DEFAULT_STARNET_INFO),
m_starNet10Permanent(DEFAULT_STARNET_PERMANENT),
m_starNet10UserTimeout(DEFAULT_STARNET_USER_TIMEOUT),
m_starNet10GroupTimeout(DEFAULT_STARNET_GROUP_TIMEOUT),
m_starNet10CallsignSwitch(DEFAULT_STARNET_CALLSIGN_SWITCH),
m_starNet10TxMsgSwitch(DEFAULT_STARNET_TXMSG_SWITCH),
m_starNet10Reflector(DEFAULT_STARNET_REFLECTOR),
m_starNet11Band(DEFAULT_STARNET_BAND),
m_starNet11Callsign(DEFAULT_STARNET_CALLSIGN),
m_starNet11Logoff(DEFAULT_STARNET_LOGOFF),
m_starNet11Info(DEFAULT_STARNET_INFO),
m_starNet11Permanent(DEFAULT_STARNET_PERMANENT),
m_starNet11UserTimeout(DEFAULT_STARNET_USER_TIMEOUT),
m_starNet11GroupTimeout(DEFAULT_STARNET_GROUP_TIMEOUT),
m_starNet11CallsignSwitch(DEFAULT_STARNET_CALLSIGN_SWITCH),
m_starNet11TxMsgSwitch(DEFAULT_STARNET_TXMSG_SWITCH),
m_starNet11Reflector(DEFAULT_STARNET_REFLECTOR),
m_starNet12Band(DEFAULT_STARNET_BAND),
m_starNet12Callsign(DEFAULT_STARNET_CALLSIGN),
m_starNet12Logoff(DEFAULT_STARNET_LOGOFF),
m_starNet12Info(DEFAULT_STARNET_INFO),
m_starNet12Permanent(DEFAULT_STARNET_PERMANENT),
m_starNet12UserTimeout(DEFAULT_STARNET_USER_TIMEOUT),
m_starNet12GroupTimeout(DEFAULT_STARNET_GROUP_TIMEOUT),
m_starNet12CallsignSwitch(DEFAULT_STARNET_CALLSIGN_SWITCH),
m_starNet12TxMsgSwitch(DEFAULT_STARNET_TXMSG_SWITCH),
m_starNet12Reflector(DEFAULT_STARNET_REFLECTOR),
m_starNet13Band(DEFAULT_STARNET_BAND),
m_starNet13Callsign(DEFAULT_STARNET_CALLSIGN),
m_starNet13Logoff(DEFAULT_STARNET_LOGOFF),
m_starNet13Info(DEFAULT_STARNET_INFO),
m_starNet13Permanent(DEFAULT_STARNET_PERMANENT),
m_starNet13UserTimeout(DEFAULT_STARNET_USER_TIMEOUT),
m_starNet13GroupTimeout(DEFAULT_STARNET_GROUP_TIMEOUT),
m_starNet13CallsignSwitch(DEFAULT_STARNET_CALLSIGN_SWITCH),
m_starNet13TxMsgSwitch(DEFAULT_STARNET_TXMSG_SWITCH),
m_starNet13Reflector(DEFAULT_STARNET_REFLECTOR),
m_starNet14Band(DEFAULT_STARNET_BAND),
m_starNet14Callsign(DEFAULT_STARNET_CALLSIGN),
m_starNet14Logoff(DEFAULT_STARNET_LOGOFF),
m_starNet14Info(DEFAULT_STARNET_INFO),
m_starNet14Permanent(DEFAULT_STARNET_PERMANENT),
m_starNet14UserTimeout(DEFAULT_STARNET_USER_TIMEOUT),
m_starNet14GroupTimeout(DEFAULT_STARNET_GROUP_TIMEOUT),
m_starNet14CallsignSwitch(DEFAULT_STARNET_CALLSIGN_SWITCH),
m_starNet14TxMsgSwitch(DEFAULT_STARNET_TXMSG_SWITCH),
m_starNet14Reflector(DEFAULT_STARNET_REFLECTOR),
m_starNet15Band(DEFAULT_STARNET_BAND),
m_starNet15Callsign(DEFAULT_STARNET_CALLSIGN),
m_starNet15Logoff(DEFAULT_STARNET_LOGOFF),
m_starNet15Info(DEFAULT_STARNET_INFO),
m_starNet15Permanent(DEFAULT_STARNET_PERMANENT),
m_starNet15UserTimeout(DEFAULT_STARNET_USER_TIMEOUT),
m_starNet15GroupTimeout(DEFAULT_STARNET_GROUP_TIMEOUT),
m_starNet15CallsignSwitch(DEFAULT_STARNET_CALLSIGN_SWITCH),
m_starNet15TxMsgSwitch(DEFAULT_STARNET_TXMSG_SWITCH),
m_starNet15Reflector(DEFAULT_STARNET_REFLECTOR),
m_remoteEnabled(DEFAULT_REMOTE_ENABLED),
m_remotePassword(DEFAULT_REMOTE_PASSWORD),
m_remotePort(DEFAULT_REMOTE_PORT),
m_logEnabled(DEFAULT_LOG_ENABLED),
m_x(DEFAULT_WINDOW_X),
m_y(DEFAULT_WINDOW_Y)
{
	wxASSERT(config != NULL);
	wxASSERT(!dir.IsEmpty());

	m_fileName.Assign(dir, CONFIG_FILE_NAME);

	wxTextFile file(m_fileName.GetFullPath());

	long temp;

	m_config->Read(wxT("/") + KEY_CALLSIGN, &m_callsign, DEFAULT_CALLSIGN);

	m_config->Read(wxT("/") + KEY_ADDRESS, &m_address, DEFAULT_ADDRESS);

	m_config->Read(wxT("/") + KEY_IRCDDB_HOSTNAME, &m_ircddbHostname, DEFAULT_IRCDDB_HOSTNAME);

	m_config->Read(wxT("/") + KEY_IRCDDB_USERNAME, &m_ircddbUsername, DEFAULT_IRCDDB_USERNAME);

	m_config->Read(wxT("/") + KEY_IRCDDB_PASSWORD, &m_ircddbPassword, DEFAULT_IRCDDB_PASSWORD);

	m_config->Read(wxT("/") + KEY_STARNET_BAND1, &m_starNet1Band, DEFAULT_STARNET_BAND);

	m_config->Read(wxT("/") + KEY_STARNET_CALLSIGN1, &m_starNet1Callsign, DEFAULT_STARNET_CALLSIGN);

	m_config->Read(wxT("/") + KEY_STARNET_LOGOFF1, &m_starNet1Logoff, DEFAULT_STARNET_LOGOFF);

	m_config->Read(wxT("/") + KEY_STARNET_INFO1, &m_starNet1Info, DEFAULT_STARNET_INFO);

	m_config->Read(wxT("/") + KEY_STARNET_PERMANENT1, &m_starNet1Permanent, DEFAULT_STARNET_PERMANENT);

	m_config->Read(wxT("/") + KEY_STARNET_USER_TIMEOUT1, &temp, long(DEFAULT_STARNET_USER_TIMEOUT));
	m_starNet1UserTimeout = (unsigned int)temp;

	m_config->Read(wxT("/") + KEY_STARNET_GROUP_TIMEOUT1, &temp, long(DEFAULT_STARNET_GROUP_TIMEOUT));
	m_starNet1GroupTimeout = (unsigned int)temp;

	m_config->Read(wxT("/") + KEY_STARNET_CALLSIGN_SWITCH1, &temp, long(DEFAULT_STARNET_CALLSIGN_SWITCH));
	m_starNet1CallsignSwitch = STARNET_CALLSIGN_SWITCH(temp);

	m_config->Read(wxT("/") + KEY_STARNET_TXMSG_SWITCH1, &m_starNet1TxMsgSwitch, DEFAULT_STARNET_TXMSG_SWITCH);

	m_config->Read(wxT("/") + KEY_STARNET_REFLECTOR1, &m_starNet1Reflector, DEFAULT_STARNET_REFLECTOR);

	m_config->Read(wxT("/") + KEY_STARNET_BAND2, &m_starNet2Band, DEFAULT_STARNET_BAND);

	m_config->Read(wxT("/") + KEY_STARNET_CALLSIGN2, &m_starNet2Callsign, DEFAULT_STARNET_CALLSIGN);

	m_config->Read(wxT("/") + KEY_STARNET_LOGOFF2, &m_starNet2Logoff, DEFAULT_STARNET_LOGOFF);

	m_config->Read(wxT("/") + KEY_STARNET_INFO2, &m_starNet2Info, DEFAULT_STARNET_INFO);

	m_config->Read(wxT("/") + KEY_STARNET_PERMANENT2, &m_starNet2Permanent, DEFAULT_STARNET_PERMANENT);

	m_config->Read(wxT("/") + KEY_STARNET_USER_TIMEOUT2, &temp, long(DEFAULT_STARNET_USER_TIMEOUT));
	m_starNet2UserTimeout = (unsigned int)temp;

	m_config->Read(wxT("/") + KEY_STARNET_GROUP_TIMEOUT2, &temp, long(DEFAULT_STARNET_GROUP_TIMEOUT));
	m_starNet2GroupTimeout = (unsigned int)temp;

	m_config->Read(wxT("/") + KEY_STARNET_CALLSIGN_SWITCH2, &temp, long(DEFAULT_STARNET_CALLSIGN_SWITCH));
	m_starNet2CallsignSwitch = STARNET_CALLSIGN_SWITCH(temp);

	m_config->Read(wxT("/") + KEY_STARNET_TXMSG_SWITCH2, &m_starNet2TxMsgSwitch, DEFAULT_STARNET_TXMSG_SWITCH);

	m_config->Read(wxT("/") + KEY_STARNET_REFLECTOR2, &m_starNet2Reflector, DEFAULT_STARNET_REFLECTOR);

	m_config->Read(wxT("/") + KEY_STARNET_BAND3, &m_starNet3Band, DEFAULT_STARNET_BAND);

	m_config->Read(wxT("/") + KEY_STARNET_CALLSIGN3, &m_starNet3Callsign, DEFAULT_STARNET_CALLSIGN);

	m_config->Read(wxT("/") + KEY_STARNET_LOGOFF3, &m_starNet3Logoff, DEFAULT_STARNET_LOGOFF);

	m_config->Read(wxT("/") + KEY_STARNET_INFO3, &m_starNet3Info, DEFAULT_STARNET_INFO);

	m_config->Read(wxT("/") + KEY_STARNET_PERMANENT3, &m_starNet3Permanent, DEFAULT_STARNET_PERMANENT);

	m_config->Read(wxT("/") + KEY_STARNET_USER_TIMEOUT3, &temp, long(DEFAULT_STARNET_USER_TIMEOUT));
	m_starNet3UserTimeout = (unsigned int)temp;

	m_config->Read(wxT("/") + KEY_STARNET_GROUP_TIMEOUT3, &temp, long(DEFAULT_STARNET_GROUP_TIMEOUT));
	m_starNet3GroupTimeout = (unsigned int)temp;

	m_config->Read(wxT("/") + KEY_STARNET_CALLSIGN_SWITCH3, &temp, long(DEFAULT_STARNET_CALLSIGN_SWITCH));
	m_starNet3CallsignSwitch = STARNET_CALLSIGN_SWITCH(temp);

	m_config->Read(wxT("/") + KEY_STARNET_TXMSG_SWITCH3, &m_starNet3TxMsgSwitch, DEFAULT_STARNET_TXMSG_SWITCH);

	m_config->Read(wxT("/") + KEY_STARNET_REFLECTOR3, &m_starNet3Reflector, DEFAULT_STARNET_REFLECTOR);

	m_config->Read(wxT("/") + KEY_STARNET_BAND4, &m_starNet4Band, DEFAULT_STARNET_BAND);

	m_config->Read(wxT("/") + KEY_STARNET_CALLSIGN4, &m_starNet4Callsign, DEFAULT_STARNET_CALLSIGN);

	m_config->Read(wxT("/") + KEY_STARNET_LOGOFF4, &m_starNet4Logoff, DEFAULT_STARNET_LOGOFF);

	m_config->Read(wxT("/") + KEY_STARNET_INFO4, &m_starNet4Info, DEFAULT_STARNET_INFO);

	m_config->Read(wxT("/") + KEY_STARNET_PERMANENT4, &m_starNet4Permanent, DEFAULT_STARNET_PERMANENT);

	m_config->Read(wxT("/") + KEY_STARNET_USER_TIMEOUT4, &temp, long(DEFAULT_STARNET_USER_TIMEOUT));
	m_starNet4UserTimeout = (unsigned int)temp;

	m_config->Read(wxT("/") + KEY_STARNET_GROUP_TIMEOUT4, &temp, long(DEFAULT_STARNET_GROUP_TIMEOUT));
	m_starNet4GroupTimeout = (unsigned int)temp;

	m_config->Read(wxT("/") + KEY_STARNET_CALLSIGN_SWITCH4, &temp, long(DEFAULT_STARNET_CALLSIGN_SWITCH));
	m_starNet4CallsignSwitch = STARNET_CALLSIGN_SWITCH(temp);

	m_config->Read(wxT("/") + KEY_STARNET_TXMSG_SWITCH4, &m_starNet4TxMsgSwitch, DEFAULT_STARNET_TXMSG_SWITCH);

	m_config->Read(wxT("/") + KEY_STARNET_REFLECTOR4, &m_starNet4Reflector, DEFAULT_STARNET_REFLECTOR);

	m_config->Read(wxT("/") + KEY_STARNET_BAND5, &m_starNet5Band, DEFAULT_STARNET_BAND);

	m_config->Read(wxT("/") + KEY_STARNET_CALLSIGN5, &m_starNet5Callsign, DEFAULT_STARNET_CALLSIGN);

	m_config->Read(wxT("/") + KEY_STARNET_LOGOFF5, &m_starNet5Logoff, DEFAULT_STARNET_LOGOFF);

	m_config->Read(wxT("/") + KEY_STARNET_INFO5, &m_starNet5Info, DEFAULT_STARNET_INFO);

	m_config->Read(wxT("/") + KEY_STARNET_PERMANENT5, &m_starNet5Permanent, DEFAULT_STARNET_PERMANENT);

	m_config->Read(wxT("/") + KEY_STARNET_USER_TIMEOUT5, &temp, long(DEFAULT_STARNET_USER_TIMEOUT));
	m_starNet5UserTimeout = (unsigned int)temp;

	m_config->Read(wxT("/") + KEY_STARNET_GROUP_TIMEOUT5, &temp, long(DEFAULT_STARNET_GROUP_TIMEOUT));
	m_starNet5GroupTimeout = (unsigned int)temp;

	m_config->Read(wxT("/") + KEY_STARNET_CALLSIGN_SWITCH5, &temp, long(DEFAULT_STARNET_CALLSIGN_SWITCH));
	m_starNet5CallsignSwitch = STARNET_CALLSIGN_SWITCH(temp);

	m_config->Read(wxT("/") + KEY_STARNET_TXMSG_SWITCH5, &m_starNet5TxMsgSwitch, DEFAULT_STARNET_TXMSG_SWITCH);

	m_config->Read(wxT("/") + KEY_STARNET_REFLECTOR5, &m_starNet5Reflector, DEFAULT_STARNET_REFLECTOR);

	m_config->Read(wxT("/") + KEY_STARNET_BAND6, &m_starNet6Band, DEFAULT_STARNET_BAND);

	m_config->Read(wxT("/") + KEY_STARNET_CALLSIGN6, &m_starNet6Callsign, DEFAULT_STARNET_CALLSIGN);

	m_config->Read(wxT("/") + KEY_STARNET_LOGOFF6, &m_starNet6Logoff, DEFAULT_STARNET_LOGOFF);

	m_config->Read(wxT("/") + KEY_STARNET_INFO6, &m_starNet6Info, DEFAULT_STARNET_INFO);

	m_config->Read(wxT("/") + KEY_STARNET_PERMANENT6, &m_starNet6Permanent, DEFAULT_STARNET_PERMANENT);

	m_config->Read(wxT("/") + KEY_STARNET_USER_TIMEOUT6, &temp, long(DEFAULT_STARNET_USER_TIMEOUT));
	m_starNet6UserTimeout = (unsigned int)temp;

	m_config->Read(wxT("/") + KEY_STARNET_GROUP_TIMEOUT6, &temp, long(DEFAULT_STARNET_GROUP_TIMEOUT));
	m_starNet6GroupTimeout = (unsigned int)temp;

	m_config->Read(wxT("/") + KEY_STARNET_CALLSIGN_SWITCH6, &temp, long(DEFAULT_STARNET_CALLSIGN_SWITCH));
	m_starNet6CallsignSwitch = STARNET_CALLSIGN_SWITCH(temp);

	m_config->Read(wxT("/") + KEY_STARNET_TXMSG_SWITCH6, &m_starNet6TxMsgSwitch, DEFAULT_STARNET_TXMSG_SWITCH);

	m_config->Read(wxT("/") + KEY_STARNET_REFLECTOR6, &m_starNet6Reflector, DEFAULT_STARNET_REFLECTOR);

	m_config->Read(wxT("/") + KEY_STARNET_BAND7, &m_starNet7Band, DEFAULT_STARNET_BAND);

	m_config->Read(wxT("/") + KEY_STARNET_CALLSIGN7, &m_starNet7Callsign, DEFAULT_STARNET_CALLSIGN);

	m_config->Read(wxT("/") + KEY_STARNET_LOGOFF7, &m_starNet7Logoff, DEFAULT_STARNET_LOGOFF);

	m_config->Read(wxT("/") + KEY_STARNET_INFO7, &m_starNet7Info, DEFAULT_STARNET_INFO);

	m_config->Read(wxT("/") + KEY_STARNET_PERMANENT7, &m_starNet7Permanent, DEFAULT_STARNET_PERMANENT);

	m_config->Read(wxT("/") + KEY_STARNET_USER_TIMEOUT7, &temp, long(DEFAULT_STARNET_USER_TIMEOUT));
	m_starNet7UserTimeout = (unsigned int)temp;

	m_config->Read(wxT("/") + KEY_STARNET_GROUP_TIMEOUT7, &temp, long(DEFAULT_STARNET_GROUP_TIMEOUT));
	m_starNet7GroupTimeout = (unsigned int)temp;

	m_config->Read(wxT("/") + KEY_STARNET_CALLSIGN_SWITCH7, &temp, long(DEFAULT_STARNET_CALLSIGN_SWITCH));
	m_starNet7CallsignSwitch = STARNET_CALLSIGN_SWITCH(temp);

	m_config->Read(wxT("/") + KEY_STARNET_TXMSG_SWITCH7, &m_starNet7TxMsgSwitch, DEFAULT_STARNET_TXMSG_SWITCH);

	m_config->Read(wxT("/") + KEY_STARNET_REFLECTOR7, &m_starNet7Reflector, DEFAULT_STARNET_REFLECTOR);

	m_config->Read(wxT("/") + KEY_STARNET_BAND8, &m_starNet8Band, DEFAULT_STARNET_BAND);

	m_config->Read(wxT("/") + KEY_STARNET_CALLSIGN8, &m_starNet8Callsign, DEFAULT_STARNET_CALLSIGN);

	m_config->Read(wxT("/") + KEY_STARNET_LOGOFF8, &m_starNet8Logoff, DEFAULT_STARNET_LOGOFF);

	m_config->Read(wxT("/") + KEY_STARNET_INFO8, &m_starNet8Info, DEFAULT_STARNET_INFO);

	m_config->Read(wxT("/") + KEY_STARNET_PERMANENT8, &m_starNet8Permanent, DEFAULT_STARNET_PERMANENT);

	m_config->Read(wxT("/") + KEY_STARNET_USER_TIMEOUT8, &temp, long(DEFAULT_STARNET_USER_TIMEOUT));
	m_starNet8UserTimeout = (unsigned int)temp;

	m_config->Read(wxT("/") + KEY_STARNET_GROUP_TIMEOUT8, &temp, long(DEFAULT_STARNET_GROUP_TIMEOUT));
	m_starNet8GroupTimeout = (unsigned int)temp;

	m_config->Read(wxT("/") + KEY_STARNET_CALLSIGN_SWITCH8, &temp, long(DEFAULT_STARNET_CALLSIGN_SWITCH));
	m_starNet8CallsignSwitch = STARNET_CALLSIGN_SWITCH(temp);

	m_config->Read(wxT("/") + KEY_STARNET_TXMSG_SWITCH8, &m_starNet8TxMsgSwitch, DEFAULT_STARNET_TXMSG_SWITCH);

	m_config->Read(wxT("/") + KEY_STARNET_REFLECTOR8, &m_starNet8Reflector, DEFAULT_STARNET_REFLECTOR);

	m_config->Read(wxT("/") + KEY_STARNET_BAND9, &m_starNet9Band, DEFAULT_STARNET_BAND);

	m_config->Read(wxT("/") + KEY_STARNET_CALLSIGN9, &m_starNet9Callsign, DEFAULT_STARNET_CALLSIGN);

	m_config->Read(wxT("/") + KEY_STARNET_LOGOFF9, &m_starNet9Logoff, DEFAULT_STARNET_LOGOFF);

	m_config->Read(wxT("/") + KEY_STARNET_INFO9, &m_starNet9Info, DEFAULT_STARNET_INFO);

	m_config->Read(wxT("/") + KEY_STARNET_PERMANENT9, &m_starNet9Permanent, DEFAULT_STARNET_PERMANENT);

	m_config->Read(wxT("/") + KEY_STARNET_USER_TIMEOUT9, &temp, long(DEFAULT_STARNET_USER_TIMEOUT));
	m_starNet9UserTimeout = (unsigned int)temp;

	m_config->Read(wxT("/") + KEY_STARNET_GROUP_TIMEOUT9, &temp, long(DEFAULT_STARNET_GROUP_TIMEOUT));
	m_starNet9GroupTimeout = (unsigned int)temp;

	m_config->Read(wxT("/") + KEY_STARNET_CALLSIGN_SWITCH9, &temp, long(DEFAULT_STARNET_CALLSIGN_SWITCH));
	m_starNet9CallsignSwitch = STARNET_CALLSIGN_SWITCH(temp);

	m_config->Read(wxT("/") + KEY_STARNET_TXMSG_SWITCH9, &m_starNet9TxMsgSwitch, DEFAULT_STARNET_TXMSG_SWITCH);

	m_config->Read(wxT("/") + KEY_STARNET_REFLECTOR9, &m_starNet9Reflector, DEFAULT_STARNET_REFLECTOR);

	m_config->Read(wxT("/") + KEY_STARNET_BAND10, &m_starNet10Band, DEFAULT_STARNET_BAND);

	m_config->Read(wxT("/") + KEY_STARNET_CALLSIGN10, &m_starNet10Callsign, DEFAULT_STARNET_CALLSIGN);

	m_config->Read(wxT("/") + KEY_STARNET_LOGOFF10, &m_starNet10Logoff, DEFAULT_STARNET_LOGOFF);

	m_config->Read(wxT("/") + KEY_STARNET_INFO10, &m_starNet10Info, DEFAULT_STARNET_INFO);

	m_config->Read(wxT("/") + KEY_STARNET_PERMANENT10, &m_starNet10Permanent, DEFAULT_STARNET_PERMANENT);

	m_config->Read(wxT("/") + KEY_STARNET_USER_TIMEOUT10, &temp, long(DEFAULT_STARNET_USER_TIMEOUT));
	m_starNet10UserTimeout = (unsigned int)temp;

	m_config->Read(wxT("/") + KEY_STARNET_GROUP_TIMEOUT10, &temp, long(DEFAULT_STARNET_GROUP_TIMEOUT));
	m_starNet10GroupTimeout = (unsigned int)temp;

	m_config->Read(wxT("/") + KEY_STARNET_CALLSIGN_SWITCH10, &temp, long(DEFAULT_STARNET_CALLSIGN_SWITCH));
	m_starNet10CallsignSwitch = STARNET_CALLSIGN_SWITCH(temp);

	m_config->Read(wxT("/") + KEY_STARNET_TXMSG_SWITCH10, &m_starNet10TxMsgSwitch, DEFAULT_STARNET_TXMSG_SWITCH);

	m_config->Read(wxT("/") + KEY_STARNET_REFLECTOR10, &m_starNet10Reflector, DEFAULT_STARNET_REFLECTOR);

	m_config->Read(wxT("/") + KEY_STARNET_BAND11, &m_starNet11Band, DEFAULT_STARNET_BAND);

	m_config->Read(wxT("/") + KEY_STARNET_CALLSIGN11, &m_starNet11Callsign, DEFAULT_STARNET_CALLSIGN);

	m_config->Read(wxT("/") + KEY_STARNET_LOGOFF11, &m_starNet11Logoff, DEFAULT_STARNET_LOGOFF);

	m_config->Read(wxT("/") + KEY_STARNET_INFO11, &m_starNet11Info, DEFAULT_STARNET_INFO);

	m_config->Read(wxT("/") + KEY_STARNET_PERMANENT11, &m_starNet11Permanent, DEFAULT_STARNET_PERMANENT);

	m_config->Read(wxT("/") + KEY_STARNET_USER_TIMEOUT11, &temp, long(DEFAULT_STARNET_USER_TIMEOUT));
	m_starNet11UserTimeout = (unsigned int)temp;

	m_config->Read(wxT("/") + KEY_STARNET_GROUP_TIMEOUT11, &temp, long(DEFAULT_STARNET_GROUP_TIMEOUT));
	m_starNet11GroupTimeout = (unsigned int)temp;

	m_config->Read(wxT("/") + KEY_STARNET_CALLSIGN_SWITCH11, &temp, long(DEFAULT_STARNET_CALLSIGN_SWITCH));
	m_starNet11CallsignSwitch = STARNET_CALLSIGN_SWITCH(temp);

	m_config->Read(wxT("/") + KEY_STARNET_TXMSG_SWITCH11, &m_starNet11TxMsgSwitch, DEFAULT_STARNET_TXMSG_SWITCH);

	m_config->Read(wxT("/") + KEY_STARNET_REFLECTOR11, &m_starNet11Reflector, DEFAULT_STARNET_REFLECTOR);

	m_config->Read(wxT("/") + KEY_STARNET_BAND12, &m_starNet12Band, DEFAULT_STARNET_BAND);

	m_config->Read(wxT("/") + KEY_STARNET_CALLSIGN12, &m_starNet12Callsign, DEFAULT_STARNET_CALLSIGN);

	m_config->Read(wxT("/") + KEY_STARNET_LOGOFF12, &m_starNet12Logoff, DEFAULT_STARNET_LOGOFF);

	m_config->Read(wxT("/") + KEY_STARNET_INFO12, &m_starNet12Info, DEFAULT_STARNET_INFO);

	m_config->Read(wxT("/") + KEY_STARNET_PERMANENT12, &m_starNet12Permanent, DEFAULT_STARNET_PERMANENT);

	m_config->Read(wxT("/") + KEY_STARNET_USER_TIMEOUT12, &temp, long(DEFAULT_STARNET_USER_TIMEOUT));
	m_starNet12UserTimeout = (unsigned int)temp;

	m_config->Read(wxT("/") + KEY_STARNET_GROUP_TIMEOUT12, &temp, long(DEFAULT_STARNET_GROUP_TIMEOUT));
	m_starNet12GroupTimeout = (unsigned int)temp;

	m_config->Read(wxT("/") + KEY_STARNET_CALLSIGN_SWITCH12, &temp, long(DEFAULT_STARNET_CALLSIGN_SWITCH));
	m_starNet12CallsignSwitch = STARNET_CALLSIGN_SWITCH(temp);

	m_config->Read(wxT("/") + KEY_STARNET_TXMSG_SWITCH12, &m_starNet12TxMsgSwitch, DEFAULT_STARNET_TXMSG_SWITCH);

	m_config->Read(wxT("/") + KEY_STARNET_REFLECTOR12, &m_starNet12Reflector, DEFAULT_STARNET_REFLECTOR);

	m_config->Read(wxT("/") + KEY_STARNET_BAND13, &m_starNet13Band, DEFAULT_STARNET_BAND);

	m_config->Read(wxT("/") + KEY_STARNET_CALLSIGN13, &m_starNet13Callsign, DEFAULT_STARNET_CALLSIGN);

	m_config->Read(wxT("/") + KEY_STARNET_LOGOFF13, &m_starNet13Logoff, DEFAULT_STARNET_LOGOFF);

	m_config->Read(wxT("/") + KEY_STARNET_INFO13, &m_starNet13Info, DEFAULT_STARNET_INFO);

	m_config->Read(wxT("/") + KEY_STARNET_PERMANENT13, &m_starNet13Permanent, DEFAULT_STARNET_PERMANENT);

	m_config->Read(wxT("/") + KEY_STARNET_USER_TIMEOUT13, &temp, long(DEFAULT_STARNET_USER_TIMEOUT));
	m_starNet13UserTimeout = (unsigned int)temp;

	m_config->Read(wxT("/") + KEY_STARNET_GROUP_TIMEOUT13, &temp, long(DEFAULT_STARNET_GROUP_TIMEOUT));
	m_starNet13GroupTimeout = (unsigned int)temp;

	m_config->Read(wxT("/") + KEY_STARNET_CALLSIGN_SWITCH13, &temp, long(DEFAULT_STARNET_CALLSIGN_SWITCH));
	m_starNet13CallsignSwitch = STARNET_CALLSIGN_SWITCH(temp);

	m_config->Read(wxT("/") + KEY_STARNET_TXMSG_SWITCH13, &m_starNet13TxMsgSwitch, DEFAULT_STARNET_TXMSG_SWITCH);

	m_config->Read(wxT("/") + KEY_STARNET_REFLECTOR13, &m_starNet13Reflector, DEFAULT_STARNET_REFLECTOR);

	m_config->Read(wxT("/") + KEY_STARNET_BAND14, &m_starNet14Band, DEFAULT_STARNET_BAND);

	m_config->Read(wxT("/") + KEY_STARNET_CALLSIGN14, &m_starNet14Callsign, DEFAULT_STARNET_CALLSIGN);

	m_config->Read(wxT("/") + KEY_STARNET_LOGOFF14, &m_starNet14Logoff, DEFAULT_STARNET_LOGOFF);

	m_config->Read(wxT("/") + KEY_STARNET_INFO14, &m_starNet14Info, DEFAULT_STARNET_INFO);

	m_config->Read(wxT("/") + KEY_STARNET_PERMANENT14, &m_starNet14Permanent, DEFAULT_STARNET_PERMANENT);

	m_config->Read(wxT("/") + KEY_STARNET_USER_TIMEOUT14, &temp, long(DEFAULT_STARNET_USER_TIMEOUT));
	m_starNet14UserTimeout = (unsigned int)temp;

	m_config->Read(wxT("/") + KEY_STARNET_GROUP_TIMEOUT14, &temp, long(DEFAULT_STARNET_GROUP_TIMEOUT));
	m_starNet14GroupTimeout = (unsigned int)temp;

	m_config->Read(wxT("/") + KEY_STARNET_CALLSIGN_SWITCH14, &temp, long(DEFAULT_STARNET_CALLSIGN_SWITCH));
	m_starNet14CallsignSwitch = STARNET_CALLSIGN_SWITCH(temp);

	m_config->Read(wxT("/") + KEY_STARNET_TXMSG_SWITCH14, &m_starNet14TxMsgSwitch, DEFAULT_STARNET_TXMSG_SWITCH);

	m_config->Read(wxT("/") + KEY_STARNET_REFLECTOR14, &m_starNet14Reflector, DEFAULT_STARNET_REFLECTOR);

	m_config->Read(wxT("/") + KEY_STARNET_BAND15, &m_starNet15Band, DEFAULT_STARNET_BAND);

	m_config->Read(wxT("/") + KEY_STARNET_CALLSIGN15, &m_starNet15Callsign, DEFAULT_STARNET_CALLSIGN);

	m_config->Read(wxT("/") + KEY_STARNET_LOGOFF15, &m_starNet15Logoff, DEFAULT_STARNET_LOGOFF);

	m_config->Read(wxT("/") + KEY_STARNET_INFO15, &m_starNet15Info, DEFAULT_STARNET_INFO);

	m_config->Read(wxT("/") + KEY_STARNET_PERMANENT15, &m_starNet15Permanent, DEFAULT_STARNET_PERMANENT);

	m_config->Read(wxT("/") + KEY_STARNET_USER_TIMEOUT15, &temp, long(DEFAULT_STARNET_USER_TIMEOUT));
	m_starNet15UserTimeout = (unsigned int)temp;

	m_config->Read(wxT("/") + KEY_STARNET_GROUP_TIMEOUT15, &temp, long(DEFAULT_STARNET_GROUP_TIMEOUT));
	m_starNet15GroupTimeout = (unsigned int)temp;

	m_config->Read(wxT("/") + KEY_STARNET_CALLSIGN_SWITCH15, &temp, long(DEFAULT_STARNET_CALLSIGN_SWITCH));
	m_starNet15CallsignSwitch = STARNET_CALLSIGN_SWITCH(temp);

	m_config->Read(wxT("/") + KEY_STARNET_TXMSG_SWITCH15, &m_starNet15TxMsgSwitch, DEFAULT_STARNET_TXMSG_SWITCH);

	m_config->Read(wxT("/") + KEY_STARNET_REFLECTOR15, &m_starNet15Reflector, DEFAULT_STARNET_REFLECTOR);

	m_config->Read(wxT("/") + KEY_REMOTE_ENABLED, &m_remoteEnabled, DEFAULT_REMOTE_ENABLED);

	m_config->Read(wxT("/") + KEY_REMOTE_PASSWORD, &m_remotePassword, DEFAULT_REMOTE_PASSWORD);

	m_config->Read(wxT("/") + KEY_REMOTE_PORT, &temp, long(DEFAULT_REMOTE_PORT));
	m_remotePort = (unsigned int)temp;

	m_config->Read(wxT("/") + KEY_LOG_ENABLED, &m_logEnabled, DEFAULT_LOG_ENABLED);

	m_config->Read(wxT("/") + KEY_WINDOW_X, &temp, long(DEFAULT_WINDOW_X));
	m_x = int(temp);

	m_config->Read(wxT("/") + KEY_WINDOW_Y, &temp, long(DEFAULT_WINDOW_Y));
	m_y = int(temp);
}

CStarNetServerConfig::~CStarNetServerConfig()
{
	delete m_config;
}

#else

CStarNetServerConfig::CStarNetServerConfig(const wxString& dir) :
m_fileName(),
m_callsign(DEFAULT_CALLSIGN),
m_address(DEFAULT_ADDRESS),
m_ircddbHostname(DEFAULT_IRCDDB_HOSTNAME),
m_ircddbUsername(DEFAULT_IRCDDB_USERNAME),
m_ircddbPassword(DEFAULT_IRCDDB_PASSWORD),
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
m_starNet6Band(DEFAULT_STARNET_BAND),
m_starNet6Callsign(DEFAULT_STARNET_CALLSIGN),
m_starNet6Logoff(DEFAULT_STARNET_LOGOFF),
m_starNet6Info(DEFAULT_STARNET_INFO),
m_starNet6Permanent(DEFAULT_STARNET_PERMANENT),
m_starNet6UserTimeout(DEFAULT_STARNET_USER_TIMEOUT),
m_starNet6GroupTimeout(DEFAULT_STARNET_GROUP_TIMEOUT),
m_starNet6CallsignSwitch(DEFAULT_STARNET_CALLSIGN_SWITCH),
m_starNet6TxMsgSwitch(DEFAULT_STARNET_TXMSG_SWITCH),
m_starNet6Reflector(DEFAULT_STARNET_REFLECTOR),
m_starNet7Band(DEFAULT_STARNET_BAND),
m_starNet7Callsign(DEFAULT_STARNET_CALLSIGN),
m_starNet7Logoff(DEFAULT_STARNET_LOGOFF),
m_starNet7Info(DEFAULT_STARNET_INFO),
m_starNet7Permanent(DEFAULT_STARNET_PERMANENT),
m_starNet7UserTimeout(DEFAULT_STARNET_USER_TIMEOUT),
m_starNet7GroupTimeout(DEFAULT_STARNET_GROUP_TIMEOUT),
m_starNet7CallsignSwitch(DEFAULT_STARNET_CALLSIGN_SWITCH),
m_starNet7TxMsgSwitch(DEFAULT_STARNET_TXMSG_SWITCH),
m_starNet7Reflector(DEFAULT_STARNET_REFLECTOR),
m_starNet8Band(DEFAULT_STARNET_BAND),
m_starNet8Callsign(DEFAULT_STARNET_CALLSIGN),
m_starNet8Logoff(DEFAULT_STARNET_LOGOFF),
m_starNet8Info(DEFAULT_STARNET_INFO),
m_starNet8Permanent(DEFAULT_STARNET_PERMANENT),
m_starNet8UserTimeout(DEFAULT_STARNET_USER_TIMEOUT),
m_starNet8GroupTimeout(DEFAULT_STARNET_GROUP_TIMEOUT),
m_starNet8CallsignSwitch(DEFAULT_STARNET_CALLSIGN_SWITCH),
m_starNet8TxMsgSwitch(DEFAULT_STARNET_TXMSG_SWITCH),
m_starNet8Reflector(DEFAULT_STARNET_REFLECTOR),
m_starNet9Band(DEFAULT_STARNET_BAND),
m_starNet9Callsign(DEFAULT_STARNET_CALLSIGN),
m_starNet9Logoff(DEFAULT_STARNET_LOGOFF),
m_starNet9Info(DEFAULT_STARNET_INFO),
m_starNet9Permanent(DEFAULT_STARNET_PERMANENT),
m_starNet9UserTimeout(DEFAULT_STARNET_USER_TIMEOUT),
m_starNet9GroupTimeout(DEFAULT_STARNET_GROUP_TIMEOUT),
m_starNet9CallsignSwitch(DEFAULT_STARNET_CALLSIGN_SWITCH),
m_starNet9TxMsgSwitch(DEFAULT_STARNET_TXMSG_SWITCH),
m_starNet9Reflector(DEFAULT_STARNET_REFLECTOR),
m_starNet10Band(DEFAULT_STARNET_BAND),
m_starNet10Callsign(DEFAULT_STARNET_CALLSIGN),
m_starNet10Logoff(DEFAULT_STARNET_LOGOFF),
m_starNet10Info(DEFAULT_STARNET_INFO),
m_starNet10Permanent(DEFAULT_STARNET_PERMANENT),
m_starNet10UserTimeout(DEFAULT_STARNET_USER_TIMEOUT),
m_starNet10GroupTimeout(DEFAULT_STARNET_GROUP_TIMEOUT),
m_starNet10CallsignSwitch(DEFAULT_STARNET_CALLSIGN_SWITCH),
m_starNet10TxMsgSwitch(DEFAULT_STARNET_TXMSG_SWITCH),
m_starNet10Reflector(DEFAULT_STARNET_REFLECTOR),
m_starNet11Band(DEFAULT_STARNET_BAND),
m_starNet11Callsign(DEFAULT_STARNET_CALLSIGN),
m_starNet11Logoff(DEFAULT_STARNET_LOGOFF),
m_starNet11Info(DEFAULT_STARNET_INFO),
m_starNet11Permanent(DEFAULT_STARNET_PERMANENT),
m_starNet11UserTimeout(DEFAULT_STARNET_USER_TIMEOUT),
m_starNet11GroupTimeout(DEFAULT_STARNET_GROUP_TIMEOUT),
m_starNet11CallsignSwitch(DEFAULT_STARNET_CALLSIGN_SWITCH),
m_starNet11TxMsgSwitch(DEFAULT_STARNET_TXMSG_SWITCH),
m_starNet11Reflector(DEFAULT_STARNET_REFLECTOR),
m_starNet12Band(DEFAULT_STARNET_BAND),
m_starNet12Callsign(DEFAULT_STARNET_CALLSIGN),
m_starNet12Logoff(DEFAULT_STARNET_LOGOFF),
m_starNet12Info(DEFAULT_STARNET_INFO),
m_starNet12Permanent(DEFAULT_STARNET_PERMANENT),
m_starNet12UserTimeout(DEFAULT_STARNET_USER_TIMEOUT),
m_starNet12GroupTimeout(DEFAULT_STARNET_GROUP_TIMEOUT),
m_starNet12CallsignSwitch(DEFAULT_STARNET_CALLSIGN_SWITCH),
m_starNet12TxMsgSwitch(DEFAULT_STARNET_TXMSG_SWITCH),
m_starNet12Reflector(DEFAULT_STARNET_REFLECTOR),
m_starNet13Band(DEFAULT_STARNET_BAND),
m_starNet13Callsign(DEFAULT_STARNET_CALLSIGN),
m_starNet13Logoff(DEFAULT_STARNET_LOGOFF),
m_starNet13Info(DEFAULT_STARNET_INFO),
m_starNet13Permanent(DEFAULT_STARNET_PERMANENT),
m_starNet13UserTimeout(DEFAULT_STARNET_USER_TIMEOUT),
m_starNet13GroupTimeout(DEFAULT_STARNET_GROUP_TIMEOUT),
m_starNet13CallsignSwitch(DEFAULT_STARNET_CALLSIGN_SWITCH),
m_starNet13TxMsgSwitch(DEFAULT_STARNET_TXMSG_SWITCH),
m_starNet13Reflector(DEFAULT_STARNET_REFLECTOR),
m_starNet14Band(DEFAULT_STARNET_BAND),
m_starNet14Callsign(DEFAULT_STARNET_CALLSIGN),
m_starNet14Logoff(DEFAULT_STARNET_LOGOFF),
m_starNet14Info(DEFAULT_STARNET_INFO),
m_starNet14Permanent(DEFAULT_STARNET_PERMANENT),
m_starNet14UserTimeout(DEFAULT_STARNET_USER_TIMEOUT),
m_starNet14GroupTimeout(DEFAULT_STARNET_GROUP_TIMEOUT),
m_starNet14CallsignSwitch(DEFAULT_STARNET_CALLSIGN_SWITCH),
m_starNet14TxMsgSwitch(DEFAULT_STARNET_TXMSG_SWITCH),
m_starNet14Reflector(DEFAULT_STARNET_REFLECTOR),
m_starNet15Band(DEFAULT_STARNET_BAND),
m_starNet15Callsign(DEFAULT_STARNET_CALLSIGN),
m_starNet15Logoff(DEFAULT_STARNET_LOGOFF),
m_starNet15Info(DEFAULT_STARNET_INFO),
m_starNet15Permanent(DEFAULT_STARNET_PERMANENT),
m_starNet15UserTimeout(DEFAULT_STARNET_USER_TIMEOUT),
m_starNet15GroupTimeout(DEFAULT_STARNET_GROUP_TIMEOUT),
m_starNet15CallsignSwitch(DEFAULT_STARNET_CALLSIGN_SWITCH),
m_starNet15TxMsgSwitch(DEFAULT_STARNET_TXMSG_SWITCH),
m_starNet15Reflector(DEFAULT_STARNET_REFLECTOR),
m_remoteEnabled(DEFAULT_REMOTE_ENABLED),
m_remotePassword(DEFAULT_REMOTE_PASSWORD),
m_remotePort(DEFAULT_REMOTE_PORT),
m_logEnabled(DEFAULT_LOG_ENABLED),
m_x(DEFAULT_WINDOW_X),
m_y(DEFAULT_WINDOW_Y)
{
	wxASSERT(!dir.IsEmpty());

	m_fileName.Assign(dir, CONFIG_FILE_NAME);

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
		} else if (key.IsSameAs(KEY_ADDRESS)) {
			m_address = val;
		} else if (key.IsSameAs(KEY_IRCDDB_HOSTNAME)) {
			m_ircddbHostname = val;
		} else if (key.IsSameAs(KEY_IRCDDB_USERNAME)) {
			m_ircddbUsername = val;
		} else if (key.IsSameAs(KEY_IRCDDB_PASSWORD)) {
			m_ircddbPassword = val;
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
		} else if (key.IsSameAs(KEY_STARNET_BAND6)) {
			m_starNet6Band = val;
		} else if (key.IsSameAs(KEY_STARNET_CALLSIGN6)) {
			m_starNet6Callsign = val;
		} else if (key.IsSameAs(KEY_STARNET_LOGOFF6)) {
			m_starNet6Logoff = val;
		} else if (key.IsSameAs(KEY_STARNET_INFO6)) {
			m_starNet6Info = val;
		} else if (key.IsSameAs(KEY_STARNET_PERMANENT6)) {
			m_starNet6Permanent = val;
		} else if (key.IsSameAs(KEY_STARNET_USER_TIMEOUT6)) {
			val.ToULong(&temp2);
			m_starNet6UserTimeout = (unsigned int)temp2;
		} else if (key.IsSameAs(KEY_STARNET_GROUP_TIMEOUT6)) {
			val.ToULong(&temp2);
			m_starNet6GroupTimeout = (unsigned int)temp2;
		} else if (key.IsSameAs(KEY_STARNET_CALLSIGN_SWITCH6)) {
			val.ToLong(&temp1);
			m_starNet6CallsignSwitch = STARNET_CALLSIGN_SWITCH(temp1);
		} else if (key.IsSameAs(KEY_STARNET_TXMSG_SWITCH6)) {
			val.ToLong(&temp1);
			m_starNet6TxMsgSwitch = temp1 == 1L;
		} else if (key.IsSameAs(KEY_STARNET_REFLECTOR6)) {
			m_starNet6Reflector = val;
		} else if (key.IsSameAs(KEY_STARNET_BAND7)) {
			m_starNet7Band = val;
		} else if (key.IsSameAs(KEY_STARNET_CALLSIGN7)) {
			m_starNet7Callsign = val;
		} else if (key.IsSameAs(KEY_STARNET_LOGOFF7)) {
			m_starNet7Logoff = val;
		} else if (key.IsSameAs(KEY_STARNET_INFO7)) {
			m_starNet7Info = val;
		} else if (key.IsSameAs(KEY_STARNET_PERMANENT7)) {
			m_starNet7Permanent = val;
		} else if (key.IsSameAs(KEY_STARNET_USER_TIMEOUT7)) {
			val.ToULong(&temp2);
			m_starNet7UserTimeout = (unsigned int)temp2;
		} else if (key.IsSameAs(KEY_STARNET_GROUP_TIMEOUT7)) {
			val.ToULong(&temp2);
			m_starNet7GroupTimeout = (unsigned int)temp2;
		} else if (key.IsSameAs(KEY_STARNET_CALLSIGN_SWITCH7)) {
			val.ToLong(&temp1);
			m_starNet7CallsignSwitch = STARNET_CALLSIGN_SWITCH(temp1);
		} else if (key.IsSameAs(KEY_STARNET_TXMSG_SWITCH7)) {
			val.ToLong(&temp1);
			m_starNet7TxMsgSwitch = temp1 == 1L;
		} else if (key.IsSameAs(KEY_STARNET_REFLECTOR7)) {
			m_starNet7Reflector = val;
		} else if (key.IsSameAs(KEY_STARNET_BAND8)) {
			m_starNet8Band = val;
		} else if (key.IsSameAs(KEY_STARNET_CALLSIGN8)) {
			m_starNet8Callsign = val;
		} else if (key.IsSameAs(KEY_STARNET_LOGOFF8)) {
			m_starNet8Logoff = val;
		} else if (key.IsSameAs(KEY_STARNET_INFO8)) {
			m_starNet8Info = val;
		} else if (key.IsSameAs(KEY_STARNET_PERMANENT8)) {
			m_starNet8Permanent = val;
		} else if (key.IsSameAs(KEY_STARNET_USER_TIMEOUT8)) {
			val.ToULong(&temp2);
			m_starNet8UserTimeout = (unsigned int)temp2;
		} else if (key.IsSameAs(KEY_STARNET_GROUP_TIMEOUT8)) {
			val.ToULong(&temp2);
			m_starNet8GroupTimeout = (unsigned int)temp2;
		} else if (key.IsSameAs(KEY_STARNET_CALLSIGN_SWITCH8)) {
			val.ToLong(&temp1);
			m_starNet8CallsignSwitch = STARNET_CALLSIGN_SWITCH(temp1);
		} else if (key.IsSameAs(KEY_STARNET_TXMSG_SWITCH8)) {
			val.ToLong(&temp1);
			m_starNet8TxMsgSwitch = temp1 == 1L;
		} else if (key.IsSameAs(KEY_STARNET_REFLECTOR8)) {
			m_starNet8Reflector = val;
		} else if (key.IsSameAs(KEY_STARNET_BAND9)) {
			m_starNet9Band = val;
		} else if (key.IsSameAs(KEY_STARNET_CALLSIGN9)) {
			m_starNet9Callsign = val;
		} else if (key.IsSameAs(KEY_STARNET_LOGOFF9)) {
			m_starNet9Logoff = val;
		} else if (key.IsSameAs(KEY_STARNET_INFO9)) {
			m_starNet9Info = val;
		} else if (key.IsSameAs(KEY_STARNET_PERMANENT9)) {
			m_starNet9Permanent = val;
		} else if (key.IsSameAs(KEY_STARNET_USER_TIMEOUT9)) {
			val.ToULong(&temp2);
			m_starNet9UserTimeout = (unsigned int)temp2;
		} else if (key.IsSameAs(KEY_STARNET_GROUP_TIMEOUT9)) {
			val.ToULong(&temp2);
			m_starNet9GroupTimeout = (unsigned int)temp2;
		} else if (key.IsSameAs(KEY_STARNET_CALLSIGN_SWITCH9)) {
			val.ToLong(&temp1);
			m_starNet9CallsignSwitch = STARNET_CALLSIGN_SWITCH(temp1);
		} else if (key.IsSameAs(KEY_STARNET_TXMSG_SWITCH9)) {
			val.ToLong(&temp1);
			m_starNet9TxMsgSwitch = temp1 == 1L;
		} else if (key.IsSameAs(KEY_STARNET_REFLECTOR9)) {
			m_starNet9Reflector = val;
		} else if (key.IsSameAs(KEY_STARNET_BAND10)) {
			m_starNet10Band = val;
		} else if (key.IsSameAs(KEY_STARNET_CALLSIGN10)) {
			m_starNet10Callsign = val;
		} else if (key.IsSameAs(KEY_STARNET_LOGOFF10)) {
			m_starNet10Logoff = val;
		} else if (key.IsSameAs(KEY_STARNET_INFO10)) {
			m_starNet10Info = val;
		} else if (key.IsSameAs(KEY_STARNET_PERMANENT10)) {
			m_starNet10Permanent = val;
		} else if (key.IsSameAs(KEY_STARNET_USER_TIMEOUT10)) {
			val.ToULong(&temp2);
			m_starNet10UserTimeout = (unsigned int)temp2;
		} else if (key.IsSameAs(KEY_STARNET_GROUP_TIMEOUT10)) {
			val.ToULong(&temp2);
			m_starNet10GroupTimeout = (unsigned int)temp2;
		} else if (key.IsSameAs(KEY_STARNET_CALLSIGN_SWITCH10)) {
			val.ToLong(&temp1);
			m_starNet10CallsignSwitch = STARNET_CALLSIGN_SWITCH(temp1);
		} else if (key.IsSameAs(KEY_STARNET_TXMSG_SWITCH10)) {
			val.ToLong(&temp1);
			m_starNet10TxMsgSwitch = temp1 == 1L;
		} else if (key.IsSameAs(KEY_STARNET_REFLECTOR10)) {
			m_starNet10Reflector = val;
		} else if (key.IsSameAs(KEY_STARNET_BAND11)) {
			m_starNet11Band = val;
		} else if (key.IsSameAs(KEY_STARNET_CALLSIGN11)) {
			m_starNet11Callsign = val;
		} else if (key.IsSameAs(KEY_STARNET_LOGOFF11)) {
			m_starNet11Logoff = val;
		} else if (key.IsSameAs(KEY_STARNET_INFO11)) {
			m_starNet11Info = val;
		} else if (key.IsSameAs(KEY_STARNET_PERMANENT11)) {
			m_starNet11Permanent = val;
		} else if (key.IsSameAs(KEY_STARNET_USER_TIMEOUT11)) {
			val.ToULong(&temp2);
			m_starNet11UserTimeout = (unsigned int)temp2;
		} else if (key.IsSameAs(KEY_STARNET_GROUP_TIMEOUT11)) {
			val.ToULong(&temp2);
			m_starNet11GroupTimeout = (unsigned int)temp2;
		} else if (key.IsSameAs(KEY_STARNET_CALLSIGN_SWITCH11)) {
			val.ToLong(&temp1);
			m_starNet11CallsignSwitch = STARNET_CALLSIGN_SWITCH(temp1);
		} else if (key.IsSameAs(KEY_STARNET_TXMSG_SWITCH11)) {
			val.ToLong(&temp1);
			m_starNet11TxMsgSwitch = temp1 == 1L;
		} else if (key.IsSameAs(KEY_STARNET_REFLECTOR11)) {
			m_starNet11Reflector = val;
		} else if (key.IsSameAs(KEY_STARNET_BAND12)) {
			m_starNet12Band = val;
		} else if (key.IsSameAs(KEY_STARNET_CALLSIGN12)) {
			m_starNet12Callsign = val;
		} else if (key.IsSameAs(KEY_STARNET_LOGOFF12)) {
			m_starNet12Logoff = val;
		} else if (key.IsSameAs(KEY_STARNET_INFO12)) {
			m_starNet12Info = val;
		} else if (key.IsSameAs(KEY_STARNET_PERMANENT12)) {
			m_starNet12Permanent = val;
		} else if (key.IsSameAs(KEY_STARNET_USER_TIMEOUT12)) {
			val.ToULong(&temp2);
			m_starNet12UserTimeout = (unsigned int)temp2;
		} else if (key.IsSameAs(KEY_STARNET_GROUP_TIMEOUT12)) {
			val.ToULong(&temp2);
			m_starNet12GroupTimeout = (unsigned int)temp2;
		} else if (key.IsSameAs(KEY_STARNET_CALLSIGN_SWITCH12)) {
			val.ToLong(&temp1);
			m_starNet12CallsignSwitch = STARNET_CALLSIGN_SWITCH(temp1);
		} else if (key.IsSameAs(KEY_STARNET_TXMSG_SWITCH12)) {
			val.ToLong(&temp1);
			m_starNet12TxMsgSwitch = temp1 == 1L;
		} else if (key.IsSameAs(KEY_STARNET_REFLECTOR12)) {
			m_starNet12Reflector = val;
		} else if (key.IsSameAs(KEY_STARNET_BAND13)) {
			m_starNet13Band = val;
		} else if (key.IsSameAs(KEY_STARNET_CALLSIGN13)) {
			m_starNet13Callsign = val;
		} else if (key.IsSameAs(KEY_STARNET_LOGOFF13)) {
			m_starNet13Logoff = val;
		} else if (key.IsSameAs(KEY_STARNET_INFO13)) {
			m_starNet13Info = val;
		} else if (key.IsSameAs(KEY_STARNET_PERMANENT13)) {
			m_starNet13Permanent = val;
		} else if (key.IsSameAs(KEY_STARNET_USER_TIMEOUT13)) {
			val.ToULong(&temp2);
			m_starNet13UserTimeout = (unsigned int)temp2;
		} else if (key.IsSameAs(KEY_STARNET_GROUP_TIMEOUT13)) {
			val.ToULong(&temp2);
			m_starNet13GroupTimeout = (unsigned int)temp2;
		} else if (key.IsSameAs(KEY_STARNET_CALLSIGN_SWITCH13)) {
			val.ToLong(&temp1);
			m_starNet13CallsignSwitch = STARNET_CALLSIGN_SWITCH(temp1);
		} else if (key.IsSameAs(KEY_STARNET_TXMSG_SWITCH13)) {
			val.ToLong(&temp1);
			m_starNet13TxMsgSwitch = temp1 == 1L;
		} else if (key.IsSameAs(KEY_STARNET_REFLECTOR13)) {
			m_starNet13Reflector = val;
		} else if (key.IsSameAs(KEY_STARNET_BAND14)) {
			m_starNet14Band = val;
		} else if (key.IsSameAs(KEY_STARNET_CALLSIGN14)) {
			m_starNet14Callsign = val;
		} else if (key.IsSameAs(KEY_STARNET_LOGOFF14)) {
			m_starNet14Logoff = val;
		} else if (key.IsSameAs(KEY_STARNET_INFO14)) {
			m_starNet14Info = val;
		} else if (key.IsSameAs(KEY_STARNET_PERMANENT14)) {
			m_starNet14Permanent = val;
		} else if (key.IsSameAs(KEY_STARNET_USER_TIMEOUT14)) {
			val.ToULong(&temp2);
			m_starNet14UserTimeout = (unsigned int)temp2;
		} else if (key.IsSameAs(KEY_STARNET_GROUP_TIMEOUT14)) {
			val.ToULong(&temp2);
			m_starNet14GroupTimeout = (unsigned int)temp2;
		} else if (key.IsSameAs(KEY_STARNET_CALLSIGN_SWITCH14)) {
			val.ToLong(&temp1);
			m_starNet14CallsignSwitch = STARNET_CALLSIGN_SWITCH(temp1);
		} else if (key.IsSameAs(KEY_STARNET_TXMSG_SWITCH14)) {
			val.ToLong(&temp1);
			m_starNet14TxMsgSwitch = temp1 == 1L;
		} else if (key.IsSameAs(KEY_STARNET_REFLECTOR14)) {
			m_starNet14Reflector = val;
		} else if (key.IsSameAs(KEY_STARNET_BAND15)) {
			m_starNet15Band = val;
		} else if (key.IsSameAs(KEY_STARNET_CALLSIGN15)) {
			m_starNet15Callsign = val;
		} else if (key.IsSameAs(KEY_STARNET_LOGOFF15)) {
			m_starNet15Logoff = val;
		} else if (key.IsSameAs(KEY_STARNET_INFO15)) {
			m_starNet15Info = val;
		} else if (key.IsSameAs(KEY_STARNET_PERMANENT15)) {
			m_starNet15Permanent = val;
		} else if (key.IsSameAs(KEY_STARNET_USER_TIMEOUT15)) {
			val.ToULong(&temp2);
			m_starNet15UserTimeout = (unsigned int)temp2;
		} else if (key.IsSameAs(KEY_STARNET_GROUP_TIMEOUT15)) {
			val.ToULong(&temp2);
			m_starNet15GroupTimeout = (unsigned int)temp2;
		} else if (key.IsSameAs(KEY_STARNET_CALLSIGN_SWITCH15)) {
			val.ToLong(&temp1);
			m_starNet15CallsignSwitch = STARNET_CALLSIGN_SWITCH(temp1);
		} else if (key.IsSameAs(KEY_STARNET_TXMSG_SWITCH15)) {
			val.ToLong(&temp1);
			m_starNet15TxMsgSwitch = temp1 == 1L;
		} else if (key.IsSameAs(KEY_STARNET_REFLECTOR15)) {
			m_starNet15Reflector = val;
		} else if (key.IsSameAs(KEY_REMOTE_ENABLED)) {
			val.ToLong(&temp1);
			m_remoteEnabled = temp1 == 1L;
		} else if (key.IsSameAs(KEY_REMOTE_PASSWORD)) {
			m_remotePassword = val;
		} else if (key.IsSameAs(KEY_REMOTE_PORT)) {
			val.ToULong(&temp2);
			m_remotePort = (unsigned int)temp2;
		} else if (key.IsSameAs(KEY_LOG_ENABLED)) {
			val.ToLong(&temp1);
			m_logEnabled = temp1 == 1L;
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

CStarNetServerConfig::~CStarNetServerConfig()
{
}

#endif

void CStarNetServerConfig::getGateway(wxString& callsign, wxString& address) const
{
	callsign = m_callsign;
	address  = m_address;
}

void CStarNetServerConfig::setGateway(const wxString& callsign, const wxString& address)
{
	m_callsign = callsign;
	m_address  = address;
}

void CStarNetServerConfig::getIrcDDB(wxString& hostname, wxString& username, wxString& password) const
{
	hostname = m_ircddbHostname;
	username = m_ircddbUsername;
	password = m_ircddbPassword;
}

void CStarNetServerConfig::setIrcDDB(const wxString& hostname, const wxString& username, const wxString& password)
{
	m_ircddbHostname = hostname;
	m_ircddbUsername = username;
	m_ircddbPassword = password;
}

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
void CStarNetServerConfig::getStarNet1(wxString& band, wxString& callsign, wxString& logoff, wxString& info, wxString& permanent, unsigned int& userTimeout, unsigned int& groupTimeout, STARNET_CALLSIGN_SWITCH& callsignSwitch, bool& txMsgSwitch, wxString& reflector) const
#else
void CStarNetServerConfig::getStarNet1(wxString& band, wxString& callsign, wxString& logoff, wxString& info, wxString& permanent, unsigned int& userTimeout, unsigned int& groupTimeout, STARNET_CALLSIGN_SWITCH& callsignSwitch, bool& txMsgSwitch) const
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
void CStarNetServerConfig::setStarNet1(const wxString& band, const wxString& callsign, const wxString& logoff, const wxString& info, const wxString& permanent, unsigned int userTimeout, unsigned int groupTimeout, STARNET_CALLSIGN_SWITCH callsignSwitch, bool txMsgSwitch, const wxString& reflector)
#else
void CStarNetServerConfig::setStarNet1(const wxString& band, const wxString& callsign, const wxString& logoff, const wxString& info, const wxString& permanent, unsigned int userTimeout, unsigned int groupTimeout, STARNET_CALLSIGN_SWITCH callsignSwitch, bool txMsgSwitch)
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
void CStarNetServerConfig::getStarNet2(wxString& band, wxString& callsign, wxString& logoff, wxString& info, wxString& permanent, unsigned int& userTimeout, unsigned int& groupTimeout, STARNET_CALLSIGN_SWITCH& callsignSwitch, bool& txMsgSwitch, wxString& reflector) const
#else
void CStarNetServerConfig::getStarNet2(wxString& band, wxString& callsign, wxString& logoff, wxString& info, wxString& permanent, unsigned int& userTimeout, unsigned int& groupTimeout, STARNET_CALLSIGN_SWITCH& callsignSwitch, bool& txMsgSwitch) const
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
void CStarNetServerConfig::setStarNet2(const wxString& band, const wxString& callsign, const wxString& logoff, const wxString& info, const wxString& permanent, unsigned int userTimeout, unsigned int groupTimeout, STARNET_CALLSIGN_SWITCH callsignSwitch, bool txMsgSwitch, const wxString& reflector)
#else
void CStarNetServerConfig::setStarNet2(const wxString& band, const wxString& callsign, const wxString& logoff, const wxString& info, const wxString& permanent, unsigned int userTimeout, unsigned int groupTimeout, STARNET_CALLSIGN_SWITCH callsignSwitch, bool txMsgSwitch)
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
void CStarNetServerConfig::getStarNet3(wxString& band, wxString& callsign, wxString& logoff, wxString& info, wxString& permanent, unsigned int& userTimeout, unsigned int& groupTimeout, STARNET_CALLSIGN_SWITCH& callsignSwitch, bool& txMsgSwitch, wxString& reflector) const
#else
void CStarNetServerConfig::getStarNet3(wxString& band, wxString& callsign, wxString& logoff, wxString& info, wxString& permanent, unsigned int& userTimeout, unsigned int& groupTimeout, STARNET_CALLSIGN_SWITCH& callsignSwitch, bool& txMsgSwitch) const
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
void CStarNetServerConfig::setStarNet3(const wxString& band, const wxString& callsign, const wxString& logoff, const wxString& info, const wxString& permanent, unsigned int userTimeout, unsigned int groupTimeout, STARNET_CALLSIGN_SWITCH callsignSwitch, bool txMsgSwitch, const wxString& reflector)
#else
void CStarNetServerConfig::setStarNet3(const wxString& band, const wxString& callsign, const wxString& logoff, const wxString& info, const wxString& permanent, unsigned int userTimeout, unsigned int groupTimeout, STARNET_CALLSIGN_SWITCH callsignSwitch, bool txMsgSwitch)
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
void CStarNetServerConfig::getStarNet4(wxString& band, wxString& callsign, wxString& logoff, wxString& info, wxString& permanent, unsigned int& userTimeout, unsigned int& groupTimeout, STARNET_CALLSIGN_SWITCH& callsignSwitch, bool& txMsgSwitch, wxString& reflector) const
#else
void CStarNetServerConfig::getStarNet4(wxString& band, wxString& callsign, wxString& logoff, wxString& info, wxString& permanent, unsigned int& userTimeout, unsigned int& groupTimeout, STARNET_CALLSIGN_SWITCH& callsignSwitch, bool& txMsgSwitch) const
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
void CStarNetServerConfig::setStarNet4(const wxString& band, const wxString& callsign, const wxString& logoff, const wxString& info, const wxString& permanent, unsigned int userTimeout, unsigned int groupTimeout, STARNET_CALLSIGN_SWITCH callsignSwitch, bool txMsgSwitch, const wxString& reflector)
#else
void CStarNetServerConfig::setStarNet4(const wxString& band, const wxString& callsign, const wxString& logoff, const wxString& info, const wxString& permanent, unsigned int userTimeout, unsigned int groupTimeout, STARNET_CALLSIGN_SWITCH callsignSwitch, bool txMsgSwitch)
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
void CStarNetServerConfig::getStarNet5(wxString& band, wxString& callsign, wxString& logoff, wxString& info, wxString& permanent, unsigned int& userTimeout, unsigned int& groupTimeout, STARNET_CALLSIGN_SWITCH& callsignSwitch, bool& txMsgSwitch, wxString& reflector) const
#else
void CStarNetServerConfig::getStarNet5(wxString& band, wxString& callsign, wxString& logoff, wxString& info, wxString& permanent, unsigned int& userTimeout, unsigned int& groupTimeout, STARNET_CALLSIGN_SWITCH& callsignSwitch, bool& txMsgSwitch) const
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
void CStarNetServerConfig::setStarNet5(const wxString& band, const wxString& callsign, const wxString& logoff, const wxString& info, const wxString& permanent, unsigned int userTimeout, unsigned int groupTimeout, STARNET_CALLSIGN_SWITCH callsignSwitch, bool txMsgSwitch, const wxString& reflector)
#else
void CStarNetServerConfig::setStarNet5(const wxString& band, const wxString& callsign, const wxString& logoff, const wxString& info, const wxString& permanent, unsigned int userTimeout, unsigned int groupTimeout, STARNET_CALLSIGN_SWITCH callsignSwitch, bool txMsgSwitch)
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

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
void CStarNetServerConfig::getStarNet6(wxString& band, wxString& callsign, wxString& logoff, wxString& info, wxString& permanent, unsigned int& userTimeout, unsigned int& groupTimeout, STARNET_CALLSIGN_SWITCH& callsignSwitch, bool& txMsgSwitch, wxString& reflector) const
#else
void CStarNetServerConfig::getStarNet6(wxString& band, wxString& callsign, wxString& logoff, wxString& info, wxString& permanent, unsigned int& userTimeout, unsigned int& groupTimeout, STARNET_CALLSIGN_SWITCH& callsignSwitch, bool& txMsgSwitch) const
#endif
{
	band           = m_starNet6Band;
	callsign       = m_starNet6Callsign;
	logoff         = m_starNet6Logoff;
	info           = m_starNet6Info;
	permanent      = m_starNet6Permanent;
	userTimeout    = m_starNet6UserTimeout;
	groupTimeout   = m_starNet6GroupTimeout;
	callsignSwitch = m_starNet6CallsignSwitch;
	txMsgSwitch    = m_starNet6TxMsgSwitch;

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
	reflector      = m_starNet6Reflector;
#endif
}

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
void CStarNetServerConfig::setStarNet6(const wxString& band, const wxString& callsign, const wxString& logoff, const wxString& info, const wxString& permanent, unsigned int userTimeout, unsigned int groupTimeout, STARNET_CALLSIGN_SWITCH callsignSwitch, bool txMsgSwitch, const wxString& reflector)
#else
void CStarNetServerConfig::setStarNet6(const wxString& band, const wxString& callsign, const wxString& logoff, const wxString& info, const wxString& permanent, unsigned int userTimeout, unsigned int groupTimeout, STARNET_CALLSIGN_SWITCH callsignSwitch, bool txMsgSwitch)
#endif
{
	m_starNet6Band           = band;
	m_starNet6Callsign       = callsign;
	m_starNet6Logoff         = logoff;
	m_starNet6Info           = info;
	m_starNet6Permanent      = permanent;
	m_starNet6UserTimeout    = userTimeout;
	m_starNet6GroupTimeout   = groupTimeout;
	m_starNet6CallsignSwitch = callsignSwitch;
	m_starNet6TxMsgSwitch    = txMsgSwitch;

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
	m_starNet6Reflector      = reflector;
#endif
}

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
void CStarNetServerConfig::getStarNet7(wxString& band, wxString& callsign, wxString& logoff, wxString& info, wxString& permanent, unsigned int& userTimeout, unsigned int& groupTimeout, STARNET_CALLSIGN_SWITCH& callsignSwitch, bool& txMsgSwitch, wxString& reflector) const
#else
void CStarNetServerConfig::getStarNet7(wxString& band, wxString& callsign, wxString& logoff, wxString& info, wxString& permanent, unsigned int& userTimeout, unsigned int& groupTimeout, STARNET_CALLSIGN_SWITCH& callsignSwitch, bool& txMsgSwitch) const
#endif
{
	band           = m_starNet7Band;
	callsign       = m_starNet7Callsign;
	logoff         = m_starNet7Logoff;
	info           = m_starNet7Info;
	permanent      = m_starNet7Permanent;
	userTimeout    = m_starNet7UserTimeout;
	groupTimeout   = m_starNet7GroupTimeout;
	callsignSwitch = m_starNet7CallsignSwitch;
	txMsgSwitch    = m_starNet7TxMsgSwitch;

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
	reflector      = m_starNet7Reflector;
#endif
}

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
void CStarNetServerConfig::setStarNet7(const wxString& band, const wxString& callsign, const wxString& logoff, const wxString& info, const wxString& permanent, unsigned int userTimeout, unsigned int groupTimeout, STARNET_CALLSIGN_SWITCH callsignSwitch, bool txMsgSwitch, const wxString& reflector)
#else
void CStarNetServerConfig::setStarNet7(const wxString& band, const wxString& callsign, const wxString& logoff, const wxString& info, const wxString& permanent, unsigned int userTimeout, unsigned int groupTimeout, STARNET_CALLSIGN_SWITCH callsignSwitch, bool txMsgSwitch)
#endif
{
	m_starNet7Band           = band;
	m_starNet7Callsign       = callsign;
	m_starNet7Logoff         = logoff;
	m_starNet7Info           = info;
	m_starNet7Permanent      = permanent;
	m_starNet7UserTimeout    = userTimeout;
	m_starNet7GroupTimeout   = groupTimeout;
	m_starNet7CallsignSwitch = callsignSwitch;
	m_starNet7TxMsgSwitch    = txMsgSwitch;

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
	m_starNet7Reflector      = reflector;
#endif
}

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
void CStarNetServerConfig::getStarNet8(wxString& band, wxString& callsign, wxString& logoff, wxString& info, wxString& permanent, unsigned int& userTimeout, unsigned int& groupTimeout, STARNET_CALLSIGN_SWITCH& callsignSwitch, bool& txMsgSwitch, wxString& reflector) const
#else
void CStarNetServerConfig::getStarNet8(wxString& band, wxString& callsign, wxString& logoff, wxString& info, wxString& permanent, unsigned int& userTimeout, unsigned int& groupTimeout, STARNET_CALLSIGN_SWITCH& callsignSwitch, bool& txMsgSwitch) const
#endif
{
	band           = m_starNet8Band;
	callsign       = m_starNet8Callsign;
	logoff         = m_starNet8Logoff;
	info           = m_starNet8Info;
	permanent      = m_starNet8Permanent;
	userTimeout    = m_starNet8UserTimeout;
	groupTimeout   = m_starNet8GroupTimeout;
	callsignSwitch = m_starNet8CallsignSwitch;
	txMsgSwitch    = m_starNet8TxMsgSwitch;

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
	reflector      = m_starNet8Reflector;
#endif
}

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
void CStarNetServerConfig::setStarNet8(const wxString& band, const wxString& callsign, const wxString& logoff, const wxString& info, const wxString& permanent, unsigned int userTimeout, unsigned int groupTimeout, STARNET_CALLSIGN_SWITCH callsignSwitch, bool txMsgSwitch, const wxString& reflector)
#else
void CStarNetServerConfig::setStarNet8(const wxString& band, const wxString& callsign, const wxString& logoff, const wxString& info, const wxString& permanent, unsigned int userTimeout, unsigned int groupTimeout, STARNET_CALLSIGN_SWITCH callsignSwitch, bool txMsgSwitch)
#endif
{
	m_starNet8Band           = band;
	m_starNet8Callsign       = callsign;
	m_starNet8Logoff         = logoff;
	m_starNet8Info           = info;
	m_starNet8Permanent      = permanent;
	m_starNet8UserTimeout    = userTimeout;
	m_starNet8GroupTimeout   = groupTimeout;
	m_starNet8CallsignSwitch = callsignSwitch;
	m_starNet8TxMsgSwitch    = txMsgSwitch;

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
	m_starNet8Reflector      = reflector;
#endif
}

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
void CStarNetServerConfig::getStarNet9(wxString& band, wxString& callsign, wxString& logoff, wxString& info, wxString& permanent, unsigned int& userTimeout, unsigned int& groupTimeout, STARNET_CALLSIGN_SWITCH& callsignSwitch, bool& txMsgSwitch, wxString& reflector) const
#else
void CStarNetServerConfig::getStarNet9(wxString& band, wxString& callsign, wxString& logoff, wxString& info, wxString& permanent, unsigned int& userTimeout, unsigned int& groupTimeout, STARNET_CALLSIGN_SWITCH& callsignSwitch, bool& txMsgSwitch) const
#endif
{
	band           = m_starNet9Band;
	callsign       = m_starNet9Callsign;
	logoff         = m_starNet9Logoff;
	info           = m_starNet9Info;
	permanent      = m_starNet9Permanent;
	userTimeout    = m_starNet9UserTimeout;
	groupTimeout   = m_starNet9GroupTimeout;
	callsignSwitch = m_starNet9CallsignSwitch;
	txMsgSwitch    = m_starNet9TxMsgSwitch;

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
	reflector      = m_starNet9Reflector;
#endif
}

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
void CStarNetServerConfig::setStarNet9(const wxString& band, const wxString& callsign, const wxString& logoff, const wxString& info, const wxString& permanent, unsigned int userTimeout, unsigned int groupTimeout, STARNET_CALLSIGN_SWITCH callsignSwitch, bool txMsgSwitch, const wxString& reflector)
#else
void CStarNetServerConfig::setStarNet9(const wxString& band, const wxString& callsign, const wxString& logoff, const wxString& info, const wxString& permanent, unsigned int userTimeout, unsigned int groupTimeout, STARNET_CALLSIGN_SWITCH callsignSwitch, bool txMsgSwitch)
#endif
{
	m_starNet9Band           = band;
	m_starNet9Callsign       = callsign;
	m_starNet9Logoff         = logoff;
	m_starNet9Info           = info;
	m_starNet9Permanent      = permanent;
	m_starNet9UserTimeout    = userTimeout;
	m_starNet9GroupTimeout   = groupTimeout;
	m_starNet9CallsignSwitch = callsignSwitch;
	m_starNet9TxMsgSwitch    = txMsgSwitch;

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
	m_starNet9Reflector      = reflector;
#endif
}

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
void CStarNetServerConfig::getStarNet10(wxString& band, wxString& callsign, wxString& logoff, wxString& info, wxString& permanent, unsigned int& userTimeout, unsigned int& groupTimeout, STARNET_CALLSIGN_SWITCH& callsignSwitch, bool& txMsgSwitch, wxString& reflector) const
#else
void CStarNetServerConfig::getStarNet10(wxString& band, wxString& callsign, wxString& logoff, wxString& info, wxString& permanent, unsigned int& userTimeout, unsigned int& groupTimeout, STARNET_CALLSIGN_SWITCH& callsignSwitch, bool& txMsgSwitch) const
#endif
{
	band           = m_starNet10Band;
	callsign       = m_starNet10Callsign;
	logoff         = m_starNet10Logoff;
	info           = m_starNet10Info;
	permanent      = m_starNet10Permanent;
	userTimeout    = m_starNet10UserTimeout;
	groupTimeout   = m_starNet10GroupTimeout;
	callsignSwitch = m_starNet10CallsignSwitch;
	txMsgSwitch    = m_starNet10TxMsgSwitch;

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
	reflector      = m_starNet10Reflector;
#endif
}

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
void CStarNetServerConfig::setStarNet10(const wxString& band, const wxString& callsign, const wxString& logoff, const wxString& info, const wxString& permanent, unsigned int userTimeout, unsigned int groupTimeout, STARNET_CALLSIGN_SWITCH callsignSwitch, bool txMsgSwitch, const wxString& reflector)
#else
void CStarNetServerConfig::setStarNet10(const wxString& band, const wxString& callsign, const wxString& logoff, const wxString& info, const wxString& permanent, unsigned int userTimeout, unsigned int groupTimeout, STARNET_CALLSIGN_SWITCH callsignSwitch, bool txMsgSwitch)
#endif
{
	m_starNet10Band           = band;
	m_starNet10Callsign       = callsign;
	m_starNet10Logoff         = logoff;
	m_starNet10Info           = info;
	m_starNet10Permanent      = permanent;
	m_starNet10UserTimeout    = userTimeout;
	m_starNet10GroupTimeout   = groupTimeout;
	m_starNet10CallsignSwitch = callsignSwitch;
	m_starNet10TxMsgSwitch    = txMsgSwitch;

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
	m_starNet10Reflector      = reflector;
#endif
}

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
void CStarNetServerConfig::getStarNet11(wxString& band, wxString& callsign, wxString& logoff, wxString& info, wxString& permanent, unsigned int& userTimeout, unsigned int& groupTimeout, STARNET_CALLSIGN_SWITCH& callsignSwitch, bool& txMsgSwitch, wxString& reflector) const
#else
void CStarNetServerConfig::getStarNet11(wxString& band, wxString& callsign, wxString& logoff, wxString& info, wxString& permanent, unsigned int& userTimeout, unsigned int& groupTimeout, STARNET_CALLSIGN_SWITCH& callsignSwitch, bool& txMsgSwitch) const
#endif
{
	band           = m_starNet11Band;
	callsign       = m_starNet11Callsign;
	logoff         = m_starNet11Logoff;
	info           = m_starNet11Info;
	permanent      = m_starNet11Permanent;
	userTimeout    = m_starNet11UserTimeout;
	groupTimeout   = m_starNet11GroupTimeout;
	callsignSwitch = m_starNet11CallsignSwitch;
	txMsgSwitch    = m_starNet11TxMsgSwitch;

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
	reflector      = m_starNet11Reflector;
#endif
}

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
void CStarNetServerConfig::setStarNet11(const wxString& band, const wxString& callsign, const wxString& logoff, const wxString& info, const wxString& permanent, unsigned int userTimeout, unsigned int groupTimeout, STARNET_CALLSIGN_SWITCH callsignSwitch, bool txMsgSwitch, const wxString& reflector)
#else
void CStarNetServerConfig::setStarNet11(const wxString& band, const wxString& callsign, const wxString& logoff, const wxString& info, const wxString& permanent, unsigned int userTimeout, unsigned int groupTimeout, STARNET_CALLSIGN_SWITCH callsignSwitch, bool txMsgSwitch)
#endif
{
	m_starNet11Band           = band;
	m_starNet11Callsign       = callsign;
	m_starNet11Logoff         = logoff;
	m_starNet11Info           = info;
	m_starNet11Permanent      = permanent;
	m_starNet11UserTimeout    = userTimeout;
	m_starNet11GroupTimeout   = groupTimeout;
	m_starNet11CallsignSwitch = callsignSwitch;
	m_starNet11TxMsgSwitch    = txMsgSwitch;

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
	m_starNet11Reflector      = reflector;
#endif
}

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
void CStarNetServerConfig::getStarNet12(wxString& band, wxString& callsign, wxString& logoff, wxString& info, wxString& permanent, unsigned int& userTimeout, unsigned int& groupTimeout, STARNET_CALLSIGN_SWITCH& callsignSwitch, bool& txMsgSwitch, wxString& reflector) const
#else
void CStarNetServerConfig::getStarNet12(wxString& band, wxString& callsign, wxString& logoff, wxString& info, wxString& permanent, unsigned int& userTimeout, unsigned int& groupTimeout, STARNET_CALLSIGN_SWITCH& callsignSwitch, bool& txMsgSwitch) const
#endif
{
	band           = m_starNet12Band;
	callsign       = m_starNet12Callsign;
	logoff         = m_starNet12Logoff;
	info           = m_starNet12Info;
	permanent      = m_starNet12Permanent;
	userTimeout    = m_starNet12UserTimeout;
	groupTimeout   = m_starNet12GroupTimeout;
	callsignSwitch = m_starNet12CallsignSwitch;
	txMsgSwitch    = m_starNet12TxMsgSwitch;

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
	reflector      = m_starNet12Reflector;
#endif
}

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
void CStarNetServerConfig::setStarNet12(const wxString& band, const wxString& callsign, const wxString& logoff, const wxString& info, const wxString& permanent, unsigned int userTimeout, unsigned int groupTimeout, STARNET_CALLSIGN_SWITCH callsignSwitch, bool txMsgSwitch, const wxString& reflector)
#else
void CStarNetServerConfig::setStarNet12(const wxString& band, const wxString& callsign, const wxString& logoff, const wxString& info, const wxString& permanent, unsigned int userTimeout, unsigned int groupTimeout, STARNET_CALLSIGN_SWITCH callsignSwitch, bool txMsgSwitch)
#endif
{
	m_starNet12Band           = band;
	m_starNet12Callsign       = callsign;
	m_starNet12Logoff         = logoff;
	m_starNet12Info           = info;
	m_starNet12Permanent      = permanent;
	m_starNet12UserTimeout    = userTimeout;
	m_starNet12GroupTimeout   = groupTimeout;
	m_starNet12CallsignSwitch = callsignSwitch;
	m_starNet12TxMsgSwitch    = txMsgSwitch;

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
	m_starNet12Reflector      = reflector;
#endif
}

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
void CStarNetServerConfig::getStarNet13(wxString& band, wxString& callsign, wxString& logoff, wxString& info, wxString& permanent, unsigned int& userTimeout, unsigned int& groupTimeout, STARNET_CALLSIGN_SWITCH& callsignSwitch, bool& txMsgSwitch, wxString& reflector) const
#else
void CStarNetServerConfig::getStarNet13(wxString& band, wxString& callsign, wxString& logoff, wxString& info, wxString& permanent, unsigned int& userTimeout, unsigned int& groupTimeout, STARNET_CALLSIGN_SWITCH& callsignSwitch, bool& txMsgSwitch) const
#endif
{
	band           = m_starNet13Band;
	callsign       = m_starNet13Callsign;
	logoff         = m_starNet13Logoff;
	info           = m_starNet13Info;
	permanent      = m_starNet13Permanent;
	userTimeout    = m_starNet13UserTimeout;
	groupTimeout   = m_starNet13GroupTimeout;
	callsignSwitch = m_starNet13CallsignSwitch;
	txMsgSwitch    = m_starNet13TxMsgSwitch;

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
	reflector      = m_starNet13Reflector;
#endif
}

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
void CStarNetServerConfig::setStarNet13(const wxString& band, const wxString& callsign, const wxString& logoff, const wxString& info, const wxString& permanent, unsigned int userTimeout, unsigned int groupTimeout, STARNET_CALLSIGN_SWITCH callsignSwitch, bool txMsgSwitch, const wxString& reflector)
#else
void CStarNetServerConfig::setStarNet13(const wxString& band, const wxString& callsign, const wxString& logoff, const wxString& info, const wxString& permanent, unsigned int userTimeout, unsigned int groupTimeout, STARNET_CALLSIGN_SWITCH callsignSwitch, bool txMsgSwitch)
#endif
{
	m_starNet13Band           = band;
	m_starNet13Callsign       = callsign;
	m_starNet13Logoff         = logoff;
	m_starNet13Info           = info;
	m_starNet13Permanent      = permanent;
	m_starNet13UserTimeout    = userTimeout;
	m_starNet13GroupTimeout   = groupTimeout;
	m_starNet13CallsignSwitch = callsignSwitch;
	m_starNet13TxMsgSwitch    = txMsgSwitch;

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
	m_starNet13Reflector      = reflector;
#endif
}

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
void CStarNetServerConfig::getStarNet14(wxString& band, wxString& callsign, wxString& logoff, wxString& info, wxString& permanent, unsigned int& userTimeout, unsigned int& groupTimeout, STARNET_CALLSIGN_SWITCH& callsignSwitch, bool& txMsgSwitch, wxString& reflector) const
#else
void CStarNetServerConfig::getStarNet14(wxString& band, wxString& callsign, wxString& logoff, wxString& info, wxString& permanent, unsigned int& userTimeout, unsigned int& groupTimeout, STARNET_CALLSIGN_SWITCH& callsignSwitch, bool& txMsgSwitch) const
#endif
{
	band           = m_starNet14Band;
	callsign       = m_starNet14Callsign;
	logoff         = m_starNet14Logoff;
	info           = m_starNet14Info;
	permanent      = m_starNet14Permanent;
	userTimeout    = m_starNet14UserTimeout;
	groupTimeout   = m_starNet14GroupTimeout;
	callsignSwitch = m_starNet14CallsignSwitch;
	txMsgSwitch    = m_starNet14TxMsgSwitch;

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
	reflector      = m_starNet14Reflector;
#endif
}

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
void CStarNetServerConfig::setStarNet14(const wxString& band, const wxString& callsign, const wxString& logoff, const wxString& info, const wxString& permanent, unsigned int userTimeout, unsigned int groupTimeout, STARNET_CALLSIGN_SWITCH callsignSwitch, bool txMsgSwitch, const wxString& reflector)
#else
void CStarNetServerConfig::setStarNet14(const wxString& band, const wxString& callsign, const wxString& logoff, const wxString& info, const wxString& permanent, unsigned int userTimeout, unsigned int groupTimeout, STARNET_CALLSIGN_SWITCH callsignSwitch, bool txMsgSwitch)
#endif
{
	m_starNet14Band           = band;
	m_starNet14Callsign       = callsign;
	m_starNet14Logoff         = logoff;
	m_starNet14Info           = info;
	m_starNet14Permanent      = permanent;
	m_starNet14UserTimeout    = userTimeout;
	m_starNet14GroupTimeout   = groupTimeout;
	m_starNet14CallsignSwitch = callsignSwitch;
	m_starNet14TxMsgSwitch    = txMsgSwitch;

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
	m_starNet14Reflector      = reflector;
#endif
}

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
void CStarNetServerConfig::getStarNet15(wxString& band, wxString& callsign, wxString& logoff, wxString& info, wxString& permanent, unsigned int& userTimeout, unsigned int& groupTimeout, STARNET_CALLSIGN_SWITCH& callsignSwitch, bool& txMsgSwitch, wxString& reflector) const
#else
void CStarNetServerConfig::getStarNet15(wxString& band, wxString& callsign, wxString& logoff, wxString& info, wxString& permanent, unsigned int& userTimeout, unsigned int& groupTimeout, STARNET_CALLSIGN_SWITCH& callsignSwitch, bool& txMsgSwitch) const
#endif
{
	band           = m_starNet15Band;
	callsign       = m_starNet15Callsign;
	logoff         = m_starNet15Logoff;
	info           = m_starNet15Info;
	permanent      = m_starNet15Permanent;
	userTimeout    = m_starNet15UserTimeout;
	groupTimeout   = m_starNet15GroupTimeout;
	callsignSwitch = m_starNet15CallsignSwitch;
	txMsgSwitch    = m_starNet15TxMsgSwitch;

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
	reflector      = m_starNet15Reflector;
#endif
}

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
void CStarNetServerConfig::setStarNet15(const wxString& band, const wxString& callsign, const wxString& logoff, const wxString& info, const wxString& permanent, unsigned int userTimeout, unsigned int groupTimeout, STARNET_CALLSIGN_SWITCH callsignSwitch, bool txMsgSwitch, const wxString& reflector)
#else
void CStarNetServerConfig::setStarNet15(const wxString& band, const wxString& callsign, const wxString& logoff, const wxString& info, const wxString& permanent, unsigned int userTimeout, unsigned int groupTimeout, STARNET_CALLSIGN_SWITCH callsignSwitch, bool txMsgSwitch)
#endif
{
	m_starNet15Band           = band;
	m_starNet15Callsign       = callsign;
	m_starNet15Logoff         = logoff;
	m_starNet15Info           = info;
	m_starNet15Permanent      = permanent;
	m_starNet15UserTimeout    = userTimeout;
	m_starNet15GroupTimeout   = groupTimeout;
	m_starNet15CallsignSwitch = callsignSwitch;
	m_starNet15TxMsgSwitch    = txMsgSwitch;

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
	m_starNet15Reflector      = reflector;
#endif
}

void CStarNetServerConfig::getRemote(bool& enabled, wxString& password, unsigned int& port) const
{
	enabled  = m_remoteEnabled;
	password = m_remotePassword;
	port     = m_remotePort;
}

void CStarNetServerConfig::setRemote(bool enabled, const wxString& password, unsigned int port)
{
	m_remoteEnabled  = enabled;
	m_remotePassword = password;
	m_remotePort     = port;
}

void CStarNetServerConfig::getMiscellaneous(bool& enabled) const
{
	enabled = m_logEnabled;
}

void CStarNetServerConfig::setMiscellaneous(bool enabled)
{
	m_logEnabled = enabled;
}

void CStarNetServerConfig::getPosition(int& x, int& y) const
{
	x = m_x;
	y = m_y;
}

void CStarNetServerConfig::setPosition(int x, int y)
{
	m_x = x;
	m_y = y;
}

bool CStarNetServerConfig::write()
{
#if defined(__WINDOWS__)
	m_config->Write(wxT("/") + KEY_CALLSIGN, m_callsign);
	m_config->Write(wxT("/") + KEY_ADDRESS, m_address);
	m_config->Write(wxT("/") + KEY_IRCDDB_HOSTNAME, m_ircddbHostname);
	m_config->Write(wxT("/") + KEY_IRCDDB_USERNAME, m_ircddbUsername);
	m_config->Write(wxT("/") + KEY_IRCDDB_PASSWORD, m_ircddbPassword);
	m_config->Write(wxT("/") + KEY_STARNET_BAND1, m_starNet1Band);
	m_config->Write(wxT("/") + KEY_STARNET_CALLSIGN1, m_starNet1Callsign);
	m_config->Write(wxT("/") + KEY_STARNET_LOGOFF1, m_starNet1Logoff);
	m_config->Write(wxT("/") + KEY_STARNET_INFO1, m_starNet1Info);
	m_config->Write(wxT("/") + KEY_STARNET_PERMANENT1, m_starNet1Permanent);
	m_config->Write(wxT("/") + KEY_STARNET_USER_TIMEOUT1, long(m_starNet1UserTimeout));
	m_config->Write(wxT("/") + KEY_STARNET_GROUP_TIMEOUT1, long(m_starNet1GroupTimeout));
	m_config->Write(wxT("/") + KEY_STARNET_CALLSIGN_SWITCH1, long(m_starNet1CallsignSwitch));
	m_config->Write(wxT("/") + KEY_STARNET_TXMSG_SWITCH1, m_starNet1TxMsgSwitch);
	m_config->Write(wxT("/") + KEY_STARNET_REFLECTOR1, m_starNet1Reflector);
	m_config->Write(wxT("/") + KEY_STARNET_BAND2, m_starNet2Band);
	m_config->Write(wxT("/") + KEY_STARNET_CALLSIGN2, m_starNet2Callsign);
	m_config->Write(wxT("/") + KEY_STARNET_LOGOFF2, m_starNet2Logoff);
	m_config->Write(wxT("/") + KEY_STARNET_INFO2, m_starNet2Info);
	m_config->Write(wxT("/") + KEY_STARNET_PERMANENT2, m_starNet2Permanent);
	m_config->Write(wxT("/") + KEY_STARNET_USER_TIMEOUT2, long(m_starNet2UserTimeout));
	m_config->Write(wxT("/") + KEY_STARNET_GROUP_TIMEOUT2, long(m_starNet2GroupTimeout));
	m_config->Write(wxT("/") + KEY_STARNET_CALLSIGN_SWITCH2, long(m_starNet2CallsignSwitch));
	m_config->Write(wxT("/") + KEY_STARNET_TXMSG_SWITCH2, m_starNet2TxMsgSwitch);
	m_config->Write(wxT("/") + KEY_STARNET_REFLECTOR2, m_starNet2Reflector);
	m_config->Write(wxT("/") + KEY_STARNET_BAND3, m_starNet3Band);
	m_config->Write(wxT("/") + KEY_STARNET_CALLSIGN3, m_starNet3Callsign);
	m_config->Write(wxT("/") + KEY_STARNET_LOGOFF3, m_starNet3Logoff);
	m_config->Write(wxT("/") + KEY_STARNET_INFO3, m_starNet3Info);
	m_config->Write(wxT("/") + KEY_STARNET_PERMANENT3, m_starNet3Permanent);
	m_config->Write(wxT("/") + KEY_STARNET_USER_TIMEOUT3, long(m_starNet3UserTimeout));
	m_config->Write(wxT("/") + KEY_STARNET_GROUP_TIMEOUT3, long(m_starNet3GroupTimeout));
	m_config->Write(wxT("/") + KEY_STARNET_CALLSIGN_SWITCH3, long(m_starNet3CallsignSwitch));
	m_config->Write(wxT("/") + KEY_STARNET_TXMSG_SWITCH3, m_starNet3TxMsgSwitch);
	m_config->Write(wxT("/") + KEY_STARNET_REFLECTOR3, m_starNet3Reflector);
	m_config->Write(wxT("/") + KEY_STARNET_BAND4, m_starNet4Band);
	m_config->Write(wxT("/") + KEY_STARNET_CALLSIGN4, m_starNet4Callsign);
	m_config->Write(wxT("/") + KEY_STARNET_LOGOFF4, m_starNet4Logoff);
	m_config->Write(wxT("/") + KEY_STARNET_INFO4, m_starNet4Info);
	m_config->Write(wxT("/") + KEY_STARNET_PERMANENT4, m_starNet4Permanent);
	m_config->Write(wxT("/") + KEY_STARNET_USER_TIMEOUT4, long(m_starNet4UserTimeout));
	m_config->Write(wxT("/") + KEY_STARNET_GROUP_TIMEOUT4, long(m_starNet4GroupTimeout));
	m_config->Write(wxT("/") + KEY_STARNET_CALLSIGN_SWITCH4, long(m_starNet4CallsignSwitch));
	m_config->Write(wxT("/") + KEY_STARNET_TXMSG_SWITCH4, m_starNet4TxMsgSwitch);
	m_config->Write(wxT("/") + KEY_STARNET_REFLECTOR4, m_starNet4Reflector);
	m_config->Write(wxT("/") + KEY_STARNET_BAND5, m_starNet5Band);
	m_config->Write(wxT("/") + KEY_STARNET_CALLSIGN5, m_starNet5Callsign);
	m_config->Write(wxT("/") + KEY_STARNET_LOGOFF5, m_starNet5Logoff);
	m_config->Write(wxT("/") + KEY_STARNET_INFO5, m_starNet5Info);
	m_config->Write(wxT("/") + KEY_STARNET_PERMANENT5, m_starNet5Permanent);
	m_config->Write(wxT("/") + KEY_STARNET_USER_TIMEOUT5, long(m_starNet5UserTimeout));
	m_config->Write(wxT("/") + KEY_STARNET_GROUP_TIMEOUT5, long(m_starNet5GroupTimeout));
	m_config->Write(wxT("/") + KEY_STARNET_CALLSIGN_SWITCH5, long(m_starNet5CallsignSwitch));
	m_config->Write(wxT("/") + KEY_STARNET_TXMSG_SWITCH5, m_starNet5TxMsgSwitch);
	m_config->Write(wxT("/") + KEY_STARNET_REFLECTOR5, m_starNet5Reflector);
	m_config->Write(wxT("/") + KEY_STARNET_BAND6, m_starNet6Band);
	m_config->Write(wxT("/") + KEY_STARNET_CALLSIGN6, m_starNet6Callsign);
	m_config->Write(wxT("/") + KEY_STARNET_LOGOFF6, m_starNet6Logoff);
	m_config->Write(wxT("/") + KEY_STARNET_INFO6, m_starNet6Info);
	m_config->Write(wxT("/") + KEY_STARNET_PERMANENT6, m_starNet6Permanent);
	m_config->Write(wxT("/") + KEY_STARNET_USER_TIMEOUT6, long(m_starNet6UserTimeout));
	m_config->Write(wxT("/") + KEY_STARNET_GROUP_TIMEOUT6, long(m_starNet6GroupTimeout));
	m_config->Write(wxT("/") + KEY_STARNET_CALLSIGN_SWITCH6, long(m_starNet6CallsignSwitch));
	m_config->Write(wxT("/") + KEY_STARNET_TXMSG_SWITCH6, m_starNet6TxMsgSwitch);
	m_config->Write(wxT("/") + KEY_STARNET_REFLECTOR6, m_starNet6Reflector);
	m_config->Write(wxT("/") + KEY_STARNET_BAND7, m_starNet7Band);
	m_config->Write(wxT("/") + KEY_STARNET_CALLSIGN7, m_starNet7Callsign);
	m_config->Write(wxT("/") + KEY_STARNET_LOGOFF7, m_starNet7Logoff);
	m_config->Write(wxT("/") + KEY_STARNET_INFO7, m_starNet7Info);
	m_config->Write(wxT("/") + KEY_STARNET_PERMANENT7, m_starNet7Permanent);
	m_config->Write(wxT("/") + KEY_STARNET_USER_TIMEOUT7, long(m_starNet7UserTimeout));
	m_config->Write(wxT("/") + KEY_STARNET_GROUP_TIMEOUT7, long(m_starNet7GroupTimeout));
	m_config->Write(wxT("/") + KEY_STARNET_CALLSIGN_SWITCH7, long(m_starNet7CallsignSwitch));
	m_config->Write(wxT("/") + KEY_STARNET_TXMSG_SWITCH7, m_starNet7TxMsgSwitch);
	m_config->Write(wxT("/") + KEY_STARNET_REFLECTOR7, m_starNet7Reflector);
	m_config->Write(wxT("/") + KEY_STARNET_BAND8, m_starNet8Band);
	m_config->Write(wxT("/") + KEY_STARNET_CALLSIGN8, m_starNet8Callsign);
	m_config->Write(wxT("/") + KEY_STARNET_LOGOFF8, m_starNet8Logoff);
	m_config->Write(wxT("/") + KEY_STARNET_INFO8, m_starNet8Info);
	m_config->Write(wxT("/") + KEY_STARNET_PERMANENT8, m_starNet8Permanent);
	m_config->Write(wxT("/") + KEY_STARNET_USER_TIMEOUT8, long(m_starNet8UserTimeout));
	m_config->Write(wxT("/") + KEY_STARNET_GROUP_TIMEOUT8, long(m_starNet8GroupTimeout));
	m_config->Write(wxT("/") + KEY_STARNET_CALLSIGN_SWITCH8, long(m_starNet8CallsignSwitch));
	m_config->Write(wxT("/") + KEY_STARNET_TXMSG_SWITCH8, m_starNet8TxMsgSwitch);
	m_config->Write(wxT("/") + KEY_STARNET_REFLECTOR8, m_starNet8Reflector);
	m_config->Write(wxT("/") + KEY_STARNET_BAND9, m_starNet9Band);
	m_config->Write(wxT("/") + KEY_STARNET_CALLSIGN9, m_starNet9Callsign);
	m_config->Write(wxT("/") + KEY_STARNET_LOGOFF9, m_starNet9Logoff);
	m_config->Write(wxT("/") + KEY_STARNET_INFO9, m_starNet9Info);
	m_config->Write(wxT("/") + KEY_STARNET_PERMANENT9, m_starNet9Permanent);
	m_config->Write(wxT("/") + KEY_STARNET_USER_TIMEOUT9, long(m_starNet9UserTimeout));
	m_config->Write(wxT("/") + KEY_STARNET_GROUP_TIMEOUT9, long(m_starNet9GroupTimeout));
	m_config->Write(wxT("/") + KEY_STARNET_CALLSIGN_SWITCH9, long(m_starNet9CallsignSwitch));
	m_config->Write(wxT("/") + KEY_STARNET_TXMSG_SWITCH9, m_starNet9TxMsgSwitch);
	m_config->Write(wxT("/") + KEY_STARNET_REFLECTOR9, m_starNet9Reflector);
	m_config->Write(wxT("/") + KEY_STARNET_BAND10, m_starNet10Band);
	m_config->Write(wxT("/") + KEY_STARNET_CALLSIGN10, m_starNet10Callsign);
	m_config->Write(wxT("/") + KEY_STARNET_LOGOFF10, m_starNet10Logoff);
	m_config->Write(wxT("/") + KEY_STARNET_INFO10, m_starNet10Info);
	m_config->Write(wxT("/") + KEY_STARNET_PERMANENT10, m_starNet10Permanent);
	m_config->Write(wxT("/") + KEY_STARNET_USER_TIMEOUT10, long(m_starNet10UserTimeout));
	m_config->Write(wxT("/") + KEY_STARNET_GROUP_TIMEOUT10, long(m_starNet10GroupTimeout));
	m_config->Write(wxT("/") + KEY_STARNET_CALLSIGN_SWITCH10, long(m_starNet10CallsignSwitch));
	m_config->Write(wxT("/") + KEY_STARNET_TXMSG_SWITCH10, m_starNet10TxMsgSwitch);
	m_config->Write(wxT("/") + KEY_STARNET_REFLECTOR10, m_starNet10Reflector);
	m_config->Write(wxT("/") + KEY_STARNET_BAND11, m_starNet11Band);
	m_config->Write(wxT("/") + KEY_STARNET_CALLSIGN11, m_starNet11Callsign);
	m_config->Write(wxT("/") + KEY_STARNET_LOGOFF11, m_starNet11Logoff);
	m_config->Write(wxT("/") + KEY_STARNET_INFO11, m_starNet11Info);
	m_config->Write(wxT("/") + KEY_STARNET_PERMANENT11, m_starNet11Permanent);
	m_config->Write(wxT("/") + KEY_STARNET_USER_TIMEOUT11, long(m_starNet11UserTimeout));
	m_config->Write(wxT("/") + KEY_STARNET_GROUP_TIMEOUT11, long(m_starNet11GroupTimeout));
	m_config->Write(wxT("/") + KEY_STARNET_CALLSIGN_SWITCH11, long(m_starNet11CallsignSwitch));
	m_config->Write(wxT("/") + KEY_STARNET_TXMSG_SWITCH11, m_starNet11TxMsgSwitch);
	m_config->Write(wxT("/") + KEY_STARNET_REFLECTOR11, m_starNet11Reflector);
	m_config->Write(wxT("/") + KEY_STARNET_BAND12, m_starNet12Band);
	m_config->Write(wxT("/") + KEY_STARNET_CALLSIGN12, m_starNet12Callsign);
	m_config->Write(wxT("/") + KEY_STARNET_LOGOFF12, m_starNet12Logoff);
	m_config->Write(wxT("/") + KEY_STARNET_INFO12, m_starNet12Info);
	m_config->Write(wxT("/") + KEY_STARNET_PERMANENT12, m_starNet12Permanent);
	m_config->Write(wxT("/") + KEY_STARNET_USER_TIMEOUT12, long(m_starNet12UserTimeout));
	m_config->Write(wxT("/") + KEY_STARNET_GROUP_TIMEOUT12, long(m_starNet12GroupTimeout));
	m_config->Write(wxT("/") + KEY_STARNET_CALLSIGN_SWITCH12, long(m_starNet12CallsignSwitch));
	m_config->Write(wxT("/") + KEY_STARNET_TXMSG_SWITCH12, m_starNet12TxMsgSwitch);
	m_config->Write(wxT("/") + KEY_STARNET_REFLECTOR12, m_starNet12Reflector);
	m_config->Write(wxT("/") + KEY_STARNET_BAND13, m_starNet13Band);
	m_config->Write(wxT("/") + KEY_STARNET_CALLSIGN13, m_starNet13Callsign);
	m_config->Write(wxT("/") + KEY_STARNET_LOGOFF13, m_starNet13Logoff);
	m_config->Write(wxT("/") + KEY_STARNET_INFO13, m_starNet13Info);
	m_config->Write(wxT("/") + KEY_STARNET_PERMANENT13, m_starNet13Permanent);
	m_config->Write(wxT("/") + KEY_STARNET_USER_TIMEOUT13, long(m_starNet13UserTimeout));
	m_config->Write(wxT("/") + KEY_STARNET_GROUP_TIMEOUT13, long(m_starNet13GroupTimeout));
	m_config->Write(wxT("/") + KEY_STARNET_CALLSIGN_SWITCH13, long(m_starNet13CallsignSwitch));
	m_config->Write(wxT("/") + KEY_STARNET_TXMSG_SWITCH13, m_starNet13TxMsgSwitch);
	m_config->Write(wxT("/") + KEY_STARNET_REFLECTOR13, m_starNet13Reflector);
	m_config->Write(wxT("/") + KEY_STARNET_BAND14, m_starNet14Band);
	m_config->Write(wxT("/") + KEY_STARNET_CALLSIGN14, m_starNet14Callsign);
	m_config->Write(wxT("/") + KEY_STARNET_LOGOFF14, m_starNet14Logoff);
	m_config->Write(wxT("/") + KEY_STARNET_INFO14, m_starNet14Info);
	m_config->Write(wxT("/") + KEY_STARNET_PERMANENT14, m_starNet14Permanent);
	m_config->Write(wxT("/") + KEY_STARNET_USER_TIMEOUT14, long(m_starNet14UserTimeout));
	m_config->Write(wxT("/") + KEY_STARNET_GROUP_TIMEOUT14, long(m_starNet14GroupTimeout));
	m_config->Write(wxT("/") + KEY_STARNET_CALLSIGN_SWITCH14, long(m_starNet14CallsignSwitch));
	m_config->Write(wxT("/") + KEY_STARNET_TXMSG_SWITCH14, m_starNet14TxMsgSwitch);
	m_config->Write(wxT("/") + KEY_STARNET_REFLECTOR14, m_starNet14Reflector);
	m_config->Write(wxT("/") + KEY_STARNET_BAND15, m_starNet15Band);
	m_config->Write(wxT("/") + KEY_STARNET_CALLSIGN15, m_starNet15Callsign);
	m_config->Write(wxT("/") + KEY_STARNET_LOGOFF15, m_starNet15Logoff);
	m_config->Write(wxT("/") + KEY_STARNET_INFO15, m_starNet15Info);
	m_config->Write(wxT("/") + KEY_STARNET_PERMANENT15, m_starNet15Permanent);
	m_config->Write(wxT("/") + KEY_STARNET_USER_TIMEOUT15, long(m_starNet15UserTimeout));
	m_config->Write(wxT("/") + KEY_STARNET_GROUP_TIMEOUT15, long(m_starNet15GroupTimeout));
	m_config->Write(wxT("/") + KEY_STARNET_CALLSIGN_SWITCH15, long(m_starNet15CallsignSwitch));
	m_config->Write(wxT("/") + KEY_STARNET_TXMSG_SWITCH15, m_starNet15TxMsgSwitch);
	m_config->Write(wxT("/") + KEY_STARNET_REFLECTOR15, m_starNet15Reflector);
	m_config->Write(wxT("/") + KEY_REMOTE_ENABLED, m_remoteEnabled);
	m_config->Write(wxT("/") + KEY_REMOTE_PASSWORD, m_remotePassword);
	m_config->Write(wxT("/") + KEY_REMOTE_PORT, long(m_remotePort));
	m_config->Write(wxT("/") + KEY_LOG_ENABLED, m_logEnabled);
	m_config->Write(wxT("/") + KEY_WINDOW_X, long(m_x));
	m_config->Write(wxT("/") + KEY_WINDOW_Y, long(m_y));
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
	buffer.Printf(wxT("%s=%s"), KEY_CALLSIGN.c_str(), m_callsign.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_ADDRESS.c_str(), m_address.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_IRCDDB_HOSTNAME.c_str(), m_ircddbHostname.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_IRCDDB_USERNAME.c_str(), m_ircddbUsername.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_IRCDDB_PASSWORD.c_str(), m_ircddbPassword.c_str()); file.AddLine(buffer);
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
	buffer.Printf(wxT("%s=%s"), KEY_STARNET_BAND6.c_str(), m_starNet6Band.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_STARNET_CALLSIGN6.c_str(), m_starNet6Callsign.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_STARNET_LOGOFF6.c_str(), m_starNet6Logoff.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_STARNET_INFO6.c_str(), m_starNet6Info.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_STARNET_PERMANENT6.c_str(), m_starNet6Permanent.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%u"), KEY_STARNET_USER_TIMEOUT6.c_str(), m_starNet6UserTimeout); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%u"), KEY_STARNET_GROUP_TIMEOUT6.c_str(), m_starNet6GroupTimeout); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"), KEY_STARNET_CALLSIGN_SWITCH6.c_str(), int(m_starNet6CallsignSwitch)); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"), KEY_STARNET_TXMSG_SWITCH6.c_str(), m_starNet6TxMsgSwitch ? 1 : 0); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_STARNET_REFLECTOR6.c_str(), m_starNet6Reflector.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_STARNET_BAND7.c_str(), m_starNet7Band.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_STARNET_CALLSIGN7.c_str(), m_starNet7Callsign.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_STARNET_LOGOFF7.c_str(), m_starNet7Logoff.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_STARNET_INFO7.c_str(), m_starNet7Info.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_STARNET_PERMANENT7.c_str(), m_starNet7Permanent.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%u"), KEY_STARNET_USER_TIMEOUT7.c_str(), m_starNet7UserTimeout); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%u"), KEY_STARNET_GROUP_TIMEOUT7.c_str(), m_starNet7GroupTimeout); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"), KEY_STARNET_CALLSIGN_SWITCH7.c_str(), int(m_starNet7CallsignSwitch)); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"), KEY_STARNET_TXMSG_SWITCH7.c_str(), m_starNet7TxMsgSwitch ? 1 : 0); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_STARNET_REFLECTOR7.c_str(), m_starNet7Reflector.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_STARNET_BAND8.c_str(), m_starNet8Band.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_STARNET_CALLSIGN8.c_str(), m_starNet8Callsign.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_STARNET_LOGOFF8.c_str(), m_starNet8Logoff.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_STARNET_INFO8.c_str(), m_starNet8Info.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_STARNET_PERMANENT8.c_str(), m_starNet8Permanent.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%u"), KEY_STARNET_USER_TIMEOUT8.c_str(), m_starNet8UserTimeout); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%u"), KEY_STARNET_GROUP_TIMEOUT8.c_str(), m_starNet8GroupTimeout); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"), KEY_STARNET_CALLSIGN_SWITCH8.c_str(), int(m_starNet8CallsignSwitch)); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"), KEY_STARNET_TXMSG_SWITCH8.c_str(), m_starNet8TxMsgSwitch ? 1 : 0); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_STARNET_REFLECTOR8.c_str(), m_starNet8Reflector.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_STARNET_BAND9.c_str(), m_starNet9Band.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_STARNET_CALLSIGN9.c_str(), m_starNet9Callsign.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_STARNET_LOGOFF9.c_str(), m_starNet9Logoff.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_STARNET_INFO9.c_str(), m_starNet9Info.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_STARNET_PERMANENT9.c_str(), m_starNet9Permanent.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%u"), KEY_STARNET_USER_TIMEOUT9.c_str(), m_starNet9UserTimeout); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%u"), KEY_STARNET_GROUP_TIMEOUT9.c_str(), m_starNet9GroupTimeout); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"), KEY_STARNET_CALLSIGN_SWITCH9.c_str(), int(m_starNet9CallsignSwitch)); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"), KEY_STARNET_TXMSG_SWITCH9.c_str(), m_starNet9TxMsgSwitch ? 1 : 0); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_STARNET_REFLECTOR9.c_str(), m_starNet9Reflector.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_STARNET_BAND10.c_str(), m_starNet10Band.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_STARNET_CALLSIGN10.c_str(), m_starNet10Callsign.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_STARNET_LOGOFF10.c_str(), m_starNet10Logoff.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_STARNET_INFO10.c_str(), m_starNet10Info.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_STARNET_PERMANENT10.c_str(), m_starNet10Permanent.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%u"), KEY_STARNET_USER_TIMEOUT10.c_str(), m_starNet10UserTimeout); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%u"), KEY_STARNET_GROUP_TIMEOUT10.c_str(), m_starNet10GroupTimeout); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"), KEY_STARNET_CALLSIGN_SWITCH10.c_str(), int(m_starNet10CallsignSwitch)); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"), KEY_STARNET_TXMSG_SWITCH10.c_str(), m_starNet10TxMsgSwitch ? 1 : 0); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_STARNET_REFLECTOR10.c_str(), m_starNet10Reflector.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_STARNET_BAND11.c_str(), m_starNet11Band.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_STARNET_CALLSIGN11.c_str(), m_starNet11Callsign.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_STARNET_LOGOFF11.c_str(), m_starNet11Logoff.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_STARNET_INFO11.c_str(), m_starNet11Info.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_STARNET_PERMANENT11.c_str(), m_starNet11Permanent.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%u"), KEY_STARNET_USER_TIMEOUT11.c_str(), m_starNet11UserTimeout); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%u"), KEY_STARNET_GROUP_TIMEOUT11.c_str(), m_starNet11GroupTimeout); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"), KEY_STARNET_CALLSIGN_SWITCH11.c_str(), int(m_starNet11CallsignSwitch)); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"), KEY_STARNET_TXMSG_SWITCH11.c_str(), m_starNet11TxMsgSwitch ? 1 : 0); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_STARNET_REFLECTOR11.c_str(), m_starNet11Reflector.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_STARNET_BAND12.c_str(), m_starNet12Band.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_STARNET_CALLSIGN12.c_str(), m_starNet12Callsign.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_STARNET_LOGOFF12.c_str(), m_starNet12Logoff.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_STARNET_INFO12.c_str(), m_starNet12Info.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_STARNET_PERMANENT12.c_str(), m_starNet12Permanent.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%u"), KEY_STARNET_USER_TIMEOUT12.c_str(), m_starNet12UserTimeout); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%u"), KEY_STARNET_GROUP_TIMEOUT12.c_str(), m_starNet12GroupTimeout); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"), KEY_STARNET_CALLSIGN_SWITCH12.c_str(), int(m_starNet12CallsignSwitch)); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"), KEY_STARNET_TXMSG_SWITCH12.c_str(), m_starNet12TxMsgSwitch ? 1 : 0); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_STARNET_REFLECTOR12.c_str(), m_starNet12Reflector.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_STARNET_BAND13.c_str(), m_starNet13Band.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_STARNET_CALLSIGN13.c_str(), m_starNet13Callsign.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_STARNET_LOGOFF13.c_str(), m_starNet13Logoff.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_STARNET_INFO13.c_str(), m_starNet13Info.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_STARNET_PERMANENT13.c_str(), m_starNet13Permanent.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%u"), KEY_STARNET_USER_TIMEOUT13.c_str(), m_starNet13UserTimeout); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%u"), KEY_STARNET_GROUP_TIMEOUT13.c_str(), m_starNet13GroupTimeout); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"), KEY_STARNET_CALLSIGN_SWITCH13.c_str(), int(m_starNet13CallsignSwitch)); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"), KEY_STARNET_TXMSG_SWITCH13.c_str(), m_starNet13TxMsgSwitch ? 1 : 0); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_STARNET_REFLECTOR13.c_str(), m_starNet13Reflector.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_STARNET_BAND14.c_str(), m_starNet14Band.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_STARNET_CALLSIGN14.c_str(), m_starNet14Callsign.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_STARNET_LOGOFF14.c_str(), m_starNet14Logoff.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_STARNET_INFO14.c_str(), m_starNet14Info.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_STARNET_PERMANENT14.c_str(), m_starNet14Permanent.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%u"), KEY_STARNET_USER_TIMEOUT14.c_str(), m_starNet14UserTimeout); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%u"), KEY_STARNET_GROUP_TIMEOUT14.c_str(), m_starNet14GroupTimeout); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"), KEY_STARNET_CALLSIGN_SWITCH14.c_str(), int(m_starNet14CallsignSwitch)); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"), KEY_STARNET_TXMSG_SWITCH14.c_str(), m_starNet14TxMsgSwitch ? 1 : 0); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_STARNET_REFLECTOR14.c_str(), m_starNet14Reflector.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_STARNET_BAND15.c_str(), m_starNet15Band.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_STARNET_CALLSIGN15.c_str(), m_starNet15Callsign.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_STARNET_LOGOFF15.c_str(), m_starNet15Logoff.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_STARNET_INFO15.c_str(), m_starNet15Info.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_STARNET_PERMANENT15.c_str(), m_starNet15Permanent.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%u"), KEY_STARNET_USER_TIMEOUT15.c_str(), m_starNet15UserTimeout); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%u"), KEY_STARNET_GROUP_TIMEOUT15.c_str(), m_starNet15GroupTimeout); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"), KEY_STARNET_CALLSIGN_SWITCH15.c_str(), int(m_starNet15CallsignSwitch)); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"), KEY_STARNET_TXMSG_SWITCH15.c_str(), m_starNet15TxMsgSwitch ? 1 : 0); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_STARNET_REFLECTOR15.c_str(), m_starNet15Reflector.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"), KEY_REMOTE_ENABLED.c_str(), m_remoteEnabled ? 1 : 0); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_REMOTE_PASSWORD.c_str(), m_remotePassword.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%u"), KEY_REMOTE_PORT.c_str(), m_remotePort); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"), KEY_LOG_ENABLED.c_str(), m_logEnabled ? 1 : 0); file.AddLine(buffer);
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
