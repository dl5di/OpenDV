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

#include "wav2ambe.h"

#include "AMBE3000Thread.h"
#include "DVDongleThread.h"
#include "DStarDefines.h"
#include "Version.h"

#include <wx/config.h>

const wxString APPLICATION_NAME = wxT("DVTool Writer");

const wxString KEY_DONGLE_TYPE    = wxT("/dongleType");
const wxString KEY_DONGLE_DEVICE  = wxT("/dongleDevice");
const wxString KEY_DONGLE_ADDRESS = wxT("/dongleAddress");
const wxString KEY_DONGLE_PORT    = wxT("/donglePort");

const DONGLE_TYPE  DEFAULT_DONGLE_TYPE    = DT_DVDONGLE;
const wxString     DEFAULT_DONGLE_DEVICE  = wxEmptyString;
const wxString     DEFAULT_DONGLE_ADDRESS = wxEmptyString;
const unsigned int DEFAULT_DONGLE_PORT    = 2460L;

int main(int argc, char** argv)
{
	if (argc < 3) {
		::fprintf(stderr, "Usage: wav2ambe <infile> <outfile>\n");
		return 1;
	}

	::wxInitialize();

	wxString inFile = wxString(argv[1U], wxConvLocal);
	wxString outFile = wxString(argv[2U], wxConvLocal);

	CWAVFileReader* reader = new CWAVFileReader(inFile, DSTAR_RADIO_BLOCK_SIZE);
	bool ret = reader->open();
	if (!ret) {
		delete reader;
		::wxUninitialize();
		::fprintf(stderr, "wav2ambe: unable to open the input file - %s\n", argv[1U]);
		return 1;
	}

	unsigned int sampleRate = reader->getSampleRate();
	if (sampleRate != 48000U) {
		delete reader;
		::wxUninitialize();
		::fprintf(stderr, "wav2ambe: invalid sample rate in the input file - %s\n", argv[1U]);
		return 1;
	}

	unsigned int channels = reader->getChannels();
	if (channels != 1U) {
		delete reader;
		::wxUninitialize();
		::fprintf(stderr, "wav2ambe: invalid number of channels in the input file - %s\n", argv[1U]);
		return 1;
	}

	CAMBEFileWriter* writer = new CAMBEFileWriter(outFile);
	ret = writer->open();
	if (!ret) {
		delete reader;
		delete writer;
		::wxUninitialize();
		::fprintf(stderr, "wav2ambe: unable to open the output file - %s\n", argv[2U]);
		return 1;
	}

#if defined(__WXMSW__)
	wxRegConfig profile(APPLICATION_NAME, VENDOR_NAME, wxEmptyString, wxEmptyString, wxCONFIG_USE_LOCAL_FILE);
#else
	wxFileConfig profile(APPLICATION_NAME, VENDOR_NAME, wxEmptyString, wxEmptyString, wxCONFIG_USE_LOCAL_FILE);
#endif

	long temp;
	profile.Read(KEY_DONGLE_TYPE, &temp, long(DEFAULT_DONGLE_TYPE));
	DONGLE_TYPE type = DONGLE_TYPE(temp);

	wxString device;
	profile.Read(KEY_DONGLE_DEVICE, &device, DEFAULT_DONGLE_DEVICE);

	wxString address;
	profile.Read(KEY_DONGLE_ADDRESS, &address, DEFAULT_DONGLE_ADDRESS);

	unsigned int port;
	profile.Read(KEY_DONGLE_PORT, &temp, long(DEFAULT_DONGLE_PORT));
	port = (unsigned int)temp;

	CDongleThread* dongle = NULL;

	switch (type) {
		case DT_DVDONGLE:
			if (!device.IsEmpty())
				dongle = new CDVDongleThread(new CDVDongleController(device));
			break;
		case DT_DV3000:
			if (!address.IsEmpty() && port > 0U)
				dongle = new CAMBE3000Thread(new CDV3000Controller(address, port));
			break;
		default:
			delete reader;
			delete writer;
			::wxUninitialize();
			::fprintf(stderr, "wav2ambe: invalid Dongle type specified\n");
			return 1;
	}

	wav2ambe program(dongle, reader, writer);

	dongle->setEncodeCallback(&program);
	dongle->setBleep(false);

	ret = dongle->open();
	if (!ret) {
		delete reader;
		delete writer;
		delete dongle;
		::wxUninitialize();
		::fprintf(stderr, "wav2ambe: couldn't find the Dongle on the port specified\n");
		return 1;
	}

	program.run();

	::wxUninitialize();

	return 0;
}

wav2ambe::wav2ambe(CDongleThread* dongle, CWAVFileReader* reader, CAMBEFileWriter* writer) :
m_dongle(dongle),
m_reader(reader),
m_writer(writer)
{
	wxASSERT(dongle != NULL);
	wxASSERT(reader != NULL);
	wxASSERT(writer != NULL);
}

wav2ambe::~wav2ambe()
{
}

void wav2ambe::encodeCallback(const unsigned char* ambe, unsigned int length)
{
	if (m_writer == NULL)
		return;

	m_writer->writeFrame(ambe, length);
}

void wav2ambe::run()
{
	m_dongle->Create();
	m_dongle->SetPriority(100U);
	m_dongle->Run();

	m_dongle->setEncode();

	for (;;) {
		unsigned int space = m_dongle->getEncodeSpace();
		if (space >= DSTAR_RADIO_BLOCK_SIZE) {
			wxFloat32 audio[DSTAR_RADIO_BLOCK_SIZE];
			::memset(audio, 0x00U, DSTAR_RADIO_BLOCK_SIZE * sizeof(wxFloat32));
			unsigned int n = m_reader->read(audio, DSTAR_RADIO_BLOCK_SIZE);

			m_dongle->writeEncode(audio, DSTAR_RADIO_BLOCK_SIZE);

			// End of file?
			if (n != DSTAR_RADIO_BLOCK_SIZE)
				break;
		}

		::wxMilliSleep(FRAME_TIME_MS);
	}

	::wxMilliSleep(FRAME_TIME_MS * 30U);

	// Release the input file
	m_reader->close();
	delete m_reader;
	m_reader = NULL;

	// Release the output file
	m_writer->close();
	delete m_writer;
	m_writer = NULL;

	m_dongle->setIdle();
	m_dongle->kill();
}
