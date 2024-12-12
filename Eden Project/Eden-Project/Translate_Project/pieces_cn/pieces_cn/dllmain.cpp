// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "framework.h"
#include <iostream>
#include "detours.h"
#include <fstream>
#pragma comment(lib, "detours.lib")
using namespace std;
DWORD BaseAddr = (DWORD)GetModuleHandle(NULL);
char* CheckFileName = NULL;

void _stdcall GetFileName(char* FileName)
{
	CheckFileName = FileName;
	cout << FileName << endl;
}
char* MemFileName = NULL;

void __stdcall LoadKzeFile(DWORD BufferOffset)
{
	if (CheckFileName)
	{
		cout << "--------------------------------LoadKzeFile--------------------------------" << endl;
		cout << "Read File Name: " << CheckFileName << endl;
		FILE* fp = fopen(CheckFileName, "rb");
		if (fp == NULL)
		{
			cout << "Can not find file:" << CheckFileName << endl;
			return;
		}
		fseek(fp, 0, SEEK_END);
		unsigned long size = ftell(fp);
		fseek(fp, 0, SEEK_SET);
		unsigned char* buff = new unsigned char[size];
		fread(buff, size, 1, fp);
		fclose(fp);
		if (buff)
		{
			cout << "Find File:" << CheckFileName << endl;
			memcpy((LPVOID)BufferOffset, buff, size);
		}
		cout << "--------------------------------LoadKzeFile--------------------------------" << endl;
	}
}

void __stdcall AymCreateTitleText(char* OldTitle)
{
	char* Title = (char*)"【咩咕噜汉化组】pieces/渡鸟之歌 - v1.0";
	memset(OldTitle, 0, strlen(Title));
	strcpy(OldTitle, Title);
}

PVOID File_JMPHookPoint = (PVOID)(BaseAddr + 0x1B80A);
PVOID File_JMPHookPointNext = (PVOID)(BaseAddr + 0x1B811);
PVOID File_JMPHookPoint2 = (PVOID)(BaseAddr + 0x1BABE);
PVOID File_JMPHookPointNext2 = (PVOID)(BaseAddr + 0x1BAB7);
PVOID File_JMPHookPoint3 = (PVOID)(BaseAddr + 0x1B970);
PVOID File_JMPHookPointNext3 = (PVOID)(BaseAddr + 0x1BAB7);
PVOID Title_JMPHookPoint = (PVOID)(BaseAddr + 0x5653F);
PVOID Title_JMPHookPointNext = (PVOID)(BaseAddr + 0x1BAB7);
__declspec(naked) void FileProc1()
{
	_asm
	{
		pushad
		pushfd
		push eax
		call GetFileName
		popfd
		popad
		jmp File_JMPHookPoint
	}
}
__declspec(naked) void FileProc2()
{
	_asm
	{
		pushad
		pushfd
		push esi
		call LoadKzeFile
		popfd
		popad
		jmp File_JMPHookPoint2
	}
}
__declspec(naked) void FileProc3()
{
	_asm
	{
		pushad
		pushfd
		push esi
		call LoadKzeFile
		popfd
		popad
		jmp File_JMPHookPoint3
	}
}

__declspec(naked) void TitleChange()
{
	_asm
	{
		pushad
		pushfd
		push eax
		call AymCreateTitleText
		popfd
		popad
		jmp Title_JMPHookPoint
	}
}

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
	char* out = new char[dwMinSize];
	WideCharToMultiByte(936, NULL, str, -1, out, dwMinSize, NULL, FALSE);//转换
	return out;
}

wchar_t* ctow(const char* str)
{
	wchar_t* buffer = nullptr;
	if (str)
	{
		size_t nu = strlen(str);
		size_t n = (size_t)MultiByteToWideChar(932, 0, (const char*)str, int(nu), NULL, 0);
		buffer = 0;
		buffer = new wchar_t[n];
		memset(buffer, 0, sizeof(wchar_t) * (n));
		MultiByteToWideChar(932, 0, (const char*)str, int(nu), buffer, int(n));
	}
	return buffer;
}

