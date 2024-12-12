// extKGO.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <Windows.h>
#include <fstream> 
#include <vector>
#include <sstream>

#define unpack

using namespace std;
#pragma pack (1)
struct KGOHeader {
	char Magic[4]; // "HA02"
	DWORD FileSize;
	DWORD ScriptNo;
	DWORD Unk1;
	DWORD Unk2;
	DWORD FuncSize;
	DWORD FuncCount1;
	DWORD CodeOffset;
	DWORD CodeSize;
	DWORD FuncCount2;
	DWORD TextOffset;
	DWORD TextSize;
};

#pragma pack ()

#ifdef unpack
int main(int argc, char* argv[])
{
	KGOHeader KGO;
	if (argc != 2)
	{
		MessageBox(0, L"extKGO.exe <input kgo file>", L"AyamiKaze", 0);
		return -1;
	}
	char* FileName = argv[1];
	//char* FileName = (char*)"scr0001.kgo";
	FILE* fp = fopen(FileName, "rb");
	fread(&KGO, sizeof(KGOHeader), 1, fp);
	fseek(fp, 0, SEEK_SET);
	DWORD size = KGO.FileSize;
	char* buff = new char[size];
	fread(buff, size, 1, fp);
	fclose(fp);


	char fnm[MAX_PATH] = { 0 };
	sprintf(fnm, "%s.txt", FileName);
	fp = fopen(fnm, "wb");
	for (int i = 0; i < KGO.TextSize;)
	{
		WORD len = *(WORD*)(buff + KGO.TextOffset + i);
		char* text = buff + KGO.TextOffset + i + 2;
		fprintf(fp, "%s\r\n", text);
		i += len;
	}
	fclose(fp);
	delete[] buff;
	//system("pause");
	return 0;
}
#else
int main(int argc, char* argv[])
{
	KGOHeader KGO;
	if (argc != 2)
	{
		MessageBox(0, L"extKGO.exe <input kgo file>", L"AyamiKaze", 0);
		return -1;
	}
	char* FileName = argv[1];
	//char* FileName = (char*)"scr0001.kgo";
	char TextName[MAX_PATH] = { 0 };
	sprintf(TextName, "%s.txt", FileName);

	FILE* fp = fopen(FileName, "rb");
	fseek(fp, 0, SEEK_END);
	DWORD size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	char* buff = new char[size];
	fread(buff, size, 1, fp);
	fclose(fp);

	KGOHeader* header = (KGOHeader*)buff;

	char* CodeBuff = new char[header->TextOffset];
	memcpy(CodeBuff, buff, header->TextOffset);

	vector<string>TextPool;
	ifstream txt(TextName);
	while (!txt.eof())
	{
		string LineTxt;
		getline(txt, LineTxt);
		TextPool.push_back(LineTxt);
	}

	char* TextBuff = new char[6553600];
	memset(TextBuff, 0, 6553600);
	int pos = 0;
	for (string str : TextPool)
	{
		int len = lstrlenA(str.c_str());
		WORD WriteLen = len + 4;
		memcpy(TextBuff + pos, &WriteLen, 2);
		memcpy(TextBuff + pos + 2, str.c_str(), len);
		memcpy(TextBuff + pos + 2 + len, "\x00\x00", 2);
		pos += WriteLen;
	}

	header->TextSize = pos;
	header->FileSize = header->TextOffset + pos;
	memcpy(CodeBuff, header, sizeof(KGOHeader));

	char fnm[MAX_PATH] = { 0 };
	sprintf(fnm, "%s.new", FileName);
	fp = fopen(fnm, "wb");
	fwrite(CodeBuff, header->TextOffset, 1, fp);
	fwrite(TextBuff, pos, 1, fp);
	fclose(fp);
	delete[] buff;
	delete[] CodeBuff;
	delete[] TextBuff;
	//system("pause");
	return 0;
}
#endif
// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
