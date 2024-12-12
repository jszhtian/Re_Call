/*  senario.h
 *     シナリオファイルの再生、フラグの管理、セーブデータの管理など
 *     システムのうち、ゲーム本体に関連するクラス群
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

#ifndef __KANON_SENARIO_H__
#define __KANON_SENARIO_H__

// #define SENARIO_DEBUG
// #define SENARIO_DUMP

#ifdef SENARIO_DUMP // DUMP mode
   // senario.cc で、ダンプモードにするのに必要な設定
#  ifndef SENARIO_DEBUG
#    define SENARIO_DEBUG
#  endif
#  define SUPRESS_JUMP
#  define SUPRESS_KEY
#  define SUPRESS_MUSIC
#  define SUPRESS_GLOBAL_CALL
#  define SUPRESS_WAIT
#  define SUPRESS_RAND
//#define DEBUG_READDATA
//#define DEBUG_DATA
#endif

// senario の実行を表示する
#ifdef SENARIO_DEBUG
#  define DEBUG_CalcVar
#  define DEBUG_CalcStr
#  define DEBUG_Select
#  define DEBUG_Condition
#  define DEBUG_GraphicsLoad
#  define DEBUG_Graphics
#  define DEBUG_0x0e
#  define DEBUG_TextWindow
#  define DEBUG_Wait
#  define DEBUG_Jump
#  define DEBUG_Other
#  define PrintLineNumber
#  define DEBUG_Graphics2
#endif

/* シナリオファイルの再生 */
#include "file.h"
#include <string.h>
#include "system.h"
#include "ard.h"

#ifndef BACKLOG_LEN
#  define BACKLOG_LEN 65536*16 /* バックログのバッファの長さ */
#endif
#define LOG_DELETE_LEN 1024*16 /* バックログが足りなくなったときに切り取る長さ */

class SENARIO_BackLog {
	class SENARIO_DECODE& decoder;
	/* 古い状態を保持する変数 */
	class SENARIO_FLAGS* old_flags;
	GosubStack old_stack;
	int& old_track;
	int& old_grp_point;
	/* 最近の graphics info を保存してある */
	int* grp_info; int* grp_hash; int& grp_info_len;
	int grp_info_orig[23];
	/* ログ本体 */
	char* log; int bottom_point; char* log_orig;
	void Print(void);
	/* バックログ実行時に一時的に使われる変数群 */
	/* exec_XXX の名前を持つ */
	class SENARIO_BackLogInfo* backlog_info;
	char* exec_log_current;

	int PopInt(void); void PushInt(int);
	short PopShort(void); void PushShort(short);
	char PopByte(void); void PushByte(char);
	int PopCmd(void); void PushCmd(int); int GetCmd(void);
	void PopStr(char*,int,int); void PushStr(const char*);
	void PopStrZ(char*,unsigned int); void PushStrZ(const char*);
	char* PopBuffer(int); char* PushBuffer(int);

	// バックログを減らす
	void CutLog(void);
	void CutLog(unsigned int len);
	void CheckLogLen(void) {
		if (log < log_orig+LOG_DELETE_LEN) CutLog();
	}
	void CheckLogLen(unsigned int check_len) {
		if (log < log_orig+check_len) CutLog(check_len);
	}
	int CheckNextCmd(void); // 次の cmd がログの範囲を超えていれば 1 を返す */
	int CheckPrevCmd(void); // 前の cmd がログの範囲を超えていれば 1 を返す */
	void NextSkip(void);
	void PopSkip(void);
public:
	class AyuSys& local_system;
	// pointer 操作
	int GetPoint(void) {
		return log_orig+BACKLOG_LEN-1-log + bottom_point;
	}
	int SetPoint(int point) { /* 成功すれば 0 が帰る */
		if (point < bottom_point) return -1;
		if (point-bottom_point >= BACKLOG_LEN) return -1;
		log = log_orig + BACKLOG_LEN - 1 - (point-bottom_point);
		return 0;
	}
	// grp hash の cut
	// 現在の log の場所よりも新しい grp hash を消す
	void CutHash(void);
public:
	SENARIO_BackLog(class SENARIO_DECODE& decoder, class AyuSys&);
	~SENARIO_BackLog();
	GlobalStackItem View(void); // バックログ表示
	void  RestoreState(void); // 現在、backlog に保存されている状態の再生

