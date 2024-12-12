#include <WinFile.h>
#pragma comment(linker, "/ENTRY:SetupMain")
#pragma comment(linker, "/SECTION:.text,ERW /MERGE:.rdata=.text /MERGE:.data=.text")
#pragma comment(linker, "/SECTION:.Xmoe,ERW /MERGE:.text=.Xmoe")

#pragma optimize("gsy", on)
#pragma warning(disable:4254)

#pragma comment(lib, "ntdll.lib")

extern "C" void* __cdecl memcpy(void* dst, const void* src, size_t count);
extern "C" void* __cdecl memset(void* s, int c, size_t n);
#pragma intrinsic(memcpy)
#pragma intrinsic(memset)

#pragma function(memcpy)
void* __cdecl memcpy(void* dst, const void* src, size_t count)
{
	void* ret = dst;

	while (count--)
	{
		*(char *)dst = *(char *)src;
		dst = (char *)dst + 1;
		src = (char *)src + 1;
	}
	return (ret);
}

#pragma function(memset)
void* __cdecl memset(void* s, int c, size_t n)
{
	unsigned char* p = (unsigned char*)s;
	while (n > 0)
	{
		*p++ = (unsigned char)c;
		--n;
	}
	return s;
}


#ifndef LOOP_ONCE
#define LOOP_ONCE for(ULONG i = 0; i < 1; i++)
#endif

static int inline AnzWideCharToUtf8(wchar_t in, char * out)
{
	if (in < (1 << 7))
	{
		if (out)
		{
			out[0] = (char)in;
		}
		return 1;
	}
	else if (in < (1 << 11))
	{
		if (out)
		{
			out[0] = (char)(0xc0 | (in >> 6));
			out[1] = (char)(0x80 | (in & 0x3f));
		}
		return 2;
	}
	else if (in < (1 << 16))
	{
		if (out)
		{
			out[0] = (char)(0xe0 | (in >> 12));
			out[1] = (char)(0x80 | ((in >> 6) & 0x3f));
			out[2] = (char)(0x80 | (in & 0x3f));
		}
		return 3;
	}
	else
	{
		return -1;
	}
	return -1;
}


int AnzWideCharToUtf8String(const wchar_t *in, char * out)
{
	int count = 0;
	while (*in)
	{
		int n;
		if (out)
		{
			n = AnzWideCharToUtf8(*in, out);
			out += n;
		}
		else
		{
			n = AnzWideCharToUtf8(*in, NULL);
		}
		if (n == -1) return -1;
		count += n;
		in++;
	}
	return count;
}

static bool inline AnzUtf8ToWideChar(const char * & in, wchar_t *out)
{
	const unsigned char * & p = (const unsigned char * &)in;
	if (p[0] < 0x80)
	{
		if (out) *out = (wchar_t)in[0];
		in++;
		return true;
	}
	else if (p[0] < 0xc2)
	{
		return false;
	}
	else if (p[0] < 0xe0)
	{
		if ((p[1] & 0xc0) != 0x80) return false;
		if (out) *out = ((p[0] & 0x1f) << 6) + (p[1] & 0x3f);
		in += 2;
		return true;
	}
	else if (p[0] < 0xf0)
	{
		if ((p[1] & 0xc0) != 0x80) return false;
		if ((p[2] & 0xc0) != 0x80) return false;
		if (out) *out = ((p[0] & 0x1f) << 12) + ((p[1] & 0x3f) << 6) + (p[2] & 0x3f);
		in += 3;
		return true;
	}
	else if (p[0] < 0xf8)
	{
		if ((p[1] & 0xc0) != 0x80) return false;
		if ((p[2] & 0xc0) != 0x80) return false;
		if ((p[3] & 0xc0) != 0x80) return false;
		if (out) *out = ((p[0] & 0x07) << 18) + ((p[1] & 0x3f) << 12) +
			((p[2] & 0x3f) << 6) + (p[3] & 0x3f);
		in += 4;
		return true;
	}
	else if (p[0] < 0xfc)
	{
		if ((p[1] & 0xc0) != 0x80) return false;
		if ((p[2] & 0xc0) != 0x80) return false;
		if ((p[3] & 0xc0) != 0x80) return false;
		if ((p[4] & 0xc0) != 0x80) return false;
		if (out) *out = ((p[0] & 0x03) << 24) + ((p[1] & 0x3f) << 18) +
			((p[2] & 0x3f) << 12) + ((p[3] & 0x3f) << 6) + (p[4] & 0x3f);
		in += 5;
		return true;
	}
	else if (p[0] < 0xfe)
	{
		if ((p[1] & 0xc0) != 0x80) return false;
		if ((p[2] & 0xc0) != 0x80) return false;
		if ((p[3] & 0xc0) != 0x80) return false;
		if ((p[4] & 0xc0) != 0x80) return false;
		if ((p[5] & 0xc0) != 0x80) return false;
		if (out) *out = ((p[0] & 0x01) << 30) + ((p[1] & 0x3f) << 24) +
			((p[2] & 0x3f) << 18) + ((p[3] & 0x3f) << 12) +
			((p[4] & 0x3f) << 6) + (p[5] & 0x3f);
		in += 6;
		return true;
	}
	return false;
}

