// dllmain.cpp : 定义 DLL 应用程序的入口点。
/*
Ever tried. Ever failed.
No matter. Try again.
Fail again. Fail better.
*/
#include "pch.h"
HINSTANCE hInst;

typedef LCID(WINAPI* fnGetUserDefaultLCID)(void);
typedef LANGID(WINAPI* fnGetSystemDefaultLangID)(void);
typedef LANGID(WINAPI* fnGetSystemDefaultUILanguage)(void);
typedef bool (WINAPI* fnSetWindowTextA)(HWND hWnd, LPCSTR lpString);
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
typedef int (WINAPI* fnMessageboxA)(
	_In_opt_ HWND    hWnd,
	_In_opt_ LPCSTR lpText,
	_In_opt_ LPCSTR lpCaption,
	_In_     UINT    uType
	);
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
fnMessageboxA MessageBoxAOLD;
fnCreateFontA pCreateFontA;
fnGetSystemDefaultLangID pGetSystemDefaultLangID;
fnGetSystemDefaultUILanguage pGetSystemDefaultUILanguage;
fnGetUserDefaultLCID pGetUserDefaultLCID;
fnSetWindowTextA pSetWindowTextA;
fnEnumFontFamiliesExA pEnumFontFamiliesExA;
PVOID g_pOldMultiByteToWideChar = NULL;
typedef int(WINAPI* PfuncMultiByteToWideChar)(
	_In_      UINT   CodePage,
	_In_      DWORD  dwFlags,
	_In_      LPCSTR lpMultiByteStr,
	_In_      int    cbMultiByte,
	_Out_opt_ LPWSTR lpWideCharStr,
	_In_      int    cchWideChar);
wchar_t* ctow932(const char* str)
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

ULONG a2u(
	LPCSTR lpAnsi,
	ULONG  Length,
	LPWSTR lpUnicodeBuffer,
	ULONG  BufferCount,
	ULONG  CodePage)
{
	return MultiByteToWideChar(CodePage, 0, lpAnsi, Length, lpUnicodeBuffer, BufferCount);
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
	fdwCharSet = 0x86;
	fnWeight = FW_BOLD;
	fdwQuality = ANTIALIASED_QUALITY;
	return CreateFontW(nHeight, nWidth, nEscapement, nOrientation, fnWeight, 
	fdwItalic, fdwUnderline, fdwStrikeOut, fdwCharSet, fdwOutputPrecision, 
	fdwClipPrecision, fdwQuality, fdwPitchAndFamily, L"SimHei");
}

LCID WINAPI newGetUserDefaultLCID()
{
	return 0x411;
}

LANGID WINAPI newGetSystemDefaultLangID()
{
	return 0x411;
}

LANGID WINAPI newGetSystemDefaultUILanguage()
{
	return 0x411;
}

bool WINAPI newSetWindowTextA(HWND hw, LPCSTR lps)
{
	wchar_t newtitle[] = L"我成为天使的理由";
	if(!strcmp(lps,"杔偑揤巊偵側偭偨棟桼"))
		return SetWindowTextW(hw, newtitle);
	return ((fnSetWindowTextA)pSetWindowTextA)(hw, lps);
}

int WINAPI newEnumFontFamiliesExA(HDC hdc, LPLOGFONTA lpLogfont, FONTENUMPROCA lpProc, LPARAM lParam, DWORD dwFlags)
{
	LOGFONTA Lfont;
	memcpy(&Lfont, lpLogfont, sizeof(LOGFONTA));
	Lfont.lfCharSet = GB2312_CHARSET;
	memcpy(lpLogfont, &Lfont, sizeof(LOGFONTA));
	return ((fnEnumFontFamiliesExA)pEnumFontFamiliesExA)(hdc, lpLogfont, lpProc, lParam, dwFlags);
}

int WINAPI NewMultiByteToWideChar(UINT cp, DWORD dwFg, LPCSTR lpMBS, int cbMB, LPWSTR lpWCS, int ccWC)
{
	return ((PfuncMultiByteToWideChar)g_pOldMultiByteToWideChar)(936, dwFg, lpMBS, cbMB, lpWCS, ccWC);
}

int WINAPI MessageBoxAEx(_In_opt_ HWND    hWnd,
	_In_opt_ LPCSTR lpText,
	_In_opt_ LPCSTR lpCaption,
	_In_     UINT    uType)
{
	char* Pstr = wtocGBK(ctowJIS((char*)lpText));
	char* Pstr2 = wtocGBK(ctowJIS((char*)lpCaption));
	return MessageBoxAOLD(hWnd, Pstr, Pstr2, uType);
}


