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
	if (!lstrcmpW(lpszFace, L"ＭＳ ゴシック") || !lstrcmpW(lpszFace, L"MS Gothic"))
		return (fnCreateFontW(pCreateFontW))(nHeight, nWidth, nEscapement, nOrientation, fnWeight, fdwItalic, fdwUnderline, fdwStrikeOut, GB2312_CHARSET,
			fdwOutputPrecision, fdwClipPrecision, fdwQuality, fdwPitchAndFamily, L"SimHei");
	return (fnCreateFontW(pCreateFontW))(nHeight, nWidth, nEscapement, nOrientation, fnWeight, fdwItalic, fdwUnderline, fdwStrikeOut, GB2312_CHARSET,
		fdwOutputPrecision, fdwClipPrecision, fdwQuality, fdwPitchAndFamily, lpszFace);
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
	if(wcsstr(lps, L"1.02"))
		return pSetWindowTextW(hw, L"【靴下汉化组】放学后的灰姑娘 - v1.0 [BuildTime: 20220130]");
	return pSetWindowTextW(hw, lps);
}

int WINAPI NewMultiByteToWideChar(UINT cp, DWORD dwFg, LPCSTR lpMBS, int cbMB, LPWSTR lpWCS, int ccWC)
{
	bool is_reliable;
	int bytes_consumed;
	Encoding encoding = CompactEncDet::DetectEncoding(lpMBS, strlen(lpMBS), nullptr, nullptr, nullptr, UNKNOWN_ENCODING, UNKNOWN_LANGUAGE, CompactEncDet::QUERY_CORPUS, true, &bytes_consumed, &is_reliable);
	UINT codepage = cp;
	if (encoding == UTF8 && is_reliable)
		codepage = CP_UTF8;
	/*
	if (cp == 932)
		cp = 936;
	*/
	return ((PfuncMultiByteToWideChar)g_pOldMultiByteToWideChar)(codepage, dwFg, lpMBS, cbMB, lpWCS, ccWC);
}

