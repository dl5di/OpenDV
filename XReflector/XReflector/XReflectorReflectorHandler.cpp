/*
 *   Copyright (C) 2010,2011,2012 by Jonathan Naylor G4KLX
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

#include "XReflectorReflectorHandler.h"
#include "XReflectorDExtraHandler.h"
#include "XReflectorDPlusHandler.h"
#include "DStarDefines.h"
#include "HeaderData.h"
#include "AMBEData.h"
#include "Utils.h"

#include <wx/filename.h>

unsigned int                  CXReflectorReflectorHandler::m_maxReflectors = 0U;
CXReflectorReflectorHandler** CXReflectorReflectorHandler::m_reflectors = NULL;
bool                          CXReflectorReflectorHandler::m_logEnabled = false;
wxString                      CXReflectorReflectorHandler::m_name;
wxFFile                       CXReflectorReflectorHandler::m_file;


CXReflectorReflectorHandler::CXReflectorReflectorHandler(const wxString& callsign) :
m_callsign(callsign),
m_gateway(callsign),
m_id(0x00U),
m_user(),
m_text(),
m_inactivityTimer(1000U, 2U)
{
	wxASSERT(!callsign.IsEmpty());

	m_gateway.Truncate(LONG_CALLSIGN_LENGTH - 1U);
	m_gateway.Append(wxT("G"));
}

CXReflectorReflectorHandler::~CXReflectorReflectorHandler()
{
}

void CXReflectorReflectorHandler::initialise(unsigned int maxReflectors, const wxString& name)
{
	wxASSERT(maxReflectors > 0U);

	m_maxReflectors = maxReflectors;
	m_name          = name;

	m_reflectors = new CXReflectorReflectorHandler*[m_maxReflectors];
	for (unsigned int i = 0U; i < m_maxReflectors; i++)
		m_reflectors[i] = NULL;
}

void CXReflectorReflectorHandler::setLogDirectory(const wxString& directory)
{
	m_logEnabled = openTextFile(directory);
}

void CXReflectorReflectorHandler::add(const wxString& callsign)
{
	wxASSERT(!callsign.IsEmpty());

	CXReflectorReflectorHandler* reflector = new CXReflectorReflectorHandler(callsign);

	for (unsigned int i = 0U; i < m_maxReflectors; i++) {
		if (m_reflectors[i] == NULL) {
			m_reflectors[i] = reflector;
			return;
		}
	}

	wxLogError(wxT("Cannot add reflector with callsign %s, no space"), callsign.c_str());

	delete reflector;
}

void CXReflectorReflectorHandler::finalise()
{
	for (unsigned int i = 0U; i < m_maxReflectors; i++) {
		delete m_reflectors[i];
		m_reflectors[i] = NULL;
	}

	delete[] m_reflectors;

	if (m_logEnabled)
		closeTextFile();
}

void CXReflectorReflectorHandler::clock(unsigned long ms)
{
	for (unsigned int i = 0U; i < m_maxReflectors; i++) {
		if (m_reflectors[i] != NULL)
			m_reflectors[i]->clockInt(ms);
	}
}

CXReflectorReflectorHandler* CXReflectorReflectorHandler::findReflector(const wxString& callsign)
{
	for (unsigned int i = 0U; i < m_maxReflectors; i++) {
		CXReflectorReflectorHandler* reflector = m_reflectors[i];
		if (reflector != NULL) {
			if (reflector->m_callsign.IsSameAs(callsign))
				return reflector;
		}
	}

	return NULL;
}

bool CXReflectorReflectorHandler::process(unsigned int id, CHeaderData &header)
{
	if (m_id != 0x00U)
		return false;

	m_id   = id;
	m_user = header.getMyCall1();
	m_text.reset();

	m_inactivityTimer.start();

	sendToDongles(header);
	sendToReflectors(header);

	return true;
}

bool CXReflectorReflectorHandler::process(unsigned int id, CAMBEData &data)
{
	if (id != m_id)
		return false;

	if (m_logEnabled)
		m_text.writeData(data);

	sendToDongles(data);
	sendToReflectors(data);

	m_inactivityTimer.reset();

	if (data.isEnd()) {
		m_id = 0x00U;
		m_inactivityTimer.stop();

		if (m_logEnabled) {
			bool ret = m_text.hasData();
			if (ret) {
				wxString text = m_text.getData();
				writeTextFile(m_user, text);
			}
		}
	}

	return true;
}

void CXReflectorReflectorHandler::timeout(unsigned int id)
{
	if (id == m_id) {
		m_id = 0x00U;
		m_inactivityTimer.stop();

		if (m_logEnabled) {
			bool ret = m_text.hasData();
			if (ret) {
				wxString text = m_text.getData();
				writeTextFile(m_user, text);
			}
		}
	}
}

void CXReflectorReflectorHandler::sendToReflectors(const CHeaderData& header)
{
	CHeaderData temp(header);

	// Send the header to the reflectors
	temp.setCQCQCQ();
	temp.setFlag1(0x00);
	temp.setFlag2(0x00);
	temp.setFlag3(0x00);

	// Incoming DPlus links
	temp.setRepeaters(m_gateway, m_callsign);
	CXReflectorDPlusHandler::writeHeader(temp);

	// Incoming DExtra links have RPT1 and RPT2 swapped
	temp.setRepeaters(m_gateway, m_callsign);
	CXReflectorDExtraHandler::writeHeader(m_callsign, temp);
}

void CXReflectorReflectorHandler::sendToReflectors(const CAMBEData& data)
{
	CAMBEData temp(data);

	CXReflectorDExtraHandler::writeAMBE(temp);
	CXReflectorDPlusHandler::writeAMBE(temp);
}

void CXReflectorReflectorHandler::sendToDongles(const CHeaderData& header)
{
	CHeaderData temp(header);

	temp.setCQCQCQ();
	temp.setFlag1(0x00);
	temp.setFlag2(0x00);
	temp.setFlag3(0x00);

	// DExtra Dongles have RPT1 and RPT2 swapped
	temp.setRepeaters(m_gateway, m_callsign);
	CXReflectorDExtraHandler::writeDongleHeader(temp);
}

void CXReflectorReflectorHandler::sendToDongles(const CAMBEData& data)
{
	CAMBEData temp(data);

	CXReflectorDExtraHandler::writeDongleAMBE(temp);
}

void CXReflectorReflectorHandler::clockInt(unsigned long ms)
{
	m_inactivityTimer.clock(ms);

	if (m_inactivityTimer.isRunning() && m_inactivityTimer.hasExpired()) {
		wxLogMessage(wxT("Reflector inactivity timer expired"));

		m_id = 0x00;
		m_inactivityTimer.stop();
	}
}

bool CXReflectorReflectorHandler::openTextFile(const wxString& directory)
{
	wxString fullName = TEXT_BASE_NAME;

	if (!m_name.IsEmpty()) {
		fullName.Append(wxT("_"));
		fullName.Append(m_name);
	}

	wxFileName fileName(directory, fullName, wxT("log"));
	wxString name = fileName.GetFullPath();

	bool ret = m_file.Open(name, wxT("a+t"));
	if (!ret) {
		wxLogError(wxT("Cannot open %s file for appending"), name.c_str());
		return false;
	}

	return true;
}

void CXReflectorReflectorHandler::writeTextFile(const wxString& user, const wxString& text)
{
	wxString buffer;
	buffer.Printf(wxT("%s : %s\n"), user.c_str(), text.c_str());

	m_file.Write(buffer);
	m_file.Flush();
}

void CXReflectorReflectorHandler::closeTextFile()
{
	m_file.Close();
}
