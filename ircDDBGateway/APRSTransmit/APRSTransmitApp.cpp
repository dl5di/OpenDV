/*
 *   Copyright (C) 2014 by Jonathan Naylor G4KLX
 *   APRSTransmit Copyright (C) 2015 Geoffrey Merck F4FXL / KC3FRA
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
#include "APRSTransmit.h"

#include <wx/textfile.h>
#include <wx/cmdline.h>

const wxChar* REPEATER_PARAM = wxT("Repeater");
const wxChar* FILE_OPTION    = wxT("file");
const wxChar* APRS_OPTION    = wxT("aprs");

int main(int argc, char** argv)
{
	bool res = ::wxInitialize();
	if (!res) {
		::fprintf(stderr, "aprstransmit: failed to initialise the wxWidgets library, exiting\n");
		return 1;
	}

	wxCmdLineParser parser(argc, argv);
	parser.AddParam(REPEATER_PARAM, wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL);
	parser.AddOption(APRS_OPTION, wxEmptyString, wxEmptyString, wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL);
	parser.AddOption(FILE_OPTION, wxEmptyString, wxEmptyString, wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL);

	int cmd = parser.Parse();
	if (cmd != 0) {
		::wxUninitialize();
		return 1;
	}

	if (parser.GetParamCount() < 1U) {
		::fprintf(stderr, "aprstransmit: invalid command line usage: aprstransmit <repeater> -aprs <aprs_tnc2_string>|-file <filename>, exiting\n");
		::wxUninitialize();
		return 1;
	}

	wxString text;
	bool aprsFound = parser.Found(APRS_OPTION, &text);

	wxString filename;
	bool fileFound = parser.Found(FILE_OPTION, &filename);

	if (!aprsFound && !fileFound) {
		::fprintf(stderr, "aprstransmit: invalid command line usage: aprstransmit <repeater> -aprs <text>|-file <filename>, exiting\n");
		::wxUninitialize();
		return 1;
	}

	if (aprsFound && fileFound) {
		::fprintf(stderr, "aprstransmit: invalid command line usage: aprstransmit <repeater> -aprs <text>|-file <filename>, exiting\n");
		::wxUninitialize();
		return 1;
	}

	wxString repeater = parser.GetParam(0U);
	repeater.Replace(wxT("_"), wxT(" "));
	repeater.resize(LONG_CALLSIGN_LENGTH, wxT(' '));
	repeater.MakeUpper();

	if (fileFound) {
		wxTextFile file;
		bool found = file.Open(filename);
		if (!found) {
			::fprintf(stderr, "aprstransmit: unable to open the file, exiting\n");
			::wxUninitialize();
			return 1;
		}

		text = file.GetFirstLine();

		file.Close();
	}

	
	CAPRSTransmit tt(repeater, text);
	bool ret = tt.run();

	::wxUninitialize();

	return ret ? 0 : 1;
}