wchar_t* ctow936(const char* str)
{
	wchar_t* buffer = nullptr;
	if (str)
	{
		size_t nu = strlen(str);
		size_t n = (size_t)MultiByteToWideChar(936, 0, (const char*)str, int(nu), NULL, 0);
		buffer = 0;
		buffer = new wchar_t[n];
		memset(buffer, 0, sizeof(wchar_t) * (n));
		MultiByteToWideChar(936, 0, (const char*)str, int(nu), buffer, int(n));
	}
	return buffer;
}

wchar_t szTitle[] = L"确认";
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
	if (strcmp(Pstr, "ゲームを終了します。よろしいですか？") == 0)
	{
		strcpy(Pstr, "即将结束游戏。确认吗？");

	}
	return MessageBoxAOLD(hWnd, Pstr, wtocGBK(szTitle), uType);
}

typedef HFONT(WINAPI* fnCreateFontA)(_In_ int     nHeight,
	_In_ int     nWidth,
	_In_ int     nEscapement,
	_In_ int     nOrientation,
	_In_ int     fnWeight,
	_In_ DWORD   fdwItalic,
	_In_ DWORD   fdwUnderline,
	_In_ DWORD   fdwStrikeOut,
	_In_ DWORD   fdwCharSet,
	_In_ DWORD   fdwOutputPrecision,
	_In_ DWORD   fdwClipPrecision,
	_In_ DWORD   fdwQuality,
	_In_ DWORD   fdwPitchAndFamily,
	_In_ LPCTSTR lpszFace);
typedef int (WINAPI* fnEnumFontFamiliesExA)(
	HDC           hdc,
	LPLOGFONTA    lpLogfont,
	FONTENUMPROCA lpProc,
	LPARAM        lParam,
	DWORD         dwFlags
	);
fnCreateFontA pCreateFontA;
fnEnumFontFamiliesExA pEnumFontFamiliesExA;
PVOID g_pOldCreateFontIndirectA = CreateFontIndirectA;
typedef HFONT(WINAPI* PfuncCreateFontIndirectA)(LOGFONTA* lplf);
HFONT WINAPI NewCreateFontIndirectA(LOGFONTA* lplf)
{
	//wchar_t* wcs = nullptr;
	lplf->lfCharSet = ANSI_CHARSET;
	//strcpy(lplf->lfFaceName, "黑体");
	char buffer[0x100] = { 0 };
	memset(buffer, 0, 0x100);
	memcpy(buffer, lplf->lfFaceName, strlen(lplf->lfFaceName));
	//wcs = ctow936(buffer);
	strcpy(lplf->lfFaceName, buffer);
	return ((PfuncCreateFontIndirectA)g_pOldCreateFontIndirectA)(lplf);
}
HFONT WINAPI newCreateFontA(_In_ int     nHeight,
	_In_ int     nWidth,
	_In_ int     nEscapement,
	_In_ int     nOrientation,
	_In_ int     fnWeight,
	_In_ DWORD   fdwItalic,
	_In_ DWORD   fdwUnderline,
	_In_ DWORD   fdwStrikeOut,
	_In_ DWORD   fdwCharSet,
	_In_ DWORD   fdwOutputPrecision,
	_In_ DWORD   fdwClipPrecision,
	_In_ DWORD   fdwQuality,
	_In_ DWORD   fdwPitchAndFamily,
	_In_ LPCSTR lpszFace)
{
	wchar_t* wcs = nullptr;
	fdwCharSet = ANSI_CHARSET;
	char buffer[0x100] = { 0 };
	memset(buffer, 0, 0x100);
	memcpy(buffer, lpszFace, strlen(lpszFace));
	wcs = ctow936(buffer);
	return CreateFontW(nHeight, nWidth, nEscapement, nOrientation, fnWeight, fdwItalic, fdwUnderline, fdwStrikeOut, fdwCharSet, fdwOutputPrecision, fdwClipPrecision, fdwQuality, fdwPitchAndFamily, wcs);
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
	//cout << strName << endl;
	if (strName == "yshpd.dat")
	{
		//MessageBoxA(NULL, "Found", "Asuka", MB_OK);
		strNewName = strDirName + "pieces_cn.siu";
	}
	else
	{
		strNewName = strOldName;
	}
	return ((PfuncCreateFileA)(g_pOldCreateFileA))(
		strNewName.c_str(),
		dwDesiredAccess,
		dwShareMode,
		lpSecurityAttributes,
		dwCreationDisposition,
		dwFlagsAndAttributes,
		hTemplateFile);
}

