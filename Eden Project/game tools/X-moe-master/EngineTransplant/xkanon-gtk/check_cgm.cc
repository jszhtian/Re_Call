/*  check_cgm.cc  : CG MODE 用の画像チェック用の関数群 */

/*
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
#include <stdio.h>

// CG Mode の画像関係
void CgmInit(void);
int SearchCgmData(char* name);
struct CgmInfo* SetCgmData(char* name, int number);
void SetCgmFile(char* path, int flen);
int* CountCgmData(void);
int CgmDeal(void);
int GetCgmInfo(int number, const char** ret_filename);

static unsigned char decode_char[256] = {
	0x8b, 0xe5, 0x5d, 0xc3, 0xa1, 0xe0, 0x30, 0x44, 
	0x00, 0x85, 0xc0, 0x74, 0x09, 0x5f, 0x5e, 0x33, 
	0xc0, 0x5b, 0x8b, 0xe5, 0x5d, 0xc3, 0x8b, 0x45, 
	0x0c, 0x85, 0xc0, 0x75, 0x14, 0x8b, 0x55, 0xec, 
	0x83, 0xc2, 0x20, 0x52, 0x6a, 0x00, 0xe8, 0xf5, 
	0x28, 0x01, 0x00, 0x83, 0xc4, 0x08, 0x89, 0x45, 
	0x0c, 0x8b, 0x45, 0xe4, 0x6a, 0x00, 0x6a, 0x00, 
	0x50, 0x53, 0xff, 0x15, 0x34, 0xb1, 0x43, 0x00, 
	0x8b, 0x45, 0x10, 0x85, 0xc0, 0x74, 0x05, 0x8b, 
	0x4d, 0xec, 0x89, 0x08, 0x8a, 0x45, 0xf0, 0x84, 
	0xc0, 0x75, 0x78, 0xa1, 0xe0, 0x30, 0x44, 0x00, 
	0x8b, 0x7d, 0xe8, 0x8b, 0x75, 0x0c, 0x85, 0xc0, 
	0x75, 0x44, 0x8b, 0x1d, 0xd0, 0xb0, 0x43, 0x00, 
	0x85, 0xff, 0x76, 0x37, 0x81, 0xff, 0x00, 0x00, 
	0x04, 0x00, 0x6a, 0x00, 0x76, 0x43, 0x8b, 0x45, 
	0xf8, 0x8d, 0x55, 0xfc, 0x52, 0x68, 0x00, 0x00, 
	0x04, 0x00, 0x56, 0x50, 0xff, 0x15, 0x2c, 0xb1, 
	0x43, 0x00, 0x6a, 0x05, 0xff, 0xd3, 0xa1, 0xe0, 
	0x30, 0x44, 0x00, 0x81, 0xef, 0x00, 0x00, 0x04, 
	0x00, 0x81, 0xc6, 0x00, 0x00, 0x04, 0x00, 0x85, 
	0xc0, 0x74, 0xc5, 0x8b, 0x5d, 0xf8, 0x53, 0xe8, 
	0xf4, 0xfb, 0xff, 0xff, 0x8b, 0x45, 0x0c, 0x83, 
	0xc4, 0x04, 0x5f, 0x5e, 0x5b, 0x8b, 0xe5, 0x5d, 
	0xc3, 0x8b, 0x55, 0xf8, 0x8d, 0x4d, 0xfc, 0x51, 
	0x57, 0x56, 0x52, 0xff, 0x15, 0x2c, 0xb1, 0x43, 
	0x00, 0xeb, 0xd8, 0x8b, 0x45, 0xe8, 0x83, 0xc0, 
	0x20, 0x50, 0x6a, 0x00, 0xe8, 0x47, 0x28, 0x01, 
	0x00, 0x8b, 0x7d, 0xe8, 0x89, 0x45, 0xf4, 0x8b, 
	0xf0, 0xa1, 0xe0, 0x30, 0x44, 0x00, 0x83, 0xc4, 
	0x08, 0x85, 0xc0, 0x75, 0x56, 0x8b, 0x1d, 0xd0, 
	0xb0, 0x43, 0x00, 0x85, 0xff, 0x76, 0x49, 0x81, 
	0xff, 0x00, 0x00, 0x04, 0x00, 0x6a, 0x00, 0x76
};

#define CgmDealMax 512

struct CgmInfo {
	char* fname;
	int number;
	int hash;
};
static CgmInfo cgm_info[CgmDealMax];
static CgmInfo** cgm_info_list = 0;
static int cgm_info_deal = 0;

void CgmInit(void) {
	int i;
	for (i=0; i<CgmDealMax; i++) {
		cgm_info[i].fname = 0;
		cgm_info[i].number = 0;
		cgm_info[i].hash = 0;
	}
}
static int Hash(char* fname) {
	int h = 0;
	while(*fname != 0) {
		h = *fname + ((h * (0x9449+*fname))>>7);
		fname++;
	}
	return h;
}

CgmInfo* SetCgmData(char* name, int number) {
	int hash = Hash(name);
	int cur = hash;
	while(1) {
		cur &= CgmDealMax-1;
		if (cgm_info[cur].fname == 0) {// not found
			cgm_info[cur].hash = hash;
			cgm_info[cur].number = number;
			cgm_info[cur].fname = new char[strlen(name)+1];
			strcpy(cgm_info[cur].fname, name);
			return &cgm_info[cur];
		}
		if (cgm_info[cur].hash == hash &&
			strcmp(cgm_info[cur].fname, name) == 0) {// found
			cgm_info[cur].number = number;
			return 0;
		}
		cur++;
	}
}

int SearchCgmData(char* name) {
	int hash = Hash(name);
	int cur = hash;
	while(1) {
		cur &= CgmDealMax-1;
		if (cgm_info[cur].fname == 0) {
			return -1; // not found
		}
		if (cgm_info[cur].hash == hash &&
			strcmp(cgm_info[cur].fname, name) == 0) {
			return cgm_info[cur].number;	// found
		}
		cur++;
	}
}

void SetCgmFile(char* fbuf, int flen) {
	int i;
	// チェック
	if ( strncmp(fbuf, "CGMDT", 5) != 0) return;
	int cgm_deal = read_little_endian_int(fbuf+0x10) & 0xffff;
	int dest_len = cgm_deal*36;
	// リストの大きさを変更
	if (cgm_info_list == 0) {
		cgm_info_list = new CgmInfo* [cgm_deal];
		cgm_info_deal = 0;
	} else {
		CgmInfo** new_cgm_info_list = new CgmInfo* [cgm_info_deal + cgm_deal];
		memcpy(new_cgm_info_list, cgm_info_list, sizeof(CgmInfo*) * cgm_info_deal);
		delete[] cgm_info_list;
		cgm_info_list = new_cgm_info_list;
	}
	// xor 解除
	for (i=0;i<flen-0x20; i++) {
		fbuf[i+0x20]^=decode_char[i&0xff];
	}
	// 展開
	char* dest = new char[dest_len+256];
	char* src = fbuf+0x30;
	char* dest_orig = dest;
	ARCINFO::Extract(dest,src,dest+dest_len,fbuf+flen);
	dest = dest_orig;
	// 登録
	for (i=0; i<cgm_deal; i++) {
		int n = read_little_endian_int(dest+32);
		CgmInfo* new_info = SetCgmData(dest, n);
		if (new_info) cgm_info_list[cgm_info_deal++] = new_info;
		dest += 36;
	}
	delete[] dest_orig;
}

int* CountCgmData(void) {
	int i; int deal = 0;
	for (i=0; i<CgmDealMax; i++) {
		if (cgm_info[i].fname != 0) deal++;
	}
	int* ret = new int[deal+2];
	ret[0] = deal; deal = 1;
	for (i=0; i<CgmDealMax; i++) {
		if (cgm_info[i].fname != 0) {
			ret[deal++] = cgm_info[i].number;
		}
	}
	ret[deal] = -1;
	return ret;
}

int CgmDeal(void) {
	return cgm_info_deal;
}

int GetCgmInfo(int number, const char** ret_filename) {
	if (number < 0 || number >= cgm_info_deal) return -1;
	if (cgm_info_list[number] == 0) return -1;
	if (ret_filename) *ret_filename = cgm_info_list[number]->fname;
	return cgm_info_list[number]->number;
	
}
