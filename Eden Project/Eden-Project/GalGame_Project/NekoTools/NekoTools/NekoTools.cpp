// NekoTools.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <Windows.h>
#define SWAP4(x)	(((x) & 0xff) << 24 | ((x) & 0xff00) << 8 | ((x) & 0xff0000) >> 8 | ((x) & 0xff000000) >> 24)

using namespace std;

typedef struct NEKO_HEAD {
	char* Magic[8];//NEKOPACK
	unsigned int seed;
	unsigned int unknown;
	unsigned int parity;
	unsigned int index_length;
}NekoHeader;

typedef struct {
	unsigned int dir_name_hash;
	unsigned int dir_entries;
	unsigned int dir_entries0;
} dat_dir_t;

typedef struct {
	unsigned int name_hash;
	unsigned int length;
} dat_entry_t;

typedef struct {
	char name[MAX_PATH];
	unsigned int offset;
	unsigned int length;
	unsigned int sync;	// for old dat
} my_dat_entry_t;

typedef struct {
	unsigned int dir_name_hash;
	unsigned int dir_entries;
} new_dat_dir_t;


static DWORD parity_check(DWORD key0, DWORD key1)
{
	DWORD tmp = (key1 ^ ((key0 ^ key1) + 0x5D588B65)) - 0x359D3E2A;
	DWORD v = (key1 ^ ((key1 ^ tmp) - 0x70E44324)) + 0x6C078965;
	return v << (tmp >> 27) | v >> (32 - (tmp >> 27));
}

static void gen_key(DWORD& ret_key0, DWORD& ret_key1, DWORD seed)
{
	DWORD tmp = seed ^ (seed + 0x5D588B65);
	DWORD tmp2 = tmp ^ (seed - 0x359D3E2A);
	ret_key0 = tmp2 ^ (tmp - 0x70E44324);
	ret_key1 = ret_key0 ^ (tmp2 + 0x6C078965);
}

static void xcode_create(unsigned int key, unsigned int* xcode, DWORD xcode_len)
{
	unsigned int eax = 0;
	unsigned int ebx = 0;

	do {
		eax <<= 1;
		ebx ^= 1;
		eax = ((eax | ebx) << (key & 1)) | ebx;
		key >>= 1;
	} while (!(eax & 0x80000000));
	key = eax << 1;
	eax = key + SWAP4(key);
	BYTE cl = (BYTE)key;
	do {
		ebx = key ^ eax;
		eax = (ebx << 4) ^ (ebx >> 4) ^ (ebx << 3) ^ (ebx >> 3) ^ ebx;
		--cl;
	} while (cl);

	for (DWORD i = 0; i < 616 / 4; i++) {
		ebx = key ^ eax;
		eax = (ebx << 4) ^ (ebx >> 4) ^ (ebx << 3) ^ (ebx >> 3) ^ ebx;
		xcode[i] = eax;
	}
}

static void xcode_init(unsigned int key, BYTE* xcode, BYTE flag)
{
	int v4;
	BYTE byte_532310[14] = { 0xEF, 0xFC, 0xFD, 0xFE, 0xF8, 0xF9, 0xFA, 0xEF, 0xF8, 0xF9, 0xFA, 0xFC, 0xFD, 0xFE };
	BYTE byte_532324[6] = { 0xCA, 0xD3, 0xDC, 0xE5, 0xEE, 0xF1 };

	xcode[0] = 0x01;
	xcode[1] = 0xF1;
	xcode[2] = 0x0F;
	xcode[3] = 0x6F;
	xcode[4] = 0x06;
	xcode += 5;
	int v15 = key & 0xffff;
	int v16 = (key >> 16) & 0xfff;
	if (flag)
		v4 = 0;
	else
		v4 = 7;
	unsigned int v17 = v4 + (key >> 28);
	for (DWORD i = 0; i < 4; i++) {
		BYTE v6;
		int v11, v12;

		if (flag)
			v6 = (BYTE)i;
		else
			v6 = (BYTE)(3 - i);
		v11 = v15 >> (4 * v6);
		v12 = v16 >> (3 * v6);
		xcode[0] = 15;
		xcode[1] = byte_532310[(v11 + v17) % 0xE];
		xcode[2] = v12 % 6 - 63;
		xcode += 3;
	}
	xcode[0] = 0x0F;
	xcode[1] = 0x7F;
	xcode[2] = 0x07;
	xcode[3] = 0x83;
	xcode[4] = 0xC6;
	xcode[5] = 0x08;
	xcode[6] = 0x83;
	xcode[7] = 0xC7;
	xcode[8] = 0x08;
	xcode += 9;
	for (unsigned int i = 0; i < 6; i++) {
		xcode[0] = 15;
		xcode[1] = -44;
		xcode[2] = byte_532324[(i + key) % 6];
		xcode += 3;
	}
	xcode[0] = 0x39;
	xcode[1] = 0xCE;
	xcode[2] = 0x72;
	xcode[3] = 0xD2;
	xcode[4] = 0xC3;
}

