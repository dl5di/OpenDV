/*
 *	Copyright (C) 2009,2010,2011,2013 by Jonathan Naylor, G4KLX
 *
 *	This program is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation; version 2 of the License.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 */

#ifndef	AnalogueDefines_H
#define	AnalogueDefines_H

#include <wx/wx.h>

const unsigned int ANALOGUE_RADIO_SAMPLE_RATE = 48000U;
const unsigned int ANALOGUE_RADIO_BLOCK_SIZE  = 960U;

const wxFloat32 MAX_VOGAD_GAIN  = 6.0F;
const wxFloat32 MAX_AUDIO_LEVEL = 0.5F;

const unsigned int ANALOGUE_FRAME_TIME_MS = 20U;

const unsigned int ANALOGUE_TICKS_PER_SEC = ANALOGUE_RADIO_SAMPLE_RATE / ANALOGUE_RADIO_BLOCK_SIZE;

const int SOUNDCARD_RADIO    = 0;
const int SOUNDCARD_EXTERNAL = 1;

enum ANALOGUE_RPT_STATE {
	ARS_SHUTDOWN,
	ARS_LISTENING,
	ARS_RELAYING_RADIO,
	ARS_RELAYING_EXTERNAL,
	ARS_WAITING_RADIO,
	ARS_WAITING_EXTERNAL,
	ARS_TIMEOUT_RADIO,
	ARS_LOCKOUT_RADIO,
	ARS_TIMEOUT_EXTERNAL,
	ARS_LOCKOUT_EXTERNAL
};

enum ANALOGUE_SQUELCH {
	AS_CLOSED,
	AS_CARRIER,
	AS_CTCSS,
	AS_TONEBURST,
	AS_EXTERNAL
};

enum ANALOGUE_CALLSIGN_START {
	ACS_NONE,
	ACS_OPEN,
	ACS_LATCH
};

enum ANALOGUE_EXTERNAL_MODE {
	AEM_DISABLED,
	AEM_ENABLED,
	AEM_CONTROL
};

enum ANALOGUE_CALLSIGN_HOLDOFF {
	ACH_NONE,
	ACH_14,
	ACH_12,
	ACH_34
};

enum ANALOGUE_CTCSS_OUTPUT {
	ACO_NONE,
	ACO_WHEN_OPEN,
	ACO_ON_AUDIO,
	ACO_ALWAYS
};

enum AUDIO_SOURCE {
	AS_CW_KEYER,
	AS_WAV_FILE
};

#endif
