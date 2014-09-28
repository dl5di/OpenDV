/*
 *   Copyright (C) 2006-2009,2013 by Jonathan Naylor G4KLX
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

#ifndef RingBuffer_H
#define RingBuffer_H

#include <wx/wx.h>

template<class T> class CRingBuffer {
public:
	CRingBuffer(unsigned int length) :
	m_length(length),
	m_buffer(NULL),
	m_iPtr(0U),
	m_oPtr(0U),
	m_mutex()
	{
		wxASSERT(length > 0U);

		m_buffer = new T[length];

		::memset(m_buffer, 0x00, length * sizeof(T));
	}

	~CRingBuffer()
	{
		delete[] m_buffer;
	}

	void addData(const T data)
	{
		wxMutexLocker locker(m_mutex);

		m_buffer[m_iPtr++] = data;

		if (m_iPtr == m_length)
			m_iPtr = 0U;
	}

	T getData()
	{
		wxMutexLocker locker(m_mutex);

		if (m_iPtr == m_oPtr)
			return 0;

		T data = m_buffer[m_oPtr++];

		if (m_oPtr == m_length)
			m_oPtr = 0U;

		return data;
	}

	void clear()
	{
		wxMutexLocker locker(m_mutex);

		m_iPtr  = 0U;
		m_oPtr  = 0U;

		::memset(m_buffer, 0x00, m_length * sizeof(T));
	}

	bool isEmpty()
	{
		wxMutexLocker locker(m_mutex);

		return m_iPtr == m_oPtr;
	}

	T peek()
	{
		wxMutexLocker locker(m_mutex);

		if (m_iPtr == m_oPtr)
			return 0;

		return m_buffer[m_oPtr];
	}

private:
	unsigned int m_length;
	T*           m_buffer;
	unsigned int m_iPtr;
	unsigned int m_oPtr;
	wxMutex      m_mutex;
};

#endif