	/* 各情報の変化を調べる */
	int IsDirty(void);
	/* 各情報を格納 */
	/* graphics info は必要に応じて
	** 古い情報を返すこともある
	** flag/stack は確実に必要とする
	** log point から連続においてある
	*/
	int SetFlagInfo(void);
	int SetGraphicsInfo(void);
	int SetMusicInfo(void);
	int SetStackInfo(void);
	int SetSystemInfo(void);
	void DoFlagInfo(void);
	void RestoreGraphicsInfo(void);
	void RestoreMusicInfo(void);
	void RestoreStackInfo(void);
public:
	/* ログ内容の表示 */
	void Dump(FILE* out, const char* tab, int len);
	void DumpOnecmd(FILE* out, const char* tab);
	void DumpText(FILE* out, const char* tab);
	void DumpSel(FILE* out, const char* tab);
	void DumpSelR(FILE* out, const char* tab);
	void DumpTitle(FILE* out, const char* tab);
	void DumpObsolete(FILE* out, const char* tab);
	void DumpFlagInf(FILE* out, const char* tab);
	void DumpMusicInf(FILE* out, const char* tab);
	void DumpStackInf(FILE* out, const char* tab);
	void DumpSysInf(FILE* out, const char* tab);
	void DumpSysInfOrig(FILE* out, const char* tab);
	void DumpGrpInf(FILE* out, const char* tab);
	void DumpGrpInf2(FILE* out, const char* tab);
public:
	// バックログ登録
	void AddEnd(void);
	void AddEnd2(void);
	void AddEnd3(void);
	void AddRet(void);
	void AddWI(void);
	void AddGameSave(void);
	void AddSysChange(void);
	void AddText(int point, int seen_no, int is_sel);
	void AddText(int point, int seen_no) { AddText(point, seen_no, 0); }
	void AddKoe(int point, int seen_no) { AddText(point, seen_no, 3); }
	void AddSelect1Start(int point, int seen_no) { AddText(point, seen_no, 2); }
	void AddSelect2Start(int point, int seen_no) { AddText(point, seen_no, 1); }
	void AddSelect2(char* str);
	void AddSelect2Result(int result);
	void AddSetTitle(char* s, int point, int seen);
	void AddSavePoint(int p, int seen);
	// バックログ実行
	void PopTextWI(int& p, int& seen, int& flag, int& grp, int& mus, int& stack);
	void DoMsgPos2New(AyuSys& sys);
	void DoMsgPosNew(AyuSys& sys);
	void DoMsgSizeNew(AyuSys& sys);
	void DoMojiSizeNew(AyuSys& sys);
	void DoIsWakuNew(AyuSys& sys);
	void DoTitleNew(AyuSys& sys);
	void DoGrpInfNew(AyuSys& sys);
	void DoMusInfNew(AyuSys& sys);
	void DoFlagInfNew(AyuSys& sys);
	void DoStackInfNew(AyuSys& sys);
	void DoSysInfNew(AyuSys& sys);
	void DoMsgPos2Old(AyuSys& sys);
	void DoMsgPosOld(AyuSys& sys);
	void DoMsgSizeOld(AyuSys& sys);
	void DoMojiSizeOld(AyuSys& sys);
	void DoIsWakuOld(AyuSys& sys);
	void DoTitleOld(AyuSys& sys);
	void DoGrpInfOld(AyuSys& sys);
	void DoMusInfOld(AyuSys& sys);
	void DoFlagInfOld(AyuSys& sys);
	void DoStackInfOld(AyuSys& sys);
	void DoSysInfOld(AyuSys& sys);
	void DoSysOrig(void);
	// info の具現化
	void DoFlag(int);
	void DoStack(int);
	void DoGraphics(int);
	void DoGraphicsNew(int);
	void DoMusic(int);
	void DoMusicNew(int);
	void DoSystem(int);
	void DoSystemNew(int);
	// バックログテキスト取得
	int CheckLogValid(void); /* ログの有効性チェック */
	void GetInfo(int& grp, int& mus);
	int SkipNewMessage(void);
	int SkipOldMessage(void);
	int SkipNewMessages(int count, int skip_flag);
	int SkipOldMessages(int count, int skip_flag);
	void GetText(char* ret_str, unsigned int str_len, int* koebuf, int koebuf_len);
	void ResumeOldText(void);
	GlobalStackItem GetSenarioPoint(void);
	// バックログ出力
	void PutLog(char* log, unsigned int len, int is_save);
	void SetLog(char* log, unsigned int len, int is_save);
	void ClearLog(void);
	// ClearLog / PutLog の後にはこれでログを再開する
	void StartLog(int is_save);
	// save point を得る。
	void GetSavePoint(int& seen, int& point, class SENARIO_FLAGS** save_flags=0, GosubStack** save_stack=0);
};

