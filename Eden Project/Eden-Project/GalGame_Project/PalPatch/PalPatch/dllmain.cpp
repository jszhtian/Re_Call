// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "framework.h"
DWORD PalAddr = NULL;
vector <char*> new_text;
#define HEAD_STRING "$TEXT_LIST__"
#define _read_uint(pt,uv) \
	uv = *(unsigned int*)pt; \
	pt += sizeof(unsigned int) 

/// <summary>
/// Read Text From File
/// </summary>
long get_file_size(FILE* f) {
	long size_of_file;
	fseek(f, 0, SEEK_END);
	size_of_file = ftell(f);
	fseek(f, 0, SEEK_SET);
	return size_of_file;
}

char* _read_str(char** pt)
{
	char* ptx;
	ptx = (char*)*pt;
	*pt += strlen(ptx) + 1;

	return ptx;
}


void dec_char(char* c)
{
	char* pt;
	unsigned int strcount;
	unsigned int strid;

	unsigned int curid;
	char* cursc;

	pt = c;

	pt += sizeof(HEAD_STRING) - 1;

	_read_uint(pt, strcount);


	for (strid = 0; strid < strcount; strid++)
	{
		_read_uint(pt, curid);
		cursc = _read_str(&pt);

		new_text.push_back(cursc);

	}
}
void do_text()
{
	char* data;
	long size_of_file;

	FILE* f;


	f = fopen("TEXT.DAT", "rb");
	if (!f) return;

	size_of_file = get_file_size(f);

	data = (char*)malloc(size_of_file);

	fread(data, size_of_file, 1, f);

	fclose(f);

	if (memcmp(data, HEAD_STRING, sizeof(HEAD_STRING) - 1) == 0)
	{
		dec_char(data);
	}
}

/// <summary>
/// Hook start
/// </summary>
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
	if (fdwCharSet == 0x80)
	{
		if (strcmp(lpszFace, "俵俽 僑僔僢僋") == 0)
		{
			fdwCharSet = 0x86;
			fnWeight = FW_SEMIBOLD;
			return CreateFontAOLD(nHeight, nWidth, nEscapement, nOrientation, fnWeight, fdwItalic, 
				fdwUnderline, fdwStrikeOut, fdwCharSet, fdwOutputPrecision, fdwClipPrecision, fdwQuality, fdwPitchAndFamily, "黑体");
		}
		else if (strcmp(lpszFace, "俵俽 柧挬") == 0)
		{
		defualt:
			fdwCharSet = 0x86;
			fnWeight = FW_SEMIBOLD;
			return CreateFontAOLD(nHeight, nWidth, nEscapement, nOrientation, fnWeight, fdwItalic, 
				fdwUnderline, fdwStrikeOut, fdwCharSet, fdwOutputPrecision, fdwClipPrecision, fdwQuality, fdwPitchAndFamily, "宋体");
		}
		else
			goto defualt;
	}
	goto defualt;
}

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
	//cout << Pstr << endl;
	if (strcmp(Pstr, "本当に終了しますか？") == 0)
	{
		strcpy(Pstr, "确认要结束游戏吗？");
	}
	return MessageBoxAOLD(hWnd, Pstr, "确认", uType);
}

