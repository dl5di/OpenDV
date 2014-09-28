/*
 *   Copyright (C) 2014 by Jonathan Naylor G4KLX
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

#include "SlowDataEncoder.h"
#include "DStarDefines.h"
#include "TextTransmit.h"

#include <wx/textfile.h>
#include <wx/cmdline.h>

const wxChar* REPEATER_PARAM = wxT("Repeater");
const wxChar* FILE_OPTION    = wxT("file");
const wxChar* TEXT_OPTION    = wxT("text");

int main(int argc, char** argv)
{
	bool res = ::wxInitialize();
	if (!res) {
		::fprintf(stderr, "texttransmit: failed to initialise the wxWidgets library, exiting\n");
		return 1;
	}

	wxCmdLineParser parser(argc, argv);
	parser.AddParam(REPEATER_PARAM, wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL);
	parser.AddOption(TEXT_OPTION, wxEmptyString, wxEmptyString, wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL);
	parser.AddOption(FILE_OPTION, wxEmptyString, wxEmptyString, wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL);

	int cmd = parser.Parse();
	if (cmd != 0) {
		::wxUninitialize();
		return 1;
	}

	if (parser.GetParamCount() < 1U) {
		::fprintf(stderr, "texttransmit: invalid command line usage: texttransmit <repeater> -text <text>|-file <filename>, exiting\n");
		::wxUninitialize();
		return 1;
	}

	wxString text;
	bool textFound = parser.Found(TEXT_OPTION, &text);

	wxString filename;
	bool fileFound = parser.Found(FILE_OPTION, &filename);

	if (!textFound && !fileFound) {
		::fprintf(stderr, "texttransmit: invalid command line usage: texttransmit <repeater> -text <text>|-file <filename>, exiting\n");
		::wxUninitialize();
		return 1;
	}

	if (textFound && fileFound) {
		::fprintf(stderr, "texttransmit: invalid command line usage: texttransmit <repeater> -text <text>|-file <filename>, exiting\n");
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
			::fprintf(stderr, "texttransmit: unable to open the file, exiting\n");
			::wxUninitialize();
			return 1;
		}

		text = file.GetFirstLine();

		file.Close();
	}

	text.resize(20U, wxT(' '));

	CTextTransmit tt(repeater, text);
	bool ret = tt.run();

	::wxUninitialize();

	return ret ? 0 : 1;
}

CTextTransmit::CTextTransmit(const wxString& callsign, const wxString& text) :
m_socket(wxEmptyString, 0U),
m_callsign(callsign),
m_text(text)
{
}

CTextTransmit::~CTextTransmit()
{
}

bool CTextTransmit::run()
{
	bool opened = m_socket.open();
	if (!opened)
		return false;

	in_addr address = CUDPReaderWriter::lookup(wxT("127.0.0.1"));

	unsigned int id = CHeaderData::createId();

	wxString callsignG = m_callsign.Left(LONG_CALLSIGN_LENGTH - 1U);
	callsignG.Append(wxT("G"));

	CHeaderData header;
	header.setId(id);
	header.setMyCall1(m_callsign);
	header.setMyCall2(wxT("INFO"));
	header.setRptCall1(callsignG);
	header.setRptCall2(m_callsign);
	header.setYourCall(wxT("CQCQCQ  "));
	header.setDestination(address, G2_DV_PORT);

	sendHeader(header);

	CSlowDataEncoder encoder;
	encoder.setHeaderData(header);
	encoder.setTextData(m_text);

	CAMBEData data;
	data.setDestination(address, G2_DV_PORT);
	data.setId(id);

	wxStopWatch timer;
	timer.Start();

	unsigned int out = 0U;

	for (;;) {
		unsigned int needed = timer.Time() / DSTAR_FRAME_TIME_MS;

		while (out < needed) {
			data.setSeq(out);

			unsigned char buffer[DV_FRAME_LENGTH_BYTES];
			::memcpy(buffer + 0U, NULL_AMBE_DATA_BYTES, VOICE_FRAME_LENGTH_BYTES);

			// Insert sync bytes when the sequence number is zero, slow data otherwise
			if (out == 0U) {
				::memcpy(buffer + VOICE_FRAME_LENGTH_BYTES, DATA_SYNC_BYTES, DATA_FRAME_LENGTH_BYTES);
				encoder.sync();
			} else {
				encoder.getTextData(buffer + VOICE_FRAME_LENGTH_BYTES);
			}

			data.setData(buffer, DV_FRAME_LENGTH_BYTES);

			sendData(data);
			out++;

			if (out == 21U) {
				data.setData(END_PATTERN_BYTES, DV_FRAME_LENGTH_BYTES);
				data.setSeq(0U);
				data.setEnd(true);

				sendData(data);

				m_socket.close();

				return true;
			}
		}

		::wxMilliSleep(10UL);
	}
}

bool CTextTransmit::sendHeader(const CHeaderData& header)
{
	unsigned char buffer[60U];
	unsigned int length = header.getG2Data(buffer, 60U, true);

	for (unsigned int i = 0U; i < 2U; i++) {
		bool res = m_socket.write(buffer, length, header.getYourAddress(), header.getYourPort());
		if (!res)
			return false;
	}

	return true;
}

bool CTextTransmit::sendData(const CAMBEData& data)
{
	unsigned char buffer[40U];
	unsigned int length = data.getG2Data(buffer, 40U);

	return m_socket.write(buffer, length, data.getYourAddress(), data.getYourPort());
}