class SENARIO_DECODE {
	unsigned char* basedata; // シナリオファイルの最初を指している
	unsigned char* data;
	int seen_no;
	int data_len;
	unsigned char cmd;
	class SENARIO& senario;
	class SENARIO_FLAGSDecode& flags;
	class SENARIO_MACRO& macro;
	class SENARIO_Graphics& graphics_save;
	class SENARIO_BackLog backlog;

	void* normal_timer; // for normal wait operation

	// 半角から全角へのテーブル。なお、SJIS コード。
	static unsigned char hankaku_to_zenkaku_table[0x60 * 2];

	// GetText() 関数で使う一時バッファ
	unsigned char* gettext_cache_data; int gettext_cache_number;
public:
	AyuSys& local_system;
	SENARIO_DECODE(int _seen,unsigned char* d, int len, class SENARIO& _senario, class SENARIO_FLAGSDecode& _flag, class SENARIO_MACRO& _mac, class SENARIO_Graphics& _gs, class AyuSys& _sys) : senario(_senario), flags(_flag), macro(_mac), graphics_save(_gs), backlog(*this, _sys), local_system(_sys){
		seen_no = _seen;
		basedata = data = d;
		data_len = len;
		normal_timer = local_system.setTimerBase();
		gettext_cache_data = 0; gettext_cache_number = -1;
	}
	~SENARIO_DECODE() {
		local_system.freeTimerBase(normal_timer);
		if (gettext_cache_data) delete[] gettext_cache_data;
	}
	// int GetTime(void) { return senario.GetTime(); }
	class SENARIO& Senario(void) { return senario; }
	class SENARIO_FLAGSDecode& Flags(void) { return flags; }
	class SENARIO_Graphics& GrpSave(void) { return graphics_save; }
	class SENARIO_BackLog& BackLog(void) { return backlog; }
	class SENARIO_MACRO& Macro(void) { return macro; }

	unsigned char Cmd(void) { return cmd;}; // 処理中のコマンドを帰す
	unsigned char NextChar(void) { return *data;}
	unsigned char NextCharwithIncl(void) { return *data++;}
	int ReadInt(void) { data+=4; return read_little_endian_int((char*)(data-4));}
	int ReadData(void); // データを１つ返す
	static int ReadDataStatic(unsigned char*&, int& is_var);
	static int ReadDataStatic(unsigned char*&, SENARIO_FLAGSDecode& flags);
	char* ReadString(char*); // 文字列を読み込んで返す
		// 引数で指定したバッファに文字列が帰るとは限らない
	static char* ReadStringStatic(unsigned char*& senario_data,char*, SENARIO_FLAGSDecode& flags); // 文字列を読み込んで帰す

