#include "stdafx.h"
#include <Windows.h>
#include <string>

using std::wstring;

//Header :
//Count 
//Unknown

typedef struct Header
{
	DWORD Count;
	DWORD InfoSize;
}Header;


//FileInfo
typedef struct FileInfo
{
	DWORD size;
	DWORD offset;
	wstring name;
}FileInfo;

int _tmain(int argc, _TCHAR* argv[])
{
	if (argc != 2)
	{
		return -1;
	}

	FILE* file = NULL;
	_wfopen_s(&file, argv[1], L"rb");
	if (file == NULL)
	{
		return -1;
	}
	Header* header = new Header;
	fread(header, 1, sizeof(Header), file);

	fseek(file, 8, SEEK_SET);
	FileInfo* FilePool = new FileInfo[header->Count];
	char* InfoBuffer = new char[header->InfoSize];
	fread(InfoBuffer, 1, header->InfoSize, file);

	DWORD iPos = 0;
	for (unsigned int i = 0; i < header->Count; i++)
	{
		DWORD Offset;
		DWORD Size;
		Size = *(DWORD*)(InfoBuffer + iPos);
		iPos += 4;
		Offset = *(DWORD*)(InfoBuffer + iPos);
		iPos += 4;
		FilePool[i].size = Size;
		FilePool[i].offset = Offset;
		FilePool[i].name = (wchar_t*)(InfoBuffer + iPos);
		iPos += (wcslen((wchar_t*)(InfoBuffer + iPos)) + 1) * 2;
	}

	for (unsigned int i = 0; i < header->Count; i++)
	{
		fseek(file, FilePool[i].offset + sizeof(Header) + header->InfoSize, SEEK_SET);
		char* pBuffer = new char[FilePool[i].size];
		fread(pBuffer, 1, FilePool[i].size, file);

		FILE* out = NULL;
		_wfopen_s(&out, FilePool[i].name.c_str(), L"wb");
		fwrite(pBuffer, 1, FilePool[i].size, out);
		fclose(out);
		delete[] pBuffer;
	}

	return 0;
}
