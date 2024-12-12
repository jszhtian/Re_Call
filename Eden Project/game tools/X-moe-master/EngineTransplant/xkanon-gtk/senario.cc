/*  senario.cc
 *     シナリオファイルの再生を行う
 *     また、その他雑多なメソッド
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
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <ctype.h>
#include <vector>
#include "senario.h"
#include "image_di.h"
#include "anm.h"

//#define SUPRESS_JUMP
//#define SUPRESS_KEY
//#define SUPRESS_MUSIC
//#define SUPRESS_WAIT
//#define SUPRESS_GLOBAL_CALL

#define pp(X) do { printf("not supported command! : "); printf X; } while(0)

/*
#define DEBUG_READDATA
#define DEBUG_CalcVar
#define DEBUG_CalcStr
#define DEBUG_Select
#define DEBUG_Condition
*/
// #define DEBUG_0x0e
//#define DEBUG_TextWindow
// #define DEBUG_Wait
//#define DEBUG_Jump
//#define DEBUG_Other
//#define PrintLineNumber

// change unsigned -> signed...
inline char* strcpy(unsigned char* d, const unsigned char* s) { 
	return strcpy( (char*)d, (const char*)s );
}

inline int strlen(const unsigned char* s) {
	return strlen( (const char*)s);
}

int* CountCgmData(void);
int GetCgmInfo(int number, const char** ret_filename);

unsigned char SENARIO_DECODE::hankaku_to_zenkaku_table[0x60*2] = {
	// 　！”＃＄％＆’
	0x81,0x40,0x81,0x49,0x81,0x68,0x81,0x94,0x81,0x90,0x81,0x93,0x81,0x95,0x81,0x66,
	// （）＊＋，−．／
	0x81,0x69,0x81,0x6a,0x81,0x96,0x81,0x7b,0x81,0x43,0x81,0x7c,0x81,0x44,0x81,0x5e,
	// ０１２３４５６７
	0x82,0x4f,0x82,0x50,0x82,0x51,0x82,0x52,0x82,0x53,0x82,0x54,0x82,0x55,0x82,0x56,
	// ８９：；＜＝＞？
	0x82,0x57,0x82,0x58,0x81,0x46,0x81,0x47,0x81,0x83,0x81,0x81,0x81,0x84,0x81,0x48,
	// ＠ＡＢＣＤＥＦＧ
	0x81,0x97,0x82,0x60,0x82,0x62,0x82,0x62,0x82,0x63,0x82,0x64,0x82,0x65,0x82,0x66,
	// ＨＩＪＫＬＭＮＯ
	0x82,0x67,0x82,0x68,0x82,0x69,0x82,0x6a,0x82,0x6b,0x82,0x6c,0x82,0x6d,0x82,0x6e,
	// ＰＱＲＳＴＵＶＷ
	0x82,0x6f,0x82,0x70,0x82,0x71,0x82,0x72,0x82,0x73,0x82,0x74,0x82,0x75,0x82,0x76,
	// ＸＹＺ［￥］＾＿
	0x82,0x77,0x82,0x78,0x82,0x79,0x81,0x6d,0x81,0x8f,0x81,0x6e,0x81,0x4f,0x81,0x51,
	// ｀ａｂｃｄｅｆｇ
	0x81,0x4d,0x82,0x81,0x82,0x82,0x82,0x83,0x82,0x84,0x82,0x85,0x82,0x86,0x82,0x87,
	// ｈｉｊｋｌｍｎｏ
	0x82,0x88,0x82,0x89,0x82,0x8a,0x82,0x8b,0x82,0x8c,0x82,0x8d,0x82,0x8e,0x82,0x8f,
	// ｐｑｒｓｔｕｖｗ
	0x82,0x90,0x82,0x91,0x82,0x92,0x82,0x93,0x82,0x94,0x82,0x95,0x82,0x96,0x82,0x97,
	// ｘｙｚ｛｜｝〜　
	0x82,0x98,0x82,0x99,0x82,0x9a,0x81,0x6f,0x81,0x62,0x81,0x70,0x81,0x60,0x81,0x40
};

/* len >= 5 では負の数かもしれないので mask = -1 */
static const int readdata_mask[8] = {
	0, 0, 0xff, 0xffff, 0xffffff, -1, -1, -1};
int SENARIO_DECODE::ReadData(void) {
	// データを１つ読み込む
	int c = *data;
	int len = (c>>4) & 0x07;
	/* 最大 32bit なので、len <= 4 を仮定できる */
	int ret = ((read_little_endian_int((char*)data+1) & readdata_mask[len]) << 4) | (c&0x0f);
	data += len;
	if (c & 0x80) {
#ifdef DEBUG_READDATA
	printf("<index %d>",ret);
#endif
		return flags.Var(ret);
	} else {
#ifdef DEBUG_READDATA
	printf("<data %d>",ret);
#endif
		return ret;
	}
}
int SENARIO_DECODE::ReadDataStatic(unsigned char*& d, int& is_var) {
	// データを１つ読み込む
	int c = *d;
	is_var = c & 0x80;
	int len = (c>>4) & 0x07;
	int ret = ((read_little_endian_int((char*)d+1) & readdata_mask[len]) << 4) | (c&0x0f);
	d += len;
	return ret;
}
inline int SENARIO_DECODE::ReadDataStatic(unsigned char*& d, SENARIO_FLAGSDecode& flags) {
	int is_var;
	int ret = ReadDataStatic(d, is_var);
	if (is_var) return flags.Var(ret);
	else return ret;
}

char* SENARIO_DECODE::ReadString(char* buf) {
	if (*data == '@') { // 文字列変数
		data++; int index =  ReadData();
		strcpy(buf, flags.StrVar(index));
		return buf;
	} else {
		int len = strlen(data);
		strcpy(buf, (char*)data); data += len+1;
		return buf;
	}
}
char* SENARIO_DECODE::ReadStringStatic(unsigned char*& s_data,char* buf, SENARIO_FLAGSDecode& flags) {
	if (*s_data == '@') { // 文字列変数
		s_data++; int index =  ReadDataStatic(s_data, flags);
		strcpy(buf, flags.StrVar(index));
		return buf;
	} else {
		int len = strlen(s_data);
		strcpy(buf, (char*)s_data); s_data += len+1;
		return buf;
	}
}

void SENARIO_DECODE::ReadStringWithFormat(TextAttribute& text, int is_verbose) {
	int condition = 1;
	char buf_orig[1024]; char* buf = buf_orig;
	int buf_len = 1023; buf[0] = '\0'; buf[1023] = '\0';
	while(*data != 0) {
		int c = *data; c&=0xff;
		if (c == 0xff) {
			data++;
			strncpy(buf, (char*)data, buf_len);
			buf_len -= strlen(buf);
			data += strlen(buf)+1;
			buf += strlen(buf);
		} else if (c == 0xfe) {
			data++;
			strncpy(buf, (char*)data, buf_len);
			buf_len -= strlen(buf);
			data += strlen(buf)+1;
			buf += strlen(buf);
		} else if (c == 0xfd) {
			data++;
			int d = ReadData();
			snprintf(buf, buf_len, "%s", flags.StrVar(d));
			buf_len -= strlen(buf);
			buf += strlen(buf);
		} else if (c == 0x28) {
			condition = 0;
			if (is_verbose)
				condition = ! flags.DecodeSenario_Condition(*this);
			else
				flags.DecodeSkip_Condition(*this);
		} else if (c == 0x10) {
			data++; c = *data++;
			if (c == 3) {
				int d = ReadData();
				strncpy(buf, flags.StrVar(d), buf_len);
			} else if (c == 2) {
				int d = ReadData(); int k = ReadData();
				char form[10]; sprintf(form,"%%0%dd",k);
				snprintf(buf, buf_len, form, d);
			} else if (c == 1) {
				int d = ReadData();
				snprintf(buf, buf_len, "%d", d);
			}
			buf_len -= strlen(buf);
			buf += strlen(buf);
		} else {
			break;
		}
	}
	data++;
	text.SetText(buf_orig); text.SetCondition(condition);
	return;
}

char* SENARIO_DECODE::Han2Zen(const char* buf) {
	int len = strlen(buf);
	char* retbuf = new char[ len*2 + 10];
	char* destbuf = retbuf;
	while(*buf != 0) {
		unsigned int c = *buf++;
		if (c >= 0x20 && c < 0x7f) { // han -> zen
			c -= 0x20; c *= 2;
			*destbuf++ = hankaku_to_zenkaku_table[c];
			*destbuf++ = hankaku_to_zenkaku_table[c+1];
		} else if ( (c >= 0x8f &&  c <= 0x9f) ||
			(c >= 0xe0 && c <= 0xfc) ) { // zenkaku
			*destbuf++ = c;
			if (*buf != 0) *destbuf++ = *buf++;
		} else *destbuf++ = c;
	}
	*destbuf = 0;
	return retbuf;
}

/* for debug */
#ifdef p
#  undef p
#endif /* defined(p) */

#ifdef DEBUG_0x0e
#define p(X) printf X
#else
#define p(X)
#endif

int SENARIO_DECODE::Decode_Music(void) {
	int subcmd; int arg, arg2, arg3, arg4; // , arg5, arg6;
	char buf[1024]; char* str = 0;
	int cur_point = GetPoint() - 1;
	subcmd = NextCharwithIncl();
	p(("cmd 0x0e - %x: ",subcmd));

	switch(subcmd) {
		case 0x01:
			str = ReadString(buf);
			p(("<music> Play CD immediately : track = %s\n",str));
#ifndef SUPRESS_MUSIC
			local_system.SetCDROMCont();
			local_system.PlayCDROM(str);
#endif
			break;
		case 0x02: // with wait
			p(("<music> Play CD once and wait until end\n"));
		case 0x03:
			str = ReadString(buf);
			p(("<music> Play CD once :  track = %s\n",str));
			arg = atoi(buf);
#ifndef SUPRESS_MUSIC
			local_system.SetCDROMOnce();
			local_system.PlayCDROM(str);
#endif
			break;
/*@@@*/
		case 0x06: case 0x07:
			pp(("??? Stop and Fade CD?, cmd 0x06 or 0x07\n"));
		case 0x05:
			str = ReadString(buf); arg = ReadData();
			pp((" ??? Stop CD? cmd 0x0e-0x05 : str %s, arg %d\n",str,arg));
			local_system.StopCDROM();
			break;
		case 0x10:
			arg = ReadData();
			p(("<music> Fade out CD : wait = %d\n",arg));
#ifndef SUPRESS_MUSIC
			//local_system.StopCDROM();
			local_system.FadeCDROM(arg*256);
#endif
			break;
		case 0x11:
			p(("cmd 0x0e - 0x11: Force stop BGM????\n"));
#ifndef SUPRESS_MUSIC
			local_system.StopCDROM();
#endif
			break;
		case 0x15:
			arg = ReadData();
			p(("<music> Fade out CD and wait : wait = %d\n",arg));
#ifndef SUPRESS_MUSIC
			local_system.FadeCDROM(arg*256);
			local_system.WaitStopCDROM();
#endif
			break;
			
		case 0x16:
			p(("cmd 0x0e - 0x16: wait stop BGM\n"));
			local_system.WaitStopCDROM();
			break;
#if 1 /* @@@ */
		case 0x20:
		case 0x21:
		case 0x22:
		case 0x28:
			/* 0x20 は声終了までシステム停止(adieu) */
			/* 0x21 と 0x22 はシナリオ継続。たぶん 0x01 で声終了 */
			if (subcmd == 0x28) {
				str = ReadString(buf);
			}
			arg = ReadData();
			
			if (subcmd == 0x22) arg2 = ReadData();
			if (subcmd == 0x21) BackLog().AddKoe(cur_point, seen_no);
			if (local_system.TextFastMode() == AyuSys::TF_SKIP) break; /* 早送り */
			/* subcmd 0x21,22 では ctrl skip / テキスト早送りでも声を飛ばす */
			if (subcmd == 0x21 || subcmd == 0x22 || subcmd == 0x28) {
				if (local_system.TextFastMode() == AyuSys::TF_FAST) break; /* 早送り */
			}
			local_system.StopKoe();
			if (subcmd == 0x28) local_system.PlayKoe(str);
			else { sprintf(buf, "%d", arg); local_system.PlayKoe(buf);}
			if (subcmd == 0x20) {
#ifndef SUPRESS_MUSIC
				void* timer = local_system.setTimerBase();
				const int wait_time = 30000; // 曲の終了まで、最大30秒待つ
				local_system.SetMouseMode(0);
				local_system.ClearMouseInfo();
				while( (!local_system.IsStopKoe()) && local_system.getTime(timer) < wait_time && (!local_system.IsIntterupted()))  {
					int x, y, flag;
					local_system.GetMouseInfo(x, y, flag);
					if (flag == 0 || flag == 2 || flag == 4) break;
					local_system.WaitNextEvent();
				}
				local_system.freeTimerBase(timer);
				local_system.ClearMouseInfo();
#endif
			}
			if (subcmd == 0x28)
				p(("cmd 0x0e - 0x20-0x28: ???? voice? arg(time/track) = %d(%s)\n",arg,str));
			else
				p(("cmd 0x0e - 0x20-0x22: ???? voice? arg(time/track) = %d\n",arg));
			break;
#endif
		case 0x23:
			pp(("cmd 0x0e - 0x23 : not supported yet.\n"));
			break;
		case 0x30:
			str = ReadString(buf);
			p(("<music> Play wave file once : file = %s\n",str));
#ifndef SUPRESS_MUSIC
			local_system.SetEffecOnce();
			local_system.PlayWave(str);
#endif
			break;
		case 0x31:
			str = ReadString(buf); arg = ReadData();
			p(("<music> Play wave file once??? : file = %s, arg = %d\n",str,arg));
#ifndef SUPRESS_MUSIC
			local_system.SetEffecOnce();
			local_system.PlayWave(str);
#endif
			break;
		case 0x32:
			str = ReadString(buf);
#ifndef SUPRESS_MUSIC
			local_system.SetEffecCont();
			local_system.PlayWave(str);
#endif
			p(("<music> Play wave file : file = %s\n",str));
			break;
		case 0x33:
			str = ReadString(buf); arg = ReadData();
#ifndef SUPRESS_MUSIC
			local_system.SetEffecCont();
			local_system.PlayWave(str);
#endif
			p(("<music> Play wave file??? : file = %s, arg = %d\n",str,arg));
			break;
		case 0x34:
			str = ReadString(buf);
			pp(("??? cmd 0x0e - 0x34 : str = %s\n",str));
#ifndef SUPRESS_MUSIC
			local_system.SetEffecOnce();
			local_system.PlayWave(str);
			local_system.WaitStopWave();
#endif
			break;
		case 0x35:
			str = ReadString(buf); arg = ReadData();
			pp(("??? cmd 0x0e - 0x35 : str = %s, arg = %d\n",str,arg));
#ifndef SUPRESS_MUSIC
			local_system.SetEffecOnce();
			local_system.PlayWave(str);
			local_system.WaitStopWave();
#endif
			break;
		case 0x36:
			p(("Stop wave file\n"));
#ifndef SUPRESS_MUSIC
			local_system.StopWave();
#endif
			break;
		case 0x37:
			arg = ReadData();
			p(("Stop wav file? arg %d (arg is currently not used)\n",arg));
#ifndef SUPRESS_MUSIC
			local_system.StopWave();
#endif
			break;
		case 0x38:
			pp(("??? (Stop wav file and clear music buffer?\n"));
#ifndef SUPRESS_MUSIC
			local_system.StopWave();
#endif
			break;
/* dsound_format
** 0 : 8bit, 11025, 2
** 1 : 16bit, 11025, 2
** 2 : 8bit, 22050, 2
** 3 : 8bit, 44100, 2
** 4 : 16bit, 44100, 2
** 5 : 16bit, 22050, 2
*/
		case 0x39:
			arg = ReadData();
			pp(("??? wave? ; subcmd 0x39, change sound type(DSOUND_FORM) to %d\n",arg));
#ifndef SUPRESS_MUSIC
			local_system.StopWave();
#endif
			break;
		case 0x3a:
			pp(("??? wave? ; subcmd 0x3a, change sound type(DSOUND_FORM) to original(gameexe.ini) form\n"));
			break;
/* @@@ */
		case 0x44:
			arg = ReadData();
			pp(("??? play SE? 0x0e - 44 : %d\n",arg));
			break;
		case 0x50:
		case 0x51:
		case 0x52:
		case 0x53:
		case 0x54:
		case 0x55: {
			char* file = ReadString(buf);
			if (subcmd == 0x54 || subcmd == 0x55) ReadString(buf+512);
			int x1 = ReadData(); int y1 = ReadData(); int x2=ReadData(); int y2 = ReadData();
			p(("movie play? %s , %d,%d,%d,%d\n",str,x1,y1,x2,y2));
			int loop_count = 1;
			if (subcmd == 0x51) loop_count = 10000;
			local_system.PlayMovie(file, x1, y1, x2, y2, loop_count);
			if (subcmd >= 0x52) { /* 再生終了を待つ */
				int is_click = 0;
				if (subcmd == 0x53 || subcmd == 0x55) is_click = 1;
				local_system.WaitStopMovie(is_click);
			}
			break;}
/*
[ebp-0c]=0; [ebp-10]=0;
if(bl>=54) {
	str = ReadString();
	[ebp-0c]=2;
	if (str[0]!=0) [ebp-10]=str;
}
[esi+0]-[esi+0c]=ReadData()x4;
if (bl>=56) {
	[esi+10]=ReadData();
	edi=[ebp-0c] | 4;
} else edi = [ebp-0c];
if (bl==53 || bl==55 || bl==57) {
} else edi |= 1;
*/
		case 0x60:
			p(("Stop movie?\n"));
			local_system.StopMovie();
			break;
		case 0x61:
			p(("Pause movie?\n"));
			local_system.PauseMovie();
			break;
		case 0x62:
			p(("Resume paused movie?\n"));
			local_system.ResumeMovie();
			break;
		case 0x70: /* mpg 系 */
		case 0x71:
		case 0x72:
		case 0x73:
			str = ReadString(buf);
			arg = ReadData(); arg2 = ReadData(); arg3=ReadData(); arg4 = ReadData();
			pp(("??? 0x0e - 0x%02x ; mpg play??? ; %s , %d,%d,%d,%d\n",subcmd,str,arg,arg2,arg3,arg4));
			break;
		case 0x78:
			pp(("??? 0x0e - 0x78 ; mpg stop???\n"));
		default: 
			return -1;
	}
	return 0;
}

