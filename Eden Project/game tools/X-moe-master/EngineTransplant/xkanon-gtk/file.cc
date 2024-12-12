/*  file.cc  : KANON の圧縮ファイル・PDT ファイル（画像ファイル）の展開の
 *            ためのメソッド
 *     class ARCFILE : 書庫ファイル全体を扱うクラス
 *     class ARCINFO : 書庫ファイルの中の１つのファイルを扱うクラス
 *     class PDTCONV : PDT ファイルの展開を行う。
 *
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

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif
#include "file.h"
#include <ctype.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <vector>
#include <string>
#include <algorithm>
#if HAVE_MMAP
#include<sys/mman.h>
#endif /* HAVE_MMAP */
#if HAVE_DIRENT_H
# include <dirent.h>
# define NAMLEN(dirent) strlen((dirent)->d_name)
#else
# define dirent direct
# define NAMLEN(dirent) (dirent)->d_namlen
# if HAVE_SYS_NDIR_H
#  include <sys/ndir.h>
# endif
# if HAVE_SYS_DIR_H
#  include <sys/dir.h>
# endif
# if HAVE_NDIR_H
#  include <ndir.h>
# endif
#endif

using namespace std;

#define PROCBLKSIZ 65536 /* Process() で処理する１ブロックの大きさ */
#define EVENT_PROCESS_COUNT 100000 /* PDT の Extract １回で処理するブロックの大きさ */

FILESEARCH file_searcher;
// #define delete fprintf(stderr,"file.cc: %d.",__LINE__), delete

/* FILESEARCH class の default の振る舞い */
FILESEARCH::ARCTYPE FILESEARCH::default_is_archived[TYPEMAX] = {
	ATYPE_DIR, ATYPE_DIR, ATYPE_DIR, ATYPE_DIR,
	ATYPE_ARC, ATYPE_ARC, ATYPE_ARC, ATYPE_ARC,
	ATYPE_DIR, ATYPE_DIR, ATYPE_DIR, ATYPE_DIR,
	ATYPE_DIR
};
char* FILESEARCH::default_dirnames[TYPEMAX] = {
	0, 0, "", "pdt", 
	"seen.txt", "allanm.anl", "allard.ard", "allcur.cur", 
	0, 0, "koe", "bgm", "mov"};

/*********************************************
**  ARCFILE / DIRFILE:
**	書庫ファイル、あるいはディレクトリの
**	全体を管理するクラス
**
**	書庫ファイルからファイルの抜き出しはFind()
**	Find したものをReadすると内容が得られる。
*/

struct ARCFILE_ATOM {
	string filename;
	string filename_lower;
	int offset;
	int arcsize;
	int filesize;
	void setname(const char* s) {
		char lower[1024];
		filename = s;
		int i; for (i=0; i<1000&&s[i]!=0; i++) lower[i] = tolower(s[i]);
		lower[i] = 0;
		filename_lower = lower;
	}
	bool operator <(const ARCFILE_ATOM& to) const {
		return filename_lower < to.filename_lower;
	}
	bool operator <(const string& to) const {
		return filename_lower < to;
	}
};

class ARCFILE {
protected:
	char* arcname;
	int list_point;
	vector<ARCFILE_ATOM> arc_atom;
	ARCFILE* next; /* FILESEARCH の一つの型が複数の ARCFILE を持つとき、リストをつくる */
	/* arcname に指定されたファイル／ディレクトリの内容チェック */
	virtual int CheckFileDeal(void);
	virtual void ListupFiles(void);
	virtual ARCINFO* MakeARCINFO(ARCFILE_ATOM*);
	ARCFILE_ATOM* SearchName(const char* f, const char* ext=0);
public:
	ARCFILE(char* fname);
	void SetNext(ARCFILE* _next) { next = _next;}
	ARCFILE* Next(void) { return next; }
	void Init(void);
	virtual ~ARCFILE();
	/* ファイル検索 */
	class ARCINFO* Find(const char* fname, const char* ext);
	/* ファイルリストの出力 */
	int Deal(void) { Init(); return arc_atom.size(); }
	void ListFiles(FILE* out);
	void InitList(void);
	char* ListItem(void);
};

class SCN2kFILE : public ARCFILE {
protected:
	virtual int CheckFileDeal(void);
	virtual void ListupFiles(void);
	virtual ARCINFO* MakeARCINFO(ARCFILE_ATOM* atom);
public:
	SCN2kFILE(char* fname) : ARCFILE(fname) {}
	virtual ~SCN2kFILE() {}
};

class RaffresiaFILE : public ARCFILE {
protected:
	virtual int CheckFileDeal(void);
	virtual void ListupFiles(void);
	virtual ARCINFO* MakeARCINFO(ARCFILE_ATOM* atom);
public:
	RaffresiaFILE(char* fname) : ARCFILE(fname) {}
	virtual ~RaffresiaFILE() {}
};

class NULFILE : public ARCFILE {
protected:
	virtual int CheckFileDeal(void);
	virtual void ListupFiles(void);
	virtual ARCINFO* MakeARCINFO(ARCFILE_ATOM* atom);
public:
	NULFILE() : ARCFILE("") {}
	virtual ~NULFILE() {}
};
class DIRFILE : public ARCFILE {
protected:
	virtual int CheckFileDeal(void);
	virtual void ListupFiles(void);
	virtual ARCINFO* MakeARCINFO(ARCFILE_ATOM* atom);
public:
	DIRFILE(char* fname) : ARCFILE(fname) {}
	virtual ~DIRFILE() {}
	FILE* Open(const char* fname); /* FILE* を開く */
	char* SearchFile(const char* dirname); /* ファイル検索 */
};
class ARCINFO2k : public ARCINFO {
	int header_size;
	int header_version;
	int data_size;
	int compdata_size;
	int decode_count;
	static char decode_seed[256];
protected:
	ARCINFO2k(char* arcf, long offset, int size) : ARCINFO(arcf, offset,size) {
		header_size = 0;
		header_version = 0;
		data_size = 0;
		compdata_size = 0;
		decode_count = 0;
	}
	virtual void Start(void);
	virtual void Extracting(void);
	friend class SCN2kFILE;
};


ARCFILE::ARCFILE(char* aname) {
	struct stat sb;
	/* 変数初期化 */
	arcname = 0;
	list_point = 0;
	next = 0;
	if (aname[0] == '\0') {arcname=new char[1]; arcname[0]='\0';return;} // NULFILE
	/* ディレクトリか否かのチェック */
	if (stat(aname,&sb) == -1) { /* error */
		perror("stat");
	}
	if ( (sb.st_mode&S_IFMT) == S_IFDIR) {
		int l = strlen(aname);
		arcname = new char[l+2]; strcpy(arcname, aname);
		if (arcname[l-1] != DIR_SPLIT) {
			arcname[l] = DIR_SPLIT;
			arcname[l+1] = 0;
		}
	} else if ( (sb.st_mode&S_IFMT) == S_IFREG) {
		arcname = new char[strlen(aname)+1];
		strcpy(arcname,aname);
		if (arcname[strlen(arcname)-1] == DIR_SPLIT)
			arcname[strlen(arcname)-1] = '\0';
	}
	return;
}
void ARCFILE::Init(void) {
	if (! arc_atom.empty()) return;
	if (arcname == 0) return;
	/* ファイル数を得る */
	CheckFileDeal();
	/* ファイル名のセット */
	ListupFiles();
	sort(arc_atom.begin(), arc_atom.end());
}
ARCFILE::~ARCFILE() {
	delete[] arcname;
}

ARCFILE_ATOM* ARCFILE::SearchName(const char* f, const char* ext) {
	char buf[1024]; char buf_ext[1024];
	vector<ARCFILE_ATOM>::iterator it;
	Init();
	if (arc_atom.empty()) return 0;
	/* エラーチェック */
	if (strlen(f)>500) return 0;
	if (ext && strlen(ext)>500) return 0;

	/* まず小文字にする */
	int i; int l = strlen(f);
	if (l > 500) l = 500;
	for (i=0; i<l; i++)
		buf[i] = tolower(f[i]);
	buf[i++] = 0;
	string str = buf;
	/* 検索 */
	it = lower_bound(arc_atom.begin(), arc_atom.end(), str);
	if (it != arc_atom.end() && it->filename_lower == str)
		return &arc_atom[it-arc_atom.begin()];

	/* 見付からない場合 */
	/* 拡張子を ext にしたファイル名の作成 */
	strcpy(buf_ext, buf);
	char* ext_pt = strrchr(buf_ext, '.');
	if (ext_pt == 0 || ext_pt == buf_ext) ext_pt = buf_ext + strlen(buf_ext);
	*ext_pt++ = '.';
	while(ext && *ext) {
		/* 拡張子の長さを得る */
		while(*ext == '.') ext++;
		if (*ext == 0) break;
		if (strchr(ext, '.')) l = strchr(ext,'.') - ext;
		else l = strlen(ext);
		for (i=0; i<l; i++)
			ext_pt[i] = tolower(*ext++);
		ext_pt[i] = 0;
		str = buf_ext;
		it = lower_bound(arc_atom.begin(), arc_atom.end(), str);
		if (it != arc_atom.end() && it->filename_lower == str)
			return &arc_atom[it-arc_atom.begin()];
	}
	return 0;
}

ARCINFO* ARCFILE::Find(const char* fname, const char* ext) {
	Init();
	ARCFILE_ATOM* atom = SearchName(fname,ext);
	if (atom == 0) {
		if (next) return next->Find(fname, ext);
		else return 0;
	}
	return MakeARCINFO(atom);
}
ARCINFO* ARCFILE::MakeARCINFO(ARCFILE_ATOM* atom) {
	return new ARCINFO(arcname, atom->offset, atom->arcsize);
}
ARCINFO* NULFILE::MakeARCINFO(ARCFILE_ATOM* atom) {
	fprintf(stderr,"NULFILE::MakeARCINFO is invalid call!\n");
	return 0;
}
ARCINFO* SCN2kFILE::MakeARCINFO(ARCFILE_ATOM* atom) {
	return new ARCINFO2k(arcname, atom->offset, atom->arcsize);
}
ARCINFO* RaffresiaFILE::MakeARCINFO(ARCFILE_ATOM* atom) {
	return new ARCINFO(arcname, atom->offset, atom->arcsize);
}
ARCINFO* DIRFILE::MakeARCINFO(ARCFILE_ATOM* atom) {
	const char* name = atom->filename.c_str();
	char* new_path = new char[strlen(arcname)+strlen(name)+1];
	strcpy(new_path,arcname); strcat(new_path, name);
	ARCINFO* ret = new ARCINFO(new_path, atom->offset, atom->arcsize);
	delete[] new_path;
	return ret;
}

FILE* DIRFILE::Open(const char* fname) {
	ARCFILE_ATOM* atom = SearchName(fname);
	if (atom == 0) return 0;
	const char* name = atom->filename.c_str();
	// make FILE*
	char* new_path = new char[strlen(arcname)+strlen(name)+1];
	strcpy(new_path,arcname); strcat(new_path, name);
	FILE* ret = fopen(new_path, "rb+");
	fseek(ret, 0, 0);
	delete[] new_path;
	return ret;
}

