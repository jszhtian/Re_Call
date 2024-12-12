/*
 * portab.h ÈÆÍÑÄêµÁ
 *
 * Copyright (C) 1997-1998 Masaki Chikama (Wren) <chikama@kasumi.ipl.mech.nagoya-u.ac.jp>
 *               1998-                           <masaki-c@is.aist-nara.ac.jp>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 *
*/

#ifndef __PORTAB__
#define __PORTAB__
#include <stdlib.h>
#include <stdio.h>

#define	YES		1
#define	NO		0
#define OK		0
#define ERROR	      (-1)
#define NG	      (-1)
#define true            1
#define false           0
#ifndef FALSE
#define FALSE           0
#endif
#ifndef TRUE
#define TRUE            (!FALSE)
#endif

#define min(x,y) ((x) < (y) ? (x) : (y))
#define max(x,y) ((x) > (y) ? (x) : (y))

typedef	unsigned char  BYTE;
typedef	unsigned short WORD;
typedef	unsigned int   DWORD; /* ??? */
typedef char           boolean;

#endif /* !__PORTAB__ */
