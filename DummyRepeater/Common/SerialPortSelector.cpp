/*
 *   Copyright (C) 2002-2004,2007-2011,2013,2015 by Jonathan Naylor G4KLX
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

#include "SerialPortSelector.h"

#include <sys/types.h>

#if defined(__APPLE__) && defined(__MACH__)
#include <CoreFoundation/CoreFoundation.h>
#include <IOKit/IOTypes.h>
#include <IOKit/IOKitLib.h>
#include <IOKit/serial/IOSerialKeys.h>
#include <IOKit/IOBSD.h>
#include <sys/param.h>
#include <sys/ioctl.h>
#include <termios.h>
#elif defined(__WINDOWS__)
#include <setupapi.h>
#include <winioctl.h>
#else
#include <wx/dir.h>
#endif


wxArrayString CSerialPortSelector::getDevices()
{
	wxArrayString devices;

	devices.Alloc(10);

#if defined(__APPLE__) && defined(__MACH__)
	mach_port_t masterPort;
	kern_return_t ret = ::IOMasterPort(MACH_PORT_NULL, &masterPort);
	if (ret != KERN_SUCCESS)
		return devices;

	CFMutableDictionaryRef match = ::IOServiceMatching(kIOSerialBSDServiceValue);
	if (match == NULL)
		wxLogWarning(wxT("IOServiceMatching() returned NULL"));
	else
		::CFDictionarySetValue(match, CFSTR(kIOSerialBSDTypeKey), CFSTR(kIOSerialBSDRS232Type));

	io_iterator_t services;
	ret = ::IOServiceGetMatchingServices(masterPort, match, &services);
	if (ret != KERN_SUCCESS)
		return devices;

	io_object_t modem;
	while ((modem = ::IOIteratorNext(services))) {
		CFTypeRef filePath = ::IORegistryEntryCreateCFProperty(modem, CFSTR(kIOCalloutDeviceKey), kCFAllocatorDefault, 0);
		if (filePath != NULL) {
			char port[MAXPATHLEN];
			Boolean result = ::CFStringGetCString((const __CFString*)filePath, port, MAXPATHLEN, kCFStringEncodingASCII);
			::CFRelease(filePath);

			if (result)
				devices.Add(port);

			::IOObjectRelease(modem);
		}
	}

	::IOObjectRelease(services);
#elif defined(__WINDOWS__)
	devices.Add(wxT("\\\\.\\COM1"));
	devices.Add(wxT("\\\\.\\COM2"));
	devices.Add(wxT("\\\\.\\COM3"));
	devices.Add(wxT("\\\\.\\COM4"));
	devices.Add(wxT("\\\\.\\COM5"));
	devices.Add(wxT("\\\\.\\COM6"));
	devices.Add(wxT("\\\\.\\COM7"));
	devices.Add(wxT("\\\\.\\COM8"));
	devices.Add(wxT("\\\\.\\COM9"));
	devices.Add(wxT("\\\\.\\COM10"));
	devices.Add(wxT("\\\\.\\COM11"));
	devices.Add(wxT("\\\\.\\COM12"));
	devices.Add(wxT("\\\\.\\COM13"));
	devices.Add(wxT("\\\\.\\COM14"));
	devices.Add(wxT("\\\\.\\COM15"));
	devices.Add(wxT("\\\\.\\COM16"));
	devices.Add(wxT("\\\\.\\COM17"));
	devices.Add(wxT("\\\\.\\COM18"));
	devices.Add(wxT("\\\\.\\COM19"));
#else
	wxDir devDir;
	bool ret = devDir.Open(wxT("/dev"));

	if (ret) {
		wxString fileName;
		ret = devDir.GetFirst(&fileName, wxT("ttyACM*"), wxDIR_FILES);

		while (ret) {
			fileName.Prepend(wxT("/dev/"));
			devices.Add(fileName);

			ret = devDir.GetNext(&fileName);
		}

		ret = devDir.GetFirst(&fileName, wxT("ttyAMA*"), wxDIR_FILES);

		while (ret) {
			fileName.Prepend(wxT("/dev/"));
			devices.Add(fileName);

			ret = devDir.GetNext(&fileName);
		}

		ret = devDir.GetFirst(&fileName, wxT("ttyS*"), wxDIR_FILES);

		while (ret) {
			fileName.Prepend(wxT("/dev/"));
			devices.Add(fileName);

			ret = devDir.GetNext(&fileName);
		}

		ret = devDir.GetFirst(&fileName, wxT("ttyUSB*"), wxDIR_FILES);

		while (ret) {
			fileName.Prepend(wxT("/dev/"));
			devices.Add(fileName);

			ret = devDir.GetNext(&fileName);
		}
	}
#endif

	return devices;
}
