/*
 *   Copyright (C) 2015 by Rick Schnicker KD0OSS
 *   based on code by Jonathan Naylor G4KLX
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

#include "STARDVNetworkController.h"
#include "DStarDefines.h"
#include "Utils.h"

const unsigned char AMBE_START_BYTE   = 0x61U;

const unsigned char AMBE_TYPE_CONTROL = 0x00U;
const unsigned char AMBE_TYPE_AMBE    = 0x01U;
const unsigned char AMBE_TYPE_AUDIO   = 0x02U;

const unsigned char AMBE_CONTROL_RATEP  = 0x0AU;
const unsigned char AMBE_CONTROL_CODEC  = 0x2AU;
const unsigned char AMBE_CONTROL_PRODID = 0x30U;
const unsigned char AMBE_CONTROL_READY  = 0x39U;

const unsigned char AMBE_REQ_PRODID[]     = {AMBE_START_BYTE, 0x00U, 0x01U, AMBE_TYPE_CONTROL, AMBE_CONTROL_PRODID};
const unsigned int AMBE_REQ_PRODID_LEN    = 5U;

const unsigned char AMBE_REQ_RATEP[]      = {AMBE_START_BYTE, 0x00U, 0x0DU, AMBE_TYPE_CONTROL, AMBE_CONTROL_RATEP, 0x01U, 0x30U, 0x07U, 0x63U, 0x40U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x48U};
const unsigned int AMBE_REQ_RATEP_LEN     = 17U;

const unsigned char AMBE_AUDIO_HEADER[]   = {AMBE_START_BYTE, 0x01U, 0x42U, AMBE_TYPE_AUDIO, 0x00U, 0xA0U};
const unsigned char AMBE_AUDIO_HEADER_LEN = 6U;

const unsigned char AMBE_AMBE_HEADER[]    = {AMBE_START_BYTE, 0x00U, 0x0BU, AMBE_TYPE_AMBE, 0x01U, 0x48U};
const unsigned char AMBE_AMBE_HEADER_LEN  = 6U;

const unsigned int AMBE_HEADER_LEN = 4U;

const unsigned char AMBE_CODEC_START[]    = {AMBE_START_BYTE, 0x00U, 0x02U, AMBE_TYPE_CONTROL, AMBE_CONTROL_CODEC, 0x04U};
const unsigned int  AMBE_CODEC_START_LEN  = 6U;

const unsigned char AMBE_CODEC_STOP[]     = {AMBE_START_BYTE, 0x00U, 0x01U, AMBE_TYPE_CONTROL, 0x2BU};
const unsigned int  AMBE_CODEC_STOP_LEN   = 5U;

const unsigned int BUFFER_LENGTH = 400U;

CSTARDVNetworkController::CSTARDVNetworkController(const wxString& address, unsigned int port) :
m_socket(address, port)
{
}

CSTARDVNetworkController::~CSTARDVNetworkController()
{
}

bool CSTARDVNetworkController::open()
{
	bool res = m_socket.open();
	if (!res)
		return false;

	m_socket.write(AMBE_REQ_PRODID, AMBE_REQ_PRODID_LEN);

	bool found = false;
	for (unsigned int i = 0U; i < 10U; i++) {
		unsigned char buffer[BUFFER_LENGTH];
		RESP_TYPE type = getResponse(buffer, BUFFER_LENGTH);

		if (type == RESP_ERROR) {
			m_socket.close();
			return false;
		}

		if (type == RESP_NAME) {
			wxString name((char*)(buffer + 5U), wxConvLocal);
			wxLogMessage(wxT("AMBE chip identified as: %s"), name.c_str());
			found = true;
			break;
		}

		::wxMilliSleep(10UL);
	}

	if (!found) {
		m_socket.close();
		return false;
	}
	
	m_socket.write(AMBE_REQ_RATEP, AMBE_REQ_RATEP_LEN);
    
    for (;;) {
        unsigned char buffer[BUFFER_LENGTH];
        RESP_TYPE type = getResponse(buffer, BUFFER_LENGTH);
        
        if (type == RESP_ERROR) {
            m_socket.close();
            return false;
        }
        
        if (type == RESP_RATEP)
            break;
        
        ::wxMilliSleep(10UL);
    }
    
    m_socket.write(AMBE_CODEC_START, AMBE_CODEC_START_LEN);
    
    for (;;) {
        unsigned char buffer[BUFFER_LENGTH];
        RESP_TYPE type = getResponse(buffer, BUFFER_LENGTH);
        
        if (type == RESP_ERROR) {
            m_socket.close();
            return false;
        }
        
        if (type == RESP_CODEC)
            return true;
        
        ::wxMilliSleep(10UL);
    }
}

void CSTARDVNetworkController::encodeIn(const wxFloat32* audio, unsigned int length)
{
	wxASSERT(audio != NULL);
	wxASSERT(length == DSTAR_AUDIO_BLOCK_SIZE);

	unsigned char buffer[AMBE_AUDIO_HEADER_LEN + DSTAR_AUDIO_BLOCK_BYTES];
	::memcpy(buffer, AMBE_AUDIO_HEADER, AMBE_AUDIO_HEADER_LEN);

	wxInt8* q = (wxInt8*)(buffer + AMBE_AUDIO_HEADER_LEN);
	for (unsigned int i = 0; i < DSTAR_AUDIO_BLOCK_SIZE; i++, q += 2U) {
		wxInt16 word = wxInt16(audio[i] * 32767.0F);

		q[0U] = (word & 0xFF00) >> 8;
		q[1U] = (word & 0x00FF) >> 0;
	}

//	m_socket.write(buffer, AMBE_AUDIO_HEADER_LEN + DSTAR_AUDIO_BLOCK_BYTES);
}

bool CSTARDVNetworkController::encodeOut(unsigned char* ambe, unsigned int length, bool& tx)
{
	wxASSERT(ambe != NULL);
	wxASSERT(length == VOICE_FRAME_LENGTH_BYTES);

	unsigned char buffer[BUFFER_LENGTH];
	RESP_TYPE type = getResponse(buffer, BUFFER_LENGTH);
	if (type != RESP_AMBE)
		return false;

    tx = (buffer[1] & 0x10U) == 0x10U;
	::memcpy(ambe, buffer + AMBE_AMBE_HEADER_LEN, VOICE_FRAME_LENGTH_BYTES);

	return true;
}

void CSTARDVNetworkController::decodeIn(const unsigned char* ambe, unsigned int length)
{
	wxASSERT(ambe != NULL);
	wxASSERT(length == VOICE_FRAME_LENGTH_BYTES);

	unsigned char buffer[AMBE_AMBE_HEADER_LEN + VOICE_FRAME_LENGTH_BYTES];
	::memcpy(buffer, AMBE_AMBE_HEADER, AMBE_AMBE_HEADER_LEN);

	::memcpy(buffer + AMBE_AMBE_HEADER_LEN, ambe, VOICE_FRAME_LENGTH_BYTES);

	m_socket.write(buffer, AMBE_AMBE_HEADER_LEN + VOICE_FRAME_LENGTH_BYTES);
}

bool CSTARDVNetworkController::decodeOut(wxFloat32* audio, unsigned int length)
{
	wxASSERT(audio != NULL);
	wxASSERT(length == DSTAR_AUDIO_BLOCK_SIZE);

	unsigned char buffer[BUFFER_LENGTH];
	RESP_TYPE type = getResponse(buffer, BUFFER_LENGTH);
	if (type != RESP_AUDIO)
		return false;

	wxUint8* q = (wxUint8*)(buffer + AMBE_AUDIO_HEADER_LEN);
	for (unsigned int i = 0U; i < DSTAR_AUDIO_BLOCK_SIZE; i++, q += 2U) {
		wxInt16 word = (q[0] << 8) | (q[1U] << 0);

		audio[i] = wxFloat32(word) / 32768.0F;
	}

	return true;
}

void CSTARDVNetworkController::close()
{
    m_socket.write(AMBE_CODEC_STOP, AMBE_CODEC_STOP_LEN);
    m_socket.close();
}

CSTARDVNetworkController::RESP_TYPE CSTARDVNetworkController::getResponse(unsigned char* buffer, unsigned int length)
{
	wxASSERT(buffer != NULL);
	wxASSERT(length >= BUFFER_LENGTH);

	int len = m_socket.read(buffer, length);
	if (len == 0)
		return RESP_NONE;
	if (len < 0)
		return RESP_ERROR;

	if (buffer[0U] != AMBE_START_BYTE) {
		wxLogError(wxT("Unknown byte from the AMBE, %02X"), buffer[0U]);
		return RESP_UNKNOWN;
	}

	unsigned int respLen = (buffer[1U] & 0x0f) * 256U + buffer[2U] + AMBE_HEADER_LEN;

	if (len != int(respLen)) {
		wxLogError(wxT("Invalid AMBE data, %d != %u"), len, respLen);
		CUtils::dump(wxT("Bad data"), buffer, len);
		return RESP_ERROR;
	}

	if (buffer[3U] == AMBE_TYPE_AUDIO) {
		return RESP_AUDIO;
	} else if (buffer[3U] == AMBE_TYPE_AMBE) {
		return RESP_AMBE;
	} else if (buffer[3U] == AMBE_TYPE_CONTROL) {
		if (buffer[4U] == AMBE_CONTROL_PRODID) {
			return RESP_NAME;
        } else if (buffer[4U] == AMBE_CONTROL_RATEP) {
            return RESP_RATEP;
        } else if (buffer[4U] == AMBE_CONTROL_CODEC) {
            return RESP_CODEC;
        } else if (buffer[4U] == AMBE_CONTROL_READY) {
			return RESP_UNKNOWN;
		} else {
			CUtils::dump(wxT("Unknown control data"), buffer, respLen);
			return RESP_UNKNOWN;
		}
	} else {
		CUtils::dump(wxT("Unknown data"), buffer, respLen);
		return RESP_UNKNOWN;
	}
}
