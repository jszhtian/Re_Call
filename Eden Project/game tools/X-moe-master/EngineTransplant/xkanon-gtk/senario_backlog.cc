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
#include<unistd.h>
#ifdef HAVE_LIBZ
#include<zlib.h>
#define ZLIB_MAGIC 0x771d9cf7
#endif /* HAVE_LIBZ */

#include "senario_backlog.h"
/* バックログの構造
**	           Pushで次にデータが入る場所    log_orig+BACKLOG_LEN
**	           |データの先頭(log)         データの末尾|
**	log_orig   ||                                    ||
**	v          vv                                    vv
**	|----------|**************************************|
**	(top)                                       (bottom)
**
**  データ内部：（メモリ上では逆順に push していく）
**
** 	BL_END / BL_END2 / BL_END3 / BL_RET / BL_SEL2R: 1byte で 1命令
**	BL_TEXT / BL_SEL2S /  BL_KOE:
**		BL_TEXT <senario_no/short> <senario_point/int> BL_TEXT の１０バイト
**	BL_TEXT_WI / BL_SEL2S_WI: (with information)
**		BL_TEXT_WI <senario_no/short> <senario_point/int> <flag info/int>
**			<graphcis info/int> <music info/int> <stack info/int> <BL_TEXT_WI>
**			合計２４バイト
**	BL_XXX_INF: information などを格納。別の場所から参照される。
**		BL_XXX_INF <len> ... <len> BL_OTHER
**	BL_MSGPOS / BL_MSGSIZ / BL_MOJSIZ :
**		BL_XXX <x> <y> <old-x> <old-y> BL_XXX
**	BL_ISWAKU:
**		BL_ISWAKU <1byte data> <old data> BL_ISWAKU
**	BL_SEL2 / BL_TITLE :
**		BL_SEL2 <len/short> <text> <len/short> BL_SEL2
**	これにより、逆からも読むことができる backlog となる。
*/

/*******************************************************************
**  バックログの処理中に情報をため込むための stack
**  本来なら std::vector<int> で十分だが、STL を使うと
**  だいぶコンパイルが遅くなる（たぶん１．５から２倍
**  の時間になる）ので自作。
*/

/* ローカルなスタック */
const int sb_step = 16;
class SBStack {
	int is_change;
	void Expand(void);
	int n,max; int* stack;
public:
	int IsChange(void) { return is_change; }
	void ClearChange(void) { is_change = 0; }
	void Push(int p) {
		if (n >= max) Expand();
		stack[n++] = p;
		is_change = 1;
	}
	int Pop(void) {
		if (! stack) return -1;
		if (n==0) return -1;
		if (n > 0) n--;
		is_change = 1;
		return stack[n];
	}
	void Clear(void) {
		n = 0;
	}
	int Bottom(void) {
		if (stack && n) return stack[0];
		else return -1;
	}
	int Top(void) {
		if (stack && n) return stack[n-1];
		else return -1;
	}
	int Count(void) {
		return n;
	}
	int operator[](int c) {
		if (stack == 0) return -1;
		if (c >= n) return -1;
		return stack[c];
	}
	SBStack(void);
	~SBStack();
};
SBStack::SBStack(void) {
	max = 0; stack = 0; n = 0;
	is_change = 0;
}
SBStack::~SBStack() {
	if (stack) delete[] stack;
}
void SBStack::Expand(void) {
	int max_new = max + sb_step;
	int* new_stack = new int[max_new];
	if (stack) {
		memcpy(new_stack, stack, sizeof(int)*max);
		delete[] stack;
	}
	max = max_new; stack = new_stack;
}

/* 事実上構造体 */
class SENARIO_BackLogInfo {
	/* 保存される情報 */
	SBStack grp, mus, flag,  stack, sys_info;
public:
	SBStack& Grp(void) { return grp; }
	SBStack& Mus(void) { return mus; }
	SBStack& Flag(void) { return flag; }
	SBStack& Stack(void) { return stack; }
	SBStack& SystemInfo(void) { return sys_info; }
	int IsChange(void) {
		if (grp.IsChange()) return 1;
		if (mus.IsChange()) return 1;
		return 0;
	}
	void Clear(void) {
		grp.Clear();
		mus.Clear();
		stack.Clear();
		flag.Clear();
		sys_info.Clear();
	}
	/* 現在表示されているべきグラフィックス・音楽を出す */
	void Do(SENARIO_BackLog& log);
	/* stack の内容まで巻きもどす */
	void RollBack(SENARIO_BackLog& log);
	SENARIO_BackLogInfo(void) {
	}
};

/* 現在表示されているべきグラフィックス・音楽を出す */
void SENARIO_BackLogInfo::Do(SENARIO_BackLog& log) {
	if (Grp().IsChange()) {
		int point = Grp().Top();
		log.DoGraphics(point);
		Grp().ClearChange();
	}
	if (Mus().IsChange()) {
		int point = Mus().Top();
		log.DoMusic(point);
		Mus().ClearChange();
	}
}

/* stack の内容まで巻きもどす */
void SENARIO_BackLogInfo::RollBack(SENARIO_BackLog& log) {
	/* スタックの復旧 */
	if (Stack().Top() != -1) {
		int point = Stack().Top();
		log.DoStack(point);
	}
	/* フラグの復旧 */
	if (Flag().Count() != 0) {
		int i; int len = Flag().Count();
		for (i=0; i<len; i++)
			log.DoFlag(Flag()[i]);
	}
	/* システム設定の復旧 */
	if (SystemInfo().Count() != 0) {
		int i; int len = SystemInfo().Count();
		for (i=0; i<len; i++)
			log.DoSystem(SystemInfo()[i]);
	}
	/* ログの開始；音楽・画像も復旧 */
	log.StartLog(1);
	return;
}

/*******************************************************************
**
** ログの雑処理
*/

inline int SENARIO_BackLog::PopCmd(void) {
	return *log++;
}
inline int SENARIO_BackLog::GetCmd(void) {
	return *log;
}
inline void SENARIO_BackLog::PushCmd(int cmd) {
	*--log = cmd;
}

inline char SENARIO_BackLog::PopByte(void) {
	return *log++;
}
inline void SENARIO_BackLog::PushByte(char n) {
	*--log = n;
}

inline int SENARIO_BackLog::PopInt(void) {
	log += 4;
	return read_little_endian_int(log-4);
}
inline void SENARIO_BackLog::PushInt(int n) {
	log -= 4;
	write_little_endian_int(log, n);
}

inline short SENARIO_BackLog::PopShort(void) {
	log += 2;
	return read_little_endian_short(log-2);
}
inline void SENARIO_BackLog::PushShort(short n) {
	log -= 2;
	write_little_endian_short(log, n);
}

inline void SENARIO_BackLog::PushStr(const char* s) {
	int len = strlen(s);
	log -= len;
	memcpy(log, s, len);
}
inline void SENARIO_BackLog::PopStr(char* buf, int buf_len, int str_len) {
	buf_len--;
	if (str_len < buf_len) buf_len = str_len;
	memcpy(buf, log, buf_len); buf[buf_len] = 0;
	log += str_len;
}
inline void SENARIO_BackLog::PushStrZ(const char* s) {
	int len = strlen(s);
	*--log = '\0';
	log -= len; memcpy(log, s, len);
}
inline void SENARIO_BackLog::PopStrZ(char* buf, unsigned int buf_len) {
	buf_len--;
	if (buf_len > strlen(log)) buf_len = strlen(log);
	strncpy(buf, log, buf_len); buf[buf_len] = '\0';
	log += strlen(log)+1;
}
inline char* SENARIO_BackLog::PushBuffer(int len) {
	log -= len;
	return log;
}
inline char* SENARIO_BackLog::PopBuffer(int len) {
	char* ret = log;
	log += len;
	return ret;
}

/* 命令を一つ pop し、その内容を無視する */
inline void SENARIO_BackLog::PopSkip(void) {
	if (log >= log_orig+BACKLOG_LEN-5) {
		log = log_orig + BACKLOG_LEN;
		return;
	}
	int cmd = PopCmd();
	int len = bl_len[cmd];
	if (len == -1) len = PopShort() - 2;
	log += len-1;
}
/* 一つ先の命令に移る */
inline void SENARIO_BackLog::NextSkip(void) {
	int cmd = log[-1];
	int len = bl_len[cmd];
	if (len == -1) len = read_little_endian_short(log-3);
	log -= len;
	return;
}
/* ログの範囲チェック（バックログ実行時） */
inline int SENARIO_BackLog::CheckNextCmd(void) {
	if (log <= exec_log_current) return 1;
	return 0;
}
inline int SENARIO_BackLog::CheckPrevCmd(void) {
	if (log >= log_orig+BACKLOG_LEN-5) return 1;
	int cmd = GetCmd();
	int len = bl_len[cmd];
	if (len == -1) {
		int pt = GetPoint(); PopCmd(); len = PopShort(); SetPoint(pt);
	}
	if (log+len >= log_orig+BACKLOG_LEN) return 1;
	return 0;
}

/* CheckLogLen() からのみ呼び出されるので範囲チェック無し */
void SENARIO_BackLog::CutLog(void) {
	log += LOG_DELETE_LEN;
	bottom_point += LOG_DELETE_LEN;
	memmove(log_orig+LOG_DELETE_LEN, log_orig, BACKLOG_LEN-LOG_DELETE_LEN);
	return;
}

