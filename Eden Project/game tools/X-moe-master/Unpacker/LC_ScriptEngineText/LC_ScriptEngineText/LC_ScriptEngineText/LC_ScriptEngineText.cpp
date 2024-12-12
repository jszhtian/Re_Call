// LC_ScriptEngineText.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <string>
#include <Windows.h>

using std::string;

typedef struct
{
	DWORD instructs;
	DWORD text_length;
} snx_header_t;


int SJISToUTF8(const char * lpJPStr, const char * lpUTF8Str, int nUTF8StrLen)
{
	wchar_t * lpUnicodeStr = NULL;
	int nRetLen = 0;

	if (!lpJPStr)
		return 0;

	nRetLen = ::MultiByteToWideChar(932, 0, (char *)lpJPStr, -1, NULL, NULL);
	lpUnicodeStr = new WCHAR[nRetLen + 1];
	nRetLen = ::MultiByteToWideChar(932, 0, (char *)lpJPStr, -1, lpUnicodeStr, nRetLen);
	if (!nRetLen)
		return 0;

	nRetLen = ::WideCharToMultiByte(CP_UTF8, 0, lpUnicodeStr, -1, NULL, 0, NULL, NULL);

	if (!lpUTF8Str)
	{
		if (lpUnicodeStr)
			delete[]lpUnicodeStr;
		return nRetLen;
	}

	if (nUTF8StrLen < nRetLen)
	{
		if (lpUnicodeStr)
			delete[]lpUnicodeStr;
		return 0;
	}
	nRetLen = ::WideCharToMultiByte(CP_UTF8, 0, lpUnicodeStr, -1, (char *)lpUTF8Str, nUTF8StrLen, NULL, NULL);

	if (lpUnicodeStr)
		delete[]lpUnicodeStr;

	return nRetLen;
}


int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		return -1;
	}
	FILE *file = fopen(argv[1], "rb");
	if (!file)
	{
		return -1;
	}
	rewind(file);
	fseek(file, 0, SEEK_END);
	DWORD FileSize = ftell(file);
	rewind(file);

	unsigned char* pBuffer = new unsigned char[FileSize];
	fread(pBuffer, 1, FileSize, file);
	fclose(file);

	snx_header_t* SNX = (snx_header_t*)pBuffer;
	//fseek(file, SNX->text_length, SEEK_END);
	DWORD iPos = FileSize - SNX->text_length;

	char UTF[2048] = { 0 };
	FILE* out = fopen((string(argv[1]) + ".txt").c_str(), "wb");
	while (iPos < FileSize)
	{
		DWORD len = *(DWORD*)(pBuffer + iPos);
		iPos += 4;
		string SJIS((char*)pBuffer + iPos, len);
		iPos += len;
		memset(UTF, 0, 2048);
		SJISToUTF8(SJIS.c_str(), UTF, 2048);
		fprintf(out, "%s\r\n", UTF);
	}
	fclose(out);
	delete[] pBuffer;
	return 0;
}

