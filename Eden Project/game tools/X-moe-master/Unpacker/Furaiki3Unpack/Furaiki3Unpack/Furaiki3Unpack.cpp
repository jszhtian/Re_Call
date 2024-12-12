// Furaiki3Unpack.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <Windows.h>
#include <cstring>
#include <vector>
#include <string>
#include <iostream>
using std::string;


#pragma comment(lib, "user32.lib")
/*
Map File Struct
*/

struct FileMap
{
	DWORD FileNameLength;
	char FileName[MAX_PATH];//big enough
	DWORD FilePart; //eg : Grp_01
	DWORD Unknown; //0
	DWORD Offset;
	//DWORD SeekCur;
	DWORD Length;

	FileMap() :
		FileNameLength(0),
		FilePart(0),
		Unknown(0),
		Offset(0),
		Length(0)
		//SeekCur(0)
	{
		memset(FileName, 0, sizeof(FileName));
	}
};


DWORD flip(DWORD x) 
{
	return (x >> 24) | ((x << 8) & 0x00FF0000) | ((x >> 8) & 0x0000FF00) | (x << 24);
}

void Decode(unsigned char* pBuffer, DWORD len)
{
	for (int i = 0; i < len; i++)
	{
		pBuffer[i] = ~pBuffer[i];
	}
}

std::vector<FileMap> fileMap;

enum
{
	GRP = 0,
	SOUND = 1,
	NONE = 2
};

