/*
 *   Copyright (C) 2014,2015 by Jonathan Naylor G4KLX
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

#ifndef	DV3000NetworkController_H
#define	DV3000NetworkController_H

#include "DV3000Controller.h"
#include "UDPReaderWriter.h"

#include <wx/wx.h>

class CDV3000NetworkController : public IDV3000Controller {
public:
	CDV3000NetworkController(const wxString& address, unsigned int port);
	virtual ~CDV3000NetworkController();

	virtual bool open();

	virtual void encodeIn(const wxFloat32* audio, unsigned int length);
	virtual bool encodeOut(unsigned char* ambe, unsigned int length);

	virtual void decodeIn(const unsigned char* ambe, unsigned int length);
	virtual bool decodeOut(wxFloat32* audio, unsigned int length);

	virtual void close();

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
