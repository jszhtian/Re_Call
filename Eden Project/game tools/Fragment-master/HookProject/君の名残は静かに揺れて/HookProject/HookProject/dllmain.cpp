// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"
#include "detours.h"
#pragma comment(lib, "detours.lib")
using namespace std;
unordered_map<int, string> textlist;
typedef HFONT(WINAPI *fnCreateFontA)(
	int nHeight, // logical height of font height
	int nWidth, // logical average character width
	int nEscapement, // angle of escapement
	int nOrientation, // base-line orientation angle
	int fnWeight, // font weight
	DWORD fdwItalic, // italic attribute flag
	DWORD fdwUnderline, // underline attribute flag
	DWORD fdwStrikeOut, // strikeout attribute flag
	DWORD fdwCharSet, // character set identifier
	DWORD fdwOutputPrecision, // output precision
	DWORD fdwClipPrecision, // clipping precision
	DWORD fdwQuality, // output quality
	DWORD fdwPitchAndFamily, // pitch and family
	LPCSTR lpszFace // pointer to typeface name string
	);
typedef int (WINAPI *fnMessageboxA)(
	_In_opt_ HWND    hWnd,
	_In_opt_ LPCSTR lpText,
	_In_opt_ LPCSTR lpCaption,
	_In_     UINT    uType
	);
fnCreateFontA CreateFontAOLD;
fnMessageboxA MessageBoxAOLD;
//bool TitleChanged = false;
//wchar_t szTitle[] = L"ナツイロココロログ～Happy Summer～TEST20170510";
LPWSTR ctowJIS(char* str)
{
	DWORD dwMinSize;
	dwMinSize = MultiByteToWideChar(932, 0, str, -1, NULL, 0); //计算长度
	LPWSTR out = new wchar_t[dwMinSize];
	MultiByteToWideChar(932, 0, str, -1, out, dwMinSize);//转换
	return out;
}

char* wtocGBK(LPCTSTR str)
{
	DWORD dwMinSize;
	dwMinSize = WideCharToMultiByte(936, NULL, str, -1, NULL, 0, NULL, FALSE); //计算长度
	char *out = new char[dwMinSize];
	WideCharToMultiByte(936, NULL, str, -1, out, dwMinSize, NULL, FALSE);//转换
	return out;
}

static void make_console() {
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
	freopen("CONIN$", "r", stdin);
	cout << "Open Console Success!" << endl;
}
char* (*GetTextByIndexOld)(int v1, int v2,int v3,int v4);

char* RepString(int v1, int v2, int v3, int v4)
{
	_asm
	{
		pushad
	}
	char* xf;
	xf = GetTextByIndexOld(v1, v2, v3, v4);
	if ((v3 & 0x10000000) || v4 == 0xFFFFFFF)
	{
		goto exiting;
	}
	int index = *(int*)xf;
	if (index < textlist.size())
	{
		strcpy(xf+sizeof(index), textlist[index].c_str());
		cout << "Replace: index=0x" << index << " Str=" << xf+ sizeof(index) << endl;
	}
exiting:
	_asm
	{
		popad
	}
	return xf;
}

bool(*is_SJIS)(char* buf);
bool GBKCheck(char* p) {
	_asm
	{
		pushad
	}
	bool ret = is_SJIS(p);

	if (!ret)
	{
		if (*p != 0 && *(p + 1) != 0)
		{
			unsigned char c1, c2;
			c1 = *p;
			c2 = *(p + 1);
			if (c1 >= 0x81 && c1 <= 0xfe && c2 >= 0x40 && c2 <= 0xfe&&c2!=0x7F)
				ret = true;
		}

	}

	
	_asm
	{
		popad
	}
	return ret;
}
bool loadText()
{
	ifstream ftxt("sens.txt");
	if (!ftxt.is_open())
	{
		return false;
	}
	int idx = 0;
	while (!ftxt.eof())
	{
		char tmp[0x1000];		
		ftxt.getline(tmp,0x1000);
		string text(tmp);
		textlist.insert(pair<int, string>(idx, text));
		idx++;
	}
	ftxt.close();
	cout << "Load Texts:0x" <<hex<<idx<<endl;
	return true;
}
HFONT WINAPI CreateFontAEx(int nHeight, int nWidth, int nEscapement, int nOrientation, int fnWeight, DWORD fdwItalic, DWORD fdwUnderline, DWORD fdwStrikeOut, DWORD fdwCharSet, DWORD fdwOutputPrecision, DWORD fdwClipPrecision, DWORD fdwQuality, DWORD fdwPitchAndFamily, LPCSTR lpszFace)
{
	if (fdwCharSet == 0x80)
	{
		fdwCharSet = 0;
		fnWeight = FW_SEMIBOLD;
		return CreateFontAOLD(nHeight, nWidth, nEscapement, nOrientation, fnWeight, fdwItalic, fdwUnderline, fdwStrikeOut, fdwCharSet, fdwOutputPrecision, fdwClipPrecision, fdwQuality, fdwPitchAndFamily, "黑体");
	}

	return CreateFontAOLD(nHeight, nWidth, nEscapement, nOrientation, fnWeight, fdwItalic, fdwUnderline, fdwStrikeOut, fdwCharSet, fdwOutputPrecision, fdwClipPrecision, fdwQuality, fdwPitchAndFamily, lpszFace);
}
/*
int WINAPI MessageBoxAEx(_In_opt_ HWND    hWnd,
_In_opt_ LPCSTR lpText,
_In_opt_ LPCSTR lpCaption,
_In_     UINT    uType)
{
return MessageBoxAOLD(hWnd, "确定要结束吗?", wtocGBK(szTitle), uType);
}
*/

void BeginDetour()
{
	DWORD BaseAddr = (DWORD)GetModuleHandle(NULL);
	*(DWORD*)&GetTextByIndexOld = BaseAddr+0x54d0;
	*(DWORD*)&is_SJIS = BaseAddr + 0x21800;
	//cout << "HookAddress:0x" << hex << (BaseAddr + 0x3E8B0) << endl;
	CreateFontAOLD = (fnCreateFontA)GetProcAddress(GetModuleHandle(L"gdi32.dll"), "CreateFontA");
	DetourTransactionBegin();
	//MessageBoxAOLD = (fnMessageboxA)GetProcAddress(GetModuleHandle(L"User32.dll"), "MessageBoxA");
	DetourAttach((void**)&GetTextByIndexOld, RepString);
	DetourAttach((void**)&is_SJIS, GBKCheck);
	DetourAttach((void**)&CreateFontAOLD, CreateFontAEx);
	//DetourAttach((void**)&MessageBoxAOLD, MessageBoxAEx);
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
		make_console();
		if (!loadText())
		{
			MessageBox(NULL, L"Load Translation file fail", L"Error", MB_OK);
			return false;
		}
		BeginDetour();
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

extern "C" __declspec(dllexport) void dummy(void) {
	return;
}

