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

#include "NetworkController.h"

CNetworkController::CNetworkController(const wxString& device, SERIALPIN txPin, SERIALPIN rxPin) :
m_serial(NULL),
m_txPin(txPin),
m_rxPin(rxPin)
{
	if (!device.IsEmpty())
		m_serial = new CSerialLineController(device);
}

CNetworkController::~CNetworkController()
{
	delete m_serial;
}

bool CNetworkController::open()
{
	if (m_serial == NULL)
		return true;

	return m_serial->open();
}

bool CNetworkController::getSquelch() const
{
	if (m_serial == NULL)
		return false;

	switch (m_rxPin) {
		case SERIAL_CD:
			return m_serial->getCD();
		case SERIAL_CTS:
			return m_serial->getCTS();
		case SERIAL_DSR:
			return m_serial->getDSR();
		default:
			return false;
	}
}

void CNetworkController::setTransmit(bool value)
{
	if (m_serial == NULL)
		return;

	switch (m_txPin) {
		case SERIAL_DTR:
			m_serial->setDTR(value);
			break;
		case SERIAL_RTS:
			m_serial->setRTS(value);
			break;
		default:
			break;
	}
}

void CNetworkController::close()
{
	if (m_serial != NULL)
		m_serial->close();
}

/*
 *	The ASCII protocol is inherited from the original design of iLINK. It
 *	is a series of one-character commands sent from the computer to the
 *	interface, each of which receives a one-character response.
 *
 *	2400 bps, 8 bits, no parity, one stop bit, no handshaking.
 *
 *	T engage PTT (response: K)
 *	R disengage PTT (response: K)
 *	C query DTMF decoder status (response: space if no tone, else ASCII digit)
 *
 *	In order to decode DTMF commands properly, EchoLink sends the "C"
 *	command to the interface approximately 20 times per second.
 */
