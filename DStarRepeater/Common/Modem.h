/*
 *   Copyright (C) 2013,2014 by Jonathan Naylor G4KLX
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

#ifndef	Modem_H
#define	Modem_H

#include "HeaderData.h"
#include "RingBuffer.h"

#include <wx/wx.h>

enum DSMT_TYPE {
	DSMTT_NONE,
	DSMTT_START,
	DSMTT_HEADER,
	DSMTT_DATA,
	DSMTT_EOT,
	DSMTT_LOST
};

class CModem : public wxThread {
public:
	CModem();
	virtual ~CModem();

	virtual bool start() = 0;

	virtual bool writeHeader(const CHeaderData& header) = 0;
	virtual bool writeData(const unsigned char* data, unsigned int length, bool end) = 0;

	virtual unsigned int getSpace() = 0;
	virtual bool isTXReady() = 0;
	virtual bool isTX();

	virtual DSMT_TYPE read();
	virtual CHeaderData* readHeader();
	virtual unsigned int readData(unsigned char* data, unsigned int length);

	virtual void stop();

protected:
	CRingBuffer<unsigned char> m_rxData;
	wxMutex                    m_mutex;
	bool                       m_tx;
	bool                       m_stopped;

private:
	DSMT_TYPE                  m_readType;
	unsigned int               m_readLength;
	unsigned char*             m_readBuffer;
};

#endif