void EdenInitHook()
{
	pCreateFontA = (fnCreateFontA)GetProcAddress(GetModuleHandle(L"gdi32.dll"), "CreateFontA");
	pEnumFontFamiliesExA = (fnEnumFontFamiliesExA)GetProcAddress(GetModuleHandle(L"gdi32.dll"), "EnumFontFamiliesExA");
	pGetSystemDefaultLangID = (fnGetSystemDefaultLangID)GetProcAddress(GetModuleHandle(L"Kernel32.dll"), "GetSystemDefaultLangID");
	pGetSystemDefaultUILanguage = (fnGetSystemDefaultUILanguage)GetProcAddress(GetModuleHandle(L"Kernel32.dll"), "GetSystemDefaultUILanguage");
	pGetUserDefaultLCID = (fnGetUserDefaultLCID)GetProcAddress(GetModuleHandle(L"Kernel32.dll"), "GetUserDefaultLCID");
	pSetWindowTextA = (fnSetWindowTextA)GetProcAddress(GetModuleHandle(L"User32.dll"), "SetWindowTextA");
	g_pOldMultiByteToWideChar = DetourFindFunction("Kernel32.dll", "MultiByteToWideChar");
	MessageBoxAOLD = (fnMessageboxA)GetProcAddress(GetModuleHandle(L"User32.dll"), "MessageBoxA");
	DetourTransactionBegin();
	DetourAttach((void**)& pCreateFontA, newCreateFontA);
	DetourAttach((void**)& pGetSystemDefaultLangID, newGetSystemDefaultLangID);
	DetourAttach((void**)& pGetSystemDefaultUILanguage, newGetSystemDefaultUILanguage);
	DetourAttach((void**)& pGetUserDefaultLCID, newGetUserDefaultLCID);
	DetourAttach((void**)& pSetWindowTextA, newSetWindowTextA);
	//DetourAttach((void**)& pEnumFontFamiliesExA, newEnumFontFamiliesExA);
	DetourAttach(&g_pOldMultiByteToWideChar, NewMultiByteToWideChar);
	DetourAttach((void**)& MessageBoxAOLD, MessageBoxAEx);
	if (DetourTransactionCommit() != NO_ERROR)
	{
		MessageBoxW(NULL, L"HOOK ERROR!!!", L"AymKze", MB_OK | MB_ICONWARNING);
	}
}
/**********************************************************************************************************************/


//////////////////////////////////FileSystem///////////////////////////////////

#include "xxhash.h"
#ifndef ASM
#define ASM __declspec(naked)
#endif /* ASM */

#ifndef INLINE_ASM
#define INLINE_ASM __asm
#endif

#define LOOP_ONCE   for (Bool __condition_ = True; __condition_; __condition_ = False)

#define DIR L"bokuten_chs\\"

DWORD ImmAlloc = (BaseAddr + 0x8D6DF);
DWORD ImmFree  = (BaseAddr + 0x937C0);

ASM PVOID CDECL HostAlloc(LONG Size)
{
	INLINE_ASM
	{
		push dword ptr[esp + 4]
		call ImmAlloc
		pop ecx
		retn
	}
}


ASM VOID CDECL HostFree(PVOID lpMem)
{
	INLINE_ASM
	{
		push dword ptr[esp + 4]
		call ImmFree
		pop ecx
		retn
	}
}

wstring WINAPI GetPackageName(wstring& fileName)
{
	wstring temp(fileName);
	wstring::size_type pos = temp.find_last_of(L"\\");

	if (pos != wstring::npos)
	{
		temp = temp.substr(pos + 1, temp.length());
	}

	wstring temp2(temp);
	wstring::size_type pos2 = temp2.find_last_of(L"/");
	if (pos2 != wstring::npos)
	{
		temp2 = temp2.substr(pos + 1, temp2.length());
	}
	return temp2;
}
BOOL Err(const wchar_t* what)
{
	MessageBox(0, what, L"BGI Info", 0);
	return FALSE;
}

