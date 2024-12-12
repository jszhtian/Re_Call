#include "stdafx.h"
#include <Windows.h>
#include "WinFile.h"
#include <string>

using std::wstring;

#define ENABLE_NONE_UNICODE

void ParseScript(const WCHAR* lpFileName)
{
	WinFile File;
	if (File.Open(lpFileName, WinFile::FileRead) != S_OK)
	{
		return;
	}
	
	PBYTE Buffer = nullptr;
	ULONG Size = 0;
	ULONG iPos = 4;
	ULONG Count = 0;

	Buffer = (PBYTE)HeapAlloc(GetProcessHeap(), 0, File.GetSize32());
	Size = File.GetSize32();
	File.Read(Buffer, Size);
	File.Release();
	if (Buffer == nullptr)
		return;

	wstring OutFileName = lpFileName;
	OutFileName += L".txt";

	FILE* f = _wfopen(OutFileName.c_str(), L"wb");

	ULONG OpCodeSize = 0;
	OpCodeSize = *(ULONG*)(Buffer + iPos);
	iPos = OpCodeSize * 8 + 8;
	Count = *(ULONG*)(Buffer + iPos);
	iPos += 4;
	//iPos += 1; //zero protection?
	Count--;
	while (Count)
	{
		WCHAR WideChar[2048] = { 0 };
		CHAR MbsChar[4096] = { 0 };
		MultiByteToWideChar(932, 0, (char*)Buffer + iPos, lstrlenA((char*)Buffer + iPos), WideChar, 2048);
		WideCharToMultiByte(CP_UTF8, 0, WideChar, lstrlenW(WideChar), MbsChar, 4096, nullptr, nullptr);

		fprintf(f, "[%08d]%s\r\n", iPos, MbsChar);
		fprintf(f, ";[%08d]%s\r\n\r\n", iPos, MbsChar);

		ULONG Strlen = lstrlenA((char*)Buffer + iPos);
		iPos += Strlen + 1;
		Count--;
	}

	fclose(f);
	HeapFree(GetProcessHeap(), 0, Buffer);
}

int _tmain(int argc, _TCHAR* argv[])
{
	WIN32_FIND_DATAW Data = { 0 };
	HANDLE hFind = FindFirstFileW(L"*.bin", &Data);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		wprintf(L"No File!\n");
		getchar();
		return 0;
	}
	ParseScript(Data.cFileName);

	while (FindNextFileW(hFind, &Data))
	{
		//if (Data.dwFileAttributes & FILE_ATTRIBUTE_NORMAL)
		{
			ParseScript(Data.cFileName);
		}
	}
	FindClose(hFind);
	return 0;
}

