/*
 *   Copyright (C) 2011,2012 by Jonathan Naylor G4KLX
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

#include "BeaconUnit.h"

#include <wx/filename.h>
#include <wx/textfile.h>
#include <wx/tokenzr.h>
#include <wx/ffile.h>

const unsigned int MAX_FRAMES = 60U * DSTAR_FRAMES_PER_SEC;

const unsigned int SILENCE_LENGTH = 10U;

CBeaconUnit::CBeaconUnit(IBeaconCallback* handler, const wxString& callsign, const wxString& text, bool voice, TEXT_LANG language) :
m_ambe(NULL),
m_ambeLength(0U),
m_data(NULL),
m_dataLength(0U),
m_index(),
m_language(language),
m_handler(handler),
m_callsign(callsign),
m_encoder(),
m_in(0U),
m_out(0U),
m_seqNo(0U),
m_time(),
m_sending(false)
{
	wxASSERT(handler != NULL);

	wxString slowData = text;
	slowData.resize(20U, wxT(' '));
	m_encoder.setTextData(slowData);

	m_data = new unsigned char[MAX_FRAMES * VOICE_FRAME_LENGTH_BYTES];
	::memset(m_data, 0x00U, MAX_FRAMES * VOICE_FRAME_LENGTH_BYTES);

	if (!voice)
		return;

	wxString ambeFileName;
	wxString indxFileName;

	switch (m_language) {
		case TL_DEUTSCH:
			ambeFileName = wxT("de_DE.ambe");
			indxFileName = wxT("de_DE.indx");
			break;
		case TL_DANSK:
			ambeFileName = wxT("dk_DK.ambe");
			indxFileName = wxT("dk_DK.indx");
			break;
		case TL_ITALIANO:
			ambeFileName = wxT("it_IT.ambe");
			indxFileName = wxT("it_IT.indx");
			break;
		case TL_FRANCAIS:
			ambeFileName = wxT("fr_FR.ambe");
			indxFileName = wxT("fr_FR.indx");
			break;
		case TL_ESPANOL:
			ambeFileName = wxT("es_ES.ambe");
			indxFileName = wxT("es_ES.indx");
			break;
		case TL_SVENSKA:
			ambeFileName = wxT("se_SE.ambe");
			indxFileName = wxT("se_SE.indx");
			break;
		case TL_POLSKI:
			ambeFileName = wxT("pl_PL.ambe");
			indxFileName = wxT("pl_PL.indx");
			break;
		case TL_ENGLISH_US:
			ambeFileName = wxT("en_US.ambe");
			indxFileName = wxT("en_US.indx");
			break;
		case TL_NORSK:
			ambeFileName = wxT("no_NO.ambe");
			indxFileName = wxT("no_NO.indx");
			break;
//		case TL_NEDERLANDS_NL:
//			ambeFileName = wxT("nl_NL.ambe");
//			indxFileName = wxT("nl_NL.indx");
//			break;
//		case TL_NEDERLANDS_BE:
//			ambeFileName = wxT("nl_BE.ambe");
//			indxFileName = wxT("nl_BE.indx");
//			break;
		default:
			ambeFileName = wxT("en_GB.ambe");
			indxFileName = wxT("en_GB.indx");
			break;
	}

	bool ret = readAMBE(ambeFileName);
	if (!ret)
		return;

	readIndex(indxFileName);
}

CBeaconUnit::~CBeaconUnit()
{
	for (CIndexList_t::iterator it = m_index.begin(); it != m_index.end(); ++it)
		delete it->second;

	delete[] m_ambe;
	delete[] m_data;
}

void CBeaconUnit::sendBeacon()
{
	m_handler->transmitBeaconHeader();

	m_sending = true;

	m_time.Start();

	m_in         = 0U;
	m_out        = 0U;
	m_seqNo      = 0U;
	m_dataLength = 0U;

	if (m_ambe == NULL) {
		for (unsigned int i = 0U; i < 21U; i++) {
			unsigned char buffer[DV_FRAME_LENGTH_BYTES];

			if (i == 0U) {
				m_encoder.sync();

				::memcpy(buffer + 0U, NULL_AMBE_DATA_BYTES, VOICE_FRAME_LENGTH_BYTES);
				::memcpy(buffer + VOICE_FRAME_LENGTH_BYTES, DATA_SYNC_BYTES, DATA_FRAME_LENGTH_BYTES);
			} else {
				unsigned char text[DATA_FRAME_LENGTH_BYTES];
				m_encoder.getTextData(text);

				::memcpy(buffer + 0U, NULL_AMBE_DATA_BYTES, VOICE_FRAME_LENGTH_BYTES);
				::memcpy(buffer + VOICE_FRAME_LENGTH_BYTES, text, DATA_FRAME_LENGTH_BYTES);
			}

			::memcpy(m_data + m_dataLength, buffer, DV_FRAME_LENGTH_BYTES);
			m_dataLength += DV_FRAME_LENGTH_BYTES;
			m_in++;
		}
	} else {
		lookup(wxT(" "));
		lookup(wxT(" "));
		lookup(wxT(" "));
		lookup(wxT(" "));

		spellCallsign(m_callsign);
	
		lookup(wxT(" "));
		lookup(wxT(" "));
		lookup(wxT(" "));
		lookup(wxT(" "));
	}
}

void CBeaconUnit::clock()
{
	if (!m_sending)
		return;

	unsigned int needed = m_time.Time() / DSTAR_FRAME_TIME_MS;

	while (m_out < needed) {
		m_handler->transmitBeaconData(m_data + m_out * DV_FRAME_LENGTH_BYTES, DV_FRAME_LENGTH_BYTES, false);
		m_out++;

		if (m_in == m_out) {
			m_handler->transmitBeaconData(END_PATTERN_BYTES, DV_FRAME_LENGTH_BYTES, true);
			m_sending = false;
			m_in  = 0U;
			m_out = 0U;
			return;
		}
	}
}

bool CBeaconUnit::lookup(const wxString &id)
{
	CIndexRecord* info = m_index[id];
	if (info == NULL) {
		// wxLogError(wxT("Cannot find the AMBE index for *%s*"), id.c_str());
		return false;
	}

	unsigned int  start = info->getStart();
	unsigned int length = info->getLength();

	for (unsigned int i = 0U; i < length; i++) {
		unsigned char* dataIn = m_ambe + (start + i) * VOICE_FRAME_LENGTH_BYTES;

		unsigned char buffer[DV_FRAME_LENGTH_BYTES];
		::memcpy(buffer + 0U, dataIn, VOICE_FRAME_LENGTH_BYTES);

		// Insert sync bytes when the sequence number is zero, slow data otherwise
		if (m_seqNo == 0U) {
			::memcpy(buffer + VOICE_FRAME_LENGTH_BYTES, DATA_SYNC_BYTES, DATA_FRAME_LENGTH_BYTES);
			m_encoder.sync();
		} else {
			m_encoder.getTextData(buffer + VOICE_FRAME_LENGTH_BYTES);
		}

		::memcpy(m_data + m_dataLength, buffer, DV_FRAME_LENGTH_BYTES);
		m_dataLength += DV_FRAME_LENGTH_BYTES;
		m_in++;

		m_seqNo++;
		if (m_seqNo == 21U)
			m_seqNo = 0U;
	}

	return true;
}

void CBeaconUnit::spellCallsign(const wxString &callsign)
{
	unsigned int length = callsign.Len();

	for (unsigned int i = 0U; i < (length - 1U); i++) {
		wxString c = callsign.Mid(i, 1U);

		if (!c.IsSameAs(wxT(" ")))
			lookup(c);
	}

	wxChar c = callsign.GetChar(length - 1U);

	switch (c) {
		case wxT('A'):
			lookup(wxT("alpha"));
			break;
		case wxT('B'):
			lookup(wxT("bravo"));
			break;
		case wxT('C'):
			lookup(wxT("charlie"));
			break;
		case wxT('D'):
			lookup(wxT("delta"));
			break;
		default:
			lookup(wxString(c));
			break;
	}
}

bool CBeaconUnit::readAMBE(const wxString& name)
{
	wxFileName fileName(wxFileName::GetHomeDir(), name);

	if (!fileName.IsFileReadable()) {
		wxLogMessage(wxT("File %s not readable"), fileName.GetFullPath().c_str());
#if defined(__WINDOWS__)
		fileName.Assign(::wxGetCwd(), name);
#else
		fileName.Assign(wxT(DATA_DIR), name);
#endif
		if (!fileName.IsFileReadable()) {
			wxLogMessage(wxT("File %s not readable"), fileName.GetFullPath().c_str());
			return false;
		}
	}

	wxFFile file;

	bool ret = file.Open(fileName.GetFullPath().c_str(), wxT("rb"));
	if (!ret) {
		wxLogMessage(wxT("Cannot open %s for reading"), fileName.GetFullPath().c_str());
		return false;
	}

	wxLogMessage(wxT("Reading %s"), fileName.GetFullPath().c_str());

	unsigned char buffer[VOICE_FRAME_LENGTH_BYTES];

	size_t n = file.Read(buffer, 4U);
	if (n != 4U) {
		wxLogMessage(wxT("Unable to read the header from %s"), fileName.GetFullPath().c_str());
		file.Close();
		return false;
	}

	if (::memcmp(buffer, "AMBE", 4U) != 0) {
		wxLogMessage(wxT("Invalid header from %s"), fileName.GetFullPath().c_str());
		file.Close();
		return false;
	}

	// Length of the file minus the header
	unsigned int length = file.Length() - 4U;

	// Hold the file data plus silence at the end
	m_ambe = new unsigned char[length + SILENCE_LENGTH * VOICE_FRAME_LENGTH_BYTES];
	m_ambeLength = length / VOICE_FRAME_LENGTH_BYTES;

	// Add silence to the beginning of the buffer
	unsigned char* p = m_ambe;
	for (unsigned int i = 0U; i < SILENCE_LENGTH; i++, p += VOICE_FRAME_LENGTH_BYTES)
		::memcpy(p, NULL_AMBE_DATA_BYTES, VOICE_FRAME_LENGTH_BYTES);

	n = file.Read(p, length);
	if (n != length) {
		wxLogMessage(wxT("Unable to read the AMBE data from %s"), fileName.GetFullPath().c_str());
		file.Close();
		delete[] m_ambe;
		m_ambe = NULL;
		return false;
	}

	file.Close();

	return true;
}

bool CBeaconUnit::readIndex(const wxString& name)
{
	wxFileName fileName(wxFileName::GetHomeDir(), name);

	if (!fileName.IsFileReadable()) {
		wxLogMessage(wxT("File %s not readable"), fileName.GetFullPath().c_str());
#if defined(__WINDOWS__)
		fileName.Assign(::wxGetCwd(), name);
#else
		fileName.Assign(wxT(DATA_DIR), name);
#endif
		if (!fileName.IsFileReadable()) {
			wxLogMessage(wxT("File %s not readable"), fileName.GetFullPath().c_str());
			return false;
		}
	}

	wxTextFile file;

	bool ret = file.Open(fileName.GetFullPath());
	if (!ret) {
		wxLogMessage(wxT("Cannot open %s for reading"), fileName.GetFullPath().c_str());
		return false;
	}

	// Add a silence entry at the beginning
	m_index[wxT(" ")] = new CIndexRecord(wxT(" "), 0U, SILENCE_LENGTH);

	wxLogMessage(wxT("Reading %s"), fileName.GetFullPath().c_str());

	unsigned int nLines = file.GetLineCount();

	for (unsigned int i = 0; i < nLines; i++) {
		wxString line = file.GetLine(i);

		if (line.length() > 0 && line.GetChar(0) != wxT('#')) {
			wxStringTokenizer t(line, wxT(" \t\r\n"), wxTOKEN_STRTOK);
			wxString name      = t.GetNextToken();
			wxString startTxt  = t.GetNextToken();
			wxString lengthTxt = t.GetNextToken();

			if (!name.IsEmpty() && !startTxt.IsEmpty() && !lengthTxt.IsEmpty()) {
				unsigned long start;
				startTxt.ToULong(&start);

				unsigned long length;
				lengthTxt.ToULong(&length);

				if (start >= m_ambeLength || (start + length) >= m_ambeLength)
					wxLogError(wxT("The start or end for *%s* is out of range, start: %lu, end: %lu"), name.c_str(), start, start + length);
				else
					m_index[name] = new CIndexRecord(name, start + SILENCE_LENGTH, length);
			}
		}
	}

	file.Close();

	return true;
}
