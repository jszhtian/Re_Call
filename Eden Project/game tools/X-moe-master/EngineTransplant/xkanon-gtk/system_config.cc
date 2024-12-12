/*  system_config.cc
 *      gameexe.ini ファイルの読み込み
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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <map>
#include "system.h"
#include "image_di.h"
#include "file.h"

using namespace std;

// #define DEBUG_CONFIG
#ifdef DEBUG_CONFIG
# define dprintf(X) printf X
#else
# define dprintf(X)
#endif /* DEBUG_CONFIG */

#define MAXTOKEN 10 /* = で区切られた領域の最大数 */
#define MAXVARS 32 /* , で区切られた数値の最大数 */

// 初期化ファイルの読み込み
/* config は 文字列、数列、その複合など、いろいろな形式がありうる */
/* 文字列と数列は一般に AyuSys_Config クラスに含める */


/**********************************************************/
/* とりあえずハッシュ比較付き文字列 */
class HashStr {
	const char* str;
	unsigned int hash;
public:
	HashStr(const char*);
	HashStr(const HashStr& orig);
	~HashStr() {
		if (str) delete[] str;
	}
	const char* c_str(void) const { return str; }
	friend inline int operator<(const HashStr& a, const HashStr& b) {
		if (a.hash == b.hash) {
			if (a.str == 0) return 1;
			else if (b.str == 0) return 0;
			else return strcmp(a.str, b.str);
		}
		else return a.hash < b.hash;
	}
};
HashStr::HashStr(const char* s ) {
	if (s == 0 || s[0] == '\0') {
		str = 0; hash = 0; return; /* invalid string */
	}
	char* new_str = new char[strlen(s)+1];
	strcpy(new_str, s);
	str = new_str;
	/* calc hash... 適当 */
	int h = strlen(s);
	while(*s != 0) {
		h = *s + ((h * (0x9449+*s))>>7);
		s++;
	}
	hash = (unsigned int)h;
}
HashStr::HashStr(const HashStr& orig) {
	if (orig.str == 0 || orig.str[0] == '\0') {
		str = 0; hash = 0; return; /* invalid */
	}
	char* new_str = new char[strlen(orig.str)+1];
	strcpy(new_str, orig.str);
	str = new_str;
	hash = orig.hash;
}

/**********************************************************
**AyuSys_Config_[String | Intlist] :
** 設定の本体
**	original : 元設定
**	old_data : 前回 ClearDiff() したときの設定
**	new_data : ClearDiff() 以降に設定した内容を保存
**	データ設定：
**	Init() : 元設定を作成
**	Set() : 設定を変更
**	Get() : 最も新しい設定を得る
**
**	変更の記録：
**	Diff() : 前回のClearDiff() から変更した内容を得る
**	DiffLen() : Diff() で必要な文字列長を得る
**	ClearDiff() : 変更記録を消す
**	PatchOld() : Diff() で得た記録に基づき、変更前の状態に戻す
**	PatchNew() : Diff() で得た記録に基づき、変更後の状態に戻す
**
**	元設定からの変更の記録：
**	SetOriginal() : 元設定に戻す
**	DiffOriginal() : 元設定から現在の設定の変更を得る
**	DiffOriginalLen() : DiffOriginal() で必要な文字列長を得る
**	PatchOriginal() : DiffOriginal() で得た記録に基づき、設定を復旧する
*/

/************************************************
** AyuSysConfigStringItem
** 文字列をデータとしてもつ設定項目
*/
class AyuSysConfigStringItem {
	char* original_data;
	char* old_data;
	char* new_data;
public:
	AyuSysConfigStringItem(void) {
		original_data = 0;
		old_data = 0;
		new_data = 0;
	}
	AyuSysConfigStringItem(const AyuSysConfigStringItem& o) {
		original_data = 0; old_data = 0; new_data = 0;
		if (o.original_data) {
			original_data = new char[strlen(o.original_data)+1];
			strcpy(original_data, o.original_data);
		}
		if (o.old_data) {
			old_data = new char[strlen(o.old_data)+1];
			strcpy(old_data, o.old_data);
		}
		if (o.new_data) {
			new_data = new char[strlen(o.new_data)+1];
			strcpy(new_data, o.new_data);
		}
	}
	/* 設定：Init で初期化、Set で変更、Get で変更を優先して取り出す */
	void Init(int deal, const char* str) { /* deal は無視 */
		if (original_data) delete[] original_data;
		int len = strlen(str);
		original_data = new char[len+1];
		strcpy(original_data, str);
		original_data[len] = '\0';
	}
	void Set(int deal, const char* str) { /* deal は無視 */
		if (new_data) delete[] new_data;
		int len = strlen(str);
		new_data = new char[len+1];
		strcpy(new_data, str);
		new_data[len] = '\0';
	}
	const char* Get(int deal) const {/* deal は無視 */
		if (new_data) return new_data;
		else if (old_data) return old_data;
		return original_data;
	}
	/* 変化の追跡：ClearDiff 以降に設定変更があれば Diff で取り出せる。
	** PatchOld で古い状態へ、PatchNew で新しい状態へ
	*/
	void ClearDiff(void) {
		if (new_data) {
			if (old_data) delete[] old_data;
			old_data = new_data;
			new_data = 0;
		}
	}
	int DiffLen(void) {
		if (new_data == 0) return 0;
		if (old_data == 0) {
			return 2+strlen(new_data);
		} else {
			return 2+strlen(new_data)+strlen(old_data);
		}
	}
	char* Diff(char* ret_str) {
		if (new_data == 0) { /* あり得ない */
			fprintf(stderr,"AyuSysConfigStringItem::Diff : this method must not called if not required!\n");
			return ret_str;
		}
		if (old_data == 0) {
			*ret_str++ = 0;
		} else {
			strcpy(ret_str, old_data);
			ret_str += strlen(ret_str)+1;
		}
		strcpy(ret_str, new_data);
		ret_str += strlen(ret_str)+1;
		return ret_str;
	}
	const char* PatchOld(const char* data) {
		if (new_data) delete[] new_data;
		if (old_data) delete[] old_data;
		new_data = 0; old_data = 0;
		if (*data == '\0') { /* データが存在しない */
			data++; data += strlen(data)+1;
			return data;
		} else {
			old_data = new char[strlen(data)+1]; strcpy(old_data, data);
			data += strlen(data)+1;
			data += strlen(data)+1;
			return data;
		}
	}
	const char* PatchNew(const char* data) {
		if (new_data) delete[] new_data;
		if (old_data) delete[] old_data;
		new_data = 0; old_data = 0;
		data += strlen(data) + 1;
		old_data = new char[strlen(data)+1]; strcpy(old_data, data);
		data += strlen(data)+1;
		return data;
	}
	const char* DumpPatch(FILE* out, const char* data) const {
		if (*data) { fprintf(out, "%s -> ",data); data += strlen(data)+1;}
		else { fprintf(out, "(nothing) -> "); data++;}
		fprintf(out, "%s\n",data); data += strlen(data)+1;
		return data;
	}
	/* オリジナルからの変化の調査 :
	** DiffOriginal で変化を文字列で取り出し、PatchOriginal で
	** 変化を反映
	*/
	int DiffOriginalLen(void) {
		if (new_data == 0 && old_data == 0) return 0;
		int change_len = (old_data==0) ? 1 : strlen(old_data)+1;
		change_len += (new_data==0) ? 1 : strlen(new_data)+1;
		return change_len;
	}
	char* DiffOriginal(char* data) {
		if (new_data == 0 && old_data == 0) { /* あり得ない */
			fprintf(stderr,"AyuSysConfigStringItem::DiffOriginal : this method must not called if not required!\n");
			return data;
		}
		if (old_data) strcpy(data, old_data);
		else data[0] = '\0';
		data += strlen(data) + 1;
		if (new_data) strcpy(data, new_data);
		else data[0] = '\0';
		data += strlen(data) + 1;
		return data;
	}
	const char* PatchOriginal(const char* data) {
		if (new_data) delete[] new_data;
		if (old_data) delete[] old_data;
		new_data = 0; old_data = 0;
		if (*data) {
			old_data = new char[strlen(data)+1];
			strcpy(old_data, data);
		}
		data += strlen(data)+1;
		if (*data) {
			new_data = new char[strlen(data)+1];
			strcpy(new_data, data);
		}
		data += strlen(data)+1;
		return data;
	}
	void SetOriginal(void) {
		if (new_data) delete[] new_data;
		if (old_data) delete[] old_data;
		new_data = 0; old_data = 0;
	}
	const char* DumpPatchOriginal(FILE* out, const char* data) const{
		return DumpPatch(out, data);
	}
	void Dump(FILE* f) const {
		if (original_data) fprintf(f, "original %s ",original_data);
		if (old_data) fprintf(f, "old_data %s ",old_data);
		if (new_data) fprintf(f, "new_data %s ",new_data);
		fprintf(f, "\n");
	}
};

