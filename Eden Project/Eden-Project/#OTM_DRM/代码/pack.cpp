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

//打开、读取、判断文件
VOID Pack::OpenPeFile(LPCSTR Path) {
	HANDLE hFile = CreateFile(Path, GENERIC_READ, FILE_SHARE_READ, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) 
		print("打开文件失败");
	PeSize = GetFileSize(hFile, NULL);
	PeBase = (DWORD)malloc(PeSize);
	DWORD ReadSize = 0;
	ReadFile(hFile, (LPVOID)PeBase, PeSize, &ReadSize, NULL);
	if (pDosHead(PeBase)->e_magic!=0x5A4D)
		print("不是有效的PE文件");	
	else if (pNtHead(PeBase)->Signature != 0x4550)
		print("不是有效的PE文件");
	CloseHandle(hFile);
}

//根据大小以及对齐粒度获取对齐大小
DWORD Pack::AlignmentToSize(DWORD Size, DWORD Algmt) {
	return (Size / Algmt)?(((Size / Algmt) + 1)*Algmt):Algmt;
}

//打开 壳代码的DLL 文件，获取基址
VOID  Pack::OpenDllFile(LPCSTR Path) {
	//加载模块，并且不调用DllMain
	DllBase = (DWORD)LoadLibraryEx(Path, NULL, DONT_RESOLVE_DLL_REFERENCES);
	//获取导出函数StartOfsset地址
	StartOfsset = (DWORD)GetProcAddress((HMODULE)DllBase, "start");
	if (!StartOfsset)
		print("获取DLLMain的导出函数失败");
	StartOfsset = StartOfsset - DllBase - (DWORD)GetSecHeadAddr(DllBase, DllSecName)->VirtualAddress;
	//获取通讯结构体,加壳器需要使用到
	pShareData = (SHARE*)GetProcAddress((HMODULE)DllBase, "ShareData");
}

//获取指定区段的首地址
PIMAGE_SECTION_HEADER  Pack::GetSecHeadAddr(DWORD ModuleBase,LPCSTR Name) {
	//区段数量
	DWORD NumberOfSec = pFileHead(ModuleBase)->NumberOfSections;
	auto FristSec = IMAGE_FIRST_SECTION(pNtHead(ModuleBase));
	//遍历区段
	for (size_t i = 0; i < NumberOfSec; i++)
	{
		if (!memcmp(FristSec[i].Name,Name, strlen(Name)+1)) 
			return &FristSec[i];
	}
	print("没有找到区段");
	return 0;
}

//拷贝区段数据
VOID Pack::CopySecData(CHAR* DestName) {	
	//拷贝源地址
	CHAR* Src = (CHAR*)(GetSecHeadAddr(DllBase, DllSecName)->VirtualAddress+DllBase);
	//拷贝目标地址
	CHAR* Dest = (CHAR*)(GetSecHeadAddr(PeBase, DestName)->PointerToRawData+PeBase);
	memcpy(Dest, Src, GetSecHeadAddr(DllBase, DllSecName)->Misc.VirtualSize);
	return;
}

//添加并拷贝一个新区段
VOID Pack::CopySection(LPCSTR DestName) {
	if (strlen(DestName) > 7)
		print("新区段名称太长");
	//获取需要添加的区段
	auto pLastSection = IMAGE_FIRST_SECTION(pNtHead(PeBase)) + (pFileHead(PeBase)->NumberOfSections - 1);
	auto pNewSection = pLastSection + 1;
	//获取区段头的首地址
	auto SrcAddr = GetSecHeadAddr(DllBase, DllSecName);
	//拷贝区段头
	memcpy(pNewSection, SrcAddr, sizeof(IMAGE_SECTION_HEADER));
	//修改属性
	pNewSection->VirtualAddress = pLastSection->VirtualAddress + 
		AlignmentToSize(pLastSection->Misc.VirtualSize, pOptionHead(PeBase)->SectionAlignment);
	pNewSection->PointerToRawData = pLastSection->PointerToRawData +
		AlignmentToSize(pLastSection->SizeOfRawData, pOptionHead(PeBase)->FileAlignment);
	// Name
	memcpy(pNewSection->Name, DestName, strlen(DestName));
	// NumberOfSections
	pFileHead(PeBase)->NumberOfSections += 1;
	// SizeOfImage大小
	pOptionHead(PeBase)->SizeOfImage += 
		AlignmentToSize(pNewSection->Misc.VirtualSize, pOptionHead(PeBase)->SectionAlignment);
	// 从新分配空间，将新区段添加到堆空间中
	PeSize = pNewSection->SizeOfRawData + pNewSection->PointerToRawData;
	PeBase = (DWORD)realloc((VOID*)PeBase, PeSize);	
	return;
}

