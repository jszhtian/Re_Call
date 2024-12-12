/*  senario_backlog.h
 *     backlog で使われるコマンドの表
 *     その他、定数など。
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

#ifndef __KANON_SENARIO_BACKLOG_H__
#define __KANON_SENARIO_BACKLOG_H__

#define BL_END 1
#define BL_RET 2
#define BL_TEXT 3
#define BL_SEL2 4
#define BL_SEL2R 5
#define BL_END2 6
#define BL_MSGPOS 7
#define BL_MSGSIZ 8
#define BL_MOJSIZ 9
#define BL_ISWAKU 10
#define BL_SEL2S 11
/* #define BL_TEXT_WI 12 */
/* #define BL_SEL2S_WI 13 */
#define BL_FLAG_INF 14
#define BL_GRP_INF 15
#define BL_MUS_INF 16
#define BL_STACK_INF 17
#define BL_TITLE 18
#define BL_SAVEPT 19
#define BL_GRP_INF2 20
/* #define BL_TITLE_WI 21 */
/* #define BL_NULL_WI 22 */
#define BL_MSGPOS2 23
#define BL_SEL1S 24
#define BL_SYS_INF 25
#define BL_SYSORIG_INF 26
#define BL_END3 27
#define BL_KOE 28
#define BL_MAX 28

/* 各 atom の長さ。-1 の場合、次/前の2byteに長さが入る */
static int bl_len[BL_MAX+2] = {
	0,
	1,  /* BL_END */
	1,  /* BL_RET */
	8, /* BL_TEXT */
	-1, /* BL_SEL2 */
	6,  /* BL_SEL2R */
	1,  /* BL_END2 */
	18, /* BL_MSGPOS */
	18, /* BL_MSGSIZ */
	18, /* BL_MOJSIZ */
	4,  /* BL_ISWAKU */
	8,  /* BL_SEL2S */
	24, /* BL_TEXT_WI */
	24, /* BL_SEL2S_WI */
	-1, /* BL_FLAG_INF */
	-1, /* BL_GRP_INF */
	-1, /* BL_MUS_INF */
	-1, /* BL_STACK_INF */
	-1, /* BL_TITLE */
	8, /* BL_SAVEPT */
	10, /* BL_GRP_INF2 */
	-1, /* BL_TITLE_WI */
	18, /* BL_NULL_WI */
	19, /* BL_MSGPOS2 */
	8,  /* BL_SEL1S */
	-1, /* BL_SYS_INF */
	-1, /* BL_SYSORIG_INF */
	1,  /* BL_END3 */
	8,  /* BL_KOE */
	0
};

static const char* bl_name[BL_MAX+2] = {
	"BL_INVALID",
	"BL_END",
	"BL_RET",
	"BL_TEXT",
	"BL_SEL2",
	"BL_SEL2R",
	"BL_END2",
	"BL_MSGPOS",
	"BL_MSGSIZ",
	"BL_MOJSIZ",
	"BL_ISWAKU",
	"BL_SEL2S",
	"BL_TEXT_WI",
	"BL_SEL2S_WI",
	"BL_FLAG_INF",
	"BL_GRP_INF",
	"BL_MUS_INF",
	"BL_STACK_INF",
	"BL_TITLE",
	"BL_SAVEPT",
	"BL_GRP_INF2",
	"BL_TITLE_WI",
	"BL_NULL_WI",
	"BL_MSGPOS2"
	"BL_SEL1S",
	"BL_SYS_INF",
	"BL_SYSORIG_INF",
	"BL_END3",
	"BL_KOE",
	"BL_INVALID"
};