/************************************************
** AyuSysConfigIntlistItem
** 数値列をデータとしてもつ設定項目
*/
class AyuSysConfigIntlistItem {
	int item_deal;
	int* original_data;
	int* old_data;
	int* new_data;
public:
	AyuSysConfigIntlistItem(void) {
		item_deal = 0;
		original_data = 0;
		old_data = 0;
		new_data = 0;
	}
	AyuSysConfigIntlistItem(const AyuSysConfigIntlistItem& o) {
		item_deal = o.item_deal;
		original_data = 0; old_data = 0; new_data = 0;
		if (o.original_data) {
			original_data = new int[item_deal];
			memcpy(original_data, o.original_data, sizeof(int)*item_deal);
		}
		if (o.old_data) {
			old_data = new int[item_deal];
			memcpy(old_data, o.old_data, sizeof(int)*item_deal);
		}
		if (o.new_data) {
			new_data = new int[item_deal];
			memcpy(new_data, o.new_data, sizeof(int)*item_deal);
		}
	}
	/* 設定：Init で初期化、Set で変更、Get で変更を優先して取り出す */
	void Init(int deal, const int* list) { /* deal は無視 */
		if (original_data) delete[] original_data;
		original_data = 0;
		if (deal <= 0) {
			item_deal = 0; return;
		}
		item_deal = deal;
		original_data = new int[item_deal];
		memcpy(original_data, list, sizeof(int)*deal);
	}
	void Set(int deal, const int* list) { /* deal は無視 */
		if (item_deal == 0) return;
		if (deal != item_deal) return;
		if (new_data) delete[] new_data;
		new_data = new int[item_deal];
		memcpy(new_data, list, sizeof(int)*item_deal);
	}
	const int* Get(int deal) const {/* deal は無視 */
		if (item_deal == 0) return 0;
		if (new_data) return new_data;
		else if (old_data) return old_data;
		return original_data;
	}
	/* 変化の追跡：ClearDiff 以降に設定変更があれば Diff で取り出せる。
	** PatchOld で古い状態へ、PatchNew で新しい状態へ
	*/
	void ClearDiff(void) {
		if (new_data) {
			if (old_data) delete[] old_data;
			old_data = new_data;
			new_data = 0;
		}
	}
	int DiffLen(void) {
		if (new_data == 0) return 0;
		if (old_data == 0) {
			return item_deal * INT_SIZE+1;
		} else {
			return item_deal * INT_SIZE * 2+1;
		}
	}
	char* Diff(char* ret_str) {
		if (new_data == 0) { /* あり得ない */
			fprintf(stderr,"AyuSysConfigIntlistItem::Diff : this method must not called if not required!\n");
			return ret_str;
		}
		int i;
		if (old_data == 0) {
			*ret_str++ = 0;
		} else {
			*ret_str++ = 1;
			for (i=0; i<item_deal; i++) {
				write_little_endian_int(ret_str, old_data[i]);
				ret_str += INT_SIZE;
			}
		}
		for (i=0; i<item_deal; i++) {
			write_little_endian_int(ret_str, new_data[i]);
			ret_str += INT_SIZE;
		}
		return ret_str;
	}
	const char* PatchOld(const char* data) {
		int i;
		if (new_data) {
			delete[] new_data;
			new_data = 0;
		}
		if (*data++) {
			/* old_data が存在 */
			if (! old_data) old_data = new int[item_deal];
			for (i=0; i<item_deal; i++) {
				old_data[i] = read_little_endian_int(data);
				data += INT_SIZE;
			}
		} else {
			if (old_data) {
				delete[] old_data;
				old_data = 0;
			}
		}
		data += INT_SIZE*item_deal;
		return data;
	}
	const char* PatchNew(const char* data) {
		int i;
		if (new_data) {
			delete[] new_data;
			new_data = 0;
		}
		if (! old_data) old_data = new int[item_deal];
		if (*data++) {
			data += INT_SIZE * item_deal;
		}
		for (i=0; i<item_deal; i++) {
			old_data[i] = read_little_endian_int(data);
			data += INT_SIZE;
		}
		return data;
	}
	const char* DumpPatch(FILE* out, const char* data) const{
		int i;
		if (*data++) {
			fprintf(out, "(");
			for (i=0; i<item_deal; i++) {
				if (i) fprintf(out, ",");
				fprintf(out, "%d", read_little_endian_int(data));
				data += INT_SIZE;
			}
			fprintf(out, ") -> ");
		} else {
			fprintf(out, "(nothing) -> ");
		}
		fprintf(out, "(");
		for (i=0; i<item_deal; i++) {
			if (i) fprintf(out, ",");
			fprintf(out, "%d", read_little_endian_int(data));
			data += INT_SIZE;
		}
		fprintf(out, ")\n");
		return data;
	}
	/* オリジナルからの変化の調査 :
	** DiffOriginal で変化を文字列で取り出し、PatchOriginal で
	** 変化を反映
	*/
	int DiffOriginalLen(void) {
		if (new_data == 0 && old_data == 0) return 0;
		if (new_data == 0 || old_data == 0)
			return INT_SIZE * item_deal + 1;
		return INT_SIZE * item_deal * 2 + 1;
	}
	char* DiffOriginal(char* data) {
		if (new_data == 0 && old_data == 0) { /* あり得ない */
			fprintf(stderr,"AyuSysConfigStringItem::DiffOriginal : this method must not called if not required!\n");
			return data;
		}
		int i; char& flag = *data++; flag = 0;
		if (old_data) {
			flag |= 1;
			for (i=0; i<item_deal; i++) {
				write_little_endian_int(data, old_data[i]);
				data += INT_SIZE;
			}
		}
		if (new_data) {
			flag |= 2;
			for (i=0; i<item_deal; i++) {
				write_little_endian_int(data, new_data[i]);
				data += INT_SIZE;
			}
		}
		return data;
	}
	const char* PatchOriginal(const char* data) {
		if (old_data) delete[] old_data;
		if (new_data) delete[] new_data;
		old_data = 0; new_data = 0;
		char flag = *data++; int i;
		if (flag & 1) {
			old_data = new int[item_deal];
			for (i=0; i<item_deal; i++) {
				old_data[i] = read_little_endian_int(data);
				data += INT_SIZE;
			}
		}
		if (flag & 2) {
			new_data = new int[item_deal];
			for (i=0; i<item_deal; i++) {
				new_data[i] = read_little_endian_int(data);
				data += INT_SIZE;
			}
		}
		return data;
	}
	void SetOriginal(void) {
		if (new_data) delete[] new_data;
		if (old_data) delete[] old_data;
		new_data = 0; old_data = 0;
	}
	const char* DumpPatchOriginal(FILE* out, const char* data) const{
		int i;
		int flag = *data++;
		if (flag & 1) {
			fprintf(out, "(");
			for (i=0; i<item_deal; i++) {
				if (i) fprintf(out, ",");
				fprintf(out, "%d", read_little_endian_int(data));
				data += INT_SIZE;
			}
			fprintf(out, ") -> ");
		} else {
			fprintf(out, "(nothing) -> ");
		}
		if (flag & 2) {
			fprintf(out, "(");
			for (i=0; i<item_deal; i++) {
				if (i) fprintf(out, ",");
				fprintf(out, "%d", read_little_endian_int(data));
				data += INT_SIZE;
			}
			fprintf(out, ")\n");
		} else {
			fprintf(out, "(nothing)\n");
		}
		return data;
	}
	void Dump(FILE* f) const {
		fprintf(f, "item deal %d, ",item_deal);
		if (original_data) {
			fprintf(f, "(%d", original_data[0]);
			int i;for (i=1; i<item_deal; i++) {
				fprintf(f, ",%d",original_data[i]);
			}
			fprintf(f, ") ");
		}
		if (old_data) {
			fprintf(f, "old %08x(%d", (unsigned int)(old_data), old_data[0]);
			int i;for (i=1; i<item_deal; i++) {
				fprintf(f, ",%d",old_data[i]);
			}
			fprintf(f, ") ");
		}
		if (new_data) {
			fprintf(f, "new %08x(%d", (unsigned int)(new_data), new_data[0]);
			int i;for (i=1; i<item_deal; i++) {
				fprintf(f, ",%d",new_data[i]);
			}
			fprintf(f, ") ");
		}
		fprintf(f, "\n");
	}
};

