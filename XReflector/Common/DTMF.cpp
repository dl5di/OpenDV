/*
 *   Copyright (C) 2012,2013 by Jonathan Naylor G4KLX
 *   Copyright (C) 2011 by DV Developer Group. DJ0ABR
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

#include "DTMF.h"

const unsigned char DTMF_MASK[] = {0x82U, 0x08U, 0x20U, 0x82U, 0x00U, 0x00U, 0x82U, 0x00U, 0x00U};
const unsigned char DTMF_SIG[]  = {0x82U, 0x08U, 0x20U, 0x82U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U};

const unsigned char DTMF_SYM_MASK[] = {0x10U, 0x40U, 0x08U, 0x20U};
const unsigned char DTMF_SYM0[]     = {0x00U, 0x40U, 0x08U, 0x20U};
const unsigned char DTMF_SYM1[]     = {0x00U, 0x00U, 0x00U, 0x00U};
const unsigned char DTMF_SYM2[]     = {0x00U, 0x40U, 0x00U, 0x00U};
const unsigned char DTMF_SYM3[]     = {0x10U, 0x00U, 0x00U, 0x00U};
const unsigned char DTMF_SYM4[]     = {0x00U, 0x00U, 0x00U, 0x20U};
const unsigned char DTMF_SYM5[]     = {0x00U, 0x40U, 0x00U, 0x20U};
const unsigned char DTMF_SYM6[]     = {0x10U, 0x00U, 0x00U, 0x20U};
const unsigned char DTMF_SYM7[]     = {0x00U, 0x00U, 0x08U, 0x00U};
const unsigned char DTMF_SYM8[]     = {0x00U, 0x40U, 0x08U, 0x00U};
const unsigned char DTMF_SYM9[]     = {0x10U, 0x00U, 0x08U, 0x00U};
const unsigned char DTMF_SYMA[]     = {0x10U, 0x40U, 0x00U, 0x00U};
const unsigned char DTMF_SYMB[]     = {0x10U, 0x40U, 0x00U, 0x20U};
const unsigned char DTMF_SYMC[]     = {0x10U, 0x40U, 0x08U, 0x00U};
const unsigned char DTMF_SYMD[]     = {0x10U, 0x40U, 0x08U, 0x20U};
const unsigned char DTMF_SYMS[]     = {0x00U, 0x00U, 0x08U, 0x20U};
const unsigned char DTMF_SYMH[]     = {0x10U, 0x00U, 0x08U, 0x20U};

CDTMF::CDTMF() :
m_data(),
m_command(),
m_pressed(false),
m_releaseCount(0U),
m_pressCount(0U),
m_lastChar(wxT(' '))
{
}

CDTMF::~CDTMF()
{
}

bool CDTMF::decode(const unsigned char* ambe, bool end)
{
	// DTMF begins with these byte values
	if (!end && (ambe[0] & DTMF_MASK[0]) == DTMF_SIG[0] && (ambe[1] & DTMF_MASK[1]) == DTMF_SIG[1] &&
				(ambe[2] & DTMF_MASK[2]) == DTMF_SIG[2] && (ambe[3] & DTMF_MASK[3]) == DTMF_SIG[3] &&
				(ambe[4] & DTMF_MASK[4]) == DTMF_SIG[4] && (ambe[5] & DTMF_MASK[5]) == DTMF_SIG[5] &&
				(ambe[6] & DTMF_MASK[6]) == DTMF_SIG[6] && (ambe[7] & DTMF_MASK[7]) == DTMF_SIG[7] &&
				(ambe[8] & DTMF_MASK[8]) == DTMF_SIG[8]) {
		unsigned char sym0 = ambe[4] & DTMF_SYM_MASK[0];
		unsigned char sym1 = ambe[5] & DTMF_SYM_MASK[1];
		unsigned char sym2 = ambe[7] & DTMF_SYM_MASK[2];
		unsigned char sym3 = ambe[8] & DTMF_SYM_MASK[3];

		wxChar c = wxT(' ');
		if (sym0 == DTMF_SYM0[0] && sym1 == DTMF_SYM0[1] && sym2 == DTMF_SYM0[2] && sym3 == DTMF_SYM0[3])
			c = wxT('0');
		else if (sym0 == DTMF_SYM1[0] && sym1 == DTMF_SYM1[1] && sym2 == DTMF_SYM1[2] && sym3 == DTMF_SYM1[3])
			c = wxT('1');
		else if (sym0 == DTMF_SYM2[0] && sym1 == DTMF_SYM2[1] && sym2 == DTMF_SYM2[2] && sym3 == DTMF_SYM2[3])
			c = wxT('2');
		else if (sym0 == DTMF_SYM3[0] && sym1 == DTMF_SYM3[1] && sym2 == DTMF_SYM3[2] && sym3 == DTMF_SYM3[3])
			c = wxT('3');
		else if (sym0 == DTMF_SYM4[0] && sym1 == DTMF_SYM4[1] && sym2 == DTMF_SYM4[2] && sym3 == DTMF_SYM4[3])
			c = wxT('4');
		else if (sym0 == DTMF_SYM5[0] && sym1 == DTMF_SYM5[1] && sym2 == DTMF_SYM5[2] && sym3 == DTMF_SYM5[3])
			c = wxT('5');
		else if (sym0 == DTMF_SYM6[0] && sym1 == DTMF_SYM6[1] && sym2 == DTMF_SYM6[2] && sym3 == DTMF_SYM6[3])
			c = wxT('6');
		else if (sym0 == DTMF_SYM7[0] && sym1 == DTMF_SYM7[1] && sym2 == DTMF_SYM7[2] && sym3 == DTMF_SYM7[3])
			c = wxT('7');
		else if (sym0 == DTMF_SYM8[0] && sym1 == DTMF_SYM8[1] && sym2 == DTMF_SYM8[2] && sym3 == DTMF_SYM8[3])
			c = wxT('8');
		else if (sym0 == DTMF_SYM9[0] && sym1 == DTMF_SYM9[1] && sym2 == DTMF_SYM9[2] && sym3 == DTMF_SYM9[3])
			c = wxT('9');
		else if (sym0 == DTMF_SYMA[0] && sym1 == DTMF_SYMA[1] && sym2 == DTMF_SYMA[2] && sym3 == DTMF_SYMA[3])
			c = wxT('A');
		else if (sym0 == DTMF_SYMB[0] && sym1 == DTMF_SYMB[1] && sym2 == DTMF_SYMB[2] && sym3 == DTMF_SYMB[3])
			c = wxT('B');
		else if (sym0 == DTMF_SYMC[0] && sym1 == DTMF_SYMC[1] && sym2 == DTMF_SYMC[2] && sym3 == DTMF_SYMC[3])
			c = wxT('C');
		else if (sym0 == DTMF_SYMD[0] && sym1 == DTMF_SYMD[1] && sym2 == DTMF_SYMD[2] && sym3 == DTMF_SYMD[3])
			c = wxT('D');
		else if (sym0 == DTMF_SYMS[0] && sym1 == DTMF_SYMS[1] && sym2 == DTMF_SYMS[2] && sym3 == DTMF_SYMS[3])
			c = wxT('*');
		else if (sym0 == DTMF_SYMH[0] && sym1 == DTMF_SYMH[1] && sym2 == DTMF_SYMH[2] && sym3 == DTMF_SYMH[3])
			c = wxT('#');

		if (c == m_lastChar) {
			m_pressCount++;
		} else {
			m_lastChar = c;
			m_pressCount = 0U;
		}

		if (c != wxT(' ') && !m_pressed && m_pressCount >= 3U) {
			m_data.Append(c);
			m_releaseCount = 0U;
			m_pressed = true;
		}

		return c != wxT(' ');
	} else {
		// If it is not a DTMF Code
		if ((end || m_releaseCount >= 100U) && m_data.Len() > 0U) {
			m_command = m_data;
			m_data.Clear();
			m_releaseCount = 0U;
		}

		m_pressed = false;
		m_releaseCount++;
		m_pressCount = 0U;
		m_lastChar = wxT(' ');

		return false;
	}
}

bool CDTMF::hasCommand() const
{
	return !m_command.IsEmpty();
}

// DTMF to YOUR call command
wxString CDTMF::translate()
{
	wxString command = m_command;
	m_command.Clear();

	if (command.IsEmpty())
		return wxEmptyString;

	if (command.IsSameAs(wxT("#")))
		return wxT("       U");

	if (command.IsSameAs(wxT("0")))
		return wxT("       I");

	if (command.IsSameAs(wxT("A")))
		return wxT("CCSA");

	if (command.IsSameAs(wxT("00")))
		return wxT("       I");

	if (command.GetChar(0U) == wxT('*'))
		return processDPlus(command.Mid(1U));
	else if (command.GetChar(0U) == wxT('B'))
		return processDExtra(command.Mid(1U));
	else if (command.GetChar(0U) == wxT('D'))
		return processDCS(command.Mid(1U));
	else
		return processCCS(command);
}

void CDTMF::reset()
{
	m_data.Clear();
	m_command.Clear();
	m_pressed = false;
	m_pressCount = 0U;
	m_releaseCount = 0U;
	m_lastChar = wxT(' ');
}

wxString CDTMF::processDExtra(const wxString& command) const
{
	unsigned int len = command.Len();
	if (len < 2U || len > 4U)
		return wxEmptyString;

	unsigned long n;
	command.Left(len - 1U).ToULong(&n);
	if (n == 0L)
		return wxEmptyString;

	wxChar c = command.GetChar(len - 1U);
	if (c != wxT('A') && c != wxT('B') && c != wxT('C') && c != wxT('D'))
		return wxEmptyString;

	wxString out;
	out.Printf(wxT("XRF%03lu%cL"), n, c);

	return out;
}

wxString CDTMF::processDPlus(const wxString& command) const
{
	unsigned int len = command.Len();
	if (len < 2U || len > 4U)
		return wxEmptyString;

	unsigned long n;
	command.Left(len - 1U).ToULong(&n);
	if (n == 0UL)
		return wxEmptyString;

	wxChar c = command.GetChar(len - 1U);
	if (c != wxT('A') && c != wxT('B') && c != wxT('C') && c != wxT('D'))
		return wxEmptyString;

	wxString out;
	out.Printf(wxT("REF%03lu%cL"), n, c);

	return out;
}

wxString CDTMF::processDCS(const wxString& command) const
{
	unsigned int len = command.Len();

	wxChar c = command.GetChar(len - 1U);
	if (c == wxT('A') || c == wxT('B') || c == wxT('C') || c == wxT('D')) {
		if (len < 2U || len > 4U)
			return wxEmptyString;

		unsigned long n;
		command.Left(len - 1U).ToULong(&n);
		if (n == 0UL)
			return wxEmptyString;

		wxString out;
		out.Printf(wxT("DCS%03lu%cL"), n, c);
	
		return out;
	} else {
		if (len < 3U || len > 5U)
			return wxEmptyString;

		unsigned long n1;
		command.Left(len - 2U).ToULong(&n1);
		if (n1 == 0UL)
			return wxEmptyString;

		unsigned long n2;
		command.Right(2U).ToULong(&n2);
		if (n2 == 0UL || n2 > 26UL)
			return wxEmptyString;

		c = wxT('A') + n2 - 1UL;

		wxString out;
		out.Printf(wxT("DCS%03lu%cL"), n1, c);
	
		return out;
	}
}

wxString CDTMF::processCCS(const wxString& command) const
{
	unsigned int len = command.Len();

	if (len != 4U)
		return wxEmptyString;

	unsigned long n;
	command.ToULong(&n);
	if (n == 0UL)
		return wxEmptyString;

	wxString out;
	out.Printf(wxT("CCS*%04lu"), n);

	return out;
}