static void xcode_execute(unsigned int key, BYTE* xcode_base, BYTE* dst, BYTE* src, SIZE_T len)
{
	unsigned __int64 mm_reg[6];
	void (*xcode_start)(void) = (void (*)(void))(xcode_base + xcode_base[0] + 320);

	for (DWORD i = 0; i < 6; i++) {
		mm_reg[i] = *(unsigned __int64*)&xcode_base[(key % 0x28) * 8];
		key /= 0x28;
	}

	__asm {
		movq    mm1, qword ptr mm_reg[0]
		movq    mm2, qword ptr mm_reg[8]
		movq    mm3, qword ptr mm_reg[16]
		movq    mm4, qword ptr mm_reg[24]
		movq    mm5, qword ptr mm_reg[32]
		movq    mm6, qword ptr mm_reg[40]
		mov		esi, src
		mov		edi, dst
		mov		ecx, len
	}
	(*xcode_start)();

	__asm { emms }
}

static BYTE* xcode = NULL;

static void decrypt_release(void)
{
	if (xcode) {
		VirtualFree(xcode, 0, MEM_RELEASE);
		xcode = NULL;
	}
}

static bool decrypt_init(unsigned int init_key)
{
	DWORD old_protection;

	xcode = (BYTE*)VirtualAlloc(NULL, 616, MEM_COMMIT, PAGE_READWRITE);
	if (!xcode)
		return false;

	xcode_create(init_key, (unsigned int*)xcode, 616);
	xcode_init(init_key, xcode + xcode[0] + 320, 0);
	if (!VirtualProtect(xcode, 616, PAGE_EXECUTE_READ, &old_protection)) 
	{
		decrypt_release();
		return false;
	}

	return true;
}

static char* dir_name_table[] = {
	(char*)"image/actor",(char*)"image/back", (char*)"sound/se",(char*)"voice",(char*)"sound/bgm", (char*)"script", (char*)"image/visual", (char*)"system", (char*)"count", NULL
};

static BYTE name_table[256] = {
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0xc9, 0xca, 0x00, 0xcb, 0xcc, 0xcd, 0xce, 0xcf, 0xd0, 0xd1, 0x00, 0xd2, 0xd3, 0x27, 0x25, 0xc8,
		0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x00, 0xd4, 0x00, 0xd5, 0x00, 0x00,
		0xd6, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19,
		0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x20, 0x21, 0x22, 0x23, 0x24, 0xd7, 0xc8, 0xd8, 0xd9, 0x26,
		0xda, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19,
		0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x20, 0x21, 0x22, 0x23, 0x24, 0xdb, 0x00, 0xdc, 0xdd, 0x00,
		0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38,
		0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f, 0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48,
		0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f, 0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58,
		0x59, 0x5a, 0x5b, 0x5c, 0x5d, 0x5e, 0x5f, 0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68,
		0x69, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78,
		0x79, 0x7a, 0x7b, 0x7c, 0x7d, 0x7e, 0x7f, 0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88,
		0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f, 0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98,
		0x99, 0x9a, 0x9b, 0x9c, 0x9d, 0x9e, 0x9f, 0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7, 0xa8
};

