/*
 *   Copyright (C) 2015 by Jonathan Naylor G4KLX
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

#if !defined(DMRSlot_H)
#define	DMRSlot_H

#include "HomebrewDMRIPSC.h"
#include "EmbeddedLC.h"
#include "RingBuffer.h"
#include "DMRSlot.h"
#include "DMRData.h"
#include "Timer.h"
#include "Modem.h"
#include "LC.h"

enum SLOT_STATE {
	SS_LISTENING,
	SS_LATE_ENTRY,
	SS_RELAYING_RF,
	SS_RELAYING_NETWORK
};

class CDMRSlot {
public:
	CDMRSlot(unsigned int slotNo);
	~CDMRSlot();

	void writeRF(unsigned char* data);

	unsigned int readRF(unsigned char* data);

	void writeNet(const CDMRData& data);

	bool readNet(CDMRData& data);

	void clock(unsigned int ms);

	static void init(unsigned int colorCode, CModem* modem, CHomebrewDMRIPSC* network);

private:
	unsigned int                 m_slotNo;
	CRingBuffer<unsigned char>** m_txQueue;
	SLOT_STATE                   m_state;
	CEmbeddedLC                  m_embeddedLC;
	CLC*                         m_lc;
	unsigned char                m_n;
	CTimer**                     m_playoutTimer;
	CTimer                       m_networkWatchdog;
	CTimer                       m_lateEntryWatchdog;
	unsigned int                 m_writeQueue;
	unsigned int                 m_readQueue;

	static unsigned int        m_colorCode;
	static CModem*             m_modem;
	static CHomebrewDMRIPSC*   m_network;

	static FLCO                m_flco1;
	static unsigned char       m_id1;
	static FLCO                m_flco2;
	static unsigned char       m_id2;

	void writeHeader(const CDMRData& data);
	void writeQueue(const unsigned char* data);
	void writeNetwork(const unsigned char* data, DMR_DATA_TYPE dataType);

	static void setShortLC(unsigned int slotNo, unsigned int id, FLCO flco = FLCO_GROUP);
};

#endif
