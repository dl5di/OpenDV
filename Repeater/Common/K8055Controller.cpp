/*
 *	Copyright (C) 2009,2010 by Jonathan Naylor, G4KLX
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

#include "K8055Controller.h"

const unsigned int VELLEMAN_VENDOR_ID  = 0x10CFU;
const unsigned int VELLEMAN_PRODUCT_ID = 0x5500U;

const char REPORT_ID              = 0x00U;

const char CMD_RESET              = 0x00U;
const char CMD_SET_ANALOG_DIGITAL = 0x05U;

const char IN_PORT1 = 0x10U;
const char IN_PORT2 = 0x20U;
const char IN_PORT3 = 0x01U;
const char IN_PORT4 = 0x40U;
const char IN_PORT5 = 0x80U;

const char OUT_PORT1 = 0x01U;
const char OUT_PORT2 = 0x02U;
const char OUT_PORT3 = 0x04U;
const char OUT_PORT4 = 0x08U;
const char OUT_PORT5 = 0x10U;
const char OUT_PORT6 = 0x20U;
const char OUT_PORT7 = 0x40U;
const char OUT_PORT8 = 0x80U;

#if defined(__WINDOWS__)

#include <Setupapi.h>
#include "HID.h"

const DWORD USB_BUFSIZE = 9UL;

CK8055Controller::CK8055Controller(unsigned int address) :
m_address(address),
m_outp1(false),
m_outp2(false),
m_outp3(false),
m_outp4(false),
m_outp5(false),
m_outp6(false),
m_outp7(false),
m_outp8(false),
m_handle(INVALID_HANDLE_VALUE)
{
}

CK8055Controller::~CK8055Controller()
{
}

bool CK8055Controller::open()
{
	wxASSERT(m_handle == INVALID_HANDLE_VALUE);

	GUID guid;
	::HidD_GetHidGuid(&guid);

	HDEVINFO devInfo = ::SetupDiGetClassDevs(&guid, NULL, NULL, DIGCF_DEVICEINTERFACE | DIGCF_PRESENT);
	if (devInfo == INVALID_HANDLE_VALUE) {
		wxLogError(wxT("Error from SetupDiGetClassDevs: err=%u"), ::GetLastError());
		return false;
	}

	SP_DEVICE_INTERFACE_DATA devInfoData;
	devInfoData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);

	for (DWORD index = 0U; ::SetupDiEnumDeviceInterfaces(devInfo, NULL, &guid, index, &devInfoData); index++) {
		// Find the required length of the device structure
		DWORD length;
		::SetupDiGetDeviceInterfaceDetail(devInfo, &devInfoData, NULL, 0U, &length, NULL);

		PSP_DEVICE_INTERFACE_DETAIL_DATA detailData = PSP_DEVICE_INTERFACE_DETAIL_DATA(::malloc(length));
		detailData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

		// Get the detailed data into the newly allocated device structure
		DWORD required;
		BOOL res = ::SetupDiGetDeviceInterfaceDetail(devInfo, &devInfoData, detailData, length, &required, NULL);
		if (!res) {
			wxLogError(wxT("Error from SetupDiGetDeviceInterfaceDetail: err=%u"), ::GetLastError());
			::SetupDiDestroyDeviceInfoList(devInfo);
			::free(detailData);
			return false;
		}

		// Get the handle for getting the attributes
		HANDLE handle = ::CreateFile(detailData->DevicePath, 0, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
		if (handle == INVALID_HANDLE_VALUE) {
			wxLogError(wxT("Error from CreateFile: err=%u"), ::GetLastError());
			::SetupDiDestroyDeviceInfoList(devInfo);
			::free(detailData);
			return false;
		}

		HIDD_ATTRIBUTES attributes;
		attributes.Size = sizeof(HIDD_ATTRIBUTES);
		res = ::HidD_GetAttributes(handle, &attributes);
		if (!res) {
			wxLogError(wxT("Error from HidD_GetAttributes: err=%u"), ::GetLastError());
			::CloseHandle(handle);
			::SetupDiDestroyDeviceInfoList(devInfo);
			::free(detailData);
			return false;
		}

		::CloseHandle(handle);

		// Is this a Velleman K8055 and the right one?
		if (attributes.VendorID  == VELLEMAN_VENDOR_ID && attributes.ProductID == (VELLEMAN_PRODUCT_ID + m_address)) {
			m_handle = ::CreateFile(detailData->DevicePath, GENERIC_WRITE | GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
			if (m_handle == INVALID_HANDLE_VALUE) {
				wxLogError(wxT("Error from CreateFile: err=%u"), ::GetLastError());
				::SetupDiDestroyDeviceInfoList(devInfo);
				::free(detailData);
				return false;
			}

			::SetupDiDestroyDeviceInfoList(devInfo);
			::free(detailData);

			setDigitalOutputs(false, false, false, false, false, false, false, false);

			return true;
		}

		::free(detailData);
	}

	::SetupDiDestroyDeviceInfoList(devInfo);

	return false;
}

void CK8055Controller::getDigitalInputs(bool& inp1, bool& inp2, bool& inp3, bool& inp4, bool& inp5)
{
	wxASSERT(m_handle != INVALID_HANDLE_VALUE);

	char buffer[USB_BUFSIZE];
	buffer[0] = REPORT_ID;
	buffer[1] = 0x00U;
	buffer[2] = 0x00U;
	buffer[3] = 0x00U;
	buffer[4] = 0x00U;
	buffer[5] = 0x00U;
	buffer[6] = 0x00U;
	buffer[7] = 0x00U;
	buffer[8] = 0x00U;

	DWORD read;
	BOOL res = ::ReadFile(m_handle, buffer, USB_BUFSIZE, &read, NULL);
	if (!res) {
		wxLogError(wxT("Error from ReadFile: err=%u, read=%u"), ::GetLastError(), read);
		return;
	}

	// Do we have data?
	if (buffer[2] != 0x00) {
		inp1 = (buffer[1] & IN_PORT1) == IN_PORT1;
		inp2 = (buffer[1] & IN_PORT2) == IN_PORT2;
		inp3 = (buffer[1] & IN_PORT3) == IN_PORT3;
		inp4 = (buffer[1] & IN_PORT4) == IN_PORT4;
		inp5 = (buffer[1] & IN_PORT5) == IN_PORT5;
	}
}

void CK8055Controller::setDigitalOutputs(bool outp1, bool outp2, bool outp3, bool outp4, bool outp5, bool outp6, bool outp7, bool outp8)
{
	wxASSERT(m_handle != INVALID_HANDLE_VALUE);

	if (outp1 == m_outp1 && outp2 == m_outp2 && outp3 == m_outp3 && outp4 == m_outp4 &&
		outp5 == m_outp5 && outp6 == m_outp6 && outp7 == m_outp7 && outp8 == m_outp8)
		return;

	char buffer[USB_BUFSIZE];
	buffer[0] = REPORT_ID;
	buffer[1] = CMD_SET_ANALOG_DIGITAL;
	buffer[2] = 0x00U;
	buffer[3] = 0x00U;
	buffer[4] = 0x00U;
	buffer[5] = 0x00U;
	buffer[6] = 0x00U;
	buffer[7] = 0x00U;
	buffer[8] = 0x00U;

	if (outp1)
		buffer[2] |= OUT_PORT1;
	if (outp2)
		buffer[2] |= OUT_PORT2;
	if (outp3)
		buffer[2] |= OUT_PORT3;
	if (outp4)
		buffer[2] |= OUT_PORT4;
	if (outp5)
		buffer[2] |= OUT_PORT5;
	if (outp6)
		buffer[2] |= OUT_PORT6;
	if (outp7)
		buffer[2] |= OUT_PORT7;
	if (outp8)
		buffer[2] |= OUT_PORT8;

	DWORD written;
	BOOL res = ::WriteFile(m_handle, buffer, USB_BUFSIZE, &written, NULL);
	if (!res || written != USB_BUFSIZE)
		wxLogError(wxT("Error from WriteFile: err=%u, written=%u"), ::GetLastError(), written);

	m_outp1 = outp1;
	m_outp2 = outp2;
	m_outp3 = outp3;
	m_outp4 = outp4;
	m_outp5 = outp5;
	m_outp6 = outp6;
	m_outp7 = outp7;
	m_outp8 = outp8;
}

void CK8055Controller::close()
{
	wxASSERT(m_handle != INVALID_HANDLE_VALUE);

	setDigitalOutputs(false, false, false, false, false, false, false, false);

	::CloseHandle(m_handle);
	m_handle = INVALID_HANDLE_VALUE;
}

#else

const unsigned int VELLEMAN_HID_INTERFACE = 0U;

const unsigned int USB_OUTPUT_ENDPOINT = 0x01U;
const unsigned int USB_INPUT_ENDPOINT  = 0x81U;

const int USB_BUFSIZE = 8;

const int USB_TIMEOUT = 5000;

CK8055Controller::CK8055Controller(unsigned int address) :
m_address(address),
m_outp1(false),
m_outp2(false),
m_outp3(false),
m_outp4(false),
m_outp5(false),
m_outp6(false),
m_outp7(false),
m_outp8(false),
m_context(NULL),
m_handle(NULL)
{
	::libusb_init(&m_context);
}

CK8055Controller::~CK8055Controller()
{
	wxASSERT(m_context != NULL);

	::libusb_exit(m_context);
}

bool CK8055Controller::open()
{
	wxASSERT(m_context != NULL);
	wxASSERT(m_handle == NULL);

	m_handle = ::libusb_open_device_with_vid_pid(m_context, VELLEMAN_VENDOR_ID, VELLEMAN_PRODUCT_ID + m_address);
	if (m_handle == NULL) {
		wxLogError(wxT("Could not open the Velleman K8055"));
		return false;
	}

	int res = ::libusb_claim_interface(m_handle, VELLEMAN_HID_INTERFACE);
	if (res != 0) {
		res = ::libusb_detach_kernel_driver(m_handle, VELLEMAN_HID_INTERFACE);
		if (res != 0) {
			wxLogError(wxT("Error from libusb_detach_kernel_driver: err=%d"), res);
			::libusb_close(m_handle);
			m_handle = NULL;
			return false;
		}

		res = ::libusb_claim_interface(m_handle, VELLEMAN_HID_INTERFACE);
		if (res != 0) {
			wxLogError(wxT("Error from libusb_claim_interface: err=%d"), res);
			::libusb_close(m_handle);
			m_handle = NULL;
			return false;
		}
	}

	::libusb_set_configuration(m_handle, 1);

	setDigitalOutputs(false, false, false, false, false, false, false, false);

	return true;
}

void CK8055Controller::getDigitalInputs(bool& inp1, bool& inp2, bool& inp3, bool& inp4, bool& inp5)
{
	wxASSERT(m_handle != NULL);

	unsigned char buffer[USB_BUFSIZE];
	buffer[0] = 0x00;
	buffer[1] = 0x00;
	buffer[2] = 0x00;
	buffer[3] = 0x00;
	buffer[4] = 0x00;
	buffer[5] = 0x00;
	buffer[6] = 0x00;
	buffer[7] = 0x00;

	int written;
	int res = ::libusb_interrupt_transfer(m_handle, USB_INPUT_ENDPOINT, buffer, USB_BUFSIZE, &written, USB_TIMEOUT);
	if (res != 0) {
		wxLogError(wxT("Error from libusb_interrupt_transfer: err=%d"), res);
		return;
	}

	if (written != USB_BUFSIZE)
		return;

	// Do we have data?
	if (buffer[1] != 0x00) {
		inp1 = (buffer[0] & IN_PORT1) == IN_PORT1;
		inp2 = (buffer[0] & IN_PORT2) == IN_PORT2;
		inp3 = (buffer[0] & IN_PORT3) == IN_PORT3;
		inp4 = (buffer[0] & IN_PORT4) == IN_PORT4;
		inp5 = (buffer[0] & IN_PORT5) == IN_PORT5;
	}
}

void CK8055Controller::setDigitalOutputs(bool outp1, bool outp2, bool outp3, bool outp4, bool outp5, bool outp6, bool outp7, bool outp8)
{
	wxASSERT(m_handle != NULL);

	if (outp1 == m_outp1 && outp2 == m_outp2 && outp3 == m_outp3 && outp4 == m_outp4 &&
		outp5 == m_outp5 && outp6 == m_outp6 && outp7 == m_outp7 && outp8 == m_outp8)
		return;

	unsigned char buffer[USB_BUFSIZE];
	buffer[0] = CMD_SET_ANALOG_DIGITAL;
	buffer[1] = 0x00;
	buffer[2] = 0x00;
	buffer[3] = 0x00;
	buffer[4] = 0x00;
	buffer[5] = 0x00;
	buffer[6] = 0x00;
	buffer[7] = 0x00;

	if (outp1)
		buffer[1] |= OUT_PORT1;
	if (outp2)
		buffer[1] |= OUT_PORT2;
	if (outp3)
		buffer[1] |= OUT_PORT3;
	if (outp4)
		buffer[1] |= OUT_PORT4;
	if (outp5)
		buffer[1] |= OUT_PORT5;
	if (outp6)
		buffer[1] |= OUT_PORT6;
	if (outp7)
		buffer[1] |= OUT_PORT7;
	if (outp8)
		buffer[1] |= OUT_PORT8;

	int written;
	int res = ::libusb_interrupt_transfer(m_handle, USB_OUTPUT_ENDPOINT, buffer, USB_BUFSIZE, &written, USB_TIMEOUT);
	if (res != 0) {
		wxLogError(wxT("Error from libusb_interrupt_transfer: err=%d"), res);
		return;
	}

	if (written != USB_BUFSIZE)
		return;

	m_outp1 = outp1;
	m_outp2 = outp2;
	m_outp3 = outp3;
	m_outp4 = outp4;
	m_outp5 = outp5;
	m_outp6 = outp6;
	m_outp7 = outp7;
	m_outp8 = outp8;
}

void CK8055Controller::close()
{
	wxASSERT(m_handle != NULL);

	setDigitalOutputs(false, false, false, false, false, false, false, false);

	::libusb_release_interface(m_handle, VELLEMAN_HID_INTERFACE);

	::libusb_close(m_handle);
	m_handle = NULL;
}

#endif

