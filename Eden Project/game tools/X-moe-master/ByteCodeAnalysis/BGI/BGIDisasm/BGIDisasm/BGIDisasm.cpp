#include <Windows.h>
#include "Instruction.h"
#include "WinFile.h"
#include <string>
#include "ScriptHeader.h"

using std::wstring;

BOOL JPMode = FALSE;


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
		JPMode = TRUE;
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

	ScriptHeader Header = { 0 };
	ULONG iPos = 0;
	ULONG StreamSize = Size;

	memcpy(&Header, Buffer, sizeof(ScriptHeader));
	iPos = sizeof(ScriptHeader) - sizeof(ULONG);
	iPos += Header.HeaderSize;

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
		GetInstructionInfo(Buffer, iPos, OpString);
		File.Write((PBYTE)OpString.c_str(), OpString.length());
		File.Write((PBYTE)"\r\n", 2);
	}

	File.Release();
	return S_OK;
}

