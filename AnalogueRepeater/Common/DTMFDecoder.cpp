/*
 *   Copyright (C) 2009,2011 by Jonathan Naylor G4KLX
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

#include "DTMFDecoder.h"

const wxFloat32 DTMF_FREQS[] = {697.0F, 770.0F, 852.0F, 941.0F, 1209.0F, 1336.0F, 1477.0F, 1633.0F};

CDTMFDecoder::CDTMFDecoder(unsigned int sampleRate, wxFloat32 threshold) :
m_decoders(NULL)
{
	wxASSERT(sampleRate > 0U);
	wxASSERT(threshold >= 0.0F);

	m_decoders = new CGoertzel*[8U];

	for (unsigned int i = 0U; i < 8U; i++)
		m_decoders[i] = new CGoertzel(sampleRate, DTMF_FREQS[i], sampleRate / 40U, threshold);
}

CDTMFDecoder::~CDTMFDecoder()
{
	for (unsigned int i = 0U; i < 8U; i++)
		delete m_decoders[i];

	delete[] m_decoders;
}

void CDTMFDecoder::setThreshold(wxFloat32 threshold)
{
	wxASSERT(threshold >= 0.0F);

	for (unsigned int i = 0U; i < 8U; i++)
		m_decoders[i]->setThreshold(threshold);
}

void CDTMFDecoder::process(const wxFloat32* audio, unsigned int length)
{
	wxASSERT(audio != NULL);

	for (unsigned int i = 0U; i < 8U; i++)
		m_decoders[i]->process(audio, length);
}

wxChar CDTMFDecoder::getCharacter() const
{
	TRISTATE res[8U];
	for (unsigned int i = 0U; i < 8U; i++)
		res[i] = m_decoders[i]->getDetected();

	// No answer from the Goertzel algorithm so send a "don't know" out.
	if (res[0] == STATE_UNKNOWN)
		return wxT('-');

	if (res[0] == STATE_TRUE && res[4] == STATE_TRUE)
		return wxT('1');
	else if (res[0] == STATE_TRUE && res[5] == STATE_TRUE)
		return wxT('2');
	else if (res[0] == STATE_TRUE && res[6] == STATE_TRUE)
		return wxT('3');
	else if (res[1] == STATE_TRUE && res[4] == STATE_TRUE)
		return wxT('4');
	else if (res[1] == STATE_TRUE && res[5] == STATE_TRUE)
		return wxT('5');
	else if (res[1] == STATE_TRUE && res[6] == STATE_TRUE)
		return wxT('6');
	else if (res[1] == STATE_TRUE && res[7] == STATE_TRUE)
		return wxT('B');
	else if (res[2] == STATE_TRUE && res[4] == STATE_TRUE)
		return wxT('7');
	else if (res[2] == STATE_TRUE && res[5] == STATE_TRUE)
		return wxT('8');
	else if (res[2] == STATE_TRUE && res[6] == STATE_TRUE)
		return wxT('9');
	else if (res[2] == STATE_TRUE && res[7] == STATE_TRUE)
		return wxT('C');
	else if (res[3] == STATE_TRUE && res[4] == STATE_TRUE)
		return wxT('*');
	else if (res[3] == STATE_TRUE && res[5] == STATE_TRUE)
		return wxT('0');
	else if (res[3] == STATE_TRUE && res[6] == STATE_TRUE)
		return wxT('#');
	else if (res[3] == STATE_TRUE && res[7] == STATE_TRUE)
		return wxT('D');
	else
		return wxT(' ');
}
