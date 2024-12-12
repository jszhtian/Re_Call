/*  senario_save.cc
 *     ゲームの保存に関係するメソッドを集めたもの
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
#include<ctype.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<errno.h>
#include<time.h>
#include<vector>
#include<fcntl.h>
#ifdef TM_IN_SYS_TIME
#include<sys/time.h>
#endif

#include<errno.h>
#define SENARIO_TEXTGEOM_MAGIC 0x9f27b34f
#define SENARIO_BACKLOG_MAGIC 0x57a9f9eb

// セーブファイルの名前をつくる
void SENARIO::MakeSaveFile(char* dir) {
	savefname = 0;
	if (dir == 0) return;
	struct stat sstatus;
	if (dir[0] == '~' && dir[1] == '/') {
		char* home = getenv("HOME");
		if (home != 0) {
			char* new_dir = new char[strlen(home)+strlen(dir)];
			strcpy(new_dir, home);
			strcat(new_dir, dir+1);
			dir = new_dir;
		}
	}
	// savepathにファイル名が入っていれば、それをセーブファイルとして使う
	if (stat(dir, &sstatus) == -1) {
		if (errno != ENOENT) {
			fprintf(stderr,"Cannot open save file; dir %s is not directory\n",dir);
			return;
		}
		if (mkdir(dir, S_IRWXU) != 0 && errno != EEXIST) {
			fprintf(stderr, "Cannot create directory %s ; Please create manually!!\n",dir);
		}
	} else {
		if ( (sstatus.st_mode & S_IFMT) == S_IFREG) {
			savefname = new char[strlen(dir)+1];
			strcpy(savefname, dir);
			return;
		}
	}
	// ファイル名を作る
	const char* regname = local_system.config->GetParaStr("#REGNAME");
	const char* fname = local_system.config->GetParaStr("#SAVENAME");
	
	int rlen = strlen(dir)+strlen(fname)+strlen(regname)+80;
	savefname = new char[rlen];
	snprintf(savefname, rlen, "%s/%s-ver%d-%s", dir,
		fname, local_system.Version(), regname
	);
	/* レジストリ名、ファイル名の部分は小文字にする */
	char* tmpbuf = savefname + strlen(dir) + 1;
	while(*tmpbuf != 0) {
		char c = *tmpbuf;
		if (c == '\\' || c == '/' || c == ':' || c <= 0x20) c = '_';
		*tmpbuf++ = tolower(c);
	}
	return;
}

#define CF_BLOCKSIZE 65536
/* 大量の０を書き込む。
** 返り値は書き込んだバイト数
*/
static int ClearOut(int handle, int n) {
	char* buf = new char[CF_BLOCKSIZE];
	int write_size = 0; int all_write_size = 0;
	memset(buf, 0, CF_BLOCKSIZE);

	lseek(handle, 0, 0); ftruncate(handle, 0);

	while(n > CF_BLOCKSIZE) {
		write_size = write(handle, buf, CF_BLOCKSIZE);
		all_write_size += write_size;
		if (write_size != CF_BLOCKSIZE) {
			fprintf(stderr,"Error in ClearOut(); Cannot write file\n");
			delete[] buf; return all_write_size; /* error */
		}
		n -= CF_BLOCKSIZE;
	}
	write_size = write(handle, buf, n);
	all_write_size += write_size;
	delete[] buf;
	return all_write_size;
}

/* src から dest へファイルのコピーを行う */
/* エラーなら dest を消去して -1 を返す */
static int CopyFile(int dest, int src) {
	/* src, dest の初期化 */
	lseek(src,0,0);
	ftruncate(dest, 0);
	char* buf = new char[CF_BLOCKSIZE];
	int read_size, write_size;
	while( (read_size = read(src,buf,CF_BLOCKSIZE)) == CF_BLOCKSIZE) {
		write_size = write(dest, buf, CF_BLOCKSIZE);
		if (write_size != CF_BLOCKSIZE) {
			return -1;
			fprintf(stderr, "Error in CopyFile(); Cannot write file\n");
		}
	}
	write_size = write(dest, buf, read_size);
	if (write_size != read_size) {
		return -1;
		fprintf(stderr, "Error in CopyFile(); Cannot write file\n");
	}
	return 0;
}
#undef CF_BLOCKSIZE

