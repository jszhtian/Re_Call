/* senario_write.cc
**	新しくシナリオファイルを構築する
**	原則として、上から順番に手続きを書いていく
*/

class SENARIO_Writer {
	char* data;
	int basepoint;
	int point;
	int datalen;
	void* labelmapptr; /* map<string,LabelList> へのポインタ */
	void* gluelistptr; /* vector<int> へのポインタ */
public:
	SENARIO_Writer(int basep);
	~SENARIO_Writer();
	const char* GetBasedata(void);
	int GetBasedataLen(void);
	void CheckLen(int add_length = 10);
	void SetPoint(int point);
	int GetPoint(void);
	void WriteData(int data, int flag=0);
	void WriteVar(int var_num){ WriteData(var_num, 1);}
	void WriteData(class SENARIO_Writer_Number n);
	void WriteString(const char* string);
	void WriteString(class SENARIO_Writer_Number n);
	void WriteChar(char c);
	void WriteNullchar(void) { WriteChar(0); }
	void WriteInt(int data);
	void WriteLabel(const char* label);
	void WriteGlue(int count);/* 使わなければ消去される領域を確保 */
	void DeleteGlue(void); /* 消去 */
	void SetLabel(const char* label);
	void ResolvLabel(void);
	void Dump(void);
	void DumpFile(const char* fname);
};

#include"file.h"
#include<vector>
#include<deque>
#include<list>
#include<map>
#include<string>
#include<algorithm>

using namespace std;

class LabelList {
	string label;
	int label_point;
	vector<int> label_ref;
public:
	LabelList(void) : label(""),label_point(-1) {}
	void Init(string new_label) { label=new_label; }
	void SetLabelPoint(int labelp) {
		label_point = labelp;
	}
	void AddLabelRef(int ref) {
		label_ref.push_back(ref);
	}
	int ResolvRef(SENARIO_Writer& writer);
};

/* system.cc より */
static void kconv_rev(const unsigned char* src, unsigned char* dest) {
	/* input : euc output: sjis */
	while(*src) {
		unsigned int high = *src++;
		if (high < 0x80) {
			/* ASCII */
			*dest++ = high; continue;
		} else if (high == 0x8e) { /* hankaku KANA */
			high = *src;
			if (high >= 0xa0 && high < 0xe0)
				*dest++ = *src++;
			continue;
		} else {
			unsigned int low = *src++;
			if (low == 0) break; /* incorrect code , EOS */
			if (low < 0x80) continue; /* incorrect code */
			/* convert */
			low &= 0x7f; high &= 0x7f;
			low += (high & 1) ? 0x1f : 0x7d;
			high = (high-0x21)>>1;
			high += (high > 0x1e) ? 0xc1 : 0x81;
			*dest++ = high;
			if (low > 0x7f) low++;
			*dest++ = low;
		}
	}
	*dest = 0;
}
SENARIO_Writer::SENARIO_Writer(int basep) {
	data = 0;
	basepoint = basep;
	point = 0;
	datalen = 0;
	labelmapptr = (void*)(new map<string,LabelList>);
	gluelistptr = (void*)(new vector<int>);
}
SENARIO_Writer::~SENARIO_Writer() {
	delete (map<string,LabelList>*) labelmapptr;
	delete (vector<int>*) gluelistptr;
}
const char* SENARIO_Writer::GetBasedata(void) {
	return data;
}
int SENARIO_Writer::GetBasedataLen(void) {
	return datalen;
}
void SENARIO_Writer::SetPoint(int p) {
	if (p >= basepoint)
		point = p-basepoint;
}
int SENARIO_Writer::GetPoint(void) {
	return point+basepoint;
}
void SENARIO_Writer::CheckLen(int add_length) {
	/* step は 4096 byte */
	if (add_length <= 0) add_length = 10;
	if (point+add_length < datalen) return;
	char* newdata = new char[datalen+add_length+4096];
	if (data) memcpy(newdata, data, datalen);
	memset(newdata+datalen, 0, 4096+add_length);
	if (data) delete[] data;
	data = newdata; datalen += 4096+add_length;
}
void SENARIO_Writer::WriteData(int var, int flag) {
	int i;
	CheckLen();
	unsigned int uvar = var;
	int len;
	if (uvar < 0x10) { len = 1;
	} else if (uvar < 0x1000) { len = 2;
	} else if (uvar < 0x100000) { len = 3;
	} else if (uvar < 0x10000000) { len = 4;
	} else { len = 5;
	}
	uvar >>= 4;
	for (i=1; i<len; i++) {
		data[point+i]=uvar;
		uvar>>=8;
	}
	data[point] = (flag) ? ( (len<<4)|0x80|(var&0x0f)):
				((len<<4)|(var&0x0f));
	point += len;
	return;
}
void SENARIO_Writer::WriteString(const char* str) {
	char buf[1024];
	int len = strlen(str);
	CheckLen(len+1);
	kconv_rev((const unsigned char*)str, (unsigned char*)(data+point));
	// strcpy(data+point, str);
	point += strlen(data+point)+1;
}
void SENARIO_Writer::WriteChar(char c) {
	CheckLen();
	data[point++] = c;
}
void SENARIO_Writer::WriteInt(int var) {
	CheckLen();
	write_little_endian_int(data+point, var);
	point += 4;
}
int LabelList::ResolvRef(SENARIO_Writer& writer) {
	if (label_point == -1) return -1;
	int writer_point = writer.GetPoint();
	vector<int>::iterator it = label_ref.begin();
	for (; it != label_ref.end(); it++) {
		writer.SetPoint(*it);
		writer.WriteInt(label_point);
	}
	writer.SetPoint(writer_point);
	return 0;
}
void SENARIO_Writer::WriteLabel(const char* label) {
	CheckLen();
	string label_str(label);
	map<string,LabelList>& labelmap = *(map<string,LabelList>*)labelmapptr;
	if (labelmap.find(label_str) == labelmap.end()) {
		labelmap[label_str].Init(label_str);
	}
	labelmap[label_str].AddLabelRef(GetPoint());
	point += 4;
	return;
}
void SENARIO_Writer::SetLabel(const char* label) {
	string label_str(label);
	vector<int>& gluelist = *(vector<int>*)gluelistptr;
	map<string,LabelList>& labelmap = *(map<string,LabelList>*)labelmapptr;
	if (! gluelist.empty()) gluelist.clear();
	if (labelmap.find(label_str) == labelmap.end()) {
		labelmap[label_str].Init(label_str);
	}
	labelmap[label_str].SetLabelPoint(GetPoint());
}
void SENARIO_Writer::WriteGlue(int count) {
	vector<int>& gluelist = *(vector<int>*)gluelistptr;
	gluelist.push_back(point);
	CheckLen(count);
	int i;for (i=0; i<count; i++)
		data[point++] = 0xee;
}
void SENARIO_Writer::DeleteGlue(void) {
	vector<int>& gluelist = *(vector<int>*)gluelistptr;
	CheckLen();
	if (gluelist.empty()) return;
	data[point]=0xee; data[point+1]=0; gluelist.push_back(point);
	sort(gluelist.begin(), gluelist.end());
	/* glueit は glue の最初から現在の point までの一続きのソート済みリストになるはず */
	vector<int>::const_iterator glueit = gluelist.begin();
	int prev_src=0, prev_dest=0;
	for (; glueit != gluelist.end(); glueit++) {
		int glue_pt = *glueit;
		if (glue_pt > point) continue; /* 無効 */
		if ((unsigned char)data[glue_pt] != 0xee) continue; /* glue 以外が書き込まれている */
		if (prev_dest == 0) { /* 始めの glue */
			prev_src = glue_pt;
			while((unsigned char)data[glue_pt] == 0xee) glue_pt++;
			prev_dest = glue_pt;
			continue;
		} else if (prev_dest > glue_pt) { /* コピー済み */
			continue;
		} else { /* コピーする */
			memmove(data+prev_src, data+prev_dest, glue_pt-prev_dest);
			prev_src += glue_pt-prev_dest;
			while((unsigned char)data[glue_pt] == 0xee) glue_pt++;
			prev_dest = glue_pt;
		}
	}
	point = prev_src;
	gluelist.clear();
}
void SENARIO_Writer::ResolvLabel(void) {
	CheckLen();
	map<string,LabelList>& labelmap = *(map<string,LabelList>*)labelmapptr;
	map<string,LabelList>::iterator it = labelmap.begin();
	for(; it != labelmap.end(); it++) {
		if (it->second.ResolvRef(*this)) {
			/* エラー */
			fprintf(stderr,"Cannot resolv label %s\n",(*it).first.c_str());
		}
	}
}

