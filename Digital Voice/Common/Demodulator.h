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

#ifndef	Demodulator_H
#define	Demodulator_H

#include <wx/wx.h>

enum DEMOD_STATE {
	DEMOD_FALSE,
	DEMOD_TRUE,
	DEMOD_UNKNOWN
};

class IDemodulator {
public:
	virtual DEMOD_STATE decode(wxFloat32 val) = 0;

	virtual void reset() = 0;

private:
};

#endif
