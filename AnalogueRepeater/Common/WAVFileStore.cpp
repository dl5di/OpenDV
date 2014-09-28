/*
 *	Copyright (C) 2011,2013 by Jonathan Naylor, G4KLX
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

#include "AnalogueDefines.h"
#include "WAVFileReader.h"
#include "WAVFileStore.h"


CWAVFileStore::CWAVFileStore() :
m_data(NULL),
m_length(0U),
m_current(0U)
{
}

CWAVFileStore::~CWAVFileStore()
{
	delete[] m_data;
}

bool CWAVFileStore::isEmpty() const
{
	return m_length == 0U;
}

bool CWAVFileStore::load(const wxString& fileName, unsigned int sampleRate)
{
	wxASSERT(sampleRate > 0U);

	if (fileName.IsEmpty())
		return false;

	CWAVFileReader file(fileName, ANALOGUE_RADIO_BLOCK_SIZE);

	bool ret = file.open();
	if (!ret) {
		wxLogError(wxT("Unable to open the WAV file - %s"), fileName.c_str());
		return false;
	}

	unsigned int rate = file.getSampleRate();
	if (rate != sampleRate) {
		wxLogError(wxT("The sample rate of the WAV file is not valid"));
		file.close();
		return false;
	}

	unsigned int channels = file.getChannels();
	if (channels != 1U) {
		wxLogError(wxT("The number of channels of the WAV file is too high"));
		file.close();
		return false;
	}

	wxFloat32 buffer[ANALOGUE_RADIO_BLOCK_SIZE];

	// Get the length of the audio
	unsigned int n1 = file.read(buffer, ANALOGUE_RADIO_BLOCK_SIZE);
	while (n1 > 0U) {
		m_length += n1;
		n1 = file.read(buffer, ANALOGUE_RADIO_BLOCK_SIZE);
	}

	m_data = new wxFloat32[m_length];

	::memset(m_data, 0x00U, m_length * sizeof(wxFloat32));

	file.rewind();

	unsigned int offset = 0U;
	unsigned int n2 = file.read(m_data + offset, ANALOGUE_RADIO_BLOCK_SIZE);
	while (n2 > 0U) {
		offset += n2;
		n2 = file.read(m_data + offset, ANALOGUE_RADIO_BLOCK_SIZE);
	}

	file.close();

	return true;
}

unsigned int CWAVFileStore::getAudio(wxFloat32* audio, unsigned int length, wxFloat32 amplitude)
{
	wxASSERT(audio != NULL);

	if (m_length == 0U)
		return 0U;

	unsigned int n = 0U;
	while (n < length && m_current < m_length)
		audio[n++] += m_data[m_current++] * amplitude;

	return n;
}

void CWAVFileStore::reset()
{
	m_current = 0U;
}