void SENARIO_Writer::Dump(void) {
	int i;
	for (i=0; i<point/16+1; i++) {
		int len = 16;
		if (i*16+len > point) len=point-i*16;
		int j; for (j=0; j<len; j++) {
			printf("%02x ",int(data[i*16+j])&0xff);
		}
		for (;j<16;j++) printf("   ");
		for (j=0; j<len; j++) {
			if (data[i*16+j]>0x20) printf("%c",data[i*16+j]);
			else printf(".");
		}
		printf("\n");
	}
}

static char senario_header[] = {
	0x54, 0x50, 0x43, 0x33, 0x32, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* 00 */
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* 10 */
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* 20 */
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* 30 */
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* 40 */
	0x01, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x6f, /* 50 */
	0xe6, 0x01, 0x00
};
void SENARIO_Writer::DumpFile(const char* file) {
	FILE* f = fopen(file,"wb");
	if (f==0) return;
	ResolvLabel();
	const char* d = GetBasedata();
	int len = point+1; data[point]=0;
	write_little_endian_int(senario_header+0x5f, len);
	fwrite(senario_header, 0x63, 1, f);
	fwrite(d, len, 1, f);
	fclose(f);
}

/* スクリプト書きを助けるためのクラス */
#if 0
class SENARIO_Writer_tmpVar {
public:
	int var_number;
	SENARIO_Writer_tmpVar(int n) { var_number = n; }
	SENARIO_Writer_tmpVar(class SENARIO_Writer_Var&);
};
#endif
inline int op_int(SENARIO_Writer* w,char cmd, int index, int var) {
	w->WriteChar(cmd);
	w->WriteData(index,0);
	w->WriteData(var,0);
}
inline void op_var(SENARIO_Writer* w, char cmd, int index, int var) {
	w->WriteChar(cmd);
	w->WriteData(index,0);
	w->WriteData(var,1);
}

/*************************************************************************
**
** 実際にシナリオを作るときの helper クラス
**
**************************************
** SENARIO_Writer_Var
** まず始めに SetContext() で一時変数として使うことが可能な変数群を
** 定義する。すると、以降 Var と Var/int の計算を行うと適当に
** これらの変数群を使って計算をするスクリプトが作られる。
** 適当に計算をしたらたまに ResetContext() すること。
** 必要な計算が終わったら ReleaseContext すれば終了。
**
**************************************
** SENARIO_Writer_Cond
** まず始めに SetContext() で writer を定義する。
** 以降、Print(instance) を行うことで instance に定義された
** 条件式の内容が出力される。適当に Reset するのと、終了時に
** Release するのは同じ。
** 具体的には
** Print( (Var(10) < 0 || Var(10) > 640) || (Var(11) < 0 || Var(11) > 480));
** のように使うことができる。
** 条件式はSENARIO_Writer_Cond のインスタンスとなるから、メソッドの
** 引数としても利用可能
**************************************
*/




class SENARIO_Writer_VarInstance {
	int var_number;
	int ref_count;
	int delay_set_pt, delay_set_src;
public:
	SENARIO_Writer_VarInstance(int v) {
		var_number = v;
		ref_count = 0;
		delay_set_pt = -1; delay_set_src = -1;
	}
	void ClearDelay(void) {
		delay_set_pt = -1; delay_set_src = -1;
	}
	void DelayWrite(SENARIO_Writer* writer) {
		if (delay_set_pt != -1) {
			/* 遅延書き込みの実行 */
			int pt = writer->GetPoint();
			writer->SetPoint(delay_set_pt);
			writer->WriteChar(0x3b);
			writer->WriteData(var_number,0);
			writer->WriteData(delay_set_src,1);
			writer->SetPoint(pt);
			ClearDelay();
		}
	}
#define swv_swap(x) tmp=x;x=dest.x;dest.x=tmp;
	void Swap(SENARIO_Writer_VarInstance& dest) {
		int tmp;
		swv_swap(var_number);
		swv_swap(delay_set_pt);
		swv_swap(delay_set_src);
	}
#undef swv_swap
	void SetDelay(SENARIO_Writer* writer, SENARIO_Writer_VarInstance& src) {
		/* 遅延書き込みのセット */
		delay_set_pt = writer->GetPoint();
		delay_set_src = src.var_number;
		/* 無効命令を書き込む */
		/* まず、長さを得る */
		writer->WriteChar(0x3b);
		writer->WriteData(var_number);
		writer->WriteData(delay_set_src);
		int len = writer->GetPoint() - delay_set_pt;
		writer->SetPoint(delay_set_pt);
		writer->WriteGlue(len);
	}
	void Ref(void) { ref_count++;}
	void Unref(void) { ref_count--;}
	int IsUsed(void) { return (ref_count > 0);}
	int VarNumber(SENARIO_Writer* w) { DelayWrite(w); return var_number; }
};

typedef class SENARIO_Writer_Var Var;
class SENARIO_Writer_Var {
	SENARIO_Writer_VarInstance* instance;
	int var_number;