map<HashStr, AyuSysConfigStringItem> tmp_var1;
map<HashStr, AyuSysConfigIntlistItem> tmp_var2;
//template map<HashStr, AyuSysConfigStringItem>;
//template map<HashStr, AyuSysConfigIntlistItem>;

/************************************************
** AyuSysConfigItem
** データ名 -> データ本体の map と、map 全体に
** 様々な操作を行うためのメソッド
*/

template<class ItemType, class DataType> class AyuSysConfigItem {
	typedef map<HashStr,ItemType> maptype;
	typedef typename maptype::iterator mapiterator;
	typedef typename maptype::const_iterator const_mapiterator;
	maptype data;
public:
	void SetOrig(HashStr& name, int deal, const DataType* str) {
		if (str == 0) return; /* 無効 */
		data[name].Init(deal, str);
	}
	void Set(HashStr& name, int deal, const DataType* new_data) {
		if (new_data == 0) return; /* 無効 */
		/* 設定を検索 */
		mapiterator it = data.find(name);
		/* 設定が元設定に見つからないなら失敗 */
		if (it == data.end()) {
			fprintf(stderr,"AyuSysConfigItem::Set : there is no '%s' parameter\n",name.c_str());
			return;
		}
		/* 設定を変更 */
		it->second.Set(deal, new_data);
	}
	/* 新しい設定を優先して返す */
	const DataType* Get(int deal, HashStr& name) const {
		const_mapiterator it = data.find(name);
		if (it == data.end()) return 0;
		return it->second.Get(deal);
	}
	void ClearDiff(void) {
		mapiterator it = data.begin();
		for (; it != data.end(); it++) {
			it->second.ClearDiff();
		}
		return;
	}
	int DiffLen(void) {
		mapiterator it = data.begin();
		int diff_len = 0;
		for (; it != data.end(); it++) {
			int len = it->second.DiffLen();
			if (len) diff_len += strlen(it->first.c_str()) + 1 + len + INT_SIZE;
		}
		return diff_len+INT_SIZE;
	}
	char* Diff(char* ret_str) {
		mapiterator it = data.begin();
		char* ret_str_first = ret_str;
		ret_str += INT_SIZE;
		for (; it != data.end(); it++) {
			int len = it->second.DiffLen();
			if (len) {
				char* ret_str_orig = ret_str;
				ret_str += INT_SIZE;
				strcpy(ret_str, it->first.c_str());
				ret_str += strlen(ret_str)+1;
				ret_str = it->second.Diff(ret_str);
				write_little_endian_int(ret_str_orig, ret_str-ret_str_orig);
			}
		}
		write_little_endian_int(ret_str_first, ret_str-ret_str_first);
		return ret_str;
	}
	const char* PatchOld(const char* diff_data) {
		const char* data_end = diff_data + read_little_endian_int(diff_data);
		diff_data += INT_SIZE;
		while(diff_data < data_end) {
			const char* next_data = diff_data + read_little_endian_int(diff_data);
			diff_data += INT_SIZE;
			mapiterator it = data.find(diff_data);
			if (it != data.end()) {
				diff_data += strlen(diff_data)+1;
				it->second.PatchOld(diff_data);
			}
			diff_data = next_data;
		}
		return data_end;
	}
	const char* PatchNew(const char* diff_data) {
		const char* data_end = diff_data + read_little_endian_int(diff_data);
		diff_data += INT_SIZE;
		while(diff_data < data_end) {
			const char* next_data = diff_data + read_little_endian_int(diff_data);
			diff_data += INT_SIZE;
			mapiterator it = data.find(diff_data);
			if (it != data.end()) {
				diff_data += strlen(diff_data)+1;
				it->second.PatchNew(diff_data);
			}
			diff_data = next_data;
		}
		return data_end;
	}
	const char* DumpPatch(FILE* out, const char* tab, const char* diff_data) const {
		const char* data_end = diff_data + read_little_endian_int(diff_data);
		diff_data += INT_SIZE;
		while(diff_data < data_end) {
			const char* new_data = diff_data + read_little_endian_int(diff_data);
			diff_data += INT_SIZE;
			const_mapiterator it = data.find(diff_data);
			if (it != data.end()) {
				fprintf(out, "%s%s : ",tab,diff_data);
				diff_data += strlen(diff_data)+1;
				it->second.DumpPatch(out, diff_data);
			}
			diff_data = new_data;
		}
		return data_end;
	}
	/* オリジナルからの変化の調査 :
	** DiffOriginal で変化を文字列で取り出し、PatchOriginal で
	** 変化を反映
	*/
	int DiffOriginalLen(void) {
		mapiterator it = data.begin();
		int diff_len = 0;
		for (; it != data.end(); it++) {
			int len = it->second.DiffOriginalLen();
			if (len) diff_len += strlen(it->first.c_str()) + 1 + len + INT_SIZE;
		}
		return diff_len+INT_SIZE;
	}
	char* DiffOriginal(char* ret_str) {
		mapiterator it = data.begin();
		char* ret_str_first = ret_str;
		ret_str += INT_SIZE;
		for (; it != data.end(); it++) {
			int len = it->second.DiffOriginalLen();
			if (len) {
				char* ret_str_orig = ret_str;
				ret_str += INT_SIZE;
				strcpy(ret_str, it->first.c_str());
				ret_str += strlen(ret_str)+1;
				ret_str = it->second.DiffOriginal(ret_str);
				write_little_endian_int(ret_str_orig, ret_str-ret_str_orig);
			}
		}
		write_little_endian_int(ret_str_first, ret_str-ret_str_first);
		return ret_str;
	}
	const char* PatchOriginal(const char* diff_data) {
		const char* data_end = diff_data + read_little_endian_int(diff_data);
		diff_data += INT_SIZE;
		while(diff_data < data_end) {
			const char* next_data = diff_data + read_little_endian_int(diff_data);
			diff_data += INT_SIZE;
			mapiterator it = data.find(diff_data);
			if (it != data.end()) {
				diff_data += strlen(diff_data)+1;
				it->second.PatchOriginal(diff_data);
			}
			diff_data = next_data;
		}
		return data_end;
	}
	const char* DumpPatchOriginal(FILE* out, const char* tab, const char* diff_data) const {
		const char* data_end = diff_data + read_little_endian_int(diff_data);
		diff_data += INT_SIZE;
		while(diff_data < data_end) {
			const char* new_data = diff_data + read_little_endian_int(diff_data);
			diff_data += INT_SIZE;
			const_mapiterator it = data.find(diff_data);
			if (it != data.end()) {
				fprintf(out, "%s%s : ",tab,diff_data);
				diff_data += strlen(diff_data)+1;
				it->second.DumpPatchOriginal(out, diff_data);
			}
			diff_data = new_data;
		}
		return data_end;
	}
	void SetOriginal(void) {
		mapiterator it = data.begin();
		for (; it != data.end(); it++) {
			it->second.SetOriginal();
		}
	}
	void Dump(FILE* f) const {
		const_mapiterator it = data.begin();
		for (; it != data.end(); it++) {
			fprintf(f, "name %s: ",it->first.c_str());
			it->second.Dump(f);
		}
	}
};
//template AyuSysConfigItem<AyuSysConfigStringItem, char> tmp_var3;
//template AyuSysConfigItem<AyuSysConfigIntlistItem, int> tmp_var4;
AyuSysConfigItem<AyuSysConfigStringItem, char> tmp_var3;
AyuSysConfigItem<AyuSysConfigIntlistItem, int> tmp_var4;