#include "WinFile.h"
BOOL WINAPI AyamiKazeReadFileFromDir(LPCSTR lpFileName, PBYTE& Buffer, ULONG& OutSize)
{
	WCHAR   szFileName[MAX_PATH] = { 0 };
	a2u(lpFileName, lstrlenA(lpFileName), szFileName, MAX_PATH, 932);
	wstring fnm(szFileName);
	wstring FileName = GetPackageName(fnm);

	wstring Path = DIR + FileName;
	cout << "Reading file from:" << wtocGBK(Path.c_str()) << endl;
	WinFile File;
	if (File.Open(Path.c_str(), WinFile::FileRead) == S_FALSE)
	{
		//cout << "Can't find file:" << wtocGBK(Path.c_str()) << ".Read file from BGI filesystem." << endl;
		File.Release();
		return FALSE;
	}
	cout << Path.c_str() << endl;
	OutSize = File.GetSize32();
	Buffer = (PBYTE)HostAlloc(OutSize);
	if (Buffer == nullptr)
	{
		File.Release();
		wchar_t what[MAX_PATH];
		wsprintf(what, FileName.c_str(), L":Alloc memory error!");
		return Err(what);
	}
	File.Read(Buffer, OutSize);
	File.Release();
	cout << "Success to read file from:" << wtocGBK(Path.c_str()) << endl;
	return TRUE;
}

std::string GetFileName(CHAR* Name)
{
	std::string Info(Name);
	if (Info.find_last_of("\\") != std::string::npos)
		Info = Info.substr(Info.find_last_of("\\") + 1, std::string::npos);

	return Info;
}

class VFSFileSytem
{
public:
	VFSFileSytem();
	~VFSFileSytem();
	BOOL InitFileSystem();
	BOOL GetFileByName(CHAR* fnm, BYTE** FileBuff, DWORD* FileSize);
	BOOL IsFileInPack(CHAR* fnm);
private:
	struct entry {
		CHAR FileName[64];
		DWORD Size;
		DWORD Offset;
	};
	DWORD FileCount;
	entry* index;
	FILE* fp;
};
VFSFileSytem::VFSFileSytem()
{

}
VFSFileSytem::~VFSFileSytem()
{
	HostFree(index);
	fclose(fp);
}
BOOL VFSFileSytem::InitFileSystem()
{
	fp = fopen("data.bin", "rb");
	if (!fp)
		return FALSE;
	FileCount = 0;
	fread(&FileCount, 4, 1, fp);
	index = new entry[FileCount * sizeof(entry)];
	fread(index, FileCount * sizeof(entry), 1, fp);
	return TRUE;
}


BOOL VFSFileSytem::GetFileByName(CHAR* fnm, BYTE** FileBuff, DWORD* FileSize)
{
	string lowfnm = string(fnm);
	FileNameToLower(lowfnm);
	for (int i = 0; i < FileCount; i++)
	{
		if (!strcmp(index[i].FileName, lowfnm.c_str()))
		{
			fseek(fp, 4 + FileCount * sizeof(entry) + index[i].Offset, SEEK_SET);
			cout << "-----------------------------------------------------" << endl;
			cout << lowfnm << endl;
			cout << hex << index[i].Offset << endl;
			cout << hex << index[i].Size << endl;
			cout << "-----------------------------------------------------" << endl;
			BYTE* OutBuff = (BYTE*)HostAlloc(index[i].Size);
			fread(OutBuff, index[i].Size, 1, fp);
			*FileBuff = OutBuff;
			*FileSize = index[i].Size;
			return TRUE;
		}
	}
	return FALSE;
}
BOOL VFSFileSytem::IsFileInPack(CHAR* fnm)
{
	string lowfnm = string(fnm);
	FileNameToLower(lowfnm);
	CHAR* c_fnm = (CHAR*)lowfnm.c_str();
	string s_fnm = GetFileName(c_fnm);
	cout << s_fnm << endl;
	for (int i = 0; i < FileCount; i++)
	{
		if (!strcmp(index[i].FileName, s_fnm.c_str()))
		{
			//cout << "patch:" << lowfnm << endl;
			return TRUE;
		}
	}
	return FALSE;
}
VFSFileSytem VFS;


auto pfnGetFileAttributesA = GetFileAttributesA;
DWORD WINAPI HookGetFileAttributesA(
	LPCSTR lpFileName
)
{
	if (VFS.IsFileInPack((CHAR*)lpFileName))
		return FILE_ATTRIBUTE_ARCHIVE;
	return pfnGetFileAttributesA(lpFileName);
}

