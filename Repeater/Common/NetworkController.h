/*
 *	Copyright (C) 2009,2013 by Jonathan Naylor, G4KLX
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

#ifndef	NetworkController_H
#define	NetworkController_H

#include "SerialLineController.h"

#include <wx/wx.h>

class CNetworkController {
public:
	CNetworkController(const wxString& device, SERIALPIN txPin, SERIALPIN rxPin);
	~CNetworkController();

	bool open();

	bool getSquelch() const;

	void setTransmit(bool value);

	void close();

private:
	CSerialLineController* m_serial;
	SERIALPIN              m_txPin;
	SERIALPIN              m_rxPin;
};

#endif
