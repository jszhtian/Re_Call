// extSPT.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <Windows.h>
#include "scriptcommand.h"
#include "scriptDefine.h"
using namespace std;

bool debug = true;

int AddError(const char* text)
{
	return MessageBoxA(0, text, 0, 0);
}

void Exec(char* FileName)
{
	unsigned int m_messagePointer1 = 0;
	unsigned int m_messagePointer2 = 0;
	unsigned int m_int32Pointer = 0;

	unsigned int m_messageKosuu1 = 0;
	unsigned int m_messageKosuu2 = 0;
	unsigned int m_storyKosuu = 0;

	unsigned int m_subCallPointer, m_selectPointer, m_commandCallPointer, m_scriptCallPointer = 0;

	FILE* fp = fopen(FileName, "rb");
	fseek(fp, 0, SEEK_END);
	DWORD size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	char* buff = (char*)malloc(size);
	if (buff)
	{
		fread(buff, size, 1, fp);
		fclose(fp);
		int xorkey = 0xFF;
		for (int i = 0; i < size; i++)
			buff[i] ^= xorkey;
		if (debug)
		{
			fp = fopen("dump.dmp", "wb");
			fwrite(buff, size, 1, fp);
			fclose(fp);
		}
		/*
		m_messageKosuu1 = buff[4] & 0xFFFFFFFF;
		m_messagePointer1 = buff[5] & 0xFFFFFFFF;
		m_messageKosuu2 = buff[6] & 0xFFFFFFFF;
		m_messagePointer2 = buff[7] & 0xFFFFFFFF;

		m_storyKosuu = buff[10];

		m_int32Pointer = buff[18];

		if (buff[0] >= 0x20)
		{
			m_subCallPointer = buff[20] & 0xFFFFFFFF;
			m_selectPointer = buff[21] & 0xFFFFFFFF;
			m_commandCallPointer = buff[22] & 0xFFFFFFFF;
			m_scriptCallPointer = buff[23] & 0xFFFFFFFF;
		}
		else
		{
			m_subCallPointer = 0;
			m_selectPointer = 0;
			m_commandCallPointer = 0;
			m_scriptCallPointer = 0;
		}
		if (debug) 
		{
			cout << "m_messageKosuu1:0x" << hex << m_messageKosuu1 << endl;
			cout << "m_messagePointer1:0x" << hex << m_messagePointer1 << endl;
			cout << "m_messageKosuu2:0x" << hex << m_messageKosuu2 << endl;
			cout << "m_messagePointer2:0x" << hex << m_messagePointer2 << endl;
			cout << "m_storyKosuu:0x" << hex << m_storyKosuu << endl;
			cout << "m_int32Pointer:0x" << hex << m_int32Pointer << endl;
			cout << "m_subCallPointer:0x" << hex << m_subCallPointer << endl;
			cout << "m_selectPointer:0x" << hex << m_selectPointer << endl;
			cout << "m_commandCallPointer:0x" << hex << m_commandCallPointer << endl;
			cout << "m_scriptCallPointer:0x" << hex << m_scriptCallPointer << endl;

		}
		*/
		char* m_data = buff;
		int m_pc = 0;
		int m_stateCount = 0;
		while (m_pc < size)
		{
			m_stateCount++;
			if (m_stateCount >= 10000)
			{
				AddError("m_stateCount loop over 10000");
				return;
			}
			int skp = m_data[m_pc];

			if (skp == 0)
			{
				AddError("No script here.");
				return;
			}

			int identify = m_data[m_pc + 1];
			int code = m_data[m_pc + 2];
			if (identify == IDENTIFY_DATA)
			{
				m_pc += m_data[m_pc];
				continue;
			}
		}
	}
	else
	{
		MessageBox(0, L"malloc buff errop", 0, 0);
		return;
	}
}



int main(int argc, char* argv[])
{
	char* FILENAME = NULL;
	if (argc != 2 && !debug)
	{
		MessageBox(0, L"exSPT.exe <input SPT file>", L"AyamiKaze", 0);
		return -1;
	}
	if(!debug)
		FILENAME = argv[1];
	else
		FILENAME = (char*)"SDB_S01.spt";
	Exec(FILENAME);
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
