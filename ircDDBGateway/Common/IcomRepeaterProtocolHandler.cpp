/*
 *   Copyright (C) 2010-2013 by Jonathan Naylor G4KLX
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

#include "IcomRepeaterProtocolHandler.h"

#include "CCITTChecksum.h"
#include "DStarDefines.h"
#include "Utils.h"

// Allow space for a big DD packet
const unsigned int BUFFER_LENGTH = 2500U;

const unsigned int QUEUE_LENGTH  = 50U;

const unsigned int LOOP_DELAY    = 5UL;
const unsigned int LOOP_TICKS    = 200U;

CIcomRepeaterProtocolHandler::CIcomRepeaterProtocolHandler(const wxString& address, unsigned int port, const wxString& icomAddress, unsigned int icomPort) :
wxThread(wxTHREAD_JOINABLE),
m_socket(address, port),
m_icomAddress(),
m_icomPort(icomPort),
m_over1(false),
m_seqNo(0U),
m_tries(0U),
m_ackQueue(NULL),
m_killed(false),
m_type(RT_NONE),
m_buffer(NULL),
m_rptrQueue(QUEUE_LENGTH),
m_gwyQueue(QUEUE_LENGTH),
m_retryTimer(LOOP_TICKS, 0U, 200U)		// 200ms
{
	wxASSERT(!icomAddress.IsEmpty());
	wxASSERT(!address.IsEmpty());
	wxASSERT(icomPort > 0U);
	wxASSERT(port > 0U);

	m_icomAddress.s_addr = ::inet_addr(icomAddress.mb_str());

	m_buffer = new unsigned char[BUFFER_LENGTH];

	wxDateTime now = wxDateTime::UNow();
	::srand(now.GetMillisecond());
}

CIcomRepeaterProtocolHandler::~CIcomRepeaterProtocolHandler()
{
	while (!m_gwyQueue.isEmpty()) {
		CDataQueue* dq = m_gwyQueue.getData();
		free(dq);
	}

	while (!m_rptrQueue.isEmpty()) {
		CDataQueue* dq = m_rptrQueue.getData();
		free(dq);
	}

	delete[] m_buffer;
}

void CIcomRepeaterProtocolHandler::setCount(unsigned int count)
{
	m_over1 = count > 1U;
}

bool CIcomRepeaterProtocolHandler::open()
{
	bool ret = m_socket.open();
	if (!ret)
		return false;

	unsigned char buffer[10U];

	buffer[0U] = 'I';
	buffer[1U] = 'N';
	buffer[2U] = 'I';
	buffer[3U] = 'T';
	buffer[4U] = 0x00;
	buffer[5U] = 0x00;
	buffer[6U] = 0x73;
	buffer[7U] = 0x00;
	buffer[8U] = 0x00;
	buffer[9U] = 0x00;

	ret = m_socket.write(buffer, 10U, m_icomAddress, m_icomPort);
	if (!ret) {
		m_socket.close();
		return false;
	}

	// Wait for a reply from the RP2C
	for (unsigned int i = 0U; i < 10U; i++) {
		in_addr address;
		unsigned int port;
		int length = m_socket.read(m_buffer, BUFFER_LENGTH, address, port);

		if (length == 10 && m_buffer[0U] == 'I' && m_buffer[1U] == 'N' && m_buffer[2U] == 'I' && m_buffer[3U] == 'T' && m_buffer[6U] == 0x72 && m_buffer[7U] == 0x00) {
			m_seqNo = m_buffer[4U] * 256U + m_buffer[5U] + 1U;
			wxLogMessage(wxT("Initial sequence number from the RP2C is %u"), m_seqNo);

			// Start the thread
			Create();
			Run();

			return true;
		}

		::wxSleep(1);
	}

	m_socket.close();

	wxLogError(wxT("No reply from the RP2C for 10 seconds, aborting"));

	return false;
}

void* CIcomRepeaterProtocolHandler::Entry()
{
	wxLogMessage(wxT("Starting the Icom controller thread"));

	while (!m_killed) {
		sendGwyPackets();

		Sleep(LOOP_DELAY);

		readIcomPackets();

		m_retryTimer.clock();
	}

	wxLogMessage(wxT("Stopping the Icom controller thread"));

	m_socket.close();

	return NULL;
}

bool CIcomRepeaterProtocolHandler::writeHeader(CHeaderData& header)
{
	CDataQueue* dq = new CDataQueue(new CHeaderData(header));

	m_gwyQueue.addData(dq);

	return true;
}

bool CIcomRepeaterProtocolHandler::writeAMBE(CAMBEData& data)
{
	CDataQueue* dq = new CDataQueue(new CAMBEData(data));

	m_gwyQueue.addData(dq);

	return true;
}

bool CIcomRepeaterProtocolHandler::writeDD(CDDData& data)
{
	CDataQueue* dq = new CDataQueue(new CDDData(data));

	m_gwyQueue.addData(dq);

	return true;
}

bool CIcomRepeaterProtocolHandler::writeText(CTextData&)
{
	return true;
}

bool CIcomRepeaterProtocolHandler::writeStatus(CStatusData&)
{
	return true;
}

void CIcomRepeaterProtocolHandler::readIcomPackets()
{
	for (;;) {
		// No more data?
		in_addr address;
		unsigned int port;
		int length = m_socket.read(m_buffer, BUFFER_LENGTH, address, port);
		if (length <= 0)
			return;

		if (address.s_addr != m_icomAddress.s_addr || port != m_icomPort) {
			wxLogError(wxT("Incoming Icom data from an unknown source"));
			continue;
		}

		// Invalid packet type?
		if (m_buffer[0] != 'D' || m_buffer[1] != 'S' || m_buffer[2] != 'T' || m_buffer[3] != 'R') {
			CUtils::dump(wxT("Missing DSTR from the RP2C"), m_buffer, length);
			continue;
		}

		// An ack
		if (length == 10 && m_buffer[6] == 0x72) {
			wxUint16 seqNo = m_buffer[4] * 256U + m_buffer[5];

			if (seqNo == m_seqNo)
				m_seqNo++;
			else
				m_seqNo = seqNo;

			// Free the ack queue
			free(m_ackQueue);
			m_ackQueue = NULL;

			m_retryTimer.stop();
			m_tries = 0U;

			continue;
		}

		// Send an ack for all packet types except an ack
		wxUint16 seqNo = m_buffer[4] * 256U + m_buffer[5];
		sendAck(seqNo);

		// Heard data
		if (m_buffer[6] == 0x73 && m_buffer[7] == 0x21) {
			CHeardData* heard = new CHeardData;
			bool ret = heard->setIcomRepeaterData(m_buffer, length, m_icomAddress, m_icomPort);
			if (!ret) {
				wxLogError(wxT("Invalid heard data from the RP2C"));
				delete heard;
				continue;
			}

			m_rptrQueue.addData(new CDataQueue(heard));
			continue;
		}

		// Poll data
		if (m_buffer[6] == 0x73 && m_buffer[7] == 0x00) {
			m_rptrQueue.addData(new CDataQueue);
			continue;
		}

		// DD Data
		if (m_buffer[6] == 0x73 && m_buffer[7] == 0x11) {
			CDDData* data = new CDDData;
			bool ret = data->setIcomRepeaterData(m_buffer, length, m_icomAddress, m_icomPort);
			if (!ret) {
				wxLogError(wxT("Invalid DD data from the RP2C"));
				delete data;
				continue;
			}

			m_rptrQueue.addData(new CDataQueue(data));
			continue;
		}

		// DV data
		if (m_buffer[6] == 0x73 && m_buffer[7] == 0x12 && m_buffer[10] == 0x20) {
			if ((m_buffer[16] & 0x80) == 0x80) {
				CHeaderData* header = new CHeaderData;
				bool ret = header->setIcomRepeaterData(m_buffer, length, true, m_icomAddress, m_icomPort);
				if (!ret) {
					wxLogError(wxT("Invalid header data or checksum from the RP2C"));
					delete header;
					continue;
				}

				if (m_over1)
					sendMultiReply(*header);
				else
					sendSingleReply(*header);

				m_rptrQueue.addData(new CDataQueue(header));
				continue;
			} else {
				CAMBEData* data = new CAMBEData;
				bool ret = data->setIcomRepeaterData(m_buffer, length, m_icomAddress, m_icomPort);
				if (!ret) {
					wxLogError(wxT("Invalid AMBE data from the RP2C"));
					delete data;
					continue;
				}

				m_rptrQueue.addData(new CDataQueue(data));
				continue;
			}
		}

		// An unknown type
		CUtils::dump(wxT("Unknown packet type from the RP2C"), m_buffer, length);
	}
}

void CIcomRepeaterProtocolHandler::sendGwyPackets()
{
	// Anything to send
	if (m_gwyQueue.isEmpty() && m_ackQueue == NULL)
		return;

	if (m_tries > 0U && !m_retryTimer.hasExpired())
		return;

	if (m_ackQueue == NULL) {
		m_ackQueue = m_gwyQueue.getData();
		if (m_ackQueue == NULL) {
			wxLogError(wxT("getData of a non-empty gateway queue is NULL"));
			return;
		}
	}

	unsigned int length = 0U;

	switch (m_ackQueue->getType()) {
		case RT_HEADER: {
				CHeaderData* header = m_ackQueue->getHeader();
				header->setRptSeq(m_seqNo);
				length = header->getIcomRepeaterData(m_buffer, 60U, true);
			}
			break;

		case RT_AMBE: {
				CAMBEData* data = m_ackQueue->getAMBE();
				data->setRptSeq(m_seqNo);
				length = data->getIcomRepeaterData(m_buffer, 60U);
			}
			break;

		case RT_DD: {
				CDDData* data = m_ackQueue->getDD();
				data->setRptSeq(m_seqNo);
				length = data->getIcomRepeaterData(m_buffer, BUFFER_LENGTH);
			}
			break;

		default:
			wxLogError(wxT("Invalid type in the gateway queue"));
			break;
	}

	if (length > 0U) {
		m_socket.write(m_buffer, length, m_icomAddress, m_icomPort);

		m_tries++;
		m_retryTimer.start();

		if (m_tries > 0U && (m_tries % 100U) == 0U)
			wxLogMessage(wxT("No reply from the RP2C after %u retries"), m_tries);
	}
}

REPEATER_TYPE CIcomRepeaterProtocolHandler::read()
{
	if (m_rptrQueue.isEmpty()) {
		m_type = RT_NONE;
	} else {
		CDataQueue* dq = m_rptrQueue.peek();
		if (dq == NULL) {
			wxLogError(wxT("Peek of a non-empty repeater queue is NULL"));
			m_type = RT_NONE;
		} else {
			m_type = dq->getType();
		}
	}

	return m_type;
}

CPollData* CIcomRepeaterProtocolHandler::readPoll()
{
	if (m_type != RT_POLL)
		return NULL;

	CDataQueue* dq = m_rptrQueue.getData();
	if (dq == NULL) {
		wxLogError(wxT("Missing DataQueue in readPoll"));
		return NULL;
	}

	if (dq->getType() != RT_POLL) {
		wxLogError(wxT("Wrong DataQueue type in readPoll"));
		delete dq;
		return NULL;
	}

	delete dq;

	CPollData* data = new CPollData;
	data->setData1(wxT("icom_rp2c"));

	return data;
}

CHeaderData* CIcomRepeaterProtocolHandler::readHeader()
{
	if (m_type != RT_HEADER)
		return NULL;

	CDataQueue* dq = m_rptrQueue.getData();
	if (dq == NULL) {
		wxLogError(wxT("Missing DataQueue in readHeader"));
		return NULL;
	}

	if (dq->getType() != RT_HEADER) {
		wxLogError(wxT("Wrong DataQueue type in readHeader"));
		delete dq;
		return NULL;
	}

	CHeaderData* header = dq->getHeader();

	delete dq;

	return header;
}

CAMBEData* CIcomRepeaterProtocolHandler::readAMBE()
{
	if (m_type != RT_AMBE)
		return NULL;

	CDataQueue* dq = m_rptrQueue.getData();
	if (dq == NULL) {
		wxLogError(wxT("Missing DataQueue in readData"));
		return NULL;
	}

	if (dq->getType() != RT_AMBE) {
		wxLogError(wxT("Wrong DataQueue type in readData"));
		delete dq;
		return NULL;
	}

	CAMBEData* data = dq->getAMBE();

	delete dq;

	return data;
}

CHeardData* CIcomRepeaterProtocolHandler::readHeard()
{
	if (m_type != RT_HEARD)
		return NULL;

	CDataQueue* dq = m_rptrQueue.getData();
	if (dq == NULL) {
		wxLogError(wxT("Missing DataQueue in readHeard"));
		return NULL;
	}

	if (dq->getType() != RT_HEARD) {
		wxLogError(wxT("Wrong DataQueue type in readHeard"));
		delete dq;
		return NULL;
	}

	CHeardData* heard = dq->getHeard();

	delete dq;

	return heard;
}

CDDData* CIcomRepeaterProtocolHandler::readDD()
{
	if (m_type != RT_DD)
		return NULL;

	CDataQueue* dq = m_rptrQueue.getData();
	if (dq == NULL) {
		wxLogError(wxT("Missing DataQueue in readData"));
		return NULL;
	}

	if (dq->getType() != RT_DD) {
		wxLogError(wxT("Wrong DataQueue type in readData"));
		delete dq;
		return NULL;
	}

	CDDData* data = dq->getDD();

	delete dq;

	return data;
}

CHeaderData* CIcomRepeaterProtocolHandler::readBusyHeader()
{
	return NULL;
}

CAMBEData* CIcomRepeaterProtocolHandler::readBusyAMBE()
{
	return NULL;
}

void CIcomRepeaterProtocolHandler::close()
{
	m_killed = true;

	Wait();
}

bool CIcomRepeaterProtocolHandler::sendAck(wxUint16 seqNo)
{
	unsigned char buffer[10U];

	buffer[0U] = 'D';
	buffer[1U] = 'S';
	buffer[2U] = 'T';
	buffer[3U] = 'R';

	buffer[4U] = seqNo / 256U;
	buffer[5U] = seqNo % 256U;

	buffer[6U] = 0x72U;
	buffer[7U] = 0x00U;

	buffer[8U] = 0x00U;
	buffer[9U] = 0x00U;

	return m_socket.write(buffer, 10U, m_icomAddress, m_icomPort);
}

void CIcomRepeaterProtocolHandler::sendSingleReply(const CHeaderData& header)
{
	unsigned int id = ::rand() % 0xFFFF;

	CHeaderData replyHdr;
	replyHdr.setId(id);
	replyHdr.setBand1(0xFFU);
	replyHdr.setBand2(0xFFU);
	replyHdr.setBand3(0xFFU);
	replyHdr.setFlag1(0x01U);
	replyHdr.setMyCall1(header.getRptCall2());
	replyHdr.setMyCall2(wxT("    "));
	replyHdr.setYourCall(header.getMyCall1());
	replyHdr.setRptCall1(header.getRptCall2());
	replyHdr.setRptCall2(header.getRptCall1());

	writeHeader(replyHdr);

	unsigned char buffer[DV_FRAME_MAX_LENGTH_BYTES];
	::memcpy(buffer + 0U, NULL_AMBE_DATA_BYTES, VOICE_FRAME_LENGTH_BYTES);
	::memcpy(buffer + VOICE_FRAME_LENGTH_BYTES, END_PATTERN_BYTES, END_PATTERN_LENGTH_BYTES);

	CAMBEData replyData;
	replyData.setId(id);
	replyData.setBand1(0xFFU);
	replyData.setBand2(0xFFU);
	replyData.setBand3(0xFFU);
	replyData.setSeq(0x40U);		// Seq = 0 and end-of-data
	replyData.setData(buffer, DV_FRAME_MAX_LENGTH_BYTES);

	writeAMBE(replyData);
}

void CIcomRepeaterProtocolHandler::sendMultiReply(const CHeaderData& header)
{
	unsigned int id = ::rand() % 0xFFFF;

	CHeaderData replyHdr;
	replyHdr.setId(id);
	replyHdr.setBand1(0xFFU);
	replyHdr.setBand2(0xFFU);
	replyHdr.setBand3(0xFFU);
	replyHdr.setFlag1(0x41U);
	replyHdr.setMyCall1(header.getRptCall2());
	replyHdr.setMyCall2(wxT("    "));
	replyHdr.setYourCall(header.getMyCall1());
	replyHdr.setRptCall1(header.getRptCall2());
	replyHdr.setRptCall2(header.getRptCall1());

	writeHeader(replyHdr);

	CAMBEData replyData;
	unsigned char buffer[DV_FRAME_LENGTH_BYTES];

	replyData.setId(id);
	replyData.setBand1(0xFFU);
	replyData.setBand2(0xFFU);
	replyData.setBand3(0xFFU);

	::memcpy(buffer + 0U, NULL_AMBE_DATA_BYTES, VOICE_FRAME_LENGTH_BYTES);
	::memcpy(buffer + VOICE_FRAME_LENGTH_BYTES, END_PATTERN_BYTES + 0U, 3U);

	replyData.setSeq(0x00U);		// Seq = 0
	replyData.setData(buffer, DV_FRAME_LENGTH_BYTES);

	writeAMBE(replyData);

	::memset(buffer + 0U, 0x00, DV_FRAME_LENGTH_BYTES);
	::memcpy(buffer + 0U, END_PATTERN_BYTES + 3U, 3U);

	replyData.setSeq(0x41U);		// Seq = 1 and end-of-data
	replyData.setData(buffer, DV_FRAME_LENGTH_BYTES);

	writeAMBE(replyData);
}

void CIcomRepeaterProtocolHandler::free(CDataQueue* dataQueue)
{
	if (dataQueue == NULL)
		return;

	switch (dataQueue->getType()) {
		case RT_HEADER:
			delete dataQueue->getHeader();
			break;
		case RT_HEARD:
			delete dataQueue->getHeard();
			break;
		case RT_AMBE:
			delete dataQueue->getAMBE();
			break;
		case RT_DD:
			delete dataQueue->getDD();
			break;
		default:
			break;
	}

	delete dataQueue;
}
