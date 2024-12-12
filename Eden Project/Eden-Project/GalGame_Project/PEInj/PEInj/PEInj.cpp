#include <windows.h>
#include <iostream>
#include <exception>
#include <string>

using namespace std;

#define ERROR_MESSAGE(Msg) std::cout<<Msg<<std::endl;
BOOL    AddImportTable(const string& strTargetFile, const string& strInjectDllName, const string& strFunctionName);
BOOL    AddNewSection(const string& strTargetFile, ULONG ulNewSectionSize);
BOOL    AddNewImportDescriptor(const string& strTargetFile, const string& strInjectDllName, const string& strFunctionName);
DWORD   RVAToFOA(PIMAGE_NT_HEADERS pNtHeaders, DWORD dwRVA);
ULONG32 PEAlign(ULONG32 dwNumber, ULONG32 dwAlign);

int main()
{
    AddImportTable("sorakoi.exe", "sorakoi_cn.dll", "AyamiKaze");

    system("pause");
    return true;
}

BOOL AddImportTable(const string& strTargetFile, const string& strInjectDllName, const string& strFunctionName)
{
    BOOL bOk = false;
    try
    {
        bOk = AddNewSection(strTargetFile, 256);
        if (!bOk)
        {
            ERROR_MESSAGE("AddImportTable:AddNewSection failed.");
            return false;
        }

        bOk = AddNewImportDescriptor(strTargetFile, strInjectDllName, strFunctionName);
        if (!bOk)
        {
            ERROR_MESSAGE("AddImportTable:AddNewImportDescriptor failed.");
            return false;
        }
    }
    catch (exception* e)
    {
        ERROR_MESSAGE((string("AddImportTable:") + e->what()).c_str());
        return false;
    }

    return true;
}