char* DIRFILE::SearchFile(const char* fname) {
	ARCFILE_ATOM* atom = SearchName(fname);
	if (atom == 0) return 0;
	const char* name = atom->filename.c_str();
	char* new_path = new char[strlen(arcname)+strlen(name)+1];
	strcpy(new_path,arcname); strcat(new_path, name);
	struct stat sb;
	if (stat(new_path, &sb) == 0 &&
		( (sb.st_mode&S_IFMT) == S_IFREG ||
		  (sb.st_mode&S_IFMT) == S_IFDIR)) {
		return new_path;
	}
	delete[] new_path;
	return 0;
}

void ARCFILE::ListFiles(FILE* out) {
	Init();
	if (arc_atom.empty()) return;
	// list file name...
	fprintf(out,"%16s %10s %10s %10s\n", "Filename", 
		"pointer","arcsize", "filesize");
	vector<ARCFILE_ATOM>::iterator it;
	for (it=arc_atom.begin(); it!=arc_atom.end(); it++) {
		fprintf(out,"%16s %10d %10d %10d\n",
			it->filename.c_str(),it->offset,it->arcsize,it->filesize);
	}
	return;
}

void ARCFILE::InitList(void) {
	Init();
	list_point = 0;
}
char* ARCFILE::ListItem(void) {
	if (list_point < 0) return 0;
	if (list_point >= int(arc_atom.size())) return 0;
	const char* fname = arc_atom[list_point].filename.c_str();
	if (fname == 0 || fname[0] == 0) return 0;
	char* ret = new char[strlen(fname)+1];
	strcpy(ret, fname);
	list_point++;
	return ret;
}

int ARCFILE::CheckFileDeal(void) {
	char buf[0x20];
	/* ヘッダのチェック */
	FILE* stream = fopen(arcname, "rb");
	if (stream == 0) {
		fprintf(stderr, "Cannot open archive file : %s\n",arcname);
		return 0;
	}
	fseek(stream, 0, 2); size_t arc_size = ftell(stream);
	fseek(stream, 0, 0);
	if (arc_size < 0x20) {
		fclose(stream);
		return 0;
	}
	fread(buf, 0x20, 1, stream);
	if (strncmp(buf, "PACL", 4) != 0) {
		fclose(stream);
		return 0;
	}
	int len = read_little_endian_int(buf+0x10);
	if (arc_size < size_t(0x20 + len*0x20)) {
		fclose(stream);
		return 0;
	}
	int i; int slen = 0;
	for (i=0; i<len; i++) {
		fread(buf, 0x20, 1, stream);
		slen += strlen(buf)+1;
	}
	fclose(stream);
	return slen;
}
void ARCFILE::ListupFiles(void) {
	int i; char fbuf[0x20];
	FILE* stream = fopen(arcname, "rb");
	if (stream == 0) {
		fprintf(stderr, "Cannot open archive file : %s\n",arcname);
		return;
	}
	fread(fbuf,0x20,1,stream);
	int len = read_little_endian_int(fbuf+0x10);
	ARCFILE_ATOM atom;
	for (i=0; i<len; i++) {
		fread(fbuf, 0x20, 1, stream);
		atom.offset = read_little_endian_int(fbuf+0x10);
		atom.arcsize = read_little_endian_int(fbuf+0x14);
		atom.filesize = read_little_endian_int(fbuf+0x18);
		atom.setname(fbuf);
		arc_atom.push_back(atom);
	}
	fclose(stream);
	return;
}
int DIRFILE::CheckFileDeal(void) {
	DIR* dir; struct dirent* ent;
	int flen = 0;
	dir = opendir(arcname);
	if (dir == 0) {
		fprintf(stderr, "Cannot open dir file : %s\n",arcname);
		return 0;
	}
	int count = 0;
	while( (ent = readdir(dir)) != NULL) {
		count++;
		flen += strlen(ent->d_name)+1;
	}
	closedir(dir);
	return flen;
}
void DIRFILE::ListupFiles(void) {
	DIR* dir;
	dir = opendir(arcname);
	if (dir == 0) { 
		fprintf(stderr, "Cannot open dir file : %s\n",arcname);
		return;
	}
	/* 一時的に arcname のディレクトリに移動する */
	int old_dir_fd = open(".",O_RDONLY);
	if (old_dir_fd < 0) {
		closedir(dir);
		return;
	}
	if (chdir(arcname) != 0) {
		fprintf(stderr, "Cannot open dir file : %s\n",arcname);
		closedir(dir);
		close(old_dir_fd);
		return;
	};
	
	ARCFILE_ATOM atom;
	struct stat sb;
	struct dirent* ent;
	while( (ent = readdir(dir)) != NULL) {
		if (stat(ent->d_name, &sb) == -1) continue;
		if ( (sb.st_mode & S_IFMT) == S_IFREG) {
			atom.offset = 0;
			atom.arcsize = sb.st_size;
			atom.filesize = sb.st_size;
		} else if ( (sb.st_mode & S_IFMT) == S_IFDIR) {
			atom.offset = 0;
			atom.arcsize = atom.filesize = 0;
		} else {
			continue;
		}
		atom.setname(ent->d_name);
		arc_atom.push_back(atom);
	}
	/* chdir() したのを元に戻る */
	closedir(dir);
	fchdir(old_dir_fd); close(old_dir_fd);
	return;
}
int NULFILE::CheckFileDeal(void) {
	return 20;
}
void NULFILE::ListupFiles(void) {
	ARCFILE_ATOM atom;
	atom.offset = 0; atom.arcsize = 0; atom.filesize = 0;
	atom.setname("** null dummy **");
	arc_atom.push_back(atom);
}
int SCN2kFILE::CheckFileDeal(void) {
	/* ヘッダのチェック */
	FILE* stream = fopen(arcname, "rb");
	if (stream == 0) {
		fprintf(stderr, "Cannot open archive file : %s\n",arcname);
		return 0;
	}
	fseek(stream, 0, 2); size_t arc_size = ftell(stream);
	fseek(stream, 0, 0);
	if (arc_size < 10000*8) {
		fclose(stream);
		return 0;
	}
	char* buf = new char[10000*8];
	fread(buf, 10000, 8, stream);
	/* size == 0 のデータは存在しない */
	int count = 0;
	int i; for (i=0; i<10000; i++) {
		int tmp_offset = read_little_endian_int(buf+i*8);
		int tmp_size = read_little_endian_int(buf+i*8+4);
		if (tmp_size <= 0 || tmp_offset < 0 || tmp_offset+tmp_size > int(arc_size) ) continue;
		count++;
	}
	fclose(stream);
	delete[] buf;
	return count*13; /* ファイル名は seenXXXX.txt だから、一つ12文字+null */
}
void SCN2kFILE::ListupFiles(void) {
	char tmp_fname[100];
	FILE* stream = fopen(arcname, "rb");
	if (stream == 0) {
		fprintf(stderr, "Cannot open archive file : %s\n",arcname);
		return;
	}
	char* buf = new char[10000*8];
	fread(buf, 10000, 8, stream);
	fseek(stream, 0, 2); size_t arc_size = ftell(stream);
	ARCFILE_ATOM atom;
	int i; for (i=0; i<10000; i++) {
		char header[0x200];
		int tmp_offset = read_little_endian_int(buf+i*8);
		int tmp_size = read_little_endian_int(buf+i*8+4);
		if (tmp_size <= 0 || tmp_offset < 0 || tmp_offset+tmp_size > int(arc_size) ) continue;
		/* header を得て圧縮形式などを調べる */
		fseek(stream, tmp_offset, 0);
		fread(header, 0x200, 1, stream);
		int header_top = read_little_endian_int(header+0);
		int file_version = read_little_endian_int(header+4);

		if (file_version != 0x2712) continue; /* system version が違う */

		if (header_top == 0x1cc) { /* 古い形式 : avg2000 */
			int header_size = read_little_endian_int(header+0)+read_little_endian_int(header+0x20)*4;
			int data_size = read_little_endian_int(header+0x24);
			atom.arcsize = data_size + header_size;
			atom.filesize = data_size + header_size;

		} else if (header_top == 0x1b8) { /* 初夜献上 */
			int header_size = read_little_endian_int(header+0)+read_little_endian_int(header+0x08)*4;
			int data_size = read_little_endian_int(header+0x0c);
			int compdata_size = read_little_endian_int(header+0x10);
			atom.arcsize = compdata_size + header_size;
			atom.filesize = data_size + header_size;
			
		} else if (header_top == 0x1d0) { /* 新しい形式： reallive */
			int header_size = read_little_endian_int(header+0x20);
			int data_size = read_little_endian_int(header+0x24);
			int compdata_size = read_little_endian_int(header+0x28);
			atom.arcsize = compdata_size + header_size;
			atom.filesize = data_size + header_size;
		} else {
			fprintf(stderr,"invalid header top; %x : not supported\n",header_top);
			continue; /* サポートしない形式 */
		}

		atom.offset = tmp_offset;
		sprintf(tmp_fname, "seen%04d.txt",i);
		atom.setname(tmp_fname);
		arc_atom.push_back(atom);
	}
	fclose(stream);
	return;
}
int RaffresiaFILE::CheckFileDeal(void) {
	/* ヘッダのチェック */
	FILE* stream = fopen(arcname, "rb");
	if (stream == 0) {
		fprintf(stderr, "Cannot open archive file : %s\n",arcname);
		return 0;
	}
	fseek(stream, 0, 2); size_t arc_size = ftell(stream);
	fseek(stream, 0, 0);
	char header[32];
	char magic[8] = {'C','A','P','F',1,0,0,0};
	if (fread(header, 1, 32, stream) != 32 || memcmp(magic, header, 8) != 0) {
		fprintf(stderr, "Invalid archive header : %s\n",arcname);
		return 0;
	}
	int index_deal = read_little_endian_int(header+12);
	int index_size = read_little_endian_int(header+8);
	if (index_size != 0x20 + index_deal * 40 || index_size+0x20 > int(arc_size) ) {
		fprintf(stderr, "Invalid archive header length : %s\n",arcname);
		return 0;
	}
	fclose(stream);
	return index_deal*64;
}
void RaffresiaFILE::ListupFiles(void) {
	FILE* stream = fopen(arcname, "rb");
	if (stream == 0) {
		fprintf(stderr, "Cannot open archive file : %s\n",arcname);
		return;
	}
	fseek(stream, 0, 2); size_t arc_size = ftell(stream);
	fseek(stream, 0, 0);

	char header[32];
	fread(header, 1, 32, stream);
	int index_deal = read_little_endian_int(header+12);
	int index_size = read_little_endian_int(header+8);
	char* buf = new char[index_size];

	fread(buf, 1, index_size, stream);
	ARCFILE_ATOM atom;
	int i; for (i=0; i<index_deal; i++) {
		int tmp_offset = read_little_endian_int(buf + i*40 + 0);
		int tmp_size = read_little_endian_int(buf + i*40 + 4);
		if (tmp_size <= 0 || tmp_offset < index_size ||
			tmp_offset+tmp_size > int(arc_size) ) continue;
		atom.arcsize = tmp_size;
		atom.filesize = tmp_size;
		atom.offset = tmp_offset;
		atom.setname(buf + i*40 + 8);
		arc_atom.push_back(atom);
	}
	fclose(stream);
	return;
}

