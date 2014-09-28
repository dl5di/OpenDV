/*
 *   Copyright (C) 2010,2011,2014 by Jonathan Naylor G4KLX
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

#include "DVDongleController.h"

#include "DStarDefines.h"

const unsigned char DVD_RESP_NOP[] = {0x02, 0x00};
const unsigned int  DVD_RESP_NOP_LEN = 2U;

const unsigned char DVD_REQ_START[] = {0x05, 0x00, 0x18, 0x00, 0x01};
const unsigned int  DVD_REQ_START_LEN = 5U;

const unsigned char DVD_RESP_START[] = {0x05, 0x00, 0x18, 0x00, 0x01};
const unsigned int  DVD_RESP_START_LEN = 5U;

const unsigned char DVD_REQ_STOP[] = {0x05, 0x00, 0x18, 0x00, 0x00};
const unsigned int  DVD_REQ_STOP_LEN = 5U;

const unsigned char DVD_RESP_STOP[] = {0x05, 0x00, 0x18, 0x00, 0x00};
const unsigned int  DVD_RESP_STOP_LEN = 5U;

const unsigned char DVD_REQ_NAME[] = {0x04, 0x20, 0x01, 0x00};
const unsigned int  DVD_REQ_NAME_LEN = 4U;

const unsigned char DVD_RESP_NAME[] = {0x0E, 0x00, 0x01, 0x00, 'D', 'V', ' ', 'D', 'o', 'n', 'g', 'l', 'e', 0x00};
const unsigned int  DVD_RESP_NAME_LEN = 14U;

const unsigned char DVD_AMBE_HEADER[]  = {0x32, 0xA0};
const unsigned char DVD_AUDIO_HEADER[] = {0x42, 0x81};
const unsigned int  DVD_HEADER_LEN = 2U;

const unsigned char DVD_AMBE_DEC_DATA[] = {0xEC, 0x13, 0x00, 0x00, 0x30, 0x10, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x48, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x20, 0x80,
										   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
const unsigned char DVD_AMBE_ENC_DATA[] = {0xEC, 0x13, 0x00, 0x00, 0x30, 0x10, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x48, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x04, 0xF0,
										   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
const unsigned int DVD_AMBE_HEADER_LEN    = 24U;
const unsigned int DVD_AMBE_LENGTH_BYTES  = 48U;
const unsigned int DVD_AUDIO_LENGTH_BYTES = 320U;

const unsigned int DVD_DATA_LEN = DVD_AMBE_LENGTH_BYTES + DVD_HEADER_LEN + DVD_AUDIO_LENGTH_BYTES + DVD_HEADER_LEN;

CDVDongleController::CDVDongleController(const wxString& device) :
m_serial(device, SERIAL_230400)
{
}

CDVDongleController::~CDVDongleController()
{
}

bool CDVDongleController::open()
{
	bool res = m_serial.open();
	if (!res)
		return false;

	m_serial.write(DVD_REQ_NAME, DVD_REQ_NAME_LEN);

	for (;;) {
		unsigned char buffer[DVD_AUDIO_LENGTH_BYTES];
		RESP_TYPE type = getResponse(buffer, DVD_AUDIO_LENGTH_BYTES);
		if (type == RESP_ERROR)
			return false;

		if (type == RESP_NAME)
			return true;

		::wxMilliSleep(5UL);
	}
}

bool CDVDongleController::start()
{
	m_serial.write(DVD_REQ_START, DVD_REQ_START_LEN);

	for (;;) {
		unsigned char buffer[DVD_AUDIO_LENGTH_BYTES];
		RESP_TYPE type = getResponse(buffer, DVD_AUDIO_LENGTH_BYTES);
		if (type == RESP_ERROR)
			return false;

		if (type == RESP_START)
			return true;

		::wxMilliSleep(5UL);
	}
}

bool CDVDongleController::stop()
{
	m_serial.write(DVD_REQ_STOP, DVD_REQ_STOP_LEN);

	for (;;) {
		unsigned char buffer[DVD_AUDIO_LENGTH_BYTES];
		RESP_TYPE type = getResponse(buffer, DVD_AUDIO_LENGTH_BYTES);
		if (type == RESP_ERROR)
			return false;

		if (type == RESP_STOP)
			return true;

		::wxMilliSleep(5UL);
	}
}

void CDVDongleController::encodeIn(const wxFloat32* audio, unsigned int length)
{
	wxASSERT(audio != NULL);
	wxASSERT(length == DSTAR_AUDIO_BLOCK_SIZE);

	unsigned char buffer[DVD_HEADER_LEN + DVD_AUDIO_LENGTH_BYTES];
	unsigned char* p = buffer;

	// First a dummy AMBE packet with parameter information in
	::memcpy(p, DVD_AMBE_HEADER, DVD_HEADER_LEN);
	p += DVD_HEADER_LEN;

	::memcpy(p, DVD_AMBE_ENC_DATA, DVD_AMBE_LENGTH_BYTES);

	m_serial.write(buffer, DVD_HEADER_LEN + DVD_AMBE_LENGTH_BYTES);

	// Then the Audio data to be encoded
	p = buffer;
	::memcpy(p, DVD_AUDIO_HEADER, DVD_HEADER_LEN);
	p += DVD_HEADER_LEN;

	wxInt16* q = (wxInt16*)p;

	for (unsigned int i = 0; i < DSTAR_AUDIO_BLOCK_SIZE; i++) {
		wxInt16 word = wxInt16(audio[i] * 32767.0F);
		*q++ = wxINT16_SWAP_ON_BE(word);
	}

	m_serial.write(buffer, DVD_HEADER_LEN + DVD_AUDIO_LENGTH_BYTES);
}

bool CDVDongleController::encodeOut(unsigned char* ambe, unsigned int length)
{
	wxASSERT(ambe != NULL);
	wxASSERT(length == VOICE_FRAME_LENGTH_BYTES);

	bool ambeFound  = false;
	bool audioFound = false;

	for (;;) {
		unsigned char buffer[DVD_AUDIO_LENGTH_BYTES];
		RESP_TYPE type = getResponse(buffer, DVD_AUDIO_LENGTH_BYTES);
		if (type == RESP_ERROR)
			return false;

		if (type == RESP_AMBE) {
			::memcpy(ambe, buffer + DVD_AMBE_HEADER_LEN, VOICE_FRAME_LENGTH_BYTES);
			ambeFound = true;
		} else if (type == RESP_AUDIO) {
			audioFound = true;
		}

		if (ambeFound && audioFound)
			return true;

		::wxMilliSleep(5UL);
	}
}

void CDVDongleController::decodeIn(const unsigned char* ambe, unsigned int length)
{
	wxASSERT(ambe != NULL);
	wxASSERT(length == VOICE_FRAME_LENGTH_BYTES);

	unsigned char buffer[DVD_HEADER_LEN + DVD_AUDIO_LENGTH_BYTES];
	unsigned char* p = buffer;

	// First the received AMBE data for decoding
	::memcpy(p, DVD_AMBE_HEADER, DVD_HEADER_LEN);
	p += DVD_HEADER_LEN;

	::memcpy(p, DVD_AMBE_DEC_DATA, DVD_AMBE_LENGTH_BYTES);
	
	::memcpy(p + DVD_AMBE_HEADER_LEN, ambe, VOICE_FRAME_LENGTH_BYTES);

	m_serial.write(buffer, DVD_HEADER_LEN + DVD_AMBE_LENGTH_BYTES);

	// Then a dummy Audio packet
	p = buffer;
	::memcpy(p, DVD_AUDIO_HEADER, DVD_HEADER_LEN);
	p += DVD_HEADER_LEN;

	::memset(p, 0x00, DVD_AUDIO_LENGTH_BYTES);

	m_serial.write(buffer, DVD_HEADER_LEN + DVD_AUDIO_LENGTH_BYTES);
}

bool CDVDongleController::decodeOut(wxFloat32* audio, unsigned int length)
{
	wxASSERT(audio != NULL);
	wxASSERT(length == DSTAR_AUDIO_BLOCK_SIZE);

	bool ambeFound  = false;
	bool audioFound = false;

	for (;;) {
		unsigned char buffer[DVD_AUDIO_LENGTH_BYTES];
		RESP_TYPE type = getResponse(buffer, DVD_AUDIO_LENGTH_BYTES);
		if (type == RESP_ERROR)
			return false;

		if (type == RESP_AMBE) {
			ambeFound = true;
		} else if (type == RESP_AUDIO) {
			wxInt16* q = (wxInt16*)buffer;
			for (unsigned int i = 0U; i < DSTAR_AUDIO_BLOCK_SIZE; i++) {
				wxInt16 word = wxINT16_SWAP_ON_BE(*q++);
				audio[i] = wxFloat32(word) / 32768.0F;
			}
			audioFound = true;
		}

		if (ambeFound && audioFound)
			return true;

		::wxMilliSleep(5UL);
	}
}

void CDVDongleController::close()
{
	m_serial.close();
}

CDVDongleController::RESP_TYPE CDVDongleController::getResponse(unsigned char* buffer, unsigned int length)
{
	wxASSERT(buffer != NULL);
	wxASSERT(length >= DVD_AUDIO_LENGTH_BYTES);

	unsigned char c1, c2;
	int len = m_serial.read(&c1, 1U);
	if (len == 0)
		return RESP_NONE;

	if (len < 0) {
		wxLogError(wxT("Unable to receive the DVD header: len=%d"), len);
		return RESP_ERROR;
	}

	switch (c1) {
		case 0xA0U:		// DVD_AMBE_HEADER
			wxLogMessage(wxT("Re-synchronising with the DV-Dongle"));
			buffer[0U] = 0x32U;
			buffer[1U] = c1;
			return processResponse(buffer, length);

		case 0x81U:		// DVD_AUDIO_HEADER
			wxLogMessage(wxT("Re-synchronising with the DV-Dongle"));
			buffer[0U] = 0x42U;
			buffer[1U] = c1;
			return processResponse(buffer, length);

		default:
			m_serial.read(&c2, 1U);
			buffer[0U] = c1;
			buffer[1U] = c2;
			return processResponse(buffer, length);
	}
}

CDVDongleController::RESP_TYPE CDVDongleController::processResponse(unsigned char* buffer, unsigned int length)
{
	wxASSERT(buffer != NULL);
	wxASSERT(length >= DVD_AUDIO_LENGTH_BYTES);

	int len;

	if (::memcmp(buffer, DVD_AUDIO_HEADER, DVD_HEADER_LEN) == 0) {			// Audio data
		do {
			len = m_serial.read(buffer, DVD_AUDIO_LENGTH_BYTES);
			if (len == 0)
				::wxMilliSleep(5UL);
		} while (len == 0);

		if (len != int(DVD_AUDIO_LENGTH_BYTES)) {
			wxLogError(wxT("Unable to receive the DVD audio data, len=%d"), len);
			return RESP_ERROR;
		}

		return RESP_AUDIO;
	} else if (::memcmp(buffer, DVD_AMBE_HEADER, DVD_HEADER_LEN) == 0) {	// AMBE data
		do {
			len = m_serial.read(buffer, DVD_AMBE_LENGTH_BYTES);
			if (len == 0)
				::wxMilliSleep(5UL);
		} while (len == 0);

		if (len != int(DVD_AMBE_LENGTH_BYTES)) {
			wxLogError(wxT("Unable to receive the DVD AMBE data, len=%d"), len);
			return RESP_ERROR;
		}

		return RESP_AMBE;
	} else if (::memcmp(buffer, DVD_RESP_STOP, DVD_HEADER_LEN) == 0) {		// Start or Stop response
		do {
			len = m_serial.read(buffer + DVD_HEADER_LEN, DVD_RESP_STOP_LEN - DVD_HEADER_LEN);
			if (len == 0)
				::wxMilliSleep(5UL);
		} while (len == 0);

		if (len != int(DVD_RESP_STOP_LEN - DVD_HEADER_LEN)) {
			wxLogError(wxT("Unable to receive the DVD start/stop response, len=%d"), len);
			return RESP_ERROR;
		}

		bool res = ::memcmp(buffer, DVD_RESP_START, DVD_RESP_START_LEN) == 0;
		if (res)
			return RESP_START;

		res = ::memcmp(buffer, DVD_RESP_STOP, DVD_RESP_STOP_LEN) == 0;
		if (res)
			return RESP_STOP;

		wxLogError(wxT("Incorrect response to start/stop request: %02X %02X %02X %02X %02X"), buffer[0U], buffer[1U], buffer[2U], buffer[3U], buffer[4U]);
		return RESP_ERROR;
	} else if (::memcmp(buffer, DVD_RESP_NAME, DVD_HEADER_LEN) == 0) {		// Dongle name
		do {
			len = m_serial.read(buffer + DVD_HEADER_LEN, DVD_RESP_NAME_LEN - DVD_HEADER_LEN);
			if (len == 0)
				::wxMilliSleep(5UL);
		} while (len == 0);

		if (len != int(DVD_RESP_NAME_LEN - DVD_HEADER_LEN)) {
			wxLogError(wxT("Unable to receive the DVD name data, len=%d"), len);
			return RESP_ERROR;
		}

		bool res = ::memcmp(buffer, DVD_RESP_NAME, DVD_RESP_NAME_LEN) == 0;
		if (res)
			return RESP_NAME;

		wxLogError(wxT("Incorrect response to name request: %02X %02X %02X %02X %02X..."), buffer[0U], buffer[1U], buffer[2U], buffer[3U], buffer[4U]);
		return RESP_ERROR;
	} else if (::memcmp(buffer, DVD_RESP_NOP, DVD_HEADER_LEN) == 0) {		// No op
		return RESP_NOP;
	} else {																// Unknown data
		wxLogError(wxT("Unknown DV Dongle header: %02X %02X"), buffer[0U], buffer[1U]);
		return RESP_UNKNOWN;
	}
}
