/*
 *   Copyright (C) 2009-2015 by Jonathan Naylor G4KLX
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

#include "AnalogueRepeaterConfig.h"
#include "AnalogueRepeaterDefs.h"

const wxString KEY_OPEN_ID_SOURCE      = wxT("openIdSource");
const wxString KEY_OPEN_ID             = wxT("openId");
const wxString KEY_CLOSE_ID_SOURCE     = wxT("closeIdSource");
const wxString KEY_CLOSE_ID            = wxT("closeId");
const wxString KEY_BEACON1_SOURCE      = wxT("beacon1Source");
const wxString KEY_BEACON1             = wxT("beacon1");
const wxString KEY_BEACON2_SOURCE      = wxT("beacon2Source");
const wxString KEY_BEACON2             = wxT("beacon2");
const wxString KEY_ID_SPEED            = wxT("idSpeed");
const wxString KEY_ID_FREQ             = wxT("idFreq");
const wxString KEY_ID_LEVEL1           = wxT("idLevel1");
const wxString KEY_ID_LEVEL2           = wxT("idLevel2");

const wxString KEY_ACK_RADIO_SOURCE    = wxT("ackRadioSource");
const wxString KEY_ACK_RADIO           = wxT("ackRadio");
const wxString KEY_ACK_EXTERNAL_SOURCE = wxT("ackExternalSource");
const wxString KEY_ACK_EXTERNAL        = wxT("ackExternal");
const wxString KEY_ACK_BATTERY_SOURCE  = wxT("ackBatterySource");
const wxString KEY_ACK_BATTERY         = wxT("ackBattery");
const wxString KEY_ACK_SPEED           = wxT("ackSpeed");
const wxString KEY_ACK_FREQ            = wxT("ackFreq");
const wxString KEY_ACK_LEVEL           = wxT("ackLevel");
const wxString KEY_ACK_DELAY           = wxT("ackDelay");
const wxString KEY_ACK_MIN             = wxT("ackMin");

const wxString KEY_CALLSIGNTIME        = wxT("callsignTime");
const wxString KEY_TIMEOUT             = wxT("timeout");
const wxString KEY_LOCKOUTTIME         = wxT("lockoutTime");
const wxString KEY_HANGTIME            = wxT("hangTime");
const wxString KEY_LATCHTIME           = wxT("latchTime");

const wxString KEY_TONEBURST_ENABLE    = wxT("toneburstEnable");
const wxString KEY_TONEBURST_THRESHOLD = wxT("toneburstThresh");
const wxString KEY_CTCSS_FREQ          = wxT("ctcssFreq");
const wxString KEY_CTCSS_INTERNAL      = wxT("ctcssInternal");
const wxString KEY_CTCSS_THRESHOLD     = wxT("ctcssThresh");
const wxString KEY_CTCSS_LEVEL         = wxT("ctcssLevel");
const wxString KEY_CTCSS_HANG_TIME     = wxT("ctcssHangTime");
const wxString KEY_CTCSS_OUTPUT        = wxT("ctcssOutput");

const wxString KEY_CALLSIGN_AT_START    = wxT("callAtStart");
const wxString KEY_CALLSIGN_START_DELAY = wxT("callStartDelay");
const wxString KEY_CALLSIGN_AT_END      = wxT("callAtEnd");
const wxString KEY_TIMEOUT_TYPE         = wxT("timeoutType");
const wxString KEY_CALLSIGN_HOLDOFF     = wxT("callHoldoff");

const wxString KEY_RADIO_READ_DEVICE   = wxT("radioReadDevice");
const wxString KEY_RADIO_WRITE_DEVICE  = wxT("radioWriteDevice");
const wxString KEY_RADIO_AUDIO_DELAY   = wxT("radioAudioDelay");
const wxString KEY_RADIO_DEEMPHASIS    = wxT("radioDeEmphasis");
const wxString KEY_RADIO_PREEMPHASIS   = wxT("radioPreEmphasis");
const wxString KEY_RADIO_VOGAD         = wxT("radioVOGAD");

const wxString KEY_EXTERNAL_MODE             = wxT("externalMode");
const wxString KEY_EXTERNAL_READ_DEVICE      = wxT("externalReadDevice");
const wxString KEY_EXTERNAL_WRITE_DEVICE     = wxT("externalWriteDevice");
const wxString KEY_EXTERNAL_AUDIO_DELAY      = wxT("externalAudioDelay");
const wxString KEY_EXTERNAL_DEEMPHASIS       = wxT("externalDeEmphasis");
const wxString KEY_EXTERNAL_PREEMPHASIS      = wxT("externalPreEmphasis");
const wxString KEY_EXTERNAL_VOGAD            = wxT("externalVOGAD");
const wxString KEY_EXTERNAL_INTERFACE_DEVICE = wxT("externalInterfaceDevice");
const wxString KEY_EXTERNAL_INTERFACE_TX_PIN = wxT("externalInterfaceTXPin");
const wxString KEY_EXTERNAL_INTERFACE_RX_PIN = wxT("externalInterfaceRXPin");
const wxString KEY_EXTERNAL_BACKGROUND       = wxT("externalBackground");

const wxString KEY_IFACE_TYPE         = wxT("interfaceType");
const wxString KEY_IFACE_CONFIG       = wxT("interfaceConfig");
const wxString KEY_PTT_DELAY          = wxT("pttDelay");
const wxString KEY_SQUELCH_DELAY      = wxT("squelchDelay");
const wxString KEY_PTT_INVERT         = wxT("pttInvert");
const wxString KEY_SQUELCH_INVERT     = wxT("squelchInvert");

const wxString KEY_DTMF_RADIO         = wxT("dtmfRadio");
const wxString KEY_DTMF_EXTERNAL      = wxT("dtmfExternal");
const wxString KEY_DTMF_SHUTDOWN      = wxT("dtmfShutdown");
const wxString KEY_DTMF_STARTUP       = wxT("dtmfStartup");
const wxString KEY_DTMF_TIMEOUT       = wxT("dtmfTimeout");
const wxString KEY_DTMF_TIMERESET     = wxT("dtmfTimeReset");
const wxString KEY_DTMF_COMMAND1      = wxT("dtmfCommand1");
const wxString KEY_DTMF_COMMAND1_LINE = wxT("dtmfCommand1Line");
const wxString KEY_DTMF_COMMAND2      = wxT("dtmfCommand2");
const wxString KEY_DTMF_COMMAND2_LINE = wxT("dtmfCommand2Line");
const wxString KEY_DTMF_OUTPUT1       = wxT("dtmfOutput1");
const wxString KEY_DTMF_OUTPUT2       = wxT("dtmfOutput2");
const wxString KEY_DTMF_OUTPUT3       = wxT("dtmfOutput3");
const wxString KEY_DTMF_OUTPUT4       = wxT("dtmfOutput4");
const wxString KEY_DTMF_THRESHOLD     = wxT("dtmfThreshold");

const wxString KEY_ACTIVE_HANG_TIME = wxT("activeHangTime");

const wxString KEY_OUTPUT1          = wxT("output1");
const wxString KEY_OUTPUT2          = wxT("output2");
const wxString KEY_OUTPUT3          = wxT("output3");
const wxString KEY_OUTPUT4          = wxT("output4");

const wxString KEY_WINDOW_X         = wxT("windowX");
const wxString KEY_WINDOW_Y         = wxT("windowY");

const AUDIO_SOURCE DEFAULT_OPEN_ID_SOURCE   = AS_CW_KEYER;
const wxString     DEFAULT_OPEN_ID          = wxT("GB3IN B");
const AUDIO_SOURCE DEFAULT_CLOSE_ID_SOURCE  = AS_CW_KEYER;
const wxString     DEFAULT_CLOSE_ID         = wxT("GB3IN B");
const AUDIO_SOURCE DEFAULT_BEACON1_SOURCE   = AS_CW_KEYER;
const wxString     DEFAULT_BEACON1          = wxT("GB3IN B");
const AUDIO_SOURCE DEFAULT_BEACON2_SOURCE   = AS_CW_KEYER;
const wxString     DEFAULT_BEACON2          = wxT("GB3IN B  IO93GD");
const unsigned int DEFAULT_ID_SPEED         = 25U;
const unsigned int DEFAULT_ID_FREQ          = 1000U;
const wxFloat32    DEFAULT_ID_LEVEL1        = 0.8F;
const wxFloat32    DEFAULT_ID_LEVEL2        = 0.5F;

const AUDIO_SOURCE DEFAULT_ACK_RADIO_SOURCE    = AS_CW_KEYER;
const wxString     DEFAULT_ACK_RADIO           = wxT("K");
const AUDIO_SOURCE DEFAULT_ACK_EXTERNAL_SOURCE = AS_CW_KEYER;
const wxString     DEFAULT_ACK_EXTERNAL        = wxT("N");
const AUDIO_SOURCE DEFAULT_ACK_BATTERY_SOURCE  = AS_CW_KEYER;
const wxString     DEFAULT_ACK_BATTERY         = wxT("B");
const unsigned int DEFAULT_ACK_SPEED           = 25U;
const unsigned int DEFAULT_ACK_FREQ            = 1750U;
const wxFloat32    DEFAULT_ACK_LEVEL           = 0.8F;
const unsigned int DEFAULT_ACK_DELAY           = 1000U;
const unsigned int DEFAULT_ACK_MIN             = 5000U;

const unsigned int DEFAULT_CALLSIGNTIME     = 600U;
const unsigned int DEFAULT_TIMEOUT          = 120U;
const unsigned int DEFAULT_LOCKOUTTIME      = 30U;
const unsigned int DEFAULT_HANGTIME         = 7U;
const unsigned int DEFAULT_LATCHTIME        = 3U;

const bool                  DEFAULT_TONEBURST_ENABLE = true;
const wxFloat32             DEFAULT_TONEBURST_THRESH = 0.5F;
const wxFloat32             DEFAULT_CTCSS_FREQ       = 71.9F;
const bool                  DEFAULT_CTCSS_INTERNAL   = true;
const wxFloat32             DEFAULT_CTCSS_THRESH     = 0.5F;
const wxFloat32             DEFAULT_CTCSS_LEVEL      = 0.5F;
const unsigned int          DEFAULT_CTCSS_HANG_TIME  = 0U;
const ANALOGUE_CTCSS_OUTPUT DEFAULT_CTCSS_OUTPUT     = ACO_WHEN_OPEN;

const ANALOGUE_CALLSIGN_START   DEFAULT_CALLSIGN_AT_START    = ACS_NONE;
const unsigned int              DEFAULT_CALLSIGN_START_DELAY = 0U;
const bool                      DEFAULT_CALLSIGN_AT_END      = true;
const ANALOGUE_TIMEOUT_TYPE     DEFAULT_TIMEOUT_TYPE         = ATT_DL;
const ANALOGUE_CALLSIGN_HOLDOFF DEFAULT_CALLSIGN_HOLDOFF     = ACH_NONE;

const wxString     DEFAULT_RADIO_READ_DEVICE    = wxEmptyString;
const wxString     DEFAULT_RADIO_WRITE_DEVICE   = wxEmptyString;
const unsigned int DEFAULT_RADIO_AUDIO_DELAY    = 10U;
const bool         DEFAULT_RADIO_DEEMPHASIS     = false;
const bool         DEFAULT_RADIO_PREEMPHASIS    = false;
const bool         DEFAULT_RADIO_VOGAD          = false;

const ANALOGUE_EXTERNAL_MODE DEFAULT_EXTERNAL_MODE             = AEM_DISABLED;
const wxString               DEFAULT_EXTERNAL_READ_DEVICE      = wxEmptyString;
const wxString               DEFAULT_EXTERNAL_WRITE_DEVICE     = wxEmptyString;
const unsigned int           DEFAULT_EXTERNAL_AUDIO_DELAY      = 0U;
const bool                   DEFAULT_EXTERNAL_DEEMPHASIS       = false;
const bool                   DEFAULT_EXTERNAL_PREEMPHASIS      = false;
const bool                   DEFAULT_EXTERNAL_VOGAD            = false;
const wxString               DEFAULT_EXTERNAL_INTERFACE_DEVICE = wxEmptyString;
const SERIALPIN              DEFAULT_EXTERNAL_INTERFACE_TX_PIN = SERIAL_DTR;
const SERIALPIN              DEFAULT_EXTERNAL_INTERFACE_RX_PIN = SERIAL_CD;
const bool                   DEFAULT_EXTERNAL_BACKGROUND       = false;

const wxString     DEFAULT_IFACE_TYPE         = wxEmptyString;
const unsigned int DEFAULT_IFACE_CONFIG       = 1U;
const unsigned int DEFAULT_PTT_DELAY          = 5U;
const unsigned int DEFAULT_SQUELCH_DELAY      = 0U;
const bool         DEFAULT_PTT_INVERT         = false;
const bool         DEFAULT_SQUELCH_INVERT     = false;

const bool      DEFAULT_DTMF_RADIO         = false;
const bool      DEFAULT_DTMF_EXTERNAL      = false;
const wxString  DEFAULT_DTMF_SHUTDOWN      = wxEmptyString;
const wxString  DEFAULT_DTMF_STARTUP       = wxEmptyString;
const wxString  DEFAULT_DTMF_TIMEOUT       = wxEmptyString;
const wxString  DEFAULT_DTMF_TIMERESET     = wxEmptyString;
const wxString  DEFAULT_DTMF_COMMAND1      = wxEmptyString;
const wxString  DEFAULT_DTMF_COMMAND1_LINE = wxEmptyString;
const wxString  DEFAULT_DTMF_COMMAND2      = wxEmptyString;
const wxString  DEFAULT_DTMF_COMMAND2_LINE = wxEmptyString;
const wxString  DEFAULT_DTMF_OUTPUT1       = wxEmptyString;
const wxString  DEFAULT_DTMF_OUTPUT2       = wxEmptyString;
const wxString  DEFAULT_DTMF_OUTPUT3       = wxEmptyString;
const wxString  DEFAULT_DTMF_OUTPUT4       = wxEmptyString;
const wxFloat32 DEFAULT_DTMF_THRESHOLD     = 0.5F;

const bool      DEFAULT_OUTPUT1            = false;
const bool      DEFAULT_OUTPUT2            = false;
const bool      DEFAULT_OUTPUT3            = false;
const bool      DEFAULT_OUTPUT4            = false;

const unsigned int DEFAULT_ACTIVE_HANG_TIME   = 0U;

const int       DEFAULT_WINDOW_X           = -1;
const int       DEFAULT_WINDOW_Y           = -1;


#if defined(__WINDOWS__)

CAnalogueRepeaterConfig::CAnalogueRepeaterConfig(wxConfigBase* config, const wxString& name) :
m_config(config),
m_name(wxT("/")),
m_openIdSource(DEFAULT_OPEN_ID_SOURCE),
m_openId(DEFAULT_OPEN_ID),
m_closeIdSource(DEFAULT_CLOSE_ID_SOURCE),
m_closeId(DEFAULT_CLOSE_ID),
m_beacon1Source(DEFAULT_BEACON1_SOURCE),
m_beacon1(DEFAULT_BEACON1),
m_beacon2Source(DEFAULT_BEACON2_SOURCE),
m_beacon2(DEFAULT_BEACON2),
m_idSpeed(DEFAULT_ID_SPEED),
m_idFreq(DEFAULT_ID_FREQ),
m_idLevel1(DEFAULT_ID_LEVEL1),
m_idLevel2(DEFAULT_ID_LEVEL2),
m_ackRadioSource(DEFAULT_ACK_RADIO_SOURCE),
m_ackRadio(DEFAULT_ACK_RADIO),
m_ackExternalSource(DEFAULT_ACK_EXTERNAL_SOURCE),
m_ackExternal(DEFAULT_ACK_EXTERNAL),
m_ackBatterySource(DEFAULT_ACK_BATTERY_SOURCE),
m_ackBattery(DEFAULT_ACK_BATTERY),
m_ackSpeed(DEFAULT_ACK_SPEED),
m_ackFreq(DEFAULT_ACK_FREQ),
m_ackLevel(DEFAULT_ACK_LEVEL),
m_ackDelay(DEFAULT_ACK_DELAY),
m_ackMin(DEFAULT_ACK_MIN),
m_callsignTime(DEFAULT_CALLSIGNTIME),
m_timeout(DEFAULT_TIMEOUT),
m_lockoutTime(DEFAULT_LOCKOUTTIME),
m_hangTime(DEFAULT_HANGTIME),
m_latchTime(DEFAULT_LATCHTIME),
m_toneburstEnable(DEFAULT_TONEBURST_ENABLE),
m_toneburstThreshold(DEFAULT_TONEBURST_THRESH),
m_ctcssFreq(DEFAULT_CTCSS_FREQ),
m_ctcssInternal(DEFAULT_CTCSS_INTERNAL),
m_ctcssThresh(DEFAULT_CTCSS_THRESH),
m_ctcssLevel(DEFAULT_CTCSS_LEVEL),
m_ctcssHangTime(DEFAULT_CTCSS_HANG_TIME),
m_ctcssOutput(DEFAULT_CTCSS_OUTPUT),
m_callAtStart(DEFAULT_CALLSIGN_AT_START),
m_callStartDelay(DEFAULT_CALLSIGN_START_DELAY),
m_callAtEnd(DEFAULT_CALLSIGN_AT_END),
m_timeoutType(DEFAULT_TIMEOUT_TYPE),
m_callHoldoff(DEFAULT_CALLSIGN_HOLDOFF),
m_radioReadDevice(DEFAULT_RADIO_READ_DEVICE),
m_radioWriteDevice(DEFAULT_RADIO_WRITE_DEVICE),
m_radioDelay(DEFAULT_RADIO_AUDIO_DELAY),
m_radioDeEmphasis(DEFAULT_RADIO_DEEMPHASIS),
m_radioPreEmphasis(DEFAULT_RADIO_PREEMPHASIS),
m_radioVOGAD(DEFAULT_RADIO_VOGAD),
m_externalMode(DEFAULT_EXTERNAL_MODE),
m_externalReadDevice(DEFAULT_EXTERNAL_READ_DEVICE),
m_externalWriteDevice(DEFAULT_EXTERNAL_WRITE_DEVICE),
m_externalDelay(DEFAULT_EXTERNAL_AUDIO_DELAY),
m_externalDeEmphasis(DEFAULT_EXTERNAL_DEEMPHASIS),
m_externalPreEmphasis(DEFAULT_EXTERNAL_PREEMPHASIS),
m_externalVOGAD(DEFAULT_EXTERNAL_VOGAD),
m_externalInterfaceDevice(DEFAULT_EXTERNAL_INTERFACE_DEVICE),
m_externalInterfaceTXPin(DEFAULT_EXTERNAL_INTERFACE_TX_PIN),
m_externalInterfaceRXPin(DEFAULT_EXTERNAL_INTERFACE_RX_PIN),
m_externalBackground(DEFAULT_EXTERNAL_BACKGROUND),
m_interfaceType(DEFAULT_IFACE_TYPE),
m_interfaceConfig(DEFAULT_IFACE_CONFIG),
m_pttDelay(DEFAULT_PTT_DELAY),
m_squelchDelay(DEFAULT_SQUELCH_DELAY),
m_pttInvert(DEFAULT_PTT_INVERT),
m_squelchInvert(DEFAULT_SQUELCH_INVERT),
m_dtmfRadio(DEFAULT_DTMF_RADIO),
m_dtmfExternal(DEFAULT_DTMF_EXTERNAL),
m_dtmfShutdown(DEFAULT_DTMF_SHUTDOWN),
m_dtmfStartup(DEFAULT_DTMF_STARTUP),
m_dtmfTimeout(DEFAULT_DTMF_TIMEOUT),
m_dtmfTimeReset(DEFAULT_DTMF_TIMERESET),
m_dtmfCommand1(DEFAULT_DTMF_COMMAND1),
m_dtmfCommand1Line(DEFAULT_DTMF_COMMAND1_LINE),
m_dtmfCommand2(DEFAULT_DTMF_COMMAND2),
m_dtmfCommand2Line(DEFAULT_DTMF_COMMAND2_LINE),
m_dtmfOutput1(DEFAULT_DTMF_OUTPUT1),
m_dtmfOutput2(DEFAULT_DTMF_OUTPUT2),
m_dtmfOutput3(DEFAULT_DTMF_OUTPUT3),
m_dtmfOutput4(DEFAULT_DTMF_OUTPUT4),
m_dtmfThreshold(DEFAULT_DTMF_THRESHOLD),
m_activeHangTime(DEFAULT_ACTIVE_HANG_TIME),
m_output1(DEFAULT_OUTPUT1),
m_output2(DEFAULT_OUTPUT2),
m_output3(DEFAULT_OUTPUT3),
m_output4(DEFAULT_OUTPUT4),
m_x(DEFAULT_WINDOW_X),
m_y(DEFAULT_WINDOW_Y)
{
	wxASSERT(config != NULL);

	if (!name.IsEmpty())
		m_name = wxT("/") + name + wxT("/");

	long temp1;
	double temp2;

	m_config->Read(m_name + KEY_OPEN_ID_SOURCE, &temp1, long(DEFAULT_OPEN_ID_SOURCE));
	m_openIdSource = AUDIO_SOURCE(temp1);

	m_config->Read(m_name + KEY_OPEN_ID, &m_openId, DEFAULT_OPEN_ID);

	m_config->Read(m_name + KEY_CLOSE_ID_SOURCE, &temp1, long(DEFAULT_CLOSE_ID_SOURCE));
	m_closeIdSource = AUDIO_SOURCE(temp1);

	m_config->Read(m_name + KEY_CLOSE_ID, &m_closeId, DEFAULT_CLOSE_ID);

	m_config->Read(m_name + KEY_BEACON1_SOURCE, &temp1, long(DEFAULT_BEACON1_SOURCE));
	m_beacon1Source = AUDIO_SOURCE(temp1);

	m_config->Read(m_name + KEY_BEACON1, &m_beacon1, DEFAULT_BEACON1);

	m_config->Read(m_name + KEY_BEACON2_SOURCE, &temp1, long(DEFAULT_BEACON2_SOURCE));
	m_beacon2Source = AUDIO_SOURCE(temp1);

	m_config->Read(m_name + KEY_BEACON2, &m_beacon2, DEFAULT_BEACON2);

	m_config->Read(m_name + KEY_ID_SPEED, &temp1, long(DEFAULT_ID_SPEED));
	m_idSpeed = (unsigned int)temp1;

	m_config->Read(m_name + KEY_ID_FREQ, &temp1, long(DEFAULT_ID_FREQ));
	m_idFreq = (unsigned int)temp1;

	m_config->Read(m_name + KEY_ID_LEVEL1, &temp2, double(DEFAULT_ID_LEVEL1));
	m_idLevel1 = wxFloat32(temp2);

	m_config->Read(m_name + KEY_ID_LEVEL2, &temp2, double(DEFAULT_ID_LEVEL2));
	m_idLevel2 = wxFloat32(temp2);

	m_config->Read(m_name + KEY_ACK_RADIO_SOURCE, &temp1, long(DEFAULT_ACK_RADIO_SOURCE));
	m_ackRadioSource = AUDIO_SOURCE(temp1);

	m_config->Read(m_name + KEY_ACK_RADIO, &m_ackRadio, DEFAULT_ACK_RADIO);

	m_config->Read(m_name + KEY_ACK_EXTERNAL_SOURCE, &temp1, long(DEFAULT_ACK_EXTERNAL_SOURCE));
	m_ackExternalSource = AUDIO_SOURCE(temp1);

	m_config->Read(m_name + KEY_ACK_EXTERNAL, &m_ackExternal, DEFAULT_ACK_EXTERNAL);

	m_config->Read(m_name + KEY_ACK_BATTERY_SOURCE, &temp1, long(DEFAULT_ACK_BATTERY_SOURCE));
	m_ackBatterySource = AUDIO_SOURCE(temp1);

	m_config->Read(m_name + KEY_ACK_BATTERY, &m_ackBattery, DEFAULT_ACK_BATTERY);

	m_config->Read(m_name + KEY_ACK_SPEED, &temp1, long(DEFAULT_ACK_SPEED));
	m_ackSpeed = (unsigned int)temp1;

	m_config->Read(m_name + KEY_ACK_FREQ, &temp1, long(DEFAULT_ACK_FREQ));
	m_ackFreq = (unsigned int)temp1;

	m_config->Read(m_name + KEY_ACK_LEVEL, &temp2, double(DEFAULT_ACK_LEVEL));
	m_ackLevel = wxFloat32(temp2);

	m_config->Read(m_name + KEY_ACK_DELAY, &temp1, long(DEFAULT_ACK_DELAY));
	m_ackDelay = (unsigned int)temp1;

	m_config->Read(m_name + KEY_ACK_MIN, &temp1, long(DEFAULT_ACK_MIN));
	m_ackMin = (unsigned int)temp1;

	m_config->Read(m_name + KEY_CALLSIGNTIME, &temp1, long(DEFAULT_CALLSIGNTIME));
	m_callsignTime = (unsigned int)temp1;

	m_config->Read(m_name + KEY_TIMEOUT, &temp1, long(DEFAULT_TIMEOUT));
	m_timeout = (unsigned int)temp1;

	m_config->Read(m_name + KEY_LOCKOUTTIME, &temp1, long(DEFAULT_LOCKOUTTIME));
	m_lockoutTime = (unsigned int)temp1;

	m_config->Read(m_name + KEY_HANGTIME, &temp1, long(DEFAULT_HANGTIME));
	m_hangTime = (unsigned int)temp1;

	m_config->Read(m_name + KEY_LATCHTIME, &temp1, long(DEFAULT_LATCHTIME));
	m_latchTime = (unsigned int)temp1;

	m_config->Read(m_name + KEY_TONEBURST_ENABLE, &m_toneburstEnable, DEFAULT_TONEBURST_ENABLE);

	m_config->Read(m_name + KEY_TONEBURST_THRESHOLD, &temp2, double(DEFAULT_TONEBURST_THRESH));
	m_toneburstThreshold = wxFloat32(temp2);

	m_config->Read(m_name + KEY_CTCSS_FREQ, &temp2, double(DEFAULT_CTCSS_FREQ));
	m_ctcssFreq = wxFloat32(temp2);

	m_config->Read(m_name + KEY_CTCSS_INTERNAL, &m_ctcssInternal, DEFAULT_CTCSS_INTERNAL);

	m_config->Read(m_name + KEY_CTCSS_THRESHOLD, &temp2, double(DEFAULT_CTCSS_THRESH));
	m_ctcssThresh = wxFloat32(temp2);

	m_config->Read(m_name + KEY_CTCSS_LEVEL, &temp2, double(DEFAULT_CTCSS_LEVEL));
	m_ctcssLevel = wxFloat32(temp2);

	m_config->Read(m_name + KEY_CTCSS_HANG_TIME, &temp1, long(DEFAULT_CTCSS_HANG_TIME));
	m_ctcssHangTime = (unsigned int)temp1;

	m_config->Read(m_name + KEY_CTCSS_OUTPUT, &temp1, long(DEFAULT_CTCSS_OUTPUT));
	m_ctcssOutput = ANALOGUE_CTCSS_OUTPUT(temp1);

	m_config->Read(m_name + KEY_CALLSIGN_AT_START, &temp1, long(DEFAULT_CALLSIGN_AT_START));
	m_callAtStart = ANALOGUE_CALLSIGN_START(temp1);

	m_config->Read(m_name + KEY_CALLSIGN_START_DELAY, &temp1, long(DEFAULT_CALLSIGN_START_DELAY));
	m_callStartDelay = (unsigned int)temp1;

	m_config->Read(m_name + KEY_CALLSIGN_AT_END, &m_callAtEnd, DEFAULT_CALLSIGN_AT_END);

	m_config->Read(m_name + KEY_TIMEOUT_TYPE, &temp1, long(DEFAULT_TIMEOUT_TYPE));
	m_timeoutType = ANALOGUE_TIMEOUT_TYPE(temp1);

	m_config->Read(m_name + KEY_CALLSIGN_HOLDOFF, &temp1, long(DEFAULT_CALLSIGN_HOLDOFF));
	m_callHoldoff = ANALOGUE_CALLSIGN_HOLDOFF(temp1);

	m_config->Read(m_name + KEY_RADIO_READ_DEVICE, &m_radioReadDevice, DEFAULT_RADIO_READ_DEVICE);

	m_config->Read(m_name + KEY_RADIO_WRITE_DEVICE, &m_radioWriteDevice, DEFAULT_RADIO_WRITE_DEVICE);

	m_config->Read(m_name + KEY_RADIO_AUDIO_DELAY, &temp1, long(DEFAULT_RADIO_AUDIO_DELAY));
	m_radioDelay = (unsigned int)temp1;

	m_config->Read(m_name + KEY_RADIO_DEEMPHASIS, &m_radioDeEmphasis,  DEFAULT_RADIO_DEEMPHASIS);

	m_config->Read(m_name + KEY_RADIO_PREEMPHASIS, &m_radioPreEmphasis, DEFAULT_RADIO_PREEMPHASIS);

	m_config->Read(m_name + KEY_RADIO_VOGAD, &m_radioVOGAD, DEFAULT_RADIO_VOGAD);

	m_config->Read(m_name + KEY_EXTERNAL_MODE, &temp1, long(DEFAULT_EXTERNAL_MODE));
	m_externalMode = ANALOGUE_EXTERNAL_MODE(temp1);

	m_config->Read(m_name + KEY_EXTERNAL_READ_DEVICE, &m_externalReadDevice, DEFAULT_EXTERNAL_READ_DEVICE);

	m_config->Read(m_name + KEY_EXTERNAL_WRITE_DEVICE, &m_externalWriteDevice, DEFAULT_EXTERNAL_WRITE_DEVICE);

	m_config->Read(m_name + KEY_EXTERNAL_AUDIO_DELAY, &temp1, long(DEFAULT_EXTERNAL_AUDIO_DELAY));
	m_externalDelay = (unsigned int)temp1;

	m_config->Read(m_name + KEY_EXTERNAL_DEEMPHASIS, &m_externalDeEmphasis,  DEFAULT_EXTERNAL_DEEMPHASIS);

	m_config->Read(m_name + KEY_EXTERNAL_PREEMPHASIS, &m_externalPreEmphasis, DEFAULT_EXTERNAL_PREEMPHASIS);

	m_config->Read(m_name + KEY_EXTERNAL_VOGAD, &m_externalVOGAD, DEFAULT_EXTERNAL_VOGAD);

	m_config->Read(m_name + KEY_EXTERNAL_INTERFACE_DEVICE, &m_externalInterfaceDevice, DEFAULT_EXTERNAL_INTERFACE_DEVICE);

	m_config->Read(m_name + KEY_EXTERNAL_INTERFACE_TX_PIN, &temp1, long(DEFAULT_EXTERNAL_INTERFACE_TX_PIN));
	m_externalInterfaceTXPin = SERIALPIN(temp1);

	m_config->Read(m_name + KEY_EXTERNAL_INTERFACE_RX_PIN, &temp1, long(DEFAULT_EXTERNAL_INTERFACE_RX_PIN));
	m_externalInterfaceRXPin = SERIALPIN(temp1);

	m_config->Read(m_name + KEY_EXTERNAL_BACKGROUND, &m_externalBackground, DEFAULT_EXTERNAL_BACKGROUND);

	m_config->Read(m_name + KEY_IFACE_TYPE, &m_interfaceType, DEFAULT_IFACE_TYPE);

	m_config->Read(m_name + KEY_IFACE_CONFIG, &temp1, long(DEFAULT_IFACE_CONFIG));
	m_interfaceConfig = (unsigned int)temp1;

	m_config->Read(m_name + KEY_PTT_DELAY, &temp1, long(DEFAULT_PTT_DELAY));
	m_pttDelay = (unsigned int)temp1;

	m_config->Read(m_name + KEY_SQUELCH_DELAY, &temp1, long(DEFAULT_SQUELCH_DELAY));
	m_squelchDelay = (unsigned int)temp1;

	m_config->Read(m_name + KEY_PTT_INVERT, &m_pttInvert, DEFAULT_PTT_INVERT);

	m_config->Read(m_name + KEY_SQUELCH_INVERT, &m_squelchInvert, DEFAULT_SQUELCH_INVERT);

	m_config->Read(m_name + KEY_DTMF_RADIO, &m_dtmfRadio, DEFAULT_DTMF_RADIO);

	m_config->Read(m_name + KEY_DTMF_EXTERNAL, &m_dtmfExternal, DEFAULT_DTMF_EXTERNAL);

	m_config->Read(m_name + KEY_DTMF_SHUTDOWN, &m_dtmfShutdown, DEFAULT_DTMF_SHUTDOWN);

	m_config->Read(m_name + KEY_DTMF_STARTUP, &m_dtmfStartup, DEFAULT_DTMF_STARTUP);

	m_config->Read(m_name + KEY_DTMF_TIMEOUT, &m_dtmfTimeout, DEFAULT_DTMF_TIMEOUT);

	m_config->Read(m_name + KEY_DTMF_TIMERESET, &m_dtmfTimeReset, DEFAULT_DTMF_TIMERESET);

	m_config->Read(m_name + KEY_DTMF_COMMAND1, &m_dtmfCommand1, DEFAULT_DTMF_COMMAND1);

	m_config->Read(m_name + KEY_DTMF_COMMAND1_LINE, &m_dtmfCommand1Line, DEFAULT_DTMF_COMMAND1_LINE);

	m_config->Read(m_name + KEY_DTMF_COMMAND2, &m_dtmfCommand2, DEFAULT_DTMF_COMMAND2);

	m_config->Read(m_name + KEY_DTMF_COMMAND2_LINE, &m_dtmfCommand2Line, DEFAULT_DTMF_COMMAND2_LINE);

	m_config->Read(m_name + KEY_DTMF_OUTPUT1, &m_dtmfOutput1, DEFAULT_DTMF_OUTPUT1);

	m_config->Read(m_name + KEY_DTMF_OUTPUT2, &m_dtmfOutput2, DEFAULT_DTMF_OUTPUT2);

	m_config->Read(m_name + KEY_DTMF_OUTPUT3, &m_dtmfOutput3, DEFAULT_DTMF_OUTPUT3);

	m_config->Read(m_name + KEY_DTMF_OUTPUT1, &m_dtmfOutput4, DEFAULT_DTMF_OUTPUT4);

	m_config->Read(m_name + KEY_DTMF_THRESHOLD, &temp2, double(DEFAULT_DTMF_THRESHOLD));
	m_dtmfThreshold = wxFloat32(temp2);

	m_config->Read(m_name + KEY_OUTPUT1, &m_output1, DEFAULT_OUTPUT1);

	m_config->Read(m_name + KEY_OUTPUT2, &m_output2, DEFAULT_OUTPUT2);

	m_config->Read(m_name + KEY_OUTPUT3, &m_output3, DEFAULT_OUTPUT3);

	m_config->Read(m_name + KEY_OUTPUT4, &m_output4, DEFAULT_OUTPUT4);

	m_config->Read(m_name + KEY_ACTIVE_HANG_TIME, &temp1, long(DEFAULT_ACTIVE_HANG_TIME));
	m_activeHangTime = (unsigned int)temp1;

	m_config->Read(m_name + KEY_WINDOW_X, &temp1, long(DEFAULT_WINDOW_X));
	m_x = int(temp1);

	m_config->Read(m_name + KEY_WINDOW_Y, &temp1, long(DEFAULT_WINDOW_Y));
	m_y = int(temp1);
}

CAnalogueRepeaterConfig::~CAnalogueRepeaterConfig()
{
	delete m_config;
}

#else

CAnalogueRepeaterConfig::CAnalogueRepeaterConfig(const wxString& dir, const wxString& name) :
m_fileName(),
m_openIdSource(DEFAULT_OPEN_ID_SOURCE),
m_openId(DEFAULT_OPEN_ID),
m_closeIdSource(DEFAULT_CLOSE_ID_SOURCE),
m_closeId(DEFAULT_CLOSE_ID),
m_beacon1Source(DEFAULT_BEACON1_SOURCE),
m_beacon1(DEFAULT_BEACON1),
m_beacon2Source(DEFAULT_BEACON2_SOURCE),
m_beacon2(DEFAULT_BEACON2),
m_idSpeed(DEFAULT_ID_SPEED),
m_idFreq(DEFAULT_ID_FREQ),
m_idLevel1(DEFAULT_ID_LEVEL1),
m_idLevel2(DEFAULT_ID_LEVEL2),
m_ackRadioSource(DEFAULT_ACK_RADIO_SOURCE),
m_ackRadio(DEFAULT_ACK_RADIO),
m_ackExternalSource(DEFAULT_ACK_EXTERNAL_SOURCE),
m_ackExternal(DEFAULT_ACK_EXTERNAL),
m_ackBatterySource(DEFAULT_ACK_BATTERY_SOURCE),
m_ackBattery(DEFAULT_ACK_BATTERY),
m_ackSpeed(DEFAULT_ACK_SPEED),
m_ackFreq(DEFAULT_ACK_FREQ),
m_ackLevel(DEFAULT_ACK_LEVEL),
m_ackDelay(DEFAULT_ACK_DELAY),
m_ackMin(DEFAULT_ACK_MIN),
m_callsignTime(DEFAULT_CALLSIGNTIME),
m_timeout(DEFAULT_TIMEOUT),
m_lockoutTime(DEFAULT_LOCKOUTTIME),
m_hangTime(DEFAULT_HANGTIME),
m_latchTime(DEFAULT_LATCHTIME),
m_toneburstEnable(DEFAULT_TONEBURST_ENABLE),
m_toneburstThreshold(DEFAULT_TONEBURST_THRESH),
m_ctcssFreq(DEFAULT_CTCSS_FREQ),
m_ctcssInternal(DEFAULT_CTCSS_INTERNAL),
m_ctcssThresh(DEFAULT_CTCSS_THRESH),
m_ctcssLevel(DEFAULT_CTCSS_LEVEL),
m_ctcssHangTime(DEFAULT_CTCSS_HANG_TIME),
m_ctcssOutput(DEFAULT_CTCSS_OUTPUT),
m_callAtStart(DEFAULT_CALLSIGN_AT_START),
m_callStartDelay(DEFAULT_CALLSIGN_START_DELAY),
m_callAtEnd(DEFAULT_CALLSIGN_AT_END),
m_timeoutType(DEFAULT_TIMEOUT_TYPE),
m_callHoldoff(DEFAULT_CALLSIGN_HOLDOFF),
m_radioReadDevice(DEFAULT_RADIO_READ_DEVICE),
m_radioWriteDevice(DEFAULT_RADIO_WRITE_DEVICE),
m_radioDelay(DEFAULT_RADIO_AUDIO_DELAY),
m_radioDeEmphasis(DEFAULT_RADIO_DEEMPHASIS),
m_radioPreEmphasis(DEFAULT_RADIO_PREEMPHASIS),
m_radioVOGAD(DEFAULT_RADIO_VOGAD),
m_externalMode(DEFAULT_EXTERNAL_MODE),
m_externalReadDevice(DEFAULT_EXTERNAL_READ_DEVICE),
m_externalWriteDevice(DEFAULT_EXTERNAL_WRITE_DEVICE),
m_externalDelay(DEFAULT_EXTERNAL_AUDIO_DELAY),
m_externalDeEmphasis(DEFAULT_EXTERNAL_DEEMPHASIS),
m_externalPreEmphasis(DEFAULT_EXTERNAL_PREEMPHASIS),
m_externalVOGAD(DEFAULT_EXTERNAL_VOGAD),
m_externalInterfaceDevice(DEFAULT_EXTERNAL_INTERFACE_DEVICE),
m_externalInterfaceTXPin(DEFAULT_EXTERNAL_INTERFACE_TX_PIN),
m_externalInterfaceRXPin(DEFAULT_EXTERNAL_INTERFACE_RX_PIN),
m_externalBackground(DEFAULT_EXTERNAL_BACKGROUND),
m_interfaceType(DEFAULT_IFACE_TYPE),
m_interfaceConfig(DEFAULT_IFACE_CONFIG),
m_pttDelay(DEFAULT_PTT_DELAY),
m_squelchDelay(DEFAULT_SQUELCH_DELAY),
m_pttInvert(DEFAULT_PTT_INVERT),
m_squelchInvert(DEFAULT_SQUELCH_INVERT),
m_dtmfRadio(DEFAULT_DTMF_RADIO),
m_dtmfExternal(DEFAULT_DTMF_EXTERNAL),
m_dtmfShutdown(DEFAULT_DTMF_SHUTDOWN),
m_dtmfStartup(DEFAULT_DTMF_STARTUP),
m_dtmfTimeout(DEFAULT_DTMF_TIMEOUT),
m_dtmfTimeReset(DEFAULT_DTMF_TIMERESET),
m_dtmfCommand1(DEFAULT_DTMF_COMMAND1),
m_dtmfCommand1Line(DEFAULT_DTMF_COMMAND1_LINE),
m_dtmfCommand2(DEFAULT_DTMF_COMMAND2),
m_dtmfCommand2Line(DEFAULT_DTMF_COMMAND2_LINE),
m_dtmfOutput1(DEFAULT_DTMF_OUTPUT1),
m_dtmfOutput2(DEFAULT_DTMF_OUTPUT2),
m_dtmfOutput3(DEFAULT_DTMF_OUTPUT3),
m_dtmfOutput4(DEFAULT_DTMF_OUTPUT4),
m_dtmfThreshold(DEFAULT_DTMF_THRESHOLD),
m_activeHangTime(DEFAULT_ACTIVE_HANG_TIME),
m_output1(DEFAULT_OUTPUT1),
m_output2(DEFAULT_OUTPUT2),
m_output3(DEFAULT_OUTPUT3),
m_output4(DEFAULT_OUTPUT4),
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

		if (key.IsSameAs(KEY_OPEN_ID_SOURCE)) {
			val.ToLong(&temp1);
			m_openIdSource = AUDIO_SOURCE(temp1);
		} else if (key.IsSameAs(KEY_OPEN_ID)) {
			m_openId = val;
		} else if (key.IsSameAs(KEY_CLOSE_ID_SOURCE)) {
			val.ToLong(&temp1);
			m_closeIdSource = AUDIO_SOURCE(temp1);
		} else if (key.IsSameAs(KEY_CLOSE_ID)) {
			m_closeId = val;
		} else if (key.IsSameAs(KEY_BEACON1_SOURCE)) {
			val.ToLong(&temp1);
			m_beacon1Source = AUDIO_SOURCE(temp1);
		} else if (key.IsSameAs(KEY_BEACON1)) {
			m_beacon1 = val;
		} else if (key.IsSameAs(KEY_BEACON2_SOURCE)) {
			val.ToLong(&temp1);
			m_beacon2Source = AUDIO_SOURCE(temp1);
		} else if (key.IsSameAs(KEY_BEACON2)) {
			m_beacon2 = val;
		} else if (key.IsSameAs(KEY_ID_SPEED)) {
			val.ToULong(&temp2);
			m_idSpeed = (unsigned int)temp2;
		} else if (key.IsSameAs(KEY_ID_FREQ)) {
			val.ToULong(&temp2);
			m_idFreq = (unsigned int)temp2;
		} else if (key.IsSameAs(KEY_ID_LEVEL1)) {
			val.ToDouble(&temp3);
			m_idLevel1 = wxFloat32(temp3);
		} else if (key.IsSameAs(KEY_ID_LEVEL2)) {
			val.ToDouble(&temp3);
			m_idLevel2 = wxFloat32(temp3);
		} else if (key.IsSameAs(KEY_ACK_RADIO_SOURCE)) {
			val.ToLong(&temp1);
			m_ackRadioSource = AUDIO_SOURCE(temp1);
		} else if (key.IsSameAs(KEY_ACK_RADIO)) {
			m_ackRadio = val;
		} else if (key.IsSameAs(KEY_ACK_EXTERNAL_SOURCE)) {
			val.ToLong(&temp1);
			m_ackExternalSource = AUDIO_SOURCE(temp1);
		} else if (key.IsSameAs(KEY_ACK_EXTERNAL)) {
			m_ackExternal = val;
		} else if (key.IsSameAs(KEY_ACK_BATTERY_SOURCE)) {
			val.ToLong(&temp1);
			m_ackBatterySource = AUDIO_SOURCE(temp1);
		} else if (key.IsSameAs(KEY_ACK_BATTERY)) {
			m_ackBattery = val;
		} else if (key.IsSameAs(KEY_ACK_SPEED)) {
			val.ToULong(&temp2);
			m_ackSpeed = (unsigned int)temp2;
		} else if (key.IsSameAs(KEY_ACK_FREQ)) {
			val.ToULong(&temp2);
			m_ackFreq = (unsigned int)temp2;
		} else if (key.IsSameAs(KEY_ACK_LEVEL)) {
			val.ToDouble(&temp3);
			m_ackLevel = wxFloat32(temp3);
		} else if (key.IsSameAs(KEY_ACK_DELAY)) {
			val.ToULong(&temp2);
			m_ackDelay = (unsigned int)temp2;
		} else if (key.IsSameAs(KEY_ACK_MIN)) {
			val.ToULong(&temp2);
			m_ackMin = (unsigned int)temp2;
		} else if (key.IsSameAs(KEY_CALLSIGNTIME)) {
			val.ToULong(&temp2);
			m_callsignTime = (unsigned int)temp2;
		} else if (key.IsSameAs(KEY_TIMEOUT)) {
			val.ToULong(&temp2);
			m_timeout = (unsigned int)temp2;
		} else if (key.IsSameAs(KEY_LOCKOUTTIME)) {
			val.ToULong(&temp2);
			m_lockoutTime = (unsigned int)temp2;
		} else if (key.IsSameAs(KEY_HANGTIME)) {
			val.ToULong(&temp2);
			m_hangTime = (unsigned int)temp2;
		} else if (key.IsSameAs(KEY_LATCHTIME)) {
			val.ToULong(&temp2);
			m_latchTime = (unsigned int)temp2;
		} else if (key.IsSameAs(KEY_TONEBURST_ENABLE)) {
			val.ToLong(&temp1);
			m_toneburstEnable = temp1 == 1L;
		} else if (key.IsSameAs(KEY_TONEBURST_THRESHOLD)) {
			val.ToDouble(&temp3);
			m_toneburstThreshold = wxFloat32(temp3);
		} else if (key.IsSameAs(KEY_CTCSS_FREQ)) {
			val.ToDouble(&temp3);
			m_ctcssFreq = wxFloat32(temp3);
		} else if (key.IsSameAs(KEY_CTCSS_INTERNAL)) {
			val.ToLong(&temp1);
			m_ctcssInternal = temp1 == 1L;
		} else if (key.IsSameAs(KEY_CTCSS_THRESHOLD)) {
			val.ToDouble(&temp3);
			m_ctcssThresh = wxFloat32(temp3);
		} else if (key.IsSameAs(KEY_CTCSS_LEVEL)) {
			val.ToDouble(&temp3);
			m_ctcssLevel = wxFloat32(temp3);
		} else if (key.IsSameAs(KEY_CTCSS_HANG_TIME)) {
			val.ToULong(&temp2);
			m_ctcssHangTime = (unsigned int)temp2;
		} else if (key.IsSameAs(KEY_CTCSS_OUTPUT)) {
			val.ToLong(&temp1);
			m_ctcssOutput = ANALOGUE_CTCSS_OUTPUT(temp1);
		} else if (key.IsSameAs(KEY_CALLSIGN_AT_START)) {
			val.ToLong(&temp1);
			m_callAtStart = ANALOGUE_CALLSIGN_START(temp1);
		} else if (key.IsSameAs(KEY_CALLSIGN_START_DELAY)) {
			val.ToULong(&temp2);
			m_callStartDelay = (unsigned int)temp2;
		} else if (key.IsSameAs(KEY_CALLSIGN_AT_END)) {
			val.ToLong(&temp1);
			m_callAtEnd = temp1 == 1L;
		} else if (key.IsSameAs(KEY_TIMEOUT_TYPE)) {
			val.ToLong(&temp1);
			m_timeoutType = ANALOGUE_TIMEOUT_TYPE(temp1);
		} else if (key.IsSameAs(KEY_CALLSIGN_HOLDOFF)) {
			val.ToLong(&temp1);
			m_callHoldoff = ANALOGUE_CALLSIGN_HOLDOFF(temp1);
		} else if (key.IsSameAs(KEY_RADIO_READ_DEVICE)) {
			m_radioReadDevice = val;
		} else if (key.IsSameAs(KEY_RADIO_WRITE_DEVICE)) {
			m_radioWriteDevice = val;
		} else if (key.IsSameAs(KEY_RADIO_AUDIO_DELAY)) {
			val.ToULong(&temp2);
			m_radioDelay = (unsigned int)temp2;
		} else if (key.IsSameAs(KEY_RADIO_DEEMPHASIS)) {
			val.ToLong(&temp1);
			m_radioDeEmphasis = temp1 == 1L;
		} else if (key.IsSameAs(KEY_RADIO_PREEMPHASIS)) {
			val.ToLong(&temp1);
			m_radioPreEmphasis = temp1 == 1L;
		} else if (key.IsSameAs(KEY_RADIO_VOGAD)) {
			val.ToLong(&temp1);
			m_radioVOGAD = temp1 == 1L;
		} else if (key.IsSameAs(KEY_EXTERNAL_MODE)) {
			val.ToLong(&temp1);
			m_externalMode = ANALOGUE_EXTERNAL_MODE(temp1);
		} else if (key.IsSameAs(KEY_EXTERNAL_READ_DEVICE)) {
			m_externalReadDevice = val;
		} else if (key.IsSameAs(KEY_EXTERNAL_WRITE_DEVICE)) {
			m_externalWriteDevice = val;
		} else if (key.IsSameAs(KEY_EXTERNAL_AUDIO_DELAY)) {
			val.ToULong(&temp2);
			m_externalDelay = (unsigned int)temp2;
		} else if (key.IsSameAs(KEY_EXTERNAL_DEEMPHASIS)) {
			val.ToLong(&temp1);
			m_externalDeEmphasis = temp1 == 1L;
		} else if (key.IsSameAs(KEY_EXTERNAL_PREEMPHASIS)) {
			val.ToLong(&temp1);
			m_externalPreEmphasis = temp1 == 1L;
		} else if (key.IsSameAs(KEY_EXTERNAL_VOGAD)) {
			val.ToLong(&temp1);
			m_externalVOGAD = temp1 == 1L;
		} else if (key.IsSameAs(KEY_EXTERNAL_INTERFACE_DEVICE)) {
			m_externalInterfaceDevice = val;
		} else if (key.IsSameAs(KEY_EXTERNAL_INTERFACE_TX_PIN)) {
			val.ToLong(&temp1);
			m_externalInterfaceTXPin = SERIALPIN(temp1);
		} else if (key.IsSameAs(KEY_EXTERNAL_INTERFACE_RX_PIN)) {
			val.ToLong(&temp1);
			m_externalInterfaceRXPin = SERIALPIN(temp1);
		} else if (key.IsSameAs(KEY_EXTERNAL_BACKGROUND)) {
			val.ToLong(&temp1);
			m_externalBackground = temp1 == 1L;
		} else if (key.IsSameAs(KEY_IFACE_TYPE)) {
			m_interfaceType = val;
		} else if (key.IsSameAs(KEY_IFACE_CONFIG)) {
			val.ToULong(&temp2);
			m_interfaceConfig = (unsigned int)temp2;
		} else if (key.IsSameAs(KEY_PTT_DELAY)) {
			val.ToULong(&temp2);
			m_pttDelay = (unsigned int)temp2;
		} else if (key.IsSameAs(KEY_SQUELCH_DELAY)) {
			val.ToULong(&temp2);
			m_squelchDelay = (unsigned int)temp2;
		} else if (key.IsSameAs(KEY_PTT_INVERT)) {
			val.ToLong(&temp1);
			m_pttInvert = temp1 == 1L;
		} else if (key.IsSameAs(KEY_SQUELCH_INVERT)) {
			val.ToLong(&temp1);
			m_squelchInvert = temp1 == 1L;
		} else if (key.IsSameAs(KEY_DTMF_RADIO)) {
			val.ToLong(&temp1);
			m_dtmfRadio = temp1 == 1L;
		} else if (key.IsSameAs(KEY_DTMF_EXTERNAL)) {
			val.ToLong(&temp1);
			m_dtmfExternal = temp1 == 1L;
		} else if (key.IsSameAs(KEY_DTMF_SHUTDOWN)) {
			m_dtmfShutdown = val;
		} else if (key.IsSameAs(KEY_DTMF_STARTUP)) {
			m_dtmfStartup = val;
		} else if (key.IsSameAs(KEY_DTMF_TIMEOUT)) {
			m_dtmfTimeout = val;
		} else if (key.IsSameAs(KEY_DTMF_TIMERESET)) {
			m_dtmfTimeReset = val;
		} else if (key.IsSameAs(KEY_DTMF_COMMAND1)) {
			m_dtmfCommand1 = val;
		} else if (key.IsSameAs(KEY_DTMF_COMMAND1_LINE)) {
			m_dtmfCommand1Line = val;
		} else if (key.IsSameAs(KEY_DTMF_COMMAND2)) {
			m_dtmfCommand2 = val;
		} else if (key.IsSameAs(KEY_DTMF_COMMAND2_LINE)) {
			m_dtmfCommand2Line = val;
		} else if (key.IsSameAs(KEY_DTMF_OUTPUT1)) {
			m_dtmfOutput1 = val;
		} else if (key.IsSameAs(KEY_DTMF_OUTPUT2)) {
			m_dtmfOutput2 = val;
		} else if (key.IsSameAs(KEY_DTMF_OUTPUT3)) {
			m_dtmfOutput3 = val;
		} else if (key.IsSameAs(KEY_DTMF_OUTPUT4)) {
			m_dtmfOutput4 = val;
		} else if (key.IsSameAs(KEY_DTMF_THRESHOLD)) {
			val.ToDouble(&temp3);
			m_dtmfThreshold = wxFloat32(temp3);
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

CAnalogueRepeaterConfig::~CAnalogueRepeaterConfig()
{
}

#endif

void CAnalogueRepeaterConfig::getCallsign(AUDIO_SOURCE& openIdSource, wxString& openId, AUDIO_SOURCE& closeIdSource, wxString& closeId, AUDIO_SOURCE& beacon1Source, wxString& beacon1, AUDIO_SOURCE& beacon2Source, wxString& beacon2, unsigned int& speed, unsigned int& freq, wxFloat32& level1, wxFloat32& level2) const
{
	openIdSource  = m_openIdSource;
	openId        = m_openId;
	closeIdSource = m_closeIdSource;
	closeId       = m_closeId;
	beacon1Source = m_beacon1Source;
	beacon1       = m_beacon1;
	beacon2Source = m_beacon2Source;
	beacon2       = m_beacon2;
	speed         = m_idSpeed;
	freq          = m_idFreq;
	level1        = m_idLevel1;
	level2        = m_idLevel2;
}

void CAnalogueRepeaterConfig::setCallsign(AUDIO_SOURCE openIdSource, const wxString& openId, AUDIO_SOURCE closeIdSource, const wxString& closeId, AUDIO_SOURCE beacon1Source, const wxString& beacon1, AUDIO_SOURCE beacon2Source, const wxString& beacon2, unsigned int speed, unsigned int freq, wxFloat32 level1, wxFloat32 level2)
{
	m_openIdSource  = openIdSource;
	m_openId        = openId;
	m_closeIdSource = closeIdSource;
	m_closeId       = closeId;
	m_beacon1Source = beacon1Source;
	m_beacon1       = beacon1;
	m_beacon2Source = beacon2Source;
	m_beacon2       = beacon2;
	m_idSpeed       = speed;
	m_idFreq        = freq;
	m_idLevel1      = level1;
	m_idLevel2      = level2;
}

void CAnalogueRepeaterConfig::getAck(AUDIO_SOURCE& radioAckSource, wxString& radioAck, AUDIO_SOURCE& extAckSource, wxString& extAck, AUDIO_SOURCE& batteryAckSource, wxString& batteryAck, unsigned int& speed, unsigned int& freq, wxFloat32& level, unsigned int& ack, unsigned int& minimum) const
{
	radioAckSource   = m_ackRadioSource;
	radioAck         = m_ackRadio;
	extAckSource     = m_ackExternalSource;
	extAck           = m_ackExternal;
	batteryAckSource = m_ackBatterySource;
	batteryAck       = m_ackBattery;
	speed            = m_ackSpeed;
	freq             = m_ackFreq;
	level            = m_ackLevel;
	ack              = m_ackDelay;
	minimum          = m_ackMin;
}

void CAnalogueRepeaterConfig::setAck(AUDIO_SOURCE radioAckSource, const wxString& radioAck, AUDIO_SOURCE extAckSource, const wxString& extAck, AUDIO_SOURCE batteryAckSource, const wxString& batteryAck, unsigned int speed, unsigned int freq, wxFloat32 level, unsigned int ack, unsigned int minimum)
{
	m_ackRadioSource    = radioAckSource;
	m_ackRadio          = radioAck;
	m_ackExternalSource = extAckSource;
	m_ackExternal       = extAck;
	m_ackBatterySource  = batteryAckSource;
	m_ackBattery        = batteryAck;
	m_ackSpeed          = speed;
	m_ackFreq           = freq;
	m_ackLevel          = level;
	m_ackDelay          = ack;
	m_ackMin            = minimum;
}

void CAnalogueRepeaterConfig::getTimes(unsigned int& callsignTime, unsigned int& timeout, unsigned int& lockoutTime, unsigned int& hangTime, unsigned int& latchTime) const
{
	callsignTime = m_callsignTime;
	timeout      = m_timeout;
	lockoutTime  = m_lockoutTime;
	hangTime     = m_hangTime;
	latchTime    = m_latchTime;
}

void CAnalogueRepeaterConfig::setTimes(unsigned int callsignTime, unsigned int timeout, unsigned int lockoutTime, unsigned int hangTime, unsigned int latchTime)
{
	m_callsignTime = callsignTime;
	m_timeout      = timeout;
	m_lockoutTime  = lockoutTime;
	m_hangTime     = hangTime;
	m_latchTime    = latchTime;
}

void CAnalogueRepeaterConfig::getTones(bool& tbEnable, wxFloat32& tbThreshold, wxFloat32& ctcssFreq, bool& ctcssInternal, wxFloat32& ctcssThresh, wxFloat32& ctcssLevel, unsigned int& ctcssHangTime, ANALOGUE_CTCSS_OUTPUT& ctcssOutput) const
{
	tbEnable      = m_toneburstEnable;
	tbThreshold   = m_toneburstThreshold;
	ctcssFreq     = m_ctcssFreq;
	ctcssInternal = m_ctcssInternal;
	ctcssThresh   = m_ctcssThresh;
	ctcssLevel    = m_ctcssLevel;
	ctcssHangTime = m_ctcssHangTime;
	ctcssOutput   = m_ctcssOutput;
}

void CAnalogueRepeaterConfig::setTones(bool tbEnable, wxFloat32 tbThreshold, wxFloat32 ctcssFreq, bool ctcssInternal, wxFloat32 ctcssThresh, wxFloat32 ctcssLevel, unsigned int ctcssHangTime, ANALOGUE_CTCSS_OUTPUT ctcssOutput)
{
	m_toneburstEnable    = tbEnable;
	m_toneburstThreshold = tbThreshold;
	m_ctcssFreq          = ctcssFreq;
	m_ctcssInternal      = ctcssInternal;
	m_ctcssThresh        = ctcssThresh;
	m_ctcssLevel         = ctcssLevel;
	m_ctcssHangTime      = ctcssHangTime;
	m_ctcssOutput        = ctcssOutput;
}

void CAnalogueRepeaterConfig::getFeel(ANALOGUE_CALLSIGN_START& callsignAtStart, unsigned int& callsignStartDelay, bool& callsignAtEnd, ANALOGUE_TIMEOUT_TYPE& timeoutType, ANALOGUE_CALLSIGN_HOLDOFF& callsignHoldoff) const
{
	callsignAtStart    = m_callAtStart;
	callsignStartDelay = m_callStartDelay;
	callsignAtEnd      = m_callAtEnd;
	timeoutType        = m_timeoutType;
	callsignHoldoff    = m_callHoldoff;
}

void CAnalogueRepeaterConfig::setFeel(ANALOGUE_CALLSIGN_START callsignAtStart, unsigned int callsignStartDelay, bool callsignAtEnd, ANALOGUE_TIMEOUT_TYPE timeoutType, ANALOGUE_CALLSIGN_HOLDOFF callsignHoldoff)
{
	m_callAtStart    = callsignAtStart;
	m_callStartDelay = callsignStartDelay;
	m_callAtEnd      = callsignAtEnd;
	m_timeoutType    = timeoutType;
	m_callHoldoff    = callsignHoldoff;
}

void CAnalogueRepeaterConfig::getRadio(wxString& readDevice, wxString& writeDevice, unsigned int& delay, bool& deEmphasis, bool& preEmphasis, bool& vogad) const
{
	readDevice  = m_radioReadDevice;
	writeDevice = m_radioWriteDevice;
	delay       = m_radioDelay;
	deEmphasis  = m_radioDeEmphasis;
	preEmphasis = m_radioPreEmphasis;
	vogad       = m_radioVOGAD;
}

void CAnalogueRepeaterConfig::setRadio(const wxString& readDevice, const wxString& writeDevice, unsigned int delay, bool deEmphasis, bool preEmphasis, bool vogad)
{
	m_radioReadDevice  = readDevice;
	m_radioWriteDevice = writeDevice;
	m_radioDelay       = delay;
	m_radioDeEmphasis  = deEmphasis;
	m_radioPreEmphasis = preEmphasis;
	m_radioVOGAD       = vogad;
}

void CAnalogueRepeaterConfig::getExternal(ANALOGUE_EXTERNAL_MODE& mode, wxString& readDevice, wxString& writeDevice, unsigned int& delay, bool& deEmphasis, bool& preEmphasis, bool& vogad, wxString& device, SERIALPIN& txPin, SERIALPIN& rxPin, bool& background) const
{
	mode        = m_externalMode;
	readDevice  = m_externalReadDevice;
	writeDevice = m_externalWriteDevice;
	delay       = m_externalDelay;
	deEmphasis  = m_externalDeEmphasis;
	preEmphasis = m_externalPreEmphasis;
	vogad       = m_externalVOGAD;
	device      = m_externalInterfaceDevice;
	txPin       = m_externalInterfaceTXPin;
	rxPin       = m_externalInterfaceRXPin;
	background  = m_externalBackground;
}

void CAnalogueRepeaterConfig::setExternal(ANALOGUE_EXTERNAL_MODE mode, const wxString& readDevice, const wxString& writeDevice, unsigned int delay, bool deEmphasis, bool preEmphasis, bool vogad, const wxString& device, SERIALPIN txPin, SERIALPIN rxPin, bool background)
{
	m_externalMode            = mode;
	m_externalReadDevice      = readDevice;
	m_externalWriteDevice     = writeDevice;
	m_externalDelay           = delay;
	m_externalDeEmphasis      = deEmphasis;
	m_externalPreEmphasis     = preEmphasis;
	m_externalVOGAD           = vogad;
	m_externalInterfaceDevice = device;
	m_externalInterfaceTXPin  = txPin;
	m_externalInterfaceRXPin  = rxPin;
	m_externalBackground      = background;
}

void CAnalogueRepeaterConfig::getController(wxString& type, unsigned int& config, unsigned int& pttDelay, unsigned int& squelchDelay, bool& pttInvert, bool& squelchInvert) const
{
	type          = m_interfaceType;
	config        = m_interfaceConfig;
	pttDelay      = m_pttDelay;
	squelchDelay  = m_squelchDelay;
	pttInvert     = m_pttInvert;
	squelchInvert = m_squelchInvert;
}

void CAnalogueRepeaterConfig::setController(const wxString& type, unsigned int config, unsigned int pttDelay, unsigned int squelchDelay, bool pttInvert, bool squelchInvert)
{
	m_interfaceType   = type;
	m_interfaceConfig = config;
	m_pttDelay        = pttDelay;
	m_squelchDelay    = squelchDelay;
	m_pttInvert       = pttInvert;
	m_squelchInvert   = squelchInvert;
}

void CAnalogueRepeaterConfig::getDTMF(bool& radio, bool& external, wxString& shutdown, wxString& startup, wxString& timeout, wxString& timeReset, wxString& command1, wxString& command1Line, wxString& command2, wxString& command2Line, wxString& output1, wxString& output2, wxString& output3, wxString& output4, wxFloat32& threshold) const
{
	radio        = m_dtmfRadio;
	external     = m_dtmfExternal;
	shutdown     = m_dtmfShutdown;
	startup      = m_dtmfStartup;
	timeout      = m_dtmfTimeout;
	timeReset    = m_dtmfTimeReset;
	command1     = m_dtmfCommand1;
	command1Line = m_dtmfCommand1Line;
	command2     = m_dtmfCommand2;
	command2Line = m_dtmfCommand2Line;
	output1      = m_dtmfOutput1;
	output2      = m_dtmfOutput2;
	output3      = m_dtmfOutput3;
	output4      = m_dtmfOutput4;
	threshold    = m_dtmfThreshold;
}

void CAnalogueRepeaterConfig::setDTMF(bool radio, bool external, const wxString& shutdown, const wxString& startup, const wxString& timeout, const wxString& timeReset, const wxString& command1, const wxString& command1Line, const wxString& command2, const wxString& command2Line, const wxString& output1, const wxString& output2, const wxString& output3, const wxString& output4, wxFloat32 threshold)
{
	m_dtmfRadio        = radio;
	m_dtmfExternal     = external;
	m_dtmfShutdown     = shutdown;
	m_dtmfStartup      = startup;
	m_dtmfTimeout      = timeout;
	m_dtmfTimeReset    = timeReset;
	m_dtmfCommand1     = command1;
	m_dtmfCommand1Line = command1Line;
	m_dtmfCommand2     = command2;
	m_dtmfCommand2Line = command2Line;
	m_dtmfOutput1      = output1;
	m_dtmfOutput2      = output2;
	m_dtmfOutput3      = output3;
	m_dtmfOutput4      = output4;
	m_dtmfThreshold    = threshold;
}

void CAnalogueRepeaterConfig::getOutputs(bool& out1, bool& out2, bool& out3, bool& out4) const
{
	out1 = m_output1;
	out2 = m_output2;
	out3 = m_output3;
	out4 = m_output4;
}

void CAnalogueRepeaterConfig::setOutputs(bool out1, bool out2, bool out3, bool out4)
{
	m_output1 = out1;
	m_output2 = out2;
	m_output3 = out3;
	m_output4 = out4;
}

void CAnalogueRepeaterConfig::getActiveHang(unsigned int& time) const
{
	time = m_activeHangTime;
}

void CAnalogueRepeaterConfig::setActiveHang(unsigned int time)
{
	m_activeHangTime = time;
}

void CAnalogueRepeaterConfig::getPosition(int& x, int& y) const
{
	x = m_x;
	y = m_y;
}

void CAnalogueRepeaterConfig::setPosition(int x, int y)
{
	m_x = x;
	m_y = y;
}

#if defined(__WINDOWS__)

bool CAnalogueRepeaterConfig::write()
{
	m_config->Write(m_name + KEY_OPEN_ID_SOURCE, long(m_openIdSource));
	m_config->Write(m_name + KEY_OPEN_ID, m_openId);
	m_config->Write(m_name + KEY_CLOSE_ID_SOURCE, long(m_closeIdSource));
	m_config->Write(m_name + KEY_CLOSE_ID, m_closeId);
	m_config->Write(m_name + KEY_BEACON1_SOURCE, long(m_beacon1Source));
	m_config->Write(m_name + KEY_BEACON1, m_beacon1);
	m_config->Write(m_name + KEY_BEACON2_SOURCE, long(m_beacon2Source));
	m_config->Write(m_name + KEY_BEACON2, m_beacon2);
	m_config->Write(m_name + KEY_ID_SPEED, long(m_idSpeed));
	m_config->Write(m_name + KEY_ID_FREQ, long(m_idFreq));
	m_config->Write(m_name + KEY_ID_LEVEL1, double(m_idLevel1));
	m_config->Write(m_name + KEY_ID_LEVEL2, double(m_idLevel2));
	m_config->Write(m_name + KEY_ACK_RADIO_SOURCE, long(m_ackRadioSource));
	m_config->Write(m_name + KEY_ACK_RADIO, m_ackRadio);
	m_config->Write(m_name + KEY_ACK_EXTERNAL_SOURCE, long(m_ackExternalSource));
	m_config->Write(m_name + KEY_ACK_EXTERNAL, m_ackExternal);
	m_config->Write(m_name + KEY_ACK_BATTERY_SOURCE, long(m_ackBatterySource));
	m_config->Write(m_name + KEY_ACK_BATTERY, m_ackBattery);
	m_config->Write(m_name + KEY_ACK_SPEED, long(m_ackSpeed));
	m_config->Write(m_name + KEY_ACK_FREQ, long(m_ackFreq));
	m_config->Write(m_name + KEY_ACK_LEVEL, double(m_ackLevel));
	m_config->Write(m_name + KEY_ACK_DELAY, long(m_ackDelay));
	m_config->Write(m_name + KEY_ACK_MIN, long(m_ackMin));
	m_config->Write(m_name + KEY_CALLSIGNTIME, long(m_callsignTime));
	m_config->Write(m_name + KEY_TIMEOUT, long(m_timeout));
	m_config->Write(m_name + KEY_LOCKOUTTIME, long(m_lockoutTime));
	m_config->Write(m_name + KEY_HANGTIME, long(m_hangTime));
	m_config->Write(m_name + KEY_LATCHTIME, long(m_latchTime));
	m_config->Write(m_name + KEY_TONEBURST_ENABLE, m_toneburstEnable);
	m_config->Write(m_name + KEY_TONEBURST_THRESHOLD, double(m_toneburstThreshold));
	m_config->Write(m_name + KEY_CTCSS_FREQ, double(m_ctcssFreq));
	m_config->Write(m_name + KEY_CTCSS_INTERNAL, m_ctcssInternal);
	m_config->Write(m_name + KEY_CTCSS_THRESHOLD, double(m_ctcssThresh));
	m_config->Write(m_name + KEY_CTCSS_LEVEL, double(m_ctcssLevel));
	m_config->Write(m_name + KEY_CTCSS_HANG_TIME, long(m_ctcssHangTime));
	m_config->Write(m_name + KEY_CTCSS_OUTPUT, long(m_ctcssOutput));
	m_config->Write(m_name + KEY_CALLSIGN_AT_START, long(m_callAtStart));
	m_config->Write(m_name + KEY_CALLSIGN_START_DELAY, long(m_callStartDelay));
	m_config->Write(m_name + KEY_CALLSIGN_AT_END, m_callAtEnd);
	m_config->Write(m_name + KEY_TIMEOUT_TYPE, long(m_timeoutType));
	m_config->Write(m_name + KEY_CALLSIGN_HOLDOFF, long(m_callHoldoff));
	m_config->Write(m_name + KEY_RADIO_READ_DEVICE, m_radioReadDevice);
	m_config->Write(m_name + KEY_RADIO_WRITE_DEVICE, m_radioWriteDevice);
	m_config->Write(m_name + KEY_RADIO_AUDIO_DELAY, long(m_radioDelay));
	m_config->Write(m_name + KEY_RADIO_DEEMPHASIS, m_radioDeEmphasis);
	m_config->Write(m_name + KEY_RADIO_PREEMPHASIS, m_radioPreEmphasis);
	m_config->Write(m_name + KEY_RADIO_VOGAD, m_radioVOGAD);
	m_config->Write(m_name + KEY_EXTERNAL_MODE, long(m_externalMode));
	m_config->Write(m_name + KEY_EXTERNAL_READ_DEVICE, m_externalReadDevice);
	m_config->Write(m_name + KEY_EXTERNAL_WRITE_DEVICE, m_externalWriteDevice);
	m_config->Write(m_name + KEY_EXTERNAL_AUDIO_DELAY, long(m_externalDelay));
	m_config->Write(m_name + KEY_EXTERNAL_DEEMPHASIS, m_externalDeEmphasis);
	m_config->Write(m_name + KEY_EXTERNAL_PREEMPHASIS, m_externalPreEmphasis);
	m_config->Write(m_name + KEY_EXTERNAL_VOGAD, m_externalVOGAD);
	m_config->Write(m_name + KEY_EXTERNAL_INTERFACE_DEVICE, m_externalInterfaceDevice);
	m_config->Write(m_name + KEY_EXTERNAL_INTERFACE_TX_PIN, long(m_externalInterfaceTXPin));
	m_config->Write(m_name + KEY_EXTERNAL_INTERFACE_RX_PIN, long(m_externalInterfaceRXPin));
	m_config->Write(m_name + KEY_EXTERNAL_BACKGROUND, m_externalBackground);
	m_config->Write(m_name + KEY_IFACE_TYPE, m_interfaceType);
	m_config->Write(m_name + KEY_IFACE_CONFIG, long(m_interfaceConfig));
	m_config->Write(m_name + KEY_PTT_DELAY, long(m_pttDelay));
	m_config->Write(m_name + KEY_SQUELCH_DELAY, long(m_squelchDelay));
	m_config->Write(m_name + KEY_PTT_INVERT, m_pttInvert);
	m_config->Write(m_name + KEY_SQUELCH_INVERT, m_squelchInvert);
	m_config->Write(m_name + KEY_DTMF_RADIO, m_dtmfRadio);
	m_config->Write(m_name + KEY_DTMF_EXTERNAL, m_dtmfExternal);
	m_config->Write(m_name + KEY_DTMF_SHUTDOWN, m_dtmfShutdown);
	m_config->Write(m_name + KEY_DTMF_STARTUP, m_dtmfStartup);
	m_config->Write(m_name + KEY_DTMF_TIMEOUT, m_dtmfTimeout);
	m_config->Write(m_name + KEY_DTMF_TIMERESET, m_dtmfTimeReset);
	m_config->Write(m_name + KEY_DTMF_COMMAND1, m_dtmfCommand1);
	m_config->Write(m_name + KEY_DTMF_COMMAND1_LINE, m_dtmfCommand1Line);
	m_config->Write(m_name + KEY_DTMF_COMMAND2, m_dtmfCommand2);
	m_config->Write(m_name + KEY_DTMF_COMMAND2_LINE, m_dtmfCommand2Line);
	m_config->Write(m_name + KEY_DTMF_OUTPUT1, m_dtmfOutput1);
	m_config->Write(m_name + KEY_DTMF_OUTPUT2, m_dtmfOutput2);
	m_config->Write(m_name + KEY_DTMF_OUTPUT3, m_dtmfOutput3);
	m_config->Write(m_name + KEY_DTMF_OUTPUT4, m_dtmfOutput4);
	m_config->Write(m_name + KEY_DTMF_THRESHOLD, double(m_dtmfThreshold));
	m_config->Write(m_name + KEY_OUTPUT1, m_output1);
	m_config->Write(m_name + KEY_OUTPUT2, m_output2);
	m_config->Write(m_name + KEY_OUTPUT3, m_output3);
	m_config->Write(m_name + KEY_OUTPUT4, m_output4);
	m_config->Write(m_name + KEY_ACTIVE_HANG_TIME, long(m_activeHangTime));
	m_config->Write(m_name + KEY_WINDOW_X, long(m_x));
	m_config->Write(m_name + KEY_WINDOW_Y, long(m_y));
	m_config->Flush();

	return true;
}

#else

bool CAnalogueRepeaterConfig::write()
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
	buffer.Printf(wxT("%s=%d"),   KEY_OPEN_ID_SOURCE.c_str(), int(m_openIdSource)); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"),   KEY_OPEN_ID.c_str(), m_openId.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"),   KEY_CLOSE_ID_SOURCE.c_str(), int(m_closeIdSource)); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"),   KEY_CLOSE_ID.c_str(), m_closeId.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"),   KEY_BEACON1_SOURCE.c_str(), int(m_beacon1Source)); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"),   KEY_BEACON1.c_str(), m_beacon1.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"),   KEY_BEACON2_SOURCE.c_str(), int(m_beacon2Source)); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"),   KEY_BEACON2.c_str(), m_beacon2.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%u"),   KEY_ID_SPEED.c_str(), m_idSpeed); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%u"),   KEY_ID_FREQ.c_str(), m_idFreq); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%.4f"), KEY_ID_LEVEL1.c_str(), m_idLevel1); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%.4f"), KEY_ID_LEVEL2.c_str(), m_idLevel2); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"),   KEY_ACK_RADIO_SOURCE.c_str(), int(m_ackRadioSource)); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"),   KEY_ACK_RADIO.c_str(), m_ackRadio.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"),   KEY_ACK_EXTERNAL_SOURCE.c_str(), int(m_ackExternalSource)); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"),   KEY_ACK_EXTERNAL.c_str(), m_ackExternal.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"),   KEY_ACK_BATTERY_SOURCE.c_str(), int(m_ackBatterySource)); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"),   KEY_ACK_BATTERY.c_str(), m_ackBattery.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%u"),   KEY_ACK_SPEED.c_str(), m_ackSpeed); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%u"),   KEY_ACK_FREQ.c_str(), m_ackFreq); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%.4f"), KEY_ACK_LEVEL.c_str(), m_ackLevel); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%u"),   KEY_ACK_DELAY.c_str(), m_ackDelay); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%u"),   KEY_ACK_MIN.c_str(), m_ackMin); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%u"),   KEY_CALLSIGNTIME.c_str(), m_callsignTime); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%u"),   KEY_TIMEOUT.c_str(), m_timeout); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%u"),   KEY_LOCKOUTTIME.c_str(), m_lockoutTime); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%u"),   KEY_HANGTIME.c_str(), m_hangTime); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%u"),   KEY_LATCHTIME.c_str(), m_latchTime); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"),   KEY_TONEBURST_ENABLE.c_str(), m_toneburstEnable ? 1 : 0); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%.4f"), KEY_TONEBURST_THRESHOLD.c_str(), m_toneburstThreshold); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%.1f"), KEY_CTCSS_FREQ.c_str(), m_ctcssFreq); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"),   KEY_CTCSS_INTERNAL.c_str(), m_ctcssInternal ? 1 : 0); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%.4f"), KEY_CTCSS_THRESHOLD.c_str(), m_ctcssThresh); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%.4f"), KEY_CTCSS_LEVEL.c_str(), m_ctcssLevel); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%u"),   KEY_CTCSS_HANG_TIME.c_str(), m_ctcssHangTime); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"),   KEY_CTCSS_OUTPUT.c_str(), int(m_ctcssOutput)); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"),   KEY_CALLSIGN_AT_START.c_str(), int(m_callAtStart)); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%u"),   KEY_CALLSIGN_START_DELAY.c_str(), m_callStartDelay); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"),   KEY_CALLSIGN_AT_END.c_str(), m_callAtEnd ? 1 : 0); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"),   KEY_TIMEOUT_TYPE.c_str(), int(m_timeoutType)); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"),   KEY_CALLSIGN_HOLDOFF.c_str(), int(m_callHoldoff)); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"),   KEY_RADIO_READ_DEVICE.c_str(), m_radioReadDevice.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"),   KEY_RADIO_WRITE_DEVICE.c_str(), m_radioWriteDevice.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%u"),   KEY_RADIO_AUDIO_DELAY.c_str(), m_radioDelay); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"),   KEY_RADIO_DEEMPHASIS.c_str(), m_radioDeEmphasis ? 1 : 0); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"),   KEY_RADIO_PREEMPHASIS.c_str(), m_radioPreEmphasis ? 1 : 0); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"),   KEY_RADIO_VOGAD.c_str(), m_radioVOGAD ? 1 : 0); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"),   KEY_EXTERNAL_MODE.c_str(), int(m_externalMode)); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"),   KEY_EXTERNAL_READ_DEVICE.c_str(), m_externalReadDevice.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"),   KEY_EXTERNAL_WRITE_DEVICE.c_str(), m_externalWriteDevice.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%u"),   KEY_EXTERNAL_AUDIO_DELAY.c_str(), m_externalDelay); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"),   KEY_EXTERNAL_DEEMPHASIS.c_str(), m_externalDeEmphasis ? 1 : 0); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"),   KEY_EXTERNAL_PREEMPHASIS.c_str(), m_externalPreEmphasis ? 1 : 0); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"),   KEY_EXTERNAL_VOGAD.c_str(), m_externalVOGAD ? 1 : 0); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"),   KEY_EXTERNAL_INTERFACE_DEVICE.c_str(), m_externalInterfaceDevice.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"),   KEY_EXTERNAL_INTERFACE_TX_PIN.c_str(), int(m_externalInterfaceTXPin)); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"),   KEY_EXTERNAL_INTERFACE_RX_PIN.c_str(), int(m_externalInterfaceRXPin)); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"),   KEY_EXTERNAL_BACKGROUND.c_str(), m_externalBackground ? 1 : 0); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"),   KEY_IFACE_TYPE.c_str(), m_interfaceType.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%u"),   KEY_IFACE_CONFIG.c_str(), m_interfaceConfig); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%u"),   KEY_PTT_DELAY.c_str(), m_pttDelay); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%u"),   KEY_SQUELCH_DELAY.c_str(), m_squelchDelay); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"),   KEY_PTT_INVERT.c_str(), m_pttInvert ? 1 : 0); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"),   KEY_SQUELCH_INVERT.c_str(), m_squelchInvert ? 1 : 0); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"),   KEY_DTMF_RADIO.c_str(), m_dtmfRadio ? 1 : 0); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"),   KEY_DTMF_EXTERNAL.c_str(), m_dtmfExternal ? 1 : 0); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"),   KEY_DTMF_SHUTDOWN.c_str(), m_dtmfShutdown.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"),   KEY_DTMF_STARTUP.c_str(), m_dtmfStartup.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"),   KEY_DTMF_TIMEOUT.c_str(), m_dtmfTimeout.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"),   KEY_DTMF_TIMERESET.c_str(), m_dtmfTimeReset.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"),   KEY_DTMF_COMMAND1.c_str(), m_dtmfCommand1.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"),   KEY_DTMF_COMMAND1_LINE.c_str(), m_dtmfCommand1Line.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"),   KEY_DTMF_COMMAND2.c_str(), m_dtmfCommand2.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"),   KEY_DTMF_COMMAND2_LINE.c_str(), m_dtmfCommand2Line.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"),   KEY_DTMF_OUTPUT1.c_str(), m_dtmfOutput1.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"),   KEY_DTMF_OUTPUT2.c_str(), m_dtmfOutput2.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"),   KEY_DTMF_OUTPUT3.c_str(), m_dtmfOutput3.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"),   KEY_DTMF_OUTPUT4.c_str(), m_dtmfOutput4.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%.4f"), KEY_DTMF_THRESHOLD.c_str(), m_dtmfThreshold); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%u"),   KEY_ACTIVE_HANG_TIME.c_str(), m_activeHangTime); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"),   KEY_OUTPUT1.c_str(), m_output1 ? 1 : 0); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"),   KEY_OUTPUT2.c_str(), m_output2 ? 1 : 0); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"),   KEY_OUTPUT3.c_str(), m_output3 ? 1 : 0); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"),   KEY_OUTPUT4.c_str(), m_output4 ? 1 : 0); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"),   KEY_WINDOW_X.c_str(), m_x); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"),   KEY_WINDOW_Y.c_str(), m_y); file.AddLine(buffer);

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
