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

#include "GMSKModemLibUsb.h"
#include "DStarDefines.h"

const unsigned int VENDOR_ID = 0x04D8U;

const int USB_TIMEOUT     = 1000;

const int SET_AD_INIT     = 0x00;
const int SET_PTT         = 0x05;
const int PUT_DATA        = 0x10;
const int GET_DATA        = 0x11;
const int GET_HEADER      = 0x21;
const int GET_AD_STATUS   = 0x30;
const int SET_MyCALL      = 0x40;
const int SET_MyCALL2     = 0x41;
const int SET_YourCALL    = 0x42;
const int SET_RPT1CALL    = 0x43;
const int SET_RPT2CALL    = 0x44;
const int SET_FLAGS       = 0x45;
const int GET_REMAINSPACE = 0x50;
const int GET_VERSION     = 0xFF;

const char COS_OnOff  = 0x02;
const char CRC_ERROR  = 0x04;
const char LAST_FRAME = 0x08;
const char PTT_OnOff  = 0x20;

const int PTT_ON  = 1;
const int PTT_OFF = 0;


#if defined(WIN32)
const wxString LIBNAME = wxT("libusb0");

wxDynamicLibrary* CGMSKModemLibUsb::m_library = NULL;
bool              CGMSKModemLibUsb::m_loaded  = false;

void            (*CGMSKModemLibUsb::m_usbInit)() = NULL;
int             (*CGMSKModemLibUsb::m_usbFindBusses)() = NULL;
int             (*CGMSKModemLibUsb::m_usbFindDevices)() = NULL;
usb_bus*        (*CGMSKModemLibUsb::m_usbGetBusses)() = NULL;
usb_dev_handle* (*CGMSKModemLibUsb::m_usbOpen)(struct usb_device*) = NULL;
int             (*CGMSKModemLibUsb::m_usbSetConfiguration)(usb_dev_handle*, int) = NULL;
int             (*CGMSKModemLibUsb::m_usbControlMsg)(usb_dev_handle*, int, int, int, int, char*, int, int) = NULL;
char*           (*CGMSKModemLibUsb::m_usbStrerror)() = NULL;
int             (*CGMSKModemLibUsb::m_usbClose)(usb_dev_handle*) = NULL;

CGMSKModemLibUsb::CGMSKModemLibUsb(unsigned int address) :
m_address(address),
m_dev(NULL),
m_brokenSpace(false)
{
	if (m_library == NULL)
		m_library = new wxDynamicLibrary(LIBNAME);

	if (!m_library->IsLoaded()) {
		wxLogError(wxT("Unable to load shared library %s"), LIBNAME.c_str());
		return;
	}

	void* ptr1 = m_library->GetSymbol(wxT("usb_init"));
	void* ptr2 = m_library->GetSymbol(wxT("usb_find_busses"));
	void* ptr3 = m_library->GetSymbol(wxT("usb_find_devices"));
	void* ptr4 = m_library->GetSymbol(wxT("usb_get_busses"));
	void* ptr5 = m_library->GetSymbol(wxT("usb_open"));
	void* ptr6 = m_library->GetSymbol(wxT("usb_set_configuration"));
	void* ptr7 = m_library->GetSymbol(wxT("usb_control_msg"));
	void* ptr8 = m_library->GetSymbol(wxT("usb_strerror"));
	void* ptr9 = m_library->GetSymbol(wxT("usb_close"));

	if (ptr1 == NULL || ptr2 == NULL || ptr3 == NULL ||
		ptr4 == NULL || ptr5 == NULL || ptr6 == NULL ||
		ptr7 == NULL || ptr8 == NULL || ptr9 == NULL) {
		wxLogError(wxT("Unable to get symbols from %s"), LIBNAME.c_str());
		return;
	}

	m_usbInit             = (void     (*)())ptr1;
	m_usbFindBusses       = (int      (*)())ptr2;
	m_usbFindDevices      = (int      (*)())ptr3;
	m_usbGetBusses        = (usb_bus* (*)())ptr4;
	m_usbOpen             = (usb_dev_handle* (*)(struct usb_device*))ptr5;
	m_usbSetConfiguration = (int      (*)(usb_dev_handle*, int))ptr6;
	m_usbControlMsg       = (int      (*)(usb_dev_handle*, int, int, int, int, char*, int, int))ptr7;
	m_usbStrerror         = (char*    (*)())ptr8;
	m_usbClose            = (int      (*)(usb_dev_handle*))ptr9;

	wxLogMessage(wxT("Successfully loaded library %s"), LIBNAME.c_str());

	m_loaded = true;
}


