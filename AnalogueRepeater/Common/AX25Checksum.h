/*
 *	Copyright (C) 2009,2011,2013 by Jonathan Naylor, G4KLX
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

#ifndef	AX25Checksum_H
#define	AX25Checksum_H

#include <wx/wx.h>

class CAX25Checksum {
public:
	CAX25Checksum();
	~CAX25Checksum();

	bool check(const unsigned char* data, unsigned int length);

	unsigned int calculate(unsigned char* data, unsigned int length);

private:
};

#endif
