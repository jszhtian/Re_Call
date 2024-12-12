/*
 * counter.h  内部カウンタ
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

#ifndef __COUNTER__
#define __COUNTER__

#define SYSTEMCOUNTER_MP3  (256 + 1)
#define SYSTEMCOUNTER_MIDI (256 + 2)
#define SYSTEMCOUNTER_MAINLOOP (256 +3)

extern int  get_counter(int division);
extern void reset_counter(int val);
extern int  get_high_counter(int num);
extern void reset_counter_high(int num,int division,int val);

#endif /* !__COUNTER__ */
