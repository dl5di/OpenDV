/*
 *   Copyright (C) 1996 by Thomas Sailer HB9JNX
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
#include "APRSRX.h"
#include "Utils.h"

const unsigned int FREQ_MARK  = 1200U;
const unsigned int FREQ_SPACE = 2200U;
const unsigned int BAUD       = 1200U;
const unsigned int SUBSAMP    = 4U;			// 2U

const unsigned int CORRLEN    = ANALOGUE_RADIO_SAMPLE_RATE / BAUD;
const unsigned int PHASEINC   = 0x10000U * BAUD * SUBSAMP / ANALOGUE_RADIO_SAMPLE_RATE;

const unsigned int MAX_PACKET_LEN = 512U;

inline wxFloat32 mac(const wxFloat32* a, const wxFloat32* b, unsigned int size)
{
	wxFloat32 sum = 0.0F;

	for (unsigned int i = 0U; i < size; i++)
		sum += (*a++) * (*b++);

	return sum;
}

inline wxFloat32 fsqr(wxFloat32 f) 
{
	return f * f;
}

CAPRSRX::CAPRSRX() :
m_dcd(0U),
m_phase(0U),
m_last(0U),
m_subsamp(0U),
m_buf(NULL),
m_ptr(NULL),
m_state(false),
m_bitStream(0x00U),
m_bitBuf(0x00U),
m_dump(false),
m_dumper()
{
	wxFloat32 f;
	unsigned int i;

	m_buf = new unsigned char[MAX_PACKET_LEN];

	m_corrMarkI  = new wxFloat32[CORRLEN];
	m_corrMarkQ  = new wxFloat32[CORRLEN];
	m_corrSpaceI = new wxFloat32[CORRLEN];
	m_corrSpaceQ = new wxFloat32[CORRLEN];

	for (f = 0.0F, i = 0U; i < CORRLEN; i++) {
		m_corrMarkI[i] = ::cos(f);
		m_corrMarkQ[i] = ::sin(f);
		f += wxFloat32(2.0 * M_PI * FREQ_MARK / ANALOGUE_RADIO_SAMPLE_RATE);
	}

	for (f = 0.0F, i = 0U; i < CORRLEN; i++) {
		m_corrSpaceI[i] = ::cos(f);
		m_corrSpaceQ[i] = ::sin(f);
		f += wxFloat32(2.0 * M_PI * FREQ_SPACE / ANALOGUE_RADIO_SAMPLE_RATE);
	}

	return;

	wxString text1 = wxT("wxFloat32 CORR_MARK_I[] = {");
	for (i = 0U; i < CORRLEN; i++) {
		wxString text2;
		text2.Printf(wxT("%.5f, "), m_corrMarkI[i]);
		text1.Append(text2);
	}
	text1.Append(wxT("};"));
	wxLogMessage(text1);

	text1 = wxT("wxFloat32 CORR_MARK_Q[] = {");
	for (i = 0U; i < CORRLEN; i++) {
		wxString text2;
		text2.Printf(wxT("%.5f, "), m_corrMarkQ[i]);
		text1.Append(text2);
	}
	text1.Append(wxT("};"));
	wxLogMessage(text1);

	text1 = wxT("wxFloat32 CORR_SPACE_I[] = {");
	for (i = 0U; i < CORRLEN; i++) {
		wxString text2;
		text2.Printf(wxT("%.5f, "), m_corrSpaceI[i]);
		text1.Append(text2);
	}
	text1.Append(wxT("};"));
	wxLogMessage(text1);

	text1 = wxT("wxFloat32 CORR_SPACE_Q[] = {");
	for (i = 0U; i < CORRLEN; i++) {
		wxString text2;
		text2.Printf(wxT("%.5f, "), m_corrSpaceQ[i]);
		text1.Append(text2);
	}
	text1.Append(wxT("};"));
	wxLogMessage(text1);
}

CAPRSRX::~CAPRSRX()
{
	wxLogMessage(wxT("RX Dumper: %s"), m_dumper.c_str());
	delete[] m_buf;
	delete[] m_corrMarkI;
	delete[] m_corrMarkQ;
	delete[] m_corrSpaceI;
	delete[] m_corrSpaceQ;
}

void CAPRSRX::process(const wxFloat32* audio, unsigned int length)
{
	if (m_subsamp > 0U) {
		unsigned int numfill = SUBSAMP - m_subsamp;
		if (length < numfill) {
			m_subsamp += length;
			return;
		}

		audio  += numfill;
		length -= numfill;

		m_subsamp = 0U;
	}

	for (; length >= SUBSAMP; length -= SUBSAMP, audio += SUBSAMP) {
		wxFloat32 f = ::fsqr(::mac(audio, m_corrMarkI, CORRLEN)) + ::fsqr(::mac(audio, m_corrMarkQ, CORRLEN)) - ::fsqr(::mac(audio, m_corrSpaceI, CORRLEN)) - ::fsqr(::mac(audio, m_corrSpaceQ, CORRLEN));

		m_dcd <<= 1;
		m_dcd |= (f > 0.0F);

		/*
		 * Check if transition
		 */
		if ((m_dcd ^ (m_dcd >> 1)) & 1) {
			if (m_phase < (0x8000U - (PHASEINC / 2U)))
				m_phase += PHASEINC / 8U;
			else
				m_phase -= PHASEINC / 8U;
		}

		m_phase += PHASEINC;

		if (m_phase >= 0x10000U) {
			m_phase &= 0xFFFFU;

			m_last <<= 1;
			m_last |= m_dcd & 1;

			bool curbit = (m_last ^ (m_last >> 1) ^ 1) & 1;
			rxbit(curbit);
		}
	}

	m_subsamp = length;
}

