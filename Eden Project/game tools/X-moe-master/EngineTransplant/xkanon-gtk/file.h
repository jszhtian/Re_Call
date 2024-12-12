/*  file.h  : KANON の圧縮ファイル・PDT ファイル（画像ファイル）の展開の
 *            ためのクラス
 *     class FILESEARCH : ファイルの管理を行う
 *     class ARCINFO : 書庫ファイルの中の１つのファイルを扱うクラス
 *     class PDTCONV : PDT ファイルの展開を行う。
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

#ifndef __KANON_FILE_H__
#define __KANON_FILE_H__

#ifndef DIR_SPLIT
#define DIR_SPLIT '/'	/* UNIX */
#endif

// read 'KANON' compressed file

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#if defined(__sparc) || defined(sparc)
#  if !defined(WORDS_BIGENDIAN)
#    define WORDS_BIGENDIAN 1
#  endif
#endif

#ifdef WORDS_BIGENDIAN

#define INT_SIZE 4

static int read_little_endian_int(const char* buf) {
	const unsigned char *p = (const unsigned char *) buf;
	return (p[3] << 24) | (p[2] << 16) | (p[1] << 8) | p[0];
}

static int read_little_endian_short(const char* buf) {
	const unsigned char *p = (const unsigned char *) buf;
	return (p[1] << 8) | p[0];
}

static int write_little_endian_int(char* buf, int number) {
	int c = read_little_endian_int(buf);
	unsigned char *p = (unsigned char *) buf;
	unsigned int unum = (unsigned int) number;
	p[0] = unum & 255;
	unum >>= 8;
	p[1] = unum & 255;
	unum >>= 8;
	p[2] = unum & 255;
	unum >>= 8;
	p[3] = unum & 255;
	return c;
}

static int write_little_endian_short(char* buf, int number) {
	int c = read_little_endian_short(buf);
	unsigned char *p = (unsigned char *) buf;
	unsigned int unum = (unsigned int) number;
	p[0] = unum & 255;
	unum >>= 8;
	p[1] = unum & 255;
	return c;
}

#else // defined(WORDS_BIGENDIAN)

// assume little endian...
#define INT_SIZE 4

inline int read_little_endian_int(const char* buf) {
	return *(int*)buf;
}

inline int read_little_endian_short(const char* buf) {
	return *(short*)buf;
}

inline int write_little_endian_int(char* buf, int number) {
	int c = *(int*)buf; *(int*)buf = number; return c;
}

inline int write_little_endian_short(char* buf, int number) {
	int c = *(short*)buf; *(short*)buf = number; return c;
}
#endif // WORDS_BIGENDIAN

/*********************************************
**  FILESEARCH:
**	書庫ファイル／ディレクトリを含め、
**	全ファイルの管理を行う。
**
**	最初に、設定ファイルからファイルの種類ごとに
**	実際に入っているディレクトリ、書庫を設定する
**
**	以降はFind() メソッドで実際のファイルの内容を得る
**
*/

/* ARCFILE と DIRFILE はファイル種類ごとの情報 */
class ARCFILE;
class DIRFILE;
class SCN2kFILE;
class RaffresiaFILE;
/* ARCINFO はファイルを読み込むために必要 */
class ARCINFO;
class FILESEARCH {
public:
#define TYPEMAX 13
	enum FILETYPE {
		/* 一応、0 - 15 まで reserved */
		ALL = 1, /* dat/ 以下のファイル(デフォルトの検索先) */
		ROOT= 2, /* ゲームのインストールディレクトリ */
		PDT = 3, /* default: PDT/ */
		SCN = 4, /* default: DAT/SEEN.TXT */
		ANM = 5, /* default: DAT/ALLANM.ANL */
		ARD = 6, /* default: DAT/ALLARD.ARD */
		CUR = 7, /* default: DAT/ALLCUR.CUR */
		MID = 8, /* default: ALL */
		WAV = 9, /* default: ALL */
		KOE = 10, /* default: KOE/ */
		BGM = 11, /* default: BGM */
		MOV = 12  /* default : MOV */
	};
	enum ARCTYPE {ATYPE_DIR, ATYPE_ARC, ATYPE_SCN2k, ATYPE_Raffresia};
private:
	/* InitRoot() の時点で初期化される変数 */
	DIRFILE* root_dir;
	DIRFILE* dat_dir;
	ARCFILE* searcher[TYPEMAX];
	/* ファイルの存在位置の information */
	ARCTYPE is_archived[TYPEMAX];
	char* filenames[TYPEMAX];
	/* デフォルトの information */
	static ARCTYPE default_is_archived[TYPEMAX];
	static char* default_dirnames[TYPEMAX];
public:
	FILESEARCH(void);
	~FILESEARCH();
	/* 初めにゲームのデータがあるディレクトリを設定する必要がある */
	int InitRoot(char* root);
	/* ファイルの型ごとの情報をセットする */
	void SetFileInformation(FILETYPE type, ARCTYPE is_arc,
		char* filename);
	/* 複数のファイルを一つの型に関連づける */
	void AppendFileInformation(FILETYPE type, ARCTYPE is_arc,
		char* filename);
	ARCFILE* MakeARCFILE(ARCTYPE tp, char* filename);
	/* fname で指定された名前のファイルを検索 */
	class ARCINFO* Find(FILETYPE type, const char* fname, const char* ext=0);
	/* ある種類のファイルをすべてリストアップ
	** 末尾は NULL pointer
	*/
	char** ListAll(FILETYPE type);
};

