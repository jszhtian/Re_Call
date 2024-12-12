// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "framework.h"
#include <iostream>
#include <mutex>
#include <string>
#include "detours.h"
#pragma comment(lib, "detours.lib")
using namespace std;
#pragma warning(disable:4311)
#pragma warning(disable:4312)  
#pragma warning(disable:4996) 
typedef LSTATUS(WINAPI* Func_RegOpenKeyExA)(HKEY   hKey, LPCSTR lpSubKey, DWORD  ulOptions, REGSAM samDesired, PHKEY  phkResult);
PVOID p_RegOpenKeyEx = RegOpenKeyExA;
typedef bool (WINAPI* Func_SetWindowTextA)(HWND hWnd, LPCSTR lpString);
PVOID p_SetWindowTextA = SetWindowTextA;
typedef HFONT(WINAPI* Func_CreateFontIndirectA)(LOGFONTA* lplf);
PVOID p_CreateFontIndirectA = CreateFontIndirectA;
typedef BYTE* (__cdecl* Func_ReadCompFile)(char* FileName, DWORD* origlength);
PVOID p_ReadCompFile;
typedef int(__cdecl* Func_Decrypt_Uncomp)(unsigned char* Uncompress_OutBuf, unsigned char* crypted_data, unsigned int compress_size, int preKey, int* Origlength);
PVOID p_Decrypt_Uncomp;
typedef HFONT(WINAPI* Func_CreateFontA)(_In_ int     nHeight,
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
	_In_ LPCSTR lpszFace);
PVOID p_CreateFontA = CreateFontA;
LSTATUS WINAPI NewRegOpenKeyExA(HKEY hk, LPCSTR subkey, DWORD ulO, REGSAM sam, PHKEY phk)
{
	wchar_t buffer[256];
	if (subkey)
	{
		size_t nu = strlen(subkey);
		size_t n = (size_t)MultiByteToWideChar(932, 0, subkey, int(nu), NULL, 0);
		memset(buffer, 0, sizeof(wchar_t) * 256);
		MultiByteToWideChar(932, 0, subkey, int(nu), buffer, int(n));
	}
	return RegOpenKeyExW(hk, buffer, ulO, sam, phk);
}

HFONT WINAPI NewCreateFontIndirectA(LOGFONTA* lplf)
{
	LOGFONTA lf;
	memcpy(&lf, lplf, sizeof(LOGFONTA));
	strcpy(lf.lfFaceName, "微软雅黑");
	lf.lfCharSet = GB2312_CHARSET;
	return (Func_CreateFontIndirectA(p_CreateFontIndirectA))(&lf);
}

bool WINAPI NewSetWindowTextA(HWND hw, LPCSTR lps)
{
	wchar_t buffer[256];
	if (lps)
	{
		size_t nu = strlen(lps);
		size_t n = (size_t)MultiByteToWideChar(932, 0, lps, int(nu), NULL, 0);
		memset(buffer, 0, sizeof(wchar_t) * 256);
		MultiByteToWideChar(932, 0, lps, int(nu), buffer, int(n));
	}

	return SetWindowTextW(hw, buffer);

}

void memcopy(void* dest, void* src, size_t size)
{
	DWORD oldProtect;
	VirtualProtect(dest, size, PAGE_EXECUTE_READWRITE, &oldProtect);
	memcpy(dest, src, size);
	DWORD oldProtect2;
	VirtualProtect(dest, size, oldProtect, &oldProtect2);
}

