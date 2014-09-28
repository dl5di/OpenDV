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

#ifndef	DV3000Controller_H
#define	DV3000Controller_H

#include "UDPReaderWriter.h"

#include <wx/wx.h>

class CDV3000Controller {
public:
	CDV3000Controller(const wxString& address, unsigned int port);
	~CDV3000Controller();

	bool open();

	void encodeIn(const wxFloat32* audio, unsigned int length);
	bool encodeOut(unsigned char* ambe, unsigned int length);

	void decodeIn(const unsigned char* ambe, unsigned int length);
	bool decodeOut(wxFloat32* audio, unsigned int length);

	void close();

private:
	CUDPReaderWriter m_socket;

	enum RESP_TYPE {
		RESP_NONE,
		RESP_ERROR,
		RESP_RATEP,
		RESP_NAME,
		RESP_AMBE,
		RESP_AUDIO,
		RESP_UNKNOWN
	};

	RESP_TYPE getResponse(unsigned char* buffer, unsigned int length);
};

#endif
