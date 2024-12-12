// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "framework.h"
PVOID g_pOldCreateFontA = CreateFontA;
typedef HFONT(WINAPI* PfuncCreateFontA)(
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
HFONT WINAPI HookCreateFontA(
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
)
{
	return ((PfuncCreateFontA)g_pOldCreateFontA)(
		nHeight, // logical height of font height
		nWidth, // logical average character width
		nEscapement, // angle of escapement
		nOrientation, // base-line orientation angle
		fnWeight, // font weight
		fdwItalic, // italic attribute flag
		fdwUnderline, // underline attribute flag
		fdwStrikeOut, // strikeout attribute flag
		GB2312_CHARSET, // character set identifier
		fdwOutputPrecision, // output precision
		fdwClipPrecision, // clipping precision
		fdwQuality, // output quality
		fdwPitchAndFamily, // pitch and family
		"黑体" // pointer to typeface name string
		);
}

PVOID g_pOldCreateFontIndirectA = CreateFontIndirectA;
typedef HFONT(WINAPI* PfuncCreateFontIndirectA)(LOGFONTA* lplf);
HFONT WINAPI HookCreateFontIndirectA(LOGFONTA* lplf)
{
	lplf->lfCharSet = GB2312_CHARSET;
	lstrcpyA(lplf->lfFaceName, "黑体");
	return ((PfuncCreateFontIndirectA)g_pOldCreateFontIndirectA)(lplf);
}

/*
PVOID g_pOldImmSetCompositionFontA = ImmSetCompositionFontA;
typedef BOOL(WINAPI* PfuncImmSetCompositionFontA)(
	HIMC       unnamedParam1,
	LPLOGFONTA lplf);
BOOL WINAPI HookImmSetCompositionFontA(
	HIMC       unnamedParam1,
	LPLOGFONTA lplf)
{
	lplf->lfCharSet = GB2312_CHARSET;
	lstrcpyA(lplf->lfFaceName, "黑体");
	return ((PfuncImmSetCompositionFontA)g_pOldImmSetCompositionFontA)(unnamedParam1, lplf);
}
*/

PVOID g_pOldMessageBoxA = MessageBoxA;
typedef int (WINAPI* PfuncMessageboxA)(
	_In_opt_ HWND    hWnd,
	_In_opt_ LPCSTR lpText,
	_In_opt_ LPCSTR lpCaption,
	_In_     UINT    uType
	);
int WINAPI HookMessageBoxA(
	_In_opt_ HWND    hWnd,
	_In_opt_ LPCSTR lpText,
	_In_opt_ LPCSTR lpCaption,
	_In_     UINT    uType)
{
	wchar_t* lpText_W = ctow((char*)lpText, 932);
	wchar_t* lpCaption_W = ctow((char*)lpCaption, 932);
	if (!lstrcmpW(lpText_W, L"ゲームを終了しますか？"))
		return MessageBoxW(hWnd, L"确认结束游戏吗？", L"提示", uType);
	return MessageBoxW(hWnd, lpText_W, lpCaption_W, uType);
}

// 边界

int __fastcall TextProc1(BYTE* a1)
{
	int result; // eax

	for (result = 0; *a1; ++result)
	{
		switch (*a1 & 0xF0)
		{
		case 0x80:
		case 0x90:
		case 0xA0:
		case 0xB0:
		case 0xC0:
		case 0xD0:
		case 0xE0:
		case 0xF0:
			++a1;
			break;
		default:
			break;
		}
		++a1;
	}
	return result;
}

int TextProc2(int a1/*edi*/, unsigned int a2/*esi*/)
{
	int v2; // ecx
	unsigned int v3; // edx

	v2 = 0;
	v3 = 0;
	if (!a2)
		return v2 + a1;
	while (*(BYTE*)(v2 + a1))
	{
		switch (*(BYTE*)(v2 + a1) & 0xF0)
		{
		case 0x80:
		case 0x90:
		case 0xA0:
		case 0xB0:
		case 0xC0:
		case 0xD0:
		case 0xE0:
		case 0xF0:
			++v2;
			break;
		default:
			break;
		}
		++v3;
		++v2;
		if (v3 >= a2)
			return v2 + a1;
	}
	return 0;
}

_declspec(naked) void _TextProc2()
{
	_asm
	{
		push esi;
		push edi;
		call TextProc2;
		pop edi;
		pop esi;
		ret;
	}
}

int __fastcall TextProc3(BYTE* a1)
{
	int result; // eax

	result = 0;
	if (a1 && *a1)
	{
		do
		{
			switch (*a1 & 0xF0)
			{
			case 0x80:
			case 0x90:
			case 0xA0:
			case 0xB0:
			case 0xC0:
			case 0xD0:
			case 0xE0:
			case 0xF0:
				++a1;
				break;
			default:
				break;
			}
			++a1;
			++result;
		} while (*a1);
	}
	return result;
}

int TextProc4(BYTE* a1/*ecx*/, unsigned int a2/*edi*/)
{
	int result; // eax
	unsigned int v3; // esi

	result = 0;
	if (a1)
	{
		if (a2)
		{
			v3 = 0;
			if (*a1)
			{
				do
				{
					if (v3 >= a2)
						break;
					switch (a1[result] & 0xF0)
					{
					case 0x80:
					case 0x90:
					case 0xA0:
					case 0xB0:
					case 0xC0:
					case 0xD0:
					case 0xE0:
					case 0xF0:
						++result;
						break;
					default:
						break;
					}
					++result;
					++v3;
				} while (a1[result]);
			}
		}
		else if (*a1)
		{
			do
				++result;
			while (a1[result]);
		}
	}
	return result;
}

_declspec(naked) void _TextProc4()
{
	_asm
	{
		push edi;
		push ecx;
		call TextProc4;
		pop ecx;
		pop edi;
		ret;
	}
}

void WINAPI SystemUntHook()
{
	PVOID TextProc1_addr = (PVOID)(SystemUntAddr + 0x13100);
	PVOID TextProc2_addr = (PVOID)(SystemUntAddr + 0x131C0);
	PVOID TextProc3_addr = (PVOID)(SystemUntAddr + 0x1EB60);
	PVOID TextProc4_addr = (PVOID)(SystemUntAddr + 0x1EC20);
	//GetMCGFileName_addr = (PVOID)(SystemUntAddr + 0x27933);

	cout << hex << "TextProc1_addr:" << TextProc1_addr << endl;
	cout << hex << "TextProc2_addr:" << TextProc2_addr << endl;
	cout << hex << "TextProc3_addr:" << TextProc3_addr << endl;
	cout << hex << "TextProc4_addr:" << TextProc4_addr << endl;
	//cout << hex << "GetMCGFileName_addr:" << GetMCGFileName_addr << endl;
	DetourTransactionBegin();
	DetourAttach((void**)&TextProc1_addr, TextProc1);
	DetourAttach((void**)&TextProc2_addr, _TextProc2); // naked
	DetourAttach((void**)&TextProc3_addr, TextProc3);
	DetourAttach((void**)&TextProc4_addr, _TextProc4); // naked
	//DetourAttach((void**)&GetMCGFileName_addr, GetMCGFileName); // naked
	if (DetourTransactionCommit() != NOERROR)
	{
		MessageBox(NULL, L"SystemUnt Hook Error", L"SystemUntHook", MB_OK | MB_ICONERROR);
		ExitProcess(-1);
	}
}

PVOID g_pOldLoadLibraryExA = LoadLibraryExA;
typedef HMODULE(WINAPI* PfuncLoadLibraryExA)(
	LPCSTR lpLibFileName,
	HANDLE hFile,
	DWORD  dwFlags
	);
HMODULE WINAPI HookLoadLibraryExA(
	LPCSTR lpLibFileName,
	HANDLE hFile,
	DWORD  dwFlags
)
{
	HMODULE ret = ((PfuncLoadLibraryExA)g_pOldLoadLibraryExA)(lpLibFileName, hFile, dwFlags);
	if (!lstrcmpA(lpLibFileName, "system.unt"))
	{
		SystemUntAddr = (DWORD)GetModuleHandleA("system.unt");
		if (SystemUntAddr == 0)
		{
			MessageBox(NULL, L"SystemUntAddr is 0", L"HookLoadLibraryExA", MB_OK | MB_ICONERROR);
			ExitProcess(-1);
		}
		cout << hex << "SystemUntAddr:" << SystemUntAddr << endl;
		SystemUntHook();
	}
	return ret;
}

void WINAPI APIHook()
{
	DetourTransactionBegin();
	DetourAttach(&g_pOldCreateFontA, HookCreateFontA);
	DetourAttach(&g_pOldCreateFontIndirectA, HookCreateFontIndirectA);
	//DetourAttach(&g_pOldImmSetCompositionFontA, HookImmSetCompositionFontA);
	//DetourAttach(&g_pOldSetWindowTextA, HookSetWindowTextA);
	DetourAttach(&g_pOldMessageBoxA, HookMessageBoxA);
	DetourAttach(&g_pOldLoadLibraryExA, HookLoadLibraryExA);
	//DetourAttach(&g_pOldLoadLibraryA, HookLoadLibraryA);
	if (DetourTransactionCommit() != NOERROR)
	{
		MessageBox(NULL, L"API Hook Error", L"APIHook", MB_OK | MB_ICONERROR);
		ExitProcess(-1);
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
		make_console();
		APIHook();
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

__declspec(dllexport)void WINAPI AyamiKaze()
{
}