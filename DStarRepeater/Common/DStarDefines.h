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

const unsigned char DATA_SYNC_BYTES[] = {0x55, 0x2D, 0x16};

const unsigned char END_PATTERN_BYTES[] = {0x55, 0x55, 0x55, 0x55, 0xC8, 0x7A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
const unsigned int END_PATTERN_LENGTH_BYTES = 6U;

const unsigned char NULL_AMBE_DATA_BYTES[] = {0x9E, 0x8D, 0x32, 0x88, 0x26, 0x1A, 0x3F, 0x61, 0xE8};

// Note that these are already scrambled, 0x66 0x66 0x66 otherwise
const unsigned char NULL_SLOW_DATA_BYTES[] = {0x16, 0x29, 0xF5};

const unsigned char NULL_FRAME_DATA_BYTES[] = {0x9E, 0x8D, 0x32, 0x88, 0x26, 0x1A, 0x3F, 0x61, 0xE8, 0x16, 0x29, 0xF5};

const unsigned int VOICE_FRAME_LENGTH_BYTES  = 9U;

const unsigned int DATA_FRAME_LENGTH_BYTES   = 3U;

const unsigned int DV_FRAME_LENGTH_BYTES     = VOICE_FRAME_LENGTH_BYTES + DATA_FRAME_LENGTH_BYTES;

// The length of the end frame, three bytes extra
const unsigned int DV_FRAME_MAX_LENGTH_BYTES = DV_FRAME_LENGTH_BYTES + 3U;

const unsigned int FEC_SECTION_LENGTH_BYTES = 83U;

const unsigned int RADIO_HEADER_LENGTH_BYTES = 41U;

const unsigned int DATA_BLOCK_SIZE_BYTES = 21U * DV_FRAME_LENGTH_BYTES;

const unsigned int DSTAR_RADIO_SAMPLE_RATE = 48000U;
const unsigned int DSTAR_RADIO_BLOCK_SIZE  = 960U;
const unsigned int DSTAR_AUDIO_BLOCK_SIZE  = 160U;

const unsigned int LOCATOR_LENGTH        = 6U;
const unsigned int LONG_CALLSIGN_LENGTH  = 8U;
const unsigned int SHORT_CALLSIGN_LENGTH = 4U;

const unsigned char SLOW_DATA_TYPE_MASK    = 0xF0U;
const unsigned char SLOW_DATA_TYPE_GPSDATA = 0x30U;
const unsigned char SLOW_DATA_TYPE_TEXT    = 0x40U;
const unsigned char SLOW_DATA_TYPE_HEADER  = 0x50U;
const unsigned char SLOW_DATA_TYPE_SQUELCH = 0xC0U;
const unsigned char SLOW_DATA_LENGTH_MASK  = 0x0FU;

const unsigned int  SLOW_DATA_TEXT_LENGTH  = 20U;

const unsigned char SCRAMBLER_BYTE1 = 0x70U;
const unsigned char SCRAMBLER_BYTE2 = 0x4FU;
const unsigned char SCRAMBLER_BYTE3 = 0x93U;

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

const unsigned int  DSTAR_RADIO_BIT_LENGTH = DSTAR_RADIO_SAMPLE_RATE / DSTAR_GMSK_SYMBOL_RATE;

const unsigned int  DSTAR_FRAME_TIME_MS  = 20U;
const unsigned int  DSTAR_FRAMES_PER_SEC = 50U;

const unsigned int  DSTAR_TICKS_PER_SEC = DSTAR_RADIO_SAMPLE_RATE / DSTAR_RADIO_BLOCK_SIZE;

const unsigned int  DEXTRA_PORT = 30001U;
const unsigned int  DCS_PORT    = 30051U;

const unsigned int  GMSK_MODEM_DATA_LENGTH = 8U;

const unsigned int  RADIO_RUN_FRAME_COUNT   = 5U;
const unsigned int  LOCAL_RUN_FRAME_COUNT   = 1U;
const unsigned int  NETWORK_RUN_FRAME_COUNT = 25U;

const unsigned int  DSTAR_BLEEP_FREQ   = 2000U;
const unsigned int  DSTAR_BLEEP_LENGTH = 100U;
const float         DSTAR_BLEEP_AMPL   = 0.5F;

const unsigned int  NETWORK_TIMEOUT = 1U;

const unsigned int SPLIT_RX_GUI_COUNT = 5U;
const unsigned int SPLIT_RX_COUNT     = 25U;
const unsigned int SPLIT_TX_GUI_COUNT = 3U;
const unsigned int SPLIT_TX_COUNT     = 5U;

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
	NETWORK_TEXT,
	NETWORK_TEMPTEXT,
	NETWORK_STATUS1,
	NETWORK_STATUS2,
	NETWORK_STATUS3,
	NETWORK_STATUS4,
	NETWORK_STATUS5,
	NETWORK_REGISTER
};

enum DSTAR_MODE {
	MODE_DUPLEX,
	MODE_SIMPLEX,
	MODE_GATEWAY,
	MODE_TXONLY,
	MODE_RXONLY,
	MODE_TXANDRX
};

enum LINK_STATUS {
	LS_NONE,
	LS_PENDING_IRCDDB,
	LS_LINKING_LOOPBACK,
	LS_LINKING_DEXTRA,
	LS_LINKING_DPLUS,
	LS_LINKING_DCS,
	LS_LINKING_CCS,
	LS_LINKED_LOOPBACK,
	LS_LINKED_DEXTRA,
	LS_LINKED_DPLUS,
	LS_LINKED_DCS,
	LS_LINKED_CCS
};

enum ACK_TYPE {
	AT_NONE,
	AT_BER,
	AT_STATUS
};

enum TEXT_LANG {
	TL_ENGLISH_UK,
	TL_DEUTSCH,
	TL_DANSK,
	TL_FRANCAIS,
	TL_ITALIANO,
	TL_POLSKI,
	TL_ESPANOL,
	TL_SVENSKA,
	TL_NEDERLANDS,
	TL_ENGLISH_US,
	TL_NORSK
};

enum CONNECTION_TYPE {
	CT_USB,
	CT_NETWORK
};

enum USB_INTERFACE {
	UI_LIBUSB,
	UI_WINUSB
};

enum DVMEGA_VARIANT {
	DVMV_NODE,
	DVMV_RADIO
};

#endif
