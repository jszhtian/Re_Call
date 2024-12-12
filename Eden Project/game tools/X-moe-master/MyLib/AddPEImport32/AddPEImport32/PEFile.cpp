/*******************************************************************************
********************************   Team AT4RE   ********************************
********************************************************************************
*******************  PLEASE DON'T CHANGE/REMOVE THIS HEADER  *******************
********************************************************************************
**                                                                            **
**	Title:		PEFile class.                                                 **
**	Desc:		A handy class to manipulate pe files.                         **
**	Author:		MohammadHi [ in4matics at hotmail dot com ]                   **
**	WwW:		AT4RE      [ http://www.at4re.com ]                           **
**	Date:		2008-01-28                                                    **
**                                                                            **
********************************************************************************
*******************************************************************************/

#include "PEFile.h"
#include <math.h>
#include <Dbghelp.h>
#pragma comment(lib, "Dbghelp.lib")
//==============================================================================
#define DEBUG_ENABLED true;
#ifdef DEBUG_ENABLED
#define echo(x)			MessageBoxW(0, x, L"DEBUG", MB_ICONERROR);
#define echo2(x, y)		{ WCHAR v[256]; wcscpy_s(v, 256, x); wcscat_s(v, 256, y); echo(v); }
#define echo3(x, y, z)	{ WCHAR w[256]; wcscpy_s(w, 256, x); wcscat_s(w, 256, y); echo2(w, z); }
#else
#define echo(x) ;
#define echo2(x, y) ;
#define echo3(x, y, z) ;
#endif
//==============================================================================
PEFile::PEFile() {
	init();
}
//==============================================================================
PEFile::PEFile(WCHAR* filePath) {
	init();
	loadFromFile(filePath);
}
//==============================================================================
PEFile::~PEFile() {
	unloadFile();
}
//==============================================================================
void PEFile::init() {
	peMemory = NULL;
	ZeroMemory(&newImports, sizeof(PE_IMPORT_DLL));
}
//==============================================================================
bool PEFile::readFileData(WCHAR* filePath) {
	// open the file for read
	HANDLE fileHandle = CreateFileW(filePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (fileHandle == INVALID_HANDLE_VALUE) {
		echo3(L"Couldn't open file : [", filePath, L"]");
		return false;
	}

	// get the file size
	DWORD fileSize = GetFileSize(fileHandle, 0);
	if (fileSize == 0) {
		CloseHandle(fileHandle);
		echo3(L"File size is ZeR0! : [", filePath, L"]");
		return false;
	}

	// allocate memory to read the pe file (note that we used VirtualAlloc not GlobalAlloc!)
	peMemory = (char*)VirtualAlloc(NULL, fileSize, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	if (peMemory == NULL) {
		CloseHandle(fileHandle);
		echo(L"Couldn't allocate memory!");
		return false;
	}

	DWORD bytesRead;
	// read whole file data
	if (!ReadFile(fileHandle, peMemory, fileSize, &bytesRead, NULL) || bytesRead != fileSize) {
		CloseHandle(fileHandle);
		echo3(L"Couldn't read file! : [", filePath, L"]");
		return false;
	}

	// close the file
	CloseHandle(fileHandle);

	return true;
}
//==============================================================================
bool PEFile::checkValidity() {
	// 'dosHeader.Signature' must be "MZ" && 'peHeaders.Signature' must be "PE\0\0"
	if (dosHeader.Signature != IMAGE_DOS_SIGNATURE || peHeaders.Signature != IMAGE_NT_SIGNATURE) {
		unloadFile();
		echo(L"Invalid PE file!");
		return false;
	}

	if (peHeaders.FileHeader.NumberOfSections > MAX_SECTION_COUNT) {
		unloadFile();
		echo(L"Number of sections > MAX_SECTION_COUNT !");
		return false;
	}

	return true;
}
//==============================================================================
bool PEFile::readHeaders() {
	// read dos/pe headers
	CopyMemory(&dosHeader, peMemory, sizeof(PE_DOS_HEADER));
	dosStub.RawData = peMemory + sizeof(PE_DOS_HEADER);
	dosStub.Size = dosHeader.PEHeaderOffset - sizeof(PE_DOS_HEADER);
	CopyMemory(&peHeaders, peMemory + dosHeader.PEHeaderOffset, sizeof(PE_NT_HEADERS));

	// check validity of the file to ensure that we loaded a "PE File" not another thing!
	if (!checkValidity()) {
		return false;
	}

	auto pDosHeader = (PIMAGE_DOS_HEADER)peMemory;
	auto pNtHeader = (PIMAGE_NT_HEADERS)((SIZE_T)(pDosHeader->e_lfanew) + (SIZE_T)peMemory);
	GlobalData.ImageBase = pNtHeader->OptionalHeader.ImageBase;

	// read section table
	ZeroMemory(sectionTable, sizeof(sectionTable));
	CopyMemory(sectionTable, peMemory + dosHeader.PEHeaderOffset + sizeof(PE_NT_HEADERS),
		peHeaders.FileHeader.NumberOfSections * sizeof(PE_SECTION_HEADER));

	return true;
}
//==============================================================================
bool PEFile::readBody() {
	// read reserved data
	DWORD reservedDataOffset = dosHeader.PEHeaderOffset + sizeof(PE_NT_HEADERS) +
		peHeaders.FileHeader.NumberOfSections * sizeof(PE_SECTION_HEADER);

	reservedData.Offset = reservedDataOffset;
	reservedData.RawData = peMemory + reservedDataOffset;
	/*reservedData.Size = peHeaders.OptionalHeader.SizeOfHeaders - reservedDataOffset;*/
	if (sectionTable[0].PointerToRawData > 0) {
		reservedData.Size = sectionTable[0].PointerToRawData - reservedDataOffset;
	}
	else {
		reservedData.Size = sectionTable[0].VirtualAddress - reservedDataOffset;
	}

	// read sections
	for (int i = 0; i < peHeaders.FileHeader.NumberOfSections; i++) {
		sections[i].Offset = sectionTable[i].PointerToRawData;
		sections[i].RawData = peMemory + sectionTable[i].PointerToRawData;
		sections[i].Size = sectionTable[i].SizeOfRawData;
	}

	return true;
}
//==============================================================================
bool PEFile::readImportTable() {
	DWORD tableRVA = peHeaders.OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress;
	DWORD tableOffset = rvaToOffset(tableRVA);
	if (tableOffset == 0) {
		return false;
	}

	ZeroMemory(&importTable, sizeof(PE_IMPORT_DLL));

	IMAGE_IMPORT_DESCRIPTOR* importDesc = (IMAGE_IMPORT_DESCRIPTOR*)(peMemory + tableOffset);
	IMAGE_THUNK_DATA* importThunk;
	PE_IMPORT_DLL* importDll = &this->importTable;
	PE_IMPORT_FUNCTION* importFunction;

	while (true) {
		importDll->DllName = (char*)(peMemory + rvaToOffset(importDesc->Name));
		if (importDesc->OriginalFirstThunk > 0) {
			importThunk = (IMAGE_THUNK_DATA*)(peMemory + rvaToOffset(importDesc->OriginalFirstThunk));
		}
		else {
			importThunk = (IMAGE_THUNK_DATA*)(peMemory + rvaToOffset(importDesc->FirstThunk));
		}

		importDll->Functions = new PE_IMPORT_FUNCTION();
		ZeroMemory(importDll->Functions, sizeof(PE_IMPORT_FUNCTION));
		importFunction = importDll->Functions;
		while (true) {
			if ((importThunk->u1.Ordinal & IMAGE_ORDINAL_FLAG32) == IMAGE_ORDINAL_FLAG32) {
				importFunction->FunctionId = IMAGE_ORDINAL32(importThunk->u1.Ordinal);
			}
			else {
				DWORD nameOffset = rvaToOffset(importThunk->u1.AddressOfData);
				importFunction->FunctionName = (char*)(peMemory + nameOffset + 2);
			}

			importThunk = (IMAGE_THUNK_DATA*)((char*)importThunk + sizeof(IMAGE_THUNK_DATA));
			if (importThunk->u1.AddressOfData == 0) {
				break;
			}
			importFunction->Next = new PE_IMPORT_FUNCTION();
			ZeroMemory(importFunction->Next, sizeof(PE_IMPORT_FUNCTION));
			importFunction = importFunction->Next;
		}

		importDesc = (IMAGE_IMPORT_DESCRIPTOR*)((char*)importDesc + sizeof(IMAGE_IMPORT_DESCRIPTOR));
		if (importDesc->Name == 0) {
			break;
		}
		importDll->Next = new PE_IMPORT_DLL();
		ZeroMemory(importDll->Next, sizeof(PE_IMPORT_DLL));
		importDll = importDll->Next;
	}

	return true;
}
//==============================================================================
bool PEFile::loadFromFile(WCHAR* filePath) {
	unloadFile();

	return readFileData(filePath) &&
		readHeaders() &&
		readBody() &&
		readImportTable();
}
//==============================================================================
bool PEFile::loadFromMemory(char* memoryAddress) {
	unloadFile();

	peMemory = memoryAddress;

	return readHeaders()/* &&
						readBody() &&
						readImportTable()*/;
}


void* PEFile::VaToPtr(DWORD dwVA)
{
	auto pDosHeader = (PIMAGE_DOS_HEADER)peMemory;
	auto pNtHeader = (PIMAGE_NT_HEADERS)((SIZE_T)(pDosHeader->e_lfanew) + (SIZE_T)peMemory);

	DWORD dwRVA = dwVA - pNtHeader->OptionalHeader.ImageBase;
	return ImageRvaToVa(pNtHeader, peMemory, dwRVA, NULL);
}

//==============================================================================
bool PEFile::saveToFile(WCHAR* filePath) {
	commit();
	buildImportTable();

	FILE* fin = fopen(DllFileName.c_str(), "rb");
	if (!fin)
	{
		MessageBoxW(NULL, L"cannot open dll", 0, 0);
		return false;
	}
	fseek(fin, 0, SEEK_END);
	SIZE_T FileSize = ftell(fin);
	rewind(fin);
	PBYTE ShellBuffer = (PBYTE)GlobalAlloc(0, FileSize);
	fread(ShellBuffer, 1, FileSize, fin);
	fclose(fin);

	auto ShellInfo = GetShellData(ShellBuffer, FileSize);
	if (ShellInfo == nullptr)
	{
		MessageBoxW(NULL, L"cannot find data", 0, 0);
		GlobalFree(ShellBuffer);
		return false;
	}

	ShellInfo->OriOEP = peHeaders.OptionalHeader.AddressOfEntryPoint + 
		peHeaders.OptionalHeader.ImageBase;
	//char Info[30] = { 0 };
	//sprintf(Info, "After %08x", ShellInfo->OriOEP);
	//MessageBoxA(0, Info, 0, 0);


	fin = fopen(DllFileName.c_str(), "wb");
	fwrite(ShellBuffer, 1, FileSize, fin);
	fclose(fin);

	// create the output file
	HANDLE fileHandle = CreateFileW(filePath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (fileHandle == INVALID_HANDLE_VALUE) {
		echo(L"Couldn't create file");
		return false;
	}

	DWORD bytesWritten;

#if 0
	if (peHeaders.OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].Size)
	{
		PIMAGE_BASE_RELOCATION pLoc = (PIMAGE_BASE_RELOCATION)
			VaToPtr(peHeaders.OptionalHeader.ImageBase +
			peHeaders.OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress);
		ZeroMemory(pLoc, peHeaders.OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].Size);
	}
#endif
	
	peHeaders.OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].Size = 0;
	peHeaders.OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress = 0;

	WriteFile(fileHandle, &dosHeader, sizeof(PE_DOS_HEADER), &bytesWritten, NULL);
	WriteFile(fileHandle, dosStub.RawData, dosStub.Size, &bytesWritten, NULL);
	writePadding(fileHandle, dosHeader.PEHeaderOffset - sizeof(PE_DOS_HEADER) - dosStub.Size);
	WriteFile(fileHandle, &peHeaders, sizeof(PE_NT_HEADERS), &bytesWritten, NULL);
	WriteFile(fileHandle, &sectionTable, peHeaders.FileHeader.NumberOfSections * sizeof(PE_SECTION_HEADER), &bytesWritten, NULL);
	WriteFile(fileHandle, reservedData.RawData, reservedData.Size, &bytesWritten, NULL);

	for (int i = 0; i < peHeaders.FileHeader.NumberOfSections; i++) {
		char SceInfo[0x10] = { 0 };
		memcpy(SceInfo, sectionTable[i].Name, 8);
		printf("Writing Scetion : %s\n", SceInfo);
		writePadding(fileHandle, sectionTable[i].PointerToRawData - GetFileSize(fileHandle, NULL));

		if (!memcmp(sectionTable[i].Name, ".CryS", 5))
		{
			ZeroMemory(sections[i].RawData, sections[i].Size);
		}
		else if (!memcmp(sectionTable[i].Name, ".reloc", 6))
		{
			printf("Delete Reloc Scetion\n");
			ZeroMemory(sections[i].RawData, sections[i].Size);
		}
		WriteFile(fileHandle, sections[i].RawData, sections[i].Size, &bytesWritten, NULL);
	}

	CloseHandle(fileHandle);

	FILE* fout = _wfopen(filePath, L"rb");
	fseek(fout, 0, SEEK_END);
	SIZE_T OutSize = ftell(fout);
	rewind(fout);
	PBYTE OutBuffer = (PBYTE)GlobalAlloc(0, OutSize);
	fread(OutBuffer, 1, OutSize, fout);
	fclose(fout);

	SIZE_T ShellOEP = 0;
	if (!IATPatch(DllFileName.c_str(), OutBuffer, ShellOEP))
	{
		GlobalFree(OutBuffer);
		return false;
	}

	auto pDosHeader = (PIMAGE_DOS_HEADER)OutBuffer;
	auto pNtHeader = (PIMAGE_NT_HEADERS)((SIZE_T)(pDosHeader->e_lfanew) + (SIZE_T)OutBuffer);

	SIZE_T PtrToRaw, VirtualAddr;
	LPBYTE pBase = (LPBYTE)OutBuffer;
	ULONG uSize = PIMAGE_OPTIONAL_HEADER((pBase + pDosHeader->e_lfanew + 4 + 20))->SizeOfHeaders;
	PIMAGE_SECTION_HEADER    pSec = (PIMAGE_SECTION_HEADER)(pBase + pDosHeader->e_lfanew + sizeof(IMAGE_NT_HEADERS));
	for (int i = 0; i < PIMAGE_FILE_HEADER(pBase + pDosHeader->e_lfanew + 4)->NumberOfSections; ++i)
	{
		if (!_stricmp((PCHAR)pSec[i].Name, ".CryS"))
		{
			PtrToRaw = pSec[i].PointerToRawData;
			VirtualAddr = pSec[i].VirtualAddress;
			break;
		}
	}

	pNtHeader->OptionalHeader.AddressOfEntryPoint = VirtualAddr /*+ GlobalData.ImageBase*/;

	BYTE JMPCode[] = { 0xFF, 0x25, 0 };
	DWORD RealShellOEP = ShellOEP + GlobalData.ImageBase;
	CopyMemory(OutBuffer + PtrToRaw, JMPCode, 2);
	CopyMemory(OutBuffer + PtrToRaw + 2, &RealShellOEP, 4);

	fout = _wfopen(filePath, L"wb");
	fwrite(OutBuffer, 1, OutSize, fout);
	fclose(fout);

	char sInfo[30] = { 0 };
	sprintf(sInfo, "OP %02x %02x", OutBuffer[PtrToRaw], OutBuffer[PtrToRaw + 1]);
	MessageBoxA(NULL, sInfo, 0, 0);

	GlobalFree(OutBuffer);
	return true;
}


