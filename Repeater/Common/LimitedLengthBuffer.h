/*
 *   Copyright (C) 2009 by Jonathan Naylor G4KLX
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

#ifndef LimitedLengthBuffer_H
#define	LimitedLengthBuffer_H

class CLimitedLengthBuffer {
public:
	CLimitedLengthBuffer(unsigned int maxLength);
	virtual ~CLimitedLengthBuffer();

	virtual void clear();
	virtual void setMaxLength(unsigned int maxLength);

	virtual void add(bool bit);
	virtual unsigned int getLength() const;
	virtual unsigned int getData(bool* buffer, unsigned int length) const;

private:
	bool*        m_buffer;
	unsigned int m_maxLength;
	unsigned int m_currLength;
	unsigned int m_allocLength;
};

#endif
