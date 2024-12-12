// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "framework.h"
#include "MyNative.h"

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
	fdwCharSet = GB2312_CHARSET;
	return CreateFontW(nHeight, nWidth, nEscapement, nOrientation, fnWeight, fdwItalic, fdwUnderline, fdwStrikeOut, fdwCharSet, fdwOutputPrecision, fdwClipPrecision, fdwQuality, fdwPitchAndFamily, L"黑体");
}

PVOID g_pOldCreateFileA = CreateFileA;
typedef HANDLE(WINAPI* PfuncCreateFileA)(
	LPCSTR lpFileName,
	DWORD dwDesiredAccess,
	DWORD dwShareMode,
	LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	DWORD dwCreationDisposition,
	DWORD dwFlagsAndAttributes,
	HANDLE hTemplateFile
	);


HANDLE WINAPI NewCreateFileA(
	LPCSTR lpFileName,
	DWORD dwDesiredAccess,
	DWORD dwShareMode,
	LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	DWORD dwCreationDisposition,
	DWORD dwFlagsAndAttributes,
	HANDLE hTemplateFile)
{
	if (!lpFileName)
	{
		return INVALID_HANDLE_VALUE;
	}

	string strOldName(lpFileName);
	string strDirName = strOldName.substr(0, strOldName.find_last_of("\\") + 1);
	string strName = strOldName.substr(strOldName.find_last_of("\\") + 1);
	string strNewName;

	cout << strOldName << endl;

	return ((PfuncCreateFileA)(g_pOldCreateFileA))(
		lpFileName,
		dwDesiredAccess,
		dwShareMode,
		lpSecurityAttributes,
		dwCreationDisposition,
		dwFlagsAndAttributes,
		hTemplateFile);
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
	const char* szWndName = "蔷薇树上、蔷薇花开 - v1.0";

	return ((pfuncCreateWindowExA)g_pOldCreateWindowExA)(dwExStyle, lpClassName, (LPCTSTR)szWndName, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
}

HFONT putDC()
{
	LOGFONTW newlogfont;
	HFONT newhFont;
	newlogfont.lfHeight = 48;
	newlogfont.lfWidth = 24;
	newlogfont.lfWeight = 0;
	newlogfont.lfEscapement = 0;
	newlogfont.lfOrientation = 0;
	newlogfont.lfItalic = 0;
	newlogfont.lfUnderline = 0;
	newlogfont.lfStrikeOut = 0;
	newlogfont.lfCharSet = SHIFTJIS_CHARSET;
	newlogfont.lfOutPrecision = 0;
	newlogfont.lfClipPrecision = 0;
	newlogfont.lfQuality = 0;
	newlogfont.lfPitchAndFamily = 0;
	wsprintf(newlogfont.lfFaceName, L"%ls", L"黑体");
	return CreateFontIndirectW(&newlogfont);
}

HFONT putDC2()
{
	LOGFONTW newlogfont;
	HFONT newhFont;
	newlogfont.lfHeight = 48;
	newlogfont.lfWidth = 24;
	newlogfont.lfWeight = 0;
	newlogfont.lfEscapement = 0;
	newlogfont.lfOrientation = 0;
	newlogfont.lfItalic = 0;
	newlogfont.lfUnderline = 0;
	newlogfont.lfStrikeOut = 0;
	newlogfont.lfCharSet = GB2312_CHARSET;
	newlogfont.lfOutPrecision = 0;
	newlogfont.lfClipPrecision = 0;
	newlogfont.lfQuality = 0;
	newlogfont.lfPitchAndFamily = 0;
	wsprintf(newlogfont.lfFaceName, L"%ls", L"黑体");
	return CreateFontIndirectW(&newlogfont);
}

PVOID pTextOutA = TextOutA;
typedef BOOL(WINAPI* pfTextOutA)(
	HDC    hdc,
	int    x,
	int    y,
	LPCSTR lpString,
	int    c
);
int sig = 0;
BOOL WINAPI NewTextOutA(HDC hdc, int x, int y, LPCSTR lpString, int c)
{
	UINT	CodePage = CP_GB2312;
	WCHAR   str, TextBuff[0x400] = { 0 };
	BOOL ret = false;
	if (c > 2)
	{
		if (IsShiftJISString((PCHAR)lpString, c))
			CodePage = CP_SHIFTJIS;
		goto MBYTE_TO_WCHAR;
	}
	else if (sig == 1)
	{
		str = *(PWCHAR)lpString;
		switch ((ULONG)str)
		{
		case 0x4881:
			SelectObject(hdc, putDC());
			ret = TextOutW(hdc, x, y, L"ä", 0x1);
			break;
		case 0x6C82:
			SelectObject(hdc, putDC());
			ret = TextOutW(hdc, x, y, L"M", 0x1);
			break;
		case 0x8482:
			SelectObject(hdc, putDC());
			ret = TextOutW(hdc, x, y, L"d", 0x1);
			break;
		case 0x8382:
			SelectObject(hdc, putDC());
			ret = TextOutW(hdc, x, y, L"c", 0x1);
			break;
		case 0x8882:
			SelectObject(hdc, putDC());
			ret = TextOutW(hdc, x, y, L"h", 0x1);
			break;
		case 0x8582:
			SelectObject(hdc, putDC());
			ret = TextOutW(hdc, x, y, L"e", 0x1);
			break;
		case 0x8E82:
			SelectObject(hdc, putDC());
			ret = TextOutW(hdc, x, y, L"n", 0x1);
			break;
		default:
			goto MBYTE_TO_WCHAR;
		}
		DeleteObject(putDC());
		sig = 0;
	}
	else 
	{
MBYTE_TO_WCHAR:
		DWORD dwMinSize = MultiByteToWideChar(CodePage, 0, lpString, c, TextBuff, _countof(TextBuff));
		SelectObject(hdc, putDC2());
		ret = TextOutW(hdc, x, y, TextBuff, dwMinSize);
		DeleteObject(putDC2());
	}
	return ret;
}
void APIHookInit()
{
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	CreateFontAOLD = (fnCreateFontA)GetProcAddress(GetModuleHandle(L"gdi32.dll"), "CreateFontA");
	DetourAttach((void**)&CreateFontAOLD, CreateFontAEx);
	//DetourAttach(&g_pOldCreateFileA, NewCreateFileA);
	DetourAttach(&g_pOldCreateWindowExA, NewCreateWindowExA);
	DetourAttach(&pTextOutA, NewTextOutA);

	if (DetourTransactionCommit() != NOERROR)
		MessageBox(NULL, L"ERROR", L"Aym", MB_OK);

}

bool IsText(char* text)
{
	int len = strlen(text);
	if (len == 0)
		return false;
	for (int i = 0; i < len; i++)
	{
		if ((unsigned int)text[i] < 0x20)
			return false;
	}
	return true;
}

unsigned short *tbl_data;

void _stdcall EdenReplaceTextA(char* Text)
{
	if (IsText(Text))
	{
		//cout << ctowJIS(Text) << endl;
		wchar_t* para = ctowJIS(Text);
//		cout << "----------------------------------------------------" << endl;
//		cout << wtoc(para) << endl;
//		cout << "----------------------------------------------------" << endl;
		if(!lstrcmpW(para,L"オープニングを見ますか？"))
			strcpy(Text, "要观看序章吗？Mädchen");
	}
}

PVOID pGetText = (PVOID)(BaseAddr + 0x402DA);
__declspec(naked)void GetText()
{
	__asm
	{
		pushad
		pushfd
		push edx
		call EdenReplaceTextA
		popfd
		popad
		jmp pGetText
	}
}
PVOID pFixText = (PVOID)(BaseAddr + 0x406EF);
PVOID pFixText2 = (PVOID)(BaseAddr + 0x406F7);
__declspec(naked)void FixText()
{
	__asm
	{
		pushad
		pushfd
		mov sig,0x1
		popfd
		popad
		jmp pFixText
	}
}

void TextHook()
{
	cout << "Base Address:" << BaseAddr << endl;
	cout << "Text Address:" << pGetText << endl;
	DetourTransactionBegin();
	DetourAttach((void**)&pGetText, GetText);
	DetourAttach((void**)&pFixText, FixText);
	if (DetourTransactionCommit() != NOERROR)
		MessageBox(NULL, L"ERROR2", L"Aym", MB_OK);
}

void InirTbl()
{
	FILE* fp = fopen("CHS.TBL", "rb");
	tbl_data = (unsigned short *)malloc(0xFF00 * 2);
	fread(&tbl_data, 1, 0xFF00 * 2, fp);
	fclose(fp);
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
		APIHookInit();
		TextHook();
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

