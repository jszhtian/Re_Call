/*
 * kconv.c -- kanji converter
 *
 * Copyright (C) 1997 Yutaka OIWA <oiwa@is.s.u-tokyo.ac.jp>
 *
 * written for Satoshi KURAMOCHI's "eplaymidi"
 *                                   <satoshi@ueda.info.waseda.ac.jp>
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
 */

#include "portab.h"
#include "kconv.h"

/*
 * convert SJIS string to EUC-japan string
 *
 * dest area must be larger than (2 * strlen(src) + 1) bytes.
 *
 */

boolean kconv(const unsigned char* src, unsigned char* dest)
{
  while(*src) {
    if (*src < 0x81)
      *dest++ = *src++;
    else if (*src >= 0xa0 && *src <= 0xdf) {
      // JIS X0201 katakana
      *dest++ = 0x8e; // ISO-2022 SS2
      *dest++ = *src++;
    } else {
      unsigned char c1, c2;
      c1 = *src++;
      if (!*src) {
	*dest++ = '*';
	break;
      }
      c2 = *src++;
      if (c1 >= 0xe0)
	c1 -= 0x40;
      c1 -= 0x81;
      if (c2 >= 0x80)
	c2--;
      c2 -= 0x40;

      if (c2 >= 94*2 || c1 > 94/2) {
	// invalid code
	*dest++ = '*';
	*dest++ = '*';
	continue;
      }
      c1 *= 2;
      if (c2 >= 94) {
	c2 -= 94;
	c1++;
      }
      *dest++ = 0xa1 + c1;
      *dest++ = 0xa1 + c2;
    }
  }
  *dest = '\0';
  return TRUE;
}

static void _jis_shift(int *p1, int *p2) {
	unsigned char c1 = *p1;
	unsigned char c2 = *p2;
	int rowOffset = c1 < 95 ? 112 : 176;
	int cellOffset = c1 % 2 ? (c2 > 95 ? 32 : 31) : 126;
	
	*p1 = ((c1 + 1) >> 1) + rowOffset;
	*p2 += cellOffset;
}

boolean euc2sjis(const unsigned char* src, unsigned char* dst)
{
	while(*src) {
		if (*src < 0x81) {
			*dst++ = *src++;
		} else if (*src == 0x8e) {
			src++;
			*dst++ = *src++;
		} else {
			int c1, c2;
			c1 = *src++;
			c2 = *src++;
			c1 -= 128;
			c2 -= 128;
			_jis_shift(&c1, &c2);
			*dst++= (char)c1;
			*dst++= (char)c2;
		}
	}
	*dst = '\0';
	return TRUE;
}
