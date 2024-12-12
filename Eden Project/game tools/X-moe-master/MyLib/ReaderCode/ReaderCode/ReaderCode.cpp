#include "TextStream.h"
#include <string>

using std::wstring;

#pragma pack(push, 4)
typedef struct
{
	ULONG Line;
	ULONG Word;
	ULONG Char;
	ULONG Blank;
}TextInfo, *pTextInfo;
#pragma pack(pop)

#define IsBlank(x) (x == L'\t' || x == L' ' || x =='\r' ||  x == '\n')

__declspec(dllexport)
BOOL NTAPI ParseText(const WCHAR* lpFileName, pTextInfo Info)
{
	if (Info == 0 || lpFileName == 0)
		return FALSE;

	iTextReadStream* stream = nullptr;
	try
	{
		stream = CreateTextStreamForRead(wstring(lpFileName), GetACP());
	}
	catch (wstring& e)
	{
		MessageBoxW(NULL, e.c_str(), L"FATAL", MB_OK);
		if (stream) delete stream;
		return FALSE;
	}
	catch (...)
	{
		MessageBoxW(NULL, L"Unknown Error", L"FATAL", MB_OK);
		if (stream) delete stream;
		return FALSE;
	}

	wstring Text;
	Text.clear();
	try
	{
		stream->Read(Text, 0);
	}
	catch (wstring& e)
	{
		MessageBoxW(NULL, e.c_str(), L"Cannot Read", MB_OK);
		if (stream) delete stream;
		return FALSE;
	}
	catch (...)
	{
		MessageBoxW(NULL, L"Unknown Error", L"Cannot Read", MB_OK);
		if (stream) delete stream;
		return FALSE;
	}

	RtlZeroMemory(Info, sizeof(TextInfo));
	//ok
	Info->Char = Text.length();
	ULONG Offset = 0;

	while(Offset < Text.length())
	{
		if (Text[Offset] == L'\n')
		{
			//Other Unix and Linux
			Offset++;
			Info->Line++;
			Info->Blank++;
		}
		else if (Text[Offset] == L'\r')
		{
			if (Offset + 1 < Text.length() && Text[Offset] == L'\n')
			{
				//Windows
				Offset += 2;
				Info->Blank += 2;
			}
			else
			{
				//OS X
				Offset++;
				Info->Blank++;
			}
			Info->Line++;
		}
		else if (IsBlank(Text[Offset]))
		{
			Info->Blank++;
			Offset++;
		}
		else //words
		{
			while (!IsBlank(Text[Offset]))
			{
				Offset++;
			}
			Offset--; //unget one char
			Info->Word++;
		}
	}

	stream->Destruct();
	return TRUE;
}
