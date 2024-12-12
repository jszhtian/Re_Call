// Test.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "TextUtil.h"

char* wtoc(LPCTSTR str, unsigned int cp)
{
	DWORD dwMinSize;
	dwMinSize = WideCharToMultiByte(cp, NULL, str, -1, NULL, 0, NULL, FALSE); //计算长度
	char* out = new char[dwMinSize];
	WideCharToMultiByte(cp, NULL, str, -1, out, dwMinSize, NULL, FALSE);//转换
	return out;
}

/*
unsigned long long MurmurHash64B(const void* key, int len, unsigned int seed = 0x114514)
{
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

int main()
{
	bin_text_header header;
	FILE* fp = fopen("Script.bin", "rb");
	fseek(fp, 0, SEEK_END);
	unsigned int FileSize = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	// Read Header
	fread(&header, sizeof(bin_text_header), 1, fp);
	
	// Check Header Info
	if (strcmp(header.flag, "AyamiKazeFormatScriptVer1.00"))
	{
		cout << "header.flag not match" << endl;
		system("pause");
		return 0;
	}
	cout << "----------------------------header.info----------------------------" << endl;
	cout << "header.HeaderSize:" << header.HeaderSize << endl;
	cout << "header.HeaderHash:" << header.HeaderHash << endl;

	cout << "header.EntrySize:" << header.EntrySize << endl;
	cout << "header.EntryHash:" << header.EntryHash << endl;
	cout << "header.EntryCompSize:" << header.EntryCompSize << endl;

	cout << "header.TextSize:" << header.TextSize << endl;
	cout << "header.TextHash:" << header.TextHash << endl;
	cout << "header.TextCompSize:" << header.TextCompSize << endl;
	cout << "----------------------------header.info----------------------------" << endl;

	// Read HeaderBuff
	char* HeaderBuff = (char*)malloc(header.HeaderSize);
	if (!HeaderBuff)
	{
		cout << "Alloc HeaderBuff Error" << endl;
		system("pause");
		return 0;
	}
	fread(HeaderBuff, header.HeaderSize, 1, fp);

	// Decrypt aym_header_buff
	for (int i = 0; i < header.HeaderSize; i++)
	{
		HeaderBuff[i] ^= header.HeaderKey[i % XOR_KEY_LEN];
		HeaderBuff[i] -=0x20;
	}

	// Get aym_header Info
	aym_header* aym_hdr = (aym_header*)HeaderBuff;

	// Check aym_header Info
	if (MurmurHash64B(HeaderBuff, header.HeaderSize) != header.HeaderHash)
	{
		cout << "HeaderBuff hash not match" << endl;
		system("pause");
		return 0;
	}
	if (strcmp(aym_hdr->flag, "AyamiKazeScriptVer1.00"))
	{
		cout << "aym_hdr->flag not match" << endl;
		system("pause");
		return 0;
	}

	// Read EntryBuff
	unsigned char* EntryBuff = (unsigned char*)malloc(header.EntryCompSize);
	if (!EntryBuff)
	{
		cout << "Alloc EntryBuff Error" << endl;
		system("pause");
		return 0;
	}
	fread(EntryBuff, header.EntryCompSize, 1, fp);

	// uncompress EntryBuff
	unsigned char* EntryUncompBuff = (unsigned char*)malloc(header.EntrySize);
	if (uncompress(EntryUncompBuff, &header.EntrySize, EntryBuff, header.EntryCompSize) != Z_OK)
	{
		cout << "uncompress EntryBuff Error" << endl;
		system("pause");
		return 0;
	}
	free(EntryBuff);

	// Check Uncompressed EntryBuff
	if (MurmurHash64B(EntryUncompBuff, header.EntrySize) != header.EntryHash)
	{
		cout << "EntryUncompBuff hash not match" << endl;
		system("pause");
		return 0;
	}

	// Read TextBuff
	unsigned char* TextBuff = (unsigned char*)malloc(header.TextCompSize);
	if (!TextBuff)
	{
		cout << "Alloc TextBuff Error" << endl;
		system("pause");
		return 0;
	}
	fread(TextBuff, header.TextCompSize, 1, fp);
	fclose(fp);

	// Uncompress TextBuff
	unsigned char* TextUncompBuff = (unsigned char*)malloc(header.TextSize);
	size_t ret = ZSTD_decompress(TextUncompBuff, header.TextSize, TextBuff, header.TextCompSize);
	if (ZSTD_isError(ret) != ZSTD_error_no_error)
	{
		cout << "uncompress TextBuff Error:" << ZSTD_isError(ret) << endl;
		system("pause");
		return 0;
	}
	free(TextBuff);

	// Check Uncompressed TextBuff
	if (MurmurHash64B(TextUncompBuff, header.TextSize) != header.TextHash)
	{
		cout << "TextUncompBuff hash not match" << endl;
		system("pause");
		return 0;
	}

	ofstream txt("log.txt");
	aym_text_struct* entry_buff = (aym_text_struct*)EntryUncompBuff;
	for (unsigned int i = 0; i < aym_hdr->str_count; i++)
	{
		txt << "----------------------------text.info----------------------------" << endl;
		txt << "entry_buff.off:" << entry_buff[i].off << endl;
		txt << "entry_buff.sz:" << entry_buff[i].sz << endl;
		txt << "entry_buff.jmp:" << entry_buff[i].jmp << endl;
		WCHAR* text = (WCHAR*)(TextUncompBuff + entry_buff[i].jmp);
		txt << "text:" << wtoc(text, CP_UTF8) << endl;
	}
	txt.close();

	free(TextUncompBuff);
	free(EntryUncompBuff);
	system("pause");
	return 0;
}
*/

int main()
{
	ReadTextFromAymScript RTFAS;
	if (!RTFAS.InitAymScript())
	{
		MessageBox(0, L"Can't comp Script.bin", 0, 0);
		return 0;
	}
	while (true)
	{
		unsigned int off;
		cout << "input off:" << endl;
		cin >> off;

		pair<WCHAR*, unsigned int>str_it = RTFAS.GetTextByOff(off);
		if (str_it.first == NULL && str_it.second == NULL)
		{
			MessageBox(0, L"str_it is 0", 0, 0);
			return 0;
		}
		cout << "size:" << str_it.second << endl;
		cout << "str:" << wtoc(str_it.first, CP_ACP) << endl;

	}
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