	void ReadStringWithFormat(TextAttribute& text, int is_verbose = 1);

	void SetPoint(int point) {
		if (point >= data_len) point = 0;
		data = basedata + point;
	}
	int GetPoint(void) { return data-basedata; }
	int GetSeen(void) { return seen_no; }
	unsigned char* GetData(int point) { return basedata+point;}
	void GetText(char* ret_str, unsigned int str_len, int* koe, GlobalStackItem& point);

	int Decode(void); // シナリオのデコード
	void DumpData(void); // 現在のデータ付近をダンプ出力

	// 半角 -> 全角の変換
	static char* Han2Zen(const char* str); // 新しく割り当てられたメモリに返る

	int Decode_Music(void);
	int Decode_Wait(void); // cmd 0x19
	int Decode_TextWindow(void); // cmd 0x01, 0x02, 0x03, 0xff
	GlobalStackItem Decode_Jump(void);
	int DecodeSkip(char** strlist, int& pt, int max);
	void DecodeSkip_Music(void);
	void DecodeSkip_Wait(void);
	void DecodeSkip_TextWindow(void);
	void DecodeSkip_Jump(void);

	// シナリオを先読みし、画像ファイルの名前を得る
	void ReadGrpFile(char** filelist, int deal);
};


class SENARIO_FLAGS {
	unsigned int bit_variables[64]; // 2048 bits
	int variables[2048];
	char string_variables[128][0x40]; // 64 bytes * 128
	static unsigned int bits[32];
	/* 変更の有無 */
	int dirty;
	unsigned int bit_dirty; // 32 bits = bit_variables の 0-32
	unsigned int var_dirty[32]; // 1024 bits = variables の前半
	unsigned int str_dirty[4]; // 128 bits
public:
	// dirty bit まわりの処理
	/* この辺の処理は senario_backlog.cc にある */
	int IsDirty(void) { return dirty; }
	void ClearDirty(void);
	int GetBitDirty(int* var_list); // 最大 32 word 必要
	int GetVarDirty(int* var_list); // 最大 1024 word 必要
	int GetStrDirty(int* var_list); // 最大 128 word 必要
	/* dirty のチェックは初めの 1000 個の変数のみ
	** 行う必要がある。
	*/
	void SetBitDirty(int number) {
		number &= 0x1f;
		dirty = 1;
		bit_dirty |= 1U<<number;
	}
	void SetVarDirty(int number) {
		number &= 1023;
		dirty = 1;
		var_dirty[number>>5] |= 1U << (number&0x1f);
	}
	void SetStrDirty(int number) {
		number &= 127;
		dirty = 1;
		str_dirty[number>>5] |= 1U << (number&0x1f);
	}
	// operation of bit variables
	int GetBit(int number) {
		number &= 2047;
		return bit_variables[number>>5] & bits[number&0x1f];
	}
	void SetBit(int number, int bit) {
		if (number > 2000 || number < 0) return;
		number &= 2047;
		if (bit) bit_variables[number>>5] |= bits[number&0x1f];
		else bit_variables[number>>5] &= ~bits[number&0x1f];
		SetBitDirty(number>>5);
	}
	// assume little endian...
	unsigned char GetBitGrp(int num) {
		num &= 2047;
		num /= 8;
		int c = num & 3; int n = num >> 2;
		c *= 8;
		return (bit_variables[n] >> c) & 0xff;
	}
	void SetBitGrp(int num, unsigned char var) {
		num &= 2047;
		num /= 8;
		int c = num & 3; int n = num >> 2;
		c *= 8;
		unsigned int mask = 0xff; mask <<= c;
		unsigned int v = var; v <<= c;
		bit_variables[n] &= ~mask;
		bit_variables[n] |= v;
		SetBitDirty(n);
	}
	unsigned int GetBitGrp2(int num) {
		num &= 63;
		return bit_variables[num];
	}
	void SetBitGrp2(int num, unsigned int var) {
		num &= 63;
		bit_variables[num] = var;
		SetBitDirty(num);
	}
	void SetVar(int number, int var) {
		number &= 2047;
		variables[number] = var;
		SetVarDirty(number);
	}
	int GetVar(int number) {
		number &= 2047;
		return variables[number];
	}
	const char* StrVar(int number) { return string_variables[number]; }
	char* StrVar_nonConst(int number) { SetStrDirty(number); return string_variables[number]; }
	void SetStrVar(int number, const char* src);
	SENARIO_FLAGS(void);
	void Copy(const SENARIO_FLAGS& src);
	~SENARIO_FLAGS();
	int Var(int index) { return variables[index];}
	int Bit(int index) { return GetBit(index);}

};
class SENARIO_FLAGSDecode : public SENARIO_FLAGS {
public:
	// variable , bit_variable の演算をする
	//  cmd 0x37 - 0x57
	void DecodeSenario_CalcVar(class SENARIO_DECODE& decoder);
	void DecodeSkip_CalcVar(class SENARIO_DECODE& decoder);

