/*
 *   Copyright (C) 2012-2015 by Jonathan Naylor G4KLX
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

#ifndef	SplitController_H
#define	SplitController_H

#include "GatewayProtocolHandler.h"
#include "DStarDefines.h"
#include "RingBuffer.h"
#include "Timer.h"
#include "Modem.h"
#include "Utils.h"
#include "Types.h"

#include <string>
#include <vector>

class CAMBESlot {
public:
	CAMBESlot(unsigned int rxCount);
	~CAMBESlot();

	void reset();

	bool isFirst() const;

	bool*          m_valid;
	unsigned int   m_errors;
	unsigned int   m_best;
	unsigned char* m_ambe;
	unsigned int   m_length;
	bool*          m_end;
	CTimer         m_timer;

private:
	unsigned int   m_rxCount;
};

class CSplitController : public CModem {
public:
	CSplitController(const std::string& localAddress, unsigned int localPort, const std::vector<std::string>& transmitterNames, const std::vector<std::string>& receiverNames, unsigned int timeout);
	virtual ~CSplitController();

	virtual void entry();

	virtual bool start();

	virtual unsigned int getSpace();
	virtual bool isTXReady();

	virtual bool writeHeader(const CHeaderData& header);
	virtual bool writeData(const unsigned char* data, unsigned int length, bool end);

private:
	CGatewayProtocolHandler    m_handler;
	std::vector<std::string>   m_transmitterNames;
	std::vector<std::string>   m_receiverNames;
	unsigned int               m_timeout;
	unsigned int               m_txCount;
	unsigned int               m_rxCount;
	in_addr*                   m_txAddresses;
	unsigned int*              m_txPorts;
	CTimer**                   m_txTimers;
	in_addr*                   m_rxAddresses;
	unsigned int*              m_rxPorts;
	CTimer**                   m_rxTimers;
	CRingBuffer<unsigned char> m_txData;
	uint16_t                   m_outId;
	uint8_t                    m_outSeq;
	CTimer                     m_endTimer;
	bool                       m_listening;
	uint8_t                    m_inSeqNo;
	uint8_t                    m_outSeqNo;
	unsigned char*             m_header;
	uint16_t*                  m_id;
	bool*                      m_valid;
	CAMBESlot**                m_slots;
	bool                       m_headerSent;
	unsigned int*              m_packets;
	unsigned int*              m_best;
	unsigned int*              m_missed;
	unsigned int               m_silence;

	void transmit();
	void receive();
	void timers(unsigned int ms);
	void processHeader(unsigned int n, uint16_t id, const unsigned char* header, unsigned int length);
	void processAMBE(unsigned int n, uint16_t id, const unsigned char* ambe, unsigned int length, uint8_t seqNo, unsigned char errors);
	bool isEnd(const CAMBESlot& slot) const;
	void sendHeader();
	void printStats() const;
};

#endif
