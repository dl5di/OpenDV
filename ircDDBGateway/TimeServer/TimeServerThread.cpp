/*
 *   Copyright (C) 2012,2013 by Jonathan Naylor G4KLX
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

#include "TimeServerThread.h"
#include "DStarDefines.h"
#include "Utils.h"

#include <wx/filename.h>
#include <wx/textfile.h>
#include <wx/tokenzr.h>
#include <wx/ffile.h>

const unsigned int MAX_FRAMES = 60U * DSTAR_FRAMES_PER_SEC;

const unsigned int SILENCE_LENGTH = 10U;

enum SLOW_DATA {
	SD_HEADER,
	SD_TEXT
};

CTimeServerThread::CTimeServerThread() :
m_socket(wxEmptyString, 0U),
m_callsign(),
m_callsignA(),
m_callsignB(),
m_callsignC(),
m_callsignD(),
m_callsignE(),
m_callsignG(),
m_address(),
m_language(LANG_ENGLISH_UK_1),
m_format(FORMAT_VOICE_TIME),
m_interval(INTERVAL_15MINS),
m_ambe(NULL),
m_ambeLength(0U),
m_index(),
m_seqNo(0U),
m_in(0U),
m_encoder(),
m_data(NULL),
m_killed(false)
{
	m_address.s_addr = INADDR_NONE;

	m_data = new CAMBEData*[MAX_FRAMES];

	for (unsigned int i = 0U; i < MAX_FRAMES; i++)
		m_data[i] = NULL;
}

CTimeServerThread::~CTimeServerThread()
{
	for (CIndexList_t::iterator it = m_index.begin(); it != m_index.end(); ++it)
		delete it->second;

	delete[] m_ambe;
	delete[] m_data;
}

void CTimeServerThread::run()
{
	// Wait here until we have the essentials to run
	while (!m_killed && m_address.s_addr == INADDR_NONE && m_callsignA.IsEmpty() && m_callsignB.IsEmpty() && m_callsignC.IsEmpty() && m_callsignD.IsEmpty() && m_callsignE.IsEmpty())
		::wxMilliSleep(500UL);		// 1/2 sec

	if (m_killed)
		return;

	if (m_format != FORMAT_TEXT_TIME) {
		bool ret = loadAMBE();
		if (!ret) {
			wxLogWarning(wxT("Cannot load the AMBE data, using text only time"));
			m_format = FORMAT_TEXT_TIME;
		}
	}

	wxLogMessage(wxT("Starting the Time Server thread"));

	unsigned int lastMin = 0U;

	while (!m_killed) {
		time_t now;
		::time(&now);

		struct tm* tm = ::localtime(&now);

		unsigned int hour = tm->tm_hour;
		unsigned int  min = tm->tm_min;

		if (min != lastMin) {
			if (m_interval == INTERVAL_15MINS && (min == 0U || min == 15U || min == 30U || min == 45U))
				sendTime(hour, min);
			else if (m_interval == INTERVAL_30MINS && (min == 0U || min == 30U))
				sendTime(hour, min);
			else if (m_interval == INTERVAL_60MINS && min == 0U)
				sendTime(hour, min);
		}

		lastMin = min;

		::wxMilliSleep(450UL);
	}

	wxLogMessage(wxT("Stopping the Time Server thread"));

	m_socket.close();
}

void CTimeServerThread::kill()
{
	m_killed = true;
}

bool CTimeServerThread::setGateway(const wxString& callsign, bool sendA, bool sendB, bool sendC, bool sendD, bool sendE, const wxString& address)
{
	m_callsign = callsign;
	m_callsign.resize(LONG_CALLSIGN_LENGTH - 1U, wxT(' '));

	m_callsignG = m_callsign;
	m_callsignG.Append(wxT("G"));

	if (sendA) {
		m_callsignA = m_callsign;
		m_callsignA.Append(wxT("A"));
	}

	if (sendB) {
		m_callsignB = m_callsign;
		m_callsignB.Append(wxT("B"));
	}

	if (sendC) {
		m_callsignC = m_callsign;
		m_callsignC.Append(wxT("C"));
	}

	if (sendD) {
		m_callsignD = m_callsign;
		m_callsignD.Append(wxT("D"));
	}

	if (sendE) {
		m_callsignE = m_callsign;
		m_callsignE.Append(wxT("E"));
	}

	m_callsign.Append(wxT(" "));

	m_address = CUDPReaderWriter::lookup(address);

	bool ret = m_socket.open();
	if (!ret)
		return false;

	return true;
}

void CTimeServerThread::setAnnouncements(LANGUAGE language, FORMAT format, INTERVAL interval)
{
	m_language = language;
	m_format   = format;
	m_interval = interval;
}

void CTimeServerThread::sendTime(unsigned int hour, unsigned int min)
{
	wxArrayString words;

	switch (m_language) {
		case LANG_ENGLISH_UK_1:
			words = sendTimeEnGB1(hour, min);
			break;
		case LANG_ENGLISH_UK_2:
			words = sendTimeEnGB2(hour, min);
			break;
		case LANG_ENGLISH_US_1:
			words = sendTimeEnUS1(hour, min);
			break;
		case LANG_ENGLISH_US_2:
			words = sendTimeEnUS2(hour, min);
			break;
		case LANG_DEUTSCH_1:
			words = sendTimeDeDE1(hour, min);
			break;
		case LANG_DEUTSCH_2:
			words = sendTimeDeDE2(hour, min);
			break;
		case LANG_FRANCAIS:
			words = sendTimeFrFR(hour, min);
			break;
		case LANG_NEDERLANDS:
			words = sendTimeNlNL(hour, min);
			break;
		case LANG_SVENSKA:
			words = sendTimeSeSE(hour, min);
			break;
		case LANG_ESPANOL:
			words = sendTimeEsES(hour, min);
			break;
		case LANG_NORSK:
			words = sendTimeNoNO(hour, min);
			break;
		case LANG_PORTUGUES:
			words = sendTimePtPT(hour, min);
			break;
		default:
			break;
	}

	send(words, hour, min);
}

wxArrayString CTimeServerThread::sendTimeEnGB1(unsigned int hour, unsigned int min)
{
	wxArrayString words;

	words.Add(wxT("It_is"));

	switch (hour) {
		case 0U:
		case 12U:
			words.Add(wxT("twelve"));
			break;
		case 1U:
		case 13U:
			words.Add(wxT("one"));
			break;
		case 2U:
		case 14U:
			words.Add(wxT("two"));
			break;
		case 3U:
		case 15U:
			words.Add(wxT("three"));
			break;
		case 4U:
		case 16U:
			words.Add(wxT("four"));
			break;
		case 5U:
		case 17U:
			words.Add(wxT("five"));
			break;
		case 6U:
		case 18U:
			words.Add(wxT("six"));
			break;
		case 7U:
		case 19U:
			words.Add(wxT("seven"));
			break;
		case 8U:
		case 20U:
			words.Add(wxT("eight"));
			break;
		case 9U:
		case 21U:
			words.Add(wxT("nine"));
			break;
		case 10U:
		case 22U:
			words.Add(wxT("ten"));
			break;
		case 11U:
		case 23U:
			words.Add(wxT("eleven"));
			break;
		default:
			break;
	}
	
	switch (min) {
		case 15U:
			words.Add(wxT("fifteen"));
			break;
		case 30U:
			words.Add(wxT("thirty"));
			break;
		case 45U:
			words.Add(wxT("forty-five"));
			break;
		default:
			break;
	}

	if (hour >= 12U)
		words.Add(wxT("PM"));
	else
		words.Add(wxT("AM"));

	return words;
}

wxArrayString CTimeServerThread::sendTimeEnGB2(unsigned int hour, unsigned int min)
{
	wxArrayString words;

	words.Add(wxT("It_is"));

	if (min == 15U) {
		words.Add(wxT("a_quarter_past"));
	} else if (min == 30U) {
		words.Add(wxT("half_past"));
	} else if (min == 45U) {
		words.Add(wxT("a_quarter_to"));
		if (hour == 23U)
			hour = 0U;
		else
			hour++;
	}

	if (hour == 0U && min == 0U) {
		words.Add(wxT("midnight"));
	} else if (hour == 12U && min == 0U) {
		words.Add(wxT("twelve"));
		words.Add(wxT("noon"));
	} else if (hour == 0U || hour == 12U) {
		words.Add(wxT("twelve"));
	} else if (hour == 1U || hour == 13U) {
		words.Add(wxT("one"));
	} else if (hour == 2U || hour == 14U) {
		words.Add(wxT("two"));
	} else if (hour == 3U || hour == 15U) {
		words.Add(wxT("three"));
	} else if (hour == 4U || hour == 16U) {
		words.Add(wxT("four"));
	} else if (hour == 5U || hour == 17U) {
		words.Add(wxT("five"));
	} else if (hour == 6U || hour == 18U) {
		words.Add(wxT("six"));
	} else if (hour == 7U || hour == 19U) {
		words.Add(wxT("seven"));
	} else if (hour == 8U || hour == 20U) {
		words.Add(wxT("eight"));
	} else if (hour == 9U || hour == 21U) {
		words.Add(wxT("nine"));
	} else if (hour == 10U || hour == 22U) {
		words.Add(wxT("ten"));
	} else if (hour == 11U || hour == 23U) {
		words.Add(wxT("eleven"));
	}

	if (hour != 0U && hour != 12U && min == 0U)
		words.Add(wxT("O_Clock"));

	return words;
}

wxArrayString CTimeServerThread::sendTimeEnUS1(unsigned int hour, unsigned int min)
{
	wxArrayString words;

	words.Add(wxT("It_is"));

	switch (hour) {
		case 0U:
		case 12U:
			words.Add(wxT("twelve"));
			break;
		case 1U:
		case 13U:
			words.Add(wxT("one"));
			break;
		case 2U:
		case 14U:
			words.Add(wxT("two"));
			break;
		case 3U:
		case 15U:
			words.Add(wxT("three"));
			break;
		case 4U:
		case 16U:
			words.Add(wxT("four"));
			break;
		case 5U:
		case 17U:
			words.Add(wxT("five"));
			break;
		case 6U:
		case 18U:
			words.Add(wxT("six"));
			break;
		case 7U:
		case 19U:
			words.Add(wxT("seven"));
			break;
		case 8U:
		case 20U:
			words.Add(wxT("eight"));
			break;
		case 9U:
		case 21U:
			words.Add(wxT("nine"));
			break;
		case 10U:
		case 22U:
			words.Add(wxT("ten"));
			break;
		case 11U:
		case 23U:
			words.Add(wxT("eleven"));
			break;
		default:
			break;
	}
	
	switch (min) {
		case 15U:
			words.Add(wxT("fifteen"));
			break;
		case 30U:
			words.Add(wxT("thirty"));
			break;
		case 45U:
			words.Add(wxT("forty-five"));
			break;
		default:
			break;
	}

	if (hour >= 12U)
		words.Add(wxT("PM"));
	else
		words.Add(wxT("AM"));

	return words;
}

wxArrayString CTimeServerThread::sendTimeEnUS2(unsigned int hour, unsigned int min)
{
	wxArrayString words;

	words.Add(wxT("It_is"));

	if (min == 15U) {
		words.Add(wxT("a_quarter_past"));
	} else if (min == 30U) {
		words.Add(wxT("half_past"));
	} else if (min == 45U) {
		words.Add(wxT("a_quarter_to"));
		if (hour == 23U)
			hour = 0U;
		else
			hour++;
	}

	if (hour == 0U && min == 0U) {
		words.Add(wxT("midnight"));
	} else if (hour == 12U && min == 0U) {
		words.Add(wxT("twelve"));
		words.Add(wxT("noon"));
	} else if (hour == 0U || hour == 12U) {
		words.Add(wxT("twelve"));
	} else if (hour == 1U || hour == 13U) {
		words.Add(wxT("one"));
	} else if (hour == 2U || hour == 14U) {
		words.Add(wxT("two"));
	} else if (hour == 3U || hour == 15U) {
		words.Add(wxT("three"));
	} else if (hour == 4U || hour == 16U) {
		words.Add(wxT("four"));
	} else if (hour == 5U || hour == 17U) {
		words.Add(wxT("five"));
	} else if (hour == 6U || hour == 18U) {
		words.Add(wxT("six"));
	} else if (hour == 7U || hour == 19U) {
		words.Add(wxT("seven"));
	} else if (hour == 8U || hour == 20U) {
		words.Add(wxT("eight"));
	} else if (hour == 9U || hour == 21U) {
		words.Add(wxT("nine"));
	} else if (hour == 10U || hour == 22U) {
		words.Add(wxT("ten"));
	} else if (hour == 11U || hour == 23U) {
		words.Add(wxT("eleven"));
	}

	if (hour != 0U && hour != 12U && min == 0U)
		words.Add(wxT("O_Clock"));

	return words;
}

wxArrayString CTimeServerThread::sendTimeDeDE1(unsigned int hour, unsigned int min)
{
	wxArrayString words;

	words.Add(wxT("Es_ist"));

	switch (hour) {
		case 0U:  words.Add(wxT("null"));  break;
		case 1U:  words.Add(wxT("ein"));  break;
		case 2U:  words.Add(wxT("zwei"));  break;
		case 3U:  words.Add(wxT("drei"));  break;
		case 4U:  words.Add(wxT("vier"));  break;
		case 5U:  words.Add(wxT("fuenf"));  break;
		case 6U:  words.Add(wxT("sechs"));  break;
		case 7U:  words.Add(wxT("sieben"));  break;
		case 8U:  words.Add(wxT("acht"));  break;
		case 9U:  words.Add(wxT("neun"));  break;
		case 10U: words.Add(wxT("zehn")); break;
		case 11U: words.Add(wxT("elf")); break;
		case 12U: words.Add(wxT("zwoelf")); break;
		case 13U: words.Add(wxT("dreizehn")); break;
		case 14U: words.Add(wxT("vierzehn")); break;
		case 15U: words.Add(wxT("fuenfzehn")); break;
		case 16U: words.Add(wxT("sechzehn")); break;
		case 17U: words.Add(wxT("siebzehn")); break;
		case 18U: words.Add(wxT("achtzehn")); break;
		case 19U: words.Add(wxT("neunzehn")); break;
		case 20U: words.Add(wxT("zwanzig")); break;
		case 21U: words.Add(wxT("einundzwanzig")); break;
		case 22U: words.Add(wxT("zweiundzwanzig")); break;
		case 23U: words.Add(wxT("dreiundzwanzig")); break;
		default: break;
	}

	words.Add(wxT("Uhr"));

	switch (min) {
		case 15U:
			words.Add(wxT("fuenfzehn"));
			break;
		case 30U:
			words.Add(wxT("dreissig"));
			break;
		case 45U:
			words.Add(wxT("fuenfundvierzig"));
			break;
		default:
			break;
	}

	return words;
}

wxArrayString CTimeServerThread::sendTimeDeDE2(unsigned int hour, unsigned int min)
{
	wxArrayString words;

	words.Add(wxT("Es_ist"));

	if (min == 15U) {
		words.Add(wxT("viertel_nach"));
	} else if (min == 30U) {
		words.Add(wxT("halb"));
		if (hour == 23U)
			hour = 0U;
		else
			hour++;
	} else if (min == 45U) {
		words.Add(wxT("viertel_vor"));
		if (hour == 23U)
			hour = 0U;
		else
			hour++;
	}

	if (hour == 0U) {
		words.Add(wxT("null"));
	} else if (hour == 1U || hour == 13U) {
		words.Add(wxT("ein"));
	} else if (hour == 2U || hour == 14U) {
		words.Add(wxT("zwei"));
	} else if (hour == 3U || hour == 15U) {
		words.Add(wxT("drei"));
	} else if (hour == 4U || hour == 16U) {
		words.Add(wxT("vier"));
	} else if (hour == 5U || hour == 17U) {
		words.Add(wxT("fuenf"));
	} else if (hour == 6U || hour == 18U) {
		words.Add(wxT("sechs"));
	} else if (hour == 7U || hour == 19U) {
		words.Add(wxT("sieben"));
	} else if (hour == 8U || hour == 20U) {
		words.Add(wxT("acht"));
	} else if (hour == 9U || hour == 21U) {
		words.Add(wxT("neun"));
	} else if (hour == 10U || hour == 22U) {
		words.Add(wxT("zehn"));
	} else if (hour == 11U || hour == 23U) {
		words.Add(wxT("elf"));
	} else if (hour == 12U) {
		words.Add(wxT("zwoelf"));
	}

	if (min == 0U)
		words.Add(wxT("Uhr"));

	return words;
}

wxArrayString CTimeServerThread::sendTimeFrFR(unsigned int hour, unsigned int min)
{
	wxArrayString words;

	// if (hour > 17U)
	//	words.Add(wxT("bonsoir"));
	// else
	//	words.Add(wxT("bonjour"));

	words.Add(wxT("il_est"));

	if (min == 45U)
		hour++;

	if (hour == 0U) {
		words.Add(wxT("minuit"));
	} else if (hour == 1U || hour == 13U) {
		words.Add(wxT("une"));
	} else if (hour == 2U || hour == 14U) {
		words.Add(wxT("deux"));
	} else if (hour == 3U || hour == 15U) {
		words.Add(wxT("trois"));
	} else if (hour == 4U || hour == 16U) {
		words.Add(wxT("quatre"));
	} else if (hour == 5U || hour == 17U) {
		words.Add(wxT("cinq"));
	} else if (hour == 6U || hour == 18U) {
		words.Add(wxT("six"));
	} else if (hour == 7U || hour == 19U) {
		words.Add(wxT("sept"));
	} else if (hour == 8U || hour == 20U) {
		words.Add(wxT("huit"));
	} else if (hour == 9U || hour == 21U) {
		words.Add(wxT("neuf"));
	} else if (hour == 10U || hour == 22U) {
		words.Add(wxT("dix"));
	} else if (hour == 11U || hour == 23U) {
		words.Add(wxT("onze"));
	} else if (hour == 12U) {
		words.Add(wxT("midi"));
	}

	if (hour == 1U || hour == 13U)
		words.Add(wxT("heure"));
	else if (hour != 12U && hour != 0U)
		words.Add(wxT("heures"));

	if (min == 15U) {
		words.Add(wxT("et_quart"));
	} else if (min == 30U) {
		words.Add(wxT("et_demie"));
	} else if (min == 45U) {
		words.Add(wxT("moins_le_quart"));
		if (hour == 23U)
			hour = 0U;
		else
			hour++;
	}

	return words;
}

wxArrayString CTimeServerThread::sendTimeNlNL(unsigned int hour, unsigned int min)
{
	wxArrayString words;

	words.Add(wxT("Het_is"));

	if (min == 15U) {
		words.Add(wxT("kwart_over"));
	} else if (min == 30U) {
		words.Add(wxT("half"));
		if (hour == 23U)
			hour = 0U;
		else
			hour++;
	} else if (min == 45U) {
		words.Add(wxT("kwart_voor"));
		if (hour == 23U)
			hour = 0U;
		else
			hour++;
	}

	if (hour == 0U || hour == 12U) {
		words.Add(wxT("twaalf"));
	} else if (hour == 1U || hour == 13U) {
		words.Add(wxT("een"));
	} else if (hour == 2U || hour == 14U) {
		words.Add(wxT("twee"));
	} else if (hour == 3U || hour == 15U) {
		words.Add(wxT("drie"));
	} else if (hour == 4U || hour == 16U) {
		words.Add(wxT("vier"));
	} else if (hour == 5U || hour == 17U) {
		words.Add(wxT("vijf"));
	} else if (hour == 6U || hour == 18U) {
		words.Add(wxT("zes"));
	} else if (hour == 7U || hour == 19U) {
		words.Add(wxT("zeven"));
	} else if (hour == 8U || hour == 20U) {
		words.Add(wxT("acht"));
	} else if (hour == 9U || hour == 21U) {
		words.Add(wxT("negen"));
	} else if (hour == 10U || hour == 22U) {
		words.Add(wxT("tien"));
	} else if (hour == 11U || hour == 23U) {
		words.Add(wxT("elf"));
	}

	if (min == 0U)
		words.Add(wxT("uur"));

	return words;
}

wxArrayString CTimeServerThread::sendTimeSeSE(unsigned int hour, unsigned int min)
{
	wxArrayString words;

	words.Add(wxT("Klockan_ar"));

	if (min == 15U) {
		words.Add(wxT("kvart_over"));
	} else if (min == 30U) {
		words.Add(wxT("halv"));
		if (hour == 23U)
			hour = 0U;
		else
			hour++;
	} else if (min == 45U) {
		words.Add(wxT("kvart_i"));
		if (hour == 23U)
			hour = 0U;
		else
			hour++;
	}

	if (hour == 0U || hour == 12U) {
		words.Add(wxT("tolv"));
	} else if (hour == 1U || hour == 13U) {
		words.Add(wxT("ett"));
	} else if (hour == 2U || hour == 14U) {
		words.Add(wxT("tva"));
	} else if (hour == 3U || hour == 15U) {
		words.Add(wxT("tre"));
	} else if (hour == 4U || hour == 16U) {
		words.Add(wxT("fyra"));
	} else if (hour == 5U || hour == 17U) {
		words.Add(wxT("fem"));
	} else if (hour == 6U || hour == 18U) {
		words.Add(wxT("sex"));
	} else if (hour == 7U || hour == 19U) {
		words.Add(wxT("sju"));
	} else if (hour == 8U || hour == 20U) {
		words.Add(wxT("atta"));
	} else if (hour == 9U || hour == 21U) {
		words.Add(wxT("nio"));
	} else if (hour == 10U || hour == 22U) {
		words.Add(wxT("tio"));
	} else if (hour == 11U || hour == 23U) {
		words.Add(wxT("elva"));
	}

	return words;
}

wxArrayString CTimeServerThread::sendTimeEsES(unsigned int hour, unsigned int min)
{
	wxArrayString words;

	if (min == 45U) {
		hour++;
		if (hour == 24U)
			hour = 0U;
	}

	if (hour == 1U)
		words.Add(wxT("Es_la"));
	else if (hour == 0U || hour == 12U)
		words.Add(wxT("Es"));
	else
		words.Add(wxT("Son_las"));

	if (hour == 0U) {
		words.Add(wxT("medianoche"));
	} else if (hour == 1U || hour == 13U) {
		words.Add(wxT("una"));
	} else if (hour == 2U || hour == 14U) {
		words.Add(wxT("dos"));
	} else if (hour == 3U || hour == 15U) {
		words.Add(wxT("tres"));
	} else if (hour == 4U || hour == 16U) {
		words.Add(wxT("cuarto"));
	} else if (hour == 5U || hour == 17U) {
		words.Add(wxT("cinco"));
	} else if (hour == 6U || hour == 18U) {
		words.Add(wxT("seis"));
	} else if (hour == 7U || hour == 19U) {
		words.Add(wxT("siete"));
	} else if (hour == 8U || hour == 20U) {
		words.Add(wxT("ocho"));
	} else if (hour == 9U || hour == 21U) {
		words.Add(wxT("nueve"));
	} else if (hour == 10U || hour == 22U) {
		words.Add(wxT("diez"));
	} else if (hour == 11U || hour == 23U) {
		words.Add(wxT("once"));
	} else {
		words.Add(wxT("mediodia"));
	}

	if (min == 15U)
		words.Add(wxT("y_cuarto"));
	else if (min == 30U)
		words.Add(wxT("y_media"));
	else if (min == 45U)
		words.Add(wxT("menos_cuarto"));

	if (hour > 0U && hour < 12U)
		words.Add(wxT("de_la_manana"));
	else if (hour > 12U && hour < 19U)
		words.Add(wxT("de_la_tarde"));
	else if (hour >= 19U && hour <= 23U)
		words.Add(wxT("de_la_noche"));

	return words;
}

wxArrayString CTimeServerThread::sendTimeNoNO(unsigned int hour, unsigned int min)
{
	wxArrayString words;

	words.Add(wxT("Klokken_er"));

	if (min == 15U) {
		words.Add(wxT("kvart_over"));
	} else if (min == 30U) {
		words.Add(wxT("halv"));
		if (hour == 23U)
			hour = 0U;
		else
			hour++;
	} else if (min == 45U) {
		words.Add(wxT("kvart_pa"));
		if (hour == 23U)
			hour = 0U;
		else
			hour++;
	}

	if (hour == 0U || hour == 12U) {
		words.Add(wxT("tolv"));
	} else if (hour == 1U || hour == 13U) {
		words.Add(wxT("ett"));
	} else if (hour == 2U || hour == 14U) {
		words.Add(wxT("to"));
	} else if (hour == 3U || hour == 15U) {
		words.Add(wxT("tre"));
	} else if (hour == 4U || hour == 16U) {
		words.Add(wxT("fire"));
	} else if (hour == 5U || hour == 17U) {
		words.Add(wxT("fem"));
	} else if (hour == 6U || hour == 18U) {
		words.Add(wxT("seks"));
	} else if (hour == 7U || hour == 19U) {
		words.Add(wxT("sju"));
	} else if (hour == 8U || hour == 20U) {
		words.Add(wxT("atte"));
	} else if (hour == 9U || hour == 21U) {
		words.Add(wxT("ni"));
	} else if (hour == 10U || hour == 22U) {
		words.Add(wxT("ti"));
	} else if (hour == 11U || hour == 23U) {
		words.Add(wxT("elleve"));
	}

	return words;
}

wxArrayString CTimeServerThread::sendTimePtPT(unsigned int hour, unsigned int min)
{
	wxArrayString words;

	if (min == 45U) {
		hour++;
		if (hour == 24U)
			hour = 0U;
	}

	if (hour == 1U || hour == 13U)
		words.Add(wxT("E"));
	else if (hour == 0U || hour == 12U)
		words.Add(wxT("Es"));
	else
		words.Add(wxT("Sao"));

	if (min == 45U) {
		if (hour == 0U || hour == 12U || hour == 1U || hour == 13U)
			words.Add(wxT("quinze_para"));
		else
			words.Add(wxT("quinze_para_as"));
	}

	if (hour == 0U) {
		words.Add(wxT("meia-noite"));
	} else if (hour == 1U || hour == 13U) {
		words.Add(wxT("uma"));
	} else if (hour == 2U || hour == 14U) {
		words.Add(wxT("duas"));
	} else if (hour == 3U || hour == 15U) {
		words.Add(wxT("tres"));
	} else if (hour == 4U || hour == 16U) {
		words.Add(wxT("quatro"));
	} else if (hour == 5U || hour == 17U) {
		words.Add(wxT("cinco"));
	} else if (hour == 6U || hour == 18U) {
		words.Add(wxT("seis"));
	} else if (hour == 7U || hour == 19U) {
		words.Add(wxT("sete"));
	} else if (hour == 8U || hour == 20U) {
		words.Add(wxT("oito"));
	} else if (hour == 9U || hour == 21U) {
		words.Add(wxT("nove"));
	} else if (hour == 10U || hour == 22U) {
		words.Add(wxT("dez"));
	} else if (hour == 11U || hour == 23U) {
		words.Add(wxT("onze"));
	} else {
		words.Add(wxT("meio-dia"));
	}

	if (min == 0U)
		words.Add(wxT("hora"));
	else if (min == 15U)
		words.Add(wxT("e_quinze"));
	else if (min == 30U)
		words.Add(wxT("e_meia"));

	return words;
}

bool CTimeServerThread::loadAMBE()
{
	wxString ambeFileName;
	wxString indxFileName;

	switch (m_language) {
		case LANG_ENGLISH_US_1:
		case LANG_ENGLISH_US_2:
			ambeFileName = wxT("TIME_en_US.ambe");
			indxFileName = wxT("TIME_en_US.indx");
			break;
		case LANG_DEUTSCH_1:
		case LANG_DEUTSCH_2:
			ambeFileName = wxT("TIME_de_DE.ambe");
			indxFileName = wxT("TIME_de_DE.indx");
			break;
		case LANG_FRANCAIS:
			ambeFileName = wxT("TIME_fr_FR.ambe");
			indxFileName = wxT("TIME_fr_FR.indx");
			break;
		case LANG_NEDERLANDS:
			ambeFileName = wxT("TIME_nl_NL.ambe");
			indxFileName = wxT("TIME_nl_NL.indx");
			break;
		case LANG_SVENSKA:
			ambeFileName = wxT("TIME_se_SE.ambe");
			indxFileName = wxT("TIME_se_SE.indx");
			break;
		case LANG_ESPANOL:
			ambeFileName = wxT("TIME_es_ES.ambe");
			indxFileName = wxT("TIME_es_ES.indx");
			break;
		case LANG_NORSK:
			ambeFileName = wxT("TIME_no_NO.ambe");
			indxFileName = wxT("TIME_no_NO.indx");
			break;
		case LANG_PORTUGUES:
			ambeFileName = wxT("TIME_pt_PT.ambe");
			indxFileName = wxT("TIME_pt_PT.indx");
			break;
		default:
			ambeFileName = wxT("TIME_en_GB.ambe");
			indxFileName = wxT("TIME_en_GB.indx");
			break;
	}

	bool ret = readAMBE(ambeFileName);
	if (!ret) {
		delete[] m_ambe;
		m_ambe = NULL;
		return false;
	}

	ret = readIndex(indxFileName);
	if (!ret) {
		delete[] m_ambe;
		m_ambe = NULL;
		return false;
	}

	return true;
}

bool CTimeServerThread::readAMBE(const wxString& name)
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

bool CTimeServerThread::readIndex(const wxString& name)
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

bool CTimeServerThread::lookup(const wxString &id)
{
	CIndexRecord* info = m_index[id];
	if (info == NULL) {
		// wxLogError(wxT("Cannot find the AMBE index for *%s*"), id.c_str());
		return false;
	}

	unsigned int  start = info->getStart();
	unsigned int length = info->getLength();

	SLOW_DATA slowData = SD_TEXT;

	for (unsigned int i = 0U; i < length; i++) {
		unsigned char* dataIn = m_ambe + (start + i) * VOICE_FRAME_LENGTH_BYTES;

		CAMBEData* dataOut = new CAMBEData;
		dataOut->setDestination(m_address, G2_DV_PORT);
		dataOut->setSeq(m_seqNo);

		unsigned char buffer[DV_FRAME_LENGTH_BYTES];
		::memcpy(buffer + 0U, dataIn, VOICE_FRAME_LENGTH_BYTES);

		// Insert sync bytes when the sequence number is zero, slow data otherwise
		if (m_seqNo == 0U) {
			::memcpy(buffer + VOICE_FRAME_LENGTH_BYTES, DATA_SYNC_BYTES, DATA_FRAME_LENGTH_BYTES);
			m_encoder.sync();

			switch (slowData) {
				case SD_HEADER:
					slowData = SD_TEXT;
					break;
				case SD_TEXT:
					slowData = SD_HEADER;
					break;
			}
		} else {
			switch (slowData) {
				case SD_HEADER:
					m_encoder.getHeaderData(buffer + VOICE_FRAME_LENGTH_BYTES);
					break;
				case SD_TEXT:
					m_encoder.getTextData(buffer + VOICE_FRAME_LENGTH_BYTES);
					break;
			}
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

void CTimeServerThread::end()
{
	CAMBEData* dataOut = new CAMBEData;
	dataOut->setData(END_PATTERN_BYTES, DV_FRAME_LENGTH_BYTES);
	dataOut->setDestination(m_address, G2_DV_PORT);
	dataOut->setSeq(m_seqNo);
	dataOut->setEnd(true);

	m_data[m_in] = dataOut;
	m_in++;
}

bool CTimeServerThread::send(const wxArrayString &words, unsigned int hour, unsigned int min)
{
	unsigned int idA = CHeaderData::createId();
	unsigned int idB = CHeaderData::createId();
	unsigned int idC = CHeaderData::createId();
	unsigned int idD = CHeaderData::createId();
	unsigned int idE = CHeaderData::createId();

	CHeaderData header;
	header.setMyCall1(m_callsign);
	header.setRptCall1(m_callsignG);
	header.setRptCall2(m_callsign);		// Just for the slow data header
	header.setYourCall(wxT("CQCQCQ  "));
	header.setDestination(m_address, G2_DV_PORT);

	wxString slowData;
	switch (m_language) {
		case LANG_DEUTSCH_1:
		case LANG_DEUTSCH_2:
			header.setMyCall2(wxT("ZEIT"));
			slowData.Printf(wxT("Es ist %02u:%02u Uhr"), hour, min);
			break;
		case LANG_FRANCAIS:
			header.setMyCall2(wxT("TIME"));
			slowData.Printf(wxT("Il est %02u:%02u"), hour, min);
			break;
		case LANG_NEDERLANDS:
			header.setMyCall2(wxT("TIJD"));
			slowData.Printf(wxT("Het is %02u:%02u"), hour, min);
			break;
		case LANG_SVENSKA:
			header.setMyCall2(wxT("TID "));
			slowData.Printf(wxT("Klockan ar %02u:%02u"), hour, min);
			break;
		case LANG_ENGLISH_US_1:
		case LANG_ENGLISH_UK_1:
			header.setMyCall2(wxT("TIME"));
			if (hour == 0U)
				slowData.Printf(wxT("It is 12:%02u AM"), min);
			else if (hour == 12U)
				slowData.Printf(wxT("It is 12:%02u PM"), min);
			else if (hour > 12U)
				slowData.Printf(wxT("It is %02u:%02u PM"), hour - 12U, min);
			else
				slowData.Printf(wxT("It is %02u:%02u AM"), hour, min);
			break;
		case LANG_ESPANOL:
			header.setMyCall2(wxT("HORA"));
			if (hour == 1U)
				slowData.Printf(wxT("Es la %02u:%02u"), hour, min);
			else
				slowData.Printf(wxT("Son las %02u:%02u"), hour, min);
			break;
		case LANG_NORSK:
			header.setMyCall2(wxT("TID "));
			slowData.Printf(wxT("Klokken er %02u:%02u"), hour, min);
			break;
		case LANG_PORTUGUES:
			header.setMyCall2(wxT("HORA"));
			if (hour == 1U)
				slowData.Printf(wxT("E %02u:%02u"), hour, min);
			else
				slowData.Printf(wxT("Sao %02u:%02u"), hour, min);
			break;
		default:
			header.setMyCall2(wxT("TIME"));
			slowData.Printf(wxT("It is %02u:%02u"), hour, min);
			break;
	}

	m_encoder.setHeaderData(header);
	m_encoder.setTextData(slowData);

	m_in = 0U;

	if (m_format != FORMAT_TEXT_TIME) {
		wxString text = words.Item(0U);
		for (unsigned int i = 1U; i < words.GetCount(); i++) {
			text.Append(wxT(" "));
			text.Append(words.Item(i));
		}

		text.Replace(wxT("_"), wxT(" "));
		wxLogMessage(wxT("Sending voice \"%s\", sending text \"%s\""), text.c_str(), slowData.c_str());

		m_seqNo = 0U;

		// Build the audio
		lookup(wxT(" "));
		lookup(wxT(" "));
		lookup(wxT(" "));
		lookup(wxT(" "));

		for (unsigned int i = 0U; i < words.GetCount(); i++)
			lookup(words.Item(i));

		lookup(wxT(" "));
		lookup(wxT(" "));
		lookup(wxT(" "));
		lookup(wxT(" "));

		end();
	} else {
		wxLogMessage(wxT("Sending text \"%s\""), slowData.c_str());

		for (unsigned int i = 0U; i < 21U; i++) {
			CAMBEData* dataOut = new CAMBEData;
			dataOut->setDestination(m_address, G2_DV_PORT);
			dataOut->setSeq(i);

			unsigned char buffer[DV_FRAME_LENGTH_BYTES];
			::memcpy(buffer + 0U, NULL_AMBE_DATA_BYTES, VOICE_FRAME_LENGTH_BYTES);

			// Insert sync bytes when the sequence number is zero, slow data otherwise
			if (i == 0U) {
				::memcpy(buffer + VOICE_FRAME_LENGTH_BYTES, DATA_SYNC_BYTES, DATA_FRAME_LENGTH_BYTES);
				m_encoder.sync();
			} else {
				m_encoder.getTextData(buffer + VOICE_FRAME_LENGTH_BYTES);
			}

			dataOut->setData(buffer, DV_FRAME_LENGTH_BYTES);

			m_data[m_in] = dataOut;
			m_in++;
		}

		CAMBEData* dataOut = new CAMBEData;
		dataOut->setData(END_PATTERN_BYTES, DV_FRAME_LENGTH_BYTES);
		dataOut->setDestination(m_address, G2_DV_PORT);
		dataOut->setSeq(0U);
		dataOut->setEnd(true);

		m_data[m_in] = dataOut;
		m_in++;
	}

	if (m_in == 0U) {
		wxLogWarning(wxT("Not sending, no audio files loaded"));
		return false;
	}

	if (!m_callsignA.IsEmpty()) {
		header.setRptCall2(m_callsignA);
		header.setId(idA);
		sendHeader(header);
	}

	if (!m_callsignB.IsEmpty()) {
		header.setRptCall2(m_callsignB);
		header.setId(idB);
		sendHeader(header);
	}

	if (!m_callsignC.IsEmpty()) {
		header.setRptCall2(m_callsignC);
		header.setId(idC);
		sendHeader(header);
	}

	if (!m_callsignD.IsEmpty()) {
		header.setRptCall2(m_callsignD);
		header.setId(idD);
		sendHeader(header);
	}

	if (!m_callsignE.IsEmpty()) {
		header.setRptCall2(m_callsignE);
		header.setId(idE);
		sendHeader(header);
	}

	unsigned int out = 0U;

	wxStopWatch timer;
	timer.Start();

	for (;;) {
		unsigned int needed = timer.Time() / DSTAR_FRAME_TIME_MS;

		while (out < needed) {
			CAMBEData* data = m_data[out];
			m_data[out] = NULL;
			out++;

			if (!m_callsignA.IsEmpty()) {
				data->setId(idA);
				sendData(*data);
			}

			if (!m_callsignB.IsEmpty()) {
				data->setId(idB);
				sendData(*data);
			}

			if (!m_callsignC.IsEmpty()) {
				data->setId(idC);
				sendData(*data);
			}

			if (!m_callsignD.IsEmpty()) {
				data->setId(idD);
				sendData(*data);
			}

			if (!m_callsignE.IsEmpty()) {
				data->setId(idE);
				sendData(*data);
			}

			delete data;

			if (m_in == out)
				return true;
		}

		::wxMilliSleep(10UL);
	}
}

bool CTimeServerThread::sendHeader(const CHeaderData &header)
{
	unsigned char buffer[60U];
	unsigned int length = header.getG2Data(buffer, 60U, true);

#if defined(DUMP_TX)
	CUtils::dump(wxT("Sending Header"), buffer, length);
	return true;
#else
	for (unsigned int i = 0U; i < 5U; i++) {
		bool res = m_socket.write(buffer, length, header.getYourAddress(), header.getYourPort());
		if (!res)
			return false;
	}

	return true;
#endif
}

bool CTimeServerThread::sendData(const CAMBEData& data)
{
	unsigned char buffer[40U];
	unsigned int length = data.getG2Data(buffer, 40U);

#if defined(DUMP_TX)
	CUtils::dump(wxT("Sending Data"), buffer, length);
	return true;
#else
	return m_socket.write(buffer, length, data.getYourAddress(), data.getYourPort());
#endif
}