/********************************************************
** FILESEARCH クラスの実装
*/

FILESEARCH::FILESEARCH(void) {
	int i;
	root_dir = 0; dat_dir = 0;
	for (i=0; i<TYPEMAX; i++) {
		searcher[i] = 0;
		filenames[i] = default_dirnames[i];
		is_archived[i] = default_is_archived[i];
	}
}
FILESEARCH::~FILESEARCH(void) {
	int i;
	for (i=0; i<TYPEMAX; i++) {
		if (filenames[i] != 0 && filenames[i] != default_dirnames[i]) delete[] filenames[i];
		if (searcher[i] && searcher[i] != dat_dir && searcher[i] != root_dir) delete searcher[i];
	}
	if (dat_dir && dat_dir != root_dir) delete dat_dir;
	if (root_dir) delete root_dir;
}

int FILESEARCH::InitRoot(char* root) {
	/* 必要に応じて ~/ を展開 */
	if (root[0] == '~' && root[1] == '/') {
		char* home = getenv("HOME");
		if (home != 0) {
			char* new_root = new char[strlen(home)+strlen(root)];
			strcpy(new_root, home);
			strcat(new_root, root+1);
			root = new_root;
		}
	}
	/* 古いデータを消す */
	int i;
	for (i=0; i<TYPEMAX; i++) {
		if (searcher[i] != 0 &&
			searcher[i] != root_dir &&
			searcher[i] != dat_dir) delete searcher[i];
		searcher[i] = 0;
	}
	if (dat_dir && root_dir != dat_dir) delete dat_dir;
	if (root_dir) delete root_dir;
	dat_dir = 0;

	/* 新しいディレクトリのもとで初期化 */
	root_dir = new DIRFILE(root);
	root_dir->Init();
	/* dat/ を検索 */
	char* dat_path = root_dir->SearchFile("dat");
	if (dat_path == 0) {
		/* 見つからなかったら root を dat の代わりにつかう */
		dat_dir = root_dir;
	} else {
		dat_dir = new DIRFILE(dat_path);
		dat_dir->Init();
	}
	searcher[ALL] = dat_dir;
	searcher[ROOT] = root_dir;
	return 0;
}

void FILESEARCH::SetFileInformation(FILETYPE tp, ARCTYPE is_arc, char* filename) {
	int type = tp;
	if (type < 0 || type >= TYPEMAX) return;
	ARCFILE* next_arc = 0;
	/* すでに searcher が存在すれば解放 */
	if (searcher[type] != 0 &&
	  searcher[type] != root_dir &&
	  searcher[type] != dat_dir) {
		next_arc = searcher[type]->Next();
		delete searcher[type];
	}
	searcher[type] = 0;
	/* 適当に初期化 */
	if (filenames[type] != 0 &&
		filenames[type] != default_dirnames[type]) delete[] filenames[type];
	filenames[type] = new char[strlen(filename)+1];
	strcpy(filenames[type], filename);
	is_archived[type] = is_arc;
	searcher[type] = MakeARCFILE(is_arc, filename);
	if (searcher[type] && next_arc)
		searcher[type]->SetNext(next_arc);
	return;
}
void FILESEARCH::AppendFileInformation(FILETYPE tp, ARCTYPE is_arc, char* filename) {
	int type = tp;
	if (type < 0 || type >= TYPEMAX) return;
	/* searcher がまだ割り当てられてない場合 */
	if (searcher[type] == 0 ||
	  searcher[type] == root_dir ||
	  searcher[type] == dat_dir) {
		searcher[type] = MakeARCFILE(is_archived[type], filenames[type]);
		if (searcher[type] == 0) { /* 作成できなかった場合 */
			/* この型情報を FileInformation とする */
			SetFileInformation(tp, is_arc, filename);
			return;
		}
	}
	/* 初期化 */
	ARCFILE* arc = MakeARCFILE(is_arc, filename);
	/* append */
	ARCFILE* cur;
	for (cur=searcher[type]; cur->Next() != 0; cur = cur->Next()) ;
	cur->SetNext(arc);
	return;
}

ARCFILE* FILESEARCH::MakeARCFILE(ARCTYPE tp, char* filename) {
	ARCFILE* arc = 0;
	char* file;
	if (filename == 0) goto err;
	if (tp == ATYPE_DIR) file = root_dir->SearchFile(filename);
	else file = dat_dir->SearchFile(filename);
	if (file == 0) goto err;
	switch(tp) {
		case ATYPE_ARC: arc = new ARCFILE(file); break;
		case ATYPE_DIR: arc = new DIRFILE(file); break;
		case ATYPE_SCN2k: arc = new SCN2kFILE(file); break;
		case ATYPE_Raffresia: arc = new RaffresiaFILE(file); break;
		default: fprintf(stderr,"FILESEARCH::MAKEARCFILE : invalid archive type; type %d name %s\n",tp,filename);
			delete[] file;
			goto err;
	}
	delete[] file;
	return arc;
err:
	arc = new NULFILE;
	return arc;
	
}

ARCINFO* FILESEARCH::Find(FILETYPE type, const char* fname, const char* ext) {
	if (searcher[type] == 0) {
		/* searcher 作成 */
		if (filenames[type] == 0) {
			searcher[type] = dat_dir;
		} else {
			searcher[type] = MakeARCFILE(is_archived[type], filenames[type]);
			if (searcher[type] == 0) {
				fprintf(stderr,"FILESEARCH::Find : invalid archive type; type %d name %s\n",type,fname);
				return 0;
			}
		}
	}
	return searcher[type]->Find(fname,ext);
}

char** FILESEARCH::ListAll(FILETYPE type) {
	/* とりあえず searcher を初期化 */
	Find(type, "THIS FILENAME MAY NOT EXIST IN THE FILE SYSTEM !!!");
	if (searcher[type] == 0) return 0;
	/* 全ファイルのリストアップ */
	int deal = 0;
	ARCFILE* file;
	for (file = searcher[type]; file != 0; file = file->Next())
		deal += file->Deal();
	if (deal <= 0) return 0;
	char** ret_list = new char*[deal+1];
	int count = 0;
	for (file = searcher[type]; file != 0; file = file->Next()) {
		file->InitList();
		char* f;
		while( (f = file->ListItem() ) != 0) {
			ret_list[count] = new char[strlen(f)+1];
			strcpy(ret_list[count], f);
			count++;
		}
	}
	ret_list[count] = 0;
	return ret_list;
}

ARCINFO::ARCINFO(char* arcf,long _off, int _size) {
	arcfile = new char[strlen(arcf)+1];strcpy(arcfile,arcf);
	offset = _off;
	arcsize = _size;
	status = INIT; retbuf = 0;
	mmapped_memory = 0; readbuf = 0; destbuf = 0; fd = -1;
}

ARCINFO::~ARCINFO() {
#ifdef HAVE_MMAP
	if (mmapped_memory) munmap(mmapped_memory, arcsize);
#endif /* HAVE_MMAP */
	if (fd != -1) close(fd);
	if (readbuf) delete[] readbuf;
	if (destbuf) delete[] destbuf;
}

/* コピーを返す */
char* ARCINFO::CopyRead(void) {
	const char* d = Read();
	int s = Size();
	if (s <= 0) return 0;
	char* ret = new char[s]; memcpy(ret, d, s);
	return ret;
}

const char* ARCINFO::Path(void) {
	if (offset != 0) return 0; /* archive file なのでパスを帰せない */
	char* ret = new char[strlen(arcfile)+1];
	strcpy(ret, arcfile);
	return ret;
}
/* 互換性専用 */
FILE* ARCINFO::OpenFile(int* length) {
	FILE* f = fopen(arcfile, "rb");
	if (offset) fseek(f, offset, 0);
	if (length) *length = arcsize;
	return f;
}

/* 読み込みを開始する */
void ARCINFO::Start(void) {
	char header[0x10];
	if (status != INIT) return;
	if (offset < 0 || arcsize <= 0x10) {
		status = ERROR; return;
	}
	/* ファイルを開く */
	fd = open(arcfile, O_RDONLY);
	if (fd < 0) {
		status = ERROR; return;
	}
	/* ヘッダの調査 */
	if (lseek(fd, offset, 0) != offset) {
		status = ERROR; close(fd); fd = -1; return;
	}
	if (read(fd, header, 0x10) != 0x10) {
		status = ERROR; close(fd); fd = -1; return;
	}
	if (strncmp(header, "PACK", 4) != 0) {
		filesize = arcsize;
		/* 圧縮無し：mmap を試みる */
#ifdef HAVE_MMAP
		mmapped_memory = (char*)mmap(0, arcsize, PROT_READ, MAP_SHARED, fd, offset);
		if (mmapped_memory != MAP_FAILED) {
			/* 成功：これでおわり */
			status = MMAP;
			retbuf = mmapped_memory;
			return;
		}
#endif /* HAVE_MMAP */
		/* 失敗：普通にファイルを読み込み */
		/* 読み込み前にオフセットを戻す */
		if (lseek(fd, offset, 0) != offset) {
			status = ERROR; close(fd); fd = -1; return;
		}
		readbuf = new char[arcsize];
		read_count = 0;
		status = READ;
		return;
	} else {
		/* 圧縮有り：ファイル長の情報を得る */
		filesize = read_little_endian_int(header+8);
		int original_size = read_little_endian_int(header+12);
		if (filesize <= 0 || original_size <= 0) {
			status = ERROR; close(fd); fd = -1; return;
		}
		if (original_size != arcsize) arcsize = original_size; /* 情報がおかしい */
		arcsize -= 0x10; /* ヘッダ分を削る */

		/* 圧縮有り：mmap を試みる */
#ifdef HAVE_MMAP
		mmapped_memory = (char*)mmap(0, arcsize, PROT_READ, MAP_SHARED, fd, offset+0x10);
		if (mmapped_memory != MAP_FAILED) {
			/* 成功 */
			status = EXTRACT_MMAP;
			extract_src = mmapped_memory;
			destbuf = new char[filesize + 256];
			src_count = 0; dest_count = 0;
			return;
		}
#endif /* HAVE_MMAP */
		/* 失敗：普通にファイルを読み込む */
		readbuf = new char[arcsize];
		read_count = 0;
		status = READ_BEFORE_EXTRACT;
		return;
	}
}

