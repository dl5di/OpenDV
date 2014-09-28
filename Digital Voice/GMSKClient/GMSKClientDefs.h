/*
 *   Copyright (C) 2010,2012 by Jonathan Naylor G4KLX
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

#ifndef	GMSKClientDefs_H
#define	GMSKClientDefs_H

#include <wx/wx.h>

const unsigned int LONG_CALLSIGN_WIDTH   = 80U;
const unsigned int SHORT_CALLSIGN_WIDTH  = 50U;

const unsigned int BORDER_SIZE   = 5U;
const unsigned int LABEL_WIDTH   = 80U;
const unsigned int CONTROL_WIDTH = 130U;

const unsigned int INFO_WIDTH    = 690U;
const unsigned int INFO_HEIGHT   = 160U;

const unsigned int HEARD_WIDTH   = 690U;
const unsigned int HEARD_HEIGHT  = 300U;

const unsigned int DATETIME_WIDTH = 135U;
const unsigned int CALLSIGN_WIDTH = 100U;
const unsigned int MESSAGE_WIDTH  = 250U;

const wxString APPLICATION_NAME = wxT("GMSK Client");

const wxString LOG_FILE_NAME    = wxT("GMSKClient");

const wxString CQCQCQ = wxT("CQCQCQ");
const wxString DIRECT = wxT("DIRECT  ");
const wxString UNUSED = wxT("<unused>");

enum GMSK_MODEM_TYPE {
	GMT_LIBUSB,
	GMT_WINUSB
};

#endif
