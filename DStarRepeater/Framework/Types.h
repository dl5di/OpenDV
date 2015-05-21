/*
 *   Copyright (C) 2015 by Jonathan Naylor G4KLX
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

#if !defined(TYPES_H)
#define	TYPES_H

#if defined(WIN32)

typedef unsigned char  uint8_t;
typedef signed   char  int8_t;

typedef unsigned short uint16_t;
typedef signed short   int16_t;

typedef unsigned int   uint32_t;
typedef signed int     int32_t;

typedef unsigned __int64 uint64_t;
typedef signed __int64   int64_t;
#else
#include <cstdint>
#endif

#endif