	static vector<SENARIO_Writer_VarInstance*> var_list;
	static deque<SENARIO_Writer_VarInstance*> free_var_list;
	static list<SENARIO_Writer_VarInstance*> alloc_var_list;
	static int alloc_var_deal;
	static SENARIO_Writer* writer;
public:
	static void ResetContext(void);
	static void ReleaseContext(void);
	static void SetContext(int* var_array, SENARIO_Writer* w = 0);
	SENARIO_Writer_Var(SENARIO_Writer_VarInstance* ins) : instance(ins) {
		instance->Ref();
		var_number = -1;
	};
	SENARIO_Writer_Var(SENARIO_Writer_Var& var) : instance(var.instance) {
		if (instance) {
			instance->Ref();
			var_number = -1;
		}
		else var_number = var.VarNumber();
	}
	SENARIO_Writer_Var(int var_num) {
		instance = 0;
		var_number = var_num;
	}
	 SENARIO_Writer_Var(void) {
		if (free_var_list.empty()) {
			fprintf(stderr,"Runtime error !! SENARIO_Writer_Var::Alloc() : all resources have already allocated\n");
			instance = 0;
			var_number = 0;
			return;
		}
		/* alloc */
		instance = *(free_var_list.begin());
		var_number = -1;
		alloc_var_deal++;
		free_var_list.pop_front();
		alloc_var_list.push_front(instance);
		instance->ClearDelay();
		instance->Ref();
		var_number = instance->VarNumber(writer);
		return;
	}
	~SENARIO_Writer_Var() {
		if (instance) {
			instance->Unref();
		}
	}
	static void GC(void) {
		list<SENARIO_Writer_VarInstance*>::iterator it;
		/*  遅延書き込みの削除 */
		for (it=alloc_var_list.begin(); it!=alloc_var_list.end();) {
			if ((*it)->IsUsed()) {
				/* 使用中の変数については遅延書き込みを実行 */
				(*it)->DelayWrite(writer);
				it++;
			} else { /* それ以外については free */
				(*it)->ClearDelay();
				list<SENARIO_Writer_VarInstance*>::iterator delete_it = it++;;
				alloc_var_deal--;
				free_var_list.push_front(*delete_it);
				alloc_var_list.erase(delete_it);
			}
		}
		/* writer 側についても無用な部分を削除 */
		writer->DeleteGlue();
	}
	
	Var& operator= (int n) {
		if (instance) instance->ClearDelay();
		op_int(writer, 0x3b, var_number, n); return *this;
	}
	int VarNumber(void) { if (instance) return instance->VarNumber(writer); else return var_number; }
	Var& operator= (Var v) {
		/* A=A はなにもしない */
		if (var_number == v.VarNumber()) return *this;
		/* v は遅延書き込み済みに、自分は遅延書き込み取り消しにしておく */
		if (v.instance) v.instance->DelayWrite(writer);
		if (instance) instance->ClearDelay();
		/* 遅延書き込みを判定 */
		/* 遅延書き込みとは、
		** A=B; A+=C; ... (以降 B は使われない) という状況を
		** 高速化するために、
		** A=B の時点で swap(A,B)を行い、以降 B が使われた
		** 時点で B=A を書き込むこと
		*/
		if (instance != 0 && v.instance != 0) {
			/* 自分と相手を取り替え */
			instance->Swap(*v.instance);
			/* 遅延をセット */
			v.instance->SetDelay(writer, *instance);
			return *this;
		}
		op_var(writer, 0x3b, var_number, v.VarNumber());
		return *this;
	}
	Var& operator+= (int n) { op_int(writer, 0x3c, VarNumber(), n); return *this; }
	Var& operator+= (Var v) { op_var(writer, 0x3c, VarNumber(), v.VarNumber()); return *this; }
	Var& operator-= (int n) { op_int(writer, 0x3d, VarNumber(), n); return *this; }
	Var& operator-= (Var v) { op_var(writer, 0x3d, VarNumber(), v.VarNumber()); return *this; }
	Var& operator*= (int n) { op_int(writer, 0x3e, VarNumber(), n); return *this; }
	Var& operator*= (Var v) { op_var(writer, 0x3e, VarNumber(), v.VarNumber()); return *this; }
	Var& operator/= (int n) { op_int(writer, 0x3f, VarNumber(), n); return *this; }
	Var& operator/= (Var v) { op_var(writer, 0x3f, VarNumber(), v.VarNumber()); return *this; }
	Var& operator%= (int n) { op_int(writer, 0x40, VarNumber(), n); return *this; }
	Var& operator%= (Var v) { op_var(writer, 0x40, VarNumber(), v.VarNumber()); return *this; }
	Var& operator&= (int n) { op_int(writer, 0x41, VarNumber(), n); return *this; }
	Var& operator&= (Var v) { op_var(writer, 0x41, VarNumber(), v.VarNumber()); return *this; }
	Var& operator|= (int n) { op_int(writer, 0x42, VarNumber(), n); return *this; }
	Var& operator|= (Var v) { op_var(writer, 0x42, VarNumber(), v.VarNumber()); return *this; }
	Var& operator^= (int n) { op_int(writer, 0x43, VarNumber(), n); return *this; }
	Var& operator^= (Var v) { op_var(writer, 0x43, VarNumber(), v.VarNumber()); return *this; }
};
	void SENARIO_Writer_Var::ResetContext(void) {
		if (writer == 0) return;
		GC();
		if (alloc_var_deal) {
			fprintf(stderr,"Runtime error !! SENARIO_Writer_var::Reset() is called though some variables are not deallocated.\n");
		}
		/* 解放 */
		for (vector<SENARIO_Writer_VarInstance*>::iterator it = var_list.begin(); it != var_list.end(); it++) delete (*it);
		alloc_var_deal = 0;
		var_list.clear();
		alloc_var_list.clear();
		free_var_list.clear();
	}
	void SENARIO_Writer_Var::ReleaseContext(void) {
		ResetContext();
		writer = 0;
	}
	void SENARIO_Writer_Var::SetContext(int* var_array, SENARIO_Writer* w = 0) { /* 一時的に計算をするためのコンテキストを定義する */
		ResetContext();
		/* 割り当て */
		while(*var_array >= 0) {
			SENARIO_Writer_VarInstance* new_item = new SENARIO_Writer_VarInstance(*var_array++);
			var_list.push_back(new_item);
			free_var_list.push_back(new_item);
		}
		alloc_var_deal = 0;
		if (w != 0) writer = w;
		if (writer == 0) {
			fprintf(stderr,"Runtime error !! SENARIO_Writer_var::SetContect() is called but writer is not defined,\n");
		}
	}

int SENARIO_Writer_Var::alloc_var_deal = 0;
SENARIO_Writer* SENARIO_Writer_Var::writer = 0;
vector<SENARIO_Writer_VarInstance*> SENARIO_Writer_Var::var_list;
deque<SENARIO_Writer_VarInstance*> SENARIO_Writer_Var::free_var_list;
list<SENARIO_Writer_VarInstance*> SENARIO_Writer_Var::alloc_var_list;

