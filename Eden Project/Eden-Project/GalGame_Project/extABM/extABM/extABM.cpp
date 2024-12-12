// extABM.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <Windows.h>
#include <io.h>
#include <direct.h>
#include <string>
#include <vector>
using namespace std;

int E(const char* msg)
{
	MessageBoxA(0, msg, 0, 0);
	return -1;
}

int write_bmp(const    string& filename,
    unsigned char* buff,
    unsigned long    len,
    unsigned long    width,
    unsigned long    height,
    unsigned short   depth_bytes)
{
    BITMAPFILEHEADER bmf;
    BITMAPINFOHEADER bmi;

    memset(&bmf, 0, sizeof(bmf));
    memset(&bmi, 0, sizeof(bmi));

    bmf.bfType = 0x4D42;
    bmf.bfSize = sizeof(bmf) + sizeof(bmi) + len;
    bmf.bfOffBits = sizeof(bmf) + sizeof(bmi);

    bmi.biSize = sizeof(bmi);
    bmi.biWidth = width;
    bmi.biHeight = height;
    bmi.biPlanes = 1;
    bmi.biBitCount = depth_bytes * 8;

    FILE* fd = fopen(filename.c_str(), "wb");
    fwrite(&bmf, 1, sizeof(bmf), fd);
    fwrite(&bmi, 1, sizeof(bmi), fd);
    fwrite(buff, 1, len, fd);
    fclose(fd);
    return 0;
}

/*
            if (1 == type || 2 == type)
            {
                int count = header.ToUInt16 (0x3A);
                if (count > 0xFF)
                    return null;
                frame_offset = header.ToUInt32 (0x42);
            }
            else if (32 == type || 24 == type || 8 == type || -8 == type)
            {
                uint unpacked_size = header.ToUInt32 (2);
                if (0 == unpacked_size || unpacked_size == stream.Length) // probably an ordinary bmp file
                    return null;
                frame_offset = header.ToUInt32 (0xA);
                if (8 == type)
                    bpp = 8;
            }
            else
                return null;
            if (frame_offset >= stream.Length)
                return null;
*/

int main(int argc, char* argv[])
{
    char* FileName = (char*)"btn_ret.abm";//argv[1];
    FILE* fp = fopen(FileName, "rb");
    fseek(fp, 0, SEEK_END);
    unsigned int FileSize = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    char* header = (char*)malloc(0x46);
    fread(header, 0x46, 1, fp);

    if (header[0] != 'B' && header[1] != 'M')
        return E("Not BM");

    unsigned int type, frame_offset, bpp, Width, Height;
    Width = *(unsigned int*)(header + 0x12);
    Height = *(unsigned int*)(header + 0x16);
    type = *(unsigned int*)(header + 0x1C);
    bpp = 24;
    cout << "abm type:" << type << endl;
    cout << "abm Width:" << Width << endl;
    cout << "abm Height:" << Height << endl;
    if (type == 1 || type == 2)
    {
        int count = *(unsigned int*)(header + 0x3A);
        if (count > 0xFF)
            return E("count over than 0xFF");
        frame_offset = *(unsigned int*)(header + 0x42);
    }
    else if (type == 32 || type == 24 || type == 8 || type == -8)
    {
        unsigned int unpacked_size = *(unsigned int*)(header + 2);
        if (unpacked_size == 0 || unpacked_size == FileSize)
            return E("Unk format");
        frame_offset = *(unsigned int*)(header + 0xA);
        if (type == 8)
            bpp = 8;
    }
    else
        return E("unsupport format");
    if (frame_offset >= FileSize)
        return E("Bad frame_offset");
    cout << "abm frame_offset:" << "0x" << hex << frame_offset << endl;
    cout << "abm bpp:" << bpp << endl;

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