/*
 byte *__cdecl sub_405940(LPCSTR LoadName, DWORD *OrigLength)
{
  const CHAR *FileName; // esi@2
  byte *fileBuf; // eax@3
  char *v4; // eax@4
  int v5; // ebp@9
  const CHAR *v6; // ebx@10
  CHAR String2; // [sp+10h] [bp-104h]@26

  if ( !sub_415390() )
	goto LABEL_27;
  FileName = LoadName;
  if ( strchr(LoadName, 58) )
  {
	fileBuf = LoadFileFromPAK(&Default, LoadName, OrigLength);
	if ( fileBuf )
	  return fileBuf;
	v4 = strrchr(LoadName, 92);
	if ( v4 || (v4 = strrchr(LoadName, 58)) != 0 )
	  FileName = v4 + 1;
  }
  if ( !Exe_Path || (fileBuf = LoadFileFromPAK(&Exe_Path, FileName, OrigLength)) == 0 )
  {
	v5 = 0;
	if ( dword_D0A7A8 > 0 )
	{
	  v6 = &byte_8EEA50;
	  do
	  {
		fileBuf = LoadFileFromPAK(v6, FileName, OrigLength);
		if ( fileBuf )
		  return fileBuf;
		++v5;
		v6 += 260;
	  }
	  while ( v5 < dword_D0A7A8 );
	}
	if ( Data )
	{
	  fileBuf = LoadFileFromPAK(&Data, FileName, OrigLength);
	  if ( fileBuf )
		return fileBuf;
	}
	if ( byte_D0A28C )
	{
	  fileBuf = LoadFileFromPAK(&byte_D0A28C, FileName, OrigLength);
	  if ( fileBuf )
		return fileBuf;
	}
	fileBuf = LoadFileFromPAK(&Buffer, FileName, OrigLength);
	if ( fileBuf )
	  return fileBuf;
	if ( String )
	{
	  fileBuf = LoadFileFromPAK(&String, FileName, OrigLength);
	  if ( fileBuf )
		return fileBuf;
	}
	if ( byte_D0A394 )
	{
	  fileBuf = LoadFileFromPAK(&byte_D0A394, FileName, OrigLength);
	  if ( fileBuf )
		return fileBuf;
	}
	if ( byte_D0A398 )
	{
	  fileBuf = LoadFileFromPAK(&byte_D0A398, FileName, OrigLength);
	  if ( fileBuf )
		return fileBuf;
	}
	if ( byte_D0A49C )
	{
	  fileBuf = LoadFileFromPAK(&byte_D0A49C, FileName, OrigLength);
	  if ( fileBuf )
		return fileBuf;
	}
	sub_415410();
	wsprintfA(&String2, aFileNotFound_S, FileName);
	lstrcpyA(&String1, &String2);
LABEL_27:
	fileBuf = 0;
  }
  return fileBuf;
}

 */

