/*
 *   Copyright (C) 2011-2016 by Jonathan Naylor G4KLX
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

#include "CCITTChecksumReverse.h"
#include "MMDVMController.h"
#include "DStarDefines.h"
#include "Timer.h"

#if defined(__WINDOWS__)
#include <setupapi.h>
#else
#include <wx/dir.h>
#endif

const unsigned char MMDVM_FRAME_START  = 0xE0U;

const unsigned char MMDVM_GET_VERSION  = 0x00U;
const unsigned char MMDVM_GET_STATUS   = 0x01U;
const unsigned char MMDVM_SET_CONFIG   = 0x02U;

const unsigned char MMDVM_DSTAR_HEADER = 0x10U;
const unsigned char MMDVM_DSTAR_DATA   = 0x11U;
const unsigned char MMDVM_DSTAR_LOST   = 0x12U;
const unsigned char MMDVM_DSTAR_EOT    = 0x13U;

const unsigned char MMDVM_ACK          = 0x70U;
const unsigned char MMDVM_NAK          = 0x7FU;

const unsigned char MMDVM_DUMP         = 0xF0U;
const unsigned char MMDVM_DEBUG1       = 0xF1U;
const unsigned char MMDVM_DEBUG2       = 0xF2U;
const unsigned char MMDVM_DEBUG3       = 0xF3U;
const unsigned char MMDVM_DEBUG4       = 0xF4U;
const unsigned char MMDVM_DEBUG5       = 0xF5U;

const unsigned int MAX_RESPONSES = 30U;

const unsigned int BUFFER_LENGTH = 200U;

CMMDVMController::CMMDVMController(const wxString& port, const wxString& path, bool rxInvert, bool txInvert, bool pttInvert, unsigned int txDelay, unsigned int rxLevel, unsigned int txLevel) :
CModem(),
m_port(port),
m_path(path),
m_rxInvert(rxInvert),
m_txInvert(txInvert),
m_pttInvert(pttInvert),
m_txDelay(txDelay),
m_txLevel(txLevel),
m_rxLevel(rxLevel),
m_serial(port, SERIAL_115200, true),
m_buffer(NULL),
m_txData(1000U),
m_rx(false)
{
	wxASSERT(!port.IsEmpty());

	m_buffer = new unsigned char[BUFFER_LENGTH];
}

CMMDVMController::~CMMDVMController()
{
	delete[] m_buffer;
}

bool CMMDVMController::start()
{
	findPort();

	bool ret = openModem();
	if (!ret)
		return false;

	findPath();

	Create();
	SetPriority(100U);
	Run();

	return true;
}

void* CMMDVMController::Entry()
{
	wxLogMessage(wxT("Starting MMDVM Controller thread"));

	// Clock every 5ms-ish
	CTimer pollTimer(200U, 0U, 100U);
	pollTimer.start();

	unsigned char  writeType   = DSMTT_NONE;
	unsigned char  writeLength = 0U;
	unsigned char* writeBuffer = new unsigned char[BUFFER_LENGTH];

	unsigned int space = 0U;

	while (!m_stopped) {
		// Poll the modem status every 100ms
		if (pollTimer.hasExpired()) {
			bool ret = readStatus();
			if (!ret) {
				ret = findModem();
				if (!ret) {
					wxLogError(wxT("Stopping MMDVM Controller thread"));
					return NULL;
				}
			}

			pollTimer.start();
		}

		unsigned int length;
		RESP_TYPE_MMDVM type = getResponse(m_buffer, length);

		switch (type) {
			case RTDVM_TIMEOUT:
				break;

			case RTDVM_ERROR: {
					bool ret = findModem();
					if (!ret) {
						wxLogError(wxT("Stopping MMDVM Controller thread"));
						return NULL;
					}
				}
				break;

			case RTDVM_DSTAR_HEADER: {
					// CUtils::dump(wxT("RT_DSTAR_HEADER"), m_buffer, length);
					wxMutexLocker locker(m_mutex);

					unsigned char data[2U];
					data[0U] = DSMTT_HEADER;
					data[1U] = RADIO_HEADER_LENGTH_BYTES;
					m_rxData.addData(data, 2U);

					m_rxData.addData(m_buffer + 3U, RADIO_HEADER_LENGTH_BYTES);

					m_rx = true;
				}
				break;

			case RTDVM_DSTAR_DATA: {
					// CUtils::dump(wxT("RT_DSTAR_DATA"), m_buffer, length);
					wxMutexLocker locker(m_mutex);

					unsigned char data[2U];
					data[0U] = DSMTT_DATA;
					data[1U] = DV_FRAME_LENGTH_BYTES;
					m_rxData.addData(data, 2U);

					m_rxData.addData(m_buffer + 3U, DV_FRAME_LENGTH_BYTES);

					m_rx = true;
				}
				break;

			case RTDVM_DSTAR_EOT: {
					// wxLogMessage(wxT("RT_DSTAR_EOT"));
					wxMutexLocker locker(m_mutex);

					unsigned char data[2U];
					data[0U] = DSMTT_EOT;
					data[1U] = 0U;
					m_rxData.addData(data, 2U);

					m_rx = false;
				}
				break;

			case RTDVM_DSTAR_LOST: {
					// wxLogMessage(wxT("RT_DSTAR_LOST"));
					wxMutexLocker locker(m_mutex);

					unsigned char data[2U];
					data[0U] = DSMTT_LOST;
					data[1U] = 0U;
					m_rxData.addData(data, 2U);

					m_rx = false;
				}
				break;

			case RTDVM_GET_STATUS: {
					bool dstar = (m_buffer[3U] & 0x01U) == 0x01U;
					if (!dstar) {
						wxLogError(wxT("D-Star not enabled in the MMDVM!!!"));
						wxLogError(wxT("Stopping MMDVM Controller thread"));
						return NULL;
					}

					m_tx  = (m_buffer[5U] & 0x01U) == 0x01U;

					bool adcOverflow = (m_buffer[5U] & 0x02U) == 0x02U;
					if (adcOverflow)
						wxLogWarning(wxT("MMDVM ADC levels have overflowed"));

					space = m_buffer[6U];
					// CUtils::dump(wxT("GET_STATUS"), m_buffer, length);
					// wxLogMessage(wxT("PTT=%d space=%u"), int(m_tx), space);
				}
				break;

			// These should not be received in this loop, but don't complain if we do
			case RTDVM_GET_VERSION:
			case RTDVM_ACK:
				break;

			case RTDVM_NAK: {
					switch (m_buffer[3U]) {
						case MMDVM_DSTAR_HEADER:
							wxLogWarning(wxT("Received a header NAK from the MMDVM, reason = %u"), m_buffer[4U]);
							break;
						case MMDVM_DSTAR_DATA:
							wxLogWarning(wxT("Received a data NAK from the MMDVM, reason = %u"), m_buffer[4U]);
							break;
						case MMDVM_DSTAR_EOT:
							wxLogWarning(wxT("Received an EOT NAK from the MMDVM, reason = %u"), m_buffer[4U]);
							break;
						default:
							wxLogWarning(wxT("Received a NAK from the MMDVM, command = 0x%02X, reason = %u"), m_buffer[3U], m_buffer[4U]);
							break;
					}
				}
				break;

			case RTDVM_DUMP:
				CUtils::dump(wxT("Modem dump"), m_buffer + 3U, length - 3U);
				break;

			case RTDVM_DEBUG1:
			case RTDVM_DEBUG2:
			case RTDVM_DEBUG3:
			case RTDVM_DEBUG4:
			case RTDVM_DEBUG5:
				printDebug();
				break;

			default:
				wxLogMessage(wxT("Unknown message, type: %02X"), m_buffer[2U]);
				CUtils::dump(wxT("Buffer dump"), m_buffer, length);
				break;
		}

		if (writeType == DSMTT_NONE && m_txData.hasData()) {
			wxMutexLocker locker(m_mutex);

			m_txData.getData(&writeType, 1U);
			m_txData.getData(&writeLength, 1U);
			m_txData.getData(writeBuffer, writeLength);
		}

		if (space > 4U && writeType == DSMTT_HEADER) {
			// CUtils::dump(wxT("Write Header"), writeBuffer, writeLength);

			int ret = m_serial.write(writeBuffer, writeLength);
			if (ret != int(writeLength))
				wxLogWarning(wxT("Error when writing the header to the MMDVM"));

			writeType = DSMTT_NONE;
			space -= 4U;
		}

		if (space > 1U && (writeType == DSMTT_DATA || writeType == DSMTT_EOT)) {
			// CUtils::dump(wxT("Write Data"), writeBuffer, writeLength);

			int ret = m_serial.write(writeBuffer, writeLength);
			if (ret != int(writeLength))
				wxLogWarning(wxT("Error when writing data to the MMDVM"));

			writeType = DSMTT_NONE;
			space--;
		}

		Sleep(5UL);

		pollTimer.clock();
	}

	wxLogMessage(wxT("Stopping MMDVM Controller thread"));

	delete[] writeBuffer;

	m_serial.close();

	return NULL;
}

bool CMMDVMController::writeHeader(const CHeaderData& header)
{
	bool ret = m_txData.hasSpace(46U);
	if (!ret) {
		wxLogWarning(wxT("No space to write the header"));
		return false;
	}

	unsigned char buffer[50U];

	buffer[0U] = MMDVM_FRAME_START;
	buffer[1U] = RADIO_HEADER_LENGTH_BYTES + 3U;
	buffer[2U] = MMDVM_DSTAR_HEADER;

	::memset(buffer + 3U, ' ', RADIO_HEADER_LENGTH_BYTES);

	buffer[3U] = header.getFlag1();
	buffer[4U] = header.getFlag2();
	buffer[5U] = header.getFlag3();

	wxString rpt2 = header.getRptCall2();
	for (unsigned int i = 0U; i < rpt2.Len() && i < LONG_CALLSIGN_LENGTH; i++)
		buffer[i + 6U]  = rpt2.GetChar(i);

	wxString rpt1 = header.getRptCall1();
	for (unsigned int i = 0U; i < rpt1.Len() && i < LONG_CALLSIGN_LENGTH; i++)
		buffer[i + 14U] = rpt1.GetChar(i);

	wxString your = header.getYourCall();
	for (unsigned int i = 0U; i < your.Len() && i < LONG_CALLSIGN_LENGTH; i++)
		buffer[i + 22U] = your.GetChar(i);

	wxString my1 = header.getMyCall1();
	for (unsigned int i = 0U; i < my1.Len() && i < LONG_CALLSIGN_LENGTH; i++)
		buffer[i + 30U] = my1.GetChar(i);

	wxString my2 = header.getMyCall2();
	for (unsigned int i = 0U; i < my2.Len() && i < SHORT_CALLSIGN_LENGTH; i++)
		buffer[i + 38U] = my2.GetChar(i);

	CCCITTChecksumReverse cksum1;
	cksum1.update(buffer + 3U, RADIO_HEADER_LENGTH_BYTES - 2U);
	cksum1.result(buffer + 42U);

	wxMutexLocker locker(m_mutex);

	unsigned char type = DSMTT_HEADER;
	m_txData.addData(&type, 1U);

	unsigned char len = RADIO_HEADER_LENGTH_BYTES + 3U;
	m_txData.addData(&len, 1U);

	m_txData.addData(buffer, RADIO_HEADER_LENGTH_BYTES + 3U);

	return true;
}

bool CMMDVMController::writeData(const unsigned char* data, unsigned int length, bool end)
{
	bool ret = m_txData.hasSpace(17U);
	if (!ret) {
		wxLogWarning(wxT("No space to write data"));
		return false;
	}

	unsigned char buffer[20U];

	if (end) {
		buffer[0U] = MMDVM_FRAME_START;
		buffer[1U] = 3U;
		buffer[2U] = MMDVM_DSTAR_EOT;

		wxMutexLocker locker(m_mutex);

		unsigned char type = DSMTT_EOT;
		m_txData.addData(&type, 1U);

		unsigned char len = 3U;
		m_txData.addData(&len, 1U);

		m_txData.addData(buffer, 3U);

		return true;
	}

	buffer[0U] = MMDVM_FRAME_START;
	buffer[1U] = DV_FRAME_LENGTH_BYTES + 3U;
	buffer[2U] = MMDVM_DSTAR_DATA;
	::memcpy(buffer + 3U, data, DV_FRAME_LENGTH_BYTES);

	wxMutexLocker locker(m_mutex);

	unsigned char type = DSMTT_DATA;
	m_txData.addData(&type, 1U);

	unsigned char len = DV_FRAME_LENGTH_BYTES + 3U;
	m_txData.addData(&len, 1U);

	m_txData.addData(buffer, DV_FRAME_LENGTH_BYTES + 3U);

	return true;
}

unsigned int CMMDVMController::getSpace()
{
	return m_txData.freeSpace() / (DV_FRAME_LENGTH_BYTES + 5U);
}

bool CMMDVMController::isTXReady()
{
	if (m_tx)
		return false;

	return m_txData.isEmpty();
}

bool CMMDVMController::readVersion()
{
	::wxSleep(2);

	for (unsigned int i = 0U; i < 6U; i++) {
		unsigned char buffer[3U];

		buffer[0U] = MMDVM_FRAME_START;
		buffer[1U] = 3U;
		buffer[2U] = MMDVM_GET_VERSION;

		// CUtils::dump(wxT("Written"), buffer, 3U);

		int ret = m_serial.write(buffer, 3U);
		if (ret != 3)
			return false;

		for (unsigned int count = 0U; count < MAX_RESPONSES; count++) {
			::wxMilliSleep(10UL);

			unsigned int length;
			RESP_TYPE_MMDVM resp = getResponse(m_buffer, length);
			if (resp == RTDVM_GET_VERSION) {
				wxString description((char*)(m_buffer + 4U), wxConvLocal, length - 4U);

				wxLogInfo(wxT("MMDVM protocol version: %u, description: %s"), m_buffer[3U], description.c_str());

				return true;
			}
		}

		::wxSleep(1);
	}

	wxLogError(wxT("Unable to read the firmware version after six attempts"));

	return false;
}

bool CMMDVMController::readStatus()
{
	unsigned char buffer[3U];

	buffer[0U] = MMDVM_FRAME_START;
	buffer[1U] = 3U;
	buffer[2U] = MMDVM_GET_STATUS;

	return m_serial.write(buffer, 3U) == 3;
}

bool CMMDVMController::setConfig()
{
	unsigned char buffer[25U];

	buffer[0U] = MMDVM_FRAME_START;

	buffer[1U] = 19U;

	buffer[2U] = MMDVM_SET_CONFIG;

	buffer[3U] = 0x00U;
	if (m_rxInvert)
		buffer[3U] |= 0x01U;
	if (m_txInvert)
		buffer[3U] |= 0x02U;
	if (m_pttInvert)
		buffer[3U] |= 0x04U;

	buffer[4U] = 0x01U;				// D-Star only

	buffer[5U] = m_txDelay / 10U;	// In 10ms units

	buffer[6U] = 1U;				// STATE_DSTAR

	buffer[7U] = (m_rxLevel * 255U) / 100U;
	buffer[8U] = (m_txLevel * 255U) / 100U;

	buffer[9U] = 0U;				// DMR Color Code

	buffer[10U] = 0U;				// DMR Delay

	buffer[11U] = 0U;				// Osc. Offset

	buffer[12U] = (m_txLevel * 255U) / 100U;
	buffer[13U] = (m_txLevel * 255U) / 100U;
	buffer[14U] = (m_txLevel * 255U) / 100U;
	buffer[15U] = (m_txLevel * 255U) / 100U;
	
	buffer[16U] = 128U;
	buffer[17U] = 128U;

	buffer[18U] = (m_txLevel * 255U) / 100U;

	// CUtils::dump(wxT("Written"), buffer, 19U);

	int ret = m_serial.write(buffer, 19U);
	if (ret != 19U)
		return false;

	unsigned int count = 0U;
	unsigned int length;
	RESP_TYPE_MMDVM resp;
	do {
		::wxMilliSleep(10UL);

		resp = getResponse(m_buffer, length);

		if (resp != RTDVM_ACK && resp != RTDVM_NAK) {
			count++;
			if (count >= MAX_RESPONSES) {
				wxLogError(wxT("The MMDVM is not responding to the SET_CONFIG command"));
				return false;
			}
		}
	} while (resp != RTDVM_ACK && resp != RTDVM_NAK);

	// CUtils::dump(wxT("Response"), m_buffer, length);

	if (resp == RTDVM_NAK) {
		wxLogError(wxT("Received a NAK to the SET_CONFIG command from the modem"));
		return false;
	}

	return true;
}

RESP_TYPE_MMDVM CMMDVMController::getResponse(unsigned char *buffer, unsigned int& length)
{
	// Get the start of the frame or nothing at all
	int ret = m_serial.read(buffer + 0U, 1U);
	if (ret < 0) {
		wxLogError(wxT("Error when reading from the MMDVM"));
		return RTDVM_ERROR;
	}

	if (ret == 0)
		return RTDVM_TIMEOUT;

	if (buffer[0U] != MMDVM_FRAME_START)
		return RTDVM_TIMEOUT;

	ret = m_serial.read(buffer + 1U, 1U);
	if (ret < 0) {
		wxLogError(wxT("Error when reading from the MMDVM"));
		return RTDVM_ERROR;
	}

	if (ret == 0)
		return RTDVM_TIMEOUT;

	length = buffer[1U];

	if (length >= 100U) {
		wxLogError(wxT("Invalid data received from the MMDVM"));
		CUtils::dump(wxT("Data"), buffer, 2U);
		return RTDVM_TIMEOUT;
	}

	unsigned int offset = 2U;

	while (offset < length) {
		int ret = m_serial.read(buffer + offset, length - offset);
		if (ret < 0) {
			wxLogError(wxT("Error when reading from the MMDVM"));
			return RTDVM_ERROR;
		}

		if (ret > 0)
			offset += ret;

		if (ret == 0)
			Sleep(5UL);
	}

	// CUtils::dump(wxT("Received"), buffer, length);

	switch (buffer[2U]) {
		case MMDVM_GET_STATUS:
			return RTDVM_GET_STATUS;
		case MMDVM_GET_VERSION:
			return RTDVM_GET_VERSION;
		case MMDVM_DSTAR_HEADER:
			return RTDVM_DSTAR_HEADER;
		case MMDVM_DSTAR_DATA:
			return RTDVM_DSTAR_DATA;
		case MMDVM_DSTAR_EOT:
			return RTDVM_DSTAR_EOT;
		case MMDVM_DSTAR_LOST:
			return RTDVM_DSTAR_LOST;
		case MMDVM_ACK:
			return RTDVM_ACK;
		case MMDVM_NAK:
			return RTDVM_NAK;
		case MMDVM_DUMP:
			return RTDVM_DUMP;
		case MMDVM_DEBUG1:
			return RTDVM_DEBUG1;
		case MMDVM_DEBUG2:
			return RTDVM_DEBUG2;
		case MMDVM_DEBUG3:
			return RTDVM_DEBUG3;
		case MMDVM_DEBUG4:
			return RTDVM_DEBUG4;
		case MMDVM_DEBUG5:
			return RTDVM_DEBUG5;
		default:
			return RTDVM_UNKNOWN;
	}
}

wxString CMMDVMController::getPath() const
{
	return m_path;
}

bool CMMDVMController::findPort()
{
	if (m_path.IsEmpty())
		return false;

#if defined(__WINDOWS__)
#else
	wxDir dir;
	bool ret1 = dir.Open(wxT("/sys/class/tty"));
	if (!ret1) {
		wxLogError(wxT("Cannot open directory /sys/class/tty"));
		return false;
	}

	wxString fileName;
	ret1 = dir.GetFirst(&fileName, wxT("ttyACM*"));
	while (ret1) {
		wxString path;
		path.Printf(wxT("/sys/class/tty/%s"), fileName.c_str());

		char cpath[255U];
		::memset(cpath, 0x00U, 255U);

		for (unsigned int i = 0U; i < path.Len(); i++)
			cpath[i] = path.GetChar(i);

		char symlink[255U];
		int ret2 = ::readlink(cpath, symlink, 255U);
		if (ret2 < 0) {
			::strcat(cpath, "/device");
			ret2 = ::readlink(cpath, symlink, 255U);
			if (ret2 < 0) {
				wxLogError(wxT("Error from readlink()"));
				return false;
			}

			path = wxString(symlink, wxConvLocal, ret2);
		} else {
			// Get all but the last section
			wxString fullPath = wxString(symlink, wxConvLocal, ret2);
			path = fullPath.BeforeLast(wxT('/'));
		}

		if (path.IsSameAs(m_path)) {
			m_port.Printf(wxT("/dev/%s"), fileName.c_str());

			wxLogMessage(wxT("Found modem port of %s based on the path"), m_port.c_str());

			return true;
		}

		ret1 = dir.GetNext(&fileName);
	}
#endif

	return false;
}

bool CMMDVMController::findPath()
{
#if defined(__WINDOWS__)
#ifdef notdef
	GUID guids[5U];

	DWORD count;
	BOOL res = ::SetupDiClassGuidsFromName(L"Multifunction", guids, 5U, &count);
	if (!res) {
		wxLogError(wxT("Error from SetupDiClassGuidsFromName: err=%u"), ::GetLastError());
		return false;
	}

	for (DWORD i = 0U; i < count; i++) {
		HDEVINFO devInfo = ::SetupDiGetClassDevs(&guids[i], NULL, NULL, DIGCF_DEVICEINTERFACE | DIGCF_PRESENT);
		if (devInfo == INVALID_HANDLE_VALUE) {
			wxLogError(wxT("Error from SetupDiGetClassDevs: err=%u"), ::GetLastError());
			return false;
		}

		SP_DEVICE_INTERFACE_DATA devInfoData;
		devInfoData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);

		for (DWORD index = 0U; ::SetupDiEnumDeviceInterfaces(devInfo, NULL, &guids[i], index, &devInfoData); index++) {
			// Find the required length of the device structure
			DWORD length;
			::SetupDiGetDeviceInterfaceDetail(devInfo, &devInfoData, NULL, 0U, &length, NULL);

			PSP_DEVICE_INTERFACE_DETAIL_DATA detailData = PSP_DEVICE_INTERFACE_DETAIL_DATA(::malloc(length));
			detailData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

			// Get the detailed data into the newly allocated device structure
			DWORD required;
			res = ::SetupDiGetDeviceInterfaceDetail(devInfo, &devInfoData, detailData, length, &required, NULL);
			if (!res) {
				wxLogError(wxT("Error from SetupDiGetDeviceInterfaceDetail: err=%u"), ::GetLastError());
				::SetupDiDestroyDeviceInfoList(devInfo);
				::free(detailData);
				return false;
			}

			::free(detailData);
		}

		::SetupDiDestroyDeviceInfoList(devInfo);
	}

	return false;
#endif
#else
	wxString path;
	path.Printf(wxT("/sys/class/tty/%s"), m_port.Mid(5U).c_str());	

	char cpath[255U];
	::memset(cpath, 0x00U, 255U);

	for (unsigned int i = 0U; i < path.Len(); i++)
		cpath[i] = path.GetChar(i);

	char symlink[255U];
	int ret = ::readlink(cpath, symlink, 255U);
	if (ret < 0) {
		::strcat(cpath, "/device");
		ret = ::readlink(cpath, symlink, 255U);
		if (ret < 0) {
			wxLogError(wxT("Error from readlink()"));
			return false;
		}

		path = wxString(symlink, wxConvLocal, ret);
	} else {
		wxString fullPath = wxString(symlink, wxConvLocal, ret);
		path = fullPath.BeforeLast(wxT('/'));
	}

	if (m_path.IsEmpty())
		wxLogMessage(wxT("Found modem path of %s"), path.c_str());

	m_path = path;
#endif

	return true;
}

bool CMMDVMController::findModem()
{
	m_serial.close();

	// Tell the repeater that the signal has gone away
	if (m_rx) {
		wxMutexLocker locker(m_mutex);

		unsigned char data[2U];
		data[0U] = DSMTT_EOT;
		data[1U] = 0U;
		m_rxData.addData(data, 2U);

		m_rx = false;
	}

	unsigned int count = 0U;

	// Purge the transmit buffer every 500ms to avoid overflow, but only try and reopen the modem every 2s
	while (!m_stopped) {
		count++;
		if (count >= 4U) {
			wxLogMessage(wxT("Trying to reopen the modem"));

			bool ret = findPort();
			if (ret) {
				ret = openModem();
				if (ret)
					return true;
			}

			count = 0U;
		}

		Sleep(500UL);
	}

	return false;
}

bool CMMDVMController::openModem()
{
	bool ret = m_serial.open();
	if (!ret)
		return false;

	ret = readVersion();
	if (!ret) {
		m_serial.close();
		return false;
	}

	ret = setConfig();
	if (!ret) {
		m_serial.close();
		return false;
	}

	return true;
}

void CMMDVMController::printDebug()
{
	unsigned int length = m_buffer[1U];
	if (m_buffer[2U] == 0xF1U) {
		wxString message((char*)(m_buffer + 3U), wxConvLocal, length - 3U);
		wxLogMessage(wxT("Debug: %s"), message.c_str());
	} else if (m_buffer[2U] == 0xF2U) {
		wxString message((char*)(m_buffer + 3U), wxConvLocal, length - 5U);
		short val1 = (m_buffer[length - 2U] << 8) | m_buffer[length - 1U];
		wxLogMessage(wxT("Debug: %s %d"), message.c_str(), val1);
	} else if (m_buffer[2U] == 0xF3U) {
		wxString message((char*)(m_buffer + 3U), wxConvLocal, length - 7U);
		short val1 = (m_buffer[length - 4U] << 8) | m_buffer[length - 3U];
		short val2 = (m_buffer[length - 2U] << 8) | m_buffer[length - 1U];
		wxLogMessage(wxT("Debug: %s %d %d"), message.c_str(), val1, val2);
	} else if (m_buffer[2U] == 0xF4U) {
		wxString message((char*)(m_buffer + 3U), wxConvLocal, length - 9U);
		short val1 = (m_buffer[length - 6U] << 8) | m_buffer[length - 5U];
		short val2 = (m_buffer[length - 4U] << 8) | m_buffer[length - 3U];
		short val3 = (m_buffer[length - 2U] << 8) | m_buffer[length - 1U];
		wxLogMessage(wxT("Debug: %s %d %d %d"), message.c_str(), val1, val2, val3);
	} else if (m_buffer[2U] == 0xF5U) {
		wxString message((char*)(m_buffer + 3U), wxConvLocal, length - 11U);
		short val1 = (m_buffer[length - 8U] << 8) | m_buffer[length - 7U];
		short val2 = (m_buffer[length - 6U] << 8) | m_buffer[length - 5U];
		short val3 = (m_buffer[length - 4U] << 8) | m_buffer[length - 3U];
		short val4 = (m_buffer[length - 2U] << 8) | m_buffer[length - 1U];
		wxLogMessage(wxT("Debug: %s %d %d %d %d"), message.c_str(), val1, val2, val3, val4);
	}
}