void SENARIO_DECODE::DecodeSkip_Music(void) {
	int subcmd;
	char buf[1024];
	subcmd = NextCharwithIncl();
#if 0
	printf("DecodeSkip_Music: 0x0e - %x\n",subcmd);
#endif
	switch(subcmd) {
		case 0x01: ReadString(buf); break;
		case 0x02: case 0x03: ReadString(buf); break;
		case 0x05: case 0x06: case 0x07: ReadString(buf); ReadData(); break;
		case 0x10: ReadData(); break;
		case 0x12: case 0x11: break;
		case 0x16: break;
#if 1 /* @@@ */
		case 0x20:
		case 0x21: /* if (local_system.Version()==0) ReadString(buf); else */ ReadData(); break;
		case 0x22: /* if (local_system.Version()==0) ReadString(buf); else */ ReadData(); ReadData(); break;
		case 0x28: ReadString(buf); ReadData(); break;
#endif
		case 0x23: break;
		case 0x30: ReadString(buf); break;
#if 1 /* @@@ */
		case 0x31: case 0x33: case 0x35: ReadString(buf); ReadData(); break;
#endif
		case 0x32: case 0x34: ReadString(buf); break;
		case 0x36: break;
		case 0x37: ReadData(); break;
		case 0x38: case 0x3a: break;
		case 0x39: ReadData(); break;
		case 0x44: ReadData(); break;
		case 0x54:
		case 0x55:
			ReadString(buf);
		case 0x50: case 0x51: case 0x52: case 0x53:
		case 0x70: case 0x71: case 0x72: case 0x73:
			ReadString(buf);
			ReadData(); ReadData();
			ReadData(); ReadData();
			break;
		case 0x60: case 0x61: case 0x62: case 0x78:
			break;
		default: 
			return;
	}
	return;
}

/* for debug */
#ifdef p
#  undef p
#endif /* defined(p) */

#ifdef DEBUG_TextWindow
#define p(X) printf X
#else
#define p(X)
#endif

int SENARIO_DECODE::Decode_TextWindow(void) {
	char buf[1024]; char buf2[1024];char* str;
	int cur_point = GetPoint() - 1;
	int arg, arg2, arg3;
	local_system.SetClickEvent(AyuSys::END_TEXTFAST);
	switch(cmd) {
		case 0x01:
		case 0x03:
		case 0x04:
			if (cmd == 1) {
				p(("cmd 0x01 : wait key , clear text?\n"));
			} else if (cmd == 3) {
				p(("cmd 0x03 : wait key , not clear text\n"));
			} else if (cmd == 4) {
				if (NextChar() == 1 || NextChar() == 2 || NextChar() == 5) {
					p(("cmd 0x04 - %d. Delete text window???\n",NextChar()));
					NextCharwithIncl();
					local_system.DeleteReturnCursor();
					local_system.DeleteText();
					BackLog().AddEnd2();
					break;
				} else if (NextChar() == 4) {
					NextCharwithIncl();
					/* 下に行く */
				} else return -1; /* invalid command */
			}
#ifndef SUPRESS_KEY
			if (local_system.TextFastMode() == AyuSys::TF_SKIP) {
				/* テキスト読み飛ばし中 */
			//	if (cmd == 3) BackLog().AddEnd3();
			//	else if (cmd == 4) BackLog().AddEnd2();
			//	else /* cmd == 1 */ BackLog().AddEnd();
			//	break;
			}
			{ // テキスト描画終了まで待つ
				void* timer = 0;
				IdleReadGrp idle_ev(&senario);
				int subcmd = cmd; // ReadGrp() で、cmd が変化してしまうので保存
				int is_writing = 1;
				local_system.SetMouseMode(0);
				local_system.ClearMouseInfo();
				while(1) {
					int x, y, click;
					int l,r,u,d,esc;
					local_system.GetMouseInfoWithClear(x,y,click);
					if (click == 3) { /* ホイールの上 : バックログに入る */
						local_system.SetBacklog(2); break;
					} else if (click == 4) { /* ホイールの下：全画面モードの画面切り替え時以外は左クリックと同じ */
						if (subcmd != 4 || local_system.DrawTextEnd(0) == 0) click = 0;
						else click = -1;
					} else if (click == 1) {
						click = -1; /* 右クリック：なかったことにする */
					}
				clicked:
					if (click != -1 && local_system.DrawTextEnd(0) == 0) {
						/* テキスト描画中にクリック : 全テキストを描画 */
						local_system.DrawTextEnd(1);
						if (click != 2) click = -1;
						local_system.ClearMouseInfo();
					}
					if (click != -1) break; /* マウスが押されたら終了 */

					if (is_writing && local_system.DrawTextEnd(0) == 1 && local_system.IsStopKoe()) {
						/* テキスト描画終了：リターンカーソル表示 */
						if (subcmd != 4) local_system.DrawReturnCursor(0);
						else local_system.DrawReturnCursor(1);
						local_system.SetIdleEvent(&idle_ev);
						timer = local_system.setTimerBase();
						is_writing = 0;
					}

					/* システム側の都合(メニューなどの操作)で終了 */
					if (local_system.IsIntterupted()) break;
					if (local_system.TextFastMode() == AyuSys::TF_SKIP) break;
					if (local_system.TextFastMode() == AyuSys::TF_FAST) break;
					if (local_system.TextFastMode() == AyuSys::TF_AUTO
						&& subcmd != 4
						&& timer &&  local_system.getTime(timer) > 200) break; // auto mode では 0.2sec 待つ
					if (local_system.TextFastMode() == AyuSys::TF_AUTO
						&& subcmd == 4
						&& timer &&  local_system.getTime(timer) > 500) break; // auto mode では 0.2sec 待つ
					/* キー入力待ち */
					local_system.GetKeyCursorInfo(l,r,u,d,esc);
					if (l) {local_system.SetBacklog(11); break;}
					else if (u) {local_system.SetBacklog(2); break;}
					else if (d) {click = 0; goto clicked;}
					else if (r) {local_system.StartTextSkipMode(10); break;}
				
					local_system.CallIdleEvent();
					local_system.WaitNextEvent();
				}
				local_system.DrawTextEnd(1);
				local_system.StopKoe();
				if (timer) local_system.freeTimerBase(timer);
				if (is_writing == 0) {
					local_system.DeleteReturnCursor();
					local_system.DeleteIdleEvent(&idle_ev);
				}
				if (subcmd == 1) {
					if (local_system.config->GetParaInt("#NVL_SYSTEM")) {
						local_system.DrawText("\n");
					} else {
						local_system.DeleteText();
					}
					BackLog().AddEnd();
				} else if (subcmd == 3) {
					BackLog().AddEnd3();
				} else if (subcmd == 4) {
					local_system.DeleteText();
					BackLog().AddEnd2();
				}
			}
#endif
			break;

		case 0x05: // text modification?
			if (NextChar() == 0x11) {
				NextCharwithIncl();
				pp(("cmd 0x05 - 0x11 :  normal size text?\n"));
			} else if (NextChar() == 0x12) {
				NextCharwithIncl();
				pp(("cmd 0x05 - 0x12 :  wide size text?\n"));
			} else {
				pp(("cmd 0x05 - 0x%02x : text size change? not supported.\n",NextChar()));
				NextCharwithIncl();
				break;
			}
			break;
		
		case 0x10: // print variables to screen
			arg = NextCharwithIncl();
			buf[0] = '\0';
			if (arg == 1) {
				arg2 = ReadData();
				sprintf(buf, "%d", flags.GetVar(arg2));
				p(("cmd 0x10 - 1 : print variable[%d] to screen\n",arg2));
			} else if (arg == 2) {
				arg2 = ReadData();
				arg3 = ReadData();
				sprintf(buf2, "%%0%dd", arg3);
				sprintf(buf, buf2, flags.GetVar(arg2));
				p(("cmd 0x10 - 2 : print variable[%d] to screen , digit %d\n",arg2, arg3));
			} else if (arg == 3) {
				arg2 = ReadData();
				sprintf(buf, "%s", flags.StrVar(arg2));
				p(("cmd 0x10 - 3 : print string variable[%d] to screen\n",arg2));
			} else {
				pp(("cmd 0x10 - %d : unknown command!! (print variables to screen.)\n",arg));
/* データをダンプしておく */
DumpData();
			}
#ifndef SUPRESS_KEY
			local_system.DrawText(buf);
			local_system.DrawTextEnd(1); //  テキストを全部書く
#endif
			break;
		case 0xf0: /* skip text */ {
				if (local_system.Version() >= 3) ReadInt();
				ReadString(buf);
				if (NextChar() == 1) NextCharwithIncl();
				break;
			}
		case 0x02:
		case 0xfe:
		case 0xff:
			{
			local_system.InclTextSkipCount();
			/* テキスト読み込み */
			char cur_text[1024]; cur_text[0] = '\0';
			while(1) {
				if (cmd == 2) {
					int subcmd = NextChar();
					if (subcmd == 1 || subcmd == 3) {
						NextCharwithIncl();
						p(("cmd 0x02 - 1/3 : text write position = first position(return text)\n"));
						strcat(cur_text, "\n");
					} else if (subcmd == 2) {
						NextCharwithIncl();
						p(("cmd 0x02 - 2 : text write position = first_position. (akz.)\n"));
/* @@@ */
strcat(cur_text, "\r\n");
						break;
					} else {
						break;
					}
				} else if (cmd == 0xff || cmd == 0xfe) {
					if (local_system.Version() >= 3) {
						arg = ReadInt();
						if (senario.IsReadFlag(arg)) {
							local_system.SetKidoku();
						} else {
							local_system.ResetKidoku();
						}
						senario.SetReadFlag(arg);
					} else arg = 0;
					str = ReadString(buf);
					p(("cmd 0x%02x : %d, <message>\n",cmd,arg));
					strcat(cur_text, str);
				}
				int next_cmd = int(NextChar()) & 0xff;
				if (next_cmd != 2 && next_cmd != 0xfe && next_cmd != 0xff) break;
				cmd = next_cmd; NextCharwithIncl();
			}
			if (cur_text[0] == '\0') break;
			BackLog().AddText(cur_point, seen_no);
			if (local_system.IsDumpMode()) {
				kconv( (unsigned char*)cur_text, (unsigned char*)buf2);
				char* s = buf2;
				while(strchr(s,'\n')) {
					*(strchr(s,'\n')) = '\0';
					p(("text: %s\n",s));
					s += strlen(s)+1;
				}
				p(("text: %s\n",s));
			}
#ifndef SUPRESS_KEY
			// if (local_system.TextFastMode() == AyuSys::TF_SKIP) break;
			str = (char*)macro.DecodeMacro((unsigned char*)cur_text, (unsigned char*)buf2);
			local_system.DrawText(str);
#endif
			break; }
	}
	return 0;
}

void SENARIO_DECODE::DecodeSkip_TextWindow(void) {
	char buf[1024]; int arg;
	switch(cmd) {
		case 0x01: case 0x03: break;
		case 0x02: NextCharwithIncl(); break;
		case 0x04: NextCharwithIncl(); break;
		case 0x05: NextCharwithIncl(); break;
		case 0x10: arg=NextCharwithIncl(); if (arg==2) ReadData(); ReadData(); break;
		case 0xf0: if (local_system.Version() >= 3) ReadInt();
			ReadString(buf);
			if (NextChar() == 1) NextCharwithIncl();
			break;
		case 0xfe:
		case 0xff: if (local_system.Version() >= 3) {
				int num = ReadInt();
				senario.SetMaxReadFlag(num);
			}
			ReadString(buf); break;
	}
	return;
}