#if 0
inline Var operator +(Var a, int b) {
	Var v(SENARIO_Writer_Var::Alloc());
	v = a; v += b; return v;
}
inline Var operator -(Var a, int b) {
	Var v(SENARIO_Writer_Var::Alloc());
	v = a; v -= b; return v;
}
inline Var operator *(Var a, int b) {
	Var v(SENARIO_Writer_Var::Alloc());
	v = a; v *= b; return v;
}
inline Var operator /(Var a, int b) {
	Var v(SENARIO_Writer_Var::Alloc());
	v = a; v /= b; return v;
}
inline Var operator %(Var a, int b) {
	Var v(SENARIO_Writer_Var::Alloc());
	v = a; v %= b; return v;
}
#endif
inline Var operator +(Var a, int b) {
	Var v; v = a; v += b; return v;
}
inline Var operator -(Var a, int b) {
	Var v;
	v = a; v -= b; return v;
}
inline Var operator *(Var a, int b) {
	Var v;
	v = a; v *= b; return v;
}
inline Var operator /(Var a, int b) {
	Var v;
	v = a; v /= b; return v;
}
inline Var operator %(Var a, int b) {
	Var v;
	v = a; v %= b; return v;
}
inline Var operator +(int b, Var a) {
	Var v; v = a; v += b; return v;
}
inline Var operator -(int a, Var b) {
	Var v;
	v = a; v -= b; return v;
}
inline Var operator *(int b, Var a) {
	Var v;
	v = a; v *= b; return v;
}
inline Var operator /(int a, Var b) {
	Var v;
	v = a; v /= b; return v;
}
inline Var operator %(int a, Var b) {
	Var v;
	v = a; v %= b; return v;
}

inline Var operator +(Var a, Var b) {
	Var v; v = a; v += b; return v;
}
inline Var operator -(Var a, Var b) {
	Var v;
	v = a; v -= b; return v;
}
inline Var operator *(Var a, Var b) {
	Var v;
	v = a; v *= b; return v;
}
inline Var operator /(Var a, Var b) {
	Var v;
	v = a; v /= b; return v;
}
inline Var operator %(Var a, Var b) {
	Var v;
	v = a; v %= b; return v;
}

#if 0 /* 論理演算子は cond 系で使えた方がうれしいのでこっちは使わない */
inline Var operator &(Var a, int b) {
	Var v = SENARIO_Writer_Var::Alloc();
	v = a; v &= b; return v;
}
inline Var operator |(Var a, int b) {
	Var v = SENARIO_Writer_Var::Alloc();
	v = a; v |= b; return v;
}
inline SENARIO_Writer_Var operator ^(Var a, int b) {
	Var v = SENARIO_Writer_Var::Alloc();
	v = a; v ^= b; return v;
}
#endif

class SENARIO_Writer_Number {
	int var;
	int flag;
public:
	SENARIO_Writer_Number(int n) {
		var=n; flag=0;
	}
	SENARIO_Writer_Number(Var v) {
		var=v.VarNumber(); flag=1;
	}
	void Write(SENARIO_Writer* w) {
		w->WriteData(var,flag);
	}
};
inline void SENARIO_Writer::WriteData(SENARIO_Writer_Number n) {
	n.Write(this);
}
inline void SENARIO_Writer::WriteString(class SENARIO_Writer_Number n) {
	WriteChar('@');
	n.Write(this);
}
typedef class SENARIO_Writer_Number Para;

class SENARIO_Writer_Cond {
public:
	int index;
	int var1, var2;
	enum {IMM,VAR,COND} var1_sort, var2_sort;
	enum {EQUAL,NOT_EQ,LE,LS,GE,GT,TEST,XOR,AND,OR} op;
	static int temporary_index;
	static vector<SENARIO_Writer_Cond> condlist;
	SENARIO_Writer_Cond() {
		index = -1;
	}
	static void SENARIO_Writer_Cond::Append(SENARIO_Writer_Cond& cond) {
		cond.index = condlist.size();
		condlist.push_back(cond);
	}
	void SENARIO_Writer_Cond::Print(SENARIO_Writer& writer);
	static void Reset(void) {
		condlist.clear();
	}
	static void SetContext(void) {
		condlist.clear();
	}
	void SetVar1(const int a) {
		var1 = a; var1_sort = IMM;
	}
	void SetVar2(const int a) {
		var2 = a; var2_sort = IMM;
	}
	void SetVar1(Var a) {
		var1 = a.VarNumber(); var1_sort = VAR;
	}
	void SetVar2(Var a) {
		var2 = a.VarNumber(); var2_sort = VAR;
	}
	void SetVar1(const SENARIO_Writer_Cond a) {
		var1 = a.index; var1_sort = COND;
	}
	void SetVar2(const SENARIO_Writer_Cond a) {
		var2 = a.index; var2_sort = COND;
	}
};
inline void Print(SENARIO_Writer& w, SENARIO_Writer_Cond cond) {
	cond.Print(w);
}
int SENARIO_Writer_Cond::temporary_index = -1;
vector<SENARIO_Writer_Cond> SENARIO_Writer_Cond::condlist;

