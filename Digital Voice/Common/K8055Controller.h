/*
 *	Copyright (C) 2009,2012 by Jonathan Naylor, G4KLX
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

#ifndef	K8055Controller_H
#define	K8055Controller_H

#include "ExternalController.h"

#include <wx/wx.h>

#if defined(__WINDOWS__)
#include <windows.h>
#else
#include "libusb-1.0/libusb.h"
#endif

class CK8055Controller : public IExternalController {
public:
	CK8055Controller(unsigned int address);
	virtual ~CK8055Controller();

	virtual bool open();

	virtual bool getSquelch();

	virtual void setTransmit(bool tx);

	virtual void close();

private:
	unsigned int          m_address;
	bool                  m_state;
#if defined(__WINDOWS__)
	HANDLE                m_handle;
#else
	libusb_context*       m_context;
	libusb_device_handle* m_handle;
#endif
};

#endif