void SENARIO_BackLog::CutLog(unsigned int cut_len) {
	/* LOG_DELETE_LEN の倍数で規格化 */
	cut_len = (cut_len/LOG_DELETE_LEN)*LOG_DELETE_LEN + LOG_DELETE_LEN;
	if (cut_len >= BACKLOG_LEN) {
		/* エラー：log は初期化する */
		fprintf(stderr, "SENARIO_BackLog::Cutlog: too long cut !!"
			"cut len %d is larger than log size %d\n",
			cut_len, BACKLOG_LEN);
		ClearLog();
		return;
	}
	log += cut_len;
	bottom_point += cut_len;
	memmove(log_orig+cut_len, log_orig, BACKLOG_LEN-cut_len);
}

/* ここでも log の direction が問題になるので注意 */
void SENARIO_BackLog::CutHash(void) {
	if (grp_info_len == 0) return;
	int point = GetPoint();
	int i;
	for (i=grp_info_len-1; i>=0; i--) {
		if (grp_info[i] < point) {
			grp_info_len = i+1; return;
		}
	}
	grp_info_len = 0;
	return;
}

/* セーブされたログのリストア */
void SENARIO_BackLog::SetLog(char* saved_log, unsigned int save_log_len, int is_save) {
	if (save_log_len < 8+sizeof(grp_info_orig)) {ClearLog(); return;}
	int log_point = read_little_endian_int(saved_log); saved_log += 4; save_log_len -= 4;
	memcpy(grp_info_orig, saved_log, sizeof(grp_info_orig));
	saved_log += sizeof(grp_info_orig); save_log_len -= sizeof(grp_info_orig);
	save_log_len = read_little_endian_int(saved_log); saved_log += 4;

#ifdef HAVE_LIBZ
	if (read_little_endian_int(saved_log) == ZLIB_MAGIC) {
		saved_log += 4; save_log_len -= 4;
		int uncompress_size = read_little_endian_int(saved_log);
		saved_log += 4; save_log_len -= 4;
		if (uncompress_size > BACKLOG_LEN) uncompress_size = BACKLOG_LEN-LOG_DELETE_LEN;

		/* zs の初期化 */
		z_stream zs;
		zs.zalloc = Z_NULL; zs.zfree = Z_NULL; zs.opaque = Z_NULL;
		if (inflateInit(&zs) != Z_OK) {
			fprintf(stderr,"SENARIO_BackLog::SetLog: zlib error : %s\n",zs.msg);
			fprintf(stderr,"sorry, clear back log and restart.\n");
			ClearLog();
			return;
		}
		zs.next_in = (Bytef*)saved_log;
		zs.avail_in = save_log_len;
		zs.next_out = (Bytef*)log_orig + BACKLOG_LEN - uncompress_size;
		log = log_orig + BACKLOG_LEN - uncompress_size;
		zs.avail_out = uncompress_size;
		inflate(&zs, Z_SYNC_FLUSH);
		if (zs.avail_out != 0) {
			/* データが余った場合、のこりは BL_END で埋めておく */
			fprintf(stderr, "SENAIRO_BackLog::SetLog: log size is too small; rest size is %d\n",zs.avail_out);
			memset(log_orig+BACKLOG_LEN-zs.avail_out, BL_END, zs.avail_out);
		}
		inflateEnd(&zs);
		bottom_point = log_point - uncompress_size;
#else /* NO LIBZ */
	if (0) {
#endif /* HAVE_LIBZ */
	} else {
		if (save_log_len > BACKLOG_LEN) save_log_len = BACKLOG_LEN-LOG_DELETE_LEN;
		log = log_orig + (BACKLOG_LEN-save_log_len);
		memcpy(log, saved_log, save_log_len);
		bottom_point = log_point - save_log_len;
	}
	/* ログの有効性確認 */
	int cur_point = GetPoint();
	while(CheckPrevCmd() == 0) {
		int cmd = GetCmd();
		if (cmd <= 0 || cmd > BL_MAX) { ClearLog(); return;}
		if (bl_len[cmd] == -1) {
			int p = GetPoint();
			PopCmd(); int l = PopShort();
			SetPoint(p);
			if (l <= 0) { ClearLog(); return;}
		}
		PopSkip();
	}
	SetPoint(cur_point);
	/* リストア後、SetSavePoint() を実行したコマンドまでスキップする */
	/* (LOAD 後、同じコマンドが実行されるため) */
	if (is_save) {
		while(CheckPrevCmd() == 0) {
			int cmd = GetCmd();
			if (cmd == BL_TEXT || cmd == BL_SEL2S || cmd == BL_SEL1S || cmd == BL_SAVEPT) {
				PopSkip(); break;
			} else if (cmd == BL_SYSORIG_INF) {
				DoSysOrig();
			} else {
				PopSkip();
			}
		}
	}
	return;
}
/* ログをセーブ */
void SENARIO_BackLog::PutLog(char* save_log, unsigned int save_log_len, int is_save) {
	/* top_point = log_orig の場所の pointer
	** log_point = log の場所の pointer
	*/
	int top_point = bottom_point + BACKLOG_LEN;
	int log_point = top_point - (log - log_orig);
	unsigned int log_len = BACKLOG_LEN - (log-log_orig);

	if (save_log_len < 8+sizeof(grp_info_orig)) { return;}
	write_little_endian_int(save_log, log_point);
	save_log += 4; save_log_len -= 4;
	memcpy(save_log, grp_info_orig, sizeof(grp_info_orig));
	save_log += sizeof(grp_info_orig); save_log_len -= sizeof(grp_info_orig); save_log_len-=4;
#ifdef HAVE_LIBZ
	if (is_save && save_log_len > 12) {
		/* zs の初期化 */
		z_stream zs;
		zs.zalloc = Z_NULL; zs.zfree = Z_NULL; zs.opaque = Z_NULL;
		if (deflateInit(&zs, Z_DEFAULT_COMPRESSION) != Z_OK) {
			fprintf(stderr,"SENARIO_BackLog::PutLog: zlib error : %s\n",zs.msg);
			goto no_compress;
		}
		zs.next_in = (Bytef*)log;
		zs.avail_in = log_len;
		zs.next_out = (Bytef*)save_log+12;
		zs.avail_out = save_log_len-12;
		deflate(&zs, Z_FULL_FLUSH);
		/* 保存したサイズを出す */
		log_len -= zs.avail_in;
		save_log_len -= zs.avail_out;
		deflateEnd(&zs);
		/* ヘッダの保存 */
		write_little_endian_int(save_log, save_log_len);
		write_little_endian_int(save_log+4, ZLIB_MAGIC);
		write_little_endian_int(save_log+8, log_len);
#else /* NO LIBZ */
	if (0) {
#endif /* HAVE_LIBZ */
	} else {
	no_compress:
		if (save_log_len > log_len) save_log_len = log_len;
		write_little_endian_int(save_log, save_log_len); save_log += 4;
		memcpy(save_log, log, save_log_len);
	}
	return;
}

void SENARIO_BackLog::ClearLog(void) {
	log = log_orig + BACKLOG_LEN;
	bottom_point = 0;
	old_track = -1; old_grp_point = -1;
	grp_info_len = 0;
	AddEnd(); /* １コマンド登録しておく */
}
/*******************************************************************
**
** バックログの処理
*/

/* ログのtopは TEXT, TEXT_KOE, SEL2S, SEL1S, TITLE のいずれかであること */
/* backlog の top にある命令の seen / point を得る */
GlobalStackItem SENARIO_BackLog::GetSenarioPoint(void) {
	GlobalStackItem ret_item;
	int cmd = GetCmd(); int cur_point = GetPoint();
	int seen,point;
	switch(cmd) {
		case BL_TEXT: case  BL_SEL2S: case BL_SEL1S:
			PopCmd();
			point = PopInt(); seen = PopShort();
			ret_item.SetGlobal(seen,point);
			break;
		case BL_TITLE:
			PopCmd(); PopShort(); // length を pop
			point = PopInt(); seen = PopShort();
			ret_item.SetGlobal(seen,point);
			break;
	}
	SetPoint(cur_point);
	return ret_item;
}
/* ログのtopは TEXT, TEXT_KOE, SEL2S, SEL1S, TITLE のいずれかであること */
/* backlog の top にある命令のテキストを得る */
/* TITLE なら "\0" が返る */
void SENARIO_BackLog::GetText(char* ret_str, unsigned int str_len, int* koebuf, int koebuf_len) {
	int point = GetPoint();
	int cmd = GetCmd();
	ret_str[0] = '\0';
	int koecount = 0;
	if (cmd == BL_SEL2S || cmd == BL_SEL1S) {
		/* 選択肢 */
		char* sel_top[10]; int sel_count = 0;
		while(CheckNextCmd() == 0) {
			NextSkip();
			int cmd = GetCmd();
			/* 選択肢：ret_str に代入 */
			if ( (cmd == BL_SEL2 || cmd == BL_SEL1S)&& sel_count < 9) {
				int local_point = GetPoint();
				PopCmd(); unsigned int len = PopShort() - 6;
				if (str_len > len+10) {
					sel_top[sel_count++] = ret_str;
					ret_str[0] = 0x81;ret_str[1] = 0x40; /* "　" of SJIS */
					ret_str += 2; str_len -= 2;
					PopStr(ret_str, str_len, len); ret_str[len] = '\n';
					str_len -= len+1; ret_str += len+1;
				}
				SetPoint(local_point);
			/* 選択肢終了：選択肢に◎をつけて終了 */
			} else if (cmd == BL_SEL2R) {
				PopCmd(); int result = PopInt();
				if (result >= 0 && result < sel_count) {
					/* SJIS, 0x819d == '◎' */
					/* やっぱりやめ */
					/* sel_top[result][0] = 0x81;
					** sel_top[result][1] = 0x9d;
					*/
				}
				break;
			} else {
				break; /* 終了 */
			}
		}
	} else if (cmd == BL_TEXT) {
		/* テキスト。TEXT, RET が続く間、読み続ける */
		while(CheckNextCmd() == 0) {
			int cmd = GetCmd();
			if (cmd == BL_TEXT) {
				/* テキストをシナリオファイルから読み込む */
				int local_point = GetPoint();
				PopCmd();
				int slocal = PopInt(); int seen = PopShort();
				GlobalStackItem item; item.SetGlobal(seen, slocal);
				int koe = -1;
				decoder.GetText(ret_str, str_len, &koe, item);
				if (koe != -1 && koecount < koebuf_len-1 && koebuf) koebuf[koecount++] = koe;
				str_len -= strlen(ret_str); ret_str += strlen(ret_str);
				SetPoint(local_point);
			} else if (cmd == BL_RET) {
				/* return を付ける */
				if (str_len > 1) {
					*ret_str++ = '\n';
					str_len--;
				}
			} else {
				break;
			}
			NextSkip();
		}
	} else if (cmd == BL_TITLE) {
		/* タイトル */
		/* 無視
		** char buf[1024];
		** PopCmd(); PopShort(); PopShort(); PopInt();
		** PopStrZ(buf,1024); PopStrZ(buf, 1024);
		** if (strlen(buf) < str_len) {
		** 	strcpy(ret_str, buf);
		** 	ret_str += strlen(buf);
		** 	str_len -= strlen(buf);
		**}
		*/
	}
	SetPoint(point);
	*ret_str = '\0';
	if (koebuf) {
		if (koebuf_len <= 0) ;
		else if (koecount < koebuf_len-1) koebuf[koecount] = -1;
		else koebuf[koebuf_len-1] = -1;
	}
	return;
}
void SENARIO_BackLog::ResumeOldText() {
	/* バックログの前の部分にあったテキストを再現する */
	int point = GetPoint();
	int cmd_end;
	/* NVL_SYSTEM なら前の BL_END2 まで全て再現 */
	if (local_system.config->GetParaInt("#NVL_SYSTEM")) {
		cmd_end = BL_END2;
	} else {
		cmd_end = BL_END;
	}
	char text_buf[1024]; char* text_ptr = text_buf; int text_len=0; int skip_count = 0;
	/* END が見つかるまでスキップ */
	while(CheckPrevCmd() == 0) {
		if (GetCmd() == cmd_end) break;
		if (GetCmd() == BL_SEL2S || GetCmd() == BL_SEL1S ||
			GetCmd() == BL_SEL2R || GetCmd() == BL_SEL2) break;
		PopSkip();
		skip_count++;
	}
	/* 全てのテキストを得る */
	int i; int skip_flag = 0;
	for (i=0; i<skip_count; i++) {
		NextSkip();
		if (bl_istext[GetCmd()]) {
			if (skip_flag == 0) {
				GetText(text_ptr, 1000-text_len, 0, 0);
				text_len += strlen(text_ptr);
				text_ptr += strlen(text_ptr);
				skip_flag = 1;
			}
		} else skip_flag = 0;
		if (GetCmd() == BL_END) {
			if (text_len < 1000) *text_ptr++ = '\n';
		}
	}
	SetPoint(point);
	if (text_len != 0) {
		*text_ptr = '\0';
		local_system.SetDrawedText(text_buf);
	} else {
		local_system.SetDrawedText("");
	}
}

/* ログにテキストが含まれているかチェック
** 含まれてないなら -1 を返す
** 含まれていれば 0 を返す
*/
int SENARIO_BackLog::CheckLogValid(void) {
	int point = GetPoint();
	while(CheckPrevCmd() == 0) {
		int cmd = GetCmd();
		if (bl_istext[cmd]) break;
		PopSkip();
	}
	if (CheckPrevCmd()) {
		SetPoint(point);
		return -1;
	}
	SetPoint(point);
	return 0;
}
void SENARIO_BackLog::GetInfo(int& grp, int& mus) {
	grp = -1; mus = -1;
	int point = GetPoint();
	while( (grp==-1 || mus==-1) && CheckPrevCmd() == 0) {
		int cmd = GetCmd();
		if (cmd == BL_MUS_INF && mus == -1)
			mus = GetPoint();
		else if ( (cmd == BL_GRP_INF || cmd == BL_GRP_INF2) && grp == -1)
			grp = GetPoint();
		PopSkip();
	}
	SetPoint(point);
	return;
}
void SENARIO_BackLog::GetSavePoint(int& seen, int& point, SENARIO_FLAGS** ret_flag, GosubStack** ret_stack) {
	seen=-1; point=-1;
	int cur_point = GetPoint();
	while(CheckPrevCmd() == 0) {
		int cmd = GetCmd();
		if (cmd == BL_TEXT || cmd == BL_SEL2S || cmd == BL_SEL1S || cmd == BL_SAVEPT) {
			PopCmd();
			point = PopInt();
			seen = PopShort();
			if (ret_flag) *ret_flag = old_flags;
			if (ret_stack) *ret_stack = &old_stack;
			break;
		}
		PopSkip();
	}
	SetPoint(cur_point);
	return;
}

/* 次の text までスキップ */
/* スキップの結果、最後まで読んでしまったら -1 が返る */
int SENARIO_BackLog::SkipNewMessage(void) {
	if (CheckPrevCmd() == 0 && GetCmd() == BL_TITLE) {
		int cur_point = GetPoint();
		(this->*(bl_do_new[BL_TITLE]))(local_system);
		SetPoint(cur_point);
	}
	while(CheckNextCmd() == 0) {
		NextSkip();
		int cmd = GetCmd();
		if (bl_istext[cmd]) return 0;
		if (bl_do_new[cmd]) {
			int cur_point = GetPoint();
			(this->*(bl_do_new[cmd]))(local_system);
			SetPoint(cur_point);
		}
	}
	/* 最初まで行ってしまった：一つ前のメッセージに戻る */
	if (CheckLogValid()) return -1;
	SkipOldMessage();
	return -1;
}
/* 前の text までスキップ */
/* スキップの結果、最初までよんでしまったら -1 が返る */
int SENARIO_BackLog::SkipOldMessage() {
	if (CheckPrevCmd() == 0 && GetCmd() == BL_TITLE) {
		int cur_point = GetPoint();
		(this->*(bl_do_old[BL_TITLE]))(local_system);
		SetPoint(cur_point);
	}
	while(CheckPrevCmd() == 0) {
		PopSkip();
		int cmd = GetCmd();
		if (bl_istext[cmd]) {
			if (CheckPrevCmd()) return 0;
			while(CheckPrevCmd() == 0) {
				PopSkip();
				if (! bl_istext[GetCmd()]) break;
			}
			NextSkip();
			return 0;
		}
		if (bl_do_old[cmd]) {
			int cur_point = GetPoint();
			(this->*(bl_do_old[cmd]))(local_system);
			SetPoint(cur_point);
		}
	}
	/* 最後まで行ってしまった：一つ前のメッセージに戻る */
	SkipNewMessage();
	return -1;
}

/* ログの最初まで読んだら -1 が返る */
/* 通常終了なら 0 が帰り、選択肢やSetTitleなどで終了なら残りカウントが返る */
/* skip flag は bit0 が 1 で選択肢終了、bit1 が 1 で title 終了 bit2 が 1 で count=infinity*/
int SENARIO_BackLog::SkipNewMessages(int count, int skip_flag) {
	for (; count>0; count--) {
		if (SkipNewMessage()) return -1;
		int cmd = GetCmd();
		if ( (skip_flag & 1) && cmd == BL_SEL2S) return count;
		if ( (skip_flag & 1) && cmd == BL_SEL1S) return count;
		if ( (skip_flag & 2) && cmd == BL_TITLE) return count;
		if ( skip_flag & 4) count=2;
	}
	return 0;
}
/* ログの最後まで読んだら -1 が帰る */
/* 通常終了なら 0 が帰り、選択肢やSetTitleで終了なら残りカウントが返る */
int SENARIO_BackLog::SkipOldMessages(int count, int skip_flag) {
	for (; count>0; count--) {
		if (SkipOldMessage()) return -1;
		int cmd = GetCmd();
		if ( (skip_flag & 1) && cmd == BL_SEL2S) return count;
		if ( (skip_flag & 1) && cmd == BL_SEL1S) return count;
		if ( (skip_flag & 2) && cmd == BL_TITLE) return count;
		if ( skip_flag & 4) count=2;
	}
	return 0;
}
/*******************************************************************
**
** ログの追加処理・実行処理
*/

void SENARIO_BackLog::AddEnd(void) {
	CheckLogLen();
	PushCmd(BL_END);
}
void SENARIO_BackLog::AddRet(void) {
	CheckLogLen();
	PushCmd(BL_RET);
}
void SENARIO_BackLog::AddEnd2(void) {
	CheckLogLen();
	PushCmd(BL_END2);
}
void SENARIO_BackLog::AddEnd3(void) {
	CheckLogLen();
	PushCmd(BL_END3);
}
void SENARIO_BackLog::AddSavePoint(int p, int seen) {
	if (IsDirty()) AddWI();
	CheckLogLen();
	PushCmd(BL_SAVEPT);
	PushShort(seen);
	PushInt(p);
	PushCmd(BL_SAVEPT);
}
void SENARIO_BackLog::AddSelect2Result(int n) {
	CheckLogLen();
	PushCmd(BL_SEL2R);
	PushInt(n);
	PushCmd(BL_SEL2R);
}
void SENARIO_BackLog::AddText(int p, int seen, int is_sel) {
	static const int cmds[4] = {BL_TEXT, BL_SEL2S, BL_SEL1S, BL_KOE};
	if (is_sel == 0 && GetCmd() == BL_KOE) { /* 前が音声発生：BL_TEXT の場所変換 */
		PopCmd();
		int koe_p = PopInt();
		int koe_seen = PopShort();
		PopCmd();
		if (koe_seen == seen && koe_p < p && koe_p > (p-50)) {
			/* シナリオ番号が同じで場所が近い場合のみ、テキストと同じ声と認識 */
			p = koe_p;
			seen = koe_seen;
		}
	}
	if (IsDirty()) AddWI();
	int cmd = cmds[is_sel];
	CheckLogLen();
	PushCmd(cmd);
	PushShort(seen);
	PushInt(p);
	PushCmd(cmd);
}

void SENARIO_BackLog::AddSelect2(char* str) {
	CheckLogLen();
	int log_len = 2 + 2*2 + strlen(str);
	CheckLogLen(log_len);
	PushCmd(BL_SEL2);
	PushShort(log_len);
	PushStr(str);
	PushShort(log_len);
	PushCmd(BL_SEL2);
}

/* obsolete code */
void SENARIO_BackLog::DoMsgPos2Old(AyuSys& local_system) {
	PopCmd();
	int type = PopByte();
	int y = PopInt();
	int x = PopInt();
	PopInt(); PopInt();
	PopCmd();
	switch(type) {
		case 1: local_system.config->SetParam("#WINDOW_MSG_POS",2,x,y); break;
		case 2: local_system.config->SetParam("#WINDOW_COM_POS",2,x,y); break;
		case 3: local_system.config->SetParam("#WINDOW_SYS_POS",2,x,y); break;
		case 4: local_system.config->SetParam("#WINDOW_SUB_POS",2,x,y); break;
		case 5: local_system.config->SetParam("#WINDOW_GRP_POS",2,x,y); break;
	}
}
void SENARIO_BackLog::DoMsgPos2New(AyuSys& local_system) {
	PopCmd();
	int type = PopByte();
	PopInt(); PopInt();
	int y = PopInt();
	int x = PopInt();
	PopCmd();
	switch(type) {
		case 1: local_system.config->SetParam("#WINDOW_MSG_POS",2,x,y); break;
		case 2: local_system.config->SetParam("#WINDOW_COM_POS",2,x,y); break;
		case 3: local_system.config->SetParam("#WINDOW_SYS_POS",2,x,y); break;
		case 4: local_system.config->SetParam("#WINDOW_SUB_POS",2,x,y); break;
		case 5: local_system.config->SetParam("#WINDOW_GRP_POS",2,x,y); break;
	}
}
void SENARIO_BackLog::DoMsgPosOld(AyuSys& local_system) {
	PopCmd();
	int y = PopInt();
	int x = PopInt();
	local_system.config->SetParam("#WINDOW_MSG_POS",2,x,y);
	PopInt(); PopInt();
	PopCmd();
}
void SENARIO_BackLog::DoMsgPosNew(AyuSys& local_system) {
	PopCmd();
	PopInt(); PopInt();
	int y = PopInt();
	int x = PopInt();
	local_system.config->SetParam("#WINDOW_MSG_POS",2,x,y);
	PopCmd();
}
void SENARIO_BackLog::DoMsgSizeOld(AyuSys& local_system) {
	PopCmd();
	int y = PopInt();
	int x = PopInt();
	PopInt(); PopInt();
	local_system.config->SetParam("#MESSAGE_SIZE",2,x,y);
	PopCmd();
}
void SENARIO_BackLog::DoMsgSizeNew(AyuSys& local_system) {
	PopCmd();
	PopInt(); PopInt();
	int y = PopInt();
	int x = PopInt();
	local_system.config->SetParam("#MESSAGE_SIZE",2,x,y);
	PopCmd();
}
void SENARIO_BackLog::DoMojiSizeOld(AyuSys& local_system) {
	PopCmd();
	int y = PopInt();
	int x = PopInt();
	PopInt(); PopInt();
	local_system.config->SetParam("#MSG_MOJI_SIZE",2,x,y);
	PopCmd();
}
void SENARIO_BackLog::DoMojiSizeNew(AyuSys& local_system) {
	PopCmd();
	PopInt(); PopInt();
	int y = PopInt();
	int x = PopInt();
	local_system.config->SetParam("#MSG_MOJI_SIZE",2,x,y);
	PopCmd();
}
void SENARIO_BackLog::DoIsWakuOld(AyuSys& local_system) {
	PopCmd();
	int w = PopByte();
	local_system.config->SetParam("#NVL_SYSTEM",1,w);
	PopByte();
	PopCmd();
}
void SENARIO_BackLog::DoIsWakuNew(AyuSys& local_system) {
	PopCmd();
	int w = PopByte();
	local_system.config->SetParam("#NVL_SYSTEM",1,w);
	PopByte();
	PopCmd();
}

void SENARIO_BackLog::AddWI(void) {
	local_system.SyncMusicState();
	if (! IsDirty()) return;
	SetGraphicsInfo();
	SetMusicInfo();
	SetFlagInfo();
	SetStackInfo();
	SetSystemInfo();
	return;
}
/* WI で作られる情報の実行：stack に保存 */
void SENARIO_BackLog::DoStackInfNew(AyuSys& local_system) {
	backlog_info->Stack().Pop();
	PopSkip();
}
void SENARIO_BackLog::DoStackInfOld(AyuSys& local_system) {
	backlog_info->Stack().Push(GetPoint());
	PopSkip();
}

void SENARIO_BackLog::DoFlagInfNew(AyuSys& local_system) {
	backlog_info->Flag().Pop();
	PopSkip();
}
void SENARIO_BackLog::DoFlagInfOld(AyuSys& local_system) {
	backlog_info->Flag().Push(GetPoint());
	PopSkip();
}

void SENARIO_BackLog::DoMusInfNew(AyuSys& local_system) {
	backlog_info->Mus().Pop();
	PopSkip();
}
void SENARIO_BackLog::DoMusInfOld(AyuSys& local_system) {
	backlog_info->Mus().Push(GetPoint());
	PopSkip();
}

void SENARIO_BackLog::DoGrpInfNew(AyuSys& local_system) {
	backlog_info->Grp().Pop();
	PopSkip();
}
void SENARIO_BackLog::DoGrpInfOld(AyuSys& local_system) {
	backlog_info->Grp().Push(GetPoint());
	PopSkip();
}

void SENARIO_BackLog::DoSysInfNew(AyuSys& local_system) {
	backlog_info->SystemInfo().Pop();
	PopSkip();
}
void SENARIO_BackLog::DoSysInfOld(AyuSys& local_system) {
	backlog_info->SystemInfo().Push(GetPoint());
	PopSkip();
}


void SENARIO_BackLog::AddSetTitle(char* new_title, int point, int seen) {
	CheckLogLen();
	if (IsDirty()) AddWI();
	char* old_title = local_system.GetTitle();
	int log_len = 2 + 2*2 + 2+4 + strlen(new_title)+1 + strlen(old_title)+1;
	CheckLogLen(log_len);
	PushCmd(BL_TITLE);
	PushShort(log_len);
	PushStrZ(new_title);
	PushStrZ(old_title);
	PushShort(seen);
	PushInt(point);
	PushShort(log_len);
	PushCmd(BL_TITLE);
}
void SENARIO_BackLog::DoTitleOld(AyuSys& local_system) {
	char buf[1024];
	PopCmd();
	PopShort();
	PopShort(); PopInt();
	PopStrZ(buf,1024); local_system.SetTitle(buf);
	PopStrZ(buf,1024);
	PopShort(); PopCmd();
}
void SENARIO_BackLog::DoTitleNew(AyuSys& local_system) {
	char buf[1024];
	PopCmd();
	PopShort();
	PopShort(); PopInt();
	PopStrZ(buf,1024);
	PopStrZ(buf,1024); local_system.SetTitle(buf);
	PopShort(); PopCmd();
}

/* backlog mode に入る前の処理：
**   最初のテキストまで log を巻き戻す
**   また、stack や flag, graphics なども巻き戻す
*/

/* old_flags / old_stack / old_track / old_grp_point
** の内容を復旧する
*/
void SENARIO_BackLog::RestoreState(void) {
	/* log の復旧 */
	/* exec_log_current はメッセージを指しているはず */
	log = exec_log_current;
	
	/* フラグ、スタックのコピー */
	if (local_system.CallStack().IsDirty()) {
		int stack_deal = old_stack.CurStackDeal();
		local_system.CallStack().InitStack();
		int i; for (i=0;i<stack_deal;i++)
			local_system.CallStack().PushStack() = old_stack[i];
		local_system.CallStack().ClearDirty();
	}
	if (decoder.Flags().IsDirty()) {
		decoder.Flags().Copy(*old_flags);
		decoder.Flags().ClearDirty();
	}
	/* 画面の復旧 */
	if (decoder.GrpSave().IsChange()) {
		DoGraphicsNew(old_grp_point);
		decoder.GrpSave().ClearChange();
	}
	/* 音楽の復旧 */
	if (local_system.IsTrackChange()) {
		DoMusicNew(old_track);
		local_system.ClearTrackChange();
	}
}
void SENARIO_BackLog::StartLog(int is_save) {
	int grp, mus;
	GetInfo(grp,mus);
	/* flag を保存 */
	old_flags->Copy(decoder.Flags());
	decoder.Flags().ClearDirty();
	/* stack の copy */
	old_stack.InitStack(); GosubStack& stack = local_system.CallStack();
	int deal = stack.CurStackDeal();
	int i; for (i=0; i<deal; i++) {
		old_stack.PushStack()=stack[i];
	}
	stack.ClearDirty();
	if (is_save) {
		/* graphics の取り出し */
		if (grp != -1) {
			DoGraphicsNew(grp);
			decoder.GrpSave().ClearChange();
		}
		if (mus != -1) {
			DoMusicNew(mus);
			local_system.ClearTrackChange();
		}
		/* 一応、マウスカーソルを表示しておく */
		local_system.DrawMouse();
	}
	old_track = mus; old_grp_point = grp;
	/* 不要になった grp info は消す */
	CutHash();
	/* テキストの回復 */
	if (is_save) {
		ResumeOldText();
	}
}
SENARIO_BackLog::~SENARIO_BackLog() {
	delete old_flags;
	delete[] log_orig;
	delete backlog_info;
}


GlobalStackItem SENARIO_BackLog::View(void) {
#if 0
	/* ログの出力 */
	int i; char buf[1024];
	for (i=0; i<1000; i++) {
		sprintf(buf,"backlog%05d",i);
		FILE* f = fopen(buf,"rb");
		if (f==NULL) break;
		fclose(f);
	}
	if (i == 1000) return GlobalStackItem();
	sprintf(buf,"backlog%05d",i);
	FILE* f = fopen(buf, "wb");
	fprintf(stderr, "log point %d, bottom %d\n",GetPoint(),bottom_point);
	char* tmp_buf = new char[BACKLOG_LEN+100];
	PutLog(tmp_buf,BACKLOG_LEN+100);
	fwrite(tmp_buf,BACKLOG_LEN+100,1,f);
	delete[] tmp_buf;
	fclose(f);
#endif

	int backlog_count = local_system.GetBacklog();
	local_system.ClearIntterupt();

	/* バックログ表示用の変数初期化 */
	exec_log_current = log;
	backlog_info->Clear();

	/* ログが無効ならなにもしない */
	/* その後、一番最近のメッセージまで巻き戻す */
	/* Skip が終わったところに exec_log_current を置く */
	if (CheckLogValid()) return GlobalStackItem();
	if (backlog_count != -1) {
		/* シナリオ巻き戻し */
		int skip = 0;
		if (backlog_count == -3) { /* 前の選択肢まで */
			skip = 1 | 4;
			backlog_count = 2;
		} else if (backlog_count == -4) { /* 前のタイトルまで */
			skip = 2 | 4;
			backlog_count = 2;
			/* 現在のタイトルの頭まで読み進めておく */
			/* SkipOldMessages(backlog_count, skip); */
		} else if (backlog_count > 0) {
			skip = 0; backlog_count--;
		}
		SkipOldMessage();
		SkipOldMessages(backlog_count, skip);
		if (CheckPrevCmd() == 0 && GetCmd() == BL_TITLE) {
			int cur_point = GetPoint();
			(this->*(bl_do_old[BL_TITLE]))(local_system);
			SetPoint(cur_point);
		}
		local_system.SetBacklog(-2); // 巻き戻しの場所から再開
	} else {
		SkipOldMessage();
		exec_log_current = log;
		/* 状態をバックログの最後の登録時に戻す */
		RestoreState();
		int valid_to_old = 1, valid_to_new = 0;
		int is_draw = 1;/* テキストを書き直す必要があるか？ */
		int koebuf[10];

		/* バックログモードのメインループ */
		while(! local_system.IsIntterupted() &&
			(local_system.GetBacklog() <= 0 &&
			 local_system.GetBacklog() >= -2) ) {
			/* 割り込み：一定数のメッセージ読み飛ばし */
			if (local_system.IsIntterupted()) {
				backlog_count = local_system.GetBacklog();
				local_system.ClearIntterupt();
				int dir = 0; /* 前と後、どちらにスキップか？ */
				int count = 0; /* スキップの数 */
				int flag = 0; /* スキップのモード */
				if (backlog_count == -3) { /* 前の選択肢 */
					count = 1; dir = 0; flag = 1 | 4;
				} else if (backlog_count == -4) { /* 前のタイトル */
					count = 1; dir = 0; flag = 2 | 4;
				} else if (backlog_count == -5) { /* 次の選択肢 */
					count = 1; dir = 1; flag = 1 | 4;
				} else if (backlog_count == -6) { /* 次のタイトル */
					count = 1; dir = 1; flag = 2 | 4;
				} else if (backlog_count > 0) {
					if (backlog_count > 10000) {
						dir = 1;
						count = backlog_count - 10000;
						flag = 1 | 2;
					} else {
						dir = 0;
						count = backlog_count;
						flag = 1 | 2;
					}
				} else continue; // 本当の intterupt
				if (dir) {
					valid_to_old = 1;
					if (SkipNewMessages(count,flag) == -1) valid_to_new = 0;
				} else {
					valid_to_new = 1;
					if (SkipOldMessages(count,flag) == -1) valid_to_old = 0;
				}
				is_draw = 1;
			} else {
				int l,r,u,d,e;
				/* カーソルの操作 */
				local_system.GetKeyCursorInfo(l,r,u,d,e);
				/* マウスの状態を得る */
				int x,y,f;
				local_system.GetMouseInfoWithClear(x,y,f);
				if (e) {
					local_system.SetBacklog(-1); /* backlog 終了 */
					continue;
				} else if (l) {
					local_system.SetBacklog(11); /* 10 message 巻き戻し */
					continue;
				} else if (r) {
					local_system.SetBacklog(10 + 10000); /* 10 message 後へ */
					continue;
				} else if (u && valid_to_old) { /* 前のメッセージへ */
					valid_to_new = 1;
					if (SkipOldMessage() == -1) valid_to_old = 0;
					is_draw = 1;
				} else if (d && valid_to_new) { /* 次のメッセージへ */
					valid_to_old = 1;
					if (SkipNewMessage() == -1) valid_to_new = 0;
					is_draw = 1;
				}
			}
			if (is_draw) {
				char str[1024];
				GetText(str, 1024, koebuf, 10);
				local_system.DeleteText();
				local_system.DrawText(str); local_system.DrawTextEnd(1);
				is_draw = 0;
			}
			local_system.CallProcessMessages();
			usleep(10000); /* 10ms sleep */
		}
	}
	/* 割り込みがかかった：終了 */
	if (local_system.GetBacklog()) {
		/* backlog 終了 */
		int mode = local_system.GetBacklog();
		local_system.ClearIntterupt();
		/* text window を消す */
		local_system.DrawTextEnd(1);
		local_system.DeleteText();
		local_system.DeleteReturnCursor();
		// local_system.DeleteTextWindow();
		/* 終了のモード：再開か、現在のバックログの場所から再開 */
		if (mode == -2) { /* 現在のバックログの場所から */
			PopSkip();
			backlog_info->RollBack(*this);
			NextSkip();
		} else { /* GetBacklog() == -1 : 普通に再開 */
			PopSkip();
			RestoreState();
			ResumeOldText();
			NextSkip();
		}
		GlobalStackItem ret_point = GetSenarioPoint();
		PopSkip(); /* 最初の text / select は消さないといけない */
		return ret_point;
	} else {
		/* intterupt : 画面の復旧もなにもなし。とりあえず終了 */
		return GlobalStackItem();
	}
}

/*******************************************************************
**
** コンストラクタなど
*/

SENARIO_BackLog::SENARIO_BackLog(SENARIO_DECODE& dec, AyuSys& sys) :
	decoder(dec), old_stack(0x100),
	old_track(grp_info_orig[22]),
	old_grp_point(grp_info_orig[0]),
	grp_info(grp_info_orig+2), grp_hash(grp_info_orig+12),
	grp_info_len(grp_info_orig[1]),
	local_system(sys)
{
	old_flags = new SENARIO_FLAGS;
	old_track = -1;
	old_grp_point = -1;
	grp_info_len = 0;
	log_orig = new char[BACKLOG_LEN];
	log = log_orig + BACKLOG_LEN;
	bottom_point = 0;

	backlog_info = new SENARIO_BackLogInfo();

	exec_log_current = 0;
}

/*******************************************************************
**
** IsDirty()
*/

int SENARIO_BackLog::IsDirty(void) {
	if (local_system.IsTrackChange()) return 1;
	if (decoder.Flags().IsDirty()) return 1;
	if (decoder.GrpSave().IsChange()) return 1;
	if (local_system.CallStack().IsDirty()) return 1;
	return 0;
}

/*******************************************************************
**
** SENARIO_FLAGS 関連
*/

/* SENARIO_FLAGS の dirty bit 処理 */
void SENARIO_FLAGS::ClearDirty(void) {
	if (dirty == 0) return;
	dirty = 0;
	bit_dirty = 0;
	memset(var_dirty, 0, sizeof(unsigned int)*32);
	memset(str_dirty, 0, sizeof(unsigned int)*4);
}

int SENARIO_FLAGS::GetBitDirty(int* var_list) {
	if (bit_dirty == 0) return 0;
	if (dirty == 0) return 0;
	int num = 0; int vn = 0;
	int v = bit_dirty;
	int f = 1; int j;
	for (j=0; j<32; j++) {
		if (f & v) var_list[num++] = vn;
		vn++; f<<=1;
	}
	return num;
}
int SENARIO_FLAGS::GetVarDirty(int* var_list) {
	if (dirty == 0) return 0;
	int i,j; int num = 0;
	for (i=0; i<32; i++) {
		unsigned int v = var_dirty[i];
		unsigned int f = 1;
		int vn = i*32;
		if (v) {
			for (j=0; j<32; j++) {
				if (f & v) var_list[num++] = vn;
				vn++; f<<=1;
			}
		}
	}
	return num;
}
int SENARIO_FLAGS::GetStrDirty(int* var_list) {
	if (dirty == 0) return 0;
	int i,j; int num = 0;
	for (i=0; i<4; i++) {
		int v = str_dirty[i];
		int f = 1;
		int vn = i*32;
		if (v) {
			for (j=0; j<32; j++) {
				if (f & v) var_list[num++] = vn;
				vn++; f<<=1;
			}
		}
	}
	return num;
}

/* flag info の構造：
**	BL_FLAG_INF <len> <bit-len> <var-len> <str-len>
**		<bit-number> <old-bit> <new-bit> ...
**		<var-number> <old-var> <new-var> ...
**		<str-number(char)> <old-str(\0)> <new-str(\0)> ...
**		<len> BL_FLAG_INF
*/

int SENARIO_BackLog::SetFlagInfo(void) {
	if (! decoder.Flags().IsDirty()) return -1;
	SENARIO_FLAGS& new_flags = decoder.Flags();
	int bit_list[34], var_list[1025], str_list[130];
	int bit_len = new_flags.GetBitDirty(bit_list);
	int var_len = new_flags.GetVarDirty(var_list);
	int str_len = new_flags.GetStrDirty(str_list);
	bit_list[bit_len] = -2;
	var_list[var_len] = -2;
	str_list[str_len] = -2;
	int i;
	int flag_len = 0;
	/* 本当に変化しているかチェック */
	/* ついでに strvar の長さもチェック */
	for (i=bit_len-1; i>=0; i--) {
		int c = bit_list[i];
		if (new_flags.GetBitGrp2(c) == old_flags->GetBitGrp2(c) && c < 1000) {
			bit_list[i] = -1; bit_len--;
		}
	}
	for (i=var_len-1; i>=0; i--) {
		int c = var_list[i];
		if (new_flags.GetVar(c) == old_flags->GetVar(c) && c < 1000) {
			var_list[i] = -1; var_len--;
		}
	}
	for (i=str_len-1; i>=0; i--) {
		int c = str_list[i];
		const char* new_str = new_flags.StrVar(c);
		const char* old_str = old_flags->StrVar(c);
		if (strcmp(new_str, old_str) == 0) {
			str_list[i] = -1; str_len--;
		} else {
			flag_len += strlen(new_str)+1;
			flag_len += strlen(old_str)+1;
		}
	}
	if (bit_len == 0 && var_len == 0 && str_len == 0) {
		new_flags.ClearDirty();
		return -1;
	}

	/* 長さを調べて領域確保 */
	flag_len += 2 + 4 + 6 + bit_len*10 + var_len*10 + str_len;
	CheckLogLen(flag_len);

	/* 領域作成 */
	PushCmd(BL_FLAG_INF);
	PushShort(flag_len);
	for (i=0; str_list[i]!=-2; i++) {
		if (str_list[i] == -1) continue;
		int c = str_list[i];
		PushByte(c);
		PushStrZ(old_flags->StrVar(c));
		PushStrZ(new_flags.StrVar(c));
		old_flags->SetStrVar(c, new_flags.StrVar(c));
	}
	for (i=0; var_list[i]!=-2; i++) {
		if (var_list[i] == -1) continue;
		int c = var_list[i];
		PushShort(c);
		PushInt(old_flags->GetVar(c));
		PushInt(new_flags.GetVar(c));
		old_flags->SetVar(c,new_flags.GetVar(c));
	}
	for (i=0; bit_list[i]!=-2; i++) {
		if (bit_list[i] == -1) continue;
		int c = bit_list[i];
		PushShort(c);
		PushInt(old_flags->GetBitGrp2(c));
		PushInt(new_flags.GetBitGrp2(c));
		old_flags->SetBitGrp2(c, new_flags.GetBitGrp2(c));
	}
	PushShort(str_len);
	PushShort(var_len);
	PushShort(bit_len);
	PushShort(flag_len);
	PushCmd(BL_FLAG_INF);
	new_flags.ClearDirty();
	return GetPoint();
}

/* point の flag の復旧 */
void SENARIO_BackLog::DoFlag(int point) {
	int cur_point = GetPoint();
	if (SetPoint(point)) return;
	
	if (GetCmd() != BL_FLAG_INF) {
		SetPoint(cur_point);
		return;
	}
	SENARIO_FLAGS& flags = decoder.Flags();

	PopCmd(); PopShort();
	int bit_len = PopShort();
	int var_len = PopShort();
	int str_len = PopShort();
	int i;
	for (i=0; i<bit_len; i++) {
		PopInt(); // new_var
		unsigned int var = PopInt(); // old_var
		int index = PopShort();
		/* 1000 以降の bit は保存される */
		if (index < 1000)
			flags.SetBitGrp2(index, var);
	}
	for (i=0; i<var_len; i++) {
		PopInt(); /* new_var */
		int var = PopInt(); /* old_var */
		int index = PopShort();
		/* 1000 以降の var は保存される */
		if (index < 1000)
			flags.SetVar(index, var);
	}
	for (i=0; i<str_len; i++) {
		char buf[128]; /* 最大64byteのはず */
		PopStrZ(buf, 128); /* new str */
		PopStrZ(buf, 128); /* old str */
		int index = PopByte();
		if (index >= 0 && index < 100)
			flags.SetStrVar(index, buf);
	}
	PopShort(); PopCmd();
	SetPoint(cur_point);
	return;
}


/*******************************************************************
**
** music track
*/

/* music track info の構造
**	BL_MUS_INF <len> <cdrom track(/0)> <wave track(\0)> <old info> <len> BL_MUS_INF
*/

int SENARIO_BackLog::SetMusicInfo(void) {
	if (! local_system.IsTrackChange()) return -1;
	char old_cd[128], old_wave[128];
	char new_cd[128], new_wave[128];
	/* track を得る */
	/* 一回だけの曲再生のばあい、トラックの末尾に '\x01'をつける */
	strcpy(new_cd,local_system.GetCDROMTrack());
	strcpy(new_wave,local_system.GetEffecTrack());
	local_system.ClearTrackChange();
	if (new_cd[0] != '\0' && local_system.GetCDROMMode() == MUSIC_ONCE)
		strcat(new_cd, "\x01");
	if (new_wave[0] != '\0' && local_system.GetEffecMode() == MUSIC_ONCE)
		strcat(new_wave, "\x01");
	/* 変化のcheck */
	int point = GetPoint();
	if (SetPoint(old_track) == 0) {
		PopCmd(); PopShort(); PopInt();
		PopStrZ(old_wave,128); PopStrZ(old_cd,128);
		if ( strcmp(old_wave, new_wave) == 0 &&
			strcmp(old_cd, new_cd) == 0) {
			SetPoint(point);
			return -1;
		}
	}
	SetPoint(point);

	int mus_len = 2 + 4 + 4 + strlen(new_cd)+1 + strlen(new_wave)+1;
	CheckLogLen();
	PushCmd(BL_MUS_INF);
	PushShort(mus_len);
	PushStrZ(new_cd);
	PushStrZ(new_wave);
	PushInt(old_track);
	PushShort(mus_len);
	PushCmd(BL_MUS_INF);
	old_track =  GetPoint();
	return GetPoint();
}
void SENARIO_BackLog::DoMusic(int point) {
	int cur_point = GetPoint();
	if (SetPoint(point)) return;
	if (GetCmd() != BL_MUS_INF) {
		SetPoint(cur_point);
		return;
	}
	/* 変更の必要がないならなにもしない */
	char new_cd[128], new_wave[128];
	new_cd[0]='\0'; new_wave[0]='\0';
	PopCmd(); PopShort(); point = PopInt();
	if (SetPoint(point)) {
		PopCmd(); PopShort(); PopInt();
		PopStrZ(new_wave,128); PopStrZ(new_cd,128);
	}
	if (new_cd[0] != '\0') {
		if (new_cd[strlen(new_cd)-1] == '\x01') {
			local_system.SetCDROMOnce(); new_cd[strlen(new_cd)-1] = '\0';
		} else {
			local_system.SetCDROMCont();
		}
	}
	if (new_wave[0] != '\0') {
		if (new_wave[strlen(new_wave)-1] == '\x01') {
			local_system.SetEffecOnce(); new_wave[strlen(new_wave)-1] = '\0';
		} else {
			local_system.SetEffecCont();
		}
	}
	
	char* old_cd,*old_wave;
	old_cd = local_system.GetCDROMTrack();
	old_wave=local_system.GetEffecTrack();
	if (strcmp(old_cd, new_cd)) local_system.PlayCDROM(new_cd);
	else if (new_cd[0] == '\0') local_system.StopCDROM();
	if (strcmp(old_wave, new_wave)) local_system.PlayWave(new_wave);
	else if (new_wave[0] == '\0') local_system.StopWave();
	SetPoint(cur_point);
	return;
}
void SENARIO_BackLog::DoMusicNew(int point) {
	int cur_point = GetPoint();
	if (SetPoint(point)) return;
	if (GetCmd() != BL_MUS_INF) {
		SetPoint(cur_point);
		return;
	}
	/* 変更の必要がないならなにもしない */
	char new_cd[128], new_wave[128];
	new_cd[0]='\0'; new_wave[0]='\0';
	PopCmd(); PopShort(); PopInt();
	PopStrZ(new_wave,128); PopStrZ(new_cd,128);
	if (new_cd[0] != '\0') {
		if (new_cd[strlen(new_cd)-1] == '\x01') {
			local_system.SetCDROMOnce(); new_cd[strlen(new_cd)-1] = '\0';
		} else {
			local_system.SetCDROMCont();
		}
	}
	if (new_wave[0] != '\0') {
		if (new_wave[strlen(new_wave)-1] == '\x01') {
			local_system.SetEffecOnce(); new_wave[strlen(new_wave)-1] = '\0';
		} else {
			local_system.SetEffecCont();
		}
	}
	
	char* old_cd,*old_wave;
	old_cd = local_system.GetCDROMTrack();
	old_wave=local_system.GetEffecTrack();
	if (strcmp(old_cd, new_cd)) local_system.PlayCDROM(new_cd);
	else if (new_cd[0] == '\0') local_system.StopCDROM();
	if (strcmp(old_wave, new_wave)) local_system.PlayWave(new_wave);
	else if (new_wave[0] == '\0') local_system.StopWave();
	SetPoint(cur_point);
	return;
}

/*******************************************************************
**
** stack info
*/

/* stack info の構造
**	BL_STACK_INF <len> <old stack len> <seen> <point> <seen> <point> ...
**		<new stack len> <seen> <point> <seen> <point> ... <len> BL_STACK_INF
*/

int SENARIO_BackLog::SetStackInfo(void) {
	GosubStack& stack = local_system.CallStack();
	int i; int deal;
	if (! stack.IsDirty()) return -1;
	/* stack 内容が本当に変更されているかチェック */
	if (stack.CurStackDeal() == old_stack.CurStackDeal()) {
		deal = stack.CurStackDeal();
		for (i=0; i<deal; i++) {
			GlobalStackItem& new_item = stack[i];
			GlobalStackItem& old_item = old_stack[i];
			if (new_item.GetLocal() != old_item.GetLocal() ||
				new_item.GetSeen() != old_item.GetSeen()) break;
		}
		if (i == deal) {
			stack.ClearDirty();
			return -1;
		}
	}
	/* stack 情報の構築 */
	/* 最後に stack[0] を push
	 * -> pop 時はそのまま push すればいい */
	int stack_len = 2 + 4 + 4 + 8*old_stack.CurStackDeal() + 4 + 8*stack.CurStackDeal();
	CheckLogLen(stack_len);
	PushCmd(BL_STACK_INF);
	PushShort(stack_len);
	deal = stack.CurStackDeal();
	for (i=deal-1; i>=0; i--) {
		GlobalStackItem& item = stack[i];
		PushInt(item.GetSeen());
		PushInt(item.GetLocal());
	}
	PushInt(deal);
	deal = old_stack.CurStackDeal();
	for (i=deal-1; i>=0; i--) {
		GlobalStackItem& item = old_stack[i];
		PushInt(item.GetSeen());
		PushInt(item.GetLocal());
	}
	PushInt(deal);
	PushShort(stack_len);
	PushCmd(BL_STACK_INF);
	/* stack の copy */
	old_stack.InitStack();
	deal = stack.CurStackDeal();
	for (i=0; i<deal; i++) {
		old_stack.PushStack() = stack[i];
	}

	stack.ClearDirty();
	return GetPoint();
}

/* point の stack の復旧 */
void SENARIO_BackLog::DoStack(int point) {
	int cur_point = GetPoint();
	if (SetPoint(point)) return;
	if (GetCmd() != BL_STACK_INF) {
		SetPoint(cur_point);
		return;
	}
	GosubStack& stack = local_system.CallStack();

	PopCmd(); PopShort();
	stack.InitStack();
	int deal = PopInt();
	int i; for (i=0; i<deal; i++) {
		GlobalStackItem& item = stack.PushStack();
		int point = PopInt();
		int seen = PopInt();
		item.SetGlobal(seen, point);
	}
	PopShort(); PopCmd();
	SetPoint(cur_point);
	return;
}

/*******************************************************************
**
** graphics info
*/

/* graphics info の構造
**	BL_GRP_INF <len> <old grp pointer> <data> <len> BL_GRP_INF
**	BL_GRP_INF2 <old grp pointer> <current grp pointer> BL_GRP_INF2
*/

int SENARIO_BackLog::SetGraphicsInfo(void) {
	SENARIO_Graphics& grp = decoder.GrpSave();
	if (! grp.IsChange()) return -1;
	/* まず、hash をチェックする */
	int glen = grp.StoreBufferLen();
	int hash = grp.HashBuffer();
	int cur_point = GetPoint();
	int i; for (i=0; i<grp_info_len; i++) {
		if (grp_hash[i] == hash) {
			if (SetPoint(grp_info[i])) continue;
			if (cur_point - grp_info[i] > BACKLOG_LEN/2) continue; /* 古すぎる hash は使わない */
			PopCmd(); int _glen = PopShort(); PopInt();
			_glen -= 2+4+4;
			char* buf = PopBuffer(_glen);
			if (grp.CompareBuffer(buf, _glen) == 0) {
				/* 見つかった */
				SetPoint(cur_point);
				PushCmd(BL_GRP_INF2);
				PushInt(old_grp_point);
				PushInt(grp_info[i]);
				PushCmd(BL_GRP_INF2);
				old_grp_point = GetPoint();
				grp.ClearChange();
				return GetPoint();
			}
		}
	}
	/* 同じ grp info が見つからなければ自分で作る */
	int len = glen + 2+4+4;
	SetPoint(cur_point);
	CheckLogLen(len);
	PushCmd(BL_GRP_INF);
	PushShort(len);
	char* buf = PushBuffer(glen);
	PushInt(old_grp_point);
	PushShort(len);
	PushCmd(BL_GRP_INF);
	int grp_point = GetPoint();

	/* grp_hash に登録 */
	if (grp_info_len >= 10) {
		for (i=0; i<9; i++) {
			grp_info[i] = grp_info[i+1];
			grp_hash[i] = grp_hash[i+1];
		}
		grp_info_len = 9;
	}
	old_grp_point = grp_point;
	grp_info[grp_info_len] = grp_point;
	grp_hash[grp_info_len] = hash;
	grp_info_len++;
	
	/* 実際にグラフィックの内容を登録 */
	grp.StoreBuffer(buf, glen);
	grp.ClearChange();

	return GetPoint();
}

void SENARIO_BackLog::DoGraphics(int point) {
	int cur_point = GetPoint();
	if (SetPoint(point)) return;
	/* まず、grp info の格納されている場所を取り出す */
	int grp_point;
	if (GetCmd() == BL_GRP_INF) {
		PopCmd();
		PopShort(); /* len */
		grp_point = PopInt(); /* old_grp_point */
	} else { /* BL_GRP_INF2 */
		PopCmd();
		PopInt(); /* current grp point */
		grp_point = PopInt(); /* old grp point */
	}
	/* grp info を restore */
	if (SetPoint(grp_point)==0 && GetCmd() == BL_GRP_INF) {
		PopCmd();
		int len = PopShort();
		PopInt(); /* old_grp_point */
		int glen = len - 2-4-4;
		char* grp_buf = PopBuffer(glen);
		if (decoder.GrpSave().CompareBuffer(grp_buf, glen)) {
			decoder.local_system.DeleteTextWindow();
			decoder.GrpSave().RestoreBuffer(grp_buf, glen);
			decoder.GrpSave().Restore();
		}
	}
	/* pointer を元に戻す */
	SetPoint(cur_point);
	return;
}
void SENARIO_BackLog::DoGraphicsNew(int point) {
	int cur_point = GetPoint();
	if (SetPoint(point)) return;
	/* まず、grp info の格納されている場所を取り出す */
	int grp_point;
	if (GetCmd() == BL_GRP_INF) {
		grp_point = point;
	} else { /* BL_GRP_INF2 */
		PopCmd();
		grp_point = PopInt(); /* current grp point */
	}
	/* grp info を restore */
	if (SetPoint(grp_point)==0 && GetCmd() == BL_GRP_INF) {
		PopCmd();
		int len = PopShort();
		PopInt(); /* old_grp_point */
		int glen = len - 2-4-4;
		char* grp_buf = PopBuffer(glen);
		if (decoder.GrpSave().CompareBuffer(grp_buf, glen)) {
			decoder.local_system.DeleteTextWindow();
			decoder.GrpSave().RestoreBuffer(grp_buf, glen);
			decoder.GrpSave().Restore();
		}
	}
	/* pointer を元に戻す */
	SetPoint(cur_point);
	return;
}

/*******************************************************************
**
** local_system info
*/

/* local_system info の構造
**	BL_SYS_INF <len> <data> <len> BL_SYS_INF
*/

int SENARIO_BackLog::SetSystemInfo(void) {
	if (! local_system.config->IsDiff()) return -1;
	int slen = local_system.config->DiffLen();
	if (slen == 0) {
		local_system.config->ClearDiff();
		return -1;
	}
	int len = slen + 2 + 4; /* cmd, short, data, short, cmd */
	CheckLogLen(len);
	PushCmd(BL_SYS_INF);
	PushShort(len);
	char* buf = PushBuffer(slen);
	PushShort(len);
	PushCmd(BL_SYS_INF);
	local_system.config->Diff(buf);
	local_system.config->ClearDiff();
	return GetPoint();
}
void SENARIO_BackLog::DoSystem(int point) {
	int cur_point = GetPoint();
	if (SetPoint(point)) return;
	if (GetCmd() != BL_SYS_INF) {
		SetPoint(cur_point);
		return;
	}
	PopCmd(); int len = PopShort(); int slen = len-2-4;
	char* sbuf = PopBuffer(slen);
	local_system.config->PatchOld(sbuf);
	SetPoint(cur_point);
	return;
}
void SENARIO_BackLog::DoSystemNew(int point) {
	int cur_point = GetPoint();
	if (SetPoint(point)) return;
	if (GetCmd() != BL_SYS_INF) {
		SetPoint(cur_point);
		return;
	}
	PopCmd(); int len = PopShort(); int slen = len-2-4;
	char* sbuf = PopBuffer(slen);
	local_system.config->PatchNew(sbuf);
	SetPoint(cur_point);
	return;
}

/*******************************************************************
**
** local_system original info
*/

/* local_system original info の構造
**	BL_SYSORIG_INF <len> <data> <len> BL_SYS_INF
*/

/* システムの本来の状態からの変更を反映する */
void SENARIO_BackLog::AddSysChange(void) {
	int slen = local_system.config->DiffOriginalLen();
	if (slen == 0) {
		return;
	}
	int len = slen + 2 + 4; /* cmd, short, data, short, cmd */
	CheckLogLen(len);
	PushCmd(BL_SYSORIG_INF);
	PushShort(len);
	char* buf = PushBuffer(slen);
	PushShort(len);
	PushCmd(BL_SYSORIG_INF);
	local_system.config->DiffOriginal(buf);
	return;
}
void SENARIO_BackLog::DoSysOrig(void) {
	if (GetCmd() != BL_SYSORIG_INF) {
		PopSkip();
		return;
	}
	PopCmd(); int len = PopShort(); int slen = len-2-4;
	char* sbuf = PopBuffer(slen);
	PopShort(); PopCmd();
	local_system.config->PatchOriginal(sbuf);
	return;
}
void SENARIO_BackLog::AddGameSave(void) {
	AddSysChange();
	return;
}

/*******************************************************************
**	バックログの内容の表示
**	呼び出し側は GetPoint / SetPoint を
**	呼び出しの前後で行う必要がある
*/

/* END, END2, END3, RET */
void SENARIO_BackLog::DumpOnecmd(FILE* out, const char* tab) {
	fprintf(out, "%s%s\n", tab, bl_name[GetCmd()]);
}
/* TEXT, SEL2S, KOE, SAVEPT */
void SENARIO_BackLog::DumpText(FILE* out, const char* tab) {
	int cmd = PopCmd(); int p = PopInt(); int seen = PopShort();
	fprintf(out, "%s%s : seen %d, point %d\n",tab, bl_name[cmd], seen, p);
}
/* SEL2 */
void SENARIO_BackLog::DumpSel(FILE* out, const char* tab) {
	char buf[1024];
	int cmd = GetCmd();
	PopShort();
	PopStrZ(buf, 1024);
	fprintf(out, "%s%s : text %s\n",tab, bl_name[cmd], buf);
}
/* SEL2R */
void SENARIO_BackLog::DumpSelR(FILE* out, const char* tab) {
	int cmd = PopCmd();
	int sel = PopInt();
	fprintf(out, "%s%s : select %d\n",tab, bl_name[cmd], sel);
}
/* TITLE */
void SENARIO_BackLog::DumpTitle(FILE* out, const char* tab) {
	char buf[1024]; char buf2[1024];
	int cmd = GetCmd();
	PopShort();
	int p = PopInt(); int seen = PopShort();
	fprintf(out, "%s%s : seen %d, point %d, ",tab, bl_name[cmd], seen, p);
	PopStrZ(buf, 1024); kconv( (unsigned char*)buf, (unsigned char*)buf2); fprintf(out, "title %s -> ",buf2);
	PopStrZ(buf, 1024); kconv( (unsigned char*)buf, (unsigned char*)buf2); fprintf(out, "%s\n",buf2);
}
/* obsolete cmd */
void SENARIO_BackLog::DumpObsolete(FILE* out, const char* tab) {
	int cmd = PopCmd();
	fprintf(out, "%s%s : obsolete command\n",tab, bl_name[cmd]);
}
/* XXX_INF */
void SENARIO_BackLog::DumpFlagInf(FILE* out, const char* tab) {
	PopCmd(); PopShort();
	int bit_len = PopShort(); int var_len = PopShort(); int str_len = PopShort();
	int i;
	fprintf(out, "%sflag info:\n",tab);
	for (i=0; i<bit_len; i++) {
		int new_var = PopInt(); int old_var = PopInt(); int index = PopShort();
		fprintf(out, "%s    bit %d : %08x -> %08x\n",tab,index,old_var,new_var);
	}
	for (i=0; i<var_len; i++) {
		int new_var = PopInt(); int old_var = PopInt(); int index = PopShort();
		fprintf(out, "%s    var %d : %d -> %d\n",tab,index,old_var,new_var);
	}
	for (i=0; i<str_len; i++) {
		char new_buf[128], old_buf[128], new_buf2[128], old_buf2[128];
		PopStrZ(new_buf,128); PopStrZ(old_buf,128); int index = PopByte();
		kconv( (unsigned char*)new_buf, (unsigned char*)new_buf2);
		kconv( (unsigned char*)old_buf, (unsigned char*)old_buf2);
		fprintf(out, "%s    str %d : \"%s\" -> \"%s\"\n",tab,index,old_buf2,new_buf2);
	}
	return;
}

void SENARIO_BackLog::DumpMusicInf(FILE* out, const char* tab) {
	char new_cd[128], new_wave[128];
	PopCmd(); PopShort(); int old_track = PopInt();
	PopStrZ(new_wave, 128); PopStrZ(new_cd, 128);
	fprintf(out, "%smusic info: old track %d, new cd %s, new wave %s\n",tab, old_track, new_cd, new_wave);
}
void SENARIO_BackLog::DumpStackInf(FILE* out, const char* tab) {
	PopCmd(); PopShort(); int deal = PopInt();
	fprintf(out, "%sstack info : old stack deal %d\n",tab,deal);
	int i; for (i=0; i<deal; i++) {
		int point = PopInt(); int seen = PopInt();
		fprintf(out, "%s    seen %d, point %d\n",tab,seen,point);
	}
	deal = PopInt();
	fprintf(out, "%s  new stack deal %d\n",tab,deal);
	for (i=0; i<deal; i++) {
		int point = PopInt(); int seen = PopInt();
		fprintf(out, "%s    seen %d, point %d\n",tab,seen,point);
	}
}
void SENARIO_BackLog::DumpSysInf(FILE* out, const char* tab) {
	PopCmd(); int len = PopShort(); int slen = len-2-4;
	char* sbuf = PopBuffer(slen);
	fprintf(out, "%ssystem info : \n",tab);
	local_system.config->DumpPatch(out, tab, sbuf);
	return;
}
void SENARIO_BackLog::DumpSysInfOrig(FILE* out, const char* tab) {
	PopCmd(); int len = PopShort(); int slen = len-2-4;
	char* sbuf = PopBuffer(slen);
	fprintf(out, "%ssystem original info : \n",tab);
	local_system.config->DumpPatchOriginal(out, tab, sbuf);
	return;
}
void SENARIO_BackLog::DumpGrpInf(FILE* out, const char* tab) {
	PopCmd(); int len = PopShort(); int old_p = PopInt(); int glen = len - 2-4-4;
	char* grp_buf = PopBuffer(glen);
	fprintf(out, "%sgraphics info(2) : old point %d\n",tab, old_p);
	SENARIO_Graphics grpsave(local_system);
	grpsave.RestoreBuffer(grp_buf, glen);
	grpsave.Dump(out, tab);
}
void SENARIO_BackLog::DumpGrpInf2(FILE* out, const char* tab) {
	PopCmd(); int new_p = PopInt(); int old_p = PopInt();
	fprintf(out, "%sgraphics info(2) : old point %d, new point %d\n",tab, old_p, new_p);
}
void SENARIO_BackLog::Dump(FILE* out, const char* tab, int len) {
	int point = GetPoint();
	int i;
	for (i=0; i<len && CheckPrevCmd() == 0; i++) {
		int local_p = GetPoint();
		fprintf(out, "%s%d : ", tab, local_p);
		int cmd = GetCmd();
		if (cmd <= 0 || cmd > BL_MAX) fprintf(out,"(error)\n");
		else (this->*(bl_dump[cmd]))(out, tab);
		SetPoint(local_p);
		PopSkip();
	}
	SetPoint(point);
	return;
}
