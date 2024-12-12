// extElfMes.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <Windows.h>
#include <fstream> 

using namespace std;

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		MessageBox(0, L"extElfMes.exe <input mes file>", L"AyamiKaze", 0);
		return -1;
	}
	char* FileName = argv[1];
	//char* FileName = (char*)"event6.mes";
	FILE* fp = fopen(FileName, "rb");
	fseek(fp, 0, SEEK_END);
	DWORD size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	char* buff = new char[size];
	fread(buff, size, 1, fp);
	fclose(fp);

	DWORD ResOff = *(DWORD*)buff;
	ResOff *= 4;
	DWORD LoopSize = size - ResOff;
	char op = 0;
	buff += ResOff + 4;

	char ofnm[MAX_PATH] = { 0 };
	strcpy(ofnm, FileName);
	strcat(ofnm, ".txt");
	ofstream txt(ofnm);
	for (DWORD i = 0; i < LoopSize;)
	{
		//op = *(char*)buff[i];
		//printf("opcode %x pos %x\n", buff[i], i);
		char* text;
		switch (buff[i])
		{
		case 0x1C:
		{
			i += 1;
			break;
		}
		case 0x14:
		case 0x15:
		case 0x16:
		case 0x19:
		case 0x1A:
		case 0x1B:
		case 0x32:
		{
			i += 4;
			break;
		}
		case 0x33:
		case 0x0A:
		{
			text = buff + i + 1;
			txt << text << endl;
			i += strlen(text);
			break;
		}
		default:
			break;
		}
		i += 1;
	}
	txt.close();
	//system("pause");
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
