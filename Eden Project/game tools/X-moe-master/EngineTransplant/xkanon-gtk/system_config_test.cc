#include"system.h"
#include<string>
#include<map>
#include<vector>
#include<stdlib.h>
#include<stdio.h>

#define SETDEAL 10 /* SetRand で適当にセットする config の数 */
struct Conf2 {
	map<string,string> config_string;
	map<string,vector<int> > config_intlist;
	map<string,int> config_is_orig;
	AyuSysConfig& conf;

	Conf2(AyuSysConfig& c) : conf(c) {};
	Conf2(Conf2& c) : config_string(c.config_string),
		config_intlist(c.config_intlist),
		config_is_orig(c.config_is_orig),
		conf(c.conf) {};
	void Init(void);
	void InitInt(const char*,int);
	void InitStr(const char*);
	void GetIntlist(const char*,int,int*);
	void Check(void);
	string MakeStr(void); vector<int> MakeVec(int size);
	void SetRand(void);
	void SetRandOrig(void);
};
string Conf2::MakeStr(void) {
	int size = (AyuSys::Rand()/51961)%250;
	char* s = new char[size+1];
	int i; for(i=0;i<size;i++)s[i]=(AyuSys::Rand()()/5627)%80+33;
	s[i]=0;
	string r(s); delete[] s;
	return r;
}
vector<int> Conf2::MakeVec(int size) {
	vector<int> r; int i;
	for (i=0;i<size;i++) {
		r.push_back(AyuSys::Rand()/15437);
	}
	return r;
}
void Conf2::SetRand(void) {
	int i;
	for (i=0;i<SETDEAL;i++) {
		map<string,int>::iterator it = config_is_orig.begin();
		int i=(AyuSys::Rand()/391687)%config_is_orig.size();
		for (;i>0;i--) it++;
		if (it == config_is_orig.end()) {
			fprintf(stderr,"error in SetRand();\n");
			exit(-1);
		}
		string name = it->first;
		if (config_string.find(name) != config_string.end()) {
			string s = MakeStr();
			conf.SetParaStr(name.c_str(),s.c_str());
			config_string[name]=s;
			config_is_orig[name]=0;
		} else if (config_intlist.find(name) != config_intlist.end()) {
			vector<int> v = MakeVec(config_intlist.find(name)->second.size());
			switch(v.size()) {
			case 1:conf.SetParam(name.c_str(),1,v[0]);break;
			case 2:conf.SetParam(name.c_str(),2,v[0],v[1]);break;
			case 3:conf.SetParam(name.c_str(),3,v[0],v[1],v[2]);break;
			case 4:conf.SetParam(name.c_str(),4,v[0],v[1],v[2],v[3]);break;
			}
			config_intlist[name.c_str()]=v;
			config_is_orig[name]=0;
		} else {
			fprintf(stderr,"error in SetRand();\n");
			exit(-1);
		}
	}
}
void Conf2::SetRandOrig(void) {
	int i;
	for (i=0;i<SETDEAL;i++) {
		map<string,int>::iterator it = config_is_orig.begin();
		int i=(AyuSys::Rand()/391687)%config_is_orig.size();
		for (;i>0;i--) it++;
		if (it == config_is_orig.end()) {
			fprintf(stderr,"error in SetRand();\n");
			exit(-1);
		}
		string name = it->first;
		if (config_string.find(name) != config_string.end()) {
			string s = MakeStr();
			conf.SetOrigParaStr(name.c_str(),s.c_str());
			if (config_is_orig[name]==1)config_string[name]=s;
		} else if (config_intlist.find(name) != config_intlist.end()) {
			vector<int> v = MakeVec(config_intlist.find(name)->second.size());
			switch(v.size()) {
			case 1:conf.SetOrigParam(name.c_str(),1,v[0]);break;
			case 2:conf.SetOrigParam(name.c_str(),2,v[0],v[1]);break;
			case 3:conf.SetOrigParam(name.c_str(),3,v[0],v[1],v[2]);break;
			case 4:conf.SetOrigParam(name.c_str(),4,v[0],v[1],v[2],v[3]);break;
			}
			if(config_is_orig[name]==1)config_intlist[name.c_str()]=v;
		} else {
			fprintf(stderr,"error in SetRand();\n");
			exit(-1);
		}
	}
}
void Conf2::Check(void) {
	map<string,string>::iterator is=config_string.begin();
	for (;is!=config_string.end();is++) {
		if (strcmp(is->second.c_str(), conf.GetParaStr(is->first.c_str())) != 0) {
			fprintf(stderr, "check failed; str %s; %s != %1\n",is->first.c_str(), is->second.c_str(), conf.GetParaStr(is->first.c_str()));
			exit(-1);
		}
	}
	map<string,vector<int> >::iterator iv=config_intlist.begin();
	for (;iv!=config_intlist.end();iv++) {
		int d = iv->second.size(); int i; int l[10];
		switch(d) {
			case 1: conf.GetParam(iv->first.c_str(),1,&l[0]);break;
			case 2: conf.GetParam(iv->first.c_str(),2,&l[0],&l[1]);break;
			case 3: conf.GetParam(iv->first.c_str(),3,&l[0],&l[1],&l[2]);break;
			case 4: conf.GetParam(iv->first.c_str(),4,&l[0],&l[1],&l[2],&l[3]);break;
		}
		for (i=0;i<d;i++) {
			if (l[i]!=iv->second[i]){
				fprintf(stderr, "check failed; str %s ; i %d, l %d, iv %d\n",iv->first.c_str(),i,l[i],iv->second[i]);
				exit(-1);
			}
		}
	}
}
void Conf2::InitStr(const char* str) {
	config_string[str]=string(conf.GetParaStr(str));
	config_is_orig[str]=1;
}
void Conf2::InitInt(const char* str, int deal) {
	vector<int> vec; int vars[10];
	switch(deal) {
	case 1: conf.GetParam(str,deal,&vars[0]); break;
	case 2: conf.GetParam(str,deal,&vars[0],&vars[1]); break;
	case 3: conf.GetParam(str,deal,&vars[0],&vars[1],&vars[2]); break;
	case 4: conf.GetParam(str,deal,&vars[0],&vars[1],&vars[2],&vars[3]); break;
	default: fprintf(stderr, "Error in InitInt; str %s\n",str);exit(-1);
	}
	int i;for (i=0;i<deal;i++) vec.push_back(vars[i]);
	config_intlist[str]=vec;
	config_is_orig[str]=1;
}
void Conf2::Init(void) {
	InitStr("#WAKUPDT");
	InitStr("#REGNAME");
	InitStr("#CAPTION");
	InitStr("#SAVENAME");
	InitStr("#SAVETITLE");
	InitStr("#SAVENOTITLE");
	InitStr("#CGM_FILE");
	InitInt("#COM2_TITLE", 1);
	InitInt("#COM2_TITLE_COLOR", 1);
	InitInt("#COM2_TITLE_INDENT", 1);
	InitInt("#SAVEFILETIME", 1);
	InitInt("#SEEN_START", 1);
	InitInt("#SEEN_MENU", 1);
	InitInt("#FADE_TIME", 1);
	InitInt("#NVL_SYSTEM",1);
	InitInt("#WINDOW_ATTR", 3);
	InitInt("#WINDOW_ATTR_AREA", 4);
	InitInt("#WINDOW_ATTR_TYPE", 1);
	InitInt("#WINDOW_MSG_POS", 2);
	InitInt("#WINDOW_COM_POS", 2);
	InitInt("#WINDOW_GRP_POS", 2);
	InitInt("#WINDOW_SUB_POS", 2);
	InitInt("#WINDOW_SYS_POS", 2);
	InitInt("#WINDOW_WAKU_TYPE", 1);
	InitInt("#RETN_CONT", 1);
	InitInt("#RETN_SPEED",1);
	InitInt("#RETN_XSIZE", 1);
	InitInt("#RETN_YSIZE", 1);
	InitInt("#FONT_SIZE", 1);
	InitInt("#FONT_WEIGHT", 1);
	InitInt("#MSG_MOJI_SIZE", 2);
	InitInt("#MESSAGE_SIZE", 2);
	InitInt("#GRP_DC_TIMES", 1);
}

