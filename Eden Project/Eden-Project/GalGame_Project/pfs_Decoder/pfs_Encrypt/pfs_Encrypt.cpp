// pfs_Encrypt.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <Windows.h>
#include <io.h>
#include <direct.h>
#include <string>
#include <vector>


#include "..\3rd\cryptopp-master\salsa.h"
#include "..\3rd\cryptopp-master\chacha.h"
#include "..\3rd\cryptopp-master\osrng.h"
#include "..\3rd\cryptopp-master\cryptlib.h"
#include "..\3rd\cryptopp-master\hex.h"
#include "..\3rd\cryptopp-master\files.h"
#include "..\3rd\cryptopp-master\secblock.h"

#pragma comment(lib, "cryptlib.lib")
using namespace std;
using namespace CryptoPP;

const char* Key = "Creative";
const char* Salt = "TudoupeigengHanhuazu copyright 2020";

void EncHelper(const char* Buf, char** OutBuf, const char* Key, const char* Salt, int OriLen)
{
	try
	{
		CryptoPP::Salsa20::Encryption enc;
		enc.SetKeyWithIV((byte*)Key, CryptoPP::Salsa20::MAX_KEYLENGTH, (byte*)Salt, CryptoPP::Salsa20::IV_LENGTH);
		enc.ProcessData((byte*)*OutBuf, (byte*)Buf, OriLen);
	}
	catch (const CryptoPP::Exception& e)
	{
		std::cerr << e.what() << std::endl;
		exit(1);
	}

}

void DecHelper(const char* Buf, char** OutBuf, const char* Key, const char* Salt, int OriLen)
{
	try
	{
		CryptoPP::Salsa20::Decryption dec;
		dec.SetKeyWithIV((byte*)Key, CryptoPP::Salsa20::MAX_KEYLENGTH, (byte*)Salt, CryptoPP::Salsa20::IV_LENGTH);
		dec.ProcessData((byte*)*OutBuf, (byte*)Buf, OriLen);
	}
	catch (const CryptoPP::Exception& e)
	{
		std::cerr << e.what() << std::endl;
		exit(1);
	}

}

int E(const char* msg)
{
	MessageBoxA(0, msg, 0, 0);
	return -1;
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
		if (strcmp(fileInfo.name, "..") && strcmp(fileInfo.name, ".") && fileInfo.attrib == _A_SUBDIR)
			fileSearch(fn);
		else if (fileInfo.name[0] != '.')
			FilePool.push_back(fn);
	} while (_findnext(hFile, &fileInfo) == 0);
	_findclose(hFile);
	return;
}
#define release
#ifdef release
int main()
{
	fileSearch("ProjectData");
	cout << "Find file:" << FilePool.size() << endl;

	for (string f : FilePool)
	{
		cout << "encrypt:" << f << endl;
		FILE* fp = fopen(f.c_str(), "rb");
		fseek(fp, 0, SEEK_END);
		DWORD size = ftell(fp);
		fseek(fp, 0, SEEK_SET);
		char* buff = (char*)malloc(size);
		if (!buff)
			return E("No buff");
		fread(buff, 0x1, size, fp);
		fclose(fp);

		char* OutBuff = (char*)malloc(size);
		EncHelper((char*)buff, &OutBuff, Key, Salt, size);
		fp = fopen(("out\\" + f.substr(f.find_first_of("\\") + 1)).c_str(), "wb");
		fwrite(OutBuff, 0x1, size, fp);
		fclose(fp);

	}
	system("pause");
	return 0;
}

#else
int main()
{
	string f = "共通-01.ast";
	FILE* fp = fopen(f.c_str(), "rb");
	fseek(fp, 0, SEEK_END);
	DWORD size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	char* buff = (char*)malloc(size);
	if (!buff)
		return E("No buff");
	fread(buff, 0x1, size, fp);
	fclose(fp);

	int key = 0x0;

	for (int i = 0; i < size; i++)
	{
		buff[i] -= key | 0xFFFFFFFF;
		key++;
	}

	DecHelper((char*)buff, &buff, Key, Salt, size);

	fp = fopen("test.txt", "wb");
	fwrite(buff, size, 1, fp);
	fclose(fp);
	system("pause");
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
