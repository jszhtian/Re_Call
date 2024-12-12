// FileProc.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <Windows.h>
#include <io.h>
#include <direct.h>
#include <string>
#include <vector>

#include "..\3rd\cryptopp-master\blake2.h"
#include "..\3rd\cryptopp-master\cryptlib.h"
#include "..\3rd\cryptopp-master\hex.h"
#include "..\3rd\cryptopp-master\files.h"
#include "..\3rd\cryptopp-master\secblock.h"
#include "..\3rd\cryptopp-master\aes.h"
#include "..\3rd\cryptopp-master\gcm.h"
#include "..\3rd\cryptopp-master\osrng.h"

#pragma comment(lib, "cryptlib.lib")

using namespace std;
using namespace CryptoPP;

int E(const char* msg)
{
	MessageBoxA(0, msg, 0, 0);
	return -1;
}

void GenBLAKE2(byte* Str, DWORD StrLen, char* Key, char** SHAContent)
{
	BLAKE2b hash;
	hash.Update(Str, StrLen);
	hash.Update((byte*)Key, AES::MAX_KEYLENGTH);
	hash.Final((byte*)*SHAContent);
}

typedef struct AymFileHeader {
	char Magic[23] = ">$CPK::NATIVE::FILE$<\n";
	char AES_ENC_IV[37] = "\n$$$$ Copyright AyamiKaze 2020 $$$$\n";
	unsigned int FileSize;
	char FileHash[BLAKE2b::DIGESTSIZE];

	char AES_ENC_KEY[AES::MAX_KEYLENGTH];
}AymFileHeader_t;
AymFileHeader_t header;

void EncHelper(const char* Buf, char** OutBuf, const char* Key, int OriLen, const char* Salt = header.AES_ENC_IV)
{
	try
	{
		GCM< AES >::Encryption enc;
		enc.SetKeyWithIV((byte*)Key, AES::MAX_KEYLENGTH, (byte*)Salt, 37);
		enc.ProcessData((byte*)*OutBuf, (byte*)Buf, OriLen);
	}
	catch (const CryptoPP::Exception& e)
	{
		std::cerr << e.what() << std::endl;
		system("pause");
		exit(1);
	}

}

void DecHelper(const char* Buf, char** OutBuf, const char* Key, int OriLen, const char* Salt)
{
	try
	{
		GCM< AES >::Decryption dec;
		dec.SetKeyWithIV((byte*)Key, AES::MAX_KEYLENGTH, (byte*)Salt, 37);
		dec.ProcessData((byte*)*OutBuf, (byte*)Buf, OriLen);
	}
	catch (const CryptoPP::Exception& e)
	{
		std::cerr << e.what() << std::endl;
		system("pause");
		exit(1);
	}

}
#define unpack

#ifdef pack
int main(int argc, char* argv[])
{
	char* FileName = argv[1];
	auto fp = fopen(FileName, "rb");
	fseek(fp, 0, SEEK_END);
	DWORD size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	byte* buff = (byte*)malloc(size);
	if (!buff)
		return E("No Buff");
	fread(buff, size, 1, fp);
	fclose(fp);
	header.FileSize = size;
	cout << "------------------------------" << FileName << "------------------------------" << endl;

	//Key gen.
	
	AutoSeededRandomPool prng;
	HexEncoder encoder(new FileSink(cout));
	SecByteBlock key(AES::MAX_KEYLENGTH);
	prng.GenerateBlock(key, key.size());

	cout << "Key: ";
	encoder.Put((const byte*)key.data(), key.size());
	encoder.MessageEnd();
	cout << endl;

	char* OutHash = (char*)malloc(BLAKE2b::DIGESTSIZE);

	//Encrypt FileBuff

	char* outbuf = (char*)malloc(size);
	EncHelper((char*)buff, &outbuf, (char*)key.data(), size);

	//Key Encrypt

	for (int i = 0; i < AES::MAX_KEYLENGTH; i++)
	{
		key.data()[i] ^= 0xFF;
		key.data()[i] += 0x01;
	}
	memcpy(header.AES_ENC_KEY, key.data(), AES::MAX_KEYLENGTH);

	//Gen Hash

	GenBLAKE2(buff, size, header.AES_ENC_KEY, &OutHash);
	cout << "Hash:" << (unsigned long long)OutHash << endl;
	memcpy(header.FileHash, OutHash, BLAKE2b::DIGESTSIZE);

	//Outfile Writer

	char ofnm[MAX_PATH];

	strcpy(ofnm, FileName);
	strcat(ofnm, ".cpt");

	//Write

	fp = fopen(ofnm, "wb");
	fwrite(&header, sizeof(header), 1, fp);
	fwrite(outbuf, size, 1, fp);
	fclose(fp);


	cout << "------------------------------" << FileName << "------------------------------" << endl;
	free(outbuf);
	free(OutHash);
	free(buff);
	system("pause");
	return 0;
}
#else
int main(int argc, char* argv[])
{
	char* FileName = argv[1];
	cout << "------------------------------" << FileName << "------------------------------" << endl;
	auto fp = fopen(FileName, "rb");
	fseek(fp, 0, SEEK_END);
	DWORD size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	fread(&header, sizeof(header), 1, fp);

	//Chrck FileHeader

	if (strncmp(header.Magic, ">$CPK::NATIVE::FILE$<\n", 23))
		return E("Not Magic");
	if (strncmp(header.AES_ENC_IV, "\n$$$$ Copyright AyamiKaze 2020 $$$$\n", 37))
		return E("Not IV");
	//Init AES key

	char AES_ENC_KEY[AES::MAX_KEYLENGTH];
	memcpy(AES_ENC_KEY, header.AES_ENC_KEY, AES::MAX_KEYLENGTH);
	for (int i = 0; i < AES::MAX_KEYLENGTH; i++)
	{
		header.AES_ENC_KEY[i] -= 0x01;
		header.AES_ENC_KEY[i] ^= 0xFF;
	}

	//Decrypt FileBuffer

	fseek(fp, sizeof(header), SEEK_SET);
	char* buff = (char*)malloc(header.FileSize);
	char* outbuf = (char*)malloc(header.FileSize);
	fread(buff, size, 1, fp);
	DecHelper((char*)buff, &outbuf, header.AES_ENC_KEY, header.FileSize, header.AES_ENC_IV);

	//Check Hash

	char* OutHash = (char*)malloc(BLAKE2b::DIGESTSIZE);
	GenBLAKE2((byte*)outbuf, header.FileSize, AES_ENC_KEY, &OutHash);
	cout << "FileHash:" << (unsigned long long)OutHash << endl;
	cout << "InfoHash:" << (unsigned long long)header.FileHash << endl;
	if (memcmp(OutHash, header.FileHash, BLAKE2b::DIGESTSIZE))
		return E("Hash Error");
	fclose(fp);

	//FileWrite

	char ofnm[MAX_PATH];

	strcpy(ofnm, FileName);
	strcat(ofnm, ".dec");
	fp = fopen(ofnm, "wb");
	fwrite(outbuf, header.FileSize, 1, fp);
	fclose(fp);
	cout << "------------------------------" << FileName << "------------------------------" << endl;
	free(outbuf);
	free(buff);
	free(OutHash);
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
