/*
 *   Copyright (C) 2011,2012,2013 by Jonathan Naylor G4KLX
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

#include "DStarDefines.h"
#include "HeaderData.h"
#include "AudioUnit.h"

#include <wx/filename.h>
#include <wx/textfile.h>
#include <wx/tokenzr.h>
#include <wx/ffile.h>

unsigned char* CAudioUnit::m_ambe = NULL;
unsigned int   CAudioUnit::m_ambeLength = 0U;
CIndexList_t   CAudioUnit::m_index;

TEXT_LANG CAudioUnit::m_language = TL_ENGLISH_UK;

const unsigned int MAX_FRAMES = 60U * DSTAR_FRAMES_PER_SEC;

const unsigned int SILENCE_LENGTH = 10U;

void CAudioUnit::initialise()
{
}

void CAudioUnit::setLanguage(TEXT_LANG language)
{
	m_language = language;

	wxString ambeFileName;
	wxString indxFileName;

	switch (language) {
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
	if (!ret) {
		delete[] m_ambe;
		m_ambe = NULL;
		return;
	}

	ret = readIndex(indxFileName);
	if (!ret) {
		delete[] m_ambe;
		m_ambe = NULL;
	}
}

void CAudioUnit::finalise()
{
	for (CIndexList_t::iterator it = m_index.begin(); it != m_index.end(); ++it)
		delete it->second;

	delete[] m_ambe;
}

CAudioUnit::CAudioUnit(IRepeaterCallback* handler, const wxString& callsign) :
m_handler(handler),
m_callsign(callsign),
m_encoder(),
m_status(AS_IDLE),
m_linkStatus(LS_NONE),
m_text(),
m_tempText(),
m_reflector(),
m_timer(1000U, 2U),			// 2 seconds
m_data(NULL),
m_in(0U),
m_out(0U),
m_seqNo(0U),
m_time()
{
	wxASSERT(handler != NULL);

	m_data = new CAMBEData*[MAX_FRAMES];

	for (unsigned int i = 0U; i < MAX_FRAMES; i++)
		m_data[i] = NULL;
}

CAudioUnit::~CAudioUnit()
{
	delete[] m_data;
}

void CAudioUnit::sendStatus()
{
	if (m_ambe == NULL)
		return;

	if (m_status != AS_IDLE)
		return;

	m_status = AS_WAIT;
	m_timer.start();
}

void CAudioUnit::setStatus(LINK_STATUS status, const wxString& reflector, const wxString& text)
{
	m_linkStatus = status;
	m_reflector  = reflector;
	m_text       = text;
}

void CAudioUnit::setTempText(const wxString& text)
{
	m_tempText = text;
}

void CAudioUnit::clock(unsigned int ms)
{
	m_timer.clock(ms);

	if (m_status == AS_WAIT && m_timer.hasExpired()) {
		if (m_tempText.IsEmpty()) {
			m_encoder.setTextData(m_text);
		} else {
			m_encoder.setTextData(m_tempText);
			m_tempText.Clear();
		}

		m_timer.stop();

		// Create the message
		unsigned int id = CHeaderData::createId();

		lookup(id, wxT(" "));
		lookup(id, wxT(" "));
		lookup(id, wxT(" "));
		lookup(id, wxT(" "));

		bool found;

		switch (m_linkStatus) {
			case LS_NONE:
				lookup(id, wxT("notlinked"));
				break;
			case LS_LINKED_DCS:
			case LS_LINKED_DPLUS:
			case LS_LINKED_DEXTRA:
				found = lookup(id, wxT("linkedto"));
				if (!found) {
					lookup(id, wxT("linked"));
					lookup(id, wxT("2"));
				}
				spellReflector(id, m_reflector);
				break;
			default:
				found = lookup(id, wxT("linkingto"));
				if (!found) {
					lookup(id, wxT("linking"));
					lookup(id, wxT("2"));
				}
				spellReflector(id, m_reflector);
				break;
		}

		lookup(id, wxT(" "));
		lookup(id, wxT(" "));
		lookup(id, wxT(" "));
		lookup(id, wxT(" "));

		// RPT1 and RPT2 will be filled in later
		CHeaderData header;
		header.setMyCall1(m_callsign);
		header.setMyCall2(wxT("INFO"));
		header.setYourCall(wxT("CQCQCQ  "));
		header.setId(id);

		m_handler->process(header, DIR_INCOMING, AS_INFO);

		m_out    = 0U;
		m_seqNo  = 0U;
		m_status = AS_TRANSMIT;

		m_time.Start();

		return;
	}

	if (m_status == AS_TRANSMIT) {
		unsigned int needed = m_time.Time() / DSTAR_FRAME_TIME_MS;

		while (m_out < needed) {
			CAMBEData* data = m_data[m_out];
			m_data[m_out] = NULL;
			m_out++;

			if (m_in == m_out)
				data->setEnd(true);

			m_handler->process(*data, DIR_INCOMING, AS_INFO);

			delete data;

			if (m_in == m_out) {
				m_in     = 0U;
				m_out    = 0U;
				m_status = AS_IDLE;
				return;
			}
		}

		return;
	}
}

void CAudioUnit::cancel()
{
	for (unsigned int i = 0U; i < MAX_FRAMES; i++) {
		if (m_data[i] != NULL) {
			delete m_data[i];
			m_data[i] = NULL;
		}
	}

	m_status = AS_IDLE;
	m_out    = 0U;
	m_in     = 0U;

	m_timer.stop();
}

bool CAudioUnit::lookup(unsigned int id, const wxString &name)
{
	CIndexRecord* info = m_index[name];
	if (info == NULL) {
		// wxLogError(wxT("Cannot find the AMBE index for *%s*"), name.c_str());
		return false;
	}

	unsigned int  start = info->getStart();
	unsigned int length = info->getLength();

	for (unsigned int i = 0U; i < length; i++) {
		unsigned char* dataIn = m_ambe + (start + i) * VOICE_FRAME_LENGTH_BYTES;

		CAMBEData* dataOut = new CAMBEData;
		dataOut->setSeq(m_seqNo);
		dataOut->setId(id);

		unsigned char buffer[DV_FRAME_LENGTH_BYTES];
		::memcpy(buffer + 0U, dataIn, VOICE_FRAME_LENGTH_BYTES);

		// Insert sync bytes when the sequence number is zero, slow data otherwise
		if (m_seqNo == 0U) {
			::memcpy(buffer + VOICE_FRAME_LENGTH_BYTES, DATA_SYNC_BYTES, DATA_FRAME_LENGTH_BYTES);
			m_encoder.sync();
		} else {
			m_encoder.getTextData(buffer + VOICE_FRAME_LENGTH_BYTES);
		}

		dataOut->setData(buffer, DV_FRAME_LENGTH_BYTES);

		m_seqNo++;
		if (m_seqNo == 21U)
			m_seqNo = 0U;

		m_data[m_in] = dataOut;
		m_in++;
	}

	return true;
}

void CAudioUnit::spellReflector(unsigned int id, const wxString &reflector)
{
	unsigned int length = reflector.Len();

	for (unsigned int i = 0U; i < (length - 1U); i++) {
		wxString c = reflector.Mid(i, 1U);

		if (!c.IsSameAs(wxT(" ")))
			lookup(id, c);
	}

	wxChar c = reflector.GetChar(length - 1U);

	if (m_linkStatus == LS_LINKING_DCS || m_linkStatus == LS_LINKED_DCS) {
		lookup(id, wxString(c));
		return;
	}

	switch (c) {
		case wxT('A'):
			lookup(id, wxT("alpha"));
			break;
		case wxT('B'):
			lookup(id, wxT("bravo"));
			break;
		case wxT('C'):
			lookup(id, wxT("charlie"));
			break;
		case wxT('D'):
			lookup(id, wxT("delta"));
			break;
		default:
			lookup(id, wxString(c));
			break;
	}
}

bool CAudioUnit::readAMBE(const wxString& name)
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

bool CAudioUnit::readIndex(const wxString& name)
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