/* TextWindow 系コマンドを解析してテキストを得る */
void SENARIO_DECODE::GetText(char* ret_str, unsigned int str_len, int* koe_ptr, GlobalStackItem& point) {
	char buf[1024]; char buf2[1024];
	unsigned char* senario_data;

	*ret_str = '\0';
	if (koe_ptr) *koe_ptr = -1;
	/* まず、シナリオのデータを得る */
	if (point.GetSeen() == -1 || point.GetSeen() == GetSeen()) {
		senario_data = GetData(point.GetLocal());
	} else {
		if (gettext_cache_number != point.GetSeen()) {
			if (gettext_cache_data) delete[] gettext_cache_data;
			gettext_cache_number = point.GetSeen();
			gettext_cache_data = Senario().MakeSenarioData(gettext_cache_number, 0);
		}
		senario_data = gettext_cache_data + point.GetLocal();
	}
	/* コマンドを読み込む */
	/* 可能なコマンドは 0xff, 0xfe と音声の 0x0e - 21 */
	cmd = *senario_data++;
	if (cmd == 0x0e && *senario_data == 0x21) {
		senario_data++;
		int koe = ReadDataStatic(senario_data, flags);
		if (koe_ptr) *koe_ptr = koe;
		cmd = *senario_data++;
	}
	if (cmd == 0xff || cmd == 0xfe) {
		if (local_system.Version() >= 3) senario_data += 4; /* ReadInt() */
		char* str = ReadStringStatic(senario_data, buf, flags);
		str = (char*)macro.DecodeMacro((unsigned char*)str, (unsigned char*)buf2);
		if (strlen(str) < str_len) strcpy(ret_str, str);
		else { strncpy(ret_str, str, str_len-1); ret_str[str_len-1]='\0';}
	}
	return;
}


/* for debug */
#ifdef p
#  undef p
#endif /* defined(p) */

#ifdef DEBUG_Wait
#define p(X) printf X
#else
#define p(X)
#endif

int SENARIO_DECODE::Decode_Wait(void) {
	int subcmd = NextCharwithIncl();
	if (subcmd < 0x10) {
		// wait operation
		int tm; int index;
		switch(subcmd) {
		case 0x01:
			tm = ReadData();
			local_system.freeTimerBase(normal_timer);
			p(("cmd 0x19 - 0x01 : wait %d us.\n",tm));
			normal_timer = local_system.setTimerBase();
#ifndef SUPRESS_WAIT
			while(local_system.getTime(normal_timer) < tm) {
				local_system.WaitNextEvent();
			}
#endif
			break;
		case 0x02:
			tm = ReadData(); index = ReadData();
			flags.SetVar(index, 0);
			p(("cmd 0x19 - 0x02 : wait %d ms , with mouse break, fbuf[%d] <- stop state\n",tm,index));
			local_system.freeTimerBase(normal_timer);
			normal_timer = local_system.setTimerBase();
#ifndef SUPRESS_WAIT
			local_system.SetMouseMode(0);
			local_system.ClearMouseInfo();
			while (local_system.getTime(normal_timer) < tm) {
				int x,y,flag;
				local_system.GetMouseInfo(x,y,flag);
				if (flag == 0 || flag == 2 || flag == 4) { // マウスが押されたら終了
					flags.SetVar(index, 1);
					break;
				}
				local_system.WaitNextEvent();
			}
			local_system.ClearMouseInfo();
#endif
			break;
		case 0x03:
			p(("cmd 0x19 - 0x03 : set base time\n"));
			senario.InitTimer();
			break;
		case 0x04:
			tm = ReadData();
			p(("cmd 0x19 - 0x04 : wait %d us from base time\n",tm));
			p(("                  current time is %d\n",senario.GetTimer() ));
#ifndef SUPRESS_WAIT
			while(senario.GetTimer() < tm-200) {
				local_system.WaitNextEvent();
			}
			while(senario.GetTimer() < tm) local_system.WaitNextEvent();
#endif
			break;
		case 0x05:
			tm = ReadData(); index = ReadData();
			flags.SetVar(index, 0);
			p(("cmd 0x19 - 0x05 : wait %d ms from base time,with mouse break, fbuf[%d] <- stop state\n",tm,index));
#ifndef SUPRESS_WAIT
			local_system.SetMouseMode(0);
			while (senario.GetTimer() < tm) {
				int x,y,flag;
				local_system.GetMouseInfo(x,y,flag);
				if (flag == 0 || flag == 2 || flag == 4) { // マウスが押されたら終了
					flags.SetVar(index, 1);
					break;
				}
				local_system.WaitNextEvent();
			}
			local_system.ClearMouseInfo();
#endif
			break;
		case 0x06:
			index = ReadData();
			tm = senario.GetTimer();
			flags.SetVar(index, tm);
			p(("cmd 0x19 - 0x06 : fbuf[%d] <- time count from base time = %d\n",index, tm));
			break;
		default:
			pp(("cmd 0x19 - %d : Error. \n",subcmd));
			return -1;
		}
	} else if (subcmd < 0x20) {
		switch(subcmd) {
		case 0x10:
			p(("cmd 0x19 - 0x10 : IniFFMode = 1.\n"));
#ifndef SUPRESS_WAIT
			local_system.SetKidoku();
#endif
			break;
		case 0x11:
			p(("cmd 0x19 - 0x11 : IniFFMode = 0.\n"));
#ifndef SUPRESS_WAIT
			local_system.ResetKidoku();
#endif
			break;
		case 0x12:
			p(("cmd 0x19 - 0x13 : ?? (wait?)Mode = 1.\n"));
			break;
		case 0x13:
			p(("cmd 0x19 - 0x13 : ?? (wait?)Mode = 0.\n"));
#ifndef SUPRESS_WAIT
			if (local_system.NowInKidoku()) {
				local_system.ResetKidoku();
				local_system.SetKidoku();
			}
#endif
			break;
		default:
			pp(("cmd 0x19 - %d : Error. \n",subcmd));
			return -1;
		}
	} else {
		pp(("cmd 0x19 - %d : Error. \n",subcmd));
		return -1;
	}
	return 0;
}


void SENARIO_DECODE::DecodeSkip_Wait(void) {
	int subcmd = NextCharwithIncl();
	if (subcmd < 0x10) {
		// wait operation
		switch(subcmd) {
		case 0x01: ReadData(); break;
		case 0x02: case 0x05: ReadData(); ReadData(); break;
		case 0x03: break;
		case 0x04: ReadData(); break;
		case 0x06: ReadData(); break;
		default: break;
		}
	} else if (subcmd < 0x20) {
	}
	return;
}


/* for debug */
#ifdef p
#  undef p
#endif /* defined(p) */

#ifdef DEBUG_Jump
#define p(X) printf X
#else
#define p(X)
#endif

GlobalStackItem SENARIO_DECODE::Decode_Jump(void)
{
	GlobalStackItem ret; int i;
	int local_p; int global_s; int subcmd;
	switch(cmd) {
		case 0x15: // 条件ジャンプ
			p(("cmd 0x15 : conditional jump : "));
			if ( flags.DecodeSenario_Condition(*this)) {
				local_p = ReadInt();
				ret.SetLocal(local_p); // jump
				p((" : true -> %d\n",local_p));
				// data = base_data + arg;
			} else {
				local_p = ReadInt();
				p((" : true -> %d\n",local_p));
				ret.SetLocal(data-basedata); // not jump
			}
			break;
		case 0x16: // global call
			subcmd = NextCharwithIncl();
			global_s = ReadData();
			if (subcmd == 1) {
				p(("cmd 0x16 - 1 : global jump to %d\n", global_s));
#ifndef SUPRESS_GLOBAL_CALL
				ret.SetGlobal(global_s, 0);
#else
				ret.SetLocal(data-basedata); // not jump
#endif
			} else {
				p(("cmd 0x16 - %d : global call to %d\n",subcmd,global_s));
#ifndef SUPRESS_GLOBAL_CALL
				local_system.CallStack().PushStack().SetGlobal(seen_no, data-basedata);
				ret.SetGlobal(global_s, 0);
#else
				ret.SetLocal(data-basedata); // not jump
#endif
			}
			break;
		case 0x1c: // 無条件ジャンプ
			local_p = ReadInt();
			p(("cmd 0x1c : jump : %d\n",local_p));
			ret.SetLocal(local_p);
			// data = base_data + arg;
			break;
		case 0x1d: { // on XX gosub
			int* jumps;
			int n = NextCharwithIncl(); int index = ReadData();
			p(("cmd 0x1d : call array : num = %d, var = %d\n",n,index));
			jumps = new int[n];
			for (i=0; i<n; i++) {
				jumps[i] = ReadInt(); 
				p(("         : call %d = %x\n",i+1,jumps[i])); 
			}
			local_p = data - basedata;
			if (flags.GetVar(index) > 0 && flags.GetVar(index) <= n) {
				local_p = jumps[flags.GetVar(index)-1];
				local_system.CallStack().PushStack().SetLocal(data-basedata);
			}
			delete[] jumps;
			ret.SetLocal(local_p);
			break;}
		case 0x1e: { // on XX goto
			int* jumps;
			int n = NextCharwithIncl(); int index = ReadData();
			p(("cmd 0x1e : jump array : num = %d, var = %d\n",n,index));
			jumps = new int[n];
			for (i=0; i<n; i++) {
				jumps[i] = ReadInt(); 
				p(("         : jump %d = %x\n",i+1,jumps[i])); 
			}
			local_p = data - basedata;
			if (flags.GetVar(index) > 0 && flags.GetVar(index) <= n) {
				local_p = jumps[flags.GetVar(index)-1];
			}
			delete[] jumps;
			ret.SetLocal(local_p);
			break;}
		case 0x1b:
			local_p = ReadInt();
			p(("cmd 0x1b : gosub to %d, stack=%d\n",local_p,data-basedata));
			local_system.CallStack().PushStack().SetLocal(data-basedata);
			ret.SetLocal(local_p);
			break;
		case 0x20:
			if (NextChar() == 2) {
				NextCharwithIncl();
				ret = local_system.CallStack().PopStack();
				p(("cmd 0x20 - 2 : global return. : seen = %d, local = %d\n",ret.GetSeen(), ret.GetLocal()));
				if (ret.GetSeen() == -1) ret.SetGlobal(0,0);
			} else if (NextChar() == 1) {
				NextCharwithIncl();
				ret = local_system.CallStack().PopStack();
				p(("cmd 0x20 - 1 : local return. : return = %d\n",ret.GetLocal()));
			} else if (NextChar() == 6) {
				NextCharwithIncl();
				local_system.CallStack().InitStack();
				ret.SetLocal(data-basedata); // not jump
				p(("cmd 0x20 - 6 : initialize stack.\n"));
			} else if (NextChar() == 3) {
				NextCharwithIncl();
				local_system.CallStack().PopStack();
				ret.SetLocal(data-basedata); // not jump
				p(("cmd 0x20 - 3 : pop stack without jump\n"));
			}
			break;
	}
	return ret;
}

void SENARIO_DECODE::DecodeSkip_Jump(void)
{
	switch(cmd) {
		case 0x15: // 条件ジャンプ
			flags.DecodeSkip_Condition(*this);
			ReadInt();
			break;
		case 0x16: // global call
			NextCharwithIncl();
			ReadData();
			break;
		case 0x1c: // 無条件ジャンプ
			ReadInt();
			break;
		case 0x1d: case 0x1e: {
			int n = NextCharwithIncl(); ReadData();
			int i; for (i=0; i<n; i++) {
				ReadInt(); 
			}
			break;}
		case 0x1b:
			ReadInt();
			break;
		case 0x20:
			NextCharwithIncl();
			break;
	}
	return;
}

/* for debug */
#ifdef p
#  undef p
#endif /* defined(p) */

#ifdef DEBUG_Other
#define p(X) printf X
#else
#define p(X)
#endif

static char cmd_sortlist[256] = { // コマンドの種類
	// 1: 変数の操作 2: 無条件ジャンプ、あるいは条件ジャンプ
	// 9: それ以外
//	0 1 2 3  4 5 6 7  8 9 a b  c d e f
	9,9,9,9, 9,9,9,9, 9,9,9,9, 9,9,9,9, // +00
	9,9,9,9, 9,2,9,9, 9,9,9,9, 2,9,9,9, // +10
	9,9,9,9, 9,9,9,9, 9,9,9,9, 9,9,9,9, // +20
	9,9,9,9, 9,9,9,1, 1,1,1,1, 1,1,1,1, // +30
	1,1,1,1, 1,1,1,1, 1,1,1,1, 1,1,1,1, // +40
	1,1,1,1, 1,1,1,1, 9,9,9,9, 9,9,9,9, // +50
	9,9,9,9, 9,9,9,9, 9,9,9,9, 9,9,9,9, // +60
	9,9,9,9, 9,9,9,9, 9,9,9,9, 9,9,9,9, // +70
	9,9,9,9, 9,9,9,9, 9,9,9,9, 9,9,9,9, // +80
	9,9,9,9, 9,9,9,9, 9,9,9,9, 9,9,9,9, // +90
	9,9,9,9, 9,9,9,9, 9,9,9,9, 9,9,9,9, // +a0
	9,9,9,9, 9,9,9,9, 9,9,9,9, 9,9,9,9, // +b0
	9,9,9,9, 9,9,9,9, 9,9,9,9, 9,9,9,9, // +c0
	9,9,9,9, 9,9,9,9, 9,9,9,9, 9,9,9,9, // +d0
	9,9,9,9, 9,9,9,9, 9,9,9,9, 9,9,9,9, // +e0
	9,9,9,9, 9,9,9,9, 9,9,9,9, 9,9,9,9, // +f0
};

