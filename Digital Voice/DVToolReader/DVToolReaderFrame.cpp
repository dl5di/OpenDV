/*
 *   Copyright (C) 2009,2010 by Jonathan Naylor G4KLX
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

#include "DVToolReaderFrame.h"
#include "DVToolReaderApp.h"
#include "DVToolReaderPreferences.h"
#include "DVTOOLFileReader.h"
#include "DStarDefines.h"
#include "MessageEvent.h"
#include "HeaderEvent.h"
#include "ErrorEvent.h"
#include "Version.h"

#if defined(__WXGTK__) || defined(__WXMAC__)
#include "DVToolReader.xpm"
#endif

#include <wx/gbsizer.h>
#include <wx/aboutdlg.h>

enum {
	Menu_Edit_Preferences = 6000
};

DEFINE_EVENT_TYPE(MESSAGE_EVENT)
DEFINE_EVENT_TYPE(HEADER_EVENT)
DEFINE_EVENT_TYPE(ERROR_EVENT)

BEGIN_EVENT_TABLE(CDVToolReaderFrame, wxFrame)
	EVT_MENU(wxID_OPEN, CDVToolReaderFrame::onOpen)
	EVT_MENU(wxID_EXIT, CDVToolReaderFrame::onQuit)
	EVT_MENU(Menu_Edit_Preferences, CDVToolReaderFrame::onPreferences)
	EVT_MENU(wxID_ABOUT, CDVToolReaderFrame::onAbout)

	EVT_CLOSE(CDVToolReaderFrame::onClose)

	EVT_CUSTOM(MESSAGE_EVENT, wxID_ANY, CDVToolReaderFrame::onMessage)
	EVT_CUSTOM(HEADER_EVENT,  wxID_ANY, CDVToolReaderFrame::onHeader)
	EVT_CUSTOM(ERROR_EVENT,   wxID_ANY, CDVToolReaderFrame::onError)
END_EVENT_TABLE()

CDVToolReaderFrame::CDVToolReaderFrame(const wxString& title) :
wxFrame(NULL, -1, title),
m_fileName(NULL),
m_your(NULL),
m_my(NULL),
m_rpt1(NULL),
m_rpt2(NULL),
m_flags(NULL),
m_message(NULL)
{
	SetIcon(wxICON(DVToolReader));

	SetMenuBar(createMenuBar());

	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

	wxPanel* panel = new wxPanel(this);

	wxGridBagSizer* panelSizer = new wxGridBagSizer(BORDER_SIZE, BORDER_SIZE);

	wxStaticText* fileNameLabel = new wxStaticText(panel, -1, _("File Name:"), wxDefaultPosition, wxSize(LABEL_WIDTH, -1), wxALIGN_RIGHT);
	panelSizer->Add(fileNameLabel, wxGBPosition(0, 0), wxDefaultSpan, wxALL, BORDER_SIZE);

	m_fileName = new wxStaticText(panel, -1, wxEmptyString, wxDefaultPosition, wxSize(LABEL_WIDTH, -1), wxALIGN_RIGHT);
	panelSizer->Add(m_fileName, wxGBPosition(0, 1), wxGBSpan(1, 5), wxALL, BORDER_SIZE);

	wxStaticText* yourLabel = new wxStaticText(panel, -1, _("Your:"), wxDefaultPosition, wxSize(LABEL_WIDTH, -1), wxALIGN_RIGHT);
	panelSizer->Add(yourLabel, wxGBPosition(1, 0), wxDefaultSpan, wxALL, BORDER_SIZE);

	m_your = new wxStaticText(panel, -1, wxEmptyString, wxDefaultPosition, wxSize(CONTROL_WIDTH, -1));
	panelSizer->Add(m_your, wxGBPosition(1, 1), wxDefaultSpan, wxALL, BORDER_SIZE);

	wxStaticText* rpt1Label = new wxStaticText(panel, -1, _("RPT1:"), wxDefaultPosition, wxSize(LABEL_WIDTH, -1), wxALIGN_RIGHT);
	panelSizer->Add(rpt1Label, wxGBPosition(1, 2), wxDefaultSpan, wxALL, BORDER_SIZE);

	m_rpt1 = new wxStaticText(panel, -1, wxEmptyString, wxDefaultPosition, wxSize(CONTROL_WIDTH, -1));
	panelSizer->Add(m_rpt1, wxGBPosition(1, 3), wxDefaultSpan, wxALL, BORDER_SIZE);

	wxStaticText* rpt2Label = new wxStaticText(panel, -1, _("RPT2:"), wxDefaultPosition, wxSize(LABEL_WIDTH, -1), wxALIGN_RIGHT);
	panelSizer->Add(rpt2Label, wxGBPosition(1, 4), wxDefaultSpan, wxALL, BORDER_SIZE);

	m_rpt2 = new wxStaticText(panel, -1, wxEmptyString, wxDefaultPosition, wxSize(CONTROL_WIDTH, -1));
	panelSizer->Add(m_rpt2, wxGBPosition(1, 5), wxDefaultSpan, wxALL, BORDER_SIZE);

	wxStaticText* myLabel = new wxStaticText(panel, -1, _("My:"), wxDefaultPosition, wxSize(LABEL_WIDTH, -1), wxALIGN_RIGHT);
	panelSizer->Add(myLabel, wxGBPosition(2, 0), wxDefaultSpan, wxALL, BORDER_SIZE);

	m_my = new wxStaticText(panel, -1, wxEmptyString, wxDefaultPosition, wxSize(CONTROL_WIDTH, -1));
	panelSizer->Add(m_my, wxGBPosition(2, 1), wxDefaultSpan, wxALL, BORDER_SIZE);

	wxStaticText* flagsLabel = new wxStaticText(panel, -1, _("Flags:"), wxDefaultPosition, wxSize(LABEL_WIDTH, -1), wxALIGN_RIGHT);
	panelSizer->Add(flagsLabel, wxGBPosition(2, 2), wxDefaultSpan, wxALL, BORDER_SIZE);

	m_flags = new wxStaticText(panel, -1, wxEmptyString, wxDefaultPosition, wxSize(LABEL_WIDTH * 3U, -1), wxALIGN_RIGHT);
	panelSizer->Add(m_flags, wxGBPosition(2, 3), wxGBSpan(1, 3), wxALL, BORDER_SIZE);

	wxStaticText* messageLabel = new wxStaticText(panel, -1, _("Message:"), wxDefaultPosition, wxSize(LABEL_WIDTH, -1), wxALIGN_RIGHT);
	panelSizer->Add(messageLabel, wxGBPosition(3, 0), wxDefaultSpan, wxALL, BORDER_SIZE);

	m_message = new wxStaticText(panel, -1, wxEmptyString, wxDefaultPosition, wxSize(LABEL_WIDTH * 3U, -1), wxALIGN_RIGHT);
	panelSizer->Add(m_message, wxGBPosition(3, 1), wxGBSpan(1, 3), wxALL, BORDER_SIZE);

	panel->SetSizer(panelSizer);
	panelSizer->SetSizeHints(panel);

	mainSizer->Add(panel);

	SetSizer(mainSizer);
	mainSizer->SetSizeHints(this);
}

CDVToolReaderFrame::~CDVToolReaderFrame()
{
}

wxMenuBar* CDVToolReaderFrame::createMenuBar()
{
	wxMenu* fileMenu = new wxMenu();
	fileMenu->Append(wxID_OPEN, _("Open...\tCtrl+N"));
	fileMenu->AppendSeparator();
	fileMenu->Append(wxID_EXIT, _("Exit"));

	wxMenu* editMenu = new wxMenu();
	editMenu->Append(Menu_Edit_Preferences, _("Preferences..."));

	wxMenu* helpMenu = new wxMenu();
	helpMenu->Append(wxID_ABOUT, _("About DVTool Reader"));

	wxMenuBar* menuBar = new wxMenuBar();
	menuBar->Append(fileMenu, _("File"));
	menuBar->Append(editMenu, _("Edit"));
	menuBar->Append(helpMenu, _("Help"));

	return menuBar;
}

void CDVToolReaderFrame::showMessage(CMessageData* message)
{
	wxASSERT(message != NULL);

	CMessageEvent event(message, MESSAGE_EVENT);

	AddPendingEvent(event);
}

void CDVToolReaderFrame::showHeader(CHeaderData* header)
{
	CHeaderEvent event(header, HEADER_EVENT);

	AddPendingEvent(event);
}

void CDVToolReaderFrame::error(const wxString& error)
{
	CErrorEvent event(error, ERROR_EVENT);

	AddPendingEvent(event);
}

void CDVToolReaderFrame::onQuit(wxCommandEvent& event)
{
	Close(false);
}

void CDVToolReaderFrame::onOpen(wxCommandEvent& event)
{
	wxFileDialog dialog(this, _("Select a DVTOOL File"), wxEmptyString, wxEmptyString, _("DVTOOL file (*.dvtool)|*.dvtool|All files (*.*)|*.*"));
	if (dialog.ShowModal() != wxID_OK)
		return;

	wxString fileName = dialog.GetPath();

	CDVTOOLFileReader* reader = new CDVTOOLFileReader;
	bool res = reader->open(fileName);
	if (!res) {
		wxMessageDialog dialog(this, _("Cannot find the file, or the file is corrupt."), _("DVTool Reader Error"), wxICON_ERROR);
		dialog.ShowModal();
		delete reader;
		return;
	}

	res = ::wxGetApp().setReader(reader);
	if (!res) {
		wxMessageDialog dialog(this, _("Cannot process the file."), _("DVTool Reader Error"), wxICON_ERROR);
		dialog.ShowModal();
		delete reader;
		return;
	}

	fileName = dialog.GetFilename();
	m_fileName->SetLabel(fileName);
}

void CDVToolReaderFrame::onPreferences(wxCommandEvent& event)
{
	wxString oldDevice;
	::wxGetApp().getDVDongle(oldDevice);

	wxString oldSoundcard;
	::wxGetApp().getSoundcard(oldSoundcard);

	bool oldBleep;
	::wxGetApp().getBleep(oldBleep);

	CDVToolReaderPreferences dialog(this, -1, oldDevice, oldSoundcard, oldBleep);
	if (dialog.ShowModal() != wxID_OK)
		return;

	wxString newDevice    = dialog.getDevice();
	wxString newSoundcard = dialog.getSoundcard();
	bool     newBleep     = dialog.getBleep();

	bool changed = false;

	if (!newDevice.IsSameAs(oldDevice)) {
		::wxGetApp().setDVDongle(newDevice);
		changed = true;
	}

	if (!newSoundcard.IsSameAs(oldSoundcard)) {
		::wxGetApp().setSoundcard(newSoundcard);
		changed = true;
	}

	if (newBleep != oldBleep)
		::wxGetApp().setBleep(newBleep);

	if (changed) {
		wxMessageDialog dialog(this, _("The changes made will not take effect\nuntil the application is restarted"), _("DVTool Reader Information"), wxICON_INFORMATION);
		dialog.ShowModal();
	}
}

void CDVToolReaderFrame::onAbout(wxCommandEvent& event)
{
	wxAboutDialogInfo info;
	info.AddDeveloper(wxT("Jonathan Naylor, G4KLX"));
	info.SetName(APPLICATION_NAME);
	info.SetVersion(VERSION);
	info.SetDescription(_("This program reads .dvtool files created\nby D-Star Client or DVTOOL and displays\nthe enclosed information and plays the\nAMBE audio."));

	::wxAboutBox(info);
}

void CDVToolReaderFrame::onClose(wxCloseEvent& event)
{
	Destroy();
}

void CDVToolReaderFrame::onMessage(wxEvent& event)
{
	CMessageEvent& msgEvent = dynamic_cast<CMessageEvent&>(event);

	CMessageData* message = msgEvent.getMessageData();
	wxASSERT(message != NULL);

	wxString msg = message->getMsgText();

	m_message->SetLabel(msg);

	delete message;
}

void CDVToolReaderFrame::onHeader(wxEvent& event)
{
	CHeaderEvent& hdrEvent = dynamic_cast<CHeaderEvent&>(event);

	CHeaderData* header = hdrEvent.getHeaderData();

	if (header != NULL) {
		wxDateTime dateTime = header->getTime();
		wxString hrdDateTime = dateTime.FormatISODate() + wxT(" ") + dateTime.FormatISOTime();

		wxString urCall = header->getYourCall();
		m_your->SetLabel(urCall);

		wxString myCall = header->getMyCall1();

		wxString myCall2 = header->getMyCall2();
		if (!myCall2.IsSameAs(wxT("    "))) {
			myCall.Append(wxT("/"));
			myCall.Append(myCall2);
		}

		m_my->SetLabel(myCall);

		wxString rpt1Call = header->getRptCall1();
		m_rpt1->SetLabel(rpt1Call);

		wxString rpt2Call = header->getRptCall2();
		m_rpt2->SetLabel(rpt2Call);

		wxString flags;
		flags.Printf(wxT("%02X %02X %02X"), header->getFlag1(), header->getFlag2(), header->getFlag3());
		m_flags->SetLabel(flags);

		m_message->SetLabel(wxEmptyString);

		delete header;
	} else {
		m_fileName->SetLabel(wxEmptyString);

		m_your->SetLabel(wxEmptyString);
		m_my->SetLabel(wxEmptyString);
		m_rpt1->SetLabel(wxEmptyString);
		m_rpt2->SetLabel(wxEmptyString);
		m_flags->SetLabel(wxEmptyString);
		m_message->SetLabel(wxEmptyString);
	}
}

void CDVToolReaderFrame::onError(wxEvent& event)
{
	CErrorEvent& errEvent = dynamic_cast<CErrorEvent&>(event);

	wxString text = errEvent.getText();

	wxMessageDialog dialog(this, text, _("DVTool Reader Error"), wxICON_ERROR);
	dialog.ShowModal();
}
