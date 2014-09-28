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

#ifndef	Scrambler_H
#define	Scrambler_H

class IScrambler {
public:
	virtual void process(bool* inOut, unsigned int length) = 0;
	virtual void process(const bool* in, bool* out, unsigned int length) = 0;

	virtual void process(unsigned char* inOut, unsigned int length) = 0;
	virtual void process(const unsigned char* in, unsigned char* out, unsigned int length) = 0;

	virtual void reset() = 0;

private:
};

#endif
