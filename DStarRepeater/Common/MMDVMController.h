/*
 *   Copyright (C) 2011-2015 by Jonathan Naylor G4KLX
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

#ifndef	MMDVMController_H
#define	MMDVMController_H

#include "SerialDataController.h"
#include "RingBuffer.h"
#include "Modem.h"
#include "Utils.h"

#include <wx/wx.h>

enum RESP_TYPE_MMDVM {
	RTDVM_TIMEOUT,
	RTDVM_ERROR,
	RTDVM_UNKNOWN,
	RTDVM_GET_STATUS,
	RTDVM_GET_VERSION,
	RTDVM_DSTAR_HEADER,
	RTDVM_DSTAR_DATA,
	RTDVM_DSTAR_EOT,
	RTDVM_DSTAR_LOST,
	RTDVM_ACK,
	RTDVM_NAK
};

class CMMDVMController : public CModem {
public:
	CMMDVMController(const wxString& port, const wxString& path, bool rxInvert, bool txInvert, bool pttInvert, unsigned int txDelay, unsigned int rxLevel, unsigned int txLevel);
	virtual ~CMMDVMController();

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
	bool                       m_pttInvert;
	unsigned int               m_txDelay;
	unsigned int               m_rxLevel;
	unsigned int               m_txLevel;
	CSerialDataController      m_serial;
	unsigned char*             m_buffer;
	CRingBuffer<unsigned char> m_txData;
	bool                       m_rx;

	bool readVersion();
	bool readStatus();
	bool setConfig();

	RESP_TYPE_MMDVM getResponse(unsigned char* buffer, unsigned int& length);

	bool findPort();
	bool findPath();

	bool findModem();
	bool openModem();
};

#endif

