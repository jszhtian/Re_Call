/*  senario_graphics.cc
 *     グラフィックの再生に関係するシナリオデコード
 *     グラフィックの操作を保存するためのクラス SENARIO_GraphicsSaveBuf
 *     の関係のメソッドなど
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

#include "file.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "senario.h"
#include "image_di.h"

int SearchCgmData(char* name);

inline void CheckCgm(char* name, SENARIO_FLAGS& flags) {
	int num = SearchCgmData(name);
	if (num < 0) return;
	flags.SetBit(num,1);
}

#define pp(X) do { printf("not supported command! : "); printf X; } while(0)

// #define DEBUG_GraphicsLoad
// #define DEBUG_Graphics


/* for debug */
#ifdef p
#  undef p
#endif /* defined(p) */

#ifdef DEBUG_GraphicsLoad
#define p(X) printf X
#else
#define p(X)
#endif

//#define DEFAULT_PDT PDT_BUFFER_DEAL-1
#define DEFAULT_PDT 1
#define MIDDLE_PDT 3

// SENARIO_Graphics のメソッド
// それぞれ、実際の描画、描画の保存、描画のリストアの順に並んでいる

// Backlog buffer への store / restore
int SENARIO_Graphics::HashBuffer(void) {
	int i; int hash = 0;
	for (i=0; i<deal; i++)
		hash += buf[i].Hash();
	hash += deal;
	return hash;
}
int SENARIO_Graphics::StoreBufferLen(void) {
	int len = 1;
	int i;for (i=0; i<deal; i++)
		len += buf[i].StoreLen();
	return len;
}
void SENARIO_Graphics::StoreBuffer(char* store_buf, int store_len) {
	*store_buf++ = deal; char* store_end = store_buf+store_len-1;
	int i;for (i=0; i<deal; i++) {
		store_buf = buf[i].Store(store_buf);
		if (store_buf > store_end) {
			fprintf(stderr, "Error in SENARIO_Graphics::StoreBuffer : Invalid store len !\n");
			break;
		}
	}
	return;
}
void SENARIO_Graphics::RestoreBuffer(const char* store_buf, int store_len) {
	Change();
	deal = *store_buf++; const char* store_end = store_buf + store_len;
	if (deal <= 0) { deal = 0; return; }
	if (deal > 32) deal = 32;
	int i;for (i=0; i<deal; i++) {
		buf[i].Restore(store_buf);
		store_buf += buf[i].StoreLen();
		if (store_buf > store_end) {
			fprintf(stderr, "Error in SENARIO_Graphics::RestoreBuffer : Invalid store len !\n");
			break;
		}
	}
	return;
}
void SENARIO_Graphics::Dump(FILE* out, const char* tab) {
	fprintf(out, "%sSENARIO_Graphics::Dump : deal = %d\n",tab,deal);
	char* new_tab = new char[strlen(tab)+5];
	strcpy(new_tab, tab); strcat(new_tab, "    "); /* tab は space x 4 */
	int i;for (i=0; i<deal; i++) 
		buf[i].Dump(out, new_tab);
	delete[] new_tab;
	fprintf(out, "%sSENARIO_Graphics::Dump : end of dump\n",tab);
	return;
}
int SENARIO_Graphics::CompareBuffer(const char* store_buf, int store_len) {
	int buf_deal = *store_buf++; if (deal != buf_deal) return 1;
	const char* store_end = store_buf + store_len;
	int i;for (i=0; i<deal; i++) {
		if (buf[i].Compare(store_buf)) return 1;
		store_buf += buf[i].StoreLen();
		if (store_buf > store_end) {
			fprintf(stderr, "Error in SENARIO_Graphics::CompareBuffer : Invalid store len !\n");
			break;
		}
	}
	return 0;
}

static int savebuflen[0x100] = {
//0   1   2   3   4   5   6   7   8  **   9   A   B   C   D   E   F
 16,  0,  2,  0,  0,  0,  0,  0,  0,/**/  0,  0,  0,  0,  0,  0,  0, // +00
  0,  0,  0,  0,  0,  0,  0,  0,  0,/**/  0,  0,  0,  0,  0,  0,  0, // +10
  0,  0,  0,  0,  0,  0,  0,  0,  0,/**/  0,  0,  0,  0,  0,  0,  0, // +20
  0,  0,  6,  0,  0,  0,  0,  0,  0,/**/  0,  0,  0,  0,  0,  0,  0, // +30
  0,  0,  0,  0,  0,  0,  0,  0,  0,/**/  0,  0,  0,  9,  0,  0,  0, // +40
  0,  0,  0,  0,  0,  0,  0,  0,  0,/**/  0,  0,  0,  0,  0,  0,  0, // +50
  0,  0,  0,  0,  9,  0,  9,  0,  9,/**/  0,  8,  0,  0,  0,  0,  0, // +60
  0,  0,  0,  0,  0,  0,  0,  0,  0,/**/  0,  0,  0,  0,  0,  0,  0, // +70
  0,  0,  0,  0,  0,  0,  0,  0,  0,/**/  0,  0,  0,  0,  0,  0,  0, // +80
  0,  0,  0,  0,  0,  0, 35, 35,  0,/**/  0,  0,  0,  0,  0,  0,  0, // +90
  0,  0,  0,  0,  0,  0,  0,  0,  0,/**/  0,  0,  0,  0,  0,  0,  0, // +A0
  0,  0,  0,  0,  0,  0,  0,  0,  0,/**/  0,  0,  0,  0,  0,  0,  0, // +B0
  0,  0,  0,  0,  0,  0,  0,  0,  0,/**/  0,  0,  0,  0,  0,  0,  0, // +C0
  0,  0,  0,  0,  0,  0,  0,  0,  0,/**/  0,  0,  0,  0,  0,  0,  0, // +D0
  0,  0,  0,  0,  0,  0,  0,  0,  0,/**/  0,  0,  0,  0,  0,  0,  0, // +E0
  0,  0,  0,  0,  0,  0,  0,  0,  0,/**/  0,  0,  0,  0,  0,  0,  0};// +F0

/* 保存されている内容：
 *   cmd 0: args 0-14, filenames. (filedeal=1)
 *   cmd 2: arg4 0, filenames. (filedeal=1)
 *   cmd 0x32: arg4, 0-4, arg2
 *   cmd 0x4c: arg4, 0-8
 *   cmd 0x64: arg4, 0-8
 *   cmd 0x66: arg4, 0-8
 *   cmd 0x68: arg4, 0-8
 *   cmd 0x6a: arg4, 0-7
 *   cmd 0x96,97; filedeal, filenames(len=args[1]), args: 0-1, arg4: 0-31
 *   cmd 0xaa; 本来はarg4 が必要だが、無視
 *   cmd 0xac; 本来は cmd0 と同じだが、無視
 *   cmd 0xf0 : xkanon 専用：pdt0 の内容を画面に反映させる
*/

int SENARIO_GraphicsSaveBuf::StoreLen(void) {
	if (cmd < 0x20) { // cmd 0, 2
		return 1 + strlen(filenames)+1 + savebuflen[cmd]*INT_SIZE;
	} else if (cmd < 0x90) { // cmd 0x32,4c,64,66,68,6a
		return 1 + savebuflen[cmd]*INT_SIZE;
	} else if (cmd < 0xa0) { // cmd 0x96, 0x97
		int slen = args[1];
		return 1 + slen + savebuflen[cmd]*INT_SIZE;
	} else if (cmd < 0x100) {
		return 1;
	} else return 0;
}
char* SENARIO_GraphicsSaveBuf::Store(char* buf) { // buf の長さは StoreLen はある
	*(unsigned char*)buf++ = cmd;
	int sz = savebuflen[cmd]; int i;
	if (cmd < 0x20) { // cmd 0, 2
		if (cmd == 2) args[0] = arg4[0]; /* cmd == 2 では arg4 に情報が保存されている */
		args[sz-1] = strlen(filenames);
		for (i=0; i<sz; i++) {
			write_little_endian_int(buf, args[i]);
			buf += INT_SIZE;
		}
		strcpy(buf, filenames); buf += strlen(filenames)+1;
	} else if (cmd < 0x90) { // cmd 0x32, 4c, 64, 66, 68, 6a
		if (cmd == 0x32) arg4[5] = arg2; /* arg2 の情報を arg4 に入れる */
		for (i=0; i<sz; i++) {
			write_little_endian_int(buf, arg4[i]);
			buf += INT_SIZE;
		}
	} else if (cmd < 0xa0) { // cmd 0x96, 0x97
		if (cmd == 0x96 || 0x97) {
			arg4[32] = filedeal;
			arg4[33] = args[0]; arg4[34] = args[1];
		}
		for (i=0; i<sz; i++) {
			write_little_endian_int(buf, arg4[i]);
			buf += INT_SIZE;
		}
		memcpy(buf, filenames, args[1]); buf += args[1];
	} else if (cmd < 0x100) {
		/* do nothing */
	}
	return buf;
}

// 適当
const int hash_seed1 = 10681091;
const int hash_seed2 = 572108;
int SENARIO_GraphicsSaveBuf::Hash(void) {
	int hash1 = cmd; int hash2 = 0;
	// まず、arg4 / args に必要な情報をまとめる
	int sz = savebuflen[cmd]; int* info = arg4; int slen = 0;
	if (cmd < 0x20) { // cmd 0, 2
		info = args;
		if (cmd == 2) args[0] = arg4[0]; /* cmd == 2 では arg4 に情報が保存されている */
		slen = args[sz-1] = strlen(filenames);
	} else if (cmd < 0x90) { // cmd 0x32, 4c, 64, 66, 68, 6a
		if (cmd == 0x32) arg4[5] = arg2; /* arg2 の情報を arg4 に入れる */
	} else if (cmd < 0xa0) { // cmd 0x96, 0x97
		if (cmd == 0x96 || 0x97) {
			arg4[32] = filedeal;
			arg4[33] = args[0]; arg4[34] = args[1];
		}
		slen = args[1]-1;
	} else if (cmd < 0x100) {
		/* do nothing */
	}
	// 計算
	int i; for (i=0; i<sz; i++) {
		hash1 *= hash_seed1;
		hash1 += hash_seed2 + *info++;
	}
	for (i=0; i<slen; i++) {
		hash2 *= hash_seed1;
		hash2 += hash_seed2 + int(filenames[i]);
	}
	return hash1 ^ hash2;
}

