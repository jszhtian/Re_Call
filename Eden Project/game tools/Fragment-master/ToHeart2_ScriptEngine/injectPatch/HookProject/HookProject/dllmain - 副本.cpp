// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"
#include "detours.h"
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <string>
#include "crc32.h"
#include <iomanip>
#include <mutex>
#include <list>
using namespace std;
//ofstream flog("out.log");
struct block {
	wchar_t Ori;
	wchar_t Map;
};
typedef int(__cdecl *Func_DrawTextEx)(void *dest, int draw_mode2, int sx, int sy, int w, int h, int pixh_w, int pich_h, int *px2, int *py2, RECT *clip,
int font, TCHAR *str, int color, int text_cnt, int step_cnt, int r, int g, int b, int alph, int kage, int cnt_flag, TCHAR *normal_str, int kaigyou_musi);
PVOID p_DrawTextEx=NULL;
unordered_map<unsigned __int32, wstring> TranInfo;
unordered_map<wchar_t, wchar_t> Mapinfo;
list<string> rebuildlist;
std::mutex mut;
wchar_t CHK1[] = L"\\";
wchar_t CHK2[] = L"k";
wchar_t CHK3[] = L"n";
void ProcSubStr(unordered_map<unsigned __int32, wstring>& info,string str)
{
	mut.lock();
	CRC32 crc32;
	crc32.Init();
	int Strlen = str.length();
	unsigned __int32 strcrc = crc32.Calc(str.c_str(), Strlen);
	if (strcrc== 0xc8932c56)
	{
		char testprefix[] = { 0x81,0x41,0x81,0x41 };
		string tpm = string(testprefix);
		tpm += str;
		rebuildlist.push_back(tpm);
	}
	if (strcrc == 0x81a88bbe)
	{
		char testprefix[] = { 0x81,0x41,0x81,0x41 };
		string tpm = string(testprefix);
		tpm += str;
		rebuildlist.push_back(tpm);
	}
	if (info.find(strcrc) != TranInfo.end())
	{
		//rebuildlist.push_back(info.at(strcrc));
		char tmpstr[0x500];
		memset(tmpstr, 0, 0x500);
		int idx = 0;
		for (auto itr= info.at(strcrc).begin();itr!= info.at(strcrc).end();++itr)
		{
			wchar_t tag = *itr;
			auto res = Mapinfo.find(tag);
			if (res !=Mapinfo.end())
			{
				//std::cout <<hex<<(int)res->second << endl;
				wchar_t getwch = res->second;
				memcpy(&tmpstr[idx], &getwch, 2);
				idx += 2;
			}
			if (memcmp(&tag, CHK1, 2)== 0)
			{
				memcpy(&tmpstr[idx], CHK1, 1);
				idx += 1;
			}
			if (memcmp(&tag, CHK2, 2) == 0)
			{
				memcpy(&tmpstr[idx], CHK2, 1);
				idx += 1;
			}
			if (memcmp(&tag, CHK3, 2) == 0)
			{
				memcpy(&tmpstr[idx], CHK3, 1);
				idx += 1;
			}
		}

		rebuildlist.push_back(tmpstr);
	}
	else
	{
		rebuildlist.push_back(str);
	}
	mut.unlock();
}

int __cdecl HookDrawTextEx(void *a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int *a9, int *a10, RECT *a11,
int a12, TCHAR *a13, int a14, int a15, int a16, int a17, int a18, int a19, int a20, int a21, int a22, TCHAR *a23, int a24)
{
	rebuildlist.clear();
	string strStm((char*)a13);
	string token;
	size_t pos1, pos2;
	pos1 = 0;
	const char* spl=R"(\s)";
	pos2 = strStm.find(spl);

	while(string::npos!=pos2){
		ProcSubStr(TranInfo, strStm.substr(pos1, pos2- pos1));
		pos1 = pos2 + strlen(spl);
		pos2 = strStm.find(spl, pos1);
	}
	ProcSubStr(TranInfo, strStm.substr(pos1));
	string rebuild_Str;
	if (!rebuildlist.empty())
	{
		for (unsigned int i=0;i<rebuildlist.size();i++)
		{
			rebuild_Str += rebuildlist.front();
			rebuildlist.pop_front();
			if (i!= rebuildlist.size()-1)
			{
				rebuild_Str += "\\s";
			}
		}
	}
	return ((Func_DrawTextEx)p_DrawTextEx)(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, (TCHAR*)rebuild_Str.c_str(), a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24);
}
 