// 一時ファイルなどとして、存在しないファイルをつくる
// ファイル名は basename-<number>

static int CreateTemporaryFile(const char* basename) {
	char* newpath = new char[strlen(basename)+100];
	int i;
	int handle = -1;
	/* ファイル作成 */
	for (i=0; i<1000; i++) {
		sprintf(newpath, "%s-%d",basename,i);
		handle = open(newpath, O_RDWR | O_CREAT | O_EXCL, S_IRUSR|S_IWUSR);
		if (handle == -1 && errno != EEXIST) {
			/* open のエラー */
			perror("Error in CreateTemporaryFile ; Cannot create file. ");
			delete[] newpath;
			return -1;
		}
		if (handle != -1) break;
	}
	delete[] newpath;
	if (handle == -1) {
		fprintf(stderr,"Error in CreateTemporaryFile; "
			"There are too many temporary files in the directory. "
			"Please remove the temporary files, whose names are %s-<number>.",basename);
		return -1;
	}
	return handle;
}

// セーブファイルの存在確認
int SENARIO::IsSavefileExist(void) {
	char buf[1024];
	if (savefname == 0) return 1;
	FILE* f = fopen(savefname, "rb+");
	if (f == 0) return 0; // 存在しない
	fread(buf, 512, 1, f);
	fseek(f, 0, 2); int size = ftell(f);
	fclose(f);
	// ヘッダを確認
	const char* header = local_system.config->GetParaStr("#SAVETITLE");
	buf[strlen(header)] = '\0';
	if (strcmp(buf, header) != 0) return 0;
	// サイズの確認
	int size2 = save_head_size + save_block_size * local_system.config->GetParaInt("#SAVEFILETIME") + save_tail_size;
	if (size == size2) return 1;
	if (size == size2-save_tail_size) {
		// read flag がない場合
		WriteReadFlag();
		return 1;
	}
	return 0;
}


// セーブファイルが存在しないとき、作成する
void SENARIO::CreateSaveFile(void) {
	// save file をつくる
	if (savefname == 0) return;
	// まず、ファイルをつくる
	int handle = open(savefname, O_RDWR | O_CREAT, S_IRUSR|S_IWUSR);
	if (handle == -1) { /* エラー */
		fprintf(stderr,"Error in SENARIO::CreateSaveFile; Cannot open/create file %s; %s\n",savefname,strerror(errno));
		return;
	}
	if (lseek(handle, 0, 2) != 0) { /* ファイルの大きさが 0 以外の時、ファイルをバックアップ */
		int pathlen = strlen(savefname);
		strcat(savefname,".orig");
		int copy_handle = CreateTemporaryFile(savefname);
		savefname[pathlen] = 0;
		/* 新しいファイルにコピー */
		CopyFile(copy_handle, handle);
	}
	// ファイルをゼロ・クリア
	int times = local_system.config->GetParaInt("#SAVEFILETIME");
	int size = save_head_size + save_block_size*times;
	ClearOut(handle, size);

	FILE* out = fdopen(handle,"rb+");
	if (out == 0) return;
	// ヘッダを書く
	fseek(out, 0, 0);
	const char* header = local_system.config->GetParaStr("#SAVETITLE"); char last = 0x1a;
	fwrite(header, strlen(header), 1, out);
	fwrite(&last, 1, 1, out);
	// macro を書く
	fseek(out, 0x11e4, 0);
	macros->Save(out);
	fclose(out);
	// read flag を書く
	WriteReadFlag();
}