/* 一般的なシナリオデコード */
int SENARIO_DECODE::Decode(void)
{
	int arg, arg2, arg3,arg4; int i;
	char buf[1024]; char* str; // int cond;
#ifdef PrintLineNumber
	printf("pt %d / %d : ",data-basedata, data_len);
#endif
	cmd = *data++;
	if (data-basedata > data_len) return -1;
#ifndef PrintLineNumber
// 変数の操作と、ジャンプ関係だけは SENARIO::Play に戻らないで処理する
	char cmd_sort = cmd_sortlist[cmd];
	while (cmd_sort <= 2) {
		if (cmd_sort == 1) {
			flags.DecodeSenario_CalcVar(*this);
		} else { // jump
			if (cmd == 0x1c) { // 無条件ジャンプ
#ifndef SUPRESS_JUMP
				data = basedata + ReadInt();
#else
				ReadInt();
#endif
			} else { // 条件ジャンプ
				if (flags.DecodeSenario_Condition(*this)) {
#ifndef SUPRESS_JUMP
					data = basedata + ReadInt();
#else
					ReadInt();
#endif
				} else {
					ReadInt();
				}
			}
					
		}
		cmd = *data++; cmd_sort = cmd_sortlist[cmd];
	}
#endif

	if (cmd >= 0x37 && cmd <= 0x59) { // 変数の操作
		flags.DecodeSenario_Calc(*this);
		return 0;
	}
	switch(cmd) {
		case 0x00:
			p(("cmd 0x00 : end senario file.\n"));
			printf("cmd 0x00 : end senario file.\n");
			if (data-basedata != data_len) {
			/* 無効な終端を無視する */
#if 0
				/* reallocated text??? */
				char buf[100];
				int number = ReadInt();
				sprintf(buf, "<%d>",number);
				pp(("cmd 0x00, ff :rellocate text??? ; number %d\n",number));
#ifndef SUPRESS_KEY
				local_system.DrawText(buf);
#endif
				break;
#else			/* 終端をきちんと検知する */
				printf("Error: cmd 0x00, seen %d, point %d: invalid data len?\n",seen_no,data-basedata);
#endif
				goto error;
			}
			{ GlobalStackItem item; item.SetGlobal(-1,-1);
			senario.SetPoint(item);}
			return -1; // end senario
		case 0x01:
		case 0x02:
		case 0x03:
		case 0x04:
		case 0x05:
		case 0x10:
		case 0xfe:
		case 0xff:
			if (Decode_TextWindow() == -1) {
				pp(("TextWindow : error\n"));
				goto error;
			}
			break;
#if 1 /* @@@ */
#if 0
		case 0x05: // unsupported
			arg = NextCharwithIncl();
			pp(("cmd 0x05 : shake screen? arg = %d\n", arg));
			break;
#endif
		case 0x75: // unsupported
			/* わっふる for Mac より：
			case 0x01: Get BGM Volume
			case 0x02: Get WAV Volume
			case 0x03: Get KOE Volume
			case 0x04: Get SE Volume
			case 0x11: Set BGM Volume
			case 0x12: Set BGM Volume
			case 0x13: Set BGM Volume
			case 0x14: Set BGM Volume
			case 0x21: Mute BGM
			case 0x22: Mute WAV
			case 0x23: Mute KOE
			case 0x24: Mute SE 
			*/
			arg = NextCharwithIncl(); arg2 = ReadData();
			pp(("unsupported command cmd 0x75 - %x : arg %d\n", arg, arg2));
			break;
#endif
		case 0x08: // unsupported
			arg = NextCharwithIncl(); arg2 = 0;
			if (arg == 1) {
				pp(("unsupported command cmd 0x08 - 1\n"));
			} else if (arg == 0x10 || arg == 0x11) {
				arg2 = ReadData();
				pp(("unsupported command cmd 0x08 - %x : arg %d\n", arg, arg2));
			} else {
				pp(("unsupported command cmd 0x08 - %x\n", arg));
				goto error;
			}
			break;
			
		case 0x0b:
			if (graphics_save.DecodeSenario_GraphicsLoad(*this) == -1) {
				pp(("cmd 0x0b - DecodeSenario_GraphicsLoad() returned -1\n"));
			}
			break;
		case 0x0c:
			arg = NextCharwithIncl();
			switch(arg) {
/* @@@ */
/* きちんとサポートしているのは0x10のみ */
				case 0x10: case 0x16: {
					str = ReadString(buf);
					int seen = ReadData();
					if (arg == 0x16) {
						ReadData();
						ReadData();
					}
					p(("cmd 0x0c - 0x%02x decode anm file %s , seen number %d\n",arg,str,seen));
					ANMDAT* anm = new ANMDAT(str, local_system);
					anm->Init();
					if (anm->IsValid())
						anm->Play(seen);
					delete anm;
					break; }
				case 0x11: case 0x12: case 0x17: case 0x18: case 0x1a: case 0x30: {
					str = ReadString(buf);
					pp(("??? cmd 0x0c - 0x%02x decode anm ,str : %s, data: ", arg,str));
					ANMDAT* anm = new ANMDAT(str, local_system);
					anm->Init();
					while(NextChar() != 0) {
						int d = ReadData();
						pp(("%d, ",d));
						/* if (anm->IsValid()) anm->Play(d); */
					}
					pp(("\n"));
					NextCharwithIncl();
					delete anm;
					break;}
				case 0x13: case 0x19:
					str = ReadString(buf);
					pp(("??? cmd 0x0c - 0x%02x decode anm ,str : %s, data: ", arg,str));
					arg2 = ReadData(); arg3 = ReadData();
					pp(("%d, %d, ",arg2,arg3));
					if (arg == 0x19) {
						arg2 = ReadData(); arg3 = ReadData();
						pp(("%d, %d, ",arg2,arg3));
					}
					pp(("\n"));
				case 0x20:
				case 0x24:
					str = ReadString(buf);
					pp(("??? cmd 0x0c - 0x%02x decode anm ,str : %s, data: ", arg,str));
					while(NextChar() != 0) {
						int d = ReadData();
						pp(("%d, ",d));
					}
					pp(("\n"));
					NextCharwithIncl();
					break;

				case 0x21:
					p(("cmd 0x0c - 0x%02x: not supported.\n",arg));
					break;
				case 0x25:
					pp(("cmd 0x0c - 0x%02x: not supported.\n",arg));
					break;
				default:
					pp(("cmd 0x0c - 0x%02x: not supported.\n",arg));
					goto error;

			}
			break;
		case 0x0e:
			if (Decode_Music() == -1) goto error;
			break;
			
		case 0x13: {// fade in / out
			arg = NextCharwithIncl();
			SEL_STRUCT sel;
			sel.x1 = sel.x3 = sel.y1 = sel.y3 = 0; sel.x2 = local_system.DefaultScreenWidth()-1; sel.y2 = local_system.DefaultScreenHeight()-1;
			sel.sel_no = 4; sel.wait_time = local_system.config->GetParaInt("#FADE_TIME");
			int c1=0, c2=0, c3=0;
			if (arg == 1) {
				arg2 = ReadData();
				COLOR_TABLE& col = local_system.FadeTable(arg2);
				c1 = col.c1; c2 = col.c2; c3 = col.c3;
				p(("cmd 0x13 - 1 : fade in/out with table %d : %d, %d, %d\n",arg2,c1,c2,c3));
			} else if (arg == 2) {
				arg2 = ReadData(); arg3 = ReadData();
				COLOR_TABLE& col = local_system.FadeTable(arg2);
				c1 = col.c1; c2 = col.c2; c3 = col.c3;
				sel.wait_time = arg3;
				p(("cmd 0x13 - 2 : fade in/out, wait %d, with table %d : %d, %d, %d\n",arg3,arg2,c1,c2,c3));
			} else if (arg == 3) {
				c1 = ReadData(); c2 = ReadData(); c3 = ReadData();
				p(("cmd 0x13 - 3 : fade in/out with color %d, %d, %d\n",c1,c2,c3));
			} else if (arg == 4) {
				c1 = ReadData(); c2 = ReadData(); c3 = ReadData();
				sel.wait_time = ReadData();
				p(("cmd 0x13 - 4 : fade in/out with color %d, %d, %d, time %d\n",c1,c2,c3,arg2));
			} else if (arg == 0x10) {
				arg2 = ReadData();
				COLOR_TABLE& col = local_system.FadeTable(arg2);
				c1 = col.c1; c2 = col.c2; c3 = col.c3;
				/* 即座に画面消去、なのかなあ？ */
				sel.wait_time = 0;
				pp(("cmd 0x13 - 0x10  : fade in/out with table %d : %d, %d, %d\n",arg2,c1,c2,c3));
			} else if (arg == 0x11) {
				c1 = ReadData(); c2 = ReadData(); c3 = ReadData();
				sel.wait_time = 0;
				pp(("cmd 0x13 - 0x11 : fade in/out with table %d, %d, %d\n",c1,c2,c3));
			} else {
				pp(("cmd 0x13 - %d : fade in / out; not implemented.\n",arg));
				goto error;
			}
			graphics_save.DecodeSenario_Fade(&sel,c1,c2,c3);
			break; }
		case 0x17: // shake
			if (NextChar() != 1) goto error;
			NextCharwithIncl();
			arg = ReadData();
			local_system.Shake(arg);
			p(("cmd 0x17 - 1 : shake screen. arg = %d\n", arg));
			break;
		case 0x18: // ???
/* @@@ */
			if (NextChar() != 1) goto error;
			NextCharwithIncl();
			arg = ReadData();
			pp(("cmd 0x18 - 1 : Set text foreground color (akz.) arg = %d\n",arg));
			if (arg > 255 || arg <= 0) arg = 0x7f;
			{ char tmp[15]; sprintf(tmp, "%c%c", 2, arg);
			local_system.DrawText(tmp);}
			break;
		case 0x19: // wait?
			if (Decode_Wait() == -1) goto error;
			break;
		case 0x15: // 条件ジャンプ
		case 0x16: // global call
		case 0x1d: // call array
		case 0x1e: // jump array
		case 0x1c: // 無条件ジャンプ
		case 0x1b:
		case 0x20:
			{ GlobalStackItem jump = Decode_Jump();
#ifndef SUPRESS_JUMP
			if (! jump.IsValid()) goto error;
			if (jump.IsGlobal()) {
				p(("global jump to senario %d\n", jump.GetSeen()));
				printf("global jump to senario %d\n", jump.GetSeen());
				senario.SetPoint(jump);
				return -1;
				// return jump.GetSeen(); // end senario
			} else { // local
				data = basedata + jump.GetLocal();
			}
#endif
			break;}
		case 0x22:
		case 0x23:
		case 0x24:
		case 0x25:
		case 0x26:
		case 0x27:
		case 0x28:
		case 0x29: {
			pp(("cmd 0x%2x : ???\n",cmd));
			break;
		}
		case 0x2d: { /* とりあえず魔薬用に */
			arg=ReadInt();
			pp(("cmd 0x2d : new sub scenario, length %d; -------------------------------------------------\n",arg));
			break;
		}
		case 0x2e: { /* 魔薬用 */
			arg = NextCharwithIncl();
			arg2 = ReadData(); arg3=0;
			if (arg != 1) arg3 = ReadData();
			pp(("cmd 0x2e - 0x%02x : ??? , arg %d,%d\n",arg,arg2,arg3));
			break;
		}
		case 0x31:
			if (NextChar() == 2) {
				NextCharwithIncl();
				pp(("cmd 0x31 - 2 : clear text rank buffer.\n"));
				break;
			} else goto error;
		case 0x5b: // 多数の変数を一気にセット
			flags.DecodeSenario_VargroupRead(*this);
			break;
		case 0x5c: // セット
			flags.DecodeSenario_VargroupSet(*this);
			break;
		case 0x5d: // コピー
			flags.DecodeSenario_VargroupCopy(*this);
			break;
		case 0x5e: // 時間を設定
			{ time_t cur_tm = time(0); struct tm* tm = localtime(&cur_tm);
			arg = NextCharwithIncl();
			arg2 = ReadData();
			p(("cmd 0x5e - 0x%02x : set time/seen_no, index %d\n",arg,arg2));
			switch(arg) {
			case 1: arg3 = tm->tm_mon*100 + tm->tm_mday; break;
			case 2: arg3 = tm->tm_hour*100+ tm->tm_min; break;
			case 3: arg3 = tm->tm_year; break;
			case 4: arg3 = tm->tm_wday; break;
			case 0x10: arg3 = seen_no; break;
			default: goto error;
			}
			flags.SetVar(arg2, arg3);
			break;
		}
		case 0x5f: // 変数操作？
			arg = NextCharwithIncl();
			p(("cmd 0x5f - %d : var group set\n",arg));
			if (arg == 0x20) {
				arg = NextCharwithIncl(); arg2 = ReadData(); arg3 = ReadData();
				p(("cmd 0x5f - 0x20 : n = %d, dest = %d, src = %d+ : ",arg,arg2,arg3));
				for (i=0; i<arg; i++) {
					int data = ReadData();
					p(("%d , ",data));
					flags.SetVar(arg2++, flags.GetVar(arg3+data));
				}
				break;
			} else if (arg == 0x10) {
				arg = ReadData(); arg2 = ReadData();
				arg3 = ReadData();
				if (arg3 != 0) arg2 /= arg3;
				if (arg2 < 0) arg2=0;
				if (arg2 > 100) arg2=100;
				flags.SetVar(arg,arg2);
				break;
			} else if (arg == 1) {
				arg = ReadData();
				int var = 0;

				arg2 = NextCharwithIncl();
				while(arg2 != 0) { switch(arg2) {
					case 1:
						arg3 = ReadData();
						pp(("cmd 0x5f - 1 - 1 : var : %d, var2 %d\n",arg,arg3));
						var += flags.GetVar(arg3);
						break;
					case 2:
						arg3 = ReadData(); arg4 = ReadData();
						pp(("cmd 0x5f - 1 - 2 : var : %d, var2 %d - %d\n",arg,arg3,arg4));
						for (; arg3<=arg4; arg3++) 
							var += flags.GetVar(arg3);
						break;
					case 0x11:
						arg3 = ReadData();
						pp(("cmd 0x5f - 1 - 0x11 : var : %d, bit %d\n",arg,arg3));
						if (flags.GetBit(arg3)) var++;
						break;
					case 0x12:
						arg3 = ReadData(); arg4 = ReadData();
						pp(("cmd 0x5f - 1 - 0x12 : var : %d, bit %d - %d\n",arg,arg3,arg4));
						for (; arg3<=arg4; arg3++) 
							if (flags.GetBit(arg3)) var++;
						break;
					}
					arg2 = NextCharwithIncl();
				}
				flags.SetVar(arg, var);
				break;
			} else goto error;
		case 0x60:
			if (NextChar() == 0x20) { // ゲーム終了
				NextCharwithIncl();
				p(("cmd 0x60 - 0x20 : Game end.\n"));
#ifndef SUPRESS_JUMP
				GlobalStackItem item; item.SetGlobal(-1,-1);
				senario.SetPoint(item);
				return -1;
#endif
				break;
			} else if (NextChar() == 4) {;
				int cur_point = GetPoint() - 1;
				NextCharwithIncl(); TextAttribute str;
				ReadStringWithFormat(str);
#ifndef SUPRESS_KEY
				if (str.Condition()) {
					BackLog().AddSetTitle(str.Text(), cur_point, seen_no);
					local_system.TitleEvent();
					local_system.SetTitle(str.Text());
				}
#endif
				char buf[1024];
				kconv( (unsigned char*)str.Text(), (unsigned char*)buf);
				p(("cmd 0x60 - 4 : set title : %s, condition %d\n",buf,str.Condition()));
				break;
			} else if (NextChar() == 5) {;
				NextCharwithIncl();
				local_system.MakePopupWindow();
				p(("cmd 0x60 - 5 : make popup???\n"));
				break;
			} else if (NextChar() == 2) {
				// ゲームのロード : 準備だけして、制御を戻す
				NextCharwithIncl();
				arg = ReadData();
				// local_system.LoadGame(arg);
				p(("cmd 0x60 - 2 : global jump? to %d , +0x3e8  : read save file???\n",arg));
#ifndef SUPRESS_JUMP
/*
				GlobalStackItem item; item.SetGlobal(seen_no, GetPoint());
				return -1;
*/
#endif
				break;
			/* } else if (NextChar() == 3) { // セーブ */
			} else if (NextChar() == 0x10 || NextChar() == 0x11 || NextChar() == 0x14 || NextChar() == 0x03) {
				/* @@@ */
				/* PureHeart のテキストより：
				** 0x60 - 10 : CD のシリアル番号を得る。Var[arg] に代入
				** 0x60 - 11 : インストールされた CD のドライブ名を得る。 Str[arg] に代入
				** 0x60 - 14 : 総トラック数を得る。Var[arg]に代入
				** 0x60 - 15 : 第一引数に与えられたトラックのなにかの情報を得る。Var[arg2]に代入
				*/
				int c = NextCharwithIncl(); arg2 = ReadData();
				pp(("cmd 0x60 - %x : ??? arg = %d\n",c,arg2));
				break;
			} else if ( NextChar() == 0x15) {
				NextCharwithIncl();  arg2 = ReadData(); arg3 = ReadData();
				pp(("cmd 0x60 - 0x15 ??? arg = %d,%d\n",arg2,arg3));
				break;
			} else if (NextChar() == 0x30) {;
				NextCharwithIncl();
				arg2 = ReadData(); arg3 = ReadData();
				pp(("cmd 0x60 - 0x30 : ??? save data check ???; data %d, var %d???\n",arg2, arg3));
				break;
			} else if (NextChar() == 0x31) {;
				NextCharwithIncl();
				arg2 = ReadData(); arg3 = ReadData();
				pp(("cmd 0x60 - 0x31 : check save validity ; data %d, var %d???\n",arg2, arg3));
#ifndef SUPRESS_JUMP
				arg2 = senario.IsValidSaveData(arg2-1, buf);
				flags.SetVar(arg3, arg2);
				pp(("cmd 0x60 - 0x31 : return data %d, var %d???\n",arg2, arg3));
#endif
				break;
			} else  goto error;
		case 0x61:
/* @@@ */
#if 0
	subcmd = databuf[curpos++];
	switch (subcmd) {
		case 0x01:		// メインスクリーン内でそのまま名前入力？（好き好き）
			x = Scn_ReadValue(); y = Scn_ReadValue();		// テキストボックス始点？
			ex = Scn_ReadValue(); ey = Scn_ReadValue();		// テキストボックス終点？
			r = Scn_ReadValue(); g = Scn_ReadValue(); b = Scn_ReadValue();		// テキスト色
			br = Scn_ReadValue(); bg = Scn_ReadValue(); bb = Scn_ReadValue();	// 背景色
			cmd = 0;
			break;
		case 0x02:		// $01で作ったテキストボックスの値の代入先？（入力完了？）
			idx = Scn_ReadValue();
			cmd = 0;
			break;
		case 0x03:		// $01で作ったテキストボックスの値の代入先？（実際の入力開始？）
			idx = Scn_ReadValue();
			cmd = 0;
			break;
		case 0x04:		// $01で作ったテキストボックスのクローズ？
			cmd = 0;
			break;
		case 0x11:
			idx = Scn_ReadValue();
			data = Scn_ReadValue();
			Sys_SetNameString(idx, Flag_GetStr(data));
			cmd = 0;
			break;
		case 0x10:
		case 0x12:
			idx = Scn_ReadValue();
			data = Scn_ReadValue();
			Sys_GetNameString(idx, Flag_GetStr(data));
			cmd = 0;
			break;
		case 0x20:		// 名前入力（1項目だけ、項目名固定）（魔薬）
			nameindex[0] = Scn_ReadValue()+1;
			sprintf(nametitle[0], "名前");
			nametitle[1][0] = 0;
			nameindex[1] = 0;
//			Sys_NameInputDlg(nametitle[0], nametitle[1], nameindex[0], nameindex[1]);
			strcpy(namenew[0], ini.name[nameindex[0]-1]);
			NameInputFlag = NI_WAIT;
			break;
		case 0x21:		// ？？？ 文字一覧から選択式の入力？（絶望）
			idx = Scn_ReadValue();
			Scn_ReadText(buf);
			Scn_ReadValue(); Scn_ReadValue(); Scn_ReadValue();
			Scn_ReadValue(); Scn_ReadValue(); Scn_ReadValue();
			Scn_ReadValue(); Scn_ReadValue(); Scn_ReadValue();
			cmd = 0;
			break;
		case 0x24:		// 名前入力（最大2項目、項目名指定可）
			data = databuf[curpos++];
			nametitle[0][0] = 0;
			nametitle[1][0] = 0;
			nameindex[0] = 0;
			nameindex[1] = 0;
			for (i=0; i<data; i++) {
				idx = Scn_ReadValue();
				Scn_ReadFormattedText(buf, &attr);
				if ( i<2 ) {
					sprintf(nametitle[i], "%s", buf);
					nameindex[i] = idx;
				}
			}
			strcpy(namenew[0], ini.name[nameindex[0]-1]);
			strcpy(namenew[1], ini.name[nameindex[1]-1]);
			NameInputFlag = NI_WAIT;
			break;
		case 0x30:
		case 0x31:
			cmd = 0;
			break;
		default:
			cmd = 0;
			break;
	}
	return TRUE;
#endif
			if (NextChar() == 0x01) {
				NextCharwithIncl();
				int x = ReadData();
				int y = ReadData();
				int width = ReadData()-x+1;
				int height = ReadData()-y+1;
				int c1,c2,c3; COLOR_TABLE fore, back;
				c1 = ReadData(); c2 = ReadData(); c3 = ReadData(); fore.SetColor(c1,c2,c3);
				c1 = ReadData(); c2 = ReadData(); c3 = ReadData(); back.SetColor(c1,c2,c3);
				pp(("cmd 0x61 - 0x01 : make name window ; (%d,%d) - (%d,%d) ; fore color %d,%d,%d ; back %d,%d,%d\n",
					x, y, x+width, y+width,
					fore.c1, fore.c2, fore.c3,
					back.c1, back.c2, back.c3));
				local_system.DeleteTextWindow();
				senario.SetNameEntry(local_system.OpenNameEntry(x, y, width, height, fore, back));
				break;
			} else if (NextChar() == 0x02) {
				NextCharwithIncl();
				arg2 = ReadData();
				flags.SetStrVar(arg2, senario.GetNameFromEntry());
				kconv((unsigned char*)flags.StrVar(arg2), (unsigned char*) buf);
				pp(("cmd 0x61 - 0x02 : read name window? %d = %s\n",arg2, buf));
				break;
			} else if (NextChar() == 0x03) {
				NextCharwithIncl();
				arg2 = ReadData();
				senario.SetNameToEntry(flags.StrVar(arg2));
				kconv((unsigned char*)flags.StrVar(arg2), (unsigned char*) buf);
				pp(("cmd 0x61 - 0x03 : set name window? %d = %s\n",arg2, buf));
				break;
			} else if (NextChar() == 0x04) {
				NextCharwithIncl();
				senario.CloseNameEntry();
				pp(("cmd 0x61 - 0x04 : close  name window?\n"));
				break;
			} else if (NextChar() == 0x10) {
				NextCharwithIncl();
				arg2 = ReadData(); arg3 = ReadData();
				flags.SetStrVar(arg3, (const char*)macro.GetMacro(arg2));
				kconv((unsigned char*)flags.StrVar(arg3), (unsigned char*)buf);
				pp(("cmd 0x61 - 0x10 : get name macro[%d] -> %d ; %s\n",arg2,arg3,buf));
				break;
			}else if (NextChar() == 0x11) {
				NextCharwithIncl();
				arg2 = ReadData(); arg3 = ReadData();
				macro.SetMacro(arg2, (const unsigned char*)flags.StrVar(arg3));
				kconv((unsigned char*)flags.StrVar(arg3), (unsigned char*)buf);
				pp(("cmd 0x61 - 0x11 : set name macro arg %d, %d ; %s\n",arg2,arg3,buf));
				break;
			}else if (NextChar() == 0x12) {
				NextCharwithIncl();
				arg2 = ReadData(); arg3 = ReadData();
				pp(("cmd 0x61 - 0x12 : name macro related; [%d] -> %d \n",arg2,arg3));
				break;
			} else if (NextChar() == 0x20) {
				NextCharwithIncl();
				arg2 = ReadData();
				AyuSys::NameInfo info;
				info.SetInfo("名前", (const char*)macro.GetMacro(arg2), arg2);
				if (local_system.OpenNameDialog(&info, 1) == 0) {
					macro.SetMacro(info.name_index, (const unsigned char*)info.new_name);
				}
				pp(("cmd 0x61 - 0x20 : name window with one arg; arg %d\n",arg2));
				break;
			} else if (NextChar() == 0x21) {
				NextCharwithIncl();
				ReadData();
				ReadString(buf);
				ReadData(); ReadData(); ReadData();
				ReadData(); ReadData(); ReadData();
				ReadData(); ReadData(); ReadData();
				pp(("cmd 0x61 - 0x21 : ???  name window?\n"));
				break;
			} else if (NextChar() == 0x24) {
				NextCharwithIncl();
				arg = NextCharwithIncl();
				pp(("cmd 0x61 -0x24 - 1: set name : arg %d:",arg));
				TextAttribute* strs = new TextAttribute[arg];
				AyuSys::NameInfo* info = new AyuSys::NameInfo[arg];
				int list_deal = 0;
				
				for (i=0; i<arg; i++) {
					arg2 = ReadData();
					ReadStringWithFormat(strs[i]);
					pp(("var %d ; str %s(%d) , ",arg2,strs[i].Text(),strs[i].Condition()));
					if (strs[i].Condition()) {
						arg2--;
						info[list_deal].SetInfo(strs[i].Text(), (const char*)macro.GetMacro(arg2), arg2);
						list_deal++;
					}
				}
				if (local_system.OpenNameDialog(info, list_deal) == 0) {
					for (i=0; i<list_deal; i++) {
						macro.SetMacro(info[i].name_index, (const unsigned char*)info[i].new_name);
					}
				}
				delete[] strs;
				delete[] info;
				pp(("\n"));
				break;
			} else if (NextChar() == 0x30) {
				NextCharwithIncl();
				pp(("cmd 0x61 - 0x30 : ???\n"));
				break;
			} else if (NextChar() == 0x31) {
				NextCharwithIncl();
				pp(("cmd 0x61 - 0x31 : ???\n"));
				break;
			}
			goto error;
		case 0x63: case 0x64: case 0x66: case 0x67: case 0x68:
			if (graphics_save.DecodeSenario_Graphics(*this) == -1) goto error;
			break;
		case 0x69:
			{ arg = NextCharwithIncl(); arg2 = NextCharwithIncl();
				pp(("cmd 0x69 - %d, %d, args : ",arg,arg2));
				for (arg=0; arg<6; arg++) {
					arg2 = ReadData(); pp(("%d,",arg2));
				}
				pp(("\n"));
			}
			break;

		case 0x6a: {// エンディングのスタッフ・ロール
#ifndef SUPRESS_KEY
			local_system.StopTextSkip();
			local_system.SetTextFastMode(false);
#endif
			int x,y,flag;
			int subcmd = NextChar();
			if (subcmd == 0x03) {
				/* AVG32 for Mac によれば flowers の OP で複数枚PDT 表示に使われている。
				** また、さよならを教えてでは
				**  0x6a, 0x03, 0x03, 0, 0,
				**  "SA_MA013",200,
				**  "SA_MA015",200,
				**  "SA_MA011",200
				**  185, 4496, 500 (?)
				** というパラメータ
				*/ 
				NextCharwithIncl();
				int deal = NextCharwithIncl();
				int pos = ReadData();
				int wait = ReadData();
				pp(("cmd 0x6a - 0x03: deal %d,%d,%d: continurous draw???",deal,pos,wait));
				void* timer = local_system.setTimerBase();
				int tm = 0;
				for (i=0; i<deal; i++) {
					str = ReadString(buf);
					int local_wait = ReadData();
					local_system.LoadPDTBuffer(0, str);
					tm += local_wait;
					local_system.waitUntil(timer, tm);
					pp(("line %d : %s (%d)\n",i,str,local_wait));
				}
				local_system.freeTimerBase(timer);
				int l = NextCharwithIncl();
				pp(("last %d\n",l));
				break;
			}
			if (subcmd != 0x10 && subcmd != 0x30 && subcmd != 0x20) goto error;
			NextCharwithIncl();
			int alignment = NextCharwithIncl(); /* 1: center 3: right other: left */
			arg = NextCharwithIncl();
			arg2 = ReadData(); arg3 = ReadData(); arg4 = ReadData();
			int index = -1;
			if (subcmd == 0x30) index = ReadData();
			if (subcmd == 0x10) {
				p(("cmd 0x6a - 0x10 - 01  : staff roll. n = %d, arg = %d,%d,%d\n",arg,arg2,arg3,arg4));
			} else {
				p(("cmd 0x6a - 0x30 - 01  : staff roll with break. n = %d, arg = %d,%d,%d, index = %d\n",arg,arg2,arg3,arg4,index));
				flags.SetVar(index, 0);
				local_system.GetMouseInfoWithClear(x, y, flag);
			}
			// おそらく、arg3 == 1line あたりの時間
			// その過程でスクロール用のselをつくる
			SEL_STRUCT sel;
			sel.x1 = sel.x3 = 0; sel.y1 = sel.y3 = 0;
			sel.x2 = local_system.DefaultScreenWidth()-1; sel.y2 = local_system.DefaultScreenHeight()-1;
			sel.wait_time = arg3;
			sel.sel_no = 15; sel.arg4 = -arg4; // スクロール方向の設定
			// ファイル読み込みは PDT 2 スクロール内容の描画は PDT 3 画面内容の保存は PDT4
			// まず、PDT3,4 をクリア、初期化
			const int read_pdt = 2; const int draw_pdt = 3; const int reserve_pdt = 4;
			local_system.DisconnectPDT(draw_pdt);
			local_system.DisconnectPDT(reserve_pdt);
			local_system.CopyBuffer(0, 0, local_system.DefaultScreenWidth()-1, local_system.DefaultScreenHeight()-1, 0, 0, 0, reserve_pdt,0);
			for (i=0; i<arg; i++) {
				// ファイル読み込み
				str = ReadString(buf);
				int hspace = ReadData()+1;
				local_system.LoadPDTBuffer(read_pdt, str);
				int imwidth = local_system.PDTWidth(read_pdt);
				int imheight = local_system.PDTHeight(read_pdt);
				// スクロールバッファに転送
				local_system.CopyBuffer(0, 0, local_system.DefaultScreenWidth()-1, imheight+hspace-1, reserve_pdt,
					0, 0, draw_pdt, 0);
				int dest_x = (alignment == 1) ? (local_system.DefaultScreenWidth()-imwidth)/2 : /* 中央揃え */
						(alignment == 3) ? (local_system.DefaultScreenWidth()-imwidth) : /* 右揃え */
						0; /* 左揃え */
				local_system.CopyPDTtoBuffer(0, 0, imwidth-1,
					imheight-1, read_pdt, dest_x, 0, draw_pdt, 0);
				// 描画
				if (imheight+hspace < 400) {
					sel.y2 = imheight + hspace-1;
					local_system.DrawPDTBuffer(draw_pdt, &sel);
				} else {
					sel.y2 = imheight - 1;
					local_system.DrawPDTBuffer(draw_pdt, &sel);
					while(hspace > 200) {
						hspace -= 200;
						sel.y2 = 199;
						local_system.DrawPDTBuffer(reserve_pdt, &sel);
					}
					sel.y2 = hspace - 1;
					local_system.DrawPDTBuffer(reserve_pdt, &sel);
				}
				local_system.GetMouseInfoWithClear(x,y,flag);
#ifndef SUPRESS_JUMP
				if (local_system.IsIntterupted() ||
					(subcmd == 0x30 && (flag == 0 || flag == 2) ) ) {
					for (; i<arg; i++) {
						ReadString(buf);
						ReadData();
					}
					if (index != -1) flags.SetVar(index, 1);
					break;
				}
#endif
				p(("\t%s : %d\n",str,hspace));
			}
			arg = NextCharwithIncl();
			p(("arg = %d\n",arg));
			break;
		}
		case 0x6c:
			arg = NextChar();
			if (arg == 2) {
				NextCharwithIncl();
				p(("cmd 0x6c - 2 :assign area buffer; "));
				str = ReadString(buf);
				p(("CUR file(not supported) : %s",str));
				str = ReadString(buf);
				senario.AssignArd(str);
				p(("ARD file : %s\n",str));
				break;
			} else if (arg == 3) {
				NextCharwithIncl();
				p(("cmd 0x6c - 3 : clear area buffer\n"));
				senario.ClearArd();
				break;
			} else if (arg == 4) {
				int x, y, flag;
				int l, r, u, d, esc;
				NextCharwithIncl();
				arg2 = ReadData(); arg3 = ReadData();

				p(("cmd 0x6c - 4 : ???, arg %d,%d\n",arg2,arg3));
				local_system.SetMouseMode(1);
				local_system.GetMouseInfoWithClear(x, y, flag); /* クリックで識別 */
				local_system.GetKeyCursorInfo(l,r,u,d,esc);
				if (flag >= 2) flag = -1; /* CTRL, ホイールなどは無視 */
				if (flag == 1 || esc) { /* 右クリック */
					flags.SetVar(arg2, 0);
					flags.SetVar(arg3, 1);
				} else {
					flags.SetVar(arg3, 0);
					flags.SetVar(arg2, senario.ArdData()->RegionNumber(x, y));
				}
				break;
			} else if (arg == 5) {
				int x, y, flag;
				int l, r, u, d, esc;
				NextCharwithIncl();
				arg2 = ReadData(); arg3 = ReadData();
				p(("cmd 0x6c - 5 : ???, arg %d,%d\n",arg2,arg3));
				local_system.SetMouseMode(1);
				local_system.GetMouseInfoWithClear(x, y, flag);
				local_system.GetKeyCursorInfo(l,r,u,d,esc);
				if (flag >= 2) flag = -1; /* CTRL, ホイールなどは無視 */
				if (flag == -1 && esc) flag = 1;
				flags.SetVar(arg2, senario.ArdData()->RegionNumber(x, y));
				flags.SetVar(arg3, flag);
				break;
			} else if (arg == 0x10) {
				NextCharwithIncl(); arg2 = ReadData();
				p(("cmd 0x6c - 0x10 : set area invalid; arg %d\n",arg2));
				senario.ArdData()->SetInvalid(arg2);
				break;
			} else if (arg == 0x11) {
				NextCharwithIncl(); arg2 = ReadData();
				p(("cmd 0x6c - 0x11 : set area valid; arg %d\n",arg2));
				senario.ArdData()->SetValid(arg2);
				break;
			} else if (arg == 0x15) {
				NextCharwithIncl();
				p(("cmd 0x6c - 0x15 : set area number; "));
				arg2 = ReadData(); arg3 = ReadData();
				p(("x= %d, y= %d",arg2,arg3));
				arg4 = ReadData();
				flags.SetVar( arg4, senario.ArdData()->RegionNumber(arg2, arg3));
				p(("var[%d]=%d\n",arg4,flags.GetVar(arg4)));
				break;
			} else if (arg == 0x20) {
				NextCharwithIncl();
				arg2 = ReadData(); arg3 = ReadData();
				pp(("cmd 0x6c - 0x20 : area ??? command : arg %d,%d\n",arg2,arg3));
				break;
			}
			goto error;
		case 0x6d:
			arg = NextChar();
			switch(arg) {
				case 1: { int x, y, flag=-1; // 0x6d - 1 : クリックがあるまで待つ
					int l, r, u, d, esc;
					NextCharwithIncl();
					arg=ReadData(); arg2=ReadData(); arg3=ReadData();
#ifndef SUPRESS_KEY
					if (local_system.GrpFastMode() ==AyuSys::GF_NoGrp) {
						local_system.SetClickEvent(AyuSys::END_TEXTFAST);
						local_system.ClickEvent();
					}
					local_system.SetClickEvent(AyuSys::NO_EVENT);
#endif
					local_system.SetMouseMode(1);
					local_system.ClearMouseInfo();
					while(flag == -1) {
						local_system.CallIdleEvent();
						local_system.WaitNextEvent();
						if (local_system.IsIntterupted()) break;
						local_system.GetMouseInfoWithClear(x,y,flag);
						local_system.GetKeyCursorInfo(l,r,u,d,esc);
						if (flag >= 2) flag = -1;
						if (flag == -1 && esc) flag = 1;
					}
					flags.SetVar(arg, x);
					flags.SetVar(arg2, y);
					flags.SetVar(arg3, flag);
					p(("cmd 0x6d - 1 : arg = x->%d:%d, y->%d:%d, click->%d:%d\n",arg,x,arg2,y,arg3,flag));
					break; }
				case 2: {
					int x, y, clicked;
					int l, r, u, d, esc;
					NextCharwithIncl();
					arg=ReadData(); arg2=ReadData(); arg3=ReadData();
#ifndef SUPRESS_KEY
					if (local_system.GrpFastMode() ==AyuSys::GF_NoGrp) {
						local_system.SetClickEvent(AyuSys::END_TEXTFAST);
						local_system.ClickEvent();
					}
					local_system.WaitNextEvent();
					local_system.SetClickEvent(AyuSys::NO_EVENT);
#endif
					local_system.SetMouseMode(1);
					local_system.GetMouseInfo(x, y, clicked);
					local_system.GetKeyCursorInfo(l,r,u,d,esc);
					if (clicked >= 2) clicked = -1; /* CTRL, ホイールを無視 */
					if (clicked == -1 && esc) clicked = 1;
					p(("cmd 0x6d - 2 : arg = x->%d : %d,y->%d : %d,click->%d : %d : get window/mouse attribute\n",arg,x,arg2,y,arg3,clicked));
					
					flags.SetVar(arg,x);
					flags.SetVar(arg2,y);
					flags.SetVar(arg3,clicked);
					break; }
				case 3:
					NextCharwithIncl();
					p(("cmd 0x6d - 3 : ??? : process messages\n"));
					local_system.SetMouseMode(1);
					local_system.ClearMouseInfo();
					break;
				case 0x20:
					NextCharwithIncl();
					p(("cmd 0x6d - 0x20 : delete cursor\n"));
					local_system.DeleteMouse();
					break;
				case 0x21:
					NextCharwithIncl();
					p(("cmd 0x6d - 0x21 : show cursor\n"));
					local_system.DrawMouse();
					break;
				default:
					goto error;
			}
			break;
		case 0x6e:
			if (NextChar() == 1 || NextChar() == 2 || NextChar() == 3) {
				arg = NextCharwithIncl();
				int* d = CountCgmData(); int* d_orig = d;
				d++; int deal = 0;
				while(*d != -1) {
					if (flags.GetBit(*d++) != 0) deal++;
				} 
				int all_deal = *d_orig; int perc = 0;
				if (all_deal == 0) perc = 0;
				else { perc = deal*100/all_deal;}
				arg2 = ReadData();
				p(("cmd 0x6e - %d : get viewed graphics arg = %d\n",arg,arg2));
				if (arg == 1) {
					deal = all_deal;
				} else if (arg == 2) {
					deal = deal;
				} else if (arg == 3) {
					deal = perc;
				}
				flags.SetVar(arg2, deal);
				break;
			} else if (NextChar() == 4) {
				/* 簡易CG viewer 。左クリックで次の絵、右クリックで終了 */
				NextCharwithIncl();
				arg = ReadData();
				pp(("??? cmd 0x6e - 4 : auto CG show ??? arg %d\n",arg));
				break;
			} else if (NextChar() == 5) {
				NextCharwithIncl();
				arg = ReadData(); arg2 = ReadData(); arg3 = ReadData();
				p(("cmd 0x6e - 5: get var[%d] th CG info; return bit %d, file str[%d]\n",arg,arg3,arg2));
				const char* pdtfile;
				int bit_number = GetCgmInfo(flags.GetVar(arg), &pdtfile);
				if (bit_number == -1) {
					flags.SetStrVar(arg2, "");
					flags.SetVar(arg3, 0);
				} else {
					flags.SetStrVar(arg2, pdtfile);
					flags.SetVar(arg3, bit_number);
				} 
				break;
			}
			goto error;
		case 0x70:
			arg = NextCharwithIncl(); pp(("cmd 0x70: unsupported - %d: ",arg));
			switch(arg) {
				case 0x01: {
					arg=ReadData(); arg2=ReadData(); arg3=ReadData(); arg4=ReadData();
					int c1,c2,c3,t;
					local_system.config->GetParam("#WINDOW_ATTR", 3, &c1, &c2, &c3);
					local_system.config->GetParam("#WINDOW_ATTR_TYPE", 1, &t);
					flags.SetVar(arg2,c1); flags.SetVar(arg3,c2); flags.SetVar(arg4,c3);
					flags.SetVar(arg, t);
					pp(("get text window color, var %d, %d, %d, %d\n",arg,arg2,arg3,arg4));
					break;}
				case 0x02:
					arg=ReadData(); arg2=ReadData(); arg3=ReadData(); arg4=ReadData();
					local_system.config->SetParam("#WINDOW_ATTR", 3,arg2,arg3,arg4);
					local_system.config->SetParam("#WINDOW_ATTR_TYPE", 1, arg);
					pp(("set text window color, arg %d, %d, %d, %d\n",arg,arg2,arg3,arg4));
					break;
				case 0x03:
					arg=ReadData(); flags.SetVar(arg,0);
					pp(("get some flag, arg %d\n",arg));
					break;
				case 0x04:
					arg=ReadData();
					pp(("set some flag, var %d\n",arg));
					break;
				case 0x05:
					arg=ReadData(); flags.SetVar(arg,0);
					pp(("get some flag, arg %d\n",arg));
					break;
				case 0x06:
					arg=ReadData();
					pp(("set some flag, var %d\n",arg));
					break;
				case 0x10:
					arg=ReadData(); flags.SetVar(arg,0);
					pp(("get some flag, arg %d\n",arg));
					break;
				case 0x11:
					arg=ReadData();
					pp(("set some flag, var %d\n",arg));
					break;
			}
			break;
		case 0x72:
			arg = NextCharwithIncl(); arg2 = ReadData(); arg3 = ReadData();
			if (arg < 0x10) {
				p(("cmd 0x72 - %x : get text position, var[%d]=x, var[%d]=y\n",arg,arg2,arg3));
			} else {
				p(("cmd 0x72 - %x : set text position, x = %d, y = %d\n",arg,arg2,arg3));
			}
			/* 0X : get local_system variable
			** 1X : set local_system variable
			**  X = 1 : MSG Position
			**      2 : COM Position
			**      3 : SYS Position
			**      4 : SUB Position
			**      5 : GRP Position
			**	** name is defined in gameexe.ini
			*/
#ifndef SUPRESS_KEY
			if (arg < 0x10) {
				int x=0, y=0;
				switch(arg) {
					case 1: local_system.config->GetParam("#WINDOW_MSG_POS", 2, &x, &y);break;
					case 2: local_system.config->GetParam("#WINDOW_COM_POS", 2, &x, &y);break;
					case 3: local_system.config->GetParam("#WINDOW_SYS_POS", 2, &x, &y);break;
					case 4: local_system.config->GetParam("#WINDOW_SUB_POS", 2, &x, &y);break;
					case 5: local_system.config->GetParam("#WINDOW_GRP_POS", 2, &x, &y);break;
				}
				flags.SetVar(arg2, x); flags.SetVar(arg3, y);
			} else {
				arg -= 0x10;
				switch(arg) {
					case 1: local_system.config->SetParam("#WINDOW_MSG_POS", 2, arg2, arg3);break;
					case 2: local_system.config->SetParam("#WINDOW_COM_POS", 2, arg2, arg3);break;
					case 3: local_system.config->SetParam("#WINDOW_SYS_POS", 2, arg2, arg3);break;
					case 4: local_system.config->SetParam("#WINDOW_SUB_POS", 2, arg2, arg3);break;
					case 5: local_system.config->SetParam("#WINDOW_GRP_POS", 2, arg2, arg3);break;
				}
			}
#endif
			break;
		case 0x73: {
			int x=0,y=0;
			arg = NextCharwithIncl(); arg2 = ReadData();
			p(("cmd 0x73 - %x : arg = %d :",arg,arg2));
			switch(arg) {
				case 0x01:
					arg3=ReadData();pp(("get msgsize cmd 0x01, arg %d,%d.\n",arg2,arg3));
#ifndef SUPRESS_KEY
					local_system.config->GetParam("#MESSAGE_SIZE", 2, &x,&y);
					flags.SetVar(arg2, x);
					flags.SetVar(arg3, y);
#endif
					break;
				case 0x02:
					arg3=ReadData();pp(("set msgsize cmd 0x02, arg %d,%d.\n",arg2,arg3));
#ifndef SUPRESS_KEY
					local_system.config->SetParam("#MESSAGE_SIZE", 2, arg2,arg3);
#endif
					break;
				case 0x05:
					arg3=ReadData();pp(("get mojisize cmd 0x05, arg %d,%d.\n",arg2,arg3));
#ifndef SUPRESS_KEY
					local_system.config->GetParam("#MSG_MOJI_SIZE", 2, &x,&y);
					flags.SetVar(arg2,x);
					flags.SetVar(arg3,y);
#endif
					break;
					
				case 0x06:
					arg3=ReadData();pp(("set mojisize cmd 0x06, arg %d,%d.\n",arg2,arg3));
#ifndef SUPRESS_KEY
					local_system.config->SetParam("#MSG_MOJI_SIZE", 2, arg2,arg3);
#endif
					break;
				case 0x19: pp(("akz: set background.\n"));
					if (arg2 > 255 || arg2 <= 0) arg2 = 0x7f;
					{ char tmp[15]; sprintf(tmp, "%c%c",3,arg2);
					local_system.DrawText(tmp);
					break;}
				case 0x1d: pp(("set IniCtlKey.\n")); break;
				case 0x1f: pp(("set ??? cmd 0x1f, arg %d.\n",arg2)); break;
				case 0x2d: pp(("set Retkey wait.\n")); break;
				case 0x28: pp(("set ??? cmd 0x28, arg %d\n",arg2)); break;
				case 0x29: pp(("set ??? cmd 0x29, arg %d\n",arg2)); break;
				case 0x30: pp(("set ??? cmd 0x30, arg %d\n",arg2)); break;
				case 0x31: arg3=ReadData();
					pp(("set ??? cmd 0x31; set cursor pos to %d,%d? \n",arg2,arg3));
					break;
				case 0x32: pp(("set ??? cmd 0x32, arg %d\n",arg2)); break;
				case 0x34: pp(("set 'GameSpecInit??? cmd 0x34, arg %d\n",arg2));break;
				default: p(("Error!\n")); goto error;
			}
			break; }
		case 0x74:
			arg = NextCharwithIncl();
			p(("cmd 0x74 - %d : ",arg));
			switch(arg) {
				case 2: // ???
					arg2 = ReadData();
					p((" arg %d :???\n",arg2));
					break;
				case 4:
					arg2 = ReadData(); arg3 = ReadData();
					p(("SysComXXX[%d] <- %d\n",arg2,arg3));
					// SysCom : ポップアップメニューの設定
					// とりあえず、0,1,25, 28 をサポート
					if (arg2 == 0 || arg2 == 1) {
						local_system.ShowMenuItem("Load", arg3);
						local_system.ShowMenuItem("Save", arg3);
					} else if (arg2 == 25) {
						std::vector<const char*> items;
						items.push_back("Skip text");
						items.push_back("Auto-skip text");
						local_system.ShowMenuItem("SkipText", arg3);
						local_system.ShowMenuItem("AutoSkipText", arg3);
					} else if (arg2 == 28) {
						local_system.ShowMenuItem("GoMenu", arg3);
					} else 
						pp(("SysComXXX[%d] <- %d\n",arg2,arg3));
					break;
				default:
					goto error;
			}
			break;
		case 0x76: {
			if (NextChar() == 1) {
				NextCharwithIncl();
				arg = ReadData();
				pp(("cmd 0x76 - 1: set NVL_SYSTEM arg %d\n",arg));
#ifndef SUPRESS_KEY
				local_system.config->SetParam("#NVL_SYSTEM",1,arg);
#endif
			} else if (NextChar() == 2) {
				NextCharwithIncl();
				arg = ReadData();
				pp(("cmd 0x76 - 2: reset? debug state.\n"));
				local_system.ClearMouseInfo();
			} else if (NextChar() == 3) {
				NextCharwithIncl();
				pp(("cmd 0x76 - 3 : ???\n"));
			} else if (NextChar() == 4) {
				NextCharwithIncl();
				pp(("cmd 0x76 - 4 : ???\n"));
			} else if (NextChar() == 5) {
				NextCharwithIncl();
				pp(("cmd 0x76 - 5 : ???\n"));
			} else {
				goto error;
			}
			break;
		}
		case 0x7f:
			arg = ReadData();
			pp(("cmd 0x7f : arg = %d : not implemented\n",arg));
			break;
		default:
		error:
			printf("Error : Cannot decode %d(0x%x) / %d ; cmd %x\n",GetPoint(),GetPoint(),data_len,cmd);
			DumpData();
			{GlobalStackItem item; item.SetGlobal(-1,-1);
			senario.SetPoint(item);}
			return -1;
	}
	return 0;
}

