/*
 *   Copyright (C) 2006-2009 by Jonathan Naylor G4KLX
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

int scrwCallback(const void* input, void* output, unsigned long nSamples, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void* userData)
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
m_buffer(NULL),
m_stream(NULL),
m_callback(NULL),
m_id(-1)
{
	m_buffer = new wxFloat32[blockSize];
}

CSoundCardReaderWriter::~CSoundCardReaderWriter()
{
	delete[] m_buffer;
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
	if (m_callback != NULL)
		m_callback->callback(input, output, nSamples, m_id);
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