void SENARIO_GraphicsSaveBuf::Restore(const char* buf) {
	cmd = *(unsigned char*)buf++; cmd &= 0xff;
	int sz = savebuflen[cmd]; int i;
	if (cmd < 0x20) { // cmd 0, 2
		for (i=0; i<sz; i++) {
			args[i] = read_little_endian_int(buf);
			buf += INT_SIZE;
		}
		if (cmd == 2) arg4[0] = args[0];
		int slen = args[sz-1];
		if (slen < 0) slen = 0;
		if (slen >= 0x200) slen = 0x1ff;
		memcpy(filenames, buf, slen); filenames[slen] = '\0';
	} else if (cmd < 0x90) { // cmd 0x32, 4c, 64, 66, 68, 6a
		for (i=0; i<sz; i++) {
			arg4[i] = read_little_endian_int(buf);
			buf += INT_SIZE;
		}
		if (cmd == 0x32) arg2 = arg4[5]; /* arg2 の情報を arg4 に入れる */
	} else if (cmd < 0xa0) { // cmd 0x96, 0x97
		for (i=0; i<sz; i++) {
			arg4[i] = read_little_endian_int(buf);
			buf += INT_SIZE;
		}
		if (cmd == 0x96 || 0x97) {
			filedeal = arg4[32];
			args[0] = arg4[33]; args[1] = arg4[34];
		}
		if (args[1] < 0) args[1] = 0;
		if (args[1] > 0x200) args[1] = 0x1ff;
		memcpy(filenames, buf, args[1]);
	} else if (cmd < 0x100) {
		/* do nothing */
	}
	return;
}

int SENARIO_GraphicsSaveBuf::Compare(const char* buf) {
	int buf_cmd = *(unsigned char*)buf++; buf_cmd &= 0xff; if (cmd != buf_cmd) return 1;
	int sz = savebuflen[cmd]; int i;
	if (cmd < 0x20) { // cmd 0, 2
		if (cmd == 2) args[0] = arg4[0]; /* cmd == 2 では arg4 に情報が保存されている */
		args[sz-1] = strlen(filenames);
		for (i=0; i<sz; i++) {
			if (read_little_endian_int(buf)!=args[i]) return 1;
			buf += INT_SIZE;
		}
		return strcmp(buf, filenames);
	} else if (cmd < 0x90) { // cmd 0x32, 4c, 64, 66, 68, 6a
		if (cmd == 0x32) arg4[5] = arg2; /* arg2 の情報を arg4 に入れる */
		for (i=0; i<sz; i++) {
			if (read_little_endian_int(buf)!=arg4[i]) return 1;
			buf += INT_SIZE;
		}
		return 0;
	} else if (cmd < 0xa0) { // cmd 0x96, 0x97
		if (cmd == 0x96 || 0x97) {
			arg4[32] = filedeal;
			arg4[33] = args[0]; arg4[34] = args[1];
		}
		for (i=0; i<sz; i++) {
			if (read_little_endian_int(buf)!=arg4[i]) return 1;
			buf += INT_SIZE;
		}
		return memcmp(buf, filenames, args[1]);
	} else if (cmd < 0x100) {
		return 1;
	}
	return 1;
}

void SENARIO_GraphicsSaveBuf::Dump(FILE* out, const char* tab) {
	int i;
	switch(cmd) {
		case 0:
			fprintf(out, "%scmd 0: LoadDraw; file %s\n", tab, filenames);

			fprintf(out, "%s       sel (%d,%d)-(%d,%d) -> (%d,%d), wait %d, no %d, kasane %d\n%s          ",
				tab,args[0],args[1],args[2],args[3],args[4],args[5],args[6],args[7],args[8],tab);
			fprintf(out, "%s",tab);
			for (i=0; i<6; i++) fprintf(out,",arg[%d]=%d ",i+1,args[i+9]);
			fprintf(out, "\n");
			break;
		case 2: fprintf(out, "%scmd 2: Load; pdt %d, file %s\n", tab, arg4[0], filenames); break;
		case 0x32: fprintf(out, "%scmd 0x32: Clear; pdt %d, (%d,%d)-(%d,%d), color %d,%d,%d\n",
			tab,
			arg4[4], arg4[0], arg4[1], arg4[2], arg4[3], (arg2>>16)&0xff, (arg2>>8)&0xff, arg2&0xff);
			break;
		case 0x4c: fprintf(out, "%scmd 0x4c: Fade; pdt %d, (%d,%d)-(%d,%d), color %d,%d,%d, count %d\n",
			tab,
			arg4[4], arg4[0], arg4[1], arg4[2], arg4[3],
			arg4[5], arg4[6], arg4[7], arg4[8]);
			break;
		case 0x64: fprintf(out, "%scmd 0x64: Copy; fade=%d\n"
					"%s               ",tab,arg4[8],tab); goto paxis;
		case 0x66: fprintf(out, "%scmd 0x66: Copy with mask; fade=%d,%d,%d\n"
					"%s                         ",
					tab,
					(arg4[8]>>16)&0xff,(arg4[8]>>8)&0xff, arg4[8]&0xff,tab); goto paxis;
		case 0x68: fprintf(out, "%scmd 0x68: Copy without color; color = %x\n"
					"%s                             ",tab,arg4[8],tab); goto paxis;
		case 0x6a: fprintf(out, "%scmd 0x6a: Swap; \n%s               ",tab,tab); goto paxis;
		paxis: fprintf(out,"%d:(%d,%d)-(%d,%d) -> %d:(%d,%d)\n",
			arg4[4],arg4[0],arg4[1],arg4[2],arg4[3],arg4[7],arg4[5],arg4[6]);
			break;
		case 0x96: case 0x97: {
			fprintf(out,"%scmd 0x%02x: MultiLoad; file deal=%d, file len=%d, sel %d,pdt %s\n",tab,cmd,filedeal, args[1],args[0],filenames);
			char* f = filenames; f += strlen(f)+1;
			for (i=0; i<filedeal-1; i++) {
				int* geos = arg4+i*8;
				fprintf(out,"%s",tab);
				fprintf(out, "    %d: file %s, geom=",i, f);
				f += strlen(f)+1;
				int j; for (j=0; j<8; j++) {
					fprintf(out,"%3d,",*geos++);
				}
				fprintf(out,"\n");
			}
			break;
			}
		case 0xaa:
			fprintf(out, "%scmd 0xaa: Copy screen to SaveBuffer\n",tab);break;
		case 0xac:
			fprintf(out, "%scmd 0xac: Restore SaveBuffer\n",tab);break;
		case 0xf0: 
			fprintf(out,"%scmd 0xf0: Copy pdt0 to screen\n",tab);break;
		default: fprintf(out, "%sInvalid command : %x\n",tab,cmd); break;
	}
	return;
}

// グラフィックの読み込みと描画
void SENARIO_Graphics::DoLoadDraw(char* str, SEL_STRUCT* sel) {
	local_system.LoadPDTBuffer(DEFAULT_PDT, str);
	local_system.DrawPDTBuffer(DEFAULT_PDT, sel);
}

void SENARIO_GraphicsSaveBuf::SaveLoadDraw(char* str, SEL_STRUCT* sel) {
	cmd = 0;
	filedeal = 1;
	strcpy(filenames, str);
	if (sel != 0) {
		args[0] = sel->x1; args[1] = sel->y1;
		args[2] = sel->x2; args[3] = sel->y2;
		args[4] = sel->x3; args[5] = sel->y3;
		args[6] = sel->wait_time; args[7] = sel->sel_no; args[8] = sel->kasane;
		args[9] = sel->arg1; args[10] = sel->arg2; args[11] = sel->arg3;
		args[12] = sel->arg4; args[13] = sel->arg5; args[14] = sel->arg6;
	}
}

void SENARIO_GraphicsSaveBuf::DoLoadDraw(SENARIO_Graphics& drawer, int draw_flag) {
	if (cmd != 0) return;
	SEL_STRUCT sel;
	sel.x1 = args[0]; sel.y1 = args[1];
	sel.x2 = args[2]; sel.y2 = args[3];
	sel.x3 = args[4]; sel.y3 = args[5];
	sel.wait_time = args[6]; sel.sel_no = args[7];
	sel.wait_time = 50; sel.sel_no = 4; /* 高速に */
	sel.kasane = args[8];
	sel.arg1 = args[9]; sel.arg2 = args[10]; sel.arg3 = args[11];
	sel.arg4 = args[12]; sel.arg5 = args[13]; sel.arg6 = args[14];

	drawer.local_system.LoadPDTBuffer(DEFAULT_PDT, filenames);
	if (draw_flag) {
		drawer.local_system.DrawPDTBuffer(DEFAULT_PDT, &sel);
	} else {
		drawer.local_system.CopyPDTtoBuffer(
			sel.x1, sel.y1, sel.x2, sel.y2, DEFAULT_PDT,
			sel.x3, sel.y3, -1, 0);
	}
};


// グラフィックの読み込み（のみ）
void SENARIO_Graphics::DoLoad(char* str, int pdt) {
	local_system.LoadPDTBuffer(pdt, str);
}

void SENARIO_GraphicsSaveBuf::SaveLoad(char* str, int pdt) {
	cmd = 2;
	filedeal = 1; strcpy(filenames, str);
	arg4[0] = pdt;
}

void SENARIO_GraphicsSaveBuf::DoLoad(SENARIO_Graphics& drawer) {
	drawer.DoLoad(filenames, arg4[0]);
}

