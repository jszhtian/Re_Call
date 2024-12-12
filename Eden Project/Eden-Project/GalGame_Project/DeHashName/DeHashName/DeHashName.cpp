// DeHashName.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <Windows.h>
#include <io.h>
#include <direct.h>
#include <string>
#include <vector>
#include <algorithm> 
using namespace std;
static const char* name_array[] = { "story.txt","start.txt","titlelogo.txt","title.txt","story.txt","CS01_N_01.txt",
"CS01_N_02.txt",
"CS01_N_03.txt",
"CS01_N_04.txt",
"CS01_N_05.txt",
"CS01_N_06.txt",
"CS01_N_07.txt",
"CS01_N_08.txt",
"CS01_N_09.txt",
"CS01_N_09H.txt",
"CS01_N_10.txt",
"CS01_N_10H.txt",
"CS01_N_11.txt",
"CS01_N_11H.txt",
"CS01_N_12.txt",
"CS01_N_13.txt",
"CS01_N_13H.txt",
"CS01_N_14.txt",
"CS01_N_15.txt",
"title.txt",
"CS01_A_01.txt",
"CS01_A_02.txt",
"CS01_A_03.txt",
"CS01_A_04.txt",
"CS01_A_05.txt",
"CS01_A_06.txt",
"CS01_A_07.txt",
"CS01_A_08.txt",
"CS01_A_09.txt",
"CS01_A_09H.txt",
"CS01_A_10.txt",
"CS01_A_11.txt",
"CS01_A_12.txt",
"CS01_A_13.txt",
"CS01_A_13H.txt",
"CS01_A_14.txt",
"CS01_A_14H.txt",
"CS01_A_15.txt",
"CS01_A_15H.txt",
"CS01_A_16.txt",
"CS01_A_F.txt",
"title.txt",
"CS01_S_01.txt",
"CS01_S_02.txt",
"CS01_S_03.txt",
"CS01_S_04.txt",
"CS01_S_05.txt",
"CS01_S_06.txt",
"CS01_S_07.txt",
"CS01_S_08.txt",
"CS01_S_08H.txt",
"CS01_S_09.txt",
"CS01_S_10.txt",
"CS01_S_10H.txt",
"CS01_S_11.txt",
"CS01_S_11H.txt",
"CS01_S_12.txt",
"CS01_S_12H.txt",
"CS01_S_13.txt",
"CS01_S_14.txt",
"CS01_S_F.txt",
"title.txt",
"CS01_H_01.txt",
"CS01_H_02.txt",
"CS01_H_03.txt",
"CS01_H_04.txt",
"CS01_H_05.txt",
"CS01_H_06.txt",
"CS01_H_07.txt",
"CS01_H_08.txt",
"CS01_H_09.txt",
"CS01_H_10.txt",
"CS01_H_11.txt",
"CS01_H_11H.txt",
"CS01_H_12.txt",
"CS01_H_12H.txt",
"CS01_H_13.txt",
"CS01_H_13H.txt",
"CS01_H_14.txt",
"CS01_H_14H.txt",
"CS01_H_F.txt",
"title.txt",
"title.txt",
"title.txt",
"title.txt",
"titlelogo.txt",
"CS01_C_01.txt",
"CS01_C_02.txt",
"CS01_C_03.txt",
"CS01_C_04.txt",
"CS01_C_05.txt",
"CS01_C_06.txt",
"CS01_C_07.txt",
"CS01_C_08.txt",
"CS01_C_09.txt",
"CS01_C_10.txt",
"CS01_C_11.txt",
"CS01_C_12.txt",
"CS01_C_13.txt",
"CS01_C_14.txt",
"CS01_C_15.txt",
"CS01_C_16.txt",
"CS01_C_17.txt",
"CS01_C_18.txt",
"CS01_C_19.txt",
"CS01_C_20.txt",
"storyH.txt",
"storyS.txt",
"storyA.txt",
"storyN.txt",
"story.txt",
"story.txt",
"title.txt",
"CS01_H_F2.txt" };
uint8_t hash_table[] =
{
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x38,0x2F,0x33,0x3C,0x40,0x3B,0x2A,0x2E,0x31,0x30,0x26,0x44,0x35,0x28,0x3E,0x12,
	0x02,0x22,0x06,0x20,0x1A,0x1C,0x0F,0x11,0x18,0x17,0x42,0x2B,0x3A,0x37,0x34,0x0C,
	0x41,0x08,0x1D,0x07,0x15,0x21,0x05,0x1E,0x0A,0x14,0x0E,0x10,0x09,0x27,0x1F,0x0B,
	0x23,0x16,0x0D,0x01,0x25,0x04,0x1B,0x03,0x13,0x24,0x19,0x2D,0x12,0x29,0x32,0x3F,
	0x3D,0x08,0x1D,0x07,0x15,0x21,0x05,0x1E,0x0A,0x14,0x0E,0x10,0x09,0x27,0x1F,0x0B,
	0x23,0x16,0x0D,0x01,0x25,0x04,0x1B,0x03,0x13,0x24,0x19,0x2C,0x39,0x43,0x36,0x00,
	0x4B,0xA9,0xA7,0xAF,0x50,0x52,0x91,0x9F,0x47,0x6B,0x96,0xAB,0x87,0xB5,0x9B,0xBB,
	0x99,0xA4,0xBF,0x5C,0xC6,0x9C,0xC2,0xC4,0xB6,0x4F,0xB8,0xC1,0x85,0xA8,0x51,0x7E,
	0x5F,0x82,0x73,0xC7,0x90,0x4E,0x45,0xA5,0x7A,0x63,0x70,0xB3,0x79,0x83,0x60,0x55,
	0x5B,0x5E,0x68,0xBA,0x53,0xA1,0x67,0x97,0xAC,0x71,0x81,0x59,0x64,0x7C,0x9D,0xBD,
	0x9D,0xBD,0x95,0xA0,0xB2,0xC0,0x6F,0x6A,0x54,0xB9,0x6D,0x88,0x77,0x48,0x5D,0x72,
	0x49,0x93,0x57,0x65,0xBE,0x4A,0x80,0xA2,0x5A,0x98,0xA6,0x62,0x7F,0x84,0x75,0xBC,
	0xAD,0xB1,0x6E,0x76,0x8B,0x9E,0x8C,0x61,0x69,0x8D,0xB4,0x78,0xAA,0xAE,0x8F,0xC3,
	0x58,0xC5,0x74,0xB7,0x8E,0x7D,0x89,0x8A,0x56,0x4D,0x86,0x94,0x9A,0x4C,0x92,0xB0
};