/************************************************/
/* ラッパ */
struct AyuSysConfigString {
	AyuSysConfigItem<AyuSysConfigStringItem,char> orig;
	void Dump(FILE* f) const {
		fprintf(f, "string config:\n");
		orig.Dump(f);
	}
};
struct AyuSysConfigIntlist {
	AyuSysConfigItem<AyuSysConfigIntlistItem, int> orig;
	void Dump(FILE* f) const {
		fprintf(f, "integer array config:\n");
		orig.Dump(f);
	}
};

/************************************************/
/* AyuSysConfig クラス */
int AyuSysConfig::SearchParam(const char* name) {
	HashStr str(name);
	if (str_config->orig.Get(1, str)) return 1; /* char* のパラメータ */
	else if (int_config->orig.Get(1, str)) return 2; /* int のパラメータ */
	else return 0;
}
const char* AyuSysConfig::GetParaStr(const char* name) {
	HashStr str(name);
	const char* ret = str_config->orig.Get(1,str);
	if (ret == 0) {
		fprintf(stderr,"Cannot find config name '%s'\n",name);
	}
	return ret;
}
int AyuSysConfig::GetParam(const char* name, int deal, ...) {
	HashStr str(name);
	va_list va; int i;
	const int* vars = int_config->orig.Get(deal, str);
	if (vars == 0) {
		fprintf(stderr,"Cannot find config name '%s'\n",name);
		va_start(va, deal);
		for (i=0; i<deal; i++) *(va_arg(va, int*)) = 0;
		return -1;
	} else {
		va_start(va, deal);
		for (i=0; i<deal; i++) *(va_arg(va, int*)) = vars[i];
	}
	return 0;
}
void AyuSysConfig::SetParaStr(const char* name, const char* var) {
	HashStr str(name);
	dirty_flag = 1; change_flag = 1;
	str_config->orig.Set(str, 1, var);
}
void AyuSysConfig::SetParam(const char* name, int deal, ...) {
	if (deal >= MAXVARS) return ;
	HashStr str(name);
	int vars[MAXVARS]; va_list va; int i;
	va_start(va, deal);
	for (i=0; i<deal; i++) vars[i] = va_arg(va, int);
	int_config->orig.Set(str, deal, vars);
	dirty_flag = 1; change_flag = 1;
	return;
}
void AyuSysConfig::SetOrigParaStr(const char* name, const char* var) {
	HashStr str(name);
	str_config->orig.SetOrig(str, 1, var);
	change_flag = 1;
}
void AyuSysConfig::SetOrigParam(const char* name, int deal, ...) {
	if (deal >= MAXVARS) return;
	HashStr str(name);
	int vars[MAXVARS]; va_list va; int i;
	va_start(va, deal);
	for(i=0; i<deal; i++) vars[i] = va_arg(va, int);
	int_config->orig.SetOrig(str, deal, vars);
	change_flag = 1;
}
void AyuSysConfig::SetOrigParamArray(const char* name, int deal, int* array) {
	HashStr str(name);
	int_config->orig.SetOrig(str, deal, array);
}
void AyuSysConfig::SetOriginal(void) {
	/* 全ての設定を元に戻す */
	str_config->orig.SetOriginal();
	int_config->orig.SetOriginal();
	change_flag = 1;
}
int AyuSysConfig::DiffOriginalLen(void) {
	return 1 + str_config->orig.DiffOriginalLen() +
		int_config->orig.DiffOriginalLen();
}
char* AyuSysConfig::DiffOriginal(char* data) {
	*data++ = dirty_flag;
	data = str_config->orig.DiffOriginal(data);
	data = int_config->orig.DiffOriginal(data);
	return data;
}
const char* AyuSysConfig::PatchOriginal(const char* data) {
	change_flag = 1;
	if (*data++) dirty_flag = 1; else dirty_flag = 0;
	data = str_config->orig.PatchOriginal(data);
	data = int_config->orig.PatchOriginal(data);
	return data;
}
const char* AyuSysConfig::DumpPatchOriginal(FILE* out, const char* tab, const char* data) const {
	data++;
	data = str_config->orig.DumpPatchOriginal(out, tab, data);
	data = int_config->orig.DumpPatchOriginal(out, tab, data);
	return data;
}
int AyuSysConfig::DiffLen(void) {
	if (! IsDiff()) return 0;
	return 3+str_config->orig.DiffLen() + int_config->orig.DiffLen();
}
void AyuSysConfig::ClearDiff(void) {
	dirty_flag = 0;
	str_config->orig.ClearDiff();
	int_config->orig.ClearDiff();
}
char* AyuSysConfig::Diff(char* data) {
	if (! IsDiff()) return data;
	*data++ = 1; /* とりあえず予約項目(データ種別) */
	data = str_config->orig.Diff(data);
	*data++ = 2; /* とりあえず予約項目(データ種別) */
	data = int_config->orig.Diff(data);
	*data++ = 0; /* 終了 */
	return data;
}
const char* AyuSysConfig::PatchNew(const char* data) {
	dirty_flag = 0; change_flag = 1;
	data++;
	data = str_config->orig.PatchNew(data);
	data++;
	data = int_config->orig.PatchNew(data);
	data++;
	return data;
}
const char* AyuSysConfig::PatchOld(const char* data) {
	dirty_flag = 0; change_flag = 1;
	data++;
	data = str_config->orig.PatchOld(data);
	data++;
	data = int_config->orig.PatchOld(data);
	data++;
	return data;
}
const char* AyuSysConfig::DumpPatch(FILE* out, const char* tab, const char* data) const {
	data++;
	data = str_config->orig.DumpPatch(out, tab, data);
	data++;
	data = int_config->orig.DumpPatch(out, tab, data);
	data++;
	return data;
}