// 複数のグラフィックの読み込み
void SENARIO_GraphicsSaveBuf::SaveMultiLoad(int _cmd, char* fnames, int deal, int all_len, int sel_no, int* geos) {
	cmd = _cmd;
	if (all_len>0x200) all_len=0x200;
	filedeal = deal;
	memcpy(filenames, fnames, all_len);
	args[0] = sel_no; args[1] = all_len;
	int i; for (i=0; i<32; i++) arg4[i] = geos[i];
}
void SENARIO_GraphicsSaveBuf::DoMultiLoad(SENARIO_Graphics& drawer, int draw_flag) {
	// バックグラウンド読み込み
	if (cmd == 0x96) {
		drawer.local_system.LoadPDTBuffer(DEFAULT_PDT, filenames);
	} else {
		int pdt = atoi(filenames);
		if (DEFAULT_PDT != pdt) {
			drawer.local_system.SyncPDT(DEFAULT_PDT);
			drawer.local_system.CopyBuffer(0, 0, global_system.DefaultScreenWidth()-1, global_system.DefaultScreenHeight()-1, pdt,
				0, 0, DEFAULT_PDT, 0);
		}
	}
	char* buf = filenames; buf += strlen(buf)+1;
	int sel = args[0]; int images = filedeal-1;
	// 重ねるファイル読み込み
	int i; for (i=0;  i<images; i++) {
		static int geos_const[2] = {0, -1}; int* geos;
		if (i < 4) geos = arg4+i*8;
		else geos = geos_const;
		// geometry を読み込みながら描画
		switch(geos[0]) {
			case 0: case 1: drawer.local_system.LoadToExistPDT(DEFAULT_PDT, buf, 0, 0,global_system.DefaultScreenWidth()-1, global_system.DefaultScreenHeight()-1, 0, 0);break;
			case 2: drawer.local_system.LoadToExistPDT(DEFAULT_PDT, buf, geos[1], geos[2], geos[3], geos[4], geos[5], geos[6]);break;
			case 3: drawer.local_system.LoadToExistPDT(DEFAULT_PDT, buf, geos[1], geos[2], geos[3], geos[4], geos[5], geos[6], geos[7]);break;
			default: drawer.local_system.LoadToExistPDT(DEFAULT_PDT, buf, 0, 0,global_system.DefaultScreenWidth()-1, global_system.DefaultScreenHeight()-1, 0, 0);break;
		}
		buf += strlen(buf)+1;
	}
	if (draw_flag) {
		drawer.local_system.DrawPDTBuffer(DEFAULT_PDT, drawer.local_system.DrawSel(sel) );
	} else {
		SEL_STRUCT* sel_p = drawer.local_system.DrawSel(sel);
		drawer.local_system.CopyPDTtoBuffer(
			sel_p->x1, sel_p->y1, sel_p->x2, sel_p->y2, DEFAULT_PDT,
			sel_p->x3, sel_p->y3, -1, 0);
	}
}

// PDT buffer の clear  /  fade
void SENARIO_Graphics::DoClear(int pdt, int x1, int y1, int x2, int y2, int c1, int c2, int c3) {
	local_system.ClearPDTRect(pdt, x1, y1, x2, y2, c1, c2, c3);
}
void SENARIO_GraphicsSaveBuf::SaveClear(int pdt, int x1, int y1, int x2, int y2, int c1, int c2, int c3) {
	c1 &= 0xff; c2 &= 0xff; c3 &= 0xff;
	int col = (c1<<16) | (c2<<8) | c3;
	cmd = 0x32;
	arg4[0] = x1; arg4[1] = y1;
	arg4[2] = x2; arg4[3] = y2;
	arg4[4] = pdt; arg2 = col;
}
void SENARIO_GraphicsSaveBuf::DoClear(SENARIO_Graphics& drawer) {
	drawer.DoClear(arg4[4], arg4[0], arg4[1], arg4[2], arg4[3], 
		(arg2>>16)&0xff, (arg2>>8)&0xff, (arg2)&0xff);
}
void SENARIO_Graphics::DoFade(int pdt, int x1, int y1, int x2, int y2, int c1, int c2, int c3, int count) {
	local_system.FadePDTBuffer(pdt, x1, y1, x2, y2, c1, c2, c3, count);
}
void SENARIO_GraphicsSaveBuf::SaveFade(int pdt, int x1, int y1, int x2, int y2, int c1, int c2, int c3, int count) {
	cmd = 0x4c;
	arg4[0] = x1; arg4[1] = y1;
	arg4[2] = x2; arg4[3] = y2;
	arg4[4] = pdt;
	arg4[5] = c1; arg4[6] = c2; arg4[7] = c3;
	arg4[8] = count;
}
void SENARIO_GraphicsSaveBuf::DoFade(SENARIO_Graphics& drawer) {
	drawer.DoFade(arg4[4], arg4[0], arg4[1], arg4[2], arg4[3], 
		arg4[5], arg4[6], arg4[7], arg4[8]);
}

// 0x67 - 1
void SENARIO_Graphics::DoCopy(int src_pdt, int src_x1, int src_y1, int src_x2, int src_y2,
	int dest_pdt, int dest_x, int dest_y, int fade) {
	local_system.CopyBuffer(src_x1, src_y1,
		src_x2, src_y2, src_pdt, dest_x, dest_y, dest_pdt, fade);
}
void SENARIO_GraphicsSaveBuf::SaveCopy(int src_pdt, int x1, int y1, int x2, int y2, int dest_pdt, int x3, int y3, int fade) {
	cmd = 0x64;
	arg4[0] = x1; arg4[1] = y1;
	arg4[2] = x2; arg4[3] = y2;
	arg4[4] = src_pdt;
	arg4[5] = x3; arg4[6] = y3;
	arg4[7] = dest_pdt;
	arg4[8] = fade;
	return;
}
void SENARIO_GraphicsSaveBuf::DoCopy(SENARIO_Graphics& drawer) {
	drawer.DoCopy(arg4[4], arg4[0], arg4[1], arg4[2], arg4[3],
		arg4[7], arg4[5], arg4[6], arg4[8]);
}

// 0x67 - 2
void SENARIO_Graphics::DoCopyWithMask(int src_pdt, int src_x1, int src_y1, int src_x2, int src_y2,
	int dest_pdt, int dest_x, int dest_y, int fade) {
	local_system.CopyPDTtoBuffer(src_x1, src_y1,
		src_x2, src_y2, src_pdt, dest_x, dest_y, dest_pdt, fade);
}
void SENARIO_GraphicsSaveBuf::SaveCopyWithMask(int src_pdt, int x1, int y1, int x2, int y2, int dest_pdt, int x3, int y3, int fade) {
	cmd = 0x66;
	arg4[0] = x1; arg4[1] = y1;
	arg4[2] = x2; arg4[3] = y2;
	arg4[4] = src_pdt;
	arg4[5] = x3; arg4[6] = y3;
	arg4[7] = dest_pdt;
	arg4[8] = fade;
	return;
}
void SENARIO_GraphicsSaveBuf::DoCopyWithMask(SENARIO_Graphics& drawer) {
	drawer.DoCopyWithMask(arg4[4], arg4[0], arg4[1], arg4[2], arg4[3],
		arg4[7], arg4[5], arg4[6], arg4[8]);
}

// 0x67 - 3
void SENARIO_Graphics::DoCopyWithoutColor(int src_pdt, int src_x1, int src_y1, int src_x2, int src_y2,
	int dest_pdt, int dest_x, int dest_y, int c1, int c2, int c3) {
	c1 &= 0xff; c2 &= 0xff; c3 &= 0xff;
	local_system.CopyWithoutColor(src_x1, src_y1,
		src_x2, src_y2, src_pdt, dest_x, dest_y, dest_pdt, c1, c2, c3);
}
void SENARIO_GraphicsSaveBuf::SaveCopyWithoutColor(int src_pdt, int x1, int y1, int x2, int y2, int dest_pdt, int x3, int y3, int c1, int c2, int c3) {
	c1 &= 0xff; c2 &= 0xff; c3 &= 0xff;
	cmd = 0x68;
	arg4[0] = x1; arg4[1] = y1;
	arg4[2] = x2; arg4[3] = y2;
	arg4[4] = src_pdt;
	arg4[5] = x3; arg4[6] = y3;
	arg4[7] = dest_pdt;
	arg4[8] = (c1<<16) | (c2<<8) | c3;
	return;
}
void SENARIO_GraphicsSaveBuf::DoCopyWithoutColor(SENARIO_Graphics& drawer) {
	drawer.DoCopyWithoutColor(arg4[4], arg4[0], arg4[1], arg4[2], arg4[3],
		arg4[7], arg4[5], arg4[6], (arg4[8]>>16)&0xff, (arg4[8]>>8)&0xff, (arg4[8])&0xff);
}

// 0x67 - 5
void SENARIO_Graphics::DoSwap(int src_pdt, int src_x1, int src_y1, int src_x2, int src_y2,
	int dest_pdt, int dest_x, int dest_y) {
	local_system.SwapBuffer(src_x1, src_y1,
		src_x2, src_y2, src_pdt, dest_x, dest_y, dest_pdt);
}
void SENARIO_GraphicsSaveBuf::SaveSwap(int src_pdt, int x1, int y1, int x2, int y2, int dest_pdt, int x3, int y3) {
	cmd = 0x6a;
	arg4[0] = x1; arg4[1] = y1;
	arg4[2] = x2; arg4[3] = y2;
	arg4[4] = src_pdt;
	arg4[5] = x3; arg4[6] = y3;
	arg4[7] = dest_pdt;
	return;
}
void SENARIO_GraphicsSaveBuf::DoSwap(SENARIO_Graphics& drawer) {
	drawer.DoSwap(arg4[4], arg4[0], arg4[1], arg4[2], arg4[3],
		arg4[7], arg4[5], arg4[6]);
}

void SENARIO_GraphicsSaveBuf::SaveSaveScreen(void) {
	cmd = 0xaa;
	/* arg4[0] = SENARIO_Graphics::StoreLen() */
}
/* 必要なし
void SENARIO_GraphicsSaveBuf::SaveRestoreScreen(void) {
	cmd = 0xac;
}
*/

