#include <Windows.h>
#include <stdio.h>


int wmain(int argc, WCHAR* argv[])
{
	if (argc != 2)
		return 0;

	FILE* fin = _wfopen(argv[1], L"rb");
	fseek(fin, 0, SEEK_END);
	unsigned long Size = ftell(fin);
	rewind(fin);
	char* Buffer = new char[Size + 1];
	memset(Buffer, 0, Size + 1);
	fread(Buffer, 1, Size, fin);
	fclose(fin);

	WCHAR bom = *(WCHAR*)Buffer;
	if (bom == 0xFEFF)
		return 0;

	WCHAR* WideBuffer = new WCHAR[Size * 4];
	memset(WideBuffer, 0, Size * 8);

	MultiByteToWideChar(932, 0, Buffer, Size, &WideBuffer[1], Size * 4 - 1);

	WideBuffer[0] = 0xFEFF;

	FILE* fout = _wfopen(argv[1], L"wb");
	fwrite(WideBuffer, 2, lstrlenW(WideBuffer), fout);
	fclose(fout);
	delete[] Buffer;
	delete[] WideBuffer;
	return 0;
}

