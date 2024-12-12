#include "pack.h"

struct TypeOffset
{
	WORD Offset : 12;
	WORD Type : 4;
};

VOID Pack::print(CHAR* Str) {
	printf(Str);
	system("pause");
	ExitProcess(0);
}
PIMAGE_DOS_HEADER Pack::pDosHead(DWORD Base) {
	return (PIMAGE_DOS_HEADER)Base;
}
PIMAGE_NT_HEADERS Pack::pNtHead(DWORD Base) {
	return (PIMAGE_NT_HEADERS)(pDosHead(Base)->e_lfanew + Base);
}
PIMAGE_FILE_HEADER Pack::pFileHead(DWORD Base) {
	return &(pNtHead(Base)->FileHeader);
}
PIMAGE_OPTIONAL_HEADER Pack::pOptionHead(DWORD Base) {
	return &(pNtHead(Base)->OptionalHeader);
}

//�򿪡���ȡ���ж��ļ�
VOID Pack::OpenPeFile(LPCSTR Path) {
	HANDLE hFile = CreateFile(Path, GENERIC_READ, FILE_SHARE_READ, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) 
		print("���ļ�ʧ��");
	PeSize = GetFileSize(hFile, NULL);
	PeBase = (DWORD)malloc(PeSize);
	DWORD ReadSize = 0;
	ReadFile(hFile, (LPVOID)PeBase, PeSize, &ReadSize, NULL);
	if (pDosHead(PeBase)->e_magic!=0x5A4D)
		print("������Ч��PE�ļ�");	
	else if (pNtHead(PeBase)->Signature != 0x4550)
		print("������Ч��PE�ļ�");
	CloseHandle(hFile);
}

//���ݴ�С�Լ��������Ȼ�ȡ�����С
DWORD Pack::AlignmentToSize(DWORD Size, DWORD Algmt) {
	return (Size / Algmt)?(((Size / Algmt) + 1)*Algmt):Algmt;
}

//�� �Ǵ����DLL �ļ�����ȡ��ַ
VOID  Pack::OpenDllFile(LPCSTR Path) {
	//����ģ�飬���Ҳ�����DllMain
	DllBase = (DWORD)LoadLibraryEx(Path, NULL, DONT_RESOLVE_DLL_REFERENCES);
	//��ȡ��������StartOfsset��ַ
	StartOfsset = (DWORD)GetProcAddress((HMODULE)DllBase, "start");
	if (!StartOfsset)
		print("��ȡDLLMain�ĵ�������ʧ��");
	StartOfsset = StartOfsset - DllBase - (DWORD)GetSecHeadAddr(DllBase, DllSecName)->VirtualAddress;
	//��ȡͨѶ�ṹ��,�ӿ�����Ҫʹ�õ�
	pShareData = (SHARE*)GetProcAddress((HMODULE)DllBase, "ShareData");
}

//��ȡָ�����ε��׵�ַ
PIMAGE_SECTION_HEADER  Pack::GetSecHeadAddr(DWORD ModuleBase,LPCSTR Name) {
	//��������
	DWORD NumberOfSec = pFileHead(ModuleBase)->NumberOfSections;
	auto FristSec = IMAGE_FIRST_SECTION(pNtHead(ModuleBase));
	//��������
	for (size_t i = 0; i < NumberOfSec; i++)
	{
		if (!memcmp(FristSec[i].Name,Name, strlen(Name)+1)) 
			return &FristSec[i];
	}
	print("û���ҵ�����");
	return 0;
}

//������������
VOID Pack::CopySecData(CHAR* DestName) {	
	//����Դ��ַ
	CHAR* Src = (CHAR*)(GetSecHeadAddr(DllBase, DllSecName)->VirtualAddress+DllBase);
	//����Ŀ���ַ
	CHAR* Dest = (CHAR*)(GetSecHeadAddr(PeBase, DestName)->PointerToRawData+PeBase);
	memcpy(Dest, Src, GetSecHeadAddr(DllBase, DllSecName)->Misc.VirtualSize);
	return;
}