void SENARIO_GraphicsSaveBuf::SaveCopytoScreen(void) {
	cmd = 0xf0;
	return;
}

void SENARIO_GraphicsSaveBuf::DoCopytoScreen(SENARIO_Graphics& drawer, int draw_flag) {
	if (draw_flag)
		drawer.DoCopy(-1,0,0,global_system.DefaultScreenWidth()-1, global_system.DefaultScreenHeight()-1,0,0,0,0);
}

void SENARIO_Graphics::DecodeSenario_Fade(SEL_STRUCT* sel, int c1, int c2, int c3) {
	// 内容が変化
	Change();
	Alloc().SaveClear(1, 0, 0, global_system.DefaultScreenWidth()-1, global_system.DefaultScreenHeight()-1, c1, c2, c3);
	Alloc().SaveLoadDraw("?",sel);

	if (local_system.GrpFastMode() == 3) return;
	// 画像処理関係をするときは、テキストウィンドウを閉じる
	local_system.DeleteReturnCursor();
	local_system.DeleteTextWindow();
	local_system.ClearPDTBuffer(1,c1,c2,c3);
	local_system.DrawPDTBuffer(1, sel);
}

// シナリオのデコード
int SENARIO_Graphics::DecodeSenario_GraphicsLoad(SENARIO_DECODE& decoder) // cmd 0x0b
{
	// 内容が変化
	Change();

	char buf[1024]; char* str; int arg; //, arg2, arg3, arg4, arg5, arg6;
	int i;
	int subcmd = decoder.NextCharwithIncl();
	// 画像処理関係をするときは、テキストウィンドウを閉じる
	if (local_system.GrpFastMode() != 3) {
		local_system.DeleteReturnCursor();
		local_system.DeleteTextWindow();
	}
	p(("cmd 0x0b - %x: ",subcmd));

	switch(subcmd) {
		case 0x05: case 0x03: case 0x01:
			if (subcmd == 5) { // 描画途中なら終了する
			}
			if (subcmd == 5) ClearBuffer(); // 保存していたグラフィックを破棄
			str = decoder.ReadString(buf);
			arg = decoder.ReadData();
			DoLoadDraw(str, local_system.DrawSel(arg));
			if (subcmd != 1)
				Alloc().SaveLoadDraw(str, local_system.DrawSel(arg));
			CheckCgm(str, decoder.Flags());
			p(("filename : %s, SelBuf number %d : draw image immediately \n",str,arg));
			break;
		case 0x06: case 0x04: case 0x02:

			str = decoder.ReadString(buf);
			// sel の読み込み
			{ SEL_STRUCT sel;
				sel.x1 = decoder.ReadData();
				sel.y1 = decoder.ReadData();
				sel.x2 = decoder.ReadData();
				sel.y2 = decoder.ReadData();
				sel.x3 = decoder.ReadData();
				sel.y3 = decoder.ReadData();
				sel.wait_time = decoder.ReadData();
				sel.sel_no = decoder.ReadData();
				sel.kasane = decoder.ReadData();
				sel.arg1 = decoder.ReadData();
				sel.arg2 = decoder.ReadData();
				sel.arg3 = decoder.ReadData();
				sel.arg4 = decoder.ReadData();
				sel.arg5 = decoder.ReadData();
				sel.arg6 = decoder.ReadData();
				if (subcmd == 6) { // 描画途中なら終了する
				}
				if (subcmd == 6) ClearBuffer(); // 保存していたグラフィックを破棄
				DoLoadDraw(str, &sel);
				if (subcmd != 1)
					Alloc().SaveLoadDraw(str, &sel);
				p(("filename : %s, sel : ",str));
				p(("(%d,%d)-(%d,%d) -> (%d,%d) ", sel.x1,sel.y1,sel.x2,sel.y2,sel.x3,sel.y3));
				p(("SelBuf %d : wait %d, kasane %d, ",sel.sel_no, sel.wait_time, sel.kasane));
				p(("arg %d,%d,%d,%d,%d,%d : draw image immediately\n",sel.arg1,sel.arg2,sel.arg3,sel.arg4,sel.arg5,sel.arg6));
			CheckCgm(str, decoder.Flags());
			break;
			}
		case 0x08:
		/* ??? */
			DoCopy(0, 0, 0, global_system.DefaultScreenWidth()-1, global_system.DefaultScreenHeight()-1, 1, 0, 0, 0);
			pp(("0x0b - 0x08 : ???; copy pdt 1 -> 0?\n"));
			break;
		// 画面への直接書き込み (?)
		case 0x09:
			str = decoder.ReadString(buf);
			arg = decoder.ReadData();
			DoLoad(str, arg);
			p(("filename : %s arg = %d, direct draw to screen.\n",str, arg));
			if (arg == 0) {
				Alloc().SaveLoad(str, -1);
				Alloc().SaveCopytoScreen();
			} else {
				Alloc().SaveLoad(str, arg);
			}
			// local_system.LoadPDTBuffer(arg, str);
			CheckCgm(str, decoder.Flags());
			break;
		// PDT バッファへの読み込み
		case 0x10:
			str = decoder.ReadString(buf);
			arg = decoder.ReadData();
			DoLoad(str, arg);
			if (arg == 0) {
				Alloc().SaveLoad(str, -1);
				Alloc().SaveCopytoScreen();
			} else {
				Alloc().SaveLoad(str, arg);
			}
			p(("filename : %s, pdt number = %d\n",str,arg));
			
			// local_system.LoadPDTBuffer(arg, str);
			CheckCgm(str, decoder.Flags());
			break;
		case 0x22: case 0x24:	
		{
			int geos_orig[128]; memset(geos_orig, 0, 32*sizeof(int));
			int* geos[16];
			for (i=0; i<16; i++) geos[i] = geos_orig+i*8;
			int images = decoder.NextCharwithIncl();
			int cmd;
			char* buf2 = buf;
			p(("cmd 0x0b - %x: ",subcmd));
			if (subcmd == 0x22) {
				ClearBuffer();
				char* fname = decoder.ReadString(buf2);
				if (buf2 != fname) strcpy(buf2, fname);
				cmd = 0x96;
				local_system.LoadPDTBuffer(DEFAULT_PDT, fname);
				CheckCgm(fname, decoder.Flags());
				p(("file : %s,",fname));
			} else if (subcmd == 0x24) {
				cmd = 0x97;
				int pdt = decoder.ReadData();
				if (pdt != DEFAULT_PDT) {
					local_system.DisconnectPDT(DEFAULT_PDT);
					local_system.CopyBuffer(0, 0, global_system.DefaultScreenWidth()-1, global_system.DefaultScreenHeight()-1, pdt,
						0, 0, DEFAULT_PDT, 0);
				}
				sprintf(buf2, "%d", pdt);
				p(("pdt : %d, ",pdt));
			} else cmd = 0;
			buf2 += strlen(buf2)+1;
			int sel = decoder.ReadData();
			p((" SelBuf number %d :\n",sel));
			// 重ねる画像の読み込み
			for (i=0; i<images; i++) {
				int sort = decoder.NextCharwithIncl();
				char* fname = decoder.ReadString(buf2);
				if (buf2 != fname) strcpy(buf2, fname);
				p(("    %dth : %d , %s ; ",i,sort,fname));
				switch(sort) {
					case 1:
						local_system.LoadToExistPDT(DEFAULT_PDT, fname, 0, 0,global_system.DefaultScreenWidth()-1, global_system.DefaultScreenHeight()-1, 0, 0);
						CheckCgm(fname, decoder.Flags());
						if (i<16) geos[i][0] = 0, geos[i][0] = -1;
						p(("-1\n"));
						break;
					case 2: {
						int flag = decoder.ReadData();
						p(("flag : %d\n",flag));
						SEL_STRUCT* sel = local_system.DrawSel(flag);
						if (sel == 0) break;
						local_system.LoadToExistPDT(DEFAULT_PDT, fname, 
							sel->x1, sel->y1, sel->x2, sel->y2, sel->x3, sel->y3);
						CheckCgm(fname, decoder.Flags());
						if (i<16) {
							/*geos[i][0] = 1, geos[i][0] = flag; */
							int* g = geos[i];
							g[0] = 2;
							g[1] = sel->x1; g[2]=sel->y1; g[3]=sel->x2;
							g[4] = sel->y2; g[5]=sel->x3; g[6]=sel->y3;
						}
						break;
					}
					case 3: {
						int x1,x2,x3,y1,y2,y3;
						x1 = decoder.ReadData();
						y1 = decoder.ReadData();
						x2 = decoder.ReadData();
						y2 = decoder.ReadData();
						x3 = decoder.ReadData();
						y3 = decoder.ReadData();
						if (i<16) { int* g = geos[i];
							g[0] = 2;
							g[1]=x1; g[2]=y1; g[3]=x2;
							g[4]=y2; g[5]=x3; g[6]=y3;
						}
						local_system.LoadToExistPDT(DEFAULT_PDT, fname, x1, y1, x2, y2, x3, y3);
						CheckCgm(fname, decoder.Flags());
						p(("(%d,%d)-(%d,%d)->(%d,%d)\n",x1,y1,x2,y2,x3,y3));
						break;
					}
					case 4: {
						int x1,x2,x3,y1,y2,y3;
						x1 = decoder.ReadData();
						y1 = decoder.ReadData();
						x2 = decoder.ReadData();
						y2 = decoder.ReadData();
						x3 = decoder.ReadData();
						y3 = decoder.ReadData();
						int flag = decoder.ReadData();
						if (i<16) { int* g = geos[i];
							g[0] = 3;
							g[1]=x1; g[2]=y1; g[3]=x2;
							g[4]=y2; g[5]=x3; g[6]=y3;
							g[7] = flag; // 重ね合わせの大きさ(255を最大とする割合)
						}
						local_system.LoadToExistPDT(DEFAULT_PDT, fname, x1, y1, x2, y2, x3, y3, flag);
						CheckCgm(fname, decoder.Flags());
						p(("(%d,%d)-(%d,%d)->(%d,%d) : flag = %d\n",x1,y1,x2,y2,x3,y3,flag));
						break;
					}
					default: pp(("Error!\n"));
				}
				buf2 += strlen(buf2)+1;
			}
			Alloc().SaveMultiLoad(cmd, buf, images+1, buf2-buf, sel, geos_orig);
			local_system.DrawPDTBuffer(DEFAULT_PDT, local_system.DrawSel(sel) );
			break;
		}
/* @@@ */
case 0x11:{ char buf[1024];
//arg2 = decoder.NextCharwithIncl();
//arg3 = decoder.ReadData();
//p(("unknown 0b. - 0x11(proceeding read?): arg2 = %d, arg3 = %d\n",arg2,arg3));
char* s = decoder.ReadString(buf);
p(("unknown 0b. - 0x11(proceeding read?) : arg = %s\n",s));
local_system.ReadPDTFile(s);

break;}
case 0x13:
pp(("unknown 0b. - %02x : clear proceeding read cache\n",subcmd));
break;
		case 0x30:
			p(("free graphics buffer.\n"));
			ClearBuffer();
			break;
		case 0x31:
			arg = decoder.ReadData();
			DeleteBuffer(arg);
			p(("free graphics buffer, length=%d\n",arg));
			break;
		case 0x32:
			arg = decoder.ReadData();
			pp(("??? graphics buffer, arg %d\n",arg));
			break;
		case 0x33:
			arg = decoder.ReadData();
			decoder.Flags().SetVar(arg, BufferLength() );
			p(("return graphics buffer length to var[%d]\n",arg));
			break;
		case 0x50:
			/* 画面の退避 */
			Alloc().SaveSaveScreen();
			p(("save screen to back buffer\n"));
			break;
		case 0x52:
			arg = decoder.ReadData();
			while( (!IsTopSaveScreen()) &&  BufferLength() > 0) DeleteBuffer(1);
			if (BufferLength() > 0) DeleteBuffer(1);
			Restore(0); /* 画像保存バッファの内容を復旧 */
			DoCopy(-1,0,0,global_system.DefaultScreenWidth()-1, global_system.DefaultScreenHeight()-1,DEFAULT_PDT,0,0,0);
			DoLoadDraw("?", local_system.DrawSel(arg));
			p(("restore screen from back buffer ; sel %d\n",arg));
			break;
		case 0x54:
			str = decoder.ReadString(buf);
			arg = decoder.ReadData();

			while( (!IsTopSaveScreen()) &&  BufferLength() > 0) DeleteBuffer(1);
			Restore(0); /* 画像保存バッファの内容を復旧 */
			DoCopy(-1,0,0,global_system.DefaultScreenWidth()-1, global_system.DefaultScreenHeight()-1,DEFAULT_PDT,0,0,0);
			/* 指定されたファイルを読み込んで重ねる */
			DoLoad(str,MIDDLE_PDT);
			DoCopyWithMask(MIDDLE_PDT, 0,0,global_system.DefaultScreenWidth()-1, global_system.DefaultScreenHeight()-1, DEFAULT_PDT,0,0,0);
			/* 画面に描画 */
			DoLoadDraw("?", local_system.DrawSel(arg));
			/* 同じ動作をスタックに積む */
			/* オリジナルでは 0xac というコマンドで全動作を実装 */
			Alloc().SaveCopy(-1,0,0,global_system.DefaultScreenWidth()-1, global_system.DefaultScreenHeight()-1,DEFAULT_PDT,0,0,0);
			Alloc().SaveLoad(str,MIDDLE_PDT);
			Alloc().SaveCopyWithMask(MIDDLE_PDT,0,0,global_system.DefaultScreenWidth()-1, global_system.DefaultScreenHeight()-1,DEFAULT_PDT,0,0,0);
			Alloc().SaveLoadDraw("?",local_system.DrawSel(arg));
			p(("restore screen from back buffer and draw file %s ; sel %d\n",str,arg));
			break;
		default: 
			return -1;
	}
	return 0;
}