void init()
{
	DWORD badd = (DWORD)GetModuleHandle(NULL);
	p_DrawTextEx = (PVOID)(badd + 0xAF290);
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&p_DrawTextEx, HookDrawTextEx);
	if (DetourTransactionCommit() != NO_ERROR)
	{
		MessageBoxW(NULL, L"Hook目标函数失败", L"严重错误", MB_OK | MB_ICONWARNING);
	}

	
}
void loadList()
{
	TranInfo.insert(pair<unsigned __int32, wstring>(0xe458ec39, wstring(L"　梦。\\k\\n")));
	TranInfo.insert(pair<unsigned __int32, wstring>(0x6f39a5f6, wstring(L"　正在做一个梦。\\k\\n　在昏暗的房间中独自一人抱着膝盖颤抖的梦。")));
	TranInfo.insert(pair<unsigned __int32, wstring>(0x2a2dee35, wstring(L"　我大汗淋漓的坐在冰冷的地板上，正拼命地阻止着快要从自己体内爬出来的“某物”。\\k\\n　就是这样的梦。")));
	TranInfo.insert(pair<unsigned __int32, wstring>(0xea6c7489, wstring(L"　呼、呼……。\\k\\n　呼吸如野兽般狂乱。\\k\\n　冷汗从肌肤上滑落。")));
	TranInfo.insert(pair<unsigned __int32, wstring>(0x4affa612, wstring(L"　已经持续了好几个小时了。\\k\\n　这是寂静而又孤独，伴随着难以忍受的苦痛的战斗。")));
	TranInfo.insert(pair<unsigned __int32, wstring>(0x6d01bab0, wstring(L"　有谁……。\\k\\n　有谁来救救我……。\\k\\n　嘴唇颤抖着动了动，却没有发出声来。")));
	TranInfo.insert(pair<unsigned __int32, wstring>(0xe89c8f25, wstring(L"　有谁……。")));
	TranInfo.insert(pair<unsigned __int32, wstring>(0x42a9db76, wstring(L"１．　老实地说出来。")));
	TranInfo.insert(pair<unsigned __int32, wstring>(0xa594e07a, wstring(L"２．　是出现了千鹤小姐的梦……这样开玩笑。")));
	TranInfo.insert(pair<unsigned __int32, wstring>(0x4a7fb46b, wstring(L"３．　笑着敷衍她。")));
	TranInfo.insert(pair<unsigned __int32, wstring>(0xc8932c56, wstring(L"「那个，算了吧。\\k反正也不是什么大不了的内容，哈哈哈」\\k\\n　我笑着敷衍道。\\k\\n")));
	TranInfo.insert(pair<unsigned __int32, wstring>(0x81a88bbe, wstring(L"「是这样啊……」\\k\\n　千鹤小姐显得有点失望。\\k\\n　总感觉很对不起她。\\k\\n　不过……\\k那个梦的内容果然还是很难说出口。\\k\\n　如果是更加抽象的梦的话也就算了，但是这个梦的内容明显像是欲求不满啊。\\k\\n　内容如同显示了我本性一般。")));
	fstream file1;
	file1.open("map.dat", ios::binary | ios::in);
	file1.seekg(0, ios::end);
	DWORD szOfFile = file1.tellg();
	file1.seekg(0, ios::beg);
	int totload = szOfFile / sizeof(block);
	auto buffer = new block[totload];
	file1.read((char*)buffer, szOfFile);
	for (unsigned int i=0;i< szOfFile / sizeof(block);i++)
	{
		Mapinfo.insert(pair<wchar_t, wchar_t>(buffer[i].Map, buffer[i].Ori));
	}
	delete[] buffer;
	std::cout << "Load NUM:"<< totload << std::endl;
}

static void make_console() {
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
	freopen("CONIN$", "r", stdin);
	std::cout << "Open Console Success!" << std::endl;
}
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
		make_console();
		loadList();
		init();
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

