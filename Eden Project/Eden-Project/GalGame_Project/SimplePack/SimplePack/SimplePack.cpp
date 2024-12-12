// SimplePack.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <Windows.h>
#include <io.h>
#include <direct.h>
#include <string>
#include <vector>
#define PACKNAME "Pack.pak"
using namespace std;

int E(const char* msg)
{
	MessageBoxA(0, msg, 0, 0);
	return 0;
}

VOID Encrypt(char* buff, DWORD size)
{
	for (int i = 0; i < size; i++)
	{
		buff[i] += 0x20;
		buff[i] ^= i;
		buff[i] -= 0x20;
	}
}

typedef struct {
	char magic[5];
	unsigned int FileCount;
}pak_header_t;

typedef struct {
	char name[100];
	unsigned int offset;
}pak_entry_t;

pak_header_t header;
pak_entry_t  entry;

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

int PackData(FILE* fp, DWORD entry_size)
{
	DWORD ordsize = 0;
	DWORD pos = 0;
	for (string f : FilePool)
	{
		//char* fn = (char*)f.substr(f.find_first_of("\\") + 1).c_str();
		
		FILE* fin = fopen(f.c_str(), "rb");
		fseek(fin, 0, SEEK_END);
		DWORD size = ftell(fin);
		fseek(fin, 0, SEEK_SET);
		char* buff = (char*)malloc(size);
		if (!buff)
			return E("No Buff");
		fread(buff, size, 1, fin);
		fclose(fin);

		cout << "Packing:" << f.substr(f.find_first_of("\\") + 1).c_str() << endl;
		cout << "FileSize:0x" << hex << size << endl;

		strcpy(entry.name, f.substr(f.find_first_of("\\") + 1).c_str());
		entry.offset = ordsize;

		fwrite(&entry, sizeof(pak_entry_t), 1, fp);

		fseek(fp, entry_size * sizeof(pak_entry_t) + ordsize, SEEK_SET);

		ordsize += size;

		Encrypt(buff,size);
		fwrite(buff, size, 1, fp);
		free(buff);
		pos += 1;

		fseek(fp, sizeof(pak_header_t) + pos * sizeof(pak_entry_t), SEEK_SET);
	}
}


int PackMain()
{
	fileSearch("PackData");
	FILE* fpack = fopen(PACKNAME, "wb");
	strcpy(header.magic, "XP3\n");
	header.FileCount = FilePool.size();
	fwrite(&header, sizeof(pak_header_t), 1, fpack);
	PackData(fpack, header.FileCount);
	fclose(fpack);
	system("pause");
	return 0;
}

FILE* fp;
BOOL InitPack()
{
	fp = fopen(PACKNAME, "rb");
	if (!fp)
		return E("No Package.");
	fread(&header, sizeof(pak_header_t), 1, fp);
	if (strcmp(header.magic, "XP3\n"))
		return E("No Magic");
	DWORD entry_size = header.FileCount * sizeof(pak_entry_t);
	pak_entry_t* entry_buff = (pak_entry_t*)malloc(entry_size);
	if (!entry_buff)
		return E("No entry_buff");
	fread(entry_buff, entry_size, 1, fp);
}



int UnpackMain()
{
	const char* FileName = "SimplePack.iobj";

	
}

#define PACK

int main()
{
#ifdef PACK
	return PackMain();
#else
	return UnapckMain();
#endif
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
