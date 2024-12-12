#include "stdafx.h"
#include "PE.h"


CPE::CPE()
{
	InitValue();
}


CPE::~CPE()
{
}

//************************************************************
// ��������:	InitValue
// ����˵��:	��ʼ������
// ��	��:	cyxvc
// ʱ	��:	2015/12/25
// �� ��	ֵ:	void
//************************************************************
void CPE::InitValue()
{
	m_hFile				= NULL;
	m_pFileBuf			= NULL;
	m_pDosHeader		= NULL;
	m_pNtHeader			= NULL;
	m_pSecHeader		= NULL;
	m_dwFileSize		= 0;
	m_dwImageSize		= 0;
	m_dwImageBase		= 0;
	m_dwCodeBase		= 0;
	m_dwCodeSize		= 0;
	m_dwPEOEP			= 0;
	m_dwShellOEP		= 0;
	m_dwSizeOfHeader	= 0;
	m_dwSectionNum		= 0;
	m_dwFileAlign		= 0;
	m_dwMemAlign		= 0;
	m_PERelocDir		= { 0 };
	m_PEImportDir		= { 0 };
	m_IATSectionBase	= 0;
	m_IATSectionSize	= 0;
}

//************************************************************
// ��������:	InitPE
// ����˵��:	��ʼ��PE����ȡPE�ļ�������PE��Ϣ
// ��	��:	cyxvc
// ʱ	��:	2015/12/25
// ��	��:	CString strFilePath
// �� ��	ֵ:	BOOL
//************************************************************
BOOL CPE::InitPE(CString strFilePath)
{
	//���ļ�
	if (OpenPEFile(strFilePath) == FALSE)
		return FALSE;

	//��PE���ļ��ֲ���ʽ��ȡ���ڴ�
	m_dwFileSize = GetFileSize(m_hFile, NULL);
	m_pFileBuf = new BYTE[m_dwFileSize];
	DWORD ReadSize = 0;
	ReadFile(m_hFile, m_pFileBuf, m_dwFileSize, &ReadSize, NULL);	
	CloseHandle(m_hFile);
	m_hFile = NULL;

	//�ж��Ƿ�ΪPE�ļ�
	if (IsPE() == FALSE)
		return FALSE;

	//��PE���ڴ�ֲ���ʽ��ȡ���ڴ�
	//����û�����Сû�ж�������
	m_dwImageSize = m_pNtHeader->OptionalHeader.SizeOfImage;
	m_dwMemAlign = m_pNtHeader->OptionalHeader.SectionAlignment;
	m_dwSizeOfHeader = m_pNtHeader->OptionalHeader.SizeOfHeaders;
	m_dwSectionNum = m_pNtHeader->FileHeader.NumberOfSections;

	if (m_dwImageSize % m_dwMemAlign)
		m_dwImageSize = (m_dwImageSize / m_dwMemAlign + 1) * m_dwMemAlign;
	LPBYTE pFileBuf_New = new BYTE[m_dwImageSize];
	memset(pFileBuf_New, 0, m_dwImageSize);
	//�����ļ�ͷ
	memcpy_s(pFileBuf_New, m_dwSizeOfHeader, m_pFileBuf, m_dwSizeOfHeader);
	//��������
	PIMAGE_SECTION_HEADER pSectionHeader = IMAGE_FIRST_SECTION(m_pNtHeader);
	for (DWORD i = 0; i < m_dwSectionNum; i++, pSectionHeader++)
	{
		memcpy_s(pFileBuf_New + pSectionHeader->VirtualAddress,
			pSectionHeader->SizeOfRawData,
			m_pFileBuf+pSectionHeader->PointerToRawData,
			pSectionHeader->SizeOfRawData);
	}
	delete[] m_pFileBuf;
	m_pFileBuf = pFileBuf_New;
	pFileBuf_New = NULL;

	//��ȡPE��Ϣ
	GetPEInfo();
	
	return TRUE;
}

