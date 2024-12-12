/*
 * LittleEndian.c  get little endian value
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
 * @version 0.00 97/11/06 ΩÈ»«
*/

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif

#include "portab.h"

int LittleEndian_getDW(const BYTE *b,int index) {
	int c0, c1, c2, c3;
	int d0, d1;
	c0 = *(b + index + 0);
	c1 = *(b + index + 1);
	c2 = *(b + index + 2);
	c3 = *(b + index + 3);
	d0 = c0 + (c1 << 8);
	d1 = c2 + (c3 << 8);
	return (DWORD)(d0 + (d1 << 16));
}

int LittleEndian_get3B(const BYTE *b,int index) {
	int c0, c1, c2;
	c0 = *(b + index + 0);
	c1 = *(b + index + 1);
	c2 = *(b + index + 2);
	return c0 + (c1 << 8) + (c2 << 16);
}

int LittleEndian_getW(const BYTE *b,int index) {
	int c0, c1;
	c0 = *(b + index + 0);
	c1 = *(b + index + 1);
	return c0 + (c1 << 8);
}

void LittleEndian_putW(int num, BYTE *b, int index) {
	int c0, c1;
	num %= 65536;
	c0 = num % 256;
	c1 = num / 256;
	b[index] = c0; b[index+1] = c1;
}