// セーブファイルの先頭を読み込む
void SENARIO::ReadSaveHeader(void) {
	if (in_proc & 1) return; // 再入禁止
	if (savefname == 0) return;
	FILE* in = fopen(savefname, "rb");
	if (in == 0) return;

	in_proc |= 1;
	// セーブファイルの先頭を読む
	// まず、ヘッダの読み飛ばし
	fseek(in, 0x80, 0);
	// var を読む
	char* buf = new char[4*1000];
	fread(buf, 1000, 4, in);
	int i; char* buf2 = buf;
	for (i=0; i<1000; i++) {
		flags->SetVar(i+1000, read_little_endian_int(buf2));
		buf2 += 4;
	}
	// bit を読む
	fread(buf, 125, 1, in);
	buf2 = buf; int n = 1000;
	for (i=0; i<125; i++) {
		flags->SetBitGrp(n, *(unsigned char*)buf2);
		buf2++; n += 8;
	}
	// macro を読む
	fseek(in, 0x11e4, 0);
	macros->Load(in);

	delete[] buf;
	fclose(in);
	// 既読フラグを読む
	ReadReadFlag();

	in_proc |= 4; /* save header 情報が有効であることを保証する */
	in_proc &= ~1;
	return;
}

// セーブファイルの先頭を書き込む
void SENARIO::WriteSaveHeader(void) {
	if (in_proc & 2) return; /* 再入禁止 */
	if (!(in_proc & 4)) return; /* save header を読んだことが無ければ write header はしない */
	FILE* out = fopen(savefname, "rb+");
	if (out == 0) return;

	in_proc |= 2;
	// セーブファイルの先頭を書く
	// ヘッダを書く
	fseek(out, 0, 0);
	const char* header = local_system.config->GetParaStr("#SAVETITLE"); char last = 0x1a;
	fwrite(header, strlen(header), 1, out);
	fwrite(&last, 1, 1, out);
	// var を書く
	fseek(out, 0x80, 0);
	char* buf = new char[4*1000];
	int i; char* buf2 = buf;
	for (i=0; i<1000; i++) {
		write_little_endian_int(buf2, flags->GetVar(i+1000));
		buf2 += 4;
	}
	fwrite(buf, 1000, 4, out);
	// bit を書く
	buf2 = buf; int n = 1000;
	for (i=0; i<125; i++) {
		*(unsigned char*)buf2 = flags->GetBitGrp(n);
		buf2++; n += 8;
	}
	fwrite(buf, 125, 1, out);
	
	// macro を書く
	fseek(out, 0x11e4, 0);
	macros->Save(out);

	delete[] buf;
	fclose(out);
	WriteReadFlag();

	in_proc &= ~2;
	return;
}

// SENARIO_MACRO のセーブ・ロード
void SENARIO_MACRO::Save(FILE* out) {
	// バッファを作る
	char* buf = new char[26*16];
	memset(buf, 0, 26*16);

	int len = 26; if (len>macro_deal) len = macro_deal;
	int i; for (i=0; i<len; i++) {
		if (macros[i] != 0)
			strncpy(buf+i*16, (char*)macros[i], 16);
	}
	// 書き込み
	fwrite(buf, 26, 16, out);
	delete[] buf;
	return;
}

void SENARIO_MACRO::Load(FILE* in) {
	char* buf = new char[26*16];
	fread(buf, 26, 16, in);
	// 読んでいく
	int i; for (i=0; i<26; i++) {
		if (buf[i*16] != '\0')
			SetMacro(i, (unsigned char*)buf + i*16);
	}
	delete[] buf;
	return;
}

int SENARIO::IsValidSaveData(int no, char* title) {
	if (no >= local_system.config->GetParaInt("#SAVEFILETIME")) return 0;
	char buf[52];
	FILE* out = fopen(savefname, "rb+");
	if (out == 0) return 0;
	// 先頭を読む
	fseek(out, save_head_size + save_block_size*no, 0);
	fread(buf, 52, 1, out);
	fclose(out);
	if (buf[0] == 0) return 0; // invalid
	// 書く
	sprintf(title, " %02d/%02d(%02d:%02d)  %s",
		buf[4], buf[8], buf[12], buf[16], buf+20);
	return 1;
}