	// string variable の操作をする
	// cmd 0x59.
	void DecodeSenario_CalcStr(class SENARIO_DECODE& decoder);
	void DecodeSkip_CalcStr(class SENARIO_DECODE& decoder);

	// selection の操作をする
	// cmd 0x58.
	void DecodeSenario_Select(class SENARIO_DECODE& decoder);
	void DecodeSkip_Select(class SENARIO_DECODE& decoder);

public:
	// cmd 0x37 - 0x59
	void DecodeSenario_Calc(class SENARIO_DECODE& decoder) {
		if (decoder.Cmd() == 0x59) DecodeSenario_CalcStr(decoder);
		else if (decoder.Cmd() == 0x58) DecodeSenario_Select(decoder);
		else DecodeSenario_CalcVar(decoder);
	}

	void DecodeSkip_Calc(class SENARIO_DECODE& decoder) {
		if (decoder.Cmd() == 0x59) DecodeSkip_CalcStr(decoder);
		else if (decoder.Cmd() == 0x58) DecodeSkip_Select(decoder);
		else DecodeSkip_CalcVar(decoder);
	}

	// cmd 0x15 で、条件判定をする
	int DecodeSenario_Condition(class SENARIO_DECODE& decoder, TextAttribute& attr);
	int DecodeSenario_Condition(class SENARIO_DECODE& decoder) {
		TextAttribute attr;
		return DecodeSenario_Condition(decoder, attr);
	}
	int DecodeSkip_Condition(class SENARIO_DECODE& decoder);

	// cmd 0x5b で、一連のデータをクリア・セットする
	void DecodeSenario_VargroupRead(class SENARIO_DECODE& decoder);
	void DecodeSkip_VargroupRead(class SENARIO_DECODE& decoder);
	// cmd 0x5c で、一連のデータをクリア・セットする
	void DecodeSenario_VargroupSet(class SENARIO_DECODE& decoder);
	void DecodeSkip_VargroupSet(class SENARIO_DECODE& decoder);
	// cmd 0x5d で、一連のデータをクリア・セットする
	void DecodeSenario_VargroupCopy(class SENARIO_DECODE& decoder);
	void DecodeSkip_VargroupCopy(class SENARIO_DECODE& decoder);

};

class SENARIO_MACRO {
	unsigned char** macros;
	int macro_deal;
public:
	SENARIO_MACRO(void);
	void SetMacro(int n, const unsigned char* str);
	const unsigned char* GetMacro(int n) {
		if (n < 0 || n >= macro_deal) return 0;
		return macros[n];
	}
	unsigned char* DecodeMacro(unsigned char* str, unsigned char* buf);
	~SENARIO_MACRO();
	void Save(FILE* out);
	void Load(FILE* out);
};

