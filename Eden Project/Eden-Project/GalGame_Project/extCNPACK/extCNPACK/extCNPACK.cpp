// extCNPACK.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
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

#pragma pack (1)
typedef struct CPKHEADER {
	unsigned int FileCount;
}CPKHEADER_T;

typedef struct CPKENTRY {
	char FileName[51];
	unsigned int Offset;
	unsigned int FileSize;
}CPKENTRY_T;
#pragma pack ()

#define unpack



#ifdef unpack
int main()
{
	CPKHEADER_T header;
	FILE* fp = fopen("cnpack", "rb");
	fseek(fp, 0, SEEK_SET);
	fread(&header, sizeof(header), 1, fp);
	cout << "FileCount:" << header.FileCount << endl;
	unsigned int entry_size = sizeof(CPKENTRY_T) * header.FileCount;
	CPKENTRY_T* entry = (CPKENTRY_T*)malloc(entry_size);
	if (!entry)
		return E("No entry");
	fread(entry, entry_size, 1, fp);

	for (unsigned int i = 0; i < header.FileCount; i++)
	{
		cout << "Unpacking:" << entry[i].FileName << endl;
		cout << "FileSize:0x" << hex << entry[i].FileSize << endl;
		cout << "FileOffset:0x" << hex << entry[i].Offset << endl;
		fseek(fp, entry[i].Offset, SEEK_SET);
		char* WriteBuff = (char*)malloc(entry[i].FileSize);
		if (!WriteBuff)
			return E("No WriteBuff");
		fread(WriteBuff, entry[i].FileSize, 1, fp);

		auto fout = fopen(entry[i].FileName, "wb");
		if (!fout)
			return E("No fout");
		fwrite(WriteBuff, entry[i].FileSize, 1, fout);
		fclose(fout);

		free(WriteBuff);
	}
	fclose(fp);
	system("pause");
	return 0;
}

#endif
#ifdef pack
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

int main()
{
	CPKHEADER_T header;
	CPKENTRY_T entry;
	fileSearch("packdata");

	cout << "File Count:" << FilePool.size() << endl;
	header.FileCount = FilePool.size();

	auto fp = fopen("cnpack", "wb");
	fseek(fp, 0, SEEK_SET);
	fwrite(&header, sizeof(header), 1, fp);

	DWORD pos = 0;
	DWORD ordsize = 0;
	for (string f : FilePool)
	{
		auto fin = fopen(f.c_str(), "rb");
		fseek(fin, 0, SEEK_END);
		DWORD size = ftell(fin);
		fseek(fin, 0, SEEK_SET);
		char* buff = (char*)malloc(size);
		if (!buff)
			return E("Alloc buff error.");
		fread(buff, size, 1, fin);
		fclose(fin);

		for (int i = 0; i < size; i++)
			buff[i] ^= 0xE4;

		cout << "Packing:" << f.substr(f.find_first_of("\\") + 1).c_str() << endl;

		memset(entry.FileName, 0, 51);
		strcpy(entry.FileName, f.substr(f.find_first_of("\\") + 1).c_str());
		entry.FileSize = size;
		entry.Offset = ordsize;
		fwrite(&entry, sizeof(entry), 1, fp);

		fseek(fp, (header.FileCount * sizeof(entry) + ordsize), SEEK_SET);
		ordsize += size;
		fwrite(buff, size, 1, fp);
		free(buff);
		pos += 1;
		fseek(fp, sizeof(header) + pos * sizeof(entry), SEEK_SET);
	}
	cout << "Over" << endl;
	fclose(fp);
	system("pause");
	return 0;
}
#endif

#ifdef one_file

FILE* fp;
CPKHEADER_T header;
CPKENTRY_T* entry = nullptr;

BOOL InitPackInfo()
{
	fp = fopen("cnpack", "rb");
	if (!fp)
	{
		MessageBox(0, L"Can't find cnpack.", 0, 0);
		return FALSE;
	}
	fread(&header, sizeof(header), 1, fp);
	entry = (CPKENTRY_T*)malloc(header.FileCount * sizeof(CPKENTRY_T));
	if (!entry)
	{
		MessageBox(0, L"Can't alloc entry.", 0, 0);
		return FALSE;
	}
	fread(entry, header.FileCount * sizeof(CPKENTRY_T), 1, fp);
	return TRUE;
}

BOOL GetFileBuffByName(char* fnm, BYTE** buff, DWORD* size)
{
	for (int i = 0; i < header.FileCount; i++)
	{
		if (!strncmp(entry[i].FileName, fnm, strlen(fnm)))
		{
			fseek(fp, header.FileCount * sizeof(CPKENTRY_T) + entry[i].Offset, SEEK_SET);
			BYTE* outbuf = (BYTE*)malloc(entry[i].FileSize);
			if (!outbuf)
			{
				char msg[MAX_PATH];
				sprintf(msg, "Can't read file:%s", fnm);
				MessageBoxA(0, msg, 0, 0);
				return FALSE;
			}
			fread(outbuf, entry[i].FileSize, 1, fp);

			for (int a = 0; a < entry[i].FileSize; a++)
				outbuf[a] ^= 0xE4;

			*buff = outbuf;
			*size = entry[i].FileSize;
			return TRUE;
		}
	}
	return FALSE;
}

int main()
{
	if (!InitPackInfo())
		return -1;
	BYTE* buff;
	DWORD size;
	if (!GetFileBuffByName((char*)"sinarioPM2k01.txt", &buff, &size))
	{
		MessageBox(0, L"Can't find file", 0, 0);
		return -1;
	}
	FILE* fout = fopen("sinarioPM2k01.txt", "wb");
	fwrite(buff, size, 1, fout);
	fclose(fout);
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
