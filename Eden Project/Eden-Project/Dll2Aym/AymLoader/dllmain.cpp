// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "framework.h"

BYTE rol(int val, int n)
{
    n %= 8;
    return (val << n) | (val >> (8 - n));
}

BYTE ror(int val, int n)
{
    n %= 8;
    return (val >> n) | (val << (8 - n));
}

void Decrypt(BYTE* data, DWORD index, DWORD length, DWORD key)
{
    while (length > 0)
    {
        BYTE v = data[index];
        data[index++] = (rol(v, 1) ^ key);
        key += length--;
    }
}

void Encrypt(BYTE* data, DWORD index, DWORD length, DWORD key)
{
    while (length > 0)
    {
        BYTE v = data[index] ^ key;
        data[index++] = ror(v, 1);
        key += length--;
    }
}


BOOL E(const char* msg)
{
    MessageBoxA(0, msg, 0, 0);
    return FALSE;
}

BOOL WINAPI Init()
{
    //char* FileName = (char*)"saclet_cn.dll.aym";//argv[1];
    FILE* fp = fopen(AYM_NAME, "rb");
    if (!fp)
        return E("MissFile");
    fseek(fp, 0, SEEK_SET);
    BYTE* HeaderBuff = new BYTE[sizeof(AYMHeader)];
    fread(HeaderBuff, sizeof(AYMHeader), 1, fp);
    Decrypt(HeaderBuff, 0, sizeof(AYMHeader), KEY);
    AYMHeader* Header = (AYMHeader*)HeaderBuff;
    if (memcmp(Header->Magic, MAGIC, 4))
        return E("Magic not match");

    //cout << "FileSize:0x" << hex << Header->FileSize << endl;
    //cout << "CompressSize:0x" << hex << Header->CompressSize << endl;

    fseek(fp, sizeof(AYMHeader), SEEK_SET);
    BYTE* buff = new BYTE[Header->CompressSize];
    fread(buff, Header->CompressSize, 1, fp);
    fclose(fp);

    BYTE* fstBuff = new BYTE[Header->CompressSize];
    try
    {
        HC256::Decryption dec;
        dec.SetKeyWithIV(Header->KeyInfo.key2.key, HC256::DEFAULT_KEYLENGTH,
            Header->KeyInfo.key2.iv, 32);
        dec.ProcessData(fstBuff, buff, Header->CompressSize);
    }
    catch (const Exception& e)
    {
        return E(e.what());
    }
    delete[] buff;

    for (int i = 0; i < Header->CompressSize; i++)
        fstBuff[i] ^= Header->HashCode.HashCode2[i % BLAKE2b::DIGESTSIZE];

    BYTE* BlakeHash = new BYTE[BLAKE2b::DIGESTSIZE];
    BLAKE2b hash;
    hash.Update(Header->KeyInfo.key2.key, HC256::DEFAULT_KEYLENGTH);
    hash.Update(fstBuff, Header->CompressSize);
    hash.Final(BlakeHash);
    if (memcmp(Header->HashCode.HashCode2, BlakeHash, BLAKE2b::DIGESTSIZE))
        return E("fst hash err");

    BYTE* secBuff = new BYTE[Header->CompressSize];
    try
    {
        EAX< Twofish >::Decryption dec;
        dec.SetKeyWithIV(Header->KeyInfo.key1.key, Twofish::DEFAULT_KEYLENGTH,
            Header->KeyInfo.key1.iv, Twofish::BLOCKSIZE);
        dec.ProcessData(secBuff, fstBuff, Header->CompressSize);
    }
    catch (const Exception& e)
    {
        return E(e.what());
    }
    delete[] fstBuff;

    for (int i = 0; i < Header->CompressSize; i++)
        secBuff[i] ^= Header->HashCode.HashCode1[i % SM3::DIGESTSIZE];

    BYTE* UncompressBuff = new BYTE[Header->FileSize];
    try
    {
        ZlibDecompressor dec;
        dec.Put(secBuff, Header->CompressSize);
        dec.MessageEnd();
        dec.Get(UncompressBuff, Header->FileSize);
    }
    catch (const Exception& e)
    {
        return E(e.what());
    }
    delete[] secBuff;

    HMODULE hModule = PeLoader_LoadLibrary(UncompressBuff, Header->FileSize, 0, NULL, NULL);
    if (!hModule)
        return E("Can't load aym");

    return TRUE;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        if (!Init())
            ExitProcess(-1);
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

__declspec(dllexport)void WINAPI CreateByAyamiKaze()
{
}