void ARCINFO::Reading(void) {
	/* ファイルを読み込む */
	if (status != READ && status != READ_BEFORE_EXTRACT) return;
	/* 最大で PROCBLKSIZ ずつ */
	int rsize = arcsize - read_count;
	if (rsize < 0) rsize = 0;
	if (rsize > PROCBLKSIZ) rsize = PROCBLKSIZ;
	if (read(fd, readbuf+read_count, rsize) != rsize) { /* エラー */
		status = ERROR;
		close(fd); fd = -1;
		delete[] readbuf; readbuf = 0;
		return;
	}
	read_count += rsize;
	if (read_count >= arcsize) {
		/* 読み込み終了 */
		close(fd); fd = -1;
		if (status == READ) {
			status = DONE;
			retbuf = readbuf;
		} else { /* READ_BEFORE_EXTRACT */
			status = EXTRACT;
			extract_src = readbuf;
			destbuf = new char[filesize + 256];
			src_count = 0; dest_count = 0;
		}
	}
	return;
}
/* 読み込みを開始する */
void ARCINFO2k::Start(void) {
	char header[0x30];
	if (status != INIT) return;
	if (offset < 0 || arcsize <= 0x1cc) {
		status = ERROR; return;
	}
	/* ファイルを開く */
	fd = open(arcfile, O_RDONLY);
	if (fd < 0) {
		status = ERROR; return;
	}
	/* ヘッダの調査 */
	if (lseek(fd, offset, 0) != offset) {
		status = ERROR; close(fd); fd = -1; return;
	}
	if (read(fd, header, 0x30) != 0x30) {
		status = ERROR; close(fd); fd = -1; return;
	}
	int header_top = read_little_endian_int(header+0);
	int file_version = read_little_endian_int(header+4);
	
	if (file_version != 0x2712 || /* system version が違う */
		(header_top != 0x1cc && header_top != 0x1d0 && header_top != 0x1b8)) {/* support しない header */
		status = ERROR; close(fd); fd = -1; return;
	}
	if (header_top == 0x1cc) header_version = 1;
	else if (header_top == 0x1d0) header_version = 2;
	else if (header_top == 0x1b8) header_version = 3;

	if (header_version == 1) { /* 古い形式(avg2000) */
		header_size = read_little_endian_int(header+0)+read_little_endian_int(header+0x20)*4;
		compdata_size = data_size = read_little_endian_int(header+0x24);
	} else if (header_version == 2) { /* 新しい形式(reallive) */
		header_size = read_little_endian_int(header+0x20);
		data_size = read_little_endian_int(header+0x24);
		compdata_size = read_little_endian_int(header+0x28);
	} else /* if (header_version == 3) */ { /* 初夜献上 */
		header_size = read_little_endian_int(header+0)+read_little_endian_int(header+0x08)*4;
		data_size = read_little_endian_int(header+0x0c);
		compdata_size = read_little_endian_int(header+0x10);
	}
	filesize = header_size + data_size;
	decode_count = 0;
#ifdef HAVE_MMAP
	/* mmap を試みる */
	mmapped_memory = (char*)mmap(0, arcsize, PROT_READ|PROT_WRITE, MAP_PRIVATE, fd, offset);
	if (mmapped_memory != MAP_FAILED) {
		/* 成功 */
		status = EXTRACT_MMAP;
		extract_src = mmapped_memory;
		destbuf = new char[filesize + 256];
		src_count = 0; dest_count = 0;
		return;
	}
#endif /* HAVE_MMAP */
	/* 失敗：普通にファイルを読み込む */
	if (lseek(fd, offset, 0) != offset) {
		status = ERROR; close(fd); fd = -1; return;
	}
	readbuf = new char[arcsize];
	read_count = 0;
	status = READ_BEFORE_EXTRACT;
	return;
}


/**********************************************
**
**	画像展開系クラスの定義、実装
**
***********************************************
*/
GRPCONV::GRPCONV(void) {
	width=0; height = 0;
	mask = 0; filename = 0;
	imagebuf = 0;
	maskbuf = 0;
	imagetype = EXTRACT_INVALID;
}
GRPCONV::~GRPCONV() {
	if (filename) delete[] filename;
	if (imagebuf) delete[] imagebuf;
	if (maskbuf) delete[] maskbuf;
}
void GRPCONV::Init(void) {
	if (imagebuf) delete[] imagebuf;
	if (maskbuf) delete[] maskbuf;
	imagebuf = 0;
	maskbuf = 0;
	imagetype = EXTRACT_INVALID;
}
void GRPCONV::SetFilename(const char* f) {
	if (filename) delete[] filename;
	char* fn = new char[strlen(f)+1];
	strcpy(fn,f);
	filename = fn;
}
void GRPCONV::SetImage(char* image, EXTRACT_TYPE type) {
	if (imagebuf) delete[] imagebuf;
	imagebuf = image;
	imagetype = type;
}
void GRPCONV::SetMask(char* m) {
	if (maskbuf) delete[] maskbuf;
	maskbuf = m;
}
class PDTCONV : public GRPCONV {
	long main_pt; size_t main_size;
	long mask_pt; size_t mask_size;
	const char* inbuf; int inlen;
	char* tmpbuf; // 変換中に使われる中間バッファ
	short* colortable; // 変換中に使われる中間バッファ

	const char* srcbuf;
	const char* src; char* dest;
	const char* srcend; char* destend;
	int index_table[0x20]; // PDT11 用
	enum { INVALID, DONE, EXTRACT10, EXTRACT10_565, EXTRACT11_1, EXTRACT11_1_565, EXTRACT11_2, EXTRACT11_2_565, EXTRACTMASK} status;
	// ファイルの展開：処理中なら、1を返す
	int Extract(void);
	int Extract_565(void);
	int Extract_rev(void);
	int Extract_565rev(void);
	int Extract1(void); // PDT11 の処理用
	int Extract2(void); // PDT11 の処理用
	int Extract2_565(void); // PDT11 の処理用
	int ExtractMask(void);
	void Init(const char* inbuf, int inlen, const char* fname);
public:
	PDTCONV(ARCINFO* info) : GRPCONV() {
		inbuf = 0; tmpbuf = 0; colortable = 0;
		const char* dat = info->Read();
		if (dat == 0) return;
		Init(dat, info->Size(), "???");
	}
	PDTCONV(const char* _inbuf, int inlen, const char* fname) : GRPCONV() {
		inbuf = 0; tmpbuf = 0; colortable = 0;
		Init(_inbuf, inlen, fname);
	}
	virtual ~PDTCONV() {
		if (tmpbuf) delete[] tmpbuf;
		if (colortable) delete[] colortable;
		return;
	}
	bool ReserveRead(EXTRACT_TYPE mode=EXTRACT_32bpp); // 読み込みの予約をする
	bool Process(void); // Read / ReadMask の処理を実行する. 最後まで終わったら 0 を返す
};
class G00CONV : public GRPCONV {
	int colortable[256]; // index color 用
	EXTRACT_TYPE colormode;
	enum { INVALID, DONE, EXTRACTING_type1, COLORCONV_type1,
		EXTRACTING_type2, COPYING_type2, EXTRACTING_type0} status;
	struct REGION {
		int x1, y1, x2, y2;
		int Width() { return x2-x1+1;}
		int Height() { return y2-y1+1;}
		void FixVar(int& v, int& w) {
			if (v < 0) v = 0;
			if (v >= w) v = w-1;
		}
		void Fix(int w, int h) {
			FixVar(x1,w);
			FixVar(x2,w);
			FixVar(y1,h);
			FixVar(y2,h);
			if (x1 > x2) x2 = x1;
			if (y1 > y2) y2 = y1;
		}
	};

	void Init(const char* inbuf, int inlen,const char* fname);

	/* status != INVALID で有効 */
	const char* inbuf; int inbuf_len;

	/* status != INVALID,DONE で有効 */
	char* midbuf; int midbuf_len;
	char* outbuf; int outbuf_len;
	char* tmpmaskbuf; int maskbuf_len;

	/* 作業中に必要に応じてバッファ割り当て */
	const char* src, *srcend;
	char* dest, *destend;
	REGION* region_table;
	int region_deal;
	int region_count;
public:
	G00CONV(ARCINFO* info) : GRPCONV() {
		inbuf = 0; status = INVALID;
		const char* dat = info->Read();
		midbuf = 0; outbuf = 0; tmpmaskbuf = 0; region_table = 0;
		if (dat == 0) return;
		Init(dat, info->Size(), "???");
	}
	G00CONV(const char* _inbuf, int _inlen, const char* fname) : GRPCONV() {
		inbuf = 0; status = INVALID;
		midbuf = 0; outbuf = 0; tmpmaskbuf = 0; region_table = 0;
		Init(_inbuf, _inlen, fname);
	}
	virtual ~G00CONV() {
		if (midbuf) delete[] midbuf;
		if (region_table) delete[] region_table;
		if (tmpmaskbuf) delete[] tmpmaskbuf;
		if (outbuf) delete[] outbuf;
	}
	bool ReserveRead(EXTRACT_TYPE mode=EXTRACT_32bpp);
	bool Process(void); // Read / ReadMask の処理を実行する. 最後まで終わったら 0 を返す
private:
	int Extract0(void); /* type0; 展開 */
	int Extract1(void); /* type1; 展開 */
	int Extract2(void); /* type1; 色変換 */
	int Extract3(void); /* type2; コピー */
};
class GPDCONV : public GRPCONV {
	EXTRACT_TYPE colormode;
	enum { INVALID, DONE, EXTRACTING} status;
	void Init(const char* inbuf, int inlen, const char* fname);

	/* status != INVALID で有効 */
	const char* inbuf; int inbuf_len;

	/* status != INVALID,DONE で有効 */
	char* midbuf; int midbuf_len;
	char* outbuf; int outbuf_len;
	char* tmpmaskbuf; int maskbuf_len;

	/* 作業中に必要に応じてバッファ割り当て */
	const char* src, *srcend;
	char* dest, *destend;
	EXTRACT_TYPE readmode;
public:
	GPDCONV(ARCINFO* info) : GRPCONV() {
		inbuf = 0; status = INVALID;
		const char* dat = info->Read();
		midbuf = 0; outbuf = 0; tmpmaskbuf = 0;
		if (dat == 0) return;
		Init(dat, info->Size(), "???");
	}
	GPDCONV(const char* _inbuf, int _inlen, const char* fname) : GRPCONV() {
		inbuf = 0; status = INVALID;
		midbuf = 0; outbuf = 0; tmpmaskbuf = 0;
		Init(_inbuf, _inlen, fname);
	}
	virtual ~GPDCONV() {
		if (midbuf) delete[] midbuf;
		if (tmpmaskbuf) delete[] tmpmaskbuf;
		if (outbuf) delete[] outbuf;
	}
	bool ReserveRead(EXTRACT_TYPE mode=EXTRACT_32bpp);
	bool Process(void); // Read / ReadMask の処理を実行する. 最後まで終わったら 0 を返す
};


