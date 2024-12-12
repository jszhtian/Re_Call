// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "framework.h"
#include "detours.h"
#include "crc32.h"

vector<wstring>rawlist;
vector<wstring>newlist;

map<DWORD, wstring> SCRList;
map<DWORD, wstring> REPList;

static char* wtocUTF(LPCTSTR str)
{
	static char result[1024];
	int len = WideCharToMultiByte(CP_UTF8, 0, str, -1, NULL, 0, NULL, 0);
	WideCharToMultiByte(CP_UTF8, 0, str, -1, result, len, NULL, 0);
	result[len] = '\0';
	return result;
}

static char* wtoc(LPCTSTR str)
{
	static char result[1024];
	int len = WideCharToMultiByte(936, 0, str, -1, NULL, 0, NULL, 0);
	WideCharToMultiByte(936, 0, str, -1, result, len, NULL, 0);
	result[len] = '\0';
	return result;
}

static LPWSTR ctowUTF(char* str)
{
	static wchar_t result[1024];
	int len = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
	MultiByteToWideChar(CP_UTF8, 0, str, -1, result, len);
	result[len] = L'\0';
	return result;
}


typedef HFONT(WINAPI* fnCreateFontA)(
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
fnCreateFontA CreateFontAOLD;
HFONT WINAPI CreateFontAEx(int nHeight, int nWidth, int nEscapement, int nOrientation, int fnWeight, DWORD fdwItalic, DWORD fdwUnderline, DWORD fdwStrikeOut, DWORD fdwCharSet, DWORD fdwOutputPrecision, DWORD fdwClipPrecision, DWORD fdwQuality, DWORD fdwPitchAndFamily, LPCSTR lpszFace)
{
	fdwCharSet = 0x86;
	fnWeight = FW_SEMIBOLD;
	return CreateFontAOLD(nHeight, nWidth, nEscapement, nOrientation, fnWeight, fdwItalic, fdwUnderline, fdwStrikeOut, fdwCharSet, fdwOutputPrecision, fdwClipPrecision, fdwQuality, fdwPitchAndFamily, "黑体");
}
PVOID g_pOldCreateWindowExA = CreateWindowExA;
typedef HWND(WINAPI* pfuncCreateWindowExA)(
	DWORD dwExStyle,
	LPCTSTR lpClassName,
	LPCTSTR lpWindowName,
	DWORD dwStyle,
	int x,
	int y,
	int nWidth,
	int nHeight,
	HWND hWndParent,
	HMENU hMenu,
	HINSTANCE hInstance,
	LPVOID lpParam);
HWND WINAPI NewCreateWindowExA(DWORD dwExStyle, LPCTSTR lpClassName, LPCTSTR lpWindowName, DWORD dwStyle, int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam)
{
	const char* szWndName = "SaDisticBlooD｜简体中文化补丁V1.0";

	return ((pfuncCreateWindowExA)g_pOldCreateWindowExA)(dwExStyle, lpClassName, (LPCTSTR)szWndName, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
}
void Init()
{
	CreateFontAOLD = (fnCreateFontA)GetProcAddress(GetModuleHandle(L"gdi32.dll"), "CreateFontA");
	DetourTransactionBegin();
	DetourAttach((void**)& CreateFontAOLD, CreateFontAEx);
	DetourAttach(&g_pOldCreateWindowExA, NewCreateWindowExA);
	DetourTransactionCommit();
}

//Text=0x463a89
char* rep = NULL;
void _stdcall EdenReplaceTextA(char* Text)
{
	CRC32 crc;
	wstring gbktext = ctowJIS(Text);
	DWORD strcrc = crc.Calc((char*)gbktext.c_str(), gbktext.length() * 2);
	auto scitr = REPList.find(strcrc);
	if (scitr != REPList.end())
	{
		char* tmp = wtoc((scitr->second).c_str());
		lstrcpyA(Text, tmp);
	}
	else
	{
		char* mText = wtocUTF(gbktext.c_str());
		cout << "MISS:" << "0x" << hex << strcrc << "|" << wtoc(gbktext.c_str()) << endl;
		FILE* fp = fopen("MissText.txt", "ab+");
		fwrite("-------------------------------------------------------", 55, 1, fp);
		fwrite("\r\n", 2, 1, fp);
		fwrite(mText, strlen(mText), 1, fp);
		fwrite("\r\n", 2, 1, fp);
		fwrite("-------------------------------------------------------", 55, 1, fp);
		fwrite("\r\n", 2, 1, fp);
		fclose(fp);
	}
}

PVOID pGetText = NULL;
PVOID dGetName = NULL;//(PVOID)(0x10021C4D);
__declspec(naked)void GetText()
{
	__asm
	{
		pushad
		pushfd
		push ecx
		call EdenReplaceTextA
		popfd
		popad
		//mov dword ptr ss : [ebp - 0x8] , eax
		//mov eax, dword ptr ss : [ebp + 0x8]
		//pop ecx
		jmp pGetText
	}
}

void TextHook()
{
	//pGetText = (PVOID)(BaseAddr + 0x74f11);
	pGetText = (PVOID)(BaseAddr + 0xd3809);
	if (debug)
	{
		cout << "Base Address:" << BaseAddr << endl;
		cout << "Text Address:" << pGetText << endl;
	}
	DetourTransactionBegin();
	DetourAttach((void**)&pGetText, GetText);
	DetourTransactionCommit();
}


void InitText()
{
	ifstream fin("scr_f.txt");
	const int LineMax = 0x1000;
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
		if (rawlist.size() != newlist.size())
		{
			cout << rawlist.size() << " | " << newlist.size() << endl;
			MessageBox(0, L"rawlist != newlist", L"InitText Error", 0);
			return;
		}
		size_t index = 0;
		CRC32 crc;
		for (index; index < rawlist.size(); ++index)
		{
			crc.Init();
			DWORD crcval = crc.Calc((char*)rawlist.at(index).c_str(), rawlist.at(index).length() * 2);
			REPList.insert(pair<DWORD, wstring>(crcval, newlist.at(index).c_str()));
			//cout << "0x" << hex << crcval << "|" << wtoc(newlist.at(index).c_str()) << endl;
		}
	}
	else
	{
		fin.close();
		MessageBox(0, L"Can't open script file", L"InitText Error", 0);
		return;
	}
	cout << "Init Text Finish" << endl;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
		if (debug)
			make_console();
		Init();
		InitText();
		TextHook();
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

