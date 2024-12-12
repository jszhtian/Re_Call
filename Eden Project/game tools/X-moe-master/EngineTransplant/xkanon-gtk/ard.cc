/*  ard.cc : ARD ファイルの再生 : classs ARDDAT
 *	data は１バイト＝１ドットに大まかに対応するマップ。
 *	この中の番号を返す
 */

/*
 *
 *  Copyright (C) 2000-   Kazunori Ueno(JAGARL) <jagarl@creator.club.ne.jp>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
*/

#include <ctype.h>
#include "file.h"
#include "ard.h"
#include "system.h"

ARDDAT::~ARDDAT() {
	if (data != 0) delete[] data;
	data = 0;
	if (valid_data != 0) delete[] valid_data;
	valid_data = 0;
}

ARDDAT::ARDDAT(char* file, AyuSys& sys) : local_system(sys)
{
	int i;
	valid_data = 0; data = 0;
	ARCINFO* info = file_searcher.Find(FILESEARCH::ARD, file,".ARD");
	if (info == 0) return;
	data = info->CopyRead(); delete info;
	if (data == 0) return;
	// check
	static const char head[8]={'A','R','E','A',0,0,0,0};
	if (memcmp(data, head, 8) != 0) {
		delete[] data; data=0; return;
	}
	// read header
	width = read_little_endian_int(data+0x08);
	height = read_little_endian_int(data+0x0c);
	max_index = read_little_endian_int(data+0x10);
	valid_data = new int[max_index+1];
	for (i=0; i<=max_index; i++) valid_data[i] = i;
}

void ARDDAT::SetValid(int item) {
	if (item <= 0 || item > max_index) return;
	if (valid_data == 0) return;
	valid_data[item] = item;
}

void ARDDAT::SetInvalid(int item) {
	if (item <= 0 || item > max_index) return;
	if (valid_data == 0) return;
	valid_data[item] = 0;
}

int ARDDAT::RegionNumber(int x, int y) {
	if (x < 0 || x >= global_system.DefaultScreenWidth()) return 0;
	if (y < 0 || y >= global_system.DefaultScreenHeight()) return 0;
	if (valid_data == 0 || data == 0) return 0;
	if (height == 0 || width == 0) return 0;
	x = x * width / global_system.DefaultScreenWidth(); y = y * height / global_system.DefaultScreenHeight();
	int d = data[0x120 + x + y*width];
	if (d <= 0 || d > max_index) return 0;
	return valid_data[d];
}