int WINAPI NewWideCharToMultiByte(UINT cp, DWORD dwFg, LPCWCH lpWCS, int ccwc, LPSTR lpMbs, int ccmb, LPCCH lpdc, LPBOOL lpusdc)
{
	UINT codepage = cp;
	if (codepage == 932)
		codepage = CP_UTF8;
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
	//DetourAttach((void**)&pCreateFontW, newCreateFontW);
	//DetourAttach((void**)& pSetWindowTextW, newSetWindowTextW);
	DetourAttach(&g_pOldMultiByteToWideChar, NewMultiByteToWideChar);
	DetourAttach(&g_pOldWideCharToMultiByte, NewWideCharToMultiByte);

	//*(DWORD*)&sub_4324C0 = BaseAddr + 0x324C0;
	//DetourAttach((void**)&sub_4324C0, Hook_sub_4324C0);

	if (DetourTransactionCommit() != NO_ERROR)
	{
		MessageBoxW(NULL, L"API Hook Error", L"EdenInitHook", MB_OK | MB_ICONERROR);
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

#define DIR L"#Cinderella_cn"

DWORD ImmAlloc = (BaseAddr + 0xCCFCD);
DWORD ImmFree = (BaseAddr + 0xDE384);

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

/*
BOOL E(const wchar_t* what)
{
	MessageBox(0, what, L"AyamiKazeFileSystemError", 0);
	return FALSE;
}

BOOL ReadFileFromDir(CHAR* FileName, PBYTE& OutBuff, ULONG& OutSize)
{
	WCHAR* w_FileName = ctow(FileName, CP_ACP);
	wstring fnm(w_FileName);
	wstring fnm2 = GetPackageNameW(fnm);
	WCHAR FileRoot[MAX_PATH];
	wsprintf(FileRoot, L"%s\\%s", DIR, fnm2.c_str());
	FILE* fp = _wfopen(FileRoot, L"rb");
	if (!fp)
		return FALSE;
	cout << "Read:" << wtoc(fnm2.c_str(), CP_ACP) << endl;
	fseek(fp, 0, SEEK_END);
	OutSize = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	OutBuff = (PBYTE)HostAlloc(OutSize);
	if (OutBuff == nullptr)
		return E(L"Alloc OutBuff Error");
	fread(OutBuff, OutSize, 1, fp);
	fclose(fp);
	return TRUE;
}
*/


CPKFileSystem::CPKFileSystem()
{
}

CPKFileSystem::~CPKFileSystem()
{
	delete[] Key_Header;
	delete[] CPK_Header;

	delete[] CPK_Index;
	delete[] Key_Index;

	fclose(cpk);
	fclose(key);
}

BOOL CPKFileSystem::E(const char* msg)
{
	MessageBoxA(0, msg, "CPKFileSystem Error", MB_OK | MB_ICONERROR);
	return FALSE;
}

BOOL CPKFileSystem::InitFileSystem()
{
	cpk = fopen(PACK_NAME, "rb");
	if (!cpk)
		return E("Can't open package.");
	key = fopen(KEY_NAME, "rb");
	if (!key)
		return E("Can't open key.");

	// Key Info Check
	KeyHeaderBuff = new BYTE[sizeof(KeyHeader)];
	fread(KeyHeaderBuff, sizeof(KeyHeader), 1, key);

	for (int i = 0; i < sizeof(KeyHeader); i++)
		KeyHeaderBuff[i] ^= KEY_TOP_KEY;

	Key_Header = (KeyHeader*)KeyHeaderBuff;
	if (memcmp(Key_Header->Magic, MAGIC, 4))
		return E("Key magic not match");

	// Decrypt CPK Header and Check CPK Info
	CPKHeaderBuff = new BYTE[sizeof(CPKHeader)];
	fread(CPKHeaderBuff, sizeof(CPKHeader), 1, cpk);

	for (int i = 0; i < sizeof(CPKHeader); i++)
		CPKHeaderBuff[i] ^= Key_Header->HeaderHash[i % SHA256::DIGESTSIZE];

	CPK_Header = (CPKHeader*)CPKHeaderBuff;
	if (memcmp(CPK_Header->Magic, MAGIC, 4) ||
		memcmp(CPK_Header->Version, VERSION, 4) ||
		lstrcmpA(CPK_Header->Warning, WARNING))
		return E("CPK magi not match");

	// Read Key Index and CPK Index
	if (CPK_Header->IndexCount != Key_Header->IndexCount)
		return E("Counter not match");
	CPK_Index = new CPKIndex[CPK_Header->IndexCount * sizeof(CPKIndex)];
	Key_Index = new KeyIndex[Key_Header->IndexCount * sizeof(KeyIndex)];
	fread(CPK_Index, CPK_Header->IndexCount * sizeof(CPKIndex), 1, cpk);
	fread(Key_Index, Key_Header->IndexCount * sizeof(KeyIndex), 1, key);

	for (int i = 0; i < CPK_Header->IndexCount; i++)
	{
		// Decrypt Key Index
		BYTE* KeyIndexBuff = new BYTE[sizeof(KeyIndex)];
		memmove(KeyIndexBuff, &Key_Index[i], sizeof(KeyIndex));
		for (int j = 0; j < sizeof(KeyIndex); j++)
			KeyIndexBuff[j] ^= KEY_TOP_KEY;
		memmove(&Key_Index[i], KeyIndexBuff, sizeof(KeyIndex));

		// Decrypt CPK Index
		BYTE* CPKIndexBuff = new BYTE[sizeof(CPKIndex)];
		memmove(CPKIndexBuff, &CPK_Index[i], sizeof(CPKIndex));
		for (int j = 0; j < sizeof(CPKIndex); j++)
			CPKIndexBuff[j] ^= Key_Index[i].IndexHash[j % SHA256::DIGESTSIZE];
		memmove(&CPK_Index[i], CPKIndexBuff, sizeof(CPKIndex));
	}

	return TRUE;
}

BOOL CPKFileSystem::UnInitFileSystem()
{
	fclose(cpk);
	fclose(key);

	delete[] Key_Header;
	delete[] CPK_Header;

	delete[] CPK_Index;
	delete[] Key_Index;

	return TRUE;
}

BOOL CPKFileSystem::GetFileByName(char* FileName, BYTE** FileBuff, DWORD* FileSize)
{
	for (int i = 0; i < CPK_Header->IndexCount; i++)
	{
		if (!lstrcmpA(CPK_Index[i].FileName, FileName))
		{
			//cout << "Read:" << CPK_Index[i].FileName << endl;
			fseek(cpk, sizeof(CPKHeader) + CPK_Header->IndexCount * sizeof(CPKIndex) + CPK_Index[i].Offset, SEEK_SET);
			BYTE* CompressBuff = (BYTE*)HostAlloc(CPK_Index[i].CompressFileSize);
			fread(CompressBuff, CPK_Index[i].CompressFileSize, 1, cpk);

			// Camellia Decrypt
			BYTE* DecryptBuff = (BYTE*)HostAlloc(CPK_Index[i].CompressFileSize);
			try
			{
				EAX<Camellia>::Decryption d;
				d.SetKeyWithIV(Key_Index[i].KeyAfterCompress.Key, Key_Index[i].KeyAfterCompress.KeySize,
					Key_Index[i].KeyAfterCompress.Iv, Key_Index[i].KeyAfterCompress.IvSize);
				d.ProcessData(DecryptBuff, CompressBuff, CPK_Index[i].CompressFileSize);

			}
			catch (const CryptoPP::Exception& e)
			{
				return E(e.what());
			}
			HostFree(CompressBuff);

			// Zlib Decompress
			BYTE* UncompressBuff = (BYTE*)HostAlloc(CPK_Index[i].FileSize);
			ZlibDecompressor dec;
			dec.Put(DecryptBuff, CPK_Index[i].CompressFileSize);
			dec.MessageEnd();
			dec.Get(UncompressBuff, CPK_Index[i].FileSize);
			HostFree(DecryptBuff);

			*FileBuff = UncompressBuff;
			*FileSize = CPK_Index[i].FileSize;

			

			return TRUE;
		}
	}
	return FALSE;
}

CPKFileSystem CPKFS;
BOOL ReadFileFromPackage(CHAR* FileName, PBYTE& OutBuff, ULONG& OutSize)
{
	string fnm(FileName);
	string fnm2 = GetPackageNameA(fnm);
	FileNameToLower(fnm2);
	//cout << fnm2.c_str() << endl;
	BOOL ret = CPKFS.GetFileByName((char*)fnm2.c_str(), &OutBuff, &OutSize);
	//if (ret)
	//	cout << "Find" << endl;
	return ret;
}

int (*sub_470450)(char* OutBuffer, size_t* OutSize, CHAR* FileBuff, unsigned int FileSize, unsigned int a5, unsigned int a6);
int FileProcess(char* OutBuffer, size_t* OutSize, CHAR* FileBuff, unsigned int FileSize, unsigned int a5, unsigned int a6)
{
	int ret = 0;
	INLINE_ASM
	{
		push a6;
		push a5;
		push FileSize;
		push FileBuff;
		mov edx, OutSize;
		mov ecx, OutBuffer;
		call sub_470450;
		add esp, 0x14
		mov ret, eax;
	};
	return ret;
}

int (*sub_470790)(LPCCH FileName, int a4, int a5);
int HookBGIOutReadFile(LPCCH FileName, int a4, int a5)
{
	size_t* OutSize;
	char* OutBuffer;
	INLINE_ASM mov OutSize, edx;
	INLINE_ASM mov OutBuffer, ecx;

	PBYTE Buff = nullptr;
	ULONG Size;
	//cout << FileName << endl;
	if (strstr(FileName, "BGI.gdb") != NULL)
		goto back;

	if (ReadFileFromPackage((CHAR*)FileName, Buff, Size))
	{
		LONG result = FileProcess(OutBuffer, OutSize, (char*)Buff, Size, 0, 0);
		//cout << "over" << endl;
		if (Buff)
			HostFree(Buff);
		return result;
	}
back:
	INLINE_ASM mov edx, OutSize;
	INLINE_ASM mov ecx, OutBuffer;
	return sub_470790(FileName, a4, a5);
}

void SetNativeFileSystem()
{

	if (!CPKFS.InitFileSystem())
	{
		MessageBoxW(0,L"Can't init package.",
			L"InitPackData",
			MB_OK | MB_ICONERROR);
		ExitProcess(-1);
	}

	*(DWORD*)&sub_470450 = BaseAddr + 0x71C40;
	*(DWORD*)&sub_470790 = BaseAddr + 0x71F80;
	DetourTransactionBegin();
	DetourAttach((void**)&sub_470790, HookBGIOutReadFile);
	if (DetourTransactionCommit() != NOERROR)
	{
		MessageBoxW(0, L"Init filesystem error.", L"SetNativeFileSystem", MB_OK | MB_ICONERROR);
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
		//make_console();
		//EdenInitHook();
		SetNativeFileSystem();
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

__declspec(dllexport)void WINAPI Kuzushita()
{
}