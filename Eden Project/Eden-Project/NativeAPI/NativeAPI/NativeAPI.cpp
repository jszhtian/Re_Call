// NativeAPI.cpp : 定义静态库的函数。
//

#include "pch.h"
#include "framework.h"

// TODO: 这是一个库函数示例
bool IsFile(const char* FileName)
{
	FILE* fp = fopen(FileName, "rb");
	if (!fp)
		return false;
	else
		return true;
}

void Error(const char* Text)
{
	MessageBoxA(0, Text, "Aym", 0);
}

bool AymFile(char* buffer, DWORD size, const char* FileName , const char* mode)
{
	if (mode == "rb") 
	{
		FILE* fp = fopen(FileName, "rb");
		if (!fp)
		{
			Error("Can't find File.");
			return false;
		}
		fseek(fp, 0, SEEK_END);
		size = ftell(fp);
		fseek(fp, 0, SEEK_SET);
		buffer = (char*)malloc(size);
		if (!buffer || !size)
		{
			Error("No Buffer or Size.");
			return false;
		}
		fread(buffer, size, 1, fp);
		fclose(fp);
		return true;
	}
	else if (mode == "wb")
	{
		FILE* fp = fopen(FileName, "wb");
		if (!buffer || !size)
		{
			Error("No Buffer or Size.");
			return false;
		}
		fwrite(buffer, size, 1, fp);
		fclose(fp);
		return true;
	}

}

void fnNativeAPI()
{

}
