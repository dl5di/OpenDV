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

#include "GMSKModemWinUSB.h"
#include "DStarDefines.h"

#include <Setupapi.h>

const unsigned char SET_AD_INIT     = 0x00U;
const unsigned char SET_PTT         = 0x05U;
const unsigned char PUT_DATA        = 0x10U;
const unsigned char GET_DATA        = 0x11U;
const unsigned char GET_HEADER      = 0x21U;
const unsigned char GET_AD_STATUS   = 0x30U;
const unsigned char SET_MyCALL      = 0x40U;
const unsigned char SET_MyCALL2     = 0x41U;
const unsigned char SET_YourCALL    = 0x42U;
const unsigned char SET_RPT1CALL    = 0x43U;
const unsigned char SET_RPT2CALL    = 0x44U;
const unsigned char SET_FLAGS       = 0x45U;
const unsigned char GET_REMAINSPACE = 0x50U;
const unsigned char GET_VERSION     = 0xFFU;

const unsigned char COS_OnOff  = 0x02U;
const unsigned char CRC_ERROR  = 0x04U;
const unsigned char LAST_FRAME = 0x08U;
const unsigned char PTT_OnOff  = 0x20U;

const unsigned char PTT_ON  = 1U;
const unsigned char PTT_OFF = 0U;

CGMSKModemWinUSB::CGMSKModemWinUSB(unsigned int address) :
m_address(address),
m_file(INVALID_HANDLE_VALUE),
m_handle(INVALID_HANDLE_VALUE),
m_broken(false)
{
}

CGMSKModemWinUSB::~CGMSKModemWinUSB()
{
}

bool CGMSKModemWinUSB::open()
{
	wxASSERT(m_handle == INVALID_HANDLE_VALUE);

	bool res = openModem();
	if (!res) {
		wxLogError(wxT("Cannot find the GMSK Modem with address: 0x%04X"), m_address);
		return false;
	}

	wxLogInfo(wxT("Found the GMSK Modem with address: 0x%04X"), m_address);

	int ret = io(SET_AD_INIT, 0x40U, 0U, NULL, 0U);
	if (ret < 0) {
		wxLogError(wxT("SET_AD_INIT returned %d"), -ret);
		close();
		return false;
	}

	wxString version;

	do {
		unsigned char buffer[GMSK_MODEM_DATA_LENGTH];
		ret = io(GET_VERSION, 0xC0U, 0U, buffer, GMSK_MODEM_DATA_LENGTH);
		if (ret > 0) {
			wxString text((char*)buffer, wxConvLocal, ret);
			version.Append(text);
		} else if (ret < 0) {
			wxLogError(wxT("GET_VERSION returned %d"), -ret);
			close();
			return false;
		}
	} while (ret == int(GMSK_MODEM_DATA_LENGTH));

	wxLogInfo(wxT("Firmware version: %s"), version.c_str());

	// Trap firmware version 0.1.00 of DUTCH*Star and complain loudly
	if (version.Find(wxT("DUTCH*Star")) != wxNOT_FOUND && version.Find(wxT("0.1.00")) != wxNOT_FOUND) {
		wxLogWarning(wxT("This modem firmware is not fully supported by the GMSK Repeater"));
		wxLogWarning(wxT("Please upgrade to a newer version if possible"));
		m_broken = true;
	}

	setPTT(false);

	return true;
}

bool CGMSKModemWinUSB::isBroken() const
{
	return m_broken;
}

