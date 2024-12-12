// CalaHash.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <Windows.h>
#include <io.h>
#include <direct.h>
#include <string>
#include <vector>
#include "md5.h"
#include "sha1.h"
using namespace std;

VOID WINAPI GenMD5Code(const char* FileName, string& OutHex)
{
	BYTE OutBuffer[16] = { 0 };

	MD5Context Context;
	MD5Init(&Context);
	MD5Update(&Context, (BYTE*)FileName, lstrlenA(FileName) * 2);
	MD5Final(OutBuffer, &Context);

	char OutTemp[3] = { 0 };

	for (ULONG32 i = 0; i < 16; i++)
	{
		sprintf(OutTemp, "%d", (OutBuffer[i] & 0xF0) >> 4);
		OutHex += OutTemp;
		RtlZeroMemory(OutTemp, 0, sizeof(char) * 3);
		sprintf(OutTemp, "%d", OutBuffer[i] & 0x0F);
		OutHex += OutTemp;
		RtlZeroMemory(OutTemp, 0, sizeof(char) * 3);
	}
}

int E(const char* msg)
{
	MessageBoxA(0, msg, 0, 0);
	return -1;
}

#include <algorithm> 
void FileNameToLower(std::string& FileName)
{
	std::transform(FileName.begin(), FileName.end(), FileName.begin(), ::tolower);
}

vector<string>FilePool;
void fileSearch(string path)
{
	long hFile = 0;
	struct _finddata_t fileInfo;
	string pathName;
	if ((hFile = _findfirst(pathName.assign(path).append("\\*").c_str(), &fileInfo)) == -1)
		return;

	do {
		string fn = path + "\\" + fileInfo.name;
		//cout << fn << endl;
		//string fnm = fn.substr(fn.find_first_of("\\") + 1);
		//cout << fnm << endl;
		//cout << fileInfo.name << endl;
		if (strcmp(fileInfo.name, "..") && strcmp(fileInfo.name, ".") && fileInfo.attrib == _A_SUBDIR)
			fileSearch(fn);
		else if (fileInfo.name[0] != '.')
			FilePool.push_back(fn);
	} while (_findnext(hFile, &fileInfo) == 0);
	_findclose(hFile);
	return;
}
int main(int argc, char* argv[])
{
	fileSearch("pak");
	if (FilePool.size() == 0)
		return E("No FilePool");
	for (string f : FilePool)
	{
		char* fnm = (char*)f.c_str();
		auto fin = fopen(fnm, "rb");
		if (!fin)
			return E("No fin");
		fseek(fin, 0, SEEK_END);
		DWORD FileSize = ftell(fin);
		fseek(fin, 0, SEEK_SET);
		BYTE* FileBuff = (BYTE*)malloc(FileSize);
		if (!FileBuff)
			return E("No FileBuff");
		fread(FileBuff, FileSize, 1, fin);
		fclose(fin);

		string fn(fnm);
		fn = fn.substr(fn.find_last_of('\\') + 1);
		FileNameToLower(fn);
		cout << "Hashing:" << fn << "|";
		char* Cfn = (char*)fn.c_str();
		for (int i = 0; i < strlen(Cfn); i++)
		{
			Cfn[i] += 0x1;
		}
		string hash_out;
		string hash_out2;
		GenMD5Code(Cfn, hash_out);
		GenMD5Code(fn.c_str(), hash_out2);

		fn = "out\\" + hash_out + hash_out2 + ".scn";
		cout  << hash_out + hash_out2 << endl;

		if (fopen(fn.c_str(), "rb"))
			return E("Hash Hit");

		fin = fopen(fn.c_str(), "wb");
		fwrite(FileBuff, FileSize, 1, fin);
		fclose(fin);
	}
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