//��Ӳ�����һ��������
VOID Pack::CopySection(LPCSTR DestName) {
	if (strlen(DestName) > 7)
		print("����������̫��");
	//��ȡ��Ҫ��ӵ�����
	auto pLastSection = IMAGE_FIRST_SECTION(pNtHead(PeBase)) + (pFileHead(PeBase)->NumberOfSections - 1);
	auto pNewSection = pLastSection + 1;
	//��ȡ����ͷ���׵�ַ
	auto SrcAddr = GetSecHeadAddr(DllBase, DllSecName);
	//��������ͷ
	memcpy(pNewSection, SrcAddr, sizeof(IMAGE_SECTION_HEADER));
	//�޸�����
	pNewSection->VirtualAddress = pLastSection->VirtualAddress + 
		AlignmentToSize(pLastSection->Misc.VirtualSize, pOptionHead(PeBase)->SectionAlignment);
	pNewSection->PointerToRawData = pLastSection->PointerToRawData +
		AlignmentToSize(pLastSection->SizeOfRawData, pOptionHead(PeBase)->FileAlignment);
	// Name
	memcpy(pNewSection->Name, DestName, strlen(DestName));
	// NumberOfSections
	pFileHead(PeBase)->NumberOfSections += 1;
	// SizeOfImage��С
	pOptionHead(PeBase)->SizeOfImage += 
		AlignmentToSize(pNewSection->Misc.VirtualSize, pOptionHead(PeBase)->SectionAlignment);
	// ���·���ռ䣬����������ӵ��ѿռ���
	PeSize = pNewSection->SizeOfRawData + pNewSection->PointerToRawData;
	PeBase = (DWORD)realloc((VOID*)PeBase, PeSize);	
	return;
}

//����OEP
VOID Pack::SetOep() {
	
	//���ض�λ֮ǰ������ԭ����OEP
	pShareData->DestOEP = pOptionHead(PeBase)->AddressOfEntryPoint;
	//����OEPΪ�Ǵ���OEP
	pOptionHead(PeBase)->AddressOfEntryPoint = StartOfsset + GetSecHeadAddr(PeBase,".pack")->VirtualAddress;	
}