// save data のタイトル部分をつくる
static void MakeSaveDataTitle(char* title, char* ret) {
	memset(ret, 0, 52);
	ret[0] = 1;
	time_t cur_tm = time(0);
	struct tm* split_tm = localtime(&cur_tm);
	ret[4] = split_tm->tm_mon+1;
	ret[8] = split_tm->tm_mday;
	ret[12] = split_tm->tm_hour;
	ret[16] = split_tm->tm_min;
	if (title == 0) {
		strcpy(ret+20, "--------------------");
	} else {
		strncpy(ret+20, title, 30);
		ret[49] = 0;
	}
}

char** SENARIO::ReadSaveTitle(void) {
	int n = local_system.config->GetParaInt("#SAVEFILETIME");
	char** list = new char*[n+1];
	char buf[1024];
	int i; for (i=0; i<n; i++) {
		if (IsValidSaveData(i, buf) == 0) {
			const char* t = local_system.config->GetParaStr("#SAVENOTITLE");
			list[i] = new char[strlen(t)+1];
			strcpy(list[i], t);
		} else {
			list[i] = new char[strlen(buf)+1];
			strcpy(list[i], buf);
		}
	}
	list[n] = 0;
	return list;
}

void SENARIO::WriteSaveFile(int no, char* title) {
	int i; char* buf = new char[8000];
	if (no >= local_system.config->GetParaInt("#SAVEFILETIME")) { delete[] buf; return;}
	// ヘッダを書き出しておく
	WriteSaveHeader();
	// ファイルを開く
	FILE* out = fopen(savefname, "rb+");
	if (out == 0) {delete[] buf; return;}
	fseek(out, save_head_size + save_block_size*no, 0);
	// タイトルを書く
	MakeSaveDataTitle(title, buf);
	fwrite(buf, 0x34, 1, out);
	// 読み飛ばし ：バックログ保存
	char* backlog = new char[0x10a00+0x8e00];
	write_little_endian_int(buf, SENARIO_BACKLOG_MAGIC);
	fwrite(buf, 4, 1, out);
	decoder->BackLog().AddGameSave();
	decoder->BackLog().PutLog(backlog, 0x10a00+0x8e00, 1);
	fwrite(backlog, 0x10a00, 1, out);

	// save する seen などを得る
	int seen=-1, spoint=-1;
	SENARIO_FLAGS* save_flag=0; GosubStack* save_stack=0;
	decoder->BackLog().GetSavePoint(seen,spoint, &save_flag, &save_stack);
	if (save_flag==0) save_flag = flags;
	if (save_stack==0) save_stack = &local_system.CallStack();
	// 変数書き込み
	char* buf2 = buf;
	for (i=0; i<2000; i++) {
		write_little_endian_int(buf2,
			save_flag->GetVar(i));
		buf2 += 4;
	}
	fwrite(buf, 2000, 4, out);
	buf2 = buf;
	for (i=0; i<250; i++) {
		*(unsigned char*)buf2++ =
			save_flag->GetBitGrp(i*8);
	}
	fwrite(buf, 250, 1, out);
	for (i=0; i<100; i++) {
		fwrite(save_flag->StrVar(i), 64, 1, out);
	}
	// seen の書き込み
	memset(buf, 0, 20);
	write_little_endian_int(buf, seen);
	write_little_endian_int(buf+16, spoint);
	fwrite(buf, 20, 1, out);
	// 読み飛ばし・・・buf+61 に特殊な数を入れることで
	// ここに入るべき CDROM track の読み込みをしなくする
	memset(buf, 0, 0x100);
	write_little_endian_int(buf+61, SENARIO_TEXTGEOM_MAGIC);
	if (local_system.Version() == 1) {
		fwrite(buf, 0xf4, 1, out);
	} else { // version 0,2,3
		fwrite(buf, 0x100, 1, out);
		fseek(out, 0x2034, 1);
	}
	// stack の書き込み
	buf2 = buf+4; memset(buf, 0, 0x1404);
	i=0; while(1) {
		GlobalStackItem& item = (*save_stack)[i];
		if (! item.IsValid()) break;
		write_little_endian_int(buf2, item.GetSeen());
		write_little_endian_int(buf2+16, item.GetLocal());
		i++; buf2 += 20;
	}
	write_little_endian_int(buf, i);
	fwrite(buf, 0x1404, 1, out);
	// Graphics Save buffer の書き込み
	// backlog buffer としてつかう
	write_little_endian_int(buf, SENARIO_BACKLOG_MAGIC);
	fwrite(buf, 4, 1, out);
	fwrite(backlog+0x10a00, 0x8e00, 1, out);
	delete[] backlog;

	delete[] buf; fclose(out);
	return;
}

