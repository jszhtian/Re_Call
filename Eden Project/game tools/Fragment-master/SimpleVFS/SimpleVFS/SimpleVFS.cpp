#include "pch.h"
#include "SimpleVFS.h"

void GenSHA256(const char* Str,char* Salt,char** SHAContent)
{
	CryptoPP::SHA256 hash;
	hash.Update((byte*)Str, strlen(Str));
	hash.Update((byte*)Salt, CryptoPP::Salsa20::IV_LENGTH);
	hash.Final((byte*)*SHAContent);
}

void DecHelper(const char* Buf, char** OutBuf, const char* Key, const char* Salt, int OriLen)
{
	CryptoPP::Salsa20::Decryption dec;
	dec.SetKeyWithIV((byte*)Key, CryptoPP::Salsa20::MAX_KEYLENGTH, (byte*)Salt, CryptoPP::Salsa20::IV_LENGTH);
	dec.ProcessData((byte*)*OutBuf, (byte*)Buf, OriLen);
}

void inflatHelper(const char* Buf, uint32_t oriLen, uint32_t compLen, char** OutBuf)
{
	CryptoPP::ZlibDecompressor dec;
	dec.Put((byte*)Buf, compLen);
	dec.MessageEnd();
	dec.Get((byte*)*OutBuf, (size_t)oriLen);
}

SimpleVFS::SimpleVFS()
{
}

bool SimpleVFS::Mount(const char * IdxFile,const char * Res)
{
	std::fstream fileIdx;
	fileIdx.open(IdxFile, std::ios::binary| std::fstream::in);
	fileIdx.seekg(0, std::ios::end);
	DWORD IdxLength = fileIdx.tellg();
	fileIdx.seekg(0, std::ios::beg);
	VFSIdxBuffer = new char[IdxLength];
	fileIdx.read(VFSIdxBuffer, IdxLength);
	fileIdx.close();
	VFSIdxHeader* Hdr = (VFSIdxHeader*)VFSIdxBuffer;
	char Magic[] = "PACK";
	if (memcmp(Hdr->Magic, Magic, 4) != 0)
	{
		return false;
	}
	VFSEntryCount = Hdr->EntryCount;
	char* tmpPtr = VFSIdxBuffer + sizeof(VFSIdxHeader);
	VFSIndexList = (VFSIndex*)tmpPtr;
	VFSRes.open(Res, std::ios::binary | std::fstream::in);
	return true;
}


void SimpleVFS::Unmount()
{
	delete[] VFSIdxBuffer;
	VFSIndexList = nullptr;
	memset(EncKey, 0, CryptoPP::Salsa20::MAX_KEYLENGTH);
	memset(salt, 0, CryptoPP::Salsa20::IV_LENGTH);
}

HANDLE SimpleVFS::GetFileHandleByName(const char * FileName)
{
	char* SHA256Content = new char[CryptoPP::SHA256::DIGESTSIZE];
	GenSHA256(FileName, salt, &SHA256Content);
	HANDLE ret = INVALID_HANDLE_VALUE;
	for (uint32_t i = 0; i < VFSEntryCount; i++)
	{
		if (memcmp(VFSIndexList[i].FileName_SHA256, SHA256Content, CryptoPP::SHA256::DIGESTSIZE) == 0)
		{
			char* RawBuf = new char[VFSIndexList[i].pck_size];
			VFSRes.seekg(VFSIndexList[i].offset, std::ios::beg);
			VFSRes.read(RawBuf, VFSIndexList[i].pck_size);
			char* OutBuf = nullptr;
			char* TMPBuf = nullptr;
			char* TMPBuf2 = nullptr;
			int Mode = 0;
			if (VFSIndexList[i].encrypt_flag&&VFSIndexList[i].packed_flag) Mode = 3;
			if (!VFSIndexList[i].encrypt_flag&&VFSIndexList[i].packed_flag) Mode = 2;
			if (VFSIndexList[i].encrypt_flag&&!VFSIndexList[i].packed_flag) Mode = 1;
			switch (Mode)
			{
			case 1:
				TMPBuf= new char[VFSIndexList[i].pck_size];
				DecHelper(RawBuf, &TMPBuf, EncKey, salt, VFSIndexList[i].pck_size);
				OutBuf = TMPBuf;
				break;
			case 2:
				TMPBuf = new char[VFSIndexList[i].orig_size];
				inflatHelper(RawBuf, VFSIndexList[i].orig_size, VFSIndexList[i].pck_size, &TMPBuf);
				OutBuf = TMPBuf;
				break;
			case 3:
				TMPBuf = new char[VFSIndexList[i].pck_size];
				TMPBuf2 = new char[VFSIndexList[i].orig_size];
				DecHelper(RawBuf, &TMPBuf, EncKey, salt, VFSIndexList[i].pck_size);
				inflatHelper(TMPBuf, VFSIndexList[i].orig_size, VFSIndexList[i].pck_size, &TMPBuf2);
				delete[] TMPBuf;
				OutBuf = TMPBuf2;
				break;
			default:
				OutBuf= new char[VFSIndexList[i].pck_size];
				memcpy(RawBuf, OutBuf, VFSIndexList[i].pck_size);
				break;
			}
			delete[] RawBuf;
			if (OutBuf == nullptr)
			{
				ret = INVALID_HANDLE_VALUE;
				return ret;
			}
			wchar_t TMPPath[MAX_PATH];
			wchar_t TMPFileName[MAX_PATH];
			GetTempPath(MAX_PATH, TMPPath);
			GetTempFileName(TMPPath, L"VFSTMP_", 0x1000, TMPFileName);
			auto hTMPFile=CreateFileW(TMPFileName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_TEMPORARY| FILE_FLAG_DELETE_ON_CLOSE,NULL);
			DWORD dwBytesWritten;
			WriteFile(hTMPFile, OutBuf, VFSIndexList[i].orig_size, &dwBytesWritten, NULL);
			SetFilePointer(hTMPFile, 0, NULL, FILE_BEGIN);
			delete[] OutBuf;
			ret = hTMPFile;
		}
	}
	return ret;
}