GRPCONV* GRPCONV::AssignConverter(const char* inbuf, int inlen, const char* fname) {
	/* ファイルの内容に応じたコンバーターを割り当てる */
	if (inlen < 10) return 0; /* invalid file */
	if (strncmp(inbuf, "PDT10", 5) == 0 || strncmp(inbuf, "PDT11", 5) == 0) { /* PDT10 or PDT11 */
		return new PDTCONV(inbuf, inlen, fname);
	}
	if (strncmp(inbuf, " DPG", 4) == 0) {
		return new GPDCONV(inbuf, inlen, fname);
	}
	if (inbuf[0] == 0 || inbuf[0] == 1 || inbuf[0] == 2) { /* G00 */
		return new G00CONV(inbuf, inlen, fname);
	}
	return 0;/* cannot assign converter */
}

void PDTCONV::Init(const char* _inbuf, int _inlen,const char* filename) {
//	PDT FILE のヘッダ
//	+00 'PDT10'	(PDT11 は未対応)
//	+08 ファイルサイズ (無視)
//	+0C width (ほぼすべて、640)
//	+10 height(ほぼすべて、480)
//	+14 (mask の) x 座標 (実際は無視・・・全ファイルで 0 )
//	+1c (mask の) y座標 (実際は無視 ・・・全ファイルで 0 )
//	+20 mask が存在すれば、mask へのポインタ
	/* 状態をリセット */
	GRPCONV::Init();
	status = DONE;
	if (tmpbuf) delete[] tmpbuf;
	if (colortable) delete[] colortable;
	tmpbuf = 0; colortable = 0;

	/* ヘッダチェック */
	if (_inlen < 0x20) {
		fprintf(stderr, "Invalid PDT file %s : size is too small\n",filename);
		return;
	}
	if (strncmp(_inbuf, "PDT10", 5) != 0 && strncmp(_inbuf, "PDT11", 5) != 0) {
		fprintf(stderr, "Invalid PDT file %s : not 'PDT10 / PDT11' file.\n", filename);
		return;
	}
	if (size_t(_inlen) != size_t(read_little_endian_int(_inbuf+0x08))) {
		fprintf(stderr, "Invalid archive file %s : invalid header.(size)\n",
			filename);
		return;
	}
	if (strncmp(_inbuf, "PDT10", 5) == 0) {
		main_pt = 0x20;
	} else if (strncmp(_inbuf, "PDT11", 5) == 0) {
		main_pt = 0x460;
	} mask_pt = read_little_endian_int(_inbuf+0x1c);
	if (mask_pt == 0) {
		mask = 0; mask_size = 0;
		main_size = _inlen - main_pt;
	} else {
		mask = 1; mask_size = _inlen - mask_pt;
		main_size = _inlen - main_pt;
	}
	width = read_little_endian_int(_inbuf+0x0c);
	height = read_little_endian_int(_inbuf+0x10);
	inbuf = _inbuf; inlen = _inlen;
	return;
}

bool PDTCONV::ReserveRead(EXTRACT_TYPE mode) {
	if (inbuf == 0) return false;
	char header[0x20];
	while(Process()) ; // なにか処理をしている最中なら、終わらせる
	/* ファイル読み込み */
	int bypp;
	if (mode == EXTRACT_32bpp) bypp=4;
	else /* mode == EXTRACT_16bpp */ bypp = 2;
	/* header 識別 */
	memcpy(header, inbuf, 0x20);
	if (strncmp(header, "PDT10", 5) == 0) {
		tmpbuf = new char[width*height*bypp+1024];
		switch(mode) {
		case EXTRACT_32bpp:
			status = EXTRACT10;
			dest = tmpbuf;
			destend = dest+width*height*4;
			break;
		case EXTRACT_16bpp:
			status = EXTRACT10_565;
			dest = tmpbuf;
			destend = dest+width*height*2;
			break;
		case EXTRACT_INVALID:
			fprintf(stderr,"PDTCONV::ReserveRead : mode is INVALID!!\n");
			break;
		}
	} else if (strncmp(header, "PDT11", 5) == 0) {
		tmpbuf = new char[width*height*bypp+1024];
		const char* table = inbuf + 0x420;
		int i; for (i=0; i<0x10; i++)
			index_table[i] = read_little_endian_int(table + i*4);
		if (mode == EXTRACT_16bpp) {
			colortable = new short[256];
			const char* cbuf = inbuf + 0x20;
			int i; for (i=0; i<256; i++) {
				colortable[i] = ((int(cbuf[2])&0xf8)<<8)|((int(cbuf[1])&0xfc)<<3)|((int(cbuf[0])&0xf8)>>3);
				cbuf += 4;
			}
			status = EXTRACT11_1_565;
		} else status = EXTRACT11_1;
		dest = tmpbuf; destend = dest+width*height;
	} else {
		status = DONE;
		return false;
	}
	// Read を予約
	src = inbuf + main_pt; srcend = src + main_size;
	// 終了
	return true;
}


// ファイルの処理をする
bool PDTCONV::Process(void) {
	if (inbuf == 0) return false;
	if (status == EXTRACT10) {
		// Read() 中
		if (Extract() == 0) {
			SetImage(tmpbuf, EXTRACT_32bpp);
			tmpbuf = 0;
			goto readmask;
		}
		return true;
	} else if (status == EXTRACTMASK) {
		// ReadMask() 中
		if (ExtractMask() == 0) {
			SetMask(tmpbuf);
			tmpbuf = 0;
			status = DONE;
			return true;
		}
		return true;
	} else if (status == EXTRACT11_1 || status == EXTRACT11_1_565) {
		// PDT11 Read() 中
		if (Extract1() == 0) {
			/* srcbuf に color table を読み込み */
			srcbuf = inbuf + 0x20;
			/* 次の状態へ */
			if (status == EXTRACT11_1) {
				destend -= width*height;
				srcend = destend;
				dest = destend + width*height*4;
				src =  srcend + width*height;
				status = EXTRACT11_2;
			} else /* EXTRACT11_1_565 */ {
				destend -= width*height;
				srcend = destend;
				dest = destend + width*height*2;
				src = srcend + width*height;
				status = EXTRACT11_2_565;
			}
		}
		return true;
	} else if (status == EXTRACT11_2) {
		// PDT11 Read() 中
		if (Extract2() == 0) {
			SetImage(tmpbuf, EXTRACT_32bpp);
			tmpbuf = 0;
			goto readmask;
		}
		return true;
	} else if (status == EXTRACT11_2_565) {
		// PDT11 Read() 中
		if (Extract2_565() == 0) {
			SetImage(tmpbuf, EXTRACT_16bpp);
			tmpbuf = 0;
			delete[] colortable;
			colortable = 0;
			goto readmask;
		}
		return true;
	} else if (status == EXTRACT10_565) {
		// 5-6-5 image 読み込み中
		if (Extract_565() == 0) {
			SetImage(tmpbuf, EXTRACT_16bpp);
			tmpbuf = 0;
			goto readmask;
		}
		return true;
	} else return false;
readmask:
	/* 画像読み込み終了、マスク読み込み開始 */
	if (! mask) {
		status = DONE;
		return true;
	}
	srcbuf = inbuf + mask_pt;
	// ReadMask を予約
	status = EXTRACTMASK;
	tmpbuf = new char[width*height+256];
	dest = tmpbuf; destend = tmpbuf + width*height;
	src = srcbuf; srcend = srcbuf + mask_size;
	return true;
}

void G00CONV::Init(const char* _inbuf, int _inlen, const char* filename) {
//	G00 FILE のヘッダ
//	+00 type (1, 2)
//	+01: width(word)
//	+03: height(word)
//	type 0:
//	type 1: (color table 付き LZ 圧縮 ; PDT11 に対応)
//		+05: 圧縮サイズ(dword) ; +5 するとデータ全体のサイズ
//		+09: 展開後サイズ(dword)
//	type 2: (マスク可、画像を矩形領域に分割してそれぞれ圧縮)
//		+05: index size
//		+09: index table(each size is 0x18)
//			+00
//			
//		+09+0x18*size+00: data size
//		+09+0x18*size+04: out size
//		+09+0x18*size+08: (data top)
//

	/* 状態を初期化 */
	GRPCONV::Init();
	if (outbuf) delete[] outbuf;
	if (midbuf) delete[] midbuf;
	if (tmpmaskbuf) delete[] tmpmaskbuf;
	if (region_table) delete[] region_table;
	outbuf = 0; midbuf = 0; tmpmaskbuf = 0; region_table = 0;
	status = INVALID;

	/* データから情報読み込み */
	int type = *_inbuf;

	width = read_little_endian_short(_inbuf+1);
	height = read_little_endian_short(_inbuf+3);
	if (width < 0 || height < 0) return;

	if (type == 0 || type == 1) { // color table 付き圧縮
		if (_inlen < 13) {
			fprintf(stderr, "Invalid G00 file %s : size is too small\n",filename);
			return;
		}
		mask = 0;
		int data_sz = read_little_endian_int(_inbuf+5);

		if (_inlen != data_sz+5) {
			fprintf(stderr, "Invalid archive file %s : invalid header.(size)\n",
				filename);
			return;
		}
		inbuf = _inbuf;
		inbuf_len = _inlen;
		status = DONE;
	} else if (type == 2) { // color table なし、マスク付き可の圧縮
		mask = 1;

		int head_size = read_little_endian_short(_inbuf+5);
		if (head_size < 0 || head_size*24 > _inlen) return;

		const char* data_top = _inbuf + 9 + head_size*24;
		int data_sz = read_little_endian_int(data_top);
		if (_inbuf + _inlen != data_top + data_sz) {
			fprintf(stderr, "Invalid archive file %s : invalid header.(size)\n",
				filename);
			return;
		}
		inbuf = _inbuf;
		inbuf_len = _inlen;
		status = DONE;
	}
	return;
}

bool G00CONV::ReserveRead(EXTRACT_TYPE mode) {
	if (status == INVALID) return false;
	while (status != DONE) Process();  // なにか処理をしている最中なら、終わらせる
	/* ファイル読み込み */
	int bypp;
	colormode = mode;
	if (mode == EXTRACT_32bpp) bypp=4;
	else if (mode == EXTRACT_16bpp) bypp = 2;
	else return false;
	/* header 識別 */
	int type = *inbuf;
	if (type == 0) {
		int extracted_size = read_little_endian_int(inbuf + 9);
		/* buf 関係を初期化 */
		outbuf = new char[extracted_size + 1024];
		outbuf_len = extracted_size;

		// Read を予約
		src = inbuf + 13;
		srcend = inbuf + inbuf_len;
		dest = outbuf;
		destend = outbuf + outbuf_len;
		status = EXTRACTING_type0;
		return true;
	} else if (type == 1) {
		int extracted_size = read_little_endian_int(inbuf + 9) + 1;
		/* buf 関係を初期化 */
		outbuf = new char[width*height*bypp + 1024];
		outbuf_len = width*height*bypp;
		midbuf = new char[extracted_size + 1024];
		midbuf_len = extracted_size;

		// Read を予約
		src = inbuf + 13;
		srcend = inbuf + inbuf_len;
		dest = midbuf;
		destend = midbuf + midbuf_len;
		status = EXTRACTING_type1;
		return true;
	} else if (type == 2) {
		/* 分割領域を得る */
		region_deal = read_little_endian_int(inbuf+5);
		region_table = new REGION[region_deal];

		const char* head = inbuf + 9;
		int i; for (i=0; i<region_deal; i++) {
			region_table[i].x1 = read_little_endian_int(head+0);
			region_table[i].y1 = read_little_endian_int(head+4);
			region_table[i].x2 = read_little_endian_int(head+8);
			region_table[i].y2 = read_little_endian_int(head+12);
			region_table[i].Fix(width, height);
			head += 24;
		}

		int extracted_size = read_little_endian_int(head+4);
		tmpmaskbuf = new char[width*height + 1024];
		memset(tmpmaskbuf, 0, width*height+1024);

		/* buf 関係の初期化 */
		outbuf = new char[width*height*bypp + 1024];
		memset(outbuf, 0, width*height*bypp+1024);
		outbuf_len = width*height*bypp;

		midbuf = new char[extracted_size+1024];
		midbuf_len = extracted_size;

		// Read を予約
		src = head + 8;
		srcend = inbuf + inbuf_len;
		dest = midbuf;
		destend = midbuf + midbuf_len;
		status = EXTRACTING_type2;
		return true;
	}
	return false;
}


