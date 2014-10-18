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

//	ALSA Version by John Wiseman G8BPQ - initially for PI

#ifndef	SoundCardReaderWriter_H
#define	SoundCardReaderWriter_H

#include "AudioCallback.h"

#include <alsa/asoundlib.h>

class CDummyRepeaterThread;

#include <wx/wx.h>

class CSoundCardReaderWriter {
public:
	CSoundCardReaderWriter(const wxString& readDevice, const wxString& writeDevice, unsigned int sampleRate, unsigned int blockSize);
	~CSoundCardReaderWriter();

	void setCallback(CDummyRepeaterThread* callback, int id);
	bool open();
	void close();

	void callback(const wxFloat32* input, wxFloat32* output, unsigned int nSamples);
	void sendtocard(const wxFloat32* input, unsigned int nSamples);
	int getfromcard(wxFloat32* output, unsigned int nSamples);
	static wxArrayString getReadDevices();
	static wxArrayString getWriteDevices();

	void* PollThread(void *userData);
	void kill();

private:
	wxString        m_readDevice;
	wxString        m_writeDevice;
	unsigned int    m_sampleRate;
	unsigned int    m_blockSize;
	CDummyRepeaterThread* m_callback;
	CDummyRepeaterThread* m_thread;
	int             m_id;
	snd_pcm_t *		m_playhandle;
	snd_pcm_t *		m_rechandle;
	snd_pcm_hw_params_t * m_hwparams;
	int				m_playchannels;
	int				m_recchannels;
	bool            m_killed;
	bool            m_started;

//	bool convertNameToDevices(PaDeviceIndex& inDev, PaDeviceIndex& outDev);
};

#endif
