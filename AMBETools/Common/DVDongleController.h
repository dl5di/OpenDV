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

#ifndef	DVDongleController_H
#define	DVDongleController_H

#include "SerialDataController.h"

#include <wx/wx.h>

class CDVDongleController {
public:
	CDVDongleController(const wxString& device);
	~CDVDongleController();

	bool open();
	bool start();

	void encodeIn(const wxFloat32* audio, unsigned int length);
	bool encodeOut(unsigned char* ambe, unsigned int length);

	void decodeIn(const unsigned char* ambe, unsigned int length);
	bool decodeOut(wxFloat32* audio, unsigned int length);

	bool stop();
	void close();

private:
	CSerialDataController m_serial;

	enum RESP_TYPE {
		RESP_NONE,
		RESP_ERROR,
		RESP_START,
		RESP_NAME,
		RESP_STOP,
		RESP_AMBE,
		RESP_AUDIO,
		RESP_NOP,
		RESP_UNKNOWN
	};

	RESP_TYPE getResponse(unsigned char* buffer, unsigned int length);
	RESP_TYPE processResponse(unsigned char* buffer, unsigned int length);
};

#endif