CHeaderData* CGMSKModemWinUSB::readHeader()
{
	wxASSERT(m_handle != INVALID_HANDLE_VALUE);

	unsigned char header[RADIO_HEADER_LENGTH_BYTES * 2U];
	unsigned int length = 0U;

	while (length < RADIO_HEADER_LENGTH_BYTES) {
		int ret = io(GET_HEADER, 0xC0U, 0U, header + length, GMSK_MODEM_DATA_LENGTH);
		if (ret < 0) {
			wxLogError(wxT("GET_HEADER returned %d"), -ret);
			return NULL;
		} else if (ret == 0) {
			if (length == 0U)
				return NULL;

			::wxMilliSleep(10UL);

			unsigned char status;
			ret = io(GET_AD_STATUS, 0xC0U, 0U, &status, 1U);
			if (ret < 0) {
				wxLogError(wxT("GET_COS returned %d"), -ret);
				return NULL;
			} else if (ret > 0) {
				if ((status & COS_OnOff) == COS_OnOff)
					length = 0U;
			}
		} else {
			length += ret;
		}
	}

	unsigned char status;
	int ret = io(GET_AD_STATUS, 0xC0U, 0U, &status, 1U);
	if (ret < 0) {
		wxLogError(wxT("GET_CRC returned %d"), -ret);
		return NULL;
	}

	if ((status & CRC_ERROR) == CRC_ERROR) {
		wxLogMessage(wxT("Invalid CRC on header"));
		return NULL;
	}

	return new CHeaderData(header, RADIO_HEADER_LENGTH_BYTES, false);
}

int CGMSKModemWinUSB::readData(unsigned char* data, unsigned int length, bool& end)
{
	wxASSERT(m_handle != INVALID_HANDLE_VALUE);
	wxASSERT(data != NULL);
	wxASSERT(length > 0U);

	end = false;

	int ret = io(GET_DATA, 0xC0U, 0U, data, GMSK_MODEM_DATA_LENGTH);
	if (ret < 0) {
		wxLogError(wxT("GET_DATA returned %d"), -ret);
		return ret;
	} else if (ret == 0) {
		unsigned char status;
		int ret = io(GET_AD_STATUS, 0xC0U, 0U, &status, 1U);
		if (ret < 0) {
			wxLogError(wxT("LAST_FRAME returned %d"), -ret);
			return ret;
		}

		if ((status & LAST_FRAME) == LAST_FRAME)
			end = true;
	}

	return ret;
}

bool CGMSKModemWinUSB::writeHeader(const CHeaderData& header)
{
	wxASSERT(m_handle != INVALID_HANDLE_VALUE);

	unsigned char myCall1[LONG_CALLSIGN_LENGTH];
	unsigned char myCall2[SHORT_CALLSIGN_LENGTH];
	unsigned char yourCall[LONG_CALLSIGN_LENGTH];
	unsigned char rptCall1[LONG_CALLSIGN_LENGTH];
	unsigned char rptCall2[LONG_CALLSIGN_LENGTH];

	for (unsigned int i = 0U; i < LONG_CALLSIGN_LENGTH; i++)
		myCall1[i] = header.getMyCall1().GetChar(i);
	for (unsigned int i = 0U; i < SHORT_CALLSIGN_LENGTH; i++)
		myCall2[i] = header.getMyCall2().GetChar(i);
	for (unsigned int i = 0U; i < LONG_CALLSIGN_LENGTH; i++)
		yourCall[i] = header.getYourCall().GetChar(i);
	for (unsigned int i = 0U; i < LONG_CALLSIGN_LENGTH; i++)
		rptCall1[i] = header.getRptCall1().GetChar(i);
	for (unsigned int i = 0U; i < LONG_CALLSIGN_LENGTH; i++)
		rptCall2[i] = header.getRptCall2().GetChar(i);

	int ret = io(SET_MyCALL2, 0x40U, 0U, myCall2, SHORT_CALLSIGN_LENGTH);
	if (ret < 0) {
		wxLogError(wxT("SET_MyCALL2 returned %d"), -ret);
		return false;
	}

	ret = io(SET_MyCALL, 0x40U, 0U, myCall1, LONG_CALLSIGN_LENGTH);
	if (ret < 0) {
		wxLogError(wxT("SET_MyCALL returned %d"), -ret);
		return false;
	}

	ret = io(SET_YourCALL, 0x40U, 0U, yourCall, LONG_CALLSIGN_LENGTH);
	if (ret < 0) {
		wxLogError(wxT("SET_YourCALL returned %d"), -ret);
		return false;
	}

	ret = io(SET_RPT1CALL, 0x40U, 0U, rptCall1, LONG_CALLSIGN_LENGTH);
	if (ret < 0) {
		wxLogError(wxT("SET_RPT1CALL returned %d"), -ret);
		return false;
	}

	ret = io(SET_RPT2CALL, 0x40U, 0U, rptCall2, LONG_CALLSIGN_LENGTH);
	if (ret < 0) {
		wxLogError(wxT("SET_RPT2CALL returned %d"), -ret);
		return false;
	}

	unsigned char flags[3U];
	flags[0U] = header.getFlag1();
	flags[1U] = header.getFlag2();
	flags[2U] = header.getFlag3();

	ret = io(SET_FLAGS, 0x40U, 0U, flags, 3U);
	if (ret < 0) {
		wxLogError(wxT("SET_FLAGS returned %d"), -ret);
		return false;
	}

	return true;
}

