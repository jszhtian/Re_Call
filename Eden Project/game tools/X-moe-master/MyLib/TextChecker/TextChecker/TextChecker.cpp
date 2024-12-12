// TextChecker.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <string>
#include <fstream>
#include <vector>
#include <Windows.h>

using std::string;
using std::wstring;
using std::vector;

typedef struct TextBuffer
{
	std::wstring OldText;
	std::wstring NewText;
}TextBuffer;


static unsigned char utf8_bom[3] = { 0xEF, 0xBB, 0xBF };
static unsigned char utf16le_bom[2] = { 0xFF, 0xFE };

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		return -1;
	}
	std::fstream fin(argv[1]);
	char ReadLine[1024] = { 0 };

	FILE* out = NULL;
	string LogFile = argv[1];
	LogFile += ".LOG";
	out = fopen(LogFile.c_str(), "wb");
	if (!out)
	{
		MessageBox(NULL, L"可能是：exe和目标文本不在一个目录下", L"Error", MB_OK);
		return -1;
	}

	//fwrite(utf16le_bom, 1, 2, out);

	int warning = 0;
	int error = 0;
	DWORD iPos = 0;
	bool bom = false;


	try
	{
		while (fin.getline(ReadLine, 1024))
		{
			if (ReadLine[0] == '[')
			{
				string ReadBuffer = ReadLine;
				string truText = ReadBuffer.substr(12, ReadBuffer.length());

				char tmp[11] = { 0 };
				memcpy(tmp, ReadLine + 1, 10);

				if (truText.length() == 0)
				{
					fprintf(out, "[Warning]%s -- 原文是空行\r\n", tmp);
					warning++;
				}
				else if (ReadBuffer[11] != ']')
				{
					fprintf(out, "[Error]%s -- 原文缺少']'作为行标终结符\r\n", tmp);
					error++;
				}
			}
			else if (ReadLine[0] == ';' && ReadLine[1] == '[')
			{
				string ReadBuffer = ReadLine;
				string truText = ReadBuffer.substr(14, ReadBuffer.length());

				char tmp[11] = { 0 };
				memcpy(tmp, ReadLine + 2, 10);

				if (truText.length() == 0 || truText[0] == '\r' || truText[0] == '\n')
				{
					fprintf(out, "[Warning]%s -- 没有翻译\r\n", tmp);
					warning++;
				}
				else if (ReadBuffer[12] != ']')
				{
					fprintf(out, "[Error]%s -- 翻译行缺少']'作为行标终结符\r\n", tmp);
					error++;
				}
			}
			else
			{
				if (!memcmp(ReadLine, utf8_bom, 3))
				{
					fprintf(out, "[LOG]检测到UTF8-BOM，要求不使用Bom\r\n");
					bom = true;
					string ext(ReadLine + 3);

					char tmp[11] = { 0 };
					memcpy(tmp, ReadLine + 4, 10);

					if (ext.length() == 0)
					{
						fprintf(out, "[Warning]%s -- 原文是空行\r\n", tmp);
						warning++;
					}
					else if (ext[11] != ']')
					{
						fprintf(out, "[Error]%s -- 原文缺少']'作为行标终结符\r\n", tmp);
						error++;
					}
				}
				else if (ReadLine[0] != NULL)
				{
					fprintf(out, "[Error]无法识别的行 %d(文本中的真实行)\r\n", iPos);
					error++;
				}
			}
			memset(ReadLine, 0, 1024);
			if (bom)
			{
				bom = false;
				iPos = 0;
			}
			else iPos++;
		}
	}
	catch (...)
	{
		wchar_t Text[256] = { 0 };
		wsprintfW(Text, L"文本执行到第[%d]行发生错误", iPos);
		MessageBox(NULL, Text, L"Fatal", MB_OK);
		fclose(out);
		fin.close();
		return -2;
	}
	fin.close();

	fprintf(out, "结果:Error[%d] Warning[%d]\r\n", error, warning);
	fclose(out);
	return 0;
}

