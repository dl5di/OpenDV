/*
 *   Copyright (C) 2011,2014 by Jonathan Naylor G4KLX
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef	OutputQueue_H
#define	OutputQueue_H

#include "RingBuffer.h"
#include "HeaderData.h"

#include <wx/wx.h>

class COutputQueue {
public:
	COutputQueue(unsigned int space, unsigned int threshold);
	~COutputQueue();

	void setHeader(CHeaderData* header);
	CHeaderData* getHeader();

	unsigned int getData(unsigned char* data, unsigned int length, bool& end);
	unsigned int addData(const unsigned char* data, unsigned int length, bool end);

	bool headerReady() const;
	bool dataReady() const;

	bool isEmpty();

	void reset();

	void setThreshold(unsigned int threshold);

private:
	CRingBuffer<unsigned char> m_data;
	unsigned int               m_threshold;
	CHeaderData*               m_header;
	unsigned int               m_count;
};

#endif