void DumpFile()
{
	char fnm[] = "ATTENTION.s25";
	DWORD FileSize;
	BYTE* buf;
	buf = (Func_ReadCompFile(p_ReadCompFile))(fnm, &FileSize);
	wchar_t fnmbuffer[256];
	if (fnm)
	{
		size_t nu = strlen(fnm);
		size_t n = (size_t)MultiByteToWideChar(932, 0, fnm, int(nu), NULL, 0);
		memset(fnmbuffer, 0, sizeof(wchar_t) * 256);
		MultiByteToWideChar(932, 0, fnm, int(nu), fnmbuffer, int(n));
	}
	wchar_t filePath[256];
	wcscpy(filePath, L"dump\\");
	wcscat(filePath, fnmbuffer);
	HANDLE pFile = CreateFile(filePath, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (pFile == INVALID_HANDLE_VALUE)MessageBoxW(NULL, L"INVALID_HANDLE_VALUE", L"ER", 0);
	DWORD dwBytesWrite = 0;
	WriteFile(pFile, buf, FileSize, &dwBytesWrite, NULL);
	if (dwBytesWrite == 0)MessageBoxW(NULL, L"Write Error", L"ER", 0);
	CloseHandle(pFile);
	wcout << "DUMP:" << filePath << endl;
}
int idx = 0;
int NewDecrypt_Uncomp(unsigned char* Uncompress_OutBuf, unsigned char* crypted_data, unsigned int compress_size, int preKey, int* Origlength)
{
	auto ret = (Func_Decrypt_Uncomp(p_Decrypt_Uncomp))(Uncompress_OutBuf, crypted_data, compress_size, preKey, Origlength);
	if (*Origlength % 0x38 != 0)
	{
		return ret;
	}
	wstring filepath;
	wstring filepath2;
	filepath = L"dump\\" + to_wstring(idx) + L".idx";
	filepath2 = L"dump\\old" + to_wstring(idx) + L".idx";
	idx += 1;
	HANDLE pFile = CreateFile(filepath.c_str(), GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (pFile == INVALID_HANDLE_VALUE)MessageBoxW(NULL, L"INVALID_HANDLE_VALUE", L"ER", 0);
	DWORD dwBytesWrite = 0;
	WriteFile(pFile, Uncompress_OutBuf, *Origlength, &dwBytesWrite, NULL);
	if (dwBytesWrite == 0)MessageBoxW(NULL, L"Write Error", L"ER", 0);
	CloseHandle(pFile);

	pFile = CreateFile(filepath2.c_str(), GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (pFile == INVALID_HANDLE_VALUE)MessageBoxW(NULL, L"INVALID_HANDLE_VALUE", L"ER", 0);
	dwBytesWrite = 0;
	WriteFile(pFile, crypted_data, compress_size, &dwBytesWrite, NULL);
	if (dwBytesWrite == 0)MessageBoxW(NULL, L"Write Error", L"ER", 0);
	CloseHandle(pFile);

	wcout << "DUMP IDX:" << filepath << endl;
	return ret;
}

BYTE* __cdecl NewReadCompFile(char* fnm, DWORD* ori)
{

	auto ret = (Func_ReadCompFile(p_ReadCompFile))(fnm, ori);
	return  ret;
}
bool dumponce = true;
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
	return ((Func_CreateFontA)p_CreateFontA)(nHeight, nWidth, nEscapement, nOrientation, fnWeight, fdwItalic, fdwUnderline, fdwStrikeOut, fdwCharSet, fdwOutputPrecision, fdwClipPrecision, fdwQuality, fdwPitchAndFamily, lpszFace);
}

void SJISBypass()
{
	DWORD baseAddr = (DWORD)GetModuleHandle(NULL);
	BYTE Patch1[] = { 0xFE };
	//cmp al,0x9F
	memcopy((void*)(baseAddr + 0x3ADD3), Patch1, sizeof(Patch1));
	memcopy((void*)(baseAddr + 0x3ADDB), Patch1, sizeof(Patch1));
	//cmp cl,0x9F
	memcopy((void*)(baseAddr + 0x29D1C), Patch1, sizeof(Patch1));
	memcopy((void*)(baseAddr + 0x29D26), Patch1, sizeof(Patch1));
	memcopy((void*)(baseAddr + 0x3ABE6), Patch1, sizeof(Patch1));
	memcopy((void*)(baseAddr + 0x3ABF0), Patch1, sizeof(Patch1));
	memcopy((void*)(baseAddr + 0x3BDD4), Patch1, sizeof(Patch1));
	memcopy((void*)(baseAddr + 0x3BDDE), Patch1, sizeof(Patch1));
}

void init()
{
	DWORD baseAddr = (DWORD)GetModuleHandle(NULL);
	p_ReadCompFile = (PVOID)(baseAddr + 0x5940);
	p_Decrypt_Uncomp = (PVOID)(baseAddr + 0x762E0);
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	//DetourAttach(&p_RegOpenKeyEx, NewRegOpenKeyExA);
	//DetourAttach(&p_SetWindowTextA, NewSetWindowTextA);
	//DetourAttach(&p_CreateFontIndirectA, NewCreateFontIndirectA);
	DetourAttach(&p_CreateFontA, newCreateFontA);
	DetourAttach(&p_ReadCompFile, NewReadCompFile);
	DetourAttach(&p_Decrypt_Uncomp, NewDecrypt_Uncomp);
	if (DetourTransactionCommit() != NO_ERROR)
	{
		MessageBoxW(NULL, L"Hook目标函数失败", L"严重错误", MB_OK | MB_ICONWARNING);
	}
	//SJISBypass();

}

static void make_console() {
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
	freopen("CONIN$", "r", stdin);
	std::cout << "Open Console Success!" << std::endl;
}

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		setlocale(LC_ALL, "chinese");
		make_console();
		init();
		//MessageBoxW(NULL, L"MSG", L"TEST", MB_OK);
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

extern "C"  void dummy(void) {
	return;
}