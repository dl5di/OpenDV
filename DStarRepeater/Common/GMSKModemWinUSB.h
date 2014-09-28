/*
 *   Copyright (C) 2010-2014 by Jonathan Naylor G4KLX
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

#ifndef	GMSKModemWinUSB_H
#define	GMSKModemWinUSB_H

#include "GMSKModem.h"
#include "Utils.h"

#include <wx/wx.h>

#include "winusb.h"

class CGMSKModemWinUSB : public IGMSKModem {
public:
	CGMSKModemWinUSB(unsigned int address);
	virtual ~CGMSKModemWinUSB();

	virtual bool open();

	virtual bool readHeader(unsigned char* header, unsigned int length);
	virtual int  readData(unsigned char* data, unsigned int length, bool& end);

	virtual TRISTATE getPTT();
	virtual void     setPTT(bool on);

	virtual TRISTATE hasSpace();

	virtual void writeHeader(unsigned char* data, unsigned int length);
	virtual int  writeData(unsigned char* data, unsigned int length);

	virtual void close();

private:
	unsigned int            m_address;
	HANDLE                  m_file;
	WINUSB_INTERFACE_HANDLE m_handle;
	bool                    m_brokenSpace;

	bool openModem();
	int  io(unsigned char type, unsigned char n1, unsigned char n2, unsigned char* buffer, unsigned int length);
};

#endif