class SENARIO_Graphics;
// SENARIO_Graphics で行った動作を保存する構造体
// Save... で保存し、Do... でその動作を実行する
struct SENARIO_GraphicsSaveBuf {
	int cmd;
	int filedeal;
	char filenames[0x200];
	int args[24];
	int arg2;
	int arg3;
	int arg4[64];
public:
	// backlog 周り
	int StoreLen(void); // この GraphicsSaveBuf を保存するのに必要な長さを返す
		// 実際は適当に LenXXX() を呼び出す
	char* Store(char*); // buffer を適当な形式で保存
	void Restore(const char*); // buffer を復元
	void Dump(FILE*,const char*); // 内容を表示
	int Hash(void); // hash を返す
	int Compare(const char*); // 内容の比較。同じなら 0 を返す

	void SaveLoadDraw(char* str, SEL_STRUCT* sel);
	void DoLoadDraw(SENARIO_Graphics& drawer, int is_draw);
	void SaveLoad(char* str, int pdt);
	void DoLoad(SENARIO_Graphics& drawer);
	

	void SaveMultiLoad(int cmd,char* fnames, int fname_deal, int all_len, int sel_no, int* geos);
	void DoMultiLoad(SENARIO_Graphics& drawer, int is_draw);

	void SaveClear(int,int,int,int,int,int,int,int);
	void DoClear(SENARIO_Graphics& drawer);
	void SaveFade(int,int,int,int,int,int,int,int,int);
	void DoFade(SENARIO_Graphics& drawer);

	void SaveCopy(int,int,int,int,int,int,int,int,int);
	void DoCopy(SENARIO_Graphics& drawer);
	void SaveCopyWithMask(int,int,int,int,int,int,int,int,int);
	void DoCopyWithMask(SENARIO_Graphics& drawer);
	void SaveCopyWithoutColor(int,int,int,int,int,int,int,int,int,int,int);
	void DoCopyWithoutColor(SENARIO_Graphics& drawer);
	void SaveSwap(int,int,int,int,int,int,int,int);
	void DoSwap(SENARIO_Graphics& drawer);

	/* バックバッファ関係*/
	void SaveSaveScreen(void);
	void SaveCopytoScreen(void);
	void DoCopytoScreen(SENARIO_Graphics& drawer,int is_draw);
	int IsSaveScreen(void) { return cmd == 0xaa; }

	// 実際に画面に描画
	// draw_flag == 1 ならバッファ内の操作のみならず、画面への描画もおこなう
	void Do(SENARIO_Graphics& drawer, int draw_flag);
	// この graphics save buf で画面への描画が行われるなら 1 が返る
	int IsDraw(void);
	SENARIO_GraphicsSaveBuf() {
	}
	// ファイルへのセーブ・ロード
	void Save(FILE* out);
	int Load(char* buf);
};

class SENARIO_Graphics {
	SENARIO_GraphicsSaveBuf buf[32];
	int deal;
	int changed;
	int saved_count;
	void DeleteBuffer(int n);
	int BufferLength(void) { return deal; }
	int IsTopSaveScreen(void) {
		if (deal == 0) return 0;
		return buf[deal-1].IsSaveScreen();
	}
public:
	AyuSys& local_system;
	SENARIO_GraphicsSaveBuf& Alloc(void);
	void ClearBuffer(void);
	// 内容のコピー
	void operator =(SENARIO_Graphics& g) {
		memcpy(buf, g.buf, sizeof(buf));
		deal = g.deal;
	}
	// 内容の変化
	int IsChange(void) { return changed;}
	void ClearChange(void) { changed = 0;}
	void Change(void) { changed = 1; }