//设置OEP
VOID Pack::SetOep() {
	
	//在重定位之前，保存原程序OEP
	pShareData->DestOEP = pOptionHead(PeBase)->AddressOfEntryPoint;
	//设置OEP为壳代码OEP
	pOptionHead(PeBase)->AddressOfEntryPoint = StartOfsset + GetSecHeadAddr(PeBase,".pack")->VirtualAddress;	
}

//将新文件写入磁盘
VOID Pack::WriteToFile(LPCSTR Path) {
	HANDLE hFile = CreateFile(Path, GENERIC_WRITE, 0, NULL, 
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	DWORD WriteSize = 0;
	DWORD Ret = WriteFile(hFile, (LPVOID)PeBase, PeSize, &WriteSize, NULL);
	if (!Ret)
		print("写入文件失败");
	CloseHandle(hFile);
	return;
}

//DLL中合并的区段名
VOID Pack::SetDllSecName(CHAR* SecName) {
	memcpy(DllSecName, SecName, strlen(SecName));
	return;
}

// 对壳代码进行重定位
VOID Pack::FixReloc()
{
	DWORD OldImageBase = pOptionHead(DllBase)->ImageBase;
	DWORD NewImageBase = pOptionHead(PeBase)->ImageBase;
	DWORD OldSectionBase = GetSecHeadAddr(DllBase, ".text")->VirtualAddress;
	DWORD NewSectionBase = GetSecHeadAddr(PeBase, ".pack")->VirtualAddress;

	auto Relocs = (PIMAGE_BASE_RELOCATION)(DllBase + pOptionHead(DllBase)->DataDirectory[5].VirtualAddress);

	// 遍历重定位表
	while (Relocs->SizeOfBlock)
	{
		DWORD OldProtect = 0;
		VirtualProtect((LPVOID)(DllBase + Relocs->VirtualAddress), 0x1000, PAGE_READWRITE, &OldProtect);
		TypeOffset* items = (TypeOffset*)(Relocs + 1);
		//重定位项数量
		int count = (Relocs->SizeOfBlock - 8) / 2;
		//遍历重定位项
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
	// 关闭源程序的随机基址
	//pOptionHead(PeBase)->DllCharacteristics &= ~IMAGE_DLLCHARACTERISTICS_DYNAMIC_BASE;
}

//加密区段
VOID Pack::Encryption(CHAR* DestName) {
	//找到需要加密的区段
	auto SectionHead = GetSecHeadAddr(PeBase, DestName);
	DWORD Size = SectionHead->Misc.VirtualSize;
	CHAR* Address =(CHAR*)(SectionHead->PointerToRawData + PeBase);
	for (size_t i = 0; i < Size; i++)
	{
		*Address ^= 0x15;
		Address++;
	}
}

//令壳代码接管IAT表
VOID Pack::SetIatTo0() {
	pShareData->ImpTableVA = pOptionHead(PeBase)->DataDirectory[1].VirtualAddress;// 在重定位之前设置
	pOptionHead(PeBase)->DataDirectory[1].VirtualAddress = 0;
	pOptionHead(PeBase)->DataDirectory[12].VirtualAddress = 0;
}

//添加一个新区段
VOID Pack::AddSection(LPCSTR Name, DWORD Size) {
	PIMAGE_DOS_HEADER pDos = pDosHead(PeBase);
	PIMAGE_NT_HEADERS pNt = pNtHead(PeBase);
	//获取需要添加的区段
	PIMAGE_SECTION_HEADER pLastSection = (IMAGE_FIRST_SECTION(pNt)) + (pNt->FileHeader.NumberOfSections - 1);
	PIMAGE_SECTION_HEADER pNewSection = pLastSection + 1;
	//Name
	memcpy(pNewSection->Name, Name, 7);
	//VirtualSize
	pNewSection->Misc.VirtualSize = AlignmentToSize(Size, pNt->OptionalHeader.SectionAlignment);
	//VirtualAddress = 最后一个区段的 VirtualAddress +最后一个区段内存大小
	pNewSection->VirtualAddress = pLastSection->VirtualAddress +
		AlignmentToSize(pLastSection->Misc.VirtualSize, pNt->OptionalHeader.SectionAlignment);
	//SizeOfRawData
	pNewSection->SizeOfRawData = AlignmentToSize(Size, pNt->OptionalHeader.FileAlignment);
	//PointerToRawData = 最后一个区段的 PointerToRawData + 最后一个区段文件大小
	pNewSection->PointerToRawData = pLastSection->PointerToRawData +
		AlignmentToSize(pLastSection->SizeOfRawData, pNt->OptionalHeader.FileAlignment);
	//Characteristics
	pNewSection->Characteristics = 0xE00000E0;
	//NumberOfSections
	pNt->FileHeader.NumberOfSections += 1;
	//SizeOfImage大小
	pNt->OptionalHeader.SizeOfImage += AlignmentToSize(pNewSection->Misc.VirtualSize, pNt->OptionalHeader.SectionAlignment);
	//从新分配空间，将新区段添加到堆空间中
	PeSize = pNewSection->SizeOfRawData + pNewSection->PointerToRawData;
	PeBase = (DWORD)realloc((VOID*)PeBase, PeSize);
	pNewSection = GetSecHeadAddr(PeBase, Name);
	memset((DWORD*)(pNewSection->PointerToRawData + PeBase), 0, pNewSection->SizeOfRawData);
	return;
}

//备份并替换重定位表
VOID Pack::SetReloc() {
	//1.备份原程序的重定位表地址，将原程序的重定位写入到通讯结构体中，将dll的重定位表替换到原程序的重定位表
	//2.在壳代码中，系统会帮我们修复壳代码的重定位，待原程序解密之后，在壳代码中获取通讯结构体，获取原程序重定位表，修复原程序重定位

	DWORD DllImageBase = pOptionHead(DllBase)->ImageBase;

	//备份原重定位表
	pShareData->dwRelocRva = pOptionHead(PeBase)->DataDirectory[5].VirtualAddress;
	pShareData->dwRelocSize = pOptionHead(PeBase)->DataDirectory[5].Size;
	pShareData->OldImageBase = pOptionHead(PeBase)->ImageBase;
	//Dll重定位表
	auto DllBaseReloc = (PIMAGE_BASE_RELOCATION)(pOptionHead(DllBase)->DataDirectory[5].VirtualAddress + DllImageBase);
	DWORD DllRelocaSize = pOptionHead(DllBase)->DataDirectory[5].Size;
	
	//新增区段
	AddSection(".mysec", DllRelocaSize);
	auto NewSecHed = GetSecHeadAddr(PeBase, ".mysec");
	auto OldSecHed = GetSecHeadAddr(DllBase, ".text");
	auto PackSecHed = GetSecHeadAddr(PeBase, ".pack");
	auto NewRelocaSection = (PIMAGE_BASE_RELOCATION)(NewSecHed->PointerToRawData + PeBase);
	DWORD OldSectionAddr = (DWORD)(OldSecHed->VirtualAddress + DllBase);

	memcpy((DWORD*)NewRelocaSection, (DWORD*)(DllBaseReloc), DllRelocaSize);
	while (NewRelocaSection->VirtualAddress)
	{
		//新的内存页起始RVA = 原RVA - 原段基址 +.pack段基址
		NewRelocaSection->VirtualAddress = NewRelocaSection->VirtualAddress - (OldSectionAddr -DllBase) + PackSecHed->VirtualAddress;
		NewRelocaSection = (PIMAGE_BASE_RELOCATION)(NewRelocaSection->SizeOfBlock + (DWORD)NewRelocaSection);
	}	
	//替换原程序重定位表
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

//备份TLS后清空TLS
VOID Pack::SetTls() {
	pShareData->TlsVirtualAddress = pOptionHead(PeBase)->DataDirectory[9].VirtualAddress;
	if (pShareData->TlsVirtualAddress) {
		pOptionHead(PeBase)->DataDirectory[9].VirtualAddress = 0;
		DWORD TlsFoa = RvaToOffset(PeBase, pShareData->TlsVirtualAddress);
		auto TlsTable = (PIMAGE_TLS_DIRECTORY)(TlsFoa + PeBase);
		pShareData->TlsCallBackTableVa = TlsTable->AddressOfCallBacks;
	}
}

//压缩
VOID Pack::Compression()
{	
	int SecTextSize = GetSecHeadAddr(PeBase, ".text")->SizeOfRawData;
	pShareData->SizeOfRawData = SecTextSize;
	char* TextSecData = (char*)(GetSecHeadAddr(PeBase, ".text")->PointerToRawData + PeBase);
	//被压缩的数据,Packed保存压缩数据的空间，WorkMem为完成压缩需要使用的空间
	char* Packed = (char*)malloc(aP_max_packed_size(SecTextSize));
	char* WorkMem = (char*)malloc(aP_workmem_size(SecTextSize));
	//压缩后的大小
	int OutSize = aPsafe_pack(TextSecData, Packed, SecTextSize, WorkMem, NULL, NULL);
	//对齐后的大小
	int AlignSize = OutSize % 0x200 == 0 ? OutSize : (OutSize / 0x200 + 1) * 0x200;
	//新空间大小
	DWORD NewFileSize = PeSize - GetSecHeadAddr(PeBase, ".text")->SizeOfRawData + AlignSize;
	//申请新的空间大小 文件大小 - 区段在文件中的大小 + 压缩后的大小(不对齐)
	DWORD NewFileBase = (DWORD)malloc(NewFileSize);
	//TextSecData之前的数据
	DWORD PreText = GetSecHeadAddr(PeBase, ".text")->PointerToRawData - 1;
	//拷贝TextSecData段之前的数据
	memcpy((LPVOID)NewFileBase, (LPVOID)PeBase, PreText);
	//拷贝压缩部分的数据
	memcpy((LPVOID)(NewFileBase + PreText + 1), Packed, OutSize);
	//拷贝TextSecData段后面的数据
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
	// 1. 获取到目标模块的区段表	
	auto Sections = IMAGE_FIRST_SECTION(pNtHead(PeBase));
	// 2. 使用文件头中的区段数量遍历区段表	
	WORD Count = pFileHead(PeBase)->NumberOfSections;
	BOOL bChangeFoa = FALSE;
	for (WORD i = 0; i < Count; ++i)
	{
		if (bChangeFoa) {
			Sections[i].PointerToRawData = Sections[i].PointerToRawData - GetSecHeadAddr(PeBase, ".text")->SizeOfRawData + AlignSize;
		}
		// 3. .text区段之前的区段不改变,操作.text区段之后的区段
		if (!memcmp(Sections[i].Name, ".text", 7)) {
			bChangeFoa = TRUE;
		}
	}	
	pShareData->TextCompressSize = OutSize;
	GetSecHeadAddr(PeBase, ".text")->SizeOfRawData = AlignSize;
	pShareData->TextRVA = GetSecHeadAddr(PeBase, ".text")->VirtualAddress;
	PeSize = NewFileSize;
}