#include <Windows.h>
#include <WinFile.h>

//Work with your diary+(PSP)

//ByteOrder = LE

//PushString(DWORD) 10 00 00 08
//args : Len(DWORD), String
//Pascal-type string

int wmain(int argc, WCHAR* argv[])
{
	if (argc != 2)
		return 0;

	WinFile File, OutFile;
	PBYTE   Buffer = nullptr;
	ULONG   Size, iPos, OpPushStr, StrLen, Offset;
	if (FAILED(File.Open(argv[1], WinFile::FileRead)))
		return 0;

	do
	{
		Size = File.GetSize32();
		Buffer = (PBYTE)HeapAlloc(GetProcessHeap(), 0, Size);
		if (!Buffer)
			break;

		File.Read(Buffer, Size);
		WCHAR OutName[MAX_PATH] = { 0 };
		wsprintfW(OutName, L"%s.txt", argv[1]);

		if (FAILED(OutFile.Open(OutName, WinFile::FileWrite)))
			break;

		iPos = 0;
		OpPushStr = 0x08000010UL;
		while (iPos < Size - 4)
		{
			if (*(PULONG)(Buffer + iPos) == OpPushStr)
			{
				Offset = iPos;
				iPos += 4;
				StrLen = *(PULONG)(Buffer + iPos);
				iPos += 4;

				CHAR JpStr[2000] = { 0 };
				WCHAR UniStr[2000] = { 0 };
				CHAR UtfStr[4000] = { 0 };

				RtlCopyMemory(JpStr, Buffer + iPos, StrLen);
				iPos += StrLen;
				MultiByteToWideChar(932, 0, JpStr, lstrlenA(JpStr), UniStr, 2000);
				WideCharToMultiByte(CP_UTF8, 0, UniStr, lstrlenW(UniStr), UtfStr, 4000, nullptr, nullptr);

				RtlZeroMemory(JpStr, 2000);
				wsprintfA(JpStr, "[0x%08x]", Offset);
				OutFile.Write((PBYTE)JpStr, lstrlenA(JpStr));
				OutFile.Write((PBYTE)UtfStr, lstrlenA(UtfStr));
				OutFile.Write((PBYTE)"\r\n;", 3);
				OutFile.Write((PBYTE)JpStr, lstrlenA(JpStr));
				OutFile.Write((PBYTE)"\r\n\r\n", 4);
			}
			else
			{
				iPos++;
			}
		}
		if(Buffer)
			HeapFree(GetProcessHeap(), 0, Buffer);

	} while (0);
	File.Release();
	OutFile.Release();
	return 0;
}

