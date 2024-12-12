/*
 * audioIO_dmy.c  dumy lowlevel access
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
*/

#include "portab.h"
#include "audio.h"
#include "audioMix_dmy.c"

int audioOpen(DSPFILE *dfile, WAVFILE *wfile, char *device) { return 0;}

int audioClose(DSPFILE *dfile) { return 0;}

void audioFlush(DSPFILE *dfile) { return;}

int audioWrite(DSPFILE *dfile, int cnt) { return 0;}

int audioCheck(DSPINFO *info, char *device) { return NG; }

int audioRest(DSPFILE *dfile){return 0;}
void audioStop(){return;}