void SENARIO_Graphics::DecodeSkip_GraphicsLoad(SENARIO_DECODE& decoder, char** filelist, int& list_pt, int max)
{
	char buf[1024]; char* str; int arg; // , arg2, arg3, arg4, arg5, arg6;
	int i;
	int subcmd = decoder.NextCharwithIncl();

	switch(subcmd) {
		case 0x05: case 0x03: case 0x01:
			str = decoder.ReadString(buf);
			if (list_pt < max && isalnum(str[0]))
				strcpy(filelist[list_pt++], str);
			decoder.ReadData();
			break;
		case 0x06: case 0x04: case 0x02:
			str = decoder.ReadString(buf);
			if (list_pt < max && isalnum(str[0]))
				strcpy(filelist[list_pt++], str);
			for (i=0; i<15; i++) decoder.ReadData();
			break;
		// PDT バッファへの読み込み
		case 0x10: case 0x09: case 0x54:
			str = decoder.ReadString(buf);
			if (list_pt < max && isalnum(str[0]))
				strcpy(filelist[list_pt++], str);
			arg = decoder.ReadData();
			break;
		case 0x11:
			str = decoder.ReadString(buf);
			if (list_pt < max && isalnum(str[0]))
				strcpy(filelist[list_pt++], str);
			break;
		case 0x22: case 0x24:	
		{
			int images = decoder.NextCharwithIncl();
			if (subcmd == 0x22) {
				char* fname = decoder.ReadString(buf); 
				if (list_pt < max && isalnum(fname[0]))
					strcpy(filelist[list_pt++], fname);
			} else if (subcmd == 0x24) {
				decoder.ReadData();
			}
			decoder.ReadData();
			// 重ねる画像の読み込み
			for (i=0; i<images; i++) {
				int sort = decoder.NextCharwithIncl();
				char* fname = decoder.ReadString(buf);
				if (list_pt < max && isalnum(fname[0]))
					strcpy(filelist[list_pt++], fname);
				switch(sort) {
					case 1: break;
					case 2: decoder.ReadData(); break;
					case 3: 
						for (i=0; i<6; i++) decoder.ReadData();
						break;
					case 4:
						for (i=0; i<7; i++) decoder.ReadData();
						break;
				}
			}
			break;
		}
		case 0x30: break;
		case 0x31: decoder.ReadData(); break;
		case 0x33: decoder.ReadData(); break;
		case 0x50: break;
		case 0x52: decoder.ReadData(); break;
		default: break;
	}
	return;
}

/* for debug */
#ifdef p
#  undef p
#endif /* defined(p) */

// #define DEBUG_Graphics
#ifdef DEBUG_Graphics
#define p(X) printf X
#else
#define p(X)
#endif

