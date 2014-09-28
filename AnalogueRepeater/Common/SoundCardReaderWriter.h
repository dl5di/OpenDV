/*
 *	Copyright (C) 2009,2010 by Jonathan Naylor, G4KLX
 *
 *	This program is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation; version 2 of the License.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 */

#ifndef	SoundCardReaderWriter_H
#define	SoundCardReaderWriter_H

#include "AudioCallback.h"

#include <wx/wx.h>

#include "portaudio.h"

class CSoundCardReaderWriter {
public:
	CSoundCardReaderWriter(const wxString& readDevice, const wxString& writeDevice, unsigned int sampleRate, unsigned int blockSize);
	~CSoundCardReaderWriter();

	void setCallback(IAudioCallback* callback, int id);
	bool open();
	void close();

	void callback(const wxFloat32* input, wxFloat32* output, unsigned int nSamples);

	static wxArrayString getReadDevices();
	static wxArrayString getWriteDevices();

private:
	wxString        m_readDevice;
	wxString        m_writeDevice;
	unsigned int    m_sampleRate;
	unsigned int    m_blockSize;
	IAudioCallback* m_callback;
	int             m_id;
	PaStream*       m_stream;

	bool convertNameToDevices(PaDeviceIndex& inDev, PaDeviceIndex& outDev);
};

#endif
