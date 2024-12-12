// extNonColor.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "3rd/zlib.h"
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

#if 1

#pragma pack (1)
struct DatEntry
{
	DWORD hash;
	BYTE  IsCompress;
	DWORD Offset;
	DWORD PackedSize;
	DWORD UnpackSize;
};
#pragma pack ()

DWORD swap_to_le(DWORD x)
{
	int y;
	char* a = (char*)&y;
	a[0] = x >> 24;
	a[1] = x >> 16;
	a[2] = x >> 8;
	a[3] = x;
	return y;
}

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		MessageBox(0, L"extNonColor.exe <input dat file>", L"AyamiKaze", 0);
		return -1;
	}
	CHAR* FileName = argv[1];
	FILE* fp = fopen(FileName, "rb");
	DWORD Count = 0;
	fread(&Count, 1, 4, fp);

	Count = swap_to_le(Count);
	cout << "Count:0x" << hex << Count << endl;
	
	DatEntry* Entry = new DatEntry[sizeof(DatEntry) * Count];
	fread(Entry, 1, sizeof(DatEntry) * Count, fp);

	FILE* idx = fopen("idx.bin", "wb");
	fwrite(Entry, 1, sizeof(DatEntry) * Count, idx);
	fclose(idx);

	for (DWORD i = 0; i < Count; i++)
	{
		DWORD hash = swap_to_le(Entry[i].hash);
		DWORD Offset = swap_to_le(Entry[i].Offset);
		DWORD PackedSize = swap_to_le(Entry[i].PackedSize);
		DWORD UnpackSize = swap_to_le(Entry[i].UnpackSize);

		cout << "---------------------------------------" << endl;
		cout << "hash:0x" << hex << hash << endl;
		cout << "Offset:0x" << hex << Offset << endl;
		cout << "PackedSize:0x" << hex << PackedSize << endl;
		cout << "UnpackSize:0x" << hex << UnpackSize << endl;

		BYTE* FileBuff = new BYTE[PackedSize];
		fseek(fp, Offset, SEEK_SET);
		fread(FileBuff, 1, PackedSize, fp);

		BYTE* DstBuff = nullptr;
		DWORD DstSize = 0;
		if (PackedSize != UnpackSize)
		{
			cout << "uncompress" << endl;
			DstBuff = new BYTE[UnpackSize];
			if (uncompress(DstBuff, &UnpackSize, FileBuff, PackedSize) != Z_OK)
				return E("zlib uncompress error");
			delete[] FileBuff;
			DstSize = UnpackSize;
		}
		else 
		{
			DstBuff = FileBuff;
			DstSize = PackedSize;
		}

		char OutName[MAX_PATH];
		sprintf(OutName, "output\\%x", hash);
		cout << "OutName:" << OutName << endl;
		FILE* fout = fopen(OutName, "wb");
		fwrite(DstBuff, 1, DstSize, fout);
		fclose(fout);
		cout << "done" << endl;
	}

	fclose(fp);
	system("pause");
	return 0;
}
#endif


UINT GetKey(UINT64 a)
{
	UINT Hi = (UINT)(a & 4294967295);
	UINT Lo = (UINT)(a >> 32);
	return Hi ^ 0x30BC61C8;
}

