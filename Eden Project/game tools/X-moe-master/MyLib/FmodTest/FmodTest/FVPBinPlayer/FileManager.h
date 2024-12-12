#ifndef __fvp_FileManager_H__
#define __fvp_FileManager_H__

#include <cstdio>

#include <list>
#include <vector>
#include <string>
#include <cstring>
#include <map>

#include "zlib\zlib.h"

#pragma comment(lib,"zlib1.lib")


using namespace std;

///bin structure
struct binHeader
{
	unsigned int FileCount;
	unsigned int FileNameLength;
};

struct binEntry
{
	unsigned int FileNameOffset;
	unsigned int Offset;
	unsigned int Length;
};

////////////////////////////////

struct FileAtom
{
	//string FileName;
	//unsigned int HashName;
	unsigned int Offset;
	unsigned int Length;
	
	FileAtom &operator = (const FileAtom &rhs)
	{
		memcpy(this, &rhs, sizeof(FileAtom));
		return *this;
	}
};

class ARCinfo 
{
public:
	
	ARCinfo();
	
	~ARCinfo();
	
	///如果这个Arc没有加载的话  先读取信息  private
	bool read(FILE *ARCINFO, unsigned int archash);
	///
	bool deleteFile(void *buffer);
	
	char* Uncompress(char* compressed_mem, unsigned int compressed_size, unsigned int ori_size);
	
	char* getFile(const char *filename, unsigned int &size);
	
private:
	///arc
	bool isLoaded(unsigned int hash);
	///arc
	bool LoadArc(const char *str);
};

#endif