void SENARIO::ReadSaveFile(int no, GlobalStackItem& go) {
	int i;
	if (no >= local_system.config->GetParaInt("#SAVEFILETIME")) return;
	char buf[1024];
	if (IsValidSaveData(no, buf) == 0) return;
	local_system.StopCDROM();
	local_system.StopWave();
	local_system.StopMovie();
	// ヘッダを書き出しておく
	WriteSaveHeader();
	// ファイルを読む
	FILE* in = fopen(savefname, "rb+");
	if (in == 0) return;
	fseek(in, save_head_size + save_block_size*no, 0);
	// タイトルを読む
	fread(buf, 0x34, 1, in); local_system.SetTitle(buf+20);
	// 読み飛ばし ：バックログ保存
	/*
	**fseek(in, 4, 1);
	**fseek(in, 0x10a00, 1);
	*/
	char* backlog = new char[0x10a00 + 0x8e00];
	int backlog_len = 0;
	decoder->BackLog().ClearLog();
	fread(buf, 4, 1, in); unsigned int magic = read_little_endian_int(buf);
	if (magic == SENARIO_BACKLOG_MAGIC) {
		fread(backlog, 0x10a00, 1, in);
		backlog_len += 0x10a00;
	} else {
		fseek(in, 0x10a00, 1);
	}

	// 変数読み込み
	char* vbuf = new char[8000]; char* vbuf2 = vbuf;
	fread(vbuf, 2000, 4, in);
	for (i=0; i<2000; i++) {
		flags->SetVar(i, read_little_endian_int(vbuf2));
		vbuf2 += 4;
	}
	fread(vbuf, 250, 1, in); vbuf2 = vbuf; int n = 0;
	for (i=0; i<250; i++) {
		unsigned char c = *(unsigned char*)vbuf2;
		flags->SetBitGrp(n, c);
		vbuf2++; n+=8;
	}
	fread(vbuf, 100, 64, in); vbuf2 = vbuf;
	for (i=0; i<100; i++) {
		flags->SetStrVar(i, vbuf2);
		vbuf2 += 64;
	}
	// seen の読みこみ
	fread(vbuf, 0x14, 1, in);
	go.SetGlobal( read_little_endian_int(vbuf),
		read_little_endian_int(vbuf+16) );
	// 読み飛ばし
	if (local_system.Version() == 1) {
		fread(vbuf, 0xf4, 1, in);
	} else { // version 0,2,3
		fread(vbuf, 0x100, 1, in);
		fseek(in, 0x2034, 1);
	}
	if ( (unsigned int)(read_little_endian_int(vbuf+61)) == SENARIO_TEXTGEOM_MAGIC) {
	} else {
		char cdrom_track[128];
		if (local_system.Version() == 1) {
			memcpy(cdrom_track, vbuf+0xa1, 100);
		} else { // version 0,2,3
			memcpy(cdrom_track, vbuf+0xc4, 100);
		}
		if (cdrom_track[0] != '\0') local_system.PlayCDROM(cdrom_track);
		else local_system.StopCDROM();
	}
	if ( (unsigned int)(read_little_endian_int(vbuf)) == SENARIO_TEXTGEOM_MAGIC) {
		int x1,y1,x2,y2,x3,y3,w; vbuf[60] = 0;
		sscanf(vbuf+4, "%d,%d,%d,%d,%d,%d,%d",
			&x1,&y1,&x2,&y2,&x3,&y3,&w);
		local_system.config->SetParam("#WINDOW_MSG_POS", 2, x1, y1);
		local_system.config->SetParam("#MESSAGE_SIZE", 2, x2, y2);
		local_system.config->SetParam("#MSG_MOJI_SIZE", 2, x3/2, y3);
		local_system.config->SetParam("#NVL_SYSTEM", 1, w);
	}
	// stack の読み込み
	// stack をクリア
	while(local_system.CallStack().PopStack().IsValid()) ;
	// stack をつくる
	fread(vbuf, 0x1404, 1, in);
	n = read_little_endian_int(vbuf); vbuf2 = vbuf+4;
	for (i=0; i<n; i++) {
		int seen = read_little_endian_int(vbuf2 + 20*i);
		int point = read_little_endian_int(vbuf2+ 20*i + 16);
		local_system.CallStack().PushStack().SetGlobal(seen, point);
	}
	// Graphics Save buffer の読み込み
	fread(vbuf, 4, 1, in); magic = read_little_endian_int(vbuf);
	if (magic == SENARIO_BACKLOG_MAGIC) {
		fread(backlog+backlog_len, 0x8e00, 1, in);
		backlog_len += 0x8e00;
		/* 画面を消す */
		local_system.ClearPDTBuffer(0,0,0,0);
		grpsave->ClearBuffer(); /* savebuf のクリア */
	} else {
		char* gbuf = new char[0x8e00 + 4];
		fseek(in,-4,1);
		fread(gbuf, 0x8e00+4, 1, in);
		grpsave->Load(gbuf);
		grpsave->Restore();
		delete[] gbuf;
	}
	if (backlog_len) decoder->BackLog().SetLog(backlog, backlog_len, 1);
	delete[] backlog;
	// header を読み直す
	ReadSaveHeader();
	// 普通の状態にする
	local_system.DeleteMouse();
	local_system.DeleteReturnCursor();
	local_system.DeleteText();
	local_system.DeleteTextWindow();
	local_system.ShowMenuItem("Load", 1);
	local_system.ShowMenuItem("Save", 1);
	local_system.ShowMenuItem("SkipText", 1);
	local_system.ShowMenuItem("AutoSkipText", 1);
	local_system.ShowMenuItem("GoMenu", 1);
	local_system.SetKidoku();
	local_system.StopTextSkip();
	local_system.SetTextFastMode(false);
	local_system.SetTextAutoMode(false);
	// 画面を描く
	local_system.DrawMouse();
	// backlog を開始
	decoder->BackLog().StartLog(1);

	delete[] vbuf;
	fclose(in);
	return;
}