DWORD RVAToOffset(LPVOID lpBase, DWORD VirtualAddress)
{
	IMAGE_DOS_HEADER *dosHeader;
	IMAGE_NT_HEADERS *ntHeader;
	IMAGE_SECTION_HEADER *SectionHeader;
	int NumOfSections;
	dosHeader = (IMAGE_DOS_HEADER*)lpBase;
	ntHeader = (IMAGE_NT_HEADERS*)((BYTE*)lpBase + dosHeader->e_lfanew);
	NumOfSections = ntHeader->FileHeader.NumberOfSections;
	for (int i = 0; i<NumOfSections; i++)
	{
		SectionHeader = (IMAGE_SECTION_HEADER*)((BYTE*)lpBase + dosHeader->e_lfanew + sizeof(IMAGE_NT_HEADERS)) + i;
		if (VirtualAddress > SectionHeader->VirtualAddress && VirtualAddress < SectionHeader->VirtualAddress + SectionHeader->SizeOfRawData)
		{
			DWORD AposRAV = VirtualAddress - SectionHeader->VirtualAddress;
			DWORD Offset = SectionHeader->PointerToRawData + AposRAV;
			return Offset;
		}
	}
	return 0;
}



bool PEFile::IATPatch(LPCSTR szFileDllName, PBYTE lpBaseAddress, SIZE_T& OutAPIRVA)
{
	PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)lpBaseAddress;
	PIMAGE_NT_HEADERS pNtHeaders = (PIMAGE_NT_HEADERS)(lpBaseAddress + pDosHeader->e_lfanew);

	int i, j;

	//导入表的rva：0x2a000;
	DWORD Rva_import_table = pNtHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress;

	if (Rva_import_table == 0)
	{
		printf("no import table!");
		return false;
	}

	//这个虽然是内存地址，但是减去文件开头的地址，就是文件地址了
	//这个地址可以直接从里面读取你想要的东西了
	PIMAGE_IMPORT_DESCRIPTOR pImportTable = (PIMAGE_IMPORT_DESCRIPTOR)ImageRvaToVa(
		pNtHeaders,
		lpBaseAddress,
		Rva_import_table,
		NULL
		);

	//减去内存映射的首地址，就是文件地址了。。（很简单吧）
	printf("FileAddress Of ImportTable: %p\n", ((DWORD)pImportTable - (DWORD)lpBaseAddress));

	//现在来到了导入表的面前：IMAGE_IMPORT_DESCRIPTOR 数组（以0元素为终止）
	//定义表示数组结尾的null元素！
	IMAGE_IMPORT_DESCRIPTOR null_iid;
	IMAGE_THUNK_DATA null_thunk;
	memset(&null_iid, 0, sizeof(null_iid));
	memset(&null_thunk, 0, sizeof(null_thunk));

	//每个元素代表了一个引入的DLL。
	for (i = 0; memcmp(pImportTable + i, &null_iid, sizeof(null_iid)) != 0; i++)
	{
		//LPCSTR: 就是 const char*
		LPCSTR szDllName = (LPCSTR)ImageRvaToVa(
			pNtHeaders, lpBaseAddress,
			pImportTable[i].Name, //DLL名称的RVA
			NULL);

		//拿到了DLL的名字
		printf("-----------------------------------------\n");
		printf("[%d]: %s\n", i, szDllName);
		printf("-----------------------------------------\n");

		if (!_stricmp(szDllName, szFileDllName))
		{
			OutAPIRVA = pImportTable[i].FirstThunk;
			return true;
		}

		//现在去看看从该DLL中引入了哪些函数
		//我们来到该DLL的 IMAGE_TRUNK_DATA 数组（IAT：导入地址表）前面
		PIMAGE_THUNK_DATA32 pThunk = (PIMAGE_THUNK_DATA32)ImageRvaToVa(
			pNtHeaders, lpBaseAddress,
			pImportTable[i].OriginalFirstThunk, //【注意】这里使用的是OriginalFirstThunk
			NULL);

		for (j = 0; memcmp(pThunk + j, &null_thunk, sizeof(null_thunk)) != 0; j++)
		{
			//这里通过RVA的最高位判断函数的导入方式，
			//如果最高位为1，按序号导入，否则按名称导入
			if (pThunk[j].u1.AddressOfData & IMAGE_ORDINAL_FLAG32)
			{
				printf("\t [%d] \t %ld \t 按序号导入\n", j, pThunk[j].u1.AddressOfData & 0xffff);
			}
			else
			{
				//按名称导入，我们再次定向到函数序号和名称
				//注意其地址不能直接用，因为仍然是RVA！
				PIMAGE_IMPORT_BY_NAME pFuncName = (PIMAGE_IMPORT_BY_NAME)ImageRvaToVa(
					pNtHeaders, lpBaseAddress,
					pThunk[j].u1.AddressOfData,
					NULL);

				printf("\t [%d] \t %ld \t %s\n", j, pFuncName->Hint, pFuncName->Name);
			}
		}
	}
	return false;
}

