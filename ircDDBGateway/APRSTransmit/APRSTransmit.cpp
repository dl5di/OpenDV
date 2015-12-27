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

#include "SlowDataEncoder.h"
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

CAPRSTransmit::CAPRSTransmit(const wxString& callsign, const wxString& text) :
m_socket(wxEmptyString, 0U),
m_repeaterCallsign(callsign),
m_APRSCallsign(callsign),
m_text(text)
{
	int index = m_text.Find(wxT(">"));
	if(index != wxNOT_FOUND)
		m_APRSCallsign = m_text.Left(index);

	//add nececessary stuff to text
	m_text.Append(wxT("\r"));
	wxString crc;
	crc.Printf(wxT("$$CRC%04X,"), calcCRC(m_text));
	m_text.Prepend(crc);
	//std::cout << m_text.mb_str() << std::endl;
}

CAPRSTransmit::~CAPRSTransmit()
{
}

bool CAPRSTransmit::run()
{
	bool opened = m_socket.open();
	if (!opened)
		return false;

	in_addr address = CUDPReaderWriter::lookup(wxT("127.0.0.1"));

	unsigned int id = CHeaderData::createId();

	wxString callsignG = m_repeaterCallsign.Left(LONG_CALLSIGN_LENGTH - 1U);
	callsignG.Append(wxT("G"));

	CHeaderData header;
	header.setId(id);
	header.setMyCall1(m_APRSCallsign);
	header.setMyCall2(wxT("APRS"));
	header.setRptCall1(callsignG);
	header.setRptCall2(m_repeaterCallsign);
	header.setYourCall(wxT("CQCQCQ  "));
	header.setDestination(address, G2_DV_PORT);

	sendHeader(header);

	CSlowDataEncoder encoder;
	encoder.setHeaderData(header);
	encoder.setGPSData(m_text);
	encoder.setTextData(wxT("APRS to DPRS"));

	CAMBEData data;
	data.setDestination(address, G2_DV_PORT);
	data.setId(id);

	wxStopWatch timer;
	timer.Start();

	unsigned int out = 0U;
        unsigned int dataOut = 0U;
	unsigned int needed = (encoder.getInterleavedDataLength() / (DATA_FRAME_LENGTH_BYTES)) * 2U;

	while (dataOut < needed) {
		data.setSeq(out);

		unsigned char buffer[DV_FRAME_LENGTH_BYTES];
		::memcpy(buffer + 0U, NULL_AMBE_DATA_BYTES, VOICE_FRAME_LENGTH_BYTES);

		// Insert sync bytes when the sequence number is zero, slow data otherwise
		if (out == 0U) {
			::memcpy(buffer + VOICE_FRAME_LENGTH_BYTES, DATA_SYNC_BYTES, DATA_FRAME_LENGTH_BYTES);
			//encoder.sync();
		} else {
			encoder.getInterleavedData(buffer + VOICE_FRAME_LENGTH_BYTES);		
			dataOut++;
		}

		data.setData(buffer, DV_FRAME_LENGTH_BYTES);

		sendData(data);
		out++;

		if (out == 21U) out = 0U;
	}

	data.setData(END_PATTERN_BYTES, DV_FRAME_LENGTH_BYTES);
	data.setSeq(out >= 21U ? 0U : out);
	data.setEnd(true);

	sendData(data);

	m_socket.close();

	return true;
}

bool CAPRSTransmit::sendHeader(const CHeaderData& header)
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

bool CAPRSTransmit::sendData(const CAMBEData& data)
{
	unsigned char buffer[60U];
	unsigned int length = data.getG2Data(buffer, 60U);

	return m_socket.write(buffer, length, data.getYourAddress(), data.getYourPort());
}

unsigned int CAPRSTransmit::calcCRC(const wxString& gpsData)
{ 
	wxASSERT(gpsDataconst != NULL);
	size_t length = gpsData.length();
	wxASSERT(length > 0U);

	unsigned int icomcrc = 0xFFFFU;

	for (unsigned int j = 0U; j < length; j++) {
		unsigned char ch = gpsData.GetChar(j);

		for (unsigned int i = 0U; i < 8U; i++) {
			bool xorflag = (((icomcrc ^ ch) & 0x01U) == 0x01U);

			icomcrc >>= 1;

			if (xorflag)
				icomcrc ^= 0x8408U;

			ch >>= 1;
		} 
	}

	return ~icomcrc & 0xFFFFU;
}
