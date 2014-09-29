/*
 *	Copyright (C) 2012 by Jonathan Naylor, G4KLX
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

#ifndef	BeaconCallback_H
#define	BeaconCallback_H

class IBeaconCallback {
public:
	virtual void transmitBeaconHeader() = 0;

	virtual void transmitBeaconData(const unsigned char* data, unsigned int length, bool end) = 0;

private:
};

#endif
