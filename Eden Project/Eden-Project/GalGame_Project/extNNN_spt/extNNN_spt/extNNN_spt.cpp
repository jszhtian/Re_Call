// extNNN_spt.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#define RELEASE
#include <iostream>
#include <Windows.h>
#include <io.h>
#include <direct.h>
#include <string>
#include <vector>
#include <fstream>
#include "scriptcommand.h"
#include "scriptDefine.h"
using namespace std;

int E(const char* msg)
{
	MessageBoxA(0, msg, 0, 0);
	return -1;
}

int End()
{
#ifdef DEBUG
	system("pause");
#endif
	return 0;
}

string FixString(string& Line)
{
	string Name;
	for (auto it : Line)
	{
		if (it == '\r')
		{
			Name += "\\r";
		}
		else if (it == '\n')
		{
			Name += "\\n";
		}
		else
		{
			Name += it;
		}
}
	return Name;
}

int main(int argc, char* argv[])
{
#ifdef RELEASE
	if (argc != 2)
	{
		MessageBox(0, L"extNNN_spt.exe <input spt file>", L"AyamiKaze", 0);
		return -1;
	}
#endif
	char* FileName = argv[1];
	auto fp = fopen(FileName, "rb");
	fseek(fp, 0, SEEK_END);
	DWORD FileSize = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	DWORD DataSize = ((FileSize + 3) / 4);
	BYTE* FileBuff = (BYTE*)malloc(FileSize);
	if (!FileBuff)
		return E("Not buff, check if your alloc memory setting.");
	fread(FileBuff, FileSize, 1, fp);
	fclose(fp);

	//Not for all games here.
	for (int i = 0; i < FileSize; i++)
		FileBuff[i] ^= 0xFF;

	// Get valune name from offical code
	DWORD szOfHeaderTable = *(DWORD*)FileBuff;
	DWORD m_messageKosuu1 = *(DWORD*)(FileBuff + 0x10);
	DWORD m_messagePointer1 = *(DWORD*)(FileBuff + 0x14);
	DWORD m_messageKosuu2 = *(DWORD*)(FileBuff + 0x18);
	DWORD m_messagePointer2 = *(DWORD*)(FileBuff + 0x1C);
	DWORD m_storyKosuu = *(DWORD*)(FileBuff + 0x28);
	DWORD m_int32Pointer = *(DWORD*)(FileBuff + 0xF9C);

	DWORD m_subCallPointer = 0;
	DWORD m_selectPointer = 0;
	DWORD m_commandCallPointer = 0;
	DWORD m_scriptCallPointer = 0;
	///*
	//newer version need it.
	if (szOfHeaderTable >= 0x20)
	{
		m_subCallPointer = *(DWORD*)(FileBuff + 0x50);
		m_selectPointer = *(DWORD*)(FileBuff + 0x54);
		m_commandCallPointer = *(DWORD*)(FileBuff + 0x58);
		m_scriptCallPointer = *(DWORD*)(FileBuff + 0x5C);
	}
	//*/
#ifdef DEBUG
	fp = fopen("DecryptDump.dmp", "wb");
	fwrite(FileBuff, FileSize, 1, fp);
	fclose(fp);
	cout << "FileSize:0x" << hex << FileSize << endl;
	cout << "DataSize:0x" << hex << DataSize << endl;
	cout << "sig:0x" << hex << szOfHeaderTable << endl;
	cout << "m_messageKosuu1:0x" << hex << m_messageKosuu1 << endl;
	cout << "m_messagePointer1:0x" << hex << m_messagePointer1 << endl;
	cout << "m_messageKosuu2:0x" << hex << m_messageKosuu2 << endl;
	cout << "m_messagePointer2:0x" << hex << m_messagePointer2 << endl;
	cout << "m_storyKosuu:0x" << hex << m_storyKosuu << endl;
	//cout << "m_int32Pointer:0x" << hex << m_int32Pointer << endl;
	/*
	cout << "m_subCallPointer:0x" << hex << m_subCallPointer << endl;
	cout << "m_selectPointer:0x" << hex << m_selectPointer << endl;
	cout << "m_commandCallPointer:0x" << hex << m_commandCallPointer << endl;
	cout << "m_scriptCallPointer:0x" << hex << m_scriptCallPointer << endl;
	*/

#endif
	string fn(FileName);
	fn += ".txt";
	fp = fopen(fn.c_str(), "wb");
	int hit = 0;
	for (DWORD ecx = 0; ecx < DataSize; )
	{
		DWORD ebx = *(DWORD*)(FileBuff + ecx * 4);
		DWORD identify = *(DWORD*)(FileBuff + ecx * 4 + 4);
		DWORD code = *(DWORD*)(FileBuff + ecx * 4 + 8);
		DWORD count = *(DWORD*)(FileBuff + ecx * 4 + 12);
		DWORD pos = *(DWORD*)(FileBuff + ecx * 4 + 16);
		if (identify == IDENTIFY_SYSTEMCOMMAND)
		{
			if ((code == CODE_SYSTEMCOMMAND_PRINT) || (code == CODE_SYSTEMCOMMAND_LPRINT) ||
				(code == CODE_SYSTEMCOMMAND_APPEND))
			{
				//cout << "mesNum:" << dec << count << endl;
				char* str = (char*)(FileBuff + *(DWORD*)(FileBuff + (m_messagePointer1 + count) * 4) * 4);
				string ch = string(str);
				ch = FixString(ch);
				//LOG << "[mesNum:" << count << "]" << ch << endl;
				//LOG << ";[mesNum:" << count << "]" << ch << "\r\n" << endl;
				fprintf(fp, "[mes|0x%08x]%s\r\n;[mes|0x%08x]%s\r\n\r\n", count, ch.c_str(), count, ch.c_str());
				hit++;
			}
			if (code == CODE_SYSTEMCOMMAND_SELECT)
			{
				if (ebx != (count + 5))
				{
					count += *(DWORD*)(FileBuff + ecx * 4 + ebx * 4 - 4);
				}
#ifdef DEBUG
				cout << count << endl;
#endif
				for (int i = 0; i < count; i++)
				{
					fprintf(fp, "[str|0x%08x]%s\r\n;[str|0x%08x]%s\r\n\r\n", pos + i, (FileBuff + *(DWORD*)(FileBuff + (m_messagePointer2 + pos + i) * 4) * 4), pos + i, (FileBuff + *(DWORD*)(FileBuff + (m_messagePointer2 + pos + i) * 4) * 4));
					//cout << "strNum:" <<dec << pos + i << endl;
					//LOG << "[strNum:" << pos + i << "]" << (FileBuff + *(DWORD*)(FileBuff + (m_messagePointer2 + pos + i) * 4) * 4) << endl;
					//LOG << ";[strNum:" << pos + i << "]" << (FileBuff + *(DWORD*)(FileBuff + (m_messagePointer2 + pos + i) * 4) * 4) << "\r\n" << endl;
				}
			}
		}

		ecx += ebx;
	}
	//cout << hex << hit << endl;
	if (hit != m_messageKosuu1)
	{
		cout << "Find Sripts:0x" << hex << hit << endl;
		cout << "Should Scripts:0x" << hex << m_messageKosuu1 << endl;
		return E("hit != m_messageKosuu1");
	}
	fclose(fp);
	return End();
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
