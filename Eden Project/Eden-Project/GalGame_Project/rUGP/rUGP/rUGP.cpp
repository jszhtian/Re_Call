// rUGP.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <Windows.h>
using namespace std;
char __cdecl UTIL_DecryptIci(LPVOID lpBuffer, int nSize, LPVOID lpOutput, int outLen)
{
    char* v4; // ecx
    BYTE* v5; // edi
    char* v6; // ebx
    BYTE* v7; // esi
    int v8; // edx
    char v9; // al
    int v10; // ebx
    BYTE* v11; // edx
    int v12; // esi
    char v13; // al
    char* v14; // ebx
    BYTE* v15; // esi
    int v16; // edx
    char v17; // al
    int v18; // ebx
    char* v19; // edx
    int v20; // esi
    char v21; // cl
    char v22; // dl
    char v23; // al
    char* v24; // esi
    int v25; // ebx
    int v26; // eax
    int v27; // edx
    int v28; // edi
    char v29; // al
    BYTE* j; // edi
    int v32; // [esp+Ch] [ebp-Ch]
    int v33; // [esp+Ch] [ebp-Ch]
    int v34; // [esp+14h] [ebp-4h]
    char i; // [esp+14h] [ebp-4h]
    int v36; // [esp+14h] [ebp-4h]

    v4 = (char*)lpOutput;
    v34 = outLen / 6;
    v5 = lpBuffer;
    v6 = (char*)lpBuffer;
    v32 = outLen % 6;
    v7 = lpOutput;
    v8 = nSize / 6;
    if (nSize / 6 > 0)
    {
        do
        {
            v9 = *v6++;
            *v7 = v9;
            --v8;
            v7[1] = v6[v34 - 1];
            v7[2] = v6[2 * v34 - 1];
            v7[3] = v6[3 * v34 - 1];
            v7[4] = v6[4 * v34 - 1];
            v7[5] = v6[5 * v34 - 1];
            v7 += 6;
        } while (v8 > 0);
        v4 = (char*)lpOutput;
        v5 = lpBuffer;
    }
    v10 = outLen % 6;                             // 剩余字节
    v11 = &v5[outLen - v32];
    if (v32 > 0)
    {
        do
        {
            ++v7;
            --v10;
            *(v7 - 1) = *v11++;                       // 仅复制
        } while (v10 > 0);
    }
    v12 = 0;
    for (i = 0; v12 < outLen; ++v12)
    {
        v13 = (v4[v12] - i) ^ 0xA5;
        i = v4[v12];
        v4[v12] = v13;
    }
    v14 = v4;
    v15 = v5;
    v36 = outLen / 5;
    v33 = outLen % 5;
    v16 = nSize / 5;
    if (nSize / 5 > 0)
    {
        do
        {
            v17 = *v14++;
            *v15 = v17;
            --v16;
            v15[1] = v14[v36 - 1];
            v15[2] = v14[2 * v36 - 1];
            v15[3] = v14[3 * v36 - 1];
            v15[4] = v14[4 * v36 - 1];
            v15 += 5;
        } while (v16 > 0);
        v4 = (char*)lpOutput;
        v5 = lpBuffer;
    }
    v18 = outLen % 5;
    v19 = &v4[outLen - v33];
    if (v33 > 0)
    {
        do
        {
            ++v15;
            --v18;
            *(v15 - 1) = *v19++;
        } while (v18 > 0);
    }
    v20 = outLen;
    if (outLen > 0)
    {
        v21 = 0;
        do
        {
            v22 = v5[v20-- - 1];
            v23 = v22 - v21;
            v21 = v22;
            v5[v20] = v23;
        } while (v20 > 0);
        v4 = (char*)lpOutput;
    }
    v24 = v5;
    v25 = outLen / 3;
    v26 = 3 * (outLen / 3);
    v27 = outLen % 3;
    if (outLen / 3 > 0)
    {
        v28 = 2 * v25;
        do
        {
            v29 = *v24++;
            --v25;
            *v4 = v29 ^ 0x18;
            v4[1] = v24[outLen / 3 - 1] ^ 0x3F;
            LOBYTE(v26) = v24[v28 - 1] ^ 0xE2;
            v4[2] = v26;
            v4 += 3;
        } while (v25 > 0);
        v5 = lpBuffer;
        v27 = outLen % 3;
    }
    for (j = &v5[outLen - v27]; v27 > 0; ++j)
    {
        LOBYTE(v26) = *j;
        ++v4;
        --v27;
        *(v4 - 1) = *j;
    }
    return v26;
}

int main()
{
    std::cout << "Hello World!\n";
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
