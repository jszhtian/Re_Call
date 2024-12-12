#include "TextUtil.h"

ReadTextFromAymScript::ReadTextFromAymScript()
{
    fp = NULL;
    EntrySize = NULL;
    TextSize = NULL;
    EntryBuff = NULL;
    TextBuff = NULL;
}

ReadTextFromAymScript::~ReadTextFromAymScript()
{
    fp = NULL;
    EntrySize = NULL;
    TextSize = NULL;
    EntryBuff = NULL;
    TextBuff = NULL;
}

unsigned long long ReadTextFromAymScript::MurmurHash64B(const void* key, int len)
{
    unsigned int seed = 0x114514;
    const unsigned int m = 0x5bd1e995;
    const int r = 24;

    unsigned int h1 = seed ^ len;
    unsigned int h2 = 0;

    const unsigned int* data = (const unsigned int*)key;

    while (len >= 8)
    {
        unsigned int k1 = *data++;
        k1 *= m; k1 ^= k1 >> r; k1 *= m;
        h1 *= m; h1 ^= k1;
        len -= 4;

        unsigned int k2 = *data++;
        k2 *= m; k2 ^= k2 >> r; k2 *= m;
        h2 *= m; h2 ^= k2;
        len -= 4;
    }

    if (len >= 4)
    {
        unsigned int k1 = *data++;
        k1 *= m; k1 ^= k1 >> r; k1 *= m;
        h1 *= m; h1 ^= k1;
        len -= 4;
    }

    switch (len)
    {
    case 3: h2 ^= ((unsigned char*)data)[2] << 16;
    case 2: h2 ^= ((unsigned char*)data)[1] << 8;
    case 1: h2 ^= ((unsigned char*)data)[0];
        h2 *= m;
    };

    h1 ^= h2 >> 18; h1 *= m;
    h2 ^= h1 >> 22; h2 *= m;
    h1 ^= h2 >> 17; h1 *= m;
    h2 ^= h1 >> 19; h2 *= m;

    unsigned long long h = h1;

    h = (h << 32) | h2;

    return h;
}

pair<WCHAR*, unsigned int> ReadTextFromAymScript::GetTextByOff(unsigned int off)
{
    auto itr = AymScriptList.find(off);
    if (itr != AymScriptList.end())
    {
        return pair<WCHAR*, unsigned int>(itr->second.txt, itr->second.sz);
    }
    return pair<WCHAR*, unsigned int>(NULL, NULL);
}

void ReadTextFromAymScript::AymGetFileSize(FILE* fp)
{
    fseek(fp, 0, SEEK_END);
    FileSize = ftell(fp);
    fseek(fp, 0, SEEK_SET);
}

bool ReadTextFromAymScript::InitAymScript()
{
    fp = fopen("Script.bin", "rb");
    if (!fp)
    {
        MessageBox(0, L"Can't Read Script.aym", 0, 0);
        return false;
    }

    AymGetFileSize(fp);

    // Read Header
    fread(&header, sizeof(bin_text_header), 1, fp);

    // Check Header Info
    if (strcmp(header.flag, "AyamiKazeFormatScriptVer1.00"))
    {
        MessageBox(0, L"header.flag not match", 0, 0);
        return false;
    }

    // Read HeaderBuff
    char* HeaderBuff = (char*)malloc(header.HeaderSize);
    if (!HeaderBuff)
    {
        MessageBox(0, L"Alloc HeaderBuff Error", 0, 0);
        return false;
    }
    fread(HeaderBuff, header.HeaderSize, 1, fp);

    // Decrypt HeraderBuff
    for (int i = 0; i < header.HeaderSize; i++)
    {
        HeaderBuff[i] ^= header.HeaderKey[i % XOR_KEY_LEN];
        HeaderBuff[i] -= 0x20;
    }

    // Get aym_header info
    aym_header* aym_hdr = (aym_header*)HeaderBuff;

    // Check aym_hdr info
    if (MurmurHash64B(HeaderBuff, header.HeaderSize) != header.HeaderHash)
    {
        MessageBox(0, L"HeaderBuff hash not match", 0, 0);
        return false;
    }
    if (strcmp(aym_hdr->flag, "AyamiKazeScriptVer1.00"))
    {
        MessageBox(0, L"aym_hdr->flag not match", 0, 0);
        return false;
    }

    // Read EntryBuff
    unsigned char* EntryCompressBuff = (unsigned char*)malloc(header.EntryCompSize);
    if (!EntryCompressBuff)
    {
        MessageBox(0, L"Alloc EntryCompressBuff Error", 0, 0);
        return false;
    }
    fread(EntryCompressBuff, header.EntryCompSize, 1, fp);

    // Uncompress EntryBuff
    EntryBuff = (unsigned char*)malloc(header.EntrySize);
    if (!EntryBuff)
    {
        MessageBox(0, L"Alloc EntryBuff Error", 0, 0);
        return false;
    }
    if (uncompress(EntryBuff, &header.EntrySize, EntryCompressBuff, header.EntryCompSize) != Z_OK)
    {
        MessageBox(0, L"Uncompress EntryBuff Error",0,0);
        return false;
    }
    free(EntryCompressBuff);

    // Check Uncompress EntryBuff
    if (MurmurHash64B(EntryBuff, header.EntrySize) != header.EntryHash)
    {
        MessageBox(0, L"EntryBuff hash not match", 0, 0);
        return false;
    }

    // Read TextBuff
    unsigned char* TextCompressBuff = (unsigned char*)malloc(header.TextCompSize);
    if (!TextCompressBuff)
    {
        MessageBox(0, L"Alloc TextCompressBuff Error", 0, 0);
        return false;
    }
    fread(TextCompressBuff, header.TextCompSize, 1, fp);
    fclose(fp);

    // Uncompress TextBuff
    TextBuff = (unsigned char*)malloc(header.TextSize);
    if (!TextBuff)
    {
        MessageBox(0, L"Alloc TextBuff Error", 0, 0);
        return false;
    }
    unsigned int err_code = ZSTD_isError(ZSTD_decompress(TextBuff, header.TextSize, TextCompressBuff, header.TextCompSize));
    if (err_code != ZSTD_error_no_error)
    {
        WCHAR err_msg[MAX_PATH];
        wsprintf(err_msg, L"Decompress TextBuff Error:%d", err_code);
        MessageBox(0, err_msg, 0, 0);
        return false;
    }
    free(TextCompressBuff);

    // Check Uncompressed TextBuff
    if (MurmurHash64B(TextBuff, header.TextSize) != header.TextHash)
    {
        MessageBox(0, L"TextBuff hash not match", 0, 0);
        return false;
    }

    // Map Text
    aym_text_struct* entry_buff = (aym_text_struct*)EntryBuff;
    for (unsigned int i = 0; i < aym_hdr->str_count; i++)
    {
        t_buf.sz = entry_buff[i].sz;
        t_buf.txt = (WCHAR*)(TextBuff + entry_buff[i].jmp);
        AymScriptList.insert(pair<unsigned int, aym_text_buff>(entry_buff[i].off, t_buf));
    }

    return true;
}