// FVPDeasm.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <Windows.h>
#include <string>

#define AppName L"FVPDeasm"

using std::wstring;

UINT DefCodePage = 932;

static char StringBuffer[4096] = { 0 };

#define Clear memset(StringBuffer, 0, sizeof(StringBuffer));

#define SJISConv(s) MultiByteToUTF8(s, StringBuffer, 4096, 932)
#define CHSConv(s) MultiByteToUTF8(s, StringBuffer, 4096, 936)

int MultiByteToUTF8(const char * lpGBKStr, const char * lpUTF8Str, int nUTF8StrLen, ULONG cp = 932)
{
	wchar_t * lpUnicodeStr = NULL;
	int nRetLen = 0;

	if (!lpGBKStr)  //���GBK�ַ���ΪNULL������˳�
		return 0;

	nRetLen = ::MultiByteToWideChar(cp, 0, (char *)lpGBKStr, -1, NULL, NULL);
	lpUnicodeStr = new WCHAR[nRetLen + 1];
	nRetLen = ::MultiByteToWideChar(cp, 0, (char *)lpGBKStr, -1, lpUnicodeStr, nRetLen);
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

int _tmain(int argc, _TCHAR* argv[])
{
	if (argc < 2)
	{
		MessageBoxW(NULL, (L"Usage :\n" + wstring(argv[1]) + L" input codepage[def = 92]").c_str(), AppName, MB_OK);
		return -1;
	}

	if (argc == 3)
	{
		UINT CodeTemp = _wtoi(argv[2]);
		if (CodeTemp != 932 && CodeTemp != 936)
		{
			WCHAR Info[1024] = { 0 };
			wsprintfW(Info, L"Unknown CodePage %s", argv[2]);
			MessageBoxW(NULL, Info, AppName, MB_OK);
			return -1;
		}
		DefCodePage = CodeTemp;
	}

	FILE *fin = _wfopen(argv[1], L"rb");
	fseek(fin, 0, SEEK_END);
	unsigned long FileSize = ftell(fin);
	rewind(fin);
	unsigned char *pBuffer = new unsigned char[FileSize];
	memset(pBuffer, 0, sizeof(pBuffer));
	fread(pBuffer, FileSize, 1, fin);
	fclose(fin);

	FILE* StringOuter = nullptr;
	FILE* DeasmCode = nullptr;

	StringOuter = _wfopen(L"Text.txt", L"wb");
	DeasmCode = _wfopen(L"DeasmCode.txt", L"wb");


	DWORD iPos = 0; //EIP

	DWORD RealStackADD;
	memcpy(&RealStackADD, pBuffer, 4);

	/************************************/
	iPos = 4;
	printf("StackEntryADD = %08x\n", RealStackADD);
	while (iPos < RealStackADD)
	{
		if (*(pBuffer + iPos) == 0x0E)
		{
			Clear
			DWORD PtrSaver = iPos;
			fprintf(DeasmCode, "[0x%08x] PushString ", iPos);
			iPos++;
			unsigned char Length = pBuffer[iPos];
			iPos++;
			if (Length == 1)
			{
				fprintf(StringOuter, "[0x%08x]\r\n");
				fprintf(StringOuter, ";[0x%08x]\r\n\r\n");

				fprintf(DeasmCode, "\"\"\r\n");
			}
			else
			{
				unsigned char Info[1024] = {0};

				if (DefCodePage == 932)
				{
					SJISConv((char*)(pBuffer + iPos));
				}
				else if (DefCodePage == 936)
				{
					CHSConv((char*)(pBuffer + iPos));
				}

				fprintf(DeasmCode, "\"%s\"\r\n", StringBuffer);

				fprintf(StringOuter, "[0x%08x]%s\r\n", PtrSaver, StringBuffer);
				fprintf(StringOuter, ";[0x%08x]\r\n\r\n", PtrSaver);
			}
			iPos += Length;
		}
		else if (*(pBuffer + iPos) == 0x06)
		{
			fprintf(DeasmCode, "[0x%08x] JMP ", iPos);
			iPos++;
			fprintf(DeasmCode, "0x%08x\r\n", *(DWORD*)(pBuffer + iPos));
			iPos += 4;
		}
		else if (*(pBuffer + iPos) == 0x02)
		{
			fprintf(DeasmCode, "[0x%08x] Call ", iPos);
			iPos++;
			fprintf(DeasmCode, "0x%08x\r\n", *(DWORD*)(pBuffer + iPos));
			iPos += 4;
		}
		else if (*(pBuffer + iPos) == 0x03)
		{
			fprintf(DeasmCode, "[0x%08x] CallSystem ", iPos);
			iPos++;
			fprintf(DeasmCode, "0x%04x\r\n", *(WORD*)(pBuffer + iPos));
			iPos += 2;
		}
		else if (*(pBuffer + iPos) == 0x01)
		{
			fprintf(DeasmCode, "[0x%08x] StartFunction(0x%08x) Info:", iPos);
			iPos++;
			fprintf(DeasmCode, "(ParamCount : 0x%02x, LocalCount : 0x%02x)\r\n", (unsigned char)pBuffer[iPos], 
				(unsigned char)pBuffer[iPos + 1]);
			iPos += 2;
		}
		else if (*(pBuffer + iPos) == 0x04)
		{
			fprintf(DeasmCode, "[0x%08x] Return(void)\r\n", iPos);
			iPos++;
		}
		else if (*(pBuffer + iPos) == 0x05)
		{
			fprintf(DeasmCode, "[0x%08x] Return(With Eax)\r\n", iPos);
			iPos++;
		}
		else if (*(pBuffer + iPos) == 0x07)
		{
			fprintf(DeasmCode, "[0x%08x] JZ ", iPos);
			iPos++;
			fprintf(DeasmCode, "0x%08x\r\n", *(DWORD*)(pBuffer + iPos));
			iPos += 4;
		}
		else if (*(pBuffer + iPos) == 0x08)
		{
			fprintf(DeasmCode, "[0x%08x] Push0\r\n", iPos);
			iPos++;
		}
		else if (*(pBuffer + iPos) == 0x09)
		{
			fprintf(DeasmCode, "[0x%08x] Push1\r\n", iPos);
			iPos++;
		}
		else if (*(pBuffer + iPos) == 0x0A)
		{
			fprintf(DeasmCode, "[0x%08x] PushInt32 ", iPos);
			iPos++;
			fprintf(DeasmCode, "0x%08x\r\n", *(DWORD*)(pBuffer + iPos));
			iPos += 4;
		}
		else if (*(pBuffer + iPos) == 0x0B)
		{
			fprintf(DeasmCode, "[0x%08x] PushInt16 ", iPos);
			iPos++;
			fprintf(DeasmCode, "0x%04x\r\n", *(WORD*)(pBuffer + iPos));
			iPos += 2;
		}
		else if (*(pBuffer + iPos) == 0x0C)
		{
			fprintf(DeasmCode, "[0x%08x] PushInt8 ", iPos);
			iPos++;
			fprintf(DeasmCode, "0x%02x\r\n", *(unsigned char*)(pBuffer + iPos));
			iPos++;
		}
		else if (*(pBuffer + iPos) == 0x0D)
		{
			fprintf(DeasmCode, "[0x%08x] PushFloat32 ", iPos);
			iPos++;
			fprintf(DeasmCode, "0x%08x\r\n", *(DWORD*)(pBuffer + iPos));
			iPos += 4;
		}
		else if (*(pBuffer + iPos) == 0x0F)
		{
			fprintf(DeasmCode, "[0x%08x] PushGlobal ", iPos);
			iPos++;
			fprintf(DeasmCode, "0x%04x\r\n", *(WORD*)(pBuffer + iPos));
			iPos += 2;
		}
		else if (*(pBuffer + iPos) == 0x10)
		{
			fprintf(DeasmCode, "[0x%08x] PushStack ", iPos);
			iPos++;
			fprintf(DeasmCode, "0x%02x\r\n", *(unsigned char*)(pBuffer + iPos));
			iPos++;
		}
		else if (*(pBuffer + iPos) == 0x11)
		{
			fprintf(DeasmCode, "[0x%08x] PushMap1 ", iPos);
			iPos++;
			fprintf(DeasmCode, "0x%04x\r\n", *(WORD*)(pBuffer + iPos));
			iPos += 2;
		}
		else if (*(pBuffer + iPos) == 0x12)
		{
			fprintf(DeasmCode, "[0x%08x] PushMap1 ", iPos);
			iPos++;
			fprintf(DeasmCode, "0x%02x\r\n", *(unsigned char*)(pBuffer + iPos));
			iPos++;
		}
		else if (*(pBuffer + iPos) == 0x13)
		{
			fprintf(DeasmCode, "[0x%08x] PushTop\r\n", iPos);
			iPos++;
		}
		else if (*(pBuffer + iPos) == 0x14)
		{
			fprintf(DeasmCode, "[0x%08x] PushTemp\r\n", iPos);
			iPos++;
		}
		else if (*(pBuffer + iPos) == 0x15)
		{
			fprintf(DeasmCode, "[0x%08x] PopGlobal ", iPos);
			iPos++;
			fprintf(DeasmCode, "0x%04x\r\n", *(WORD*)(pBuffer + iPos));
			iPos += 2;
		}
		else if (*(pBuffer + iPos) == 0x16)
		{
			fprintf(DeasmCode, "[0x%08x] PopStack \r\n", iPos);
			iPos++;
			fprintf(DeasmCode, "0x%02x\r\n", *(unsigned char*)(pBuffer + iPos));
			iPos++;
		}
		else if (*(pBuffer + iPos) == 0x17)
		{
			fprintf(DeasmCode, "[0x%08x] PopMap1 ", iPos);
			iPos++;
			fprintf(DeasmCode, "0x%04x\r\n", *(WORD*)(pBuffer + iPos));
			iPos += 2;
		}
		else if (*(pBuffer + iPos) == 0x18)
		{
			fprintf(DeasmCode, "[0x%08x] PopMap2 ", iPos);
			iPos++;
			fprintf(DeasmCode, "0x%02x\r\n", *(unsigned char*)(pBuffer + iPos));
			iPos++;
		}
		else if (*(pBuffer + iPos) == 0x19)
		{
			fprintf(DeasmCode, "[0x%08x] Neg\r\n", iPos);
			iPos++;
		}
		else if (*(pBuffer + iPos) == 0x1A)
		{
			fprintf(DeasmCode, "[0x%08x] Add\r\n", iPos);
			iPos++;
		}
		else if (*(pBuffer + iPos) == 0x1B)
		{
			fprintf(DeasmCode, "[0x%08x] Sub\r\n", iPos);
			iPos++;
		}
		else if (*(pBuffer + iPos) == 0x1C)
		{
			fprintf(DeasmCode, "[0x%08x] Mul\r\n", iPos);
			iPos++;
		}
		else if (*(pBuffer + iPos) == 0x1D)
		{
			fprintf(DeasmCode, "[0x%08x] Div\r\n", iPos);
			iPos++;
		}
		else if (*(pBuffer + iPos) == 0x1E)
		{
			fprintf(DeasmCode, "[0x%08x] Mod\r\n", iPos);
			iPos++;
		}
		else if (*(pBuffer + iPos) == 0x1F)
		{
			fprintf(DeasmCode, "[0x%08x] Neg\r\n", iPos);
			iPos++;
		}
		else if (*(pBuffer + iPos) == 0x20)
		{
			fprintf(DeasmCode, "[0x%08x] Condition Add\r\n", iPos);
			iPos++;
		}
		else if (*(pBuffer + iPos) == 0x21)
		{
			fprintf(DeasmCode, "[0x%08x] Condition Or\r\n", iPos);
			iPos++;
		}
		else if (*(pBuffer + iPos) == 0x22)
		{
			fprintf(DeasmCode, "[0x%08x] SetE\r\n", iPos);
			iPos++;
		}
		else if (*(pBuffer + iPos) == 0x23)
		{
			fprintf(DeasmCode, "[0x%08x] SetNE\r\n", iPos);
			iPos++;
		}
		else if (*(pBuffer + iPos) == 0x24)
		{
			fprintf(DeasmCode, "[0x%08x] SetG\r\n", iPos);
			iPos++;
		}
		else if (*(pBuffer + iPos) == 0x25)
		{
			fprintf(DeasmCode, "[0x%08x] SetLE\r\n", iPos);
			iPos++;
		}
		else if (*(pBuffer + iPos) == 0x26)
		{
			fprintf(DeasmCode, "[0x%08x] SetL\r\n", iPos);
			iPos++;
		}
		else if (*(pBuffer + iPos) == 0x27)
		{
			fprintf(DeasmCode, "[0x%08x] SetGE\r\n", iPos);
			iPos++;
		}
		else
		{ //unknown
		if (*(pBuffer + iPos) > 0x27 || *(pBuffer + iPos) == 0)
			iPos++;
		else
		{
			WCHAR Info[1024] = { 0 };
			wsprintfW(Info, L"Unknown VM Code at[0x%08x] = [0x%02x]", iPos, (unsigned char)pBuffer[iPos]);
			MessageBoxW(NULL, Info, AppName, MB_OK);
			return -1;
		}
		}
	}

	fclose(DeasmCode);
	fclose(StringOuter);
	system("pause");
	return 0;
}

