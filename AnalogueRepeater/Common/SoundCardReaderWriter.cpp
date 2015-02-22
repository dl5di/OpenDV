/*
 *   Copyright (C) 2006-2010,2015 by Jonathan Naylor G4KLX
 *   Copyright (C) 2014 by John Wiseman, G8BPQ
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

#include "SoundCardReaderWriter.h"

#if (defined(__APPLE__) && defined(__MACH__)) || defined(__WINDOWS__)

static int scrwCallback(const void* input, void* output, unsigned long nSamples, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void* userData)
{
	wxASSERT(userData != NULL);

	CSoundCardReaderWriter* object = reinterpret_cast<CSoundCardReaderWriter*>(userData);

	object->callback(static_cast<const wxFloat32*>(input), static_cast<wxFloat32*>(output), nSamples);

	return paContinue;
}

CSoundCardReaderWriter::CSoundCardReaderWriter(const wxString& readDevice, const wxString& writeDevice, unsigned int sampleRate, unsigned int blockSize) :
m_readDevice(readDevice),
m_writeDevice(writeDevice),
m_sampleRate(sampleRate),
m_blockSize(blockSize),
m_callback(NULL),
m_id(-1),
m_stream(NULL)
{
}

CSoundCardReaderWriter::~CSoundCardReaderWriter()
{
}

wxArrayString CSoundCardReaderWriter::getReadDevices()
{
	wxArrayString devices;

	devices.Alloc(10);

	PaError error = ::Pa_Initialize();
	if (error != paNoError)
		return devices;

#if defined(__WINDOWS__)
	PaHostApiIndex apiIndex = ::Pa_HostApiTypeIdToHostApiIndex(paDirectSound);
#elif defined(__APPLE__) && defined(__MACH__)
	PaHostApiIndex apiIndex = ::Pa_HostApiTypeIdToHostApiIndex(paCoreAudio);
#else
	PaHostApiIndex apiIndex = ::Pa_HostApiTypeIdToHostApiIndex(paALSA);
#endif
	if (apiIndex == paHostApiNotFound) {
		::Pa_Terminate();
		return devices;
	}

	PaDeviceIndex n = ::Pa_GetDeviceCount();
	if (n <= 0) {
		::Pa_Terminate();
		return devices;
	}

	for (PaDeviceIndex i = 0; i < n; i++) {
		const PaDeviceInfo* device = ::Pa_GetDeviceInfo(i);

		if (device->hostApi != apiIndex)
			continue;

		if (device->maxInputChannels > 0) {
			wxString name(device->name, wxConvLocal);
			devices.Add(name);
		}
	}

	::Pa_Terminate();

	return devices;
}

wxArrayString CSoundCardReaderWriter::getWriteDevices()
{
	wxArrayString devices;

	devices.Alloc(10);

	PaError error = ::Pa_Initialize();
	if (error != paNoError)
		return devices;

#if defined(__WINDOWS__)
	PaHostApiIndex apiIndex = ::Pa_HostApiTypeIdToHostApiIndex(paDirectSound);
#elif defined(__APPLE__) && defined(__MACH__)
	PaHostApiIndex apiIndex = ::Pa_HostApiTypeIdToHostApiIndex(paCoreAudio);
#else
	PaHostApiIndex apiIndex = ::Pa_HostApiTypeIdToHostApiIndex(paALSA);
#endif
	if (apiIndex == paHostApiNotFound) {
		::Pa_Terminate();
		return devices;
	}

	PaDeviceIndex n = ::Pa_GetDeviceCount();
	if (n <= 0) {
		::Pa_Terminate();
		return devices;
	}

	for (PaDeviceIndex i = 0; i < n; i++) {
		const PaDeviceInfo* device = ::Pa_GetDeviceInfo(i);

		if (device->hostApi != apiIndex)
			continue;

		if (device->maxOutputChannels > 0) {
			wxString name(device->name, wxConvLocal);
			devices.Add(name);
		}
	}

	::Pa_Terminate();

	return devices;
}

void CSoundCardReaderWriter::setCallback(IAudioCallback* callback, int id)
{
	wxASSERT(callback != NULL);

	m_callback = callback;
	m_id       = id;
}

bool CSoundCardReaderWriter::open()
{
	PaError error = ::Pa_Initialize();
	if (error != paNoError) {
		wxLogError(wxT("Cannot initialise PortAudio"));
		return false;
	}

	PaStreamParameters* pParamsIn  = NULL;
	PaStreamParameters* pParamsOut = NULL;

	PaStreamParameters paramsIn;
	PaStreamParameters paramsOut;

	PaDeviceIndex inDev, outDev;
	bool res = convertNameToDevices(inDev, outDev);
	if (!res) {
		wxLogError(wxT("Cannot convert name to device"));
		return false;
	}

	if (inDev != -1) {
		const PaDeviceInfo* inInfo  = ::Pa_GetDeviceInfo(inDev);
		if (inInfo == NULL) {
			wxLogError(wxT("Cannot get device information for the input device"));
			return false;
		}

		paramsIn.device                    = inDev;
		paramsIn.channelCount              = 1;
		paramsIn.sampleFormat              = paFloat32;
		paramsIn.hostApiSpecificStreamInfo = NULL;
		paramsIn.suggestedLatency          = inInfo->defaultLowInputLatency;

		pParamsIn = &paramsIn;
	}

	if (outDev != -1) {
		const PaDeviceInfo* outInfo = ::Pa_GetDeviceInfo(outDev);
		if (outInfo == NULL) {
			wxLogError(wxT("Cannot get device information for the output device"));
			return false;
		}

		paramsOut.device                    = outDev;
		paramsOut.channelCount              = 1;
		paramsOut.sampleFormat              = paFloat32;
		paramsOut.hostApiSpecificStreamInfo = NULL;
		paramsOut.suggestedLatency          = outInfo->defaultLowOutputLatency;

		pParamsOut = &paramsOut;
	}

	error = ::Pa_OpenStream(&m_stream, pParamsIn, pParamsOut, double(m_sampleRate), m_blockSize, paNoFlag, &scrwCallback, this);
	if (error != paNoError) {
		wxLogError(wxT("Cannot open the audios stream(s)"));
		::Pa_Terminate();
		return false;
	}

	error = ::Pa_StartStream(m_stream);
	if (error != paNoError) {
		wxLogError(wxT("Cannot start the audio stream(s)"));
		::Pa_CloseStream(m_stream);
		m_stream = NULL;

		::Pa_Terminate();
		return false;
	}

 	return true;
}

void CSoundCardReaderWriter::close()
{
	wxASSERT(m_stream != NULL);

	::Pa_AbortStream(m_stream);

	::Pa_CloseStream(m_stream);

	::Pa_Terminate();
}

void CSoundCardReaderWriter::callback(const wxFloat32* input, wxFloat32* output, unsigned int nSamples)
{
	if (m_callback != NULL) {
		m_callback->readCallback(input, nSamples, m_id);
		m_callback->writeCallback(output, nSamples, m_id);
	}
}

bool CSoundCardReaderWriter::convertNameToDevices(PaDeviceIndex& inDev, PaDeviceIndex& outDev)
{
	inDev = outDev = -1;

#if defined(__WINDOWS__)
	PaHostApiIndex apiIndex = ::Pa_HostApiTypeIdToHostApiIndex(paDirectSound);
#elif defined(__APPLE__) && defined(__MACH__)
	PaHostApiIndex apiIndex = ::Pa_HostApiTypeIdToHostApiIndex(paCoreAudio);
#else
	PaHostApiIndex apiIndex = ::Pa_HostApiTypeIdToHostApiIndex(paALSA);
#endif
	if (apiIndex == paHostApiNotFound)
		return false;

	PaDeviceIndex n = ::Pa_GetDeviceCount();
	if (n <= 0)
		return false;

	for (PaDeviceIndex i = 0; i < n; i++) {
		const PaDeviceInfo* device = ::Pa_GetDeviceInfo(i);

		if (device->hostApi != apiIndex)
			continue;

		wxString name(device->name, wxConvLocal);

		if (!m_readDevice.IsEmpty() && m_readDevice.IsSameAs(name) && device->maxInputChannels > 0)
			inDev = i;

		if (!m_writeDevice.IsEmpty() && m_writeDevice.IsSameAs(name) && device->maxOutputChannels > 0)
			outDev = i;
	}

	if (inDev == -1 && outDev == -1)
		return false;

	return true;
}

#else

CSoundCardReaderWriter::CSoundCardReaderWriter(const wxString& readDevice, const wxString& writeDevice, unsigned int sampleRate, unsigned int blockSize) :
m_readDevice(readDevice),
m_writeDevice(writeDevice),
m_sampleRate(sampleRate),
m_blockSize(blockSize),
m_callback(NULL),
m_id(-1),
m_reader(NULL),
m_writer(NULL)
{
    wxASSERT(sampleRate > 0U);
    wxASSERT(blockSize > 0U);
}

CSoundCardReaderWriter::~CSoundCardReaderWriter()
{
}

wxArrayString CSoundCardReaderWriter::getReadDevices()
{
	snd_ctl_t *handle = NULL;
	snd_pcm_t *pcm = NULL;
	char NameString[256];

	wxArrayString devices;
	devices.Alloc(10);

	snd_ctl_card_info_t* info;
	snd_ctl_card_info_alloca(&info);

	snd_pcm_info_t* pcminfo;
	snd_pcm_info_alloca(&pcminfo);

	snd_pcm_hw_params_t* pars;
	snd_pcm_hw_params_alloca(&pars);

	unsigned min, max;
	int err;
	snd_pcm_stream_t stream = SND_PCM_STREAM_CAPTURE;

	int card = -1;
	while (::snd_card_next(&card) == 0 && card >= 0) {
		char hwdev[80];
		::sprintf(hwdev, "hw:%d", card);

		if (::snd_ctl_open(&handle, hwdev, 0) < 0)
			continue;

		::snd_ctl_card_info(handle, info);
                ::snd_ctl_card_info_get_name(info);

		int dev = -1;
		while (::snd_ctl_pcm_next_device(handle, &dev) == 0 && dev >= 0) {
			::snd_pcm_info_set_device(pcminfo, dev);
			::snd_pcm_info_set_subdevice(pcminfo, 0);
			::snd_pcm_info_set_stream(pcminfo, stream);

			err = ::snd_ctl_pcm_info(handle, pcminfo);
			if (err != -ENOENT) {
				::sprintf(hwdev, "hw:%d,%d", card, dev);

				if (::snd_pcm_open(&pcm, hwdev, stream, SND_PCM_NONBLOCK) < 0)
					continue;

				::snd_pcm_hw_params_any(pcm, pars);
				::snd_pcm_hw_params_get_channels_min(pars, &min);
				::snd_pcm_hw_params_get_channels_max(pars, &max);

				::snd_pcm_hw_params_get_rate_min(pars, &min, NULL);
				::snd_pcm_hw_params_get_rate_max(pars, &max, NULL);
	
				::sprintf(NameString, "hw:%d,%d %s(%s)",
					card, dev,
					::snd_pcm_info_get_name(pcminfo),
					snd_ctl_card_info_get_name(info));

				wxString name(NameString, wxConvLocal);
				devices.Add(name);

				::snd_pcm_close(pcm);
				pcm = NULL;
			}
                }

                ::snd_ctl_close(handle);
	}

	return devices;
}

wxArrayString CSoundCardReaderWriter::getWriteDevices()
{
	snd_ctl_t *handle = NULL;
	snd_pcm_t *pcm = NULL;
	char NameString[256];

	wxArrayString devices;
	devices.Alloc(10);

	snd_ctl_card_info_t* info;
	snd_ctl_card_info_alloca(&info);

	snd_pcm_info_t* pcminfo;
	snd_pcm_info_alloca(&pcminfo);

	snd_pcm_hw_params_t* pars;
	snd_pcm_hw_params_alloca(&pars);

	unsigned min, max;
	int err;
	snd_pcm_stream_t stream = SND_PCM_STREAM_PLAYBACK;
	
	int card = -1;
	while (::snd_card_next(&card) == 0 && card >= 0) {
		char hwdev[80];
		::sprintf(hwdev, "hw:%d", card);

		if (::snd_ctl_open(&handle, hwdev, 0) < 0)
			continue;

		::snd_ctl_card_info(handle, info);
                ::snd_ctl_card_info_get_name(info);

                int dev = -1;
		while (::snd_ctl_pcm_next_device(handle, &dev) == 0 && dev >= 0) {
			::snd_pcm_info_set_device(pcminfo, dev);
			::snd_pcm_info_set_subdevice(pcminfo, 0);
			::snd_pcm_info_set_stream(pcminfo, stream);

			err= ::snd_ctl_pcm_info(handle, pcminfo);
			if (err != -ENOENT) {
                                ::sprintf(hwdev, "hw:%d,%d", card, dev);

        			if (::snd_pcm_open(&pcm, hwdev, stream, SND_PCM_NONBLOCK) < 0)
        				continue;

        			::snd_pcm_hw_params_any(pcm, pars); 
        			::snd_pcm_hw_params_get_channels_min(pars, &min);
	        		::snd_pcm_hw_params_get_channels_max(pars, &max);
			
        			::snd_pcm_hw_params_get_rate_min(pars, &min, NULL);
        			::snd_pcm_hw_params_get_rate_max(pars, &max, NULL);

        			::sprintf(NameString, "hw:%d,%d %s(%s)",
        				card, dev,
					::snd_pcm_info_get_name(pcminfo),
					::snd_ctl_card_info_get_name(info));

	        		wxString name(NameString, wxConvLocal);
		        	devices.Add(name);

        			::snd_pcm_close(pcm);
        			pcm = NULL;
			}
                }

                ::snd_ctl_close(handle);
	}

	return devices;
}

void CSoundCardReaderWriter::setCallback(IAudioCallback* callback, int id)
{
	wxASSERT(callback != NULL);

	m_callback = callback;

	m_id = id;
}

bool CSoundCardReaderWriter::open()
{
	int err = 0;

	char buf1[100];
	char buf2[100];
	char* ptr;

	::strcpy(buf1, (const char*)m_writeDevice.mb_str(wxConvUTF8));
	::strcpy(buf2, (const char*)m_readDevice.mb_str(wxConvUTF8));

	ptr = ::strchr(buf1, ' ');
	if (ptr) *ptr = 0;				// Get Device part of name

	ptr = ::strchr(buf2, ' ');
	if (ptr) *ptr = 0;				// Get Device part of name

	wxString writeDevice(buf1, wxConvLocal);
	wxString readDevice(buf2, wxConvLocal);

	snd_pcm_t* playHandle = NULL;
	if ((err = ::snd_pcm_open(&playHandle, buf1, SND_PCM_STREAM_PLAYBACK, 0)) < 0) {
		wxString error(::snd_strerror(err), wxConvLocal);
		wxLogError(wxT("Cannot open playback audio device %s (%s)"), writeDevice.c_str(), error.c_str());
		return false;
	}

	snd_pcm_hw_params_t* hw_params;
	if ((err = ::snd_pcm_hw_params_malloc(&hw_params)) < 0) {
		wxString error(::snd_strerror(err), wxConvLocal);
		wxLogError(wxT("Cannot allocate hardware parameter structure (%s)"), error.c_str());
		return false;
	}

	if ((err = ::snd_pcm_hw_params_any(playHandle, hw_params)) < 0) {
		wxString error(::snd_strerror(err), wxConvLocal);
		wxLogError(wxT("Cannot initialize hardware parameter structure (%s)"), error.c_str());
		return false;
	}

	if ((err = ::snd_pcm_hw_params_set_access(playHandle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0) {
		wxString error(::snd_strerror(err), wxConvLocal);
		wxLogError(wxT("Cannot set access type (%s)"), error.c_str());
		return false;
	}

	if ((err = ::snd_pcm_hw_params_set_format(playHandle, hw_params, SND_PCM_FORMAT_S16_LE)) < 0) {
		wxString error(::snd_strerror(err), wxConvLocal);
		wxLogError(wxT("Cannot set sample format (%s)"), error.c_str());
		return false;
	}
	
	if ((err = ::snd_pcm_hw_params_set_rate(playHandle, hw_params, m_sampleRate, 0)) < 0) {
		wxString error(::snd_strerror(err), wxConvLocal);
		wxLogError(wxT("Cannot set sample rate (%s)"), error.c_str());
		return false;
	}

	unsigned int playChannels = 1U;

	if ((err = ::snd_pcm_hw_params_set_channels(playHandle, hw_params, 1)) < 0) {
		playChannels = 2U;

		if ((err = ::snd_pcm_hw_params_set_channels(playHandle, hw_params, 2)) < 0) {
			wxString error(::snd_strerror(err), wxConvLocal);
			wxLogError(wxT("Cannot play set channel count (%s)"), error.c_str());
			return false;
		}
	}
	
	if ((err = ::snd_pcm_hw_params(playHandle, hw_params)) < 0) {
		wxString error(::snd_strerror(err), wxConvLocal);
		wxLogError(wxT("Cannot set parameters (%s)"), error.c_str());
		return false;
	}
	
	::snd_pcm_hw_params_free(hw_params);
	
	if ((err = ::snd_pcm_prepare(playHandle)) < 0) {
		wxString error(::snd_strerror(err), wxConvLocal);
		wxLogError(wxT("Cannot prepare audio interface for use (%s)"), error.c_str());
		return false;
	}

	// Open Capture
	snd_pcm_t* recHandle = NULL;
	if ((err = ::snd_pcm_open(&recHandle, buf2, SND_PCM_STREAM_CAPTURE, 0)) < 0) {
		wxString error(::snd_strerror(err), wxConvLocal);
		wxLogError(wxT("Cannot open capture audio device %s (%s)"), readDevice.c_str(), error.c_str());
		return false;
	}

	if ((err = ::snd_pcm_hw_params_malloc(&hw_params)) < 0) {
		wxString error(::snd_strerror(err), wxConvLocal);
		wxLogError(wxT("Cannot allocate hardware parameter structure (%s)"), error.c_str());
		return false;
	}

	if ((err = ::snd_pcm_hw_params_any(recHandle, hw_params)) < 0) {
		wxString error(::snd_strerror(err), wxConvLocal);
		wxLogError(wxT("Cannot initialize hardware parameter structure (%s)"), error.c_str());
		return false;
	}
	
	if ((err = ::snd_pcm_hw_params_set_access(recHandle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0) {
		wxString error(::snd_strerror(err), wxConvLocal);
		wxLogError(wxT("Cannot set access type (%s)"), error.c_str());
		return false;
	}

	if ((err = ::snd_pcm_hw_params_set_format(recHandle, hw_params, SND_PCM_FORMAT_S16_LE)) < 0) {
		wxString error(::snd_strerror(err), wxConvLocal);
		wxLogError(wxT("Cannot set sample format (%s)"), error.c_str());
		return false;
	}
	
	if ((err = ::snd_pcm_hw_params_set_rate(recHandle, hw_params, m_sampleRate, 0)) < 0) {
		wxString error(::snd_strerror(err), wxConvLocal);
		wxLogError(wxT("Cannot set sample rate (%s)"), error.c_str());
		return false;
	}
	
	unsigned int recChannels = 1U;
	
	if ((err = ::snd_pcm_hw_params_set_channels(recHandle, hw_params, 1)) < 0) {
		recChannels = 2U;

		if ((err = ::snd_pcm_hw_params_set_channels (recHandle, hw_params, 2)) < 0) {
			wxString error(::snd_strerror(err), wxConvLocal);
			wxLogError(wxT("Cannot rec set channel count (%s)"), error.c_str());
			return false;
		}
	}
	
	if ((err = ::snd_pcm_hw_params(recHandle, hw_params)) < 0) {
		wxString error(::snd_strerror(err), wxConvLocal);
		wxLogError(wxT("Cannot set parameters (%s)"), error.c_str());
		return false;
	}

	::snd_pcm_hw_params_free(hw_params);

	if ((err = ::snd_pcm_prepare(recHandle)) < 0) {
		wxString error(::snd_strerror(err), wxConvLocal);
		wxLogError(wxT("Cannot prepare audio interface for use (%s)"), error.c_str());
		return false;
	}

	short samples[256];
	for (unsigned int i = 0U; i < 10U; ++i)
		::snd_pcm_readi(recHandle, samples, 128);

	wxLogMessage(wxT("Opened %s %s Rate %u"), writeDevice.c_str(), readDevice.c_str(), m_sampleRate);

	m_reader = new CSoundCardReader(recHandle,  m_blockSize, recChannels,  m_callback, m_id);
	m_writer = new CSoundCardWriter(playHandle, m_blockSize, playChannels, m_callback, m_id);

	m_reader->Create();
	m_reader->Run();

	m_writer->Create();
	m_writer->Run();

 	return true;
}

void CSoundCardReaderWriter::close()
{
	m_reader->kill();
	m_writer->kill();

	m_reader->Wait();
	m_writer->Wait();
}

CSoundCardReader::CSoundCardReader(snd_pcm_t* handle, unsigned int blockSize, unsigned int channels, IAudioCallback* callback, int id) :
wxThread(wxTHREAD_JOINABLE),
m_handle(handle),
m_blockSize(blockSize),
m_channels(channels),
m_callback(callback),
m_id(id),
m_killed(false),
m_buffer(NULL),
m_samples(NULL)
{
	wxASSERT(handle != NULL);
	wxASSERT(blockSize > 0U);
	wxASSERT(channels == 1U || channels == 2U);
	wxASSERT(callback != NULL);

	m_buffer  = new wxFloat32[blockSize];
	m_samples = new short[2U * blockSize];
}

CSoundCardReader::~CSoundCardReader()
{
	delete[] m_buffer;
	delete[] m_samples;
}

void* CSoundCardReader::Entry()
{
	wxLogMessage(wxT("Starting ALSA reader thread"));

	while (!m_killed) {
		snd_pcm_sframes_t ret;
		while ((ret = ::snd_pcm_readi(m_handle, m_samples, m_blockSize)) < 0) {
			wxLogWarning(wxT("snd_pcm_readi returned %d"), ret);
			::snd_pcm_recover(m_handle, ret, 1);
		}

		if (m_channels == 1U) {
			for (int n = 0; n < ret; n++)
				m_buffer[n] = wxFloat32(m_samples[n]) / 32768.0F;
		} else {
			int i = 0;
			for (int n = 0; n < (ret * 2); n += 2)
				m_buffer[i++] = wxFloat32(m_samples[n + 1]) / 32768.0F;
		}

		m_callback->readCallback(m_buffer, (unsigned int)ret, m_id);
	}

	wxLogMessage(wxT("Stopping ALSA reader thread"));

	::snd_pcm_close(m_handle);

	return NULL;
}

void CSoundCardReader::kill()
{
	m_killed = true;
}

CSoundCardWriter::CSoundCardWriter(snd_pcm_t* handle, unsigned int blockSize, unsigned int channels, IAudioCallback* callback, int id) :
wxThread(wxTHREAD_JOINABLE),
m_handle(handle),
m_blockSize(blockSize),
m_channels(channels),
m_callback(callback),
m_id(id),
m_killed(false),
m_buffer(NULL),
m_samples(NULL)
{
	wxASSERT(handle != NULL);
	wxASSERT(blockSize > 0U);
	wxASSERT(channels == 1U || channels == 2U);
	wxASSERT(callback != NULL);

	m_buffer  = new wxFloat32[2U * blockSize];
	m_samples = new short[4U * blockSize];
}

CSoundCardWriter::~CSoundCardWriter()
{
	delete[] m_buffer;
	delete[] m_samples;
}

void* CSoundCardWriter::Entry()
{
	wxLogMessage(wxT("Starting ALSA writer thread"));

	while (!m_killed) {
		unsigned int nSamples = 2U * m_blockSize;
		m_callback->writeCallback(m_buffer, nSamples, m_id);

		if (nSamples == 0U) {
			Sleep(10UL);
		} else {
			if (m_channels == 1U) {
				for (unsigned int n = 0U; n < nSamples; n++)
					m_samples[n] = short(m_buffer[n] * 32767.0F);
			} else {
				unsigned int i = 0U;
				for (unsigned int n = 0U; n < nSamples; n++) {
					short sample = short(m_buffer[n] * 32767.0F);
					m_samples[i++] = sample;
					m_samples[i++] = sample;			// Same value to both channels
				}
			}
        
			snd_pcm_sframes_t ret;
			while ((ret = ::snd_pcm_writei(m_handle, m_samples, nSamples)) < 0) {
				wxLogWarning(wxT("snd_pcm_writei returned %d"), ret);
				::snd_pcm_recover(m_handle, ret, 1);
			}
		}
	}

	wxLogMessage(wxT("Stopping ALSA writer thread"));

	::snd_pcm_close(m_handle);

	return NULL;
}

void CSoundCardWriter::kill()
{
	m_killed = true;
}

#endif
