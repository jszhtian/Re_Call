/*
 * kconv.h -- kanji converter
 *
 * Copyright (C) 1996,1997 Satoshi KURAMOCHI <satoshi@ueda.info.waseda.ac.jp>
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


#ifndef _KCONV_H_
#define _KCONV_H_
#include "portab.h"

extern boolean kconv(const unsigned char* src, unsigned char* dest);
extern boolean euc2sjis(const unsigned char* src, unsigned char* dst);

#endif /* !_KCONV_H_ */
