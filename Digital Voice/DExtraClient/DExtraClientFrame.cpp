/*
 *   Copyright (C) 2010,2011 by Jonathan Naylor G4KLX
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

#include "DExtraClientPreferences.h"
#include "DExtraClientFrame.h"
#include "DExtraClientApp.h"
#include "MessageEvent.h"
#include "DStarDefines.h"
#include "HeaderEvent.h"
#include "BusyEvent.h"
#include "Version.h"

#if defined(__WXGTK__) || defined(__WXMAC__)
#include "DExtraClient.xpm"
#endif

#include <wx/aboutdlg.h>

enum {
	Menu_File_Recording = 6000,
	Menu_Edit_Preferences
};

DEFINE_EVENT_TYPE(MESSAGE_EVENT)
DEFINE_EVENT_TYPE(HEADER_EVENT)
DEFINE_EVENT_TYPE(ERROR_EVENT)

BEGIN_EVENT_TABLE(CDExtraClientFrame, wxFrame)
	EVT_MENU(Menu_File_Recording, CDExtraClientFrame::onRecording)
	EVT_MENU(wxID_EXIT, CDExtraClientFrame::onQuit)
	EVT_MENU(Menu_Edit_Preferences, CDExtraClientFrame::onPreferences)
	EVT_MENU(wxID_ABOUT, CDExtraClientFrame::onAbout)

	EVT_CLOSE(CDExtraClientFrame::onClose)

	EVT_CUSTOM(MESSAGE_EVENT, wxID_ANY, CDExtraClientFrame::onMessage)
	EVT_CUSTOM(HEADER_EVENT,  wxID_ANY, CDExtraClientFrame::onHeader)
	EVT_CUSTOM(ERROR_EVENT,   wxID_ANY, CDExtraClientFrame::onError)
END_EVENT_TABLE()

const unsigned int BORDER_SIZE    = 5U;

CDExtraClientFrame::CDExtraClientFrame(const wxString& title) :
wxFrame(NULL, -1, title),
m_panel(NULL)
{
	SetIcon(wxICON(DExtraClient));

	SetMenuBar(createMenuBar());

	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

	m_panel = new CDExtraClientPanel(this, -1);

	mainSizer->Add(m_panel);

	SetSizer(mainSizer);
	mainSizer->SetSizeHints(this);
}

CDExtraClientFrame::~CDExtraClientFrame()
{
}

wxMenuBar* CDExtraClientFrame::createMenuBar()
{
	wxMenu* fileMenu = new wxMenu();
	fileMenu->AppendCheckItem(Menu_File_Recording, _("&Record data\tCtrl+R"));
	fileMenu->AppendSeparator();
	fileMenu->Append(wxID_EXIT, _("Exit"));

	wxMenu* editMenu = new wxMenu();
	editMenu->Append(Menu_Edit_Preferences,  _("Preferences..."));

	wxMenu* helpMenu = new wxMenu();
	helpMenu->Append(wxID_ABOUT, _("About DExtra Client"));

	wxMenuBar* menuBar = new wxMenuBar();
	menuBar->Append(fileMenu, _("File"));
	menuBar->Append(editMenu, _("Edit"));
	menuBar->Append(helpMenu, _("Help"));

	return menuBar;
}

void CDExtraClientFrame::showMessage(CMessageData* message)
{
	wxASSERT(message != NULL);

	CMessageEvent event(message, MESSAGE_EVENT);

	AddPendingEvent(event);
}

void CDExtraClientFrame::showHeader(CHeaderData* header)
{
	CHeaderEvent event(header, HEADER_EVENT);

	AddPendingEvent(event);
}

void CDExtraClientFrame::error(const wxString& error)
{
	CErrorEvent event(error, ERROR_EVENT);

	AddPendingEvent(event);
}

void CDExtraClientFrame::onRecording(wxCommandEvent& event)
{
	bool recording = event.IsChecked();

	::wxGetApp().setRecording(recording);
}

void CDExtraClientFrame::onQuit(wxCommandEvent& event)
{
	Close(false);
}

void CDExtraClientFrame::onClose(wxCloseEvent& event)
{
	if (!event.CanVeto()) {
		Destroy();
		return;
	}

	int reply = ::wxMessageBox(_("Do you want to exit DExtra Client"), _("Exit DExtra Client"), wxOK | wxCANCEL | wxICON_QUESTION);
	switch (reply) {
		case wxOK:
			Destroy();
			break;
		case wxCANCEL:
			event.Veto();
			break;
	}
}

void CDExtraClientFrame::onPreferences(wxCommandEvent& event)
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
	unsigned int oldStartupModule;
	::wxGetApp().getStartup(oldStartupReflector, oldStartupModule);

	CDExtraClientPreferences dialog(this, -1, oldCallsign, oldReadDevice, oldWriteDevice, oldDVDDevice, oldMessage, oldBleep, oldIfaceType, oldAudioDelay, oldPTTInvert, oldSquelchInvert, oldStartupReflector, oldStartupModule);
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
	unsigned int newStartupModule    = dialog.getStartupModule();

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

	if (!newStartupReflector.IsSameAs(oldStartupReflector) || oldStartupModule != newStartupModule) {
		::wxGetApp().setStartup(newStartupReflector, newStartupModule);
		changed = true;
	}

	if (changed) {
		wxMessageDialog dialog(this, _("The changes made will not take effect\nuntil the application is restarted"), _("D-Star Client Information"), wxICON_INFORMATION);
		dialog.ShowModal();
	}
}

void CDExtraClientFrame::onAbout(wxCommandEvent& event)
{
	wxAboutDialogInfo info;
	info.AddDeveloper(wxT("Jonathan Naylor, G4KLX"));
	info.SetName(APPLICATION_NAME);
	info.SetVersion(VERSION);
	info.SetDescription(_("This program allows a computer with a DV Dongle\nand an Internet connection to connect to DExtra reflectors."));

	::wxAboutBox(info);
}

void CDExtraClientFrame::onMessage(wxEvent& event)
{
	CMessageEvent& msgEvent = dynamic_cast<CMessageEvent&>(event);

	CMessageData* message = msgEvent.getMessageData();
	wxASSERT(message != NULL);

	m_panel->onMessage(message);
}

void CDExtraClientFrame::onHeader(wxEvent& event)
{
	CHeaderEvent& hdrEvent = dynamic_cast<CHeaderEvent&>(event);

	CHeaderData* header = hdrEvent.getHeaderData();

	m_panel->onHeader(header);
}

void CDExtraClientFrame::onError(wxEvent& event)
{
	CErrorEvent& errEvent = dynamic_cast<CErrorEvent&>(event);

	wxString text = errEvent.getText();

	m_panel->onError(text);
}
