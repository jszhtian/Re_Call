// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "framework.h"
#include "detours.h"
#include <fstream>
#include <iostream>
#include <map>
#include <vector>
#include <sstream>
#include "crc32.h"
#include <iomanip>
#pragma comment(lib, "detours.lib")
using namespace std;
ofstream TXT("Text.txt");
DWORD BaseAddr = (DWORD)GetModuleHandle(NULL);

map<DWORD, wstring> SCRList;
map<DWORD, wstring> REPList;

PVOID g_pOldMultiByteToWideChar = NULL;
typedef int(WINAPI* PfuncMultiByteToWideChar)(
	_In_      UINT   CodePage,
	_In_      DWORD  dwFlags,
	_In_      LPCSTR lpMultiByteStr,
	_In_      int    cbMultiByte,
	_Out_opt_ LPWSTR lpWideCharStr,
	_In_      int    cchWideChar);


char* wtocUTF(LPCTSTR str)
{
	DWORD dwMinSize;
	dwMinSize = WideCharToMultiByte(CP_UTF8, NULL, str, -1, NULL, 0, NULL, FALSE); //计算长度
	char* out = new char[dwMinSize];
	WideCharToMultiByte(CP_UTF8, NULL, str, -1, out, dwMinSize, NULL, FALSE);//转换
	return out;
}

char* wtocGBK(LPCTSTR str)
{
	DWORD dwMinSize;
	dwMinSize = WideCharToMultiByte(936, NULL, str, -1, NULL, 0, NULL, FALSE); //计算长度
	char* out = new char[dwMinSize];
	WideCharToMultiByte(936, NULL, str, -1, out, dwMinSize, NULL, FALSE);//转换
	return out;
}

LPWSTR ctowUTF(char* str)
{
	DWORD dwMinSize;
	dwMinSize = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0); //计算长度
	LPWSTR out = new wchar_t[dwMinSize];
	MultiByteToWideChar(CP_UTF8, 0, str, -1, out, dwMinSize);//转换
	return out;
}

char* wtocSJS(LPCTSTR str)
{
	DWORD dwMinSize;
	dwMinSize = WideCharToMultiByte(932, NULL, str, -1, NULL, 0, NULL, FALSE); //计算长度
	char* out = new char[dwMinSize];
	WideCharToMultiByte(932, NULL, str, -1, out, dwMinSize, NULL, FALSE);//转换
	return out;
}



static void make_console() {
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
	freopen("CONIN$", "r", stdin);
	cout << "Crescendo Ver 1.0.4\n\n" << endl;
}


int WINAPI NewMultiByteToWideChar(UINT cp, DWORD dwFg, LPCSTR lpMBS, int cbMB, LPWSTR lpWCS, int ccWC)
{
	__asm
	{
		pushad
		pushfd
	}
	CRC32 crc;
	int ret = 0;
	ret = ((PfuncMultiByteToWideChar)g_pOldMultiByteToWideChar)(cp, dwFg, lpMBS, cbMB, lpWCS, ccWC);
	if (lpWCS != 0 && (USHORT)* lpWCS > 0x20)//检测所有的有效字符
	{
		wstring wstr = lpWCS;
		DWORD strcrc = crc.Calc((char*)lpWCS, ret);
		auto scitr = REPList.find(strcrc);
		if (scitr != REPList.end())
		{
			wcscpy(lpWCS, scitr->second.c_str());
			ret = scitr->second.length();
			cout << "REPLACE:" << "0x" << hex << scitr->first << wtocGBK(scitr->second.c_str()) << endl;
		}
		else
		{
			auto itr = SCRList.find(strcrc);
			if (itr == SCRList.end())
			{
				TXT << "0x" << hex << setw(8) << setfill('0') << strcrc << "|" << wtocUTF(wstr.c_str()) << endl;
				//cout << "0x" << hex << setw(8) << setfill('0') << strcrc << "|" << wtocUTF(wstr.c_str()) << endl;
				SCRList.insert(pair<DWORD, wstring>(strcrc, wstr));
			}
		}
	}
	/*
	if (lpWCS != 0 && (USHORT)* lpWCS > 0x60)
	{
		ret = ((PfuncMultiByteToWideChar)g_pOldMultiByteToWideChar)(932, dwFg, lpMBS, cbMB, lpWCS, ccWC);
	}
	*/
	__asm
	{
		popfd
		popad
	}
	return ret;
}