BOOL AddNewSection(const string& strTargetFile, ULONG ulNewSectionSize)
{
    BOOL bOk = true;
    HANDLE TargetFileHandle = nullptr;
    HANDLE MappingHandle = nullptr;
    PVOID FileData = nullptr;

    try
    {
        // 打开文件
        TargetFileHandle = CreateFileA(strTargetFile.c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if (TargetFileHandle == INVALID_HANDLE_VALUE)
        {
            ERROR_MESSAGE(string("AddNewSection:CreateFileA error with error code:" + GetLastError()).c_str());
            bOk = false;
            goto EXIT;
        }

        ULONG ulFileSize = GetFileSize(TargetFileHandle, NULL);

        // 映射文件
        MappingHandle = CreateFileMappingA(TargetFileHandle, NULL, PAGE_READWRITE, 0, ulFileSize, NULL);
        if (MappingHandle == NULL)
        {
            ERROR_MESSAGE(string("AddNewSection:CreateFileMapping error with error code:" + GetLastError()).c_str());
            bOk = false;
            goto EXIT;
        }

        // 得到缓存头
        FileData = MapViewOfFile(MappingHandle, FILE_MAP_ALL_ACCESS, 0, 0, ulFileSize);
        if (FileData == NULL)
        {
            ERROR_MESSAGE(string("AddNewSection:MapViewOfFile error with error code:" + GetLastError()).c_str());
            bOk = false;
            goto EXIT;
        }

        // 判断是否是PE文件
        if (((PIMAGE_DOS_HEADER)FileData)->e_magic != IMAGE_DOS_SIGNATURE)
        {
            ERROR_MESSAGE("AddNewSection:Target File is not a vaild file");
            bOk = false;
            goto EXIT;
        }

        PIMAGE_NT_HEADERS pNtHeaders = (PIMAGE_NT_HEADERS)((ULONG_PTR)FileData + ((PIMAGE_DOS_HEADER)FileData)->e_lfanew);
        if (pNtHeaders->Signature != IMAGE_NT_SIGNATURE)
        {
            ERROR_MESSAGE("AddNewSection:Target File is not a vaild file");
            bOk = false;
            goto EXIT;
        }

        // 判断是否可以增加一个新节
        if ((pNtHeaders->FileHeader.NumberOfSections + 1) * sizeof(IMAGE_SECTION_HEADER) > pNtHeaders->OptionalHeader.SizeOfHeaders/*三个部分的总大小*/)
        {
            ERROR_MESSAGE("AddNewSection:There is not enough space to add a new section.");
            bOk = false;
            goto EXIT;
        }

        // 得到新节的起始地址， 最后的起始地址
        PIMAGE_SECTION_HEADER pNewSectionHeader = (PIMAGE_SECTION_HEADER)(pNtHeaders + 1) + pNtHeaders->FileHeader.NumberOfSections;
        PIMAGE_SECTION_HEADER pLastSectionHeader = pNewSectionHeader - 1;

        // 对齐RVA和偏移
        DWORD FileSize = PEAlign(ulNewSectionSize, pNtHeaders->OptionalHeader.FileAlignment);
        DWORD FileOffset = PEAlign(pLastSectionHeader->PointerToRawData + pLastSectionHeader->SizeOfRawData, pNtHeaders->OptionalHeader.FileAlignment);
        DWORD VirtualSize = PEAlign(ulNewSectionSize, pNtHeaders->OptionalHeader.SectionAlignment);
        DWORD VirtualOffset = PEAlign(pLastSectionHeader->VirtualAddress + pLastSectionHeader->Misc.VirtualSize, pNtHeaders->OptionalHeader.SectionAlignment);

        // 填充新节表
        memcpy(pNewSectionHeader->Name, "Inject", strlen("Inject"));
        pNewSectionHeader->VirtualAddress = VirtualOffset;
        pNewSectionHeader->Misc.VirtualSize = VirtualSize;
        pNewSectionHeader->PointerToRawData = FileOffset;
        pNewSectionHeader->SizeOfRawData = FileSize;
        pNewSectionHeader->Characteristics = IMAGE_SCN_MEM_READ | IMAGE_SCN_MEM_WRITE;

        // 修改IMAGE_NT_HEADERS
        pNtHeaders->FileHeader.NumberOfSections++;
        pNtHeaders->OptionalHeader.SizeOfImage += VirtualSize;
        pNtHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT].Size = 0;            // 关闭绑定导入
        pNtHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT].VirtualAddress = 0;

        // 添加新节到文件尾部
        SetFilePointer(TargetFileHandle, 0, 0, FILE_END);
        PCHAR pNewSectionContent = new CHAR[FileSize];
        RtlZeroMemory(pNewSectionContent, FileSize);
        DWORD dwWrittenLength = 0;
        bOk = WriteFile(TargetFileHandle, pNewSectionContent, FileSize, &dwWrittenLength, nullptr);
        if (bOk == false)
        {
            ERROR_MESSAGE(string("AddNewSection:WriteFile error with error code:" + GetLastError()).c_str());
            bOk = false;
            goto EXIT;
        }
    }
    catch (exception* e)
    {
        ERROR_MESSAGE((string("AddNewSection:") + e->what()).c_str());
        bOk = false;
    }
EXIT:
    if (TargetFileHandle != NULL)
    {
        CloseHandle(TargetFileHandle);
        TargetFileHandle = nullptr;
    }
    if (FileData != NULL)
    {
        UnmapViewOfFile(FileData);
        FileData = nullptr;
    }
    if (MappingHandle != NULL)
    {
        CloseHandle(MappingHandle);
        MappingHandle = nullptr;
    }

    return bOk;
}

ULONG32 PEAlign(ULONG32 dwNumber, ULONG32 dwAlign)
{
    return(((dwNumber + dwAlign - 1) / dwAlign) * dwAlign);        //  想 dwAlign 对齐，加上 dwAlign - 1，这样就可以保证对齐后的值 >= dwNumber
}