// シナリオをデコード
// 実際の処理はおこなわない。使用しているファイルの名前だけ返す
int SENARIO_DECODE::DecodeSkip(char** strlist, int& list_pt, int list_max)
{
	int i; int arg; char buf[1024];
#ifdef PrintLineNumber
//	printf("pt %d / %d :\n",data-basedata, data_len);
#endif
	cmd = *data++;
	if (data-basedata > data_len) return -1;
	if (cmd >= 0x37 && cmd <= 0x59) { // 変数の操作
		flags.DecodeSkip_Calc(*this);
		return 0;
	}
	switch(cmd) {
		case 0x00:
			return -1; // end senario
		case 0x01:
		case 0x02:
		case 0x03:
		case 0x04:
		case 0x05:
		case 0x10:
		case 0xff:
			DecodeSkip_TextWindow();
			break;
#if 1 /* @@@ */
#if 0
		case 0x05:
			arg = NextCharwithIncl();
			break;
#endif
		case 0x75:
			arg = NextCharwithIncl();
			ReadData();
			break;
#endif
		case 0x08:
			arg = NextCharwithIncl();
			if (arg == 0x10 || arg == 0x11) ReadData();
			break;
		case 0x0b:
			graphics_save.DecodeSkip_GraphicsLoad(*this,strlist, list_pt, list_max);
			break;
		case 0x0c:
			arg = NextCharwithIncl();
			switch(arg) {
				case 0x10: ReadString(buf); ReadData(); break;
				case 0x16: ReadString(buf); ReadData(); ReadData(); ReadData(); break;
				case 0x13: ReadString(buf); ReadData(); ReadData(); break;
				case 0x19: ReadString(buf); ReadData(); ReadData(); ReadData(); ReadData(); break;
				case 0x11: case 0x12: case 0x17: case 0x18: case 0x1a: case 0x30: case 0x20: case 0x24:
					ReadString(buf);
					while(NextChar() != 0) ReadData();
					NextCharwithIncl();
					break;
				case 0x21: case 0x25: break;
				default: break;
			}
			break;
		case 0x0e: DecodeSkip_Music(); break;
		case 0x13: // fade in / out
			arg = NextCharwithIncl();
			if (arg == 1 || arg == 0x10) ReadData();
			else if (arg == 2) { ReadData(); ReadData(); }
			else if (arg == 3) { ReadData(); ReadData(); ReadData(); }
			else if (arg == 4) { ReadData(); ReadData(); ReadData(); ReadData(); }
			else if (arg == 0x11) { ReadData(); ReadData(); ReadData(); }
			break;
			
		case 0x17: // shake
			NextCharwithIncl();
			arg = ReadData();
			break;
		case 0x18: // ???
			NextCharwithIncl();
			arg = ReadData();
			break;
		case 0x19: // wait?
			DecodeSkip_Wait();
			break;
		case 0x15: // 条件ジャンプ
		case 0x16: // global call
		case 0x1c: // 無条件ジャンプ
		case 0x1d:
		case 0x1e:
		case 0x1b:
		case 0x20:
			DecodeSkip_Jump();
			break;
		case 0x22:
		case 0x23:
		case 0x24:
		case 0x25:
		case 0x26:
		case 0x27:
		case 0x28:
		case 0x29:
			break;
		case 0x2d:
			ReadInt(); break;
		case 0x2e:
			if (NextCharwithIncl() == 1) { ReadData();
			} else { ReadData(); ReadData();
			}
			break;
		case 0x60:
			arg = NextCharwithIncl();
			if (arg == 4) {
				TextAttribute text;
				ReadStringWithFormat(text,0);
#ifdef DEBUG_Other
				local_system.SetTitle(text.Text());
#endif
			} else if (arg == 2 || arg == 3 || arg == 0x10 || arg == 0x11 || arg == 0x14 || arg == 0x31) ReadData();
			else if (arg == 0x15) { ReadData(); ReadData(); }
			else if (arg == 5 || arg == 0x20) ;
			break;
		case 0x63: case 0x64: case 0x66: case 0x67: case 0x68:
			graphics_save.DecodeSkip_Graphics(*this);
			break;
		case 0x76:
			if (NextChar() == 1) {
				NextCharwithIncl();
				arg = ReadData();
			} else {
				NextCharwithIncl();
			}
			break;
		case 0xfe:
			ReadString(buf);
			break;
		case 0x5b: // 多数の変数を一気にセット
			flags.DecodeSkip_VargroupRead(*this);
			break;
		case 0x5c: // セット
			flags.DecodeSkip_VargroupSet(*this);
			break;
		case 0x5d: // コピー
			flags.DecodeSkip_VargroupCopy(*this);
			break;
		case 0x5e: NextCharwithIncl(); ReadData(); break;
		case 0x5f: // 変数操作？
			arg = NextCharwithIncl();
			if (arg == 0x20) {
				arg = NextCharwithIncl(); ReadData(); ReadData();
				for (i=0; i<arg; i++) {
					ReadData();
				}
				break;
			} else if (arg == 0x10) {
				ReadData(); ReadData(); ReadData(); break;
			} else if (arg == 1) {
				ReadData(); arg = NextCharwithIncl();
				while(arg != 0) {
					ReadData();
					if (arg == 2 || arg == 0x12) ReadData();
					arg = NextCharwithIncl();
				}
				break;
			}
			break;
		case 0x61:
			if (NextChar() == 1) {
				NextCharwithIncl();
				ReadData();ReadData();ReadData();ReadData();ReadData();
				ReadData();ReadData();ReadData();ReadData();ReadData();
			} else if (NextChar() == 2 || NextChar() == 3) {
				NextCharwithIncl();
				ReadData();
			} else if (NextChar() == 4) {
				NextCharwithIncl();
			} else if (NextChar() == 0x10 || NextChar() == 0x11 || NextChar() == 0x12) {
				NextCharwithIncl();
				ReadData(); ReadData();
			} else if (NextChar() == 0x20) {
				NextCharwithIncl();
				ReadData();
			} else if (NextChar() == 0x21) {
				NextCharwithIncl();
				ReadData();
				ReadString(buf);
				ReadData(); ReadData(); ReadData();
				ReadData(); ReadData(); ReadData();
				ReadData(); ReadData(); ReadData();
				break;
			} else if (NextChar() == 0x24) {
				NextCharwithIncl();
				arg = NextCharwithIncl();
				TextAttribute str;
				for (i=0; i<arg; i++) {
					ReadData();
					ReadStringWithFormat(str,0);
				}
				break;
			} else if (NextChar() == 0x30) {
				NextCharwithIncl();
				break;
			} else if (NextChar() == 0x31) {
				NextCharwithIncl();
				break;
			}
			break;
		case 0x69:
			{ NextCharwithIncl(); NextCharwithIncl();
				for (i=0; i<6; i++) {
					ReadData();
				}
			}
			break;

		case 0x6a:
			if (NextChar() == 0x10 || NextChar() == 0x30) {
				int subcmd = NextChar();
				NextCharwithIncl();
				NextCharwithIncl();
				arg = NextCharwithIncl();
				ReadData(); ReadData(); ReadData();
				if (subcmd == 0x30) ReadData();
				for (i=0; i<arg; i++) {
					ReadString(buf);
					ReadData();
				}
				ReadData(); ReadData(); ReadData();
				break;
			} else if (NextChar() == 0x03) {
				NextCharwithIncl();
				arg = NextCharwithIncl();
				ReadData(); ReadData();
				for (i=0; i<arg; i++) {
					ReadString(buf);
					ReadData();
				}
				NextCharwithIncl();
			}
			break;
		case 0x6c: {
			arg = NextChar();
			if (arg == 2) {
				NextCharwithIncl();
				ReadString(buf);
				ReadString(buf);
			} else if (arg == 3) {
				NextCharwithIncl();
			} else if (arg == 4) {
				NextCharwithIncl(); ReadData(); ReadData();
			} else if (arg == 5) {
				NextCharwithIncl(); ReadData(); ReadData();
			} else if (arg == 0x10) {
				NextCharwithIncl(); ReadData();
			} else if (arg == 0x11) {
				NextCharwithIncl(); ReadData();
			} else if (arg == 0x15) {
				NextCharwithIncl();
				ReadData(); ReadData();
				ReadData(); ReadData();
				flags.DecodeSkip_Condition(*this);
				ReadInt();
			} else if (arg == 0x20) {
				NextCharwithIncl(); ReadData(); ReadData();
			}
			break;
			}
		case 0x6d:
			arg = NextChar();
			switch(arg) {
				case 1:
					NextCharwithIncl();
					ReadData();ReadData(); ReadData();
					break;
				case 2: {
					NextCharwithIncl();
					ReadData(); ReadData(); ReadData();
					break; }
				case 3:
					NextCharwithIncl();
					break;
				case 0x20:
					NextCharwithIncl();
					break;
				case 0x21:
					NextCharwithIncl();
					break;
				default: break;
			}
			break;
		case 0x6e:
			if ( NextChar() == 1 || NextChar() == 2 ||
				NextChar() == 3 || NextChar() == 4) {
				NextCharwithIncl();
				arg = ReadData();
				break;
			} else if ( NextChar() == 5 ) {
				NextCharwithIncl();
				ReadData(); ReadData(); ReadData();
			}
			break;
		case 0x70:
			arg = NextCharwithIncl();
			switch(arg) {
				case 1: case 2: ReadData(); ReadData(); ReadData(); ReadData(); break;
				case 3: case 4: case 5: case 6: case 0x10: case 0x11: ReadData(); break;
			}
			break;
		case 0x72:
			NextCharwithIncl(); ReadData(); ReadData();
			break;
		case 0x73:
			NextCharwithIncl(); ReadData();
			break;
		case 0x74:
			arg = NextCharwithIncl();
			switch(arg) {
				case 2: ReadData(); break;
				case 4:
					ReadData(); ReadData(); break;
				default: break;
			}
			break;
		case 0x7f: ReadData(); break;
		case 0x31:
			if (NextChar() == 2) {
				NextCharwithIncl();
				break;
			}
			break;
		default:
			break;
	}
	return 0;
}


