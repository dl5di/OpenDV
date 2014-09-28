/*
 *   Copyright (C) 2014 by Jonathan Naylor G4KLX
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

#if !defined(WAV2AMBE_H)
#define	WAV2AMBE

#include "EncodeCallback.h"
#include "AMBEFileWriter.h"
#include "WAVFileReader.h"
#include "DongleThread.h"

#include <wx/wx.h>

class wav2ambe : public IEncodeCallback {
public:
	wav2ambe(CDongleThread* dongle, CWAVFileReader* reader, CAMBEFileWriter* writer);
	virtual ~wav2ambe();

	virtual void encodeCallback(const unsigned char* ambe, unsigned int length);

	virtual void run();

private:
	CDongleThread*   m_dongle;
	CWAVFileReader*  m_reader;
	CAMBEFileWriter* m_writer;
};

#endif