static BYTE new_name_table[256] = {
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x38, 0x2f, 0x33, 0x3c, 0x40, 0x3b, 0x2a, 0x2e, 0x31, 0x30, 0x26, 0x44, 0x35, 0x28, 0x3e, 0x12,
		0x02, 0x22, 0x06, 0x20, 0x1a, 0x1c, 0x0f, 0x11, 0x18, 0x17, 0x42, 0x2b, 0x3a, 0x37, 0x34, 0x0c,
		0x41, 0x08, 0x1d, 0x07, 0x15, 0x21, 0x05, 0x1e, 0x0a, 0x14, 0x0e, 0x10, 0x09, 0x27, 0x1f, 0x0b,
		0x23, 0x16, 0x0d, 0x01, 0x25, 0x04, 0x1b, 0x03, 0x13, 0x24, 0x19, 0x2d, 0x12, 0x29, 0x32, 0x3f,
		0x3d, 0x08, 0x1d, 0x07, 0x15, 0x21, 0x05, 0x1e, 0x0a, 0x14, 0x0e, 0x10, 0x09, 0x27, 0x1f, 0x0b,
		0x23, 0x16, 0x0d, 0x01, 0x25, 0x04, 0x1b, 0x03, 0x13, 0x24, 0x19, 0x2c, 0x39, 0x43, 0x36, 0x00,
		0x4b, 0xa9, 0xa7, 0xaf, 0x50, 0x52, 0x91, 0x9f, 0x47, 0x6b, 0x96, 0xab, 0x87, 0xb5, 0x9b, 0xbb,
		0x99, 0xa4, 0xbf, 0x5c, 0xc6, 0x9c, 0xc2, 0xc4, 0xb6, 0x4f, 0xb8, 0xc1, 0x85, 0xa8, 0x51, 0x7e,
		0x5f, 0x82, 0x73, 0xc7, 0x90, 0x4e, 0x45, 0xa5, 0x7a, 0x63, 0x70, 0xb3, 0x79, 0x83, 0x60, 0x55,
		0x5b, 0x5e, 0x68, 0xba, 0x53, 0xa1, 0x67, 0x97, 0xac, 0x71, 0x81, 0x59, 0x64, 0x7c, 0x9d, 0xbd,
		0x9d, 0xbd, 0x95, 0xa0, 0xb2, 0xc0, 0x6f, 0x6a, 0x54, 0xb9, 0x6d, 0x88, 0x77, 0x48, 0x5d, 0x72,
		0x49, 0x93, 0x57, 0x65, 0xbe, 0x4a, 0x80, 0xa2, 0x5a, 0x98, 0xa6, 0x62, 0x7f, 0x84, 0x75, 0xbc,
		0xad, 0xb1, 0x6e, 0x76, 0x8b, 0x9e, 0x8c, 0x61, 0x69, 0x8d, 0xb4, 0x78, 0xaa, 0xae, 0x8f, 0xc3,
		0x58, 0xc5, 0x74, 0xb7, 0x8e, 0x7d, 0x89, 0x8a, 0x56, 0x4d, 0x86, 0x94, 0x9a, 0x4c, 0x92, 0xb0,
};

static unsigned int get_hash(unsigned int hash, char* name)
{
	while (1) {
		BYTE chr = *name++;
		if (!chr)
			break;
		hash = 16777258 * (name_table[chr] ^ hash);
	}
	return hash;
}

static unsigned int get_new_hash(unsigned int hash, char* name)
{
	while (1) {
		BYTE chr = *name++;
		if (!chr)
			break;
		hash = 81 * (new_name_table[chr] ^ hash);
	}
	return hash;
}

static void decode(BYTE* buf, DWORD len, unsigned short* key)
{
	for (DWORD i = 0; i < len / 2; ++i) {
		*(unsigned short*)buf ^= key[i & 3];
		key[i & 3] += *(unsigned short*)buf;
		buf += 2;
	}
}

static DWORD index_decode_table[0x270];
static DWORD index_decode_table_count;
static DWORD data_decode_table[0x270];
static DWORD data_decode_table_count;

