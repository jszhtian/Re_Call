#pragma once

#ifdef _DEBUG 
	#pragma comment(lib, "cryptlib_Debug.lib") 
#else
	#pragma comment(lib, "cryptlib_Release.lib") 
#endif


#include<windows.h>
#include<string>
#include<fstream>
#include<cryptopp/sha.h>
#include<cryptopp/salsa.h>
#include<cryptopp/cryptlib.h>
#include<vector>
#include<cryptopp/zlib.h>
#pragma pack(1)
struct VFSIdxHeader 
{
	char Magic[4];
	uint32_t EntryCount;
};
struct VFSIndex
{
	uint32_t offset;
	uint32_t orig_size;
	uint32_t pck_size;
	bool encrypt_flag;
	bool packed_flag;
	char FileName_SHA256[CryptoPP::SHA256::DIGESTSIZE];
};
struct VFSHandle
{
	HANDLE TmpFileHandle;
	char FileName_SHA256[CryptoPP::SHA256::DIGESTSIZE];
};
struct VFSFilePtr
{
	char* BufferPtr;
	char FileName_SHA256[CryptoPP::SHA256::DIGESTSIZE];
};
#pragma pack()
class SimpleVFS
{
public:
	SimpleVFS();
	bool Mount(const char* IdxFile,const char* ResFile);
	void Unmount();
	HANDLE GetFileHandleByName(const char* FileName);
	char* GetFileBufferByName(const char* FileName, DWORD* FileLength);
	void SetKey(char* Key, char* salt);
	~SimpleVFS();
private:
	char EncKey[CryptoPP::Salsa20::MAX_KEYLENGTH];
	char salt[CryptoPP::Salsa20::IV_LENGTH];
	char* VFSIdxBuffer;
	uint32_t VFSEntryCount;
	VFSIndex* VFSIndexList;
	std::fstream VFSRes;
};