void AyuSysConfig::Dump(FILE* f) const {
	str_config->Dump(f);
	int_config->Dump(f);
}

/************************************************
** AyuSysConfig のコンストラクタ：
** 全ての config 項目を初期化する
*/
AyuSysConfig::AyuSysConfig(void) {
	change_flag = 1; dirty_flag = 0;
	str_config = new AyuSysConfigString;
	int_config = new AyuSysConfigIntlist;

	/****** 文字列 *******/
	SetOrigParaStr("#WAKUPDT", "GRDAT");	/* 枠、マウスカーソルなどの画像ファイル */
	SetOrigParaStr("#REGNAME", "xkanon");	/* レジストリ名。セーブファイルの作成に使う */
	SetOrigParaStr("#CAPTION", "xkanon");	/* ウィンドウのタイトル */
	SetOrigParaStr("#SAVENAME","SAVE.INI");	/* セーブファイルの名前 */
	SetOrigParaStr("#SAVETITLE", "This is save file"); /* セーブファイルの先頭の文字列 */
	SetOrigParaStr("#SAVENOTITLE", "-----------------"); /* 使われてないセーブデータの名前 */
	SetOrigParaStr("#CGM_FILE", "MODE.CGM");/* CG mode の設定が保存されたファイル名 */

	/****** 数値列 *******/
	SetOrigParam("#COM2_TITLE", 1, 1); /* ？ */
	SetOrigParam("#COM2_TITLE_COLOR", 1, 2); /* 選択肢タイトルの色 */
	SetOrigParam("#COM2_TITLE_INDENT", 1, 2); /* ？ */
	SetOrigParam("#SAVEFILETIME", 1, 24); /* セーブする場所の数 */
	SetOrigParam("#SEEN_START", 1, 0); /* ゲームを開始するシナリオ番号 */
	SetOrigParam("#SEEN_SRT", 1, 0); /* ゲームを開始するシナリオ番号(好き好き大好き) */
	SetOrigParam("#SEEN_MENU",  1, 0); /* メニューのシナリオ番号 */
	SetOrigParam("#SEEN_TEXT_CURRENT",  1, 0); /* seen.txt を root directory に置くか */
	SetOrigParam("#FADE_TIME", 1, 40); /* 画面のフェード・アウトの速度 */
	SetOrigParam("#NVL_SYSTEM",1, 0); /* テキストウィンドウが全画面か否か */
	SetOrigParam("#WINDOW_ATTR", 3, 80, 112,160); /* テキストウィンドウの色 */
	SetOrigParam("#WINDOW_ATTR_AREA", 4, 4,4,4,4); /* テキストウィンドウの範囲 */
	SetOrigParam("#WINDOW_ATTR_TYPE", 1, 0); /* テキストウィンドウを半透明にするか */
	SetOrigParam("#WINDOW_MSG_POS", 2, 22, 350); /* テキストウィンドウの位置 */
	SetOrigParam("#WINDOW_COM_POS", 2,450, 250); /* 選択ウィンドウの位置 */
	SetOrigParam("#WINDOW_GRP_POS", 2, 16, 100); /* なにかのウィンドウの位置 */
	SetOrigParam("#WINDOW_SUB_POS", 2, 48, 100); /* なにかのウィンドウの位置 */
	SetOrigParam("#WINDOW_SYS_POS", 2, 32, 100); /* なにかのウィンドウの位置 */
	SetOrigParam("#WINDOW_WAKU_TYPE", 1, 0); /* テキストウィンドウの枠の種類。xkanon 独自設定 */
	SetOrigParam("#RETN_CONT", 1, 16); /* リターンカーソルの数 */
	SetOrigParam("#RETN_SPEED",1,100); /* リターンカーソルの動く速度 */
	SetOrigParam("#RETN_XSIZE", 1, 16); /* リターンカーソルの大きさ */
	SetOrigParam("#RETN_YSIZE", 1, 16); /* リターンカーソルの大きさ */
	SetOrigParam("#FONT_SIZE", 1, 26); /* フォントの大きさ */
	SetOrigParam("#FONT_WEIGHT", 1, 100); /* フォントの weight */
	SetOrigParam("#MSG_MOJI_SIZE", 2, 12, 29); /* 文字の大きさ(半角) */
	SetOrigParam("#MESSAGE_SIZE", 2, 23, 3); /* メッセージウィンドウの文字数 */
	SetOrigParam("#COM_MESSAGE_SIZE", 2, 23, 3); /* メッセージウィンドウの文字数 */
	SetOrigParam("#GRP_DC_TIMES", 1, 4); /* 裏画面の数 */
	SetOrigParam("#MUSIC_LINEAR_PAC",1,0); /* PCM データの 8bit -> 16bit 変換を行うか */
	SetOrigParam("#MUSIC_TYPE",1,0); /* PCM データの種類 */
	SetOrigParam("#WINDOW_MSGBK_BOX",1,0); /* バックログ用のボタン */
	SetOrigParam("#WINDOW_MSGBK_LBOX_POS",4,15,7,8,0); /* バックログ用のボタン(左)の位置 */
	SetOrigParam("#WINDOW_MSGBK_RBOX_POS",4,7,7,0,0); /* バックログ用のボタン(左)の位置 */
	SetOrigParam("#MSGBK_MOD",1,0); /* バックログ用のボタンを使用するか */
}