BOOL AddNewImportDescriptor(const string& strTargetFile, const string& strInjectDllName, const string& strFunctionName)
{
    bool bOk = true;
    HANDLE TargetFileHandle = nullptr;
    HANDLE MappingHandle = nullptr;
    PVOID FileData = nullptr;
    PIMAGE_IMPORT_DESCRIPTOR pImportTable = nullptr;

    try
    {
        // 打开文件
        TargetFileHandle = CreateFileA(strTargetFile.c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if (TargetFileHandle == INVALID_HANDLE_VALUE)
        {
            ERROR_MESSAGE(string("AddNewImportDescriptor:CreateFileA error with error code:" + GetLastError()).c_str());
            bOk = false;
            goto EXIT;
        }

        ULONG ulFileSize = GetFileSize(TargetFileHandle, NULL);

        // 映射文件
        MappingHandle = CreateFileMappingA(TargetFileHandle, NULL, PAGE_READWRITE, 0, ulFileSize, NULL);
        if (MappingHandle == NULL)
        {
            cout << "AddNewImportDescriptor:CreateFileMapping error with error code:" << std::to_string(GetLastError()).c_str();
            bOk = false;
            goto EXIT;
        }

        // 得到缓存头
        FileData = MapViewOfFile(MappingHandle, FILE_MAP_ALL_ACCESS, 0, 0, ulFileSize);
        if (FileData == NULL)
        {
            ERROR_MESSAGE(string("AddNewImportDescriptor:MapViewOfFile error with error code:" + GetLastError()).c_str());
            bOk = false;
            goto EXIT;
        }

        // 判断是否是PE文件
        if (((PIMAGE_DOS_HEADER)FileData)->e_magic != IMAGE_DOS_SIGNATURE)
        {
            ERROR_MESSAGE("AddNewImportDescriptor:Target File is not a vaild file");
            bOk = false;
            goto EXIT;
        }

        PIMAGE_NT_HEADERS pNtHeaders = (PIMAGE_NT_HEADERS)((ULONG_PTR)FileData + ((PIMAGE_DOS_HEADER)FileData)->e_lfanew);
        if (pNtHeaders->Signature != IMAGE_NT_SIGNATURE)
        {
            ERROR_MESSAGE("AddNewImportDescriptor:Target File is not a vaild file");
            bOk = false;
            goto EXIT;
        }

        // 得到原导入表
        pImportTable = (PIMAGE_IMPORT_DESCRIPTOR)((ULONG_PTR)FileData + RVAToFOA(pNtHeaders, pNtHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress));
        // 判断是否使用了绑定导入表
        bool bBoundImport = false;
        if (pImportTable->Characteristics == 0 && pImportTable->FirstThunk != 0)
        {
            // 桥一为0 桥二不是0 说明使用了绑定导入表
            bBoundImport = true;
            pNtHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT].Size = 0;    // 关闭绑定导入
            pNtHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT].VirtualAddress = 0;
        }

        // 找到自己添加的新节
        PIMAGE_SECTION_HEADER pNewSectionHeader = (PIMAGE_SECTION_HEADER)(pNtHeaders + 1) + pNtHeaders->FileHeader.NumberOfSections - 1;
        PBYTE pNewSectionData = pNewSectionHeader->PointerToRawData + (PBYTE)FileData;
        PBYTE pNewImportDescriptor = pNewSectionData;
        // 往新节中拷贝原导入表内容
        int i = 0;
        while (pImportTable->FirstThunk != 0 || pImportTable->Characteristics != 0)
        {
            memcpy(pNewSectionData + i * sizeof(IMAGE_IMPORT_DESCRIPTOR), pImportTable, sizeof(IMAGE_IMPORT_DESCRIPTOR));
            pImportTable++;
            pNewImportDescriptor += sizeof(IMAGE_IMPORT_DESCRIPTOR);
            i++;
        }
        // 复制最后一个描述符
        memcpy(pNewImportDescriptor, pNewImportDescriptor - sizeof(IMAGE_IMPORT_DESCRIPTOR), sizeof(IMAGE_IMPORT_DESCRIPTOR));

        // 计算修正值
        DWORD dwDelt = pNewSectionHeader->VirtualAddress - pNewSectionHeader->PointerToRawData;

        // pNewImportDescriptor 当前指向要构造的新描述符 再空出一个空描述符作为导入表的结束符 所以是 2 *
        PIMAGE_THUNK_DATA pNewThunkData = PIMAGE_THUNK_DATA(pNewImportDescriptor + 2 * sizeof(IMAGE_IMPORT_DESCRIPTOR));
        PBYTE pszDllName = (PBYTE)(pNewThunkData + 2);
        memcpy(pszDllName, strInjectDllName.c_str(), strInjectDllName.length());
        // 确定 DllName 的位置
        pszDllName[strInjectDllName.length() + 1] = 0;
        // 确定 IMAGE_IMPORT_BY_NAM 的位置
        PIMAGE_IMPORT_BY_NAME pImportByName = (PIMAGE_IMPORT_BY_NAME)(pszDllName + strInjectDllName.length() + 1);
        // 初始化 IMAGE_THUNK_DATA
        pNewThunkData->u1.Ordinal = (DWORD_PTR)pImportByName - (DWORD_PTR)FileData + /*加上修正值 - 这里应该填充在内存中的地址*/dwDelt;
        // 初始化 IMAGE_IMPORT_BY_NAME
        pImportByName->Hint = 1;
        memcpy(pImportByName->Name, strFunctionName.c_str(), strFunctionName.length());
        pImportByName->Name[strFunctionName.length() + 1] = 0;
        // 初始化 PIMAGE_IMPORT_DESCRIPTOR
        if (bBoundImport)
        {
            ((PIMAGE_IMPORT_DESCRIPTOR)pNewImportDescriptor)->OriginalFirstThunk = 0;
        }
        else
        {
            ((PIMAGE_IMPORT_DESCRIPTOR)pNewImportDescriptor)->OriginalFirstThunk = dwDelt + (DWORD_PTR)pNewThunkData - (DWORD_PTR)FileData;
        }
        ((PIMAGE_IMPORT_DESCRIPTOR)pNewImportDescriptor)->FirstThunk = dwDelt + (DWORD_PTR)pNewThunkData - (DWORD_PTR)FileData;
        ((PIMAGE_IMPORT_DESCRIPTOR)pNewImportDescriptor)->Name = dwDelt + (DWORD_PTR)pszDllName - (DWORD_PTR)FileData;
        // 修改导入表入口
        pNtHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress = pNewSectionHeader->VirtualAddress;
        pNtHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size = (i + 1) * sizeof(IMAGE_IMPORT_DESCRIPTOR);
    }
    catch (exception* e)
    {
        ERROR_MESSAGE((string("AddNewImportDescriptor:") + e->what()).c_str());
        bOk = false;
    }

