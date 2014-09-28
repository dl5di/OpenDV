/*
 *   Copyright (C) 2010-2014 by Jonathan Naylor G4KLX
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
m_brokenSpace(false)
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

	wxString version;

	int ret;
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
		wxLogWarning(wxT("This modem firmware is not supported by the repeater"));
		wxLogWarning(wxT("Please upgrade to a newer version"));
		close();
		return false;
	}

	// DUTCH*Star firmware has a broken concept of free space
	if (version.Find(wxT("DUTCH*Star")) != wxNOT_FOUND)
		m_brokenSpace = true;

	return true;
}

bool CGMSKModemWinUSB::readHeader(unsigned char* header, unsigned int length)
{
	wxASSERT(header != NULL);
	wxASSERT(length > (RADIO_HEADER_LENGTH_BYTES * 2U));

	unsigned int offset = 0U;

	while (offset < RADIO_HEADER_LENGTH_BYTES) {
		int ret = io(GET_HEADER, 0xC0U, 0U, header + offset, GMSK_MODEM_DATA_LENGTH);
		if (ret < 0) {
			wxLogError(wxT("GET_HEADER returned %d"), -ret);
			return false;
		} else if (ret == 0) {
			if (offset == 0U)
				return false;

			::wxMilliSleep(10UL);

			unsigned char status;
			ret = io(GET_AD_STATUS, 0xC0U, 0U, &status, 1U);
			if (ret < 0) {
				wxLogError(wxT("GET_COS returned %d"), -ret);
				return false;
			} else if (ret > 0) {
				if ((status & COS_OnOff) == COS_OnOff)
					offset = 0U;
			}
		} else {
			offset += ret;
		}
	}

	unsigned char status;
	int ret = io(GET_AD_STATUS, 0xC0U, 0U, &status, 1U);
	if (ret < 0) {
		wxLogError(wxT("GET_CRC returned %d"), -ret);
		return false;
	}

	if ((status & CRC_ERROR) == CRC_ERROR) {
		wxLogMessage(wxT("Invalid CRC on header"));
		return false;
	}

	return true;
}

int CGMSKModemWinUSB::readData(unsigned char* data, unsigned int length, bool& end)
{
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

void CGMSKModemWinUSB::writeHeader(unsigned char* header, unsigned int length)
{
	wxASSERT(header != NULL);
	wxASSERT(length >= (RADIO_HEADER_LENGTH_BYTES - 2U));

	io(SET_MyCALL2,  0x40U, 0U, header + 35U, SHORT_CALLSIGN_LENGTH);
	io(SET_MyCALL,   0x40U, 0U, header + 27U, LONG_CALLSIGN_LENGTH);
	io(SET_YourCALL, 0x40U, 0U, header + 19U, LONG_CALLSIGN_LENGTH);
	io(SET_RPT1CALL, 0x40U, 0U, header + 11U, LONG_CALLSIGN_LENGTH);
	io(SET_RPT2CALL, 0x40U, 0U, header + 3U,  LONG_CALLSIGN_LENGTH);
	io(SET_FLAGS,    0x40U, 0U, header + 0U,  3U);
}

TRISTATE CGMSKModemWinUSB::getPTT()
{
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

void CGMSKModemWinUSB::setPTT(bool on)
{
	unsigned char c;
	io(SET_PTT, 0x40U, on ? PTT_ON : PTT_OFF, &c, 0U);
}

TRISTATE CGMSKModemWinUSB::hasSpace()
{
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

int CGMSKModemWinUSB::writeData(unsigned char* data, unsigned int length)
{
	wxASSERT(data != NULL);
	wxASSERT(length > 0U && length <= DV_FRAME_LENGTH_BYTES);

	if (length > GMSK_MODEM_DATA_LENGTH) {
		int ret = io(PUT_DATA, 0x40U, 0U, data, GMSK_MODEM_DATA_LENGTH);
		if (ret < 0) {
			if (ret == -22) {
				wxLogError(wxT("PUT_DATA 1, returned %d"), -ret);
				return ret;
			}

			return 0;
		}
		
		ret = io(PUT_DATA, 0x40U, 0U, data + GMSK_MODEM_DATA_LENGTH, length - GMSK_MODEM_DATA_LENGTH);
		if (ret < 0) {
			if (ret == -22) {
				wxLogError(wxT("PUT_DATA 2, returned %d"), -ret);
				return ret;
			}

			return int(GMSK_MODEM_DATA_LENGTH);
		}
	
		return length;
	} else {
		int ret = io(PUT_DATA, 0x40U, 0U, data, length);
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
		BOOL ret1 = ::SetupDiGetDeviceInterfaceDetail(devInfo, &devInfoData, detailData, length, &required, NULL);
		if (!ret1) {
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

		ret1 = ::WinUsb_Initialize(m_file, &m_handle);
		if (!ret1) {
			wxLogError(wxT("Error from WinUsb_Initialize: err=%lu"), ::GetLastError());
			::SetupDiDestroyDeviceInfoList(devInfo);
			::CloseHandle(m_file);
			::free(detailData);
			return false;
		}

		::SetupDiDestroyDeviceInfoList(devInfo);
		::free(detailData);

		unsigned char c;
		io(SET_AD_INIT, 0x40U, 0U, &c, 0U);

		setPTT(false);

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

