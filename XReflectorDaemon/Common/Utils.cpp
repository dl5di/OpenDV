/*
 *	Copyright (C) 2009,2013 Jonathan Naylor, G4KLX
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

#include "Utils.h"

void CUtils::dump(const wxChar* title, const bool* data, unsigned int length)
{
	wxASSERT(title != NULL);
	wxASSERT(data != NULL);

	wxLogMessage(wxT("%s"), title);

	unsigned int offset = 0U;

	while (offset < length) {
		wxString output;

		unsigned char buffer[16];
		unsigned int bytes = 0U;
		for (unsigned int bits = 0U; bits < 128U && (offset + bits) < length; bits += 8U)
			buffer[bytes++] = bitsToByte(data + offset + bits);

		for (unsigned i = 0U; i < bytes; i++) {
			wxString temp;
			temp.Printf(wxT("%02X "), buffer[i]);
			output += temp;
		}

		for (unsigned int i = bytes; i < 16U; i++)
			output += wxT("   ");

		output += wxT("   *");

		for (unsigned i = 0U; i < bytes; i++) {
			unsigned char c = buffer[i];

			if (::isprint(c))
				output += wxChar(c);
			else
				output += wxT('.');
		}

		output += wxT('*');

		wxLogMessage(wxT("%04X:  %s"), offset / 8U, output.c_str());

		offset += 128U;
	}
}

void CUtils::dumpRev(const wxChar* title, const bool* data, unsigned int length)
{
	wxASSERT(title != NULL);
	wxASSERT(data != NULL);

	wxLogMessage(wxT("%s"), title);

	unsigned int offset = 0U;

	while (offset < length) {
		wxString output;

		unsigned char buffer[16];
		unsigned int bytes = 0U;
		for (unsigned int bits = 0U; bits < 128U && (offset + bits) < length; bits += 8U)
			buffer[bytes++] = bitsToByteRev(data + offset + bits);

		for (unsigned i = 0U; i < bytes; i++) {
			wxString temp;
			temp.Printf(wxT("%02X "), buffer[i]);
			output += temp;
		}

		for (unsigned int i = bytes; i < 16U; i++)
			output += wxT("   ");

		output += wxT("   *");

		for (unsigned i = 0U; i < bytes; i++) {
			unsigned char c = buffer[i];

			if (::isprint(c))
				output += wxChar(c);
			else
				output += wxT('.');
		}

		output += wxT('*');

		wxLogMessage(wxT("%04X:  %s"), offset / 8U, output.c_str());

		offset += 128U;
	}
}

void CUtils::dump(const wxChar* title, const unsigned char* data, unsigned int length)
{
	wxASSERT(title != NULL);
	wxASSERT(data != NULL);

	wxLogMessage(wxT("%s"), title);

	unsigned int offset = 0U;

	while (length > 0U) {
		wxString output;

		unsigned int bytes = (length > 16U) ? 16U : length;

		for (unsigned i = 0U; i < bytes; i++) {
			wxString temp;
			temp.Printf(wxT("%02X "), data[offset + i]);
			output += temp;
		}

		for (unsigned int i = bytes; i < 16U; i++)
			output += wxT("   ");

		output += wxT("   *");

		for (unsigned i = 0U; i < bytes; i++) {
			unsigned char c = data[offset + i];

			if (::isprint(c))
				output += wxChar(c);
			else
				output += wxT('.');
		}

		output += wxT('*');

		wxLogMessage(wxT("%04X:  %s"), offset, output.c_str());

		offset += 16U;

		if (length >= 16U)
			length -= 16U;
		else
			length = 0U;
	}
}

unsigned char CUtils::bitsToByte(const bool* bits)
{
	wxASSERT(bits != NULL);

	unsigned char val = 0x00;

	for (unsigned int i = 0U; i < 8U; i++) {
		val <<= 1;

		if (bits[i])
			val |= 0x01;
	}

	return val;
}

unsigned char CUtils::bitsToByteRev(const bool* bits)
{
	wxASSERT(bits != NULL);

	unsigned char val = 0x00;

	for (unsigned int i = 0U; i < 8U; i++) {
		val >>= 1;

		if (bits[i])
			val |= 0x80;
	}

	return val;
}

void CUtils::byteToBits(unsigned char byte, bool* data)
{
	wxASSERT(data != NULL);

	unsigned char mask = 0x80U;
	for (unsigned int i = 0U; i < 8U; i++, mask >>= 1)
		data[i] = byte & mask ? true : false;
}

void CUtils::byteToBitsRev(unsigned char byte, bool* data)
{
	wxASSERT(data != NULL);

	unsigned char mask = 0x01U;
	for (unsigned int i = 0U; i < 8U; i++, mask <<= 1)
		data[i] = byte & mask ? true : false;
}

wxString CUtils::latLonToLoc(double latitude, double longitude)
{
	if (latitude < -90.0 || latitude > 90.0)
		return wxEmptyString;

	if (longitude < -360.0 || longitude > 360.0)
		return wxEmptyString;

	latitude += 90.0;

	if (longitude > 180.0)
		longitude -= 360.0;

	if (longitude < -180.0)
		longitude += 360.0;

	longitude += 180.0;

	char locator[6U];

	double lon = ::floor(longitude / 20.0);
	double lat = ::floor(latitude / 10.0);

	locator[0U] = 'A' + (unsigned int)lon;
	locator[1U] = 'A' + (unsigned int)lat;

	longitude -= lon * 20.0;
	latitude  -= lat * 10.0;

	lon = ::floor(longitude / 2.0);
	lat = ::floor(latitude / 1.0);

	locator[2U] = '0' + (unsigned int)lon;
	locator[3U] = '0' + (unsigned int)lat;

	longitude -= lon * 2.0;
	latitude  -= lat * 1.0;

	lon = ::floor(longitude / (2.0 / 24.0));
	lat = ::floor(latitude / (1.0 / 24.0));

	locator[4U] = 'A' + (unsigned int)lon;
	locator[5U] = 'A' + (unsigned int)lat;

	return wxString(locator, wxConvLocal, 6U);
}
