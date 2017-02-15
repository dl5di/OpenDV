/*
 *   Copyright (C) 2010-2013,2015 by Jonathan Naylor G4KLX
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
 
#include "XLXHostsFileDownloader.h"
#ifdef XLX_USE_WGET
#include <wx/utils.h> 
#else
#include <wx/protocol/http.h>
#endif
#include <wx/filename.h>
 
/* wxHTTP randomly crashes when called on a worker thread, this must be called from main thread ! */
wxString CXLXHostsFileDownloader::Download(const wxString & xlxHostsFileURL)
{
#ifdef XLX_USE_WGET
	wxString xlxHostsFileName = wxFileName::CreateTempFileName(_T("XLX_Hosts_"));
	wxString commandLine = _T("wget -q -O ") + xlxHostsFileName + _T(" ") + xlxHostsFileURL;
	bool execResult = wxShell(commandLine);
	
	if(!execResult) {
		wxLogError(_T("Unable do download XLX host file, make sure wget is installed"));
		return wxEmptyString;
	}
	
	return xlxHostsFileName;
#else
	wxHTTP http;
	http.SetNotify(false);
	http.SetFlags(wxSOCKET_WAITALL);
	http.SetHeader( _T("Accept") , _T("text/*") );
	http.SetHeader( _T("User-Agent"), _T("ircddbGateway") );
	http.SetTimeout(5); // seconds

	wxLogMessage(_T("Downloading XLX reflector list from %s"), xlxHostsFileURL.c_str());

	// remove "http://" from the url, i.e. minus 7 chars
	size_t len = xlxHostsFileURL.length();
	wxString path = xlxHostsFileURL.Right(len-7);
	
	// find the first forward slash
	int slash = path.Find(wxChar('/'));
	len = path.length();
	wxString server = path.Left(slash);
	path = path.Right(len-slash);

	// Note that Connect() wants a host address, not an URL. 80 is the server's port.
	if(!http.Connect(server, 80 )) {
		wxLogError(_T("Failed to connect to %s"), server);
		return wxEmptyString;
	}
	
	wxInputStream* in = NULL;
	if((in = http.GetInputStream(path)) && in->IsOk()) {
		wxFile file;
		wxString xlxHostsFileName = wxFileName::CreateTempFileName(_T("XLX_Hosts_"), &file);
		wxLogMessage(_T("Created temporary file %s"), xlxHostsFileName);
		if(!file.IsOpened()) {
			wxLogError(_T("Failed to open temporary file %s"), xlxHostsFileName);
			wxDELETE(in);
			return wxEmptyString;
		}
		
		//in->Read(fileStream);
		//fileStream.Write(*in);
		//in->Read(tempFileStream);
		//Both call above seem to not work when run in a separate thread, hence the old fashion loop below
		unsigned char buffer[2048];
		while(!in->Eof()) {
			in->Read(buffer, 2048);
			int readCount = in->LastRead();
			if(readCount <= 0)
				break;

			file.Write(buffer, readCount);
		}
		wxLogMessage(_T("XLX Successfuly downloaded to %s"), xlxHostsFileName);
		file.Close();
		http.Close();
		wxDELETE(in);
		return xlxHostsFileName;
	}
	
	wxLogError(_T("Failed to get HTTP stream"));
	if(in) wxDELETE(in);

	return wxEmptyString;
#endif
}
