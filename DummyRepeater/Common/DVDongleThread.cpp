/*
 *   Copyright (C) 2010,2011,2012,2014 by Jonathan Naylor G4KLX
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

#include "DVDongleThread.h"
#include "DStarDefines.h"

CDVDongleThread::CDVDongleThread(CDVDongleController* dongle) :
CDongleThread(),
m_dongle(dongle),
m_wantMode(DM_IDLE),
m_mode(DM_IDLE)
{
	wxASSERT(dongle != NULL);
}

CDVDongleThread::~CDVDongleThread()
{
}

bool CDVDongleThread::open()
{
	return m_dongle->open();
}

void* CDVDongleThread::Entry()
{
	m_dongle->start();

	while (!m_killed) {
		if (m_mode != m_wantMode) {
			if (m_mode == DM_DECODE && m_bleep)
				sendBleep();

			// Reset the FIR filter shift register
			reset();

			m_mode = m_wantMode;
		}

		switch (m_mode) {
			case DM_IDLE:
				processIdle();
				break;

			case DM_DECODE:
				processDecode();
				break;

			case DM_ENCODE:
				processEncode();
				break;
		}
	}

	m_dongle->stop();
	m_dongle->close();
	delete m_dongle;

	return NULL;
}

void CDVDongleThread::setDecode()
{
	if (m_mode == DM_ENCODE && m_decodeCallback == NULL)
		return;

	m_wantMode = DM_DECODE;
}

void CDVDongleThread::setEncode()
{
	if (m_mode == DM_DECODE && m_encodeCallback == NULL)
		return;

	m_wantMode = DM_ENCODE;
}

void CDVDongleThread::setIdle()
{
	m_wantMode = DM_IDLE;
}

void CDVDongleThread::processDecode()
{
	unsigned char ambe[VOICE_FRAME_LENGTH_BYTES];
	::memset(ambe, 0x00, VOICE_FRAME_LENGTH_BYTES);

	m_decodeData.getData(ambe, VOICE_FRAME_LENGTH_BYTES);

	// Convert the AMBE data to real audio, even if it's nonsense
	m_dongle->decodeIn(ambe, VOICE_FRAME_LENGTH_BYTES);

	wxFloat32 audioIn[DSTAR_AUDIO_BLOCK_SIZE];
	bool res = m_dongle->decodeOut(audioIn, DSTAR_AUDIO_BLOCK_SIZE);
	if (!res)
		wxLogError(wxT("An error occurred in decodeOut"));

	wxFloat32 audioOut[DSTAR_RADIO_BLOCK_SIZE];
	upSample(audioIn, audioOut);

	m_decodeCallback->decodeCallback(audioOut, DSTAR_RADIO_BLOCK_SIZE);
}

void CDVDongleThread::processEncode()
{
	wxFloat32 audioIn[DSTAR_RADIO_BLOCK_SIZE];
	::memset(audioIn, 0x00, DSTAR_RADIO_BLOCK_SIZE * sizeof(wxFloat32));

	m_encodeAudio.getData(audioIn, DSTAR_RADIO_BLOCK_SIZE);

	wxFloat32 audioOut[DSTAR_AUDIO_BLOCK_SIZE];
	downSample(audioIn, audioOut);

	// Convert the audio into AMBE data
	m_dongle->encodeIn(audioOut, DSTAR_AUDIO_BLOCK_SIZE);

	unsigned char ambe[VOICE_FRAME_LENGTH_BYTES];
	bool res = m_dongle->encodeOut(ambe, VOICE_FRAME_LENGTH_BYTES);
	if (!res)
		wxLogError(wxT("An error occurred in encodeOut"));

	m_encodeCallback->encodeCallback(ambe, VOICE_FRAME_LENGTH_BYTES);
}

void CDVDongleThread::processIdle()
{
	unsigned char ambe[VOICE_FRAME_LENGTH_BYTES];
	::memset(ambe, 0x00, VOICE_FRAME_LENGTH_BYTES);

	// Convert the AMBE data to real audio, even if it's nonsense
	m_dongle->decodeIn(ambe, VOICE_FRAME_LENGTH_BYTES);

	wxFloat32 audio[DSTAR_RADIO_BLOCK_SIZE];
	bool res = m_dongle->decodeOut(audio, DSTAR_AUDIO_BLOCK_SIZE);
	if (!res)
		wxLogError(wxT("An error occurred in decodeOut"));
}
