// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "framework.h"

typedef bool (WINAPI* fnSetWindowTextW)(HWND hWnd, LPCWSTR lpString);

PVOID g_pOldMultiByteToWideChar = NULL;
typedef int(WINAPI* PfuncMultiByteToWideChar)(
	_In_      UINT   CodePage,
	_In_      DWORD  dwFlags,
	_In_      LPCSTR lpMultiByteStr,
	_In_      int    cbMultiByte,
	_Out_opt_ LPWSTR lpWideCharStr,
	_In_      int    cchWideChar);
PVOID g_pOldWideCharToMultiByte = NULL;
typedef int(WINAPI* PfuncWideCharToMultiByte)(
	UINT                               CodePage,
	DWORD                              dwFlags,
	_In_NLS_string_(cchWideChar)LPCWCH lpWideCharStr,
	int                                cchWideChar,
	LPSTR                              lpMultiByteStr,
	int                                cbMultiByte,
	LPCCH                              lpDefaultChar,
	LPBOOL                             lpUsedDefaultChar);
typedef HFONT(WINAPI* fnCreateFontW)(_In_ int     nHeight,
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
	_In_ LPCWSTR lpszFace);
fnCreateFontW pCreateFontW;
fnSetWindowTextW pSetWindowTextW;

LPWSTR ctow(char* str, UINT cp)
{
	DWORD dwMinSize;
	dwMinSize = MultiByteToWideChar(cp, 0, str, -1, NULL, 0); //计算长度
	LPWSTR out = new wchar_t[dwMinSize];
	MultiByteToWideChar(cp, 0, str, -1, out, dwMinSize);//转换
	return out;
}

char* wtoc(LPCTSTR str, UINT cp)
{
	DWORD dwMinSize;
	dwMinSize = WideCharToMultiByte(cp, NULL, str, -1, NULL, 0, NULL, FALSE); //计算长度
	char* out = new char[dwMinSize];
	WideCharToMultiByte(cp, NULL, str, -1, out, dwMinSize, NULL, FALSE);//转换
	return out;
}

HFONT WINAPI newCreateFontW(_In_ int     nHeight,
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
	_In_ LPCWSTR lpszFace)
{
	return (fnCreateFontW(pCreateFontW))(nHeight, nWidth, nEscapement, nOrientation, FW_BOLD, fdwItalic, fdwUnderline, fdwStrikeOut, GB2312_CHARSET,
		fdwOutputPrecision, fdwClipPrecision, ANTIALIASED_QUALITY, fdwPitchAndFamily, L"SimHei");
}

CHAR OldTitle[0x24] = { 0x3E, 0x65, 0xB2, 0x8A, 0x8C, 0x5F, 0xB7, 0x30, 0xF3, 0x30, 0xC7, 0x30, 0xEC,
						0x30, 0xE9, 0x30, 0x20, 0x00, 0x20, 0x00, 0x56, 0x00, 0x65, 0x00, 0x72, 0x00,
						0x31, 0x00, 0x2E, 0x00, 0x30, 0x00, 0x32, 0x00, 0x00, 0x00
};
CHAR OldTitle2[0x20] = { 0x3E, 0x65, 0x66, 0x5B, 0x0E, 0x54, 0x84, 0x76, 0x70, 0x70, 0xD1, 0x59, 0x18, 0x5A, 0x20, 0x00,
						 0x56, 0x00, 0x65, 0x00, 0x72, 0x00, 0x31, 0x00, 0x2E, 0x00, 0x30, 0x00, 0x32, 0x00, 0x00, 0x00
};
bool WINAPI newSetWindowTextW(HWND hw, LPCWSTR lps)
{
	//if(!memcmp(lps, OldTitle, 0x24) || !memcmp(lps, OldTitle2, 0x20))
	if(wcsstr(lps, L"Bokuten"))
		return pSetWindowTextW(hw, L"我成为天使的理由");
	return pSetWindowTextW(hw, lps);
}

