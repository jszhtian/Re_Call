// packLIB.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <Windows.h>
#include <io.h>
#include <direct.h>
#include <string>
#include <vector>
#include <algorithm> 

using namespace std;

static vector<string>FilePool;
static void fileSearch(string path)
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

#pragma pack (1)
struct LIBHeader {
	CHAR magic[4];	// "LIB" or "LIBU"
	DWORD version;	// 0x00010000
	DWORD index_entries;
	DWORD reserved;	// 0
};

struct LIBEntry {
	CHAR name[36];
	DWORD length;
	DWORD offset;
	DWORD reserved;		// 0
};
#pragma pack ()
int main()
{
	LIBHeader header;
	LIBEntry  entry;

	fileSearch("data");
	cout << "Count:" << dec << FilePool.size() << endl;

	FILE* lib = fopen("movie.dat", "wb");
	memcpy(header.magic, "LIBU", 4);
	header.version = 0x00010000;
	header.index_entries = FilePool.size();
	header.reserved = 0;
	fwrite(&header, sizeof(LIBHeader), 1, lib);

	DWORD pos = 0;
	DWORD ordsize = 0;

	for (string f : FilePool)
	{
		FILE* fp = fopen(f.c_str(), "rb");
		fseek(fp, 0, SEEK_END);
		DWORD size = ftell(fp);
		fseek(fp, 0, SEEK_SET);
		BYTE* buff = new BYTE[size];
		fread(buff, size, 1, fp);
		fclose(fp);
		
		cout << "packing:" << f << endl;
		memset(entry.name, 0, 36);
		strcpy(entry.name, f.c_str());
		entry.length = size;
		entry.offset = ordsize;
		entry.reserved = 0;
		fwrite(&entry, sizeof(LIBEntry), 1, lib);

		fseek(lib, (header.index_entries * sizeof(LIBEntry) + ordsize), SEEK_SET);
		ordsize += size;
		fwrite(buff, size, 1, lib);
		delete[] buff;
		pos += 1;
		fseek(lib, sizeof(LIBHeader) + pos * sizeof(LIBEntry), SEEK_SET);
	}

	cout << "over" << endl;
	fclose(lib);
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
