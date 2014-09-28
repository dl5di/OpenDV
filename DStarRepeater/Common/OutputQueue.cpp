/*
 *   Copyright (C) 2011,2013,2014 by Jonathan Naylor G4KLX
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

#include "OutputQueue.h"

COutputQueue::COutputQueue(unsigned int space, unsigned int threshold) :
m_data(space),
m_threshold(threshold),
m_header(NULL),
m_count(0U)
{
	wxASSERT(space > 0U);
	wxASSERT(threshold > 0U);
}

COutputQueue::~COutputQueue()
{
	delete m_header;
}

void COutputQueue::setHeader(CHeaderData* header)
{
	wxASSERT(header != NULL);

	delete m_header;

	m_header = header;
}

CHeaderData* COutputQueue::getHeader()
{
	CHeaderData* header = m_header;

	m_header = NULL;

	return header;
}

unsigned int COutputQueue::getData(unsigned char *data, unsigned int length, bool& end)
{
	wxASSERT(data != NULL);

	if (m_data.isEmpty()) {
		end = false;
		return 0U;
	}

	unsigned char hdr[2U];
	m_data.getData(hdr, 2U);

	end = hdr[0U] == 1U;

	if (length < hdr[1U]) {
		wxLogWarning(wxT("Output buffer is too short, %u < %u"), length, hdr[1U]);

		unsigned int len = m_data.getData(data, length);

		// Purge the excess data
		unsigned int n = hdr[1U] - length;
		unsigned char c;
		for (unsigned int i = 0U; i < n; i++)
			m_data.getData(&c, 1U);

		return len;
	} else {
		return m_data.getData(data, hdr[1U]);
	}
}

unsigned int COutputQueue::addData(const unsigned char *data, unsigned int length, bool end)
{
	wxASSERT(data != NULL);

	bool ret = m_data.hasSpace(length + 2U);
	if (!ret) {
		// XXX wxLogWarning(wxT("Not enough space in the output queue"));
		return 0U;
	}

	unsigned char hdr[2U];
	hdr[0U] = end ? 1U : 0U;
	hdr[1U] = length;
	m_data.addData(hdr, 2U);

	if (end)
		m_count = m_threshold;
	else
		m_count++;

	return m_data.addData(data, length);
}

bool COutputQueue::headerReady() const
{
	return m_header != NULL && m_count >= m_threshold;
}

bool COutputQueue::dataReady() const
{
	return m_header == NULL && m_count >= m_threshold;
}

bool COutputQueue::isEmpty()
{
	return m_header == NULL && m_data.isEmpty();
}

void COutputQueue::reset()
{
	m_data.clear();

	delete m_header;
	m_header = NULL;

	m_count = 0U;
}

void COutputQueue::setThreshold(unsigned int threshold)
{
	wxASSERT(threshold > 0U);

	m_threshold = threshold;
}