void CAPRSRX::rxbit(bool bit)
{
	m_bitStream <<= 1;
	m_bitStream |= bit ? 0x01U : 0x00U;

	if ((m_bitStream & 0xFFU) == 0x7EU) {
		if (m_state && (m_ptr - m_buf) > 0U) {
			CUtils::dump(wxT("AX.25 RX Packet"), m_buf, m_ptr - m_buf);

			if ((m_ptr - m_buf) >= 25U)
				decodeMicE(m_buf, m_ptr - m_buf);
		}

		m_state  = true;
		m_dump   = true;
		m_ptr    = m_buf;
		m_bitBuf = 0x80U;
		return;
	}

	if ((m_bitStream & 0x7FU) == 0x7FU) {
		m_state = false;
		return;
	}

	if (!m_state)
		return;

	if ((m_bitStream & 0x3FU) == 0x3EU)		/* Stuffed bit */
		return;

	if ((m_bitStream & 0x01U) == 0x01U)
		m_bitBuf |= 0x100U;

	if ((m_bitBuf & 0x01U) == 0x01U) {
		if (m_ptr >= (m_buf + MAX_PACKET_LEN)) {
			m_state = false;
			return;
		}

		unsigned char byte = m_bitBuf >> 1;
		*m_ptr++ = byte;
		wxString t;
		t.Printf(wxT("%u%u%u%u%u%u%u%u "), byte & 0x01U ? 1U : 0U, byte & 0x02U ? 1U : 0U, byte & 0x04U ? 1U : 0U, byte & 0x08U ? 1U : 0U, byte & 0x10U ? 1U : 0U, byte & 0x20U ? 1U : 0U, byte & 0x40U ? 1U : 0U, byte & 0x80U ? 1U : 0U);
		m_dumper.Append(t);
		// *m_ptr++ = m_bitBuf >> 1;
		m_bitBuf = 0x80U;
		return;
	}

	m_bitBuf >>= 1;
}

