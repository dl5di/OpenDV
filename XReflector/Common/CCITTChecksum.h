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

#ifndef	CCITTChecksum_H
#define	CCITTChecksum_H

#include "Checksum.h"

#include <wx/wx.h>

class CCCITTChecksum : public IChecksum {
public:
	CCCITTChecksum();
	virtual ~CCCITTChecksum();

	virtual void update(const unsigned char* data, unsigned int length);
	virtual void update(const bool* data);

	virtual void result(unsigned char* data);
	virtual void result(bool* data);

	virtual bool check(const unsigned char* data);
	virtual bool check(const bool* data);

	virtual void reset();

private:
	wxUint16 m_crc;
};

#endif