int* SENARIO::ListSeens(void) {
	/* ファイルリストを得る */
	char** file_list = file_searcher.ListAll(FILESEARCH::SCN);
	if (file_list == 0) return 0;
	/* 大きさを得る */
	int i;
	for (i=0; file_list[i] != 0; i++) ;
	int size = i;
	int* retlist = new int[size+1];
	
	/* 番号のリストにする */
	/* 同時に retlist を解放する */
	for (i=0; i<size; i++) {
		retlist[i] = atoi(file_list[i] + 4);
		delete[] file_list[i];
	}
	delete[] file_list;
	retlist[i] = -1;
	return retlist;
}

SENARIO::SENARIO(char* savedir, AyuSys& sys) : local_system(sys) {
	isReadHeader = 0; arddata = 0; in_proc = 0;
	if (local_system.Version() == 1) {
		save_head_size = 0x11e4 + 0x284;
		save_block_size = 0x20000;
		save_tail_size = 0;
	} else if (local_system.Version() == 0 || local_system.Version() == 2) { // version == 0,2
		save_head_size = 0x11e4 + 0x284;
		save_block_size = 0x21f9c;
		save_tail_size = 0;
	} else { // version >= 3
		save_head_size = 0x11e4 + 0x284;
		save_block_size = 0x21f9c;
		save_tail_size = READ_FLAG_SIZE*INT_SIZE;
	}

	basetime = local_system.setTimerBase();
	ClearReadFlag();

	data_orig = 0;
	seen_no = 0;
	name_entry = 0;
	last_grp_read_point = 0; grp_read_deal = 0;
	int i; for(i=0; i<SENARIO_GRPREAD; i++) {
		grp_read_buf[i] = new char[256];
		grp_read_buf[i][0] = '\0';
	}
	flags = new SENARIO_FLAGSDecode();
	macros = new SENARIO_MACRO();
	grpsave = new SENARIO_Graphics(sys);
	decoder = 0;
#ifndef SENARIO_DUMP
	MakeSaveFile(savedir);
	if (IsSavefileExist()) {
		ReadSaveHeader();
	} else
#endif /* SENARIO_DUMP */
		{
		// macro の初期化
		for (i=0; i<26; i++)
			if (sys.IniMacroName(i) != 0)
				macros->SetMacro(i, (unsigned char*)sys.IniMacroName(i));
		CreateSaveFile();
	}

	old_grp_state = 0; old_cdrom_track[0] = 0; old_effec_track[0] = 0;
	isReadHeader = 1;
}