TRISTATE CGMSKModemWinUSB::getPTT()
{
	wxASSERT(m_handle != INVALID_HANDLE_VALUE);

	unsigned char status;
	int ret = io(GET_AD_STATUS, 0xC0U, 0U, &status, 1U);
	if (ret != 1) {
		wxLogError(wxT("GET_PTT returned %d"), -ret);
		return STATE_UNKNOWN;
	}

	if ((status & PTT_OnOff) == PTT_OnOff)
		return STATE_TRUE;
	else
		return STATE_FALSE;
}

bool CGMSKModemWinUSB::setPTT(bool on)
{
	wxASSERT(m_handle != INVALID_HANDLE_VALUE);

	int ret = io(SET_PTT, 0x40U, on ? PTT_ON : PTT_OFF, NULL, 0U);
	if (ret < 0) {
		wxLogError(wxT("SET_PTT returned %d"), -ret);
		return false;
	}

	return true;
}

TRISTATE CGMSKModemWinUSB::hasSpace()
{
	wxASSERT(m_handle != INVALID_HANDLE_VALUE);

	if (m_broken)
		return STATE_TRUE;

	unsigned char space;
	int ret = io(GET_REMAINSPACE, 0xC0U, 0U, &space, 1U);
	if (ret != 1) {
		wxLogError(wxT("GET_REMAINSPACE returned %d"), -ret);
		return STATE_UNKNOWN;
	}

	if (space >= DV_FRAME_LENGTH_BYTES)
		return STATE_TRUE;
	else
		return STATE_FALSE;
}

int CGMSKModemWinUSB::writeData(const unsigned char* data, unsigned int length)
{
	wxASSERT(m_handle != INVALID_HANDLE_VALUE);
	wxASSERT(data != NULL);
	wxASSERT(length > 0U && length <= DV_FRAME_LENGTH_BYTES);

	unsigned char* buffer = const_cast<unsigned char*>(data);

	if (length > GMSK_MODEM_DATA_LENGTH) {
		int ret = io(PUT_DATA, 0x40U, 0U, buffer, GMSK_MODEM_DATA_LENGTH);
		if (ret < 0) {
			if (ret == -22) {
				wxLogError(wxT("PUT_DATA 1, returned %d"), -ret);
				return ret;
			}

			return 0;
		}
		
		ret = io(PUT_DATA, 0x40U, 0U, buffer + GMSK_MODEM_DATA_LENGTH, length - GMSK_MODEM_DATA_LENGTH);
		if (ret < 0) {
			if (ret == -22) {
				wxLogError(wxT("PUT_DATA 2, returned %d"), -ret);
				return ret;
			}

			return int(GMSK_MODEM_DATA_LENGTH);
		}
	
		return length;
	} else {
		int ret = io(PUT_DATA, 0x40U, 0U, buffer, length);
		if (ret < 0) {
			if (ret == -22) {
				wxLogError(wxT("PUT_DATA returned %d"), -ret);
				return ret;
			}

			return 0;
		}

		return length;
	}
}

