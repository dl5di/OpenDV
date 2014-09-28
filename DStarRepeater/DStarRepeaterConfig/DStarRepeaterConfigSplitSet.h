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

#ifndef	DStarRepeaterConfigSplitSet_H
#define	DStarRepeaterConfigSplitSet_H

#include "DStarRepeaterConfigDefs.h"
#include "AddressTextCtrl.h"
#include "PortTextCtrl.h"

#include <wx/wx.h>

class CDStarRepeaterConfigSplitSet : public wxDialog {
public:
	CDStarRepeaterConfigSplitSet(wxWindow* parent, int id, const wxString& localAddress, unsigned int localPort, const wxArrayString& transmitterNames, const wxArrayString& receiverNames, unsigned int timeout);
	virtual ~CDStarRepeaterConfigSplitSet();

	virtual bool Validate();

	virtual wxString      getLocalAddress() const;
	virtual unsigned int  getLocalPort() const;

	virtual wxArrayString getTransmitterNames() const;

	virtual wxArrayString getReceiverNames() const;

	virtual unsigned int  getTimeout() const;

private:
	CAddressTextCtrl* m_localAddress;
	CPortTextCtrl*    m_localPort;
	wxTextCtrl**      m_transmitterNames;
	wxTextCtrl**      m_receiverNames;
	wxSlider*         m_timeout;
	unsigned int      m_txCount;
	unsigned int      m_rxCount;
};

#endif
