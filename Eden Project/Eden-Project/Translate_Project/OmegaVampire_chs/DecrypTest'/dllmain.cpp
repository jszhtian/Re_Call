// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "framework.h"

struct LIBPHDR {
    unsigned char signature[4]; // "LIBP"
    unsigned long entry1_count;
    unsigned long entry2_count;
    unsigned long unknown1;
};

struct LIBPENTRY1 {
    char          filename[20];
    unsigned long flags;
    unsigned long offset_index;
    unsigned long length;
};

struct LIBPENTRY2 {
    unsigned long offset; // in 4096 byte blocks (or 1024 byte blocks...)
};

template<class T> T __ROL__(T value, int count)
{
    const unsigned int nbits = sizeof(T) * 8;

    if (count > 0)
    {
        count %= nbits;
        T high = value >> (nbits - count);
        if (T(-1) < 0) // signed value
            high &= ~((T(-1) << count));
        value <<= count;
        value |= high;
    }
    else
    {
        count = -count % nbits;
        T low = value << (nbits - count);
        value >>= count;
        value |= low;
    }
    return value;
}


inline unsigned int __ROL4__(unsigned int value, int count) { return __ROL__((unsigned int)value, count); }

inline unsigned int __ROR4__(unsigned int value, int count) { return __ROL__((unsigned int)value, -count); }


typedef int (__cdecl* Func_Camellia_Ekeygen)(int a1, unsigned char* a2, unsigned int* a3);
PVOID p_Camellia_Ekeygen;

typedef int(__cdecl* Func_Camellia_DecryptBlock)(const int a1, const unsigned char* a2, const KEY_TABLE_TYPE a3, unsigned char* a4);
PVOID p_Camellia_DecryptBlock;


DWORD base_off = 0;
void Decrypt(FILE* fp, KEY_TABLE_TYPE kt, void* buff, DWORD size, DWORD offset)
{

    BYTE* encBuff = new BYTE[size];
    memset(encBuff, 0, size);

    DWORD block_pad = offset % CAMELLIA_BLOCK_SIZE;
    DWORD aligned_len = (size + block_pad + 15) & ~15;
    BYTE* aligned_buff = new BYTE[aligned_len];

    BYTE* block = (BYTE*)aligned_buff;
    DWORD block_count = aligned_len / CAMELLIA_BLOCK_SIZE;

    fseek(fp, offset - block_pad, SEEK_SET);
    cout << "offset:0x" << hex << offset - block_pad << endl;
    fread(aligned_buff, 1, aligned_len, fp);


    for (int i = 0; i < size / CAMELLIA_BLOCK_SIZE; i++)
    {
        DWORD* block_DWORD = (DWORD*)block;
        unsigned int roll_bits = ((offset >> 4) & 0xF) + 16;

        block_DWORD[0] = __ROL4__(block_DWORD[0], roll_bits);
        block_DWORD[1] = __ROR4__(block_DWORD[1], roll_bits);
        block_DWORD[2] = __ROL4__(block_DWORD[2], roll_bits);
        block_DWORD[3] = __ROR4__(block_DWORD[3], roll_bits);


        (Func_Camellia_DecryptBlock(p_Camellia_DecryptBlock))(128, block, kt, block);
        block += CAMELLIA_BLOCK_SIZE;
        offset += CAMELLIA_BLOCK_SIZE;
    }

    memcpy(buff, aligned_buff + block_pad, size);
}

int Ext()
{

    p_Camellia_Ekeygen = (PVOID)0x42ADC0;
    p_Camellia_DecryptBlock = (PVOID)0x42B760;

    unsigned char key[] = {
    0x32, 0x4A, 0x45, 0x56,
    0x7E, 0x66, 0x43, 0x5D,
    0x2C, 0x29, 0x6B, 0x76,
    0x2D, 0x4C, 0x5D, 0x5A
    };

    KEY_TABLE_TYPE kt;
    memset(kt, 0, sizeof(kt));
    (Func_Camellia_Ekeygen(p_Camellia_Ekeygen))(128, key, kt);

    FILE* fkey = fopen("key.bin", "wb");
    fwrite(kt, sizeof(kt), 1, fkey);
    fclose(fkey);


    FILE* fp = fopen("old.dat", "rb");


    LIBPHDR* hdrBuff = new LIBPHDR[sizeof(LIBPHDR)];
    Decrypt(fp, kt, hdrBuff, sizeof(LIBPHDR), base_off);

    FILE* dmp = fopen("hdr.bin", "wb");
    fwrite(hdrBuff, sizeof(LIBPHDR), 1, dmp);
    fclose(dmp);

    cout << "out_off:0x" << hex << base_off << endl;
    base_off += sizeof(LIBPHDR);

    LIBPENTRY1* idx1 = new LIBPENTRY1[hdrBuff->entry1_count];
    Decrypt(fp, kt, idx1, sizeof(LIBPENTRY1) * hdrBuff->entry1_count, base_off);

    dmp = fopen("index1.bin", "wb");
    fwrite(idx1, sizeof(LIBPENTRY1) * hdrBuff->entry1_count, 1, dmp);
    fclose(dmp);

    cout << "out_off:0x" << base_off << endl;
    base_off += sizeof(LIBPENTRY1) * hdrBuff->entry1_count;

    LIBPENTRY2* idx2 = new LIBPENTRY2[hdrBuff->entry2_count];
    Decrypt(fp, kt, idx2, sizeof(LIBPENTRY2) * hdrBuff->entry2_count, base_off);

    dmp = fopen("index2.bin", "wb");
    fwrite(idx2, sizeof(LIBPENTRY2) * hdrBuff->entry2_count, 1, dmp);
    fclose(dmp);

    cout << "out_off:0x" << base_off << endl;
    base_off += sizeof(LIBPENTRY2) * hdrBuff->entry2_count;

    base_off = (base_off + 1023) & ~1023;

    //ProcD(fp, idx1, idx2, kt, 0, 1);


    fclose(fp);

    system("pause");
    return 0;

}

static void make_console()
{
    AllocConsole();
    freopen("CONOUT$", "w", stdout);
    freopen("CONIN$", "r", stdin);
    cout << "Console\n" << endl;
}


bool IsOnce = false;
PVOID g_pOldGetCurrentDirectoryW = GetCurrentDirectoryW;
typedef DWORD(WINAPI* PfuncGetCurrentDirectoryW)(DWORD nBufferLength, LPTSTR lpBuffer);
DWORD WINAPI NewGetCurrentDirectoryW(DWORD nBufferLength, LPTSTR lpBuffer)
{
    if (!IsOnce)
    {
        Ext();
        IsOnce = true;
    }
    return ((PfuncGetCurrentDirectoryW)g_pOldGetCurrentDirectoryW)(nBufferLength, lpBuffer);
}

void Init()
{
    DetourTransactionBegin();
    DetourAttach(&g_pOldGetCurrentDirectoryW, NewGetCurrentDirectoryW);
    if (DetourTransactionCommit() != NOERROR)
    {
        MessageBox(NULL, L"Init hook error", L"Init", MB_OK | MB_ICONERROR);
        ExitProcess(-1);
    }
}



BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        LoadLibraryW(L"kDays.dll");
        make_console();
        Init();
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

