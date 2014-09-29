/*
 *   Copyright (C) 2010,2011,2012 by Jonathan Naylor G4KLX
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

#ifndef	IcomRepeaterProtocolHandler_H
#define	IcomRepeaterProtocolHandler_H

#include "RepeaterProtocolHandler.h"
#include "UDPReaderWriter.h"
#include "DStarDefines.h"
#include "RingBuffer.h"
#include "HeaderData.h"
#include "StatusData.h"
#include "HeardData.h"
#include "AMBEData.h"
#include "TextData.h"
#include "PollData.h"
#include "DDData.h"
#include "Timer.h"

#if defined(__WINDOWS__)
#include "Inaddr.h"
#else
#include <netinet/in.h>
#endif

#include <wx/wx.h>

class CDataQueue {
public:
	CDataQueue(CHeaderData* header) :
	m_type(RT_HEADER),
	m_header(header),
	m_heard(NULL),
	m_ambe(NULL),
	m_dd(NULL)
	{
	}

	CDataQueue(CHeardData* heard) :
	m_type(RT_HEARD),
	m_header(NULL),
	m_heard(heard),
	m_ambe(NULL),
	m_dd(NULL)
	{
	}

	CDataQueue(CAMBEData* data) :
	m_type(RT_AMBE),
	m_header(NULL),
	m_heard(NULL),
	m_ambe(data),
	m_dd(NULL)
	{
	}

	CDataQueue(CDDData* data) :
	m_type(RT_DD),
	m_header(NULL),
	m_heard(NULL),
	m_ambe(NULL),
	m_dd(data)
	{
	}

	CDataQueue() :
	m_type(RT_POLL),
	m_header(NULL),
	m_heard(NULL),
	m_ambe(NULL),
	m_dd(NULL)
	{
	}

	REPEATER_TYPE getType() const
	{
		return m_type;
	}

	CHeaderData* getHeader() const
	{
		return m_header;
	}

	CHeardData* getHeard() const
	{
		return m_heard;
	}

	CAMBEData* getAMBE() const
	{
		return m_ambe;
	}

	CDDData* getDD() const
	{
		return m_dd;
	}

private:
	REPEATER_TYPE m_type;
	CHeaderData*  m_header;
	CHeardData*   m_heard;
	CAMBEData*    m_ambe;
	CDDData*      m_dd;
};

class CIcomRepeaterProtocolHandler : public IRepeaterProtocolHandler, public wxThread {
public:
	CIcomRepeaterProtocolHandler(const wxString& address, unsigned int port, const wxString& icomAddress, unsigned int icomPort);
	virtual ~CIcomRepeaterProtocolHandler();

	virtual void setCount(unsigned int count);

	virtual bool open();

	virtual void* Entry();

	virtual bool writeHeader(CHeaderData& header);
	virtual bool writeAMBE(CAMBEData& data);
	virtual bool writeDD(CDDData& data);
	virtual bool writeText(CTextData& text);
	virtual bool writeStatus(CStatusData& status);

	virtual REPEATER_TYPE read();
	virtual CPollData*    readPoll();
	virtual CHeardData*   readHeard();
	virtual CHeaderData*  readHeader();
	virtual CAMBEData*    readAMBE();
	virtual CDDData*      readDD();
	virtual CHeaderData*  readBusyHeader();
	virtual CAMBEData*    readBusyAMBE();

	virtual void close();

private:
	CUDPReaderWriter         m_socket;
	in_addr                  m_icomAddress;
	unsigned int             m_icomPort;
	bool                     m_over1;
	wxUint16                 m_seqNo;
	unsigned int             m_tries;
	CDataQueue*              m_ackQueue;
	bool                     m_killed;
	REPEATER_TYPE            m_type;
	unsigned char*           m_buffer;
	CRingBuffer<CDataQueue*> m_rptrQueue;
	CRingBuffer<CDataQueue*> m_gwyQueue;
	CTimer                   m_retryTimer;

	void readIcomPackets();
	void sendGwyPackets();
	bool sendAck(wxUint16 seqNo);

	void sendSingleReply(const CHeaderData& header);
	void sendMultiReply(const CHeaderData& header);

	void free(CDataQueue* dataQueue);
};

#endif