int main(int argc, char* argv[])
{
	if (argc != 3)
	{
		MessageBoxW(NULL, L"Usage : Furaiki3Unpack.exe <type string> <mapfile>", L"Furaiki3Unpack", MB_OK);
	}
	
	//FILE* file = fopen(argv[1], "rb");
	char filename[260] = { 0 };
	int type = 0;
	if (strcmp(argv[1], "grp") == 0)
	{
		type = GRP;
	}
	else if (strcmp(argv[1], "sound") == 0)
	{
		type = SOUND;
	}
	else
		type = NONE;

	FILE* map = fopen(argv[2], "rb");

	if (map == NULL)
	{
		MessageBoxW(NULL, L"Cannot open map file", L"Furaiki3Unpack", MB_OK);
		return 0;
	}

	fseek(map, 0, SEEK_END);
	unsigned int len = ftell(map);
	rewind(map);

	unsigned char* pBufferMap = new unsigned char[len];
	fread(pBufferMap, 1, len, map);

	for (int i = 0; i < len; i++)
	{
		pBufferMap[i] = ~pBufferMap[i];
	}

	//fseek(file, 0, SEEK_END);
	//DWORD FileLength = ftell(file);
	//rewind(file);

	DWORD iPos = 0;
	while (iPos < len)
	{
		FileMap map;
		map.FileNameLength = *(DWORD*)(pBufferMap + iPos);
		map.FileNameLength = flip(map.FileNameLength);
		iPos += 4; 
		printf("Name Length : %x\n", map.FileNameLength);
		strncpy(map.FileName, (char*)(pBufferMap + iPos), map.FileNameLength);
		printf("File name : %s\n", map.FileName);
		iPos += map.FileNameLength;
		map.FilePart = *(DWORD*)(pBufferMap + iPos);
		map.FilePart = flip(map.FilePart);
		printf("File part : %d\n", map.FilePart);
		iPos += 4;
		iPos += 4; //unk
		map.Offset = *(DWORD*)(pBufferMap + iPos);
		map.Offset = flip(map.Offset);
		printf("File offset : %x\n", map.Offset);
		iPos += 4;
		//iPos += 4;//SeekCur
		map.Length = *(DWORD*)(pBufferMap + iPos);
		map.Length = flip(map.Length);
		printf("File length : %x\n", map.Length);
		iPos += 4;
		fileMap.push_back(map);
	}


	iPos = 0;
	for (unsigned int i = 0; i < fileMap.size(); i++)
	{

		//if (fileMap[i].FilePart != 0)
		//{
			memset(filename, 0, sizeof(filename));
			if (type == GRP)
			{
				//sprintf(filename, "Grp_%02d.dat", fileMap[i].FilePart);
				sprintf(filename, "Grp_%02d.dat", fileMap[i].FilePart);
			}
			else if (type == SOUND)
			{
				sprintf(filename, "Snd_%02d.dat", fileMap[i].FilePart);
			}
			else
			{
				sprintf(filename, "%s", fileMap[i].FileName);
			}
		//}

		char dic[260] = { 0 };
		GetCurrentDirectoryA(260, dic);
		
		string szDic(dic);
		szDic += "\\";
		szDic += filename;
		std::cout << szDic << std::endl;
		HANDLE file = INVALID_HANDLE_VALUE;
		file = CreateFileA(szDic.c_str(), GENERIC_READ, FILE_SHARE_READ, 
			NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		//file = fopen(szDic.c_str(), "rb");
		//file = fopen("Grp_02.dat", "rb");
		if (file == INVALID_HANDLE_VALUE)
		{
			char szInfo[512];
			sprintf(szInfo, "Cannot find %s  --%08x\n", filename, GetLastError());
			printf(szInfo);
			//MessageBoxA(NULL, szInfo, "Error", MB_OK);
			//printf("Cannot find 2\n");
			continue;
		}

		char path[260] = { 0 };
		int ptr = 0;
		bool needCreate = false;
		int j = 0;
		int count = 0;
		while (*(fileMap[i].FileName + j) != 0)
		{
			if (fileMap[i].FileName[j] == '\\'&&
				fileMap[i].FileName[j] == '/')
			{
				count++;
			}
			j++;
		}
		while (*(fileMap[i].FileName + ptr) != 0)
		{
			if (fileMap[i].FileName[ptr] == '\\'&&
				fileMap[i].FileName[ptr] == '/')
			{
				needCreate = true;
				count--;
				if (count == 0)
				{
					break;
				}
			}
			path[ptr] = fileMap[i].FileName[ptr];
			ptr++;
		}

		if (needCreate)
		{
			string rPath(dic);
			rPath += "\\";
			rPath += path;

			printf("Creating : %s\n", rPath.c_str());
			getchar();
			if (CreateDirectoryA(rPath.c_str(), NULL) == 0)
			{
				printf("Failed to create directory %08x\n", GetLastError());
				getchar();
			}
		}

		j = 0;
		while (*(fileMap[i].FileName + j) != 0)
		{
			if (fileMap[i].FileName[j] == '\\' ||
				fileMap[i].FileName[j] == '/')
			{
				fileMap[i].FileName[j] = '_';
			}
			j++;
		}

		

		HANDLE out = INVALID_HANDLE_VALUE;
		out = CreateFileA(fileMap[i].FileName, GENERIC_WRITE, FILE_SHARE_WRITE,
			NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
		//FILE* out = fopen(fileMap[i].FileName, "wb");
		if (out == INVALID_HANDLE_VALUE)
		{
			printf("An error ocurred while dumping file %s\n", fileMap[i].FileName);
			continue;
		}

		unsigned char* pFileBuffer = NULL;
		try
		{
			pFileBuffer = new unsigned char[fileMap[i].Length];
		}
		catch (...)
		{
			printf("You should ensure that you have enough memory to do this operation\n");
			if (pFileBuffer)
				delete[] pFileBuffer;
			continue;
		}

		/*
		if (fileMap[i].Offset >= FileLength)
			break;
		*/

		//fseek(file, fileMap[i].Offset, SEEK_SET);
		SetFilePointer(file, fileMap[i].Offset, NULL, SEEK_SET);
		DWORD readed = 0;
		ReadFile(file, pFileBuffer, fileMap[i].Length, &readed, NULL);
		//fread(pFileBuffer, 1, fileMap[i].Length, file);
		Decode(pFileBuffer, fileMap[i].Length);
		//fwrite(pFileBuffer, 1, fileMap[i].Length, out);
		WriteFile(out, pFileBuffer, fileMap[i].Length, &readed, NULL);

		//fclose(out);
		CloseHandle(out);
		delete pFileBuffer;
		//fclose(file);
		CloseHandle(file);
	}

	fclose(map);

	printf("Done!\n");
	return 0;
}

