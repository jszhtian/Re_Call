// extFNT.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <Windows.h>
using namespace std;

typedef struct FNTHEADER
{
    char Magic[3];
    unsigned short x;
    unsigned short y;
    DWORD UnkSize;
    DWORD UnkSize2;
}FNT_T;
FNT_T FNTH;

signed int __cdecl DecryptHeader(char* a1)
{
    char v2; // dl
    char v3; // cl

    v2 = *(BYTE*)(a1 + 1);
    *(BYTE*)a1 = ~*(BYTE*)a1;
    v3 = ~*(BYTE*)(a1 + 2);
    *(BYTE*)(a1 + 1) = ~v2;
    *(BYTE*)(a1 + 2) = v3;
    return 1;
}

void Exec(char* FileName)
{
    FILE* fp = fopen(FileName, "rb");
    fseek(fp, 0, SEEK_END);
    DWORD size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    char* buff = (char*)malloc(size);
    if (buff)
    {
        fread(buff, size, 1, fp);
        fclose(fp);
        DecryptHeader(buff);
        memcpy(FNTH.Magic, buff, 0x3);
        if (!strcmp(FNTH.Magic, "FNT") || !strcmp(FNTH.Magic, "FN2"))
        {
            cout << "Header:" << FNTH.Magic << endl;
            FNTH.x = *(unsigned short*)(buff + 0x3);
            FNTH.y = *(unsigned short*)(buff + 0x5);
            cout << "x:0x" << hex << FNTH.x << endl;
            cout << "y:0x" << hex << FNTH.y << endl;
            FNTH.UnkSize = *(DWORD*)(buff + 0x7);
            cout << "UnkSize:0x" << hex << FNTH.UnkSize << endl;
            FNTH.UnkSize2 = *(DWORD*)(buff + 11);
            cout << "UnkSize2:0x" << hex << FNTH.UnkSize2 << endl;
            char* pos1 = buff + 15;
            FNTH.UnkSize = (unsigned int)((FNTH.UnkSize2 + 7) >> 3);
            char* Buffer = (char*)malloc(FNTH.UnkSize);
            memcpy(Buffer, buff, FNTH.UnkSize);
            pos1 += FNTH.UnkSize;
            FNTH.UnkSize = *(DWORD*)pos1;
            char* Buffer2 = (char*)malloc(FNTH.UnkSize);

        }
    }
}

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        //cout << "Usage:" << argv[0] << " File" << endl;
        MessageBox(0, L"exCD.exe <input cd file>", L"AyamiKaze", 0);
        return -1;
    }
    char* FILENAME = argv[1];
    //Cryption(FILENAME);
    Exec(FILENAME);
    system("pause");
    return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
