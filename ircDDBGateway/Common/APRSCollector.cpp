/*
 *   Copyright (C) 2010,2012,2013,2014 by Jonathan Naylor G4KLX
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

#include "APRSCollector.h"
#include "DStarDefines.h"
#include "Utils.h"

const unsigned int APRS_CSUM_LENGTH = 4U;
const unsigned int APRS_DATA_LENGTH = 300U;
const unsigned int SLOW_DATA_BLOCK_LENGTH = 6U;

CAPRSCollector::CAPRSCollector() :
m_state(AS_NONE),
m_ggaData(NULL),
m_ggaLength(0U),
m_ggaValid(false),
m_rmcData(NULL),
m_rmcLength(0U),
m_rmcValid(false),
m_crcData(NULL),
m_crcLength(0U),
m_crcValid(false),
m_txtData(NULL),
m_txtLength(0U),
m_txtValid(false),
m_buffer(NULL),
m_slowData(SS_FIRST)
{
	m_ggaData = new unsigned char[APRS_DATA_LENGTH];
	m_rmcData = new unsigned char[APRS_DATA_LENGTH];
	m_crcData = new unsigned char[APRS_DATA_LENGTH];
	m_txtData = new unsigned char[APRS_DATA_LENGTH];
	m_buffer  = new unsigned char[SLOW_DATA_BLOCK_LENGTH];
}

CAPRSCollector::~CAPRSCollector()
{
	delete[] m_ggaData;
	delete[] m_rmcData;
	delete[] m_crcData;
	delete[] m_txtData;
	delete[] m_buffer;
}

bool CAPRSCollector::writeData(const unsigned char* data)
{
	wxASSERT(data != NULL);

	switch (m_slowData) {
		case SS_FIRST:
			m_buffer[0U] = data[0U] ^ SCRAMBLER_BYTE1;
			m_buffer[1U] = data[1U] ^ SCRAMBLER_BYTE2;
			m_buffer[2U] = data[2U] ^ SCRAMBLER_BYTE3;
			m_slowData = SS_SECOND;
			return false;

		case SS_SECOND:
			m_buffer[3U] = data[0U] ^ SCRAMBLER_BYTE1;
			m_buffer[4U] = data[1U] ^ SCRAMBLER_BYTE2;
			m_buffer[5U] = data[2U] ^ SCRAMBLER_BYTE3;
			m_slowData = SS_FIRST;
			break;
	}

	// Is it GPS data, or are we collecting data already?
	if ((m_buffer[0U] & SLOW_DATA_TYPE_MASK) != SLOW_DATA_TYPE_GPS)
		return false;

	return addData(m_buffer + 1U);
}

void CAPRSCollector::reset()
{
	m_state     = AS_NONE;
	m_ggaLength = 0U;
	m_ggaValid  = false;
	m_rmcLength = 0U;
	m_rmcValid  = false;
	m_crcLength = 0U;
	m_crcValid  = false;
	m_txtLength = 0U;
	m_txtValid  = false;
	m_slowData  = SS_FIRST;
}

void CAPRSCollector::sync()
{
	m_slowData = SS_FIRST;
}

bool CAPRSCollector::addData(const unsigned char* data)
{
	wxASSERT(data != NULL);

	if (::memcmp(data, "$GPGG", 5U) == 0) {
		m_state     = AS_GGA;
		m_ggaLength = 0U;
		m_ggaValid  = false;
		m_rmcLength = 0U;
		m_rmcValid  = false;
		m_txtLength = 0U;
		m_txtValid  = false;
		addGGAData(data);
		return false;
	} else if (::memcmp(data, "$GPRM", 5U) == 0) {
		m_state     = AS_RMC;
		m_rmcLength = 0U;
		m_rmcValid  = false;
		m_txtLength = 0U;
		m_txtValid  = false;
		addRMCData(data);
		return false;
	} else if (::memcmp(data, "$$CRC", 5U) == 0) {
		m_state     = AS_CRC;
		m_crcLength = 0U;
		m_crcValid  = false;
		return addCRCData(data);
	} else if (m_state == AS_RMC && m_rmcLength == 0U) {
		m_state     = AS_TXT;
		m_txtLength = 0U;
		m_txtValid  = false;
		addTXTData(data);
		return false;
	} else if (m_state == AS_GGA) {
		addGGAData(data);
		return false;
	} else if (m_state == AS_RMC) {
		addRMCData(data);
		return false;
	} else if (m_state == AS_CRC) {
		return addCRCData(data);
	} else if (m_state == AS_TXT) {
		return addTXTData(data);
	}

	return false;
}

void CAPRSCollector::addGGAData(const unsigned char* data)
{
	for (unsigned int i = 0U; i < 5U; i++) {
		unsigned char c = data[i];

		m_ggaData[m_ggaLength] = c & 0x7FU;
		m_ggaLength++;

		if (m_ggaLength >= APRS_DATA_LENGTH) {
			// CUtils::dump(wxT("Missed end of $GPGGA data"), m_ggaData, m_ggaLength);
			m_ggaLength = 0U;
			m_ggaValid  = false;
			m_state     = AS_NONE;
			return;
		}

		if (c == 0x0AU) {
			bool ret = checkXOR(m_ggaData + 1U, m_ggaLength - 1U);
			if (ret) {
				// CUtils::dump(wxT("$GPGGA Valid"), m_ggaData, m_ggaLength);
				m_ggaValid  = true;
				m_state     = AS_RMC;
				return;
			} else {
				// CUtils::dump(wxT("$GPGGA Bad checksum"), m_ggaData, m_ggaLength);
				m_ggaLength = 0U;
				m_ggaValid  = false;
				m_state     = AS_RMC;
				return;
			}
		}
	}
}

void CAPRSCollector::addRMCData(const unsigned char* data)
{
	for (unsigned int i = 0U; i < 5U; i++) {
		unsigned char c = data[i];

		m_rmcData[m_rmcLength] = c & 0x7FU;
		m_rmcLength++;

		if (m_rmcLength >= APRS_DATA_LENGTH) {
			// CUtils::dump(wxT("Missed end of $GPRMC data"), m_rmcData, m_rmcLength);
			m_rmcLength = 0U;
			m_rmcValid  = false;
			m_state     = AS_NONE;
			return;
		}

		if (c == 0x0AU) {
			bool ret = checkXOR(m_rmcData + 1U, m_rmcLength - 1U);
			if (ret) {
				// CUtils::dump(wxT("$GPRMC Valid"), m_rmcData, m_rmcLength);
				m_rmcValid = true;
				m_state    = AS_TXT;
				return;
			} else {
				// CUtils::dump(wxT("$GPRMC Bad checksum"), m_rmcData, m_rmcLength);
				m_rmcLength = 0U;
				m_rmcValid  = false;
				m_state     = AS_TXT;
				return;
			}
		}
	}
}

bool CAPRSCollector::addCRCData(const unsigned char* data)
{
	for (unsigned int i = 0U; i < 5U; i++) {
		unsigned char c = data[i];

		// m_crcData[m_crcLength] = c & 0x7FU;		// XXX
		m_crcData[m_crcLength] = c;
		m_crcLength++;

		if (m_crcLength >= APRS_DATA_LENGTH) {
			// CUtils::dump(wxT("Missed end of $$CRC data"), m_crcData, m_crcLength);
			m_state     = AS_NONE;
			m_crcLength = 0U;
			m_crcValid  = false;
			return false;
		}

		if (c == 0x0DU) {
			bool ret = checkCRC(m_crcData, m_crcLength);
			if (ret) {
				// CUtils::dump(wxT("$$CRC Valid"), m_crcData, m_crcLength);
				m_state    = AS_NONE;
				m_crcValid = true;
				return true;
			} else {
				// CUtils::dump(wxT("$$CRC Bad checksum"), m_crcData, m_crcLength);
				m_state     = AS_NONE;
				m_crcLength = 0U;
				m_crcValid  = false;
				return false;
			}
		}
	}

	return false;
}

bool CAPRSCollector::addTXTData(const unsigned char* data)
{
	for (unsigned int i = 0U; i < 5U; i++) {
		unsigned char c = data[i];

		m_txtData[m_txtLength] = c & 0x7FU;
		m_txtLength++;

		if (m_txtLength >= APRS_DATA_LENGTH) {
			// CUtils::dump(wxT("Missed end of TEXT data"), m_txtData, m_txtLength);
			m_state     = AS_NONE;
			m_txtLength = 0U;
			m_txtValid  = false;
			return false;
		}

		if (c == 0x0AU) {
			bool ret = checkXOR(m_txtData, m_txtLength);
			if (ret) {
				// CUtils::dump(wxT("TEXT Valid"), m_txtData, m_txtLength);
				m_state    = AS_NONE;
				m_txtValid = true;
				return true;
			} else {
				// CUtils::dump(wxT("TEXT Bad checksum"), m_txtData, m_txtLength);
				m_state     = AS_NONE;
				m_txtLength = 0U;
				m_txtValid  = false;
				return false;
			}
		}
	}

	return false;
}

unsigned int CAPRSCollector::getData(unsigned char* data, unsigned int length)
{
	wxASSERT(data != NULL);

	// Have we got GPS-A data?
	if (m_crcValid) {
		unsigned int len = m_crcLength - 10U;
		if (len > length)
			len = length;

		::memcpy(data, m_crcData + 10U, len);

		m_crcLength = 0U;
		m_crcValid  = false;

		return len;
	}

	// Have we got GGA and text data?
	if (m_ggaValid && m_txtValid) {
		unsigned int len = convertNMEA1(data, length);

		m_ggaLength = 0U;
		m_rmcLength = 0U;
		m_txtLength = 0U;
		m_ggaValid  = false;
		m_rmcValid  = false;
		m_txtValid  = false;

		return len;
	}

	// Have we got RMC and text data?
	if (m_rmcValid && m_txtValid) {
		unsigned int len = convertNMEA2(data, length);

		m_ggaLength = 0U;
		m_rmcLength = 0U;
		m_txtLength = 0U;
		m_ggaValid  = false;
		m_rmcValid  = false;
		m_txtValid  = false;

		return len;
	}

	return 0U;
}

bool CAPRSCollector::checkXOR(const unsigned char* data, unsigned int length) const
{
	unsigned int posStar = 0U;
	for (unsigned int i = length - 1U; i > 0U; i--) {
		if (data[i] == '*') {
			posStar = i;
			break;
		}
	}

	if (posStar == 0U)
		return false;

	unsigned char csum = calcXOR(data, posStar);

	char buffer[10U];
	::sprintf(buffer, "%02X", csum);

	return ::memcmp(buffer, data + posStar + 1U, 2U) == 0;
}

unsigned char CAPRSCollector::calcXOR(const unsigned char* buffer, unsigned int length) const
{ 
	wxASSERT(buffer != NULL);
	wxASSERT(length > 0U);

	unsigned char res = 0U;

	for (unsigned int i = 0U; i < length; i++)
		res ^= buffer[i];

	return res;
}

bool CAPRSCollector::checkCRC(const unsigned char* data, unsigned int length) const
{
	unsigned int csum = calcCRC(data + 10U, length - 10U);

	char buffer[10U];
	::sprintf(buffer, "%04X", csum);

	return ::memcmp(buffer, data + 5U, APRS_CSUM_LENGTH) == 0;
}

unsigned int CAPRSCollector::calcCRC(const unsigned char* buffer, unsigned int length) const
{ 
	wxASSERT(buffer != NULL);
	wxASSERT(length > 0U);

	unsigned int icomcrc = 0xFFFFU;

	for (unsigned int j = 0U; j < length; j++) {
		unsigned char ch = buffer[j];

		for (unsigned int i = 0U; i < 8U; i++) {
			bool xorflag = (((icomcrc ^ ch) & 0x01U) == 0x01U);

			icomcrc >>= 1;

			if (xorflag)
				icomcrc ^= 0x8408U;

			ch >>= 1;
		} 
	}

	return ~icomcrc & 0xFFFFU;
}

unsigned int CAPRSCollector::convertNMEA1(unsigned char* data, unsigned int length)
{
	// Parse the $GPGGA string into tokens
	char* pGGA[20U];
	::memset(pGGA, 0x00U, 20U * sizeof(char*));
	unsigned int nGGA = 0U;

	char* str = (char*)m_ggaData;
	for (;;) {
		char* p = mystrsep(&str, ",\r\n");

		pGGA[nGGA++] = p;
		if (p == NULL)
			break;
	}

	// Is there any position data?
	if (pGGA[2U] == NULL || pGGA[3U] == NULL || pGGA[4U] == NULL || pGGA[5U] == NULL || ::strlen(pGGA[2U]) == 0U || ::strlen(pGGA[3U]) == 0U || ::strlen(pGGA[4U]) == 0 || ::strlen(pGGA[5U]) == 0)
		return 0U;

	// Is it a valid GPS fix?
	if (::strcmp(pGGA[6U], "0") == 0)
		return 0U;

	char callsign[10U];
	::memset(callsign, ' ', 10U);
	::strncpy(callsign, (char*)m_txtData, 7U);

	// This can't fail!
	char* p = ::strchr(callsign, ' ');

	if (m_txtData[6U] == ' ' && m_txtData[7U] != ' ') {
		*p++ = '-';
		*p++ = m_txtData[7U];
	} else if (m_txtData[6U] != ' ' && m_txtData[7U] != ' ') {
		*p++ = m_txtData[7U];
	}

	*p = '\0';

	char symbol, overlay;
	getSymbol(m_txtData + 9U, symbol, overlay);

	::sprintf((char*)data, "%s>APDPRS,DSTAR*:!%.7s%s%c%.8s%s%c", callsign, pGGA[2U], pGGA[3U], overlay, pGGA[4U], pGGA[5U], symbol);

	// Get the bearing and speed from the RMC data
	if (m_rmcValid) {
		// Parse the $GPRMC string into tokens
		char* pRMC[20U];
		::memset(pRMC, 0x00U, 20U * sizeof(char*));
		unsigned int nRMC = 0U;

		char* str = (char*)m_rmcData;
		for (;;) {
			char* p = mystrsep(&str, ",\r\n");

			pRMC[nRMC++] = p;
			if (p == NULL)
				break;
		}

		// Check that we have a bearing and speed
		if (pRMC[7U] != NULL && pRMC[8U] != NULL && ::strlen(pRMC[7U]) > 0U && ::strlen(pRMC[8U]) > 0U) {
			int bearing = ::atoi(pRMC[8U]);
			int speed   = ::atoi(pRMC[7U]);

			::sprintf((char*)data + ::strlen((char*)data), "%03d/%03d", bearing, speed);
		}
	}

	::strcat((char*)data, " ");

	// Insert the message text
	unsigned int j = ::strlen((char*)data);
	for (unsigned int i = 13U; i < 29U; i++) {
		unsigned char c = m_txtData[i];

		if (c == '*') {
			data[j] = '\0';
			break;
		}

		data[j++] = c;
	}

	if (pGGA[9U] != NULL && ::strlen(pGGA[9U]) > 0U) {
		// Convert altitude from metres to feet
		int altitude = ::atoi(pGGA[9U]);
		::sprintf((char*)data + ::strlen((char*)data), "/A=%06.0f", float(altitude) * 3.28F);
	}

	return ::strlen((char*)data);
}

unsigned int CAPRSCollector::convertNMEA2(unsigned char* data, unsigned int length)
{
	// Parse the $GPRMC string into tokens
	char* pRMC[20U];
	::memset(pRMC, 0x00U, 20U * sizeof(char*));
	unsigned int nRMC = 0U;

	char* str = (char*)m_rmcData;
	for (;;) {
		char* p = mystrsep(&str, ",\r\n");

		pRMC[nRMC++] = p;
		if (p == NULL)
			break;
	}

	// Is there any position data?
	if (pRMC[3U] == NULL || pRMC[4U] == NULL || pRMC[5U] == NULL || pRMC[6U] == NULL || ::strlen(pRMC[3U]) == 0U || ::strlen(pRMC[4U]) == 0U || ::strlen(pRMC[5U]) == 0 || ::strlen(pRMC[6U]) == 0)
		return 0U;

	// Is it a valid GPS fix?
	if (::strcmp(pRMC[2U], "A") != 0)
		return 0U;

	char callsign[10U];
	::memset(callsign, ' ', 10U);
	::strncpy(callsign, (char*)m_txtData, 7U);

	// This can't fail!
	char* p = ::strchr(callsign, ' ');

	if (m_txtData[6U] == ' ' && m_txtData[7U] != ' ') {
		*p++ = '-';
		*p++ = m_txtData[7U];
	} else if (m_txtData[6U] != ' ' && m_txtData[7U] != ' ') {
		*p++ = m_txtData[7U];
	}

	*p = '\0';

	char symbol, overlay;
	getSymbol(m_txtData + 9U, symbol, overlay);

	::sprintf((char*)data, "%s>APDPRS,DSTAR*:!%.7s%s%c%.8s%s%c", callsign, pRMC[3U], pRMC[4U], overlay, pRMC[5U], pRMC[6U], symbol);

	if (pRMC[7U] != NULL && pRMC[8U] != NULL && ::strlen(pRMC[7U]) > 0U && ::strlen(pRMC[8U]) > 0U) {
		int bearing = ::atoi(pRMC[8U]);
		int speed   = ::atoi(pRMC[7U]);

		::sprintf((char*)data + ::strlen((char*)data), "%03d/%03d", bearing, speed);
	}

	if (m_txtData[13U] != '*')
		::strcat((char*)data, " ");

	// Insert the message text
	unsigned int j = ::strlen((char*)data);
	for (unsigned int i = 13U; i < 29U; i++) {
		unsigned char c = m_txtData[i];

		if (c == '*') {
			data[j] = '\0';
			break;
		}

		data[j++] = c;
	}

	return ::strlen((char*)data);
}

// Function taken from DPRSIntf.java from Pete Loveall AE5PL
void CAPRSCollector::getSymbol(const unsigned char* data, char& symbol, char& overlay)
{
	symbol = '.';

	if (data[3U] == ' ') {
		int offset = -1;

		switch (data[0U]) {
			case 'B':
			case 'O':
				offset = -33;
				break;
			case 'P':
			case 'A':
				offset = 0;
				break;
			case 'M':
			case 'N':
				offset = -24;
				break;
			case 'H':
			case 'D':
				offset = 8;
				break;
			case 'L':
			case 'S':
				offset = 32;
				break;
			case 'J':
			case 'Q':
				offset = 74;
				break;
			default:
				break;
		}

		if (offset != -1 && ::isalnum(data[1U])) {
			bool altIcons = false;

			// x is valid, lets get y
			switch (data[0U]) {
				case 'O':
				case 'A':
				case 'N':
				case 'D':
				case 'S':
				case 'Q':
					altIcons = true;
					break;
			}

			symbol = char(data[1U] + offset);

			overlay = '/';

			if (altIcons) {
				if (data[2] == ' ')
					overlay = '\\';
				else if (::isalnum(data[2]))
					overlay = data[2U];
				else
					overlay = 0;
			}
		}
	}
}

// Source found at <http://unixpapa.com/incnote/string.html>
char* CAPRSCollector::mystrsep(char** sp, const char* sep) const
{
	if (sp == NULL || *sp == NULL || **sp == '\0')
		return NULL;

	char* s = *sp;
	char* p = s + ::strcspn(s, sep);

	if (*p != '\0')
		*p++ = '\0';

	*sp = p;

	return s;
}