// シナリオファイルを先読みする
void SENARIO::ReadGrp(void) {
	if (decoder == 0) return;
	if (last_grp_read_point <= decoder->GetPoint() &&
		(last_grp_read_point+300) > decoder->GetPoint() ) {
		// 先読みが終わっていれば、なにもしない
		if (grp_read_deal >= SENARIO_GRPREAD ||
			grp_read_buf[grp_read_deal][0] == '\0') return;
		// そうでなければ、ファイルを読む
		local_system.PrereadPDTFile(grp_read_buf[grp_read_deal++]);
		return;
	} else {
		// シナリオの場所が変わった：読み直し
		last_grp_read_point = decoder->GetPoint();
		int i; for (i=0; i<SENARIO_GRPREAD; i++) grp_read_buf[i][0] = '\0';
		int list_pt = 0;
		while(list_pt < SENARIO_GRPREAD) {
			if (decoder->DecodeSkip(grp_read_buf, list_pt, SENARIO_GRPREAD) == -1) break;
		}
		grp_read_deal = 0;
		decoder->SetPoint(last_grp_read_point);
	}
}

/* #define DUMP_SENARIO_HEADER */
/* 作成されるデータは末尾に 0 で埋められた 1024byte の領域を予備として持つ*/
unsigned char* SENARIO::MakeSenarioData(int seen_no, int* ret_len) {
	char fname[20]; snprintf(fname, 20, "SEEN%03d.TXT",seen_no);
	ARCINFO* info = file_searcher.Find(FILESEARCH::SCN, fname,".TXT");
	if (info == 0) return 0; // cannot found
	// read raw data
	int flen = info->Size();
	const char* data_o = info->Read();
	// read header
	int pad_len = read_little_endian_int(data_o + 0x18);
	const char* header = data_o + 0x50 + pad_len*4;
	int header_len = read_little_endian_int(header-0x24);
	int header_str_len = 0;
	int i; for (i=0; i<header_len; i++) {
#ifdef DUMP_SENARIO_HEADER
		printf("header %02x\n",int(header[0])&0xff);
#endif
		int hlen2 = header[1]; header += 2;
		header_str_len += hlen2+1;
		int j; for (j=0; j<hlen2; j++) {
#ifdef DUMP_SENARIO_HEADER
		printf("\t%d : %02x\n",j,int(header[0])&0xff);
#endif
			int hlen3 = header[1]; header += 2;
			int k; for (k=0; k<hlen3; k++) {
				int hlen4 = *header++;
#ifdef DUMP_SENARIO_HEADER
				printf("\t\t%d : ",k);
				int l; for (l=0; l<hlen4; l++) {
					printf("(%d,%d,%d), ",header[0],header[1],header[2]);
					header += 3;
				}
				printf("\n");
#else
				header += hlen4*3;
#endif
			}
		}
	}
	for (i=0; i<header_str_len; i++) {
#ifdef DUMP_SENARIO_HEADER
		printf("head string[%2d] = %s\n",i,header+1);
#endif
		header += *header + 1;
	}
	int first_menu_point = read_little_endian_int(header+0x0f);
	header += 0x13; // 5*3 + 4
	flen -= header - data_o;
	// make patch
	const unsigned char* sdata = (const unsigned char*)header;
	/* pdata には sdata と異なる、新しい領域が new されて返る。
	** また、1024byte の余裕を持つ
	*/
	unsigned char* pdata = SENARIO_PATCH::PatchAll(seen_no,
		sdata, flen, local_system.Version());
	delete info; /* data_o も同時に解放される */
	/* シナリオが特殊な構造を持つ場合、とりあえず読める形式にする */
	if (header_len > 0) {
		while(first_menu_point < flen) {
			pdata[first_menu_point-1] = 0x2d;
			first_menu_point += read_little_endian_int((char*)pdata+first_menu_point);
			first_menu_point += 4;
		}
	}
	if (ret_len) *ret_len = flen;
	return pdata;
}

