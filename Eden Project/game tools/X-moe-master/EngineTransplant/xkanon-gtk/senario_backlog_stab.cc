/*  senario_backlog.cc
 *     バックログの操作用クラス SENARIO_BackLog の定義
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
#include<stdio.h>

#define BL_END 1
#define BL_RET 2
#define BL_TEXT 3
#define BL_SEL2 4
#define BL_SEL2R 5

static int bl_saveseen=-1, bl_savepoint=-1;
/* バックログの構造
** 	BL_END / BL_RET / BL_SEL2R: 1byte で 1命令
**	BL_TEXT:
**		BL_TEXT <senario_no> <seen_no> BL_TEXT の１０バイト
**	BL_SEL2:
**		BL_SEL2 00 <text> 00 BL_SEL2
**	これにより、逆からも読むことができる backlog となる。
*/

void SENARIO_BackLog::CutLog(void) {
	return;
}

/* セーブされたログのリストア */
void SENARIO_BackLog::SetLog(char* saved_log, unsigned int len, int is_save) {
	return;
}
/* ログをセーブする長さにカット */
void SENARIO_BackLog::PutLog(char* save_log, unsigned int log_len, int flag) {
	if (log_len < 4) return;
	*(unsigned char*)(save_log+0) = 0;
	*(unsigned char*)(save_log+1) = 0;
	*(unsigned char*)(save_log+2) = 0;
	*(unsigned char*)(save_log+3) = 0;
	return;
		
}

/* その他、ログの追加処理 */
void SENARIO_BackLog::AddEnd(void) {
}
void SENARIO_BackLog::AddEnd2(void) {
}
void SENARIO_BackLog::AddRet(void) {
}
void SENARIO_BackLog::AddSelect2Result(int n) {
}
void SENARIO_BackLog::AddText(int p, int seen, int issel) {
	bl_saveseen = seen;
	bl_savepoint = p;
}
void SENARIO_BackLog::AddSelect2(char* str) {
}
void SENARIO_BackLog::AddSetTitle(char* s, int sn, int p) {
	bl_saveseen = sn;
	bl_savepoint = p;
}
void SENARIO_BackLog::AddSavePoint(int p, int s){
	bl_saveseen = s;
	bl_savepoint = p;
}

GlobalStackItem SENARIO_BackLog::View(void) {
	return GlobalStackItem();
}
void SENARIO_BackLog::StartLog(int n) {}
SENARIO_BackLog::SENARIO_BackLog(SENARIO_DECODE& dec, AyuSys& sys) :
	decoder(dec), local_system(sys), old_stack(0x100), old_track(grp_info_orig[22]),
	old_grp_point(grp_info_orig[0]), grp_info_len(grp_info_orig[1]),
	grp_info(grp_info_orig+2), grp_hash(grp_info_orig+12) {
	log = log_orig = 0;
}
SENARIO_BackLog::~SENARIO_BackLog() {
}
void SENARIO_BackLog::GetSavePoint(int& seen, int& point,SENARIO_FLAGS ** fl, GosubStack ** st) {
	seen = bl_saveseen;
	point = bl_savepoint;
}
