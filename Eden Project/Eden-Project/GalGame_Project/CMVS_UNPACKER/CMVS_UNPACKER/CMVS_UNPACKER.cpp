// CMVS_UNPACKER.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//from crass

#include <iostream>
#include <Windows.h>
using namespace std;

char cpz3_decode_string[] = "夝愅偟偪傖偆埆偄巕偼偍巇抲偒側傫偩傛丄儊僢両侓乧扤偵尵偭偰傞偺丠";
//解析しちゃう悪い子はお仕置きなんだよ、メッ！♪…誰に言ってるの？
char cpz5_decode_string[] = "夝愅偡傞埆偄巕偼儕僐偑偍巇抲偒偟偪傖偄傑偡丅庺傢傟偪傖偄傑偡傛乣丄偲偄偆偐傕偆庺偭偪傖偄傑偟偨侓";
// "解析する悪い子はリコがお仕置きしちゃいます。呪われちゃいますよ～、というかもう呪っちゃいました♪"
//神他喵的诅咒

static BYTE cpz5_decode_table[256];


#pragma pack (1)
typedef struct {
	char magic[4];				// "CPZ5"
	unsigned int dir_entries;			// 目录项个数
	unsigned int total_dir_entries_size;	// 所有目录项的总长度
	unsigned int total_file_entries_size;// 所有文件项的总长度
	unsigned int index_verify[4];
	unsigned int md5_data[4];
	unsigned int index_key;				// 解密index的key
	unsigned int is_encrypt;
	unsigned int is_compressed;			// ？当然现在仍是0
	unsigned int header_crc;
} cpz5_header_t;

static u32 crc(void* __buf, DWORD len, u32 crc)
{
	u32* buf = (u32*)__buf;

	for (u32 k = 0; k < len / 4; k++)
		crc += buf[k];

	for (u32 i = 0; i < (len & 3); i++)
		crc += *((u8*)& buf[k] + i);

	return crc;
}

static void decode_cpz5(BYTE* __buf, DWORD len, u32 key)
{
	DWORD decode_key[32];
	DWORD* buf = (DWORD*)__buf;
	int align;

	DWORD string_len = lstrlenA(cmvs_cpz5_decode_string) / 4;
	for (DWORD i = 0; i < string_len; ++i)
		decode_key[i] = *(u32*)& cmvs_cpz5_decode_string[i * 4] - key;

	u32 shifts = key;
	shifts = (shifts >> 8) ^ key;
	shifts = (shifts >> 8) ^ key;
	shifts = (shifts >> 8) ^ key;
	shifts = (shifts >> 8) ^ key;
	shifts = ((shifts ^ 0xFFFFFFFB) & 0x0f) + 7;

	i = 5;
	for (DWORD k = 0; k < len / 4; ++k) {
		buf[k] = _lrotr((decode_key[i] ^ buf[k]) + 0x784C5962, shifts) + 0x01010101;
		i = (i + 1) % 24;
	}

	align = len & 3;
	for (int n = align; n > 0; --n) {
		*((BYTE*)& buf[k] + align - n) = (*((BYTE*)& buf[k] + align - n) ^ (BYTE)(decode_key[i] >> (n * 4))) - 0x79;
		i = (i + 1) % 24;
	}
}

static void decode_resource_cpz5(BYTE* __buf, DWORD len, u32* key_array, u32 seed)
{
	DWORD decode_key[32];
	DWORD* buf = (DWORD*)__buf;

	DWORD string_len = lstrlenA(cmvs_cpz5_decode_string);
	u32 key = key_array[1] >> 2;
	BYTE* p = (BYTE*)decode_key;

	for (DWORD i = 0; i < string_len; ++i)
		p[i] = (BYTE)key ^ cpz5_decode_table[cmvs_cpz5_decode_string[i] & 0xff];

	for (i = 0; i < string_len / 4; ++i)
		decode_key[i] ^= seed;

	key = 0x2547A39E;
	DWORD j = 9;
	for (i = 0; i < len / 4; ++i) {
		buf[i] = key_array[key & 3] ^ ((buf[i] ^ decode_key[(key >> 6) & 0xf] ^ (decode_key[j] >> 1)) - seed);
		j = (j + 1) & 0xf;
		key += seed + buf[i];
	}
	p = (BYTE*)(buf + i);
	for (i = 0; i < (len & 3); ++i)
		p[i] = cpz5_decode_table[p[i] ^ 0xBC];
}

static DWORD cpz5_decode_init(DWORD key, DWORD seed)
{
	for (DWORD i = 0; i < 256; ++i)
		cpz5_decode_table[i] = (BYTE)i;

	for (i = 0; i < 256; ++i) {
		BYTE tmp = cpz5_decode_table[(key >> 16) & 0xff];
		cpz5_decode_table[(key >> 16) & 0xff] = cpz5_decode_table[key & 0xff];
		cpz5_decode_table[key & 0xff] = tmp;

		tmp = cpz5_decode_table[(key >> 8) & 0xff];
		cpz5_decode_table[(key >> 8) & 0xff] = cpz5_decode_table[key >> 24];
		cpz5_decode_table[key >> 24] = tmp;

		key = seed + 0x1A743125 * _lrotr(key, 2);
	}

	return key;
}

static void cpz5_decode(BYTE* data, DWORD len, DWORD key)
{
	for (DWORD i = 0; i < len; ++i)
		data[i] = cpz5_decode_table[key ^ data[i]];
}

bool EdenReadFile(char* FileName)
{
	cpz5_header_t cpz5_header;
	FILE* fp = fopen(FileName, "rb");
	if (!fp)
	{
		MessageBox(0, L"找不到文件。", 0, 0);
		return false;
	}
	fread(&cpz5_header, sizeof(cpz5_header), 1, fp);
	if (strncmp(cpz5_header.magic, "CPZ5", 4) != 0)
	{
		cout << cpz5_header.magic << endl;
		MessageBox(0, L"不是CPZ5。", 0, 0);
		return false;
	}
	if (crc(&cpz5_header, sizeof(cpz5_header) - 4, 0x923A564C) != cpz5_header.header_crc)
	{
		cout << cpz5_header.header_crc << endl;
		MessageBox(0, L"CRC校验失败。", 0, 0);
		return false;
	}

	/*
	fseek(fp, 0, SEEK_END);
	DWORD size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	char* buff = (char*)malloc(size);
	fread(buff, size, 1, fp);
	fclose(fp);
	*/

}

int main()
{
    std::cout << "Hello World!\n";
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
