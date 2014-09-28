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

#if !defined(WAV2DVTOOL_H)
#define	WAV2DVTOOL

#include "DVTOOLFileWriter.h"
#include "SlowDataEncoder.h"
#include "EncodeCallback.h"
#include "WAVFileReader.h"
#include "DongleThread.h"

#include <wx/wx.h>

class wav2dvtool : public IEncodeCallback {
public:
	wav2dvtool(CDongleThread* dongle, CWAVFileReader* reader, CDVTOOLFileWriter* writer, CSlowDataEncoder* encoder);
	virtual ~wav2dvtool();

	virtual void encodeCallback(const unsigned char* ambe, unsigned int length);

	virtual void run();

private:
	CDongleThread*     m_dongle;
	CWAVFileReader*    m_reader;
	CDVTOOLFileWriter* m_writer;
	CSlowDataEncoder*  m_encoder;
	unsigned int       m_count;
};

#endif