inline SENARIO_Writer_Cond operator ==(Var a, int b) {
	SENARIO_Writer_Cond cond;
	cond.SetVar1(a); cond.SetVar2(b);
	cond.op = SENARIO_Writer_Cond::EQUAL; SENARIO_Writer_Cond::Append(cond); return cond;
}
inline SENARIO_Writer_Cond operator ==(Var a, Var b) {
	SENARIO_Writer_Cond cond;
	cond.SetVar1(a); cond.SetVar2(b);
	cond.op = SENARIO_Writer_Cond::EQUAL; SENARIO_Writer_Cond::Append(cond); return cond;
}
inline SENARIO_Writer_Cond operator ==(int b, Var a) {
	SENARIO_Writer_Cond cond;
	cond.SetVar1(a); cond.SetVar2(b);
	cond.op = SENARIO_Writer_Cond::EQUAL; SENARIO_Writer_Cond::Append(cond); return cond;
}
inline SENARIO_Writer_Cond operator !=(Var a, int b) {
	SENARIO_Writer_Cond cond;
	cond.SetVar1(a); cond.SetVar2(b);
	cond.op = SENARIO_Writer_Cond::NOT_EQ; SENARIO_Writer_Cond::Append(cond); return cond;
}
inline SENARIO_Writer_Cond operator !=(Var a, Var b) {
	SENARIO_Writer_Cond cond;
	cond.SetVar1(a); cond.SetVar2(b);
	cond.op = SENARIO_Writer_Cond::NOT_EQ; SENARIO_Writer_Cond::Append(cond); return cond;
}
inline SENARIO_Writer_Cond operator !=(int b, Var a) {
	SENARIO_Writer_Cond cond;
	cond.SetVar1(a); cond.SetVar2(b);
	cond.op = SENARIO_Writer_Cond::NOT_EQ; SENARIO_Writer_Cond::Append(cond); return cond;
}
inline SENARIO_Writer_Cond operator <=(Var a, int b) {
	SENARIO_Writer_Cond cond;
	cond.SetVar1(a); cond.SetVar2(b);
	cond.op = SENARIO_Writer_Cond::LE; SENARIO_Writer_Cond::Append(cond); return cond;
}
inline SENARIO_Writer_Cond operator <=(Var a, Var b) {
	SENARIO_Writer_Cond cond;
	cond.SetVar1(a); cond.SetVar2(b);
	cond.op = SENARIO_Writer_Cond::LE; SENARIO_Writer_Cond::Append(cond); return cond;
}
inline SENARIO_Writer_Cond operator >(int b, Var a) {
	SENARIO_Writer_Cond cond;
	cond.SetVar1(a); cond.SetVar2(b);
	cond.op = SENARIO_Writer_Cond::LE; SENARIO_Writer_Cond::Append(cond); return cond;
}
inline SENARIO_Writer_Cond operator <(Var a, int b) {
	SENARIO_Writer_Cond cond;
	cond.SetVar1(a); cond.SetVar2(b);
	cond.op = SENARIO_Writer_Cond::LS; SENARIO_Writer_Cond::Append(cond); return cond;
}
inline SENARIO_Writer_Cond operator <(Var a, Var b) {
	SENARIO_Writer_Cond cond;
	cond.SetVar1(a); cond.SetVar2(b);
	cond.op = SENARIO_Writer_Cond::LS; SENARIO_Writer_Cond::Append(cond); return cond;
}
inline SENARIO_Writer_Cond operator >=(int b, Var a) {
	SENARIO_Writer_Cond cond;
	cond.SetVar1(a); cond.SetVar2(b);
	cond.op = SENARIO_Writer_Cond::LS; SENARIO_Writer_Cond::Append(cond); return cond;
}
inline SENARIO_Writer_Cond operator >=(Var a, int b) {
	SENARIO_Writer_Cond cond;
	cond.SetVar1(a); cond.SetVar2(b);
	cond.op = SENARIO_Writer_Cond::GE; SENARIO_Writer_Cond::Append(cond); return cond;
}
inline SENARIO_Writer_Cond operator >=(Var a, Var b) {
	SENARIO_Writer_Cond cond;
	cond.SetVar1(a); cond.SetVar2(b);
	cond.op = SENARIO_Writer_Cond::GE; SENARIO_Writer_Cond::Append(cond); return cond;
}
inline SENARIO_Writer_Cond operator <(int b, Var a) {
	SENARIO_Writer_Cond cond;
	cond.SetVar1(a); cond.SetVar2(b);
	cond.op = SENARIO_Writer_Cond::GE; SENARIO_Writer_Cond::Append(cond); return cond;
}
inline SENARIO_Writer_Cond operator >(Var a, int b) {
	SENARIO_Writer_Cond cond;
	cond.SetVar1(a); cond.SetVar2(b);
	cond.op = SENARIO_Writer_Cond::GT; SENARIO_Writer_Cond::Append(cond); return cond;
}
inline SENARIO_Writer_Cond operator >(Var a, Var b) {
	SENARIO_Writer_Cond cond;
	cond.SetVar1(a); cond.SetVar2(b);
	cond.op = SENARIO_Writer_Cond::GT; SENARIO_Writer_Cond::Append(cond); return cond;
}
inline SENARIO_Writer_Cond operator <=(int b, Var a) {
	SENARIO_Writer_Cond cond;
	cond.SetVar1(a); cond.SetVar2(b);
	cond.op = SENARIO_Writer_Cond::GT; SENARIO_Writer_Cond::Append(cond); return cond;
}
#if 0
inline SENARIO_Writer_Cond operator &(Var a, int b) {
	SENARIO_Writer_Cond cond;
	cond.SetVar1(a); cond.SetVar2(b);
	cond.op = SENARIO_Writer_Cond::TEST; SENARIO_Writer_Cond::Append(cond); return cond;
}
inline SENARIO_Writer_Cond operator &(Var a, Var b) {
	SENARIO_Writer_Cond cond;
	cond.SetVar1(a); cond.SetVar2(b);
	cond.op = SENARIO_Writer_Cond::TEST; SENARIO_Writer_Cond::Append(cond); return cond;
}
inline SENARIO_Writer_Cond operator &(int b, Var a) {
	SENARIO_Writer_Cond cond;
	cond.SetVar1(a); cond.SetVar2(b);
	cond.op = SENARIO_Writer_Cond::TEST; SENARIO_Writer_Cond::Append(cond); return cond;
}
inline SENARIO_Writer_Cond operator ^(Var a, int b) {
	SENARIO_Writer_Cond cond;
	cond.SetVar1(a); cond.SetVar2(b);
	cond.op = SENARIO_Writer_Cond::XOR; SENARIO_Writer_Cond::Append(cond); return cond;
}
inline SENARIO_Writer_Cond operator ^(Var a, Var b) {
	SENARIO_Writer_Cond cond;
	cond.SetVar1(a); cond.SetVar2(b);
	cond.op = SENARIO_Writer_Cond::XOR; SENARIO_Writer_Cond::Append(cond); return cond;
}
inline SENARIO_Writer_Cond operator ^(int b, Var a) {
	SENARIO_Writer_Cond cond;
	cond.SetVar1(a); cond.SetVar2(b);
	cond.op = SENARIO_Writer_Cond::XOR; SENARIO_Writer_Cond::Append(cond); return cond;
}
#endif
inline SENARIO_Writer_Cond operator ||(SENARIO_Writer_Cond a, SENARIO_Writer_Cond b) {
	SENARIO_Writer_Cond cond;
	cond.SetVar1(a); cond.SetVar2(b);
	cond.op = SENARIO_Writer_Cond::OR; SENARIO_Writer_Cond::Append(cond); return cond;
}
inline SENARIO_Writer_Cond operator &&(SENARIO_Writer_Cond a, SENARIO_Writer_Cond b) {
	SENARIO_Writer_Cond cond;
	cond.SetVar1(a); cond.SetVar2(b);
	cond.op = SENARIO_Writer_Cond::AND; SENARIO_Writer_Cond::Append(cond); return cond;
}
void SENARIO_Writer_Cond::Print(SENARIO_Writer& writer, void) {
	if (op == AND || op == OR) {
		writer.WriteChar('(');
		condlist[var1].Print(writer);
		if (op == AND) writer.WriteChar(0x27); // and_char
		else writer.WriteChar(0x26); // or_char
		condlist[var2].Print(writer);
		writer.WriteChar(')');
	} else {
		int cmd = 0;
		if (op == EQUAL) cmd = 0x3a;
		else if (op == NOT_EQ) cmd = 0x3b;
		else if (op == LE) cmd = 0x44;
		else if (op == LS) cmd = 0x46;
		else if (op == GE) cmd = 0x45;
		else if (op == GT) cmd = 0x47;
#if 0
		else if (op == TEST) cmd = 0x41;
		else /* if (op == XOR) */ cmd = 0x43;
#endif
		if (var2_sort == VAR) cmd += 0x0e;
		writer.WriteChar('(');
		writer.WriteChar(cmd);
		writer.WriteData(var1);
		writer.WriteData(var2);
		writer.WriteChar(')');
	}
}