void CAPRSRX::decodeMicE(const unsigned char* packet, unsigned int length)
{
	CAX25Checksum csum;
	bool ok = csum.check(packet, length);
	if (!ok)
		return;

	const unsigned char* ptr = packet + 13U;

	// There are digipeters, find their end
	for (unsigned int n = 0U; (*ptr & 0x01U) == 0x00U && n < 7U; n++)
		ptr += 7U;

	ptr++;		// Point to the type

	// Check type and PID
	if (ptr[0U] != 0x03U || ptr[1U] != 0xF0U || ptr[2U] != 0x60U)
		return;

	ptr += 3U;	// Point to the rest of the info field

	wxLogMessage(wxT("Source callsign: %c%c%c%c%c%c-%u"), packet[7U] >> 1, packet[8U] >> 1, packet[9U] >> 1,
		packet[10U] >> 1, packet[11U] >> 1, packet[12U] >> 1, (packet[13U] & 0x1EU) >> 1);

	if ((packet[6U] & 0x1EU) == 0x02U)
		wxLogMessage(wxT("Destination callsign: WIDE-1"));

	bool north = (packet[3U] & 0x80U) == 0x80U;
	bool west  = (packet[5U] & 0x80U) == 0x80U;
	bool cent  = (packet[4U] & 0x80U) == 0x80U;

	unsigned char latText[6U];
	for (unsigned int i = 0U; i < 6U; i++) {
		unsigned char c = packet[i] >> 1;
		if (c >= 'A' && c <= 'J')
			latText[i] = c - '0' - 17U;
		else if (c >= 'P' && c <= 'Y')
			latText[i] = c - '0' - 32U;
		else if (c == 'K' || c == 'L' || c == 'Z')
			return;
		else
			latText[i] = c - '0';
	}

	wxFloat32 latitude = wxFloat32(latText[0U] * 10U + latText[1U]);
	wxFloat32 latMin   = wxFloat32(latText[2U] * 1000U + latText[3U] * 100U + latText[4U] * 10U + latText[5U]) / 100.0F;
	latitude += latMin / 60.0F;
	if (!north)
		latitude = -latitude;

	wxFloat32 longitude = wxFloat32(ptr[0U] - 28U);
	wxFloat32 lonMin    = wxFloat32(ptr[1U] - 28U);
	if (cent)
		longitude += 100.0F;
	if (longitude >= 180.0F && longitude <= 189.0F)
		longitude -= 80.0F;
	else if (longitude >= 190.0F && longitude <= 199.0F)
		longitude -= 190.0F;
	if (lonMin >= 60.0F)
		lonMin -= 60.0F;
	lonMin += wxFloat32(ptr[2U] - 28U) / 100.0F;
	longitude += lonMin / 60.0F;
	if (west)
		longitude = -longitude;

	wxLogMessage(wxT("Latitude: %f deg, Longitude: %f deg"), latitude, longitude);

	unsigned int speed  = (ptr[3U] - 28U) * 10U;
	unsigned int course = ptr[5U] - 28U;
	unsigned int cs     = ptr[4U] - 28U;
	unsigned int csTemp = cs / 10U;
	speed += csTemp;
	if (speed > 800U)
		speed -= 800U;
	course += 100U * (cs - csTemp * 10U);
	if (course > 400U)
		course -= 400U;

	wxLogMessage(wxT("Speed: %u mph, course: %u deg"), speed, course);

	wxLogMessage(wxT("APRS symbols: %c %c"), ptr[6U], ptr[7U]);

	ptr += 9U;

	if (ptr[3U] == '}') {
		int altitude = (ptr[0U] - 33) * 8281 + (ptr[1U] - 33) * 91 + (ptr[2U] - 33) - 10000;
		wxLogMessage(wxT("Altitude: %d m"), altitude);
		ptr += 4U;
	}

	unsigned int len = length - (ptr - packet);
	if (len > 0U) {
		wxString text((char*)ptr, wxConvLocal, len);
		wxLogMessage(wxT("Text: %s"), text.c_str());
	}
}
