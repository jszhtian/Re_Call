#pragma once
#include <iostream>
#include "..\3rdLib\cryptopp\sha3.h"
#include "..\3rdLib\cryptopp\gcm.h"
#include "..\3rdLib\cryptopp\aes.h"
#include "..\3rdLib\cryptopp\cryptlib.h"
#include "..\3rdLib\cryptopp\hex.h"
#include "..\3rdLib\cryptopp\filters.h"
#include "..\3rdLib\cryptopp\files.h"
#include <filesystem>
#include <tuple>

#ifdef _DEBUG 
#pragma comment(lib, "../3rdLib/cryptlib_Debug.lib") 
#else
#pragma comment(lib, "../3rdLib/cryptlib_Release.lib") 
#endif
using namespace CryptoPP;
using namespace std;
class CryptHelper
{
public:
	CryptHelper();
	~CryptHelper();
	string genSHA3_256(wstring filepath);
	tuple<char*,int> decData(string filepath);
	string getLastError();
	bool releaseBuffer();
private:
	string lastError;
	char* databuffer = nullptr;
	static constexpr const char* salt = "Starbucks and fluttering moths";
	static constexpr const char* key= "Yayoitsuki Hanhuazu 2020";
	static constexpr const int TAG_SIZE = 16;
};