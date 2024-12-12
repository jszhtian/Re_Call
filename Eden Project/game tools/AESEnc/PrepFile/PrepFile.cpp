// PrepFile.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "..\3rdLib\cryptopp\sha3.h"
#include "..\3rdLib\cryptopp\gcm.h"
#include "..\3rdLib\cryptopp\aes.h"
#include "..\3rdLib\cryptopp\cryptlib.h"
#include "..\3rdLib\cryptopp\hex.h"
#include "..\3rdLib\cryptopp\filters.h"
#include "..\3rdLib\cryptopp\files.h"
#include <sstream>
#include <fstream>
#include <Windows.h>
#include <fcntl.h>
#include <io.h>
#include <stdio.h>

#ifdef _DEBUG 
#pragma comment(lib, "../3rdLib/cryptlib_Debug.lib") 
#else
#pragma comment(lib, "../3rdLib/cryptlib_Release.lib") 
#endif
using namespace CryptoPP;
using namespace std;
static char salt[] = "Starbucks and fluttering moths";
static char key[] = "Yayoitsuki Hanhuazu 2020";
const int TAG_SIZE = 16;

template<class T>
void FileNameToLower(T& FileName) 
{
    std::transform(FileName.begin(), FileName.end(), FileName.begin(), ::tolower);
}

int wmain(int argc, wchar_t *argv[])
{
    wstring filename(argv[1]);
    FileNameToLower(filename);
    _setmode(_fileno(stdout), _O_U16TEXT);
    if (argc == 1)
        return -1;
    std::wcout << L"Processing!\n";
    //Gen Hash with salt
    string display;
    try
    {
        SHA3_256 hash;
        hash.Update(reinterpret_cast<byte*>(salt), strlen(salt));
        hash.Update((byte*)filename.c_str(), filename.length() * sizeof(wchar_t));
        auto digest = new char[hash.DigestSize()];
        hash.Final(reinterpret_cast<byte*>(&digest[0]));
        StringSource(reinterpret_cast<byte*>(&digest[0]), hash.DigestSize(), true, new HexEncoder(new StringSink(display)));
        auto wdisplay = new wchar_t[display.length()+1];
        memset(wdisplay, 0, sizeof(wchar_t) * (display.length() + 1));
        mbstowcs(wdisplay, display.c_str(), display.length());
        std::wcout << L"HASH Done!" << endl;
        std::wcout << filename << "|" << wdisplay << endl;
        delete[] wdisplay;
    }
    catch (CryptoPP::Exception & e)
    {
        cerr << "Caught Exception..." << endl;
        cerr << e.what() << endl;
        cerr << endl;
        return -1;
    }
    
    //AES Enc
    unsigned char AESkey[32];
    memset(AESkey, 0xFE, sizeof(AESkey));
    unsigned char iv[12];
    memset(iv, 0, sizeof(iv));
    memcpy(AESkey, key, strlen(key));
    memcpy(iv, salt, sizeof(iv));
    try
    {
        GCM< AES >::Encryption e;
        e.SetKeyWithIV(AESkey, sizeof(AESkey), iv, sizeof(iv));
        FileSource(argv[1],true, new AuthenticatedEncryptionFilter(e,
            new FileSink(display.c_str(),true), false, TAG_SIZE
        ));
    }
    catch (CryptoPP::Exception & e)
    {
        cerr << "Caught Exception..." << endl;
        cerr << e.what() << endl;
        cerr << endl;
        return -1;
    }
    return 0;
}