// SENARIO_Graphics のファイル保存/復帰
int SENARIO_Graphics::Load(char* mem) {
	ClearBuffer();
	int n = read_little_endian_int(mem); mem += 4;
	int i; for (i=0; i<n; i++) {
		buf[i].Load(mem);
		mem += 0x470;
	}
	deal = n;
	Change();
	return 0x470*32+4;
}

void SENARIO_Graphics::Save(FILE* out) {
	int point = ftell(out);
	char mem[4]; write_little_endian_int(mem, deal);
	fwrite(mem, 4, 1, out); point += 4;
	int i;for (i=0; i<deal; i++) {
		fseek(out, point + i*0x470, 0);
		buf[i].Save(out);
	}
	return;
}

int SENARIO_GraphicsSaveBuf::Load(char* buf) {
	cmd = read_little_endian_int(buf); buf += 4;
	filedeal = read_little_endian_int(buf); buf += 4;
	memcpy(filenames, buf, 0x200); buf += 0x200;
	int i; for (i=0; i<24; i++) {
		args[i] = read_little_endian_int(buf);
		buf += 4;
	}
	arg2 = read_little_endian_int(buf); buf += 4;
	arg3 = read_little_endian_int(buf); buf += 4;
	for (i=0; i<64; i++) {
		arg4[i] = read_little_endian_int(buf);
		buf += 4;
	}
	return 0x470;
}