class SENARIO_Writer_Cmd : public SENARIO_Writer {
	int text_count;
	int label_count;
public:
	SENARIO_Writer_Cmd(int basep) : SENARIO_Writer(basep) {
		text_count=0;
		label_count = 0;
	}
	void FreeGraphicsBuffer(void) {
		WriteChar(0x0b);
		WriteChar(0x30);
	}
	void LoadPDT(const char* fname, Para pdt) {
		WriteChar(0x0b);
		WriteChar(0x10);
		WriteString(fname);
		WriteData(pdt);
	}
	void DrawEvent(const char* fname, int sel_no) {
		WriteChar(0x0b);
		WriteChar(0x05);
		WriteString(fname);
		WriteData(sel_no);
	}
	void CopyPDT(int src_pdt,Para sx1,Para sy1,Para sx2,Para sy2,int dest_pdt,Para dx,Para dy) {
		WriteChar(0x67);
		WriteChar(0x01);
		WriteData(sx1);
		WriteData(sy1);
		WriteData(sx2);
		WriteData(sy2);
		WriteData(src_pdt);
		WriteData(dx);
		WriteData(dy);
		WriteData(dest_pdt);
		WriteData(0);
	}
	void CopyPDTwithMask(int src_pdt,Para sx1,Para sy1,Para sx2,Para sy2,int dest_pdt,Para dx,Para dy) {
		WriteChar(0x67);
		WriteChar(0x02);
		WriteData(sx1);
		WriteData(sy1);
		WriteData(sx2);
		WriteData(sy2);
		WriteData(src_pdt);
		WriteData(dx);
		WriteData(dy);
		WriteData(dest_pdt);
		WriteData(0);
	}
	void CopyPDTAll(Para src_pdt, Para dest_pdt) {
		WriteChar(0x67);
		WriteChar(0x11);
		WriteData(src_pdt);
		WriteData(dest_pdt);
		WriteData(0);
	}
	void CopyPDTAllwithMask(Para src_pdt, Para dest_pdt) {
		WriteChar(0x67);
		WriteChar(0x12);
		WriteData(src_pdt);
		WriteData(dest_pdt);
		WriteData(0);
	}
	void ClearPDTAll(Para src_pdt, Para c1, Para c2, Para c3) {
		WriteChar(0x68);
		WriteChar(0x01);
		WriteData(src_pdt);
		WriteData(c1);
		WriteData(c2);
		WriteData(c3);
	}
	void DrawTextPDT(int pdt, Para x, Para y, const char* text) {
		int r=255,g=255,b=255;
		WriteChar(0x66);
		WriteChar(0);
		WriteData(x);
		WriteData(y);
		WriteData(pdt);
		WriteData(r);
		WriteData(g);
		WriteData(b);
		WriteChar(0xff);
		WriteString(text);
		WriteChar(0);
	}
	void DrawTextPDT(int pdt, Para x, Para y, Para text) {
		int r=255,g=255,b=255;
		WriteChar(0x66);
		WriteChar(2);
		WriteData(x);
		WriteData(y);
		WriteData(pdt);
		WriteData(r);
		WriteData(g);
		WriteData(b);
		WriteChar(0xfd);
		WriteData(text);
		WriteChar(0);
	}
	//void CopyPDTwithMask(int src_pdt,
	void PlayCD(int track) {
		char buf[20]; sprintf(buf,"%03d",track);
		WriteChar(0x0e);
		WriteChar(0x01);
		WriteString(buf);
	}
	void FadeCD(Para tm) {
		WriteChar(0x0e);
		WriteChar(0x10);
		WriteData(tm);
	}
	void PlayEffec(const char* wave) {
		WriteChar(0x0e);
		WriteChar(0x30);
		WriteString(wave);
	}
	void WriteText(const char* text) {
		WriteChar(0xff);
		WriteInt(text_count++);
		WriteString(text);
	}
	void WriteTextVar(Para  para) {
		WriteChar(0x10);
		WriteChar(1);
		WriteData(para);
	}
	void WriteRet(void) {
		WriteChar(0x02);
		WriteChar(0x01);
	}
	void WriteTStr(Para d) {
		WriteChar(0x10);
		WriteChar(0x03);
		WriteData(d);
	}
	void WriteTVar(Para d) {
		WriteChar(0x10);
		WriteChar(0x01);
		WriteData(d);
	}
	void StrCmd(int cmd, Para d1) {
		WriteChar(0x59);
		WriteChar(cmd);
		WriteData(d1);
	}
	void StrCmd(int cmd, Para d1, Para d2) {
		WriteChar(0x59);
		WriteChar(cmd);
		WriteData(d1);
		WriteData(d2);
	}
	void StrCmdS(int cmd, Para d1, const char* d2) {
		WriteChar(0x59);
		WriteChar(cmd);
		WriteData(d1);
		WriteString(d2);
	}
	void StrCmd(int cmd, Para d1, Para d2, Para d3) {
		WriteChar(0x59);
		WriteChar(cmd);
		WriteData(d1);
		WriteData(d2);
		WriteData(d3);
	}
	void StrCmd(int cmd, Para d1, Para d2, Para d3, Para d4) {
		WriteChar(0x59);
		WriteChar(cmd);
		WriteData(d1);
		WriteData(d2);
		WriteData(d3);
		WriteData(d4);
	}
	void WaitText(void) {
		WriteChar(0x01);
	}
	void GlobalReturn(void) {
		WriteChar(0x20);
		WriteChar(0x02);
	}
	void Cond(SENARIO_Writer_Cond cond, const char* lab) {
		WriteChar(0x15);
		cond.Print(*this);
		WriteLabel(lab);
		return;
	}
	char* Cond(SENARIO_Writer_Cond cond) {
		char* ret_label = new char[20];
		sprintf(ret_label,"lb%d",label_count++);
		WriteChar(0x15);
		cond.Print(*this);
		WriteLabel(ret_label);
		return ret_label;
	}
	void CondBit(int bit_no, int var, const char* lab) {
		WriteChar(0x15);
		WriteChar('(');
		WriteChar(0x36);
		WriteData(bit_no);
		WriteData(var);
		WriteChar(')');
		WriteLabel(lab);
		return;
	}
	char* CondBit(int bit_no, int var) {
		char* ret_label = new char[20];
		sprintf(ret_label,"lb%d",label_count++);
		WriteChar(0x15);
		WriteChar('(');
		WriteChar(0x36);
		WriteData(bit_no);
		WriteData(var);
		WriteChar(')');
		WriteLabel(ret_label);
		return ret_label;
	}
	void GlobalJump(int n, int p) {
		WriteChar(0x16);
		WriteChar(1);
		WriteData(n);
	}
	void GlobalCall(int n, int p) {
		WriteChar(0x16);
		WriteChar(2);
		WriteData(n);
	}
	char* Jump(void) {
		char* ret_label = new char[20];
		sprintf(ret_label,"lb%d",label_count++);
		WriteChar(0x1c);
		WriteLabel(ret_label);
		return ret_label;
	}
	void Jump(const char* lab) {
		WriteChar(0x1c);
		WriteLabel(lab);
		return;
	}
	void Wait(int time) {
		WriteChar(0x19);
		WriteChar(0x01);
		WriteData(time);
	}
	void WaitwithBreak(int time, Var var) {
		WriteChar(0x19);
		WriteChar(0x02);
		WriteData(time);
		WriteData(var.VarNumber());
	}
	void LocalReturn(void) {
		WriteChar(0x20);
		WriteChar(0x01);
	}
	char* LocalCall(void) {
		char* ret_label = new char[20];
		sprintf(ret_label,"lb%d",label_count++);
		WriteChar(0x1b);
		WriteLabel(ret_label);
		return ret_label;
	}
	void LocalCall(const char* lab) {
		WriteChar(0x1b);
		WriteLabel(lab);
	}
};

