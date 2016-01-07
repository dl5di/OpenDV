/*
 *	Copyright (C) 2009,2010,2015 by Jonathan Naylor, G4KLX
 *	Copyright (C) 2014 by John Wiseman, G8BPQ
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

#if (defined(__APPLE__) && defined(__MACH__)) || defined(__WINDOWS__)

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

#else

#include <alsa/asoundlib.h>

class CSoundCardReader : public wxThread {
public:
	CSoundCardReader(snd_pcm_t* handle, unsigned int blockSize, unsigned int channels, IAudioCallback* callback, int id);
	virtual ~CSoundCardReader();

	virtual void* Entry();

	virtual void kill();

private:
	snd_pcm_t*      m_handle;
	unsigned int    m_blockSize;
	unsigned int    m_channels;
	IAudioCallback* m_callback;
	int             m_id;
	bool            m_killed;
	wxFloat32*      m_buffer;
	short*          m_samples;
};

class CSoundCardWriter : public wxThread {
public:
	CSoundCardWriter(snd_pcm_t* handle, unsigned int blockSize, unsigned int channels, IAudioCallback* callback, int id);
	virtual ~CSoundCardWriter();

	virtual void* Entry();

	virtual void kill();

	virtual bool isBusy() const;

private:
	snd_pcm_t*      m_handle;
	unsigned int    m_blockSize;
	unsigned int    m_channels;
	IAudioCallback* m_callback;
	int             m_id;
	bool            m_killed;
	wxFloat32*      m_buffer;
	short*          m_samples;
};

class CSoundCardReaderWriter {
public:
	CSoundCardReaderWriter(const wxString& readDevice, const wxString& writeDevice, unsigned int sampleRate, unsigned int blockSize);
	~CSoundCardReaderWriter();

	void setCallback(IAudioCallback* callback, int id);
	bool open();
	void close();

	bool isWriterBusy() const;

	static wxArrayString getReadDevices();
	static wxArrayString getWriteDevices();

private:
	wxString             m_readDevice;
	wxString             m_writeDevice;
	unsigned int         m_sampleRate;
	unsigned int         m_blockSize;
	IAudioCallback*      m_callback;
	int                  m_id;
	CSoundCardReader*    m_reader;
	CSoundCardWriter*    m_writer;

	static wxArrayString m_readDevices;
	static wxArrayString m_writeDevices;
};

#endif

#endif
