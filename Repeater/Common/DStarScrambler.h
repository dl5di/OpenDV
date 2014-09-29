/*
 *	Copyright (C) 2009,2013 by Jonathan Naylor, G4KLX
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

#ifndef	DStarScrambler_H
#define	DStarScrambler_H

class CDStarScrambler {
public:
	CDStarScrambler();
	~CDStarScrambler();

	void process(bool* inOut, unsigned int length);
	void process(const bool* in, bool* out, unsigned int length);

	void process(unsigned char* inOut, unsigned int length);
	void process(const unsigned char* in, unsigned char* out, unsigned int length);

	void reset();

private:
	unsigned int m_count;
};

#endif
