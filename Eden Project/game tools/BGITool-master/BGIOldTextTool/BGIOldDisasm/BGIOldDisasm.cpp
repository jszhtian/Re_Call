#include <WinFile.h>
#include "Instruction.h"
using std::wstring;

BOOL JPMode = 1;


HRESULT DisasmProc(wstring& FileName, PBYTE Buffer, ULONG Size);

int wmain(int argc, WCHAR* argv[])
{
	if (argc != 2 && argc != 3)
	{
		printf("exe file [mode]\n");
		getchar();
		return 0;
	}

	if (argc == 3)
	{
		JPMode = FALSE;
	}

	if (wcsstr(argv[1], L".txt"))
	{
		//��ֹ������233
		return 0;
	}

	WinFile File;
	if (FAILED(File.Open(argv[1], WinFile::FileRead)))
	{
		return 0;
	}

	PBYTE Buffer = (PBYTE)HeapAlloc(GetProcessHeap(), 0, File.GetSize32());
	File.Read(Buffer, File.GetSize32());

	HRESULT Result = DisasmProc((wstring(argv[1]) + L".txt"), Buffer, File.GetSize32());

	HeapFree(GetProcessHeap(), 0, Buffer);
	File.Release();
	return 0;
}


//û�м�¼�ֽ��볤�ȣ������Ǳ�����VM���= =
//Ŀǰ����ɨ���ֽ��볤�ȵķ�ʽ��
//1.VM�ֽ��뿪ʼ�ط���һ������ʱ�ļ�¼
//2.ɨ��Return(һ���ļ��ڿ����ж��Return), ɨ�����һ��DWORD�����Return
//�ֽ������һ��һ����Return ����������String Pool �ţ��Һ�û��sort

//���ϣ�ʹ�÷���2
HRESULT DisasmProc(wstring& FileName, PBYTE Buffer, ULONG Size)
{
	WinFile File;
	if (File.Open(FileName.c_str(), WinFile::FileWrite) != S_OK)
	{
		printf("Failed to open\n");
		getchar();
		return E_FAIL;
	}

	ULONG iPos = 0;
	ULONG StreamSize = Size;

	ULONG Finder = iPos;
	ULONG LastOffset = Size;
	while (Finder < Size)
	{
		if (*(PULONG)(Buffer + Finder) == 0x0000001B)
		{
			LastOffset = Finder + 4;
		}
		Finder += 4;
	}

	StreamSize = LastOffset;
	while (iPos < StreamSize)
	{
		string OpString;

#ifdef DUMP_TEXT_ONLY
		if (GetInstructionInfo(Buffer, iPos, OpString) == S_OK)
		{
#if 0 
			CHAR PrefixLineNo[100] = { 0 };
			wsprintfA(PrefixLineNo, "[0x%08x]", iPos - 4); //size of Instruction
			File.Write((PBYTE)PrefixLineNo, lstrlenA(PrefixLineNo));
			File.Write((PBYTE)OpString.c_str(), OpString.length());
			File.Write((PBYTE)"\r\n", 2);
			File.Write((PBYTE)";", 1);
			File.Write((PBYTE)PrefixLineNo, lstrlenA(PrefixLineNo));
			File.Write((PBYTE)"\r\n\r\n", 4);
#else
			File.Write((PBYTE)OpString.c_str(), OpString.length());
			File.Write((PBYTE)"\r\n", 2);
#endif
		}
#else

		GetInstructionInfo(Buffer, iPos, OpString);
		{
			File.Write((PBYTE)OpString.c_str(), OpString.length());
			File.Write((PBYTE)"\r\n", 2);
		}

#endif
	}

	File.Release();
	return S_OK;
}


