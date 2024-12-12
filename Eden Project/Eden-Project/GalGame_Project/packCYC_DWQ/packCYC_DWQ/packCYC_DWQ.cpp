// packCYC_DWQ.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include <iostream>
#include <Windows.h>
#include <io.h>
#include <direct.h>
#include <string>
#include <vector>
#include "png.h"
#include "pngloader.h"
#pragma comment(lib, "libpng16.lib")
using namespace std;

char pngHeader[] = "\
PNG             \
                \
                \
PACKTYPE=8A     ";


int E(const char* msg)
{
	MessageBoxA(0, msg, 0, 0);
	return -1;
}

char* m_loadBuffer;
char* m_loadBuffer2;
char* m_saveBuffer;

RECT CalcuCutPng(SIZE picSize, char* maskData)
{
	RECT rc;

	//cut main
	int upperCut = 0;
	int lowerCut = 0;
	int leftCut = 0;
	int rightCut = 0;

	int i, j;

	int picSizeX = picSize.cx;
	int picSizeY = picSize.cy;


	for (j = 0; j < picSizeY - 1; j++)
	{
		BOOL found = FALSE;
		char* ptr = maskData;
		ptr += j * picSizeX;

		for (i = 0; i < picSizeX; i++)
		{
			char c = *ptr;
			if (c != 0)
			{
				found = TRUE;
				break;
			}
			ptr++;
		}

		if (found)
		{
			break;
		}
		upperCut++;
	}

	for (j = picSizeY - 1; j > 0; j--)
	{
		BOOL found = FALSE;
		char* ptr = maskData;
		ptr += j * picSizeX;

		for (i = 0; i < picSizeX; i++)
		{
			char c = *ptr;
			if (c != 0)
			{
				found = TRUE;
				break;
			}
			ptr++;
		}

		if (found)
		{
			break;
		}
		lowerCut++;
	}



	if ((upperCut + lowerCut) >= picSizeY)
	{
		upperCut = picSizeY - 1;
		lowerCut = 0;
	}


	for (i = 0; i < picSizeX - 1; i++)
	{
		BOOL found = FALSE;
		char* ptr = maskData;
		ptr += i;

		for (j = 0; j < picSizeY; j++)
		{
			char c = *ptr;
			if (c != 0)
			{
				found = TRUE;
				break;
			}

			ptr += picSizeX;
		}

		if (found)
		{
			break;
		}
		leftCut++;
	}

	for (i = picSizeX - 1; i > 0; i--)
	{
		BOOL found = FALSE;
		char* ptr = maskData;
		ptr += i;

		for (j = 0; j < picSizeY; j++)
		{
			char c = *ptr;
			if (c != 0)
			{
				found = TRUE;
				break;
			}


			ptr += picSizeX;
		}

		if (found)
		{
			break;
		}
		rightCut++;
	}

	if ((leftCut + rightCut) >= picSizeX)
	{
		leftCut = picSizeX - 1;
		rightCut = 0;
	}


	rc.top = upperCut;
	rc.left = leftCut;
	rc.right = rightCut;
	rc.bottom = lowerCut;

	return rc;
}

int main(int argc, char* argv[])
{
	/*if (argc != 2)
	{
		MessageBox(0, L"packCYC_DWQ.exe <input png file>", L"AyamiKaze", 0);
		return -1;
	}*/
	CPngLoader* m_pngLoader = new CPngLoader(8);
	//char* FileName = argv[1];
	char* FileName = (char*)"test.png";

	if (!m_pngLoader->LoadFile(FileName))
		return E("LoadFileError.");
	cout << "Read:" << FileName << endl;
	RECT cutter;
	cutter.left = 0;
	cutter.right = 0;
	cutter.top = 0;
	cutter.bottom = 0;

	SIZE pngPicSize = m_pngLoader->GetPicSize();
	SIZE size;
	size.cx = pngPicSize.cx;
	size.cy = pngPicSize.cy;

	if (m_pngLoader->GetMaskFlag())
	{
		m_pngLoader->GetMaskData(m_loadBuffer);
		cutter = CalcuCutPng(pngPicSize, m_loadBuffer);
	}

	cout << "info fin"  << endl;

	FILE* file = fopen(FileName, "rb");

	int fileSize = fread(m_loadBuffer, sizeof(char), 1024 * 16384, file);
	fclose(file);
	cout << "ReadFin" << endl;
	int headerBuffer[64 / 4];
	memcpy(headerBuffer, pngHeader, 64);

	headerBuffer[8] = fileSize;
	headerBuffer[9] = size.cx;
	headerBuffer[10] = size.cy;


	//	RECT cutter = CalcuCutData(srcDir,filename);

	char dst[MAX_PATH];
	strcpy(dst, FileName);
	strcat(dst, ".dwq");



	headerBuffer[4] = cutter.top;
	headerBuffer[5] = cutter.left;
	headerBuffer[6] = cutter.right;
	headerBuffer[7] = cutter.bottom;




	int sx = size.cx;
	int sy = size.cy;


	FILE* file2 = NULL;
	fopen_s(&file2, dst, "wb");
	if (file2 == NULL)
	{
		return E("Can't create DWQ file");
	}


	fwrite(headerBuffer, sizeof(char), 64, file2);
	fwrite(m_loadBuffer, sizeof(char), fileSize, file2);


	//mask data
//	MakeAndSaveMask(file2,srcDir,filename);//@@@


	fclose(file2);


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
