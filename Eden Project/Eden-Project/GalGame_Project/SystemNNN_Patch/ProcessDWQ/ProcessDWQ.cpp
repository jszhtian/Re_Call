// ProcessDWQ.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <Windows.h>
#include <io.h>
#include <direct.h>
#include <string>
#include <vector>
#include <fstream>
using namespace std;

int E(const char* msg)
{
	MessageBoxA(0, msg, 0, 0);
	return -1;
}

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		MessageBox(0, L"ProcessDWQ.exe <input dwq file>", L"AyamiKaze", 0);
		return -1;
	}
	char* FileName = argv[1];
	FILE* fp = nullptr;

	BYTE* m_pic, * m_dwqBuffer;
	char m_dwq[64];
	DWORD m_dwqSize, m_picBufferSize;
	int m_pictureSizeX, m_pictureSizeY;
	int m_dy;

	auto err = fopen_s(&fp, FileName, "rb");
	fseek(fp, 0, SEEK_END);
	DWORD FileSize = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	fread(m_dwq, sizeof(m_dwq), 1, fp);

	m_dwqSize = *(uint32_t*)&m_dwq[0x20];
	m_pictureSizeX = *(uint32_t*)&m_dwq[0x24];
	m_pictureSizeY = *(uint32_t*)&m_dwq[0x28];
	if (m_pictureSizeY < 0) {
		m_pictureSizeY = -m_pictureSizeY;
		m_dy = 1;
	}
	else
		m_dy = -1;

	m_pic = NULL;
	m_picBufferSize = 0;

	BITMAPFILEHEADER bh;
	BITMAPINFOHEADER bi;
	DWORD pitch;
	DWORD palette_size;

	fread(&bh, sizeof(bh), 1, fp);
	fread(&bi, sizeof(bi), 1, fp);

	pitch = ((m_pictureSizeX * bi.biBitCount / 8) + 3) & ~3;

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
