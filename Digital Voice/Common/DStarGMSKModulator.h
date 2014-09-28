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

#ifndef	DStarGMSKModulator_H
#define	DStarGMSKModulator_H

#include "Modulator.h"
#include "FIRFilter.h"

class CDStarGMSKModulator : public IModulator {
public:
	CDStarGMSKModulator();
	virtual ~CDStarGMSKModulator();

	virtual unsigned int code(bool bit, wxFloat32* buffer, unsigned int length);

	virtual void setInvert(bool set);

private:
	CFIRFilter m_filter;
	bool       m_invert;
};

#endif
