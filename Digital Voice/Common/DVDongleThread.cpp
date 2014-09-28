/*
 *   Copyright (C) 2010,2011,2012 by Jonathan Naylor G4KLX
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
#include "Bleeper.h"

// A 3 kHz low pass filter at a sample rate of 48000 samples/sec. Calculated by the fir1 command in
// MATLAB.
const wxFloat32 FIR_TAPS[] = {
	 0.001215122399171F,  0.001363519692447F,  0.001447384226195F,  0.001287712318712F,  0.000614680825630F,
	-0.000833383976996F, -0.003176495118124F, -0.006261958697776F, -0.009578203328048F, -0.012249770207721F,
	-0.013132705407240F, -0.011005254575316F, -0.004822958879920F,  0.006013373011413F,  0.021438048217733F,
	 0.040605942188804F,  0.061921094815680F,  0.083217403172960F,  0.102067175368858F,  0.116163746412471F,
	 0.123705527541065F,  0.123705527541065F,  0.116163746412471F,  0.102067175368858F,  0.083217403172960F,
	 0.061921094815680F,  0.040605942188804F,  0.021438048217733F,  0.006013373011413F, -0.004822958879920F,
	-0.011005254575316F, -0.013132705407240F, -0.012249770207721F, -0.009578203328048F, -0.006261958697776F,
	-0.003176495118124F, -0.000833383976996F,  0.000614680825630F,  0.001287712318712F,  0.001447384226195F,
	 0.001363519692447F,  0.001215122399171F};

const unsigned int FIR_LEN = 42U;

const wxFloat32 UPSAMPLE_AMP   = 6.0F;
const wxFloat32 DOWNSAMPLE_AMP = 0.9F;

CDVDongleThread::CDVDongleThread(CDVDongleController* dongle, CRingBuffer<wxFloat32>* decodeAudio, CRingBuffer<unsigned char>* decodeData, CRingBuffer<wxFloat32>* encodeAudio, CRingBuffer<unsigned char>* encodeData) :
wxThread(wxTHREAD_JOINABLE),
m_dongle(dongle),
m_decodeAudio(decodeAudio),
m_decodeData(decodeData),
m_encodeAudio(encodeAudio),
m_encodeData(encodeData),
m_wantMode(DVDMODE_IDLE),
m_mode(DVDMODE_IDLE),
m_killed(false),
m_bleep(true),
m_filter(FIR_TAPS, FIR_LEN)
{
	wxASSERT(dongle != NULL);
	wxASSERT(decodeAudio != NULL);
	wxASSERT(decodeData != NULL);
}

CDVDongleThread::~CDVDongleThread()
{
}

void* CDVDongleThread::Entry()
{
	m_dongle->start();

	while (!m_killed) {
		if (m_mode != m_wantMode) {
			if (m_mode == DVDMODE_DECODE && m_bleep)
				sendBleep();

			// Reset the FIR filter shift register
			m_filter.reset();

			m_mode = m_wantMode;
		}

		switch (m_mode) {
			case DVDMODE_IDLE:
				processIdle();
				break;

			case DVDMODE_DECODE:
				processDecode();
				break;

			case DVDMODE_ENCODE:
				processEncode();
				break;
		}
	}

	m_dongle->stop();
	m_dongle->close();
	delete m_dongle;

	return NULL;
}

void CDVDongleThread::kill()
{
	m_killed = true;
}

void CDVDongleThread::setMode(DVDMODE mode)
{
	if (mode == DVDMODE_ENCODE && (m_encodeData == NULL || m_encodeAudio == NULL))
		return;

	m_wantMode = mode;
}

void CDVDongleThread::setBleep(bool on)
{
	m_bleep = on;
}

void CDVDongleThread::processDecode()
{
	unsigned char ambe[VOICE_FRAME_LENGTH_BYTES];
	::memset(ambe, 0x00, VOICE_FRAME_LENGTH_BYTES);

	m_decodeData->getData(ambe, VOICE_FRAME_LENGTH_BYTES);

	// Convert the AMBE data to real audio, even if it's nonsense
	m_dongle->decodeIn(ambe, VOICE_FRAME_LENGTH_BYTES);

	wxFloat32 audioIn[DSTAR_AUDIO_BLOCK_SIZE];
	unsigned int ber;
	bool res = m_dongle->decodeOut(audioIn, DSTAR_AUDIO_BLOCK_SIZE, ber);
	if (!res)
		wxLogError(wxT("An error occurred in decodeOut"));

	wxFloat32 audioOut[DSTAR_RADIO_BLOCK_SIZE];
	upSample(audioIn, audioOut);

	m_decodeAudio->addData(audioOut, DSTAR_RADIO_BLOCK_SIZE);
}

void CDVDongleThread::processEncode()
{
	wxASSERT(m_encodeAudio != NULL);
	wxASSERT(m_encodeData != NULL);

	wxFloat32 audioIn[DSTAR_RADIO_BLOCK_SIZE];
	::memset(audioIn, 0x00, DSTAR_RADIO_BLOCK_SIZE * sizeof(wxFloat32));

	m_encodeAudio->getData(audioIn, DSTAR_RADIO_BLOCK_SIZE);

	wxFloat32 audioOut[DSTAR_AUDIO_BLOCK_SIZE];
	downSample(audioIn, audioOut);

	// Convert the audio into AMBE data
	m_dongle->encodeIn(audioOut, DSTAR_AUDIO_BLOCK_SIZE);

	unsigned char ambe[VOICE_FRAME_LENGTH_BYTES];
	bool res = m_dongle->encodeOut(ambe, VOICE_FRAME_LENGTH_BYTES);
	if (!res)
		wxLogError(wxT("An error occurred in encodeOut"));

	m_encodeData->addData(ambe, VOICE_FRAME_LENGTH_BYTES);
}

void CDVDongleThread::processIdle()
{
	unsigned char ambe[VOICE_FRAME_LENGTH_BYTES];
	::memset(ambe, 0x00, VOICE_FRAME_LENGTH_BYTES);

	// Convert the AMBE data to real audio, even if it's nonsense
	m_dongle->decodeIn(ambe, VOICE_FRAME_LENGTH_BYTES);

	wxFloat32 audio[DSTAR_RADIO_BLOCK_SIZE];
	unsigned int ber;
	bool res = m_dongle->decodeOut(audio, DSTAR_AUDIO_BLOCK_SIZE, ber);
	if (!res)
		wxLogError(wxT("An error occurred in decodeOut"));

	// Ensure silence
	::memset(audio, 0x00, DSTAR_RADIO_BLOCK_SIZE * sizeof(wxFloat32));

	m_decodeAudio->addData(audio, DSTAR_RADIO_BLOCK_SIZE);
}

void CDVDongleThread::sendBleep()
{
	wxFloat32 audio[DSTAR_RADIO_BLOCK_SIZE];
	unsigned int n;

	CBleeper bleep(DSTAR_RADIO_SAMPLE_RATE, DSTAR_BLEEP_FREQ, DSTAR_BLEEP_LENGTH, DSTAR_BLEEP_AMPL);

	while ((n = bleep.getAudio(audio, DSTAR_RADIO_BLOCK_SIZE)) > 0U)
		m_decodeAudio->addData(audio, n);
}

// Convert from 8000 samples/sec to 48000 samples/sec
void CDVDongleThread::upSample(const wxFloat32* audioIn, wxFloat32* audioOut)
{
	unsigned int out = 0U;
	for (unsigned int in = 0U; in < DSTAR_AUDIO_BLOCK_SIZE; in++) {
		audioOut[out++] = m_filter.process(audioIn[in]) * UPSAMPLE_AMP;

		audioOut[out++] = m_filter.process(0.0F) * UPSAMPLE_AMP;
		audioOut[out++] = m_filter.process(0.0F) * UPSAMPLE_AMP;
		audioOut[out++] = m_filter.process(0.0F) * UPSAMPLE_AMP;
		audioOut[out++] = m_filter.process(0.0F) * UPSAMPLE_AMP;
		audioOut[out++] = m_filter.process(0.0F) * UPSAMPLE_AMP;
	}
}

// Convert from 48000 samples/sec to 8000 samples/sec
void CDVDongleThread::downSample(const wxFloat32* audioIn, wxFloat32* audioOut)
{
	unsigned int in = 0U;
	for (unsigned int out = 0U; out < DSTAR_AUDIO_BLOCK_SIZE; out++) {
		audioOut[out] = m_filter.process(audioIn[in++]) * DOWNSAMPLE_AMP;

		m_filter.process(audioIn[in++]);
		m_filter.process(audioIn[in++]);
		m_filter.process(audioIn[in++]);
		m_filter.process(audioIn[in++]);
		m_filter.process(audioIn[in++]);
	}
}