// ファイルの処理をする
bool G00CONV::Process(void) {
	if (inbuf == 0) return 0;
	if (status == EXTRACTING_type0) {
		if (Extract0() == 0) {
			/* 画像変換終了 */
			status = DONE;
			SetImage(outbuf, colormode);
			midbuf = 0;
			outbuf = 0;
		}
		return true;
	} else if (status == EXTRACTING_type1) {
		if (Extract1() == 0) {
			/* 画像展開が終わったので色変換開始 */
			int colortable_len = read_little_endian_short(midbuf);
			if (colortable_len*4 > midbuf_len) {
				status = INVALID;
				return false; /* エラー */
			}
			/* 次の準備 */
			status = COLORCONV_type1;
			src = midbuf + colortable_len*4 + 2;
			srcend = midbuf + midbuf_len;
			dest = outbuf;
			destend = outbuf + outbuf_len;
			
			/* color table の準備 */
			if (colortable_len < 0) colortable_len = 0;
			else if (colortable_len > 256) colortable_len = 256;
			if (colormode == EXTRACT_32bpp) {
				memcpy(colortable, midbuf+2, colortable_len * 4);
			} else { /* EXTRACT_16bpp */
				int i; unsigned char* colortable_ptr = (unsigned char*)(midbuf + 2);
				for (i=0; i<colortable_len; i++) {
					colortable[i] = (int(colortable_ptr[0]&0xf8)<<8) | (int(colortable_ptr[1]&0xfc)<<3) | (int(colortable_ptr[2])>>3);
					colortable_ptr += 4;
				}
			}
			memset(colortable + colortable_len, 0, (256 - colortable_len) * sizeof(int));
		}
		return true;
	} else if (status == COLORCONV_type1) {
		if (Extract2() == 0) {
			/* 画像変換終了 */
			status = DONE;
			delete[] midbuf;
			SetImage(outbuf, colormode);
			midbuf = 0;
			outbuf = 0;
		}
		return true;
	} else if (status == EXTRACTING_type2) {
		if (Extract1() == 0) {
			/* 画像展開が終わったのでテーブル変換開始 */

			/* region_deal2 == region_deal のはず……*/
			int region_deal2 = read_little_endian_int(midbuf);
			if (region_deal > region_deal2) region_deal = region_deal2;
			region_count = 0;
			src = 0;
			srcend = 0;
			status = COPYING_type2;
		}
		return true;
	} else if (status == COPYING_type2) {
		if (Extract3() == 0) {
			/* 画像変換終了 */
			status = DONE;
			SetImage(outbuf, colormode);
			SetMask(tmpmaskbuf);
			delete[] midbuf;
			delete[] region_table;
			region_table = 0;
			midbuf = 0;
			outbuf = 0;
			tmpmaskbuf = 0;
		}
		return true;
	}
	return 0;
}

/* 一般的な LZ 圧縮の展開ルーチン */
/* datasize はデータの大きさ、char / short / int を想定 */
/* datatype は Copy1Pixel (1データのコピー)及び ExtractData(LZ 圧縮の情報を得る
** というメソッドを実装したクラス */
static int bitrev_table[256] = {
	0x00, 0x80, 0x40, 0xc0, 0x20, 0xa0, 0x60, 0xe0, 0x10, 0x90, 0x50, 0xd0, 0x30, 0xb0, 0x70, 0xf0,
	0x08, 0x88, 0x48, 0xc8, 0x28, 0xa8, 0x68, 0xe8, 0x18, 0x98, 0x58, 0xd8, 0x38, 0xb8, 0x78, 0xf8,
	0x04, 0x84, 0x44, 0xc4, 0x24, 0xa4, 0x64, 0xe4, 0x14, 0x94, 0x54, 0xd4, 0x34, 0xb4, 0x74, 0xf4,
	0x0c, 0x8c, 0x4c, 0xcc, 0x2c, 0xac, 0x6c, 0xec, 0x1c, 0x9c, 0x5c, 0xdc, 0x3c, 0xbc, 0x7c, 0xfc,
	0x02, 0x82, 0x42, 0xc2, 0x22, 0xa2, 0x62, 0xe2, 0x12, 0x92, 0x52, 0xd2, 0x32, 0xb2, 0x72, 0xf2,
	0x0a, 0x8a, 0x4a, 0xca, 0x2a, 0xaa, 0x6a, 0xea, 0x1a, 0x9a, 0x5a, 0xda, 0x3a, 0xba, 0x7a, 0xfa,
	0x06, 0x86, 0x46, 0xc6, 0x26, 0xa6, 0x66, 0xe6, 0x16, 0x96, 0x56, 0xd6, 0x36, 0xb6, 0x76, 0xf6,
	0x0e, 0x8e, 0x4e, 0xce, 0x2e, 0xae, 0x6e, 0xee, 0x1e, 0x9e, 0x5e, 0xde, 0x3e, 0xbe, 0x7e, 0xfe,
	0x01, 0x81, 0x41, 0xc1, 0x21, 0xa1, 0x61, 0xe1, 0x11, 0x91, 0x51, 0xd1, 0x31, 0xb1, 0x71, 0xf1,
	0x09, 0x89, 0x49, 0xc9, 0x29, 0xa9, 0x69, 0xe9, 0x19, 0x99, 0x59, 0xd9, 0x39, 0xb9, 0x79, 0xf9,
	0x05, 0x85, 0x45, 0xc5, 0x25, 0xa5, 0x65, 0xe5, 0x15, 0x95, 0x55, 0xd5, 0x35, 0xb5, 0x75, 0xf5,
	0x0d, 0x8d, 0x4d, 0xcd, 0x2d, 0xad, 0x6d, 0xed, 0x1d, 0x9d, 0x5d, 0xdd, 0x3d, 0xbd, 0x7d, 0xfd,
	0x03, 0x83, 0x43, 0xc3, 0x23, 0xa3, 0x63, 0xe3, 0x13, 0x93, 0x53, 0xd3, 0x33, 0xb3, 0x73, 0xf3,
	0x0b, 0x8b, 0x4b, 0xcb, 0x2b, 0xab, 0x6b, 0xeb, 0x1b, 0x9b, 0x5b, 0xdb, 0x3b, 0xbb, 0x7b, 0xfb,
	0x07, 0x87, 0x47, 0xc7, 0x27, 0xa7, 0x67, 0xe7, 0x17, 0x97, 0x57, 0xd7, 0x37, 0xb7, 0x77, 0xf7,
	0x0f, 0x8f, 0x4f, 0xcf, 0x2f, 0xaf, 0x6f, 0xef, 0x1f, 0x9f, 0x5f, 0xdf, 0x3f, 0xbf, 0x7f, 0xff};
template<class DataType, class DataSize> inline int genExtract(DataType& datatype,const char*& src, char*& dest, const char* srcend, char* destend) {
	int count = 0;
	const char* lsrcend = srcend; char* ldestend = destend;
	const char* lsrc = src; char* ldest = dest;
	/* 範囲チェックは適当。本来の dest_end, src_end より
	** 最悪で 134bytes, 16bytes 先まで読み書きする可能性があるので、
	** lsrcend は適宜減らしておく(dest はもともと1024byteの余裕がある)
	*/
	lsrcend -= 50;
	while (ldest < ldestend && lsrc < lsrcend) {
		count += 8;
		if (count > datatype.ProcessBlockSize()) {
			dest=ldest; src=lsrc; return 1;
		}
		int flag = int(*(unsigned char*)lsrc++);
		if (datatype.IsRev()) flag = bitrev_table[flag];
		int i; for (i=0; i<8; i++) {
			if (flag & 0x80) {
				datatype.Copy1Pixel(lsrc, ldest);
			} else {
				int data, size;
				datatype.ExtractData(lsrc, data, size);
				DataSize* p_dest = ((DataSize*)ldest) - data;
				int k; for (k=0; k<size; k++) {
					p_dest[data] = *p_dest;
					p_dest++;
				}
				ldest += size*sizeof(DataSize);
			}
			flag <<= 1;
		}
	}
	/* 残りを変換 */
	lsrcend = srcend;
	while (ldest < ldestend && lsrc < lsrcend) {
		count += 8;
		int flag = int(*(unsigned char*)lsrc++);
		if (datatype.IsRev()) flag = bitrev_table[flag];
		int i; for (i=0; i<8 && ldest < ldestend && lsrc < lsrcend; i++) {
			if (flag & 0x80) {
				datatype.Copy1Pixel(lsrc, ldest);
			} else {
				int data, size;
				datatype.ExtractData(lsrc, data, size);
				DataSize* p_dest = ((DataSize*)ldest) - data;
				int k; for (k=0; k<size; k++) {
					p_dest[data] = *p_dest;
					p_dest++;
				}
				ldest += size*sizeof(DataSize);
			}
			flag <<= 1;
		}
	}
	dest=ldest; src=lsrc;
	return 0;
}
/* 引数を減らすためのwrapper */
template<class DataType, class DataSize> inline int genExtract(DataType datatype, DataSize datasize ,const char*& src, char*& dest, const char* srcend, char* destend) {
	return genExtract<DataType, DataSize>(datatype,src,dest,srcend,destend);
}

