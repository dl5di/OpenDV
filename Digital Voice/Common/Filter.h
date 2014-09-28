/*
 *	Copyright (C) 2009 by Jonathan Naylor, G4KLX
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

#ifndef	Filter_H
#define	Filter_H

#include <wx/wx.h>

class IFilter {
public:
	virtual wxFloat32 process(wxFloat32 in) = 0;
	virtual void      process(const wxFloat32* in, wxFloat32* out, unsigned int length) = 0;

private:
};

#endif
