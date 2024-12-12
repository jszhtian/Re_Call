// extSOB2.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string>
#include <set>
#include <map>
#include <vector>
#include <fstream>
#include <Windows.h>
using namespace std;

int main(int argc, char** argv)
{
    char out[MAX_PATH];
    auto f = fopen(argv[1], "rb");
    strcpy(out, argv[1]);
    fseek(f, 0x00, SEEK_END);
    DWORD size = ftell(f);
    fseek(f, 0x00, SEEK_SET);
    char* buff = (char*)malloc(size);
    fread(buff, size, 1, f);
    fclose(f);
    buff += 0x4;
    DWORD tbsize = *(DWORD*)buff;
    buff += 0x4 + tbsize;
    DWORD start = *(DWORD*)buff;
    buff += 0x4;
    DWORD end = *(DWORD*)buff;
    buff += 0x4 + start;
    DWORD sizeoftext = end - start;
    cout << hex << size << endl;
    cout << hex << tbsize << endl;
    cout << hex << start << endl;
    cout << hex << end << endl;
    cout << hex << sizeoftext << endl;
    for (int i = 0; i < sizeoftext; i++)
    {
        if (buff[i] == '\x00' && buff[i + 2] == '\x00' || buff[i] == '\x00' && buff[i + 2] == '\x01')
        {
            buff[i] = '\n';
            buff[i + 1] = '\n';
            buff[i + 2] = '\n';
        }
        if (buff[i] == '\x00')
            buff[i] = '\n';
    }
    strcat(out, ".txt");
    FILE* fp = fopen(out, "wb");
    fwrite(buff, sizeoftext, 1, fp);
    fclose(fp);
    //system("pause");
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
