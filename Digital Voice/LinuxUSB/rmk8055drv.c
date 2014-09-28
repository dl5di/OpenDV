/*
 *   Copyright (C) 2009 by Jonathan Naylor G4KLX
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <usb.h>

const unsigned int VELLEMAN_VENDOR_ID    = 0x10CFU;
const unsigned int VELLEMAN_K8055_BOARD0 = 0x5500U;
const unsigned int VELLEMAN_K8055_BOARD1 = 0x5501U;
const unsigned int VELLEMAN_K8055_BOARD2 = 0x5502U;
const unsigned int VELLEMAN_K8055_BOARD3 = 0x5503U;
const unsigned int VELLEMAN_INTERFACE    = 0U;

int main(int argc, char **argv)
{
	unsigned int count = 0U, removed = 0U;
	struct usb_bus *bus;
	struct usb_device *dev;
	static usb_dev_handle *handle;
	char name[256];

	usb_init();
	usb_find_busses();
	usb_find_devices();

	for (bus = usb_get_busses(); bus != NULL; bus = bus->next) {
		for (dev = bus->devices; dev != NULL; dev = dev->next) {
			if (dev->descriptor.idVendor  == VELLEMAN_VENDOR_ID && 
			   (dev->descriptor.idProduct == VELLEMAN_K8055_BOARD0 ||
			    dev->descriptor.idProduct == VELLEMAN_K8055_BOARD1 ||
			    dev->descriptor.idProduct == VELLEMAN_K8055_BOARD2 ||
			    dev->descriptor.idProduct == VELLEMAN_K8055_BOARD3)) {
				count++;
				if ((handle = usb_open(dev)) == NULL) {
					fprintf(stderr, "rmk8055drv: cannot open the USB device: %s\n", usb_strerror());
					continue;
				}

				memset(name, 0, 256);
				if (usb_get_driver_np(handle, VELLEMAN_INTERFACE, name, 256) == 0) {
					if (strcmp(name, "usbhid") == 0) {
						if (usb_detach_kernel_driver_np(handle, VELLEMAN_INTERFACE) == 0)
							removed++;
						else
							fprintf(stderr, "rmk8055drv: could not disconnect from usbhid: %s\n", usb_strerror());
					}
				}

				usb_close(handle);
			}
		}
	}

	fprintf(stdout, "rmk8055drv: found %u K8055 devices and removed %u from the usbhid driver\n", count, removed);

	return 0;
}
