/*
 *	Copyright (C) 2009,2010 by Jonathan Naylor, G4KLX
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

#include "Demodulator.h"
#include "FIRFilter.h"

class CDStarGMSKDemodulator : public IDemodulator {
public:
	CDStarGMSKDemodulator();
	virtual ~CDStarGMSKDemodulator();

	virtual DEMOD_STATE decode(wxFloat32 val);

	virtual void setInvert(bool set);

	virtual void reset();

private:
	CFIRFilter   m_filter;
	bool         m_invert;
	unsigned int m_pll;
	bool         m_prev;
};

#endif