char* SimpleVFS::GetFileBufferByName(const char * FileName,DWORD* FileLength)
{
	char* ret = nullptr;
	char* SHA256Content = new char[CryptoPP::SHA256::DIGESTSIZE];
	GenSHA256(FileName, salt, &SHA256Content);
	for (uint32_t i = 0; i < VFSEntryCount; i++)
	{
		if (memcmp(VFSIndexList[i].FileName_SHA256, SHA256Content, CryptoPP::SHA256::DIGESTSIZE) == 0)
		{
			char* RawBuf = new char[VFSIndexList[i].pck_size];
			VFSRes.seekg(VFSIndexList[i].offset, std::ios::beg);
			VFSRes.read(RawBuf, VFSIndexList[i].pck_size);
			char* OutBuf = nullptr;
			char* TMPBuf = nullptr;
			char* TMPBuf2 = nullptr;
			int Mode = 0;
			if (VFSIndexList[i].encrypt_flag&&VFSIndexList[i].packed_flag) Mode = 3;
			if (!VFSIndexList[i].encrypt_flag&&VFSIndexList[i].packed_flag) Mode = 2;
			if (VFSIndexList[i].encrypt_flag && !VFSIndexList[i].packed_flag) Mode = 1;
			switch (Mode)
			{
			case 1:
				TMPBuf = new char[VFSIndexList[i].pck_size];
				DecHelper(RawBuf, &TMPBuf, EncKey, salt, VFSIndexList[i].pck_size);
				OutBuf = TMPBuf;
				break;
			case 2:
				TMPBuf = new char[VFSIndexList[i].orig_size];
				inflatHelper(RawBuf, VFSIndexList[i].orig_size, VFSIndexList[i].pck_size, &TMPBuf);
				OutBuf = TMPBuf;
				break;
			case 3:
				TMPBuf = new char[VFSIndexList[i].pck_size];
				TMPBuf2 = new char[VFSIndexList[i].orig_size];
				DecHelper(RawBuf, &TMPBuf, EncKey, salt, VFSIndexList[i].pck_size);
				inflatHelper(TMPBuf, VFSIndexList[i].orig_size, VFSIndexList[i].pck_size, &TMPBuf2);
				delete[] TMPBuf;
				OutBuf = TMPBuf2;
				break;
			default:
				OutBuf = new char[VFSIndexList[i].pck_size];
				memcpy(RawBuf, OutBuf, VFSIndexList[i].pck_size);
				break;
			}
			delete[] RawBuf;
			if (OutBuf == nullptr)
			{
				ret = nullptr;
				return ret;
			}
			ret = OutBuf;
			*FileLength = VFSIndexList[i].orig_size;
		}
	}
	return ret;
}

void SimpleVFS::SetKey(char * Key, char * salt)
{
	memcpy(this->EncKey, Key, CryptoPP::Salsa20::MAX_KEYLENGTH);
	memcpy(this->salt,salt, CryptoPP::Salsa20::IV_LENGTH);
}

SimpleVFS::~SimpleVFS()
{
	delete[] VFSIdxBuffer;
	VFSIndexList = nullptr;
	VFSRes.close();
	memset(EncKey, 0, CryptoPP::Salsa20::MAX_KEYLENGTH);
	memset(salt, 0, CryptoPP::Salsa20::IV_LENGTH);
}
