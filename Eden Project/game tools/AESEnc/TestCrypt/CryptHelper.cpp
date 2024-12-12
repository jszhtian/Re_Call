#include "CryptHelper.h"

CryptHelper::CryptHelper()
{
}

CryptHelper::~CryptHelper()
{
	this->releaseBuffer();
}

string CryptHelper::genSHA3_256(wstring filepath)
{
	try
	{
		this->lastError.clear();
		SHA3_256 hash;
		hash.Update((unsigned char*)(salt), strlen(salt));
		hash.Update((unsigned char*)filepath.c_str(), filepath.length() * sizeof(wchar_t));
		auto digest = new char[hash.DigestSize()];
		hash.Final((unsigned char*)(&digest[0]));
		string finalhash;
		StringSource((unsigned char*)(&digest[0]), hash.DigestSize(), true, new HexEncoder(new StringSink(finalhash)));
		delete[] digest;
		return finalhash;
	}
	catch (CryptoPP::Exception & e)
	{
		cerr << "Caught Exception..." << endl;
		cerr << e.what() << endl;
		cerr << endl;
		this->lastError = e.what();
		return string();
	}
}

tuple<char*, int> CryptHelper::decData(string filepath)
{
	try
	{
		this->lastError.clear();
		unsigned char AESkey[32];
		memset(AESkey, 0xFE, sizeof(AESkey));
		unsigned char iv[12];
		memset(iv, 0, sizeof(iv));
		memcpy(AESkey, key, strlen(key));
		memcpy(iv, salt, sizeof(iv));
		GCM< AES >::Decryption d;
		d.SetKeyWithIV(AESkey, sizeof(AESkey), iv, sizeof(iv));
		filesystem::path pFile(filepath);
		auto isFileExist = filesystem::exists(pFile);
		if (!isFileExist) return make_tuple<char*, int>(nullptr, -1);;
		auto fSize = filesystem::file_size(pFile);
		if (fSize == 0)throw CryptoPP::Exception(CryptoPP::Exception::ErrorType::IO_ERROR, "File size is zero");
		this->databuffer = new char[fSize];
		FileSource(filepath.c_str(), true, new AuthenticatedDecryptionFilter(d, new ArraySink((unsigned char*)this->databuffer, fSize), AuthenticatedDecryptionFilter::Flags::DEFAULT_FLAGS, TAG_SIZE));
		return make_tuple<char*, int>((char*)this->databuffer, (int)fSize);
	}
	catch (CryptoPP::Exception & e)
	{
		cerr << "Caught Exception..." << endl;
		cerr << e.what() << endl;
		cerr << endl;
		releaseBuffer();
		this->lastError = e.what();
		return make_tuple<char*,int>(nullptr,-2);
	}
	
}

string CryptHelper::getLastError()
{
	return this->lastError;
}

bool CryptHelper::releaseBuffer()
{
	try
	{
		if (databuffer!= nullptr)
		{
			delete[] databuffer;
			databuffer = nullptr;
			return true;
		}
		return false;
	}
	catch (const std::exception&)
	{
		return false;
	}
	
}
