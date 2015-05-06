/*
 *   Copyright (C) 2015 by Rick Schnicker KD0OSS
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

#ifndef DVEMU_H
#define DVEMU_H


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <errno.h>
#include <termios.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/socket.h>

#include <netinet/in.h>
#include "sdvtools.h"

//#define	AMBE3000_VERSION		"2015-04-25"

// FTDI Bit-Bang mode
#define BIT_BANG 0x22

#define	AMBE3000_HEADER_LEN	4U
#define	AMBE3000_START_BYTE	0x61U

#define	DEFAULT_PORT		2460U
#define	BUFFER_LENGTH		400U

int dvemu(FT_HANDLE ftHandle, int port, int debug);

#endif /* DVEMU_H */

