/*
 *	Copyright (C) 2009-2013 by Jonathan Naylor, G4KLX
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

#include "URIUSBController.h"

const unsigned int C108_VENDOR_ID     = 0x0D8CU;
const unsigned int C108_PRODUCT_ID    = 0x000CU;
const unsigned int C108AH_PRODUCT_ID  = 0x013CU;
const unsigned int C119_PRODUCT_ID    = 0x0008U;
const unsigned int C119A_PRODUCT_ID   = 0x013AU;

const char REPORT_ID     = 0x00;

const char HID_OR0       = 0x00;
const char HID_OR1       = 0x00;
const char HID_OR2       = 0x0F;	// Set GPIO1 to GPIO4 to output
const char HID_OR3       = 0x00;
const char HID_OR1_GPIO4 = 0x08;	// GPIO4, Switched
const char HID_OR1_GPIO3 = 0x04;	// GPIO3, PTT
const char HID_OR1_GPIO2 = 0x02;	// GPIO2, Switched
const char HID_OR1_GPIO1 = 0x01;	// GPIO1, Heartbeat
const char HID_IR0_RM    = 0x08;	// Record-Mute, Disable
const char HID_IR0_PM    = 0x04;	// Playback-Mute, Battery
const char HID_IR0_VD    = 0x02;	// Volume-Down, COR_DET
const char HID_IR0_VU    = 0x01;	// Volume-Up, CTCSS_DET

#if defined(__WINDOWS__)

#include <Setupapi.h>
#include "HID.h"

const ULONG USB_BUFSIZE = 5UL;

CURIUSBController::CURIUSBController(unsigned int address, bool checkInput) :
m_address(address),
m_checkInput(checkInput),
m_outp1(false),
m_outp3(false),
m_outp5(false),
m_outp6(false),
m_handle(INVALID_HANDLE_VALUE)
{
}

CURIUSBController::~CURIUSBController()
{
}

bool CURIUSBController::open()
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

	unsigned int count = 0U;
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

		// Is this a CM108 or equivalent?
		if (attributes.VendorID  == C108_VENDOR_ID    &&
		   (attributes.ProductID == C108_PRODUCT_ID   ||
			attributes.ProductID == C108AH_PRODUCT_ID ||
			attributes.ProductID == C119_PRODUCT_ID   ||
			attributes.ProductID == C119A_PRODUCT_ID)) {
				count++;

			// Is this the right device?
			if (count == m_address) {
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
		}

		::free(detailData);
	}

	::SetupDiDestroyDeviceInfoList(devInfo);

	return false;
}

void CURIUSBController::getDigitalInputs(bool& inp1, bool& inp2, bool& inp3, bool& inp4, bool& inp5)
{
	wxASSERT(m_handle != INVALID_HANDLE_VALUE);

	if (!m_checkInput) {
		inp1 = inp2 = inp3 = inp4 = inp5 = false;
		return;
	}

	char buffer[USB_BUFSIZE];
	buffer[0] = REPORT_ID;
	buffer[1] = 0x00;
	buffer[2] = 0x00;
	buffer[3] = 0x00;
	buffer[4] = 0x00;
	BOOL res = ::HidD_GetInputReport(m_handle, buffer, USB_BUFSIZE);
	if (!res) {
		wxLogError(wxT("Error from HidD_GetInputReport: err=%u"), ::GetLastError());
		return;
	}

	inp3 = false;

	inp5 = (buffer[1] & HID_IR0_RM) == HID_IR0_RM;	// Disable
	inp4 = (buffer[1] & HID_IR0_PM) == HID_IR0_PM;	// Battery
	inp2 = (buffer[1] & HID_IR0_VU) == HID_IR0_VU;	// Squelch 2
	inp1 = (buffer[1] & HID_IR0_VD) == HID_IR0_VD;	// Squelch 1
}

void CURIUSBController::setDigitalOutputs(bool outp1, bool, bool outp3, bool outp4, bool outp5, bool outp6, bool, bool)
{
	wxASSERT(m_handle != INVALID_HANDLE_VALUE);

	if (outp1 == m_outp1 && outp3 == m_outp3 && outp5 == m_outp5 && outp6 == m_outp6)
		return;

	char buffer[USB_BUFSIZE];
	buffer[0] = REPORT_ID;
	buffer[1] = HID_OR0;
	buffer[2] = HID_OR1;
	buffer[3] = HID_OR2;
	buffer[4] = HID_OR3;

	if (outp1)			// Transmit
		buffer[2] |= HID_OR1_GPIO3;
	if (outp3)			// Heartbeat
		buffer[2] |= HID_OR1_GPIO1;
	if (outp4)			// Active
		buffer[2] |= HID_OR1_GPIO2;
	if (outp5)			// Output 1
		buffer[2] |= HID_OR1_GPIO4;

	BOOL res = ::HidD_SetOutputReport(m_handle, buffer, USB_BUFSIZE);
	if (!res)
		wxLogError(wxT("Error from HidD_SetOutputReport: err=%u"), ::GetLastError());

	m_outp1 = outp1;
	m_outp3 = outp3;
	m_outp5 = outp5;
	m_outp6 = outp6;
}

void CURIUSBController::close()
{
	wxASSERT(m_handle != INVALID_HANDLE_VALUE);

	setDigitalOutputs(false, false, false, false, false, false, false, false);

	::CloseHandle(m_handle);
	m_handle = INVALID_HANDLE_VALUE;
}

#else

const unsigned int C108_HID_INTERFACE = 3U;

const unsigned int HID_REPORT_GET = 0x01U;
const unsigned int HID_REPORT_SET = 0x09U;

const unsigned int HID_RT_INPUT   = 0x01U;
const unsigned int HID_RT_OUTPUT  = 0x02U;

const int USB_BUFSIZE = 4;

const int USB_TIMEOUT = 5000;

CURIUSBController::CURIUSBController(unsigned int address, bool checkInput) :
m_address(address),
m_checkInput(checkInput),
m_outp1(false),
m_outp3(false),
m_outp5(false),
m_outp6(false),
m_context(NULL),
m_handle(NULL)
{
	::libusb_init(&m_context);
}

CURIUSBController::~CURIUSBController()
{
	wxASSERT(m_context != NULL);

	::libusb_exit(m_context);
}

bool CURIUSBController::open()
{
	wxASSERT(m_context != NULL);
	wxASSERT(m_handle == NULL);

	libusb_device** list;
	ssize_t cnt = ::libusb_get_device_list(NULL, &list);
	if (cnt <= 0) {
		wxLogError(wxT("Cannot find any USB devices!"));
		return false;
	}

	int err = 0;
	unsigned int count = 0U;
	for (ssize_t i = 0; i < cnt; i++) {
		libusb_device_descriptor desc;
		err = ::libusb_get_device_descriptor(list[i], &desc);
		if (err < 0)
			continue;

		if (desc.idVendor == C108_VENDOR_ID     &&
		   (desc.idProduct == C108_PRODUCT_ID   ||
			desc.idProduct == C108AH_PRODUCT_ID ||
			desc.idProduct == C119_PRODUCT_ID   ||
			desc.idProduct == C119A_PRODUCT_ID)) {
			count++;

			// Is this the correct device?
			if (count == m_address) {
				err = ::libusb_open(list[i], &m_handle);
		        break;
		    }
		}
	}

	if (err != 0) {
		wxLogError(wxT("libusb_open failed, err=%d"), err);
		::libusb_free_device_list(list, 1);
		return false;
	}

	if (m_handle == NULL) {
		wxLogError(wxT("Could not find a suitable CM108 based device"));
		::libusb_free_device_list(list, 1);
		return false;
	}

	::libusb_free_device_list(list, 1);

	int res = ::libusb_claim_interface(m_handle, C108_HID_INTERFACE);
	if (res != 0) {
		res = ::libusb_detach_kernel_driver(m_handle, C108_HID_INTERFACE);
		if (res != 0) {
			wxLogError(wxT("libusb_detach_kernel_driver failed, err=%d"), res);
			::libusb_close(m_handle);
			m_handle = NULL;
			return false;
		}

		res = ::libusb_claim_interface(m_handle, C108_HID_INTERFACE);
		if (res != 0) {
			wxLogError(wxT("libusb_claim_interface failed, err=%d"), res);
			::libusb_close(m_handle);
			m_handle = NULL;
			return false;
		}
	}

	setDigitalOutputs(false, false, false, false, false, false, false, false);

	return true;
}

void CURIUSBController::getDigitalInputs(bool& inp1, bool& inp2, bool& inp3, bool& inp4, bool& inp5)
{
	wxASSERT(m_handle != NULL);

	if (!m_checkInput) {
		inp1 = inp2 = inp3 = inp4 = inp5 = false;
		return;
	}

	unsigned char buffer[USB_BUFSIZE];
	buffer[0] = 0x00;
	buffer[1] = 0x00;
	buffer[2] = 0x00;
	buffer[3] = 0x00;
	int res = ::libusb_control_transfer(m_handle, LIBUSB_ENDPOINT_IN + LIBUSB_REQUEST_TYPE_CLASS + LIBUSB_RECIPIENT_INTERFACE, HID_REPORT_GET, REPORT_ID + (HID_RT_INPUT << 8), C108_HID_INTERFACE, buffer, USB_BUFSIZE, USB_TIMEOUT);
	if (res < 0) {
		wxLogError(wxT("Error from libusb_control_transfer: err=%d"), res);
		return;
	}

	if (res < USB_BUFSIZE)
		return;

	inp3 = false;

	inp5 = (buffer[0] & HID_IR0_RM) == HID_IR0_RM;	// Disable
	inp4 = (buffer[0] & HID_IR0_PM) == HID_IR0_PM;	// Battery
	inp2 = (buffer[0] & HID_IR0_VU) == HID_IR0_VU;	// Squelch 2
	inp1 = (buffer[0] & HID_IR0_VD) == HID_IR0_VD;	// Squelch 1
}

void CURIUSBController::setDigitalOutputs(bool outp1, bool, bool outp3, bool outp4, bool outp5, bool outp6, bool, bool)
{
	wxASSERT(m_handle != NULL);

	if (outp1 == m_outp1 && outp3 == m_outp3 && outp5 == m_outp5 && outp6 == m_outp6)
		return;

	unsigned char buffer[USB_BUFSIZE];
	buffer[0] = HID_OR0;
	buffer[1] = HID_OR1;
	buffer[2] = HID_OR2;
	buffer[3] = HID_OR3;

	if (outp1)			// Transmit
		buffer[1] |= HID_OR1_GPIO3;
	if (outp3)			// Heartbeat
		buffer[1] |= HID_OR1_GPIO1;
	if (outp4)			// Active
		buffer[1] |= HID_OR1_GPIO2;
	if (outp5)			// Output 1
		buffer[1] |= HID_OR1_GPIO4;

	int res = ::libusb_control_transfer(m_handle, LIBUSB_ENDPOINT_OUT + LIBUSB_REQUEST_TYPE_CLASS + LIBUSB_RECIPIENT_INTERFACE, HID_REPORT_SET, REPORT_ID + (HID_RT_OUTPUT << 8), C108_HID_INTERFACE, buffer, USB_BUFSIZE, USB_TIMEOUT);
	if (res < 0) {
		wxLogError(wxT("Error from libusb_control_transfer: err=%d"), res);
		return;
	}

	if (res < USB_BUFSIZE)
		return;

	m_outp1 = outp1;
	m_outp3 = outp3;
	m_outp5 = outp5;
	m_outp6 = outp6;
}

void CURIUSBController::close()
{
	wxASSERT(m_handle != NULL);

	setDigitalOutputs(false, false, false, false, false, false, false, false);

	::libusb_release_interface(m_handle, C108_HID_INTERFACE);

	::libusb_close(m_handle);
	m_handle = NULL;
}

#endif

