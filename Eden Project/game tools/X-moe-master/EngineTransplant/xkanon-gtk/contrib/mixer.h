/*
 * mixer.h  mixer definision
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

#ifndef __MIXER__
#define __MIXER__

#define MIX_MASTER 0
#define MIX_CD     1
#define MIX_MIDI   2
#define MIX_PCM    3

#define MIX_PCM_TOP	4
#define MIX_PCM_BGM	4
#define MIX_PCM_EFFEC	5
#define MIX_PCM_KOE	6
#define MIX_PCM_OTHER	7

extern void mixer_set_level(int device, int level);
extern int  mixer_get_level(int device);
extern int  mixer_initilize();
extern void mixer_setDeviceName(char *name);
extern int  mixer_remove();

#endif /* __MIXER__ */