int AnzUtf8ToWideCharString(const char * in, wchar_t *out)
{
	int count = 0;
	while (*in)
	{
		wchar_t c;
		if (out)
		{
			if (!AnzUtf8ToWideChar(in, &c))
				return -1;
			*out++ = c;
		}
		else
		{
			if (!AnzUtf8ToWideChar(in, NULL))
				return -1;
		}
		count++;
	}
	return count;
}



int XmoeCSetup(int argc, WCHAR* argv[])
{
	if (argc < 2)
		return 0;

	WinFile         File, OutFile;
	ULONG           FileSize, ByteTransferred, CodePage, NewSize, Offset, Magic;
	PBYTE           FileBuffer = nullptr;
	PWCHAR          NewBuffer = nullptr;

	CodePage = argc > 2 ? 936 : 932;

	LOOP_ONCE
	{
		if (File.Open(argv[1], WinFile::FileRead)!=S_OK)
			break;

		FileBuffer = (PBYTE)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, File.GetSize32() + 1);
		if (!FileBuffer)
		{
			File.Release();
			break;
		}

		FileSize = File.GetSize32() + 1;
		File.Read(FileBuffer, FileSize - 1);

		File.Release();

		if (OutFile.Open(argv[1], WinFile::FileWrite) != S_OK)
			break;

		Magic = *(PULONG)FileBuffer;
		if ((Magic & 0x00FFFFFF) == 0xBFBBEF)
			Offset = 3;
		else
			Offset = 0;

		if (( NewSize = AnzUtf8ToWideCharString((PCHAR)(FileBuffer + Offset), nullptr)) != -1)
		{
			CodePage = CP_UTF8;
		}
		else
		{
			if ((Magic & 0x0000FFFF) == 0x0000FEFF)
			{
				CodePage = -1;
				Offset = 2;
			}
		}

		if (CodePage == -1)
		{
			NewBuffer = (PWCHAR)&FileBuffer[2];
			NewSize = (FileSize - 2) / 2;
		}
		else
		{
			NewSize = FileSize * 4;
			NewBuffer = (PWCHAR)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, NewSize);

			if (!NewBuffer)
				break;

			MultiByteToWideChar(CodePage, 0, ((PCHAR)FileBuffer + Offset),
				lstrlenA(((PCHAR)FileBuffer + Offset)), NewBuffer, NewSize);

			NewSize = lstrlenW(NewBuffer);
		}

		WCHAR lpInfoLine[1000] = {0};
		CHAR  lpWriteString[3000] = {0};
		CHAR  TempInfo[100];
		ULONG CurPos = 0, iPos = 0, iLine = 0;
		ULONG TalkNameLen = lstrlenW(L"@Talk name");

		while(iPos < NewSize)
		{
			if (NewBuffer[iPos] == 0x000D)
			{
				if (NewBuffer[iPos + 1] == 0x000A)
					iPos++;

				if ((lpInfoLine[0] == L';' ||
					 lpInfoLine[0] == L'*' ||
					 lpInfoLine[0] == L'@' ||
					 lpInfoLine[0] == NULL) &&
					(RtlCompareMemory(lpInfoLine, L"@Talk name", TalkNameLen) != TalkNameLen))
				{
					RtlZeroMemory(lpInfoLine, _countof(lpInfoLine) * 2);
					CurPos = 0;
				}
				else
				{
					WideCharToMultiByte(CP_UTF8, 0, lpInfoLine, CurPos, lpWriteString,
							sizeof(lpWriteString), nullptr, nullptr);

					RtlZeroMemory(TempInfo, sizeof(TempInfo));
					wsprintfA(TempInfo, "[0x%08x]", iLine);
					OutFile.Write((PBYTE)TempInfo, lstrlenA(TempInfo));
					OutFile.Write((PBYTE)lpWriteString, lstrlenA(lpWriteString));
					OutFile.Write((PBYTE)"\r\n;", 3);
					OutFile.Write((PBYTE)TempInfo, lstrlenA(TempInfo));
					OutFile.Write((PBYTE)"\r\n\r\n", 4);
					RtlZeroMemory(lpWriteString, sizeof(lpWriteString));
					RtlZeroMemory(lpInfoLine, _countof(lpInfoLine) * 2);
					iLine++;
					CurPos = 0;
				}
					
			}
			else if (NewBuffer[iPos] == 0x000A)
			{
				if ((lpInfoLine[0] == L';' ||
					 lpInfoLine[0] == L'*' ||
					 lpInfoLine[0] == L'@' ||
					 lpInfoLine[0] == NULL) &&
					(RtlCompareMemory(lpInfoLine, L"@Talk name", TalkNameLen) != TalkNameLen))
				{
					RtlZeroMemory(lpInfoLine, _countof(lpInfoLine) * 2);
					CurPos = 0;
				}
				else
				{
					WideCharToMultiByte(CP_UTF8, 0, lpInfoLine, CurPos, lpWriteString,
							sizeof(lpWriteString), nullptr, nullptr);

					RtlZeroMemory(TempInfo, sizeof(TempInfo));
					wsprintfA(TempInfo, "[0x%08x]", iLine);
					OutFile.Write((PBYTE)TempInfo, lstrlenA(TempInfo));
					OutFile.Write((PBYTE)lpWriteString, lstrlenA(lpWriteString));
					OutFile.Write((PBYTE)"\r\n;", 3);
					OutFile.Write((PBYTE)TempInfo, lstrlenA(TempInfo));
					OutFile.Write((PBYTE)"\r\n\r\n", 4);
					RtlZeroMemory(lpWriteString, sizeof(lpWriteString));
					RtlZeroMemory(lpInfoLine, _countof(lpInfoLine) * 2);
					iLine++;
					CurPos = 0;
				}
			}
			else
			{
				lpInfoLine[CurPos] = NewBuffer[iPos];
				CurPos++;
			}
			iPos++;
		}
	}
	
	OutFile.Release();
	if (FileBuffer) HeapFree(GetProcessHeap(), 0, FileBuffer);
	if (NewBuffer)  HeapFree(GetProcessHeap(), 0, NewBuffer);
	return 0;
}


VOID SetupMain()
{
	LPWSTR *szArglist = nullptr;
	int nArgs = 1;
	szArglist = CommandLineToArgvW(GetCommandLineW(), &nArgs);

	ULONG Result = XmoeCSetup(nArgs, szArglist);
	ExitProcess(Result);
}