class ARCINFO {
	/* ファイルそのものの情報 */
protected:
	char* arcfile;
	long offset;
	int arcsize; int filesize;
	/* 状態 */
	enum {INIT, ERROR, MMAP, READ, READ_BEFORE_EXTRACT, EXTRACT, EXTRACT_MMAP, DONE} status;
	/* ファイル読み込み中の情報 */
	int fd;
	char* readbuf;
	int read_count;
	/* ファイル変換中の情報 */
	int extract_size; char* destbuf;
	char* extract_src;
	int src_count, dest_count;
	/* ファイルを mmap している場合、その情報 */
	char* mmapped_memory;
	/* 変換が終了して読めるようになると、バッファの先頭アドレスが代入される */
	char* retbuf;

protected:
	ARCINFO(char* arcf, long offset, int size); // only from ARCFILE
	friend class ARCFILE;
	friend class DIRFILE;
	friend class RaffresiaFILE;

	virtual void Start(void);
	virtual void Reading(void);
	virtual void Extracting(void);
public:
	/* dest は256byte 程度の余裕があること */
	static void Extract(char*& dest, char*& src, char* destend, char* srcend);
public:
	virtual ~ARCINFO();
	/* 必要なら Read 前に呼ぶことで処理を分割できる */
	int Process(void) {
		if (status == INIT) Start();
		else if (status == READ || status == READ_BEFORE_EXTRACT) Reading();
		else if (status == EXTRACT || status == EXTRACT_MMAP) Extracting();
		else if (status == MMAP || status == DONE) return 0;
		if (status == ERROR) return 0;
		return 1;
	}
	int Size(void) {
		if (status == INIT) Process();
		if (status == ERROR) return 0;
		return filesize;
	}
	char* CopyRead(void); /* Read() して内容のコピーを返す */
	const char* Read(void) {
		/* 読み込みが必要ならファイルを読み込み、内容を返す */
		while(retbuf == 0 && status != ERROR) Process();
		return retbuf;
	}
	/* ファイルが regular file の場合、ファイル名を帰す */
	/* そうでないなら 0 を帰す */
	const char* Path(void);
	FILE* OpenFile(int* length=0); /* 互換性のため：raw file の場合、ファイルを開く */
};

class GRPCONV {
public:
	enum EXTRACT_TYPE { EXTRACT_INVALID, EXTRACT_32bpp, EXTRACT_16bpp};

	int width;
	int height;
	int mask;
	const char* filename;
	char* imagebuf;
	char* maskbuf;
	EXTRACT_TYPE imagetype;

	int Width(void) { return width;}
	int Height(void) { return height;}
	int IsMask(void) { return mask;}
	void SetFilename(const char*);

	GRPCONV(void);
	virtual ~GRPCONV();
	void Init(void);
	void SetImage(char* image, EXTRACT_TYPE type);
	void SetMask(char* m);

	virtual bool ReserveRead(EXTRACT_TYPE mode) = 0; // 読み込みの予約をする
	virtual bool Process(void) = 0; // Read / ReadMask の処理を実行する. 最後まで終わったら 0 を返す

	const char* Read(EXTRACT_TYPE mode = EXTRACT_32bpp) {
		// なにかしているなら終了まで待つ
		while(Process()) ;
		// 展開済みの画像があればそれを返す
		if (imagebuf && imagetype == mode) return imagebuf;

		if (ReserveRead(mode) == false) return 0;
		while(Process()) ;
		return imagebuf;
	}
	const char* ReadMask(void) {
		// マスクなしならなにもしない
		if (! mask) return 0;
		// 展開済みの画像があればそれを返す
		if (maskbuf) return maskbuf;
		Read();
		return maskbuf;
	}
	static GRPCONV* AssignConverter(const char* inbuf, int inlen, const char* fname);
	static GRPCONV* AssignConverter(ARCINFO* info) {
		const char* dat = info->Read();
		if (dat == 0) return 0;
		return AssignConverter(dat, info->Size(), "???");
	}
};

extern FILESEARCH file_searcher;

#endif // !defined(__KANON_FILE_H__)
