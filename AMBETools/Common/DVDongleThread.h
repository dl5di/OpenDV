/*
 *   Copyright (C) 2010,2011,2012,2014 by Jonathan Naylor G4KLX
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
#include "DongleThread.h"

#include <wx/wx.h>

class CDVDongleThread : public CDongleThread {
public:
	CDVDongleThread(CDVDongleController* dongle);
	virtual ~CDVDongleThread();

	virtual bool open();

	virtual void setEncode();
	virtual void setDecode();
	virtual void setIdle();

	virtual void* Entry();

private:
	enum DVDMODE {
		DM_IDLE,
		DM_DECODE,
		DM_ENCODE
	};

	CDVDongleController* m_dongle;
	DVDMODE              m_wantMode;
	DVDMODE              m_mode;

	void processDecode();
	void processEncode();
	void processIdle();
};

#endif
