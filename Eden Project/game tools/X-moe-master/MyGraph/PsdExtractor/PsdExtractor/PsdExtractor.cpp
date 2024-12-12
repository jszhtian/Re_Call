// PsdExtractor.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <string>
#include <cstdio>
#include <Windows.h>
#include "libpsd\include\libpsd.h"

#pragma comment(lib, "libjpeg.lib")
#pragma comment(lib, "libpsd.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "zlib.lib")

#undef NEED_SHORT_EXTERNAL_NAMES

using std::string;

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		MessageBoxW(NULL, L"Usage : PsdExtractor.exe <infile>\n"
			              L"xmoe.project@gmail.com", L"PsdExtractor", MB_OK);
		return -1;
	}
	FILE* fin = NULL;
	fin = fopen(argv[1], "rb");
	if (fin == NULL)
	{
		printf("Cannot open file %s\n", argv[1]);
		return -1;
	}
	else
	{
		fclose(fin);
	}
	psd_context *context = NULL;
	psd_status s = psd_image_load(&context, argv[1]);

	if (psd_status_done != s)
	{
		MessageBoxW(NULL, L"Failed to read file", L"PsdExtractor", MB_OK);
		return -1;
	}
	printf("%s contains %d Layer(s)\n", context->caption, context->layer_count);
	printf("Depth %d\n", context->depth);

	//for (int i = 0; i < context->layer_count; i++)
	//{
		BITMAPFILEHEADER bmf;
		BITMAPINFOHEADER bmi;

		memset(&bmf, 0, sizeof(bmf));
		memset(&bmi, 0, sizeof(bmi));

		bmf.bfType = 0x4D42;
		bmf.bfSize = sizeof(bmf)+sizeof(bmi)+context->temp_image_length;
		bmf.bfOffBits = sizeof(bmf)+sizeof(bmi);

		bmi.biSize = sizeof(bmi);
		bmi.biWidth = context->width;
		bmi.biHeight = context->height;
		bmi.biPlanes = 1;
		bmi.biBitCount = context->depth * 8;

		FILE* fp = fopen((string(argv[1]) + string(".bmp")).c_str(), "wb");
		fwrite(&bmf, sizeof(bmf), 1, fp);
		fwrite(&bmi, sizeof(bmi), 1, fp);
		fwrite(context->temp_image_data, 1, context->temp_image_length, fp);
		fclose(fp);
	//}
	return 0;

}

