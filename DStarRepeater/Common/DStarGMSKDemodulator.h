/*
 *	Copyright (C) 2009,2010,2013,2014 by Jonathan Naylor, G4KLX
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

#ifndef	DStarGMSKDemodulator_H
#define	DStarGMSKDemodulator_H

#include "DStarDefines.h"
#include "FIRFilter.h"
#include "Utils.h"

class CDStarGMSKDemodulator {
public:
	CDStarGMSKDemodulator();
	~CDStarGMSKDemodulator();

	TRISTATE decode(wxFloat32 val);

	void setInvert(bool set);

	void reset();

	void lock(bool on);

private:
	CFIRFilter     m_filter;
	bool           m_invert;
	unsigned int   m_pll;
	bool           m_prev;
	unsigned int   m_inc;
	bool           m_locked;
	wxFloat32      m_offset;
	wxFloat32      m_accum;
	unsigned int   m_count;
};

#endif
