/*
 *   Copyright (C) 2012 by Jonathan Naylor G4KLX
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

#include "DCSClientPreferences.h"
#include "DCSClientFrame.h"
#include "DCSClientApp.h"
#include "MessageEvent.h"
#include "DStarDefines.h"
#include "HeaderEvent.h"
#include "BusyEvent.h"
#include "Version.h"

#if defined(__WXGTK__) || defined(__WXMAC__)
#include "DCSClient.xpm"
#endif

#include <wx/aboutdlg.h>

enum {
	Menu_File_Recording = 6000,
	Menu_Edit_Preferences
};

DEFINE_EVENT_TYPE(MESSAGE_EVENT)
DEFINE_EVENT_TYPE(HEADER_EVENT)
DEFINE_EVENT_TYPE(ERROR_EVENT)

BEGIN_EVENT_TABLE(CDCSClientFrame, wxFrame)
	EVT_MENU(Menu_File_Recording, CDCSClientFrame::onRecording)
	EVT_MENU(wxID_EXIT, CDCSClientFrame::onQuit)
	EVT_MENU(Menu_Edit_Preferences, CDCSClientFrame::onPreferences)
	EVT_MENU(wxID_ABOUT, CDCSClientFrame::onAbout)

	EVT_CLOSE(CDCSClientFrame::onClose)

	EVT_CUSTOM(MESSAGE_EVENT, wxID_ANY, CDCSClientFrame::onMessage)
	EVT_CUSTOM(HEADER_EVENT,  wxID_ANY, CDCSClientFrame::onHeader)
	EVT_CUSTOM(ERROR_EVENT,   wxID_ANY, CDCSClientFrame::onError)
END_EVENT_TABLE()

const unsigned int BORDER_SIZE    = 5U;

CDCSClientFrame::CDCSClientFrame(const wxString& title) :
wxFrame(NULL, -1, title),
m_panel(NULL)
{
	SetIcon(wxICON(DCSClient));

	SetMenuBar(createMenuBar());

	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

	m_panel = new CDCSClientPanel(this, -1);

	mainSizer->Add(m_panel);

	SetSizer(mainSizer);
	mainSizer->SetSizeHints(this);
}

CDCSClientFrame::~CDCSClientFrame()
{
}

wxMenuBar* CDCSClientFrame::createMenuBar()
{
	wxMenu* fileMenu = new wxMenu();
	fileMenu->AppendCheckItem(Menu_File_Recording, _("&Record data\tCtrl+R"));
	fileMenu->AppendSeparator();
	fileMenu->Append(wxID_EXIT, _("Exit"));

	wxMenu* editMenu = new wxMenu();
	editMenu->Append(Menu_Edit_Preferences,  _("Preferences..."));

	wxMenu* helpMenu = new wxMenu();
	helpMenu->Append(wxID_ABOUT, _("About DCS Client"));

	wxMenuBar* menuBar = new wxMenuBar();
	menuBar->Append(fileMenu, _("File"));
	menuBar->Append(editMenu, _("Edit"));
	menuBar->Append(helpMenu, _("Help"));

	return menuBar;
}

void CDCSClientFrame::showMessage(CMessageData* message)
{
	wxASSERT(message != NULL);

	CMessageEvent event(message, MESSAGE_EVENT);

	AddPendingEvent(event);
}

void CDCSClientFrame::showHeader(CHeaderData* header)
{
	CHeaderEvent event(header, HEADER_EVENT);

	AddPendingEvent(event);
}

void CDCSClientFrame::error(const wxString& error)
{
	CErrorEvent event(error, ERROR_EVENT);

	AddPendingEvent(event);
}

void CDCSClientFrame::onRecording(wxCommandEvent& event)
{
	bool recording = event.IsChecked();

	::wxGetApp().setRecording(recording);
}

void CDCSClientFrame::onQuit(wxCommandEvent& event)
{
	Close(false);
}

void CDCSClientFrame::onClose(wxCloseEvent& event)
{
	if (!event.CanVeto()) {
		Destroy();
		return;
	}

	int reply = ::wxMessageBox(_("Do you want to exit DCS Client"), _("Exit DCS Client"), wxOK | wxCANCEL | wxICON_QUESTION);
	switch (reply) {
		case wxOK:
			Destroy();
			break;
		case wxCANCEL:
			event.Veto();
			break;
	}
}

void CDCSClientFrame::onPreferences(wxCommandEvent& event)
{
	wxString oldCallsign;
	::wxGetApp().getCallsign(oldCallsign);

	wxString oldReadDevice, oldWriteDevice;
	::wxGetApp().getSoundcard(oldReadDevice, oldWriteDevice);

	wxString oldDVDDevice;
	::wxGetApp().getDVDongle(oldDVDDevice);

	wxString oldMessage;
	::wxGetApp().getMessage(oldMessage);

	bool oldBleep;
	::wxGetApp().getBleep(oldBleep);

	wxString oldIfaceType;
	unsigned int oldAudioDelay;
	bool oldSquelchInvert, oldPTTInvert;
	::wxGetApp().getController(oldIfaceType, oldAudioDelay, oldSquelchInvert, oldPTTInvert);

	wxString oldStartupReflector;
	::wxGetApp().getStartup(oldStartupReflector);

	CDCSClientPreferences dialog(this, -1, oldCallsign, oldReadDevice, oldWriteDevice, oldDVDDevice, oldMessage, oldBleep, oldIfaceType, oldAudioDelay, oldPTTInvert, oldSquelchInvert, oldStartupReflector);
	if (dialog.ShowModal() != wxID_OK)
		return;

	wxString newCallsign    = dialog.getCallsign();
	wxString newReadDevice  = dialog.getReadDevice();
	wxString newWriteDevice = dialog.getWriteDevice();
	wxString newDVDDevice   = dialog.getDVDDevice();
	wxString newMessage     = dialog.getMessage();
	bool     newBleep       = dialog.getBleep();

	wxString     newIfaceType     = dialog.getIfaceType();
	unsigned int newAudioDelay    = dialog.getAudioDelay();
	bool         newPTTInvert     = dialog.getPTTInvert();
	bool         newSquelchInvert = dialog.getSquelchInvert();

	wxString     newStartupReflector = dialog.getStartupReflector();

	bool changed = false;

	if (!newCallsign.IsSameAs(oldCallsign))
		::wxGetApp().setCallsign(newCallsign);

	if (!newReadDevice.IsSameAs(oldReadDevice) || !newWriteDevice.IsSameAs(oldWriteDevice)) {
		::wxGetApp().setSoundcard(newReadDevice, newWriteDevice);
		changed = true;
	}

	if (!newDVDDevice.IsSameAs(oldDVDDevice)) {
		::wxGetApp().setDVDongle(newDVDDevice);
		changed = true;
	}

	if (!newMessage.IsSameAs(oldMessage))
		::wxGetApp().setMessage(newMessage);

	if (newBleep != oldBleep)
		::wxGetApp().setBleep(newBleep);

	if (!newIfaceType.IsSameAs(oldIfaceType) || newAudioDelay != oldAudioDelay || newPTTInvert != oldPTTInvert || newSquelchInvert != oldSquelchInvert) {
		::wxGetApp().setController(newIfaceType, newAudioDelay, newSquelchInvert, newPTTInvert);
		changed = true;
	}

	if (!newStartupReflector.IsSameAs(oldStartupReflector)) {
		::wxGetApp().setStartup(newStartupReflector);
		changed = true;
	}

	if (changed) {
		wxMessageDialog dialog(this, _("The changes made will not take effect\nuntil the application is restarted"), _("D-Star Client Information"), wxICON_INFORMATION);
		dialog.ShowModal();
	}
}

void CDCSClientFrame::onAbout(wxCommandEvent& event)
{
	wxAboutDialogInfo info;
	info.AddDeveloper(wxT("Jonathan Naylor, G4KLX"));
	info.SetName(APPLICATION_NAME);
	info.SetVersion(VERSION);
	info.SetDescription(_("This program allows a computer with a DV Dongle\nand an Internet connection to connect to DCS reflectors."));

	::wxAboutBox(info);
}

void CDCSClientFrame::onMessage(wxEvent& event)
{
	CMessageEvent& msgEvent = dynamic_cast<CMessageEvent&>(event);

	CMessageData* message = msgEvent.getMessageData();
	wxASSERT(message != NULL);

	m_panel->onMessage(message);
}

void CDCSClientFrame::onHeader(wxEvent& event)
{
	CHeaderEvent& hdrEvent = dynamic_cast<CHeaderEvent&>(event);

	CHeaderData* header = hdrEvent.getHeaderData();

	m_panel->onHeader(header);
}

void CDCSClientFrame::onError(wxEvent& event)
{
	CErrorEvent& errEvent = dynamic_cast<CErrorEvent&>(event);

	wxString text = errEvent.getText();

	m_panel->onError(text);
}