static int SplitVar(const char* str, int* ret_var, int ret_size) {
	/* , あるいは ),( をセパレータとして、-?[0-9]+ の
	** フォーマットの数値列を読み込む。先頭に (、末尾に ) が付きうる。
	** (),-[0-9] 以外の文字があったらそこで終了
	** 得られたデータ数を返す
	*/
	if (*str == '(') str++;
	int i; for (i=0; i<ret_size; i++) {
		int c; int is_positive = 1;
		/* セパレータの読み飛ばし */
		c = *str;
		if (c == ',') {
			str++;
		} else if (c == ')' && str[1] == '(') {
			str += 2;
		}
		/* - を parse */
		c = *str;
		if (c == '-' && isdigit(str[1])) {
			is_positive = -1; str++;
		} else if (! isdigit(c)) {
			return i; /* 異常な文字を見つけた：終了 */
		}
		int number = 0;
		/* 数字読み込み */
		while(isdigit( (c=*str) )) {
			number *= 10;
			number += c-'0';
			str++;
		}
		ret_var[i] = is_positive * number;
	}
	return i;
}
/* 決められた数の引数を得る。-1 ならエラーが生じた */
static inline int SplitVar(const char* str, int& var1) {
	if (SplitVar(str, &var1, 1) != 1) return -1;
	return 0;
}
static inline int SplitVar(const char* str, int& var1, int& var2) {
	int vars[2];
	if (SplitVar(str, vars, 2) != 2) return -1;
	var1 = vars[0]; var2 = vars[1];
	return 0;
}
static inline int SplitVar(const char* str, int& var1, int& var2, int& var3) {
	int vars[3];
	if (SplitVar(str, vars, 3) != 3) return -1;
	var1 = vars[0]; var2 = vars[1]; var3 = vars[2];
	return 0;
}
static inline int SplitVar(const char* str, int& var1, int& var2, int& var3, int& var4) {
	int vars[4];
	if (SplitVar(str, vars, 4) != 4) return -1;
	var1 = vars[0]; var2 = vars[1]; var3 = vars[2]; var4 = vars[3];
	return 0;
}