#if 0
int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		MessageBox(0, L"extNonColor.exe <input dat file>", L"AyamiKaze", 0);
		return -1;
	}
	CHAR* FileName = argv[1];
	FILE* fp = fopen(FileName, "rb");
	DWORD Header = 0;
	DWORD Count = 0;
	fread(&Header, 1, 4, fp);
	fread(&Count, 1, 4, fp);

	Count = Count ^ 0x8B6A4E5F;

	cout << "Count:0x" << hex << Count << endl;

	DatEntry* Entry = new DatEntry[sizeof(DatEntry) * Count];
	fread(Entry, 1, sizeof(DatEntry) * Count, fp);

	FILE* idx = fopen("idx.bin", "wb");
	fwrite(Entry, 1, sizeof(DatEntry) * Count, idx);
	fclose(idx);

	for (DWORD i = 0; i < Count; i++)
	{
		UINT64 hash = Entry[i].hash;
		BYTE IsCompress = Entry[i].IsCompress ^ Entry[i].hash;
		DWORD Offset = Entry[i].Offset ^ Entry[i].hash ^ 0x8B6A4E5F;
		DWORD PackedSize = Entry[i].PackedSize ^ Entry[i].hash;
		DWORD UnpackSize = Entry[i].UnpackSize ^ Entry[i].hash;

		cout << "---------------------------------------" << endl;
		cout << "hash:0x" << hex << hash << endl;
		cout << "IsCompress:0x" << hex << (DWORD)IsCompress << endl;
		cout << "Offset:0x" << hex << Offset << endl;
		cout << "PackedSize:0x" << hex << PackedSize << endl;
		cout << "UnpackSize:0x" << hex << UnpackSize << endl;

		///*
		BYTE* FileBuff = new BYTE[PackedSize];
		fseek(fp, Offset, SEEK_SET);
		fread(FileBuff, 1, PackedSize, fp);

		DWORD Key = GetKey(hash);
		//Key ^= 0x30BC61C8;

		cout << hex << Key << endl;

		DWORD xorCount = PackedSize >> 2;
//		/*
		_asm
		{
			mov esi, FileBuff;
			xor eax, eax;
			mov ecx, xorCount;
			mov edx, Key;

		con:
			xor DWORD PTR DS:[esi + eax * 4], edx;
			inc eax;
			cmp eax, ecx;
			jb con;
		}
//		*/
//		for (int j = 0; j < xorCount; j++)
//			*(DWORD*)&FileBuff[j * 4] ^= Key;
		BYTE* DstBuff = nullptr;
		DWORD DstSize = 0;
		if (PackedSize != UnpackSize)
		{
			cout << "uncompress" << endl;
			DstBuff = new BYTE[UnpackSize * 2];
			if (uncompress(DstBuff, &UnpackSize, FileBuff, PackedSize) != Z_OK)
				return E("zlib uncompress error");
			delete[] FileBuff;
			DstSize = UnpackSize;
		}
		else
		{
			DstBuff = FileBuff;
			DstSize = PackedSize;
		}

		char OutName[MAX_PATH];
		string HashName = to_string(hash);
		sprintf(OutName, "output\\%s", HashName.c_str());
		cout << "OutName:" << OutName << endl;
		FILE* fout = fopen(OutName, "wb");
		fwrite(DstBuff, 1, DstSize, fout);
		fclose(fout);
		cout << "done" << endl;
		//*/
	}

	fclose(fp);
	system("pause");
	return 0;
}
#endif

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
#if 0
int main(int argc, char* argv[])
{
	fileSearch("output2");
	
	FILE* fp = fopen("script.chs", "wb");
	DWORD FileCount = FilePool.size() ^ 0x8B6A4E5F;
	fwrite("ACV1", 1, 4, fp);
	fwrite(&FileCount, 1, 4, fp);

	cout << "FileCount:" << FilePool.size() << endl;

	DatEntry Index;
	DWORD Count = 0;
	DWORD Pos = 0;
	for (auto f : FilePool)
	{
		string  FileName = f.substr(f.find("\\") + 1);
		UINT64 Hash = stoull(FileName);
		//cout << FileName << "|" << Hash << endl;
		cout << "--------------------------------------------" << endl;
		cout << "FileHash:" << Hash << endl;

		FILE* fin = fopen(f.c_str(), "rb");
		fseek(fin, 0, SEEK_END);
		DWORD FileSize = ftell(fin);
		fseek(fin, 0, SEEK_SET);
		BYTE* FileBuff = new BYTE[FileSize];
		fread(FileBuff, FileSize, 1, fin);
		fclose(fin);

		cout << "UncompressSize:0x" << hex << FileSize << endl;

		DWORD CompressSize = FileSize;
		BYTE* CompressBuff = new BYTE[CompressSize];

		if (compress(CompressBuff, &CompressSize, FileBuff, FileSize) != Z_OK)
			return E("Compress error");
		delete[] FileBuff;

		cout << "CompressSize:0x" << hex << CompressSize << endl;

		DWORD Key = GetKey(Hash);
		DWORD xorCount = CompressSize >> 2;

		cout << "start encrypt" << endl;
		_asm
		{
			mov esi, CompressBuff;
			xor eax, eax;
			mov ecx, xorCount;
			mov edx, Key;

		con:
			xor DWORD PTR DS : [esi + eax * 4] , edx;
			inc eax;
			cmp eax, ecx;
			jb con;
		}
		cout << "end encrypt" << endl;

		DWORD Off = 8 + sizeof(DatEntry) * FilePool.size() + Pos;
		cout << "Offset:0x" << hex << Off << endl;

		Index.hash = Hash;
		Index.IsCompress = 0x2 ^ Hash;
		Index.Offset = Off ^ Hash ^ 0x8B6A4E5F;
		Index.PackedSize = CompressSize ^ Hash;
		Index.UnpackSize = FileSize ^ Hash;

		//Pos += CompressSize;

		fwrite(&Index, 1, sizeof(DatEntry), fp);

		fseek(fp, Off, SEEK_SET);

		fwrite(CompressBuff, CompressSize, 1, fp);
		delete[] CompressBuff;

		Count++;
		Pos += CompressSize;

		fseek(fp, 8 + sizeof(DatEntry) * Count, SEEK_SET);
	}

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
