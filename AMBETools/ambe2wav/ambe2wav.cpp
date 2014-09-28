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

#include "ambe2wav.h"

#include "AMBE3000Thread.h"
#include "DVDongleThread.h"
#include "DStarDefines.h"
#include "HeaderData.h"
#include "Version.h"

#include <wx/config.h>

const wxString APPLICATION_NAME = wxT("DVTool Reader");

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
		::fprintf(stderr, "Usage: ambe2wav <infile> <outfile>\n");
		return 1;
	}

	::wxInitialize();

	wxString inFile  = wxString(argv[1U], wxConvLocal);
	wxString outFile = wxString(argv[2U], wxConvLocal);

	CAMBEFileReader* reader = new CAMBEFileReader;
	bool ret = reader->open(inFile);
	if (!ret) {
		delete reader;
		::wxUninitialize();
		::fprintf(stderr, "ambe2wav: unable to open the input file - %s\n", argv[1U]);
		return 1;
	}

	CWAVFileWriter* writer = new CWAVFileWriter(outFile, DSTAR_RADIO_SAMPLE_RATE, 1U, 16U, DSTAR_RADIO_BLOCK_SIZE);
	ret = writer->open();
	if (!ret) {
		delete reader;
		delete writer;
		::wxUninitialize();
		::fprintf(stderr, "ambe2wav: unable to open the output file - %s\n", argv[2U]);
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
			::fprintf(stderr, "ambe2wav: invalid Dongle type specified\n");
			return 1;
	}

	ambe2wav program(dongle, reader, writer);

	dongle->setDecodeCallback(&program);
	dongle->setBleep(false);

	ret = dongle->open();
	if (!ret) {
		delete reader;
		delete writer;
		delete dongle;
		::wxUninitialize();
		::fprintf(stderr, "ambe2wav: couldn't find the Dongle on the port specified\n");
		return 1;
	}

	program.run();

	::wxUninitialize();

	return 0;
}

ambe2wav::ambe2wav(CDongleThread* dongle, CAMBEFileReader* reader, CWAVFileWriter* writer) :
m_dongle(dongle),
m_reader(reader),
m_writer(writer)
{
	wxASSERT(dongle != NULL);
	wxASSERT(reader != NULL);
	wxASSERT(writer != NULL);
}

ambe2wav::~ambe2wav()
{
}

void ambe2wav::decodeCallback(const wxFloat32* audio, unsigned int length)
{
	if (m_writer == NULL)
		return;

	m_writer->write(audio, length);
}

void ambe2wav::run()
{
	m_dongle->Create();
	m_dongle->SetPriority(100U);
	m_dongle->Run();

	m_dongle->setDecode();

	for (;;) {
		unsigned int space = m_dongle->getDecodeSpace();
		if (space >= VOICE_FRAME_LENGTH_BYTES) {
			DVTFR_TYPE type;
			unsigned char frame[RADIO_HEADER_LENGTH_BYTES];
			unsigned int n = m_reader->read(frame, RADIO_HEADER_LENGTH_BYTES, type);

			// End of file?
			if (n == 0U || type != DVTFR_DETAIL)
				break;

			if (n != DV_FRAME_LENGTH_BYTES && n != VOICE_FRAME_LENGTH_BYTES)
				break;

			m_dongle->writeDecode(frame, VOICE_FRAME_LENGTH_BYTES);
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
