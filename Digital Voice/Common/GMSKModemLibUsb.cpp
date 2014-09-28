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

#include "GMSKModemLibUsb.h"
#include "DStarDefines.h"

const unsigned int VENDOR_ID = 0x04D8U;

const int USB_TIMEOUT     = 100;

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
m_broken(false)
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
	if (!m_loaded)
		return false;

	wxASSERT(m_dev == NULL);

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

	if (m_dev == NULL) {
		wxLogError(wxT("Cannot find the GMSK Modem with address: 0x%04X"), m_address);
		return false;
	}

	wxLogInfo(wxT("Found the GMSK Modem with address: 0x%04X"), m_address);

	m_usbSetConfiguration(m_dev, 1);

	char c;
	m_usbControlMsg(m_dev, 0x40, SET_AD_INIT, 0, 0, &c, 0, USB_TIMEOUT);

	wxString version;

	int ret;
	do {
		char buffer[GMSK_MODEM_DATA_LENGTH];
		ret = m_usbControlMsg(m_dev, 0xC0, GET_VERSION, 0, 0, buffer, GMSK_MODEM_DATA_LENGTH, USB_TIMEOUT);
		if (ret > 0) {
			wxString text(buffer, wxConvLocal, ret);
			version.Append(text);
		} else if (ret < 0) {
			wxString error(m_usbStrerror(), wxConvLocal);
			wxLogError(wxT("GET_VERSION, ret: %d, err=%s"), ret, error.c_str());
			m_usbClose(m_dev);
			m_dev = NULL;
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

bool CGMSKModemLibUsb::isBroken() const
{
	return m_broken;
}

CHeaderData* CGMSKModemLibUsb::readHeader()
{
	wxASSERT(m_dev != NULL);

	unsigned char header[RADIO_HEADER_LENGTH_BYTES * 2U];
	unsigned int length = 0U;

	while (length < RADIO_HEADER_LENGTH_BYTES) {
		int ret = m_usbControlMsg(m_dev, 0xC0, GET_HEADER, 0, 0, (char*)(header + length), 8, USB_TIMEOUT);
		if (ret < 0) {
			wxString errorText(m_usbStrerror(), wxConvLocal);
			wxLogMessage(wxT("GET_HEADER, ret: %d, err=%s"), ret, errorText.c_str());

			if (ret == -19)		// -ENODEV
				return NULL;

			::wxMilliSleep(10UL);
		} else if (ret == 0) {
			if (length == 0U)
				return NULL;

			::wxMilliSleep(10UL);

			char status;
			int ret = m_usbControlMsg(m_dev, 0xC0, GET_AD_STATUS, 0, 0, &status, 1, USB_TIMEOUT);
			if (ret < 0) {
				wxString errorText(m_usbStrerror(), wxConvLocal);
				wxLogMessage(wxT("GET_COS, ret: %d, err=%s"), ret, errorText.c_str());

				if (ret == -19)		// -ENODEV
					return NULL;

				::wxMilliSleep(10UL);
			} else if (ret > 0) {
				if ((status & COS_OnOff) == COS_OnOff)
					length = 0U;
			}
		} else {
			length += ret;
		}
	}

	char status;
	int ret = m_usbControlMsg(m_dev, 0xC0, GET_AD_STATUS, 0, 0, &status, 1, USB_TIMEOUT);
	if (ret < 0) {
		wxString errorText(m_usbStrerror(), wxConvLocal);
		wxLogMessage(wxT("GET_CRC, ret: %d, err=%s"), ret, errorText.c_str());

		if (ret == -19)		// -ENODEV
			return NULL;

		return 0;
	}

	if ((status & CRC_ERROR) == CRC_ERROR) {
		wxLogMessage(wxT("Header - CRC Error"));
		return NULL;
	}

	return new CHeaderData(header, RADIO_HEADER_LENGTH_BYTES, false);
}

int CGMSKModemLibUsb::readData(unsigned char* data, unsigned int length, bool& end)
{
	wxASSERT(m_dev != NULL);
	wxASSERT(data != NULL);
	wxASSERT(length > 0U);

	end = false;

	int ret = m_usbControlMsg(m_dev, 0xC0, GET_DATA, 0, 0, (char*)data, GMSK_MODEM_DATA_LENGTH, USB_TIMEOUT);
	if (ret < 0) {
		wxString errorText(m_usbStrerror(), wxConvLocal);
		wxLogMessage(wxT("GET_DATA, ret: %d, err=%s"), ret, errorText.c_str());

		if (ret == -19)		// -ENODEV
			return ret;

		return 0;
	} else if (ret == 0) {
		char status;
		int ret = m_usbControlMsg(m_dev, 0xC0, GET_AD_STATUS, 0, 0, &status, 1, USB_TIMEOUT);
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

bool CGMSKModemLibUsb::writeHeader(const CHeaderData& header)
{
	wxASSERT(m_dev != NULL);

	char myCall1[LONG_CALLSIGN_LENGTH];
	char myCall2[SHORT_CALLSIGN_LENGTH];
	char yourCall[LONG_CALLSIGN_LENGTH];
	char rptCall1[LONG_CALLSIGN_LENGTH];
	char rptCall2[LONG_CALLSIGN_LENGTH];

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

	int ret = m_usbControlMsg(m_dev, 0x40, SET_MyCALL2, 0, 0, myCall2, SHORT_CALLSIGN_LENGTH, USB_TIMEOUT);
	if (ret < 0) {
		wxString errorText(m_usbStrerror(), wxConvLocal);
		wxLogMessage(wxT("SET_MyCALL2, ret: %d, err=%s"), ret, errorText.c_str());
		return false;
	}

	ret = m_usbControlMsg(m_dev, 0x40, SET_MyCALL, 0, 0, myCall1, LONG_CALLSIGN_LENGTH, USB_TIMEOUT);
	if (ret < 0) {
		wxString errorText(m_usbStrerror(), wxConvLocal);
		wxLogMessage(wxT("SET_MyCALL, ret: %d, err=%s"), ret, errorText.c_str());
		return false;
	}

	ret = m_usbControlMsg(m_dev, 0x40, SET_YourCALL, 0, 0, yourCall, LONG_CALLSIGN_LENGTH, USB_TIMEOUT);
	if (ret < 0) {
		wxString errorText(m_usbStrerror(), wxConvLocal);
		wxLogMessage(wxT("SET_YourCALL, ret: %d, err=%s"), ret, errorText.c_str());
		return false;
	}

	ret = m_usbControlMsg(m_dev, 0x40, SET_RPT1CALL, 0, 0, rptCall1, LONG_CALLSIGN_LENGTH, USB_TIMEOUT);
	if (ret < 0) {
		wxString errorText(m_usbStrerror(), wxConvLocal);
		wxLogMessage(wxT("SET_RPT1CALL, ret: %d, err=%s"), ret, errorText.c_str());
		return false;
	}

	ret = m_usbControlMsg(m_dev, 0x40, SET_RPT2CALL, 0, 0, rptCall2, LONG_CALLSIGN_LENGTH, USB_TIMEOUT);
	if (ret < 0) {
		wxString errorText(m_usbStrerror(), wxConvLocal);
		wxLogMessage(wxT("SET_RPT2CALL, ret: %d, err=%s"), ret, errorText.c_str());
		return false;
	}

	char flags[3U];
	flags[0U] = header.getFlag1();
	flags[1U] = header.getFlag2();
	flags[2U] = header.getFlag3();

	ret = m_usbControlMsg(m_dev, 0x40, SET_FLAGS, 0, 0, flags, 3, USB_TIMEOUT);
	if (ret < 0) {
		wxString errorText(m_usbStrerror(), wxConvLocal);
		wxLogMessage(wxT("SET_FLAGS, ret: %d, err=%s"), ret, errorText.c_str());
		return false;
	}

	return true;
}

TRISTATE CGMSKModemLibUsb::hasSpace()
{
	wxASSERT(m_dev != NULL);

	if (m_broken)
		return STATE_TRUE;

	unsigned char space;
	int rc = m_usbControlMsg(m_dev, 0xC0, GET_REMAINSPACE, 0, 0, (char*)&space, 1, USB_TIMEOUT);
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
	wxASSERT(m_dev != NULL);

	char status;
	int rc = m_usbControlMsg(m_dev, 0xC0, GET_AD_STATUS, 0, 0, &status, 1, USB_TIMEOUT);
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

bool CGMSKModemLibUsb::setPTT(bool on)
{
	wxASSERT(m_dev != NULL);

	char c;
	int ret = m_usbControlMsg(m_dev, 0x40, SET_PTT, on ? PTT_ON : PTT_OFF, 0, &c, 0, USB_TIMEOUT);
	if (ret < 0) {
		wxString errorText(m_usbStrerror(), wxConvLocal);
		wxLogMessage(wxT("SET_PTT, ret: %d, err=%s"), ret, errorText.c_str());
		return false;
	}

	return true;
}

int CGMSKModemLibUsb::writeData(const unsigned char* data, unsigned int length)
{
	wxASSERT(m_dev != NULL);
	wxASSERT(data != NULL);
	wxASSERT(length > 0U && length <= DV_FRAME_LENGTH_BYTES);

	unsigned char* buffer = const_cast<unsigned char*>(data);

	if (length > GMSK_MODEM_DATA_LENGTH) {
		int ret = m_usbControlMsg(m_dev, 0x40, PUT_DATA, 0, 0, (char*)buffer, GMSK_MODEM_DATA_LENGTH, USB_TIMEOUT);
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

		ret = m_usbControlMsg(m_dev, 0x40, PUT_DATA, 0, 0, (char*)(buffer + GMSK_MODEM_DATA_LENGTH), length - GMSK_MODEM_DATA_LENGTH, USB_TIMEOUT);
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
		int ret = m_usbControlMsg(m_dev, 0x40, PUT_DATA, 0, 0, (char*)buffer, length, USB_TIMEOUT);
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

#else

CGMSKModemLibUsb::CGMSKModemLibUsb(unsigned int address) :
m_address(address),
m_context(NULL),
m_handle(NULL),
m_broken(false)
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

	m_handle = ::libusb_open_device_with_vid_pid(m_context, VENDOR_ID, m_address);
	if (m_handle == NULL) {
		wxLogError(wxT("Cannot open the GMSK Modem with address: 0x%04X"), m_address);
		return false;
	}

	wxLogInfo(wxT("Found the GMSK Modem with address: 0x%04X"), m_address);

	::libusb_set_configuration(m_handle, 1);

	unsigned char c;
	int ret = ::libusb_control_transfer(m_handle, 0x40, SET_AD_INIT, 0, 0, &c, 0, USB_TIMEOUT);
	if (ret < 0) {
		wxLogError(wxT("SET_AD_INIT, err=%d"), ret);
		close();
		return false;
	}

	wxString version;

	do {
		unsigned char buffer[GMSK_MODEM_DATA_LENGTH];
		ret = ::libusb_control_transfer(m_handle, 0xC0, GET_VERSION, 0, 0, buffer, GMSK_MODEM_DATA_LENGTH, USB_TIMEOUT);
		if (ret > 0) {
			wxString text((char*)buffer, wxConvLocal, ret);
			version.Append(text);
		} else if (ret < 0) {
			wxLogError(wxT("GET_VERSION, err=%d"), ret);
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

bool CGMSKModemLibUsb::isBroken() const
{
	return m_broken;
}

CHeaderData* CGMSKModemLibUsb::readHeader()
{
	wxASSERT(m_handle != NULL);

	unsigned char header[RADIO_HEADER_LENGTH_BYTES * 2U];
	unsigned int length = 0U;

	while (length < RADIO_HEADER_LENGTH_BYTES) {
		int ret = ::libusb_control_transfer(m_handle, 0xC0, GET_HEADER, 0, 0, header + length, 8, USB_TIMEOUT);
		if (ret < 0) {
			wxLogMessage(wxT("GET_HEADER, err=%d"), ret);

			if (ret == LIBUSB_ERROR_NO_DEVICE)
				return NULL;

			::wxMilliSleep(10UL);
		} else if (ret == 0) {
			if (length == 0U)
				return NULL;

			::wxMilliSleep(10UL);

			unsigned char status;
			int ret = ::libusb_control_transfer(m_handle, 0xC0, GET_AD_STATUS, 0, 0, &status, 1, USB_TIMEOUT);
			if (ret < 0) {
				wxLogMessage(wxT("GET_COS, err=%d"), ret);

				if (ret == LIBUSB_ERROR_NO_DEVICE)
					return NULL;

				::wxMilliSleep(10UL);
			} else if (ret > 0) {
				if ((status & COS_OnOff) == COS_OnOff)
					length = 0U;
			}
		} else {
			length += ret;
		}
	}

	unsigned char status;
	int ret = ::libusb_control_transfer(m_handle, 0xC0, GET_AD_STATUS, 0, 0, &status, 1, USB_TIMEOUT);
	if (ret < 0) {
		wxLogMessage(wxT("GET_CRC, err=%d"), ret);
		return NULL;
	}

	if ((status & CRC_ERROR) == CRC_ERROR) {
		wxLogMessage(wxT("Header - CRC Error"));
		return NULL;
	}

	return new CHeaderData(header, RADIO_HEADER_LENGTH_BYTES, false);
}

int CGMSKModemLibUsb::readData(unsigned char* data, unsigned int length, bool& end)
{
	wxASSERT(m_handle != NULL);
	wxASSERT(length > 0U);

	end = false;

	int ret = ::libusb_control_transfer(m_handle, 0xC0, GET_DATA, 0, 0, data, GMSK_MODEM_DATA_LENGTH, USB_TIMEOUT);
	if (ret < 0) {
		wxLogMessage(wxT("GET_DATA, err=%d"), ret);

		if (ret == LIBUSB_ERROR_NO_DEVICE)
			return ret;

		return 0;
	} else if (ret == 0) {
		unsigned char status;
		int ret = ::libusb_control_transfer(m_handle, 0xC0, GET_AD_STATUS, 0, 0, &status, 1, USB_TIMEOUT);
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

bool CGMSKModemLibUsb::writeHeader(const CHeaderData& header)
{
	wxASSERT(m_handle != NULL);

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

	int ret = ::libusb_control_transfer(m_handle, 0x40, SET_MyCALL2, 0, 0, myCall2, SHORT_CALLSIGN_LENGTH, USB_TIMEOUT);
	if (ret < 0) {
		wxLogMessage(wxT("SET_MyCALL2, err=%d"), ret);
		return false;
	}

	ret = ::libusb_control_transfer(m_handle, 0x40, SET_MyCALL, 0, 0, myCall1, LONG_CALLSIGN_LENGTH,  USB_TIMEOUT);
	if (ret < 0) {
		wxLogMessage(wxT("SET_MyCALL, err=%d"), ret);
		return false;
	}

	ret = ::libusb_control_transfer(m_handle, 0x40, SET_YourCALL, 0, 0, yourCall, LONG_CALLSIGN_LENGTH,  USB_TIMEOUT);
	if (ret < 0) {
		wxLogMessage(wxT("SET_YourCALL, err=%d"), ret);
		return false;
	}

	ret = ::libusb_control_transfer(m_handle, 0x40, SET_RPT1CALL, 0, 0, rptCall1, LONG_CALLSIGN_LENGTH,  USB_TIMEOUT);
	if (ret < 0) {
		wxLogMessage(wxT("SET_RPT1CALL, err=%d"), ret);
		return false;
	}

	ret = ::libusb_control_transfer(m_handle, 0x40, SET_RPT2CALL, 0, 0, rptCall2, LONG_CALLSIGN_LENGTH,  USB_TIMEOUT);
	if (ret < 0) {
		wxLogMessage(wxT("SET_RPT2CALL, err=%d"), ret);
		return false;
	}

	unsigned char flags[3U];
	flags[0U] = header.getFlag1();
	flags[1U] = header.getFlag2();
	flags[2U] = header.getFlag3();

	ret = ::libusb_control_transfer(m_handle, 0x40, SET_FLAGS, 0, 0, flags, 3, USB_TIMEOUT);
	if (ret < 0) {
		wxLogMessage(wxT("SET_FLAGS, err=%d"), ret);
		return false;
	}

	return true;
}

TRISTATE CGMSKModemLibUsb::hasSpace()
{
	wxASSERT(m_handle != NULL);

	if (m_broken)
		return STATE_TRUE;

	unsigned char space;
	int rc = ::libusb_control_transfer(m_handle, 0xC0, GET_REMAINSPACE, 0, 0, &space, 1, USB_TIMEOUT);
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
	wxASSERT(m_handle != NULL);

	unsigned char status;
	int rc = ::libusb_control_transfer(m_handle, 0xC0, GET_AD_STATUS, 0, 0, &status, 1, USB_TIMEOUT);
	if (rc != 1) {
		wxLogMessage(wxT("GET_PTT, err=%d"), rc);
		return STATE_UNKNOWN;
	}

	if ((status & PTT_OnOff) == PTT_OnOff)
		return STATE_TRUE;
	else
		return STATE_FALSE;
}

bool CGMSKModemLibUsb::setPTT(bool on)
{
	wxASSERT(m_handle != NULL);

	unsigned char c;
	int ret = ::libusb_control_transfer(m_handle, 0x40, SET_PTT, on ? PTT_ON : PTT_OFF, 0, &c, 0, USB_TIMEOUT);
	if (ret < 0) {
		wxLogMessage(wxT("SET_PTT, err=%d"), ret);
		return false;
	}

	return true;
}

int CGMSKModemLibUsb::writeData(const unsigned char* data, unsigned int length)
{
	wxASSERT(m_handle != NULL);
	wxASSERT(data != NULL);
	wxASSERT(length > 0U && length <= DV_FRAME_LENGTH_BYTES);

	unsigned char* buffer = const_cast<unsigned char*>(data);

	if (length > GMSK_MODEM_DATA_LENGTH) {
		int ret = ::libusb_control_transfer(m_handle, 0x40, PUT_DATA, 0, 0, buffer, GMSK_MODEM_DATA_LENGTH, USB_TIMEOUT);
		if (ret < 0) {
			if (ret == LIBUSB_ERROR_NO_DEVICE) {
				wxLogMessage(wxT("PUT_DATA 1, err=%d"), ret);
				return ret;
			}

			return 0;
		}

		// Give libUSB some recovery time
		::wxMilliSleep(3UL);

		ret = ::libusb_control_transfer(m_handle, 0x40, PUT_DATA, 0, 0, buffer + GMSK_MODEM_DATA_LENGTH, length - GMSK_MODEM_DATA_LENGTH, USB_TIMEOUT);
		if (ret < 0) {
			if (ret == LIBUSB_ERROR_NO_DEVICE) {
				wxLogMessage(wxT("PUT_DATA 2, err=%d"), ret);
				return ret;
			}

			return int(GMSK_MODEM_DATA_LENGTH);
		}
	
		return length;
	} else {
		int ret = ::libusb_control_transfer(m_handle, 0x40, PUT_DATA, 0, 0, buffer, length, USB_TIMEOUT);
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

#endif