void LoadStringMap()
{
	ifstream fin("Crack_Info.ini");
	const int LineMax = 0x3927;
	char str[LineMax];
	if (fin.is_open())
	{
		while (fin.getline(str, LineMax))
		{
			auto wtmp = ctowUTF(str);
			wstring wline = wtmp;
			wstring crcval = wline.substr(2, 8);
			wstring wstr = wline.substr(11);
			DWORD crc = wcstoul(crcval.c_str(), NULL, 16);
			REPList.insert(pair<DWORD, wstring>(crc, wstr));
		}
	}
	else
	{
		MessageBox(0, L"配置文件读取失败", L"ERROR", MB_OK);
	}
}

void ErrorFileNameFix()
{
	g_pOldMultiByteToWideChar = DetourFindFunction("Kernel32.dll", "MultiByteToWideChar");
	DetourTransactionBegin();
	DetourAttach(&g_pOldMultiByteToWideChar, NewMultiByteToWideChar);
	if (DetourTransactionCommit() != NO_ERROR)
	{
		MessageBox(0, L"Hook Error.", L"ERROR", MB_OK);
	}
}

void memcopy(void* dest, void* src, size_t size)
{
	DWORD oldProtect;
	VirtualProtect(dest, size, PAGE_EXECUTE_READWRITE, &oldProtect);
	memcpy(dest, src, size);
}


void MyCharSet()
{
	BYTE Patch1[] = { 0xC6 };
	BYTE Patch2[] = { 0x45 };
	BYTE Patch3[] = { 0xD7 };
	BYTE Patch4[] = { 0x86 };
	BYTE Patch5[] = { 0x90 };
	BYTE Patch6[] = { 0x90 };

	BYTE Patch7[] = { 0x86 };

	int CharSet1 = BaseAddr + 0xAE055;
	int CharSet2 = BaseAddr + 0xAE056;
	int CharSet3 = BaseAddr + 0xAE057;
	int CharSet4 = BaseAddr + 0xAE058;
	int CharSet5 = BaseAddr + 0xAE059;
	int CharSet6 = BaseAddr + 0xAE05A;
	int FontChange = BaseAddr + 0x124D39;

	memcopy((void*)CharSet1, Patch1, sizeof(Patch1));
	memcopy((void*)CharSet2, Patch2, sizeof(Patch2));
	memcopy((void*)CharSet3, Patch3, sizeof(Patch3));
	memcopy((void*)CharSet4, Patch4, sizeof(Patch4));
	memcopy((void*)CharSet5, Patch5, sizeof(Patch5));
	memcopy((void*)CharSet6, Patch6, sizeof(Patch6));

	memcopy((void*)FontChange, Patch7, sizeof(Patch7));

}

LPWSTR ctowJIS(char* str)
{
	DWORD dwMinSize;
	dwMinSize = MultiByteToWideChar(932, 0, str, -1, NULL, 0); //计算长度
	LPWSTR out = new wchar_t[dwMinSize];
	MultiByteToWideChar(932, 0, str, -1, out, dwMinSize);//转换
	return out;
}

wchar_t szTitle[] = L"提示";
typedef int (WINAPI* fnMessageboxA)(
	_In_opt_ HWND    hWnd,
	_In_opt_ LPCSTR lpText,
	_In_opt_ LPCSTR lpCaption,
	_In_     UINT    uType
	);
fnMessageboxA MessageBoxAOLD;
int WINAPI MessageBoxAEx(_In_opt_ HWND    hWnd,
	_In_opt_ LPCSTR lpText,
	_In_opt_ LPCSTR lpCaption,
	_In_     UINT    uType)
{
	char* Pstr = wtocGBK(ctowJIS((char*)lpText));
	return MessageBoxAOLD(hWnd, Pstr, wtocGBK(szTitle), uType);
}

void Init()
{
	
	MessageBoxAOLD = (fnMessageboxA)GetProcAddress(GetModuleHandle(L"User32.dll"), "MessageBoxA");
	DetourTransactionBegin();
	DetourAttach((void**)& MessageBoxAOLD, MessageBoxAEx);
	DetourTransactionCommit();
	
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
		//make_console();
		MyCharSet();
		LoadStringMap();
		ErrorFileNameFix();
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

