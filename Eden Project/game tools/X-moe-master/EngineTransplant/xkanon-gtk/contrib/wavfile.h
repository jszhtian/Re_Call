/*
 * wavfile.h  WAV file´ØÏ¢
 *
 *  Copyright: wavfile.c (c) Erik de Castro Lopo  erikd@zip.com.au
 *
 *  Modified : 1997-1998 Masaki Chikama (Wren) <chikama@kasumi.ipl.mech.nagoya-u.ac.jp>
 *             1998-                           <masaki-c@is.aist-nara.ac.jp>
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
*/
#ifndef __WAVEFILE__
#define __WAVEFILE__

#include "audio.h"

#define WW_BADOUTPUTFILE	1
#define WW_BADWRITEHEADER	2

#define WR_BADALLOC		3
#define WR_BADSEEK		4
#define WR_BADRIFF		5
#define WR_BADWAVE		6
#define WR_BADFORMAT		7
#define WR_BADFORMATSIZE	8

#define WR_NOTPCMFORMAT		9
#define WR_NODATACHUNK		10
#define WR_BADFORMATDATA	11

extern WAVFILE *WavGetInfo(char *data);
extern int      WavRemoveInfo(WAVFILE *wfile);

#endif /* !__WAVEFILE__ */
