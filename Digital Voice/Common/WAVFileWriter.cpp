/*
 *   Copyright (C) 2002-2004,2006-2009 by Jonathan Naylor G4KLX
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

#include "WAVFileWriter.h"

#if defined(__WINDOWS__)

const int WAVE_FORMAT_IEEE_FLOAT = 3;

CWAVFileWriter::CWAVFileWriter(const wxString& fileName, unsigned int sampleRate, unsigned int channels, unsigned int sampleWidth, unsigned int blockSize) :
m_fileName(fileName),
m_sampleRate(sampleRate),
m_channels(channels),
m_sampleWidth(sampleWidth),
m_blockSize(blockSize),
m_buffer8(NULL),
m_buffer16(NULL),
m_buffer32(NULL),
m_handle(NULL),
m_parent(),
m_child()
{
	wxASSERT(sampleRate > 0U);
	wxASSERT(channels == 1U || channels == 2U);
	wxASSERT(sampleWidth == 8U || sampleWidth == 16U || sampleWidth == 32U);
	wxASSERT(blockSize > 0U);

	m_buffer8  = new wxUint8[blockSize * channels];
	m_buffer16 = new wxInt16[blockSize * channels];
	m_buffer32 = new wxFloat32[blockSize * channels];
}

CWAVFileWriter::~CWAVFileWriter()
{
	delete[] m_buffer8;
	delete[] m_buffer16;
	delete[] m_buffer32;
}

bool CWAVFileWriter::open()
{
	m_handle = ::mmioOpen(LPWSTR(m_fileName.c_str()), 0, MMIO_WRITE | MMIO_CREATE | MMIO_ALLOCBUF);
	if (m_handle == NULL) {
		wxLogError(wxT("WAVFileWriter: could not open the file %s\n"), m_fileName.c_str());
		return false;
	}

	m_parent.fccType = mmioFOURCC('W', 'A', 'V', 'E');
	m_parent.cksize  = 0;

	MMRESULT res = ::mmioCreateChunk(m_handle, &m_parent, MMIO_CREATERIFF);
	if (res != MMSYSERR_NOERROR) {
		wxLogError(wxT("WAVFileWriter: could not write to file %s\n"), m_fileName.c_str());
		return false;
	}

	m_child.ckid   = mmioFOURCC('f', 'm', 't', ' ');
	m_child.cksize = sizeof(WAVEFORMATEX);

	res = ::mmioCreateChunk(m_handle, &m_child, 0);
	if (res != MMSYSERR_NOERROR) {
		wxLogError(wxT("WAVFileWriter: could not write to the file %s\n"), m_fileName.c_str());
		return false;
	}

	WAVEFORMATEX format;
	format.wBitsPerSample  = m_sampleWidth;
	if (m_sampleWidth == 8U || m_sampleWidth == 16U)
		format.wFormatTag = WAVE_FORMAT_PCM;
	else
		format.wFormatTag = WAVE_FORMAT_IEEE_FLOAT;
	format.nChannels       = m_channels;
	format.nSamplesPerSec  = m_sampleRate;
	format.nAvgBytesPerSec = m_sampleRate * m_channels * m_sampleWidth / 8;
	format.nBlockAlign     = m_channels * m_sampleWidth / 8;
	format.cbSize          = 0;

	LONG n = ::mmioWrite(m_handle, (CHAR *)&format, sizeof(WAVEFORMATEX));
	if (n != sizeof(WAVEFORMATEX)) {
		wxLogError(wxT("WAVFileWriter: could not write to the file %s\n"), m_fileName.c_str());
		return false;
	}

	::mmioAscend(m_handle, &m_child, 0);

	m_child.ckid   = mmioFOURCC('d', 'a', 't', 'a');
	m_child.cksize = 0;

	res = ::mmioCreateChunk(m_handle, &m_child, 0);
	if (res != MMSYSERR_NOERROR) {
		wxLogError(wxT("WAVFileWriter: could not write to the file %s\n"), m_fileName.c_str());
		return false;
	}

	return true;
}

bool CWAVFileWriter::write(const float* buffer, unsigned int length)
{
	wxASSERT(m_handle != NULL);
	wxASSERT(buffer != NULL);
	wxASSERT(length > 0U);

	unsigned int i;
	LONG bytes = 0L;
	LONG n = 0L;

	switch (m_sampleWidth) {
		case 8U:
			switch (m_channels) {
				case 1U:
					for (i = 0U; i < length; i++)
						m_buffer8[i] = wxUint8(buffer[i] * 128.0F + 127.0F);
					break;
				case 2U:
					for (i = 0U; i < (length * 2U); i++)
						m_buffer8[i] = wxUint8(buffer[i] * 128.0F + 127.0F);
					break;
			}

			bytes = length * m_channels * sizeof(wxUint8);

			n = ::mmioWrite(m_handle, (char *)m_buffer8, bytes);

			break;

		case 16U:
			switch (m_channels) {
				case 1U:
					for (i = 0U; i < length; i++)
						m_buffer16[i] = wxInt16(buffer[i] * 32768.0F);
					break;
				case 2U:
					for (i = 0U; i < (length * 2U); i++)
						m_buffer16[i] = wxInt16(buffer[i] * 32768.0F);
					break;
			}

			bytes = length * m_channels * sizeof(wxInt16);

			n = ::mmioWrite(m_handle, (char *)m_buffer16, bytes);

			break;

		case 32U:
			switch (m_channels) {
				case 1U:
					for (i = 0U; i < length; i++)
						m_buffer32[i] = wxFloat32(buffer[i]);
					break;
				case 2U:
					// Swap I and Q
					for (i = 0U; i < length; i++) {
						m_buffer32[i * 2U + 0U] = wxFloat32(buffer[i * 2U + 1U]);
						m_buffer32[i * 2U + 1U] = wxFloat32(buffer[i * 2U + 0U]);
					}
					break;
			}

			bytes = length * m_channels * sizeof(wxFloat32);

			n = ::mmioWrite(m_handle, (char *)m_buffer32, bytes);

			break;
	}

	return n == bytes;
}

void CWAVFileWriter::close()
{
	wxASSERT(m_handle != NULL);

	::mmioAscend(m_handle, &m_child, 0);
	::mmioAscend(m_handle, &m_parent, 0);

	::mmioClose(m_handle, 0);
	m_handle = NULL;
}

unsigned int CWAVFileWriter::getSampleRate() const
{
	return m_sampleRate;
}

unsigned int CWAVFileWriter::getChannels() const
{
	return m_channels;
}

#else

CWAVFileWriter::CWAVFileWriter(const wxString& fileName, unsigned int sampleRate, unsigned int channels, unsigned int sampleWidth, unsigned int blockSize) :
m_fileName(fileName),
m_sampleRate(sampleRate),
m_channels(channels),
m_sampleWidth(sampleWidth),
m_blockSize(blockSize),
m_buffer8(NULL),
m_buffer16(NULL),
m_buffer32(NULL),
m_file(NULL),
m_offset1(0),
m_offset2(0),
m_length(0U)
{
	wxASSERT(sampleRate > 0U);
	wxASSERT(channels == 1U || channels == 2U);
	wxASSERT(sampleWidth == 8U || sampleWidth == 16U || sampleWidth == 32U);
	wxASSERT(blockSize > 0U);

	m_buffer8  = new wxUint8[channels * blockSize];
	m_buffer16 = new wxInt16[channels * blockSize];
	m_buffer32 = new wxFloat32[channels * blockSize];
}

CWAVFileWriter::~CWAVFileWriter()
{
	delete[] m_buffer8;
	delete[] m_buffer16;
	delete[] m_buffer32;
}

bool CWAVFileWriter::open()
{
	m_length = 0U ;

	m_file = new wxFFile(m_fileName.c_str(), wxT("wb"));

	bool ret = m_file->IsOpened();
	if (!ret) {
		wxLogError(wxT("WAVFileWriter: could not open the file %s in WAVFileWriter"), m_fileName.c_str());
		delete m_file;
		m_file = NULL;
		return false;
	}

	m_file->Write("RIFF", 4);                       // 4 bytes, file signature

	m_offset1 = m_file->Tell();

	wxUint32 uint32 = 0;
	m_file->Write(&uint32, sizeof(wxUint32));       // 4 bytes, length of file, filled in later

	m_file->Write("WAVE", 4);                       // 4 bytes, RIFF file type

	m_file->Write("fmt ", 4);                       // 4 bytes, chunk signature

	uint32 = wxUINT32_SWAP_ON_BE(wxUint32(16));
	m_file->Write(&uint32, sizeof(wxUint32));       // 4 bytes, length of "fmt " chunk

	wxUint16 uint16;
	if (m_sampleWidth == 8U || m_sampleWidth == 16U)
		uint16 = wxUINT16_SWAP_ON_BE(wxUint16(1));      // 2 bytes, integer PCM/uncompressed
	else
		uint16 = wxUINT16_SWAP_ON_BE(wxUint16(3));      // 2 bytes, float PCM/uncompressed
	m_file->Write(&uint16, sizeof(uint16));

	uint16 = wxUINT16_SWAP_ON_BE(wxUint16(m_channels));
	m_file->Write(&uint16, sizeof(uint16));         // 2 bytes, no of channels
        
	uint32 = wxUINT32_SWAP_ON_BE(wxUint32(m_sampleRate));
	m_file->Write(&uint32, sizeof(wxUint32));       // 4 bytes, sample rate

	uint32 = wxUINT32_SWAP_ON_BE(wxUint32(m_sampleRate * m_channels * m_sampleWidth / 8U));
	m_file->Write(&uint32, sizeof(wxUint32));       // 4 bytes, average bytes per second

	uint16 = wxUINT16_SWAP_ON_BE(wxUint16(m_channels * m_sampleWidth / 8U));
	m_file->Write(&uint16, sizeof(uint16));         // 2 bytes, block alignment

	uint16 = wxUINT16_SWAP_ON_BE(wxUint16(m_sampleWidth));
	m_file->Write(&uint16, sizeof(uint16));         // 2 bytes, significant bits per sample

	m_file->Write("data", 4);                       // 4 bytes, chunk signature

	m_offset2 = m_file->Tell();

	uint32 = 0U;
	m_file->Write(&uint32, sizeof(wxUint32));       // 4 bytes, length of "data" chunk, filled in later

	return true;
}

bool CWAVFileWriter::write(const wxFloat32* buffer, unsigned int length)
{
	wxASSERT(m_file != NULL);
	wxASSERT(buffer != NULL);
	wxASSERT(length > 0U && length <= m_blockSize);

	unsigned int bytes = 0U;
	unsigned int i;
	size_t n = 0UL;

	switch (m_sampleWidth) {
		case 8U:
			switch (m_channels) {
				case 1U:
					for (i = 0U; i < length; i++)
						m_buffer8[i] = wxUint8(buffer[i] * 128.0F + 127.0F);
					break;
				case 2U:
					for (i = 0U; i < (length * 2U); i++)
						m_buffer8[i] = wxUint8(buffer[i] * 128.0F + 127.0F);
					break;
			}

			bytes = length * m_channels * sizeof(wxUint8);

			n = m_file->Write(m_buffer8, bytes);

			break;

		case 16U:
			switch (m_channels) {
				case 1U:
					for (i = 0U; i < length; i++)
						m_buffer16[i] = wxInt16(buffer[i] * 32768.0F);
					break;
				case 2U:
					for (i = 0U; i < (length * 2U); i++)
						m_buffer16[i] = wxInt16(buffer[i] * 32768.0F);
					break;
			}

			bytes = length * m_channels * sizeof(wxInt16);

			n = m_file->Write(m_buffer16, bytes);

			break;

		case 32U:
			switch (m_channels) {
				case 1U:
					for (i = 0U; i < length; i++)
						m_buffer32[i] = wxFloat32(buffer[i]);
					break;
				case 2U:
					// Swap I and Q
					for (i = 0U; i < length; i++) {
						m_buffer32[i * 2U + 0U] = wxFloat32(buffer[i * 2U + 1U]);
						m_buffer32[i * 2U + 1U] = wxFloat32(buffer[i * 2U + 0U]);
					}
					break;
			}

			bytes = length * m_channels * sizeof(wxFloat32);

			n = m_file->Write(m_buffer32, bytes);

			break;
	}

	m_length += n;

	return n == bytes;
}

void CWAVFileWriter::close()
{
	wxASSERT(m_file != NULL);

	if ((m_length % 2U) != 0U) {
		unsigned char c = 0U;
		m_file->Write(&c, 1);
	}

	wxUint32 length = wxUINT32_SWAP_ON_BE(m_length);

	m_file->Seek(m_offset2);
	m_file->Write(&length, sizeof(wxUint32));

	length = wxUINT32_SWAP_ON_BE(m_length + 36U);

	m_file->Seek(m_offset1);
	m_file->Write(&length, sizeof(wxUint32));

	m_file->Close();
	delete m_file;
	m_file = NULL;
}

#endif
