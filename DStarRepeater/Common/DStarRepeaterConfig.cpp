/*
 *   Copyright (C) 2011-2014 by Jonathan Naylor G4KLX
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

#include "DStarRepeaterConfig.h"

#include <wx/textfile.h>

const wxString  KEY_CALLSIGN           = wxT("callsign");
const wxString  KEY_GATEWAY            = wxT("gateway");
const wxString  KEY_MODE               = wxT("mode");
const wxString  KEY_ACK                = wxT("ack");
const wxString  KEY_RESTRICTION        = wxT("restriction");
const wxString  KEY_RPT1_VALIDATION    = wxT("rpt1Validation");
const wxString  KEY_DTMF_BLANKING      = wxT("dtmfBlanking");
const wxString  KEY_ERROR_REPLY        = wxT("errorReply");
const wxString  KEY_GATEWAY_ADDRESS    = wxT("gatewayAddress");
const wxString  KEY_GATEWAY_PORT       = wxT("gatewayPort");
const wxString  KEY_LOCAL_ADDRESS      = wxT("localAddress");
const wxString  KEY_LOCAL_PORT         = wxT("localPort");
const wxString  KEY_NETWORK_NAME       = wxT("networkName");
const wxString  KEY_MODEM_TYPE         = wxT("modemType");
const wxString  KEY_TIMEOUT            = wxT("timeout");
const wxString  KEY_ACK_TIME           = wxT("ackTime");
const wxString  KEY_BEACON_TIME        = wxT("beaconTime");
const wxString  KEY_BEACON_TEXT        = wxT("beaconText");
const wxString  KEY_BEACON_VOICE       = wxT("beaconVoice");
const wxString  KEY_LANGUAGE           = wxT("language");
const wxString  KEY_ANNOUNCEMENT_ENABLED     = wxT("announcementEnabled");
const wxString  KEY_ANNOUNCEMENT_TIME        = wxT("announcementTime");
const wxString  KEY_ANNOUNCEMENT_RECORD_RPT1 = wxT("announcementRecordRPT1");
const wxString  KEY_ANNOUNCEMENT_RECORD_RPT2 = wxT("announcementRecordRPT2");
const wxString  KEY_ANNOUNCEMENT_DELETE_RPT1 = wxT("announcementDeleteRPT1");
const wxString  KEY_ANNOUNCEMENT_DELETE_RPT2 = wxT("announcementDeleteRPT2");
const wxString  KEY_CONTROL_ENABLED    = wxT("controlEnabled");
const wxString  KEY_CONTROL_RPT1       = wxT("controlRPT1");
const wxString  KEY_CONTROL_RPT2       = wxT("controlRPT2");
const wxString  KEY_CONTROL_SHUTDOWN   = wxT("controlShutdown");
const wxString  KEY_CONTROL_STARTUP    = wxT("controlStartup");
const wxString  KEY_CONTROL_STATUS1    = wxT("controlStatus1");
const wxString  KEY_CONTROL_STATUS2    = wxT("controlStatus2");
const wxString  KEY_CONTROL_STATUS3    = wxT("controlStatus3");
const wxString  KEY_CONTROL_STATUS4    = wxT("controlStatus4");
const wxString  KEY_CONTROL_STATUS5    = wxT("controlStatus5");
const wxString  KEY_CONTROL_COMMAND1      = wxT("controlCommand1");
const wxString  KEY_CONTROL_COMMAND1_LINE = wxT("controlCommand1Line");
const wxString  KEY_CONTROL_COMMAND2      = wxT("controlCommand2");
const wxString  KEY_CONTROL_COMMAND2_LINE = wxT("controlCommand2Line");
const wxString  KEY_CONTROL_COMMAND3      = wxT("controlCommand3");
const wxString  KEY_CONTROL_COMMAND3_LINE = wxT("controlCommand3Line");
const wxString  KEY_CONTROL_COMMAND4      = wxT("controlCommand4");
const wxString  KEY_CONTROL_COMMAND4_LINE = wxT("controlCommand4Line");
const wxString  KEY_CONTROL_COMMAND5      = wxT("controlCommand5");
const wxString  KEY_CONTROL_COMMAND5_LINE = wxT("controlCommand5Line");
const wxString  KEY_CONTROL_COMMAND6      = wxT("controlCommand6");
const wxString  KEY_CONTROL_COMMAND6_LINE = wxT("controlCommand6Line");
const wxString  KEY_CONTROL_OUTPUT1    = wxT("controlOutput1");
const wxString  KEY_CONTROL_OUTPUT2    = wxT("controlOutput2");
const wxString  KEY_CONTROL_OUTPUT3    = wxT("controlOutput3");
const wxString  KEY_CONTROL_OUTPUT4    = wxT("controlOutput4");
const wxString  KEY_CONTROLLER_TYPE    = wxT("controllerType");
const wxString  KEY_SERIAL_CONFIG      = wxT("serialConfig");
const wxString  KEY_PTT_INVERT         = wxT("pttInvert");
const wxString  KEY_ACTIVE_HANG_TIME   = wxT("activeHangTime");
const wxString  KEY_OUTPUT1            = wxT("output1");
const wxString  KEY_OUTPUT2            = wxT("output2");
const wxString  KEY_OUTPUT3            = wxT("output3");
const wxString  KEY_OUTPUT4            = wxT("output4");
const wxString  KEY_LOGGING            = wxT("logging");
const wxString  KEY_WINDOW_X           = wxT("windowX");
const wxString  KEY_WINDOW_Y           = wxT("windowY");

const wxString  KEY_DVAP_PORT          = wxT("dvapPort");
const wxString  KEY_DVAP_FREQUENCY     = wxT("dvapFrequency");
const wxString  KEY_DVAP_POWER         = wxT("dvapPower");
const wxString  KEY_DVAP_SQUELCH       = wxT("dvapSquelch");

const wxString  KEY_GMSK_INTERFACE     = wxT("gmskInterfaceType");
const wxString  KEY_GMSK_ADDRESS       = wxT("gmskAddress");

const wxString  KEY_DVRPTR1_PORT       = wxT("dvrptr1Port");
const wxString  KEY_DVRPTR1_RXINVERT   = wxT("dvrptr1RXInvert");
const wxString  KEY_DVRPTR1_TXINVERT   = wxT("dvrptr1TXInvert");
const wxString  KEY_DVRPTR1_CHANNEL    = wxT("dvrptr1Channel");
const wxString  KEY_DVRPTR1_MODLEVEL   = wxT("dvrptr1ModLevel");
const wxString  KEY_DVRPTR1_TXDELAY    = wxT("dvrptr1TXDelay");

const wxString  KEY_DVRPTR2_CONNECTION = wxT("dvrptr2Connection");
const wxString  KEY_DVRPTR2_USBPORT    = wxT("dvrptr2USBPort");
const wxString  KEY_DVRPTR2_ADDRESS    = wxT("dvrptr2Address");
const wxString  KEY_DVRPTR2_PORT       = wxT("dvrptr2Port");
const wxString  KEY_DVRPTR2_TXINVERT   = wxT("dvrptr2TXInvert");
const wxString  KEY_DVRPTR2_MODLEVEL   = wxT("dvrptr2ModLevel");
const wxString  KEY_DVRPTR2_TXDELAY    = wxT("dvrptr2TXDelay");

const wxString  KEY_DVRPTR3_CONNECTION = wxT("dvrptr3Connection");
const wxString  KEY_DVRPTR3_USBPORT    = wxT("dvrptr3USBPort");
const wxString  KEY_DVRPTR3_ADDRESS    = wxT("dvrptr3Address");
const wxString  KEY_DVRPTR3_PORT       = wxT("dvrptr3Port");
const wxString  KEY_DVRPTR3_TXINVERT   = wxT("dvrptr3TXInvert");
const wxString  KEY_DVRPTR3_MODLEVEL   = wxT("dvrptr3ModLevel");
const wxString  KEY_DVRPTR3_TXDELAY    = wxT("dvrptr3TXDelay");

const wxString  KEY_DVMEGA_PORT        = wxT("dvmegaPort");
const wxString  KEY_DVMEGA_VARIANT     = wxT("dvmegaVariant");
const wxString  KEY_DVMEGA_RXINVERT    = wxT("dvmegaRXInvert");
const wxString  KEY_DVMEGA_TXINVERT    = wxT("dvmegaTXInvert");
const wxString  KEY_DVMEGA_TXDELAY     = wxT("dvmegaTXDelay");
const wxString  KEY_DVMEGA_FREQUENCY   = wxT("dvmegaFrequency");
const wxString  KEY_DVMEGA_POWER       = wxT("dvmegaPower");

const wxString  KEY_SOUNDCARD_RXDEVICE = wxT("soundCardRXDevice");
const wxString  KEY_SOUNDCARD_TXDEVICE = wxT("soundCardTXDevice");
const wxString  KEY_SOUNDCARD_RXINVERT = wxT("soundCardRXInvert");
const wxString  KEY_SOUNDCARD_TXINVERT = wxT("soundCardTXInvert");
const wxString  KEY_SOUNDCARD_RXLEVEL  = wxT("soundCardRXLevel");
const wxString  KEY_SOUNDCARD_TXLEVEL  = wxT("soundCardTXLevel");
const wxString  KEY_SOUNDCARD_TXDELAY  = wxT("soundCardTXDelay");
const wxString  KEY_SOUNDCARD_TXTAIL   = wxT("soundCardTXTail");

const wxString  KEY_SPLIT_LOCALADDRESS = wxT("splitLocalAddress");
const wxString  KEY_SPLIT_LOCALPORT    = wxT("splitLocalPort");
const wxString  KEY_SPLIT_TXNAME       = wxT("splitTXName");
const wxString  KEY_SPLIT_RXNAME       = wxT("splitRXName");
const wxString  KEY_SPLIT_TIMEOUT      = wxT("splitTimeout");


const wxString        DEFAULT_CALLSIGN           = wxT("GB3IN  C");
const wxString        DEFAULT_GATEWAY            = wxEmptyString;
const DSTAR_MODE      DEFAULT_MODE               = MODE_DUPLEX;
const ACK_TYPE        DEFAULT_ACK                = AT_BER;
const bool            DEFAULT_RESTRICTION        = false;
const bool            DEFAULT_RPT1_VALIDATION    = true;
const bool            DEFAULT_DTMF_BLANKING      = true;
const bool            DEFAULT_ERROR_REPLY        = true;
const wxString        DEFAULT_GATEWAY_ADDRESS    = wxT("127.0.0.1");
const unsigned int    DEFAULT_GATEWAY_PORT       = 20010U;
const wxString        DEFAULT_LOCAL_ADDRESS      = wxT("127.0.0.1");
const unsigned int    DEFAULT_LOCAL_PORT         = 20011U;
const wxString        DEFAULT_NETWORK_NAME       = wxEmptyString;
const wxString        DEFAULT_MODEM_TYPE         = wxT("DVAP");
const unsigned int    DEFAULT_TIMEOUT            = 180U;
const unsigned int    DEFAULT_ACK_TIME           = 500U;
const unsigned int    DEFAULT_BEACON_TIME        = 600U;
const wxString        DEFAULT_BEACON_TEXT        = wxT("D-Star Repeater");
const bool            DEFAULT_BEACON_VOICE       = false;
const TEXT_LANG       DEFAULT_LANGUAGE           = TL_ENGLISH_UK;
const bool            DEFAULT_ANNOUNCEMENT_ENABLED     = false;
const unsigned int    DEFAULT_ANNOUNCEMENT_TIME        = 500U;
const wxString        DEFAULT_ANNOUNCEMENT_RECORD_RPT1 = wxEmptyString;
const wxString        DEFAULT_ANNOUNCEMENT_RECORD_RPT2 = wxEmptyString;
const wxString        DEFAULT_ANNOUNCEMENT_DELETE_RPT1 = wxEmptyString;
const wxString        DEFAULT_ANNOUNCEMENT_DELETE_RPT2 = wxEmptyString;
const bool            DEFAULT_CONTROL_ENABLED    = false;
const wxString        DEFAULT_CONTROL_RPT1       = wxEmptyString;
const wxString        DEFAULT_CONTROL_RPT2       = wxEmptyString;
const wxString        DEFAULT_CONTROL_SHUTDOWN   = wxEmptyString;
const wxString        DEFAULT_CONTROL_STARTUP    = wxEmptyString;
const wxString        DEFAULT_CONTROL_STATUS     = wxEmptyString;
const wxString        DEFAULT_CONTROL_COMMAND    = wxEmptyString;
const wxString        DEFAULT_CONTROL_COMMAND_LINE = wxEmptyString;
const wxString        DEFAULT_CONTROL_OUTPUT     = wxEmptyString;
const wxString        DEFAULT_CONTROLLER_TYPE    = wxEmptyString;
const unsigned int    DEFAULT_SERIAL_CONFIG      = 1U;
const bool            DEFAULT_PTT_INVERT         = false;
const unsigned int    DEFAULT_ACTIVE_HANG_TIME   = 0U;
const bool            DEFAULT_OUTPUT             = false;
const bool            DEFAULT_LOGGING            = false;
const int             DEFAULT_WINDOW_X           = -1;
const int             DEFAULT_WINDOW_Y           = -1;

const wxString        DEFAULT_DVAP_PORT          = wxEmptyString;
const unsigned int    DEFAULT_DVAP_FREQUENCY     = 145500000U;
const int             DEFAULT_DVAP_POWER         = 10;
const int             DEFAULT_DVAP_SQUELCH       = -100;

#if defined(WIN32)
const USB_INTERFACE   DEFAULT_GMSK_INTERFACE     = UI_WINUSB;
#else
const USB_INTERFACE   DEFAULT_GMSK_INTERFACE     = UI_LIBUSB;
#endif
const unsigned int    DEFAULT_GMSK_ADDRESS       = 0x0300U;

const wxString        DEFAULT_DVRPTR1_PORT       = wxEmptyString;
const bool            DEFAULT_DVRPTR1_RXINVERT   = false;
const bool            DEFAULT_DVRPTR1_TXINVERT   = false;
const bool            DEFAULT_DVRPTR1_CHANNEL    = false;
const unsigned int    DEFAULT_DVRPTR1_MODLEVEL   = 20U;
const unsigned int    DEFAULT_DVRPTR1_TXDELAY    = 150U;

const CONNECTION_TYPE DEFAULT_DVRPTR2_CONNECTION = CT_USB;
const wxString        DEFAULT_DVRPTR2_USBPORT    = wxEmptyString;
const wxString        DEFAULT_DVRPTR2_ADDRESS    = wxT("127.0.0.1");
const unsigned int    DEFAULT_DVRPTR2_PORT       = 0U;
const bool            DEFAULT_DVRPTR2_TXINVERT   = false;
const unsigned int    DEFAULT_DVRPTR2_MODLEVEL   = 20U;
const unsigned int    DEFAULT_DVRPTR2_TXDELAY    = 150U;

const CONNECTION_TYPE DEFAULT_DVRPTR3_CONNECTION = CT_USB;
const wxString        DEFAULT_DVRPTR3_USBPORT    = wxEmptyString;
const wxString        DEFAULT_DVRPTR3_ADDRESS    = wxT("127.0.0.1");
const unsigned int    DEFAULT_DVRPTR3_PORT       = 0U;
const bool            DEFAULT_DVRPTR3_TXINVERT   = false;
const unsigned int    DEFAULT_DVRPTR3_MODLEVEL   = 20U;
const unsigned int    DEFAULT_DVRPTR3_TXDELAY    = 150U;

const wxString        DEFAULT_DVMEGA_PORT        = wxEmptyString;
const DVMEGA_VARIANT  DEFAULT_DVMEGA_VARIANT     = DVMV_NODE;
const bool            DEFAULT_DVMEGA_RXINVERT    = false;
const bool            DEFAULT_DVMEGA_TXINVERT    = false;
const unsigned int    DEFAULT_DVMEGA_TXDELAY     = 150U;
const unsigned int    DEFAULT_DVMEGA_FREQUENCY   = 145500000U;
const unsigned int    DEFAULT_DVMEGA_POWER       = 100U;

const wxString        DEFAULT_SOUNDCARD_RXDEVICE = wxEmptyString;
const wxString        DEFAULT_SOUNDCARD_TXDEVICE = wxEmptyString;
const bool            DEFAULT_SOUNDCARD_RXINVERT = false;
const bool            DEFAULT_SOUNDCARD_TXINVERT = false;
const wxFloat32       DEFAULT_SOUNDCARD_RXLEVEL  = 1.0F;
const wxFloat32       DEFAULT_SOUNDCARD_TXLEVEL  = 1.0F;
const unsigned int    DEFAULT_SOUNDCARD_TXDELAY  = 150U;
const unsigned int    DEFAULT_SOUNDCARD_TXTAIL   = 50U;

const wxString        DEFAULT_SPLIT_LOCALADDRESS = wxEmptyString;
const unsigned int    DEFAULT_SPLIT_LOCALPORT    = 0U;
const wxString        DEFAULT_SPLIT_TXNAME       = wxEmptyString;
const wxString        DEFAULT_SPLIT_RXNAME       = wxEmptyString;
const unsigned int    DEFAULT_SPLIT_TIMEOUT      = 0U;

#if defined(__WINDOWS__)

CDStarRepeaterConfig::CDStarRepeaterConfig(wxConfigBase* config, const wxString& dir, const wxString& configName, const wxString& name) :
m_config(config),
m_name(wxT("/")),
m_fileName(),
m_callsign(DEFAULT_CALLSIGN),
m_gateway(DEFAULT_GATEWAY),
m_mode(DEFAULT_MODE),
m_ack(DEFAULT_ACK),
m_restriction(DEFAULT_RESTRICTION),
m_rpt1Validation(DEFAULT_RPT1_VALIDATION),
m_dtmfBlanking(DEFAULT_DTMF_BLANKING),
m_errorReply(DEFAULT_ERROR_REPLY),
m_gatewayAddress(DEFAULT_GATEWAY_ADDRESS),
m_gatewayPort(DEFAULT_GATEWAY_PORT),
m_localAddress(DEFAULT_LOCAL_ADDRESS),
m_localPort(DEFAULT_LOCAL_PORT),
m_networkName(DEFAULT_NETWORK_NAME),
m_modemType(DEFAULT_MODEM_TYPE),
m_timeout(DEFAULT_TIMEOUT),
m_ackTime(DEFAULT_ACK_TIME),
m_beaconTime(DEFAULT_BEACON_TIME),
m_beaconText(DEFAULT_BEACON_TEXT),
m_beaconVoice(DEFAULT_BEACON_VOICE),
m_language(DEFAULT_LANGUAGE),
m_announcementEnabled(DEFAULT_ANNOUNCEMENT_ENABLED),
m_announcementTime(DEFAULT_ANNOUNCEMENT_TIME),
m_announcementRecordRPT1(DEFAULT_ANNOUNCEMENT_RECORD_RPT1),
m_announcementRecordRPT2(DEFAULT_ANNOUNCEMENT_RECORD_RPT2),
m_announcementDeleteRPT1(DEFAULT_ANNOUNCEMENT_DELETE_RPT1),
m_announcementDeleteRPT2(DEFAULT_ANNOUNCEMENT_DELETE_RPT2),
m_controlEnabled(DEFAULT_CONTROL_ENABLED),
m_controlRpt1Callsign(DEFAULT_CONTROL_RPT1),
m_controlRpt2Callsign(DEFAULT_CONTROL_RPT2),
m_controlShutdown(DEFAULT_CONTROL_SHUTDOWN),
m_controlStartup(DEFAULT_CONTROL_STARTUP),
m_controlStatus1(DEFAULT_CONTROL_STATUS),
m_controlStatus2(DEFAULT_CONTROL_STATUS),
m_controlStatus3(DEFAULT_CONTROL_STATUS),
m_controlStatus4(DEFAULT_CONTROL_STATUS),
m_controlStatus5(DEFAULT_CONTROL_STATUS),
m_controlCommand1(DEFAULT_CONTROL_COMMAND),
m_controlCommand1Line(DEFAULT_CONTROL_COMMAND_LINE),
m_controlCommand2(DEFAULT_CONTROL_COMMAND),
m_controlCommand2Line(DEFAULT_CONTROL_COMMAND_LINE),
m_controlCommand3(DEFAULT_CONTROL_COMMAND),
m_controlCommand3Line(DEFAULT_CONTROL_COMMAND_LINE),
m_controlCommand4(DEFAULT_CONTROL_COMMAND),
m_controlCommand4Line(DEFAULT_CONTROL_COMMAND_LINE),
m_controlCommand5(DEFAULT_CONTROL_COMMAND),
m_controlCommand5Line(DEFAULT_CONTROL_COMMAND_LINE),
m_controlCommand6(DEFAULT_CONTROL_COMMAND),
m_controlCommand6Line(DEFAULT_CONTROL_COMMAND_LINE),
m_controlOutput1(DEFAULT_CONTROL_OUTPUT),
m_controlOutput2(DEFAULT_CONTROL_OUTPUT),
m_controlOutput3(DEFAULT_CONTROL_OUTPUT),
m_controlOutput4(DEFAULT_CONTROL_OUTPUT),
m_controllerType(DEFAULT_CONTROLLER_TYPE),
m_serialConfig(DEFAULT_SERIAL_CONFIG),
m_pttInvert(DEFAULT_PTT_INVERT),
m_activeHangTime(DEFAULT_ACTIVE_HANG_TIME),
m_output1(DEFAULT_OUTPUT),
m_output2(DEFAULT_OUTPUT),
m_output3(DEFAULT_OUTPUT),
m_output4(DEFAULT_OUTPUT),
m_logging(DEFAULT_LOGGING),
m_x(DEFAULT_WINDOW_X),
m_y(DEFAULT_WINDOW_Y),
m_dvapPort(DEFAULT_DVAP_PORT),
m_dvapFrequency(DEFAULT_DVAP_FREQUENCY),
m_dvapPower(DEFAULT_DVAP_POWER),
m_dvapSquelch(DEFAULT_DVAP_SQUELCH),
m_gmskInterface(DEFAULT_GMSK_INTERFACE),
m_gmskAddress(DEFAULT_GMSK_ADDRESS),
m_dvrptr1Port(DEFAULT_DVRPTR1_PORT),
m_dvrptr1RXInvert(DEFAULT_DVRPTR1_RXINVERT),
m_dvrptr1TXInvert(DEFAULT_DVRPTR1_TXINVERT),
m_dvrptr1Channel(DEFAULT_DVRPTR1_CHANNEL),
m_dvrptr1ModLevel(DEFAULT_DVRPTR1_MODLEVEL),
m_dvrptr1TXDelay(DEFAULT_DVRPTR1_TXDELAY),
m_dvrptr2Connection(DEFAULT_DVRPTR2_CONNECTION),
m_dvrptr2USBPort(DEFAULT_DVRPTR2_USBPORT),
m_dvrptr2Address(DEFAULT_DVRPTR2_ADDRESS),
m_dvrptr2Port(DEFAULT_DVRPTR2_PORT),
m_dvrptr2TXInvert(DEFAULT_DVRPTR2_TXINVERT),
m_dvrptr2ModLevel(DEFAULT_DVRPTR2_MODLEVEL),
m_dvrptr2TXDelay(DEFAULT_DVRPTR2_TXDELAY),
m_dvrptr3Connection(DEFAULT_DVRPTR3_CONNECTION),
m_dvrptr3USBPort(DEFAULT_DVRPTR3_USBPORT),
m_dvrptr3Address(DEFAULT_DVRPTR3_ADDRESS),
m_dvrptr3Port(DEFAULT_DVRPTR3_PORT),
m_dvrptr3TXInvert(DEFAULT_DVRPTR3_TXINVERT),
m_dvrptr3ModLevel(DEFAULT_DVRPTR3_MODLEVEL),
m_dvrptr3TXDelay(DEFAULT_DVRPTR3_TXDELAY),
m_dvmegaPort(DEFAULT_DVMEGA_PORT),
m_dvmegaVariant(DEFAULT_DVMEGA_VARIANT),
m_dvmegaRXInvert(DEFAULT_DVMEGA_RXINVERT),
m_dvmegaTXInvert(DEFAULT_DVMEGA_TXINVERT),
m_dvmegaTXDelay(DEFAULT_DVMEGA_TXDELAY),
m_dvmegaFrequency(DEFAULT_DVMEGA_FREQUENCY),
m_dvmegaPower(DEFAULT_DVMEGA_POWER),
m_soundCardRXDevice(DEFAULT_SOUNDCARD_RXDEVICE),
m_soundCardTXDevice(DEFAULT_SOUNDCARD_TXDEVICE),
m_soundCardRXInvert(DEFAULT_SOUNDCARD_RXINVERT),
m_soundCardTXInvert(DEFAULT_SOUNDCARD_TXINVERT),
m_soundCardRXLevel(DEFAULT_SOUNDCARD_RXLEVEL),
m_soundCardTXLevel(DEFAULT_SOUNDCARD_TXLEVEL),
m_soundCardTXDelay(DEFAULT_SOUNDCARD_TXDELAY),
m_soundCardTXTail(DEFAULT_SOUNDCARD_TXTAIL),
m_splitLocalAddress(DEFAULT_SPLIT_LOCALADDRESS),
m_splitLocalPort(DEFAULT_SPLIT_LOCALPORT),
m_splitTXNames(),
m_splitRXNames(),
m_splitTimeout(DEFAULT_SPLIT_TIMEOUT)
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
	double temp1;

	m_config->Read(m_name + KEY_CALLSIGN, &m_callsign, DEFAULT_CALLSIGN);

	m_config->Read(m_name + KEY_GATEWAY, &m_gateway, DEFAULT_GATEWAY);

	m_config->Read(m_name + KEY_MODE, &temp, long(DEFAULT_MODE));
	m_mode = DSTAR_MODE(temp);

	m_config->Read(m_name + KEY_ACK, &temp, long(DEFAULT_ACK));
	m_ack = ACK_TYPE(temp);

	m_config->Read(m_name + KEY_RESTRICTION, &m_restriction, DEFAULT_RESTRICTION);

	m_config->Read(m_name + KEY_RPT1_VALIDATION, &m_rpt1Validation, DEFAULT_RPT1_VALIDATION);

	m_config->Read(m_name + KEY_DTMF_BLANKING, &m_dtmfBlanking, DEFAULT_DTMF_BLANKING);

	m_config->Read(m_name + KEY_ERROR_REPLY, &m_errorReply, DEFAULT_ERROR_REPLY);

	m_config->Read(m_name + KEY_GATEWAY_ADDRESS, &m_gatewayAddress, DEFAULT_GATEWAY_ADDRESS);

	m_config->Read(m_name + KEY_GATEWAY_PORT, &temp, long(DEFAULT_GATEWAY_PORT));
	m_gatewayPort = (unsigned int)temp;

	m_config->Read(m_name + KEY_LOCAL_ADDRESS, &m_localAddress, DEFAULT_LOCAL_ADDRESS);

	m_config->Read(m_name + KEY_LOCAL_PORT, &temp, long(DEFAULT_LOCAL_PORT));
	m_localPort = (unsigned int)temp;

	m_config->Read(m_name + KEY_NETWORK_NAME, &m_networkName, DEFAULT_NETWORK_NAME);

	m_config->Read(m_name + KEY_MODEM_TYPE, &m_modemType, DEFAULT_MODEM_TYPE);

	m_config->Read(m_name + KEY_TIMEOUT, &temp, long(DEFAULT_TIMEOUT));
	m_timeout = (unsigned int)temp;

	m_config->Read(m_name + KEY_ACK_TIME, &temp, long(DEFAULT_ACK_TIME));
	m_ackTime = (unsigned int)temp;

	m_config->Read(m_name + KEY_BEACON_TIME, &temp, long(DEFAULT_BEACON_TIME));
	m_beaconTime = (unsigned int)temp;

	m_config->Read(m_name + KEY_BEACON_TEXT, &m_beaconText, DEFAULT_BEACON_TEXT);

	m_config->Read(m_name + KEY_BEACON_VOICE, &m_beaconVoice, DEFAULT_BEACON_VOICE);

	m_config->Read(m_name + KEY_LANGUAGE, &temp, long(DEFAULT_LANGUAGE));
	m_language = TEXT_LANG(temp);

	m_config->Read(m_name + KEY_ANNOUNCEMENT_ENABLED, &m_announcementEnabled, DEFAULT_ANNOUNCEMENT_ENABLED);

	m_config->Read(m_name + KEY_ANNOUNCEMENT_TIME, &temp, long(DEFAULT_ANNOUNCEMENT_TIME));
	m_announcementTime = (unsigned int)temp;

	m_config->Read(m_name + KEY_ANNOUNCEMENT_RECORD_RPT1, &m_announcementRecordRPT1, DEFAULT_ANNOUNCEMENT_RECORD_RPT1);

	m_config->Read(m_name + KEY_ANNOUNCEMENT_RECORD_RPT2, &m_announcementRecordRPT2, DEFAULT_ANNOUNCEMENT_RECORD_RPT2);

	m_config->Read(m_name + KEY_ANNOUNCEMENT_DELETE_RPT1, &m_announcementDeleteRPT1, DEFAULT_ANNOUNCEMENT_DELETE_RPT1);

	m_config->Read(m_name + KEY_ANNOUNCEMENT_DELETE_RPT2, &m_announcementDeleteRPT2, DEFAULT_ANNOUNCEMENT_DELETE_RPT2);

	m_config->Read(m_name + KEY_CONTROL_ENABLED, &m_controlEnabled, DEFAULT_CONTROL_ENABLED);

	m_config->Read(m_name + KEY_CONTROL_RPT1, &m_controlRpt1Callsign, DEFAULT_CONTROL_RPT1);

	m_config->Read(m_name + KEY_CONTROL_RPT2, &m_controlRpt2Callsign, DEFAULT_CONTROL_RPT2);

	m_config->Read(m_name + KEY_CONTROL_SHUTDOWN, &m_controlShutdown, DEFAULT_CONTROL_SHUTDOWN);

	m_config->Read(m_name + KEY_CONTROL_STARTUP, &m_controlStartup, DEFAULT_CONTROL_STARTUP);

	m_config->Read(m_name + KEY_CONTROL_STATUS1, &m_controlStatus1, DEFAULT_CONTROL_STATUS);

	m_config->Read(m_name + KEY_CONTROL_STATUS2, &m_controlStatus2, DEFAULT_CONTROL_STATUS);

	m_config->Read(m_name + KEY_CONTROL_STATUS3, &m_controlStatus3, DEFAULT_CONTROL_STATUS);

	m_config->Read(m_name + KEY_CONTROL_STATUS4, &m_controlStatus4, DEFAULT_CONTROL_STATUS);

	m_config->Read(m_name + KEY_CONTROL_STATUS5, &m_controlStatus5, DEFAULT_CONTROL_STATUS);

	m_config->Read(m_name + KEY_CONTROL_COMMAND1, &m_controlCommand1, DEFAULT_CONTROL_COMMAND);

	m_config->Read(m_name + KEY_CONTROL_COMMAND1_LINE, &m_controlCommand1Line, DEFAULT_CONTROL_COMMAND_LINE);

	m_config->Read(m_name + KEY_CONTROL_COMMAND2, &m_controlCommand2, DEFAULT_CONTROL_COMMAND);

	m_config->Read(m_name + KEY_CONTROL_COMMAND2_LINE, &m_controlCommand2Line, DEFAULT_CONTROL_COMMAND_LINE);

	m_config->Read(m_name + KEY_CONTROL_COMMAND3, &m_controlCommand3, DEFAULT_CONTROL_COMMAND);

	m_config->Read(m_name + KEY_CONTROL_COMMAND3_LINE, &m_controlCommand3Line, DEFAULT_CONTROL_COMMAND_LINE);

	m_config->Read(m_name + KEY_CONTROL_COMMAND4, &m_controlCommand4, DEFAULT_CONTROL_COMMAND);

	m_config->Read(m_name + KEY_CONTROL_COMMAND4_LINE, &m_controlCommand4Line, DEFAULT_CONTROL_COMMAND_LINE);

	m_config->Read(m_name + KEY_CONTROL_COMMAND5, &m_controlCommand5, DEFAULT_CONTROL_COMMAND);

	m_config->Read(m_name + KEY_CONTROL_COMMAND5_LINE, &m_controlCommand5Line, DEFAULT_CONTROL_COMMAND_LINE);

	m_config->Read(m_name + KEY_CONTROL_COMMAND6, &m_controlCommand6, DEFAULT_CONTROL_COMMAND);

	m_config->Read(m_name + KEY_CONTROL_COMMAND6_LINE, &m_controlCommand6Line, DEFAULT_CONTROL_COMMAND_LINE);

	m_config->Read(m_name + KEY_CONTROL_OUTPUT1, &m_controlOutput1, DEFAULT_CONTROL_OUTPUT);

	m_config->Read(m_name + KEY_CONTROL_OUTPUT2, &m_controlOutput2, DEFAULT_CONTROL_OUTPUT);

	m_config->Read(m_name + KEY_CONTROL_OUTPUT3, &m_controlOutput3, DEFAULT_CONTROL_OUTPUT);

	m_config->Read(m_name + KEY_CONTROL_OUTPUT4, &m_controlOutput4, DEFAULT_CONTROL_OUTPUT);

	m_config->Read(m_name + KEY_CONTROLLER_TYPE, &m_controllerType, DEFAULT_CONTROLLER_TYPE);

	m_config->Read(m_name + KEY_SERIAL_CONFIG, &temp, long(DEFAULT_SERIAL_CONFIG));
	m_serialConfig = (unsigned int)temp;

	m_config->Read(m_name + KEY_PTT_INVERT, &m_pttInvert, DEFAULT_PTT_INVERT);

	m_config->Read(m_name + KEY_ACTIVE_HANG_TIME, &temp, long(DEFAULT_ACTIVE_HANG_TIME));
	m_activeHangTime = (unsigned int)temp;

	m_config->Read(m_name + KEY_OUTPUT1, &m_output1, DEFAULT_OUTPUT);

	m_config->Read(m_name + KEY_OUTPUT2, &m_output2, DEFAULT_OUTPUT);

	m_config->Read(m_name + KEY_OUTPUT3, &m_output3, DEFAULT_OUTPUT);

	m_config->Read(m_name + KEY_OUTPUT4, &m_output4, DEFAULT_OUTPUT);

	m_config->Read(m_name + KEY_LOGGING, &m_logging, DEFAULT_LOGGING);

	m_config->Read(m_name + KEY_WINDOW_X, &temp, long(DEFAULT_WINDOW_X));
	m_x = int(temp);

	m_config->Read(m_name + KEY_WINDOW_Y, &temp, long(DEFAULT_WINDOW_Y));
	m_y = int(temp);

	m_config->Read(m_name + KEY_DVAP_PORT, &m_dvapPort, DEFAULT_DVAP_PORT);

	m_config->Read(m_name + KEY_DVAP_FREQUENCY, &temp, long(DEFAULT_DVAP_FREQUENCY));
	m_dvapFrequency = (unsigned int)temp;

	m_config->Read(m_name + KEY_DVAP_POWER, &temp, long(DEFAULT_DVAP_POWER));
	m_dvapPower = int(temp);

	m_config->Read(m_name + KEY_DVAP_SQUELCH, &temp, long(DEFAULT_DVAP_SQUELCH));
	m_dvapSquelch = int(temp);

	m_config->Read(m_name + KEY_GMSK_INTERFACE, &temp, long(DEFAULT_GMSK_INTERFACE));
	m_gmskInterface = USB_INTERFACE(temp);

	m_config->Read(m_name + KEY_GMSK_ADDRESS, &temp, long(DEFAULT_GMSK_ADDRESS));
	m_gmskAddress = (unsigned int)temp;

	m_config->Read(m_name + KEY_DVRPTR1_PORT, &m_dvrptr1Port, DEFAULT_DVRPTR1_PORT);

	m_config->Read(m_name + KEY_DVRPTR1_RXINVERT, &m_dvrptr1RXInvert, DEFAULT_DVRPTR1_RXINVERT);

	m_config->Read(m_name + KEY_DVRPTR1_TXINVERT, &m_dvrptr1TXInvert, DEFAULT_DVRPTR1_TXINVERT);

	m_config->Read(m_name + KEY_DVRPTR1_CHANNEL, &m_dvrptr1Channel, DEFAULT_DVRPTR1_CHANNEL);

	m_config->Read(m_name + KEY_DVRPTR1_MODLEVEL, &temp, long(DEFAULT_DVRPTR1_MODLEVEL));
	m_dvrptr1ModLevel = (unsigned int)temp;

	m_config->Read(m_name + KEY_DVRPTR1_TXDELAY, &temp, long(DEFAULT_DVRPTR1_TXDELAY));
	m_dvrptr1TXDelay = (unsigned int)temp;

	m_config->Read(m_name + KEY_DVRPTR2_CONNECTION, &temp, long(DEFAULT_DVRPTR2_CONNECTION));
	m_dvrptr2Connection = CONNECTION_TYPE(temp);

	m_config->Read(m_name + KEY_DVRPTR2_USBPORT, &m_dvrptr2USBPort, DEFAULT_DVRPTR2_USBPORT);

	m_config->Read(m_name + KEY_DVRPTR2_ADDRESS, &m_dvrptr2Address, DEFAULT_DVRPTR2_ADDRESS);

	m_config->Read(m_name + KEY_DVRPTR2_PORT, &temp, long(DEFAULT_DVRPTR2_PORT));
	m_dvrptr2Port = (unsigned int)temp;

	m_config->Read(m_name + KEY_DVRPTR2_TXINVERT, &m_dvrptr2TXInvert, DEFAULT_DVRPTR2_TXINVERT);

	m_config->Read(m_name + KEY_DVRPTR2_MODLEVEL, &temp, long(DEFAULT_DVRPTR2_MODLEVEL));
	m_dvrptr2ModLevel = (unsigned int)temp;

	m_config->Read(m_name + KEY_DVRPTR2_TXDELAY, &temp, long(DEFAULT_DVRPTR2_TXDELAY));
	m_dvrptr2TXDelay = (unsigned int)temp;

	m_config->Read(m_name + KEY_DVRPTR3_CONNECTION, &temp, long(DEFAULT_DVRPTR3_CONNECTION));
	m_dvrptr3Connection = CONNECTION_TYPE(temp);

	m_config->Read(m_name + KEY_DVRPTR3_USBPORT, &m_dvrptr3USBPort, DEFAULT_DVRPTR3_USBPORT);

	m_config->Read(m_name + KEY_DVRPTR3_ADDRESS, &m_dvrptr3Address, DEFAULT_DVRPTR3_ADDRESS);

	m_config->Read(m_name + KEY_DVRPTR3_PORT, &temp, long(DEFAULT_DVRPTR3_PORT));
	m_dvrptr3Port = (unsigned int)temp;

	m_config->Read(m_name + KEY_DVRPTR3_TXINVERT, &m_dvrptr3TXInvert, DEFAULT_DVRPTR3_TXINVERT);

	m_config->Read(m_name + KEY_DVRPTR3_MODLEVEL, &temp, long(DEFAULT_DVRPTR3_MODLEVEL));
	m_dvrptr3ModLevel = (unsigned int)temp;

	m_config->Read(m_name + KEY_DVRPTR3_TXDELAY, &temp, long(DEFAULT_DVRPTR3_TXDELAY));
	m_dvrptr3TXDelay = (unsigned int)temp;

	m_config->Read(m_name + KEY_DVMEGA_PORT, &m_dvmegaPort, DEFAULT_DVMEGA_PORT);

	m_config->Read(m_name + KEY_DVMEGA_VARIANT, &temp, long(DEFAULT_DVMEGA_VARIANT));
	m_dvmegaVariant = DVMEGA_VARIANT(temp);

	m_config->Read(m_name + KEY_DVMEGA_RXINVERT, &m_dvmegaRXInvert, DEFAULT_DVMEGA_RXINVERT);

	m_config->Read(m_name + KEY_DVMEGA_TXINVERT, &m_dvmegaTXInvert, DEFAULT_DVMEGA_TXINVERT);

	m_config->Read(m_name + KEY_DVMEGA_TXDELAY, &temp, long(DEFAULT_DVMEGA_TXDELAY));
	m_dvmegaTXDelay = (unsigned int)temp;

	m_config->Read(m_name + KEY_DVMEGA_FREQUENCY, &temp, long(DEFAULT_DVMEGA_FREQUENCY));
	m_dvmegaFrequency = (unsigned int)temp;

	m_config->Read(m_name + KEY_DVMEGA_POWER, &temp, long(DEFAULT_DVMEGA_POWER));
	m_dvmegaPower = (unsigned int)temp;

	m_config->Read(m_name + KEY_SOUNDCARD_RXDEVICE, &m_soundCardRXDevice, DEFAULT_SOUNDCARD_RXDEVICE);

	m_config->Read(m_name + KEY_SOUNDCARD_TXDEVICE, &m_soundCardTXDevice, DEFAULT_SOUNDCARD_TXDEVICE);

	m_config->Read(m_name + KEY_SOUNDCARD_RXINVERT, &m_soundCardRXInvert, DEFAULT_SOUNDCARD_RXINVERT);

	m_config->Read(m_name + KEY_SOUNDCARD_TXINVERT, &m_soundCardTXInvert, DEFAULT_SOUNDCARD_TXINVERT);

	m_config->Read(m_name + KEY_SOUNDCARD_RXLEVEL, &temp1, double(DEFAULT_SOUNDCARD_RXLEVEL));
	m_soundCardRXLevel = wxFloat32(temp1);

	m_config->Read(m_name + KEY_SOUNDCARD_TXLEVEL, &temp1, double(DEFAULT_SOUNDCARD_TXLEVEL));
	m_soundCardTXLevel = wxFloat32(temp1);

	m_config->Read(m_name + KEY_SOUNDCARD_TXDELAY, &temp, long(DEFAULT_SOUNDCARD_TXDELAY));
	m_soundCardTXDelay = (unsigned int)temp;

	m_config->Read(m_name + KEY_SOUNDCARD_TXTAIL, &temp, long(DEFAULT_SOUNDCARD_TXTAIL));
	m_soundCardTXTail = (unsigned int)temp;

	m_config->Read(m_name + KEY_SPLIT_LOCALADDRESS, &m_splitLocalAddress, DEFAULT_SPLIT_LOCALADDRESS);

	m_config->Read(m_name + KEY_SPLIT_LOCALPORT, &temp, long(DEFAULT_SPLIT_LOCALPORT));
	m_splitLocalPort = (unsigned int)temp;

	for (unsigned int i = 0U; i < SPLIT_TX_COUNT; i++) {
		wxString name;
		name.Printf(wxT("%s%u"), KEY_SPLIT_TXNAME.c_str(), i);

		wxString value;
		m_config->Read(m_name + name, &value, DEFAULT_SPLIT_TXNAME);
		m_splitTXNames.Add(value);
	}

	for (unsigned int i = 0U; i < SPLIT_RX_COUNT; i++) {
		wxString name;
		name.Printf(wxT("%s%u"), KEY_SPLIT_RXNAME.c_str(), i);

		wxString value;
		m_config->Read(m_name + name, &value, DEFAULT_SPLIT_RXNAME);
		m_splitRXNames.Add(value);
	}

	m_config->Read(m_name + KEY_SPLIT_TIMEOUT, &temp, long(DEFAULT_SPLIT_TIMEOUT));
	m_splitTimeout = (unsigned int)temp;
}

CDStarRepeaterConfig::~CDStarRepeaterConfig()
{
	delete m_config;
}

#else

CDStarRepeaterConfig::CDStarRepeaterConfig(const wxString& dir, const wxString& configName, const wxString& name) :
m_fileName(),
m_callsign(DEFAULT_CALLSIGN),
m_gateway(DEFAULT_GATEWAY),
m_mode(DEFAULT_MODE),
m_ack(DEFAULT_ACK),
m_restriction(DEFAULT_RESTRICTION),
m_rpt1Validation(DEFAULT_RPT1_VALIDATION),
m_dtmfBlanking(DEFAULT_DTMF_BLANKING),
m_errorReply(DEFAULT_ERROR_REPLY),
m_gatewayAddress(DEFAULT_GATEWAY_ADDRESS),
m_gatewayPort(DEFAULT_GATEWAY_PORT),
m_localAddress(DEFAULT_LOCAL_ADDRESS),
m_localPort(DEFAULT_LOCAL_PORT),
m_networkName(DEFAULT_NETWORK_NAME),
m_modemType(DEFAULT_MODEM_TYPE),
m_timeout(DEFAULT_TIMEOUT),
m_ackTime(DEFAULT_ACK_TIME),
m_beaconTime(DEFAULT_BEACON_TIME),
m_beaconText(DEFAULT_BEACON_TEXT),
m_beaconVoice(DEFAULT_BEACON_VOICE),
m_language(DEFAULT_LANGUAGE),
m_announcementEnabled(DEFAULT_ANNOUNCEMENT_ENABLED),
m_announcementTime(DEFAULT_ANNOUNCEMENT_TIME),
m_announcementRecordRPT1(DEFAULT_ANNOUNCEMENT_RECORD_RPT1),
m_announcementRecordRPT2(DEFAULT_ANNOUNCEMENT_RECORD_RPT2),
m_announcementDeleteRPT1(DEFAULT_ANNOUNCEMENT_DELETE_RPT1),
m_announcementDeleteRPT2(DEFAULT_ANNOUNCEMENT_DELETE_RPT2),
m_controlEnabled(DEFAULT_CONTROL_ENABLED),
m_controlRpt1Callsign(DEFAULT_CONTROL_RPT1),
m_controlRpt2Callsign(DEFAULT_CONTROL_RPT2),
m_controlShutdown(DEFAULT_CONTROL_SHUTDOWN),
m_controlStartup(DEFAULT_CONTROL_STARTUP),
m_controlStatus1(DEFAULT_CONTROL_STATUS),
m_controlStatus2(DEFAULT_CONTROL_STATUS),
m_controlStatus3(DEFAULT_CONTROL_STATUS),
m_controlStatus4(DEFAULT_CONTROL_STATUS),
m_controlStatus5(DEFAULT_CONTROL_STATUS),
m_controlCommand1(DEFAULT_CONTROL_COMMAND),
m_controlCommand1Line(DEFAULT_CONTROL_COMMAND_LINE),
m_controlCommand2(DEFAULT_CONTROL_COMMAND),
m_controlCommand2Line(DEFAULT_CONTROL_COMMAND_LINE),
m_controlCommand3(DEFAULT_CONTROL_COMMAND),
m_controlCommand3Line(DEFAULT_CONTROL_COMMAND_LINE),
m_controlCommand4(DEFAULT_CONTROL_COMMAND),
m_controlCommand4Line(DEFAULT_CONTROL_COMMAND_LINE),
m_controlCommand5(DEFAULT_CONTROL_COMMAND),
m_controlCommand5Line(DEFAULT_CONTROL_COMMAND_LINE),
m_controlCommand6(DEFAULT_CONTROL_COMMAND),
m_controlCommand6Line(DEFAULT_CONTROL_COMMAND_LINE),
m_controlOutput1(DEFAULT_CONTROL_OUTPUT),
m_controlOutput2(DEFAULT_CONTROL_OUTPUT),
m_controlOutput3(DEFAULT_CONTROL_OUTPUT),
m_controlOutput4(DEFAULT_CONTROL_OUTPUT),
m_controllerType(DEFAULT_CONTROLLER_TYPE),
m_serialConfig(DEFAULT_SERIAL_CONFIG),
m_pttInvert(DEFAULT_PTT_INVERT),
m_activeHangTime(DEFAULT_ACTIVE_HANG_TIME),
m_output1(DEFAULT_OUTPUT),
m_output2(DEFAULT_OUTPUT),
m_output3(DEFAULT_OUTPUT),
m_output4(DEFAULT_OUTPUT),
m_logging(DEFAULT_LOGGING),
m_x(DEFAULT_WINDOW_X),
m_y(DEFAULT_WINDOW_Y),
m_dvapPort(DEFAULT_DVAP_PORT),
m_dvapFrequency(DEFAULT_DVAP_FREQUENCY),
m_dvapPower(DEFAULT_DVAP_POWER),
m_dvapSquelch(DEFAULT_DVAP_SQUELCH),
m_gmskInterface(DEFAULT_GMSK_INTERFACE),
m_gmskAddress(DEFAULT_GMSK_ADDRESS),
m_dvrptr1Port(DEFAULT_DVRPTR1_PORT),
m_dvrptr1RXInvert(DEFAULT_DVRPTR1_RXINVERT),
m_dvrptr1TXInvert(DEFAULT_DVRPTR1_TXINVERT),
m_dvrptr1Channel(DEFAULT_DVRPTR1_CHANNEL),
m_dvrptr1ModLevel(DEFAULT_DVRPTR1_MODLEVEL),
m_dvrptr1TXDelay(DEFAULT_DVRPTR1_TXDELAY),
m_dvrptr2Connection(DEFAULT_DVRPTR2_CONNECTION),
m_dvrptr2USBPort(DEFAULT_DVRPTR2_USBPORT),
m_dvrptr2Address(DEFAULT_DVRPTR2_ADDRESS),
m_dvrptr2Port(DEFAULT_DVRPTR2_PORT),
m_dvrptr2TXInvert(DEFAULT_DVRPTR2_TXINVERT),
m_dvrptr2ModLevel(DEFAULT_DVRPTR2_MODLEVEL),
m_dvrptr2TXDelay(DEFAULT_DVRPTR2_TXDELAY),
m_dvrptr3Connection(DEFAULT_DVRPTR3_CONNECTION),
m_dvrptr3USBPort(DEFAULT_DVRPTR3_USBPORT),
m_dvrptr3Address(DEFAULT_DVRPTR3_ADDRESS),
m_dvrptr3Port(DEFAULT_DVRPTR3_PORT),
m_dvrptr3TXInvert(DEFAULT_DVRPTR3_TXINVERT),
m_dvrptr3ModLevel(DEFAULT_DVRPTR3_MODLEVEL),
m_dvrptr3TXDelay(DEFAULT_DVRPTR3_TXDELAY),
m_dvmegaPort(DEFAULT_DVMEGA_PORT),
m_dvmegaVariant(DEFAULT_DVMEGA_VARIANT),
m_dvmegaRXInvert(DEFAULT_DVMEGA_RXINVERT),
m_dvmegaTXInvert(DEFAULT_DVMEGA_TXINVERT),
m_dvmegaTXDelay(DEFAULT_DVMEGA_TXDELAY),
m_dvmegaFrequency(DEFAULT_DVMEGA_FREQUENCY),
m_dvmegaPower(DEFAULT_DVMEGA_POWER),
m_soundCardRXDevice(DEFAULT_SOUNDCARD_RXDEVICE),
m_soundCardTXDevice(DEFAULT_SOUNDCARD_TXDEVICE),
m_soundCardRXInvert(DEFAULT_SOUNDCARD_RXINVERT),
m_soundCardTXInvert(DEFAULT_SOUNDCARD_TXINVERT),
m_soundCardRXLevel(DEFAULT_SOUNDCARD_RXLEVEL),
m_soundCardTXLevel(DEFAULT_SOUNDCARD_TXLEVEL),
m_soundCardTXDelay(DEFAULT_SOUNDCARD_TXDELAY),
m_soundCardTXTail(DEFAULT_SOUNDCARD_TXTAIL),
m_splitLocalAddress(DEFAULT_SPLIT_LOCALADDRESS),
m_splitLocalPort(DEFAULT_SPLIT_LOCALPORT),
m_splitTXNames(),
m_splitRXNames(),
m_splitTimeout(DEFAULT_SPLIT_TIMEOUT)
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

	wxString* splitTXName = new wxString[SPLIT_TX_COUNT];
	wxString* splitRXName = new wxString[SPLIT_RX_COUNT];

	long temp1;
	unsigned long temp2;
	double temp3;

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
		} else if (key.IsSameAs(KEY_GATEWAY)) {
			m_gateway = val;
		} else if (key.IsSameAs(KEY_MODE)) {
			val.ToLong(&temp1);
			m_mode = DSTAR_MODE(temp1);
		} else if (key.IsSameAs(KEY_ACK)) {
			val.ToLong(&temp1);
			m_ack = ACK_TYPE(temp1);
		} else if (key.IsSameAs(KEY_RESTRICTION)) {
			val.ToLong(&temp1);
			m_restriction = temp1 == 1L;
		} else if (key.IsSameAs(KEY_RPT1_VALIDATION)) {
			val.ToLong(&temp1);
			m_rpt1Validation = temp1 == 1L;
		} else if (key.IsSameAs(KEY_DTMF_BLANKING)) {
			val.ToLong(&temp1);
			m_dtmfBlanking = temp1 == 1L;
		} else if (key.IsSameAs(KEY_ERROR_REPLY)) {
			val.ToLong(&temp1);
			m_errorReply = temp1 == 1L;
		} else if (key.IsSameAs(KEY_GATEWAY_ADDRESS)) {
			m_gatewayAddress = val;
		} else if (key.IsSameAs(KEY_GATEWAY_PORT)) {
			val.ToULong(&temp2);
			m_gatewayPort = (unsigned int)temp2;
		} else if (key.IsSameAs(KEY_LOCAL_ADDRESS)) {
			m_localAddress = val;
		} else if (key.IsSameAs(KEY_LOCAL_PORT)) {
			val.ToULong(&temp2);
			m_localPort = (unsigned int)temp2;
		} else if (key.IsSameAs(KEY_NETWORK_NAME)) {
			m_networkName = val;
		} else if (key.IsSameAs(KEY_MODEM_TYPE)) {
			m_modemType = val;
		} else if (key.IsSameAs(KEY_TIMEOUT)) {
			val.ToULong(&temp2);
			m_timeout = (unsigned int)temp2;
		} else if (key.IsSameAs(KEY_ACK_TIME)) {
			val.ToULong(&temp2);
			m_ackTime = (unsigned int)temp2;
		} else if (key.IsSameAs(KEY_BEACON_TIME)) {
			val.ToULong(&temp2);
			m_beaconTime = (unsigned int)temp2;
		} else if (key.IsSameAs(KEY_BEACON_TEXT)) {
			m_beaconText = val;
		} else if (key.IsSameAs(KEY_BEACON_VOICE)) {
			val.ToLong(&temp1);
			m_beaconVoice = temp1 == 1L;
		} else if (key.IsSameAs(KEY_LANGUAGE)) {
			val.ToLong(&temp1);
			m_language = TEXT_LANG(temp1);
		} else if (key.IsSameAs(KEY_ANNOUNCEMENT_ENABLED)) {
			val.ToLong(&temp1);
			m_announcementEnabled = temp1 == 1L;
		} else if (key.IsSameAs(KEY_ANNOUNCEMENT_TIME)) {
			val.ToULong(&temp2);
			m_announcementTime = (unsigned int)temp2;
		} else if (key.IsSameAs(KEY_ANNOUNCEMENT_RECORD_RPT1)) {
			m_announcementRecordRPT1 = val;
		} else if (key.IsSameAs(KEY_ANNOUNCEMENT_RECORD_RPT2)) {
			m_announcementRecordRPT2 = val;
		} else if (key.IsSameAs(KEY_ANNOUNCEMENT_DELETE_RPT1)) {
			m_announcementDeleteRPT1 = val;
		} else if (key.IsSameAs(KEY_ANNOUNCEMENT_DELETE_RPT2)) {
			m_announcementDeleteRPT2 = val;
		} else if (key.IsSameAs(KEY_CONTROL_ENABLED)) {
			val.ToLong(&temp1);
			m_controlEnabled = temp1 == 1L;
		} else if (key.IsSameAs(KEY_CONTROL_RPT1)) {
			m_controlRpt1Callsign = val;
		} else if (key.IsSameAs(KEY_CONTROL_RPT2)) {
			m_controlRpt2Callsign = val;
		} else if (key.IsSameAs(KEY_CONTROL_SHUTDOWN)) {
			m_controlShutdown = val;
		} else if (key.IsSameAs(KEY_CONTROL_STARTUP)) {
			m_controlStartup = val;
		} else if (key.IsSameAs(KEY_CONTROL_STATUS1)) {
			m_controlStatus1 = val;
		} else if (key.IsSameAs(KEY_CONTROL_STATUS2)) {
			m_controlStatus2 = val;
		} else if (key.IsSameAs(KEY_CONTROL_STATUS3)) {
			m_controlStatus3 = val;
		} else if (key.IsSameAs(KEY_CONTROL_STATUS4)) {
			m_controlStatus4 = val;
		} else if (key.IsSameAs(KEY_CONTROL_STATUS5)) {
			m_controlStatus5 = val;
		} else if (key.IsSameAs(KEY_CONTROL_COMMAND1)) {
			m_controlCommand1 = val;
		} else if (key.IsSameAs(KEY_CONTROL_COMMAND1_LINE)) {
			m_controlCommand1Line = val;
		} else if (key.IsSameAs(KEY_CONTROL_COMMAND2)) {
			m_controlCommand2 = val;
		} else if (key.IsSameAs(KEY_CONTROL_COMMAND2_LINE)) {
			m_controlCommand2Line = val;
		} else if (key.IsSameAs(KEY_CONTROL_COMMAND3)) {
			m_controlCommand3 = val;
		} else if (key.IsSameAs(KEY_CONTROL_COMMAND3_LINE)) {
			m_controlCommand3Line = val;
		} else if (key.IsSameAs(KEY_CONTROL_COMMAND4)) {
			m_controlCommand4 = val;
		} else if (key.IsSameAs(KEY_CONTROL_COMMAND4_LINE)) {
			m_controlCommand4Line = val;
		} else if (key.IsSameAs(KEY_CONTROL_COMMAND5)) {
			m_controlCommand5 = val;
		} else if (key.IsSameAs(KEY_CONTROL_COMMAND5_LINE)) {
			m_controlCommand5Line = val;
		} else if (key.IsSameAs(KEY_CONTROL_COMMAND6)) {
			m_controlCommand6 = val;
		} else if (key.IsSameAs(KEY_CONTROL_COMMAND6_LINE)) {
			m_controlCommand6Line = val;
		} else if (key.IsSameAs(KEY_CONTROL_OUTPUT1)) {
			m_controlOutput1 = val;
		} else if (key.IsSameAs(KEY_CONTROL_OUTPUT2)) {
			m_controlOutput2 = val;
		} else if (key.IsSameAs(KEY_CONTROL_OUTPUT3)) {
			m_controlOutput3 = val;
		} else if (key.IsSameAs(KEY_CONTROL_OUTPUT4)) {
			m_controlOutput4 = val;
		} else if (key.IsSameAs(KEY_CONTROLLER_TYPE)) {
			m_controllerType = val;
		} else if (key.IsSameAs(KEY_SERIAL_CONFIG)) {
			val.ToULong(&temp2);
			m_serialConfig = (unsigned int)temp2;
		} else if (key.IsSameAs(KEY_PTT_INVERT)) {
			val.ToLong(&temp1);
			m_pttInvert = temp1 == 1L;
		} else if (key.IsSameAs(KEY_ACTIVE_HANG_TIME)) {
			val.ToULong(&temp2);
			m_activeHangTime = (unsigned int)temp2;
		} else if (key.IsSameAs(KEY_OUTPUT1)) {
			val.ToLong(&temp1);
			m_output1 = temp1 == 1L;
		} else if (key.IsSameAs(KEY_OUTPUT2)) {
			val.ToLong(&temp1);
			m_output2 = temp1 == 1L;
		} else if (key.IsSameAs(KEY_OUTPUT3)) {
			val.ToLong(&temp1);
			m_output3 = temp1 == 1L;
		} else if (key.IsSameAs(KEY_OUTPUT4)) {
			val.ToLong(&temp1);
			m_output4 = temp1 == 1L;
		} else if (key.IsSameAs(KEY_LOGGING)) {
			val.ToLong(&temp1);
			m_logging = temp1 == 1L;
		} else if (key.IsSameAs(KEY_WINDOW_X)) {
			val.ToLong(&temp1);
			m_x = int(temp1);
		} else if (key.IsSameAs(KEY_WINDOW_Y)) {
			val.ToLong(&temp1);
			m_y = int(temp1);
		} else if (key.IsSameAs(KEY_DVAP_PORT)) {
			m_dvapPort = val;
		} else if (key.IsSameAs(KEY_DVAP_FREQUENCY)) {
			val.ToULong(&temp2);
			m_dvapFrequency = (unsigned int)temp2;
		} else if (key.IsSameAs(KEY_DVAP_POWER)) {
			val.ToLong(&temp1);
			m_dvapPower = int(temp1);
		} else if (key.IsSameAs(KEY_DVAP_SQUELCH)) {
			val.ToLong(&temp1);
			m_dvapSquelch = int(temp1);
		} else if (key.IsSameAs(KEY_GMSK_ADDRESS)) {
			val.ToULong(&temp2);
			m_gmskAddress = (unsigned int)temp2;
		} else if (key.IsSameAs(KEY_DVRPTR1_PORT)) {
			m_dvrptr1Port = val;
		} else if (key.IsSameAs(KEY_DVRPTR1_RXINVERT)) {
			val.ToLong(&temp1);
			m_dvrptr1RXInvert = temp1 == 1L;
		} else if (key.IsSameAs(KEY_DVRPTR1_TXINVERT)) {
			val.ToLong(&temp1);
			m_dvrptr1TXInvert = temp1 == 1L;
		} else if (key.IsSameAs(KEY_DVRPTR1_CHANNEL)) {
			val.ToLong(&temp1);
			m_dvrptr1Channel = temp1 == 1L;
		} else if (key.IsSameAs(KEY_DVRPTR1_MODLEVEL)) {
			val.ToULong(&temp2);
			m_dvrptr1ModLevel = (unsigned int)temp2;
		} else if (key.IsSameAs(KEY_DVRPTR1_TXDELAY)) {
			val.ToULong(&temp2);
			m_dvrptr1TXDelay = (unsigned int)temp2;
		} else if (key.IsSameAs(KEY_DVRPTR2_CONNECTION)) {
			val.ToLong(&temp1);
			m_dvrptr2Connection = CONNECTION_TYPE(temp1);
		} else if (key.IsSameAs(KEY_DVRPTR2_USBPORT)) {
			m_dvrptr2USBPort = val;
		} else if (key.IsSameAs(KEY_DVRPTR2_ADDRESS)) {
			m_dvrptr2Address = val;
		} else if (key.IsSameAs(KEY_DVRPTR2_PORT)) {
			val.ToULong(&temp2);
			m_dvrptr2Port = (unsigned int)temp2;
		} else if (key.IsSameAs(KEY_DVRPTR2_TXINVERT)) {
			val.ToLong(&temp1);
			m_dvrptr2TXInvert = temp1 == 1L;
		} else if (key.IsSameAs(KEY_DVRPTR2_MODLEVEL)) {
			val.ToULong(&temp2);
			m_dvrptr2ModLevel = (unsigned int)temp2;
		} else if (key.IsSameAs(KEY_DVRPTR2_TXDELAY)) {
			val.ToULong(&temp2);
			m_dvrptr2TXDelay = (unsigned int)temp2;
		} else if (key.IsSameAs(KEY_DVRPTR3_CONNECTION)) {
			val.ToLong(&temp1);
			m_dvrptr3Connection = CONNECTION_TYPE(temp1);
		} else if (key.IsSameAs(KEY_DVRPTR3_USBPORT)) {
			m_dvrptr3USBPort = val;
		} else if (key.IsSameAs(KEY_DVRPTR3_ADDRESS)) {
			m_dvrptr3Address = val;
		} else if (key.IsSameAs(KEY_DVRPTR3_PORT)) {
			val.ToULong(&temp2);
			m_dvrptr3Port = (unsigned int)temp2;
		} else if (key.IsSameAs(KEY_DVRPTR3_TXINVERT)) {
			val.ToLong(&temp1);
			m_dvrptr3TXInvert = temp1 == 1L;
		} else if (key.IsSameAs(KEY_DVRPTR3_MODLEVEL)) {
			val.ToULong(&temp2);
			m_dvrptr3ModLevel = (unsigned int)temp2;
		} else if (key.IsSameAs(KEY_DVRPTR3_TXDELAY)) {
			val.ToULong(&temp2);
			m_dvrptr3TXDelay = (unsigned int)temp2;
		} else if (key.IsSameAs(KEY_DVMEGA_PORT)) {
			m_dvmegaPort = val;
		} else if (key.IsSameAs(KEY_DVMEGA_VARIANT)) {
			val.ToLong(&temp1);
			m_dvmegaVariant = DVMEGA_VARIANT(temp1);
		} else if (key.IsSameAs(KEY_DVMEGA_RXINVERT)) {
			val.ToLong(&temp1);
			m_dvmegaRXInvert = temp1 == 1L;
		} else if (key.IsSameAs(KEY_DVMEGA_TXINVERT)) {
			val.ToLong(&temp1);
			m_dvmegaTXInvert = temp1 == 1L;
		} else if (key.IsSameAs(KEY_DVMEGA_TXDELAY)) {
			val.ToULong(&temp2);
			m_dvmegaTXDelay = (unsigned int)temp2;
		} else if (key.IsSameAs(KEY_DVMEGA_FREQUENCY)) {
			val.ToULong(&temp2);
			m_dvmegaFrequency = (unsigned int)temp2;
		} else if (key.IsSameAs(KEY_DVMEGA_POWER)) {
			val.ToULong(&temp2);
			m_dvmegaPower = (unsigned int)temp2;
		} else if (key.IsSameAs(KEY_SOUNDCARD_RXDEVICE)) {
			m_soundCardRXDevice = val;
		} else if (key.IsSameAs(KEY_SOUNDCARD_TXDEVICE)) {
			m_soundCardTXDevice = val;
		} else if (key.IsSameAs(KEY_SOUNDCARD_RXINVERT)) {
			val.ToLong(&temp1);
			m_soundCardRXInvert = temp1 == 1L;
		} else if (key.IsSameAs(KEY_SOUNDCARD_TXINVERT)) {
			val.ToLong(&temp1);
			m_soundCardTXInvert = temp1 == 1L;
		} else if (key.IsSameAs(KEY_SOUNDCARD_RXLEVEL)) {
			val.ToDouble(&temp3);
			m_soundCardRXLevel = wxFloat32(temp3);
		} else if (key.IsSameAs(KEY_SOUNDCARD_TXLEVEL)) {
			val.ToDouble(&temp3);
			m_soundCardTXLevel = wxFloat32(temp3);
		} else if (key.IsSameAs(KEY_SOUNDCARD_TXDELAY)) {
			val.ToULong(&temp2);
			m_soundCardTXDelay = (unsigned int)temp2;
		} else if (key.IsSameAs(KEY_SOUNDCARD_TXTAIL)) {
			val.ToULong(&temp2);
			m_soundCardTXTail = (unsigned int)temp2;
		} else if (key.IsSameAs(KEY_SPLIT_LOCALADDRESS)) {
			m_splitLocalAddress = val;
		} else if (key.IsSameAs(KEY_SPLIT_LOCALPORT)) {
			val.ToULong(&temp2);
			m_splitLocalPort = (unsigned int)temp2;
		} else if (key.IsSameAs(KEY_SPLIT_TIMEOUT)) {
			val.ToULong(&temp2);
			m_splitTimeout = (unsigned int)temp2;
		} else {
			for (unsigned int i = 0U; i < SPLIT_TX_COUNT; i++) {
				wxString name;
				name.Printf(wxT("%s%u"), KEY_SPLIT_TXNAME.c_str(), i);
				if (key.IsSameAs(name))
					splitTXName[i] = val;
			}

			for (unsigned int i = 0U; i < SPLIT_RX_COUNT; i++) {
				wxString name;
				name.Printf(wxT("%s%u"), KEY_SPLIT_RXNAME.c_str(), i);
				if (key.IsSameAs(name))
					splitRXName[i] = val;
			}
		}

		str = file.GetNextLine();
	}

	file.Close();

	// Move the array of wxStrings into the wxArrayString
	for (unsigned int i = 0U; i < SPLIT_TX_COUNT; i++)
		m_splitTXNames.Add(splitTXName[i]);
	for (unsigned int i = 0U; i < SPLIT_RX_COUNT; i++)
		m_splitRXNames.Add(splitRXName[i]);

	delete[] splitTXName;	
	delete[] splitRXName;
}

CDStarRepeaterConfig::~CDStarRepeaterConfig()
{
}

#endif

void CDStarRepeaterConfig::getCallsign(wxString& callsign, wxString& gateway, DSTAR_MODE& mode, ACK_TYPE& ack, bool& restriction, bool& rpt1Validation, bool& dtmfBlanking, bool& errorReply) const
{
	callsign       = m_callsign;
	gateway        = m_gateway;
	mode           = m_mode;
	ack            = m_ack;
	restriction    = m_restriction;
	rpt1Validation = m_rpt1Validation;
	dtmfBlanking   = m_dtmfBlanking;
	errorReply     = m_errorReply;
}

void CDStarRepeaterConfig::setCallsign(const wxString& callsign, const wxString& gateway, DSTAR_MODE mode, ACK_TYPE ack, bool restriction, bool rpt1Validation, bool dtmfBlanking, bool errorReply)
{
	m_callsign       = callsign;
	m_gateway        = gateway;
	m_mode           = mode;
	m_ack            = ack;
	m_restriction    = restriction;
	m_rpt1Validation = rpt1Validation;
	m_dtmfBlanking   = dtmfBlanking;
	m_errorReply     = errorReply;
}

void CDStarRepeaterConfig::getNetwork(wxString& gatewayAddress, unsigned int& gatewayPort, wxString& localAddress, unsigned int& localPort, wxString& name) const
{
	gatewayAddress = m_gatewayAddress;
	gatewayPort    = m_gatewayPort;
	localAddress   = m_localAddress;
	localPort      = m_localPort;
	name           = m_networkName;
}

void CDStarRepeaterConfig::setNetwork(const wxString& gatewayAddress, unsigned int gatewayPort, const wxString& localAddress, unsigned int localPort, const wxString& name)
{
	m_gatewayAddress = gatewayAddress;
	m_gatewayPort    = gatewayPort;
	m_localAddress   = localAddress;
	m_localPort      = localPort;
	m_networkName    = name;
}

void CDStarRepeaterConfig::getModem(wxString& type) const
{
	type = m_modemType;
}

void CDStarRepeaterConfig::setModem(const wxString& type)
{
	m_modemType = type;
}

void CDStarRepeaterConfig::getTimes(unsigned int& timeout, unsigned int& ackTime) const
{
	timeout = m_timeout;
	ackTime = m_ackTime;
}

void CDStarRepeaterConfig::setTimes(unsigned int timeout, unsigned int ackTime)
{
	m_timeout = timeout;
	m_ackTime = ackTime;
}

void CDStarRepeaterConfig::getBeacon(unsigned int& time, wxString& text, bool& voice, TEXT_LANG& language) const
{
	time     = m_beaconTime;
	text     = m_beaconText;
	voice    = m_beaconVoice;
	language = m_language;
}

void CDStarRepeaterConfig::setBeacon(unsigned int time, const wxString& text, bool voice, TEXT_LANG language)
{
	m_beaconTime  = time;
	m_beaconText  = text;
	m_beaconVoice = voice;
	m_language    = language;
}

void CDStarRepeaterConfig::getAnnouncement(bool& enabled, unsigned int& time, wxString& recordRPT1, wxString& recordRPT2, wxString& deleteRPT1, wxString& deleteRPT2) const
{
	enabled    = m_announcementEnabled;
	time       = m_announcementTime;
	recordRPT1 = m_announcementRecordRPT1;
	recordRPT2 = m_announcementRecordRPT2;
	deleteRPT1 = m_announcementDeleteRPT1;
	deleteRPT2 = m_announcementDeleteRPT2;
}

void CDStarRepeaterConfig::setAnnouncement(bool enabled, unsigned int time, const wxString& recordRPT1, const wxString& recordRPT2, const wxString& deleteRPT1, const wxString& deleteRPT2)
{
	m_announcementEnabled     = enabled;
	m_announcementTime        = time;
	m_announcementRecordRPT1  = recordRPT1;
	m_announcementRecordRPT2  = recordRPT2;
	m_announcementDeleteRPT1  = deleteRPT1;
	m_announcementDeleteRPT2  = deleteRPT2;
}

void CDStarRepeaterConfig::getControl(bool& enabled, wxString& rpt1Callsign, wxString& rpt2Callsign, wxString& shutdown, wxString& startup, wxString& status1, wxString& status2, wxString& status3, wxString& status4, wxString& status5, wxString& command1, wxString& command1Line, wxString& command2, wxString& command2Line, wxString& command3, wxString& command3Line, wxString& command4, wxString& command4Line, wxString& command5, wxString& command5Line, wxString& command6, wxString& command6Line, wxString& output1, wxString& output2, wxString& output3, wxString& output4) const
{
	enabled      = m_controlEnabled;
	rpt1Callsign = m_controlRpt1Callsign;
	rpt2Callsign = m_controlRpt2Callsign;
	shutdown     = m_controlShutdown;
	startup      = m_controlStartup;
	status1      = m_controlStatus1;
	status2      = m_controlStatus2;
	status3      = m_controlStatus3;
	status4      = m_controlStatus4;
	status5      = m_controlStatus5;
	command1     = m_controlCommand1;
	command1Line = m_controlCommand1Line;
	command2     = m_controlCommand2;
	command2Line = m_controlCommand2Line;
	command3     = m_controlCommand3;
	command3Line = m_controlCommand3Line;
	command4     = m_controlCommand4;
	command4Line = m_controlCommand4Line;
	command5     = m_controlCommand5;
	command5Line = m_controlCommand5Line;
	command6     = m_controlCommand6;
	command6Line = m_controlCommand6Line;
	output1      = m_controlOutput1;
	output2      = m_controlOutput2;
	output3      = m_controlOutput3;
	output4      = m_controlOutput4;
}

void CDStarRepeaterConfig::setControl(bool enabled, const wxString& rpt1Callsign, const wxString& rpt2Callsign, const wxString& shutdown, const wxString& startup, const wxString& status1, const wxString& status2, const wxString& status3, const wxString& status4, const wxString& status5, const wxString& command1, const wxString& command1Line, const wxString& command2, const wxString& command2Line, const wxString& command3, const wxString& command3Line, const wxString& command4, const wxString& command4Line, const wxString& command5, const wxString& command5Line, const wxString& command6, const wxString& command6Line, const wxString& output1, const wxString& output2, const wxString& output3, const wxString& output4)
{
	m_controlEnabled      = enabled;
	m_controlRpt1Callsign = rpt1Callsign;
	m_controlRpt2Callsign = rpt2Callsign;
	m_controlShutdown     = shutdown;
	m_controlStartup      = startup;
	m_controlStatus1      = status1;
	m_controlStatus2      = status2;
	m_controlStatus3      = status3;
	m_controlStatus4      = status4;
	m_controlStatus5      = status5;
	m_controlCommand1     = command1;
	m_controlCommand1Line = command1Line;
	m_controlCommand2     = command2;
	m_controlCommand2Line = command2Line;
	m_controlCommand3     = command3;
	m_controlCommand3Line = command3Line;
	m_controlCommand4     = command4;
	m_controlCommand4Line = command4Line;
	m_controlCommand5     = command5;
	m_controlCommand5Line = command5Line;
	m_controlCommand6     = command6;
	m_controlCommand6Line = command6Line;
	m_controlOutput1      = output1;
	m_controlOutput2      = output2;
	m_controlOutput3      = output3;
	m_controlOutput4      = output4;
}

void CDStarRepeaterConfig::getController(wxString& type, unsigned int& serialConfig, bool& pttInvert, unsigned int& activeHangTime) const
{
	type           = m_controllerType;
	serialConfig   = m_serialConfig;
	pttInvert      = m_pttInvert;
	activeHangTime = m_activeHangTime;
}

void CDStarRepeaterConfig::setController(const wxString& type, unsigned int serialConfig, bool pttInvert, unsigned int activeHangTime)
{
	m_controllerType = type;
	m_serialConfig   = serialConfig;
	m_pttInvert      = pttInvert;
	m_activeHangTime = activeHangTime;
}

void CDStarRepeaterConfig::getOutputs(bool& out1, bool& out2, bool& out3, bool& out4) const
{
	out1 = m_output1;
	out2 = m_output2;
	out3 = m_output3;
	out4 = m_output4;
}

void CDStarRepeaterConfig::setOutputs(bool out1, bool out2, bool out3, bool out4)
{
	m_output1 = out1;
	m_output2 = out2;
	m_output3 = out3;
	m_output4 = out4;
}

void CDStarRepeaterConfig::getLogging(bool& logging) const
{
	logging = m_logging;
}

void CDStarRepeaterConfig::setLogging(bool logging)
{
	m_logging = logging;
}

void CDStarRepeaterConfig::getPosition(int& x, int& y) const
{
	x = m_x;
	y = m_y;
}

void CDStarRepeaterConfig::setPosition(int x, int y)
{
	m_x = x;
	m_y = y;
}

void CDStarRepeaterConfig::getDVAP(wxString& port, unsigned int& frequency, int& power, int& squelch) const
{
	port      = m_dvapPort;
	frequency = m_dvapFrequency;
	power     = m_dvapPower;
	squelch   = m_dvapSquelch;
}

void CDStarRepeaterConfig::setDVAP(const wxString& port, unsigned int frequency, int power, int squelch)
{
	m_dvapPort      = port;
	m_dvapFrequency = frequency;
	m_dvapPower     = power;
	m_dvapSquelch   = squelch;
}

void CDStarRepeaterConfig::getGMSK(USB_INTERFACE& type, unsigned int& address) const
{
	type    = m_gmskInterface;
	address = m_gmskAddress;
}

void CDStarRepeaterConfig::setGMSK(USB_INTERFACE type, unsigned int address)
{
	m_gmskInterface = type;
	m_gmskAddress   = address;
}

void CDStarRepeaterConfig::getDVRPTR1(wxString& port, bool& rxInvert, bool& txInvert, bool& channel, unsigned int& modLevel, unsigned int& txDelay) const
{
	port     = m_dvrptr1Port;
	rxInvert = m_dvrptr1RXInvert;
	txInvert = m_dvrptr1TXInvert;
	channel  = m_dvrptr1Channel;
	modLevel = m_dvrptr1ModLevel;
	txDelay  = m_dvrptr1TXDelay;
}

void CDStarRepeaterConfig::setDVRPTR1(const wxString& port, bool rxInvert, bool txInvert, bool channel, unsigned int modLevel, unsigned int txDelay)
{
	m_dvrptr1Port     = port;
	m_dvrptr1RXInvert = rxInvert;
	m_dvrptr1TXInvert = txInvert;
	m_dvrptr1Channel  = channel;
	m_dvrptr1ModLevel = modLevel;
	m_dvrptr1TXDelay  = txDelay;
}

void CDStarRepeaterConfig::getDVRPTR2(CONNECTION_TYPE& connection, wxString& usbPort, wxString& address, unsigned int& port, bool& txInvert, unsigned int& modLevel, unsigned int& txDelay) const
{
	connection = m_dvrptr2Connection;
	usbPort    = m_dvrptr2USBPort;
	address    = m_dvrptr2Address;
	port       = m_dvrptr2Port;
	txInvert   = m_dvrptr2TXInvert;
	modLevel   = m_dvrptr2ModLevel;
	txDelay    = m_dvrptr2TXDelay;
}

void CDStarRepeaterConfig::setDVRPTR2(CONNECTION_TYPE connection, const wxString& usbPort, const wxString& address, unsigned int port, bool txInvert, unsigned int modLevel, unsigned int txDelay)
{
	m_dvrptr2Connection = connection;
	m_dvrptr2USBPort    = usbPort;
	m_dvrptr2Address    = address;
	m_dvrptr2Port       = port;
	m_dvrptr2TXInvert   = txInvert;
	m_dvrptr2ModLevel   = modLevel;
	m_dvrptr2TXDelay    = txDelay;
}

void CDStarRepeaterConfig::getDVRPTR3(CONNECTION_TYPE& connection, wxString& usbPort, wxString& address, unsigned int& port, bool& txInvert, unsigned int& modLevel, unsigned int& txDelay) const
{
	connection = m_dvrptr3Connection;
	usbPort    = m_dvrptr3USBPort;
	address    = m_dvrptr3Address;
	port       = m_dvrptr3Port;
	txInvert   = m_dvrptr3TXInvert;
	modLevel   = m_dvrptr3ModLevel;
	txDelay    = m_dvrptr3TXDelay;
}

void CDStarRepeaterConfig::setDVRPTR3(CONNECTION_TYPE connection, const wxString& usbPort, const wxString& address, unsigned int port, bool txInvert, unsigned int modLevel, unsigned int txDelay)
{
	m_dvrptr3Connection = connection;
	m_dvrptr3USBPort    = usbPort;
	m_dvrptr3Address    = address;
	m_dvrptr3Port       = port;
	m_dvrptr3TXInvert   = txInvert;
	m_dvrptr3ModLevel   = modLevel;
	m_dvrptr3TXDelay    = txDelay;
}

void CDStarRepeaterConfig::getDVMEGA(wxString& port, DVMEGA_VARIANT& variant, bool& rxInvert, bool& txInvert, unsigned int& txDelay, unsigned int& frequency, unsigned int& power) const
{
	port      = m_dvmegaPort;
	variant   = m_dvmegaVariant;
	rxInvert  = m_dvmegaRXInvert;
	txInvert  = m_dvmegaTXInvert;
	txDelay   = m_dvmegaTXDelay;
	frequency = m_dvmegaFrequency;
	power     = m_dvmegaPower;
}

void CDStarRepeaterConfig::setDVMEGA(const wxString& port, DVMEGA_VARIANT variant, bool rxInvert, bool txInvert, unsigned int txDelay, unsigned int frequency, unsigned int power)
{
	m_dvmegaPort      = port;
	m_dvmegaVariant   = variant;
	m_dvmegaRXInvert  = rxInvert;
	m_dvmegaTXInvert  = txInvert;
	m_dvmegaTXDelay   = txDelay;
	m_dvmegaFrequency = frequency;
	m_dvmegaPower     = power;
}

void CDStarRepeaterConfig::getSoundCard(wxString& rxDevice, wxString& txDevice, bool& rxInvert, bool& txInvert, wxFloat32& rxLevel, wxFloat32& txLevel, unsigned int& txDelay, unsigned int& txTail) const
{
	rxDevice = m_soundCardRXDevice;
	txDevice = m_soundCardTXDevice;
	rxInvert = m_soundCardRXInvert;
	txInvert = m_soundCardTXInvert;
	rxLevel  = m_soundCardRXLevel;
	txLevel  = m_soundCardTXLevel;
	txDelay  = m_soundCardTXDelay;
	txTail   = m_soundCardTXTail;
}

void CDStarRepeaterConfig::setSoundCard(const wxString& rxDevice, const wxString& txDevice, bool rxInvert, bool txInvert, wxFloat32 rxLevel, wxFloat32 txLevel, unsigned int txDelay, unsigned int txTail)
{
	m_soundCardRXDevice = rxDevice;
	m_soundCardTXDevice = txDevice;
	m_soundCardRXInvert = rxInvert;
	m_soundCardTXInvert = txInvert;
	m_soundCardRXLevel  = rxLevel;
	m_soundCardTXLevel  = txLevel;
	m_soundCardTXDelay  = txDelay;
	m_soundCardTXTail   = txTail;
}

void CDStarRepeaterConfig::getSplit(wxString& localAddress, unsigned int& localPort, wxArrayString& transmitterNames, wxArrayString& receiverNames, unsigned int& timeout) const
{
	localAddress     = m_splitLocalAddress;
	localPort        = m_splitLocalPort;
	transmitterNames = m_splitTXNames;
	receiverNames    = m_splitRXNames;
	timeout          = m_splitTimeout;
}

void CDStarRepeaterConfig::setSplit(const wxString& localAddress, unsigned int localPort, const wxArrayString& transmitterNames, const wxArrayString& receiverNames, unsigned int timeout)
{
	m_splitLocalAddress = localAddress;
	m_splitLocalPort    = localPort;
	m_splitTXNames      = transmitterNames;
	m_splitRXNames      = receiverNames;
	m_splitTimeout      = timeout;
}

bool CDStarRepeaterConfig::write()
{
#if defined(__WINDOWS__)
	m_config->Write(m_name + KEY_CALLSIGN, m_callsign);
	m_config->Write(m_name + KEY_GATEWAY, m_gateway);
	m_config->Write(m_name + KEY_MODE, long(m_mode));
	m_config->Write(m_name + KEY_ACK, long(m_ack));
	m_config->Write(m_name + KEY_RESTRICTION, m_restriction);
	m_config->Write(m_name + KEY_RPT1_VALIDATION, m_rpt1Validation);
	m_config->Write(m_name + KEY_DTMF_BLANKING, m_dtmfBlanking);
	m_config->Write(m_name + KEY_ERROR_REPLY, m_errorReply);
	m_config->Write(m_name + KEY_GATEWAY_ADDRESS, m_gatewayAddress);
	m_config->Write(m_name + KEY_GATEWAY_PORT, long(m_gatewayPort));
	m_config->Write(m_name + KEY_LOCAL_ADDRESS, m_localAddress);
	m_config->Write(m_name + KEY_LOCAL_PORT, long(m_localPort));
	m_config->Write(m_name + KEY_NETWORK_NAME, m_networkName);
	m_config->Write(m_name + KEY_MODEM_TYPE, m_modemType);
	m_config->Write(m_name + KEY_TIMEOUT, long(m_timeout));
	m_config->Write(m_name + KEY_ACK_TIME, long(m_ackTime));
	m_config->Write(m_name + KEY_BEACON_TIME, long(m_beaconTime));
	m_config->Write(m_name + KEY_BEACON_TEXT, m_beaconText);
	m_config->Write(m_name + KEY_BEACON_VOICE, m_beaconVoice);
	m_config->Write(m_name + KEY_LANGUAGE, long(m_language));
	m_config->Write(m_name + KEY_ANNOUNCEMENT_ENABLED, m_announcementEnabled);
	m_config->Write(m_name + KEY_ANNOUNCEMENT_TIME, long(m_announcementTime));
	m_config->Write(m_name + KEY_ANNOUNCEMENT_RECORD_RPT1, m_announcementRecordRPT1);
	m_config->Write(m_name + KEY_ANNOUNCEMENT_RECORD_RPT2, m_announcementRecordRPT2);
	m_config->Write(m_name + KEY_ANNOUNCEMENT_DELETE_RPT1, m_announcementDeleteRPT1);
	m_config->Write(m_name + KEY_ANNOUNCEMENT_DELETE_RPT2, m_announcementDeleteRPT2);
	m_config->Write(m_name + KEY_CONTROL_ENABLED, m_controlEnabled);
	m_config->Write(m_name + KEY_CONTROL_RPT1, m_controlRpt1Callsign);
	m_config->Write(m_name + KEY_CONTROL_RPT2, m_controlRpt2Callsign);
	m_config->Write(m_name + KEY_CONTROL_SHUTDOWN, m_controlShutdown);
	m_config->Write(m_name + KEY_CONTROL_STARTUP, m_controlStartup);
	m_config->Write(m_name + KEY_CONTROL_STATUS1, m_controlStatus1);
	m_config->Write(m_name + KEY_CONTROL_STATUS2, m_controlStatus2);
	m_config->Write(m_name + KEY_CONTROL_STATUS3, m_controlStatus3);
	m_config->Write(m_name + KEY_CONTROL_STATUS4, m_controlStatus4);
	m_config->Write(m_name + KEY_CONTROL_STATUS5, m_controlStatus5);
	m_config->Write(m_name + KEY_CONTROL_COMMAND1, m_controlCommand1);
	m_config->Write(m_name + KEY_CONTROL_COMMAND1_LINE, m_controlCommand1Line);
	m_config->Write(m_name + KEY_CONTROL_COMMAND2, m_controlCommand2);
	m_config->Write(m_name + KEY_CONTROL_COMMAND2_LINE, m_controlCommand2Line);
	m_config->Write(m_name + KEY_CONTROL_COMMAND3, m_controlCommand3);
	m_config->Write(m_name + KEY_CONTROL_COMMAND3_LINE, m_controlCommand3Line);
	m_config->Write(m_name + KEY_CONTROL_COMMAND4, m_controlCommand4);
	m_config->Write(m_name + KEY_CONTROL_COMMAND4_LINE, m_controlCommand4Line);
	m_config->Write(m_name + KEY_CONTROL_COMMAND5, m_controlCommand5);
	m_config->Write(m_name + KEY_CONTROL_COMMAND5_LINE, m_controlCommand5Line);
	m_config->Write(m_name + KEY_CONTROL_COMMAND6, m_controlCommand6);
	m_config->Write(m_name + KEY_CONTROL_COMMAND6_LINE, m_controlCommand6Line);
	m_config->Write(m_name + KEY_CONTROL_OUTPUT1, m_controlOutput1);
	m_config->Write(m_name + KEY_CONTROL_OUTPUT2, m_controlOutput2);
	m_config->Write(m_name + KEY_CONTROL_OUTPUT3, m_controlOutput3);
	m_config->Write(m_name + KEY_CONTROL_OUTPUT4, m_controlOutput4);
	m_config->Write(m_name + KEY_CONTROLLER_TYPE, m_controllerType);
	m_config->Write(m_name + KEY_SERIAL_CONFIG, long(m_serialConfig));
	m_config->Write(m_name + KEY_PTT_INVERT, m_pttInvert);
	m_config->Write(m_name + KEY_ACTIVE_HANG_TIME, long(m_activeHangTime));
	m_config->Write(m_name + KEY_OUTPUT1, m_output1);
	m_config->Write(m_name + KEY_OUTPUT2, m_output2);
	m_config->Write(m_name + KEY_OUTPUT3, m_output3);
	m_config->Write(m_name + KEY_OUTPUT4, m_output4);
	m_config->Write(m_name + KEY_LOGGING, m_logging);
	m_config->Write(m_name + KEY_WINDOW_X, long(m_x));
	m_config->Write(m_name + KEY_WINDOW_Y, long(m_y));

	m_config->Write(m_name + KEY_DVAP_PORT, m_dvapPort);
	m_config->Write(m_name + KEY_DVAP_FREQUENCY, long(m_dvapFrequency));
	m_config->Write(m_name + KEY_DVAP_POWER, long(m_dvapPower));
	m_config->Write(m_name + KEY_DVAP_SQUELCH, long(m_dvapSquelch));

	m_config->Write(m_name + KEY_GMSK_INTERFACE, long(m_gmskInterface));
	m_config->Write(m_name + KEY_GMSK_ADDRESS, long(m_gmskAddress));

	m_config->Write(m_name + KEY_DVRPTR1_PORT, m_dvrptr1Port);
	m_config->Write(m_name + KEY_DVRPTR1_RXINVERT, m_dvrptr1RXInvert);
	m_config->Write(m_name + KEY_DVRPTR1_TXINVERT, m_dvrptr1TXInvert);
	m_config->Write(m_name + KEY_DVRPTR1_CHANNEL, m_dvrptr1Channel);
	m_config->Write(m_name + KEY_DVRPTR1_MODLEVEL, long(m_dvrptr1ModLevel));
	m_config->Write(m_name + KEY_DVRPTR1_TXDELAY, long(m_dvrptr1TXDelay));

	m_config->Write(m_name + KEY_DVRPTR2_CONNECTION, long(m_dvrptr2Connection));
	m_config->Write(m_name + KEY_DVRPTR2_USBPORT, m_dvrptr2USBPort);
	m_config->Write(m_name + KEY_DVRPTR2_ADDRESS, m_dvrptr2Address);
	m_config->Write(m_name + KEY_DVRPTR2_PORT, long(m_dvrptr2Port));
	m_config->Write(m_name + KEY_DVRPTR2_TXINVERT, m_dvrptr2TXInvert);
	m_config->Write(m_name + KEY_DVRPTR2_MODLEVEL, long(m_dvrptr2ModLevel));
	m_config->Write(m_name + KEY_DVRPTR2_TXDELAY, long(m_dvrptr2TXDelay));

	m_config->Write(m_name + KEY_DVRPTR3_CONNECTION, long(m_dvrptr3Connection));
	m_config->Write(m_name + KEY_DVRPTR3_USBPORT,    m_dvrptr3USBPort);
	m_config->Write(m_name + KEY_DVRPTR3_ADDRESS,    m_dvrptr3Address);
	m_config->Write(m_name + KEY_DVRPTR3_PORT,       long(m_dvrptr3Port));
	m_config->Write(m_name + KEY_DVRPTR3_TXINVERT,   m_dvrptr3TXInvert);
	m_config->Write(m_name + KEY_DVRPTR3_MODLEVEL,   long(m_dvrptr3ModLevel));
	m_config->Write(m_name + KEY_DVRPTR3_TXDELAY,    long(m_dvrptr3TXDelay));

	m_config->Write(m_name + KEY_DVMEGA_PORT,      m_dvmegaPort);
	m_config->Write(m_name + KEY_DVMEGA_VARIANT,   long(m_dvmegaVariant));
	m_config->Write(m_name + KEY_DVMEGA_RXINVERT,  m_dvmegaRXInvert);
	m_config->Write(m_name + KEY_DVMEGA_TXINVERT,  m_dvmegaTXInvert);
	m_config->Write(m_name + KEY_DVMEGA_TXDELAY,   long(m_dvmegaTXDelay));
	m_config->Write(m_name + KEY_DVMEGA_FREQUENCY, long(m_dvmegaFrequency));
	m_config->Write(m_name + KEY_DVMEGA_POWER,     long(m_dvmegaPower));

	m_config->Write(m_name + KEY_SOUNDCARD_RXDEVICE, m_soundCardRXDevice);
	m_config->Write(m_name + KEY_SOUNDCARD_TXDEVICE, m_soundCardTXDevice);
	m_config->Write(m_name + KEY_SOUNDCARD_RXINVERT, m_soundCardRXInvert);
	m_config->Write(m_name + KEY_SOUNDCARD_TXINVERT, m_soundCardTXInvert);
	m_config->Write(m_name + KEY_SOUNDCARD_RXLEVEL,  double(m_soundCardRXLevel));
	m_config->Write(m_name + KEY_SOUNDCARD_TXLEVEL,  double(m_soundCardTXLevel));
	m_config->Write(m_name + KEY_SOUNDCARD_TXDELAY,  long(m_soundCardTXDelay));
	m_config->Write(m_name + KEY_SOUNDCARD_TXTAIL,   long(m_soundCardTXTail));

	m_config->Write(m_name + KEY_SPLIT_LOCALADDRESS, m_splitLocalAddress);
	m_config->Write(m_name + KEY_SPLIT_LOCALPORT,    long(m_splitLocalPort));

	for (unsigned int i = 0U; i < m_splitTXNames.GetCount(); i++) {
		wxString name;
		name.Printf(wxT("%s%u"), KEY_SPLIT_TXNAME.c_str(), i);

		m_config->Write(m_name + name, m_splitTXNames.Item(i));
	}

	for (unsigned int i = 0U; i < m_splitRXNames.GetCount(); i++) {
		wxString name;
		name.Printf(wxT("%s%u"), KEY_SPLIT_RXNAME.c_str(), i);

		m_config->Write(m_name + name, m_splitRXNames.Item(i));
	}

	m_config->Write(m_name + KEY_SPLIT_TIMEOUT,      long(m_splitTimeout));

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
	buffer.Printf(wxT("%s=%s"), KEY_GATEWAY.c_str(), m_gateway.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"), KEY_MODE.c_str(), int(m_mode)); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"), KEY_ACK.c_str(), int(m_ack)); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"), KEY_RESTRICTION.c_str(), m_restriction ? 1 : 0); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"), KEY_RPT1_VALIDATION.c_str(), m_rpt1Validation ? 1 : 0); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"), KEY_DTMF_BLANKING.c_str(), m_dtmfBlanking ? 1 : 0); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"), KEY_ERROR_REPLY.c_str(), m_errorReply ? 1 : 0); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_GATEWAY_ADDRESS.c_str(), m_gatewayAddress.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%u"), KEY_GATEWAY_PORT.c_str(), m_gatewayPort); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_LOCAL_ADDRESS.c_str(), m_localAddress.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%u"), KEY_LOCAL_PORT.c_str(), m_localPort); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_NETWORK_NAME.c_str(), m_networkName.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_MODEM_TYPE.c_str(), m_modemType.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%u"), KEY_TIMEOUT.c_str(), m_timeout); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%u"), KEY_ACK_TIME.c_str(), m_ackTime); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%u"), KEY_BEACON_TIME.c_str(), m_beaconTime); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_BEACON_TEXT.c_str(), m_beaconText.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"), KEY_BEACON_VOICE.c_str(), m_beaconVoice ? 1 : 0); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"), KEY_LANGUAGE.c_str(), int(m_language)); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"), KEY_ANNOUNCEMENT_ENABLED.c_str(), m_announcementEnabled ? 1 : 0); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%u"), KEY_ANNOUNCEMENT_TIME.c_str(), m_announcementTime); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_ANNOUNCEMENT_RECORD_RPT1.c_str(), m_announcementRecordRPT1.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_ANNOUNCEMENT_RECORD_RPT2.c_str(), m_announcementRecordRPT2.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_ANNOUNCEMENT_DELETE_RPT1.c_str(), m_announcementDeleteRPT1.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_ANNOUNCEMENT_DELETE_RPT2.c_str(), m_announcementDeleteRPT2.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"), KEY_CONTROL_ENABLED.c_str(), m_controlEnabled ? 1 : 0); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_CONTROL_RPT1.c_str(), m_controlRpt1Callsign.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_CONTROL_RPT2.c_str(), m_controlRpt2Callsign.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_CONTROL_SHUTDOWN.c_str(), m_controlShutdown.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_CONTROL_STARTUP.c_str(), m_controlStartup.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_CONTROL_STATUS1.c_str(), m_controlStatus1.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_CONTROL_STATUS2.c_str(), m_controlStatus2.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_CONTROL_STATUS3.c_str(), m_controlStatus3.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_CONTROL_STATUS4.c_str(), m_controlStatus4.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_CONTROL_STATUS5.c_str(), m_controlStatus5.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_CONTROL_COMMAND1.c_str(), m_controlCommand1.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_CONTROL_COMMAND1_LINE.c_str(), m_controlCommand1Line.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_CONTROL_COMMAND2.c_str(), m_controlCommand2.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_CONTROL_COMMAND2_LINE.c_str(), m_controlCommand2Line.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_CONTROL_COMMAND3.c_str(), m_controlCommand3.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_CONTROL_COMMAND3_LINE.c_str(), m_controlCommand3Line.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_CONTROL_COMMAND4.c_str(), m_controlCommand4.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_CONTROL_COMMAND4_LINE.c_str(), m_controlCommand4Line.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_CONTROL_COMMAND5.c_str(), m_controlCommand5.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_CONTROL_COMMAND5_LINE.c_str(), m_controlCommand5Line.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_CONTROL_COMMAND6.c_str(), m_controlCommand6.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_CONTROL_COMMAND6_LINE.c_str(), m_controlCommand6Line.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_CONTROL_OUTPUT1.c_str(), m_controlOutput1.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_CONTROL_OUTPUT2.c_str(), m_controlOutput2.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_CONTROL_OUTPUT3.c_str(), m_controlOutput3.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_CONTROL_OUTPUT4.c_str(), m_controlOutput4.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_CONTROLLER_TYPE.c_str(), m_controllerType.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%u"), KEY_SERIAL_CONFIG.c_str(), m_serialConfig); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"), KEY_PTT_INVERT.c_str(), m_pttInvert ? 1 : 0); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%u"), KEY_ACTIVE_HANG_TIME.c_str(), m_activeHangTime); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"), KEY_OUTPUT1.c_str(), m_output1 ? 1 : 0); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"), KEY_OUTPUT2.c_str(), m_output2 ? 1 : 0); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"), KEY_OUTPUT3.c_str(), m_output3 ? 1 : 0); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"), KEY_OUTPUT4.c_str(), m_output4 ? 1 : 0); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"), KEY_LOGGING.c_str(), m_logging ? 1 : 0); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"), KEY_WINDOW_X.c_str(), m_x); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"), KEY_WINDOW_Y.c_str(), m_y); file.AddLine(buffer);

	buffer.Printf(wxT("%s=%s"), KEY_DVAP_PORT.c_str(),      m_dvapPort.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%u"), KEY_DVAP_FREQUENCY.c_str(), m_dvapFrequency); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"), KEY_DVAP_POWER.c_str(),     m_dvapPower); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"), KEY_DVAP_SQUELCH.c_str(),   m_dvapSquelch); file.AddLine(buffer);

	buffer.Printf(wxT("%s=%u"), KEY_GMSK_ADDRESS.c_str(), m_gmskAddress); file.AddLine(buffer);

	buffer.Printf(wxT("%s=%s"), KEY_DVRPTR1_PORT.c_str(),     m_dvrptr1Port.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"), KEY_DVRPTR1_RXINVERT.c_str(), m_dvrptr1RXInvert ? 1 : 0); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"), KEY_DVRPTR1_TXINVERT.c_str(), m_dvrptr1TXInvert ? 1 : 0); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"), KEY_DVRPTR1_CHANNEL.c_str(),  m_dvrptr1Channel ? 1 : 0); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%u"), KEY_DVRPTR1_MODLEVEL.c_str(), m_dvrptr1ModLevel); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%u"), KEY_DVRPTR1_TXDELAY.c_str(),  m_dvrptr1TXDelay); file.AddLine(buffer);

	buffer.Printf(wxT("%s=%d"), KEY_DVRPTR2_CONNECTION.c_str(), int(m_dvrptr2Connection)); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_DVRPTR2_USBPORT.c_str(),    m_dvrptr2USBPort.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_DVRPTR2_ADDRESS.c_str(),    m_dvrptr2Address.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%u"), KEY_DVRPTR2_PORT.c_str(),       m_dvrptr2Port); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"), KEY_DVRPTR2_TXINVERT.c_str(),   m_dvrptr2TXInvert ? 1 : 0); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%u"), KEY_DVRPTR2_MODLEVEL.c_str(),   m_dvrptr2ModLevel); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%u"), KEY_DVRPTR2_TXDELAY.c_str(),    m_dvrptr2TXDelay); file.AddLine(buffer);

	buffer.Printf(wxT("%s=%d"), KEY_DVRPTR3_CONNECTION.c_str(), int(m_dvrptr3Connection)); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_DVRPTR3_USBPORT.c_str(),    m_dvrptr3USBPort.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_DVRPTR3_ADDRESS.c_str(),    m_dvrptr3Address.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%u"), KEY_DVRPTR3_PORT.c_str(),       m_dvrptr3Port); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"), KEY_DVRPTR3_TXINVERT.c_str(),   m_dvrptr3TXInvert ? 1 : 0); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%u"), KEY_DVRPTR3_MODLEVEL.c_str(),   m_dvrptr3ModLevel); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%u"), KEY_DVRPTR3_TXDELAY.c_str(),    m_dvrptr3TXDelay); file.AddLine(buffer);

	buffer.Printf(wxT("%s=%s"), KEY_DVMEGA_PORT.c_str(),      m_dvmegaPort.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"), KEY_DVMEGA_VARIANT.c_str(),   int(m_dvmegaVariant)); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"), KEY_DVMEGA_RXINVERT.c_str(),  m_dvmegaRXInvert ? 1 : 0); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"), KEY_DVMEGA_TXINVERT.c_str(),  m_dvmegaTXInvert ? 1 : 0); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%u"), KEY_DVMEGA_TXDELAY.c_str(),   m_dvmegaTXDelay); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%u"), KEY_DVMEGA_FREQUENCY.c_str(), m_dvmegaFrequency); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%u"), KEY_DVMEGA_POWER.c_str(),     m_dvmegaPower); file.AddLine(buffer);

	buffer.Printf(wxT("%s=%s"),   KEY_SOUNDCARD_RXDEVICE.c_str(), m_soundCardRXDevice.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"),   KEY_SOUNDCARD_TXDEVICE.c_str(), m_soundCardTXDevice.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"),   KEY_SOUNDCARD_RXINVERT.c_str(), m_soundCardRXInvert ? 1 : 0); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"),   KEY_SOUNDCARD_TXINVERT.c_str(), m_soundCardTXInvert ? 1 : 0); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%.4f"), KEY_SOUNDCARD_RXLEVEL.c_str(),  m_soundCardRXLevel); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%.4f"), KEY_SOUNDCARD_TXLEVEL.c_str(),  m_soundCardTXLevel); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%u"),   KEY_SOUNDCARD_TXDELAY.c_str(),  m_soundCardTXDelay); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%u"),   KEY_SOUNDCARD_TXTAIL.c_str(),   m_soundCardTXTail);  file.AddLine(buffer);

	buffer.Printf(wxT("%s=%s"), KEY_SPLIT_LOCALADDRESS.c_str(), m_splitLocalAddress.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%u"), KEY_SPLIT_LOCALPORT.c_str(),    m_splitLocalPort); file.AddLine(buffer);

	for (unsigned int i = 0U; i < m_splitTXNames.GetCount(); i++) {
		wxString name;
		name.Printf(wxT("%s%u"), KEY_SPLIT_TXNAME.c_str(), i);
		buffer.Printf(wxT("%s=%s"), name.c_str(), m_splitTXNames.Item(i).c_str());
		file.AddLine(buffer);
	}

	for (unsigned int i = 0U; i < m_splitRXNames.GetCount(); i++) {
		wxString name;
		name.Printf(wxT("%s%u"), KEY_SPLIT_RXNAME.c_str(), i);
		buffer.Printf(wxT("%s=%s"), name.c_str(), m_splitRXNames.Item(i).c_str());
		file.AddLine(buffer);
	}

	buffer.Printf(wxT("%s=%u"), KEY_SPLIT_TIMEOUT.c_str(),      m_splitTimeout); file.AddLine(buffer);

	bool ret = file.Write();
	if (!ret) {
		file.Close();
		wxLogError(wxT("Cannot write the config file - %s"), m_fileName.GetFullPath().c_str());
		return false;
	}

	file.Close();

	return true;
}