#ifndef MAIN_EXTERNAL
inline void p(SENARIO_Writer& w, Var v1, Var v2) {
	Print(w,v1==5 || v2==3);
}
int main(void) {
	SENARIO_Writer w(0);
	SENARIO_Writer_Cond::SetContext();
	int varlist[100]; int i;
	for (i=0; i<99; i++) varlist[i]=i+10; varlist[i]=-1;
	SENARIO_Writer_Var::SetContext(varlist,&w);
	Var karen_flag(0);
	Var karen_bit(1);
	Var hina_flag(2);
	Var day(3);

	karen_flag += 5;
	karen_bit |= 0x10;
	Print(w,hina_flag < 10);
	// Print(w,hina_flag < 10 && (karen_bit & 0x10) || (hina_flag > 5));
	Var x1,x2;
	x1 = (day+4)/7+5+(hina_flag+8)*10;
	x2 = (day+4)/8;
	Print(w,x1==5 || x2==3);
	x1.GC();
	x1 = (day+5)/8+10+(hina_flag+7)*11;
	x2=x1;
	x1+=10;
	Print(w,x1==5 || x2==3);
	Print(w,((day+4)/7+5+(hina_flag+8)*10)==5);
	p(w,x1,x2);
	x1.GC();
	w.Dump();
	return 0;
}
#endif
#if 0 /* 0x59 の拡張命令のテスト */
#define w writer
void main(void) {
	SENARIO_Writer_Cmd writer(0);
	SENARIO_Writer_Cond::SetContext();
	int varlist[100]; int i;
	for (i=0; i<99; i++) varlist[i]=i; varlist[i]=-1;
	SENARIO_Writer_Var::SetContext(varlist,&writer);
	w.StrCmdS(0x01,0, "0123456789012345678901234567890"); w.StrCmd(0x10, 1, 0, 5);
		w.WriteText("0x59 - 0x10 ; trim left 5 bytes"); w.WriteRet();
		w.WriteTStr(0); w.WriteRet();
		w.WriteTStr(1); w.WriteRet();
		w.WaitText();
	w.StrCmdS(0x01,0, "0123456789012345678901234567890"); w.StrCmd(0x11, 1, 0, 5);
		w.WriteText("0x59 - 0x11 ; trim right 5 bytes"); w.WriteRet();
		w.WriteTStr(0); w.WriteRet();
		w.WriteTStr(1); w.WriteRet();
		w.WaitText();
	w.StrCmdS(0x01,0, "0123456789012345678901234567890"); w.StrCmd(0x20, 1, 0, 5);
		w.WriteText("0x59 - 0x20 ; trim left 5 words"); w.WriteRet();
		w.WriteTStr(0); w.WriteRet();
		w.WriteTStr(1); w.WriteRet();
		w.WaitText();
	w.StrCmdS(0x01,0, "0123456789012345678901234567890"); w.StrCmd(0x21, 1, 0 , 5);
		w.WriteText("0x59 - 0x21 ; trim right 5 words"); w.WriteRet();
		w.WriteTStr(0); w.WriteRet();
		w.WriteTStr(1); w.WriteRet();
		w.WaitText();
	w.StrCmdS(0x01,0, "０１２３４５６７８９０１２３４５６７８９０"); w.StrCmd(0x20, 1, 0, 5);
		w.WriteText("0x59 - 0x20 ; trim left 5 words"); w.WriteRet();
		w.WriteTStr(0); w.WriteRet();
		w.WriteTStr(1); w.WriteRet();
		w.WaitText();
	w.StrCmdS(0x01,0, "０１２３４５６７８９０１２３４５６７８９０"); w.StrCmd(0x21, 1, 0, 5);
		w.WriteText("0x59 - 0x21 ; trim right 5 words"); w.WriteRet();
		w.WriteTStr(0); w.WriteRet();
		w.WriteTStr(1); w.WriteRet();
		w.WaitText();
	w.StrCmdS(0x01,0, "0123456789012345678901234567890"); w.StrCmd(0x12, 1, 0, 5);
		w.WriteText("0x59 - 0x12 ; get left 5 bytes"); w.WriteRet();
		w.WriteTStr(0); w.WriteRet();
		w.WriteTStr(1); w.WriteRet();
		w.WaitText();
	w.StrCmdS(0x01,0, "0123456789012345678901234567890"); w.StrCmd(0x13, 1, 0, 5);
		w.WriteText("0x59 - 0x13 ; get right 5 bytes"); w.WriteRet();
		w.WriteTStr(0); w.WriteRet();
		w.WriteTStr(1); w.WriteRet();
		w.WaitText();
	w.StrCmdS(0x01,0, "0123456789012345678901234567890"); w.StrCmd(0x14, 1, 0, 5, 7);
		w.WriteText("0x59 - 0x13 ; get mid pos 5, len 7 "); w.WriteRet();
		w.WriteTStr(0); w.WriteRet();
		w.WriteTStr(1); w.WriteRet();
		w.WaitText();
	w.StrCmdS(0x01,0, "0123456789012345678901234567890"); w.StrCmd(0x22, 1, 0, 5);
		w.WriteText("0x59 - 0x22 ; get left 5 words"); w.WriteRet();
		w.WriteTStr(0); w.WriteRet();
		w.WriteTStr(1); w.WriteRet();
		w.WaitText();
	w.StrCmdS(0x01,0, "0123456789012345678901234567890"); w.StrCmd(0x23, 1, 0, 5);
		w.WriteText("0x59 - 0x23; get right 5 words"); w.WriteRet();
		w.WriteTStr(0); w.WriteRet();
		w.WriteTStr(1); w.WriteRet();
		w.WaitText();
	w.StrCmdS(0x01,0, "0123456789012345678901234567890"); w.StrCmd(0x24, 1, 0, 5, 7);
		w.WriteText("0x59 - 0x24; get mid pos 5, len 7"); w.WriteRet();
		w.WriteTStr(0); w.WriteRet();
		w.WriteTStr(1); w.WriteRet();
		w.WaitText();
	w.StrCmdS(0x01,0, "０１２３４５６７８９０１２３４５６７８９０"); w.StrCmd(0x22, 1, 0, 5);
		w.WriteText("0x59 - 0x22 ; get left 5 words"); w.WriteRet();
		w.WriteTStr(0); w.WriteRet();
		w.WriteTStr(1); w.WriteRet();
		w.WaitText();
	w.StrCmdS(0x01,0, "０１２３４５６７８９０１２３４５６７８９０"); w.StrCmd(0x23, 1, 0, 5);
		w.WriteText("0x59 - 0x23 ; get right 5 words"); w.WriteRet();
		w.WriteTStr(0); w.WriteRet();
		w.WriteTStr(1); w.WriteRet();
		w.WaitText();
	w.StrCmdS(0x01,0, "０１２３４５６７８９０１２３４５６７８９０"); w.StrCmd(0x24, 1, 0, 5, 7);
		w.WriteText("0x59 - 0x23 ; get mid pos 5 len 7"); w.WriteRet();
		w.WriteTStr(0); w.WriteRet();
		w.WriteTStr(1); w.WriteRet();
		w.WaitText();
	w.StrCmdS(0x01,0, "0123456789012345678901234567890"); w.StrCmd(0x15, 1, 0);
		w.WriteText("0x59 - 0x15 ; trim left char"); w.WriteRet();
		w.WriteTStr(0); w.WriteRet();
		w.WriteTVar(1); w.WriteRet();
		w.WaitText();
	w.StrCmdS(0x01,0, "0123456789012345678901234567890"); w.StrCmd(0x16, 1, 0);
		w.WriteText("0x59 - 0x16 ; trim right char"); w.WriteRet();
		w.WriteTStr(0); w.WriteRet();
		w.WriteTVar(1); w.WriteRet();
		w.WaitText();
	w.StrCmdS(0x01,0, "0123456789012345678901234567890"); w.StrCmd(0x25, 1, 0);
		w.WriteText("0x59 - 0x25 ; trim left word"); w.WriteRet();
		w.WriteTStr(0); w.WriteRet();
		w.WriteTVar(1); w.WriteRet();
		w.WaitText();
	w.StrCmdS(0x01,0, "0123456789012345678901234567890"); w.StrCmd(0x26, 1, 0);
		w.WriteText("0x59 - 0x26 ; trim right word"); w.WriteRet();
		w.WriteTStr(0); w.WriteRet();
		w.WriteTVar(1); w.WriteRet();
		w.WaitText();
	w.StrCmdS(0x01,0, "０１２３４５６７８９０１２３４５６７８９０"); w.StrCmd(0x25, 1, 0);
		w.WriteText("0x59 - 0x25 ; trim left word"); w.WriteRet();
		w.WriteTStr(0); w.WriteRet();
		w.WriteTVar(1); w.WriteRet();
		w.WaitText();
	w.StrCmdS(0x01,0, "０１２３４５６７８９０１２３４５６７８９０"); w.StrCmd(0x26, 1, 0);
		w.WriteText("0x59 - 0x26 ; trim right word"); w.WriteRet();
		w.WriteTStr(0); w.WriteRet();
		w.WriteTVar(1); w.WriteRet();
		w.WaitText();
	w.StrCmdS(0x01,0, "0123456789012345678901234567890"); w.StrCmd(0x17, 0, 0x8270);
		w.WriteText("0x59 - 0x17 ; insert left char"); w.WriteRet();
		w.WriteTStr(0); w.WriteRet();
		w.WriteTStr(1); w.WriteRet();
		w.WaitText();
	w.StrCmdS(0x01,0, "0123456789012345678901234567890"); w.StrCmd(0x18, 0, 0x8270);
		w.WriteText("0x59 - 0x18 ; insert right char"); w.WriteRet();
		w.WriteTStr(0); w.WriteRet();
		w.WriteTStr(1); w.WriteRet();
		w.WaitText();
	w.StrCmdS(0x01,0, "0123456789012345678901234567890"); w.StrCmd(0x19, 0, 0x8270, 5);
		w.WriteText("0x59 - 0x19 ; insert mid, pos 5"); w.WriteRet();
		w.WriteTStr(0); w.WriteRet();
		w.WriteTStr(1); w.WriteRet();
		w.WaitText();
	w.StrCmdS(0x01,0, "0123456789012345678901234567890"); w.StrCmd(0x27, 0, 0x8270);
		w.WriteText("0x59 - 0x27 ; insert left word"); w.WriteRet();
		w.WriteTStr(0); w.WriteRet();
		w.WriteTStr(1); w.WriteRet();
		w.WaitText();
	w.StrCmdS(0x01,0, "0123456789012345678901234567890"); w.StrCmd(0x28, 0, 0x8270);
		w.WriteText("0x59 - 0x28; insert right word"); w.WriteRet();
		w.WriteTStr(0); w.WriteRet();
		w.WriteTStr(1); w.WriteRet();
		w.WaitText();
	w.StrCmdS(0x01,0, "0123456789012345678901234567890"); w.StrCmd(0x29, 0, 0x8270, 5);
		w.WriteText("0x59 - 0x29; insert mid word , pos 5"); w.WriteRet();
		w.WriteTStr(0); w.WriteRet();
		w.WriteTStr(1); w.WriteRet();
		w.WaitText();
	w.StrCmdS(0x01,0, "０１２３４５６７８９０１２３４５６７８９０"); w.StrCmd(0x27, 0, 0x8270);
		w.WriteText("0x59 - 0x27 ; insert left word"); w.WriteRet();
		w.WriteTStr(0); w.WriteRet();
		w.WriteTStr(1); w.WriteRet();
		w.WaitText();
	w.StrCmdS(0x01,0, "０１２３４５６７８９０１２３４５６７８９０"); w.StrCmd(0x28,  0, 0x8270);
		w.WriteText("0x59 - 0x28 ; insert right word"); w.WriteRet();
		w.WriteTStr(0); w.WriteRet();
		w.WriteTStr(1); w.WriteRet();
		w.WaitText();
	w.StrCmdS(0x01,0, "０１２３４５６７８９０１２３４５６７８９０"); w.StrCmd(0x29, 0, 0x8270, 5);
		w.WriteText("0x59 - 0x29 ; insert mid word, pos 5"); w.WriteRet();
		w.WriteTStr(0); w.WriteRet();
		w.WriteTStr(1); w.WriteRet();
		w.WaitText();
	writer.DumpFile("seen001.txt");
	SENARIO_Writer_Var::ReleaseContext();
}
#endif