bool AyuSys::LoadInitFile(void)
{
	char buf[1024]; int i;
	char* tokens[MAXTOKEN]; int token_deal; int buf_ptr;
	int numbers[MAXVARS];

	/* sels[0] を初期化しておく */
	sels[0] = new SEL_STRUCT;
	sels[0]->x1=sels[0]->y1=sels[0]->x3=sels[0]->y3=0;
	sels[0]->x2=DefaultScreenWidth()-1;sels[0]->y2=DefaultScreenHeight()-1;sels[0]->wait_time=50;
	sels[0]->sel_no=4;sels[0]->kasane=0;
	sels[0]->arg1=0; sels[0]->arg2=0; sels[0]->arg3=0;
	sels[0]->arg4=0; sels[0]->arg5=0; sels[0]->arg6=0;

	ARCINFO* info = file_searcher.Find(FILESEARCH::ROOT, "gameexe.ini");
	if (info == NULL) return false;
	int size = info->Size();
	unsigned char* buf_orig = (unsigned char*)info->Read();
	if (size <= 0 || buf_orig == 0) {
		delete info; return false;
	}
	unsigned char* buf_end = buf_orig + size;
	int line_count = 0;
	while(buf_orig < buf_end) {
		/* buf_orig から一行読み込む */
		/* その際に、
		** ・頭が # 以外なら次の行までとばす
		** ・"" 外のスペース、TABを初めとする制御文字 (0x20 以下のASCIIコード)を削除
		** ・= で区切る。区切りは最大で10個で、tokens に代入される
		** などの操作を行う
		*/

		/* # チェック */
		if (*buf_orig != '#') {
			/* 次の '\n' まで読み飛ばし */
			while(buf_orig < buf_end &&
				*buf_orig != '\n' && *buf_orig != '\r') buf_orig++;
			if (buf_orig < buf_end-1 && *buf_orig == '\r' && buf_orig[1] == '\n') buf_orig += 2;
			else if (*buf_orig == '\r' || *buf_orig == '\n') buf_orig++;
			line_count++;
			continue;
		}
		/* 初期化 */
		token_deal = 1; tokens[0] = buf; buf_ptr = 0;
		int in_quote = 0;

		while(buf_orig < buf_end && buf_ptr < 1023) {
			if (in_quote) {
				/* "" の中 */
				int c = *buf_orig;
				if (c == '\n' || c == '\r') {
					break;
				} else if (c == '\"') {
					in_quote = 0;
				} else {
					buf[buf_ptr++] = c;
				}
				buf_orig++;
			} else { /* quote されてない */
				/* 制御文字を読み飛ばす */
				while(*buf_orig <= 0x20 && buf_orig < buf_end &&
					*buf_orig != '\n' && *buf_orig != '\r') buf_orig++;
				int c = *buf_orig;
				if (c == '\n' || c == '\r') break;
				/* = なら次の token */
				if (c == '=') {
					c = 0; tokens[token_deal++] = buf+buf_ptr+1;
					if (token_deal >= MAXTOKEN) break;
				} else if (c == '\"') {
					in_quote = 1; buf_orig++; continue;
				}
				buf[buf_ptr++] = c;
				buf_orig++;
			}
		}
		buf[buf_ptr] = '\0';
		/* 末尾の \r\n を消去 */
		if (buf_orig < buf_end-1 && buf_orig[0] == '\r' && buf_orig[1] == '\n') buf_orig += 2;
		else if (buf_orig < buf_end && (buf_orig[0] == '\r' || buf_orig[0] == '\n')) buf_orig++;
		/* 必要なら parse 内容を出力 */
		dprintf(("line %3d ",line_count));
		for (i=0; i<token_deal; i++) {
			dprintf(("%d:\"%s\", ",i,tokens[i]));
		}
		dprintf(("\n"));
		if (in_quote) {
			fprintf(stderr, "Warning : open quote is found while parsing gameexe.ini, line %d\n",line_count);
		}


		/* 得られた内容を parse */

		/* #NAME=<文字列> */
		int type = config->SearchParam(tokens[0]);
		if (type == 1) { /* #NAME=<文字列> */
			if (token_deal != 2) {
				dprintf(("Parse error, line %d, %s\n",line_count, tokens[0]));
				goto parse_error;
			}
			config->SetOrigParaStr(tokens[0], tokens[1]);
			goto parse_end;
		} else if (type == 2) { /* #NAME=<数値列> */
			if (token_deal != 2) {
				dprintf(("Parse error, line %d, %s\n",line_count, tokens[0]));
				goto parse_error;
			}
			int number_deal = SplitVar(tokens[1], numbers, MAXVARS);
			config->SetOrigParamArray(tokens[0], number_deal, numbers);
			goto parse_end;
		}
		/* 一般的な設定以外 : sel, cdrom track など */
		/* 以下の設定項目はセーブされない */
		if (strncmp(tokens[0], "#SEL.", 5) == 0) {
			if (token_deal != 2) goto parse_error;
			int sel_no; if (SplitVar(tokens[0]+5, sel_no) == -1) goto parse_error;
			if (sel_no > SEL_DEAL || sel_no < 0) goto parse_error;
			if (SplitVar(tokens[1], numbers, 15) != 15) goto parse_error;

			sels[sel_no] = new SEL_STRUCT;
			SEL_STRUCT& sel = *sels[sel_no];
			sel.x1 = numbers[0]; sel.y1 = numbers[1];
			sel.x2 = numbers[2]; sel.y2 = numbers[3];
			sel.x3 = numbers[4]; sel.y3 = numbers[5];
			sel.wait_time = numbers[6];
			sel.sel_no = numbers[7];
			sel.kasane = numbers[8];
			sel.arg1 = numbers[ 9]; sel.arg2 = numbers[10];
			sel.arg3 = numbers[11]; sel.arg4 = numbers[12];
			sel.arg5 = numbers[13]; sel.arg6 = numbers[14];
			dprintf(("set SEL[%d]\n",sel_no));
			goto parse_end;
		}
		if (strncmp(tokens[0],"#DIRC.",6) == 0) {
			if (token_deal != 3) goto parse_error;
			/* ファイル形式の指定 */
			FILESEARCH::FILETYPE type;
			char* name = tokens[0]+6;
			if (strcmp(name, "PDT") == 0) type = FILESEARCH::PDT;
			else if (strcmp(name, "TXT") == 0) type = FILESEARCH::SCN;
			else if (strcmp(name, "ANM") == 0) type = FILESEARCH::ANM; 
			else if (strcmp(name, "ARD") == 0) type = FILESEARCH::ARD;
			else if (strcmp(name, "CUR") == 0) type = FILESEARCH::CUR;
			else if (strcmp(name, "WAV") == 0) type = FILESEARCH::WAV;
			else if (strcmp(name, "KOE") == 0) type = FILESEARCH::KOE;
			else goto parse_error; /* 他に ALL,ROOT,MID,KOE,BGM。たぶん、存在しない */
			if (tokens[2][0] == 'N') { /* directory */
				file_searcher.SetFileInformation(type, FILESEARCH::ATYPE_DIR, tokens[1]);
				dprintf(("set file directory; type %s, directory %s\n",name,tokens[1]));
			} else if (tokens[2][0] == 'P' && tokens[2][1] == ':') { /* アーカイブ */
				file_searcher.SetFileInformation(type, FILESEARCH::ATYPE_ARC, tokens[2]+2);
				dprintf(("set file archive; type %s, file %s\n",name,tokens[2]+2));
			} else goto parse_error;
			goto parse_end;
		}
		if (strncmp(tokens[0],"#ADRC.",6) == 0) {
			if (token_deal != 3) goto parse_error;
			/* ファイル形式の指定 */
			FILESEARCH::FILETYPE type;
			char* name = tokens[0]+6;
			if (strcmp(name, "PDT") == 0) type = FILESEARCH::PDT;
			else if (strcmp(name, "TXT") == 0) type = FILESEARCH::SCN;
			else if (strcmp(name, "ANM") == 0) type = FILESEARCH::ANM; 
			else if (strcmp(name, "ARD") == 0) type = FILESEARCH::ARD;
			else if (strcmp(name, "CUR") == 0) type = FILESEARCH::CUR;
			else if (strcmp(name, "WAV") == 0) type = FILESEARCH::WAV;
			else if (strcmp(name, "KOE") == 0) type = FILESEARCH::KOE;
			else goto parse_error; /* 他に ALL,ROOT,MID,KOE,BGM。たぶん、存在しない */
			if (tokens[2][0] == 'N') { /* directory */
				file_searcher.AppendFileInformation(type, FILESEARCH::ATYPE_DIR, tokens[1]);
				dprintf(("set file directory; type %s, directory %s\n",name,tokens[1]));
			} else if (tokens[2][0] == 'P' && tokens[2][1] == ':') { /* アーカイブ */
				file_searcher.AppendFileInformation(type, FILESEARCH::ATYPE_ARC, tokens[2]+2);
				dprintf(("set file archive; type %s, file %s\n",name,tokens[2]+2));
			} else if (tokens[2][0] == 'R' && tokens[2][1] == ':') { /* それ散るアーカイブ */
				file_searcher.AppendFileInformation(type, FILESEARCH::ATYPE_Raffresia, tokens[2]+2);
				dprintf(("set file archive; type %s, file %s\n",name,tokens[2]+2));
			} else goto parse_error;
			goto parse_end;
		}
		if (strncmp(tokens[0],"#FOLDNAME.",10) == 0) {
			if (token_deal != 3) goto parse_error;
			/* ファイル形式の指定 */
			FILESEARCH::FILETYPE type;
			char* name = tokens[0]+10;
			if (strcmp(name, "PDT") == 0) type = FILESEARCH::PDT;
			else if (strcmp(name, "TXT") == 0) type = FILESEARCH::SCN;
			else if (strcmp(name, "ANM") == 0) type = FILESEARCH::ANM; 
			else if (strcmp(name, "ARD") == 0) type = FILESEARCH::ARD;
			else if (strcmp(name, "CUR") == 0) type = FILESEARCH::CUR;
			else if (strcmp(name, "WAV") == 0) type = FILESEARCH::WAV;
			else if (strcmp(name, "BGM") == 0) type = FILESEARCH::BGM;
			else goto parse_error; /* 他に ALL,ROOT,MID,KOE,BGM。たぶん、存在しない */
			if (tokens[2][0] == '0') { /* directory */
				file_searcher.SetFileInformation(type, FILESEARCH::ATYPE_DIR, tokens[1]);
				dprintf(("set file directory; type %s, directory %s\n",name,tokens[1]));
			} else if (tokens[2][0] == '1' && tokens[2][1] == ':') { /* アーカイブ */
				file_searcher.SetFileInformation(type, FILESEARCH::ATYPE_SCN2k, tokens[2]+2);
				dprintf(("set file archive; type %s, file %s\n",name,tokens[2]+2));
			} else goto parse_error;
			goto parse_end;
		}
		if (strncmp(tokens[0],"#COLOR_TABLE.",13) == 0) {
			if (token_deal != 2) goto parse_error;
			int n, c1, c2, c3;
			n = atoi(tokens[0]+13);
			if (SplitVar(tokens[1], c1, c2, c3) == -1) goto parse_error;
			dprintf(("set color: %d <- %d,%d,%d\n",n,c1,c2,c3));
			if (n < COLOR_TABLE_DEAL)
				colors[n].SetColor(c1,c2,c3);
			goto parse_end;
		}
		if (strncmp(tokens[0],"#FADE_TABLE.",12) == 0) {
			if (token_deal != 2) goto parse_error;
			int n, c1, c2, c3;
			n = atoi(tokens[0]+13);
			if (SplitVar(tokens[1], c1, c2, c3) == -1) goto parse_error;
			dprintf(("set fade: %d <- %d,%d,%d\n",n,c1,c2,c3));
			if (n < COLOR_TABLE_DEAL)
				fades[n].SetColor(c1,c2,c3);
			goto parse_end;
		}
		if (strncmp(tokens[0], "#NAME.",6) == 0) {
			if (token_deal != 2) goto parse_error;
			if (strlen(tokens[0]) != 7) goto parse_error;
			int n = tokens[0][6]-'A'; if (n<0 || n>=26) goto parse_error;
			ini_macroname[n] = strdup(tokens[1]);
			dprintf(("Set MACRO %d <- %s\n",n,ini_macroname));
			goto parse_end;
		}
		if (strcmp(tokens[0], "#CDTRACK") == 0) {
			if (token_deal != 3) goto parse_error;
			track_name.AddCDROM(tokens[2], atoi(tokens[1]));
			dprintf(("Set CDTRACK, name %s, track %d\n",tokens[2], atoi(tokens[1])));
			goto parse_end;
		}
		if (strcmp(tokens[0], "#DSTRACK") == 0) {
			/* #DSTRACK=00000000-99999000-00782556="filename"   ="name"       */
			/* #DSTRACK=00000000-99999000-00782556="name"       */
			if (token_deal == 3) {
				track_name.AddWave(tokens[2], tokens[2]);
				dprintf(("Set Wave track, name %s\n",tokens[2]));
			} else if (token_deal == 4) {
				track_name.AddWave(tokens[3], tokens[2]);
				dprintf(("Set Wave track, name %s, file %s\n",tokens[3], tokens[2]));
			} else goto parse_error;
			goto parse_end;
		}
		if (strncmp(tokens[0], "#SE.", 4) == 0) {
			/* SE.XXX="XXX" */
			if (token_deal != 2) goto parse_error;
			track_name.AddSE(atoi(tokens[0]+4), tokens[1]);
			dprintf(("Set SE %d, name %s\n",atoi(tokens[0]+4), tokens[1]));
			goto parse_end;
		}
		if (strncmp(tokens[0], "#SHAKE.",7) == 0) {
			if (token_deal != 2) goto parse_error;
			int n = atoi(tokens[0]+7);
			if (n < 0 || n >= SHAKE_DEAL) goto parse_error;
			/* テーブルの数は 3 の倍数 */
			int size = SplitVar(tokens[1], numbers, MAXVARS);
			if (size <= 0 || (size%3) != 0) goto parse_error;
			shake[n] = new int[size+3];
			memcpy(shake[n], numbers, sizeof(int)*size);
			shake[n][size] = 0; shake[n][size+1] = 0; shake[n][size+2] = -1;
			dprintf(("set shake[%d] <- ",n));
			for (i=0; i<size; i++) dprintf(("%d, ",shake[n][i]));
			dprintf(("\n"));
			goto parse_end;
		}
                if (strncmp(tokens[0], "#EXFONT_N_NAME", 14) == 0) {
                        if (token_deal != 2) goto parse_error;
                        config->SetOrigParaStr(tokens[0], tokens[1]);
                        goto parse_end;
                }
                if (strncmp(tokens[0], "#EXFONT_N_XCONT", 15)) {
                        if (token_deal != 2) goto parse_error;
                        config->SetOrigParam(tokens[0], 1, atoi(tokens[1]));
                        goto parse_end;
                }
                if (strncmp(tokens[0], "#EXFONT_N_YCONT", 15)) {
                        if (token_deal != 2) goto parse_error;
                        config->SetOrigParam(tokens[0], 1, atoi(tokens[1]));
                        goto parse_end;
                }
                if (strncmp(tokens[0], "#EXFONT_N_XSIZE", 15)) {
                        if (token_deal != 2) goto parse_error;
                        config->SetOrigParam(tokens[0], 1, atoi(tokens[1]));
                        goto parse_end;
                }
                if (strncmp(tokens[0], "#EXFONT_N_YSIZE", 15)) {
                        if (token_deal != 2) goto parse_error;
                        config->SetOrigParam(tokens[0], 1, atoi(tokens[1]));
                        goto parse_end;
                }
		/* 設定項目が見つからなかった */
		dprintf(("Cannot find configuration name: %s\n",tokens[0]));
	parse_error:
	parse_end:
		line_count++;
	}
	delete info;
	/* デフォルトのオプションを指定する */
	set_game(config->GetParaStr("#REGNAME"), *this);
	return true;
}

void AyuSys::InitConfig(void) {
	config = new AyuSysConfig();
	int i; for (i=0; i<26; i++) ini_macroname[i] = 0;
}
