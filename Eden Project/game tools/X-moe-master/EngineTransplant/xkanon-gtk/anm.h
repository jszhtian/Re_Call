/*  anm.h  : ANM ファイルの再生 : classs ANMDAT */

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

#ifndef __KANON_ANM_H__
#define __KANON_ANM_H__

// 'ANM' データ

class ANMDAT {
	class AyuSys& local_system;
	char* data;

	struct Seen {
		int src_x1, src_y1;
		int src_x2, src_y2;
		int dest_x, dest_y;
		int time;
	}* seens;
	struct SeenList {
		int* list;
		int length;
	};
	SeenList* seenlist; SeenList* seenlistlist;
	char* data2;
	int seen_len, seenlist_len, seenlistlist_len;
	void FixSeenAxis(Seen& seen);

public:
	ANMDAT(char* file, class AyuSys& sys);
	~ANMDAT();
	int Init(void); // pixmap をつくる
	int IsValid(void) { if (data == 0) return 0; else return 1;}
	void ChangeAxis(int x, int y); // ANM ファイルの x,y 座標の変更
	
	void Play(int seen);
};

#endif /* !defined(__KANON_ANM_H__) */
