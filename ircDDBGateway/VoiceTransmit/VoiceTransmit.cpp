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

#include "DStarDefines.h"
#include "VoiceTransmit.h"

int main(int argc, char** argv)
{
	bool res = ::wxInitialize();
	if (!res) {
		::fprintf(stderr, "voicetransmit: failed to initialise the wxWidgets library, exiting\n");
		return 1;
	}

	if (argc < 3) {
		::fprintf(stderr, "voicetransmit: invalid command line usage: voicetransmit <repeater> <file1> <file2> ..., exiting\n");
		::wxUninitialize();
		return 1;
	}

	wxString repeater = wxString(argv[1], wxConvLocal);
	repeater.Replace(wxT("_"), wxT(" "));
	repeater.resize(LONG_CALLSIGN_LENGTH, wxT(' '));
	repeater.MakeUpper();

	wxArrayString filenames;
	for (int i = 2; i < argc; i++)
		filenames.Add(wxString(argv[i], wxConvLocal));

	CVoiceStore store(filenames);
	bool opened = store.open();
	if (!opened) {
		::fprintf(stderr, "voicetransmit: unable to open one of the files, exiting\n");
		::wxUninitialize();
		return 1;
	}

	CVoiceTransmit tt(repeater, &store);
	bool ret = tt.run();

	store.close();

	::wxUninitialize();

	return ret ? 0 : 1;
}

CVoiceTransmit::CVoiceTransmit(const wxString& callsign, CVoiceStore* store) :
m_socket(wxEmptyString, 0U),
m_callsign(callsign),
m_store(store)
{
	wxASSERT(store != NULL);
}

CVoiceTransmit::~CVoiceTransmit()
{
}

bool CVoiceTransmit::run()
{
	bool opened = m_socket.open();
	if (!opened)
		return false;

	in_addr address = CUDPReaderWriter::lookup(wxT("127.0.0.1"));

	unsigned int id = CHeaderData::createId();

	wxString callsignG = m_callsign.Left(LONG_CALLSIGN_LENGTH - 1U);
	callsignG.Append(wxT("G"));

	CHeaderData* header = m_store->getHeader();
	if (header == NULL) {
		m_socket.close();
		return false;
	}

	header->setId(id);
	header->setRptCall1(callsignG);
	header->setRptCall2(m_callsign);
	header->setDestination(address, G2_DV_PORT);

	sendHeader(header);

	delete header;

	wxStopWatch timer;
	timer.Start();

	unsigned int out   = 0U;
	unsigned int seqNo = 0U;

	for (;;) {
		unsigned int needed = timer.Time() / DSTAR_FRAME_TIME_MS;

		while (out < needed) {
			CAMBEData* ambe = m_store->getAMBE();

			if (ambe == NULL) {
				seqNo++;
				if (seqNo >= 21U)
					seqNo = 0U;

				CAMBEData data;
				data.setData(END_PATTERN_BYTES, DV_FRAME_LENGTH_BYTES);
				data.setDestination(address, G2_DV_PORT);
				data.setId(id);
				data.setSeq(seqNo);
				data.setEnd(true);

				sendData(&data);

				m_socket.close();

				return true;
			}

			seqNo = ambe->getSeq();

			ambe->setDestination(address, G2_DV_PORT);
			ambe->setEnd(false);
			ambe->setId(id);

			sendData(ambe);

			delete ambe;

			out++;
		}

		::wxMilliSleep(10UL);
	}
}

bool CVoiceTransmit::sendHeader(CHeaderData* header)
{
	wxASSERT(header != NULL);

	unsigned char buffer[60U];
	unsigned int length = header->getG2Data(buffer, 60U, true);

	for (unsigned int i = 0U; i < 2U; i++) {
		bool res = m_socket.write(buffer, length, header->getYourAddress(), header->getYourPort());
		if (!res)
			return false;
	}

	return true;
}

bool CVoiceTransmit::sendData(CAMBEData* data)
{
	wxASSERT(data != NULL);

	unsigned char buffer[40U];
	unsigned int length = data->getG2Data(buffer, 40U);

	return m_socket.write(buffer, length, data->getYourAddress(), data->getYourPort());
}
