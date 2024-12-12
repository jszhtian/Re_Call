#include <stdio.h>
#include <string>
#include <windows.h>

using std::string;
using std::wstring;

#define ParseSetMark "<Mark:0x%08x>"
#define ParseSetCode "<Code:0x%02x>"

/*
BOOL isPrintable(BYTE Code)
{
	if ((Code >= 'A' && Code <= 'Z') || 
		(Code >= 'a' && Code <= 'z') || 
		IsDBCSLeadByteEx(932, Code)  || 
		(Code >= '0' && Code <= '9'))
		return TRUE;
	else
		return FALSE;
}
*/
BOOL isPrintable(BYTE Code)
{
	if ((Code >= 0x20 && Code < 0x7F) || IsDBCSLeadByteEx(932, Code))
		return TRUE;
	else
		return FALSE;
}
#if !defined(BREAK_IF)
#define BREAK_IF(c) if (c) break;
#endif

#if !defined(CONTINUE_IF)
#define CONTINUE_IF(c) if (c) continue;
#endif

#define SWAP2(v) (USHORT)(((DWORD)(v) << 8) | ((USHORT)(v) >> 8))
#define SWAPCHAR(v) ((DWORD)SWAP2(v))


BOOL IsShiftJISChar(SIZE_T uChar)
{
	BYTE LowByte, HighByte;

	LowByte = LOBYTE(uChar);
	HighByte = HIBYTE(uChar);
	if ((HighByte >= 0x80 && HighByte <= 0x9F) ||
		(HighByte >= 0xE0 && HighByte <= 0xFC))
	{
		if ((LowByte >= 0x40 && LowByte <= 0x7E) ||
			(LowByte >= 0x80 && LowByte <= 0xFC))
		{
			return TRUE;
		}
	}
	return FALSE;
}

BOOL IsShiftJISString(PCHAR pString, SIZE_T Length)
{
	BOOL   bMBChar;
	PCHAR  p;

	if (pString == NULL)
		return FALSE;

	bMBChar = FALSE;
	p = pString;
	for (; Length; ++p, --Length)
	{
		CHAR  c;
		INT32 ch;

		c = *p;
		CONTINUE_IF(c > 0);
		BREAK_IF(c == 0 || --Length == 0);

		ch = *(PUSHORT)p++;
		if (!IsShiftJISChar(SWAPCHAR(ch)))
			return FALSE;
		else
			bMBChar = TRUE;
	}

	return bMBChar;
}


//The buffer should be pre-decoded
BOOL ParseWillPlusScript(PBYTE Buffer, ULONG Size, const wstring& CodeFileName, const wstring& TextFileName)
{
	BOOL  Result;
	FILE* HandleOfCode;
	FILE* HandleOfText;
	ULONG MarkLine, iPos;

	do
	{
		Result = FALSE;
		MarkLine = 0;
		iPos = 0;

		HandleOfCode = _wfopen(CodeFileName.c_str(), L"wb");
		HandleOfText = _wfopen(TextFileName.c_str(), L"wb");

		while (iPos < Size)
		{
			if (isPrintable(Buffer[iPos]) && IsShiftJISString((PCHAR)&Buffer[iPos], lstrlenA((PCHAR)&Buffer[iPos])))
			{
				fprintf(HandleOfCode, ParseSetMark, MarkLine);
				fprintf(HandleOfCode, "\r\n");
				MarkLine++;

				WCHAR WideLine[2048] = { 0 };
				CHAR  UTF8Line[3000] = { 0 };

				printf("iPos : 0x%08x\n", iPos);
				printf("%s\n", &Buffer[iPos]);

				MultiByteToWideChar(932, 0, (PCHAR)&Buffer[iPos], lstrlenA((PCHAR)&Buffer[iPos]), WideLine, 2048);
				WideCharToMultiByte(CP_UTF8, 0, WideLine, lstrlenW(WideLine), UTF8Line, 3000, 0, 0);

				fprintf(HandleOfText, "[0x%08x]%s\r\n", iPos, UTF8Line);
				fprintf(HandleOfText, ";[0x%08x]%s\r\n\r\n", iPos, UTF8Line);
				iPos += lstrlenA((PCHAR)&Buffer[iPos]);
			}
			else
			{
				fprintf(HandleOfCode, ParseSetCode, Buffer[iPos]);
				fprintf(HandleOfCode, "\r\n");
				iPos++;
			}
		}

		fclose(HandleOfCode);
		fclose(HandleOfCode);
	} while (FALSE);

	return Result;
}

int wmain(int argc, WCHAR* argv[])
{
	if (argc != 2)
		return -1;

	wstring CodeFileName(argv[1]);
	CodeFileName += L".asm";
	wstring TextFileName(argv[1]);
	TextFileName += L".txt";

	do
	{
		ULONG FileSize;
		PBYTE FileBuffer;
		FILE* fin = _wfopen(argv[1], L"rb");
		if (fin == NULL)
		{
			wprintf(L"Couldn't open script file[%s]\n", argv[1]);
			break;
		}

		fseek(fin, 0, SEEK_END);
		FileSize = ftell(fin);
		rewind(fin);
		FileBuffer = (PBYTE)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, FileSize);

		if (!FileBuffer)
		{
			printf("Insufficient memory to complete requested task(>_<)\n");
			break;
		}
		fread(FileBuffer, 1, FileSize, fin);
		ParseWillPlusScript(FileBuffer, FileSize, CodeFileName, TextFileName);

		HeapFree(GetProcessHeap(), 0, FileBuffer);
		fclose(fin);
	} while (FALSE);
	return 0;
}