//==============================================================================
bool PEFile::writePadding(HANDLE fileHandle, long paddingSize) {
	if (paddingSize <= 0)
		return false;

	DWORD bytesWritten;
	char* padding = new char[paddingSize];
	memset(padding, 0, paddingSize);
	WriteFile(fileHandle, padding, paddingSize, &bytesWritten, NULL);
	delete padding;

	return (bytesWritten == paddingSize);
}
//==============================================================================
void PEFile::unloadFile() {
	if (peMemory != NULL) {
		VirtualFree(peMemory, 0, MEM_RELEASE);
		peMemory = NULL;
	}
}
//==============================================================================
void PEFile::buildImportTable() {
	DWORD sizeDlls = 0;
	DWORD sizeFunctions = 0;
	DWORD sizeStrings = 0;
	DWORD newImportsSize = calcNewImportsSize(sizeDlls, sizeFunctions, sizeStrings);

	// we'll move the old dll list to the new import table, so we'll calc its size
	DWORD oldImportDllsSize = 0;
	PE_IMPORT_DLL* importDll = &this->importTable;
	while (importDll != NULL) {
		oldImportDllsSize += sizeof(IMAGE_IMPORT_DESCRIPTOR);
		importDll = importDll->Next;
	}

	// add a new section to handle the new import table
	int index = addSection(SECTION_IMPORT, oldImportDllsSize + newImportsSize, false);

	// copy old import dll list
	DWORD oldImportTableRVA = peHeaders.OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress;
	DWORD oldImportTableOffset = rvaToOffset(oldImportTableRVA);
	CopyMemory(sections[index].RawData, peMemory + oldImportTableOffset, oldImportDllsSize);

	// copy new imports
	char* newImportsData = buildNewImports(sectionTable[index].VirtualAddress + oldImportDllsSize);
	CopyMemory(sections[index].RawData + oldImportDllsSize, newImportsData, newImportsSize);

	peHeaders.OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress = sectionTable[index].VirtualAddress;
	peHeaders.OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size = sectionTable[index].SizeOfRawData;
	peHeaders.OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IAT].VirtualAddress = 0;
	peHeaders.OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IAT].Size = 0;
}
//==============================================================================
char* PEFile::buildNewImports(DWORD baseRVA) {
	commit();

	IMAGE_IMPORT_DESCRIPTOR importDesc;
	IMAGE_THUNK_DATA importThunk;
	PE_IMPORT_DLL* importDll;
	PE_IMPORT_FUNCTION* importFunction;

	DWORD sizeDlls = 0;
	DWORD sizeFunctions = 0;
	DWORD sizeStrings = 0;
	DWORD newImportsSize = calcNewImportsSize(sizeDlls, sizeFunctions, sizeStrings);
	DWORD offsetDlls = 0;
	DWORD offsetFunctions = sizeDlls;
	DWORD offsetStrings = sizeDlls + 2 * sizeFunctions;

	char* buffer = new char[newImportsSize];
	ZeroMemory(buffer, newImportsSize);

	importDll = &newImports;
	while (importDll != NULL) {
		ZeroMemory(&importDesc, sizeof(IMAGE_IMPORT_DESCRIPTOR));
		importDesc.OriginalFirstThunk = baseRVA + offsetFunctions;
		importDesc.FirstThunk = baseRVA + offsetFunctions + sizeFunctions;
		importDesc.Name = baseRVA + offsetStrings;
		CopyMemory(buffer + offsetStrings, importDll->DllName, strlen(importDll->DllName));
		offsetStrings += alignNumber((DWORD)strlen(importDll->DllName) + 1, 2);

		CopyMemory(buffer + offsetDlls, &importDesc, sizeof(IMAGE_IMPORT_DESCRIPTOR));
		offsetDlls += sizeof(IMAGE_IMPORT_DESCRIPTOR);

		importFunction = importDll->Functions;
		while (importFunction != NULL) {
			ZeroMemory(&importThunk, sizeof(IMAGE_THUNK_DATA));
			if (importFunction->FunctionId != 0) {
				importThunk.u1.Ordinal = importFunction->FunctionId | IMAGE_ORDINAL_FLAG32;
			}
			else {
				importThunk.u1.AddressOfData = baseRVA + offsetStrings;
				CopyMemory(buffer + offsetStrings + 2, importFunction->FunctionName, strlen(importFunction->FunctionName));
				offsetStrings += 2 + alignNumber((DWORD)strlen(importFunction->FunctionName) + 1, 2);
			}

			CopyMemory(buffer + offsetFunctions, &importThunk, sizeof(IMAGE_THUNK_DATA));
			CopyMemory(buffer + offsetFunctions + sizeFunctions, &importThunk, sizeof(IMAGE_THUNK_DATA));
			offsetFunctions += sizeof(IMAGE_THUNK_DATA);

			importFunction = importFunction->Next;
		}
		offsetFunctions += sizeof(IMAGE_THUNK_DATA);

		importDll = importDll->Next;
	}

	return buffer;
}
//==============================================================================
DWORD PEFile::calcNewImportsSize(DWORD &sizeDlls, DWORD &sizeFunctions, DWORD &sizeStrings) {
	PE_IMPORT_DLL* importDll = &this->newImports;
	PE_IMPORT_FUNCTION* importFunction;

	// calc added imports size
	while (importDll != NULL) {
		sizeDlls += sizeof(IMAGE_IMPORT_DESCRIPTOR);
		sizeStrings += alignNumber((DWORD)strlen(importDll->DllName) + 1, 2);
		importFunction = importDll->Functions;
		while (importFunction != NULL) {
			sizeFunctions += sizeof(IMAGE_THUNK_DATA);
			if (importFunction->FunctionId == 0) {
				sizeStrings += 2 + alignNumber((DWORD)strlen(importFunction->FunctionName) + 1, 2);
			}
			importFunction = importFunction->Next;
		}
		sizeFunctions += sizeof(IMAGE_THUNK_DATA); // for the terminator thunk data
		importDll = importDll->Next;
	}
	sizeDlls += sizeof(IMAGE_IMPORT_DESCRIPTOR); // for the terminator import descriptor

	return sizeDlls + 2 * sizeFunctions + sizeStrings;
}
//==============================================================================
int PEFile::addSection(char* name, DWORD size, bool isExecutable) {
	if (peHeaders.FileHeader.NumberOfSections == MAX_SECTION_COUNT) {
		return -1;
	}

	PE_SECTION_DATA &newSection = sections[peHeaders.FileHeader.NumberOfSections];
	PE_SECTION_HEADER &newSectionHeader = sectionTable[peHeaders.FileHeader.NumberOfSections];
	PE_SECTION_HEADER &lastSectionHeader = sectionTable[peHeaders.FileHeader.NumberOfSections - 1];

	DWORD sectionSize = alignNumber(size, peHeaders.OptionalHeader.FileAlignment);
	DWORD virtualSize = alignNumber(sectionSize, peHeaders.OptionalHeader.SectionAlignment);

	DWORD sectionOffset = alignNumber(lastSectionHeader.PointerToRawData + lastSectionHeader.SizeOfRawData, peHeaders.OptionalHeader.FileAlignment);
	DWORD virtualOffset = alignNumber(lastSectionHeader.VirtualAddress + lastSectionHeader.Misc.VirtualSize, peHeaders.OptionalHeader.SectionAlignment);

	ZeroMemory(&newSectionHeader, sizeof(IMAGE_SECTION_HEADER));
	CopyMemory(newSectionHeader.Name, name, (strlen(name) > 8 ? 8 : strlen(name)));

	newSectionHeader.PointerToRawData = sectionOffset;
	newSectionHeader.VirtualAddress = virtualOffset;
	newSectionHeader.SizeOfRawData = sectionSize;
	newSectionHeader.Misc.VirtualSize = virtualSize;
	newSectionHeader.Characteristics = //0xC0000040; 
		IMAGE_SCN_MEM_READ | IMAGE_SCN_MEM_WRITE | IMAGE_SCN_CNT_INITIALIZED_DATA;

	if (isExecutable) {
		newSectionHeader.Characteristics |= IMAGE_SCN_CNT_CODE | IMAGE_SCN_MEM_EXECUTE;
	}

	newSection.RawData = (char*)GlobalAlloc(GMEM_FIXED | GMEM_ZEROINIT, sectionSize);
	newSection.Size = sectionSize;

	peHeaders.FileHeader.NumberOfSections++;
	if (reservedData.Size > 0) {
		reservedData.Size -= sizeof(IMAGE_SECTION_HEADER);
	}

	// return new section index
	return peHeaders.FileHeader.NumberOfSections - 1;
}
//==============================================================================
void PEFile::addImport(char* dllName, char** functions, int functionCount) {
	PE_IMPORT_DLL* importDll = &this->newImports;
	PE_IMPORT_FUNCTION* importFunction;

	if (newImports.DllName != NULL) {
		while (importDll->Next != NULL) {
			importDll = importDll->Next;
		}
		importDll->Next = new PE_IMPORT_DLL();
		importDll = importDll->Next;
	}
	importDll->DllName = dllName;
	importDll->Functions = new PE_IMPORT_FUNCTION();
	importDll->Next = NULL;

	importFunction = importDll->Functions;
	importFunction->FunctionName = functions[0];
	for (int i = 1; i < functionCount; i++) {
		importFunction->Next = new PE_IMPORT_FUNCTION();
		importFunction = importFunction->Next;
		importFunction->FunctionName = functions[i];
	}
	DllFileName = dllName;
	importFunction->Next = NULL;
}
//==============================================================================
DWORD PEFile::alignNumber(DWORD number, DWORD alignment) {
	return (DWORD)(ceil(number / (alignment + 0.0)) * alignment);
}
//==============================================================================
DWORD PEFile::rvaToOffset(DWORD rva) {
	for (int i = 0; i < peHeaders.FileHeader.NumberOfSections; i++) {
		if (rva >= sectionTable[i].VirtualAddress &&
			rva < sectionTable[i].VirtualAddress + sectionTable[i].Misc.VirtualSize) {
			return sectionTable[i].PointerToRawData + (rva - sectionTable[i].VirtualAddress);
		}
	}
	return 0;
}
//==============================================================================
DWORD PEFile::offsetToRVA(DWORD offset) {
	for (int i = 0; i < peHeaders.FileHeader.NumberOfSections; i++) {
		if (offset >= sectionTable[i].PointerToRawData &&
			offset < sectionTable[i].PointerToRawData + sectionTable[i].SizeOfRawData) {
			return sectionTable[i].VirtualAddress + (offset - sectionTable[i].PointerToRawData);
		}
	}
	return 0;
}
//==============================================================================
void PEFile::commit() {
	fixReservedData();
	fixHeaders();
	fixSectionTable();
}
//==============================================================================
void PEFile::fixReservedData() {
	DWORD dirIndex = 0;
	for (dirIndex = 0; dirIndex < peHeaders.OptionalHeader.NumberOfRvaAndSizes; dirIndex++) {
		if (peHeaders.OptionalHeader.DataDirectory[dirIndex].VirtualAddress > 0 &&
			peHeaders.OptionalHeader.DataDirectory[dirIndex].VirtualAddress >= reservedData.Offset &&
			peHeaders.OptionalHeader.DataDirectory[dirIndex].VirtualAddress < reservedData.Size) {
			break;
		}
	}

	if (dirIndex == peHeaders.OptionalHeader.NumberOfRvaAndSizes) {
		return;
	}

	int sectionIndex = addSection(SECTION_RESERV, reservedData.Size, false);
	CopyMemory(sections[sectionIndex].RawData, reservedData.RawData, reservedData.Size);

	for (dirIndex = 0; dirIndex < peHeaders.OptionalHeader.NumberOfRvaAndSizes; dirIndex++) {
		if (peHeaders.OptionalHeader.DataDirectory[dirIndex].VirtualAddress > 0 &&
			peHeaders.OptionalHeader.DataDirectory[dirIndex].VirtualAddress >= reservedData.Offset &&
			peHeaders.OptionalHeader.DataDirectory[dirIndex].VirtualAddress < reservedData.Size) {
			peHeaders.OptionalHeader.DataDirectory[dirIndex].VirtualAddress +=
				sectionTable[sectionIndex].VirtualAddress - reservedData.Offset;
		}
	}

	reservedData.Size = 0;
}
//==============================================================================
void PEFile::fixHeaders() {
	peHeaders.OptionalHeader.SizeOfHeaders = alignNumber(dosHeader.PEHeaderOffset + peHeaders.FileHeader.SizeOfOptionalHeader +
		peHeaders.FileHeader.NumberOfSections * sizeof(PE_SECTION_HEADER), peHeaders.OptionalHeader.FileAlignment);

	DWORD imageSize = peHeaders.OptionalHeader.SizeOfHeaders;
	for (int i = 0; i < peHeaders.FileHeader.NumberOfSections; i++) {
		imageSize += alignNumber(sectionTable[i].Misc.VirtualSize, peHeaders.OptionalHeader.SectionAlignment);
	}
	peHeaders.OptionalHeader.SizeOfImage = alignNumber(imageSize, peHeaders.OptionalHeader.SectionAlignment);

	peHeaders.OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT].VirtualAddress = 0;
	peHeaders.OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT].Size = 0;
}
//==============================================================================
void PEFile::fixSectionTable() {
	DWORD offset = peHeaders.OptionalHeader.SizeOfHeaders;
	for (int i = 0; i < peHeaders.FileHeader.NumberOfSections; i++) {
		sectionTable[i].Characteristics |= IMAGE_SCN_MEM_WRITE;
		offset = alignNumber(offset, peHeaders.OptionalHeader.FileAlignment);
		sectionTable[i].PointerToRawData = offset;
		//sectionTable[i].SizeOfRawData = alignNumber(offset + sectionTable[i].Misc.VirtualSize, peHeaders.OptionalHeader.FileAlignment);
		offset += sectionTable[i].SizeOfRawData;
	}
}
//==============================================================================


PSHELL_DATA32 PEFile::GetShellData(PBYTE Code, SIZE_T Len)
{
	ULONG64 Magic1 = 0x6C6C617473797243;
	ULONG64 Magic2 = 0x7461745372657A69;

	SIZE_T iPos = 0;
	while (iPos < Len)
	{
		if (Len - iPos > 0x10)
		{
			ULONG64 Checker1 = *(PULONG64)(Code + iPos + 0);
			ULONG64 Checker2 = *(PULONG64)(Code + iPos + 8);

			if (Checker1 == Magic1 && Checker2 == Magic2)
			{
				//memset(Code + iPos, 0, 0x10);
				return (PSHELL_DATA32)(Code + iPos);
			}
			else
			{
				iPos++;
			}
		}
		else
		{
			break;
		}
	}
	return nullptr;
}