int SENARIO_Graphics::DecodeSenario_Graphics(SENARIO_DECODE& decoder)
{
	int arg;
	// 画像処理関係をするときは、テキストウィンドウを閉じる
	if (local_system.GrpFastMode() != 3) {
		local_system.DeleteReturnCursor();
		local_system.DeleteTextWindow();
	}
	switch(decoder.Cmd()) {
		case 0x63:
			arg = decoder.NextCharwithIncl();
			p(("cmd 0x63 - 0x%02x : ",arg));
			if (arg == 0x20) {
				decoder.NextCharwithIncl();
				arg = decoder.ReadData();
				p(("cmd 0x63 - 0x20 : bitblt to mainWindowDC ; %d\n",arg));
/* @@@ */
			} else if (arg == 1) {
				int x1 = decoder.ReadData(); int y1 = decoder.ReadData();
				int x2 = decoder.ReadData(); int y2 = decoder.ReadData();
				int pdt = decoder.ReadData();
				pp(("??? graphics ??? cmd 0x63 - 1 , %d:(%d,%d)-(%d,%d)\n",
					pdt,x1,y1,x2,y2));
/* @@@ */
			} else if (arg == 2) {
				int x1 = decoder.ReadData(); int y1 = decoder.ReadData();
				int pdt = decoder.ReadData();
				pp(("??? graphics ??? cmd 0x63 - 2, %d:(%d,%d)\n",
					pdt,x1,y1));
/* @@@ */
			} else if (arg == 0x10) {
				pp(("??? graphics ??? cmd 0x63 - 0x10\n"));
/* @@@ */
			} else return -1;
			break;
		case 0x64: {
			arg = decoder.NextCharwithIncl();
			if (arg == 2) {
				// 指定された領域を消去
				int x1 = decoder.ReadData(); int y1 = decoder.ReadData();
				int x2 = decoder.ReadData(); int y2 = decoder.ReadData();
				int pdt = decoder.ReadData();
				int c1 = decoder.ReadData(); int c2 = decoder.ReadData(); int c3 = decoder.ReadData();
				p(("cmd 0x64 - 2 : clear rect : %d:(%d,%d)-(%d,%d) , color %d,%d,%d\n",
					pdt, x1,y1, x2,y2, c1,c2,c3));
				local_system.ClearPDTRect(pdt, x1,y1, x2,y2, c1,c2,c3);
				if (pdt == 0) {
					Alloc().SaveClear(-1, x1, y1, x2, y2, c1, c2, c3);
					Alloc().SaveCopytoScreen();
				} else {
					Alloc().SaveClear(pdt, x1, y1, x2, y2, c1, c2, c3);
				}
			} else if (arg == 4) {
				int arg1 = decoder.ReadData();
				int arg2 = decoder.ReadData();
				int arg3 = decoder.ReadData();
				int arg4 = decoder.ReadData();
				int arg5 = decoder.ReadData();
				int arg6 = decoder.ReadData();
				int arg7 = decoder.ReadData();
				int arg8 = decoder.ReadData();
				pp(("cmd 0x64 - 4 : ??? arg = %d,%d, %d,%d, %d, %d,%d,%d(axis,pdt,color?)\n",
					arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8));
			} else if (arg == 6) {
				int x1 = decoder.ReadData();
				int y1 = decoder.ReadData();
				int x2 = decoder.ReadData();
				int y2 = decoder.ReadData();
				int pdt = decoder.ReadData();
				int c1 = decoder.ReadData();
				int c2 = decoder.ReadData();
				int c3 = decoder.ReadData();
				int c4 = decoder.ReadData();
				int c5 = decoder.ReadData();
				int c6 = decoder.ReadData();
				pp(("cmd 0x64 - 6 : ??? arg = (%d,%d)-(%d,%d), %d, (%d,%d,%d), (%d,%d,%d)\n",
					x1,y1,x2,y2,pdt,c1,c2,c3,c4,c5,c6));
			} else if (arg == 7) {
				int x1 = decoder.ReadData(); int y1 = decoder.ReadData();
				int x2 = decoder.ReadData(); int y2 = decoder.ReadData();
				int pdt = decoder.ReadData();
				p(("cmd 0x64 - 7 : invert color %d:(%d,%d)-(%d,%d)\n",
					pdt,x1,y1,x2,y2));
				local_system.InvertColor(pdt, x1, y1, x2, y2);
			} else if (arg == 16) {
/*@@@*/
/* 好き好き大好きで使用。c1,c2,c3 に向けて画面全体をすこし fade するらしい */
				int x1 = decoder.ReadData(); int y1 = decoder.ReadData();
				int x2 = decoder.ReadData(); int y2 = decoder.ReadData();
				int pdt = decoder.ReadData();
				int c1 = decoder.ReadData(); int c2 = decoder.ReadData(); int c3 = decoder.ReadData();
				pp(("cmd 0x64 - 16 : ??? graphics ??? cmd 0x64 - 16 , %d:(%d,%d)-(%d,%d) , color %d,%d,%d\n",
					pdt,x1,y1,x2,y2,c1,c2,c3));
			} else if (arg == 18) {
			// from akz. version
				int x1 = decoder.ReadData(); int y1 = decoder.ReadData();
				int x2 = decoder.ReadData(); int y2 = decoder.ReadData();
				int pdt = decoder.ReadData();
				p(("cmd 0x64 - 18 : dark rect : %d : (%d,%d)-(%d,%d)\n",pdt,x1,y1,x2,y2));
				local_system.FadePDTBuffer(pdt, x1,y1, x2,y2, 0, 0, 0, 0xc0);
				if (pdt == 0) {
					Alloc().SaveFade(-1, x1, y1, x2, y2, 0, 0, 0, 0xc0);
					Alloc().SaveCopytoScreen();
				} else {
					Alloc().SaveFade(pdt, x1, y1, x2, y2, 0, 0, 0, 0xc0);
				}
			} else if (arg == 17) {
				// (x1,y1,x2,y2), pdt
				// 座標で指定されたところをすこし fade out する
				int x1 = decoder.ReadData(); int y1 = decoder.ReadData();
				int x2 = decoder.ReadData(); int y2 = decoder.ReadData();
				int pdt = decoder.ReadData();
				p(("cmd 0x64 - 17 : fade out rect : %d : (%d,%d)-(%d,%d)\n",pdt,x1,y1,x2,y2));
				local_system.FadePDTBuffer(pdt, x1,y1, x2,y2, 0, 0, 0, 0x80);
				if (pdt == 0) {
					Alloc().SaveFade(-1, x1, y1, x2, y2, 0, 0, 0, 0x80);
					Alloc().SaveCopytoScreen();
				} else {
					Alloc().SaveFade(pdt, x1, y1, x2, y2, 0, 0, 0, 0x80);
				}
			} else if (arg == 21) {
				// (x1,y1,x2,y2) ,pdt,(col,col,col,count)
				// 座標で指定された場所を count で指定されただけ fade out させる

				int x1 = decoder.ReadData(); int y1 = decoder.ReadData();
				int x2 = decoder.ReadData(); int y2 = decoder.ReadData();
				int pdt = decoder.ReadData();
				int c1 = decoder.ReadData(); int c2 = decoder.ReadData(); int c3 = decoder.ReadData();
				int count = decoder.ReadData();
				p(("cmd 0x64 - 21 : fade rect : %d:(%d,%d)-(%d,%d) , color %d,%d,%d , count %d\n",
					pdt, x1,y1, x2,y2, c1,c2,c3, count));
				local_system.FadePDTBuffer(pdt, x1,y1, x2,y2, c1,c2,c3, count);
				if (pdt == 0) {
					Alloc().SaveFade(-1, x1,y1, x2,y2, c1,c2,c3, count);
					Alloc().SaveCopytoScreen();
				} else {
					Alloc().SaveFade(pdt, x1,y1, x2,y2, c1,c2,c3, count);
				}
			} else if (arg == 32) {
				// make monochrome image
				int x1 = decoder.ReadData(); int y1 = decoder.ReadData();
				int x2 = decoder.ReadData(); int y2 = decoder.ReadData();
				int pdt = decoder.ReadData();
				p(("cmd 0x64 - 32 : make monochrome image : %d:(%d,%d) - (%d,%d)\n",
					pdt,x1,y1,x2,y2));
				local_system.ChangeMonochrome(pdt, x1, y1, x2, y2);
			} else if (arg == 48) {
				int x1 = decoder.ReadData(); int y1 = decoder.ReadData();
				int x2 = decoder.ReadData(); int y2 = decoder.ReadData();
				int src_pdt =decoder.ReadData();
				int x3 = decoder.ReadData(); int y3 = decoder.ReadData();
				int x4 = decoder.ReadData(); int y4 = decoder.ReadData();
				int dest_pdt =decoder.ReadData();
				p(("cmd 0x64 - 48 : stretch rect : %d: (%d,%d)-(%d,%d) -> %d:(%d,%d) - (%d,%d),"
					,src_pdt,x1,y1,x2,y2, dest_pdt,x3,y3,x4,y4,dest_pdt));
				local_system.StretchBuffer(x1,y1,x2,y2,src_pdt,x3,y3,x4,y4,dest_pdt);
			} else if (arg == 50) {
				int x1 = decoder.ReadData(); int y1 = decoder.ReadData();
				int x2 = decoder.ReadData(); int y2 = decoder.ReadData();
				int x3 = decoder.ReadData(); int y3 = decoder.ReadData();
				int x4 = decoder.ReadData(); int y4 = decoder.ReadData();
				int src_pdt = decoder.ReadData();
				int x5 = decoder.ReadData(); int y5 = decoder.ReadData();
				int x6 = decoder.ReadData(); int y6 = decoder.ReadData();
				int dest_pdt = decoder.ReadData();
				int count = decoder.ReadData(); int wait = decoder.ReadData();
				p(("cmd 0x64 - 50 : stretch rect : %d: from (%d,%d)-(%d,%d) to (%d,%d)-(%d,%d) -> %d:(%d,%d) - (%d,%d),"
					"count %d, wait %d\n",src_pdt,x1,y1,x2,y2,x3,y3,x4,y4,dest_pdt,x5,y5,x6,y6,count,wait));
				// dest == 0 と決め打ち。
				SEL_STRUCT sel;
				sel.x1 = x1; sel.x2 = x2; sel.x3 = x5;
				sel.y1 = y1; sel.y2 = y2; sel.y3 = y5;
				sel.arg1 = x6; sel.arg2 = y6;
				sel.arg3 = x3; sel.arg4 = y3; sel.arg5 = x4; sel.arg6 = (y4&0xffff) | (count<<16);
				sel.kasane = 0; sel.wait_time = wait;
				sel.sel_no = 200;
				local_system.DrawPDTBuffer(src_pdt, &sel);
			} else {
				pp(("cmd 0x64 - %d : ???? : Error !! ",arg));
				return -1;
			}
			break; }
		case 0x66: {
/* @@@ */
/* 文字描画だそうな */
			arg = decoder.NextCharwithIncl();
			if (arg == 2) {
				int x = decoder.ReadData(); int y = decoder.ReadData();
				int pdt = decoder.ReadData();
				int c1, c2, c3; char ktemp[1024];
				c1 = decoder.ReadData(); c2 = decoder.ReadData(); c3 = decoder.ReadData();
				TextAttribute str;
				decoder.ReadStringWithFormat(str);
				kconv((unsigned char*)str.Text(), (unsigned char*)ktemp);
				p(("!!! not supported ! cmd 0x66 - 2 : %d:(%d,%d), (%d,%d,%d) , %s\n",pdt,x,y,c1,c2,c3,ktemp));
				char* s = str.Text();
				local_system.DrawTextPDT(x,y,pdt,s,c1,c2,c3);
				break;
			} else return -1;
		}
		case 0x67:
			arg = decoder.NextCharwithIncl();
			p(("cmd 0x67 - 0x%02x : ",arg));
			if (arg == 0) {
				int x1 = decoder.ReadData();
				int y1 = decoder.ReadData();
				int x2 = decoder.ReadData();
				int y2 = decoder.ReadData();
				int src_pdt = decoder.ReadData();
				int flag = decoder.ReadData();
				p(("0x67 - 0 : x,y = (%d, %d) - (%d,%d) , src pdt = %d, update flag = %d\n",x1,y1,x2,y2,src_pdt,flag));
				if (src_pdt == 0) src_pdt = -1;
				/* flag の役割不定 */
				DoCopy(src_pdt, x1, y1, x2, y2, 0, x1, y1, 0);
			} else if (arg == 1) {
				int src_x = decoder.ReadData();
				int src_y = decoder.ReadData();
				int src_x2 = decoder.ReadData();
				int src_y2 = decoder.ReadData();
				int src_pdt = decoder.ReadData();
				int dest_x = decoder.ReadData();
				int dest_y = decoder.ReadData();
				int dest_pdt = decoder.ReadData();
				int flag = 0;
				if (local_system.Version() >= 2) flag = decoder.ReadData(); // new version
				DoCopy(src_pdt, src_x, src_y, src_x2, src_y2,
					dest_pdt, dest_x, dest_y, flag);
				if (dest_pdt == 0) {
					Alloc().SaveCopy(src_pdt, src_x, src_y, src_x2, src_y2,
						-1, dest_x, dest_y, flag);
					Alloc().SaveCopytoScreen();
				} else {
					Alloc().SaveCopy(src_pdt, src_x, src_y, src_x2, src_y2,
						dest_pdt, dest_x, dest_y, flag);
				}
				p((" %d:(%d,%d)-(%d,%d) -> %d:(%d,%d) : flag %d\n",
					src_pdt,src_x, src_y, src_x2, src_y2, 
					dest_pdt, dest_x, dest_y, flag));
			}else if (arg == 2) {
				int src_x = decoder.ReadData();
				int src_y = decoder.ReadData();
				int src_x2 = decoder.ReadData();
				int src_y2 = decoder.ReadData();
				int src_pdt = decoder.ReadData();
				int dest_x = decoder.ReadData();
				int dest_y = decoder.ReadData();
				int dest_pdt = decoder.ReadData();
				int flag = 0;
				if (local_system.Version() > 0) flag = decoder.ReadData();
				DoCopyWithMask(src_pdt, src_x, src_y, src_x2, src_y2,
					dest_pdt, dest_x, dest_y, flag);
				if (dest_pdt == 0) {
					Alloc().SaveCopyWithMask(src_pdt, src_x, src_y, src_x2, src_y2,
						-1, dest_x, dest_y, flag);
					Alloc().SaveCopytoScreen();
				} else {
					Alloc().SaveCopyWithMask(src_pdt, src_x, src_y, src_x2, src_y2,
						dest_pdt, dest_x, dest_y, flag);
				}
				p((" %d:(%d,%d)-(%d,%d) -> %d:(%d,%d) : flag %d\n",
					src_pdt,src_x, src_y, src_x2, src_y2, 
					dest_pdt, dest_x, dest_y, flag));
			}else if (arg == 3) {
				int src_x = decoder.ReadData();
				int src_y = decoder.ReadData();
				int src_x2 = decoder.ReadData();
				int src_y2 = decoder.ReadData();
				int src_pdt = decoder.ReadData();
				int dest_x = decoder.ReadData();
				int dest_y = decoder.ReadData();
				int dest_pdt = decoder.ReadData();
				int c1 = decoder.ReadData();
				int c2 = decoder.ReadData();
				int c3 = decoder.ReadData();
				 DoCopyWithoutColor(src_pdt, src_x, src_y, src_x2, src_y2,
					dest_pdt, dest_x, dest_y, c1, c2, c3);
				if (dest_pdt == 0) {
					Alloc().SaveCopyWithoutColor(src_pdt, src_x, src_y, src_x2, src_y2,
						-1, dest_x, dest_y, c1, c2, c3);
					Alloc().SaveCopytoScreen();
				} else {
					Alloc().SaveCopyWithoutColor(src_pdt, src_x, src_y, src_x2, src_y2,
						dest_pdt, dest_x, dest_y, c1, c2, c3);
				}
				p(("0x67 - 03 : copy without color %d:(%d,%d)-(%d,%d) -> %d:(%d,%d) : color %d,%d,%d\n",
					src_pdt,src_x, src_y, src_x2, src_y2, 
					dest_pdt, dest_x, dest_y,c1,c2,c3));
			} else if (arg == 5) {
				int x1 = decoder.ReadData(); int y1 = decoder.ReadData();
				int x2 = decoder.ReadData(); int y2 = decoder.ReadData();
				int pdt = decoder.ReadData();
				int x3 = decoder.ReadData(); int y3 = decoder.ReadData();
				int dest_pdt = decoder.ReadData();
				p(("cmd 0x67 - 05 : swap region  %d:(%d,%d)-(%d,%d) , %d:(%d,%d)\n",
					pdt,x1,y1,x2,y2,dest_pdt,x3,y3));
				DoSwap(pdt, x1, y1, x2, y2,
					dest_pdt, x3, y3);
				if (dest_pdt == 0) {
					Alloc().SaveSwap(pdt, x1, y1, x2, y2,
						-1, x3, y3);
					Alloc().SaveCopytoScreen();
				} else {
					Alloc().SaveSwap(pdt, x1, y1, x2, y2,
						dest_pdt, x3, y3);
				}
			}else if (arg == 8) {
				int src_x = decoder.ReadData();
				int src_y = decoder.ReadData();
				int src_x2 = decoder.ReadData();
				int src_y2 = decoder.ReadData();
				int src_pdt = decoder.ReadData();
				int dest_x = decoder.ReadData();
				int dest_y = decoder.ReadData();
				int dest_pdt = decoder.ReadData();
				int flag = 0;
				if (local_system.Version() > 0) flag = decoder.ReadData();
				DoCopyWithMask(src_pdt, src_x, src_y, src_x2, src_y2,
					dest_pdt, dest_x, dest_y, flag);
				if (dest_pdt == 0) {
					Alloc().SaveCopyWithMask(src_pdt, src_x, src_y, src_x2, src_y2,
						-1, dest_x, dest_y, flag);
					Alloc().SaveCopytoScreen();
				} else {
					Alloc().SaveCopyWithMask(src_pdt, src_x, src_y, src_x2, src_y2,
						dest_pdt, dest_x, dest_y, flag);
				}
				pp(("0x68 - 8 is not supported; use cmd 0x67-2. %d:(%d,%d)-(%d,%d) -> %d:(%d,%d) : flag %d\n",
					src_pdt,src_x, src_y, src_x2, src_y2, 
					dest_pdt, dest_x, dest_y, flag));
			} else if (arg == 0x11) {
				int src_pdt, dest_pdt, fade = 0;
				src_pdt=decoder.ReadData();
				dest_pdt=decoder.ReadData();
				if (local_system.Version() >= 2) fade = decoder.ReadData(); // new version
				local_system.DisconnectPDT(dest_pdt);
				local_system.CopyBuffer(0, 0, global_system.DefaultScreenWidth()-1, global_system.DefaultScreenHeight()-1, src_pdt, 0, 0, dest_pdt,fade);
				if (dest_pdt == 0) {
					Alloc().SaveCopy(src_pdt, 0, 0, global_system.DefaultScreenWidth()-1, global_system.DefaultScreenHeight()-1, -1, 0, 0, fade);
					Alloc().SaveCopytoScreen();
				} else {
					Alloc().SaveCopy(src_pdt, 0, 0, global_system.DefaultScreenWidth()-1, global_system.DefaultScreenHeight()-1, dest_pdt, 0, 0, fade);
				}
				p(("pdt %d -> pdt %d, flag %d\n",src_pdt,dest_pdt,fade));
				p(("\n"));
			} else if (arg == 0x12) {
				int src_pdt, dest_pdt, fade=0;
				src_pdt=decoder.ReadData();
				dest_pdt=decoder.ReadData();
				if (local_system.Version() > 0) fade = decoder.ReadData();
				local_system.SyncPDT(dest_pdt);
				local_system.CopyPDTtoBuffer(0, 0, global_system.DefaultScreenWidth()-1, global_system.DefaultScreenHeight()-1, src_pdt, 0, 0, dest_pdt,fade);
				if (dest_pdt == 0) {
					Alloc().SaveCopyWithMask(src_pdt, 0, 0, global_system.DefaultScreenWidth()-1, global_system.DefaultScreenHeight()-1, -1, 0, 0, fade);
					Alloc().SaveCopytoScreen();
				} else {
					Alloc().SaveCopyWithMask(src_pdt, 0, 0, global_system.DefaultScreenWidth()-1, global_system.DefaultScreenHeight()-1, dest_pdt, 0, 0, fade);
				}
				p(("pdt %d -> pdt %d, flag %d\n",src_pdt,dest_pdt,fade));
				p(("\n"));
			// arg == 0x13: 3 と同じ方法で全画面描画
			// arg == 0x15: 5 と同じ方法で全画面描画
			// arg == 0x18: 8 と同じ方法で全画面描画
			} else if (arg == 0x20 || arg == 0x21 || arg == 0x22) {
				// arg==0x20: 0x67-1 と同様の方法で文字描画。引数１５個
				// arg==0x21: 0x67-2 と同様の方法で文字描画。引数１６個
				// arg==0x22: 0x67-3 と同様の方法で文字描画。引数１８個
				p(("cmd 0x67 - 0x%02x ; print number? \n",arg));
				int var = decoder.ReadData();
				int from_x = decoder.ReadData(); int from_y = decoder.ReadData();
				int from_w = decoder.ReadData(); int from_h = decoder.ReadData();
				int from_dx= decoder.ReadData(); int from_dy= decoder.ReadData();
				int from_pdt = decoder.ReadData();
				int to_x = decoder.ReadData(); int to_y = decoder.ReadData();
				int to_dx = decoder.ReadData(); int to_dy = decoder.ReadData();
				int keta = decoder.ReadData();
				int draw_flag = decoder.ReadData(); // 最初に０をつけるかつけないか
				int to_pdt = decoder.ReadData();
				int to_pdt_save = to_pdt;
				if (to_pdt_save == 0) to_pdt_save = -1;
				int flag=0; int c1=0,c2=0,c3=0;
				if (arg == 0x20) {
				} else if (arg == 0x21) {
					flag = decoder.ReadData();
				} else if (arg == 0x22) {
					c1 = decoder.ReadData();
					c2 = decoder.ReadData();
					c3= decoder.ReadData();
				}

				// 最大桁数以上をけずる
				var = decoder.Flags().GetVar(var);
				int i;int max_var = 1; for (i=0; i<keta; i++) max_var *= 10;
				if (var < 0) var = 0;
				var %= max_var;
				// コピー
				for (i=0; i<keta; i++) {
					max_var /= 10;
					int v = var/max_var; var %= max_var;
					if (v != 0 || i == keta-1) draw_flag = 1;
					int x = from_x + from_dx * v;
					int y = from_y + from_dy * v;
					if (draw_flag) {
						if (arg == 0x20) {
							DoCopy(from_pdt, x, y, x+from_w-1, y+from_h-1,
								to_pdt, to_x, to_y, 0);
							Alloc().SaveCopy(from_pdt, x, y, x+from_w-1, y+from_h-1,
								to_pdt_save, to_x, to_y, 0);
						} else if (arg == 0x21) {
							DoCopyWithMask(from_pdt, x, y, x+from_w-1, y+from_h-1,
								to_pdt, to_x, to_y, flag);
							Alloc().SaveCopyWithMask(from_pdt, x, y, x+from_w-1, y+from_h-1,
								to_pdt_save, to_x, to_y, flag);
						} else if (arg == 0x22) {
							DoCopyWithoutColor(from_pdt, x, y, x+from_w-1, y+from_h-1,
								to_pdt, to_x, to_y, c1, c2, c3);
							Alloc().SaveCopyWithoutColor(from_pdt, x, y, x+from_w-1, y+from_h-1,
								to_pdt_save, to_x, to_y, c1, c2, c3);
						}
					}
					to_x += to_dx; to_y += to_dy;
				}
				if (to_pdt == 0) Alloc().SaveCopytoScreen();
			} else {
				pp(("cmd 0x67 - %d : Error!!! ",arg));
				return -1;
			}
			break;
		case 0x68: {
			if (decoder.NextChar() == 0x10) {
/* @@@ */
				decoder.NextCharwithIncl();
				int c1,c2,c3; int time; int count;
				c1 = decoder.ReadData(); c2 = decoder.ReadData(); c3 = decoder.ReadData();
				time = decoder.ReadData(); count = decoder.ReadData();
				pp(("cmd 0x68 - 10 : blink screen, color %d,%d,%d; wait time %d, count %d\n",
					c1, c2, c3, time, count));
				local_system.BlinkScreen(c1, c2, c3, time, count);
				break;
			}
			if (decoder.NextChar() != 1) {
				pp(("cmd 0x68 - %d : Error!!! ",decoder.NextChar()));
				return -1;
			}
			decoder.NextCharwithIncl();
			int pdt=decoder.ReadData(); int c1=decoder.ReadData(); int c2=decoder.ReadData(); int c3=decoder.ReadData();
			p(("cmd 0x68 - 1 : ???  clear window: pdt %d,color = %d, %d, %d\n",pdt,c1,c2,c3));
			local_system.ClearPDTBuffer(pdt,c1,c2,c3);
			if (pdt == 0) local_system.ClearPDTBuffer(-1,c1,c2,c3);
			break; }
	}
	return 0;
}

