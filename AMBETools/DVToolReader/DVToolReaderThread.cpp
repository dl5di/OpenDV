/*
 *   Copyright (C) 2009,2010,2012,2014 by Jonathan Naylor G4KLX
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

#include "DVToolReaderThread.h"

#include "DVToolReaderApp.h"
#include "MessageData.h"
#include "HeaderData.h"

CDVToolReaderThread::CDVToolReaderThread() :
wxThread(wxTHREAD_JOINABLE),
m_reader(NULL),
m_soundcard(NULL),
m_dongle(NULL),
m_slowDataDecoder(),
m_stopped(true),
m_audioBuffer(DSTAR_AUDIO_BLOCK_SIZE * 30U),
m_killed(false)
{
}

CDVToolReaderThread::~CDVToolReaderThread()
{
}

void* CDVToolReaderThread::Entry()
{
	// Wait here until we have the essentials to run
	while (!m_killed && (m_dongle == NULL || m_soundcard == NULL))
		Sleep(500UL);		// 1/2 sec

	if (m_killed)
		return NULL;

	m_dongle->Create();
	m_dongle->SetPriority(100U);
	m_dongle->Run();

	while (!m_killed) {
		if (m_reader != NULL)
			processFile();

		Sleep(500UL);		// 1/2 sec
	}

	m_soundcard->close();
	delete m_soundcard;

	m_dongle->kill();

	return NULL;
}

void CDVToolReaderThread::kill()
{
	m_killed = true;
}

void CDVToolReaderThread::decodeCallback(const wxFloat32* audio, unsigned int length)
{
	m_audioBuffer.addData(audio, length);
}

bool CDVToolReaderThread::setReader(IFileReader* reader)
{
	wxASSERT(reader != NULL);

	wxLogMessage(wxT("Processing file - %s"), reader->getFileName().c_str());

	if (!m_stopped) {
		wxLogError(wxT("Already processing a file"));
		return false;
	}

	if (m_dongle == NULL || m_soundcard == NULL) {
		wxLogError(wxT("No Dongle and/or Soundcard has been set"));
		return false;
	}

	if (m_reader != NULL) {
		m_reader->close();
		delete m_reader;
	}

	m_reader = reader;

	return true;
}


void CDVToolReaderThread::setSoundCard(CSoundCardReaderWriter* soundcard)
{
	wxASSERT(soundcard != NULL);

	if (!m_stopped) {
		soundcard->close();
		delete soundcard;
		return;
	}

	if (m_soundcard != NULL) {
		m_soundcard->close();
		delete m_soundcard;
	}

	m_soundcard = soundcard;
}

void CDVToolReaderThread::setDongle(CDongleThread* dongle)
{
	wxASSERT(dongle != NULL);

	if (!m_stopped) {
		dongle->kill();
		return;
	}

	if (m_dongle != NULL)
		m_dongle->kill();

	m_dongle = dongle;
}

void CDVToolReaderThread::setBleep(bool on)
{
	if (m_dongle != NULL)
		m_dongle->setBleep(on);
}

void CDVToolReaderThread::processFile()
{
	m_stopped = false;

	DVTFR_TYPE type;
	unsigned char header[RADIO_HEADER_LENGTH_BYTES];
	unsigned int n = m_reader->read(header, RADIO_HEADER_LENGTH_BYTES, type);
	if (n == 0U) {
		// Release the input file
		m_reader->close();
		delete m_reader;
		m_reader = NULL;
		return;
	}

	if (type == DVTFR_HEADER)
		processHeader(header);

	// Clear any state that may be hanging around
	m_slowDataDecoder.reset();

	m_dongle->setDecode();

	for (;;) {
		if (m_killed) {
			m_dongle->setIdle();
			m_reader->close();
			delete m_reader;
			return;
		}

		unsigned int space = m_dongle->getDecodeSpace();
		if (space >= VOICE_FRAME_LENGTH_BYTES) {
			DVTFR_TYPE type;
			unsigned char frame[RADIO_HEADER_LENGTH_BYTES];
			unsigned int n = m_reader->read(frame, RADIO_HEADER_LENGTH_BYTES, type);

			// End of file?
			if (n == 0U || type != DVTFR_DETAIL)
				break;

			if (n != DV_FRAME_LENGTH_BYTES && n != VOICE_FRAME_LENGTH_BYTES) {
				wxLogError(wxT("Invalid frame size of %u in the file"), n);
				::wxGetApp().error(_("Invalid frame size in the file."));
				break;
			}

			// Only process slow data if we have any
			if (n == DV_FRAME_LENGTH_BYTES) {
				// Don't pass data re-sync to the slow data processor
				if (::memcmp(frame + VOICE_FRAME_LENGTH_BYTES, DATA_SYNC_BYTES, DATA_FRAME_LENGTH_BYTES) != 0)
					processSlowData(frame + VOICE_FRAME_LENGTH_BYTES);
				else
					m_slowDataDecoder.sync();
			}

			m_dongle->writeDecode(frame, VOICE_FRAME_LENGTH_BYTES);
		}

		Sleep(FRAME_TIME_MS);
	}

	Sleep(FRAME_TIME_MS * 30U);

	// Clear the GUI
	::wxGetApp().showHeader(NULL);

	// Release the input file
	m_reader->close();
	delete m_reader;
	m_reader = NULL;

	wxLogMessage(wxT("Finished processing the file"));

	m_stopped = true;

	m_dongle->setIdle();
}

void CDVToolReaderThread::processHeader(const unsigned char* buffer)
{
	// Tell the GUI about the header, we assume that it's valid
	CHeaderData* header = new CHeaderData(buffer, RADIO_HEADER_LENGTH_BYTES, false);

	wxLogMessage(wxT("Header decoded - My: %s/%s  Your: %s  Rpt1: %s  Rpt2: %s  Flags: %02X %02X %02X"), header->getMyCall1().c_str(), header->getMyCall2().c_str(), header->getYourCall().c_str(), header->getRptCall1().c_str(), header->getRptCall2().c_str(), header->getFlag1(), header->getFlag2(), header->getFlag3());

	::wxGetApp().showHeader(header);
}

void CDVToolReaderThread::processSlowData(const unsigned char* buffer)
{
	m_slowDataDecoder.addData(buffer);

	CMessageData* message = m_slowDataDecoder.getMessageData();
	if (message != NULL)
		::wxGetApp().showMessage(message);
}

void CDVToolReaderThread::callback(const wxFloat32* input, wxFloat32* output, unsigned int nSamples, int id)
{
	::memset(output, 0x00, nSamples * sizeof(wxFloat32));
	m_audioBuffer.getData(output, nSamples);
}
