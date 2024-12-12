#include "stdafx.h"
#include "PACK.h"
#include <psapi.h>
#include "../Shell/Shell.h"
#pragma comment(lib,"../Debug/Shell.lib")

CPACK::CPACK()
{
}


CPACK::~CPACK()
{
}

//************************************************************
// ��������:	Pack
// ����˵��:	�ӿ�
// ��	��:	cyxvc
// ʱ	��:	2015/12/25
// ��	��:	CString strFilePath
// ��	��:	BOOL bIsShowMesBox
// �� ��	ֵ:	BOOL
//************************************************************
BOOL CPACK::Pack(CString strFilePath, BOOL bIsShowMesBox)
{
	//1.��ȡPE�ļ���Ϣ������
	CPE objPE;
	if (objPE.InitPE(strFilePath) == FALSE)
		return FALSE;

	//2.���ܴ���β���
	DWORD dwXorSize = 0;
	dwXorSize=objPE.XorCode(0x15);

	//3.����Ҫ����Ϣ���浽Shell
	HMODULE hShell = LoadLibrary(L"AyamiKaze.dll");
	if (hShell == NULL)
	{
		MessageBox(NULL, _T("Load AyamiKaze.dll error."), _T("Attention"), MB_OK);
		//�ͷ���Դ
		delete[] objPE.m_pFileBuf;
		return FALSE;
	}

	PSHELL_DATA pstcShellData = (PSHELL_DATA)GetProcAddress(hShell, "g_stcShellData");

	pstcShellData->dwXorKey = 0x15;
	pstcShellData->dwCodeBase = objPE.m_dwCodeBase;
	pstcShellData->dwXorSize = dwXorSize;
	pstcShellData->dwPEOEP = objPE.m_dwPEOEP;
	pstcShellData->dwPEImageBase = objPE.m_dwImageBase;
	pstcShellData->stcPERelocDir = objPE.m_PERelocDir;
	pstcShellData->stcPEImportDir = objPE.m_PEImportDir;
	pstcShellData->dwIATSectionBase = objPE.m_IATSectionBase;
	pstcShellData->dwIATSectionSize = objPE.m_IATSectionSize;
	pstcShellData->bIsShowMesBox = bIsShowMesBox;

	//4.��Shell���ӵ�PE�ļ�
	//4.1.��ȡShell����
	MODULEINFO modinfo = { 0 };
	GetModuleInformation(GetCurrentProcess(), hShell, &modinfo, sizeof(MODULEINFO));
	PBYTE  pShellBuf = new BYTE[modinfo.SizeOfImage];
	memcpy_s(pShellBuf, modinfo.SizeOfImage, hShell, modinfo.SizeOfImage);
	//4.2.����Shell�ض�λ��Ϣ
	objPE.SetShellReloc(pShellBuf, (DWORD)hShell);	
	//4.3.�޸ı��ӿǳ����OEP��ָ��Shell
	DWORD dwShellOEP = pstcShellData->dwStartFun - (DWORD)hShell;
	objPE.SetNewOEP(dwShellOEP);
	//4.4.�ϲ�PE�ļ���Shell�Ĵ��뵽�µĻ�����
	LPBYTE pFinalBuf = NULL;
	DWORD dwFinalBufSize = 0;
	objPE.MergeBuf(objPE.m_pFileBuf, objPE.m_dwImageSize,
		pShellBuf, modinfo.SizeOfImage, 
		pFinalBuf, dwFinalBufSize);

	//5.�����ļ���������ɵĻ�������
	SaveFinalFile(pFinalBuf, dwFinalBufSize, strFilePath);
	
	//6.�ͷ���Դ
	delete[] objPE.m_pFileBuf;
	delete[] pShellBuf;
	delete[] pFinalBuf;
	objPE.InitValue();

	return TRUE;
}


//************************************************************
// ��������:	SaveFinalFile
// ����˵��:	�����ļ�����
// ��	��:	cyxvc
// ʱ	��:	2015/12/25
// ��	��:	LPBYTE pFinalBuf
// ��	��:	DWORD pFinalBufSize
// ��	��:	CString strFilePath
// �� ��	ֵ:	BOOL
//************************************************************
BOOL CPACK::SaveFinalFile(LPBYTE pFinalBuf, DWORD pFinalBufSize, CString strFilePath)
{
	//����������Ϣ�� �ļ������С���ļ������Сͬ�ڴ�����С��
	PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)pFinalBuf;
	PIMAGE_NT_HEADERS pNtHeader = (PIMAGE_NT_HEADERS)(pFinalBuf + pDosHeader->e_lfanew);
	PIMAGE_SECTION_HEADER pSectionHeader = IMAGE_FIRST_SECTION(pNtHeader);
	for (DWORD i = 0; i < pNtHeader->FileHeader.NumberOfSections; i++, pSectionHeader++)
	{
		pSectionHeader->PointerToRawData = pSectionHeader->VirtualAddress;
	}

	//�������Ҫ��Ŀ¼����Ϣ
	//ֻ��������ض�λ����Դ��
	DWORD dwCount = 15;
	for (DWORD i = 0; i < dwCount; i++)
	{
		if (i != IMAGE_DIRECTORY_ENTRY_EXPORT && 
			i != IMAGE_DIRECTORY_ENTRY_RESOURCE &&
			i != IMAGE_DIRECTORY_ENTRY_BASERELOC )
		{
			pNtHeader->OptionalHeader.DataDirectory[i].VirtualAddress = 0;
			pNtHeader->OptionalHeader.DataDirectory[i].Size = 0;
		}
	}

	//��ȡ����·��
	TCHAR strOutputPath[MAX_PATH] = { 0 };
	LPWSTR strSuffix = PathFindExtension(strFilePath);
	wcsncpy_s(strOutputPath, MAX_PATH, strFilePath, wcslen(strFilePath));
	PathRemoveExtension(strOutputPath);
	wcscat_s(strOutputPath, MAX_PATH, L"_crack");
	wcscat_s(strOutputPath, MAX_PATH, strSuffix);

	//�����ļ�
	HANDLE hNewFile = CreateFile(
		strOutputPath,
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hNewFile == INVALID_HANDLE_VALUE)
	{
		MessageBox(NULL, _T("Can't save file."), _T("Attention"), MB_OK);
		return FALSE;
	}
	DWORD WriteSize = 0;
	BOOL bRes = WriteFile(hNewFile, pFinalBuf, pFinalBufSize, &WriteSize, NULL);
	if (bRes)
	{
		CloseHandle(hNewFile);
		return TRUE;
	}
	else
	{
		CloseHandle(hNewFile);
		MessageBox(NULL, _T("Can't save file."), _T("Attention"), MB_OK);
		return FALSE;
	}
}