//************************************************************
// ��������:	OpenPEFile
// ����˵��:	���ļ�
// ��	��:	cyxvc
// ʱ	��:	2015/12/25
// ��	��:	CString strFilePath
// �� ��	ֵ:	BOOL
//************************************************************
BOOL CPE::OpenPEFile(CString strFilePath)
{
	m_hFile = CreateFile(strFilePath,
		GENERIC_READ | GENERIC_WRITE, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (m_hFile == INVALID_HANDLE_VALUE)
	{
		MessageBox(NULL, _T("Load file error."), _T("Attention"), MB_OK);
		m_hFile = NULL;
		return FALSE;
	}
	return TRUE;
}

//************************************************************
// ��������:	IsPE
// ����˵��:	�ж��Ƿ�ΪPE�ļ�
// ��	��:	cyxvc
// ʱ	��:	2015/12/25
// �� ��	ֵ:	BOOL
//************************************************************
BOOL CPE::IsPE()
{
	//�ж��Ƿ�ΪPE�ļ�
	m_pDosHeader = (PIMAGE_DOS_HEADER)m_pFileBuf;
	if (m_pDosHeader->e_magic != IMAGE_DOS_SIGNATURE)
	{
		//����PE
		MessageBox(NULL, _T("Check PE error"), _T("Attention"), MB_OK);
		delete[] m_pFileBuf;
		InitValue();
		return FALSE;
	}
	m_pNtHeader = (PIMAGE_NT_HEADERS)(m_pFileBuf + m_pDosHeader->e_lfanew);
	if (m_pNtHeader->Signature != IMAGE_NT_SIGNATURE)
	{
		//����PE�ļ�
		MessageBox(NULL, _T("Check PE error"), _T("Attention"), MB_OK);
		delete[] m_pFileBuf;
		InitValue();
		return FALSE;
	}
	return TRUE;
}

//************************************************************
// ��������:	GetPEInfo
// ����˵��:	��ȡPE��Ϣ
// ��	��:	cyxvc
// ʱ	��:	2015/12/25
// �� ��	ֵ:	void
//************************************************************
void CPE::GetPEInfo()
{
	m_pDosHeader	= (PIMAGE_DOS_HEADER)m_pFileBuf;
	m_pNtHeader		= (PIMAGE_NT_HEADERS)(m_pFileBuf + m_pDosHeader->e_lfanew);

	m_dwFileAlign	= m_pNtHeader->OptionalHeader.FileAlignment;
	m_dwMemAlign	= m_pNtHeader->OptionalHeader.SectionAlignment;
	m_dwImageBase	= m_pNtHeader->OptionalHeader.ImageBase;
	m_dwPEOEP		= m_pNtHeader->OptionalHeader.AddressOfEntryPoint;
	m_dwCodeBase	= m_pNtHeader->OptionalHeader.BaseOfCode;
	m_dwCodeSize	= m_pNtHeader->OptionalHeader.SizeOfCode;
	m_dwSizeOfHeader= m_pNtHeader->OptionalHeader.SizeOfHeaders;
	m_dwSectionNum	= m_pNtHeader->FileHeader.NumberOfSections;
	m_pSecHeader	= IMAGE_FIRST_SECTION(m_pNtHeader);
	m_pNtHeader->OptionalHeader.SizeOfImage = m_dwImageSize;

	//�����ض�λĿ¼��Ϣ
	m_PERelocDir = 
		IMAGE_DATA_DIRECTORY(m_pNtHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC]);

	//����IAT��ϢĿ¼��Ϣ
	m_PEImportDir =
		IMAGE_DATA_DIRECTORY(m_pNtHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT]);

	//��ȡIAT���ڵ����ε���ʼλ�úʹ�С
	PIMAGE_SECTION_HEADER pSectionHeader = IMAGE_FIRST_SECTION(m_pNtHeader);
	for (DWORD i = 0; i < m_dwSectionNum; i++, pSectionHeader++)
	{
		if (m_PEImportDir.VirtualAddress >= pSectionHeader->VirtualAddress&&
			m_PEImportDir.VirtualAddress <= pSectionHeader[1].VirtualAddress)
		{
			//��������ε���ʼ��ַ�ʹ�С
			m_IATSectionBase = pSectionHeader->VirtualAddress;
			m_IATSectionSize = pSectionHeader[1].VirtualAddress - pSectionHeader->VirtualAddress;
			break;
		}
	}
}

