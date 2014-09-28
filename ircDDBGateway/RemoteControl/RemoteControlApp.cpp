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

#include "RemoteControlDefs.h"
#include "RemoteControlApp.h"
#include "Version.h"

#include <wx/config.h>
#include <wx/cmdline.h>

IMPLEMENT_APP(CRemoteControlApp)

const wxChar* NAME_PARAM = wxT("Name");

CRemoteControlApp::CRemoteControlApp() :
wxApp(),
m_name(),
m_frame(NULL),
m_config(NULL)
{
}

CRemoteControlApp::~CRemoteControlApp()
{
}

bool CRemoteControlApp::OnInit()
{
	SetVendorName(VENDOR_NAME);

	if (!wxApp::OnInit())
		return false;

	m_config = new CRemoteControlConfig(new wxConfig(APPLICATION_NAME), m_name);

	wxString frameName = APPLICATION_NAME + wxT(" - ");
	if (!m_name.IsEmpty()) {
		frameName.Append(m_name);
		frameName.Append(wxT(" - "));
	}
	frameName.Append(VERSION);

	wxPoint position = wxDefaultPosition;

	int x, y;
	getPosition(x, y);
	if (x >= 0 && y >= 0)
		position = wxPoint(x, y);

	m_frame = new CRemoteControlFrame(frameName, position);
	m_frame->Show();

	SetTopWindow(m_frame);

	return wxApp::OnInit();
}

int CRemoteControlApp::OnExit()
{
	delete m_config;

	return 0;
}

void CRemoteControlApp::OnInitCmdLine(wxCmdLineParser& parser)
{
	parser.AddParam(NAME_PARAM, wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL);

	wxApp::OnInitCmdLine(parser);
}

bool CRemoteControlApp::OnCmdLineParsed(wxCmdLineParser& parser)
{
	if (!wxApp::OnCmdLineParsed(parser))
		return false;

	if (parser.GetParamCount() > 0U)
		m_name = parser.GetParam(0U);

	return true;
}

void CRemoteControlApp::getConfig(wxString& address, unsigned int& port, wxString& password) const
{
	m_config->getConfig(address, port, password);
}

void CRemoteControlApp::setConfig(const wxString& address, unsigned int port, const wxString& password) const
{
	m_config->setConfig(address, port, password);
}

void CRemoteControlApp::getPosition(int& x, int& y) const
{
	m_config->getPosition(x, y);
}

void CRemoteControlApp::setPosition(int x, int y)
{
	m_config->setPosition(x, y);
}

void CRemoteControlApp::repeaterRefresh(const wxString& callsign)
{
	m_frame->repeaterRefresh(callsign);
}

void CRemoteControlApp::starNetRefresh(const wxString& callsign)
{
	m_frame->starNetRefresh(callsign);
}

void CRemoteControlApp::link(const wxString& callsign, RECONNECT reconnect, const wxString& reflector)
{
	m_frame->link(callsign, reconnect, reflector);
}

void CRemoteControlApp::unlink(const wxString& callsign, PROTOCOL protocol, const wxString& reflector)
{
	m_frame->unlink(callsign, protocol, reflector);
}

void CRemoteControlApp::starNetLogoff(const wxString& callsign, const wxString& user)
{
	m_frame->starNetLogoff(callsign, user);
}