int WINAPI NewMultiByteToWideChar(UINT cp, DWORD dwFg, LPCSTR lpMBS, int cbMB, LPWSTR lpWCS, int ccWC)
{
	//bool is_reliable;
	//int bytes_consumed;
	//Encoding encoding = CompactEncDet::DetectEncoding(lpMBS, strlen(lpMBS), nullptr, nullptr, nullptr, UNKNOWN_ENCODING, UNKNOWN_LANGUAGE, CompactEncDet::QUERY_CORPUS, true, &bytes_consumed, &is_reliable);
	//UINT codepage = 936;
	//if (encoding == CHINESE_CNS && is_reliable)
	//	codepage = 936;
	/*
	if (cp == 932)
		cp = 936;
	*/
	if (cp == UTF8)
		cp = UTF8;
	else
		cp = 936;
	return ((PfuncMultiByteToWideChar)g_pOldMultiByteToWideChar)(cp, dwFg, lpMBS, cbMB, lpWCS, ccWC);
}

int WINAPI NewWideCharToMultiByte(UINT cp, DWORD dwFg, LPCWCH lpWCS, int ccwc, LPSTR lpMbs, int ccmb, LPCCH lpdc, LPBOOL lpusdc)
{
	UINT codepage = cp;
	if (codepage == 932)
		codepage = 936;
	return((PfuncWideCharToMultiByte)g_pOldWideCharToMultiByte)(codepage, dwFg, lpWCS, ccwc, lpMbs, ccmb, lpdc, lpusdc);
}

LPARAM(__fastcall* sub_4324C0)(LPARAM a1, BYTE CharSet);
LPARAM __fastcall Hook_sub_4324C0(LPARAM a1, BYTE CharSet)
{
	return sub_4324C0(a1, 0x86);
}


void EdenInitHook()
{
	pCreateFontW = (fnCreateFontW)GetProcAddress(GetModuleHandle(L"gdi32.dll"), "CreateFontW");
	pSetWindowTextW = (fnSetWindowTextW)GetProcAddress(GetModuleHandle(L"User32.dll"), "SetWindowTextW");

	g_pOldMultiByteToWideChar = DetourFindFunction("Kernel32.dll", "MultiByteToWideChar");
	g_pOldWideCharToMultiByte = DetourFindFunction("Kernel32.dll", "WideCharToMultiByte");


	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach((void**)&pCreateFontW, newCreateFontW);
	DetourAttach((void**)& pSetWindowTextW, newSetWindowTextW);
	DetourAttach(&g_pOldMultiByteToWideChar, NewMultiByteToWideChar);
	DetourAttach(&g_pOldWideCharToMultiByte, NewWideCharToMultiByte);

	*(DWORD*)&sub_4324C0 = BaseAddr + 0x324C0;
	//DetourAttach((void**)&sub_4324C0, Hook_sub_4324C0);

	if (DetourTransactionCommit() != NO_ERROR)
	{
		MessageBoxW(NULL, L"API Hook Error", L"InitHook", MB_OK | MB_ICONERROR);
		ExitProcess(-1);
	}
}

#ifndef ASM
#define ASM __declspec(naked)
#endif /* ASM */

#ifndef INLINE_ASM
#define INLINE_ASM __asm
#endif

#define LOOP_ONCE   for (Bool __condition_ = True; __condition_; __condition_ = False)

#define DIR L"bokuten_chs\\"

DWORD ImmAlloc = (BaseAddr + 0xC7C8D);
DWORD ImmFree = (BaseAddr + 0xD9014);

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


std::string GetFileName(CHAR* Name)
{
	std::string Info(Name);
	if (Info.find_last_of("\\") != std::string::npos)
		Info = Info.substr(Info.find_last_of("\\") + 1, std::string::npos);

	return std::move(Info);
}

