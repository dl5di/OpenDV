/*
 *   Copyright (C) 2009-2014 by Jonathan Naylor G4KLX
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

#ifndef	SoundCardController_H
#define	SoundCardController_H

#include "SoundCardReaderWriter.h"
#include "DStarGMSKDemodulator.h"
#include "DStarGMSKModulator.h"
#include "AudioCallback.h"
#include "RingBuffer.h"
#include "Modem.h"
#include "Utils.h"

#include <wx/wx.h>

enum DSRSCC_STATE {
	DSRSCCS_NONE,
	DSRSCCS_HEADER,
	DSRSCCS_DATA
};

class CSoundCardController : public CModem, public IAudioCallback {
public:
	CSoundCardController(const wxString& rxDevice, const wxString& txDevice, bool rxInvert, bool txInvert, wxFloat32 rxLevel, wxFloat32 txLevel, unsigned int txDelay, unsigned int txTail);
	virtual ~CSoundCardController();

	virtual void* Entry();

	virtual bool start();

	virtual unsigned int getSpace();
	virtual bool isTXReady();
	virtual bool isTX();

	virtual bool writeHeader(const CHeaderData& header);
	virtual bool writeData(const unsigned char* data, unsigned int length, bool end);

	virtual void callback(const wxFloat32* input, wxFloat32* output, unsigned int n, int id);

private:
	CSoundCardReaderWriter     m_sound;
	wxFloat32                  m_rxLevel;
	wxFloat32                  m_txLevel;
	unsigned int               m_txDelay;
	unsigned int               m_txTail;
	CRingBuffer<wxFloat32>     m_txAudio;
	CRingBuffer<wxFloat32>     m_rxAudio;
	DSRSCC_STATE               m_rxState;
	wxUint32                   m_patternBuffer;
	CDStarGMSKDemodulator      m_demodulator;
	CDStarGMSKModulator        m_modulator;
	unsigned int               m_preambleCount;
	unsigned int               m_preambleTimer;
	unsigned char*             m_rxBuffer;
	unsigned int               m_rxBufferBits;
	unsigned int               m_dataBits;
	unsigned int               m_mar;
	int*                       m_pathMetric;
	unsigned int*              m_pathMemory0;
	unsigned int*              m_pathMemory1;
	unsigned int*              m_pathMemory2;
	unsigned int*              m_pathMemory3;
	unsigned char*             m_fecOutput;

	void processNone(bool bit);
	void processHeader(bool bit);
	void processData(bool bit);

	bool rxHeader(unsigned char* in, unsigned char* out);
	void acs(int* metric);
	void viterbiDecode(int* data);
	void traceBack();

	void txHeader(const unsigned char* in, unsigned char* out);
	void writeBits(unsigned char c);

	unsigned int countBits(wxUint32 num);
};

#endif

