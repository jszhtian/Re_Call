#include "framework.h"

CPKFileSystem::CPKFileSystem()
{
}

CPKFileSystem::~CPKFileSystem()
{
	delete[] Key_Header;
	delete[] CPK_Header;

	delete[] CPK_Index;
	delete[] Key_Index;
	
	fclose(cpk);
	fclose(key);
}

BYTE* HostAlloc(DWORD size)
{
	return new BYTE[size];
}

void HostFree(void* buff)
{
	delete[] buff;
}

BOOL CPKFileSystem::E(const char* msg)
{
	MessageBoxA(0, msg, "CPKFileSystem Error", 0);
	return FALSE;
}

BOOL CPKFileSystem::InitFileSystem()
{
	cpk = fopen(PACK_NAME, "rb");
	if (!cpk)
		return E("Can't open package.");
	key = fopen(KEY_NAME, "rb");
	if (!key)
		return E("Can't open key.");

	// Key Info Check
	KeyHeaderBuff = new BYTE[sizeof(KeyHeader)];
	fread(KeyHeaderBuff, sizeof(KeyHeader), 1, key);

	for (int i = 0; i < sizeof(KeyHeader); i++)
		KeyHeaderBuff[i] ^= KEY_TOP_KEY;

	Key_Header = (KeyHeader*)KeyHeaderBuff;
	if (memcmp(Key_Header->Magic, MAGIC, 4))
		return E("Key magic not match");

	// Decrypt CPK Header and Check CPK Info
	CPKHeaderBuff = new BYTE[sizeof(CPKHeader)];
	fread(CPKHeaderBuff, sizeof(CPKHeader), 1, cpk);

	for (int i = 0; i < sizeof(CPKHeader); i++)
		CPKHeaderBuff[i] ^= Key_Header->HeaderHash[i % SHA256::DIGESTSIZE];

	CPK_Header = (CPKHeader*)CPKHeaderBuff;
	if (memcmp(CPK_Header->Magic, MAGIC, 4) ||
		memcmp(CPK_Header->Version, VERSION, 4) ||
		lstrcmpA(CPK_Header->Warning, WARNING))
		return E("CPK magi not match");

	// Read Key Index and CPK Index
	if (CPK_Header->IndexCount != Key_Header->IndexCount)
		return E("Counter not match");
	CPK_Index = new CPKIndex[CPK_Header->IndexCount * sizeof(CPKIndex)];
	Key_Index = new KeyIndex[Key_Header->IndexCount * sizeof(KeyIndex)];
	fread(CPK_Index, CPK_Header->IndexCount * sizeof(CPKIndex), 1, cpk);
	fread(Key_Index, Key_Header->IndexCount * sizeof(KeyIndex), 1, key);

	for (int i = 0; i < CPK_Header->IndexCount; i++)
	{
		// Decrypt Key Index
		BYTE* KeyIndexBuff = new BYTE[sizeof(KeyIndex)];
		memmove(KeyIndexBuff, &Key_Index[i], sizeof(KeyIndex));
		for (int j = 0; j < sizeof(KeyIndex); j++)
			KeyIndexBuff[j] ^= KEY_TOP_KEY;
		memmove(&Key_Index[i], KeyIndexBuff, sizeof(KeyIndex));

		// Decrypt CPK Index
		BYTE* CPKIndexBuff = new BYTE[sizeof(CPKIndex)];
		memmove(CPKIndexBuff, &CPK_Index[i], sizeof(CPKIndex));
		for (int j = 0; j < sizeof(CPKIndex); j++)
			CPKIndexBuff[j] ^= Key_Index[i].IndexHash[j % SHA256::DIGESTSIZE];
		memmove(&CPK_Index[i], CPKIndexBuff, sizeof(CPKIndex));
	}

	return TRUE;
}

BOOL CPKFileSystem::UnInitFileSystem()
{
	fclose(cpk);
	fclose(key);

	delete[] Key_Header;
	delete[] CPK_Header;

	delete[] CPK_Index;
	delete[] Key_Index;

	return TRUE;
}

BOOL CPKFileSystem::GetFileByName(WCHAR* FileName, BYTE** FileBuff, DWORD* FileSize)
{
	for (int i = 0; i < CPK_Header->IndexCount; i++)
	{
		if (!lstrcmpW(CPK_Index[i].FileName, FileName))
		{
			fseek(cpk, sizeof(CPKHeader) + CPK_Header->IndexCount * sizeof(CPKIndex) + CPK_Index[i].Offset, SEEK_SET);
			BYTE* CompressBuff = HostAlloc(CPK_Index[i].CompressFileSize);
			fread(CompressBuff, CPK_Index[i].CompressFileSize, 1, cpk);

			// Camellia Decrypt
			BYTE* DecryptBuff = HostAlloc(CPK_Index[i].CompressFileSize);
			try
			{
				EAX<Camellia>::Decryption d;
				d.SetKeyWithIV(Key_Index[i].KeyAfterCompress.Key, Key_Index[i].KeyAfterCompress.KeySize, 
					Key_Index[i].KeyAfterCompress.Iv, Key_Index[i].KeyAfterCompress.IvSize);
				d.ProcessData(DecryptBuff, CompressBuff, CPK_Index[i].CompressFileSize);

			}
			catch (const CryptoPP::Exception& e)
			{
				return E(e.what());
			}
			HostFree(CompressBuff);

			// Zlib Decompress
			BYTE* UncompressBuff = HostAlloc(CPK_Index[i].FileSize);
			ZlibDecompressor dec;
			dec.Put(DecryptBuff, CPK_Index[i].CompressFileSize);
			dec.MessageEnd();
			dec.Get(UncompressBuff, CPK_Index[i].FileSize);
			HostFree(DecryptBuff);

			*FileBuff = UncompressBuff;
			*FileSize = CPK_Index[i].FileSize;

			return TRUE;
		}
	}
	return FALSE;
}