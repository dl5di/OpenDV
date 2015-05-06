/*
 *   Copyright (C) 2014,2015 by Jonathan Naylor G4KLX
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

#include "DV3000SerialController.h"
#include "DStarDefines.h"
#include "Utils.h"

const unsigned char DV3000_START_BYTE   = 0x61U;

const unsigned char DV3000_TYPE_CONTROL = 0x00U;
const unsigned char DV3000_TYPE_AMBE    = 0x01U;
const unsigned char DV3000_TYPE_AUDIO   = 0x02U;

const unsigned char DV3000_CONTROL_RATEP  = 0x0AU;
const unsigned char DV3000_CONTROL_PRODID = 0x30U;
const unsigned char DV3000_CONTROL_READY  = 0x39U;

const unsigned char DV3000_REQ_PRODID[]     = {DV3000_START_BYTE, 0x00U, 0x01U, DV3000_TYPE_CONTROL, DV3000_CONTROL_PRODID};
const unsigned int DV3000_REQ_PRODID_LEN    = 5U;

const unsigned char DV3000_REQ_RATEP[]      = {DV3000_START_BYTE, 0x00U, 0x0DU, DV3000_TYPE_CONTROL, DV3000_CONTROL_RATEP, 0x01U, 0x30U, 0x07U, 0x63U, 0x40U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x48U};
const unsigned int DV3000_REQ_RATEP_LEN     = 17U;

const unsigned char DV3000_AUDIO_HEADER[]   = {DV3000_START_BYTE, 0x01U, 0x42U, DV3000_TYPE_AUDIO, 0x00U, 0xA0U};
const unsigned char DV3000_AUDIO_HEADER_LEN = 6U;

const unsigned char DV3000_AMBE_HEADER[]    = {DV3000_START_BYTE, 0x00U, 0x0BU, DV3000_TYPE_AMBE, 0x01U, 0x48U};
const unsigned char DV3000_AMBE_HEADER_LEN  = 6U;

const unsigned int DV3000_HEADER_LEN = 4U;

const unsigned int BUFFER_LENGTH = 400U;

CDV3000SerialController::CDV3000SerialController(const wxString& device) :
m_serial(device, SERIAL_230400)
{
}

CDV3000SerialController::~CDV3000SerialController()
{
}

bool CDV3000SerialController::open()
{
	bool res = m_serial.open();
	if (!res)
		return false;

	m_serial.write(DV3000_REQ_PRODID, DV3000_REQ_PRODID_LEN);

	bool found = false;
	for (unsigned int i = 0U; i < 10U; i++) {
		unsigned char buffer[BUFFER_LENGTH];
		RESP_TYPE type = getResponse(buffer, BUFFER_LENGTH);

		if (type == RESP_ERROR) {
			m_serial.close();
			return false;
		}

		if (type == RESP_NAME) {
			wxString name((char*)(buffer + 5U), wxConvLocal);
			wxLogMessage(wxT("DV3000 chip identified as: %s"), name.c_str());
			found = true;
			break;
		}

		::wxMilliSleep(10UL);
	}

	if (!found) {
		m_serial.close();
		return false;
	}

	m_serial.write(DV3000_REQ_RATEP, DV3000_REQ_RATEP_LEN);

	for (;;) {
		unsigned char buffer[BUFFER_LENGTH];
		RESP_TYPE type = getResponse(buffer, BUFFER_LENGTH);

		if (type == RESP_ERROR) {
			m_serial.close();
			return false;
		}

		if (type == RESP_RATEP)
			return true;

		::wxMilliSleep(10UL);
	}
}

void CDV3000SerialController::encodeIn(const wxFloat32* audio, unsigned int length)
{
	wxASSERT(audio != NULL);
	wxASSERT(length == DSTAR_AUDIO_BLOCK_SIZE);

	unsigned char buffer[DV3000_AUDIO_HEADER_LEN + DSTAR_AUDIO_BLOCK_BYTES];
	::memcpy(buffer, DV3000_AUDIO_HEADER, DV3000_AUDIO_HEADER_LEN);

	wxInt8* q = (wxInt8*)(buffer + DV3000_AUDIO_HEADER_LEN);
	for (unsigned int i = 0; i < DSTAR_AUDIO_BLOCK_SIZE; i++, q += 2U) {
		wxInt16 word = wxInt16(audio[i] * 32767.0F);

		q[0U] = (word & 0xFF00) >> 8;
		q[1U] = (word & 0x00FF) >> 0;
	}

	m_serial.write(buffer, DV3000_AUDIO_HEADER_LEN + DSTAR_AUDIO_BLOCK_BYTES);
}

bool CDV3000SerialController::encodeOut(unsigned char* ambe, unsigned int length)
{
	wxASSERT(ambe != NULL);
	wxASSERT(length == VOICE_FRAME_LENGTH_BYTES);

	unsigned char buffer[BUFFER_LENGTH];
	RESP_TYPE type = getResponse(buffer, BUFFER_LENGTH);
	if (type != RESP_AMBE)
		return false;

	::memcpy(ambe, buffer + DV3000_AMBE_HEADER_LEN, VOICE_FRAME_LENGTH_BYTES);

	return true;
}

void CDV3000SerialController::decodeIn(const unsigned char* ambe, unsigned int length)
{
	wxASSERT(ambe != NULL);
	wxASSERT(length == VOICE_FRAME_LENGTH_BYTES);

	unsigned char buffer[DV3000_AMBE_HEADER_LEN + VOICE_FRAME_LENGTH_BYTES];
	::memcpy(buffer, DV3000_AMBE_HEADER, DV3000_AMBE_HEADER_LEN);

	::memcpy(buffer + DV3000_AMBE_HEADER_LEN, ambe, VOICE_FRAME_LENGTH_BYTES);

	m_serial.write(buffer, DV3000_AMBE_HEADER_LEN + VOICE_FRAME_LENGTH_BYTES);
}

bool CDV3000SerialController::decodeOut(wxFloat32* audio, unsigned int length)
{
	wxASSERT(audio != NULL);
	wxASSERT(length == DSTAR_AUDIO_BLOCK_SIZE);

	unsigned char buffer[BUFFER_LENGTH];
	RESP_TYPE type = getResponse(buffer, BUFFER_LENGTH);
	if (type != RESP_AUDIO)
		return false;

	wxUint8* q = (wxUint8*)(buffer + DV3000_AUDIO_HEADER_LEN);
	for (unsigned int i = 0U; i < DSTAR_AUDIO_BLOCK_SIZE; i++, q += 2U) {
		wxInt16 word = (q[0] << 8) | (q[1U] << 0);

		audio[i] = wxFloat32(word) / 32768.0F;
	}

	return true;
}

void CDV3000SerialController::close()
{
	m_serial.close();
}

CDV3000SerialController::RESP_TYPE CDV3000SerialController::getResponse(unsigned char* buffer, unsigned int length)
{
	wxASSERT(buffer != NULL);
	wxASSERT(length >= BUFFER_LENGTH);

	int len1 = m_serial.read(buffer, 1U);
	if (len1 == 0)
		return RESP_NONE;
	if (len1 < 0)
		return RESP_ERROR;

	unsigned int offset = 3U;	// The normal case

	if (buffer[0U] != DV3000_START_BYTE) {
		int len2 = m_serial.read(buffer + 1U, 2U);
		if (len2 == 0)
			return RESP_NONE;
		if (len2 < 0)
			return RESP_ERROR;

		bool found = false;
		for (unsigned int i = 1U; i < 4U && !found; i++) {
			int ret1 = ::memcmp(buffer, DV3000_AUDIO_HEADER + i, 3U);
			int ret2 = ::memcmp(buffer, DV3000_AMBE_HEADER + i, 3U);

			if (ret1 == 0) {
				::memcpy(buffer + i, buffer + 0U, 3U);
				::memcpy(buffer, DV3000_AUDIO_HEADER, i);
				offset = i + 3U;
				found = true;
			} else if (ret2 == 0) {
				::memcpy(buffer + i, buffer + 0U, 3U);
				::memcpy(buffer, DV3000_AMBE_HEADER, i);
				offset = i + 3U;
				found = true;
			}
		}

		if (!found) {
			wxLogError(wxT("Unknown bytes from the DV3000, %02X %02X %02X"), buffer[0U], buffer[1U], buffer[2U]);
			return RESP_UNKNOWN;
		}
	} else {
		int len2 = m_serial.read(buffer + 1U, 2U);
		if (len2 == 0)
			return RESP_NONE;
		if (len2 < 0)
			return RESP_ERROR;

		if (len2 != 2) {
			wxLogError(wxT("Invalid DV3000 data read, %d != 2"), len2);
			CUtils::dump(wxT("Bad data"), buffer, len2 + 1U);
			return RESP_ERROR;
		}
	}

	unsigned int respLen = (buffer[1U] & 0x0FU) * 256U + buffer[2U] + DV3000_HEADER_LEN;

	int len3 = m_serial.read(buffer + offset, respLen - offset);
	if (len3 == 0)
		return RESP_NONE;
	if (len3 < 0)
		return RESP_ERROR;

	if (len3 != int(respLen - offset)) {
		wxLogError(wxT("Invalid DV3000 data, %d != %u"), len3, respLen - offset);
		CUtils::dump(wxT("Bad data"), buffer, len3 + offset);
		return RESP_ERROR;
	}

	if (buffer[3U] == DV3000_TYPE_AUDIO) {
		return RESP_AUDIO;
	} else if (buffer[3U] == DV3000_TYPE_AMBE) {
		return RESP_AMBE;
	} else if (buffer[3U] == DV3000_TYPE_CONTROL) {
		if (buffer[4U] == DV3000_CONTROL_PRODID) {
			return RESP_NAME;
		} else if (buffer[4U] == DV3000_CONTROL_RATEP) {
			return RESP_RATEP;
		} else if (buffer[4U] == DV3000_CONTROL_READY) {
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