int(__cdecl* sub_44F340)(char* a1, unsigned int* pFileName, BYTE* a3, unsigned int a4, __int64 a5);
int(__cdecl* sub_44F5F0)(char* a1, unsigned int* pFileName, LPCSTR lpFileName, __int64 a4);
int __cdecl HookFilePos1(char* a1, unsigned int* pFileName, LPCSTR lpFileName, __int64 a4)
{
	int retValue = 5;
	BYTE* InBuffer = NULL;
	DWORD InSize = 0;
	DWORD FileNameAddr = *(DWORD*)(pFileName);
	CHAR* FNM = (CHAR*)FileNameAddr;
	//cout << FNM << endl;
	if (strstr(FNM, ".sud") || strstr(FNM,".arc"))
		return sub_44F5F0(a1, pFileName, lpFileName, a4);
	if (VFS.GetFileByName(FNM, &InBuffer, &InSize))
	//if(AyamiKazeReadFileFromDir(FNM, InBuffer, InSize))
	{
		retValue = sub_44F340(a1, pFileName, InBuffer, InSize, a4);
		if (InBuffer)
			HostFree(InBuffer);
		return retValue;
	}
	else
	{
		return sub_44F5F0(a1, pFileName, lpFileName, a4);
	}
}

BOOL(__cdecl* sub_425CD0)(unsigned __int8 a1);
BOOL __cdecl CharCheck1(unsigned __int8 a1)
{
	int result = 0;
	if (a1 >= 0x81u)
	{
		result = 1;
	}
	else
	{
		result = 0;
	}
	return result;
}

BOOL(__cdecl* sub_477CC0)(unsigned __int8 a1);
BOOL __cdecl CharCheck2(unsigned __int8 a1)
{
	return a1 >= 0x81u && a1 < 0xFFu;
}
void CharCheckHookSet()
{
	*(DWORD*)&sub_425CD0 = BaseAddr + 0x25CD0;
	*(DWORD*)&sub_477CC0 = BaseAddr + 0x77CC0;
	DetourTransactionBegin();
	DetourAttach((void**)&sub_425CD0, CharCheck1);
	DetourAttach((void**)&sub_477CC0, CharCheck2);
}

int(__fastcall* sub_406330)(DWORD* pThis, void* NullPtr, CHAR* a2, CHAR* a3);
int __fastcall  HookSize(DWORD* pThis, void* NullPtr, CHAR* a2, CHAR* a3)
{
	BYTE* InBuffer = NULL;
	DWORD InSize = 0;
	if (VFS.GetFileByName(a3, &InBuffer, &InSize))
	{
		HostFree(InBuffer);
		return InSize;
	}
	return sub_406330(pThis, NullPtr, a2, a3);
}

int(__fastcall* sub_4062A0)(PVOID pThis, void* NullPtr, PVOID a1, CHAR* a2, CHAR* a3);
int __fastcall  HookFile(PVOID pThis, void* NullPtr, PVOID a1, CHAR* a2, CHAR* a3)
{
	BYTE* InBuffer = NULL;
	DWORD InSize = 0;
	if (VFS.GetFileByName(a3, &InBuffer, &InSize))
	{
		//a1 = std::move(InBuffer);
		memcpy(a1, InBuffer, InSize);
		return InSize;
	}
	return sub_4062A0(pThis, NullPtr, a1, a2, a3);
}

void FileSystemInit()
{
	if (!VFS.InitFileSystem())
	{
		MessageBoxW(0, L"Can't init package.",
			L"InitPackData",
			MB_OK | MB_ICONERROR);
		ExitProcess(-1);
	}

	*(DWORD*)&sub_44F5F0 = BaseAddr + 0x4F5F0;
	*(DWORD*)&sub_44F340 = BaseAddr + 0x4F340;
	*(DWORD*)&sub_406330 = BaseAddr + 0x6330;
	*(DWORD*)&sub_4062A0 = BaseAddr + 0x62A0;
	cout << "Hooking function:0x" << hex << sub_44F5F0 << endl;
	cout << "Hooking function:0x" << hex << sub_44F340 << endl;
	DetourTransactionBegin();
	//DetourAttach((void**)&sub_44F5F0, HookFilePos1);
	DetourAttach((void**)&pfnGetFileAttributesA, HookGetFileAttributesA);
	DetourAttach((void**)&sub_406330, HookSize);
	DetourAttach((void**)&sub_4062A0, HookFile);

	if (DetourTransactionCommit() != NOERROR)
		MessageBox(0, L"Init filesystem error.", L"InitFileSystem", 0);
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
		//CharCheckHookSet();
		EdenInitHook();
		//FileSystemInit();
		break;
    case DLL_THREAD_ATTACH:break;
    case DLL_THREAD_DETACH:break;
    case DLL_PROCESS_DETACH:break;
		//FileClose();
        break;
    }
    return TRUE;
}
__declspec(dllexport)void WINAPI proc()
{
}
