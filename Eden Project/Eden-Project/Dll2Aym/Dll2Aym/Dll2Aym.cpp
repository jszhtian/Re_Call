// Dll2Aym.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "Native.h"

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

#ifdef to_aym
int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        MessageBox(0, L"Dll2Aym.exe <input Dll file>", L"AyamiKaze", 0);
        //return -1;
    }
    char* FileName = (char*)"saclet_cn.dll";//argv[1];
    FILE* fp = fopen(FileName, "rb");
    fseek(fp, 0, SEEK_END);
    DWORD size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    BYTE* buff = new BYTE[size];
    fread(buff, size, 1, fp);
    fclose(fp);

    ZlibCompressor cmp;
    cmp.Put(buff, size);
    cmp.MessageEnd();
    DWORD CompressSize = cmp.MaxRetrievable();
    BYTE* CompressBuff = new BYTE[CompressSize];
    cmp.Get(CompressBuff, CompressSize);
    delete[] buff;

    AutoSeededRandomPool TwofishPrng;
    SecByteBlock TwofishKey(Twofish::DEFAULT_KEYLENGTH);
    TwofishPrng.GenerateBlock(TwofishKey, TwofishKey.size());
    BYTE Twofishiv[Twofish::BLOCKSIZE];
    TwofishPrng.GenerateBlock(Twofishiv, sizeof(Twofishiv));

    BYTE* SM3Hash = new BYTE[SM3::DIGESTSIZE];
    SM3 SM3_hash;
    SM3_hash.Update(TwofishKey.data(), TwofishKey.size());
    SM3_hash.Update(CompressBuff, CompressSize);
    SM3_hash.Final(SM3Hash);
    
    for (int i = 0; i < CompressSize; i++)
        CompressBuff[i] ^= SM3Hash[i % SM3::DIGESTSIZE];

    BYTE* fstCryptBuff = new BYTE[CompressSize];
    try 
    {
        EAX< Twofish >::Encryption T_e;
        T_e.SetKeyWithIV(TwofishKey.data(), TwofishKey.size(),
            Twofishiv, sizeof(Twofishiv));
        T_e.ProcessData(fstCryptBuff, CompressBuff, CompressSize);
    }
    catch (const Exception& e)
    {
        std::cerr << e.what() << std::endl;
        return E("Twofish Encrypt Error");
    }
    delete[] CompressBuff;

    AutoSeededRandomPool HC256Prng;
    SecByteBlock HC256Key(HC256::DEFAULT_KEYLENGTH);
    HC256Prng.GenerateBlock(HC256Key, HC256Key.size());
    SecByteBlock HC256iv(32);
    HC256Prng.GenerateBlock(HC256iv, HC256iv.size());

    BYTE* BlakeHash = new BYTE[BLAKE2b::DIGESTSIZE];
    BLAKE2b BLAKE_hash;
    BLAKE_hash.Update(HC256Key.data(), HC256Key.size());
    BLAKE_hash.Update(fstCryptBuff, CompressSize);
    BLAKE_hash.Final(BlakeHash);

    for (int i = 0; i < CompressSize; i++)
        fstCryptBuff[i] ^= BlakeHash[i % BLAKE2b::DIGESTSIZE];

    BYTE* secCryptBuff = new BYTE[CompressSize];
    try
    {
        HC256::Encryption H_e;
        H_e.SetKeyWithIV(HC256Key.data(), HC256Key.size(),
            HC256iv.data(), HC256iv.size());
        H_e.ProcessData(secCryptBuff, fstCryptBuff, CompressSize);
    }
    catch (const Exception& e)
    {
        std::cerr << e.what() << std::endl;
        return E("HC256 Encrypt Error");
    }
    delete[] fstCryptBuff;

    AYMHeader Header;
    memcpy(Header.Magic, MAGIC, 4);
    Header.FileSize = size;
    Header.CompressSize = CompressSize;
    memcpy(Header.KeyInfo.key1.key, TwofishKey.data(), Twofish::DEFAULT_KEYLENGTH);
    memcpy(Header.KeyInfo.key1.iv, Twofishiv, Twofish::BLOCKSIZE);
    memcpy(Header.KeyInfo.key2.key, HC256Key.data(), HC256::DEFAULT_KEYLENGTH);
    memcpy(Header.KeyInfo.key2.iv, HC256iv.data(), 32);
    memcpy(Header.HashCode.HashCode1, SM3Hash, SM3::DIGESTSIZE);
    memcpy(Header.HashCode.HashCode2, BlakeHash, BLAKE2b::DIGESTSIZE);

    BYTE* HeaderBuff = new BYTE[sizeof(AYMHeader)];
    memmove(HeaderBuff, &Header, sizeof(AYMHeader));
    Encrypt(HeaderBuff, 0, sizeof(AYMHeader), KEY);

    char fnm[MAX_PATH] = { 0 };
    strcpy(fnm, FileName);
    strcat(fnm, ".aym");
    
    fp = fopen(fnm, "wb");
    fwrite(HeaderBuff, sizeof(AYMHeader), 1, fp);
    fwrite(secCryptBuff, CompressSize, 1, fp);
    fclose(fp);
    delete[] HeaderBuff;
    //cout << hex << sizeof(AYMHeader) << endl;
    system("pause");
    return 0;
}
#else
int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        MessageBox(0, L"Dll2Aym.exe <input Dll file>", L"AyamiKaze", 0);
        //return -1;
    }
    char* FileName = (char*)"saclet_cn.dll.aym";//argv[1];
    FILE* fp = fopen(FileName, "rb");
    fseek(fp, 0, SEEK_SET);
    BYTE* HeaderBuff = new BYTE[sizeof(AYMHeader)];
    fread(HeaderBuff, sizeof(AYMHeader), 1, fp);
    Decrypt(HeaderBuff, 0, sizeof(AYMHeader), KEY);
    AYMHeader* Header = (AYMHeader*)HeaderBuff;
    if (memcmp(Header->Magic, MAGIC, 4))
        return E("Magic not match");

    cout << "FileSize:0x" << hex << Header->FileSize << endl;
    cout << "CompressSize:0x" << hex << Header->CompressSize << endl;

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
        std::cerr << e.what() << std::endl;
        return E("fst Decrypt Error");
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
        std::cerr << e.what() << std::endl;
        return E("sec Decrypt Error");
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
        std::cerr << e.what() << std::endl;
        return E("Uncompress Error");
    }
    delete[] secBuff;

    char fnm[MAX_PATH] = { 0 };
    strcpy(fnm, FileName);
    strcat(fnm, ".dll");

    fp = fopen(fnm, "wb");
    fwrite(UncompressBuff, Header->FileSize, 1, fp);
    fclose(fp);

    delete[] UncompressBuff;
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
