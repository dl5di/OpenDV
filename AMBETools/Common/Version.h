/*
 *	Copyright (C) 2010-2014 by Jonathan Naylor, G4KLX
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

#ifndef	Version_H
#define	Version_H

#include <wx/wx.h>

const wxString VENDOR_NAME = wxT("G4KLX");

const wxString SVNREV = wxT("$Revision: 722 $ on $Date: 2014-05-21 19:42:16 +0200 (Mi, 21. Mai 2014) $");

#if defined(__WXDEBUG__)
const wxString VERSION = wxT("20140519 - DEBUG");
#else
const wxString VERSION = wxT("20140519");
#endif

#endif
