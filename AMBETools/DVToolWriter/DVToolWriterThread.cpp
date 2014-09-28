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

#include "DVToolWriterThread.h"

#include "DVToolWriterApp.h"
#include "MessageData.h"
#include "HeaderData.h"

CDVToolWriterThread::CDVToolWriterThread() :
wxThread(wxTHREAD_JOINABLE),
m_soundcard(NULL),
m_dongle(NULL),
m_encoder(),
m_stopped(true),
m_reader(NULL),
m_writer(NULL),
m_audioBuffer(DSTAR_AUDIO_BLOCK_SIZE * 30U),
m_count(0U),
m_killed(false)
{
}

CDVToolWriterThread::~CDVToolWriterThread()
{
}

void* CDVToolWriterThread::Entry()
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
		if (m_writer != NULL && m_reader != NULL)
			processFile();

		if (m_writer != NULL && m_reader == NULL)
			processSoundcard();

		Sleep(500UL);		// 1/2 sec
	}

	m_soundcard->close();
	delete m_soundcard;

	m_dongle->kill();

	return NULL;
}

void CDVToolWriterThread::kill()
{
	m_killed = true;
}

void CDVToolWriterThread::encodeCallback(const unsigned char* ambe, unsigned int length)
{
	if (m_writer == NULL)
		return;

	unsigned char frame[DV_FRAME_LENGTH_BYTES];

	::memcpy(frame, ambe, VOICE_FRAME_LENGTH_BYTES);

	if (m_count == 0U)
		::memcpy(frame + VOICE_FRAME_LENGTH_BYTES, DATA_SYNC_BYTES, DATA_FRAME_LENGTH_BYTES);
	else
		m_encoder.getData(frame + VOICE_FRAME_LENGTH_BYTES);

	m_count++;
	if (m_count >= 21U)
		m_count = 0U;

	m_writer->writeFrame(frame, DV_FRAME_LENGTH_BYTES);
}

bool CDVToolWriterThread::processFile(CWAVFileReader* reader, CHeaderData* header, const wxString& text, IFileWriter* writer)
{
	wxASSERT(reader != NULL);
	wxASSERT(header != NULL);
	wxASSERT(writer != NULL);

	wxLogMessage(wxT("Processing file - %s"), reader->getFilename().c_str());

	if (!m_stopped) {
		wxLogError(wxT("Already processing a file"));
		return false;
	}

	if (m_dongle == NULL) {
		wxLogError(wxT("No Dongle has been set"));
		return false;
	}

	if (m_reader != NULL) {
		m_reader->close();
		delete m_reader;
	}

	if (m_writer != NULL) {
		m_writer->close();
		delete m_writer;
	}

	wxLogMessage(wxT("Writing file - %s"), writer->getFilename().c_str());

	bool ret = writer->open();
	if (!ret) {
		wxLogError(wxT("Cannot open the file for writing"));
		return false;
	}

	writer->writeHeader(*header);

	m_encoder.reset();
	m_encoder.setMessageData(text);
	m_encoder.setHeaderData(*header);

	delete header;

	m_reader = reader;
	m_writer = writer;

	return true;
}

bool CDVToolWriterThread::processAudio(CHeaderData* header, const wxString& text, IFileWriter* writer)
{
	wxASSERT(header != NULL);
	wxASSERT(writer != NULL);

	if (!m_stopped) {
		wxLogError(wxT("Already processing a file"));
		return false;
	}

	if (m_dongle == NULL) {
		wxLogError(wxT("No Dongle has been set"));
		return false;
	}

	if (m_writer != NULL) {
		m_writer->close();
		delete m_writer;
	}

	wxLogMessage(wxT("Writing file - %s"), writer->getFilename().c_str());

	bool ret = writer->open();
	if (!ret) {
		wxLogError(wxT("Cannot open the file for writing"));
		return false;
	}

	writer->writeHeader(*header);

	m_encoder.reset();
	m_encoder.setMessageData(text);
	m_encoder.setHeaderData(*header);

	delete header;

	m_writer = writer;

	return true;
}

void CDVToolWriterThread::processAudio()
{
	m_stopped = true;
}

void CDVToolWriterThread::setSoundCard(CSoundCardReaderWriter* soundcard)
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

void CDVToolWriterThread::setDongle(CDongleThread* dongle)
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

void CDVToolWriterThread::processFile()
{
	m_stopped = false;

	m_count = 0U;

	m_dongle->setEncode();

	for (;;) {
		if (m_killed) {
			m_dongle->setIdle();
			m_reader->close();
			m_writer->close();
			delete m_reader;
			delete m_writer;
			m_reader = NULL;
			m_writer = NULL;
			return;
		}

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

		Sleep(FRAME_TIME_MS);
	}

	Sleep(FRAME_TIME_MS * 30U);

	// Release the input file
	m_reader->close();
	delete m_reader;
	m_reader = NULL;

	// Release the output file
	m_writer->close();
	delete m_writer;
	m_writer = NULL;

	::wxGetApp().showFinished();

	wxLogMessage(wxT("Finished processing the file"));

	m_stopped = true;

	m_dongle->setIdle();
}

void CDVToolWriterThread::processSoundcard()
{
	m_stopped = false;

	m_count = 0U;

	m_dongle->setEncode();

	for (;;) {
		if (m_killed) {
			m_dongle->setIdle();
			m_writer->close();
			delete m_writer;
			m_writer = NULL;
			return;
		}

		if (m_stopped)
			break;

		unsigned int size = m_audioBuffer.dataSpace();
		if (size >= DSTAR_RADIO_BLOCK_SIZE) {
			wxFloat32 audio[DSTAR_RADIO_BLOCK_SIZE];
			unsigned int n = m_audioBuffer.getData(audio, DSTAR_RADIO_BLOCK_SIZE);
			m_dongle->writeEncode(audio, n);
		}

		Sleep(FRAME_TIME_MS / 4U);
	}

	Sleep(FRAME_TIME_MS * 30U);

	// Release the output file
	m_writer->close();
	delete m_writer;
	m_writer = NULL;

	wxLogMessage(wxT("Finished processing from the sound card"));

	m_stopped = true;

	m_dongle->setIdle();
}

void CDVToolWriterThread::callback(const wxFloat32* input, wxFloat32* output, unsigned int nSamples, int id)
{
	if (m_writer != NULL && m_reader == NULL)
		m_audioBuffer.addData(input, nSamples);
}
