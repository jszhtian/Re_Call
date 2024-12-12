// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "framework.h"
//ofstream flog("LOG.txt");
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
char* (*GetTextByIndexOld)(int pos, int bufsize);
char* RepString(int pos, int bufsize)
{
	_asm
	{
		pushad
		pushfd
	}
	char* ret;
	int index;
	ret = GetTextByIndexOld(pos, bufsize);
	index = *(int*)ret;
	char* Pstr = ret + sizeof(int);
	//if (debug)
		//cout << "Index:" << index << " Str:" << wtocGBK(ctowJIS(Pstr)) << endl;
	if (Exec) 
	{
		wchar_t* tmpStr = ctowJIS(Pstr);
		auto res = translatetext.searchtext(tmpStr);
		if (res != nullptr)
		{
			if (debug)
				cout << "Replaced:" << wtoc(res) << endl;
			strcpy(Pstr, wtoc(res));
			//copy_str(Old, L"夏休み。", PushStr, (char*)"暑假。");
		}
		else
		{
			if (debug)
			{
				cout << "---------------------------ATTENTION---------------------------" << endl;
				cout << "Missed:" << wtocGBK(ctowJIS(Pstr)) << endl;
				//MT << "Missed:" << Pstr << endl;
				cout << "---------------------------ATTENTION---------------------------" << endl;
			}
		}
	}
	//ScriptInsert(tmpStr, Pstr);
	_asm
	{
		popfd
		popad
	}
	return ret;
}

HFONT WINAPI CreateFontAEx(int nHeight, int nWidth, int nEscapement, int nOrientation, int fnWeight, DWORD fdwItalic, DWORD fdwUnderline, DWORD fdwStrikeOut, DWORD fdwCharSet, DWORD fdwOutputPrecision, DWORD fdwClipPrecision, DWORD fdwQuality, DWORD fdwPitchAndFamily, LPCSTR lpszFace)
{
	if (fdwCharSet == 0x80)
	{
		if (strcmp(lpszFace, "俵俽 僑僔僢僋") == 0)
		{
			fdwCharSet = 0x86;
			fnWeight = FW_SEMIBOLD;
			return CreateFontAOLD(nHeight, nWidth, nEscapement, nOrientation, fnWeight, fdwItalic, fdwUnderline, fdwStrikeOut, fdwCharSet, fdwOutputPrecision, fdwClipPrecision, fdwQuality, fdwPitchAndFamily, "黑体");
		}
		else if (strcmp(lpszFace, "俵俽 柧挬") == 0)
		{
		defualt:
			fdwCharSet = 0x86;
			fnWeight = FW_SEMIBOLD;
			return CreateFontAOLD(nHeight, nWidth, nEscapement, nOrientation, fnWeight, fdwItalic, fdwUnderline, fdwStrikeOut, fdwCharSet, fdwOutputPrecision, fdwClipPrecision, fdwQuality, fdwPitchAndFamily, "宋体");
		}
		else
			goto defualt;
		//return CreateFontAOLD(nHeight, nWidth, nEscapement, nOrientation, fnWeight, fdwItalic, fdwUnderline, fdwStrikeOut, fdwCharSet, fdwOutputPrecision, fdwClipPrecision, fdwQuality, fdwPitchAndFamily, "黑体");
	}
	goto defualt;
	//return CreateFontAOLD(nHeight, nWidth, nEscapement, nOrientation, fnWeight, fdwItalic, fdwUnderline, fdwStrikeOut, fdwCharSet, fdwOutputPrecision, fdwClipPrecision, fdwQuality, fdwPitchAndFamily, lpszFace);
}

PVOID g_pOldCreateFontIndirectA = NULL;
typedef int (WINAPI* PfuncCreateFontIndirectA)(LOGFONTA* lplf);
int WINAPI NewCreateFontIndirectA(LOGFONTA* lplf)
{
	//strcpy(lplf->lfFaceName, "黑体");
	//strcpy(lplf->lfFaceName, "楷体");
	lplf->lfCharSet = GB2312_CHARSET;
	//lplf->lfCharSet = ANSI_CHARSET;
	//lplf->lfCharSet = SHIFTJIS_CHARSET;

	return ((PfuncCreateFontIndirectA)g_pOldCreateFontIndirectA)(lplf);
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
	return MessageBoxAOLD(hWnd, Pstr, "ルルカリリカルラリラルララ～ ", uType);
}
DWORD PalAddr=NULL;

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
	HMODULE Head = LoadLibraryExA_Old(lpLibFileName,HfILE,dwFlags);
	DWORD Addr = (DWORD)Head;
	string LibName(lpLibFileName);
	//string DirName = LibName.substr(0, LibName.find_last_of("\\") + 1);
	string strName = LibName.substr(LibName.find_last_of("\\") + 1);
	if (strName == "PAL.dll")
	{
		//cout << strName << endl;
		//cout << "0x" << hex << Addr << endl;
		//JMPHelper((PVOID)(Addr+0x82D0))
		PalAddr = Addr;
	}
	return Head;
}

