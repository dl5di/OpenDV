/*
 *   Copyright (C) 2009,2010,2014 by Jonathan Naylor G4KLX
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

#ifndef	DVToolReaderThread_H
#define	DVToolReaderThread_H

#include "SoundCardReaderWriter.h"
#include "SlowDataDecoder.h"
#include "DecodeCallback.h"
#include "AudioCallback.h"
#include "DStarDefines.h"
#include "DongleThread.h"
#include "RingBuffer.h"
#include "FileReader.h"

#include <wx/wx.h>

class CDVToolReaderThread : public wxThread, public IAudioCallback, public IDecodeCallback {
public:
	CDVToolReaderThread();
	virtual ~CDVToolReaderThread();

	virtual void callback(const wxFloat32* input, wxFloat32* output, unsigned int nSamples, int id);

	virtual void decodeCallback(const wxFloat32* audio, unsigned int length);

	virtual void setBleep(bool on);
	virtual bool setReader(IFileReader* reader);
	virtual void setSoundCard(CSoundCardReaderWriter* soundcard);
	virtual void setDongle(CDongleThread* dongle);

	virtual void* Entry();
	virtual void  kill();

private:
	IFileReader*            m_reader;
	CSoundCardReaderWriter* m_soundcard;
	CDongleThread*          m_dongle;
	CSlowDataDecoder        m_slowDataDecoder;
	bool                    m_stopped;
	CRingBuffer<wxFloat32>  m_audioBuffer;
	bool                    m_killed;

	void processFile();
	void processHeader(const unsigned char* buffer);
	void processSlowData(const unsigned char* buffer);
	void processAudio(const unsigned char* buffer);
};

#endif
