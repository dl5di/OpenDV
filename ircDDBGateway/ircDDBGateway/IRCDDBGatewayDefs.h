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

#ifndef	IRCDDBGatewayDefs_H
#define	IRCDDBGatewayDefs_H

#include <wx/wx.h>

const wxString APPLICATION_NAME = wxT("ircDDB Gateway");

const wxString LOG_BASE_NAME    = wxT("ircDDBGateway");

const wxString CONFIG_FILE_NAME = wxT("ircddbgateway");

const wxString STATUS1_FILE_NAME = wxT("status1.txt");
const wxString STATUS2_FILE_NAME = wxT("status2.txt");
const wxString STATUS3_FILE_NAME = wxT("status3.txt");
const wxString STATUS4_FILE_NAME = wxT("status4.txt");
const wxString STATUS5_FILE_NAME = wxT("status5.txt");

const wxString RESTRICT_FILE_NAME = wxT("restrict.txt");

const unsigned int MAX_OUTGOING       = 6U;
const unsigned int MAX_REPEATERS      = 4U;
const unsigned int MAX_DEXTRA_LINKS   = 5U;
const unsigned int MAX_DPLUS_LINKS    = 5U;
const unsigned int MAX_DCS_LINKS      = 5U;
const unsigned int MAX_STARNETS       = 5U;
const unsigned int MAX_ROUTES         = MAX_REPEATERS + 5U;
const unsigned int MAX_DD_ROUTES      = 20U;

#endif