int WINAPI newEnumFontFamiliesExA(HDC hdc, LPLOGFONTA lpLogfont, FONTENUMPROCA lpProc, LPARAM lParam, DWORD dwFlags)
{
	LOGFONTA Lfont;
	memcpy(&Lfont, lpLogfont, sizeof(LOGFONTA));
	Lfont.lfCharSet = GB2312_CHARSET;
	memcpy(lpLogfont, &Lfont, sizeof(LOGFONTA));
	return ((fnEnumFontFamiliesExA)pEnumFontFamiliesExA)(hdc, lpLogfont, lpProc, lParam, dwFlags);
}
///////////////////////////////////////////////////MAP///////////////////////////////////////////////////
uint16_t SjisUnicodeTable[65536];
void init_fnt_table()
{
	auto fs = fopen("CHS.TBL", "rb");
	if (!fs)
	{
		MessageBox(0, L"Can't Find AymTextOut Suppost List File.", L"Aym", 0);
		return;
	}
	fread(SjisUnicodeTable, 1, 65536 * 2, fs);
	fclose(fs);
}

PVOID g_pOldTextOutA = NULL;
typedef int(WINAPI* PfuncTextOutA)(HDC hdc, int x, int y, LPCSTR lpString, int c);
int WINAPI NewTextOutA(HDC hdc, int x, int y, LPCSTR lpString, int c)
{
	unsigned short uChar = 0;
	memcpy((unsigned char*)&uChar + 1, lpString, 1);
	memcpy((unsigned char*)&uChar, lpString + 1, 1);
	if (uChar >= 0x81 && uChar <= 0x9F || uChar >= 0xE0 && uChar <= 0xFC)
	{
		uChar = SjisUnicodeTable[uChar];
		unsigned char p = 0;
		memcpy(&p, (unsigned char*)&uChar + 1, 1);
		memcpy((unsigned char*)&uChar + 1, (unsigned char*)&uChar, 1);
		memcpy((unsigned char*)&uChar, &p, 1);
		//return TextOutW(hdc, x, y, (LPWSTR)uChar, c);
	}
	return ((PfuncTextOutA)g_pOldTextOutA)(hdc, x, y, (LPCSTR)&uChar, c);
}

///////////////////////////////////////////////////END///////////////////////////////////////////////////
void Init()
{
	MessageBoxAOLD = (fnMessageboxA)GetProcAddress(GetModuleHandle(L"User32.dll"), "MessageBoxA");
	pCreateFontA = (fnCreateFontA)GetProcAddress(GetModuleHandle(L"gdi32.dll"), "CreateFontA");
	pEnumFontFamiliesExA = (fnEnumFontFamiliesExA)GetProcAddress(GetModuleHandle(L"gdi32.dll"), "EnumFontFamiliesExA");
	g_pOldTextOutA = DetourFindFunction("GDI32.dll", "TextOutA");
	DetourTransactionBegin();
	//DetourAttach(&g_pOldCreateFontIndirectA, NewCreateFontIndirectA);
	//DetourAttach((void**)& pCreateFontA, newCreateFontA);
	//DetourAttach(&g_pOldCreateFileA, NewCreateFileA);
	//DetourAttach((void**)& pEnumFontFamiliesExA, newEnumFontFamiliesExA);
	//DetourAttach(&g_pOldTextOutA, NewTextOutA);
	DetourAttach((void**)& MessageBoxAOLD, MessageBoxAEx);

	//DetourAttach((void**)& File_JMPHookPoint, FileProc1);
	//DetourAttach((void**)& File_JMPHookPoint2, FileProc2);
	//DetourAttach((void**)& File_JMPHookPoint3, FileProc3);
	//DetourAttach((void**)&Title_JMPHookPoint, TitleChange);
	if (DetourTransactionCommit() != NOERROR)
		MessageBox(NULL, L"Hook Error", L"Aym", MB_OK);

}

