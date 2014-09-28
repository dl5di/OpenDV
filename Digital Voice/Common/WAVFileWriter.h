/*
 *   Copyright (C) 2002-2004,2006-2010 by Jonathan Naylor G4KLX
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

#ifndef	WAVFileWriter_H
#define WAVFileWriter_H

#include <wx/wx.h>

#if defined(__WINDOWS__)
#include <windows.h>
#include <mmsystem.h>
#else
#include <wx/ffile.h>
#endif

class CWAVFileWriter {
public:
	CWAVFileWriter(const wxString& fileName, unsigned int sampleRate, unsigned int channels, unsigned int sampleWidth, unsigned int blockSize);
	~CWAVFileWriter();

	bool open();
	bool write(const wxFloat32* buffer, unsigned int length);
	void close();

	unsigned int getSampleRate() const;
	unsigned int getChannels() const;

private:
	wxString     m_fileName;
	unsigned int m_sampleRate;
	unsigned int m_channels;
	unsigned int m_sampleWidth;
	unsigned int m_blockSize;
	wxUint8*     m_buffer8;
	wxInt16*     m_buffer16;
	wxFloat32*   m_buffer32;
#if defined(__WINDOWS__)
	HMMIO        m_handle;
	MMCKINFO     m_parent;
	MMCKINFO     m_child;
#else
	wxFFile*     m_file;
	wxFileOffset m_offset1;
	wxFileOffset m_offset2;
	wxUint32     m_length;
#endif
};

#endif
