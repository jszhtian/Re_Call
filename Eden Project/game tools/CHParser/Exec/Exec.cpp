// Exec.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>


// asb-filter.cpp  by  wiki908@github

#include <stdio.h>


int openfs(const wchar_t* fn, char*& buff)
{
    FILE* fs = _wfopen(fn, L"rb");
    if (fs == 0) throw 1;
    fseek(fs, 0, SEEK_END);
    int len = ftell(fs);
    fseek(fs, 0, SEEK_SET);
    buff = new char[len];
    fread(buff, 1, len, fs);
    if (ferror(fs)) throw 1;
    fclose(fs);
    return len;
}

// Line structure:
// 00 00 00 03 ** ** ** ** 07 .N 00 ** 07 .S 00

void filt(const char* buff, const int len, FILE* fp)
{
    for (int i = 0, cnt = 0; i < len; ++i)
    {
        if (buff[i] == 0)
        {
            ++cnt;
        }
        else if (cnt >= 3 && buff[i] == 0x03)
        {
            // test chara name
            i += 5;
            if (i >= len) break;
            if (buff[i] == 0x07)
            {
                // printf("match ");

                fputs(buff + i + 1, fp);
                fputs("\n", fp);
                if (ferror(fp)) throw 1;
                i += strlen(buff + i);

                // test main text
                i += 2;
                if (i >= len) break;
                if (buff[i] == 0x07)
                {
                    fputs(buff + i + 1, fp);
                    fputs("\n", fp);
                    if (ferror(fp)) throw 1;
                    i += strlen(buff + i);
                    cnt = 0;
                }
                else
                {
                    // do not throw
                    cnt = 0;
                }
            }
            else
            {
                cnt = 0;
            }
        }
        else
        {
            cnt = 0;
        }
    }
    fflush(fp);
}

int wmain(int argc, wchar_t* argv[])
{
    try {
        if (argc != 3)
        {
            printf("exe i.asb o.txt\n");
            return 0;
        }

        char* buff;
        int len = openfs(argv[1], buff);

        FILE* fp = _wfopen(argv[2], L"w+");
        if (fp == 0) throw 1;
        filt(buff, len, fp);
        fclose(fp);

        delete[] buff;
    }
    catch (...)
    {
        printf("error\n");
        return 1;
    }
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
