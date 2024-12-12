// EncryptAYMScript.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <stdio.h>
#include <tchar.h>
#include "windows.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <string.h>
#include <vector>
#include <unordered_map>

#include "zdict.h"
#include "zlib.h"
#include "zstd.h"
#include "zstd_errors.h"

#include "crc32.h"

#include "Key.h"
using namespace std;
unordered_map<UINT, string>TextList;
#pragma pack(1)
struct AYMHeader
{
    CHAR Magic[4]; // "AYM\0"
    UINT Version;
    UINT StrCount;
};
struct AYMIndex
{
    UINT off;
    UINT jmp_off;
};
struct CompressInfo
{
    UINT UncompressSize;
    UINT CompressSize;
};
#pragma pack()

int E(const char* msg)
{
    MessageBoxA(0, msg, 0, 0);
    return -1;
}

vector<string> SplitTxtA(BYTE* txt, DWORD size)
{
    auto buff = (uint8_t*)txt;
    uint32_t i = 0;
    uint32_t last = 0;
    auto ls = vector<string>();
    while (true)
    {
        while (i < size && buff[i] != '\r')
            i++;
        ls.push_back(string((char*)buff + last, i - last));
        if (i + 2 > size)
            break;
        i += 2;
        last = i;
    }
    return ls;
}

vector<UINT> SplitIdx(BYTE* buff, DWORD size)
{
    auto ls = vector<UINT>();
    for (int i = 0; i < size; i += 4)
        ls.push_back(*(DWORD*)(buff + i));
    return ls;
}

static char* wtoc(LPCTSTR str, UINT cp)
{
    DWORD dwMinSize;
    dwMinSize = WideCharToMultiByte(cp, NULL, str, -1, NULL, 0, NULL, FALSE); //计算长度
    char* out = new char[dwMinSize];
    WideCharToMultiByte(cp, NULL, str, -1, out, dwMinSize, NULL, FALSE);//转换
    return out;
}

static LPWSTR ctow(char* str, UINT cp)
{
    DWORD dwMinSize;
    dwMinSize = MultiByteToWideChar(cp, 0, str, -1, NULL, 0); //计算长度
    LPWSTR out = new wchar_t[dwMinSize];
    MultiByteToWideChar(cp, 0, str, -1, out, dwMinSize);//转换
    return out;
}

#if 0
int main(int argc, char* argv[])
{
    AYMHeader *hdr;
    //AYMIndex *idx;
    if (argc != 2)
    {
        MessageBox(0, L"CreateAYMScript.exe <input file>", L"AyamiKaze", 0);
        return -1;
    }
    string fnm = argv[1];
    //string fnm = "00_001.aym";
    FILE* fp;
    fopen_s(&fp, fnm.c_str(), "rb");
    if (!fp)
        return E("No fp");
    fseek(fp, 0, SEEK_END);
    DWORD size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    BYTE* buff = new BYTE[size];
    fread(buff, size, 1, fp);
    fclose(fp);

    hdr = (AYMHeader*)buff;
    CRC32 crc;
    DWORD crcVal = crc.Calc((char*)hdr, sizeof(AYMHeader));


    DWORD IndexSize = hdr->StrCount * sizeof(AYMIndex);
    DWORD ResSzie = size - sizeof(AYMHeader) - IndexSize;

    DWORD pk_IndexSize = hdr->StrCount * sizeof(AYMIndex);
    DWORD pk_ResSzie = size - sizeof(AYMHeader) - IndexSize;

    BYTE* idx = new BYTE[IndexSize];
    BYTE* res = new BYTE[ResSzie];

    BYTE* pk_idx = new BYTE[IndexSize];
    BYTE* pk_res = new BYTE[ResSzie];

    memcpy(idx, buff + sizeof(AYMHeader), IndexSize);
    memcpy(res, buff + sizeof(AYMHeader) + IndexSize, ResSzie);

    cout << "IndexSize:0x" << hex << IndexSize << endl;
    cout << "ResSzie:0x" << hex << ResSzie << endl;

    if (compress(pk_idx, &pk_IndexSize, idx, IndexSize) != Z_OK)
        return E("Zlib Compress Error");
    size_t ret = ZSTD_compress(pk_res, pk_ResSzie, res, ResSzie, 1);
    if (ZSTD_isError(ret) != ZSTD_error_no_error)
        return E("ZSTD Compress Error");
    pk_ResSzie = ret;

    cout << "pk_IndexSize:0x" << pk_IndexSize << endl;
    cout << "pk_ResSzie:0x" << pk_ResSzie << endl;

    hdr->StrCount ^= crcVal;

    for (int i = 0; i < pk_IndexSize; i++)
        pk_idx[i] ^= IdxKey[i % 1024];
    for (int i = 0; i < pk_ResSzie; i++)
        pk_res[i] ^= ResKey[i % 1024];

    CompressInfo idx_info;
    idx_info.UncompressSize = IndexSize;
    idx_info.CompressSize = pk_IndexSize;

    CompressInfo res_info;
    res_info.UncompressSize = ResSzie;
    res_info.CompressSize = pk_ResSzie;

    fnm = fnm.substr(0, fnm.length() - 4);
    fnm += ".aym";

    fopen_s(&fp, fnm.c_str(), "wb");
    if (!fp)
        return E("No fp2");

    fwrite(&crcVal, 4, 1, fp);

    fwrite(hdr, sizeof(AYMHeader), 1, fp);

    fwrite(&idx_info, sizeof(CompressInfo), 1, fp);
    fwrite(pk_idx, pk_IndexSize, 1, fp);

    fwrite(&res_info, sizeof(CompressInfo), 1, fp);
    fwrite(pk_res, pk_ResSzie, 1, fp);

    fclose(fp);

    //system("pause");
    return 0;
}
#endif