//************************************************************
// ��������:	XorCode
// ����˵��:	����μ���
// ��	��:	cyxvc
// ʱ	��:	2015/12/25
// ��	��:	BYTE byXOR
// �� ��	ֵ:	DWORD
//************************************************************
DWORD CPE::XorCode(BYTE byXOR)
{
	PBYTE pCodeBase = (PBYTE)((DWORD)m_pFileBuf + m_dwCodeBase);
	for (DWORD i = 0; i < m_dwCodeSize; i++)
	{
		pCodeBase[i] ^= i;
	}
	return m_dwCodeSize;
}

//************************************************************
// ��������:	SetShellReloc
// ����˵��:	����Shell���ض�λ��Ϣ
// ��	��:	cyxvc
// ʱ	��:	2015/12/25
// ��	��:	LPBYTE pShellBuf
// �� ��	ֵ:	BOOL
//************************************************************
BOOL CPE::SetShellReloc(LPBYTE pShellBuf, DWORD hShell)
{
	typedef struct _TYPEOFFSET
	{
		WORD offset : 12;			//ƫ��ֵ
		WORD Type	: 4;			//�ض�λ����(��ʽ)
	}TYPEOFFSET, *PTYPEOFFSET;

	//1.��ȡ���ӿ�PE�ļ����ض�λĿ¼��ָ����Ϣ
	PIMAGE_DATA_DIRECTORY pPERelocDir =
		&(m_pNtHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC]);
	
	//2.��ȡShell���ض�λ��ָ����Ϣ
	PIMAGE_DOS_HEADER		pShellDosHeader = (PIMAGE_DOS_HEADER)pShellBuf;
	PIMAGE_NT_HEADERS		pShellNtHeader = (PIMAGE_NT_HEADERS)(pShellBuf + pShellDosHeader->e_lfanew);
	PIMAGE_DATA_DIRECTORY	pShellRelocDir =
		&(pShellNtHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC]);
	PIMAGE_BASE_RELOCATION	pShellReloc = 
		(PIMAGE_BASE_RELOCATION)((DWORD)pShellBuf + pShellRelocDir->VirtualAddress);
	
	//3.��ԭ�޸��ض�λ��Ϣ
	//����Shell.dll��ͨ��LoadLibrary���صģ�����ϵͳ��������һ���ض�λ
	//������Ҫ��Shell.dll���ض�λ��Ϣ�ָ���ϵͳû����ǰ�����ӣ�Ȼ����д�뱻�ӿ��ļ���ĩβ
	PTYPEOFFSET pTypeOffset = (PTYPEOFFSET)(pShellReloc + 1);
	DWORD dwNumber = (pShellReloc->SizeOfBlock - 8) / 2;

	for (DWORD i = 0; i < dwNumber; i++)
	{
		if (*(PWORD)(&pTypeOffset[i]) == NULL)
			break;
		//RVA
		DWORD dwRVA =pTypeOffset[i].offset + pShellReloc->VirtualAddress;
		//FAR��ַ��LordPE��������ע��
		//***�µ��ض�λ��ַ=�ض�λ��ĵ�ַ-����ʱ�ľ����ַ+�µľ����ַ+�����ַ(PE�ļ������С)
		DWORD AddrOfNeedReloc =	*(PDWORD)((DWORD)pShellBuf + dwRVA);
		*(PDWORD)((DWORD)pShellBuf + dwRVA) 
			= AddrOfNeedReloc - pShellNtHeader->OptionalHeader.ImageBase + m_dwImageBase + m_dwImageSize;
	}
	//3.1�޸�Shell�ض�λ����.text��RVA
	pShellReloc->VirtualAddress += m_dwImageSize;

	//4.�޸�PE�ض�λĿ¼ָ�룬ָ��Shell���ض�λ����Ϣ
	pPERelocDir->Size = pShellRelocDir->Size;
	pPERelocDir->VirtualAddress = pShellRelocDir->VirtualAddress + m_dwImageSize;

	return TRUE;
}