static void decrypt(unsigned int exec_key, BYTE* dec, BYTE* enc, DWORD len)
{
	if (xcode)
		xcode_execute(exec_key, xcode, dec, enc, len);
	else
		memcpy(dec, enc, len);
}

bool Main2(char* FileName)
{
	FILE* fp = fopen(FileName, "rb");
	if (!fp)
	{
		MessageBox(0, L"找不到文件。", 0, 0);
		return false;
	}
	NekoHeader header;
	fseek(fp, 0, SEEK_END);
	DWORD size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	BYTE* buff = (BYTE*)malloc(size);
	fread(buff, size, 1, fp);
	fclose(fp);
	memcpy(header.Magic, buff, 8);
	buff += 8;
	header.seed = *(unsigned int*)buff;
	buff += 4;
	header.unknown = *(unsigned int*)buff;
	buff += 4;
	header.parity = *(unsigned int*)buff;
	buff += 4;
	header.index_length = *(unsigned int*)buff;
	buff += 4;
	cout << "Magic:" << header.Magic << endl;
	cout << "seed:0x" << hex << header.seed << endl;
	cout << "unknown:0x" << hex << header.unknown << endl;
	cout << "parity:0x" << hex << header.parity << endl;
	cout << "index_length:0x" << hex << header.index_length << endl;
	unsigned int size_2 = (header.index_length + 63) & ~63;
	cout << "malloc_size:0x" << hex << size_2 << endl;
	BYTE* index = (BYTE*)malloc(size_2);
	if (!index)
	{
		MessageBox(0, L"index NULL", 0, 0);
		return false;
	}
	memcpy(index, buff, size_2);

	DWORD key[2];
	gen_key(key[0], key[1], header.parity);
	decode(index, header.index_length, (unsigned short*)key);

	DWORD dirs = 0;
	DWORD index_entries = 0;
	for (BYTE* p = index; p < index + header.index_length; ) {
		new_dat_dir_t* dir = (new_dat_dir_t*)p;

		index_entries += dir->dir_entries;
		dirs++;
		p += sizeof(new_dat_dir_t) + dir->dir_entries * sizeof(dat_entry_t);
	}

	DWORD index_buffer_length = index_entries * sizeof(my_dat_entry_t);
	my_dat_entry_t* index_buffer = (my_dat_entry_t*)malloc(index_buffer_length);
	if (!index_buffer) {
		free(index);
		MessageBox(0, L"index_buffer NULL", 0, 0);
		return false;
	}

	BYTE* p = index;
	DWORD k = 0;
	DWORD offset = sizeof(NekoHeader) + header.index_length;
	for (DWORD d = 0; d < dirs; d++) {
		new_dat_dir_t* dir = (new_dat_dir_t*)p;
		const char* dir_name = NULL;

		p += sizeof(new_dat_dir_t);

		for (DWORD n = 0; dir_name_table[n]; n++) {
			if (get_new_hash(header.seed, dir_name_table[n]) == dir->dir_name_hash) {
				dir_name = dir_name_table[n];
				break;
			}
		}

		for (DWORD i = 0; i < dir->dir_entries; i++) {
			dat_entry_t* entry = (dat_entry_t*)p;

			if (dir_name)
				sprintf(index_buffer[k].name, "%s/%08x", dir_name, entry->name_hash);
			else
				sprintf(index_buffer[k].name, "%08x/%08x", dir->dir_name_hash, entry->name_hash);
			index_buffer[k].length = entry->length;
			index_buffer[k++].offset = offset;
			offset += entry->length + 8;
			p += sizeof(dat_entry_t);
			cout << index_buffer[k].name << endl;
		}
	}

	free(index);
	return true;
}

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		MessageBox(0, L"Usage: NekoTools.exe <InputFile.dat>", 0, 0);
		//return -1;
	}
	//char* FILENAME = argv[1];
	char* FILENAME = (char*)"patch101.dat";
	bool succ = Main2(FILENAME);
	if (!succ)
	{
		MessageBox(0, L"程序运行失败。", 0, 0);
		return -1;
	}
	system("pause");
	return 0;
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
