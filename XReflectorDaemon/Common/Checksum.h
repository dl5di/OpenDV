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

#ifndef	Checksum_H
#define	Checksum_H

#include <wx/wx.h>

class IChecksum {
public:
	virtual void update(const unsigned char* data, unsigned int length) = 0;
	virtual void update(const bool* data) = 0;

	virtual void result(unsigned char* data) = 0;
	virtual void result(bool* data) = 0;

	virtual bool check(const unsigned char* data) = 0;
	virtual bool check(const bool* data) = 0;

	virtual void reset() = 0;

private:
};

#endif