	SENARIO_Graphics(AyuSys& sys);
	void DoClear(int,int,int,int,int,int,int,int);
	void DoFade(int,int,int,int,int,int,int,int,int);
	void DoLoadDraw(char* file, struct SEL_STRUCT* sel);
	void DoLoad(char* file, int pdt);
	void DoCopy(int,int,int,int,int,int,int,int,int);
	void DoCopyWithMask(int,int,int,int,int,int,int,int,int);
	void DoCopyWithoutColor(int,int,int,int,int,int,int,int,int,int,int);
	void DoSwap(int,int,int,int,int,int,int,int);
	// セーブファイルの読み込み・保存
	void Save(FILE* out);
	int Load(char* buf);
	void Restore(int draw_flag = 1); // グラフィックをもとに戻す
	// backlog の保存
	int HashBuffer(void);
	int StoreBufferLen(void);
	void StoreBuffer(char*, int);
	void RestoreBuffer(const char*, int);
	int CompareBuffer(const char*, int);
	void Dump(FILE*,const char*);
	// シナリオファイルのデコード
	int DecodeSenario_GraphicsLoad(SENARIO_DECODE& decoder);
	void DecodeSkip_GraphicsLoad(SENARIO_DECODE& decoder, char** filelist, int& list_pt, int max);
	int DecodeSenario_Graphics(SENARIO_DECODE& decoder);
	void DecodeSkip_Graphics(SENARIO_DECODE& decoder);
	void DecodeSenario_Fade(SEL_STRUCT* sel, int,int,int);
};

// シナリオのパッチあてクラス
// システム側でパッチは用意されており、ユーザー側は
// そのなかからAddPatch() を使って自由なパッチを選ぶ
class SENARIO_PATCH {
	char* identifier;
	virtual unsigned char* Patch(int seen_no, unsigned char* origdata, int datalen, int version) = 0; // このパッチを当てる
	int is_used; // 使用されているか
	SENARIO_PATCH* next;
	static SENARIO_PATCH* head;
public:
	SENARIO_PATCH(char* identifier);
	virtual ~SENARIO_PATCH(){}
	const char* ID(void) const { return identifier; }
	static void AddPatch(char* identifier); // あてるべきパッチのパターンを追加
	/* 全てのパッチを当てる */
	/* 返り値として origdata と異なる、新しい領域が new されて返る。
	** また、その領域は末尾に 1024byte の余裕を持つ
	*/
	static unsigned char* PatchAll(int seen_no, const unsigned char* origdata, int datalen, int version); // すべてのパッチあてを実行
};

#define SENARIO_GRPREAD 8 // グラフィックの先読みの大きさ
#define MAX_SEEN_NO 1000 // seen no の最大値
#define MAX_READ_FLAGS 8192 // 4*65536 個のメッセージまで対応

#if SENARIO_GRPREAD > (MaxPDTImage-2)
#  undef SENARIO_GRPREAD
#  if MaxPDTImage <= 2
#    define SENARIO_GRPREAD 0
#  else
#    define SENARIO_GRPREAD (MaxPDTImage-2)
#  endif
#endif

class SENARIO {
	int seen_no;
	int save_head_size, save_block_size, save_tail_size;
	int isReadHeader;
	unsigned char* data_orig;
	SENARIO_DECODE* decoder;
	AyuSys& local_system;
	GlobalStackItem current_point;
	SENARIO_FLAGSDecode* flags;
	SENARIO_MACRO* macros;
	SENARIO_Graphics* grpsave;
	ARDDAT* arddata;
	AyuSys::GrpFastType old_grp_mode; int old_glen;
	char* old_grp_state; char old_cdrom_track[128]; char old_effec_track[128];

	// 既読フラグ
#define READ_FLAG_SIZE (MAX_SEEN_NO+MAX_READ_FLAGS+1+1) /* セーブファイルに必要なブロック数 */
#define READ_FLAG_MAGIC 0xde491260

	int read_flag_table[MAX_SEEN_NO];
	int max_read_flag_number;
	int read_flags[MAX_READ_FLAGS];
	int max_read_flag;
	void ClearReadFlag(void); // 全既読フラグをクリア
	void ReadReadFlag(void);
	void WriteReadFlag(void);