//�����ļ�д�����
VOID Pack::WriteToFile(LPCSTR Path) {
	HANDLE hFile = CreateFile(Path, GENERIC_WRITE, 0, NULL, 
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	DWORD WriteSize = 0;
	DWORD Ret = WriteFile(hFile, (LPVOID)PeBase, PeSize, &WriteSize, NULL);
	if (!Ret)
		print("д���ļ�ʧ��");
	CloseHandle(hFile);
	return;
}

//DLL�кϲ���������
VOID Pack::SetDllSecName(CHAR* SecName) {
	memcpy(DllSecName, SecName, strlen(SecName));
	return;
}

// �ԿǴ�������ض�λ
VOID Pack::FixReloc()
{
	DWORD OldImageBase = pOptionHead(DllBase)->ImageBase;
	DWORD NewImageBase = pOptionHead(PeBase)->ImageBase;
	DWORD OldSectionBase = GetSecHeadAddr(DllBase, ".text")->VirtualAddress;
	DWORD NewSectionBase = GetSecHeadAddr(PeBase, ".pack")->VirtualAddress;

	auto Relocs = (PIMAGE_BASE_RELOCATION)(DllBase + pOptionHead(DllBase)->DataDirectory[5].VirtualAddress);

	// �����ض�λ��
	while (Relocs->SizeOfBlock)
	{
		DWORD OldProtect = 0;
		VirtualProtect((LPVOID)(DllBase + Relocs->VirtualAddress), 0x1000, PAGE_READWRITE, &OldProtect);
		TypeOffset* items = (TypeOffset*)(Relocs + 1);
		//�ض�λ������
		int count = (Relocs->SizeOfBlock - 8) / 2;
		//�����ض�λ��
		for (int i = 0; i < count; ++i)
		{			
			if (items[i].Type == 3)
			{
				DWORD* item = (DWORD*)(pOptionHead(DllBase)->ImageBase + Relocs->VirtualAddress + items[i].Offset);
				*item = *item - OldImageBase - OldSectionBase + NewImageBase + NewSectionBase;
			}
		}
		VirtualProtect((LPVOID)(DllBase + Relocs->VirtualAddress), 0x1000, OldProtect, &OldProtect);
		Relocs = (PIMAGE_BASE_RELOCATION)(Relocs->SizeOfBlock + (DWORD)Relocs);
	}
	// �ر�Դ����������ַ
	//pOptionHead(PeBase)->DllCharacteristics &= ~IMAGE_DLLCHARACTERISTICS_DYNAMIC_BASE;
}

//��������
VOID Pack::Encryption(CHAR* DestName) {
	//�ҵ���Ҫ���ܵ�����
	auto SectionHead = GetSecHeadAddr(PeBase, DestName);
	DWORD Size = SectionHead->Misc.VirtualSize;
	CHAR* Address =(CHAR*)(SectionHead->PointerToRawData + PeBase);
	for (size_t i = 0; i < Size; i++)
	{
		*Address ^= 0x15;
		Address++;
	}
}

//��Ǵ���ӹ�IAT��
VOID Pack::SetIatTo0() {
	pShareData->ImpTableVA = pOptionHead(PeBase)->DataDirectory[1].VirtualAddress;// ���ض�λ֮ǰ����
	pOptionHead(PeBase)->DataDirectory[1].VirtualAddress = 0;
	pOptionHead(PeBase)->DataDirectory[12].VirtualAddress = 0;
}

//���һ��������
VOID Pack::AddSection(LPCSTR Name, DWORD Size) {
	PIMAGE_DOS_HEADER pDos = pDosHead(PeBase);
	PIMAGE_NT_HEADERS pNt = pNtHead(PeBase);
	//��ȡ��Ҫ��ӵ�����
	PIMAGE_SECTION_HEADER pLastSection = (IMAGE_FIRST_SECTION(pNt)) + (pNt->FileHeader.NumberOfSections - 1);
	PIMAGE_SECTION_HEADER pNewSection = pLastSection + 1;
	//Name
	memcpy(pNewSection->Name, Name, 7);
	//VirtualSize
	pNewSection->Misc.VirtualSize = AlignmentToSize(Size, pNt->OptionalHeader.SectionAlignment);
	//VirtualAddress = ���һ�����ε� VirtualAddress +���һ�������ڴ��С
	pNewSection->VirtualAddress = pLastSection->VirtualAddress +
		AlignmentToSize(pLastSection->Misc.VirtualSize, pNt->OptionalHeader.SectionAlignment);
	//SizeOfRawData
	pNewSection->SizeOfRawData = AlignmentToSize(Size, pNt->OptionalHeader.FileAlignment);
	//PointerToRawData = ���һ�����ε� PointerToRawData + ���һ�������ļ���С
	pNewSection->PointerToRawData = pLastSection->PointerToRawData +
		AlignmentToSize(pLastSection->SizeOfRawData, pNt->OptionalHeader.FileAlignment);
	//Characteristics
	pNewSection->Characteristics = 0xE00000E0;
	//NumberOfSections
	pNt->FileHeader.NumberOfSections += 1;
	//SizeOfImage��С
	pNt->OptionalHeader.SizeOfImage += AlignmentToSize(pNewSection->Misc.VirtualSize, pNt->OptionalHeader.SectionAlignment);
	//���·���ռ䣬����������ӵ��ѿռ���
	PeSize = pNewSection->SizeOfRawData + pNewSection->PointerToRawData;
	PeBase = (DWORD)realloc((VOID*)PeBase, PeSize);
	pNewSection = GetSecHeadAddr(PeBase, Name);
	memset((DWORD*)(pNewSection->PointerToRawData + PeBase), 0, pNewSection->SizeOfRawData);
	return;
}

//���ݲ��滻�ض�λ��
VOID Pack::SetReloc() {
	//1.����ԭ������ض�λ���ַ����ԭ������ض�λд�뵽ͨѶ�ṹ���У���dll���ض�λ���滻��ԭ������ض�λ��
	//2.�ڿǴ����У�ϵͳ��������޸��Ǵ�����ض�λ����ԭ�������֮���ڿǴ����л�ȡͨѶ�ṹ�壬��ȡԭ�����ض�λ���޸�ԭ�����ض�λ

	DWORD DllImageBase = pOptionHead(DllBase)->ImageBase;

	//����ԭ�ض�λ��
	pShareData->dwRelocRva = pOptionHead(PeBase)->DataDirectory[5].VirtualAddress;
	pShareData->dwRelocSize = pOptionHead(PeBase)->DataDirectory[5].Size;
	pShareData->OldImageBase = pOptionHead(PeBase)->ImageBase;
	//Dll�ض�λ��
	auto DllBaseReloc = (PIMAGE_BASE_RELOCATION)(pOptionHead(DllBase)->DataDirectory[5].VirtualAddress + DllImageBase);
	DWORD DllRelocaSize = pOptionHead(DllBase)->DataDirectory[5].Size;
	
	//��������
	AddSection(".mysec", DllRelocaSize);
	auto NewSecHed = GetSecHeadAddr(PeBase, ".mysec");
	auto OldSecHed = GetSecHeadAddr(DllBase, ".text");
	auto PackSecHed = GetSecHeadAddr(PeBase, ".pack");
	auto NewRelocaSection = (PIMAGE_BASE_RELOCATION)(NewSecHed->PointerToRawData + PeBase);
	DWORD OldSectionAddr = (DWORD)(OldSecHed->VirtualAddress + DllBase);

	memcpy((DWORD*)NewRelocaSection, (DWORD*)(DllBaseReloc), DllRelocaSize);
	while (NewRelocaSection->VirtualAddress)
	{
		//�µ��ڴ�ҳ��ʼRVA = ԭRVA - ԭ�λ�ַ +.pack�λ�ַ
		NewRelocaSection->VirtualAddress = NewRelocaSection->VirtualAddress - (OldSectionAddr -DllBase) + PackSecHed->VirtualAddress;
		NewRelocaSection = (PIMAGE_BASE_RELOCATION)(NewRelocaSection->SizeOfBlock + (DWORD)NewRelocaSection);
	}	
	//�滻ԭ�����ض�λ��
	pOptionHead(PeBase)->DataDirectory[5].VirtualAddress = NewSecHed->VirtualAddress;
	pOptionHead(PeBase)->DataDirectory[5].Size = DllRelocaSize;	
}

DWORD RvaToOffset(DWORD lpImage, DWORD dwRva)
{
	PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)lpImage;
	PIMAGE_NT_HEADERS32 pNT32 = (PIMAGE_NT_HEADERS32)((LONG)lpImage + pDos->e_lfanew);
	PIMAGE_FILE_HEADER pFileHeader = &pNT32->FileHeader;
	PIMAGE_SECTION_HEADER pSeciton = IMAGE_FIRST_SECTION(pNT32);
	for (int i = 0; i < pFileHeader->NumberOfSections; i++)
	{
		if (dwRva > pSeciton->VirtualAddress&& dwRva < pSeciton->VirtualAddress + pSeciton->SizeOfRawData)
		{
			DWORD dwChazhi = dwRva - pSeciton->VirtualAddress;
			return pSeciton->PointerToRawData + dwChazhi;
		}
		pSeciton++;
	}
}

