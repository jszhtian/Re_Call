// BGIPack.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <Windows.h>
#include <io.h>
#include <direct.h>
#include <string>
#include <vector>
#include <algorithm> 
#include "xxhash.h"
#include <unordered_map>
using namespace std;
struct entry {
    CHAR FileName[64];
	DWORD Size;
    DWORD Offset;
};
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

void FileNameToLower(std::string& FileName)
{
	std::transform(FileName.begin(), FileName.end(), FileName.begin(), ::tolower);
}

#define m_pack
#ifdef m_pack;
int main()
{
	entry index;
	fileSearch("packdata");
	FILE* pack = fopen("data.bin", "wb");
	DWORD FileCount = FilePool.size();
	fwrite(&FileCount, 4, 1, pack);

	DWORD pos = 0;
	DWORD ordsize = 0;
	for (string f : FilePool)
	{
		string fnm = f.substr(f.find_first_of("\\") + 1);
		FileNameToLower(fnm);
		FILE* fp = fopen(f.c_str(), "rb");
		fseek(fp, 0, SEEK_END);
		DWORD size = ftell(fp);
		fseek(fp, 0, SEEK_SET);
		BYTE* buff = new BYTE[size];
		fread(buff, size, 1, fp);
		fclose(fp);
		
		memset(index.FileName, 0, 64);
		CHAR* LowFileName = (CHAR*)fnm.c_str();
		strcpy(index.FileName, LowFileName);
		index.Size = size;
		index.Offset = ordsize;
		fwrite(&index, sizeof(entry), 1, pack);

		fseek(pack, 4 + FileCount * sizeof(entry) + ordsize, SEEK_SET);
		ordsize += size;
		fwrite(buff, size, 1, pack);
		delete[] buff;
		pos += 1;
		fseek(pack, 4 + pos * sizeof(entry), SEEK_SET);
	}
	cout << "over" << endl;
	fclose(pack);
	system("pause");
	return 0;
}
#endif
BYTE* HostAlloc(DWORD Size)
{
	return new BYTE[Size];
}
VOID HostFree(PVOID MEM)
{
	delete[] MEM;
}
class VFSFileSytem
{
public:
	VFSFileSytem();
	~VFSFileSytem();
	BOOL InitFileSystem();
	BOOL GetFileByName(CHAR* fnm, BYTE** FileBuff, DWORD* FileSize);
private:
	DWORD FileCount;
	entry* index;
	FILE* fp;
};
VFSFileSytem::VFSFileSytem()
{

}
VFSFileSytem::~VFSFileSytem()
{
	HostFree(index);
	fclose(fp);
}
BOOL VFSFileSytem::InitFileSystem()
{
	fp = fopen("data.bin", "rb");
	if (!fp)
		return FALSE;
	FileCount = 0;
	fread(&FileCount, 4, 1, fp);
	index = new entry[FileCount * sizeof(entry)];
	fread(index, FileCount * sizeof(entry), 1, fp);
	return TRUE;
}


BOOL VFSFileSytem::GetFileByName(CHAR* fnm, BYTE** FileBuff, DWORD* FileSize)
{
	string lowfnm = string(fnm);
	FileNameToLower(lowfnm);
	for (int i = 0; i < FileCount; i++)
	{
		if (!strcmp(index[i].FileName, lowfnm.c_str()))
		{
			fseek(fp, 4 + FileCount * sizeof(entry) + index[i].Offset, SEEK_SET);
			cout << "-----------------------------------------------------" << endl;
			cout << lowfnm << endl;
			cout << hex << index[i].Offset << endl;
			cout << hex << index[i].Size << endl;
			BYTE* OutBuff = HostAlloc(index[i].Size);
			fread(OutBuff, index[i].Size, 1, fp);
			*FileBuff = OutBuff;
			*FileSize = index[i].Size;
			return TRUE;
		}
	}
	return FALSE;
}
#ifdef unpack
int main()
{
	VFSFileSytem VFS;
	VFS.InitFileSystem();
	BYTE* buff;
	DWORD size;
	if (VFS.GetFileByName((CHAR*)"_logo_mg", &buff, &size))
	{
		FILE* fp = fopen("_logo_mg.bin", "wb");
		fwrite(buff, size, 1, fp);
		fclose(fp);
	}
	else
		cout << "err" << endl;
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
