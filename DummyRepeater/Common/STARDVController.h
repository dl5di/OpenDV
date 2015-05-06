/*
 *   Copyright (C) 2015 by Rick Schnicker KD0OSS
 *   based on code by Jonathan Naylor G4KLX
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

#ifndef	STARDVController_H
#define	STARDVController_H

#include <wx/wx.h>

class ISTARDVController {
public:
	virtual ~ISTARDVController() = 0;

	virtual bool open() = 0;

	virtual void encodeIn(const wxFloat32* audio, unsigned int length) = 0;
	virtual bool encodeOut(unsigned char* ambe, unsigned int length, bool& tx) = 0;

	virtual void decodeIn(const unsigned char* ambe, unsigned int length) = 0;
	virtual bool decodeOut(wxFloat32* audio, unsigned int length) = 0;

	virtual void close() = 0;

private:
};

#endif
