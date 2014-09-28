/*
 *   Copyright (C) 2010-2014 by Jonathan Naylor G4KLX
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

#include "IRCDDBGatewayConfigDefs.h"
#include "IRCDDBGatewayConfigApp.h"
#include "Version.h"
#include "Utils.h"

#include <wx/config.h>
#include <wx/cmdline.h>
#include <wx/filename.h>

IMPLEMENT_APP(CIRCDDBGatewayConfigApp)

const wxChar*     NAME_PARAM = wxT("Gateway Name");
const wxChar* CONFDIR_OPTION = wxT("confdir");

CIRCDDBGatewayConfigApp::CIRCDDBGatewayConfigApp() :
wxApp(),
m_name(),
m_confDir(),
m_frame(NULL)
{
}

CIRCDDBGatewayConfigApp::~CIRCDDBGatewayConfigApp()
{
}

bool CIRCDDBGatewayConfigApp::OnInit()
{
	SetVendorName(VENDOR_NAME);

	if (!wxApp::OnInit())
		return false;

	wxString frameName = APPLICATION_NAME + wxT(" - ");
	if (!m_name.IsEmpty()) {
		frameName.Append(m_name);
		frameName.Append(wxT(" - "));
	}
	frameName.Append(VERSION);

	m_frame = new CIRCDDBGatewayConfigFrame(frameName, m_confDir, m_name);
	m_frame->Show();

	SetTopWindow(m_frame);

	return true;
}

int CIRCDDBGatewayConfigApp::OnExit()
{
	return 0;
}

void CIRCDDBGatewayConfigApp::OnInitCmdLine(wxCmdLineParser& parser)
{
	parser.AddOption(CONFDIR_OPTION,   wxEmptyString, wxEmptyString, wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL);
	parser.AddParam(NAME_PARAM, wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL);

	wxApp::OnInitCmdLine(parser);
}

bool CIRCDDBGatewayConfigApp::OnCmdLineParsed(wxCmdLineParser& parser)
{
	if (!wxApp::OnCmdLineParsed(parser))
		return false;

	wxString confDir;
	bool found = parser.Found(CONFDIR_OPTION, &confDir);
	if (found)
		m_confDir = confDir;

	if (parser.GetParamCount() > 0U)
		m_name = parser.GetParam(0U);

	return true;
}
