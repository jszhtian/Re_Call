// Pack2.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <Windows.h>
#include <io.h>
#include <direct.h>
#include <string>
#include <vector>
#include <algorithm> 
#include "zlib.h"
#include "BH_huffman_comp.h"
#include "WinFile.h"

#include "..\3rd\cryptopp-master\sha.h"
#include "..\3rd\cryptopp-master\salsa.h"
#include "..\3rd\cryptopp-master\chacha.h"
#include "..\3rd\cryptopp-master\osrng.h"
#include "..\3rd\cryptopp-master\cryptlib.h"
#include "..\3rd\cryptopp-master\hex.h"
#include "..\3rd\cryptopp-master\files.h"
#include "..\3rd\cryptopp-master\secblock.h"
#include "..\3rd\cryptopp-master\zlib.h"

#pragma comment(lib, "zlib.lib")
#pragma comment(lib, "cryptlib.lib")

#define PackageName L"sts_chs.xp3"
#define PackageList L"sts_chs2.xp3"
using namespace std;
using namespace CryptoPP;

void GenSHA256(const wchar_t* Str, byte* Salt, char** SHAContent)
{
	CryptoPP::SHA256 hash;
	hash.Update((byte*)Str, lstrlen(Str));
	hash.Update(Salt, CryptoPP::Salsa20::IV_LENGTH);
	hash.Final((byte*)*SHAContent);
}


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