__declspec(naked)void hash_string()
{
	__asm
	{
		push    ebp
		mov     ebp, esp
		add     esp, -0x24
		push    esi
		push    edi
		mov     esi, dword ptr[ebp + 0xC]
		mov     edi, offset hash_table
		mov     eax, dword ptr[ebp + 0x8]
		jmp     label2
		label1 :
		inc     esi
			xor edx, edx
			mov     dl, byte ptr[ecx + edi]
			xor eax, edx
			mov     edx, eax
			shl     edx, 0x4
			add     eax, edx
			shl     edx, 0x2
			add     eax, edx
			label2 :
		xor ecx, ecx
			mov     cl, byte ptr[esi]
			cmp     cl, 0x0
			jnz     label1
			pop     edi
			pop     esi
			mov     esp, ebp
			pop     ebp
			retn
	}
}

vector<string>FilePool;

int E(const char* msg)
{
	MessageBoxA(0, msg, 0, 0);
	return -1;
}


void fileSearch(string path)
{
	long hFile = 0;
	struct _finddata_t fileInfo;
	string pathName;
	if ((hFile = _findfirst(pathName.assign(path).append("\\*").c_str(), &fileInfo)) == -1)
		return;

	do {
		string fn = path + "\\" + fileInfo.name;
		//cout << fn << endl;
		//string fnm = fn.substr(fn.find_first_of("\\") + 1);
		//cout << fnm << endl;
		//cout << fileInfo.name << endl;
		if (strcmp(fileInfo.name, "..") && strcmp(fileInfo.name, ".") && fileInfo.attrib == _A_SUBDIR)
			fileSearch(fn);
		else if (fileInfo.name[0] != '.')
			FilePool.push_back(fn);
	} while (_findnext(hFile, &fileInfo) == 0);
	_findclose(hFile);
	return;
}
typedef uint32_t(*hash_string_func_t)(uint32_t top_name, const char* hash_name);
hash_string_func_t hash_string_func = (hash_string_func_t)&hash_string;
void FileNameToLower(std::string& FileName)
{
	std::transform(FileName.begin(), FileName.end(), FileName.begin(), ::tolower);
}
int main()
{
	fileSearch("hash");
	_mkdir("dehash");
	for (const char* n : name_array)
	{
		uint32_t result = hash_string_func(0x8c416067, n);
		char fnm[MAX_PATH];
		sprintf(fnm,"%x", result);
		for (string f : FilePool)
		{
			string fn = f.substr(f.find_last_of("\\") + 1);
			FileNameToLower(fn);
			if (!strcmp(fn.c_str(), fnm))
			{
				char out[MAX_PATH];
				strcpy(out, "dehash\\");
				strcat(out, n);
				auto fin = fopen(f.c_str(), "rb");
				if (!fin)
				{
					cout << f << endl;
					return E("no search file");
				}
				fseek(fin, 0, SEEK_END);
				DWORD size = ftell(fin);
				fseek(fin, 0, SEEK_SET);
				BYTE* buff = (BYTE*)malloc(size);
				if (!buff)
					return E("No buffer alloc");
				cout << "open:" << f << endl;
				if (!fread(buff, size, 1, fin))
					return E("ReadFile Error.");
				fclose(fin);
				
				auto fout = fopen(out, "wb");
				if (!fout)
				{
					cout << out << endl;
					return E("No where to create");
				}
				cout << "write:" << out << endl;
				fwrite(buff, size, 1, fout);
				fclose(fout);
				free(buff);

			}
			/*
			else
			{
				char out[MAX_PATH];
				strcpy(out, "dehash\\");
				strcat(out, fn.c_str());
				auto in = fopen(f.c_str(), "rb");
				if (!in)
				{
					cout << f << endl;
					return E("no search file");
				}
				fseek(in, 0, SEEK_END);
				DWORD size = ftell(in);
				fseek(in, 0, SEEK_SET);
				BYTE* buff = (BYTE*)malloc(size);
				if (!buff)
					return E("No buffer alloc");
				cout << "open:" << f << endl;
				fread(buff, size, 1, in);
				fclose(in);

				in = fopen(out, "wb");
				if (!in)
				{
					cout << out << endl;
					return E("No where to create");
				}
				cout << "write:" << out << endl;
				fwrite(buff, size, 1, in);
				fclose(in);
				free(buff);
			}*/
		}

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
