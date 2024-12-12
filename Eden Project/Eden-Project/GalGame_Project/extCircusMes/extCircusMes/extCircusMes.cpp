// extCircusMes.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
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



char* DecryptText(char* text)
{
	for (int i = 0; i < strlen(text); i++)
		text[i] += 0x20;
	return text;
}

void WriteString(FILE* fp, char* str)
{
	fwrite(str, lstrlenA(str), 1, fp);
	fwrite("\r\n", 2, 1, fp);
}

#define DEBUG
const unsigned int opcode_name = 0x60;
const unsigned int opcode_text = 0x61;
const unsigned int opcode_select = 0x62;
const unsigned int opcode_voice1 = 0x47;
const unsigned int opcode_voice2 = 0x46;
int main(int argc, char* argv[])
{
#ifdef RELEASE
	if (argc != 2)
	{
		MessageBox(0, L"extCircusMes.exe <input mes file>", L"AyamiKaze", 0);
		return -1;
	}
	char* FileName = argv[1];
#else
	char FileName[] = "ds_1331d_hm.mes";
#endif
	auto fp = fopen(FileName, "rb");
	fseek(fp, 0, SEEK_END);
	DWORD size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	BYTE* buff = (BYTE*)malloc(size);
	if (!buff)
		return E("No buff");
	fread(buff, size, 1, fp);
	fclose(fp);

	cout << "FileInfo:" << endl;
	cout << "FileSize:0x" << hex << size << endl;

	DWORD command_count = *(DWORD*)buff;
	DWORD index_count = command_count * 4;
	BYTE* index_buff = (BYTE*)malloc(index_count);
	if(!index_buff)
		return E("No index_buff");
	memcpy(index_buff, buff + 4, index_count);

	//DWORD unksize = 2 * command_count;
	//BYTE* unkbuff = (BYTE*)malloc(unksize);
	//if (!unkbuff)
	//	return E("No unkbuff");
	//memcpy(unkbuff, buff + index_count, unksize);

	DWORD ResSize = size - 6 * command_count - 4;
	BYTE* ResBuff = (BYTE*)malloc(ResSize);
	if (!ResBuff)
		return E("No ResBuff");
	memcpy(ResBuff, buff + (6 * command_count) + 0x4, ResSize);

	char out[MAX_PATH];
	strcpy(out, FileName);
	strcat(out, ".txt");

	fp = fopen(out, "wb");

	DWORD pos = 0;
	pos += 8;
	char* text = nullptr;
	do
	{
	pass:
		DWORD ipos = *(DWORD*)(index_buff + pos)&0x7FFFFFFF;
		unsigned int op = ResBuff[ipos];
		if (op == opcode_voice1)
		{
			text = (char*)(ResBuff + ipos + 1);
			ipos += strlen(text) + 2;
			op = ResBuff[ipos];
			if (op == opcode_voice2)
			{
				text = (char*)(ResBuff + ipos + 1);
				ipos += strlen(text) + 2;
				op = ResBuff[ipos];
			}
			else if (op == opcode_text || op == opcode_name)
			{
				text = DecryptText((char*)(ResBuff + ipos + 1));
				WriteString(fp, text);
				ipos += strlen(text) + 2;
				op = ResBuff[ipos];
				if (op == opcode_voice2)
				{
					text = (char*)(ResBuff + ipos + 1);
					ipos += strlen(text) + 2;
					op = ResBuff[ipos];

				}
				if (op == opcode_text)
				{
					text = DecryptText((char*)(ResBuff + ipos + 1));
					WriteString(fp, text);
				}
				else
				{
					cout << hex << op << endl;
					return E("no swarch text in name op2");
				}
			}
			else
			{
				{
					cout << hex << op << endl;
					return E("no swarch code in name op");
				}
			}
		}
		else if (op == opcode_name)
		{
			text = DecryptText((char*)(ResBuff + ipos + 1));
			WriteString(fp, text);
			ipos += strlen(text) + 2;
			unsigned int op2 = ResBuff[ipos];
			if (op2 == opcode_voice2)
			{
				text = (char*)(ResBuff + ipos + 1);
				ipos += strlen(text) + 2;
				op2 = ResBuff[ipos];

			}
			if (op2 == opcode_text)
			{
				text = DecryptText((char*)(ResBuff + ipos + 1));
				WriteString(fp, text);
			}
			else
			{
				cout << hex << op2 << endl;
				return E("no swarch text in name op2");
			}

		}
		else if (op == opcode_text)
		{
			text = DecryptText((char*)(ResBuff + ipos + 1));
			WriteString(fp, text);
		}
#ifdef DEBUG
		cout << "Addr:0x" << hex << ipos << endl;
#endif
		pos += 4;
	} while (pos < index_count);
	fclose(fp);
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
