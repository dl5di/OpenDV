/*
 *	Copyright (C) 2013 by Jonathan Naylor, G4KLX
 *
 *	This program is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation; version 2 of the License.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 */

#ifndef	ArduinoController_H
#define	ArduinoController_H

#include "SerialDataController.h"
#include "HardwareController.h"

#include <wx/wx.h>


class CArduinoController : public IHardwareController {
public:
	CArduinoController(const wxString& port);
	virtual ~CArduinoController();

	virtual bool open();

	virtual void getDigitalInputs(bool& inp1, bool& inp2, bool& inp3, bool& inp4, bool& inp5);

	virtual void setDigitalOutputs(bool outp1, bool outp2, bool outp3, bool outp4, bool outp5, bool outp6, bool outp7, bool outp8);

	virtual void close();

private:
	CSerialDataController m_serial;
	unsigned char         m_out;
	unsigned char         m_in;
};

#endif
