/*  senario_patch.cc
 *     シナリオのパッチあて：
 *		xayusys と VasGame （本来のKANONのシステム）の
 *		非互換な部分をサポートするためのルーチン
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

#include "senario.h"
// #include<stdio.h>
// #include<unistd.h>
// #include<sys/types.h>
// #include<sys/stat.h>

SENARIO_PATCH* SENARIO_PATCH::head = 0;

SENARIO_PATCH::SENARIO_PATCH(char* id) {
	identifier = new char[strlen(id)+1]; strcpy(identifier, id);
	// SENARIO_PATCH のリストに自分を加える
	// 自分がすでにリストにあるなら、なにもしない
	if (head == 0) { head = this;
	} else {
		SENARIO_PATCH* current = head;
		while(current->next != 0) {
			if (strcmp(current->identifier, identifier) == 0) goto not_register;
			current = current->next;
		}
		if (strcmp(current->identifier, identifier) == 0) goto not_register;
		current->next = this;
	}
not_register:
	next = 0;
	return;
}

void SENARIO_PATCH::AddPatch(char* identifier) {
	// identifier を探す
	if (head == 0) return;
	SENARIO_PATCH* current = head;
	while(current != 0) {
		if (strcmp(current->identifier, identifier) == 0) {
			// みつかったら「使用中」フラグを立てる
			if (current->is_used) return;
			current->is_used = 1;
		}
		current = current->next;
	}
	return;
}

/* 返り値として origdata と異なる、新しい領域が new されて返る。
** また、その領域は末尾に 1024byte の余裕を持つ
*/
unsigned char* SENARIO_PATCH::PatchAll(int seen_no, const unsigned char* origdata, int len, int version) {
	unsigned char* cur_data = new unsigned char[len+1024];
	memcpy(cur_data, origdata, len);
	memset(cur_data+len, 0, 1024); /* 無駄な領域を 0で埋める */
	// パッチがないならなにもしない
	if (head == 0) return cur_data;
	// パッチあて
	SENARIO_PATCH* current = head;
	while(current != 0) {
		if (current->is_used)
			cur_data = current->Patch(seen_no, cur_data, len, version);
		current = current->next;
	}
	return cur_data;
	
}

// オープニングでゴミが出るのを防ぐ
class SENARIO_PATCH_Opening : public SENARIO_PATCH {
	unsigned char* Patch(int seen_no, unsigned char* data, int len, int version);
public:
	SENARIO_PATCH_Opening(void) : SENARIO_PATCH("opening") {};
};
static SENARIO_PATCH_Opening patch_opening;

unsigned char* SENARIO_PATCH_Opening::Patch(int seen_no, unsigned char* data, int len, int version) {

	if (seen_no != 502) return data;
	if (version == 1) {
		if (data[6504] == 0x46) data[6504] = 0x44;
	} else if (version == 2) {
		if (data[6532] == 0x46) data[6532] = 0x44;
	}
	return data;
}

// ススキノハラの約束デモでゴミが出るのを防ぐ
class SENARIO_PATCH_susukinohara_demo_Opening : public SENARIO_PATCH {
	unsigned char* Patch(int seen_no, unsigned char* data, int len, int version);
public:
	SENARIO_PATCH_susukinohara_demo_Opening(void) : SENARIO_PATCH("susukinohara-demo-opening") {};
};
static SENARIO_PATCH_susukinohara_demo_Opening patch_susukinohara_demo_opening;

unsigned char* SENARIO_PATCH_susukinohara_demo_Opening::Patch(int seen_no, unsigned char* data, int len, int version) {
	if (seen_no != 906) return data;
	if (data[772] == 0x90) data[772] = 0x08; /* if - jump を無効にする */
	return data;
}

#if 0
class SENARIO_PATCH_kanon_noMakoto : public SENARIO_PATCH {
	unsigned char* Patch(int seen_no, unsigned char* data, int len, int version);
public:
	SENARIO_PATCH_kanon_noMakoto(void) : SENARIO_PATCH("noMakoto") {};
};

static SENARIO_PATCH_kanon_noMakoto patch_kanon_noMakoto;

unsigned char* SENARIO_PATCH_kanon_noMakoto::Patch(int seen_no, unsigned char* data, int len, int version) {
	if (seen_no != 502) return data;
	if (version != 1) return;
	switch(seen_no) {
	case 100: {
		
		}
	}
	return data;
}

90
	JumpText(81546,81642,101222)
	JumpText(101258,101269,110074)
100
	DelText(2363),
	DelText(3754),
	DelText(55140),
	DelText(55189),
	DelText(28915),
	DelText(29430),
	JumpText(4424,4436,28508),
	JumpTextWithFade(79563,79578,85791), /* 13 01 11 */

110
	JumpText(966,977,4981),
#endif
