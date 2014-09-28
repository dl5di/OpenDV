/*
 *   Copyright (C) 2010,2011,2012 by Jonathan Naylor G4KLX
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

#ifndef	DVDongleThread_H
#define	DVDongleThread_H

#include "DVDongleController.h"
#include "RingBuffer.h"
#include "FIRFilter.h"

#include <wx/wx.h>

enum DVDMODE {
	DVDMODE_IDLE,
	DVDMODE_DECODE,
	DVDMODE_ENCODE
};

class CDVDongleThread : public wxThread {
public:
	CDVDongleThread(CDVDongleController* dongle, CRingBuffer<wxFloat32>* decodeAudio, CRingBuffer<unsigned char>* decodeData, CRingBuffer<wxFloat32>* encodeAudio = NULL, CRingBuffer<unsigned char>* encodeData = NULL);
	virtual ~CDVDongleThread();

	virtual void setMode(DVDMODE mode);
	virtual void setBleep(bool on);

	virtual void* Entry();

	virtual void  kill();

private:
	CDVDongleController*        m_dongle;
	CRingBuffer<wxFloat32>*     m_decodeAudio;
	CRingBuffer<unsigned char>* m_decodeData;
	CRingBuffer<wxFloat32>*     m_encodeAudio;
	CRingBuffer<unsigned char>* m_encodeData;
	DVDMODE                     m_wantMode;
	DVDMODE                     m_mode;
	bool                        m_killed;
	bool                        m_bleep;
	CFIRFilter                  m_filter;

	void upSample(const wxFloat32* audioIn, wxFloat32* audioOut);
	void downSample(const wxFloat32* audioIn, wxFloat32* audioOut);
	void processDecode();
	void processEncode();
	void processIdle();
	void sendBleep();
};

#endif
