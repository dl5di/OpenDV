/*
 *   Copyright (C) 2009-2014 by Jonathan Naylor G4KLX
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

#include "AnalogueRepeaterPreferences.h"
#include "AnalogueRepeaterFrame.h"
#include "AnalogueRepeaterApp.h"
#include "LogEvent.h"
#include "Version.h"

#include <wx/aboutdlg.h>

#include <cmath>

DEFINE_EVENT_TYPE(LOG_EVENT)

enum {
	Menu_Edit_Preferences = 6000,

	Menu_View_Updates,

	Menu_Action_Shutdown,
	Menu_Action_Startup,
	Menu_Action_Timeout,
	Menu_Action_TimeReset,
	Menu_Action_Command1,
	Menu_Action_Command2,

	Menu_Output_1,
	Menu_Output_2,
	Menu_Output_3,
	Menu_Output_4
};

BEGIN_EVENT_TABLE(CAnalogueRepeaterFrame, wxFrame)
#if defined(__WXDEBUG__)
	EVT_MENU(wxID_OPEN,             CAnalogueRepeaterFrame::onOpen)
#endif
	EVT_MENU(wxID_EXIT,             CAnalogueRepeaterFrame::onQuit)
	EVT_MENU(Menu_Edit_Preferences, CAnalogueRepeaterFrame::onPreferences)
	EVT_MENU(Menu_View_Updates,     CAnalogueRepeaterFrame::onUpdates)
	EVT_MENU(wxID_ABOUT,            CAnalogueRepeaterFrame::onAbout)

	EVT_MENU(Menu_Action_Startup,   CAnalogueRepeaterFrame::onActions)
	EVT_MENU(Menu_Action_Shutdown,  CAnalogueRepeaterFrame::onActions)
	EVT_MENU(Menu_Action_Timeout,   CAnalogueRepeaterFrame::onActions)
	EVT_MENU(Menu_Action_TimeReset, CAnalogueRepeaterFrame::onActions)
	EVT_MENU(Menu_Action_Command1,  CAnalogueRepeaterFrame::onActions)
	EVT_MENU(Menu_Action_Command2,  CAnalogueRepeaterFrame::onActions)

	EVT_MENU(Menu_Output_1, CAnalogueRepeaterFrame::onOutputs)
	EVT_MENU(Menu_Output_2, CAnalogueRepeaterFrame::onOutputs)
	EVT_MENU(Menu_Output_3, CAnalogueRepeaterFrame::onOutputs)
	EVT_MENU(Menu_Output_4, CAnalogueRepeaterFrame::onOutputs)

	EVT_CLOSE(CAnalogueRepeaterFrame::onClose)

	EVT_TIMER(-1, CAnalogueRepeaterFrame::onTimer)

	EVT_CUSTOM(LOG_EVENT, wxID_ANY, CAnalogueRepeaterFrame::onLog)
END_EVENT_TABLE()

const unsigned int BORDER_SIZE   = 5U;
const unsigned int LABEL_WIDTH   = 75U;
const unsigned int DATA_WIDTH    = 100U;
const unsigned int LOGTEXT_WIDTH = 575U;


CAnalogueRepeaterFrame::CAnalogueRepeaterFrame(const wxString& title, const wxPoint& position, bool gui) :
wxFrame(NULL, -1, title, position),
m_radioTransmit(NULL),
m_radioSquelch(NULL),
m_radioVOGAD(NULL),
m_toneburst(NULL),
m_ctcss(NULL),
m_radioDTMF(NULL),
m_extSquelch(NULL),
m_extTransmit(NULL),
m_extVOGAD(NULL),
m_extDTMF(NULL),
m_level(NULL),
m_state(NULL),
m_battery(NULL),
m_timeout(NULL),
m_lockout(NULL),
m_latch(NULL),
m_hang(NULL),
m_ack(NULL),
m_minimum(NULL),
m_callsign(NULL),
m_active(NULL),
m_logLine1(NULL),
m_logLine2(NULL),
m_logLine3(NULL),
m_timer(this),
m_outputMenu(NULL),
#if defined(__WXDEBUG__)
m_updates(true)
#else
m_updates(gui)
#endif
{
	SetMenuBar(createMenuBar());

	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

	wxPanel* panel = new wxPanel(this);

	wxBoxSizer* panelSizer = new wxBoxSizer(wxVERTICAL);

	wxStaticBoxSizer* status1Sizer = new wxStaticBoxSizer(new wxStaticBox(panel, -1, _("Status")), wxVERTICAL);
	wxFlexGridSizer* status2Sizer = new wxFlexGridSizer(6);

	wxStaticText* stateLabel = new wxStaticText(panel, -1, _("State:"), wxDefaultPosition, wxSize(LABEL_WIDTH, -1));
	status2Sizer->Add(stateLabel, 0, wxALL, BORDER_SIZE);

	m_state = new wxStaticText(panel, -1, wxEmptyString, wxDefaultPosition, wxSize(DATA_WIDTH, -1));
	status2Sizer->Add(m_state, 0, wxALL, BORDER_SIZE);

	wxStaticText* levelLabel = new wxStaticText(panel, -1, _("Level:"), wxDefaultPosition, wxSize(LABEL_WIDTH, -1));
	status2Sizer->Add(levelLabel, 0, wxALL, BORDER_SIZE);

	m_level = new wxStaticText(panel, -1, wxEmptyString, wxDefaultPosition, wxSize(DATA_WIDTH, -1));
	status2Sizer->Add(m_level, 0, wxALL, BORDER_SIZE);

	wxStaticText* batteryLabel = new wxStaticText(panel, -1, _("Battery"), wxDefaultPosition, wxSize(LABEL_WIDTH, -1));
	status2Sizer->Add(batteryLabel, 0, wxALL, BORDER_SIZE);

	m_battery = new wxStaticText(panel, -1, wxEmptyString, wxDefaultPosition, wxSize(DATA_WIDTH, -1));
	status2Sizer->Add(m_battery, 0, wxALL, BORDER_SIZE);

	status1Sizer->Add(status2Sizer);
	panelSizer->Add(status1Sizer, 0, wxALL, BORDER_SIZE);

	wxStaticBoxSizer* radio1Sizer = new wxStaticBoxSizer(new wxStaticBox(panel, -1, _("Radio")), wxVERTICAL);
	wxFlexGridSizer* radio2Sizer = new wxFlexGridSizer(6);

	wxStaticText* radioSquelchLabel = new wxStaticText(panel, -1, _("Squelch:"), wxDefaultPosition, wxSize(LABEL_WIDTH, -1));
	radio2Sizer->Add(radioSquelchLabel, 0, wxALL, BORDER_SIZE);

	m_radioSquelch = new wxStaticText(panel, -1, wxEmptyString, wxDefaultPosition, wxSize(DATA_WIDTH, -1));
	radio2Sizer->Add(m_radioSquelch, 0, wxALL, BORDER_SIZE);

	wxStaticText* radioTransmitLabel = new wxStaticText(panel, -1, _("Transmit:"), wxDefaultPosition, wxSize(LABEL_WIDTH, -1));
	radio2Sizer->Add(radioTransmitLabel, 0, wxALL, BORDER_SIZE);

	m_radioTransmit = new wxStaticText(panel, -1, wxEmptyString, wxDefaultPosition, wxSize(DATA_WIDTH, -1));
	radio2Sizer->Add(m_radioTransmit, 0, wxALL, BORDER_SIZE);

	wxStaticText* radioVOGADLabel = new wxStaticText(panel, -1, _("VOGAD:"), wxDefaultPosition, wxSize(LABEL_WIDTH, -1));
	radio2Sizer->Add(radioVOGADLabel, 0, wxALL, BORDER_SIZE);

	m_radioVOGAD = new wxStaticText(panel, -1, wxEmptyString, wxDefaultPosition, wxSize(DATA_WIDTH, -1));
	radio2Sizer->Add(m_radioVOGAD, 0, wxALL, BORDER_SIZE);

	wxStaticText* toneburstLabel = new wxStaticText(panel, -1, _("1750Hz:"), wxDefaultPosition, wxSize(LABEL_WIDTH, -1));
	radio2Sizer->Add(toneburstLabel, 0, wxALL, BORDER_SIZE);

	m_toneburst = new wxStaticText(panel, -1, wxEmptyString, wxDefaultPosition, wxSize(DATA_WIDTH, -1));
	radio2Sizer->Add(m_toneburst, 0, wxALL, BORDER_SIZE);

	wxStaticText* ctcssLabel = new wxStaticText(panel, -1, _("CTCSS:"), wxDefaultPosition, wxSize(LABEL_WIDTH, -1));
	radio2Sizer->Add(ctcssLabel, 0, wxALL, BORDER_SIZE);

	m_ctcss = new wxStaticText(panel, -1, wxEmptyString, wxDefaultPosition, wxSize(DATA_WIDTH, -1));
	radio2Sizer->Add(m_ctcss, 0, wxALL, BORDER_SIZE);

	wxStaticText* radioDTMFLabel = new wxStaticText(panel, -1, _("DTMF:"), wxDefaultPosition, wxSize(LABEL_WIDTH, -1));
	radio2Sizer->Add(radioDTMFLabel, 0, wxALL, BORDER_SIZE);

	m_radioDTMF = new wxStaticText(panel, -1, wxEmptyString, wxDefaultPosition, wxSize(DATA_WIDTH, -1));
	radio2Sizer->Add(m_radioDTMF, 0, wxALL, BORDER_SIZE);

	radio1Sizer->Add(radio2Sizer);
	panelSizer->Add(radio1Sizer, 0, wxALL, BORDER_SIZE);

	wxStaticBoxSizer* external1Sizer = new wxStaticBoxSizer(new wxStaticBox(panel, -1, _("External")), wxVERTICAL);
	wxFlexGridSizer* external2Sizer = new wxFlexGridSizer(6);

	wxStaticText* extSquelchLabel = new wxStaticText(panel, -1, _("Squelch:"), wxDefaultPosition, wxSize(LABEL_WIDTH, -1));
	external2Sizer->Add(extSquelchLabel, 0, wxALL, BORDER_SIZE);

	m_extSquelch = new wxStaticText(panel, -1, wxEmptyString, wxDefaultPosition, wxSize(DATA_WIDTH, -1));
	external2Sizer->Add(m_extSquelch, 0, wxALL, BORDER_SIZE);

	wxStaticText* extTransmitLabel = new wxStaticText(panel, -1, _("Transmit:"), wxDefaultPosition, wxSize(LABEL_WIDTH, -1));
	external2Sizer->Add(extTransmitLabel, 0, wxALL, BORDER_SIZE);

	m_extTransmit = new wxStaticText(panel, -1, wxEmptyString, wxDefaultPosition, wxSize(DATA_WIDTH, -1));
	external2Sizer->Add(m_extTransmit, 0, wxALL, BORDER_SIZE);

	wxStaticText* extVOGADLabel = new wxStaticText(panel, -1, _("VOGAD:"), wxDefaultPosition, wxSize(LABEL_WIDTH, -1));
	external2Sizer->Add(extVOGADLabel, 0, wxALL, BORDER_SIZE);

	m_extVOGAD = new wxStaticText(panel, -1, wxEmptyString, wxDefaultPosition, wxSize(DATA_WIDTH, -1));
	external2Sizer->Add(m_extVOGAD, 0, wxALL, BORDER_SIZE);

	wxStaticText* extDTMFLabel = new wxStaticText(panel, -1, _("DTMF:"), wxDefaultPosition, wxSize(LABEL_WIDTH, -1));
	external2Sizer->Add(extDTMFLabel, 0, wxALL, BORDER_SIZE);

	m_extDTMF = new wxStaticText(panel, -1, wxEmptyString, wxDefaultPosition, wxSize(DATA_WIDTH, -1));
	external2Sizer->Add(m_extDTMF, 0, wxALL, BORDER_SIZE);

	external1Sizer->Add(external2Sizer);
	panelSizer->Add(external1Sizer, 0, wxALL, BORDER_SIZE);

	wxStaticBoxSizer* timers1Sizer = new wxStaticBoxSizer(new wxStaticBox(panel, -1, _("Timers")), wxVERTICAL);
	wxFlexGridSizer* timers2Sizer = new wxFlexGridSizer(6);

	wxStaticText* timeoutLabel = new wxStaticText(panel, -1, _("Timeout:"), wxDefaultPosition, wxSize(LABEL_WIDTH, -1));
	timers2Sizer->Add(timeoutLabel, 0, wxALL, BORDER_SIZE);

	m_timeout = new wxStaticText(panel, -1, wxEmptyString, wxDefaultPosition, wxSize(DATA_WIDTH, -1));
	timers2Sizer->Add(m_timeout, 0, wxALL, BORDER_SIZE);

	wxStaticText* lockoutLabel = new wxStaticText(panel, -1, _("Lockout:"), wxDefaultPosition, wxSize(LABEL_WIDTH, -1));
	timers2Sizer->Add(lockoutLabel, 0, wxALL, BORDER_SIZE);

	m_lockout = new wxStaticText(panel, -1, wxEmptyString, wxDefaultPosition, wxSize(DATA_WIDTH, -1));
	timers2Sizer->Add(m_lockout, 0, wxALL, BORDER_SIZE);

	wxStaticText* latchLabel = new wxStaticText(panel, -1, _("Latch:"), wxDefaultPosition, wxSize(LABEL_WIDTH, -1));
	timers2Sizer->Add(latchLabel, 0, wxALL, BORDER_SIZE);

	m_latch = new wxStaticText(panel, -1, wxEmptyString, wxDefaultPosition, wxSize(DATA_WIDTH, -1));
	timers2Sizer->Add(m_latch, 0, wxALL, BORDER_SIZE);

	wxStaticText* hangLabel = new wxStaticText(panel, -1, _("Hang:"), wxDefaultPosition, wxSize(LABEL_WIDTH, -1));
	timers2Sizer->Add(hangLabel, 0, wxALL, BORDER_SIZE);

	m_hang = new wxStaticText(panel, -1, wxEmptyString, wxDefaultPosition, wxSize(DATA_WIDTH, -1));
	timers2Sizer->Add(m_hang, 0, wxALL, BORDER_SIZE);

	wxStaticText* ackLabel = new wxStaticText(panel, -1, _("Ack:"), wxDefaultPosition, wxSize(LABEL_WIDTH, -1));
	timers2Sizer->Add(ackLabel, 0, wxALL, BORDER_SIZE);

	m_ack = new wxStaticText(panel, -1, wxEmptyString, wxDefaultPosition, wxSize(DATA_WIDTH, -1));
	timers2Sizer->Add(m_ack, 0, wxALL, BORDER_SIZE);

	wxStaticText* minimumLabel = new wxStaticText(panel, -1, _("Minimum:"), wxDefaultPosition, wxSize(LABEL_WIDTH, -1));
	timers2Sizer->Add(minimumLabel, 0, wxALL, BORDER_SIZE);

	m_minimum = new wxStaticText(panel, -1, wxEmptyString, wxDefaultPosition, wxSize(DATA_WIDTH, -1));
	timers2Sizer->Add(m_minimum, 0, wxALL, BORDER_SIZE);

	wxStaticText* callsignLabel = new wxStaticText(panel, -1, _("Callsign:"), wxDefaultPosition, wxSize(LABEL_WIDTH, -1));
	timers2Sizer->Add(callsignLabel, 0, wxALL, BORDER_SIZE);

	m_callsign = new wxStaticText(panel, -1, wxEmptyString, wxDefaultPosition, wxSize(DATA_WIDTH, -1));
	timers2Sizer->Add(m_callsign, 0, wxALL, BORDER_SIZE);

	wxStaticText* activeLabel = new wxStaticText(panel, -1, _("Active:"), wxDefaultPosition, wxSize(LABEL_WIDTH, -1));
	timers2Sizer->Add(activeLabel, 0, wxALL, BORDER_SIZE);

	m_active = new wxStaticText(panel, -1, wxEmptyString, wxDefaultPosition, wxSize(DATA_WIDTH, -1));
	timers2Sizer->Add(m_active, 0, wxALL, BORDER_SIZE);

	timers1Sizer->Add(timers2Sizer);
	panelSizer->Add(timers1Sizer, 0, wxALL, BORDER_SIZE);

	wxStaticBoxSizer* log1Sizer = new wxStaticBoxSizer(new wxStaticBox(panel, -1, _("Log")), wxVERTICAL);
	wxBoxSizer* log2Sizer = new wxBoxSizer(wxVERTICAL);

	m_logLine1 = new wxStaticText(panel, -1, wxEmptyString, wxDefaultPosition, wxSize(LOGTEXT_WIDTH, -1));
	m_logLine1->Wrap(LOGTEXT_WIDTH);
	log2Sizer->Add(m_logLine1, 0, wxTOP | wxLEFT | wxRIGHT, BORDER_SIZE);

	m_logLine2 = new wxStaticText(panel, -1, wxEmptyString, wxDefaultPosition, wxSize(LOGTEXT_WIDTH, -1));
	m_logLine2->Wrap(LOGTEXT_WIDTH);
	log2Sizer->Add(m_logLine2, 0, wxLEFT | wxRIGHT, BORDER_SIZE);

	m_logLine3 = new wxStaticText(panel, -1, wxEmptyString, wxDefaultPosition, wxSize(LOGTEXT_WIDTH, -1));
	m_logLine3->Wrap(LOGTEXT_WIDTH);
	log2Sizer->Add(m_logLine3, 0, wxLEFT | wxRIGHT | wxBOTTOM, BORDER_SIZE);

	log1Sizer->Add(log2Sizer);
	panelSizer->Add(log1Sizer, 0, wxALL, BORDER_SIZE);

	panel->SetSizer(panelSizer);
	panelSizer->SetSizeHints(panel);

	mainSizer->Add(panel);

	SetSizer(mainSizer);
	mainSizer->SetSizeHints(this);

	m_timer.Start(1000);
}

CAnalogueRepeaterFrame::~CAnalogueRepeaterFrame()
{
}

wxMenuBar* CAnalogueRepeaterFrame::createMenuBar()
{
	wxMenu* fileMenu = new wxMenu();
#if defined(__WXDEBUG__)
	fileMenu->Append(wxID_OPEN, _("Open...\tCtrl+N"));
	fileMenu->AppendSeparator();
#endif
	fileMenu->Append(wxID_EXIT, _("Exit"));

	wxMenu* editMenu = new wxMenu();
	editMenu->Append(Menu_Edit_Preferences,  _("Preferences..."));

	wxMenu* viewMenu = new wxMenu();
	viewMenu->AppendCheckItem(Menu_View_Updates, _("GUI Updates"));
	viewMenu->Check(Menu_View_Updates, m_updates);

	wxMenu* actionMenu = new wxMenu();
	actionMenu->Append(Menu_Action_Shutdown,  _("Shutdown"));
	actionMenu->Append(Menu_Action_Startup,   _("Startup"));
	actionMenu->Append(Menu_Action_Timeout,   _("Timeout"));
	actionMenu->Append(Menu_Action_TimeReset, _("Time Reset"));

	m_outputMenu = new wxMenu();
	m_outputMenu->AppendCheckItem(Menu_Output_1, _("Output 1"));
	m_outputMenu->AppendCheckItem(Menu_Output_2, _("Output 2"));
	m_outputMenu->AppendCheckItem(Menu_Output_3, _("Output 3"));
	m_outputMenu->AppendCheckItem(Menu_Output_4, _("Output 4"));

	wxMenu* helpMenu = new wxMenu();
	helpMenu->Append(wxID_ABOUT, _("About Analogue Repeater"));

	wxMenuBar* menuBar = new wxMenuBar();
	menuBar->Append(fileMenu,     _("File"));
	menuBar->Append(editMenu,     _("Edit"));
	menuBar->Append(viewMenu,     _("View"));
	menuBar->Append(actionMenu,   _("Action"));
	menuBar->Append(m_outputMenu, _("Output"));
	menuBar->Append(helpMenu,     _("Help"));

	return menuBar;
}

#if defined(__WXDEBUG__)
void CAnalogueRepeaterFrame::onOpen(wxCommandEvent&)
{
	wxFileDialog dialog(this, _("Select a WAV File"), wxEmptyString, wxEmptyString, _("WAV file (*.wav)|*.wav|All files (*.*)|*.*"));
	if (dialog.ShowModal() != wxID_OK)
		return;

	wxString fileName = dialog.GetPath();

	CWAVFileReader* reader = new CWAVFileReader(fileName, ANALOGUE_RADIO_BLOCK_SIZE);
	bool res = reader->open();
	if (!res) {
		wxMessageDialog dialog(this, _("Cannot find the file, or the file is corrupt."), _("Analogue Repeater Error"), wxICON_ERROR);
		dialog.ShowModal();
		delete reader;
		return;
	}

	if (reader->getSampleRate() != ANALOGUE_RADIO_SAMPLE_RATE) {
		wxLogError(wxT("File %s has an invalid sample rate of %u"), fileName.c_str(), reader->getSampleRate());
		wxMessageDialog dialog(this, _("The sound file has an invalid sample rate."), _("Analogue Repeater Error"), wxICON_ERROR);
		dialog.ShowModal();
		reader->close();
		delete reader;
		return;
	}

	if (reader->getChannels() != 1U) {
		wxLogError(wxT("File %s has too many channels - %u"), fileName.c_str(), reader->getChannels());
		wxMessageDialog dialog(this, _("The sound file has too many channels."), _("Analogue Repeater Error"), wxICON_ERROR);
		dialog.ShowModal();
		reader->close();
		delete reader;
		return;
	}

	::wxGetApp().setReader(reader);
}
#endif

void CAnalogueRepeaterFrame::onQuit(wxCommandEvent&)
{
	Close(false);
}

void CAnalogueRepeaterFrame::onPreferences(wxCommandEvent&)
{
	wxString openId, closeId, beacon1, beacon2;
	unsigned int idSpeed, idFreq;
	wxFloat32 idLevel1, idLevel2;
	AUDIO_SOURCE openIdSource, closeIdSource, beacon1Source, beacon2Source;
	::wxGetApp().getCallsign(openIdSource, openId, closeIdSource, closeId, beacon1Source, beacon1, beacon2Source, beacon2, idSpeed, idFreq, idLevel1, idLevel2);

	wxString ackRadio, ackExternal, ackBattery;
	unsigned int ackSpeed, ackFreq, ackDelay, ackMinimum;
	wxFloat32 ackLevel;
	AUDIO_SOURCE ackRadioSource, ackExternalSource, ackBatterySource;
	::wxGetApp().getAck(ackRadioSource, ackRadio, ackExternalSource, ackExternal, ackBatterySource, ackBattery, ackSpeed, ackFreq, ackLevel, ackDelay, ackMinimum);

	unsigned int callsignTime, timeout, lockoutTime, hangTime, latchTime;
	::wxGetApp().getTimes(callsignTime, timeout, lockoutTime, hangTime, latchTime);

	bool tbEnable, ctcssInternal;
	wxFloat32 tbThreshold, ctcssFreq, ctcssThresh, ctcssLevel;
	unsigned int ctcssHangTime;
	ANALOGUE_CTCSS_OUTPUT ctcssOutput;
	::wxGetApp().getTones(tbEnable, tbThreshold, ctcssFreq, ctcssInternal, ctcssThresh, ctcssLevel, ctcssHangTime, ctcssOutput);

	ANALOGUE_CALLSIGN_START callsignAtStart;
	ANALOGUE_TIMEOUT_TYPE timeoutType;
	unsigned int callsignStartDelay;
	bool callsignAtEnd;
	ANALOGUE_CALLSIGN_HOLDOFF callsignHoldoff;
	::wxGetApp().getFeel(callsignAtStart, callsignStartDelay, callsignAtEnd, timeoutType, callsignHoldoff);

	wxString radioReadDevice, radioWriteDevice;
	unsigned int radioAudioDelay;
	bool radioDeEmphasis, radioPreEmphasis, radioVOGAD;
	::wxGetApp().getRadio(radioReadDevice, radioWriteDevice, radioAudioDelay, radioDeEmphasis, radioPreEmphasis, radioVOGAD);

	ANALOGUE_EXTERNAL_MODE externalMode;
	bool externalVOGAD, externalBackground, externalDeEmphasis, externalPreEmphasis;
	wxString externalReadDevice, externalWriteDevice, externalDevice;
	unsigned int externalAudioDelay;
	SERIALPIN externalTXPin, externalRXPin;
	::wxGetApp().getExternal(externalMode, externalReadDevice, externalWriteDevice, externalAudioDelay, externalDeEmphasis, externalPreEmphasis, externalVOGAD, externalDevice, externalTXPin, externalRXPin, externalBackground);

	wxString interfaceType;
	unsigned int pttDelay, squelchDelay, interfaceConfig;
	bool pttInvert, squelchInvert;
	::wxGetApp().getController(interfaceType, interfaceConfig, pttDelay, squelchDelay, pttInvert, squelchInvert);

	bool dtmfRadio, dtmfExternal;
	wxString dtmfShutdown, dtmfStartup, dtmfTimeout, dtmfTimeReset, dtmfOutput1, dtmfOutput2, dtmfOutput3, dtmfOutput4;
	wxString dtmfCommand1, dtmfCommand1Line, dtmfCommand2, dtmfCommand2Line;
	wxFloat32 dtmfThreshold;
	::wxGetApp().getDTMF(dtmfRadio, dtmfExternal, dtmfShutdown, dtmfStartup, dtmfTimeout, dtmfTimeReset, dtmfCommand1, dtmfCommand1Line, dtmfCommand2, dtmfCommand2Line, dtmfOutput1, dtmfOutput2, dtmfOutput3, dtmfOutput4, dtmfThreshold);

	bool aprsTxEnabled;
	wxString aprsCallsign, aprsDescription;
	wxFloat32 aprsLatitude, aprsLongitude;
	int aprsHeight;
	::wxGetApp().getAPRS(aprsTxEnabled, aprsCallsign, aprsLatitude, aprsLongitude, aprsHeight, aprsDescription);

	unsigned int activeHangTime;
	::wxGetApp().getActiveHang(activeHangTime);

	CAnalogueRepeaterPreferences dialog1(this, -1, openIdSource, openId, closeIdSource, closeId, beacon1Source,
		beacon1, beacon2Source, beacon2, idSpeed, idFreq, idLevel1, idLevel2, ackRadioSource, ackRadio,
		ackExternalSource, ackExternal, ackBatterySource, ackBattery, ackSpeed, ackFreq, ackDelay, ackMinimum,
		ackLevel, callsignTime, timeout, lockoutTime, hangTime, latchTime, tbEnable, ctcssInternal, tbThreshold,
		ctcssFreq, ctcssThresh, ctcssLevel, ctcssHangTime, ctcssOutput, callsignAtStart, callsignStartDelay, timeoutType,
		callsignAtEnd, callsignHoldoff, radioReadDevice, radioWriteDevice, radioAudioDelay, radioDeEmphasis,
		radioPreEmphasis, radioVOGAD, externalMode, externalVOGAD, externalReadDevice, externalWriteDevice, 
		externalAudioDelay, externalDeEmphasis, externalPreEmphasis, externalDevice, externalTXPin,
		externalRXPin, externalBackground, interfaceType, interfaceConfig, pttDelay, squelchDelay, pttInvert,
		squelchInvert, dtmfRadio, dtmfExternal, dtmfShutdown, dtmfStartup, dtmfTimeout, dtmfTimeReset,
		dtmfOutput1, dtmfOutput2, dtmfOutput3, dtmfOutput4, dtmfCommand1, dtmfCommand1Line, dtmfCommand2,
		dtmfCommand2Line, dtmfThreshold, aprsTxEnabled, aprsCallsign, aprsLatitude, aprsLongitude, aprsHeight,
		aprsDescription, activeHangTime);
	if (dialog1.ShowModal() != wxID_OK)
		return;

	openIdSource   = dialog1.getOpenIdSource();
	openId         = dialog1.getOpenId();
	closeIdSource  = dialog1.getCloseIdSource();
	closeId        = dialog1.getCloseId();
	beacon1Source  = dialog1.getBeacon1Source();
	beacon1        = dialog1.getBeacon1();
	beacon2Source  = dialog1.getBeacon2Source();
	beacon2        = dialog1.getBeacon2();
	idSpeed        = dialog1.getIdSpeed();
	idFreq         = dialog1.getIdFreq();
	idLevel1       = dialog1.getIdLevel1();
	idLevel2       = dialog1.getIdLevel2();
	::wxGetApp().setCallsign(openIdSource, openId, closeIdSource, closeId, beacon1Source, beacon1, beacon2Source, beacon2, idSpeed, idFreq, idLevel1, idLevel2);

	ackRadioSource    = dialog1.getRadioAckSource();
	ackRadio          = dialog1.getRadioAck();
	ackExternalSource = dialog1.getExternalAckSource();
	ackExternal       = dialog1.getExternalAck();
	ackBatterySource  = dialog1.getBatteryAckSource();
	ackBattery        = dialog1.getBatteryAck();
	ackSpeed          = dialog1.getAckSpeed();
	ackFreq           = dialog1.getAckFreq();
	ackLevel          = dialog1.getAckLevel();
	ackDelay          = dialog1.getAckDelay();
	ackMinimum        = dialog1.getAckMinimum();
	::wxGetApp().setAck(ackRadioSource, ackRadio, ackExternalSource, ackExternal, ackBatterySource, ackBattery, ackSpeed, ackFreq, ackLevel, ackDelay, ackMinimum);

	callsignTime = dialog1.getCallsignTime();
	timeout      = dialog1.getTimeout();
	lockoutTime  = dialog1.getLockoutTime();
	hangTime     = dialog1.getHangTime();
	latchTime    = dialog1.getLatchTime();
	::wxGetApp().setTimes(callsignTime, timeout, lockoutTime, hangTime, latchTime);

	tbEnable      = dialog1.getTbEnable();
	tbThreshold   = dialog1.getTbThreshold();
	ctcssInternal = dialog1.getCTCSSInternal();
	ctcssFreq     = dialog1.getCTCSSFreq();
	ctcssThresh   = dialog1.getCTCSSThresh();
	ctcssLevel    = dialog1.getCTCSSLevel();
	ctcssHangTime = dialog1.getCTCSSHangTime();
	ctcssOutput   = dialog1.getCTCSSOutput();
	::wxGetApp().setTones(tbEnable, tbThreshold, ctcssFreq, ctcssInternal, ctcssThresh, ctcssLevel, ctcssHangTime, ctcssOutput);

	callsignAtStart    = dialog1.getCallAtStart();
	callsignStartDelay = dialog1.getCallStartDelay();
	callsignAtEnd      = dialog1.getCallAtEnd();
	timeoutType        = dialog1.getTimeoutType();
	callsignHoldoff    = dialog1.getHoldoff();
	::wxGetApp().setFeel(callsignAtStart, callsignStartDelay, callsignAtEnd, timeoutType, callsignHoldoff);

	radioReadDevice  = dialog1.getRadioReadDevice();
	radioWriteDevice = dialog1.getRadioWriteDevice();
	radioAudioDelay  = dialog1.getRadioAudioDelay();
	radioDeEmphasis  = dialog1.getRadioDeEmphasis();
	radioPreEmphasis = dialog1.getRadioPreEmphasis();
	radioVOGAD       = dialog1.getRadioVOGAD();
	::wxGetApp().setRadio(radioReadDevice, radioWriteDevice, radioAudioDelay, radioDeEmphasis, radioPreEmphasis, radioVOGAD);

	externalMode        = dialog1.getExternalMode();
	externalReadDevice  = dialog1.getExternalReadDevice();
	externalWriteDevice = dialog1.getExternalWriteDevice();
	externalAudioDelay  = dialog1.getExternalAudioDelay();
	externalDeEmphasis  = dialog1.getExternalDeEmphasis();
	externalPreEmphasis = dialog1.getExternalPreEmphasis();
	externalVOGAD       = dialog1.getExternalVOGAD();
	externalDevice      = dialog1.getExternalDevice();
	externalTXPin       = dialog1.getExternalTXPin();
	externalRXPin       = dialog1.getExternalRXPin();
	externalBackground  = dialog1.getExternalBackground();
	::wxGetApp().setExternal(externalMode, externalReadDevice, externalWriteDevice, externalAudioDelay, externalDeEmphasis, externalPreEmphasis, externalVOGAD, externalDevice, externalTXPin, externalRXPin, externalBackground);

	interfaceType   = dialog1.getInterfaceType();
	interfaceConfig = dialog1.getInterfaceConfig();
	pttDelay        = dialog1.getPTTDelay();
	squelchDelay    = dialog1.getSquelchDelay();
	pttInvert       = dialog1.getPTTInvert();
	squelchInvert   = dialog1.getSquelchInvert();
	::wxGetApp().setController(interfaceType, interfaceConfig, pttDelay, squelchDelay, pttInvert, squelchInvert);

	dtmfRadio        = dialog1.getDTMFRadio();
	dtmfExternal     = dialog1.getDTMFExternal();
	dtmfShutdown     = dialog1.getDTMFShutdown();
	dtmfStartup      = dialog1.getDTMFStartup();
	dtmfTimeout      = dialog1.getDTMFTimeout();
	dtmfTimeReset    = dialog1.getDTMFTimeReset();
	dtmfCommand1     = dialog1.getDTMFCommand1();
	dtmfCommand1Line = dialog1.getDTMFCommand1Line();
	dtmfCommand2     = dialog1.getDTMFCommand2();
	dtmfCommand2Line = dialog1.getDTMFCommand2Line();
	dtmfOutput1      = dialog1.getDTMFOutput1();
	dtmfOutput2      = dialog1.getDTMFOutput2();
	dtmfOutput3      = dialog1.getDTMFOutput3();
	dtmfOutput4      = dialog1.getDTMFOutput4();
	dtmfThreshold    = dialog1.getDTMFThreshold();
	::wxGetApp().setDTMF(dtmfRadio, dtmfExternal, dtmfShutdown, dtmfStartup, dtmfTimeout, dtmfTimeReset, dtmfCommand1, dtmfCommand1Line, dtmfCommand2, dtmfCommand2Line, dtmfOutput1, dtmfOutput2, dtmfOutput3, dtmfOutput4, dtmfThreshold);

	aprsTxEnabled   = dialog1.getAPRSTXEnabled();
	aprsCallsign    = dialog1.getAPRSCallsign();
	aprsLatitude    = dialog1.getAPRSLatitude();
	aprsLongitude   = dialog1.getAPRSLongitude();
	aprsHeight      = dialog1.getAPRSHeight();
	aprsDescription = dialog1.getAPRSDescription();
	::wxGetApp().setAPRS(aprsTxEnabled, aprsCallsign, aprsLatitude, aprsLongitude, aprsHeight, aprsDescription);

	activeHangTime = dialog1.getActiveHangTime();
	::wxGetApp().setActiveHang(activeHangTime);

	::wxGetApp().writeConfig();

	wxMessageDialog dialog2(this, _("The changes made will not take effect\nuntil the application is restarted"), _("Analogue Repeater Information"), wxICON_INFORMATION);
	dialog2.ShowModal();
}

void CAnalogueRepeaterFrame::onClose(wxCloseEvent&)
{
	int x, y;
	GetPosition(&x, &y);
	if (x >= 0 && y >= 0) {
		::wxGetApp().setPosition(x, y);
		::wxGetApp().writeConfig();
	}

	Destroy();
}

void CAnalogueRepeaterFrame::onUpdates(wxCommandEvent& event)
{
	m_updates = event.IsChecked();
}

void CAnalogueRepeaterFrame::onAbout(wxCommandEvent&)
{
	wxAboutDialogInfo info;
	info.AddDeveloper(wxT("Jonathan Naylor, G4KLX"));
	info.SetCopyright(wxT("(C) 2009-2014 using GPL v2 or later"));
	info.SetName(APPLICATION_NAME);
	info.SetVersion(VERSION);
	info.SetDescription(_("This program allows a computer with a soundcard to act as a repeater."));

	::wxAboutBox(info);
}

void CAnalogueRepeaterFrame::onActions(wxCommandEvent& event)
{
	int id = event.GetId();

	switch (id) {
		case Menu_Action_Shutdown:
			::wxGetApp().shutdown();
			break;
		case Menu_Action_Startup:
			::wxGetApp().startup();
			break;
		case Menu_Action_Timeout:
			::wxGetApp().timeout();
			break;
		case Menu_Action_TimeReset:
			::wxGetApp().timeReset();
			break;
		case Menu_Action_Command1:
			::wxGetApp().command1();
			break;
		case Menu_Action_Command2:
			::wxGetApp().command2();
			break;
	}
}

void CAnalogueRepeaterFrame::onOutputs(wxCommandEvent&)
{
	bool out1 = m_outputMenu->IsChecked(Menu_Output_1);
	bool out2 = m_outputMenu->IsChecked(Menu_Output_2);
	bool out3 = m_outputMenu->IsChecked(Menu_Output_3);
	bool out4 = m_outputMenu->IsChecked(Menu_Output_4);

	::wxGetApp().setOutputs(out1, out2, out3, out4);
}

void CAnalogueRepeaterFrame::onTimer(wxTimerEvent&)
{
	if (!m_updates)
		return;

	CAnalogueRepeaterStatusData* status = ::wxGetApp().getStatus();
	if (status == NULL)
		return;

	bool radioTransmit = status->isRadioTransmit();
	m_radioTransmit->SetLabel(radioTransmit ? _("On") : _("Off"));

	switch (status->getSquelch()) {
		case AS_CLOSED:
		case AS_EXTERNAL:
			m_radioSquelch->SetLabel(_("Closed"));
			break;
		default:
			m_radioSquelch->SetLabel(_("Open"));
			break;
	}

	bool battery = status->isBattery();
	m_battery->SetLabel(battery ? _("Yes") : _("No"));

	wxString buffer;
	wxFloat32 level = status->getLevel();
	if (level == 0.0F)
		level = 1.0F;
	buffer.Printf(wxT("%.1f dB"), 20.0F * ::log10(level));
	m_level->SetLabel(buffer);

	wxFloat32 vogad = status->getRadioVOGAD();
	if (vogad == 0.0F)
		vogad = 1.0F;
	buffer.Printf(wxT("%.1f dB"), 20.0F * ::log10(vogad));
	m_radioVOGAD->SetLabel(buffer);

	vogad = status->getExternalVOGAD();
	if (vogad == 0.0F)
		vogad = 1.0F;
	buffer.Printf(wxT("%.1f dB"), 20.0F * ::log10(vogad));
	m_extVOGAD->SetLabel(buffer);

	wxString dtmf = status->getRadioDTMF();
	m_radioDTMF->SetLabel(dtmf);

	dtmf = status->getExternalDTMF();
	m_extDTMF->SetLabel(dtmf);

	ANALOGUE_RPT_STATE state = status->getState();
	switch (state) {
		case ARS_SHUTDOWN:
			m_state->SetLabel(_("Shutdown"));
			break;
		case ARS_LISTENING:
			m_state->SetLabel(_("Listening"));
			break;
		case ARS_RELAYING_RADIO:
			m_state->SetLabel(_("Relaying Radio"));
			break;
		case ARS_RELAYING_EXTERNAL:
			m_state->SetLabel(_("Relaying External"));
			break;
		case ARS_WAITING_RADIO:
		case ARS_WAITING_EXTERNAL:
			m_state->SetLabel(_("Waiting"));
			break;
		case ARS_TIMEOUT_RADIO:
		case ARS_TIMEOUT_EXTERNAL:
			m_state->SetLabel(_("Timeout"));
			break;
		case ARS_LOCKOUT_RADIO:
		case ARS_LOCKOUT_EXTERNAL:
			m_state->SetLabel(_("Lockout"));
			break;
	}

	bool toneburst = status->getSquelch() == AS_TONEBURST;
	m_toneburst->SetLabel(toneburst ? _("Yes") : _("No"));

	bool ctcss = status->getSquelch() == AS_CTCSS;
	m_ctcss->SetLabel(ctcss ? _("Yes") : _("No"));

	buffer.Printf(wxT("%u/%u"), status->getTimeoutTimer(), status->getTimeoutExpiry());
	m_timeout->SetLabel(buffer);

	buffer.Printf(wxT("%u/%u"), status->getLockoutTimer(), status->getLockoutExpiry());
	m_lockout->SetLabel(buffer);

	buffer.Printf(wxT("%u/%u"), status->getHangTimer(), status->getHangExpiry());
	m_hang->SetLabel(buffer);

	buffer.Printf(wxT("%u/%u"), status->getLatchTimer(), status->getLatchExpiry());
	m_latch->SetLabel(buffer);

	buffer.Printf(wxT("%u/%u"), status->getAckTimer(), status->getAckExpiry());
	m_ack->SetLabel(buffer);

	buffer.Printf(wxT("%u/%u"), status->getMinimumTimer(), status->getMinimumExpiry());
	m_minimum->SetLabel(buffer);

	buffer.Printf(wxT("%u/%u"), status->getCallsignTimer(), status->getCallsignExpiry());
	m_callsign->SetLabel(buffer);

	buffer.Printf(wxT("%u/%u"), status->getActiveTimer(), status->getActiveExpiry());
	m_active->SetLabel(buffer);

	bool extSquelch = status->getSquelch() == AS_EXTERNAL;
	m_extSquelch->SetLabel(extSquelch ? _("Open") : _("Closed"));

	bool extTransmit = status->isExternalTransmit();
	m_extTransmit->SetLabel(extTransmit ? _("On") : _("Off"));

	delete status;
}

void CAnalogueRepeaterFrame::onLog(wxEvent& event)
{
	CLogEvent& logEvent = dynamic_cast<CLogEvent&>(event);

	wxString text = logEvent.getText();

	m_logLine1->SetLabel(m_logLine2->GetLabel());
	m_logLine2->SetLabel(m_logLine3->GetLabel());
	m_logLine3->SetLabel(text);
}

void CAnalogueRepeaterFrame::showLog(const wxString& text)
{
	if (!m_updates)
		return;

	CLogEvent event(text, LOG_EVENT);

	AddPendingEvent(event);
}

void CAnalogueRepeaterFrame::setOutputs(bool out1, bool out2, bool out3, bool out4)
{
	m_outputMenu->Check(Menu_Output_1, out1);
	m_outputMenu->Check(Menu_Output_2, out2);
	m_outputMenu->Check(Menu_Output_3, out3);
	m_outputMenu->Check(Menu_Output_4, out4);
}