void SENARIO_GraphicsSaveBuf::Save(FILE* out) {
	char buf[0x470]; char* buf2 = buf;
	memset(buf, 0, 0x470);
	write_little_endian_int(buf2, cmd); buf2 += 4;
	write_little_endian_int(buf2, filedeal); buf2 += 4;
	memcpy(buf2, filenames, 0x200); buf2 += 0x200;
	int i; for (i=0; i<24; i++) {
		write_little_endian_int(buf2, args[i]);
		buf2 += 4;
	}
	write_little_endian_int(buf2, arg2); buf2 += 4;
	write_little_endian_int(buf2, arg3); buf2 += 4;
	for (i=0; i<64; i++) {
		write_little_endian_int(buf2, arg4[i]);
		buf2 += 4;
	}
	fwrite(buf, 0x470, 1, out);
}

void SENARIO::ClearReadFlag(void) {
	int i;
	if (local_system.Version() < 3) return;
	for (i=0; i<MAX_SEEN_NO; i++)
		read_flag_table[i] = -1;
	for (i=0; i<MAX_READ_FLAGS; i++)
		read_flags[i] = 0;
	max_read_flag_number = 0;
}

void SENARIO::WriteReadFlag(void) {
	if (local_system.Version() < 3) return;
	char* buf_orig = new char[(2+MAX_SEEN_NO+MAX_READ_FLAGS)*INT_SIZE];
	int i; char* buf = buf_orig;
	FILE* f = fopen(savefname, "rb+");
	if (f == 0) return; // 存在しない
	int point = save_head_size + save_block_size * local_system.config->GetParaInt("#SAVEFILETIME");
	fseek(f, point, 0);
	if (ftell(f) != point) {fclose(f);return;} // point 移動に失敗したら return
	// 書き込み
	int magic = READ_FLAG_MAGIC;
	write_little_endian_int(buf, magic); buf += INT_SIZE;
	write_little_endian_int(buf, max_read_flag_number); buf += INT_SIZE;
	for (i=0; i<MAX_SEEN_NO; i++) {
		write_little_endian_int(buf, read_flag_table[i]);
		buf += INT_SIZE;
	}
	for (i=0; i<MAX_READ_FLAGS; i++) {
		write_little_endian_int(buf, read_flags[i]);
		buf += INT_SIZE;
	}
	fwrite(buf_orig, INT_SIZE, 2+MAX_SEEN_NO+MAX_READ_FLAGS, f);
	// 終了
	fclose(f);
	return;
}

void SENARIO::ReadReadFlag(void) {
	if (local_system.Version() < 3) return;
	char* buf_orig = new char[(2+MAX_SEEN_NO+MAX_READ_FLAGS)*INT_SIZE];
	char* buf = buf_orig; int i;
	FILE* f = fopen(savefname, "rb+");
	if (f == 0) return; // 存在しない
	int point = save_head_size + save_block_size * local_system.config->GetParaInt("#SAVEFILETIME");
	fseek(f, point, 0);
	if (ftell(f) != point) {fclose(f); return;} // point 移動に失敗したら return
	// 読み込み
	fread(buf_orig, INT_SIZE, 2+MAX_SEEN_NO+MAX_READ_FLAGS, f);
	unsigned int magic = read_little_endian_int(buf);
	if (magic != READ_FLAG_MAGIC) { fclose(f); return; } // magic がおかしければ return
	buf += INT_SIZE;
	max_read_flag_number = read_little_endian_int(buf); buf += INT_SIZE;
	for (i=0; i<MAX_SEEN_NO; i++) {
		read_flag_table[i] = read_little_endian_int(buf);
		buf += INT_SIZE;
	}
	for (i=0; i<MAX_READ_FLAGS; i++) {
		read_flags[i] = read_little_endian_int(buf);
		buf += INT_SIZE;
	}
	// 終了
	fclose(f);
	return;
}
