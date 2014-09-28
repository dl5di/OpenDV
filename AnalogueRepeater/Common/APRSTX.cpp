/*
 *   Copyright (C) 2013 by Jonathan Naylor G4KLX
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

#include "AnalogueDefines.h"
#include "AX25Checksum.h"
#include "AX25TX.h"
#include "APRSTX.h"
#include "Utils.h"

CAPRSTX::CAPRSTX(const wxString& callsign, wxFloat32 latitude, wxFloat32 longitude, int altitude, const wxString& text) :
m_audio(NULL),
m_length(0U),
m_offset(0U)
{
	m_audio = new wxFloat32[ANALOGUE_RADIO_SAMPLE_RATE * 2U];

	unsigned char packet[300U];

	::memset(packet, ' ' << 1, 14U);

	bool north = true;
	if (latitude < 0.0F) {
		latitude = -latitude;
		north = false;
	}

	bool east = true;
	if (longitude < 0.0F) {
		longitude = -longitude;
		east = false;
	}

	unsigned int latDeg = (unsigned int)latitude;
	unsigned int latMin = (unsigned int)((latitude - wxFloat32(latDeg)) * 60.0F);
	unsigned int latHun = (unsigned int)((latitude - wxFloat32(latDeg) - wxFloat32(latMin) / 60.0F) * 6000.0F);

	unsigned int lonDeg = (unsigned int)longitude;
	unsigned int lonMin = (unsigned int)((longitude - wxFloat32(lonDeg)) * 60.0F);
	unsigned int lonHun = (unsigned int)((longitude - wxFloat32(lonDeg) - wxFloat32(lonMin) / 60.0F) * 6000.0F);

	// The destination callsign
	packet[0U] = 0x50U;
	packet[0U] |= latDeg / 10U;
	packet[0U] <<= 1;

	packet[1U] = 0x30U;
	packet[1U] |= latDeg % 10U;
	packet[1U] <<= 1;

	packet[2U] = 0x50U;
	packet[2U] |= latMin / 10U;
	packet[2U] <<= 1;

	packet[3U] = north ? 0x50U : 0x30U;
	packet[3U] |= latMin % 10U;
	packet[3U] <<= 1;

	packet[4U] = lonDeg >= 100U || lonDeg < 10U ? 0x50U : 0x30U;
	packet[4U] |= latHun / 10U;
	packet[4U] <<= 1;

	packet[5U] = east ? 0x30U : 0x50U;
	packet[5U] |= latHun % 10U;
	packet[5U] <<= 1;

	packet[6U] = 0x02U;			// SSID = 1 for WIDE-1	

	// The source callsign
	for (unsigned int i = 0U; i < callsign.Len() && i < 6U; i++)
		packet[7U + i] = callsign.GetChar(i) << 1;

	packet[13U] = 0x01U;		// SSID = 0, plus last byte of address

	packet[14U] = 0x03U;		// UI
	packet[15U] = 0xF0U;		// No layer 3

	packet[16U] = 0x60U;		// GPS data is valid

	if (lonDeg < 10U)
		packet[17U] = lonDeg + 90U + 28U;
	else if (lonDeg < 100U)
		packet[17U] = lonDeg + 28U;
	else if (lonDeg < 110U)
		packet[17U] = lonDeg + 80U + 28U;
	else
		packet[17U] = lonDeg - 100U + 28U;

	packet[18U] = lonMin > 10U ? lonMin + 28U : lonMin + 60U + 28U;
	packet[19U] = lonHun + 28U;

	packet[20U] = 28U;			// Zero for speed and direction
	packet[21U] = 28U;
	packet[22U] = 28U;

	packet[23U] = 'm';
	packet[24U] = '/';

	packet[25U] = '\'';

	unsigned int len = 26U;

	altitude += 10000;
	packet[len++] = altitude / 8281 + 33U;
	altitude %= 8281U;
	packet[len++] = altitude / 91 + 33U;
	packet[len++] = altitude % 91 + 33U;
	packet[len++] = '}';

	if (text.Len() > 0U) {
		packet[len++] = ' ';

		for (unsigned int i = 0U; i < text.Len(); i++)
			packet[len++] = text.GetChar(i);
	}

	CAX25Checksum csum;
	len = csum.calculate(packet, len);

	CUtils::dump(wxT("AX.25 TX Packet"), packet, len);

	insertSilence(100U);

	CAX25TX tx;
	m_length += tx.getAudio(m_audio + m_length, packet, len);

	insertSilence(100U);
}

CAPRSTX::~CAPRSTX()
{
	delete[] m_audio;
}

unsigned int CAPRSTX::getAudio(wxFloat32* audio, unsigned int length, wxFloat32 amplitude)
{
	wxASSERT(audio != NULL);
	wxASSERT(m_audio != NULL);

	unsigned int len = 0U;

	while (m_offset < m_length && len < length) {
		audio[len] = m_audio[m_offset] * amplitude;
		len++;
		m_offset++;
	}

	return len;
}

bool CAPRSTX::isEmpty() const
{
	return m_length == 0U;
}

void CAPRSTX::reset()
{
	m_offset = 0U;
}

void CAPRSTX::insertSilence(unsigned int ms)
{
	unsigned int length = (ms * ANALOGUE_RADIO_SAMPLE_RATE) / 1000U;

	for (unsigned int i = 0U; i < length; i++) {
		m_audio[m_length] = 0x00U;
		m_length++;
	}
}
