/*
 *   Copyright (C) 2010,2011,2012 by Jonathan Naylor G4KLX
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

#ifndef	DCSGatewayDefs_H
#define	DCSGatewayDefs_H

#include <wx/wx.h>

const wxString APPLICATION_NAME = wxT("DCS Gateway");
const wxString LOG_BASE_NAME    = wxT("DCSGateway");

#if !defined(__WINDOWS__)
const wxString CONFIG_FILE_NAME = wxT("dcsgateway");
const wxString LOG_DIR  = wxT("/var/log");
const wxString CONF_DIR = wxT("/etc");
#endif

const wxString HOSTS_FILE_NAME  = wxT("DCS_Hosts.txt");

enum RECONNECT {
	RECONNECT_FIXED,
	RECONNECT_5MINS,
	RECONNECT_10MINS,
	RECONNECT_15MINS,
	RECONNECT_20MINS,
	RECONNECT_25MINS,
	RECONNECT_30MINS,
	RECONNECT_NEVER
};

enum LINK_STATE {
	LINK_NONE,
	LINK_LINKING,
	LINK_LINKED
};

enum SLOWDATA_STATE {
	SS_FIRST,
	SS_SECOND
};

#endif