void EncHelper_chacha(const char* buff, char** out_buff, const char* key, const char* salt, int OriLen)
{
	try
	{
		ChaCha::Encryption enc;
		enc.SetKeyWithIV((byte*)key, ChaCha::MAX_KEYLENGTH, (byte*)salt, ChaCha::IV_LENGTH);
		enc.ProcessData((byte*)*out_buff, (byte*)buff, OriLen);
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

void DecHelper_chacha(const char* buff, char** out_buff, const char* key, const char* salt, int OriLen)
{
	try
	{
		ChaCha::Decryption dec;
		dec.SetKeyWithIV((byte*)key, ChaCha::MAX_KEYLENGTH, (byte*)salt, ChaCha::IV_LENGTH);
		dec.ProcessData((byte*)*out_buff, (byte*)buff, OriLen);
	}
	catch (const CryptoPP::Exception& e)
	{
		std::cerr << e.what() << std::endl;
		exit(1);
	}
}

void inflatHelper(const char* Buf, uint32_t oriLen, uint32_t compLen, char** OutBuf)
{
	CryptoPP::ZlibDecompressor dec;
	dec.Put((byte*)Buf, compLen);
	dec.MessageEnd();
	dec.Get((byte*)*OutBuf, (size_t)oriLen);
}

void deflatHelper(const char* Buf, uint32_t oriLen, uint32_t* compLen, char** OutBuf)
{
	CryptoPP::ZlibCompressor cmp;
	cmp.Put((byte*)Buf, oriLen);
	cmp.MessageEnd();
	*compLen = cmp.MaxRetrievable();
	*OutBuf = new char[*compLen];
	cmp.Get((byte*)*OutBuf, (size_t)*compLen);
}

int E(const char* msg)
{
	MessageBoxA(0, msg, 0, 0);
	return -1;
}
vector<wstring>FilePool;

void TravelDir(wstring& Path)
{
	WIN32_FIND_DATAW FindData;
	HANDLE           Handle;
	std::wstring     CurrentPath, FullPath;

	CurrentPath = Path + L"\\*.*";
	Handle = FindFirstFileW(CurrentPath.c_str(), &FindData);
	if (Handle == INVALID_HANDLE_VALUE)
		return;

	while (FindNextFileW(Handle, &FindData))
	{
		if (lstrcmpW(FindData.cFileName, L".") == 0 ||
			lstrcmpW(FindData.cFileName, L"..") == 0)
		{
			continue;
		}

		FullPath = Path + L"\\";
		FullPath += FindData.cFileName;
		if (!(FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			FilePool.push_back(FullPath);
		}
		else
		{
			TravelDir(FullPath);
		}
	}
}

void FileNameToLower(std::wstring& FileName)
{
	std::transform(FileName.begin(), FileName.end(), FileName.begin(), ::tolower);
}



char* wtoc(LPCTSTR str)
{
	DWORD dwMinSize;
	dwMinSize = WideCharToMultiByte(936, NULL, str, -1, NULL, 0, NULL, FALSE); //计算长度
	char* out = new char[dwMinSize];
	WideCharToMultiByte(936, NULL, str, -1, out, dwMinSize, NULL, FALSE);//转换
	return out;
}

#pragma pack (1)
typedef struct {
	unsigned int FileCount;
	unsigned int index_size;
	char w_key[Salsa20::MAX_KEYLENGTH];
	char w_iv[Salsa20::IV_LENGTH];
}cpk_header_t;
typedef struct {
	char chacha_key[ChaCha::MAX_KEYLENGTH];
	char chacha_iv[ChaCha::IV_LENGTH];
	char FileName[SHA256::DIGESTSIZE];
	unsigned int CompSize;
	unsigned int FileSize;
	unsigned int offset;
}cpk_entry_t;
#pragma pack ()
cpk_header_t header;
cpk_entry_t entry;


#define pack

#ifdef pack
int main()
{
	AutoSeededRandomPool prng;
	HexEncoder encoder(new FileSink(std::cout));
	SecByteBlock key(CryptoPP::Salsa20::MAX_KEYLENGTH), iv(CryptoPP::Salsa20::IV_LENGTH);
	prng.GenerateBlock(key, key.size());
	prng.GenerateBlock(iv, iv.size());

	std::cout << "Key: ";
	encoder.Put((const byte*)key.data(), key.size());
	encoder.MessageEnd();
	std::cout << std::endl;

	std::cout << "IV: ";
	encoder.Put((const byte*)iv.data(), iv.size());
	encoder.MessageEnd();
	std::cout << std::endl;



	wstring Dir(L"PackData");
	TravelDir(Dir);
	auto cpk = _wfopen(PackageName, L"wb");
	auto idx = _wfopen(PackageList, L"wb");
	header.FileCount = FilePool.size();
	header.index_size = header.FileCount * sizeof(cpk_entry_t);
	memcpy(header.w_key, key.data(), key.size());
	memcpy(header.w_iv, iv.data(), iv.size());

	fwrite(&header, sizeof(cpk_header_t), 1, idx);
	DWORD ord = 0;
	DWORD pos = 0;
	for (wstring f : FilePool)
	{
		wstring fn = f.substr(f.find_first_of(L"\\") + 1).c_str();
		FileNameToLower(fn);

		char* OutSHA = (char*)malloc(SHA256::DIGESTSIZE);
		GenSHA256(fn.c_str(), (byte*)header.w_iv, &OutSHA);
		cout << fn.c_str() << "|" << (unsigned long long)OutSHA << endl;
		memcpy(entry.FileName, OutSHA, SHA256::DIGESTSIZE);

		AutoSeededRandomPool FilePrng;
		HexEncoder FileEncoder(new FileSink(std::cout));
		SecByteBlock FileKey(32), FileIv(8);
		FilePrng.GenerateBlock(FileKey, FileKey.size());
		FilePrng.GenerateBlock(FileIv, FileIv.size());

		std::cout << "FileKey: ";
		FileEncoder.Put((const byte*)FileKey.data(), FileKey.size());
		FileEncoder.MessageEnd();
		std::cout << std::endl;

		std::cout << "FileIV: ";
		FileEncoder.Put((const byte*)FileIv.data(), FileIv.size());
		FileEncoder.MessageEnd();
		std::cout << std::endl;
		memcpy(entry.chacha_key, FileKey.data(), FileKey.size());
		memcpy(entry.chacha_iv, FileIv.data(), FileIv.size());
		cout << "Packing:" << wtoc(fn.c_str()) << endl;
		auto fp = _wfopen(f.c_str(), L"rb");// read file with dir
		if (!fp)
			return E("no fp");
		fseek(fp, 0, SEEK_END);
		DWORD size = ftell(fp);
		fseek(fp, 0, SEEK_SET);
		BYTE* buff = (BYTE*)malloc(size);
		if (!buff)
			return E("no buff");
		fread(buff, size, 1, fp);
		fclose(fp);

		entry.offset = ord;
		entry.FileSize = size;
		char* FinBuff2 = nullptr;
		char* WriteToPack = (char*)malloc(entry.FileSize);
		if (!WriteToPack)
			return E("no WriteToPack");
		memset(WriteToPack, 0, entry.FileSize);
		DWORD WriteSize = 0;
		if (size < 0x500)
		{
			entry.CompSize = 0;
			FinBuff2 = (char*)buff;
			EncHelper((char*)FinBuff2, &WriteToPack, header.w_key, header.w_iv, entry.FileSize);
			ord += entry.FileSize;
			WriteSize = entry.FileSize;
		}
		else
		{
			cout << "compress" << endl;
			char* comp_buff = (char*)malloc(entry.FileSize);
			size_t comp_size = entry.FileSize;
			deflatHelper((char*)buff, entry.FileSize, &comp_size, &comp_buff);
			cout << comp_size << endl;
			cout << "compress finish" << endl;
			entry.CompSize = comp_size;
			EncHelper(comp_buff, &WriteToPack, header.w_key, header.w_iv, entry.CompSize);
			cout << "enc finish" << endl;
			ord += entry.CompSize;
			WriteSize = entry.CompSize;
		}
		fwrite(&entry, sizeof(cpk_entry_t), 1, idx);
		char* WriteBuff = (char*)malloc(WriteSize);
		EncHelper_chacha(WriteToPack, &WriteBuff, entry.chacha_key, entry.chacha_iv, WriteSize);
		fwrite(WriteBuff, WriteSize, 1, cpk);
		cout << "----------------------------------------------------------------------" << endl;
	}
	fclose(cpk);
	fclose(idx);
	system("pause");
	return 0;
}
#else

FILE* cpk;
FILE* idx;
cpk_entry_t* entry_buff = nullptr;
int Init()
{
	cpk = _wfopen(PackageName, L"rb");

	idx = _wfopen(PackageList, L"rb");
	fread(&header, sizeof(cpk_header_t), 1, idx);
	cout << header.FileCount << endl;
	cout << header.index_size << endl;
	entry_buff = (cpk_entry_t*)malloc(header.index_size);
	if (!entry_buff)
		return E("No entry_buff");
	fread(entry_buff, header.index_size, 1, idx);
	return 0;
}

int ReadFileByFileName(wstring fnm, BYTE** buff, DWORD* size)
{
	FileNameToLower(fnm);
	char* OutSHA = (char*)malloc(SHA256::DIGESTSIZE);
	GenSHA256(fnm.c_str(), (byte*)header.w_iv, &OutSHA);
	cout << header.FileCount << endl;
	for (int i = 0; i < header.FileCount; i++)
	{
		if (memcmp(entry_buff[i].FileName, OutSHA, SHA256::DIGESTSIZE) == 0)
		{
			fseek(cpk, entry_buff[i].offset, SEEK_SET);
			BYTE* out_buff = nullptr;
			char* FinBuff1 = nullptr;
			char* FinBuff2 = nullptr;
			if (entry_buff[i].CompSize != 0)
			{
				cout << "compress" << endl;
				out_buff = (BYTE*)malloc(entry_buff[i].CompSize);
				fread(out_buff, entry_buff[i].CompSize, 1, cpk);
				FinBuff1 = (char*)malloc(entry_buff[i].CompSize);
				FinBuff2 = (char*)malloc(entry_buff[i].CompSize);
				size_t FinSize = 0;
				DecHelper_chacha((char*)out_buff, &FinBuff1, entry_buff[i].chacha_key, entry_buff[i].chacha_iv, entry_buff[i].CompSize);
				DecHelper(FinBuff1, &FinBuff2, header.w_key, header.w_iv, entry_buff[i].CompSize);
				cout << "decrypt finish" << endl;
				char* decomp_buff = (char*)malloc(entry_buff[i].FileSize);
				size_t decomp_size = entry_buff[i].FileSize;
				inflatHelper((char*)FinBuff2, decomp_size,  entry_buff[i].CompSize, &decomp_buff);
				*buff = (BYTE*)decomp_buff;
				*size = decomp_size;
				free(out_buff);
				free(FinBuff1);
				free(FinBuff2);
			}
			else 
			{
				out_buff = (BYTE*)malloc(entry_buff[i].FileSize);
				fread(out_buff, entry_buff[i].FileSize, 1, cpk);
				FinBuff1 = (char*)malloc(entry_buff[i].FileSize);
				FinBuff2 = (char*)malloc(entry_buff[i].FileSize);
				size_t FinSize = 0;
				DecHelper_chacha((char*)out_buff, &FinBuff1, entry_buff[i].chacha_key, entry_buff[i].chacha_iv, entry_buff[i].FileSize);
				DecHelper((char*)FinBuff1, &FinBuff2, header.w_key, header.w_iv, entry_buff[i].FileSize);
				*buff = (BYTE*)FinBuff2;
				*size = entry_buff[i].FileSize;
				free(out_buff);
				free(FinBuff1);
			}
			free(OutSHA);
			return 1;
		}
	}
	free(OutSHA);
	MessageBox(0, L"NO SEARCH", 0, 0);
	return 0;
}

int main()
{
	if (Init() == -1)
		return -1;

	wstring fnm(L"AppConfig.tjs");
	//ALL MAP
	BYTE* buff;
	DWORD size = 0;
	if (ReadFileByFileName(fnm, &buff, &size)) 
	{
		cout << size << endl;
		auto fp = fopen(wtoc(fnm.c_str()), "wb");
		fwrite(buff, size, 1, fp);
		fclose(fp);
		free(buff);
	}
	free(entry_buff);
	fclose(cpk);
	fclose(idx);
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
