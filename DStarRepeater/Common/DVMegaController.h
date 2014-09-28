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

#ifndef	DVMegaController_H
#define	DVMegaController_H

#include "SerialDataController.h"
#include "RingBuffer.h"
#include "Modem.h"
#include "Utils.h"

#include <wx/wx.h>

enum RESP_TYPE_MEGA {
	RTM_TIMEOUT,
	RTM_ERROR,
	RTM_UNKNOWN,
	RTM_GET_STATUS,
	RTM_GET_VERSION,
	RTM_GET_SERIAL,
	RTM_GET_CONFIG,
	RTM_SET_CONFIG,
	RTM_RXPREAMBLE,
	RTM_START,
	RTM_HEADER,
	RTM_RXSYNC,
	RTM_DATA,
	RTM_EOT,
	RTM_RXLOST,
	RTM_SET_TESTMDE
};

class CDVMegaController : public CModem {
public:
	CDVMegaController(const wxString& port, const wxString& path, bool rxInvert, bool txInvert, unsigned int txDelay);
	CDVMegaController(const wxString& port, const wxString& path, unsigned int txDelay, unsigned int frequency, unsigned int power);
	virtual ~CDVMegaController();

	virtual void* Entry();

	virtual bool start();

	virtual unsigned int getSpace();
	virtual bool isTXReady();

	virtual bool writeHeader(const CHeaderData& header);
	virtual bool writeData(const unsigned char* data, unsigned int length, bool end);

	virtual wxString getPath() const;

private:
	wxString                   m_port;
	wxString                   m_path;
	bool                       m_rxInvert;
	bool                       m_txInvert;
	unsigned int               m_txDelay;
	unsigned int               m_frequency;
	unsigned int               m_power;
	CSerialDataController      m_serial;
	unsigned char*             m_buffer;
	CRingBuffer<unsigned char> m_txData;
	unsigned char              m_txCounter;
	unsigned char              m_pktCounter;
	bool                       m_rx;
	unsigned int               m_txSpace;
	bool                       m_txEnabled;
	bool                       m_checksum;

	bool readVersion();
	bool readStatus();
	bool setConfig();
	bool setFrequencyAndPower();
	bool setEnabled(bool enable);

	RESP_TYPE_MEGA getResponse(unsigned char* buffer, unsigned int& length);

	bool findPort();
	bool findPath();

	bool findModem();
	bool openModem();
};

#endif

