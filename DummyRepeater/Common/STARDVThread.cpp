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

#include "STARDVThread.h"
#include "DStarDefines.h"

const unsigned int MAX_PACKETS = 10U;

CSTARDVThread::CSTARDVThread(ISTARDVController* dongle) :
CDongleThread(),
m_dongle(dongle),
m_wantMode(A3_IDLE),
m_mode(A3_IDLE),
m_packets(0U)
{
	wxASSERT(dongle != NULL);
}

CSTARDVThread::~CSTARDVThread()
{
}

bool CSTARDVThread::open()
{
	m_mode = m_wantMode = A3_ENCODE;

	m_packets = 0U;

	return m_dongle->open();
}

void* CSTARDVThread::Entry()
{
	while (!m_killed) {
		if (m_mode != m_wantMode) {
			if (m_mode == A3_DECODE && m_packets == 0U && m_decodeData.isEmpty()) {
				if (m_bleep)
					sendBleep();

				reset();
				m_mode = m_wantMode;
			} else if (/*m_mode == A3_ENCODE && */m_packets == 0U/* && m_encodeAudio.isEmpty()*/) {
				reset();
				m_mode = m_wantMode;
			} else if (m_mode == A3_IDLE) {
				m_mode = m_wantMode;
			}
		}

		switch (m_mode) {
			case A3_DECODE:
				processDecodeIn();
			//	processDecodeOut();
				break;

			case A3_ENCODE:
				processEncodeIn();
				processEncodeOut();
				break;

			default:
				break;
		}

		Sleep(5UL);
	}

	m_dongle->close();
	delete m_dongle;

	return NULL;
}

void CSTARDVThread::setDecode()
{
	if (m_mode != A3_DECODE && m_decodeCallback == NULL)
		return;

	m_wantMode = A3_DECODE;
}

void CSTARDVThread::setEncode()
{
	if (m_mode != A3_ENCODE && m_encodeCallback == NULL)
		return;

	m_wantMode = A3_ENCODE;
}

void CSTARDVThread::setIdle()
{
	m_wantMode = A3_ENCODE;
}

void CSTARDVThread::processDecodeIn() //************************
{
	if (m_packets >= MAX_PACKETS)
		return;

	unsigned char ambe[VOICE_FRAME_LENGTH_BYTES];
	unsigned int len = m_decodeData.getData(ambe, VOICE_FRAME_LENGTH_BYTES);
	if (len == 0U)
		return;
    
	// Convert the AMBE data to real audio, even if it's nonsense
	m_dongle->decodeIn(ambe, VOICE_FRAME_LENGTH_BYTES);

//	m_packets++;
}

void CSTARDVThread::processDecodeOut()
{
	wxFloat32 audioIn[DSTAR_AUDIO_BLOCK_SIZE];
	bool res = m_dongle->decodeOut(audioIn, DSTAR_AUDIO_BLOCK_SIZE);
	if (res) {
		wxFloat32 audioOut[DSTAR_RADIO_BLOCK_SIZE];
		upSample(audioIn, audioOut);

		m_decodeCallback->decodeCallback(audioOut, DSTAR_RADIO_BLOCK_SIZE);

		if (m_packets > 0U)
			m_packets--;
	}
}

void CSTARDVThread::processEncodeIn()
{
	if (m_packets >= MAX_PACKETS)
		return;

	wxFloat32 audioIn[DSTAR_RADIO_BLOCK_SIZE];
	::memset(audioIn, 0x00, DSTAR_RADIO_BLOCK_SIZE * sizeof(wxFloat32));

	unsigned int len = m_encodeAudio.getData(audioIn, DSTAR_RADIO_BLOCK_SIZE);
	if (len == 0U)
		return;

//	wxFloat32 audioOut[DSTAR_AUDIO_BLOCK_SIZE];
//	downSample(audioIn, audioOut);

	// Convert the audio into AMBE data
//	m_dongle->encodeIn(audioOut, DSTAR_AUDIO_BLOCK_SIZE);

//	m_packets++;
}

void CSTARDVThread::processEncodeOut()  // ******************************
{
    unsigned char ambe[VOICE_FRAME_LENGTH_BYTES];
    bool tx;
	bool res = m_dongle->encodeOut(ambe, VOICE_FRAME_LENGTH_BYTES, tx);
	if (res) {
		m_encodeCallback->encodeCallback(ambe, VOICE_FRAME_LENGTH_BYTES, tx ? PS_TRANSMIT : PS_RECEIVE);

		if (m_packets > 0U)
			m_packets--;
    }
}
