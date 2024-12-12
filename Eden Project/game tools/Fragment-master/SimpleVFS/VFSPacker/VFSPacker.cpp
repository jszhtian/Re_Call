// VFSPacker.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"


void GenSHA256(const char* Str, byte* Salt, char** SHAContent)
{
	CryptoPP::SHA256 hash;
	hash.Update((byte*)Str, strlen(Str));
	hash.Update(Salt, CryptoPP::Salsa20::IV_LENGTH);
	hash.Final((byte*)*SHAContent);
}

void EncHelper(const char* Buf, char** OutBuf,const char* Key,const char* Salt, int OriLen)
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
	bool encrypt_flag= true;
	bool packed_flag=false;
	char FileName_SHA256[CryptoPP::SHA256::DIGESTSIZE];
};
#pragma pack()
using namespace std;
int main()
{
	/*
    std::cout << "Hello World!\n"; 
	std::string test = "TESTFILE.exe";
	CryptoPP::AutoSeededRandomPool prng;
	CryptoPP::HexEncoder encoder(new CryptoPP::FileSink(std::cout));
	CryptoPP::SecByteBlock key(16), iv(8);
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
	char* OutSHA = new char[CryptoPP::SHA256::DIGESTSIZE];
	GenSHA256(test.c_str(), iv.data(), &OutSHA);
	std::cout << "SHA256: ";
	encoder.Put((const byte*)OutSHA, CryptoPP::SHA256::DIGESTSIZE);
	encoder.MessageEnd();
	std::cout << std::endl;
	char* CmpBuf = nullptr;
	uint32_t COMPSIZE = 0;
	deflatHelper(test.c_str(), test.length(), &COMPSIZE, &CmpBuf);
	char* output = new char[test.length() + 1];
	memset(output, 0, test.length() + 1);
	inflatHelper(CmpBuf, test.length(), COMPSIZE, &output);
	std::cout << "TEST STR:" << output << std::endl;
	char* encOUT= new char[test.length() + 1];
	char* decOUT = new char[test.length() + 1];
	memset(encOUT, 0, test.length() + 1);
	memset(decOUT, 0, test.length() + 1);
	EncHelper(output, &encOUT, (char*)key.data(), (char*)iv.data(), test.length());
	DecHelper(encOUT, &decOUT, (char*)key.data(), (char*)iv.data(), test.length());
	std::cout << "DEC TEST STR:" << decOUT << std::endl;
	*/
	CryptoPP::AutoSeededRandomPool prng;
	CryptoPP::HexEncoder encoder(new CryptoPP::FileSink(std::cout));
	CryptoPP::SecByteBlock key(CryptoPP::Salsa20::MAX_KEYLENGTH), iv(CryptoPP::Salsa20::IV_LENGTH);
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

	VFSIndex IdxList[0x1000];
	ifstream ftxt("files.txt");
	fstream fpak;
	fstream fidx;
	fstream fKey;
	fpak.open("pack.pck", ios::binary | std::fstream::out);
	fidx.open("pack.idx", ios::binary | std::fstream::out);
	fKey.open("pack.key", ios::binary | std::fstream::out);
	fKey.write((char*)key.data(), key.size());
	fKey.write((char*)iv.data(), iv.size());
	fKey.close();
	if (!ftxt.is_open())
	{
		return false;
	}
	int idx = 0;
	uint32_t offset = 0;
	while (!ftxt.eof())
	{
		char tmp[0x1000] = {0};
		ftxt.getline(tmp, 0x1000);
		//string text(tmp);
		fstream felm;
		felm.open(tmp, ios::binary| std::fstream::in);
		felm.seekg(0, std::ios::end);
		uint32_t OriLength = felm.tellg();
		felm.seekg(0, std::ios::beg);
		char* RawBuf = new char[OriLength];
		felm.read(RawBuf, OriLength);
		felm.close();
		char* OutSHA = new char[CryptoPP::SHA256::DIGESTSIZE];
		GenSHA256(tmp, iv.data(), &OutSHA);
		memcpy(IdxList[idx].FileName_SHA256, OutSHA, CryptoPP::SHA256::DIGESTSIZE);		
		IdxList[idx].orig_size = OriLength;
		char* AftComp = nullptr;
		if (OriLength < 0x500)
		{
			IdxList[idx].packed_flag = false;
			IdxList[idx].pck_size = OriLength;
			AftComp = RawBuf;
		}
		else
		{
			IdxList[idx].packed_flag = true;
			char* CmpBuf = nullptr;
			uint32_t COMPSIZE = 0;
			deflatHelper(RawBuf, OriLength, &COMPSIZE, &CmpBuf);
			IdxList[idx].pck_size = COMPSIZE;
			AftComp = CmpBuf;
		}
		char* encOUT = new char[IdxList[idx].pck_size];
		::memset(encOUT, 0, IdxList[idx].pck_size);
		EncHelper(AftComp, &encOUT, (char*)key.data(), (char*)iv.data(), IdxList[idx].pck_size);
		IdxList[idx].encrypt_flag = true;
		fpak.write(encOUT, IdxList[idx].pck_size);
		IdxList[idx].offset = offset;
		offset += IdxList[idx].pck_size;
		
		idx++;
	}
	VFSIdxHeader hdr;
	hdr.EntryCount = idx;
	char Magic[] = "PACK";
	memcpy(hdr.Magic, Magic, 4);
	fidx.write((char*)&hdr, sizeof(VFSIdxHeader));
	fidx.write((char*)IdxList, sizeof(VFSIndex)*idx);
	fidx.close();
	fpak.close();
	ftxt.close();
	std::cout << "Load Files:0x" << hex << idx << endl;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