void memcopy(void* dest, void* src, size_t size)
{
	DWORD oldProtect;
	VirtualProtect(dest, size, PAGE_EXECUTE_READWRITE, &oldProtect);
	memcpy(dest, src, size);
}

void InitBypassCheck()
{
	BYTE FontEnumSuppost[] = { 0x90,0x90 };
	memcopy((void*)(BaseAddr + 0x397C), FontEnumSuppost, sizeof(FontEnumSuppost));

	BYTE JMP[] = { 0xEB };
	BYTE JNB[] = { 0x0F,0x83 };
	BYTE OffsetFix[] = { 0xEB,0x53 };
	BYTE JeAddrFix[] = { 0x0F,0x84,0xB4 };
	memcopy((void*)(BaseAddr + 0x67887), JMP, sizeof(JMP));
	memcopy((void*)(BaseAddr + 0x679F1), OffsetFix, sizeof(OffsetFix));
	memcopy((void*)(BaseAddr + 0x67A46), JNB, sizeof(JNB));
	memcopy((void*)(BaseAddr + 0x67A73), JNB, sizeof(JNB));
	memcopy((void*)(BaseAddr + 0x672BD), JeAddrFix, sizeof(JeAddrFix));
	cout << "0x" << hex << (BaseAddr + 0x67887) << endl;
	cout << "0x" << hex << (BaseAddr + 0x679F1) << endl;
	cout << "0x" << hex << (BaseAddr + 0x67A46) << endl;
	cout << "0x" << hex << (BaseAddr + 0x67A73) << endl;
	cout << "0x" << hex << (BaseAddr + 0x672BD) << endl;
}

static void make_console() {
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
	freopen("CONIN$", "r", stdin);
	cout << "Console\n" << endl;
}

bool IsKnji(char* s)
{
	for (int i = 0; i < lstrlenA(s); i++)
	{
		if ((unsigned int)s[i] > 0x7F)
			return true;
	}
	return false;
}

ofstream LOG("log.txt");
void __stdcall PrintMsg(char* txt)
{
	//cout << wtocGBK(ctowJIS(txt)) << endl;
	if (IsKnji(txt) == true) 
	{
		LOG << txt << endl;
		/*
		if (!lstrcmpA(txt, "愄乆偺嬻偺忋丅"))
		{
			//cout << "hit" << endl;
			lstrcpyA(txt, "中文显示测试，超过当前长度测试测试测试啊啊啊啊啊啊啊");
		}

		else if (!lstrcmpA(txt, "揤偺崙偱偼恄條偺壓丄懡偔偺揤巊偨偪偑曢傜偟偰偄傑偟偨丅"))
		{
			lstrcpyA(txt, "继续中文超很长测试测试场啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊中文显示测试，超过当前长度测试测试测试啊啊啊啊啊啊啊");
		}
		else if (!lstrcmpA(txt, "\\VO(CAS_0_ALL_000_0001)亂揤巊亃乽懜偄傕偺丄偱偡偐丠乿"))
		{
			lstrcpyA(txt, "\\VO(CAS_0_ALL_000_0001)亂揤巊亃乽中文文本显示测试。");
		}
		else if (!lstrcmpA(txt, "亂揤巊亃乽懜偄傕偺丄偱偡偐丠乿"))
		{
			lstrcpyA(txt, "亂揤巊亃乽中文文本显示测试。");
		}
		else if (!lstrcmpA(txt, "乽懜偄傕偺丄偱偡偐丠乿"))
		{
			lstrcpyA(txt, "乽中文文本显示测试。");
		}
		*/
	}
}

PVOID gPrintMsg = (PVOID)(BaseAddr + 0x4D649);
__declspec(naked) void pPrintMsg()
{
	_asm
	{
		pushad;
		pushfd;
		push ebx;
		call PrintMsg;
		popfd;
		popad;
		jmp gPrintMsg;
	}
}

void testhook()
{
	DetourTransactionBegin();
	DetourAttach(&gPrintMsg, pPrintMsg);
	if (DetourTransactionCommit() != NOERROR)
		MessageBox(NULL, L"Hook Error", L"Aym", MB_OK);
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
		//init_fnt_table();
		//InitBypassCheck();
		//Init();
		testhook();
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
extern "C" __declspec(dllexport) void AyamiKaze(void) {
	return;
}
