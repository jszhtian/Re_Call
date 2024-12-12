// Xuse_Project.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <fstream>
#include <cstdio>
#include <memory>
#include  <Windows.h>
using namespace std;

LPWSTR ctowJIS(char* str)
{
	DWORD dwMinSize;
	dwMinSize = MultiByteToWideChar(932, 0, str, -1, NULL, 0); //计算长度
	LPWSTR out = new wchar_t[dwMinSize];
	MultiByteToWideChar(932, 0, str, -1, out, dwMinSize);//转换
	return out;
}

char* wtocGBK(LPCTSTR str)
{
	DWORD dwMinSize;
	dwMinSize = WideCharToMultiByte(936, NULL, str, -1, NULL, 0, NULL, FALSE); //计算长度
	char* out = new char[dwMinSize];
	WideCharToMultiByte(936, NULL, str, -1, out, dwMinSize, NULL, FALSE);//转换
	return out;
}

char* wtocUTF(LPCTSTR str)
{
	DWORD dwMinSize;
	dwMinSize = WideCharToMultiByte(CP_UTF8, NULL, str, -1, NULL, 0, NULL, FALSE); //计算长度
	char* out = new char[dwMinSize];
	WideCharToMultiByte(CP_UTF8, NULL, str, -1, out, dwMinSize, NULL, FALSE);//转换
	return out;
}

//0.cd解密
void Cryption(char* FileName)
{
	FILE* fp = fopen(FileName, "rb");
	fseek(fp, 0, SEEK_END);
	unsigned long size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	unsigned char* buff = new unsigned char[size];
	fread(buff, size, 1, fp);
	fclose(fp);
	char key = 0x1b;
	for (int i = 0; i < size;)
	{
		buff[i] ^= key;
		key = (i++) + ((unsigned __int8)(key ^ 0xA1) >> 1);
	}
	strcat(FileName, ".dec");
	fp = fopen(FileName, "wb");
	fwrite(buff, size, 1, fp);
	cout <<hex<< size << endl;
	fclose(fp);
	delete[] buff;
}

typedef struct CD_FILE_HEADER {
	DWORD Unk1;
	DWORD Unk2;
	char ScriptName[260];
}CD_HEADER;

typedef struct CD_FILE_INDEX {//自制
	DWORD index;
	DWORD VMCODE_SIZE;
	DWORD OPCODE_SIZE;
	DWORD YUV_SIZE;
	char* VMCODE;
	char* YUV;
	char* NextOffset;
}CD_INDEX;

typedef struct CD_FILE_TEXT {//自制
	DWORD Size;
	char* Buff;
}CD_TEXT;

void Exec(char* FileName)
{
	FILE* fp = fopen(FileName, "rb");
	if (!fp)
	{
		MessageBox(0, L"找不到文件。", 0, 0);
		return;
	}
	fseek(fp, 0, SEEK_END);
	DWORD size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	char* buff = (char*)malloc(size);
	fread(buff, size, 1, fp);
	fclose(fp);
	if (buff)
	{
		CD_HEADER Header;
		//buff += 0x10;
		Header.Unk1 = *(DWORD*)buff;
		buff += 0x10;
		Header.Unk2 = *(DWORD*)buff;
		buff += 0x10;
		for (int i = 0; i < 260; i++)
			buff[i] ^= 0x16;
		memcpy(Header.ScriptName, buff, 260);
		char* SN = wtocGBK(ctowJIS(Header.ScriptName));
		cout << "Name:" << SN << endl;
		buff += 260;
		CD_INDEX CI;
		CI.index = *(DWORD*)buff;
		CI.NextOffset = buff;
		CI.NextOffset += 4;
		CI.VMCODE_SIZE = *(DWORD*)CI.NextOffset;
		CI.NextOffset += 4;
		CI.YUV_SIZE = *(DWORD*)CI.NextOffset;
		CI.NextOffset += 4;
		CI.OPCODE_SIZE = CI.index + 2 * CI.index;//OPCODE_SIZE
		CI.VMCODE = (char*)(CI.NextOffset + 12 * CI.index);
		CI.YUV = (char*)(CI.VMCODE + CI.VMCODE_SIZE);
		cout << "OPCODE_SIZE:0x" << hex << CI.OPCODE_SIZE << endl;
		cout << "VMCODE_SIZE:0x" << hex << CI.VMCODE_SIZE << endl;
		cout << "YUV_SIZE:0x" << hex << CI.YUV_SIZE << endl;
		cout << "NextOffset:0x" << hex << CI.NextOffset << endl;
		cout << "VMCODE:0x" << hex << (DWORD)CI.VMCODE << endl;
		cout << "YUV:0x" << hex << (DWORD)CI.YUV << endl;
		/*
		fp = fopen("VMCODE.bin", "wb");
		fwrite(CI.VMCODE, CI.VMCODE_SIZE, 1, fp);
		fclose(fp);
		fp = fopen("YUV.bin", "wb");
		fwrite(CI.YUV, CI.YUV_SIZE, 1, fp);
		fclose(fp);
		fp = fopen("NextOffset.bin", "wb");
		fwrite(CI.NextOffset, CI.OPCODE_SIZE, 1, fp);
		fclose(fp);
		*/
		string Old(SN);
		string NewFileName = Old + ".txt";
		fp = fopen(NewFileName.c_str(), "wb");
		for (int i = 0; i < CI.VMCODE_SIZE; i+=12)
		{
			if ((unsigned char)CI.VMCODE[i] == 0x40 || (unsigned char)CI.VMCODE[i] == 0x41)
			{
				//offset += 4
				//然后就是偏移量，跳过第一个DWORD，第二个DWORD是偏移量，直接跳到当前文本块的末端。offset += 4后跳到文本块末端，把offset记下。
				//每一句文本前都有当前文本长度，大小为一个DWORD，这个长度后期解密需要。
				DWORD idx = *(DWORD*)(CI.VMCODE+i+4);
				//cout << idx << endl;
				DWORD offset = (DWORD)CI.YUV + idx;
				//cout << hex << offset << endl;
				offset += 4;
				DWORD EndBlockIndex = *(DWORD*)offset;
				//cout << hex << EndBlockIndex << endl;
				offset += 4;
				DWORD EndBlockPos = offset + EndBlockIndex;
				//cout << hex << EndBlockPos << endl;
				offset += 8;
				CD_TEXT CT;
				CT.Size = *(DWORD*)offset;
				offset += 4;
				CT.Buff = (char*)offset;
				Decrypt:
				for (int i = 0; i < CT.Size; i++)
				{
					CT.Buff[i] ^= 0x53;
				}
				fwrite(CT.Buff, CT.Size, 1, fp);
				fwrite("\n", 1, 1, fp);
				offset += CT.Size;
				if (offset < EndBlockPos)
				{
					CT.Size = *(DWORD*)offset;
					offset += 4;
					CT.Buff = (char*)offset;
					goto Decrypt;
				}
				//cout << CT.Buff << endl;
			}
		}
		fclose(fp);
	}
}

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		//cout << "Usage:" << argv[0] << " File" << endl;
		MessageBox(0, L"exCD.exe <input cd file>", L"AyamiKaze", 0);
		return -1;
	}
	char* FILENAME = argv[1];
	//Cryption(FILENAME);
	Exec(FILENAME);
	//system("pause");
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
