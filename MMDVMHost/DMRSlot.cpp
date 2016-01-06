/*
 *	Copyright (C) 2015,2016 Jonathan Naylor, G4KLX
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

#include "SlotType.h"
#include "ShortLC.h"
#include "DMRSlot.h"
#include "Defines.h"
#include "DMRSync.h"
#include "FullLC.h"
#include "CSBK.h"
#include "Utils.h"
#include "EMB.h"
#include "CRC.h"
#include "Log.h"

#include <cassert>

unsigned int      CDMRSlot::m_colorCode = 0U;
CModem*           CDMRSlot::m_modem = NULL;
CHomebrewDMRIPSC* CDMRSlot::m_network = NULL;

FLCO              CDMRSlot::m_flco1;
unsigned char     CDMRSlot::m_id1 = 0U;
FLCO              CDMRSlot::m_flco2;
unsigned char     CDMRSlot::m_id2 = 0U;

const unsigned int NUM_QUEUES = 1U;

CDMRSlot::CDMRSlot(unsigned int slotNo) :
m_slotNo(slotNo),
m_txQueue(NULL),
m_state(SS_LISTENING),
m_embeddedLC(),
m_lc(NULL),
m_n(0U),
m_playoutTimer(NULL),
m_networkWatchdog(1000U, 2U),
m_lateEntryWatchdog(1000U, 1U),
m_writeQueue(0U),
m_readQueue(0U)
{
	m_txQueue      = new CRingBuffer<unsigned char>*[NUM_QUEUES];
	m_playoutTimer = new CTimer*[NUM_QUEUES];

	for (unsigned int i = 0U; i < NUM_QUEUES; i++) {
		m_txQueue[i] = new CRingBuffer<unsigned char>(1000U);
		m_playoutTimer[i] = new CTimer(1000U, 0U, 300U);
	}
}

CDMRSlot::~CDMRSlot()
{
	for (unsigned int i = 0U; i < NUM_QUEUES; i++) {
		delete m_txQueue[i];
		delete m_playoutTimer[i];
	}

	delete[] m_txQueue;
	delete[] m_playoutTimer;
}

void CDMRSlot::writeRF(unsigned char *data)
{
	if (data[0U] == TAG_LOST && m_state == SS_RELAYING_RF) {
		LogMessage("DMR Slot %u, transmission lost", m_slotNo);
		setShortLC(m_slotNo, 0U);
		m_state = SS_LISTENING;
		return;
	}

	if (data[0U] == TAG_LOST && m_state == SS_LATE_ENTRY) {
		m_lateEntryWatchdog.stop();
		m_state = SS_LISTENING;
		return;
	}

	if (m_state == SS_RELAYING_NETWORK)
		return;

	bool dataSync  = (data[1U] & DMR_SYNC_DATA)  == DMR_SYNC_DATA;
	bool audioSync = (data[1U] & DMR_SYNC_AUDIO) == DMR_SYNC_AUDIO;

	if (dataSync) {
		CSlotType slotType;
		slotType.putData(data + 2U);

		unsigned char colorCode = slotType.getColorCode();
		unsigned char dataType  = slotType.getDataType();

		if (colorCode != m_colorCode)
			return;

		if (dataType == DT_VOICE_LC_HEADER) {
			CFullLC fullLC;
			CLC* lc = fullLC.decode(data + 2U, DT_VOICE_LC_HEADER);
			if (lc == NULL)
				return;

			delete m_lc;
			m_lc = lc;

			// Regenerate the LC
			fullLC.encode(*m_lc, data + 2U, DT_VOICE_LC_HEADER);

			// Regenerate the Slot Type
			slotType.getData(data + 2U);

			// Convert the Data Sync to be from the BS
			CDMRSync sync;
			sync.addSync(data + 2U, DST_BS_DATA);

			data[0U] = TAG_DATA;
			data[1U] = 0x00U;
			m_n = 0U;

			m_writeQueue = (m_writeQueue + 1U) % 2U;
			m_playoutTimer[m_writeQueue]->start();

			writeNetwork(data, DMRDT_VOICE_LC_HEADER);
			writeQueue(data);

			m_state = SS_RELAYING_RF;
			setShortLC(m_slotNo, m_lc->getDstId(), m_lc->getFLCO());

			LogMessage("DMR Slot %u, received RF header from %u to %s:%u", m_slotNo, m_lc->getSrcId(), m_lc->getFLCO() == FLCO_GROUP ? "Group" : "User", m_lc->getDstId());
		} else if (dataType == DT_VOICE_PI_HEADER) {
			if (m_state == SS_RELAYING_RF) {
				// Regenerate the Slot Type
				slotType.getData(data + 2U);

				// Convert the Data Sync to be from the BS
				CDMRSync sync;
				sync.addSync(data + 2U, DST_BS_DATA);

				data[0U] = TAG_DATA;
				data[1U] = 0x00U;
				m_n = 0U;

				writeNetwork(data, DMRDT_VOICE_PI_HEADER);
				writeQueue(data);

				LogMessage("DMR Slot %u, received PI header", m_slotNo);
			} else {
				// Should save the PI header for after we have a valid LC
			}
		} else {
			// Ignore wakeup CSBKs
			if (dataType == DT_CSBK) {
				CCSBK csbk(data + 2U);
				CSBKO csbko = csbk.getCSBKO();
				if (csbko == CSBKO_BSDWNACT)
					return;
			}

			if (m_state == SS_RELAYING_RF) {
				unsigned char end[DMR_FRAME_LENGTH_BYTES + 2U];

				// Generate the LC
				CFullLC fullLC;
				fullLC.encode(*m_lc, end + 2U, DT_TERMINATOR_WITH_LC);

				// Generate the Slot Type
				CSlotType slotType;
				slotType.setColorCode(m_colorCode);
				slotType.setDataType(DT_TERMINATOR_WITH_LC);
				slotType.getData(end + 2U);

				// Set the Data Sync to be from the BS
				CDMRSync sync;
				sync.addSync(end + 2U, DST_BS_DATA);

				end[0U] = TAG_EOT;
				end[1U] = 0x00U;

				writeNetwork(end, DMRDT_TERMINATOR);
				writeQueue(end);

				LogMessage("DMR Slot %u, received RF end of transmission", m_slotNo);

				m_state = SS_LISTENING;
				setShortLC(m_slotNo, 0U);

				if (dataType == DT_TERMINATOR_WITH_LC)
					return;
			}

			// Regenerate the LC
			CFullLC fullLC;
			fullLC.encode(*m_lc, data + 2U, DT_TERMINATOR_WITH_LC);

			// Regenerate the Slot Type
			slotType.getData(data + 2U);

			// Convert the Data Sync to be from the BS
			CDMRSync sync;
			sync.addSync(data + 2U, DST_BS_DATA);

			data[0U] = TAG_EOT;
			data[1U] = 0x00U;

			writeNetwork(data, DMRDT_TERMINATOR);
			writeQueue(data);
		}
	} else if (audioSync) {
		if (m_state == SS_RELAYING_RF) {
			// Convert the Audio Sync to be from the BS
			CDMRSync sync;
			sync.addSync(data + 2U, DST_BS_AUDIO);

			unsigned char fid = m_lc->getFID();
			if (fid == FID_ETSI || fid == FID_DMRA)
				; // AMBE FEC

			data[0U] = TAG_DATA;
			data[1U] = 0x00U;
			m_n = 0U;

			writeQueue(data);
			writeNetwork(data, DMRDT_VOICE_SYNC);
		} else if (m_state == SS_LISTENING) {
			m_lateEntryWatchdog.start();
			m_state = SS_LATE_ENTRY;
		}
	} else {
		if (m_state == SS_RELAYING_RF) {
			// Regenerate the EMB
			CEMB emb;
			emb.putData(data + 2U);
			emb.setColorCode(m_colorCode);
			emb.getData(data + 2U);

			unsigned char fid = m_lc->getFID();
			if (fid == FID_ETSI || fid == FID_DMRA)
				; // AMBE FEC

			data[0U] = TAG_DATA;
			data[1U] = 0x00U;
			m_n++;

			writeQueue(data);
			writeNetwork(data, DMRDT_VOICE);
		} else if (m_state == SS_LATE_ENTRY) {
			// If we haven't received an LC yet, then be strict on the color code
			CEMB emb;
			emb.putData(data + 2U);
			unsigned char colorCode = emb.getColorCode();
			if (colorCode != m_colorCode)
				return;

			CLC* lc = m_embeddedLC.addData(data + 2U, emb.getLCSS());
			if (lc != NULL) {
				delete m_lc;
				m_lc = lc;

				// Create a dummy start frame to replace the received frame
				unsigned char start[DMR_FRAME_LENGTH_BYTES + 2U];

				CDMRSync sync;
				sync.addSync(start + 2U, DST_BS_DATA);

				CFullLC fullLC;
				fullLC.encode(*m_lc, start + 2U, DT_VOICE_LC_HEADER);

				CSlotType slotType;
				slotType.setColorCode(m_colorCode);
				slotType.setDataType(DT_VOICE_LC_HEADER);
				slotType.getData(start + 2U);

				start[0U] = TAG_DATA;
				start[1U] = 0x00U;
				m_n = 0U;

				m_writeQueue = (m_writeQueue + 1U) % 2U;
				m_playoutTimer[m_writeQueue]->start();

				writeNetwork(start, DMRDT_VOICE_LC_HEADER);
				writeQueue(start);

				// Send the original audio frame out
				unsigned char fid = m_lc->getFID();
				if (fid == FID_ETSI || fid == FID_DMRA)
					; // AMBE FEC

				data[0U] = TAG_DATA;
				data[1U] = 0x00U;
				m_n++;

				writeQueue(data);
				writeNetwork(data, DMRDT_VOICE);

				m_state = SS_RELAYING_RF;

				m_lateEntryWatchdog.stop();
				setShortLC(m_slotNo, m_lc->getDstId(), m_lc->getFLCO());

				LogMessage("DMR Slot %u, received RF late entry from %u to %s:%u", m_slotNo, m_lc->getSrcId(), m_lc->getFLCO() == FLCO_GROUP ? "Group" : "User", m_lc->getDstId());
			}
		}
	}
}

unsigned int CDMRSlot::readRF(unsigned char* data)
{
	if (!m_playoutTimer[m_readQueue]->isRunning() || !m_playoutTimer[m_readQueue]->hasExpired())
		return 0U;

	if (m_txQueue[m_readQueue]->isEmpty())
		return 0U;

	unsigned char len = 0U;
	m_txQueue[m_readQueue]->getData(&len, 1U);

	m_txQueue[m_readQueue]->getData(data, len);

	return len;
}

void CDMRSlot::writeNet(const CDMRData& dmrData)
{
	if (m_state == SS_RELAYING_RF || m_state == SS_LATE_ENTRY)
		return;

	m_networkWatchdog.start();

	DMR_DATA_TYPE dataType = dmrData.getDataType();

	unsigned char data[DMR_FRAME_LENGTH_BYTES + 2U];
	dmrData.getData(data + 2U);

	if (dataType == DMRDT_VOICE_LC_HEADER) {
		writeHeader(dmrData);

		LogMessage("DMR Slot %u, received network header from %u to %s:%u", m_slotNo, m_lc->getSrcId(), m_lc->getFLCO() == FLCO_GROUP ? "Group" : "User", m_lc->getDstId());
	} else if (dataType == DMRDT_TERMINATOR) {
		if (m_state != SS_RELAYING_NETWORK) {
			m_state = SS_LISTENING;
			return;
		}

		// Regenerate the LC
		CFullLC fullLC;
		fullLC.encode(*m_lc, data + 2U, DT_TERMINATOR_WITH_LC);

		// Regenerate the Slot Type
		CSlotType slotType;
		slotType.setColorCode(m_colorCode);
		slotType.setDataType(DT_TERMINATOR_WITH_LC);
		slotType.getData(data + 2U);

		// Convert the Data Sync to be from the BS
		CDMRSync sync;
		sync.addSync(data + 2U, DST_BS_DATA);

		data[0U] = TAG_EOT;
		data[1U] = 0x00U;

		writeQueue(data);

		m_state = SS_LISTENING;
		setShortLC(m_slotNo, 0U);
		m_networkWatchdog.stop();

		LogMessage("DMR Slot %u, received network end of transmission", m_slotNo);
	} else if (dataType == DMRDT_VOICE_SYNC) {
		if (m_state == SS_LISTENING) {
			// We must have missed the opening header
			writeHeader(dmrData);
		}

		// Convert the Audio Sync to be from the BS
		CDMRSync sync;
		sync.addSync(data + 2U, DST_BS_AUDIO);

		unsigned char fid = m_lc->getFID();
		if (fid == FID_ETSI || fid == FID_DMRA)
			; // AMBE FEC

		data[0U] = TAG_DATA;
		data[1U] = 0x00U;

		writeQueue(data);
	} else if (dataType == DMRDT_VOICE) {
		if (m_state == SS_LISTENING) {
			// We must have missed the opening header
			writeHeader(dmrData);
		}

		unsigned char fid = m_lc->getFID();
		if (fid == FID_ETSI || fid == FID_DMRA)
			; // AMBE FEC

		// Change the color code in the EMB
		CEMB emb;
		emb.putData(data + 2U);
		emb.setColorCode(m_colorCode);
		emb.getData(data + 2U);

		data[0U] = TAG_DATA;
		data[1U] = 0x00U;

		writeQueue(data);
	} else {
		// Change the Color Code of the Slot Type
		CSlotType slotType;
		slotType.putData(data + 2U);
		slotType.setColorCode(m_colorCode);
		slotType.getData(data + 2U);

		// Convert the Data Sync to be from the BS
		CDMRSync sync;
		sync.addSync(data + 2U, DST_BS_DATA);

		data[0U] = TAG_DATA;
		data[1U] = 0x00U;

		writeQueue(data);
	}
}

void CDMRSlot::clock(unsigned int ms)
{
	for (unsigned int i = 0U; i < NUM_QUEUES; i++)
		m_playoutTimer[i]->clock(ms);

	if (m_state == SS_RELAYING_NETWORK) {
		m_networkWatchdog.clock(ms);

		if (m_networkWatchdog.hasExpired()) {
			LogMessage("DMR Slot %u, network watchdog has expired", m_slotNo);
			setShortLC(m_slotNo, 0U);
			m_networkWatchdog.stop();
			m_state = SS_LISTENING;
		}
	}

	if (m_state == SS_LATE_ENTRY) {
		m_lateEntryWatchdog.clock(ms);

		if (m_lateEntryWatchdog.hasExpired()) {
			m_lateEntryWatchdog.stop();
			m_state = SS_LISTENING;
		}
	}
}

void CDMRSlot::writeQueue(const unsigned char *data)
{
	unsigned char len = DMR_FRAME_LENGTH_BYTES + 2U;
	m_txQueue[m_writeQueue]->addData(&len, 1U);

	m_txQueue[m_writeQueue]->addData(data, len);
}

void CDMRSlot::writeNetwork(const unsigned char* data, DMR_DATA_TYPE dataType)
{
	assert(m_lc != NULL);

	if (m_network == NULL)
		return;

	CDMRData dmrData;
	dmrData.setSlotNo(m_slotNo);
	dmrData.setDataType(dataType);
	dmrData.setSrcId(m_lc->getSrcId());
	dmrData.setDstId(m_lc->getDstId());
	dmrData.setFLCO(m_lc->getFLCO());
	dmrData.setN(m_n);

	dmrData.setData(data + 2U);

	m_network->write(dmrData);
}

void CDMRSlot::writeHeader(const CDMRData& dmrData)
{
	delete m_lc;

	m_lc = new CLC(dmrData.getFLCO(), dmrData.getSrcId(), dmrData.getDstId());

	unsigned char data[DMR_FRAME_LENGTH_BYTES + 2U];

	// Regenerate the LC
	CFullLC fullLC;
	fullLC.encode(*m_lc, data + 2U, DT_VOICE_LC_HEADER);

	// Regenerate the Slot Type
	CSlotType slotType;
	slotType.setColorCode(m_colorCode);
	slotType.setDataType(DT_VOICE_LC_HEADER);
	slotType.getData(data + 2U);

	// Convert the Data Sync to be from the BS
	CDMRSync sync;
	sync.addSync(data + 2U, DST_BS_DATA);

	data[0U] = TAG_DATA;
	data[1U] = 0x00U;

	m_writeQueue = (m_writeQueue + 1U) % 2U;
	m_playoutTimer[m_writeQueue]->start();

	writeQueue(data);

	m_state = SS_RELAYING_NETWORK;
	setShortLC(m_slotNo, m_lc->getDstId(), m_lc->getFLCO());
}

void CDMRSlot::init(unsigned int colorCode, CModem* modem, CHomebrewDMRIPSC* network)
{
	assert(modem != NULL);

	m_colorCode = colorCode;
	m_modem     = modem;
	m_network   = network;
}

void CDMRSlot::setShortLC(unsigned int slotNo, unsigned int id, FLCO flco)
{
	assert(m_modem != NULL);

	switch (slotNo) {
		case 1U:
			m_id1   = 0U;
			m_flco1 = flco;
			if (id != 0U) {
				unsigned char buffer[3U];
				buffer[0U] = (id << 16) & 0xFFU;
				buffer[1U] = (id << 8)  & 0xFFU;
				buffer[2U] = (id << 0)  & 0xFFU;
				m_id1 = CCRC::crc8(buffer, 3U);
			}
			break;
		case 2U:
			m_id2   = 0U;
			m_flco2 = flco;
			if (id != 0U) {
				unsigned char buffer[3U];
				buffer[0U] = (id << 16) & 0xFFU;
				buffer[1U] = (id << 8)  & 0xFFU;
				buffer[2U] = (id << 0)  & 0xFFU;
				m_id2 = CCRC::crc8(buffer, 3U);
			}
			break;
		default:
			LogError("Invalid slot number passed to setShortLC - %u", slotNo);
			return;
	}

	unsigned char lc[5U];
	lc[0U] = 0x01U;
	lc[1U] = 0x00U;
	lc[2U] = 0x00U;
	lc[3U] = 0x00U;

	if (m_id1 != 0U) {
		lc[2U] = m_id1;
		if (m_flco1 == FLCO_GROUP)
			lc[1U] |= 0x80U;
		else
			lc[1U] |= 0x90U;
	}

	if (m_id2 != 0U) {
		lc[3U] = m_id2;
		if (m_flco2 == FLCO_GROUP)
			lc[1U] |= 0x08U;
		else
			lc[1U] |= 0x09U;
	}

	lc[4U] = CCRC::crc8(lc, 4U);

	unsigned char sLC[9U];

	CUtils::dump(1U, "Input Short LC", lc, 5U);

	CShortLC shortLC;
	shortLC.encode(lc, sLC);

	CUtils::dump(1U, "Output Short LC", sLC, 9U);

	m_modem->writeDMRShortLC(sLC);
}
