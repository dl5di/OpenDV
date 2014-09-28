/*
 *   Copyright (C) 2010,2012,2013 by Jonathan Naylor G4KLX
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

#include "PollData.h"

#include "DStarDefines.h"
#include "Utils.h"

CPollData::CPollData(const wxString& data1, const wxString& data2, DIRECTION direction, const in_addr& yourAddress, unsigned int yourPort, unsigned int myPort) :
m_data1(data1),
m_data2(data2),
m_direction(direction),
m_dongle(false),
m_length(0U),
m_yourAddress(yourAddress),
m_yourPort(yourPort),
m_myPort(myPort)
{
	wxASSERT(yourPort > 0U);
}

CPollData::CPollData(const wxString& data, const in_addr& yourAddress, unsigned int yourPort, unsigned int myPort) :
m_data1(data),
m_data2(),
m_direction(DIR_OUTGOING),
m_dongle(false),
m_length(0U),
m_yourAddress(yourAddress),
m_yourPort(yourPort),
m_myPort(myPort)
{
	wxASSERT(yourPort > 0U);
}

CPollData::CPollData(const in_addr& yourAddress, unsigned int yourPort, unsigned int myPort) :
m_data1(),
m_data2(),
m_direction(DIR_OUTGOING),
m_dongle(false),
m_length(0U),
m_yourAddress(yourAddress),
m_yourPort(yourPort),
m_myPort(myPort)
{
	wxASSERT(yourPort > 0U);
}

CPollData::CPollData() :
m_data1(),
m_data2(),
m_direction(DIR_OUTGOING),
m_dongle(false),
m_length(0U),
m_yourAddress(),
m_yourPort(0U),
m_myPort(0U)
{
}

CPollData::~CPollData()
{
}

bool CPollData::setDExtraData(const unsigned char* data, unsigned int length, const in_addr& yourAddress, unsigned int yourPort, unsigned int myPort)
{
	wxASSERT(data != NULL);
	wxASSERT(length >= 9U);
	wxASSERT(yourPort > 0U);

	m_data1   = wxString((const char*)data, wxConvLocal, LONG_CALLSIGN_LENGTH);
	m_dongle  = data[LONG_CALLSIGN_LENGTH] != 0x00;

	m_length      = length;
	m_yourAddress = yourAddress;
	m_yourPort    = yourPort;
	m_myPort      = myPort;


	return true;
}

bool CPollData::setDCSData(const unsigned char* data, unsigned int length, const in_addr& yourAddress, unsigned int yourPort, unsigned int myPort)
{
	wxASSERT(data != NULL);
	wxASSERT(yourPort > 0U);

	switch (length) {
		case 17U:
			m_data1       = wxString((const char*)(data + 0U), wxConvLocal, LONG_CALLSIGN_LENGTH);
			m_data2       = wxString((const char*)(data + 9U), wxConvLocal, LONG_CALLSIGN_LENGTH);
			m_length      = length;
			m_direction   = DIR_INCOMING;
			m_yourAddress = yourAddress;
			m_yourPort    = yourPort;
			m_myPort      = myPort;
			break;

		case 22U:
			m_data1       = wxString((const char*)(data + 0U), wxConvLocal, LONG_CALLSIGN_LENGTH);
			m_data2       = wxString((const char*)(data + 9U), wxConvLocal, LONG_CALLSIGN_LENGTH - 1U);
			m_data2.Append(wxString((const char*)(data + 17U), wxConvLocal, 1U));
			m_length      = length;
			m_direction   = DIR_OUTGOING;
			m_yourAddress = yourAddress;
			m_yourPort    = yourPort;
			m_myPort      = myPort;
			break;
	}

	return true;
}

bool CPollData::setCCSData(const unsigned char* data, unsigned int length, const in_addr& yourAddress, unsigned int yourPort, unsigned int myPort)
{
	wxASSERT(data != NULL);
	wxASSERT(length >= 25U);
	wxASSERT(yourPort > 0U);

	m_data1       = wxString((const char*)(data + 0U), wxConvLocal, 25U);
	m_length      = length;
	m_yourAddress = yourAddress;
	m_yourPort    = yourPort;
	m_myPort      = myPort;

	return true;
}

bool CPollData::setDPlusData(const unsigned char* data, unsigned int length, const in_addr& yourAddress, unsigned int yourPort, unsigned int myPort)
{
	wxASSERT(yourPort > 0U);

	m_length      = length;
	m_yourAddress = yourAddress;
	m_yourPort    = yourPort;
	m_myPort      = myPort;

	return true;
}

unsigned int CPollData::getDExtraData(unsigned char *data, unsigned int length) const
{
	wxASSERT(data != NULL);
	wxASSERT(length >= 9U);

	::memset(data, ' ', LONG_CALLSIGN_LENGTH);

	for (unsigned int i = 0U; i < m_data1.Len() && i < LONG_CALLSIGN_LENGTH; i++)
		data[i] = m_data1.GetChar(i);

	data[LONG_CALLSIGN_LENGTH] = 0x00;

	return 9U;
}

unsigned int CPollData::getDCSData(unsigned char *data, unsigned int length) const
{
	wxASSERT(data != NULL);
	wxASSERT(length >= 22U);

	if (m_direction == DIR_OUTGOING) {
		::memset(data, ' ', 17U);

		for (unsigned int i = 0U; i < m_data1.Len() && i < LONG_CALLSIGN_LENGTH; i++)
			data[i + 0U] = m_data1.GetChar(i);

		data[8U] = 0x00U;

		for (unsigned int i = 0U; i < m_data2.Len() && i < LONG_CALLSIGN_LENGTH; i++)
			data[i + 9U] = m_data2.GetChar(i);

		return 17U;
	} else {
		::memset(data, ' ', 22U);

		for (unsigned int i = 0U; i < m_data1.Len() && i < LONG_CALLSIGN_LENGTH; i++)
			data[i + 0U] = m_data1.GetChar(i);

		for (unsigned int i = 0U; i < m_data2.Len() && i < (LONG_CALLSIGN_LENGTH - 1U); i++)
			data[i + 9U] = m_data2.GetChar(i);

		if (m_data2.Len() >= LONG_CALLSIGN_LENGTH)
			data[17U] = m_data2.GetChar(LONG_CALLSIGN_LENGTH - 1U);

		data[18U] = 0x0AU;
		data[19U] = 0x00U;

		return 22U;
	}
}

unsigned int CPollData::getCCSData(unsigned char *data, unsigned int length) const
{
	wxASSERT(data != NULL);
	wxASSERT(length >= 25U);

	::memset(data, ' ', 25U);

	for (unsigned int i = 0U; i < m_data1.Len() && i < LONG_CALLSIGN_LENGTH; i++)
		data[i + 0U] = m_data1.GetChar(i);

	if (!m_data2.IsEmpty()) {
		for (unsigned int i = 0U; i < m_data2.Len() && i < LONG_CALLSIGN_LENGTH; i++)
			data[i + 8U] = m_data2.GetChar(i);
	}

	return 25U;
}

unsigned int CPollData::getDPlusData(unsigned char *data, unsigned int length) const
{
	wxASSERT(data != NULL);
	wxASSERT(length >= 3U);

	data[0U] = 0x03;
	data[1U] = 0x60;
	data[2U] = 0x00;

	return 3U;
}

wxString CPollData::getData1() const
{
	return m_data1;
}

void CPollData::setData1(const wxString& data)
{
	m_data1 = data;
}

wxString CPollData::getData2() const
{
	return m_data2;
}

void CPollData::setData2(const wxString& data)
{
	m_data2 = data;
}

bool CPollData::isDongle() const
{
	return m_dongle;
}

in_addr CPollData::getYourAddress() const
{
	return m_yourAddress;
}

unsigned int CPollData::getYourPort() const
{
	return m_yourPort;
}

unsigned int CPollData::getMyPort() const
{
	return m_myPort;
}

DIRECTION CPollData::getDirection() const
{
	return m_direction;
}

unsigned int CPollData::getLength() const
{
	return m_length;
}
