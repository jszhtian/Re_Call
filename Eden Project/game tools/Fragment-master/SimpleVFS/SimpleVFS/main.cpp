// SimpleVFS.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include "SimpleVFS.h"

struct KeyFile
{
	char EncKey[CryptoPP::Salsa20::MAX_KEYLENGTH];
	char salt[CryptoPP::Salsa20::IV_LENGTH];
};
using namespace std;
int main()
{
	std::cout << "Hello World!\n";
	SimpleVFS VFS;
	fstream fKey;
	KeyFile KeyInfo;
	fKey.open("pack.key", ios::binary | std::fstream::in);
	fKey.read((char*)&KeyInfo, sizeof(KeyFile));
	VFS.SetKey(KeyInfo.EncKey, KeyInfo.salt);
	VFS.Mount("pack.idx", "pack.pck");
	DWORD FileLen = 0;
	char* buf=VFS.GetFileBufferByName("TEST.txt", &FileLen);
	char* txt1 = new char[FileLen + 1];
	memset(txt1, 0, FileLen + 1);
	memcpy(txt1, buf, FileLen);
	delete[] buf;
	cout << "Len" << FileLen << endl << "Content" << txt1 << endl;
	HANDLE TMPHd = VFS.GetFileHandleByName("AACF.txt");
	char* buf2 = new char[0x10000];
	DWORD ToRead = 0;
	ReadFile(TMPHd, buf2, 0x10000, &ToRead, NULL);
	CloseHandle(TMPHd);
	char* txt2 = new char[ToRead + 1];
	memset(txt2, 0, ToRead + 1);
	memcpy(txt2, buf2, ToRead);
	cout << "Len" << ToRead << endl << "Content" << txt2 << endl;
	

}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件