/* 普通の PDT */
class Extract_DataType {
public:
	static void ExtractData(const char*& lsrc, int& data, int& size) {
		data = read_little_endian_short(lsrc) & 0xffff;
		size = (data & 0x0f) + 1;
		data = (data>>4)+1;
		lsrc += 2;
	}
	static void Copy1Pixel(const char*& lsrc, char*& ldest) {
#ifdef WORDS_BIGENDIAN
		ldest[0] = lsrc[0];
		ldest[1] = lsrc[1];
		ldest[2] = lsrc[2];
#else /* LITTLE ENDIAN / intel architecture */
		*(int*)ldest = *(int*)lsrc; ldest[3]=0;
#endif
		lsrc += 3; ldest += 4;
	}
	static int ProcessBlockSize(void) {
		return EVENT_PROCESS_COUNT;
	}
	static int IsRev(void) { return 0; }
};
/* 色順逆のPDT */
class Extract_DataType_rev : public Extract_DataType{
public:
	static void Copy1Pixel(const char*& lsrc, char*& ldest) {
		ldest[0] = lsrc[2];
		ldest[1] = lsrc[1];
		ldest[2] = lsrc[0];
		ldest[3] = 0;
		lsrc += 3; ldest += 4;
	}
};
/* 16bpp 変換付き */
class Extract_DataType_565 : public Extract_DataType{
public:
	static void Copy1Pixel(const char*& lsrc, char*& ldest) {
		*(short*)ldest =
			((int(*(unsigned char*)(lsrc+2)) & 0xf8)<<8) |
			((int(*(unsigned char*)(lsrc+1)) & 0xfc)<<3) |
			((int(*(unsigned char*)(lsrc+0)) )>>3);
		lsrc += 3; ldest += 2;
	}
};
/* 色順逆、16bpp 変換 */
class Extract_DataType_565rev : public Extract_DataType {
public:
	static void Copy1Pixel(const char*& lsrc, char*& ldest) {
		*(short*)ldest =
			((int(*(unsigned char*)(lsrc+0)) & 0xf8)<<8) |
			((int(*(unsigned char*)(lsrc+1)) & 0xfc)<<3) |
			((int(*(unsigned char*)(lsrc+2)) )>>3);
		lsrc += 3; ldest += 2;
	}
};

/* PDT11 の第一段階変換 */
class Extract_DataType_PDT11 {
	int* index_table;
public:
	Extract_DataType_PDT11(int* it) { index_table = it; }
	void ExtractData(const char*& lsrc, int& data, int& size) {
		data = int(*(const unsigned char*)lsrc);
		size = (data>>4) + 2;
		data = index_table[data&0x0f];
		lsrc++;
	}
	static void Copy1Pixel(const char*& lsrc, char*& ldest) {
		*ldest = *lsrc;
		ldest++; lsrc++;
	}
	static int ProcessBlockSize(void) {
		return EVENT_PROCESS_COUNT;
	}
	static int IsRev(void) { return 0; }
};
/* マスク用 */
class Extract_DataType_Mask {
public:
	void ExtractData(const char*& lsrc, int& data, int& size) {
		int d = read_little_endian_short(lsrc) & 0xffff;
		size = (d & 0xff) + 2;
		data = (d>>8)+1;
		lsrc += 2;
	}
	static void Copy1Pixel(const char*& lsrc, char*& ldest) {
		*ldest = *lsrc;
		ldest++; lsrc++;
	}
	static int ProcessBlockSize(void) {
		return EVENT_PROCESS_COUNT;
	}
	static int IsRev(void) { return 0; }
};
/* 書庫用 */
class Extract_DataType_ARC {
public:
	void ExtractData(const char*& lsrc, int& data, int& size) {
		data = read_little_endian_short(lsrc) & 0xffff;
		size = (data&0x0f) + 2;
		data = (data>>4) + 1;
		lsrc+= 2;
	}
	static void Copy1Pixel(const char*& lsrc, char*& ldest) {
		*ldest = *lsrc;
		ldest++; lsrc++;
	}
	static int ProcessBlockSize(void) {
		return PROCBLKSIZ;
	}
	static int IsRev(void) { return 0; }
};
class Extract_DataType_ARC_end : public Extract_DataType_ARC {
public:
	static int ProcessBlockSize(void) {
		return 1024*1024*1024; /* 最後まで一気に展開 */
	}
};
/* avg2000 のシナリオ用 */
class Extract_DataType_SCN2k {
public:
	void ExtractData(const char*& lsrc, int& data, int& size) {
		data = read_little_endian_short(lsrc) & 0xffff;
		size = (data&0x0f) + 2;
		data = (data>>4);
		lsrc+= 2;
	}
	static void Copy1Pixel(const char*& lsrc, char*& ldest) {
		*ldest = *lsrc;
		ldest++; lsrc++;
	}
	static int ProcessBlockSize(void) {
		return PROCBLKSIZ;
	}
	static int IsRev(void) { return 1; }
};
class Extract_DataType_SCN2k_end : public Extract_DataType_ARC {
public:
	static int ProcessBlockSize(void) {
		return 1024*1024*1024; /* 最後まで一気に展開 */
	}
};
class Extract_DataType_G00Type0 {
public:
	static void ExtractData(const char*& lsrc, int& data, int& size) {
		data = read_little_endian_short(lsrc) & 0xffff;
		size = ((data & 0x0f)+ 1) * 4;
		data = (data>>4) * 4;
		lsrc += 2;
	}
	static void Copy1Pixel(const char*& lsrc, char*& ldest) {
#ifdef WORDS_BIGENDIAN
		ldest[0] = lsrc[0];
		ldest[1] = lsrc[1];
		ldest[2] = lsrc[2];
#else /* LITTLE ENDIAN / intel architecture */
		*(int*)ldest = *(int*)lsrc; ldest[3]=0;
#endif
		lsrc += 3; ldest += 4;
	}
	static int ProcessBlockSize(void) {
		return EVENT_PROCESS_COUNT;
	}
	static int IsRev(void) { return 1; }
};

int PDTCONV::Extract(void) {
	return genExtract(Extract_DataType(), int(), src, dest, srcend, destend);
}
int PDTCONV::Extract_rev(void) {
	return genExtract(Extract_DataType_rev(), int(), src, dest, srcend, destend);
}
int PDTCONV::Extract_565(void) {
	return genExtract(Extract_DataType_565(), short(), src, dest, srcend, destend);
}
int PDTCONV::Extract_565rev(void) {
	return genExtract(Extract_DataType_565rev(), short(), src, dest, srcend, destend);
}
int PDTCONV::Extract1(void) {
	return genExtract(Extract_DataType_PDT11(index_table), char(), src, dest, srcend, destend);
}

int PDTCONV::Extract2(void) {
	int count = EVENT_PROCESS_COUNT;
	unsigned char* s = (unsigned char*)src;
	unsigned int* d = (unsigned int*)dest;
	unsigned int* dend = (unsigned int*)destend;
	unsigned int* table = (unsigned int*)srcbuf;
	if (d-count < dend) count = d - dend;
	int i; for (i=0; i<count; i++) {
		*--d = table[*--s];
	}
	dest = (char*)d; src = (char*)s;
	if (d <= dend) return 0;
	else return 1; // 処理の途中で終了
}
int PDTCONV::Extract2_565(void) {
	int count = EVENT_PROCESS_COUNT;
	unsigned char* s = (unsigned char*)src;
	short* d = (short*)dest;
	short* dend = (short*)destend;
	if (d-count < dend) count = d - dend;
	int i; for (i=0; i<count; i++) {
		*--d = colortable[*--s];
	}
	dest = (char*)d; src = (char*)s;
	if (d <= dend) return 0;
	else return 1; // 処理の途中で終了
}

int PDTCONV::ExtractMask(void) {
	return genExtract(Extract_DataType_Mask(), char(), src, dest, srcend, destend);
}

/* dest は dest_end よりも 256 byte 以上先まで
** 書き込み可能であること。
*/
void ARCINFO::Extract(char*& dest_start, char*& src_start, char* dest_end, char* src_end) {
	const char* src = src_start;
	while (genExtract(Extract_DataType_ARC_end(), char(), src, dest_start, src_end, dest_end)) ;
	src_start = (char*)src;
	return;
}

void ARCINFO::Extracting(void) {
	if (status != EXTRACT && status != EXTRACT_MMAP) return;
	char* dest = destbuf + dest_count;
	const char* src = extract_src + src_count;
	if (genExtract(Extract_DataType_ARC(), char(), src, dest, extract_src+arcsize, destbuf+filesize)) {
		/* 展開中 */
		dest_count = dest - destbuf;
		src_count = (char*)src - extract_src;
	} else { /* 展開終了 */
		if (status == EXTRACT) {
			delete[] readbuf; readbuf = 0;
		} else { /* EXTRACT_MMAP */
#ifdef HAVE_MMAP
			munmap(mmapped_memory, arcsize); mmapped_memory = 0;
#endif /* HAVE_MMAP */
			close(fd); fd = -1;
		}
		status = DONE; retbuf = destbuf;
	}
	return;
}

char ARCINFO2k::decode_seed[256] ={
 0x8b ,0xe5 ,0x5d ,0xc3 ,0xa1 ,0xe0 ,0x30 ,0x44 ,0x00 ,0x85 ,0xc0 ,0x74 ,0x09 ,0x5f ,0x5e ,0x33
,0xc0 ,0x5b ,0x8b ,0xe5 ,0x5d ,0xc3 ,0x8b ,0x45 ,0x0c ,0x85 ,0xc0 ,0x75 ,0x14 ,0x8b ,0x55 ,0xec
,0x83 ,0xc2 ,0x20 ,0x52 ,0x6a ,0x00 ,0xe8 ,0xf5 ,0x28 ,0x01 ,0x00 ,0x83 ,0xc4 ,0x08 ,0x89 ,0x45
,0x0c ,0x8b ,0x45 ,0xe4 ,0x6a ,0x00 ,0x6a ,0x00 ,0x50 ,0x53 ,0xff ,0x15 ,0x34 ,0xb1 ,0x43 ,0x00
,0x8b ,0x45 ,0x10 ,0x85 ,0xc0 ,0x74 ,0x05 ,0x8b ,0x4d ,0xec ,0x89 ,0x08 ,0x8a ,0x45 ,0xf0 ,0x84
,0xc0 ,0x75 ,0x78 ,0xa1 ,0xe0 ,0x30 ,0x44 ,0x00 ,0x8b ,0x7d ,0xe8 ,0x8b ,0x75 ,0x0c ,0x85 ,0xc0
,0x75 ,0x44 ,0x8b ,0x1d ,0xd0 ,0xb0 ,0x43 ,0x00 ,0x85 ,0xff ,0x76 ,0x37 ,0x81 ,0xff ,0x00 ,0x00
,0x04 ,0x00 ,0x6a ,0x00 ,0x76 ,0x43 ,0x8b ,0x45 ,0xf8 ,0x8d ,0x55 ,0xfc ,0x52 ,0x68 ,0x00 ,0x00
,0x04 ,0x00 ,0x56 ,0x50 ,0xff ,0x15 ,0x2c ,0xb1 ,0x43 ,0x00 ,0x6a ,0x05 ,0xff ,0xd3 ,0xa1 ,0xe0
,0x30 ,0x44 ,0x00 ,0x81 ,0xef ,0x00 ,0x00 ,0x04 ,0x00 ,0x81 ,0xc6 ,0x00 ,0x00 ,0x04 ,0x00 ,0x85
,0xc0 ,0x74 ,0xc5 ,0x8b ,0x5d ,0xf8 ,0x53 ,0xe8 ,0xf4 ,0xfb ,0xff ,0xff ,0x8b ,0x45 ,0x0c ,0x83
,0xc4 ,0x04 ,0x5f ,0x5e ,0x5b ,0x8b ,0xe5 ,0x5d ,0xc3 ,0x8b ,0x55 ,0xf8 ,0x8d ,0x4d ,0xfc ,0x51
,0x57 ,0x56 ,0x52 ,0xff ,0x15 ,0x2c ,0xb1 ,0x43 ,0x00 ,0xeb ,0xd8 ,0x8b ,0x45 ,0xe8 ,0x83 ,0xc0
,0x20 ,0x50 ,0x6a ,0x00 ,0xe8 ,0x47 ,0x28 ,0x01 ,0x00 ,0x8b ,0x7d ,0xe8 ,0x89 ,0x45 ,0xf4 ,0x8b
,0xf0 ,0xa1 ,0xe0 ,0x30 ,0x44 ,0x00 ,0x83 ,0xc4 ,0x08 ,0x85 ,0xc0 ,0x75 ,0x56 ,0x8b ,0x1d ,0xd0
,0xb0 ,0x43 ,0x00 ,0x85 ,0xff ,0x76 ,0x49 ,0x81 ,0xff ,0x00 ,0x00 ,0x04 ,0x00 ,0x6a ,0x00 ,0x76};

