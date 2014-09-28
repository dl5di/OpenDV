/*
 *   Copyright (C) 2002-2004,2006-2009,2014 by Jonathan Naylor G4KLX
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

#include "WAVFileReader.h"

#if defined(__WINDOWS__)

const int WAVE_FORMAT_IEEE_FLOAT = 3;

CWAVFileReader::CWAVFileReader(const wxString& fileName, unsigned int blockSize) :
m_fileName(fileName),
m_blockSize(blockSize),
m_format(FORMAT_16BIT),
m_channels(0U),
m_sampleRate(0U),
m_buffer8(NULL),
m_buffer16(NULL),
m_buffer32(NULL),
m_handle(NULL),
m_parent(),
m_child(),
m_offset(0L)
{
	wxASSERT(blockSize > 0U);

	m_buffer8  = new wxUint8[blockSize * 4U];
	m_buffer16 = new wxInt16[blockSize * 4U];
	m_buffer32 = new wxFloat32[blockSize * 4U];
}

CWAVFileReader::~CWAVFileReader()
{
	delete[] m_buffer8;
	delete[] m_buffer16;
	delete[] m_buffer32;
}

wxString CWAVFileReader::getFilename() const
{
	return m_fileName;
}

bool CWAVFileReader::open()
{
	m_handle = ::mmioOpen(LPWSTR(m_fileName.c_str()), 0, MMIO_READ | MMIO_ALLOCBUF);
	if (m_handle == NULL) {
		wxLogError(wxT("WAVFileReader: could not open the WAV file %s\n"), m_fileName.c_str());
		return false;
	}

	MMCKINFO parent;
	parent.fccType = mmioFOURCC('W', 'A', 'V', 'E');

	MMRESULT res = ::mmioDescend(m_handle, &parent, 0, MMIO_FINDRIFF);
	if (res != MMSYSERR_NOERROR) {
		wxLogError(wxT("WAVFileReader: %s has no \"WAVE\" header\n"), m_fileName.c_str());
		::mmioClose(m_handle, 0U);
		m_handle = NULL;
		return false;
	}

	MMCKINFO child;
	child.ckid = mmioFOURCC('f', 'm', 't', ' ');

	res = ::mmioDescend(m_handle, &child, &parent, MMIO_FINDCHUNK);
	if (res != MMSYSERR_NOERROR) {
		wxLogError(wxT("WAVFileReader: %s has no \"fmt \" chunk\n"), m_fileName.c_str());
		::mmioClose(m_handle, 0U);
		m_handle = NULL;
		return false;
	}

	WAVEFORMATEX format;

	LONG len = ::mmioRead(m_handle, (char *)&format, child.cksize);
	if (len != LONG(child.cksize)) {
		wxLogError(wxT("WAVFileReader: %s is corrupt, cannot read the WAVEFORMATEX structure\n"), m_fileName.c_str());
		::mmioClose(m_handle, 0U);
		m_handle = NULL;
		return false;
	}
        
	if (format.wFormatTag != WAVE_FORMAT_PCM && format.wFormatTag != WAVE_FORMAT_IEEE_FLOAT) {
		wxLogError(wxT("WAVFileReader: %s is not PCM or IEEE Float format, is %u\n"), m_fileName.c_str(), format.wFormatTag);
		::mmioClose(m_handle, 0U);
		m_handle = NULL;
		return false;
	}

	m_sampleRate = format.nSamplesPerSec;

	m_channels = format.nChannels;
	if (m_channels > 2U) {
		wxLogError(wxT("WAVFileReader: %s has %u channels, more than 2\n"), m_fileName.c_str(), m_channels);
		::mmioClose(m_handle, 0U);
		m_handle = NULL;
		return false;
	}

	if (format.wBitsPerSample == 8U && format.wFormatTag == WAVE_FORMAT_PCM) {
		m_format = FORMAT_8BIT;
	} else if (format.wBitsPerSample == 16U && format.wFormatTag == WAVE_FORMAT_PCM) {
		m_format = FORMAT_16BIT;
	} else if (format.wBitsPerSample == 32U && format.wFormatTag == WAVE_FORMAT_IEEE_FLOAT) {
		m_format = FORMAT_32BIT;
	} else {
		wxLogError(wxT("WAVFileReader: %s has sample width %u and format %u\n"), m_fileName.c_str(), format.wBitsPerSample, format.wFormatTag);
		::mmioClose(m_handle, 0U);
		m_handle = NULL;
		return false;
	}

	res = ::mmioAscend(m_handle, &child, 0);
	if (res != MMSYSERR_NOERROR) {
		wxLogError(wxT("WAVFileReader: %s is corrupt, cannot ascend\n"), m_fileName.c_str());
		::mmioClose(m_handle, 0U);
		m_handle = NULL;
		return false;
	}

	child.ckid = mmioFOURCC('d', 'a', 't', 'a');

	res = ::mmioDescend(m_handle, &child, &parent, MMIO_FINDCHUNK);
	if (res != MMSYSERR_NOERROR) {
		wxLogError(wxT("WAVFileReader: %s has no \"data\" chunk\n"), m_fileName.c_str());
		::mmioClose(m_handle, 0U);
		m_handle = NULL;
		return false;
	}

	// Get the current location so we can rewind if needed
	m_offset = ::mmioSeek(m_handle, 0L, SEEK_CUR);

	return true;
}

unsigned int CWAVFileReader::read(wxFloat32* data, unsigned int length)
{
	wxASSERT(m_handle != NULL);
	wxASSERT(data != NULL);

	if (length == 0U)
		return 0U;

	LONG n = 0L;
	LONG i;

	switch (m_format) {
		case FORMAT_8BIT:
			n = ::mmioRead(m_handle, (char *)m_buffer8, length * m_channels * sizeof(wxUint8));

			if (n <= 0L)
				return 0U;

			n /= sizeof(wxUint8);

			for (i = 0L; i < n; i++)
				data[i] = (float(m_buffer8[i]) - 127.0F) / 128.0F;

			n /= m_channels;
			break;

		case FORMAT_16BIT:
			n = ::mmioRead(m_handle, (char *)m_buffer16, length * m_channels * sizeof(wxInt16));

			if (n <= 0L)
				return 0U;

			n /= sizeof(wxInt16);

			for (i = 0L; i < n; i++)
				data[i] = float(m_buffer16[i]) / 32768.0F;

			n /= m_channels;
			break;

		case FORMAT_32BIT:
			n = ::mmioRead(m_handle, (char *)m_buffer32, length * m_channels * sizeof(float));

			if (n <= 0L)
				return 0U;

			n /= (sizeof(float) * m_channels);

			switch (m_channels) {
				case 1U:
					for (i = 0L; i < n; i++)
						data[i] = m_buffer32[i];
					break;
				case 2U:
					// Swap I and Q for SDR-1000 data
					for (i = 0L; i < n; i++) {
						data[i * 2U + 0U] = m_buffer32[i * 2U + 1U];
						data[i * 2U + 1U] = m_buffer32[i * 2U + 0U];
					}
					break;
			}
			break;
	}

	return n;
}

void CWAVFileReader::rewind()
{
	wxASSERT(m_handle != NULL);

	::mmioSeek(m_handle, m_offset, SEEK_SET);
}

void CWAVFileReader::close()
{
	wxASSERT(m_handle != NULL);

	::mmioClose(m_handle, 0U);

	m_handle = NULL;
}

unsigned int CWAVFileReader::getSampleRate() const
{
	return m_sampleRate;
}

unsigned int CWAVFileReader::getChannels() const
{
	return m_channels;
}

#else

const int FORMAT_PCM        = 1;
const int FORMAT_IEEE_FLOAT = 3;

CWAVFileReader::CWAVFileReader(const wxString& fileName, unsigned int blockSize) :
m_fileName(fileName),
m_blockSize(blockSize),
m_format(FORMAT_16BIT),
m_channels(0U),
m_sampleRate(0U),
m_buffer8(NULL),
m_buffer16(NULL),
m_buffer32(NULL),
m_file(NULL),
m_offset(0),
m_length(0U)
{
	wxASSERT(blockSize > 0U);

	m_buffer8  = new wxUint8[blockSize * 4U];
	m_buffer16 = new wxInt16[blockSize * 4U];
	m_buffer32 = new wxFloat32[blockSize * 4U];
}

CWAVFileReader::~CWAVFileReader()
{
	delete[] m_buffer8;
	delete[] m_buffer16;
	delete[] m_buffer32;
}

wxString CWAVFileReader::getFilename() const
{
	return m_fileName;
}

bool CWAVFileReader::open()
{
	m_file = new wxFFile(m_fileName.c_str(), wxT("rb"));

	bool ret = m_file->IsOpened();
	if (!ret) {
		wxLogError(wxT("WAVFileReader: could not open the WAV file %s."), m_fileName.c_str());
		delete m_file;
		m_file = NULL;
		return false;
	}

	unsigned char buffer[4];
	unsigned int n = m_file->Read(buffer, 4);
	if (n != 4U || ::memcmp(buffer, "RIFF", 4) != 0) {
		wxLogError(wxT("WAVFileReader: %s has no \"RIFF\" signature."), m_fileName.c_str());
		m_file->Close();
		delete m_file;
		m_file = NULL;
		return false;
	}

	n = m_file->Read(buffer, 4);
	if (n != 4U) {
		wxLogError(wxT("WAVFileReader: %s is corrupt, cannot read the file length."), m_fileName.c_str());
		m_file->Close();
		delete m_file;
		m_file = NULL;
		return false;
	}

	n = m_file->Read(buffer, 4);
	if (n != 4U || ::memcmp(buffer, "WAVE", 4) != 0) {
		wxLogError(wxT("WAVFileReader: %s has no \"WAVE\" header."), m_fileName.c_str());
		m_file->Close();
		delete m_file;
		m_file = NULL;
		return false;
	}

	n = m_file->Read(buffer, 4);
	if (n != 4U || ::memcmp(buffer, "fmt ", 4) != 0) {
		wxLogError(wxT("WAVFileReader: %s has no \"fmt \" chunk."), m_fileName.c_str());
		m_file->Close();
		delete m_file;
		m_file = NULL;
		return false;
	}

	wxUint32 uint32;
	n = m_file->Read(&uint32, sizeof(wxUint32));

	wxUint32 length = wxUINT32_SWAP_ON_BE(uint32);
	if (n != sizeof(wxUint32) || length < 16U) {
		wxLogError(wxT("WAVFileReader: %s is corrupt, cannot read the WAVEFORMATEX structure length."), m_fileName.c_str());
		m_file->Close();
		delete m_file;
		m_file = NULL;
		return false;
	}

	wxUint16 uint16;
	n = m_file->Read(&uint16, sizeof(wxUint16));

	wxUint16 compCode = wxUINT16_SWAP_ON_BE(uint16);
	if (n != sizeof(wxUint16) || (compCode != FORMAT_PCM && compCode != FORMAT_IEEE_FLOAT)) {
		wxLogError(wxT("WAVFileReader: %s is not PCM or IEEE Float format, is %u."), m_fileName.c_str(), compCode);
		m_file->Close();
		delete m_file;
		m_file = NULL;
		return false;
	}

	n = m_file->Read(&uint16, sizeof(wxUint16));

	m_channels = wxUINT16_SWAP_ON_BE(uint16);
	if (n != sizeof(wxUint16) || m_channels > 2U) {
		wxLogError(wxT("WAVFileReader: %s has %u channels, more than 2."), m_fileName.c_str(), m_channels);
		m_file->Close();
		delete m_file;
		m_file = NULL;
		return false;
	}

	n = m_file->Read(&uint32, sizeof(wxUint32));

	m_sampleRate = wxUINT32_SWAP_ON_BE(uint32);
	if (n != sizeof(wxUint32)) {
		wxLogError(wxT("WAVFileReader: %s is corrupt, cannot read the sample rate."), m_fileName.c_str());
		m_file->Close();
		delete m_file;
		m_file = NULL;
		return false;
	}

	n = m_file->Read(&uint32, sizeof(wxUint32));

	if (n != sizeof(wxUint32)) {
		wxLogError(wxT("WAVFileReader: %s is corrupt, cannot read the average bytes per second"), m_fileName.c_str());
		m_file->Close();
		delete m_file;
		m_file = NULL;
		return false;
	}

	n = m_file->Read(&uint16, sizeof(wxUint16));

	if (n != sizeof(wxUint16)) {
		wxLogError(wxT("WAVFileReader: %s is corrupt, cannot read the block align."), m_fileName.c_str());
		m_file->Close();
		delete m_file;
		m_file = NULL;
		return false;
	}

	n = m_file->Read(&uint16, sizeof(wxUint16));

	if (n != sizeof(wxUint16)) {
		wxLogError(wxT("WAVFileReader: %s is corrupt, cannot read the bitsPerSample."), m_fileName.c_str());
		m_file->Close();
		delete m_file;
		m_file = NULL;
		return false;
	}

	wxUint16 bitsPerSample = wxUINT16_SWAP_ON_BE(uint16);

	if (bitsPerSample == 8U && compCode == FORMAT_PCM) {
		m_format = FORMAT_8BIT;
	} else if (bitsPerSample == 16U && compCode == FORMAT_PCM) {
		m_format = FORMAT_16BIT;
	} else if (bitsPerSample == 32U && compCode == FORMAT_IEEE_FLOAT) {
		m_format = FORMAT_32BIT;
	} else {
		wxLogError(wxT("WAVFileReader: %s has sample width %u and format %u."), m_fileName.c_str(), bitsPerSample, compCode);
		m_file->Close();
		delete m_file;
		m_file = NULL;
		return false;
	}

	// Now drain any extra bytes of data
	if (length > 16U)
		m_file->Seek(length - 16U, wxFromCurrent);

	n = m_file->Read(buffer, 4);

	if (n != 4U || ::memcmp(buffer, "data", 4) != 0) {
		wxLogError(wxT("WAVFileReader: %s has no \"data\" chunk."), m_fileName.c_str());
		m_file->Close();
		delete m_file;
		m_file = NULL;
		return false;
	}

	n = m_file->Read(&uint32, sizeof(wxUint32));

	if (n != sizeof(wxUint32)) {
		wxLogError(wxT("WAVFileReader: %s is corrupt, cannot read the \"data\" chunk size"), m_fileName.c_str());
		m_file->Close();
		delete m_file;
		m_file = NULL;
		return false;
	}

	m_length = wxUINT32_SWAP_ON_BE(uint32);

	// Get the current location so we can rewind if needed
	m_offset = m_file->Tell();

	return true;
}

unsigned int CWAVFileReader::read(wxFloat32* data, unsigned int length)
{
	wxASSERT(m_file != NULL);
	wxASSERT(data != NULL);

	if (length == 0U)
		return 0U;

	unsigned int i;
	size_t n = 0U;

	switch (m_format) {
		case FORMAT_8BIT:
			n = m_file->Read(m_buffer8, length * m_channels * sizeof(wxUint8));

			if (n == 0U)
				return 0U;

			n /= sizeof(wxUint8);

			for (i = 0U; i < n; i++)
				data[i] = (wxFloat32(m_buffer8[i]) - 127.0F) / 128.0F;

			n /= m_channels;
			break;

		case FORMAT_16BIT:
			n = m_file->Read(m_buffer16, length * m_channels * sizeof(wxInt16));

			if (n == 0U)
				return 0U;

			n /= sizeof(wxInt16);

			for (i = 0U; i < n; i++)
				data[i] = wxFloat32(m_buffer16[i]) / 32768.0F;

			n /= m_channels;
			break;

		case FORMAT_32BIT:
			n = m_file->Read(m_buffer32, length * m_channels * sizeof(wxFloat32));

			if (n == 0U)
				return 0U;

			n /= (sizeof(wxFloat32) * m_channels);

			switch (m_channels) {
				case 1U:
					for (i = 0U; i < n; i++)
						data[i] = m_buffer32[i];
					break;
				case 2U:
					// Swap I and Q for SDR-1000 data
					for (i = 0U; i < n; i++) {
						data[i * 2U + 0U] = m_buffer32[i * 2U + 1U];
						data[i * 2U + 1U] = m_buffer32[i * 2U + 0U];
					}
					break;
			}
			break;
	}

	return n;
}

void CWAVFileReader::rewind()
{
	wxASSERT(m_file != NULL);

	m_file->Seek(m_offset);
}

void CWAVFileReader::close()
{
	wxASSERT(m_file != NULL);

	m_file->Close();

	delete m_file;

	m_file = NULL;
}

unsigned int CWAVFileReader::getSampleRate() const
{
	return m_sampleRate;
}

unsigned int CWAVFileReader::getChannels() const
{
	return m_channels;
}

#endif
