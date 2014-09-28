/*
 *   Copyright (C) 2010,2012,2013 by Jonathan Naylor G4KLX
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

#include "DVAPClientApp.h"

#include "SerialDataController.h"
#include "DVAPController.h"
#include "DStarDefines.h"
#include "Version.h"
#include "Logger.h"

#include <wx/tokenzr.h>
#include <wx/config.h>
#include <wx/filename.h>

IMPLEMENT_APP(CDVAPClientApp)

static const wxString KEY_CALLSIGN1          = wxT("/callsign1");
static const wxString KEY_CALLSIGN2          = wxT("/callsign2");
static const wxString KEY_MESSAGE_TYPE       = wxT("/msgType");
static const wxString KEY_MESSAGE_TEXT       = wxT("/msgText");
static const wxString KEY_DVAP_PORT          = wxT("/dvapPort");
static const wxString KEY_DVAP_FREQUENCY     = wxT("/dvapFrequency");
static const wxString KEY_DVAP_POWER         = wxT("/dvapPower");
static const wxString KEY_DVAP_SQUELCH       = wxT("/dvapSquelch");
static const wxString KEY_SOUND_READ_DEVICE  = wxT("/readDevice");
static const wxString KEY_SOUND_WRITE_DEVICE = wxT("/writeDevice");
static const wxString KEY_DVD_DEVICE         = wxT("/dvdDevice");
static const wxString KEY_YOUR_CALL          = wxT("/yourCall");
static const wxString KEY_YOUR_LIST          = wxT("/yourList");
static const wxString KEY_RPT1_CALL          = wxT("/rpt1Call");
static const wxString KEY_RPT1_LIST          = wxT("/rpt1List");
static const wxString KEY_RPT2_CALL          = wxT("/rpt2Call");
static const wxString KEY_RPT2_LIST          = wxT("/rpt2List");
static const wxString KEY_BLEEP              = wxT("/bleep");

static const wxString   DEFAULT_CALLSIGN1          = wxEmptyString;
static const wxString   DEFAULT_CALLSIGN2          = wxEmptyString;
static const wxString   DEFAULT_MESSAGE_TEXT       = wxEmptyString;
static const wxString   DEFAULT_DVAP_PORT          = wxEmptyString;
static const long       DEFAULT_DVAP_FREQUENCY     = 145500000L;
static const long       DEFAULT_DVAP_POWER         = 10L;
static const long       DEFAULT_DVAP_SQUELCH       = -100L;
static const wxString   DEFAULT_SOUND_READ_DEVICE  = wxEmptyString;
static const wxString   DEFAULT_SOUND_WRITE_DEVICE = wxEmptyString;
static const wxString   DEFAULT_DVD_DEVICE         = wxEmptyString;
static const wxString   DEFAULT_YOUR_CALL          = wxEmptyString;
static const wxString   DEFAULT_YOUR_LIST          = wxEmptyString;
static const wxString   DEFAULT_RPT1_CALL          = wxEmptyString;
static const wxString   DEFAULT_RPT1_LIST          = wxEmptyString;
static const wxString   DEFAULT_RPT2_CALL          = wxEmptyString;
static const wxString   DEFAULT_RPT2_LIST          = wxEmptyString;
static const bool       DEFAULT_BLEEP              = true;


CDVAPClientApp::CDVAPClientApp() :
wxApp(),
m_frame(NULL),
m_thread(NULL)
{
}

CDVAPClientApp::~CDVAPClientApp()
{
}

bool CDVAPClientApp::OnInit()
{
	SetVendorName(VENDOR_NAME);

	wxFileName loggerName(wxFileName::GetHomeDir(), LOG_FILE_NAME, wxT("log"));
	wxLog* log = new CLogger(loggerName.GetFullPath());
	wxLog::SetActiveTarget(log);

	m_frame = new CDVAPClientFrame(APPLICATION_NAME + wxT(" - ") + VERSION);
	m_frame->Show();

	SetTopWindow(m_frame);

	wxLogInfo(wxT("Starting ") + APPLICATION_NAME + wxT(" - ") + VERSION);

	// Log the SVN revsion and the version of wxWidgets and the Operating System
	wxLogInfo(SVNREV);
	wxLogInfo(wxT("Using wxWidgets %d.%d.%d on %s"), wxMAJOR_VERSION, wxMINOR_VERSION, wxRELEASE_NUMBER, ::wxGetOsDescription().c_str());

	createThread();

	return wxApp::OnInit();
}

int CDVAPClientApp::OnExit()
{
	wxLogInfo(APPLICATION_NAME + wxT(" is exiting"));

	m_thread->kill();
	m_thread->Wait();
	delete m_thread;

	return 0;
}

#if defined(__WXDEBUG__)
void CDVAPClientApp::OnAssertFailure(const wxChar* file, int line, const wxChar* func, const wxChar* cond, const wxChar* msg)
{
	wxLogFatalError(wxT("Assertion failed on line %d in file %s and function %s: %s %s"), line, file, func, cond, msg);
}
#endif

void CDVAPClientApp::showMessage(CMessageData* message) const
{
	wxASSERT(message != NULL);

	m_frame->showMessage(message);
}

void CDVAPClientApp::showHeader(CHeaderData* header) const
{
	wxASSERT(header != NULL);

	m_frame->showHeader(header);
}

void CDVAPClientApp::showBusy(CBusyData* busy) const
{
	wxASSERT(busy != NULL);

	m_frame->showBusy(busy);
}

void CDVAPClientApp::error(const wxString& text) const
{
	m_frame->error(text);
}

void CDVAPClientApp::setYour(const wxString& your)
{
	m_thread->setYour(your);
}

void CDVAPClientApp::setRpt1(const wxString& rpt1)
{
	m_thread->setRpt1(rpt1);
}

void CDVAPClientApp::setRpt2(const wxString& rpt2)
{
	m_thread->setRpt2(rpt2);
}

bool CDVAPClientApp::setTransmit(bool on)
{
	return m_thread->setTransmit(on);
}

void CDVAPClientApp::setRecording(bool recording)
{
	m_thread->setRecording(recording);
}

void CDVAPClientApp::getCallsign(wxString& callsign1, wxString& callsign2) const
{
	wxConfigBase* profile = new wxConfig(APPLICATION_NAME);
	wxASSERT(profile != NULL);

	profile->Read(KEY_CALLSIGN1, &callsign1, DEFAULT_CALLSIGN1);
	profile->Read(KEY_CALLSIGN2, &callsign2, DEFAULT_CALLSIGN2);

	wxLogInfo(wxT("Callsign set to %s/%s"), callsign1.c_str(), callsign2.c_str());

	delete profile;
}

void CDVAPClientApp::setCallsign(const wxString& callsign1, const wxString& callsign2) const
{
	wxConfigBase* profile = new wxConfig(APPLICATION_NAME);
	wxASSERT(profile != NULL);

	profile->Write(KEY_CALLSIGN1, callsign1);
	profile->Write(KEY_CALLSIGN2, callsign2);
	profile->Flush();

	wxLogInfo(wxT("Callsign set to %s/%s"), callsign1.c_str(), callsign2.c_str());

	m_thread->setCallsign(callsign1, callsign2);

	delete profile;
}

void CDVAPClientApp::getMessage(wxString& message) const
{
	wxConfigBase* profile = new wxConfig(APPLICATION_NAME);
	wxASSERT(profile != NULL);

	profile->Read(KEY_MESSAGE_TEXT, &message, DEFAULT_MESSAGE_TEXT);

	wxLogInfo(wxT("Message set to %s"), message.c_str());

	delete profile;
}

void CDVAPClientApp::setMessage(const wxString& message) const
{
	wxConfigBase* profile = new wxConfig(APPLICATION_NAME);
	wxASSERT(profile != NULL);

	profile->Write(KEY_MESSAGE_TEXT, message);
	profile->Flush();

	wxLogInfo(wxT("Message set to %s"), message.c_str());

	m_thread->setMessage(message);

	delete profile;
}

void CDVAPClientApp::getDVAP(wxString& port, unsigned int& frequency, int& power, int& squelch) const
{
	wxConfigBase* profile = new wxConfig(APPLICATION_NAME);
	wxASSERT(profile != NULL);

	profile->Read(KEY_DVAP_PORT,      &port, DEFAULT_DVAP_PORT);

	long temp;
	profile->Read(KEY_DVAP_FREQUENCY, &temp, DEFAULT_DVAP_FREQUENCY);
	frequency = (unsigned int)temp;

	profile->Read(KEY_DVAP_POWER,     &temp, DEFAULT_DVAP_POWER);
	power = int(temp);

	profile->Read(KEY_DVAP_SQUELCH,   &temp, DEFAULT_DVAP_SQUELCH);
	squelch = int(temp);

	wxLogInfo(wxT("DVAP: port: %s, frequency: %u Hz, power: %d dBm, squelch: %d dBm"), port.c_str(), frequency, power, squelch);

	delete profile;
}

void CDVAPClientApp::setDVAP(const wxString& port, unsigned int frequency, int power, int squelch) const
{
	wxConfigBase* profile = new wxConfig(APPLICATION_NAME);
	wxASSERT(profile != NULL);

	profile->Write(KEY_DVAP_PORT,      port);
	profile->Write(KEY_DVAP_FREQUENCY, long(frequency));
	profile->Write(KEY_DVAP_POWER,     long(power));
	profile->Write(KEY_DVAP_SQUELCH,   long(squelch));
	profile->Flush();

	wxLogInfo(wxT("DVAP: port: %s, frequency: %u Hz, power: %d dBm, squelch: %d dBm"), port.c_str(), frequency, power, squelch);

	delete profile;
}

void CDVAPClientApp::getSoundcard(wxString& readDevice, wxString& writeDevice) const
{
	wxConfigBase* profile = new wxConfig(APPLICATION_NAME);
	wxASSERT(profile != NULL);

	profile->Read(KEY_SOUND_READ_DEVICE,  &readDevice,  DEFAULT_SOUND_READ_DEVICE);
	profile->Read(KEY_SOUND_WRITE_DEVICE, &writeDevice, DEFAULT_SOUND_WRITE_DEVICE);

	wxLogInfo(wxT("Soundcard set to %s:%s"), readDevice.c_str(), writeDevice.c_str());

	delete profile;
}

void CDVAPClientApp::setSoundcard(const wxString& readDevice, const wxString& writeDevice) const
{
	wxConfigBase* profile = new wxConfig(APPLICATION_NAME);
	wxASSERT(profile != NULL);

	profile->Write(KEY_SOUND_READ_DEVICE,  readDevice);
	profile->Write(KEY_SOUND_WRITE_DEVICE, writeDevice);
	profile->Flush();

	wxLogInfo(wxT("Soundcard set to %s:%s"), readDevice.c_str(), writeDevice.c_str());

	delete profile;
}

void CDVAPClientApp::getDVDongle(wxString& device) const
{
	wxConfigBase* profile = new wxConfig(APPLICATION_NAME);
	wxASSERT(profile != NULL);

	profile->Read(KEY_DVD_DEVICE, &device, DEFAULT_DVD_DEVICE);

	wxLogInfo(wxT("DV Dongle set to: %s"), device.c_str());

	delete profile;
}

void CDVAPClientApp::setDVDongle(const wxString& device) const
{
	wxConfigBase* profile = new wxConfig(APPLICATION_NAME);
	wxASSERT(profile != NULL);

	profile->Write(KEY_DVD_DEVICE, device);
	profile->Flush();

	wxLogInfo(wxT("DV Dongle set to: %s"), device.c_str());

	delete profile;
}

void CDVAPClientApp::getYourCalls(wxString& call, wxSortedArrayString& list) const
{
	wxConfigBase* profile = new wxConfig(APPLICATION_NAME);
	wxASSERT(profile != NULL);

	wxString temp;

	profile->Read(KEY_YOUR_CALL, &call, DEFAULT_YOUR_CALL);
	profile->Read(KEY_YOUR_LIST, &temp, DEFAULT_YOUR_LIST);

	wxLogInfo(wxT("Your call set to %s"), call.c_str());
	wxLogInfo(wxT("Your list set to %s"), temp.c_str());

	// Convert the comma delimited list to an array
	list.Clear();
	wxStringTokenizer tokens(temp, wxT(","));
	while (tokens.HasMoreTokens()) {
		wxString item = tokens.GetNextToken();
		if (!item.IsEmpty())
			list.Add(item);
	}

	delete profile;
}

void CDVAPClientApp::setYourCalls(const wxString& call, const wxSortedArrayString& list) const
{
	wxConfigBase* profile = new wxConfig(APPLICATION_NAME);
	wxASSERT(profile != NULL);

	// Convert the array into a comma delimited list
	wxString temp;
	size_t count = list.GetCount();
	for (unsigned int i = 0U; i < count; i++) {
		temp += list.Item(i);
		temp += wxT(',');
	}

	profile->Write(KEY_YOUR_CALL, call);
	profile->Write(KEY_YOUR_LIST, temp);
	profile->Flush();

	wxLogInfo(wxT("Your call set to %s"), call.c_str());
	wxLogInfo(wxT("Your list set to %s"), temp.c_str());

	delete profile;
}

void CDVAPClientApp::getRpt1Calls(wxString& call, wxSortedArrayString& list) const
{
	wxConfigBase* profile = new wxConfig(APPLICATION_NAME);
	wxASSERT(profile != NULL);

	wxString temp;

	profile->Read(KEY_RPT1_CALL, &call, DEFAULT_RPT1_CALL);
	profile->Read(KEY_RPT1_LIST, &temp, DEFAULT_RPT1_LIST);

	wxLogInfo(wxT("RPT1 call set to %s"), call.c_str());
	wxLogInfo(wxT("RPT1 list set to %s"), temp.c_str());

	// Convert the comma delimited list to an array
	list.Clear();
	wxStringTokenizer tokens(temp, wxT(","));
	while (tokens.HasMoreTokens()) {
		wxString item = tokens.GetNextToken();
		if (!item.IsEmpty())
			list.Add(item);
	}

	delete profile;
}

void CDVAPClientApp::setRpt1Calls(const wxString& call, const wxSortedArrayString& list) const
{
	wxConfigBase* profile = new wxConfig(APPLICATION_NAME);
	wxASSERT(profile != NULL);

	// Convert the array into a comma delimited list
	wxString temp;
	size_t count = list.GetCount();
	for (unsigned int i = 0U; i < count; i++) {
		temp += list.Item(i);
		temp += wxT(',');
	}

	profile->Write(KEY_RPT1_CALL, call);
	profile->Write(KEY_RPT1_LIST, temp);
	profile->Flush();

	wxLogInfo(wxT("RPT1 call set to %s"), call.c_str());
	wxLogInfo(wxT("RPT1 list set to %s"), temp.c_str());

	delete profile;
}

void CDVAPClientApp::getRpt2Calls(wxString& call, wxSortedArrayString& list) const
{
	wxConfigBase* profile = new wxConfig(APPLICATION_NAME);
	wxASSERT(profile != NULL);

	wxString temp;

	profile->Read(KEY_RPT2_CALL, &call, DEFAULT_RPT2_CALL);
	profile->Read(KEY_RPT2_LIST, &temp, DEFAULT_RPT2_LIST);

	wxLogInfo(wxT("RPT2 call set to %s"), call.c_str());
	wxLogInfo(wxT("RPT2 list set to %s"), temp.c_str());

	// Convert the comma delimited list to an array
	list.Clear();
	wxStringTokenizer tokens(temp, wxT(","));
	while (tokens.HasMoreTokens()) {
		wxString item = tokens.GetNextToken();
		if (!item.IsEmpty())
			list.Add(item);
	}

	delete profile;
}

void CDVAPClientApp::setRpt2Calls(const wxString& call, const wxSortedArrayString& list) const
{
	wxConfigBase* profile = new wxConfig(APPLICATION_NAME);
	wxASSERT(profile != NULL);

	// Convert the array into a comma delimited list
	wxString temp;
	size_t count = list.GetCount();
	for (unsigned int i = 0U; i < count; i++) {
		temp += list.Item(i);
		temp += wxT(',');
	}

	profile->Write(KEY_RPT2_CALL, call);
	profile->Write(KEY_RPT2_LIST, temp);
	profile->Flush();

	wxLogInfo(wxT("RPT2 call set to %s"), call.c_str());
	wxLogInfo(wxT("RPT2 list set to %s"), temp.c_str());

	delete profile;
}

void CDVAPClientApp::getBleep(bool& on) const
{
	wxConfigBase* profile = new wxConfig(APPLICATION_NAME);
	wxASSERT(profile != NULL);

	profile->Read(KEY_BLEEP, &on, DEFAULT_BLEEP);

	wxLogInfo(wxT("End bleep set to %d"), int(on));

	delete profile;
}

void CDVAPClientApp::setBleep(bool on) const
{
	wxConfigBase* profile = new wxConfig(APPLICATION_NAME);
	wxASSERT(profile != NULL);

	profile->Write(KEY_BLEEP, on);
	profile->Flush();

	wxLogInfo(wxT("End bleep set to %d"), int(on));

	m_thread->setBleep(on);

	delete profile;
}

void CDVAPClientApp::createThread()
{
	m_thread = new CDVAPClientThread;
	m_thread->Create();
	m_thread->Run();

	wxString callsign1, callsign2;
	getCallsign(callsign1, callsign2);
	m_thread->setCallsign(callsign1, callsign2);

	wxString call;
	wxSortedArrayString list;

	getYourCalls(call, list);
	m_thread->setYour(call);

	getRpt1Calls(call, list);
	m_thread->setRpt1(call);

	getRpt2Calls(call, list);
	m_thread->setRpt2(call);

	wxString message;
	getMessage(message);
	m_thread->setMessage(message);

	wxString device;
	getDVDongle(device);

	if (!device.IsEmpty()) {
		CDVDongleController* dongle = new CDVDongleController(new CSerialDataController(device, SERIAL_230400));

		bool res = dongle->open();
		if (!res) {
			wxLogError(wxT("Can't find the DV-Dongle on the port specified"));
			error(_("Can't find the DV-Dongle on the port specified"));
		} else {
			m_thread->setDVDongle(dongle);
		}
	}

	wxString readDevice, writeDevice;
	getSoundcard(readDevice, writeDevice);

	if (!readDevice.IsEmpty() && !writeDevice.IsEmpty()) {
#if defined(__WINDOWS__)
		CSoundCardReaderWriter* soundcard = new CSoundCardReaderWriter(readDevice, writeDevice, DSTAR_RADIO_SAMPLE_RATE, DSTAR_RADIO_BLOCK_SIZE);
#else
		CSoundCardReaderWriter* soundcard = new CSoundCardReaderWriter(readDevice, writeDevice, DSTAR_RADIO_SAMPLE_RATE, 64U);
#endif
		soundcard->setCallback(m_thread, 0);

		bool res = soundcard->open();
		if (!res) {
			wxLogError(wxT("Cannot open the sound card"));
			error(_("Cannot open the sound card"));
		} else {
			m_thread->setSoundCard(soundcard);
		}
	}

	wxString dvapPort;
	unsigned int dvapFrequency;
	int dvapPower, dvapSquelch;
	getDVAP(dvapPort, dvapFrequency, dvapPower, dvapSquelch);

	if (!dvapPort.IsEmpty()) {
		CDVAPController* dvap = new CDVAPController(dvapPort, dvapFrequency, dvapPower, dvapSquelch);
		bool res = dvap->open();
		if (!res) {
			wxLogError(wxT("Unable to open the DVAP"));
			error(_("Cannot open the DVAP modem"));
		} else {
			m_thread->setDVAP(dvap);
		}
	}

	bool bleep;
	getBleep(bleep);
	m_thread->setBleep(bleep);
}
