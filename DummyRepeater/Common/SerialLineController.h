/*
 *   Copyright (C) 2002-2004,2007-2009,2011-2013 by Jonathan Naylor G4KLX
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

#ifndef SerialLineController_H
#define SerialLineController_H

#include "HardwareController.h"

#include <wx/wx.h>

#if defined(__WINDOWS__)
#include <windows.h>
#endif

enum SERIALPIN {
	SERIAL_CD,
	SERIAL_CTS,
	SERIAL_DSR,
	SERIAL_DTR,
	SERIAL_RTS,
	SERIAL_ECHOLINK
};

const unsigned int MAX_DEVICE_NAME = 255U;

class CSerialLineController : public IHardwareController {
public:
	CSerialLineController(const wxString& device, unsigned int config = 1U);
	virtual ~CSerialLineController();

	virtual bool open();

	virtual bool setRTS(bool set);
	virtual bool setDTR(bool set);

	virtual bool getCD() const;
	virtual bool getCTS() const;
	virtual bool getDSR() const;

	virtual void getDigitalInputs(bool& inp1, bool& inp2, bool& inp3, bool& inp4, bool& inp5);
	virtual void setDigitalOutputs(bool outp1, bool outp2, bool outp3, bool outp4, bool outp5, bool outp6, bool outp7, bool outp8);

	virtual void close();

	static wxArrayString getDevices();

private:
	wxString     m_device;
	unsigned int m_config;
	bool         m_rts;
	bool         m_dtr;
#if defined(__WINDOWS__)
	HANDLE       m_handle;
#else
	int          m_fd;
#endif
};

#endif
