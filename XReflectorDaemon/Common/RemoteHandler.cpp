/*
 *   Copyright (C) 2011,2012 by Jonathan Naylor G4KLX
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

#include "RepeaterHandler.h"
#include "StarNetHandler.h"
#include "RemoteHandler.h"
#include "DExtraHandler.h"
#include "DPlusHandler.h"
#include "DStarDefines.h"
#include "DCSHandler.h"

CRemoteHandler::CRemoteHandler(const wxString& password, unsigned int port, const wxString& address) :
m_password(password),
m_handler(port, address),
m_random(0U)
{
	wxASSERT(port > 0U);
	wxASSERT(!password.IsEmpty());
}

CRemoteHandler::~CRemoteHandler()
{
}

bool CRemoteHandler::open()
{
	return m_handler.open();
}

void CRemoteHandler::process()
{
	RPH_TYPE type = m_handler.readType();
	switch (type) {
		case RPHT_LOGOUT:
			m_handler.setLoggedIn(false);
			wxLogMessage(wxT("Remote control user has logged out"));
			break;
		case RPHT_LOGIN:
			m_random = ::rand();
			m_handler.sendRandom(m_random);
			break;
		case RPHT_HASH: {
				bool valid = m_handler.readHash(m_password, m_random);
				if (valid) {
					wxLogMessage(wxT("Remote control user has logged in"));
					m_handler.setLoggedIn(true);
					m_handler.sendACK();
				} else {
					wxLogMessage(wxT("Remote control user has failed login authentication"));
					m_handler.setLoggedIn(false);
					m_handler.sendNAK(wxT("Invalid password"));
				}
			}
			break;
		case RPHT_CALLSIGNS:
			sendCallsigns();
			break;
		case RPHT_REPEATER: {
				wxString callsign = m_handler.readRepeater();
				sendRepeater(callsign);
			}
			break;
		case RPHT_STARNET: {
				wxString callsign = m_handler.readStarNetGroup();
				sendStarNetGroup(callsign);
			}
			break;
		case RPHT_LINK: {
				wxString callsign, reflector;
				RECONNECT reconnect;
				m_handler.readLink(callsign, reconnect, reflector);
				if (reflector.IsEmpty())
					wxLogMessage(wxT("Remote control user has linked \"%s\" to \"None\" with reconnect %d"), callsign.c_str(), reconnect);
				else
					wxLogMessage(wxT("Remote control user has linked \"%s\" to \"%s\" with reconnect %d"), callsign.c_str(), reflector.c_str(), reconnect);
				link(callsign, reconnect, reflector, true);
			}
			break;
		case RPHT_LINKSCR: {
				wxString callsign, reflector;
				RECONNECT reconnect;
				m_handler.readLinkScr(callsign, reconnect, reflector);
				if (reflector.IsEmpty())
					wxLogMessage(wxT("Remote control user has linked \"%s\" to \"None\" with reconnect %d from localhost"), callsign.c_str(), reconnect);
				else
					wxLogMessage(wxT("Remote control user has linked \"%s\" to \"%s\" with reconnect %d from localhost"), callsign.c_str(), reflector.c_str(), reconnect);
				link(callsign, reconnect, reflector, false);
			}
			break;
		case RPHT_LOGOFF: {
				wxString callsign, user;
				m_handler.readLogoff(callsign, user);
				wxLogMessage(wxT("Remote control user has logged off \"%s\" from \"%s\""), user.c_str(), callsign.c_str());
				logoff(callsign, user);
			}
			break;
		default:
			break;
	}
}

void CRemoteHandler::close()
{
	m_handler.close();
}

void CRemoteHandler::sendCallsigns()
{
	wxArrayString repeaters = CRepeaterHandler::listDVRepeaters();
	wxArrayString starNets  = CStarNetHandler::listStarNets();

	m_handler.sendCallsigns(repeaters, starNets);
}

void CRemoteHandler::sendRepeater(const wxString& callsign)
{
	CRepeaterHandler* repeater = CRepeaterHandler::findDVRepeater(callsign);
	if (repeater == NULL) {
		m_handler.sendNAK(wxT("Invalid repeater callsign"));
		return;
	}

	CRemoteRepeaterData* data = repeater->getInfo();
	if (data != NULL) {
		CDExtraHandler::getInfo(repeater, *data);
		CDPlusHandler::getInfo(repeater, *data);
		CDCSHandler::getInfo(repeater, *data);

		m_handler.sendRepeater(*data);
	}

	delete data;
}

void CRemoteHandler::sendStarNetGroup(const wxString& callsign)
{
	CStarNetHandler* starNet = CStarNetHandler::findStarNet(callsign);
	if (starNet == NULL) {
		m_handler.sendNAK(wxT("Invalid STARnet Group callsign"));
		return;
	}

	CRemoteStarNetGroup* data = starNet->getInfo();
	if (data != NULL)
		m_handler.sendStarNetGroup(*data);

	delete data;
}

void CRemoteHandler::link(const wxString& callsign, RECONNECT reconnect, const wxString& reflector, bool respond)
{
	CRepeaterHandler* repeater = CRepeaterHandler::findDVRepeater(callsign);
	if (repeater == NULL) {
		m_handler.sendNAK(wxT("Invalid repeater callsign"));
		return;
	}

	repeater->link(reconnect, reflector);

	if (respond){
	    m_handler.sendACK();
	}

}

void CRemoteHandler::logoff(const wxString& callsign, const wxString& user)
{
	CStarNetHandler* starNet = CStarNetHandler::findStarNet(callsign);
	if (starNet == NULL) {
		m_handler.sendNAK(wxT("Invalid STARnet group callsign"));
		return;
	}

	bool res = starNet->logoff(user);
	if (!res)
		m_handler.sendNAK(wxT("Invalid STARnet user callsign"));
	else
		m_handler.sendACK();
}