#ifdef __KANON_SENARIO_H__
typedef void (SENARIO_BackLog::*BL_FUNC)(AyuSys& sys);
typedef void (SENARIO_BackLog::*BL_DUMP_FUNC)(FILE* out, const char* tab);
/* backlog の実行処理：順方向 */
static BL_FUNC bl_do_new[BL_MAX+2] = {
	0,
	0,  /* BL_END */
	0,  /* BL_RET */
	0, /* BL_TEXT */
	0, /* BL_SEL2 */
	0,  /* BL_SEL2R */
	0,  /* BL_END2 */
	&SENARIO_BackLog::DoMsgPosNew, /* BL_MSGPOS */
	&SENARIO_BackLog::DoMsgSizeNew, /* BL_MSGSIZ */
	&SENARIO_BackLog::DoMojiSizeNew, /* BL_MOJSIZ */
	&SENARIO_BackLog::DoIsWakuNew,  /* BL_ISWAKU */
	0,  /* BL_SEL2S */
	0, /* BL_TEXT_WI */
	0, /* BL_SEL2S_WI */
	&SENARIO_BackLog::DoFlagInfNew, /* BL_FLAG_INF */
	&SENARIO_BackLog::DoGrpInfNew, /* BL_GRP_INF */
	&SENARIO_BackLog::DoMusInfNew, /* BL_MUS_INF */
	&SENARIO_BackLog::DoStackInfNew, /* BL_STACK_INF */
	&SENARIO_BackLog::DoTitleNew, /* BL_TITLE */
	0, /* BL_SAVEPT */
	0, /* BL_GRP_INF2 */
	0, /* BL_TEXT_WI */
	0, /* BL_NULL_WI */
	&SENARIO_BackLog::DoMsgPos2New, /* BL_MSGPOS2 */
	0,  /* BL_SEL1S */
	&SENARIO_BackLog::DoSysInfNew, /* BL_SYS_INF */
	0, /* BL_SYSORIG_INF */
	0, /* BL_END3 */
	0, /* BL_KOE */
	0
};
/* backlog の実行処理：逆方向 */
static BL_FUNC bl_do_old[BL_MAX+2] = {
	0,
	0,  /* BL_END */
	0,  /* BL_RET */
	0, /* BL_TEXT */
	0, /* BL_SEL2 */
	0,  /* BL_SEL2R */
	0,  /* BL_END2 */
	&SENARIO_BackLog::DoMsgPosOld, /* BL_MSGPOS */
	&SENARIO_BackLog::DoMsgSizeOld, /* BL_MSGSIZ */
	&SENARIO_BackLog::DoMojiSizeOld, /* BL_MOJSIZ */
	&SENARIO_BackLog::DoIsWakuOld,  /* BL_ISWAKU */
	0,  /* BL_SEL2S */
	0, /* BL_TEXT_WI */
	0, /* BL_SEL2S_WI */
	&SENARIO_BackLog::DoFlagInfOld, /* BL_FLAG_INF */
	&SENARIO_BackLog::DoGrpInfOld, /* BL_GRP_INF */
	&SENARIO_BackLog::DoMusInfOld, /* BL_MUS_INF */
	&SENARIO_BackLog::DoStackInfOld, /* BL_STACK_INF */
	&SENARIO_BackLog::DoTitleOld, /* BL_TITLE */
	0, /* BL_SAVEPT */
	0, /* BL_GRP_INF2 */
	0, /* BL_TITLE_WI */
	0, /* BL_NULL_WI */
	&SENARIO_BackLog::DoMsgPos2Old, /* BL_MSGPOS2 */
	0,  /* BL_SEL1S */
	&SENARIO_BackLog::DoSysInfOld, /* BL_SYS_INF */
	0, /* BL_SYSORIG_INF */
	0, /* BL_END3 */
	0, /* BL_KOE */
	0
};
/* backlog の内容の表示 */
static BL_DUMP_FUNC bl_dump[BL_MAX+2] = {
	0,
	&SENARIO_BackLog::DumpOnecmd,  /* BL_END */
	&SENARIO_BackLog::DumpOnecmd,  /* BL_RET */
	&SENARIO_BackLog::DumpText, /* BL_TEXT */
	&SENARIO_BackLog::DumpSel, /* BL_SEL2 */
	&SENARIO_BackLog::DumpSelR,  /* BL_SEL2R */
	&SENARIO_BackLog::DumpOnecmd,  /* BL_END2 */
	&SENARIO_BackLog::DumpObsolete, /* BL_MSGPOS */
	&SENARIO_BackLog::DumpObsolete, /* BL_MSGSIZ */
	&SENARIO_BackLog::DumpObsolete, /* BL_MOJSIZ */
	&SENARIO_BackLog::DumpObsolete,  /* BL_ISWAKU */
	&SENARIO_BackLog::DumpText,  /* BL_SEL2S */
	&SENARIO_BackLog::DumpObsolete, /* BL_TEXT_WI */
	&SENARIO_BackLog::DumpObsolete, /* BL_SEL2S_WI */
	&SENARIO_BackLog::DumpFlagInf, /* BL_FLAG_INF */
	&SENARIO_BackLog::DumpGrpInf, /* BL_GRP_INF */
	&SENARIO_BackLog::DumpMusicInf, /* BL_MUS_INF */
	&SENARIO_BackLog::DumpStackInf, /* BL_STACK_INF */
	&SENARIO_BackLog::DumpTitle, /* BL_TITLE */
	&SENARIO_BackLog::DumpText, /* BL_SAVEPT */
	&SENARIO_BackLog::DumpGrpInf2, /* BL_GRP_INF2 */
	&SENARIO_BackLog::DumpObsolete, /* BL_TITLE_WI */
	&SENARIO_BackLog::DumpObsolete, /* BL_NULL_WI */
	&SENARIO_BackLog::DumpObsolete, /* BL_MSGPOS2 */
	&SENARIO_BackLog::DumpText,  /* BL_SEL1S */
	&SENARIO_BackLog::DumpSysInf, /* BL_SYS_INF */
	&SENARIO_BackLog::DumpSysInf, /* BL_SYSORIG_INF */
	&SENARIO_BackLog::DumpOnecmd, /* BL_END3 */
	&SENARIO_BackLog::DumpText, /* BL_KOE */
	0
};
#endif /* defined __KANON_SENARIO_H__ */
/* text を含むか、など */
static int bl_istext[BL_MAX+2] = {
	0,
	0,  /* BL_END */
	0,  /* BL_RET */
	BL_TEXT, /* BL_TEXT */
	0, /* BL_SEL2 */
	0,  /* BL_SEL2R */
	0,  /* BL_END2 */
	0, /* BL_MSGPOS */
	0, /* BL_MSGSIZ */
	0, /* BL_MOJSIZ */
	0,  /* BL_ISWAKU */
	BL_SEL2S,  /* BL_SEL2S */
	0, /* BL_TEXT_WI */
	0, /* BL_SEL2S_WI */
	0, /* BL_FLAG_INF */
	0, /* BL_GRP_INF */
	0, /* BL_MUS_INF */
	0, /* BL_STACK_INF */
	BL_TITLE, /* BL_TITLE */
	0, /* BL_SAVEPT */
	0, /* BL_GRP_INF2 */
	0, /* BL_TITLE_WI */
	0,
	0,
	BL_SEL1S,  /* BL_SEL1S */
	0, /* BL_SYS_INF */
	0, /* BL_SYSORIG_INF */
	0, /* BL_END3 */
	0, /* BL_KOE */
	0
};

#endif
