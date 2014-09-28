/*
 *   Copyright (C) 2002-2004,2007-2010,2012 by Jonathan Naylor G4KLX
 *   Copyright (C) 1999-2001 by Thomas Sailor HB9JNX
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

#ifndef SerialController_H
#define SerialController_H

#include "ExternalController.h"

#include <wx/wx.h>

#if defined(__WINDOWS__)
#include <windows.h>
#endif

#if !defined(SerialSpeed)
enum SERIAL_SPEED {
	SERIAL_1200   = 1200,
	SERIAL_2400   = 2400,
	SERIAL_4800   = 4800,
	SERIAL_9600   = 9600,
	SERIAL_19200  = 19200,
	SERIAL_38400  = 38400,
	SERIAL_76800  = 76800,
	SERIAL_230400 = 230400
};

#define	SerialSpeed
#endif

enum SERIAL_FORMAT {
	SERIAL_8N1
};

enum SERIAL_CONTROL {
	SERIAL_NOFC,
	SERIAL_HWFC
};

enum SERIALPIN {
	SERIAL_CD,
	SERIAL_CTS,
	SERIAL_DSR,
	SERIAL_DTR,
	SERIAL_RTS,
	SERIAL_ECHOLINK
};

const unsigned int MAX_DEVICE_NAME = 255U;

class CSerialController : public IExternalController {
public:
	CSerialController(const wxString& device, SERIAL_SPEED speed = SERIAL_9600, SERIAL_FORMAT format = SERIAL_8N1, SERIAL_CONTROL control = SERIAL_NOFC);
	virtual ~CSerialController();

	virtual bool open();

	virtual bool getSquelch();
	virtual void setTransmit(bool tx);

	virtual void close();

	static wxArrayString getDevices();

private:
	wxString       m_device;
	SERIAL_SPEED   m_speed;
	SERIAL_FORMAT  m_format;
	SERIAL_CONTROL m_control;
	bool           m_state;
#if defined(__WINDOWS__)
	HANDLE         m_handle;
#else
	int            m_fd;
#endif
};

#endif
