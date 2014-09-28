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

#ifndef	DVToolWriterThread_H
#define	DVToolWriterThread_H

#include "SoundCardReaderWriter.h"
#include "SlowDataEncoder.h"
#include "EncodeCallback.h"
#include "WAVFileReader.h"
#include "AudioCallback.h"
#include "DStarDefines.h"
#include "DongleThread.h"
#include "RingBuffer.h"
#include "FileWriter.h"
#include "HeaderData.h"

#include <wx/wx.h>

class CDVToolWriterThread : public wxThread, public IAudioCallback, public IEncodeCallback {
public:
	CDVToolWriterThread();
	virtual ~CDVToolWriterThread();

	virtual void callback(const wxFloat32* input, wxFloat32* output, unsigned int nSamples, int id);

	virtual void encodeCallback(const unsigned char* ambe, unsigned int length);

	virtual bool processFile(CWAVFileReader* reader, CHeaderData* header, const wxString& text, IFileWriter* writer);

	virtual bool processAudio(CHeaderData* header, const wxString& text, IFileWriter* writer);
	virtual void processAudio();

	virtual void setSoundCard(CSoundCardReaderWriter* soundcard);
	virtual void setDongle(CDongleThread* dongle);

	virtual void* Entry();
	virtual void  kill();

private:
	CSoundCardReaderWriter* m_soundcard;
	CDongleThread*          m_dongle;
	CSlowDataEncoder        m_encoder;
	bool                    m_stopped;
	CWAVFileReader*         m_reader;
	IFileWriter*            m_writer;
	CRingBuffer<wxFloat32>  m_audioBuffer;
	unsigned int            m_count;
	bool                    m_killed;

	void processFile();
	void processSoundcard();
};

#endif