GlobalStackItem SENARIO::Play(GlobalStackItem item) {
	if (! item.IsValid()) return item;
	if (item.IsGlobal()) {
		// ファイルの読み込み
		if (item.GetSeen() != seen_no) {
			char* backlog = 0;
			if (decoder) {
				if (item.GetSeen() == local_system.config->GetParaInt("#SEEN_MENU")) {
					local_system.DeleteText();
				} else {
					backlog = new char[BACKLOG_LEN+10];
					decoder->BackLog().AddSavePoint(item.GetLocal(), item.GetSeen());
					decoder->BackLog().PutLog(backlog, BACKLOG_LEN+10, 0);
				}
				delete decoder;
			}
			decoder = 0;
			seen_no = item.GetSeen();
			int slen; unsigned char* sdata;
			sdata = MakeSenarioData(seen_no, &slen);
			if (sdata == 0) {
				seen_no = 0;
				item.SetGlobal(-1,-1);
				return item;
			}
			decoder = new SENARIO_DECODE(seen_no,
				sdata, slen, *this, *flags, *macros, *grpsave, local_system);
			if (data_orig) delete[] data_orig;
			data_orig = sdata;
			if (backlog) {
				decoder->BackLog().SetLog(backlog, BACKLOG_LEN+10, 0);
				delete[] backlog;
			}
			// 必要なら read flag を割り当て
			AssignReadFlag();
		}
		// global jump があったら、save point を動かす
		decoder->BackLog().AddSavePoint(item.GetLocal(), item.GetSeen());
		// 最後に backlog を開始
		decoder->BackLog().StartLog(0);
	}
	// item で指定された場所から開始
	decoder->SetPoint(item.GetLocal());
	current_point = item;
	local_system.ClearIntterupt();
	while( decoder->Decode() == 0) {
#ifdef DEBUG_DATA
		// シナリオのデータそのものをダンプ
		int point;
		printf(" <dump %d - %d >: ",current_point.GetLocal(),decoder->GetPoint());
		for (point=current_point.GetLocal(); point<decoder->GetPoint(); point++) {
			printf("%02x ",int(patched_data[point])&0xff);
		}
		printf("\n");
		fflush(stdout);
#endif
		local_system.CallProcessMessages();
		current_point.SetLocal(decoder->GetPoint());
		if (local_system.IsIntterupted()) {
			/* 必要なら、backlog mode にはいる */
			if (local_system.GetBacklog()) {
				GlobalStackItem item = decoder->BackLog().View();
				/* seen が変更しないならlocal jump */
				if (item.GetSeen() == seen_no)
					item.SetLocal(item.GetLocal());
				if (item.IsValid()) SetPoint(item);
				CheckGrpMode();
			}
			break;
		}
		/* 画像読み飛ばしモードの変化の検知 */
		CheckGrpMode();
	}
	CloseNameEntry(); // 名前入力ウィンドウが開いていれば閉じる
		// この処理ではシナリオ間ジャンプなどがあると閉じてしまうが・・・
	return current_point;
}

void AyuSys::RestoreGrp(void) {
	if (main_senario && GrpFastMode() != GF_NoGrp) main_senario->RestoreGrp();
}
void SENARIO::RestoreGrp(void) {
	if (old_grp_mode != AyuSys::GF_NoGrp) return;
	old_grp_mode = AyuSys::GF_Normal;
	local_system.SetIsRestoringFlag(true);
	if (old_grp_state == 0 || grpsave->CompareBuffer(old_grp_state, old_glen)) {
		local_system.DeleteTextWindow();
		grpsave->Restore();
	}
	if (strcmp(old_cdrom_track, local_system.GetCDROMTrack())) {
		strcpy(old_cdrom_track, local_system.GetCDROMTrack());
		if (old_cdrom_track[0] != 0 && local_system.GetCDROMMode() == MUSIC_CONT)
			local_system.PlayCDROM(old_cdrom_track);
		else
			local_system.StopCDROM();
	}
	if (strcmp(old_effec_track, local_system.GetEffecTrack())) {
		strcpy(old_effec_track, local_system.GetEffecTrack());
		if (old_effec_track[0] != 0 && local_system.GetEffecMode() == MUSIC_CONT)
			local_system.PlayWave(old_effec_track);
		else
			local_system.StopWave();
	}
	local_system.SetIsRestoringFlag(false);
	if (old_grp_state) {
		delete[] old_grp_state;
		old_grp_state = 0;
	}
	old_cdrom_track[0] = 0; old_effec_track[0] = 0;
}

void SENARIO::CheckGrpMode(void) {
	if (old_grp_mode != AyuSys::GF_NoGrp && local_system.GrpFastMode() == AyuSys::GF_NoGrp) {
		/* スキップ前の状態を保存 */
		if (old_grp_state != 0) delete[] old_grp_state;
		old_glen = grpsave->StoreBufferLen();
		old_grp_state = new char[old_glen];
		grpsave->StoreBuffer(old_grp_state, old_glen);
		strcpy(old_cdrom_track, local_system.GetCDROMTrack());
		strcpy(old_effec_track, local_system.GetEffecTrack());
	} else if (old_grp_mode == AyuSys::GF_NoGrp && local_system.GrpFastMode() != AyuSys::GF_NoGrp) {
		/* スキップ後に状態が変わっていれば書き直し */
		RestoreGrp();
	}
	old_grp_mode = local_system.GrpFastMode();
}
char* SENARIO::GetTitle(int s) {
	// ファイルを読み込み
	if (decoder) delete decoder;
	decoder = 0; seen_no = s;
	// decoder をつくる
	unsigned char* sdata; int slen;
	sdata = MakeSenarioData(seen_no, &slen);
	if (sdata == 0) return 0;
	decoder = new SENARIO_DECODE(seen_no,
		sdata, slen, *this, *flags, *macros, *grpsave, local_system);
	// 読み込む
	local_system.SetTitle(0);
	while(local_system.GetTitle() == 0) {
		int pt = 0; 
		if (decoder->DecodeSkip(grp_read_buf, pt, 0) == -1) break;
	}
	delete decoder; decoder = 0;
	delete[] sdata;
	return local_system.GetTitle();
}

// 読み込んだseenのread flag を適当な位置に割り当てる
void SENARIO::AssignReadFlag(void) {
	max_read_flag = -1;
	if (local_system.Version() < 3) return;
	if (decoder == 0) return;
	int point = decoder->GetPoint();
	// 最初から最後まで読み込む
	decoder->SetPoint(0);
	int pt = 0;
	while( decoder->DecodeSkip(grp_read_buf, pt, 0) != -1) {
		pt = 0;
	}
	decoder->SetPoint(point);
	// 割り当て
	if (max_read_flag == -1) return;
	if (read_flag_table[seen_no] != -1) {
		return;
	}
	int flag_deal = max_read_flag/32 + 1;
	if (max_read_flag_number + flag_deal > MAX_READ_FLAGS) {
		fprintf(stderr, "Warning in AssignReadFlag in seen %d: too many texts; cannot assign read flag.\n",seen_no);
		return;
	}
	read_flag_table[seen_no] = max_read_flag_number;
	max_read_flag_number += flag_deal;
}

SENARIO::~SENARIO() {
	local_system.freeTimerBase(basetime);
	ClearArd();
	// ヘッダをセーブ
	if (isReadHeader) WriteSaveHeader();
	int i; for (i=0; i<SENARIO_GRPREAD; i++) delete[] grp_read_buf[i];
	if (data_orig) delete[] data_orig;
	if (decoder) delete decoder;
	if (flags) delete flags;
	if (macros) delete macros;
	if (grpsave) delete grpsave;
	if (old_grp_state) delete[] old_grp_state;
	if (name_entry) local_system.CloseNameEntry(name_entry);
}

SENARIO_MACRO::SENARIO_MACRO(void) {
	macro_deal = 32;
	macros = new unsigned char*[macro_deal];
	int i;  for (i=0; i<macro_deal; i++) macros[i] = 0;
}

SENARIO_MACRO::~SENARIO_MACRO() {
	int i;
	for (i=0; i<macro_deal; i++) {
		if (macros[i] != 0) delete[] macros[i];
	}
	delete[] macros;
}

void SENARIO_MACRO::SetMacro(int n, const unsigned char* str) {
	if (n >= macro_deal || n < 0) return;
	if (strlen(str) == 0) return;
	if (macros[n] != 0) delete[] macros[n];
	macros[n] = new unsigned char[strlen((char*)str)+1];
	strcpy((char*)macros[n], (const char*)str);
}

unsigned char* SENARIO_MACRO::DecodeMacro(unsigned char* str, unsigned char* buf)
{
	unsigned char c; unsigned char* ret = buf;
	while( (c=*str) ) {
		if (c < 0x80) {
			*buf++ = *str++;
		} else { // kanji
			if (c == 0x81 &&
				strlen(str) >= 4 &&
				str[1] == 0x96 &&
				str[2] == 0x82 &&
				str[3] >= 0x60 &&
				str[3] < 0x80) { // macro
				c = str[3] - 0x60; str += 4;
				if (macros[c] != 0) {
					strcpy(buf, macros[c]);
					buf += strlen(buf);
				}
			} else {
				*buf++ = *str++;
				if (*str!=0) *buf++ = *str++;
			}
		}
	}
	*buf = 0;
	return ret;
}

void SENARIO_DECODE::DumpData(void) {
	int point = GetPoint();
	printf("start dump ; current point %d(%x)\n",point,point);
	unsigned char* d = data; d -= 0x50;
	if (d < basedata) d = basedata;
	int i; for (i=0; i<20; i++) {
		printf("%6x: ",d-basedata);
		int j; for (j=0; j<16; j++) {
			printf("%02x ",int(*d++)&0xff);
		}
		printf("\n");
	}
}
