/*
 *   Copyright (C) 2006-2010 by Jonathan Naylor G4KLX
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

//	ALSA Version by John Wiseman G8BPQ - initially for PI


#include "SoundCardReaderWriter.h"
#include "DStarDefines.h"
#include "DummyRepeaterThread.h"
#include <wx/wx.h> 

// Nasty Hacks!

snd_pcm_t *	playhandle = NULL;
snd_pcm_t *	rechandle = NULL;
CSoundCardReaderWriter* object = NULL;


CSoundCardReaderWriter::CSoundCardReaderWriter(const wxString& readDevice, const wxString& writeDevice, unsigned int sampleRate, unsigned int blockSize) :
m_readDevice(readDevice),
m_writeDevice(writeDevice),
m_sampleRate(sampleRate),
m_blockSize(blockSize),
m_callback(NULL),
m_id(-1),
m_playhandle(NULL),
m_rechandle(NULL),
m_hwparams(NULL),
m_playchannels(1),
m_recchannels(1),
m_killed(0),
m_started(0)
{ 
}

CSoundCardReaderWriter::~CSoundCardReaderWriter()
{
}

wxArrayString CSoundCardReaderWriter::getReadDevices()
{
	snd_ctl_t *handle= NULL;
	snd_pcm_t *pcm= NULL;
	snd_ctl_card_info_t *info;
	snd_pcm_info_t *pcminfo;
	snd_pcm_hw_params_t *pars;
	snd_pcm_format_mask_t *fmask;
	char NameString[256];

	wxArrayString devices;

	devices.Alloc(10);

	printf("getReadDevices\n");

	//	Get Device List from ALSA

	snd_ctl_card_info_alloca(&info);
	snd_pcm_info_alloca(&pcminfo);
	snd_pcm_hw_params_alloca(&pars);
	snd_pcm_format_mask_alloca(&fmask);

	char hwdev[80];
	unsigned min, max;
	int card, err, dev, nsubd;
	int thecard = -1, thedev = -1;
	snd_pcm_stream_t stream = SND_PCM_STREAM_CAPTURE;

	card= -1;

	if( snd_card_next(&card) < 0 )
	{
		printf("No Devices\n");
		return devices;
	}

	// Hacks!! - 
	
	if (rechandle)
		snd_pcm_close(rechandle);

	if (object)
		object->m_killed = TRUE;		// Stop the poll thread

	while( card >= 0 )
	{
		sprintf(hwdev, "hw:%d", card);
		err = snd_ctl_open(&handle, hwdev, 0);
//    if( sc_errcheck(err, "opening control interface", card, -1) ) goto nextcard;

		err = snd_ctl_card_info(handle, info);
  //  if( sc_errcheck(err, "obtaining card info", card, -1) ) {
  //    snd_ctl_close(handle);
  //    goto nextcard;
    

		printf("Card %d, ID `%s', name `%s'\n", card, snd_ctl_card_info_get_id(info),
                snd_ctl_card_info_get_name(info));

		dev= -1;

		if (snd_ctl_pcm_next_device(handle, &dev) < 0) {
			snd_ctl_close(handle);
			goto nextcard;
		}
    
		while( dev >= 0 )
		{
			snd_pcm_info_set_device(pcminfo, dev);
			snd_pcm_info_set_subdevice(pcminfo, 0);
			snd_pcm_info_set_stream(pcminfo, stream);
			err = snd_ctl_pcm_info(handle, pcminfo);
			if (thedev <0 && err == -ENOENT )
			{
			}
			else
			{
				nsubd = snd_pcm_info_get_subdevices_count(pcminfo);
				printf("  Device %d, ID `%s', name `%s', %d subdevices (%d available)\n",
					dev, snd_pcm_info_get_id(pcminfo), snd_pcm_info_get_name(pcminfo),
					nsubd, snd_pcm_info_get_subdevices_avail(pcminfo));

				sprintf(hwdev, "hw:%d,%d", card, dev);

				err = snd_pcm_open(&pcm, hwdev, stream, SND_PCM_NONBLOCK);
 
				if (err)
				{
					if (err == 16)
					{
						snd_pcm_close(rechandle);
						err = snd_pcm_open(&pcm, hwdev, stream, SND_PCM_NONBLOCK);
					}
					else
						printf("Error %d opening sound device", err);
				}

				err = snd_pcm_hw_params_any(pcm, pars);
				snd_pcm_hw_params_get_channels_min(pars, &min);
				snd_pcm_hw_params_get_channels_max(pars, &max);
				if( min == max )
					if( min == 1 )  printf("    1 channel, ");
					else            printf("    %d channels, ", min);
				else              printf("    %u..%u channels, ", min, max);
			
				snd_pcm_hw_params_get_rate_min(pars, &min, NULL);
				snd_pcm_hw_params_get_rate_max(pars, &max, NULL);
				printf("sampling rate %u..%u Hz\n", min, max);
	
				sprintf(NameString, "hw:%d,%d %s(%s)", card, dev,
					snd_pcm_info_get_name(pcminfo), snd_ctl_card_info_get_name(info));
				wxString name(NameString, wxConvLocal);
				devices.Add(name);

				snd_pcm_close(pcm);
				pcm= NULL;
			}
	
			if( thedev >= 0 || snd_ctl_pcm_next_device(handle, &dev) < 0 )
				break;
	    }
		snd_ctl_close(handle);

nextcard:

		if( thecard >= 0 || snd_card_next(&card) < 0 )
			break;
	}

	return devices;
}



wxArrayString CSoundCardReaderWriter::getWriteDevices()
{
	snd_ctl_t *handle= NULL;
	snd_pcm_t *pcm= NULL;
	snd_ctl_card_info_t *info;
	snd_pcm_info_t *pcminfo;
	snd_pcm_hw_params_t *pars;
	snd_pcm_format_mask_t *fmask;
	char NameString[256];

	wxArrayString devices;

	devices.Alloc(10);

	printf("getWriteDevices\n");

	//	Get Device List from ALSA
	
	snd_ctl_card_info_alloca(&info);
	snd_pcm_info_alloca(&pcminfo);
	snd_pcm_hw_params_alloca(&pars);
	snd_pcm_format_mask_alloca(&fmask);

	char hwdev[80];
	unsigned min, max;
	int card, err, dev, nsubd;
	int thecard = -1, thedev = -1;
	snd_pcm_stream_t stream = SND_PCM_STREAM_PLAYBACK;
	
	card = -1;
	if(snd_card_next(&card) < 0)
	{
		printf("No Devices\n");
		return devices;
	}

	if (playhandle)
		snd_pcm_close(playhandle);

	if (object)
		object->m_killed = TRUE;		// Stop the poll thread

	while(card >= 0)
	{
		sprintf(hwdev, "hw:%d", card);
		err = snd_ctl_open(&handle, hwdev, 0);
		err = snd_ctl_card_info(handle, info);
    
		printf("Card %d, ID `%s', name `%s'\n", card, snd_ctl_card_info_get_id(info),
                snd_ctl_card_info_get_name(info));

		if(thedev >= 0)
			dev = thedev;
		else {
			dev = -1;
			if( snd_ctl_pcm_next_device(handle, &dev) < 0 ) {
				snd_ctl_close(handle);
				goto nextcard;
      
			}
    
		}
		while( dev >= 0 )
		{
			snd_pcm_info_set_device(pcminfo, dev);
			snd_pcm_info_set_subdevice(pcminfo, 0);
			snd_pcm_info_set_stream(pcminfo, stream);
			err= snd_ctl_pcm_info(handle, pcminfo);
			if( thedev<0 && err == -ENOENT )
			{
			}
			else
			{
			nsubd= snd_pcm_info_get_subdevices_count(pcminfo);
			printf("  Device %d, ID `%s', name `%s', %d subdevices (%d available)\n",
				dev, snd_pcm_info_get_id(pcminfo), snd_pcm_info_get_name(pcminfo),
				nsubd, snd_pcm_info_get_subdevices_avail(pcminfo));

			sprintf(hwdev, "hw:%d,%d", card, dev);

			err = snd_pcm_open(&pcm, hwdev, stream, SND_PCM_NONBLOCK);
			if (err)
			{
				if (err == 16)
				{
					snd_pcm_close(playhandle);
					err = snd_pcm_open(&pcm, hwdev, stream, SND_PCM_NONBLOCK);
				}
				else
					printf("Error %d opening sound device", err);
			}

			err= snd_pcm_hw_params_any(pcm, pars);
 
			snd_pcm_hw_params_get_channels_min(pars, &min);
			snd_pcm_hw_params_get_channels_max(pars, &max);
			if( min == max )
				if( min == 1 )  printf("    1 channel, ");
				else            printf("    %d channels, ", min);
			else              printf("    %u..%u channels, ", min, max);
			
			snd_pcm_hw_params_get_rate_min(pars, &min, NULL);
			snd_pcm_hw_params_get_rate_max(pars, &max, NULL);
			printf("sampling rate %u..%u Hz\n", min, max);

			sprintf(NameString, "hw:%d,%d %s(%s)", card, dev,
				snd_pcm_info_get_name(pcminfo), snd_ctl_card_info_get_name(info));

			wxString name(NameString, wxConvLocal);
			devices.Add(name);

			snd_pcm_close(pcm);
			pcm= NULL;
			}

			if( thedev >= 0 || snd_ctl_pcm_next_device(handle, &dev) < 0 )
				break;
	    }
		snd_ctl_close(handle);

nextcard:

		if( thecard >= 0 || snd_card_next(&card) < 0 )
			break;
	}

	return devices;
}

void CSoundCardReaderWriter::setCallback(CDummyRepeaterThread* callback, int id)
{
	wxASSERT(callback != NULL);

	m_callback = callback;
	m_thread = (CDummyRepeaterThread *)callback;

	m_id = id;

	//	This is the first thing called, so start the background thread here

	pthread_t thread;
	int rc;

	m_killed = FALSE;

	rc = pthread_create(&thread, NULL, (void * (*)(void *))&CSoundCardReaderWriter::PollThread,static_cast<void*>(this));

	if (rc)
		printf("Create Thread Failed %d\n", rc);
	else
		pthread_detach(thread);
}

bool CSoundCardReaderWriter::open()
{
	int err = 0;

	char buf1[100];
	char buf2[100];
	char * ptr;

	strcpy( buf1, (const char*)m_writeDevice.mb_str(wxConvUTF8) );
	strcpy( buf2, (const char*)m_readDevice.mb_str(wxConvUTF8) );

	ptr = strchr(buf1, ' ');
	if (ptr) *ptr = 0;				// Get Device part of name

	ptr = strchr(buf2, ' ');
	if (ptr) *ptr = 0;				// Get Device part of name

	printf("Opening %s %s Rate %d\n", buf1, buf2, m_sampleRate);

	snd_pcm_hw_params_t *hw_params;
	
	if ((err = snd_pcm_open(&m_playhandle, buf1, SND_PCM_STREAM_PLAYBACK, SND_PCM_NONBLOCK)) < 0) {
		fprintf (stderr, "cannot open playback audio device %s (%s)\n",  buf1, snd_strerror(err));
		wxLogError(wxT("Cannot open the Playback stream"));
		return false;
	}

	playhandle = m_playhandle;		// Nasty Hack
		   
	if ((err = snd_pcm_hw_params_malloc (&hw_params)) < 0) {
		fprintf (stderr, "cannot allocate hardware parameter structure (%s)\n", snd_strerror(err));
		return false;
	}
				 
	if ((err = snd_pcm_hw_params_any (m_playhandle, hw_params)) < 0) {
		fprintf (stderr, "cannot initialize hardware parameter structure (%s)\n", snd_strerror(err));
		return false;
	}
	
	if ((err = snd_pcm_hw_params_set_access (m_playhandle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0) {
			fprintf (stderr, "cannot set access type (%s)\n", snd_strerror (err));
		return false;
	}
	if ((err = snd_pcm_hw_params_set_format (m_playhandle, hw_params, SND_PCM_FORMAT_S16_LE)) < 0) {
		fprintf (stderr, "cannot set sample format (%s)\n", snd_strerror(err));
		return false;
	}
	
	if ((err = snd_pcm_hw_params_set_rate (m_playhandle, hw_params, m_sampleRate, 0)) < 0) {
		fprintf (stderr, "cannot set sample rate (%s)\n", snd_strerror(err));
		return false;
	}

	m_playchannels = 1;
	
	if ((err = snd_pcm_hw_params_set_channels (m_playhandle, hw_params, 1)) < 0)
	{
		fprintf (stderr, "cannot set play channel count to 1 (%s)\n", snd_strerror(err));
		m_playchannels = 2;

		if ((err = snd_pcm_hw_params_set_channels (m_playhandle, hw_params, 2)) < 0)
		{
			fprintf (stderr, "cannot play set channel count to 2 (%s)\n", snd_strerror(err));
				return false;
		}
		fprintf (stderr, "Play channel count set to 2 (%s)\n", snd_strerror(err));
	}
	
	if ((err = snd_pcm_hw_params (m_playhandle, hw_params)) < 0) {
		fprintf (stderr, "cannot set parameters (%s)\n", snd_strerror(err));
		return false;
	}
	
	snd_pcm_hw_params_free(hw_params);
	
	if ((err = snd_pcm_prepare (m_playhandle)) < 0) {
		fprintf (stderr, "cannot prepare audio interface for use (%s)\n", snd_strerror(err));
		wxLogError(wxT("Cannot start the audio stream(s)"));
		return false;
	}

	// Open Capture

	
	if ((err = snd_pcm_open (&m_rechandle, buf2, SND_PCM_STREAM_CAPTURE, 0)) < 0) {
		fprintf (stderr, "cannot open capture audio device %s (%s)\n",  buf2, snd_strerror(err));
		wxLogError(wxT("Cannot open the capture stream"));
		return false;
	}

	rechandle = m_rechandle;
		   
	if ((err = snd_pcm_hw_params_malloc (&hw_params)) < 0) {
		fprintf (stderr, "cannot allocate hardware parameter structure (%s)\n", snd_strerror(err));
		return false;
	}
				 
	if ((err = snd_pcm_hw_params_any (m_rechandle, hw_params)) < 0) {
		fprintf (stderr, "cannot initialize hardware parameter structure (%s)\n", snd_strerror(err));
		return false;
	}
	
	if ((err = snd_pcm_hw_params_set_access (m_rechandle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0) {
			fprintf (stderr, "cannot set access type (%s)\n", snd_strerror (err));
		return false;
	}
	if ((err = snd_pcm_hw_params_set_format (m_rechandle, hw_params, SND_PCM_FORMAT_S16_LE)) < 0) {
		fprintf (stderr, "cannot set sample format (%s)\n", snd_strerror(err));
		return false;
	}
	
	if ((err = snd_pcm_hw_params_set_rate (m_rechandle, hw_params, m_sampleRate, 0)) < 0) {
		fprintf (stderr, "cannot set sample rate (%s)\n", snd_strerror(err));
		return false;
	}
	
	m_recchannels = 1;
	
	if ((err = snd_pcm_hw_params_set_channels (m_rechandle, hw_params, 1)) < 0)
	{
		fprintf (stderr, "cannot set rec channel count to 1 (%s)\n", snd_strerror(err));
		m_recchannels = 2;

		if ((err = snd_pcm_hw_params_set_channels (m_rechandle, hw_params, 2)) < 0)
		{
			fprintf (stderr, "cannot rec set channel count to 2 (%s)\n", snd_strerror(err));
			return false;
		}
		fprintf (stderr, "Record channel count set to 2 (%s)\n", snd_strerror(err));
	}
	
	if ((err = snd_pcm_hw_params (m_rechandle, hw_params)) < 0) {
		fprintf (stderr, "cannot set parameters (%s)\n", snd_strerror(err));
		return false;
	}
	
	snd_pcm_hw_params_free(hw_params);
	
	if ((err = snd_pcm_prepare (m_rechandle)) < 0) {
		fprintf (stderr, "cannot prepare audio interface for use (%s)\n", snd_strerror(err));
		wxLogError(wxT("Cannot start the audio stream(s)"));
		return false;
	}

	int i;
	short buf[256];

	for (i = 0; i < 10; ++i)
	{
		if ((err = snd_pcm_readi (m_rechandle, buf, 128)) != 128)
		{
			fprintf (stderr, "read from audio interface failed (%s)\n",
				 snd_strerror (err));
		}
	}

	printf("Read got %d\n", err);

	
 	return true;
}

void CSoundCardReaderWriter::close()
{
	snd_pcm_close(m_rechandle);
	snd_pcm_close(m_playhandle);
}

void CSoundCardReaderWriter::sendtocard(const wxFloat32* input, unsigned int nSamples)
{
	short samples[24000];

	unsigned int n;
	int ret;
	snd_pcm_sframes_t avail;

//	Check for underrun. If found, inject some silence

	avail = snd_pcm_avail_update(m_playhandle);

	if (avail < 0)
	{
		if (avail != -32)
			printf("Playback Avail Recovering from %ld ..\n", avail);
		snd_pcm_recover(m_playhandle, avail, 1);
		memset(samples, 0, 48000);
		ret = snd_pcm_writei(m_playhandle, samples, 960);		// one time slot 
		avail = snd_pcm_avail_update(m_playhandle);
//		printf("avail play after recovery and silence insert returned %d\n", avail);
	}

	if (m_playchannels == 1)
	{
		for (n = 0; n < nSamples; n++)
			samples[n] = input[n] * 32768.0;
	}
	else
	{
		int i = 0;
		for (n = 0; n < nSamples; n++)
		{
			samples[i++] = input[n] * 32768.0;
			samples[i++] = input[n] * 32768.0;			// Same value to both channels
		}
	}
        
	ret = snd_pcm_writei(m_playhandle, samples, nSamples);

	if (ret < 0)
	{
		printf("Write Recovering from %d ..\n", ret);
		snd_pcm_recover(m_playhandle, ret, 1);
		ret = snd_pcm_writei(m_playhandle, samples, nSamples);
		printf("Write after recovery returned %d\n", ret);
		// Add a bit of silence
		memset(samples, 0, 48000);
		ret = snd_pcm_writei(m_playhandle, samples, 960);		// one time slot 
		printf("Write after recovery silence returned %d\n", ret);
	}

//	printf("writing %d returned %d\n", nSamples, ret);

}

int CSoundCardReaderWriter::getfromcard(wxFloat32* input, unsigned int nSamples)
{
	short samples[3200];		// 1600 max, but could be stereo
	int n;
	int ret;
	int avail;

	if (nSamples == 0)
	{
		// Clear queue 
	
		avail = snd_pcm_avail_update(m_rechandle);

		if (avail < 0)
		{
			printf("Discard Recovering from %d ..\n", avail);
			close();
			open();
			avail = snd_pcm_avail_update(m_rechandle);
		}

		printf("Discarding %d samples from card\n", avail);

		while (avail)
		{
			if (avail > 1600)
				avail = 1600;

			ret = snd_pcm_readi(m_rechandle, samples, avail);
			printf("Discarded %d samples from card\n", ret);

			avail = snd_pcm_avail_update(m_rechandle);

			printf("Discarding %d samples from card\n", avail);
		}
	}

	avail = snd_pcm_avail_update(m_rechandle);

	if (avail < 0)
	{
		printf("Read Recovering from %d ..\n", avail);
		close();
		open();
	//	snd_pcm_recover(m_rechandle, avail, 0);
		avail = snd_pcm_avail_update(m_rechandle);
		printf("Read After recovery %d ..\n", avail);
	}

	if (avail < 960)
		return 0;

	avail = 960;

	ret = snd_pcm_readi(m_rechandle, samples, avail);

	if (ret < 0)
	{
		printf("RX Error %d\n", ret);
		snd_pcm_recover(m_rechandle, avail, 0);
		return 0;
	}

	if (m_recchannels == 1)
	{
		for (n = 0; n < ret; n++)
			input[n] = samples[n] / 32768.0;
	}
	else
	{
		int i = 0;
		for (n = 0; n < (ret * 2); n+=2)
		{
			input[i++] = samples[n+1] / 32768.0;
		}
	}

	return ret;
 
}
//void CSoundCardReaderWriter::callback(const wxFloat32* input, wxFloat32* output, unsigned int nSamples)
//{
//	if (m_callback != NULL)
//		m_callback->callback(input, output, nSamples, m_id);
//}

void* CSoundCardReaderWriter::PollThread(void *userData)
{	
	object = static_cast<CSoundCardReaderWriter*>(this);

	//	Thread to read/write from/to the soundcard. Calls the DUmmyRepeaterThread callback
	//	to do the actual work

	wxMilliSleep(1000);

	while (!object->m_killed)
	{
		m_callback->callback();
		wxMilliSleep(2);
	}

	printf("RW poll thread stopped\n");

	return false;
}


void CSoundCardReaderWriter::kill()
{
	m_killed = true;
}