PVOID g_pOldSetWindowTextA = NULL;
typedef int (WINAPI* PfuncSetWindowTextA)(HWND hwnd, LPCTSTR lpString);
int WINAPI NewSetWindowTextA(HWND hwnd, LPCTSTR lpString)
{
	if (strcmp((char*)(LPCTSTR)lpString, "弶婲摦帪偺僂僀儞僪僂儌乕僪傪愝掕偟傑偡丅")==0)
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

void GetPalAddr()
{
	LoadLibraryExA_Old = (PfuncLoadLibraryExA)GetProcAddress(GetModuleHandle(L"kernel32.dll"), "LoadLibraryExA");
	DetourTransactionBegin();
	DetourAttach((void**)& LoadLibraryExA_Old, MyLLEA);
	DetourTransactionCommit();
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
	const char* szWndName = "【伊甸学院-5TH】萝莉吸血鬼！～与吸血鬼姐妹的性福生活～｜简体中文化补丁V1.0";

	return ((pfuncCreateWindowExA)g_pOldCreateWindowExA)(dwExStyle, lpClassName, (LPCTSTR)szWndName, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
}
void SavePatch()
{
	BYTE PatchName[] = { 0x73,0x61,0x76,0x5F,0x63,0x6E };
	int PatchPos = BaseAddr + 0x656b1;
	memcopy((void*)PatchPos, PatchName, sizeof(PatchName));
}

void Init()
{
	*(DWORD*)& GetTextByIndexOld = BaseAddr + 0x31B00;
	if (debug) 
	{
		cout << "MainAddress:0x00" << hex << (BaseAddr) << endl;
		cout << "HookAddress:0x" << hex << (GetTextByIndexOld) << endl;
	}
	CreateFontAOLD = (fnCreateFontA)GetProcAddress(GetModuleHandle(L"gdi32.dll"), "CreateFontA");
	MessageBoxAOLD = (fnMessageboxA)GetProcAddress(GetModuleHandle(L"User32.dll"), "MessageBoxA");
	g_pOldSetWindowTextA = DetourFindFunction("USER32.dll", "SetWindowTextA");
	g_pOldCreateFontIndirectA = DetourFindFunction("GDI32.dll", "CreateFontIndirectA");
	DetourTransactionBegin();
	DetourAttach((void**)& GetTextByIndexOld, RepString);
	DetourAttach((void**)& CreateFontAOLD, CreateFontAEx);
	DetourAttach((void**)& MessageBoxAOLD, MessageBoxAEx);
	DetourAttach(&g_pOldSetWindowTextA, NewSetWindowTextA);
	DetourAttach(&g_pOldCreateWindowExA, NewCreateWindowExA);

	DetourTransactionCommit();
}

void PalFileInfo(char* FileName)
{
	_asm {
		pushad
		pushfd
	}
	cout << "Find File Name:" << FileName << endl;
	_asm {
		popfd
		popad
	}
}

PVOID pGetName = NULL;
PVOID dGetName = NULL;//(PVOID)(0x10021C4D);
__declspec(naked)void GetName()
{
	__asm
	{
		push eax
		mov ecx, dword ptr ss : [ebp + 0xC]
		push ecx
		call PalFileInfo
		jmp dGetName
	}
}


HANDLE  (*PalFileCreate)(char* FileName, void* lpBuffer);
HANDLE  EdenFileCreate(char* FileName, void* lpBuffer)
{
	if (strcmp(FileName, "SYSTEM.INI") == 0)
		strcpy(FileName, "EDEN.INI");
	return PalFileCreate(FileName, lpBuffer);
}

int (*PalSpriteLoadMemory)(int a1, int FileName, int FileBuff, int FileSize, int a5);
int EdenSpriteLoadMemory(int a1, int FileName, int FileBuff, int FileSize, int a5)
{
	string fnm((char*)FileName);
	string Dir = "PGDN\\";
	string OutName = Dir + fnm;
	FILE* f = fopen(OutName.c_str(), "rb");
	if (f != NULL) 
	{
		cout << "------------------File Check info------------------" << endl;
		cout << "Read PGD File:" << (char*)FileName << endl;
		cout << "Find PGD File:0x" << (DWORD)FileBuff << endl;
		cout << "Size PGD File:0x" << (DWORD)FileSize << endl;
		DWORD size = 0;
		char* buff = NULL;
		cout << "Read Out PGD File:" << (char*)FileName << endl;
		fseek(f, 0, SEEK_END);
		size = ftell(f);
		fseek(f, 0, SEEK_SET);
		buff = (char*)malloc(size);
		if (buff != NULL && size != 0) {
			fread(buff, size, 1, f);
			FileBuff = (int)buff;
			FileSize = (int)size;
			cout << "Out PGD File Buff:" << (DWORD)buff << endl;
			cout << "Out PGD File Size:" << (DWORD)size << endl;
			fclose(f);
			buff = NULL;
			size = 0;
			cout << "Read Out PGD File:" << (char*)FileName << " success." << endl;
		}
		else
		{
			cout << "Read Out PGD File:" << (char*)FileName << " failed." << endl;
			cout << "Out PGD File Buff:" << (DWORD)buff << endl;
			cout << "Out PGD File Size:" << (DWORD)size << endl;
			fclose(f);
			buff = NULL;
			size = 0;
		}
				cout << "------------------------END------------------------" << endl;
		goto retn;
	}
retn:
	return PalSpriteLoadMemory(a1, FileName, FileBuff, FileSize, a5);
}

int sig = 0;

void PalFontBypass()
{
	BYTE FontBypass[] = { 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 };
	int BypassAddress1 = PalAddr + 0xECC18;
	int BypassAddress2 = PalAddr + 0xECC2C;
	if (debug)
		cout << "BypassAddress1:0x" << hex << BypassAddress1 << "\n" << "BypassAddress2:0x" << hex << BypassAddress2 << endl;
	memcopy((void*)BypassAddress1, FontBypass, sizeof(FontBypass));
	memcopy((void*)BypassAddress2, FontBypass, sizeof(FontBypass));
}

void PalConf()
{
	if (debug)
	{
		cout << "------------------Pal Check info------------------" << endl;
		cout << "Pal.dll Address:0x00" << hex << (PalAddr) << endl;
	}
	PalFontBypass();
	*(DWORD*)&PalFileCreate = PalAddr + 0x15000;
	*(DWORD*)&PalSpriteLoadMemory = PalAddr + 0x21D60;
	DetourTransactionBegin();
	DetourAttach((void**)&PalFileCreate, EdenFileCreate);
	DetourAttach((void**)&PalSpriteLoadMemory, EdenSpriteLoadMemory);
	DetourTransactionCommit();
	if (debug)
		cout << "------------------------END-----------------------" << endl;
	sig = 1;
}

void ExeCharCheck()
{
	BYTE Patch[] = { 0xFF };
	int CheckPoint1 = BaseAddr + 0x26BF7;
	int CheckPoint2 = BaseAddr + 0x26C9F;
	int CheckPoint3 = BaseAddr + 0x272BF;
	int CheckPoint4 = BaseAddr + 0x27765;
	int CheckPoint5 = BaseAddr + 0x27B03;
	int CheckPoint6 = BaseAddr + 0x26BFF;
	int CheckPoint7 = BaseAddr + 0x26C0A;
	int CheckPoint8 = BaseAddr + 0x26CA9;
	int CheckPoint9 = BaseAddr + 0x26CB5;
	int CheckPointA = BaseAddr + 0x272C7;
	int CheckPointB = BaseAddr + 0x272D6;
	int CheckPointC = BaseAddr + 0x2776D;
	int CheckPointD = BaseAddr + 0x27779;
	int CheckPointE = BaseAddr + 0x27B0D;
	int CheckPointF = BaseAddr + 0x27B19;
	//int CheckPointG = BaseAddr + 0x1df011;
	if (debug)
	{
		cout << "------------------Exe Char Check info------------------" << endl;
		cout << "CheckPoint1:0x" << hex << CheckPoint1 << endl;
		cout << "CheckPoint2:0x" << hex << CheckPoint2 << endl;
		cout << "CheckPoint3:0x" << hex << CheckPoint3 << endl;
		cout << "CheckPoint4:0x" << hex << CheckPoint4 << endl;
		cout << "CheckPoint5:0x" << hex << CheckPoint5 << endl;
		cout << "CheckPoint6:0x" << hex << CheckPoint6 << endl;
		cout << "CheckPoint7:0x" << hex << CheckPoint7 << endl;
		cout << "CheckPoint8:0x" << hex << CheckPoint8 << endl;
		cout << "CheckPoint9:0x" << hex << CheckPoint9 << endl;
		cout << "CheckPointA:0x" << hex << CheckPointA << endl;
		cout << "CheckPointB:0x" << hex << CheckPointB << endl;
		cout << "CheckPointC:0x" << hex << CheckPointC << endl;
		cout << "CheckPointD:0x" << hex << CheckPointD << endl;
		cout << "CheckPointE:0x" << hex << CheckPointE << endl;
		cout << "CheckPointF:0x" << hex << CheckPointF << endl;
		cout << "--------------------------END--------------------------" << endl;
	}
	memcopy((void*)CheckPoint1, Patch, sizeof(Patch));
	memcopy((void*)CheckPoint2, Patch, sizeof(Patch));
	memcopy((void*)CheckPoint3, Patch, sizeof(Patch));
	memcopy((void*)CheckPoint4, Patch, sizeof(Patch));
	memcopy((void*)CheckPoint5, Patch, sizeof(Patch));
	memcopy((void*)CheckPoint6, Patch, sizeof(Patch));
	memcopy((void*)CheckPoint7, Patch, sizeof(Patch));
	memcopy((void*)CheckPoint8, Patch, sizeof(Patch));
	memcopy((void*)CheckPoint9, Patch, sizeof(Patch));
	memcopy((void*)CheckPointA, Patch, sizeof(Patch));
	memcopy((void*)CheckPointB, Patch, sizeof(Patch));
	memcopy((void*)CheckPointC, Patch, sizeof(Patch));
	memcopy((void*)CheckPointD, Patch, sizeof(Patch));
	memcopy((void*)CheckPointE, Patch, sizeof(Patch));
	memcopy((void*)CheckPointF, Patch, sizeof(Patch));
	//memcopy((void*)CheckPoint1, Patch, sizeof(Patch));
}

void InitFileSystem()
{

	FILE* fp = fopen("Exec.aym", "rb");
	fseek(fp, 0, SEEK_END);
	DWORD size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	char* buff = (char*)malloc(size);
	fread(buff, size, 1, fp);
	fclose(fp);
	head_t header;
	memcpy(header.flag, "AymScriptVer1.00", 17);
	if (strcmp(header.flag, "AymScriptVer1.00") != 0)
	{
		MessageBox(0, L"Can't read Exec.aym [0x00000001]", L"Error", 0);
		return;
	}
	buff += 17;
	header.Length = *(DWORD*)buff;
	buff += 4;
	header.Zero = *(DWORD*)buff;
	buff += 4;
	header.DEC_SIZE = *(DWORD*)buff;
	buff += 4;
	header.COMP_SIZE = *(DWORD*)buff;
	buff += 4;
	if (debug) 
	{
		cout << "------------------Exec info------------------" << endl;
		cout <<"Exec.Length:0x" << hex << header.Length << endl;
		cout << "Exec.DEC_SIZE:0x" << hex << header.DEC_SIZE << endl;
		cout << "Exec.COMP_SIZE:0x" << hex << header.COMP_SIZE << endl;
		cout << "---------------------END---------------------" << endl;
	}
	DWORD key = 0x114514;
	unsigned char Key[4] = { 0x26,0x8B,0x7F,0x9C };
	DWORD uncomp_size = header.DEC_SIZE;
	char* uncomp_buff = (char*)malloc(header.DEC_SIZE);
	cryption((BYTE*)buff, header.COMP_SIZE, 0);
	if (uncompress((BYTE*)uncomp_buff, &uncomp_size, (BYTE*)buff, header.COMP_SIZE) == Z_OK)
	{
		if (uncomp_size != header.DEC_SIZE)
		{
			MessageBox(0, L"Can't read Exec.aym [0x00000002]", L"Error", 0);
			return;
		}
		cryption((BYTE*)uncomp_buff, uncomp_size, 1);

		for (int i = 0; i < uncomp_size; i++)
		{
			key += i;
			uncomp_buff[i] -= key;
			uncomp_buff[i] ^= Key[i % 4];
		}
	}
	else
	{
		MessageBox(0, L"Can't read Exec.aym [0x00000003]", L"Error", 0);
		return;
	}
	textbuf = (TextStruct*)uncomp_buff;
	translatetext.setStorage(textbuf);
	translatetext.setTableLength(header.Length);
	Exec = true;
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
		{
			make_console();
		}
		//EdenCharCheck();
		InitFileSystem();
		SavePatch();
		ExeCharCheck();
		GetPalAddr();
		Init();
    case DLL_THREAD_ATTACH:
		if (PalAddr != NULL)
		{
			if (sig==0)
				PalConf();
		}
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
__declspec(dllexport)void WINAPI Dummy()
{
}