	int last_grp_read_point;
	char* grp_read_buf[SENARIO_GRPREAD];
	int grp_read_deal;
	int in_proc; // save 関係などで、再入を禁止する

	void* basetime;
	char* savefname;
	friend class IdleReadGrp;

	NameSubEntry* name_entry;
public:
	static int* ListSeens(void);
	SENARIO(char* savedir, AyuSys& sys);
	~SENARIO();
	int Init(void); // シナリオを読み込む
	int IsValid(void) { if (data_orig == 0) return false; return true; }
	// シナリオを先読みし、グラフィックを読み込む
	void ReadGrp(void);
	// 画像の回復
	void CheckGrpMode(void);
	void RestoreGrp();

	// 既読フラグ周り
	void SetReadFlag(int flag_no) {
		if (flag_no > max_read_flag || flag_no < 0) return;
		if (read_flag_table[seen_no] == -1) return;
		int table_no = read_flag_table[seen_no] + flag_no/32;
		read_flags[table_no] |= 1 << (flag_no&0x1f);
	}
	int IsReadFlag(int flag_no) {
		if (flag_no > max_read_flag || flag_no < 0) return 0;
		if (read_flag_table[seen_no] == -1) return 0;
		int table_no = read_flag_table[seen_no] + flag_no/32;
		return (read_flags[table_no] & (1 << (flag_no&0x1f))) != 0;
	}
	void AssignReadFlag(void); // decoder 割り当ての最初に呼び出される
	void SetMaxReadFlag(int no) { // AssignReadFlag 用
		if (max_read_flag < no) max_read_flag = no;
	}

	// ARD ファイルの操作
	void ClearArd(void) { if (arddata) delete arddata; arddata = 0; }
	void AssignArd(char* fname) { ClearArd(); arddata = new ARDDAT(fname, local_system); }
	ARDDAT* ArdData(void) { return arddata; }

	// タイマー
	int GetTimer(void) { return local_system.getTime(basetime); }
	void InitTimer(void) { local_system.freeTimerBase(basetime); basetime = local_system.setTimerBase(); }

	// 名前入力
	void SetNameEntry(NameSubEntry* e ) {
		if (name_entry) local_system.CloseNameEntry(name_entry);
		name_entry = e;
	}
	void SetNameToEntry(const char* s) {
		if (name_entry) local_system.SetNameToEntry(name_entry, s);
	}
	const char* GetNameFromEntry(void) {
		if (name_entry == 0) return "";
		else return local_system.GetNameFromEntry(name_entry);
	}
	void CloseNameEntry(void) {
		if (name_entry) local_system.CloseNameEntry(name_entry);
		name_entry = 0;
		return;
	}

	unsigned char* MakeSenarioData(int seen_no, int* slen);
	void PlayFirst(void); // シナリオの初期化ルーチンを実行
	GlobalStackItem Play(GlobalStackItem item); 
	char* GetTitle(int seen_no);
	void SetPoint(GlobalStackItem item) { current_point = item; }
	void PlayLast(void);
	// ファイルの読み込み
	void MakeSaveFile(char* dir);
	int IsSavefileExist(void);
	void CreateSaveFile(void);
	void ReadSaveHeader(void);
	void WriteSaveHeader(void);
	void ReadSaveFile(int n, GlobalStackItem& go);
	void WriteSaveFile(int n, char* title);
	int IsValidSaveData(int n, char* title);
	char** ReadSaveTitle(void);
};
class IdleReadGrp : public IdleEvent{
	SENARIO* senario;
public:
	IdleReadGrp(SENARIO* s) : IdleEvent(s->local_system) {
		senario = s;
	}
	int Process(void) {
		senario->ReadGrp();
		return 1;
	}
};

#endif // !defined( __KANON_SENARIO_H__)
