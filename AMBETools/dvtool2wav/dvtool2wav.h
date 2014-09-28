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

#if !defined(DVTOOL2WAV_H)
#define	DVTOOL2WAV

#include "DVTOOLFileReader.h"
#include "DecodeCallback.h"
#include "WAVFileWriter.h"
#include "DongleThread.h"

#include <wx/wx.h>

class dvtool2wav : public IDecodeCallback {
public:
	dvtool2wav(CDongleThread* dongle, CDVTOOLFileReader* reader, CWAVFileWriter* writer);
	virtual ~dvtool2wav();

	virtual void decodeCallback(const wxFloat32* audio, unsigned int length);

	virtual void run();

private:
	CDongleThread*     m_dongle;
	CDVTOOLFileReader* m_reader;
	CWAVFileWriter*    m_writer;
};

#endif
