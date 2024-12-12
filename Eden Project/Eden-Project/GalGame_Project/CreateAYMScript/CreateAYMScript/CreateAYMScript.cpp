// CreateAYMScript.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
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
#include <map>
#include <unordered_map>
using namespace std;

const UINT AYM_Version = 0x00000001;

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
#pragma pack()

int E(const char* msg)
{
    MessageBoxA(0, msg, 0, 0);
    return -1;
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



unordered_map<UINT, string>TextList;

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

DWORD SetMap(string fnm)
{
    string txt = "txt\\" + fnm + ".txt";
    string idx = "idx\\" + fnm + ".idx";
    FILE* fp = nullptr;
    // txt
    fopen_s(&fp, txt.c_str(), "rb");
    if (!fp)
    {
        CHAR err_msg[MAX_PATH] = { 0 };
        wsprintfA(err_msg, "找不到%s", txt.c_str());
        MessageBoxA(0, err_msg, "错误", MB_OK | MB_ICONERROR);
        ExitProcess(-1);
    }
    fseek(fp, 0, SEEK_END);
    DWORD Txt_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    BYTE* Txt_buff = new BYTE[Txt_size];
    fread(Txt_buff, 1, Txt_size, fp);
    fclose(fp);

    // idx
    fopen_s(&fp, idx.c_str(), "rb");
    if (!fp)
    {
        CHAR err_msg[MAX_PATH] = { 0 };
        wsprintfA(err_msg, "找不到%s", idx.c_str());
        MessageBoxA(0, err_msg, "错误", MB_OK | MB_ICONERROR);
        ExitProcess(-1);
    }
    fseek(fp, 0, SEEK_END);
    DWORD Idx_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    BYTE* Idx_buff = new BYTE[Idx_size];
    fread(Idx_buff, 1, Idx_size, fp);
    fclose(fp);

    auto txt_line = SplitTxtA(Txt_buff + 3, Txt_size - 3);// 跳过BOM
    auto idx_line = SplitIdx(Idx_buff + 4, Idx_size - 4); // 跳过头部CRC

    if (txt_line.size() != idx_line.size())
    {
        CHAR err_msg[MAX_PATH] = { 0 };
        wsprintfA(err_msg, "%s行数不匹配。", fnm.c_str());
        MessageBoxA(0, err_msg, "错误", MB_OK | MB_ICONERROR);
        ExitProcess(-1);
    }

    for (int i = 0; i < txt_line.size(); i++)
    {
        //cout << hex << idx_line[i] << "|" << wtoc(ctow((CHAR*)txt_line[i].c_str(), CP_UTF8), 936) << endl;

        TextList.insert(pair<UINT, string>(idx_line[i], txt_line[i]));
    }
    return txt_line.size();
}
static const char* FileList[]{
    "00_001",
    "00_002",
    "00_003",
    "00_101",
    "00_102",
    "00_103",
    "00_104",
    "00_104b",
    "00_105",
    "00_106",
    "00_107",
    "00_108",
    "00_108a",
    "00_108b",
    "00_108c",
    "00_201",
    "00_201b",
    "00_202",
    "00_203",
    "00_204",
    "00_204a",
    "00_204b",
    "00_204ba",
    "00_204bb",
    "01_001",
    "01_002",
    "01_003",
    "01_003b",
    "01_004",
    "01_004a",
    "01_004b",
    "01_004ba",
    "01_004bb",
    "01_101",
    "01_102",
    "01_103",
    "01_104",
    "01_105",
    "01_901",
    "01_902",
    "02_001",
    "02_002",
    "02_003",
    "02_004",
    "02_005",
    "02_006",
    "02_007",
    "02_008",
    "02_009",
    "02_010",
    "03_001",
    "03_002",
    "03_005",
    "03_006",
    "03_007",
    "03_008",
    "03_008b",
    "03_009",
    "03_010",
    "03_011",
    "03_012",
    "03_013",
    "03_013b",
    "03_014",
    "03_015",
    "04_001",
    "04_002",
    "04_003",
    "04_004",
    "04_005",
    "04_006",
    "04_007",
    "04_008",
    "04_009",
    "04_010",
    "t00_002",
    "t00_003",
    "t00_101",
    "t00_102",
    "t00_103",
    "t00_104",
    "t00_201",
    "t02_001",
    "t03_002",
    "t04_004",
    "trial_inf",
};
#if 1
int main(int argc, char* argv[])
{
    AYMHeader hdr;
    AYMIndex idx;
    for (string fnm : FileList) 
    {
        DWORD LineCount = SetMap(fnm);
        memcpy(hdr.Magic, "AYM\0", 4);
        hdr.Version = AYM_Version;
        cout << "line:" << dec << LineCount << endl;
        hdr.StrCount = LineCount;
        UINT IndexSize = LineCount * sizeof(AYMIndex);

        FILE* fp = nullptr;
        fnm += ".bin";
        fopen_s(&fp, fnm.c_str(), "wb");
        if (!fp)
        {
            MessageBox(0, L"创建文件失败。", L"错误", MB_OK | MB_ICONERROR);
            ExitProcess(-1);
        }

        fwrite(&hdr, sizeof(AYMHeader), 1, fp);
        DWORD pos = 0;
        DWORD pos2 = 0;
        for (auto& it : TextList)
        {
            CHAR* str = (CHAR*)it.second.c_str();
            lstrcatA(str, "\0");

            idx.off = it.first;
            DWORD sz = lstrlenA(str) + 1;
            idx.jmp_off = pos2;
            fwrite(&idx, sizeof(AYMIndex), 1, fp);

            fseek(fp, pos2 + sizeof(AYMHeader) + IndexSize, SEEK_SET);
            fwrite(str, sz, 1, fp);

            pos2 += sz;
            pos += sizeof(AYMIndex);

            fseek(fp, pos + sizeof(AYMHeader), SEEK_SET);
        }
        fclose(fp);
        TextList.clear();
    }
    system("pause");
    return 0;
}
#endif

#if 0
int main()
{
    FILE* fp;
    fopen_s(&fp, "00_001.aym", "rb");
    fseek(fp, 0, SEEK_END);
    DWORD size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    BYTE* buff = new BYTE[size];
    fread(buff, size, 1, fp);
    fclose(fp);

    AYMHeader *hdr;
    AYMIndex *idx;
    hdr = (AYMHeader*)(buff);
    if (memcmp(hdr->Magic, "AYM\0", 4))
        return E("No Magic");
    DWORD IndexSize = sizeof(AYMIndex) * hdr->StrCount;
    idx = new AYMIndex[IndexSize];
    memcpy(idx, buff + sizeof(AYMHeader), IndexSize);

    for (int i = 0; i < hdr->StrCount; i++)
    {
        CHAR* Cstr = (CHAR*)(buff + IndexSize + sizeof(AYMHeader) + idx[i].jmp_off);
        string str = string(Cstr);
        TextList.insert(pair<UINT, string>(idx[i].off, str));
    }

    delete[] buff;

    UINT off = 0x4C86;
    auto it = TextList.find(off);
    if (it != TextList.end())
        cout << wtoc(ctow((char*)it->second.c_str(), CP_UTF8),936) << endl;

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