void ARCINFO2k::Extracting(void) {
	if (status != EXTRACT && status != EXTRACT_MMAP) return;
	if (src_count == 0) { /* header のコピー */
		memcpy(destbuf, extract_src, header_size);
		src_count = header_size+8;
		dest_count = header_size;
	}
	char* dest = destbuf + dest_count;
	const char* src = extract_src + src_count;
	/* まず、xor の暗号化を解く */
	int decode_count_end = decode_count+Extract_DataType_SCN2k::ProcessBlockSize();
	if (decode_count == 0) {
		decode_count_end += Extract_DataType_SCN2k::ProcessBlockSize();
	}
	if (decode_count_end > compdata_size) decode_count_end = compdata_size;
	char* decode_src = extract_src + header_size;
	int i; for (i=decode_count; i<decode_count_end; i++) {
		decode_src[i] ^= decode_seed[i&0xff];
	}
	decode_count = i;
	/* 圧縮されてないなら終了チェック */
	if (header_version == 1) {
		if (decode_count < compdata_size) return;
	} else /* if (header_version == 2) */ {
		/* 圧縮されているなら、展開 */
		if (genExtract(Extract_DataType_SCN2k(), char(), src, dest, extract_src+arcsize, destbuf+filesize) != 0) {
			dest_count = dest - destbuf;
			src_count = (char*)src - extract_src;
			return;
		}
	}
	/* 展開終了 */
	if (header_version == 1) {
		delete[] destbuf; destbuf = 0;
		retbuf = extract_src;
	} else /* if (header_version == 2) */ {
		if (status == EXTRACT) {
			delete[] readbuf; readbuf = 0;
		} else /* EXTRACT_MMAP */ {
#ifdef HAVE_MMAP
			munmap(mmapped_memory, arcsize); mmapped_memory = 0;
#endif /* HAVE_MMAP */
			close(fd); fd = -1;
		}
		retbuf = destbuf;
	}
	status = DONE;
}

int G00CONV::Extract1(void) {
	return genExtract(Extract_DataType_SCN2k(), char(), src, dest, srcend, destend);
}
int G00CONV::Extract0(void) {
	return genExtract(Extract_DataType_G00Type0(), char(), src, dest, srcend, destend);
}
int G00CONV::Extract2(void) {
	int count = EVENT_PROCESS_COUNT;
	const unsigned char* s = (const unsigned char*)src;
	if (srcend-src < count) count = srcend - src;

	if (colormode == EXTRACT_16bpp) {
		short* d = (short*)dest;
		short* dend = (short*)destend;
		if (dend-d < count) count = dend-d;
		int i; for (i=0; i<count; i++) {
			*d++ = colortable[*s++];
		}
		dest = (char*)d;
	} else { /* EXTRACT_32bpp */
		unsigned int* d = (unsigned int*)dest;
		unsigned int* dend = (unsigned int*)destend;
		if (dend-d < count) count = dend-d;
		int i; for (i=0; i<count; i++) {
			*d++ = colortable[*s++];
		}
		dest = (char*)d;
	}
	src = (const char*)s;
	if (dest >= destend) return 0;
	else return 1; // 処理の途中で終了
}

int G00CONV::Extract3(void) {
	int count = EVENT_PROCESS_COUNT;
	for (; region_count < region_deal; region_count++) {
		if (src == 0) {
			int offset = read_little_endian_int(midbuf + 4 + region_count*8);
			int length = read_little_endian_int(midbuf + 8 + region_count*8);
			src = midbuf + offset + 0x74;
			srcend = midbuf + offset + length;
		}
		while(count > 0 && src < srcend) {
			int x, y, w, bpl, h;
			/* コピーする領域を得る */
			x = read_little_endian_short(src);
			y = read_little_endian_short(src+2);
			bpl = w = read_little_endian_short(src+6);
			h = read_little_endian_short(src+8);
			src += 0x5c;

			x += region_table[region_count].x1;
			y += region_table[region_count].y1;
			if (x+w > region_table[region_count].x2) w = region_table[region_count].x2+1-x;
			if (y+h > region_table[region_count].y2) h = region_table[region_count].y2+1-y;

			/* コピー */
			char* m = tmpmaskbuf + x + y*width;
			const char* s = src;
			int i,j;
			if (colormode == EXTRACT_32bpp) {
				char* d = outbuf + x*4 + y*width*4;
				for (i=0; i<h; i++) {
					memcpy(d, s, bpl*4);
					for (j=0; j<w; j++) m[j] = s[j*4+3];
					d += width*4;
					s += bpl*4;
					m += width;
				}
			} else { /* color_mode == EXTRACT_16bpp */
				short* d = (short*)(outbuf + x*2 + y*width*2);
				for (i=0; i<h; i++) {
					for (j=0; j<w; j++) {
						d[j] = ((int(s[0])&0xf8)<<8)| ((int(s[1])&0xfc)<<3)| ((int(s[2])&0xf8)>>3);
						m[j] = s[3];
						s += 4;
					}
					d += width*4;
					s += (bpl-j)*4;
					m += width;
				}
			}
			/* 後処理 */
			src += bpl*h*4;
			count -= bpl*h*4;
		}
		if (count <= 0) break;
		if (src >= srcend) src = 0;
	}
	if (count <= 0) return 1;
	return 0;
}

void GPDCONV::Init(const char* _inbuf, int _inlen, const char* _filename) {
//	GPD FILE のヘッダ
//	+00 magic (" DPG")
//	+12: width(dword)
//	+16: height(dword)
//	+20: bpp(dword) mask つき：32 maskなし：24

	/* 状態を初期化 */
	GRPCONV::Init();
	if (outbuf) delete[] outbuf;
	if (midbuf) delete[] midbuf;
	if (tmpmaskbuf) delete[] tmpmaskbuf;
	outbuf = 0; midbuf = 0; tmpmaskbuf = 0;
	status = INVALID;
	SetFilename(_filename);

	/* データから情報読み込み */
	width = read_little_endian_int(_inbuf+12);
	height = read_little_endian_int(_inbuf+16);
	if (width < 0 || height < 0) return;

	inbuf = _inbuf;
	inbuf_len = _inlen;
	status = DONE;
	return;
}

bool GPDCONV::ReserveRead(EXTRACT_TYPE mode) {
	if (status == INVALID) return false;
	while (status != DONE) Process();  // なにか処理をしている最中なら、終わらせる
	/* ファイル読み込み */
	int bypp;
	colormode = mode;
	if (mode == EXTRACT_32bpp) bypp=4;
	else if (mode == EXTRACT_16bpp) bypp = 2;
	else return false;
	/* マスクのチェック */
	int bpp = read_little_endian_int(inbuf+20);
printf("file %s %d\n",filename,bpp);
	if (bpp == 32) {
		mask = 1;
		tmpmaskbuf = new char[width*height + 1024];
	} else {
		mask = 0;
		tmpmaskbuf = 0;
	}
	/* buf 関係の初期化 */
	outbuf = new char[width*height*bypp + 1024];
	outbuf_len = width * height * bypp;

	src = inbuf+read_little_endian_int(inbuf+32)+0x10;
	srcend = src + read_little_endian_int(inbuf+36);

	int extracted_size = read_little_endian_int(src-0x10+4);
	midbuf = new char[extracted_size + 1024];
	midbuf_len = extracted_size;

	readmode = mode;
	dest = midbuf;
	destend = midbuf + midbuf_len;
	status = EXTRACTING;
	return true;
}

// ファイルの処理をする
bool GPDCONV::Process(void) {
	if (inbuf == 0) return true;
	if (status == EXTRACTING) {
		if (genExtract(Extract_DataType_ARC(), char(), src, dest, srcend, destend)) return true;
		/* extract 終了 */
		/* 色変換を行う */
		int len = width * height;
		int i;
		if (readmode == EXTRACT_16bpp) {
			unsigned short* out = (unsigned short*)outbuf;
			unsigned char* s = (unsigned char*)midbuf; unsigned char* mask = (unsigned char*)tmpmaskbuf;
			if (tmpmaskbuf) {
				for(i=0; i<len; i++) {
					/* マスクの形式が違うので、AVG32 互換の形にする */
					unsigned int m = *(unsigned char*)(s+3);
					m = m + (m>>7);
					*out++ = 
						(((int(*(unsigned char*)(s))*m))&0xf800) |
						(((int(*(unsigned char*)(s+1))*m)>>5)&0x07e0) |
						(((int(*(unsigned char*)(s+2))*m)>>8)&0x001f);
					*mask++ = s[3];
					s += 4;
				}
			} else {
				for(i=0; i<len; i++) {
					*out++ = 
						(((int(*(unsigned char*)(s)))<<8)&0xf800) |
						(((int(*(unsigned char*)(s+1)))<<3)&0x07e0) |
						(((int(*(unsigned char*)(s+2))))&0x001f);
					s += 3;
				}
			}
		} else { /* 32bpp */
			unsigned char* out = (unsigned char*)outbuf;
			unsigned char* s = (unsigned char*)midbuf; unsigned char* mask = (unsigned char*)tmpmaskbuf;
			if (tmpmaskbuf) {
				for (i=0; i<len; i++) {
					unsigned int m = *(unsigned char*)(s+3);
					m = m + (m>>7);
					*out++ = (int(*s++)*m)>>8;
					*out++ = (int(*s++)*m)>>8;
					*out++ = (int(*s++)*m)>>8;
					*mask++= *s++;
					out++;
				}
			} else {
				for (i=0; i<len; i++) {
					*out++ = *s++;
					*out++ = *s++;
					*out++ = *s++;
					out++;
				}
			}
		}
		SetImage(outbuf, colormode);
		SetMask(tmpmaskbuf);
		delete[] midbuf;
		tmpmaskbuf = 0;
		outbuf = 0;
		midbuf = 0;
		status = DONE;
		return false;
	}
	return false;
}
