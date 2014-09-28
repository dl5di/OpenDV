/*
 *	Audio FSK modem for AX25 (1200 Baud, 1200/2200Hz).
 * 
 *	Copyright (C) Sivan Toledo, 2012
 *	Copyright (C) Jonathan Naylor, 2013
 * 
 *      This program is free software; you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation; either version 2 of the License, or
 *      (at your option) any later version.
 *
 *      This program is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 *
 *      You should have received a copy of the GNU General Public License
 *      along with this program; if not, write to the Free Software
 *      Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "AnalogueDefines.h"
#include "AX25TX.h"

const wxFloat32     PHASE_INC_F0 = 2.0F * M_PI * 1200.0F / wxFloat32(ANALOGUE_RADIO_SAMPLE_RATE);
const wxFloat32     PHASE_INC_F1 = 2.0F * M_PI * 2200.0F / wxFloat32(ANALOGUE_RADIO_SAMPLE_RATE);
const wxFloat32 PHASE_INC_SYMBOL = 2.0F * M_PI * 1200.0F / wxFloat32(ANALOGUE_RADIO_SAMPLE_RATE);

CAX25TX::CAX25TX() :
m_symbolPhase(0.0F),
m_ddsPhase(0.0F),
m_lastSymbol(false),
m_stuffCount(0U)
{
}

CAX25TX::~CAX25TX()
{
}

unsigned int CAX25TX::generateSymbolSamples(bool symbol, wxFloat32* audio, unsigned int position)
{
	wxASSERT(audio != NULL);

	unsigned int count = 0U;

	while (m_symbolPhase < wxFloat32(2.0 * M_PI)) {
		audio[position] = ::sin(m_ddsPhase);

		if (!symbol) m_ddsPhase += PHASE_INC_F0;
		else         m_ddsPhase += PHASE_INC_F1;

		m_symbolPhase += PHASE_INC_SYMBOL;

		if (m_ddsPhase > wxFloat32(2.0 * M_PI)) m_ddsPhase -= wxFloat32(2.0 * M_PI);

		position++;
		count++;
	}

	m_symbolPhase -= wxFloat32(2.0 * M_PI);

	return count;		
}

unsigned int CAX25TX::byteToSymbols(unsigned char bits, bool stuff, wxFloat32* audio)
{
	wxASSERT(audio != NULL);

	unsigned int position = 0U;

	for (unsigned int i = 0U; i < 8U; i++) {
		bool bit = (bits & 0x01U) == 0x01U;
		bits >>= 1;

		if (!bit) { // we switch symbols (frequencies)
	  		bool symbol = !m_lastSymbol;
	  		position += generateSymbolSamples(symbol, audio, position);

	  		if (stuff) m_stuffCount = 0U;
	  		m_lastSymbol = symbol;
	  	} else {
	  		bool symbol = m_lastSymbol;
	  		position += generateSymbolSamples(symbol, audio, position);

	  		if (stuff) m_stuffCount++;
	  		m_lastSymbol = symbol;
	  		
	  		if (stuff && m_stuffCount == 5U) {
	  			// send a zero
				symbol = !m_lastSymbol;
		  		position += generateSymbolSamples(symbol, audio, position);

		  		m_stuffCount = 0U;
		  		m_lastSymbol = symbol;
			}
		}
	}

	return position;
}

unsigned int CAX25TX::getAudio(wxFloat32* audio, unsigned char* packet, unsigned int length)
{
	wxASSERT(audio != NULL);
	wxASSERT(packet != NULL);
	wxASSERT(length > 0U);

	m_symbolPhase = 0.0F;
	m_ddsPhase    = 0.0F;
	m_lastSymbol  = false;
	m_stuffCount  = 0U;

	unsigned int count;
	count  = byteToSymbols(0x7EU, false, audio);
	count += byteToSymbols(0x7EU, false, audio + count);

	for (unsigned int i = 0U; i < length; i++)
		count += byteToSymbols(packet[i], true, audio + count);

	count += byteToSymbols(0x7EU, false, audio + count);
	count += byteToSymbols(0x7EU, false, audio + count);

	return count;
}