CGMSKModemLibUsb::~CGMSKModemLibUsb()
{
}

bool CGMSKModemLibUsb::open()
{
	wxASSERT(m_dev == NULL);

	bool ret1 = openModem();
	if (!ret1) {
		wxLogError(wxT("Cannot find the GMSK Modem with address: 0x%04X"), m_address);
		return false;
	}

	wxLogInfo(wxT("Found the GMSK Modem with address: 0x%04X"), m_address);

	wxString version;

	int ret2;
	do {
		char buffer[GMSK_MODEM_DATA_LENGTH];
		ret2 = io(0xC0, GET_VERSION, 0, 0, buffer, GMSK_MODEM_DATA_LENGTH, USB_TIMEOUT);
		if (ret2 > 0) {
			wxString text(buffer, wxConvLocal, ret2);
			version.Append(text);
		} else if (ret2 < 0) {
			wxString error(m_usbStrerror(), wxConvLocal);
			wxLogError(wxT("GET_VERSION, ret: %d, err=%s"), ret2, error.c_str());
			close();
			return false;
		}
	} while (ret2 == int(GMSK_MODEM_DATA_LENGTH));

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

bool CGMSKModemLibUsb::readHeader(unsigned char* header, unsigned int length)
{
	wxASSERT(header != NULL);
	wxASSERT(length > (RADIO_HEADER_LENGTH_BYTES * 2U));

	unsigned int offset = 0U;

	while (offset < RADIO_HEADER_LENGTH_BYTES) {
		int ret = io(0xC0, GET_HEADER, 0, 0, (char*)(header + offset), 8, USB_TIMEOUT);
		if (ret < 0) {
			wxString errorText(m_usbStrerror(), wxConvLocal);
			wxLogMessage(wxT("GET_HEADER, ret: %d, err=%s"), ret, errorText.c_str());

			if (ret == -19)				// -ENODEV
				return false;

			::wxMilliSleep(10UL);
		} else if (ret == 0) {
			if (offset == 0U)
				return false;

			::wxMilliSleep(10UL);

			char status;
			int ret = io(0xC0, GET_AD_STATUS, 0, 0, &status, 1, USB_TIMEOUT);
			if (ret < 0) {
				wxString errorText(m_usbStrerror(), wxConvLocal);
				wxLogMessage(wxT("GET_COS, ret: %d, err=%s"), ret, errorText.c_str());

				if (ret == -19)			// -ENODEV
					return false;

				::wxMilliSleep(10UL);
			} else if (ret > 0) {
				if ((status & COS_OnOff) == COS_OnOff)
					offset = 0U;
			}
		} else {
			offset += ret;
		}
	}

	char status;
	int ret = io(0xC0, GET_AD_STATUS, 0, 0, &status, 1, USB_TIMEOUT);
	if (ret < 0) {
		wxString errorText(m_usbStrerror(), wxConvLocal);
		wxLogMessage(wxT("GET_CRC, ret: %d, err=%s"), ret, errorText.c_str());
		return false;
	}

	if ((status & CRC_ERROR) == CRC_ERROR) {
		wxLogMessage(wxT("Header - CRC Error"));
		return false;
	}

	return true;
}

int CGMSKModemLibUsb::readData(unsigned char* data, unsigned int length, bool& end)
{
	wxASSERT(data != NULL);
	wxASSERT(length > 0U);

	end = false;

	int ret = io(0xC0, GET_DATA, 0, 0, (char*)data, GMSK_MODEM_DATA_LENGTH, USB_TIMEOUT);
	if (ret < 0) {
		if (ret == -19) {		// -ENODEV
			wxString errorText(m_usbStrerror(), wxConvLocal);
			wxLogMessage(wxT("GET_DATA, ret: %d, err=%s"), ret, errorText.c_str());
			return ret;
		}

		return 0;
	} else if (ret == 0) {
		char status;
		int ret = io(0xC0, GET_AD_STATUS, 0, 0, &status, 1, USB_TIMEOUT);
		if (ret < 0) {
			wxString error(m_usbStrerror(), wxConvLocal);
			wxLogMessage(wxT("LAST_FRAME, ret: %d, err=%s"), ret, error.c_str());

			if (ret == -19)		// -ENODEV
				return ret;

			return 0;
		}

		if ((status & LAST_FRAME) == LAST_FRAME)
			end = true;
	}

	return ret;
}

void CGMSKModemLibUsb::writeHeader(unsigned char* header, unsigned int length)
{
	wxASSERT(header != NULL);
	wxASSERT(length >= (RADIO_HEADER_LENGTH_BYTES - 2U));

	io(0x40, SET_MyCALL2,  0, 0, (char*)(header + 35U), SHORT_CALLSIGN_LENGTH, USB_TIMEOUT);
	io(0x40, SET_MyCALL,   0, 0, (char*)(header + 27U), LONG_CALLSIGN_LENGTH,  USB_TIMEOUT);
	io(0x40, SET_YourCALL, 0, 0, (char*)(header + 19U), LONG_CALLSIGN_LENGTH,  USB_TIMEOUT);
	io(0x40, SET_RPT1CALL, 0, 0, (char*)(header + 11U), LONG_CALLSIGN_LENGTH,  USB_TIMEOUT);
	io(0x40, SET_RPT2CALL, 0, 0, (char*)(header + 3U),  LONG_CALLSIGN_LENGTH,  USB_TIMEOUT);
	io(0x40, SET_FLAGS,    0, 0, (char*)(header + 0U),  3U, USB_TIMEOUT);
}

TRISTATE CGMSKModemLibUsb::hasSpace()
{
	unsigned char space;
	int rc = io(0xC0, GET_REMAINSPACE, 0, 0, (char*)&space, 1, USB_TIMEOUT);
	if (rc != 1) {
		wxString errorText(m_usbStrerror(), wxConvLocal);
		wxLogMessage(wxT("GET_REMAINSPACE, ret: %d, err=%s"), rc, errorText.c_str());
		return STATE_UNKNOWN;
	}

	if (space >= DV_FRAME_LENGTH_BYTES)
		return STATE_TRUE;
	else
		return STATE_FALSE;
}

TRISTATE CGMSKModemLibUsb::getPTT()
{
	char status;
	int rc = io(0xC0, GET_AD_STATUS, 0, 0, &status, 1, USB_TIMEOUT);
	if (rc <= 0) {
		wxString errorText(m_usbStrerror(), wxConvLocal);
		wxLogMessage(wxT("GET_PTT, ret: %d, err=%s"), rc, errorText.c_str());
		return STATE_UNKNOWN;
	}

	if ((status & PTT_OnOff) == PTT_OnOff)
		return STATE_TRUE;
	else
		return STATE_FALSE;
}

void CGMSKModemLibUsb::setPTT(bool on)
{
	char c;
	io(0x40, SET_PTT, on ? PTT_ON : PTT_OFF, 0, &c, 0, USB_TIMEOUT);
}

int CGMSKModemLibUsb::writeData(unsigned char* data, unsigned int length)
{
	wxASSERT(data != NULL);
	wxASSERT(length > 0U && length <= DV_FRAME_LENGTH_BYTES);

	if (length > GMSK_MODEM_DATA_LENGTH) {
		int ret = io(0x40, PUT_DATA, 0, 0, (char*)data, GMSK_MODEM_DATA_LENGTH, USB_TIMEOUT);
		if (ret < 0) {
			if (ret == -19) {		// -ENODEV
				wxString errorText(m_usbStrerror(), wxConvLocal);
				wxLogMessage(wxT("PUT_DATA 1, ret: %d, err=%s"), ret, errorText.c_str());
				return ret;
			}

			return 0;
		}

		// Give libUSB some recovery time
		::wxMilliSleep(3UL);

		ret = io(0x40, PUT_DATA, 0, 0, (char*)(data + GMSK_MODEM_DATA_LENGTH), length - GMSK_MODEM_DATA_LENGTH, USB_TIMEOUT);
		if (ret < 0) {
			if (ret == -19) {		// -ENODEV
				wxString errorText(m_usbStrerror(), wxConvLocal);
				wxLogMessage(wxT("PUT_DATA 2, ret: %d, err=%s"), ret, errorText.c_str());
				return ret;
			}

			return int(GMSK_MODEM_DATA_LENGTH);
		}
	
		return length;
	} else {
		int ret = io(0x40, PUT_DATA, 0, 0, (char*)data, length, USB_TIMEOUT);
		if (ret < 0) {
			if (ret == -19) {			// -ENODEV
				wxString errorText(m_usbStrerror(), wxConvLocal);
				wxLogMessage(wxT("PUT_DATA, ret: %d, err=%s"), ret, errorText.c_str());
				return ret;
			}

			return 0;
		}

		return length;
	}
}

void CGMSKModemLibUsb::close()
{
	wxASSERT(m_dev != NULL);

	m_usbClose(m_dev);
	m_dev = NULL;
}

bool CGMSKModemLibUsb::openModem()
{
	if (!m_loaded)
		return false;

	m_usbInit();
	m_usbFindBusses();
	m_usbFindDevices();

	for (struct usb_bus* bus = m_usbGetBusses(); bus != NULL; bus = bus->next) {
		for (struct usb_device* dev = bus->devices; dev != NULL; dev = dev->next) {
			if (dev->descriptor.idVendor == VENDOR_ID && dev->descriptor.idProduct == m_address) {
				m_dev = m_usbOpen(dev);
				break;
			}
		}
	}

	if (m_dev == NULL)
		return false;

	m_usbSetConfiguration(m_dev, 1);

	char c;
	io(0x40, SET_AD_INIT, 0, 0, &c, 0, USB_TIMEOUT);

	setPTT(false);

	return true;
}

int CGMSKModemLibUsb::io(int requestType, int request, int value, int index, char* bytes, int size, int timeout)
{
	wxASSERT(m_dev != NULL);
	wxASSERT(bytes != NULL);

	int ret = 0;
	for (unsigned int i = 0U; i < 4U; i++) {
		ret = m_usbControlMsg(m_dev, requestType, request, value, index, bytes, size, timeout);
		if (ret >= 0)
			return ret;

		if (ret == -19)		// ENODEV
			return ret;

		::wxMilliSleep(5UL);
	}

	return ret;
}

#else

CGMSKModemLibUsb::CGMSKModemLibUsb(unsigned int address) :
m_address(address),
m_context(NULL),
m_handle(NULL),
m_brokenSpace(false)
{
	::libusb_init(&m_context);
}


CGMSKModemLibUsb::~CGMSKModemLibUsb()
{
	wxASSERT(m_context != NULL);

	::libusb_exit(m_context);
}

bool CGMSKModemLibUsb::open()
{
	wxASSERT(m_context != NULL);
	wxASSERT(m_handle == NULL);

	bool ret1 = openModem();
	if (!ret1) {
		wxLogError(wxT("Cannot find the GMSK Modem with address: 0x%04X"), m_address);
		return false;
	}

	wxLogInfo(wxT("Found the GMSK Modem with address: 0x%04X"), m_address);

	wxString version;

	int ret2;
	do {
		unsigned char buffer[GMSK_MODEM_DATA_LENGTH];
		ret2 = io(0xC0, GET_VERSION, 0, 0, buffer, GMSK_MODEM_DATA_LENGTH, USB_TIMEOUT);
		if (ret2 > 0) {
			wxString text((char*)buffer, wxConvLocal, ret2);
			version.Append(text);
		} else if (ret2 < 0) {
			wxLogError(wxT("GET_VERSION, err=%d"), ret2);
			close();
			return false;
		}
	} while (ret2 == int(GMSK_MODEM_DATA_LENGTH));

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

bool CGMSKModemLibUsb::readHeader(unsigned char* header, unsigned int length)
{
	wxASSERT(header != NULL);
	wxASSERT(length > (RADIO_HEADER_LENGTH_BYTES * 2U));

	unsigned int offset = 0U;

	while (offset < RADIO_HEADER_LENGTH_BYTES) {
		int ret = io(0xC0, GET_HEADER, 0, 0, header + offset, 8, USB_TIMEOUT);
		if (ret < 0) {
			wxLogMessage(wxT("GET_HEADER, err=%d"), ret);

			if (ret == LIBUSB_ERROR_NO_DEVICE)
				return false;

			::wxMilliSleep(10UL);
		} else if (ret == 0) {
			if (offset == 0U)
				return false;

			::wxMilliSleep(10UL);

			unsigned char status;
			int ret = io(0xC0, GET_AD_STATUS, 0, 0, &status, 1, USB_TIMEOUT);
			if (ret < 0) {
				wxLogMessage(wxT("GET_COS, err=%d"), ret);

				if (ret == LIBUSB_ERROR_NO_DEVICE)
					return false;

				::wxMilliSleep(10UL);
			} else if (ret > 0) {
				if ((status & COS_OnOff) == COS_OnOff)
					offset = 0U;
			}
		} else {
			offset += ret;
		}
	}

	unsigned char status;
	int ret = io(0xC0, GET_AD_STATUS, 0, 0, &status, 1, USB_TIMEOUT);
	if (ret < 0) {
		wxLogMessage(wxT("GET_CRC, err=%d"), ret);
		return false;
	}

	if ((status & CRC_ERROR) == CRC_ERROR) {
		wxLogMessage(wxT("Header - CRC Error"));
		return false;
	}

	return true;
}

int CGMSKModemLibUsb::readData(unsigned char* data, unsigned int length, bool& end)
{
	wxASSERT(length > 0U);

	end = false;

	int ret = io(0xC0, GET_DATA, 0, 0, data, GMSK_MODEM_DATA_LENGTH, USB_TIMEOUT);
	if (ret < 0) {
		if (ret == LIBUSB_ERROR_NO_DEVICE) {
			wxLogMessage(wxT("GET_DATA, err=%d"), ret);
			return ret;
		}

		return 0;
	} else if (ret == 0) {
		unsigned char status;
		int ret = io(0xC0, GET_AD_STATUS, 0, 0, &status, 1, USB_TIMEOUT);
		if (ret < 0) {
			wxLogMessage(wxT("LAST_FRAME, err=%d"), ret);

			if (ret == LIBUSB_ERROR_NO_DEVICE)
				return ret;

			return 0;
		}

		if ((status & LAST_FRAME) == LAST_FRAME)
			end = true;
	}

	return ret;
}

void CGMSKModemLibUsb::writeHeader(unsigned char* header, unsigned int length)
{
	wxASSERT(header != NULL);
	wxASSERT(length >= (RADIO_HEADER_LENGTH_BYTES - 2U));

	io(0x40, SET_MyCALL2,  0, 0, header + 35U, SHORT_CALLSIGN_LENGTH, USB_TIMEOUT);
	io(0x40, SET_MyCALL,   0, 0, header + 27U, LONG_CALLSIGN_LENGTH,  USB_TIMEOUT);
	io(0x40, SET_YourCALL, 0, 0, header + 19U, LONG_CALLSIGN_LENGTH,  USB_TIMEOUT);
	io(0x40, SET_RPT1CALL, 0, 0, header + 11U, LONG_CALLSIGN_LENGTH,  USB_TIMEOUT);
	io(0x40, SET_RPT2CALL, 0, 0, header + 3U,  LONG_CALLSIGN_LENGTH,  USB_TIMEOUT);
	io(0x40, SET_FLAGS,    0, 0, header + 0U,  3U, USB_TIMEOUT);
}

TRISTATE CGMSKModemLibUsb::hasSpace()
{
	unsigned char space;
	int rc = io(0xC0, GET_REMAINSPACE, 0, 0, &space, 1, USB_TIMEOUT);
	if (rc != 1) {
		wxLogMessage(wxT("GET_REMAINSPACE, err=%d"), rc);
		return STATE_UNKNOWN;
	}

	if (space >= DV_FRAME_LENGTH_BYTES)
		return STATE_TRUE;
	else
		return STATE_FALSE;
}

TRISTATE CGMSKModemLibUsb::getPTT()
{
	unsigned char status;
	int rc = io(0xC0, GET_AD_STATUS, 0, 0, &status, 1, USB_TIMEOUT);
	if (rc != 1) {
		wxLogMessage(wxT("GET_PTT, err=%d"), rc);
		return STATE_UNKNOWN;
	}

	if ((status & PTT_OnOff) == PTT_OnOff)
		return STATE_TRUE;
	else
		return STATE_FALSE;
}

void CGMSKModemLibUsb::setPTT(bool on)
{
	unsigned char c;
	io(0x40, SET_PTT, on ? PTT_ON : PTT_OFF, 0, &c, 0, USB_TIMEOUT);
}

int CGMSKModemLibUsb::writeData(unsigned char* data, unsigned int length)
{
	wxASSERT(data != NULL);
	wxASSERT(length > 0U && length <= DV_FRAME_LENGTH_BYTES);

	if (length > GMSK_MODEM_DATA_LENGTH) {
		int ret = io(0x40, PUT_DATA, 0, 0, data, GMSK_MODEM_DATA_LENGTH, USB_TIMEOUT);
		if (ret < 0) {
			if (ret == LIBUSB_ERROR_NO_DEVICE) {
				wxLogMessage(wxT("PUT_DATA 1, err=%d"), ret);
				return ret;
			}

			return 0;
		}

		// Give libUSB some recovery time
		::wxMilliSleep(3UL);

		ret = io(0x40, PUT_DATA, 0, 0, data + GMSK_MODEM_DATA_LENGTH, length - GMSK_MODEM_DATA_LENGTH, USB_TIMEOUT);
		if (ret < 0) {
			if (ret == LIBUSB_ERROR_NO_DEVICE) {
				wxLogMessage(wxT("PUT_DATA 2, err=%d"), ret);
				return ret;
			}

			return int(GMSK_MODEM_DATA_LENGTH);
		}
	
		return length;
	} else {
		int ret = io(0x40, PUT_DATA, 0, 0, data, length, USB_TIMEOUT);
		if (ret < 0) {
			if (ret == LIBUSB_ERROR_NO_DEVICE) {
				wxLogMessage(wxT("PUT_DATA, err=%d"), ret);
				return ret;
			}

			return 0;
		}

		return length;
	}
}

void CGMSKModemLibUsb::close()
{
	wxASSERT(m_handle != NULL);

	::libusb_close(m_handle);
	m_handle = NULL;
}

bool CGMSKModemLibUsb::openModem()
{
	m_handle = ::libusb_open_device_with_vid_pid(m_context, VENDOR_ID, m_address);
	if (m_handle == NULL)
		return false;

	::libusb_set_configuration(m_handle, 1);

	unsigned char c;
	io(0x40, SET_AD_INIT, 0, 0, &c, 0, USB_TIMEOUT);

	setPTT(false);

	return true;
}

int CGMSKModemLibUsb::io(uint8_t requestType, uint8_t request, uint16_t value, uint16_t index, unsigned char* data, uint16_t length, unsigned int timeout)
{
	wxASSERT(m_handle != NULL);
	wxASSERT(data != NULL);

	int ret = 0;
	for (unsigned int i = 0U; i < 4U; i++) {
		ret = ::libusb_control_transfer(m_handle, requestType, request, value, index, data, length, timeout);
		if (ret >= 0)
			return ret;

		if (ret == LIBUSB_ERROR_NO_DEVICE)
			return ret;

		::wxMilliSleep(5UL);
	}

	return ret;
}

#endif
