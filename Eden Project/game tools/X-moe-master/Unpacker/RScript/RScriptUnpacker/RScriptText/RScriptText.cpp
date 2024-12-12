#include <Windows.h>
#include <stdio.h>
#include <string>

using std::wstring;

//0x1C
typedef struct RScriptHeader
{
	DWORD FileSize;
	DWORD HeaderSize;
	DWORD ByteCodeSize1;
	DWORD ByteCodeSize2;
	DWORD StringPoolSize;
	DWORD ByteCodeSize3;
	DWORD ByteCodeSize4;
}RScriptHeader;

int wmain(int argc, WCHAR* argv[])
{
	if (argc != 2)
		return 0;

	FILE* fin = _wfopen(argv[1], L"rb");
	if (!fin)
		return 0;

	fseek(fin, 0, SEEK_END);
	ULONG Size = ftell(fin);
	rewind(fin);
	PBYTE Buffer = new BYTE[Size];
	if (!Buffer)
	{
		fclose(fin);
		return 0;
	}

	fread(Buffer, 1, Size, fin);
	fclose(fin);

	RScriptHeader* Header = (RScriptHeader*)Buffer;
	if (Header->StringPoolSize == 0)
	{
		delete[] Buffer;
		return 0;
	}

	ULONG PostOffset = Header->HeaderSize + Header->ByteCodeSize1 + Header->ByteCodeSize2;
	ULONG iPos = 0;

	wstring OutName(argv[1]);
	OutName += L".txt";

	FILE* fout = _wfopen(OutName.c_str(), L"wb");

	while (iPos < Header->StringPoolSize)
	{
		WCHAR WideName[1000] = { 0 };
		CHAR UTF8Name[3000] = { 0 };

		MultiByteToWideChar(932, 0, (PCHAR)(Buffer + PostOffset + iPos),
			lstrlenA((PCHAR)(Buffer + PostOffset + iPos)), WideName, 1000);
		WideCharToMultiByte(CP_UTF8, 0, WideName, lstrlenW(WideName), UTF8Name, 3000, 
			nullptr, nullptr);

		fprintf(fout, "[0x%08x]%s\r\n", PostOffset + iPos, UTF8Name);
		fprintf(fout, ";[0x%08x]%s\r\n", PostOffset + iPos, UTF8Name);
		fprintf(fout, ">[0x%08x]\r\n\r\n", PostOffset + iPos);

		iPos += lstrlenA((PCHAR)(Buffer + PostOffset + iPos)) + 1;
	}

	fclose(fout);
	delete[] Buffer;
	return 0;
}