EXIT:
    {
        if (TargetFileHandle != NULL)
        {
            CloseHandle(TargetFileHandle);
            TargetFileHandle = nullptr;
        }

        if (FileData != NULL)
        {
            UnmapViewOfFile(FileData);
            FileData = nullptr;
        }

        if (MappingHandle != NULL)
        {
            CloseHandle(MappingHandle);
            MappingHandle = nullptr;
        }
    }

    return bOk;
}

PIMAGE_SECTION_HEADER GetOwnerSection(PIMAGE_NT_HEADERS pNTHeaders, DWORD dwRVA)
{
    int i;
    PIMAGE_SECTION_HEADER pSectionHeader = (PIMAGE_SECTION_HEADER)(pNTHeaders + 1);
    for (i = 0; i < pNTHeaders->FileHeader.NumberOfSections; i++)
    {
        if ((dwRVA >= (pSectionHeader + i)->VirtualAddress) && (dwRVA <= ((pSectionHeader + i)->VirtualAddress + (pSectionHeader + i)->SizeOfRawData)))
        {
            return ((PIMAGE_SECTION_HEADER)(pSectionHeader + i));
        }
    }
    return PIMAGE_SECTION_HEADER(NULL);
}

DWORD RVAToFOA(PIMAGE_NT_HEADERS pNTHeaders, DWORD dwRVA)
{
    DWORD _offset;
    PIMAGE_SECTION_HEADER section;
    // 找到偏移所在节
    section = GetOwnerSection(pNTHeaders, dwRVA);
    if (section == NULL)
    {
        return(0);
    }
    // 修正偏移
    _offset = dwRVA + section->PointerToRawData - section->VirtualAddress;
    return(_offset);
}