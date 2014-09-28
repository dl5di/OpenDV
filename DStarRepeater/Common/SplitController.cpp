/*
 *   Copyright (C) 2012-2014 by Jonathan Naylor G4KLX
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

#include "SplitController.h"

const unsigned int REGISTRATION_TIMEOUT = 200U;

const unsigned int BUFFER_LENGTH = 200U;

CAMBESlot::CAMBESlot(unsigned int rxCount) :
m_valid(NULL),
m_errors(999U),
m_best(99U),
m_ambe(NULL),
m_length(0U),
m_end(NULL),
m_timer(1000U),
m_rxCount(rxCount)
{
	wxASSERT(rxCount > 0U);

	m_ambe  = new unsigned char[DV_FRAME_MAX_LENGTH_BYTES];
	m_valid = new bool[rxCount];
	m_end   = new bool[rxCount];

	reset();
}

CAMBESlot::~CAMBESlot() 
{
	delete[] m_end;
	delete[] m_valid;
	delete[] m_ambe;
}

void CAMBESlot::reset()
{
	m_errors = 999U;
	m_best   = 99U;
	m_length = 0U;

	for (unsigned int i = 0U; i < m_rxCount; i++) {
		m_valid[i] = false;
		m_end[i]   = false;
	}

	m_timer.stop();
}

bool CAMBESlot::isFirst() const
{
	return m_length == 0U;
}

CSplitController::CSplitController(const wxString& localAddress, unsigned int localPort, const wxArrayString& transmitterNames, const wxArrayString& receiverNames, unsigned int timeout) :
CModem(),
m_handler(localAddress, localPort),
m_transmitterNames(transmitterNames),
m_receiverNames(receiverNames),
m_timeout(timeout),
m_txCount(0U),
m_rxCount(0U),
m_txAddresses(NULL),
m_txPorts(NULL),
m_txTimers(NULL),
m_rxAddresses(NULL),
m_rxPorts(NULL),
m_rxTimers(NULL),
m_txData(1000U),
m_outId(0x00U),
m_outSeq(0U),
m_endTimer(1000U, 1U),
m_listening(true),
m_inSeqNo(0x00U),
m_outSeqNo(0x00U),
m_header(NULL),
m_id(NULL),
m_valid(NULL),
m_slots(NULL),
m_headerSent(false),
m_packets(NULL),
m_best(NULL),
m_missed(NULL),
m_silence(0U)
{
	m_txCount = transmitterNames.GetCount();
	m_rxCount = receiverNames.GetCount();

	wxASSERT(m_txCount > 0U);
	wxASSERT(m_rxCount > 0U);

	m_txAddresses = new in_addr[m_txCount];
	m_txPorts     = new unsigned int[m_txCount];
	m_txTimers    = new CTimer*[m_txCount];

	m_rxAddresses = new in_addr[m_rxCount];
	m_rxPorts     = new unsigned int[m_rxCount];
	m_rxTimers    = new CTimer*[m_rxCount];

	m_header = new unsigned char[RADIO_HEADER_LENGTH_BYTES];

	m_id      = new wxUint16[m_rxCount];
	m_valid   = new bool[m_rxCount];
	m_packets = new unsigned int[m_rxCount];
	m_best    = new unsigned int[m_rxCount];
	m_missed  = new unsigned int[m_rxCount];

	for (unsigned int i = 0U; i < m_rxCount; i++) {
		m_rxTimers[i] = new CTimer(1000U, REGISTRATION_TIMEOUT);
		m_rxPorts[i]  = 0U;
		m_valid[i]    = false;
		m_id[i]       = 0x00U;
		m_packets[i]  = 0U;
		m_best[i]     = 0U;
		m_missed[i]   = 0U;
	}

	for (unsigned int i = 0U; i < m_txCount; i++) {
		m_txTimers[i] = new CTimer(1000U, REGISTRATION_TIMEOUT);
		m_txPorts[i]  = 0U;
	}

	m_slots = new CAMBESlot*[21U];
	for (unsigned int i = 0U; i < 21U; i++)
		m_slots[i] = new CAMBESlot(m_rxCount);
}

CSplitController::~CSplitController()
{
	delete[] m_txAddresses;
	delete[] m_txPorts;
	delete[] m_rxAddresses;
	delete[] m_rxPorts;
	delete[] m_missed;
	delete[] m_best;
	delete[] m_packets;
	delete[] m_header;
	delete[] m_id;
	delete[] m_valid;

	for (unsigned int i = 0U; i < 21U; i++)
		delete m_slots[i];
	delete[] m_slots;

	for (unsigned int i = 0U; i < m_txCount; i++)
		delete m_txTimers[i];
	for (unsigned int i = 0U; i < m_rxCount; i++)
		delete m_rxTimers[i];

	delete[] m_txTimers;
	delete[] m_rxTimers;
}

bool CSplitController::start()
{
	bool ret = m_handler.open();
	if (!ret)
		return false;

	Create();
	SetPriority(100U);
	Run();

	return true;
}

void* CSplitController::Entry()
{
	wxLogMessage(wxT("Starting Split Controller thread"));

	wxStopWatch stopWatch;

	while (!m_stopped) {
		stopWatch.Start();

		transmit();

		receive();

		Sleep(10UL);

		unsigned int ms = stopWatch.Time();
		timers(ms);
	}

	wxLogMessage(wxT("Stopping Split Controller thread"));

	m_handler.close();

	return NULL;
}

bool CSplitController::writeHeader(const CHeaderData& header)
{
	bool ret = m_txData.hasSpace(RADIO_HEADER_LENGTH_BYTES);
	if (!ret) {
		wxLogWarning(wxT("No space to write the header"));
		return false;
	}

	unsigned char buffer[100U];

	::memset(buffer, ' ', RADIO_HEADER_LENGTH_BYTES - 2U);

	buffer[0U] = header.getFlag1();
	buffer[1U] = header.getFlag2();
	buffer[2U] = header.getFlag3();

	wxString rpt2 = header.getRptCall2();
	for (unsigned int i = 0U; i < rpt2.Len() && i < LONG_CALLSIGN_LENGTH; i++)
		buffer[i + 3U]  = rpt2.GetChar(i);

	wxString rpt1 = header.getRptCall1();
	for (unsigned int i = 0U; i < rpt1.Len() && i < LONG_CALLSIGN_LENGTH; i++)
		buffer[i + 11U] = rpt1.GetChar(i);

	wxString your = header.getYourCall();
	for (unsigned int i = 0U; i < your.Len() && i < LONG_CALLSIGN_LENGTH; i++)
		buffer[i + 19U] = your.GetChar(i);

	wxString my1 = header.getMyCall1();
	for (unsigned int i = 0U; i < my1.Len() && i < LONG_CALLSIGN_LENGTH; i++)
		buffer[i + 27U] = my1.GetChar(i);

	wxString my2 = header.getMyCall2();
	for (unsigned int i = 0U; i < my2.Len() && i < SHORT_CALLSIGN_LENGTH; i++)
		buffer[i + 35U] = my2.GetChar(i);

	wxMutexLocker locker(m_mutex);

	unsigned char data[2U];
	data[0U] = DSMTT_HEADER;
	data[1U] = RADIO_HEADER_LENGTH_BYTES - 2U;
	m_txData.addData(data, 2U);

	m_txData.addData(buffer, RADIO_HEADER_LENGTH_BYTES - 2U);

	return true;
}

bool CSplitController::writeData(const unsigned char* data, unsigned int length, bool end)
{
	bool ret = m_txData.hasSpace(DV_FRAME_LENGTH_BYTES + 2U);
	if (!ret) {
		wxLogWarning(wxT("No space to write data"));
		return false;
	}

	wxMutexLocker locker(m_mutex);

	unsigned char buffer[2U];
	buffer[0U] = end ? DSMTT_EOT : DSMTT_DATA;
	buffer[1U] = DV_FRAME_LENGTH_BYTES;
	m_txData.addData(buffer, 2U);

	m_txData.addData(data, DV_FRAME_LENGTH_BYTES);

	return true;
}

unsigned int CSplitController::getSpace()
{
	return m_txData.freeSpace() / (DV_FRAME_LENGTH_BYTES + 2U);
}

bool CSplitController::isTXReady()
{
	return m_txData.isEmpty();
}

void CSplitController::transmit()
{
	if (!m_txData.hasData())
		return;

	unsigned char type = 0U;
	unsigned char length = 0U;
	unsigned char buffer[RADIO_HEADER_LENGTH_BYTES];
	{
		wxMutexLocker locker(m_mutex);

		m_txData.getData(&type, 1U);
		m_txData.getData(&length, 1U);
		m_txData.getData(buffer, length);
	}

	if (type == DSMTT_HEADER) {
		m_outId  = (::rand() % 65535U) + 1U;
		m_outSeq = 0U;
		m_tx     = true;

		for (unsigned int i = 0U; i < m_txCount; i++) {
			if (m_txPorts[i] > 0U)
				m_handler.writeHeader(buffer, m_outId, m_txAddresses[i], m_txPorts[i]);
		}
	} else {
		// If this is a data sync, reset the sequence to zero
		if (buffer[9] == 0x55U && buffer[10] == 0x2DU && buffer[11] == 0x16U)
			m_outSeq = 0U;

		if (type == DSMTT_EOT) {
			m_outSeq |= 0x40U;
			m_tx = false;
		}

		for (unsigned int i = 0U; i < m_txCount; i++) {
			if (m_txPorts[i] > 0U)
				m_handler.writeData(buffer, length, m_outId, m_outSeq, m_txAddresses[i], m_txPorts[i]);
		}

		m_outSeq++;
		if (m_outSeq > 0x14U)
			m_outSeq = 0U;
	}
}

void CSplitController::receive()
{
	NETWORK_TYPE type = NETWORK_DATA;

	while (type != NETWORK_NONE) {
		wxUint16 id;
		in_addr address;
		unsigned int port;

		type = m_handler.read(id, address, port);

		if (type == NETWORK_NONE) {
			// Do nothing
		} else if (type == NETWORK_HEADER) {
			unsigned char header[RADIO_HEADER_LENGTH_BYTES];
			unsigned int length = m_handler.readHeader(header, RADIO_HEADER_LENGTH_BYTES);

			// A length of zero is a bad checksum, ignore it
			if (length > 0U) {
				bool found = false;
				for (unsigned int i = 0U; i < m_rxCount; i++) {
					if (address.s_addr == m_rxAddresses[i].s_addr && port == m_rxPorts[i]) {
						processHeader(i, id, header, length);
						m_rxTimers[i]->start();
						found = true;
						break;
					}
				}

				if (!found) {
					wxString addr(::inet_ntoa(address), wxConvLocal);
					wxLogError(wxT("Header received from unknown repeater - %s:%u"), addr.c_str(), port);
				}
			}
		} else if (type == NETWORK_DATA) {
			unsigned char ambe[DV_FRAME_MAX_LENGTH_BYTES];
			wxUint8 seqNo;
			unsigned int errors;
			unsigned int length = m_handler.readData(ambe, DV_FRAME_MAX_LENGTH_BYTES, seqNo, errors);

			for (unsigned int i = 0U; i < m_rxCount; i++) {
				if (address.s_addr == m_rxAddresses[i].s_addr && port == m_rxPorts[i]) {
					processAMBE(i, id, ambe, length, seqNo, errors);
					m_rxTimers[i]->start();
					break;
				}
			}
		} else if (type == NETWORK_REGISTER) {
			// These can be from transmitters and receivers
			wxString name;
			m_handler.readRegister(name);

			int n1 = m_receiverNames.Index(name);
			int n2 = m_transmitterNames.Index(name);

			if (n1 != wxNOT_FOUND) {
				wxASSERT(n1 < int(m_rxCount));
				m_rxTimers[n1]->start();

				if (m_rxAddresses[n1].s_addr != address.s_addr || m_rxPorts[n1] != port) {
					wxString addr1(::inet_ntoa(m_rxAddresses[n1]), wxConvLocal);
					wxString addr2(::inet_ntoa(address), wxConvLocal);

					if (m_rxPorts[n1] == 0U)
						wxLogMessage(wxT("Registration of RX %d \"%s\" set to %s:%u"), n1 + 1, name.c_str(), addr2.c_str(), port);
					else
						wxLogMessage(wxT("Registration of RX %d \"%s\" changed from %s:%u to %s:%u"), n1 + 1, name.c_str(), addr1.c_str(), m_rxPorts[n1], addr2.c_str(), port);

					m_rxAddresses[n1].s_addr = address.s_addr;
					m_rxPorts[n1] = port;
				}
			}

			if (n2 != wxNOT_FOUND) {
				wxASSERT(n2 < int(m_txCount));
				m_txTimers[n2]->start();

				if (m_txAddresses[n2].s_addr != address.s_addr || m_txPorts[n2] != port) {
					wxString addr1(::inet_ntoa(m_txAddresses[n2]), wxConvLocal);
					wxString addr2(::inet_ntoa(address), wxConvLocal);

					if (m_txPorts[n2] == 0U)
						wxLogMessage(wxT("Registration of TX %d \"%s\" set to %s:%u"), n2 + 1, name.c_str(), addr2.c_str(), port);
					else
						wxLogMessage(wxT("Registration of TX %d \"%s\" changed from %s:%u to %s:%u"), n2 + 1, name.c_str(), addr1.c_str(), m_txPorts[n2], addr2.c_str(), port);

					m_txAddresses[n2].s_addr = address.s_addr;
					m_txPorts[n2] = port;
				}
			}

			if (n1 == wxNOT_FOUND && n2 == wxNOT_FOUND) {
				wxString addr(::inet_ntoa(address), wxConvLocal);
				wxLogError(wxT("Registration of \"%s\" received from unknown repeater - %s:%u"), name.c_str(), addr.c_str(), port);
			}
		} else {
			wxString addr(::inet_ntoa(address), wxConvLocal);
			wxLogError(wxT("Received invalid frame type %d from %s:%u"), int(type), addr.c_str(), port);
		}
	}
}

void CSplitController::timers(unsigned int ms)
{
	m_endTimer.clock(ms);
	if (m_endTimer.isRunning() && m_endTimer.hasExpired()) {
		printStats();

		wxMutexLocker locker(m_mutex);
		unsigned char data[2U];
		data[0U] = DSMTT_EOT;
		data[1U] = 0U;
		m_rxData.addData(data, 2U);

		m_listening = true;
		m_endTimer.stop();

		return;
	}

	// Run the registration timers
	for (unsigned int i = 0U; i < m_txCount; i++) {
		m_txTimers[i]->clock(ms);
		if (m_txTimers[i]->isRunning() && m_txTimers[i]->hasExpired()) {
			wxLogWarning(wxT("TX %u registration has expired"), i + 1U);
			m_txTimers[i]->stop();
			m_txPorts[i] = 0U;
		}
	}

	for (unsigned int i = 0U; i < m_rxCount; i++) {
		m_rxTimers[i]->clock(ms);
		if (m_rxTimers[i]->isRunning() && m_rxTimers[i]->hasExpired()) {
			wxLogWarning(wxT("RX %u registration has expired"), i + 1U);
			m_rxTimers[i]->stop();
			m_rxPorts[i] = 0U;
		}
	}

	// If we are listening then no need to run the slot timers
	if (m_listening)
		return;

	// Run the slot timers
	for (unsigned int i = 0U; i < 21U; i++)
		m_slots[i]->m_timer.clock(ms);

	// Check for expired timers
	for (unsigned int i = 0U; i < 21U; i++) {
		CAMBESlot* slot = m_slots[m_outSeqNo];

		// Got to an unexpired timer
		if (!slot->m_timer.hasExpired())
			return;

		// The last frame?
		if (isEnd(*slot)) {
			printStats();

			wxMutexLocker locker(m_mutex);

			if (!m_headerSent)
				sendHeader();

			unsigned char data[2U];
			data[0U] = DSMTT_EOT;
			data[1U] = 0U;
			m_rxData.addData(data, 2U);

			m_listening = true;
			m_endTimer.stop();

			return;
		}

		// Is there any data?		
		if (slot->m_length > 0U) {
			m_best[slot->m_best]++;

			wxMutexLocker locker(m_mutex);

			if (!m_headerSent)
				sendHeader();

			unsigned char data[2U];
			data[0U] = DSMTT_DATA;
			data[1U] = DV_FRAME_LENGTH_BYTES;
			m_rxData.addData(data, 2U);

			m_rxData.addData(slot->m_ambe, DV_FRAME_LENGTH_BYTES);
		} else {
			m_silence++;

			// Send a silence frame to the repeater
			wxMutexLocker locker(m_mutex);

			if (!m_headerSent)
				sendHeader();

			unsigned char data[2U];
			data[0U] = DSMTT_DATA;
			data[1U] = DV_FRAME_LENGTH_BYTES;
			m_rxData.addData(data, 2U);

			m_rxData.addData(NULL_FRAME_DATA_BYTES, DV_FRAME_LENGTH_BYTES);
		}

		slot->reset();
		slot->m_timer.start(0U, 21U * DSTAR_FRAME_TIME_MS);

		m_outSeqNo++;
		if (m_outSeqNo >= 21U)
			m_outSeqNo = 0U;
	}
}

void CSplitController::processHeader(unsigned int n, wxUint16 id, const unsigned char* header, unsigned int length)
{
	if (m_listening) {
		::memcpy(m_header, header, RADIO_HEADER_LENGTH_BYTES - 2U);

		// Reset the slots and start the timers
		for (unsigned int i = 0U; i < 21U; i++) {
			m_slots[i]->reset();
			m_slots[i]->m_timer.start(0U, DSTAR_FRAME_TIME_MS + DSTAR_FRAME_TIME_MS * i + m_timeout);
		}

		for (unsigned int i = 0U; i < m_rxCount; i++) {
			m_valid[i]   = false;
			m_id[i]      = 0x00U;
			m_packets[i] = 0U;
			m_best[i]    = 0U;
			m_missed[i]  = 0U;
		}
		m_silence = 0U;

		m_valid[n] = true;
		m_id[n] = id;

		m_inSeqNo    = 0U;
		m_outSeqNo   = 0U;
		m_listening  = false;
		m_headerSent = false;
		m_endTimer.start();
	} else {
		m_valid[n] = ::memcmp(m_header, header, RADIO_HEADER_LENGTH_BYTES - 2U) == 0;
		m_id[n] = id;		// Makes no difference as the valid flag will ensure we ignore bad ids
	}
}

void CSplitController::processAMBE(unsigned int n, wxUint16 id, const unsigned char* ambe, unsigned int length, wxUint8 seqNo, unsigned char errors)
{
	if (m_listening)
		return;

	if (!m_valid[n] || id != m_id[n])
		return;

	// Mask out the control bits of the sequence number
	unsigned char seqNo1 = seqNo & 0x1FU;

	// Check for an out of order frame
	unsigned char seqNo2 = m_inSeqNo;
	unsigned char count = 0U;
	while (seqNo1 != seqNo2) {
		count++;

		seqNo2++;
		if (seqNo2 >= 21U)
			seqNo2 = 0U;
	}

	if (count > 18U) {
		m_missed[n]++;
		return;
	}

	m_inSeqNo = (seqNo & 0x3FU) + 1U;
	if (m_inSeqNo >= 21U)
		m_inSeqNo = 0U;

	CAMBESlot* slot = m_slots[seqNo & 0x3FU];

	m_endTimer.start();

	bool end = (seqNo & 0x40U) == 0x40U;
	slot->m_valid[n] = true;
	m_packets[n]++;

	// An EOF removes the receiver from all future data
	if (end) {
		slot->m_end[n] = true;
		m_valid[n]    = false;
	}

	// If the existing data is of a better quality and not an end packet
	if (slot->m_errors < errors || end)
		return;

	::memcpy(slot->m_ambe, ambe, length);
	slot->m_length = length;

	slot->m_errors = errors;
	slot->m_best   = n;
}

bool CSplitController::isEnd(const CAMBESlot& slot) const
{
	bool hasNoEnd = false;
	bool hasEnd   = false;

	for (unsigned int i = 0U; i < m_rxCount; i++) {
		if (slot.m_valid[i] && slot.m_end[i])
			hasEnd = true;

		if (slot.m_valid[i] && !slot.m_end[i])
			hasNoEnd = true;
	}

	return hasEnd && !hasNoEnd;	
}

void CSplitController::sendHeader()
{
	// Assume that the caller has the mutex lock
	unsigned char data[2U];
	data[0U] = DSMTT_HEADER;
	data[1U] = RADIO_HEADER_LENGTH_BYTES - 2U;
	m_rxData.addData(data, 2U);

	m_rxData.addData(m_header, RADIO_HEADER_LENGTH_BYTES - 2U);

	m_headerSent = true;
}

void CSplitController::printStats() const
{
	// Count the total number of packets in this transmission
	unsigned int total = m_silence + 1U;
	for (unsigned int i = 0U; i < m_rxCount; i++) {
		if (m_rxPorts[i] > 0U)
			total += m_best[i];
	}

	wxString temp;
	wxString text = wxT("Packets: total/");

	for (unsigned int i = 0U; i < m_rxCount; i++) {
		if (m_rxPorts[i] > 0U) {
			temp.Printf(wxT("rx%u/"), i + 1U);
			text.Append(temp);
		}
	}

	temp.Printf(wxT("silence %u/"), total);
	text.Append(temp);

	for (unsigned int i = 0U; i < m_rxCount; i++) {
		if (m_rxPorts[i] > 0U) {
			temp.Printf(wxT("%u/"), m_packets[i]);
			text.Append(temp);
		}
	}

	temp.Printf(wxT("%u, Proportion: "), m_silence);
	text.Append(temp);

	for (unsigned int i = 0U; i < m_rxCount; i++) {
		if (m_rxPorts[i] > 0U) {
			temp.Printf(wxT("rx%u/"), i + 1U);
			text.Append(temp);
		}
	}

	text.Append(wxT("silence "));

	for (unsigned int i = 0U; i < m_rxCount; i++) {
		if (m_rxPorts[i] > 0U) {
			temp.Printf(wxT("%u%%%%/"), (100U * m_best[i]) / total);
			text.Append(temp);
		}
	}

	temp.Printf(wxT("%u%%%%, Missed: "), (100U * m_silence) / total);
	text.Append(temp);

	unsigned int n = 0U;
	for (unsigned int i = 0U; i < m_rxCount; i++) {
		if (m_rxPorts[i] > 0U) {
			if (n > 0U)
				temp.Printf(wxT("/rx%u"), i + 1U);
			else
				temp.Printf(wxT("rx%u"), i + 1U);

			n++;
			text.Append(temp);
		}
	}

	text.Append(wxT(" "));

	n = 0U;
	for (unsigned int i = 0U; i < m_rxCount; i++) {
		if (m_rxPorts[i] > 0U) {
			if (n > 0U)
				temp.Printf(wxT("/%u%%%%"), (100U * m_missed[i]) / total);
			else
				temp.Printf(wxT("%u%%%%"), (100U * m_missed[i]) / total);

			n++;
			text.Append(temp);
		}
	}

	wxLogMessage(text);
}
