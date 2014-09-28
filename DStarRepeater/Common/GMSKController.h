/*
 *   Copyright (C) 2011-2014 by Jonathan Naylor G4KLX
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

#ifndef	GMSKController_H
#define	GMSKController_H

#include "DStarDefines.h"
#include "RingBuffer.h"
#include "GMSKModem.h"
#include "Modem.h"
#include "Utils.h"

#include <wx/wx.h>

class CGMSKController : public CModem {
public:
	CGMSKController(USB_INTERFACE iface, unsigned int address, bool duplex);
	virtual ~CGMSKController();

	virtual void* Entry();

	virtual bool start();

	virtual unsigned int getSpace();
	virtual bool isTXReady();

	virtual bool writeHeader(const CHeaderData& header);
	virtual bool writeData(const unsigned char* data, unsigned int length, bool end);

private:
	IGMSKModem*                m_modem;
	bool                       m_duplex;
	unsigned char*             m_buffer;
	CRingBuffer<unsigned char> m_txData;

	bool reopenModem();
};

#endif
