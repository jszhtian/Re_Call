// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"
vector<wstring>rawlist;
vector<wstring>newlist;

map<DWORD, wstring> SCRList;
map<DWORD, wstring> REPList;

bool IsJis(wchar_t* Text)
{
	for (int i = 0; i < wcslen(Text); i++)
	{

		if ((unsigned int)Text[i] < 0x20)
			return false;
		if ((unsigned int)Text[i] > 0x80)
			return true;
	}
	return false;
}

int WINAPI HookMultiByteToWideChar(
	_In_      UINT   CodePage,
	_In_      DWORD  dwFlags,
	_In_      LPCSTR lpMultiByteStr,
	_In_      int    cbMultiByte,
	_Out_opt_ LPWSTR lpWideCharStr,
	_In_      int    cchWideChar
)
{
	int rey = 0;
	switch (CodePage)
	{
	case 932:
		rey = MultiByteToWideChar(CodePage, dwFlags,
			lpMultiByteStr, cbMultiByte, lpWideCharStr,0x1000);
		if (IsJis(lpWideCharStr))
		{
			CRC32 crc;
			wstring gbktext(lpWideCharStr);
			DWORD strcrc = crc.Calc((char*)gbktext.c_str(), gbktext.length() * 2);
			auto scitr = REPList.find(strcrc);
			if (scitr != REPList.end())
			{
				wcscpy(lpWideCharStr, (scitr->second).c_str());
			}
			/*
			auto res = translatetext.searchtext(lpWideCharStr);
			if (res != nullptr)
			{
				wcscpy(lpWideCharStr, res);
			}
			*/
		}

		return rey;
	default:
		return MultiByteToWideChar(CodePage, dwFlags,
			lpMultiByteStr, cbMultiByte, lpWideCharStr, cchWideChar);
	}
}

HFONT WINAPI HookCreateFontIndirectW(LOGFONTW* lplf)
{
	LOGFONTW lf;
	memcpy(&lf, lplf, sizeof(LOGFONTW));
	wcscpy(lf.lfFaceName, L"SimHei");
	lf.lfCharSet = GB2312_CHARSET;
	return CreateFontIndirectW(&lf);
}

/*00199A88   007EBB70  \Text = "見上げてごらん、夜空の星を Interstellar Focus-Settings"

*/

BOOL WINAPI HookSetWindowTextW(HWND hWnd, LPCWSTR lpString)
{
	if(!wcscmp(lpString,L"見上げてごらん、夜空の星を Interstellar Focus-Settings"))
		return SetWindowTextW(hWnd, L"Game Start Setting");
	return SetWindowTextW(hWnd, lpString);
}

wchar_t* TITLE = (wchar_t*)L"【天之圣杯汉化组】敬请昂首 看那漫天繁星 Interstellar Focus - v1.0";
HWND WINAPI HookCreateWindowExW(
	DWORD     dwExStyle,
	LPCWSTR   lpClassName,
	LPCWSTR   lpWindowName,
	DWORD     dwStyle,
	int       X,
	int       Y,
	int       nWidth,
	int       nHeight,
	HWND      hWndParent,
	HMENU     hMenu,
	HINSTANCE hInstance,
	LPVOID    lpParam
)
{
	return CreateWindowExW(dwExStyle, lpClassName, TITLE, dwStyle, X, Y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
}
void IATHook()
{
	if (!IATPatch("Kernel32.dll", GetProcAddress(GetModuleHandleW(L"Kernel32.dll"), "MultiByteToWideChar"), (PROC)HookMultiByteToWideChar))
	{
		MessageBoxW(0, L"MultiByteToWideChar Hook Error。", L"IATHook Error", 0);
		return;
	}
	if (!IATPatch("Gdi32.dll", GetProcAddress(GetModuleHandleW(L"Gdi32.dll"), "CreateFontIndirectW"), (PROC)HookCreateFontIndirectW))
	{
		MessageBoxW(0, L"CreateFontIndirectW Hook Error。", L"IATHook Error", 0);
		return;
	}
	if (!IATPatch("User32.dll", GetProcAddress(GetModuleHandleW(L"User32.dll"), "SetWindowTextW"), (PROC)HookSetWindowTextW))
	{
		MessageBoxW(0, L"SetWindowTextW Hook Error。", L"IATHook Error", 0);
		return;
	}
	if (!IATPatch("User32.dll", GetProcAddress(GetModuleHandleW(L"User32.dll"), "CreateWindowExW"), (PROC)HookCreateWindowExW))
	{
		MessageBoxW(0, L"CreateWindowExW Hook Error。", L"IATHook Error", 0);
		return;
	}
}
void __stdcall AymChangeFont(wchar_t* text)
{
	wcscpy(text, L"MS UI Gothic");
}

void __stdcall AymGetFileNameW(wchar_t* FileName)
{
	cout << "ReadFile:" << wtoc(FileName) << endl;
}

PVOID pChangeFont = (PVOID)(BaseAddr + 0x3F39F);
__declspec(naked)void mChangeFont()
{
	__asm
	{
		pushad
		pushfd
		push [ebx + 0x14]
		call AymChangeFont
		popfd
		popad
		jmp pChangeFont
	}
}

PVOID pAllocMem = (PVOID)(BaseAddr + 0xC2A5D);
__declspec(naked)void mAllocMem()
{
	__asm
	{
		mov ecx,0x1000
		jmp pAllocMem
	}
}

void InlineHook()
{
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&pChangeFont, mChangeFont);
	DetourAttach(&pAllocMem, mAllocMem);
	if (DetourTransactionCommit() != NOERROR)
	{
		MessageBoxW(0, L"InlineHook Hook Error。", L"InlineHook Error", 0);
		return;
	}
}


void InitText()
{
	ifstream fin("Script.txt");
	const int LineMax = 0x500;
	char str[LineMax];
	if (fin.is_open())
	{
		int counter = 0;
		while (fin.getline(str, LineMax))
		{
			auto wtmp = ctowUTF(str);
			wstring wline = wtmp;
			if (wline[0] == (wchar_t)L'/' && wline[1] == (wchar_t)L'/')
			{
				wline = wline.substr(2);
				rawlist.push_back(wline);
			}
			else
			{
				newlist.push_back(wline);
			}
		}
		fin.close();
		size_t index = 0;
		CRC32 crc;
		for (index; index < rawlist.size(); ++index)
		{
			crc.Init();
			DWORD crcval = crc.Calc((char*)rawlist.at(index).c_str(), rawlist.at(index).length() * 2);
			REPList.insert(pair<DWORD, wstring>(crcval, newlist.at(index).c_str()));
		}
	}
	else
	{
		fin.close();
		MessageBox(0, L"Can't open script file", L"InitText Error", 0);
		return;
	}
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
	{
		//make_console();
		InitText();
		IATHook();
		InlineHook();
	}
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