#if 1
int main()
{
    FILE* fp = nullptr;
    fopen_s(&fp, "Scr.aym", "rb");
    if (!fp)
        return E("No fp");
    fseek(fp, 0, SEEK_END);
    DWORD size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    BYTE* buff = new BYTE[size];
    fread(buff, size, 1, fp);
    fclose(fp);

    AYMHeader* hdr = (AYMHeader*)(buff + 4);
    hdr->StrCount ^= *(DWORD*)buff;
    cout << "line:0x" << hex << hdr->StrCount << endl;

    // idx
    CompressInfo* idx_info = (CompressInfo*)(buff + 4 + sizeof(AYMHeader));
    cout << "IdxSize:0x" << hex << idx_info->UncompressSize << endl;
    cout << "pk_IdxSize:0x" << hex << idx_info->CompressSize << endl;

    BYTE* idx = buff + 4 + sizeof(AYMHeader) + sizeof(CompressInfo);
    for (int i = 0; i < idx_info->CompressSize; i++)
        idx[i] ^= IdxKey[i % 1024];

    BYTE* upk_idx = new BYTE[idx_info->UncompressSize];
    DWORD upk_idx_sz = idx_info->UncompressSize;
    if (uncompress(upk_idx, &upk_idx_sz, idx, idx_info->CompressSize) != Z_OK)
        return E("Zlib Uncompress Error");

    // res
    CompressInfo* res_info = (CompressInfo*)(buff + 4 + sizeof(AYMHeader) + sizeof(CompressInfo) + idx_info->CompressSize);
    cout << "ResSize:0x" << hex << res_info->UncompressSize << endl;
    cout << "pk_ResSize:0x" << hex << res_info->CompressSize << endl;

    BYTE* res = buff + 4 + sizeof(AYMHeader) + sizeof(CompressInfo) + idx_info->CompressSize + sizeof(CompressInfo);
    for (int i = 0; i < res_info->CompressSize; i++)
        res[i] ^= ResKey[i % 1024];

    BYTE* upk_res = new BYTE[res_info->UncompressSize];
    DWORD upk_res_sz = res_info->UncompressSize;
    size_t ret = ZSTD_decompress(upk_res, upk_res_sz, res, res_info->CompressSize);
    if (ZSTD_isError(ret) != ZSTD_error_no_error)
        return E("ZSTD Decompress Error");

    AYMIndex* aidx = (AYMIndex*)upk_idx;

    for (int i = 0; i < hdr->StrCount; i++)
    {
        char* Cstring = (char*)upk_res + aidx[i].jmp_off;
        string str = string(Cstring);
        TextList.insert(pair<UINT, string>(aidx[i].off, str));
    }

    delete[] buff;

    UINT off = 0x144;
    auto it = TextList.find(off);
    if (it != TextList.end())
        cout << wtoc(ctow((char*)it->second.c_str(), CP_UTF8), 936) << endl;
    system("pause");
    return 0;
}
#endif
// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