void CGMSKModemWinUSB::close()
{
	wxASSERT(m_file != INVALID_HANDLE_VALUE);
	wxASSERT(m_handle != INVALID_HANDLE_VALUE);

	::CloseHandle(m_file);
	::WinUsb_Free(m_handle);

	m_file = INVALID_HANDLE_VALUE;
	m_handle = INVALID_HANDLE_VALUE;
}

bool CGMSKModemWinUSB::openModem()
{
	WCHAR id1[15U], id2[15U];
	::swprintf(id1, L"pid_%04x", m_address);
	::swprintf(id2, L"pid_%04X", m_address);

	wxString wxId1(id1, wxConvLocal);
	wxString wxId2(id2, wxConvLocal);

	CLSID clsId;
	LPOLESTR str = OLESTR("{136C76EF-3F4E-4030-A7E3-E1003EF0A715}");
	HRESULT result = ::CLSIDFromString(str, &clsId);
	if (result != NOERROR) {
		wxLogError(wxT("Error from CLSIDFromString: err=%lu"), ::GetLastError());
		return false;
	}

	HDEVINFO devInfo = ::SetupDiGetClassDevs(&clsId, NULL, NULL, DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);
	if (devInfo == INVALID_HANDLE_VALUE) {
		wxLogError(wxT("Error from SetupDiGetClassDevs: err=%lu"), ::GetLastError());
		return false;
	}

	SP_DEVICE_INTERFACE_DATA devInfoData;
	devInfoData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);

	for (unsigned int index = 0U; ::SetupDiEnumDeviceInterfaces(devInfo, NULL, &clsId, index, &devInfoData); index++) {
		DWORD length;
		::SetupDiGetDeviceInterfaceDetail(devInfo, &devInfoData, NULL, 0U, &length, NULL);

		PSP_DEVICE_INTERFACE_DETAIL_DATA detailData = PSP_DEVICE_INTERFACE_DETAIL_DATA(::malloc(length));
		detailData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

		DWORD required;
		BOOL ret = ::SetupDiGetDeviceInterfaceDetail(devInfo, &devInfoData, detailData, length, &required, NULL);
		if (!ret) {
			wxLogError(wxT("Error from SetupDiGetDeviceInterfaceDetail: err=%lu"), ::GetLastError());
			::SetupDiDestroyDeviceInfoList(devInfo);
			::free(detailData);
			return false;
		}

		// Check the name to see if it's the correct vendor id and address
		if (::wcsstr(detailData->DevicePath, id1) == NULL && ::wcsstr(detailData->DevicePath, id2) == NULL) {
			::free(detailData);
			continue;
		}

		m_file = ::CreateFile(detailData->DevicePath, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
		if (m_file == INVALID_HANDLE_VALUE) {
			wxLogError(wxT("Error from CreateFile: err=%lu"), ::GetLastError());
			::SetupDiDestroyDeviceInfoList(devInfo);
			::free(detailData);
			return false;
		}

		ret = ::WinUsb_Initialize(m_file, &m_handle);
		if (!ret) {
			wxLogError(wxT("Error from WinUsb_Initialize: err=%lu"), ::GetLastError());
			::SetupDiDestroyDeviceInfoList(devInfo);
			::CloseHandle(m_file);
			::free(detailData);
			return false;
		}

		::SetupDiDestroyDeviceInfoList(devInfo);
		::free(detailData);

		return true;
	}

	::SetupDiDestroyDeviceInfoList(devInfo);

	return false;
}

int CGMSKModemWinUSB::io(unsigned char type, unsigned char n1, unsigned char n2, unsigned char* buffer, unsigned int length)
{
	wxASSERT(m_handle != INVALID_HANDLE_VALUE);

	WINUSB_SETUP_PACKET packet;
	packet.RequestType = n1;
	packet.Request     = type;
	packet.Value       = n2;
	packet.Index       = 0U;
	packet.Length      = length;

	ULONG transferred;
	BOOL ret = ::WinUsb_ControlTransfer(m_handle, packet, buffer, length, &transferred, 0);
	if (!ret) {
		long error = ::GetLastError();
		return -error;
	}

	return transferred;
}

