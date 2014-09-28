/*
 *	Copyright (C) 2009-2014 by Jonathan Naylor, G4KLX
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

#ifndef	DStarDefines_H
#define	DStarDefines_H

#include <wx/wx.h>

const unsigned int DSTAR_GMSK_SYMBOL_RATE = 4800U;
const float        DSTAR_GMSK_BT          = 0.5F;

const bool BIT_SYNC_BITS[]    = {true, false, true, false};
const unsigned int BIT_SYNC_LENGTH_BITS = 4U;

const bool FRAME_SYNC_BITS[]  = {true, true,  true, false, true,  true,  false, false,
							     true, false, true, false, false, false, false};
const unsigned int FRAME_SYNC_LENGTH_BITS = 15U;

const unsigned char DATA_SYNC_BYTES[] = {0x55, 0x2D, 0x16};
const bool DATA_SYNC_BITS[]   = {true,  false, true,  false, true,  false, true,  false,
							     true,  false, true,  true,  false, true,  false, false,
							     false, true,  true,  false, true,  false, false, false};

const unsigned char END_PATTERN_BYTES[] = {0x55, 0x55, 0x55, 0x55, 0xC8, 0x7A,
										   0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
const bool END_PATTERN_BITS[] = {true,  false, true,  false, true,  false, true,  false,
							     true,  false, true,  false, true,  false, true,  false,
							     true,  false, true,  false, true,  false, true,  false,
							     true,  false, true,  false, true,  false, true,  false,
							     false, false, false, true,  false, false, true,  true,
							     false, true,  false, true,  true,  true,  true,  false};
const unsigned int END_PATTERN_LENGTH_BITS  = 48U;
const unsigned int END_PATTERN_LENGTH_BYTES = END_PATTERN_LENGTH_BITS / 8U;

const unsigned char NULL_AMBE_DATA_BYTES[] = {0x9E, 0x8D, 0x32, 0x88, 0x26, 0x1A, 0x3F, 0x61, 0xE8};
const bool NULL_AMBE_DATA_BITS[] = {false, true,  true,  true,  true,  false, false, true,
									true,  false, true,  true,  false, false, false, true,
									false, true,  false, false, true,  true,  false, false,
									false, false, false, true,  false, false, false, true,
									false, true,  true,  false, false, true,  false, false,
									false, true,  false, true,  true,  false, false, false,
									true,  true,  true,  true,  true,  true,  false, false,
									true,  false, false, false, false, true,  true,  false,
									false, false, false, true,  false, true,  true,  true};

// Note that these are already scrambled, 0x66 0x66 0x66 otherwise
const unsigned char NULL_SLOW_DATA_BYTES[] = {0x16, 0x29, 0xF5};
const bool NULL_SLOW_DATA_BITS[] = {false, true,  true,  false, true,  false, false, false,
									true,  false, false, true,  false, true,  false, false,
									true,  false, true,  false, true,  true,  true,  true};

const unsigned int VOICE_FRAME_LENGTH_BITS   = 72U;
const unsigned int VOICE_FRAME_LENGTH_BYTES  = VOICE_FRAME_LENGTH_BITS / 8U;

const unsigned int DATA_FRAME_LENGTH_BITS    = 24U;
const unsigned int DATA_FRAME_LENGTH_BYTES   = DATA_FRAME_LENGTH_BITS / 8U;

const unsigned int DV_FRAME_LENGTH_BITS      = VOICE_FRAME_LENGTH_BITS + DATA_FRAME_LENGTH_BITS;
const unsigned int DV_FRAME_LENGTH_BYTES     = VOICE_FRAME_LENGTH_BYTES + DATA_FRAME_LENGTH_BYTES;

// The length of the end frame, three bytes extra
const unsigned int DV_FRAME_MAX_LENGTH_BITS  = DV_FRAME_LENGTH_BITS + 24U;
const unsigned int DV_FRAME_MAX_LENGTH_BYTES = DV_FRAME_MAX_LENGTH_BITS / 8U;

const unsigned int FEC_SECTION_LENGTH_BITS  = 660U;

const unsigned int RADIO_HEADER_LENGTH_BITS  = 330U;
const unsigned int RADIO_HEADER_LENGTH_BYTES = 41U;

const unsigned int DATA_BLOCK_SIZE_BITS  = 21U * DV_FRAME_LENGTH_BITS;
const unsigned int DATA_BLOCK_SIZE_BYTES = 21U * DV_FRAME_LENGTH_BYTES;

const unsigned int LONG_CALLSIGN_LENGTH  = 8U;
const unsigned int SHORT_CALLSIGN_LENGTH = 4U;

const unsigned char SLOW_DATA_TYPE_MASK   = 0xF0U;
const unsigned char SLOW_DATA_TYPE_GPS    = 0x30U;
const unsigned char SLOW_DATA_TYPE_TEXT   = 0x40U;
const unsigned char SLOW_DATA_TYPE_HEADER = 0x50U;

const unsigned char DATA_MASK           = 0x80U;
const unsigned char REPEATER_MASK       = 0x40U;
const unsigned char INTERRUPTED_MASK    = 0x20U;
const unsigned char CONTROL_SIGNAL_MASK = 0x10U;
const unsigned char URGENT_MASK         = 0x08U;

const unsigned char REPEATER_CONTROL_MASK = 0x07U;
const unsigned char REPEATER_CONTROL      = 0x07U;
const unsigned char AUTO_REPLY            = 0x06U;
const unsigned char RESEND_REQUESTED      = 0x04U;
const unsigned char ACK_FLAG              = 0x03U;
const unsigned char NO_RESPONSE           = 0x02U;
const unsigned char RELAY_UNAVAILABLE     = 0x01U;

const unsigned int  DSTAR_FRAME_TIME_MS  = 20U;
const unsigned int  DSTAR_FRAMES_PER_SEC = 50U;

const unsigned char SCRAMBLER_BYTE1 = 0x70U;
const unsigned char SCRAMBLER_BYTE2 = 0x4FU;
const unsigned char SCRAMBLER_BYTE3 = 0x93U;

const unsigned int  DPLUS_PORT  = 20001U;
const unsigned int  DEXTRA_PORT = 30001U;
const unsigned int  DCS_PORT    = 30051U;
const unsigned int  CCS_PORT    = 30061U;
const unsigned int  G2_DV_PORT  = 40000U;
const unsigned int  G2_DD_PORT  = 40001U;

const unsigned int NETWORK_TIMEOUT  = 1U;		// Network timeout for G2, CCS, DCS, DExtra, and D-Plus
const unsigned int REPEATER_TIMEOUT = 2U;		// Repeater timeout
const unsigned int REPLY_TIME       = 2U;		// The turnaround time for version, echo, audio prompts

enum DSTAR_PROTOCOL {
	DP_UNKNOWN,
	DP_LOOPBACK,
	DP_DEXTRA,
	DP_DPLUS,
	DP_DCS
};

enum AUDIO_SOURCE {
	AS_G2,
	AS_ECHO,
	AS_INFO,
	AS_XBAND,
	AS_DRATS,
	AS_DPLUS,
	AS_DEXTRA,
	AS_DCS,
	AS_DUP,
	AS_VERSION,
	AS_CCS
};

enum DSTAR_RX_STATE {
	DSRXS_LISTENING,
	DSRXS_PROCESS_HEADER,
	DSRXS_PROCESS_DATA,
	DSRXS_PROCESS_SLOW_DATA
};

enum DSTAR_RPT_STATE {
	DSRS_SHUTDOWN,
	DSRS_LISTENING,
	DSRS_VALID,
	DSRS_VALID_WAIT,
	DSRS_INVALID,
	DSRS_INVALID_WAIT,
	DSRS_TIMEOUT,
	DSRS_TIMEOUT_WAIT,
	DSRS_NETWORK
};

enum NETWORK_TYPE {
	NETWORK_NONE,
	NETWORK_HEADER,
	NETWORK_DATA,
	NETWORK_TEXT
};

enum DSTAR_MODE {
	MODE_DUPLEX,
	MODE_SIMPLEX,
	MODE_GATEWAY
};

#endif