class VFSFileSytem
{
public:
	VFSFileSytem();
	~VFSFileSytem();
	BOOL InitFileSystem();
	BOOL GetFileByName(CHAR* fnm, __m128i** FileBuff, DWORD* FileSize);
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
	delete[] index;
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


BOOL VFSFileSytem::GetFileByName(CHAR* fnm, __m128i** FileBuff, DWORD* FileSize)
{
	string lowfnm = string(fnm);
	FileNameToLower(lowfnm);
	for (int i = 0; i < FileCount; i++)
	{
		if (!strcmp(index[i].FileName, lowfnm.c_str()))
		{
			fseek(fp, 4 + FileCount * sizeof(entry) + index[i].Offset, SEEK_SET);
			//cout << "-----------------------------------------------------" << endl;
			//cout << lowfnm << endl;
			//cout << hex << index[i].Offset << endl;
			//cout << hex << index[i].Size << endl;
			//cout << "-----------------------------------------------------" << endl;
			__m128i* OutBuff = (__m128i*)HostAlloc(index[i].Size);
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

auto pfnGetFileAttributesW = GetFileAttributesW;
DWORD WINAPI HookGetFileAttributesW(
	LPCWSTR  lpFileName
)
{
	if (VFS.IsFileInPack(wtoc(lpFileName,936)))
		return FILE_ATTRIBUTE_ARCHIVE;
	return pfnGetFileAttributesW(lpFileName);
}

//int(__fastcall* sub_4077B0)(DWORD* pThis, void* NullPtr, CHAR* a2, CHAR* a3);
//int __fastcall  HookSize(DWORD* pThis, void* NullPtr, CHAR* a2, CHAR* a3)
//{
//	BYTE* InBuffer = NULL;
//	DWORD InSize = 0;
//	if (VFS.GetFileByName(a3, &InBuffer, &InSize))
//	{
//		HostFree(InBuffer);
//		return InSize;
//	}
//	return sub_4077B0(pThis, NullPtr, a2, a3);
//}
//
//int(__fastcall* sub_407710)(PVOID pThis, void* NullPtr, PVOID a1, CHAR* a2, CHAR* a3);
//int __fastcall  HookFile(PVOID pThis, void* NullPtr, PVOID a1, CHAR* a2, CHAR* a3)
//{
//	BYTE* InBuffer = NULL;
//	DWORD InSize = 0;
//	if (VFS.GetFileByName(a3, &InBuffer, &InSize))
//	{
//		memcpy(a1, InBuffer, InSize);
//		return InSize;
//	}
//	return sub_407710(pThis, NullPtr, a1, a2, a3);
//}

int(__cdecl* sub_46ED40)(__m128i* a1, size_t* a2, __m128i* a3, unsigned int a4, int a5, int a6);
int __cdecl FileProcess(__m128i* a1, size_t* a2, __m128i* a3, unsigned int a4, int a5, int a6)
{
	int ret = 0;
	_asm
	{
		push a6;
		push a5;
		push a4;
		push a3;
		mov edx, a2;
		mov ecx, a1;
		call sub_46ED40;
		add esp, 0x10;
		mov ret, eax;
	}
	return ret;
}
int(__cdecl* sub_46EFD0)(LPCCH lpMultiByteStr, int a4, int a5);
int __cdecl HookOutFile(LPCCH lpMultiByteStr, int a4, int a5)
{
	__m128i* OutBuffer = 0;
	size_t* OutSize = 0;
	_asm mov OutSize, edx;
	_asm mov OutBuffer, ecx;

	int retValue = 5;
	__m128i* InBuffer = nullptr;
	ULONG InSize = 0;
	string s_fnm = GetFileName((CHAR*)lpMultiByteStr);
	char* fnm = (char*)s_fnm.c_str();
	if (VFS.GetFileByName(fnm, &InBuffer, &InSize))
	{
		cout << "----------------------------------------------" << endl;
		cout << fnm << endl;
		retValue = FileProcess(OutBuffer, OutSize, InBuffer, InSize, 0, 0);
		cout << "retValue:0x" << hex << retValue << endl;
		cout << "----------------------------------------------" << endl;
		if (InBuffer)
			HostFree(InBuffer);
		return retValue;
	}
	else
	{
		_asm mov edx, OutSize;
		_asm mov ecx, OutBuffer;
		return sub_46EFD0(lpMultiByteStr, a4, a5);
	}
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

	//*(DWORD*)&sub_4077B0 = BaseAddr + 0x77B0;
	//*(DWORD*)&sub_407710 = BaseAddr + 0x7710;
	*(DWORD*)&sub_46EFD0 = BaseAddr + 0x6EFD0;
	*(DWORD*)&sub_46ED40 = BaseAddr + 0x6ED40;
	DetourTransactionBegin();
	DetourAttach((void**)&pfnGetFileAttributesW, HookGetFileAttributesW);
	//DetourAttach((void**)&sub_4077B0, HookSize);
	//DetourAttach((void**)&sub_407710, HookFile);
	DetourAttach((void**)&sub_46EFD0, HookOutFile);

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
		//make_console();
		EdenInitHook();
		FileSystemInit();
		//SetNativeFileSystem();
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

__declspec(dllexport)void WINAPI proc()
{
}