void SENARIO_Graphics::DecodeSkip_Graphics(SENARIO_DECODE& decoder)
{
	int arg;
	switch(decoder.Cmd()) {
		case 0x63:
			arg = decoder.NextCharwithIncl();
			if (arg == 0x20) {
				decoder.NextCharwithIncl();
				decoder.ReadData();
			} else if (arg == 1) {
				decoder.ReadData(); decoder.ReadData();
				decoder.ReadData(); decoder.ReadData();
				decoder.ReadData();
			} else if (arg == 2) {
				decoder.ReadData();
				decoder.ReadData();
				decoder.ReadData();
			} else if (arg == 0x10) {
			}
			break;
		case 0x64: {
			arg = decoder.NextCharwithIncl();
			if (arg == 2) {
				int i ;for (i=0; i<8; i++) decoder.ReadData();
			} else if (arg == 4) {
				int i ;for (i=0; i<8; i++) decoder.ReadData();
			} else if (arg == 6) {
				int i ;for (i=0; i<11; i++) decoder.ReadData();
			} else if (arg == 7) {
				int i ;for (i=0; i<5; i++) decoder.ReadData();
			} else if (arg == 16) {
				int i ;for (i=0; i<8; i++) decoder.ReadData();
			} else if (arg == 17) {
				int i ;for (i=0; i<5; i++) decoder.ReadData();
			} else if (arg == 18) {
				int i ;for (i=0; i<5; i++) decoder.ReadData();
			} else if (arg == 21) {
				int i ;for (i=0; i<9; i++) decoder.ReadData();
			} else if (arg == 32) {
				int i ;for (i=0; i<5; i++) decoder.ReadData();
			} else if (arg == 48) {
				int i ;for (i=0; i<10; i++) decoder.ReadData();
			} else if (arg == 50) {
				int i ;for (i=0; i<16; i++) decoder.ReadData();
			} else {
			}
			break; }
		case 0x66:
			if (decoder.NextChar() == 0x02) {
				decoder.NextCharwithIncl();
				int i; for (i=0; i<6; i++) decoder.ReadData();
				TextAttribute str;
				decoder.ReadStringWithFormat(str,0);
			}
			break;

		case 0x67:
			arg = decoder.NextCharwithIncl();
			if (0) {
			} else if (arg == 0) {
				int i ;for (i=0; i<6; i++) decoder.ReadData();
			} else if (arg == 1) {
				int i ;for (i=0; i<8; i++) decoder.ReadData();
				if (local_system.Version() >= 2) decoder.ReadData(); // new version
			}else if (arg == 2) {
				int i ;for (i=0; i<9; i++) decoder.ReadData();
			}else if (arg == 3) {
				int i ;for (i=0; i<11; i++) decoder.ReadData();
			} else if (arg == 5) {
				int i ;for (i=0; i<8; i++) decoder.ReadData();
			} else if (arg == 8) {
				int i ;for (i=0; i<8; i++) decoder.ReadData();
				if (local_system.Version() > 0)  decoder.ReadData();
			} else if (arg == 0x11) {
				decoder.ReadData(); decoder.ReadData();
				if (local_system.Version() >= 2)  decoder.ReadData(); // new version
			} else if (arg == 0x12) {
				decoder.ReadData(); decoder.ReadData(); decoder.ReadData();
			} else if (arg == 0x13) {
				int i ;for (i=0; i<5; i++) decoder.ReadData();
			} else if (arg == 0x15) {
				decoder.ReadData(); decoder.ReadData();
			} else if (arg == 0x18) {
				decoder.ReadData(); decoder.ReadData();
				if (local_system.Version() > 0)  decoder.ReadData();
			} else if (arg == 0x20) {
				int i;for (i=0; i<15; i++) decoder.ReadData();
			} else if (arg == 0x21) {
				int i;for (i=0; i<16; i++) decoder.ReadData();
			} else if (arg == 0x22) {
				int i;for (i=0; i<18; i++) decoder.ReadData();
			} else {
			}
			break;
		case 0x68:
			if (decoder.NextChar() == 0x10) {
				decoder.NextCharwithIncl();
				int i;for (i=0; i<5; i++) decoder.ReadData();
				break;
			}
				
			if (decoder.NextChar() != 1) {

				return;
			}
			decoder.NextCharwithIncl();
			decoder.ReadData(); decoder.ReadData();
			decoder.ReadData(); decoder.ReadData();
			break;
	}
	return;
}

