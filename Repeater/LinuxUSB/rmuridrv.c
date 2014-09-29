/*
 *   Copyright (C) 2009,2011 by Jonathan Naylor G4KLX
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
#include <libusb-1.0/libusb.h>

const unsigned int C108_VENDOR_ID     = 0x0D8CU;
const unsigned int C108_PRODUCT_ID    = 0x000CU;
const unsigned int C108AH_PRODUCT_ID  = 0x013CU;
const unsigned int C119_PRODUCT_ID    = 0x0008U;
const unsigned int CM108_INTERFACE    = 3U;

int main(int argc, char **argv)
{
	unsigned int count = 0U, removed = 0U;
	struct libusb_context *context;
	struct libusb_device_handle *handle;
	struct libusb_device **list;
	struct libusb_device_descriptor desc;
	ssize_t i;
	int err;

	libusb_init(&context);

	ssize_t cnt = libusb_get_device_list(NULL, &list);
	if (cnt <= 0) {
		fprintf(stderr, "rmkuridrv: cannot find any USB devices!\n");
		return 1;
	}

	for (i = 0; i < cnt; i++) {
		err = libusb_get_device_descriptor(list[i], &desc);
		if (err < 0)
			continue;

		if (desc.idVendor == C108_VENDOR_ID     &&
		   (desc.idProduct == C108_PRODUCT_ID   ||
			desc.idProduct == C108AH_PRODUCT_ID ||
			desc.idProduct == C119_PRODUCT_ID)) {
			count++;

			err = libusb_open(list[i], &handle);
			if (err != 0) {
				fprintf(stderr, "rmkuridrv: cannot open the USB device: err=%d\n", err);
				continue;
			}

			if (libusb_kernel_driver_active(handle, CM108_INTERFACE) == 1) {
				err = libusb_detach_kernel_driver(handle, CM108_INTERFACE);
				if (err == 0)
					removed++;
				else
					fprintf(stderr, "rmkuridrv: could not disconnect from kernel driver: err=%d\n", err);
			}

			libusb_close(handle);
		}
	}

	libusb_free_device_list(list, 1);

	libusb_exit(context);

	fprintf(stdout, "rmuridrv: found %u CM108 devices and removed %u from the kernel driver\n", count, removed);

	return 0;
}

