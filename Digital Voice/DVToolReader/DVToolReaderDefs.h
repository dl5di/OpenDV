/*
 *   Copyright (C) 2009,2010 by Jonathan Naylor G4KLX
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

#ifndef	DVToolReaderDefs_H
#define	DVToolReaderDefs_H

#include <wx/wx.h>

const unsigned int LONG_CALLSIGN_WIDTH   = 80U;
const unsigned int SHORT_CALLSIGN_WIDTH  = 50U;

const unsigned int BORDER_SIZE   = 5U;
const unsigned int LABEL_WIDTH   = 80U;
const unsigned int CONTROL_WIDTH = 100U;

const unsigned int INFO_WIDTH    = 690U;
const unsigned int INFO_HEIGHT   = 160U;

const unsigned int HEARD_WIDTH   = 690U;
const unsigned int HEARD_HEIGHT  = 300U;

const unsigned int DATETIME_WIDTH = 125U;
const unsigned int CALLSIGN_WIDTH = 90U;
const unsigned int MESSAGE_WIDTH  = 200U;

const wxString LOG_FILE_NAME    = wxT("DVToolReader");

const wxString APPLICATION_NAME = wxT("DVTool Reader");

#endif