/* GraphicsSavebufが画面の更新をするか？ */
inline int SENARIO_GraphicsSaveBuf::IsDraw(void) {
	if (cmd == 0 || /* DoLoadDraw() */
	    cmd == 0x96 || cmd == 0x97 || /* DoMultiLoad */
	    cmd == 0xf0) /* CopytoScreen */
		return 1;
	return 0;
}

// グラフィックを実際に書き直す 
void SENARIO_Graphics::Restore(int draw_flag) {
	int i;
	// 画面のリストア
	// すべての PDT buffer をクリア
	// ただし pdt 0 はそのまま（画面の内容は保存）
	local_system.ClearAllPDT();
	// 画面を fade out : 必要なし？
	/* local_system.DrawPDTBuffer(1, local_system.DrawSel(0)); */

	/* 画面を更新するべき Do() の決定 */
	int draw_index = -1;
	for (i=0; i<deal; i++) {
		if (buf[i].IsDraw()) draw_index = i;
	}

	// リストア
	for (i=0; i<deal; i++) {
		if (i == draw_index)
			buf[i].Do(*this,draw_flag);
		else
			buf[i].Do(*this,0);
	}
}

void SENARIO_GraphicsSaveBuf::Do(SENARIO_Graphics& drawer, int draw_flag) {
	switch(cmd) {
		case 0: DoLoadDraw(drawer, draw_flag); break;
		case 2: DoLoad(drawer); break;
		case 0x32: DoClear(drawer); break;
		case 0x64: // copy : 0x66-1
			DoCopy(drawer); break;
		case 0x66: // copy : 0x67-2
			DoCopyWithMask(drawer); break;
		case 0x6a: // copy : 0x67-5
			DoSwap(drawer); break;
		case 0x96: case 0x97: DoMultiLoad(drawer, draw_flag); break;
		case 0xaa: break;
		case 0xac: break;
		case 0xf0: DoCopytoScreen(drawer, draw_flag); break;
		default: printf("Unknown graphcis save buffer command : %d\n",cmd); break;
	}
	return;
}
// グラフィックバッファを割り当て・解放
SENARIO_GraphicsSaveBuf& SENARIO_Graphics::Alloc(void) {
	if (deal == 32) {
	/*	memcpy(buf, buf+1, sizeof(SENARIO_GraphicsSaveBuf)*31); */
		deal--;
	}
	memset(buf+deal, 0, sizeof(SENARIO_GraphicsSaveBuf));
	return buf[deal++];
}

void SENARIO_Graphics::ClearBuffer(void) {
	memset(buf, 0, sizeof(SENARIO_GraphicsSaveBuf)*deal);
	deal = 0;
}

void SENARIO_Graphics::DeleteBuffer(int n) {
	if (deal <= n || n < 0) ClearBuffer();
	else {
		deal -= n;
		memset(buf+deal, 0, sizeof(SENARIO_GraphicsSaveBuf)*n);
	}
}


SENARIO_Graphics::SENARIO_Graphics(AyuSys& sys) :
	local_system(sys) {
	memset(buf, 0, sizeof(SENARIO_GraphicsSaveBuf)*32);
	deal = 0;
}
