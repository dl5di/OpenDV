/*
 *   Copyright (C) 2014 by Rick Schnicker KD0OSS
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

#ifndef SDVTOOLS_H
#define SDVTOOLS_H

typedef unsigned int	ULONG;
typedef void		*PVOID;
typedef unsigned int	DWORD;

typedef PVOID		FT_HANDLE;
typedef ULONG		FT_STATUS;

FT_HANDLE sdv_open(int iport, int mode, int baud, int reset);
void sdv_close(FT_HANDLE ftHandle);
int sdv_write(FT_HANDLE ftHandle, unsigned char *ucBytes, int length, DWORD *dwBytesInQueue);
int sdv_read(FT_HANDLE ftHandle, char *cBytes, int length, int timeout, DWORD *dwBytesReceived);
unsigned char sdv_get_bitmode(FT_HANDLE ftHandle);
short int is_ptt(FT_HANDLE ftHandle);


#endif  /* SDVTOOLS_H */