//����TLS�����TLS
VOID Pack::SetTls() {
	pShareData->TlsVirtualAddress = pOptionHead(PeBase)->DataDirectory[9].VirtualAddress;
	if (pShareData->TlsVirtualAddress) {
		pOptionHead(PeBase)->DataDirectory[9].VirtualAddress = 0;
		DWORD TlsFoa = RvaToOffset(PeBase, pShareData->TlsVirtualAddress);
		auto TlsTable = (PIMAGE_TLS_DIRECTORY)(TlsFoa + PeBase);
		pShareData->TlsCallBackTableVa = TlsTable->AddressOfCallBacks;
	}
}

//ѹ��
VOID Pack::Compression()
{	
	int SecTextSize = GetSecHeadAddr(PeBase, ".text")->SizeOfRawData;
	pShareData->SizeOfRawData = SecTextSize;
	char* TextSecData = (char*)(GetSecHeadAddr(PeBase, ".text")->PointerToRawData + PeBase);
	//��ѹ��������,Packed����ѹ�����ݵĿռ䣬WorkMemΪ���ѹ����Ҫʹ�õĿռ�
	char* Packed = (char*)malloc(aP_max_packed_size(SecTextSize));
	char* WorkMem = (char*)malloc(aP_workmem_size(SecTextSize));
	//ѹ����Ĵ�С
	int OutSize = aPsafe_pack(TextSecData, Packed, SecTextSize, WorkMem, NULL, NULL);
	//�����Ĵ�С
	int AlignSize = OutSize % 0x200 == 0 ? OutSize : (OutSize / 0x200 + 1) * 0x200;
	//�¿ռ��С
	DWORD NewFileSize = PeSize - GetSecHeadAddr(PeBase, ".text")->SizeOfRawData + AlignSize;
	//�����µĿռ��С �ļ���С - �������ļ��еĴ�С + ѹ����Ĵ�С(������)
	DWORD NewFileBase = (DWORD)malloc(NewFileSize);
	//TextSecData֮ǰ������
	DWORD PreText = GetSecHeadAddr(PeBase, ".text")->PointerToRawData - 1;
	//����TextSecData��֮ǰ������
	memcpy((LPVOID)NewFileBase, (LPVOID)PeBase, PreText);
	//����ѹ�����ֵ�����
	memcpy((LPVOID)(NewFileBase + PreText + 1), Packed, OutSize);
	//����TextSecData�κ��������
	LPVOID DestAddr = (LPVOID)(NewFileBase + PreText + AlignSize + 1);
	DWORD TextSecSize = GetSecHeadAddr(PeBase, ".text")->SizeOfRawData;
	DWORD TextSecPointRaw = GetSecHeadAddr(PeBase, ".text")->PointerToRawData;
	LPVOID SrcAddr = (LPVOID)(PeBase + TextSecSize + TextSecPointRaw);
	DWORD LastSize = NewFileSize - PreText - AlignSize;
	memcpy(DestAddr, SrcAddr, LastSize);
	//free(&FileBase);
	PeBase = NewFileBase;
	//free((void*)NewFileBase);
	NewFileBase = NULL;
	// 1. ��ȡ��Ŀ��ģ������α�	
	auto Sections = IMAGE_FIRST_SECTION(pNtHead(PeBase));
	// 2. ʹ���ļ�ͷ�е����������������α�	
	WORD Count = pFileHead(PeBase)->NumberOfSections;
	BOOL bChangeFoa = FALSE;
	for (WORD i = 0; i < Count; ++i)
	{
		if (bChangeFoa) {
			Sections[i].PointerToRawData = Sections[i].PointerToRawData - GetSecHeadAddr(PeBase, ".text")->SizeOfRawData + AlignSize;
		}
		// 3. .text����֮ǰ�����β��ı�,����.text����֮�������
		if (!memcmp(Sections[i].Name, ".text", 7)) {
			bChangeFoa = TRUE;
		}
	}	
	pShareData->TextCompressSize = OutSize;
	GetSecHeadAddr(PeBase, ".text")->SizeOfRawData = AlignSize;
	pShareData->TextRVA = GetSecHeadAddr(PeBase, ".text")->VirtualAddress;
	PeSize = NewFileSize;
}