//************************************************************
// ��������:	MergeBuf
// ����˵��:	�ϲ�PE�ļ���Shell
// ��	��:	cyxvc
// ʱ	��:	2015/12/25
// ��	��:	LPBYTE pFileBuf
// ��	��:	DWORD pFileBufSize
// ��	��:	LPBYTE pShellBuf
// ��	��:	DWORD pShellBufSize
// ��	��:	LPBYTE & pFinalBuf
// �� ��	ֵ:	void
//************************************************************
void CPE::MergeBuf(LPBYTE pFileBuf, DWORD pFileBufSize,
	LPBYTE pShellBuf, DWORD pShellBufSize, 
	LPBYTE& pFinalBuf, DWORD& pFinalBufSize)
{
	//��ȡ���һ�����ε���Ϣ
	PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)pFileBuf;
	PIMAGE_NT_HEADERS pNtHeader = (PIMAGE_NT_HEADERS)(pFileBuf + pDosHeader->e_lfanew);
	PIMAGE_SECTION_HEADER pSectionHeader = IMAGE_FIRST_SECTION(pNtHeader);
	PIMAGE_SECTION_HEADER pLastSection =
		&pSectionHeader[pNtHeader->FileHeader.NumberOfSections - 1];

	//1.�޸���������
	pNtHeader->FileHeader.NumberOfSections += 1;

	//2.�༭���α�ͷ�ṹ����Ϣ
	PIMAGE_SECTION_HEADER AddSectionHeader =
		&pSectionHeader[pNtHeader->FileHeader.NumberOfSections - 1];
	memcpy_s(AddSectionHeader->Name, 8, ".Aym", 5);

	//VOffset(1000����)
	DWORD dwTemp = 0;
	dwTemp = (pLastSection->Misc.VirtualSize / m_dwMemAlign) * m_dwMemAlign;
	if (pLastSection->Misc.VirtualSize % m_dwMemAlign)
	{
		dwTemp += 0x1000;
	}
	AddSectionHeader->VirtualAddress = pLastSection->VirtualAddress + dwTemp;

	//Vsize��ʵ����ӵĴ�С��
	AddSectionHeader->Misc.VirtualSize = pShellBufSize;

	//ROffset�����ļ���ĩβ��
	AddSectionHeader->PointerToRawData = pFileBufSize;

	//RSize(200����)
	dwTemp = (pShellBufSize / m_dwFileAlign) * m_dwFileAlign;
	if (pShellBufSize % m_dwFileAlign)
	{
		dwTemp += m_dwFileAlign;
	}
	AddSectionHeader->SizeOfRawData = dwTemp;

	//��־
	AddSectionHeader->Characteristics = 0XE0000040;

	//3.�޸�PEͷ�ļ���С���ԣ������ļ���С
	dwTemp = (pShellBufSize / m_dwMemAlign) * m_dwMemAlign;
	if (pShellBufSize % m_dwMemAlign)
	{
		dwTemp += m_dwMemAlign;
	}
	pNtHeader->OptionalHeader.SizeOfImage += dwTemp;


	//4.����ϲ�����Ҫ�Ŀռ�
	pFinalBuf = new BYTE[pFileBufSize + dwTemp];
	pFinalBufSize = pFileBufSize + dwTemp;
	memset(pFinalBuf, 0, pFileBufSize + dwTemp);
	memcpy_s(pFinalBuf, pFileBufSize, pFileBuf, pFileBufSize);
	memcpy_s(pFinalBuf + pFileBufSize, dwTemp, pShellBuf, dwTemp);
}

//************************************************************
// ��������:	SetNewOEP
// ����˵��:	�޸��µ�OEPΪShell��Start����
// ��	��:	cyxvc
// ʱ	��:	2015/12/25
// ��	��:	DWORD dwOEP
// �� ��	ֵ:	void
//************************************************************
void CPE::SetNewOEP(DWORD dwShellOEP)
{
	m_dwShellOEP = dwShellOEP + m_dwImageSize;
	m_pNtHeader->OptionalHeader.AddressOfEntryPoint = m_dwShellOEP;
}





