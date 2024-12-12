/*  anm.cc : ANM ファイルの再生 : classs ANMDAT
 *           プログラム的には system_graphics.cc 内の DrawPDTBuffer() にきわめて近い。
 *           要するに、書いて、wait おいて、の繰り返し。
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
#include "anm.h"
#include "system.h"
#include "image_pdt.h"
#include "image_di.h"

ANMDAT::~ANMDAT() {
	local_system.ClearAnmPDT();
	if (data != 0) delete[] data;
	if (seens != 0) delete[] seens;
	if (seenlist != 0) delete[] seenlist;
	if (seenlistlist != 0) delete[] seenlistlist;
}

ANMDAT::ANMDAT(char* file, AyuSys& sys) : local_system(sys)
{
	seens = 0; seenlist = 0; seenlistlist = 0;
	ARCINFO* info = file_searcher.Find(FILESEARCH::ANM, file,".ANM");
	if (info == 0) return;
	data = info->CopyRead(); delete info;
	if (data == 0) return;
	// check
	static const char head[12]={'A','N','M','3','2',0,0,0,0,1,0,0};
	if (memcmp(data, head, 12) != 0) {
		delete[] data; data=0; return;
	}
	// read header
	seen_len = read_little_endian_int(data+0x8c);
	seenlist_len = read_little_endian_int(data+0x90);
	seenlistlist_len = read_little_endian_int(data+0x94);
	if (seenlistlist_len < 0) { // invalid
		delete[] data; data=0; return;
	}
}

// データを読み込み、pixmap をつくる
int ANMDAT::Init(void) {
	int i,j;
	if (data == 0) return false;
	// ファイルを読み込む
	char* fname = data + 0x1c; // １枚以上の画像はサポートしない
	
	if (*fname == 0) return false;
	local_system.LoadAnmPDT(fname);
	// data をよみこむ
	char* buf = data + 0xb8;
	seens = new Seen[seen_len];
	for (i=0; i<seen_len; i++) {
		Seen& s = seens[i];
		s.src_x1 = read_little_endian_int(buf);
		s.src_y1 = read_little_endian_int(buf+4);
		s.src_x2 = read_little_endian_int(buf+8);
		s.src_y2 = read_little_endian_int(buf+12);
		s.dest_x = read_little_endian_int(buf+16);
		s.dest_y = read_little_endian_int(buf+20);
		s.time = read_little_endian_int(buf+0x38);
		FixSeenAxis(s);
		buf += 0x60;
	}

	// seenlist, seenlistlist : リスト
	seenlist = new SeenList[seenlist_len]; buf = data + 0xb8 + seen_len*0x60;
	for (i=0; i<seenlist_len; i++) {
		SeenList& s = seenlist[i];
		s.length = read_little_endian_int(buf+4);
		s.list = new int[s.length];
		char* tmpbuf = buf + 8; int len = s.length;
		for (j=0; j<len; j++) {
			s.list[j] = read_little_endian_int(tmpbuf);
			tmpbuf += 4;
		}
		buf += 0x68;
	}

	seenlistlist = new SeenList[seenlistlist_len]; buf = data + 0xb8 + seen_len*0x60 + seenlist_len*0x68;
	for (i=0; i<seenlistlist_len; i++) {
		SeenList& s = seenlistlist[i];
		s.length = read_little_endian_int(buf+4);
		s.list = new int[s.length];
		char* tmpbuf = buf+8; int len = s.length;
		for (j=0; j<len; j++) {
			s.list[j] = read_little_endian_int(tmpbuf);
			tmpbuf += 4;
		}
		buf += 0x78;
	}
	return true;
}


void ANMDAT::ChangeAxis(int x, int y)
{
	int i; for (i=0; i<seen_len; i++) {
		Seen& s = seens[i];
		s.dest_x += x; s.dest_y += y;
		FixSeenAxis(s);
	}
}

void ANMDAT::FixSeenAxis(Seen& seen) {
	if (seen.src_x1 > seen.src_x2) { // swap
		int tmp = seen.src_x1; seen.src_x1 = seen.src_x2; seen.src_x2 = tmp;
	}
	if (seen.src_y1 > seen.src_y2) { // swap
		int tmp = seen.src_y1; seen.src_y1 = seen.src_y2; seen.src_y2 = tmp;
	}
	// check screen size
	// int tmp_x = seen.dest_x, tmp_y = seen.dest_y;
	if (seen.dest_x + (seen.src_x2-seen.src_x1+1) > local_system.DefaultScreenWidth()) {
		seen.src_x2 = seen.src_x1 + (local_system.DefaultScreenWidth()-seen.dest_x);
	}
	if (seen.dest_y + (seen.src_y2-seen.src_y1+1) > local_system.DefaultScreenWidth()) {
		seen.src_y2 = seen.src_y1 + (local_system.DefaultScreenWidth()-seen.dest_y);
	}
};

void ANMDAT::Play(int seen) {
	if (local_system.main_window == 0) return;
	if (seen >= seenlistlist_len) return;
	SeenList& ss = seenlistlist[seen];
	// 画面を消す
	// local_system.ClearPDTBuffer(0, 255,255,255);
	//local_system.CopyBuffer(0,0,639,479,1, 0,0,0, 0);
	int i; for (i=0; i<ss.length; i++) {
		SeenList& s = seenlist[ ss.list[i] ];
		int j; for (j=0; j<s.length; j++) {
			/* draw and wait */
			void* tm_handle = local_system.setTimerBase();
			Seen& seen = seens[s.list[j]];
			local_system.DrawAnmPDT(seen.dest_x, seen.dest_y,
				seen.src_x1, seen.src_y1, seen.src_x2-seen.src_x1+1,seen.src_y2-seen.src_y1+1);
			local_system.waitUntil(tm_handle,seen.time);
			local_system.freeTimerBase(tm_handle);
			if (local_system.main_window == 0) return;
		}
	}
}
