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

#ifndef	RadioHeaderDecoder_H
#define	RadioHeaderDecoder_H

#include "HeaderData.h"

class CRadioHeaderDecoder {
public:
	CRadioHeaderDecoder(const bool* data, unsigned int length);
	~CRadioHeaderDecoder();

	unsigned int getHeaderBytes(unsigned char* buffer, unsigned int length);

	CHeaderData* getHeaderData();
	unsigned int getBER() const;

private:
	CHeaderData*   m_header;
	unsigned char* m_headerBytes;
	unsigned int   m_ber;

	unsigned char bitsToByte(const bool* bits) const;
};

#endif