AyuSysConfig conf;
int main(int argc, char* argv[]) {
	printf("start config test\n");
	/* 乱数初期化 */
	FILE* f = fopen("/dev/random","rb");
	if (f != 0) {
		int seed;
		fread(&seed,sizeof(int),1,f);
		srand(seed);
		fclose(f);
	}
	Conf2 conf2(conf); Conf2 conf4(conf);conf4.Init();
	printf("Conf2 Init end (SetPara* func)\n");
{FILE* f=fopen("d0","w");conf.Dump(f);fclose(f);}
	conf2.Init();
	conf2.Check();
	/* conf.Dump(stdout); */
	printf("Check() end (GetPara* func)\n");
	conf2.SetRand();
	conf2.Check();
	printf("Check() end (SetPara* func - 2)\n");
//	conf2.SetRandOrig();
	conf2.Check();
	printf("Check() end (SetParaOrig* func)\n");
	conf2.SetRand();
	conf2.Check();

	/* 新しいテスト：diff / patch */
	Conf2 conf3(conf);
	conf3.Init();
	conf3.SetRand();
	if (! conf.IsDiff()){fprintf(stderr,"dirty2???\n");exit(-1);}
	conf3.SetRand();
	if (! conf.IsDiff()){fprintf(stderr,"dirty3???\n");exit(-1);}
	printf("difflen %d\n",conf.DiffLen());
	conf.ClearDiff();
	if (conf.IsDiff()){fprintf(stderr,"dirty4???\n");exit(-1);}
	printf("difflen %d (maybe 0)\n",conf.DiffLen());
	Conf2 conf3_old(conf3); /* 変更前のconf */
{FILE* f=fopen("d1","w");conf.Dump(f);fclose(f);}
	conf3.SetRand();
{FILE* f=fopen("d2","w");conf.Dump(f);fclose(f);}
	int len = conf.DiffLen(); char* d = new char[len+1];
	printf("len %d / %d (must be equal)\n",conf.Diff(d)-d,len);

	/* 変更後の conf にパッチを当てる */
	Conf2 conf3_new(conf3);
	conf.PatchOld(d);
{FILE* f=fopen("d3","w");conf.Dump(f);fclose(f);}
	conf3_old.Check();
	printf("Check() after PatchOld() end\n");
	/* 変更前の conf にパッチを当てる */
	conf.PatchNew(d);
{FILE* f=fopen("d4","w");conf.Dump(f);fclose(f);}
	conf3_new.Check();
	printf("Check() after PatchNew() end\n");

	/* Original 系 */
	conf3.SetRand();
	int l2=conf.DiffLen(); char* d2=new char[l2+1];conf.Diff(d2);
	len = conf.DiffOriginalLen(); d = new char[len+1];
	printf("len %d / %d(must be equal)\n",conf.DiffOriginal(d)-d,len);
	conf.SetOriginal();
{FILE* f=fopen("d5","w");conf.Dump(f);fclose(f);}
	conf4.Check();
	printf("Check() after SetOriginal() end\n");
	printf("diff len is %d; must be 0\n",conf.DiffOriginalLen());
	conf.PatchOriginal(d);
	conf3.Check();
	printf("Check() after PatchOriginal()\n");
	printf("diff len is %d; must be %d\n",conf.DiffOriginalLen(),len);
	int l3=conf.DiffLen(); char* d3=new char[l3+1];conf.Diff(d3);
	if (l3 != l2 || memcmp(d3,d2,l2) != 0) {
		printf("error!\n");
	}
}
void TrackName::AddCDROM(char* a,int b) {}
void TrackName::AddWave(char* a, char* b) {}
