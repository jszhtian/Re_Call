#pragma once

#pragma comment(lib, "..//..//3rd//cryptopp-CRYPTOPP_8_5_0//Win32//Output//Release//cryptlib_MD.lib") 


#include <iostream>
#include <Windows.h>
#include <io.h>
#include <direct.h>
#include <string>
#include <vector>
#include <algorithm>

#include "..\..\3rd\cryptopp-CRYPTOPP_8_5_0\sm3.h"
#include "..\..\3rd\cryptopp-CRYPTOPP_8_5_0\blake2.h"

#include "..\..\3rd\cryptopp-CRYPTOPP_8_5_0\eax.h"

#include "..\..\3rd\cryptopp-CRYPTOPP_8_5_0\twofish.h"
#include "..\..\3rd\cryptopp-CRYPTOPP_8_5_0\hc256.h"

#include "..\..\3rd\cryptopp-CRYPTOPP_8_5_0\zlib.h"

#include "..\..\3rd\cryptopp-CRYPTOPP_8_5_0\hex.h"
#include "..\..\3rd\cryptopp-CRYPTOPP_8_5_0\osrng.h"


using namespace CryptoPP;
using namespace std;

static int E(const char* msg)
{
	MessageBoxA(0, msg, 0, 0);
	return -1;
}

#pragma pack (1)
struct TwofishInfo {
    BYTE key[Twofish::DEFAULT_KEYLENGTH];
    BYTE iv[Twofish::BLOCKSIZE];
};
struct hc256Info {
    BYTE key[HC256::DEFAULT_KEYLENGTH];
    BYTE iv[32];
};
struct KeyNode {
    TwofishInfo key1;
    hc256Info   key2;
};
struct HashNode {
    BYTE HashCode1[SM3::DIGESTSIZE];
    BYTE HashCode2[BLAKE2b::DIGESTSIZE];
};
struct AYMHeader {
    char Magic[4]; // "AYM\0"
    DWORD FileSize;
    DWORD CompressSize;
    KeyNode KeyInfo;
    HashNode HashCode;
};
#pragma pack ()

#define MAGIC "AYM\0"
#define KEY 0x29FC7BFA
#define to_dll