PVOID g_pOldSetWindowTextA = NULL;
typedef int (WINAPI* PfuncSetWindowTextA)(HWND hwnd, LPCTSTR lpString);
int WINAPI NewSetWindowTextA(HWND hwnd, LPCTSTR lpString)
{
	if (strcmp((char*)(LPCTSTR)lpString, "弶婲摦帪偺僂僀儞僪僂儌乕僪傪愝掕偟傑偡丅") == 0)
	{
		return ((PfuncSetWindowTextA)g_pOldSetWindowTextA)(hwnd, (LPCTSTR)(char*)"第一次启动游戏请选择显示模式。");
	}
	else if (strcmp((char*)(LPCTSTR)lpString, "僼儖僗僋儕乕儞偱僎乕儉傪奐巒偡傞丅") == 0)
	{
		return ((PfuncSetWindowTextA)g_pOldSetWindowTextA)(hwnd, (LPCTSTR)(char*)"以全屏模式启动");
	}
	else
	{
		return ((PfuncSetWindowTextA)g_pOldSetWindowTextA)(hwnd, lpString);
	}
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
	const char* szWndName = "【三辉汉化组】爱丽丝大游行 - v1.0";

	return ((pfuncCreateWindowExA)g_pOldCreateWindowExA)(dwExStyle, lpClassName, (LPCTSTR)szWndName, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
}

void WINAPI retree_text(char* trx_text)
{
	ULONG index = *(ULONG*)trx_text;
	trx_text += sizeof(ULONG);

	strcpy((char*)trx_text, new_text[index]);
}
char* (*GetTextByIndexOld)(int v1, int v2, int v3, int v4);
char* RepString(int v1, int v2, int v3, int v4)
{
	char* xf;
	int index;
	char* str;
	xf = GetTextByIndexOld(v1, v2, v3, v4);

	index = *(int*)xf;
	str = xf + sizeof(int);
//	if ((v3 & 0x10000000) || v4 == 0xFFFFFFF)
//		return xf;
//	retree_text(xf);
	cout << hex << index << ":" << wtocGBK(ctowJIS(str)) << endl;
	return xf;
}

void Init()
{
	*(DWORD*)& GetTextByIndexOld = BaseAddr + 0x28ca0;
	CreateFontAOLD = (fnCreateFontA)GetProcAddress(GetModuleHandle(L"gdi32.dll"), "CreateFontA");
	MessageBoxAOLD = (fnMessageboxA)GetProcAddress(GetModuleHandle(L"User32.dll"), "MessageBoxA");
	g_pOldSetWindowTextA = DetourFindFunction("USER32.dll", "SetWindowTextA");
	DetourTransactionBegin();
	DetourAttach((void**)& GetTextByIndexOld, RepString);
	DetourAttach((void**)& CreateFontAOLD, CreateFontAEx);
	DetourAttach((void**)& MessageBoxAOLD, MessageBoxAEx);
	DetourAttach(&g_pOldSetWindowTextA, NewSetWindowTextA);
	DetourAttach(&g_pOldCreateWindowExA, NewCreateWindowExA);

	DetourTransactionCommit();
}


HANDLE(*PalFileCreate)(char* FileName, void* lpBuffer);
HANDLE  EdenFileCreate(char* FileName, void* lpBuffer)
{
	if (strcmp(FileName, "SYSTEM.INI") == 0)
		strcpy(FileName, "SYS_CN.INI");
	return PalFileCreate(FileName, lpBuffer);
}

int (*PalSpriteLoadMemory)(int a1, int FileName, int FileBuff, int FileSize, int a5);
int EdenSpriteLoadMemory(int a1, int FileName, int FileBuff, int FileSize, int a5)
{
	char OutName[MAX_PATH];
	sprintf(OutName, "cn_pho\\%s", (char*)FileName);
	FILE* f = fopen(OutName, "rb");
	if (f != NULL)
	{
		DWORD size = 0;
		char* buff = NULL;
		fseek(f, 0, SEEK_END);
		size = ftell(f);
		fseek(f, 0, SEEK_SET);
		buff = (char*)malloc(size);
		if (buff != NULL && size != 0) {
			fread(buff, size, 1, f);
			FileBuff = (int)buff;
			FileSize = (int)size;
			fclose(f);
			buff = NULL;
			size = 0;
		}
		else
		{
			fclose(f);
			buff = NULL;
			size = 0;
		}
		goto retn;
	}
retn:
	return PalSpriteLoadMemory(a1, FileName, FileBuff, FileSize, a5);
}

void PalConf()
{
	*(DWORD*)&PalFileCreate = PalAddr + 0x79580;
	*(DWORD*)&PalSpriteLoadMemory = PalAddr + 0xB5C60;
	DetourTransactionBegin();
	DetourAttach((void**)&PalFileCreate, EdenFileCreate);
	DetourAttach((void**)&PalSpriteLoadMemory, EdenSpriteLoadMemory);
	DetourTransactionCommit();
}

typedef HMODULE(WINAPI* PfuncLoadLibraryExA)(
	LPCSTR lpLibFileName,
	HANDLE HfILE,
	DWORD dwFlags
	);
PfuncLoadLibraryExA LoadLibraryExA_Old;
HMODULE WINAPI MyLLEA(
	LPCSTR lpLibFileName,
	HANDLE HfILE,
	DWORD dwFlags
)
{
	HMODULE Head = LoadLibraryExA_Old(lpLibFileName, HfILE, dwFlags);
	DWORD Addr = (DWORD)Head;
	string LibName(lpLibFileName);
	string strName = LibName.substr(LibName.find_last_of("\\") + 1);
	if (strName == "PAL.dll")
	{
		PalAddr = Addr;
		PalConf();
	}
	return Head;
}

void GetPalAddr()
{
	LoadLibraryExA_Old = (PfuncLoadLibraryExA)GetProcAddress(GetModuleHandle(L"kernel32.dll"), "LoadLibraryExA");
	DetourTransactionBegin();
	DetourAttach((void**)&LoadLibraryExA_Old, MyLLEA);
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
		Init();
		//do_text();
		//GetPalAddr();
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

