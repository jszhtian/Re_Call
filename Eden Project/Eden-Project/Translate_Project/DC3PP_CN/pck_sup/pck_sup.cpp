// pck_sup.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "Global.h"

unsigned char* CrxFileBuffer1;
unsigned char* CrxFileBuffer2;
unsigned char* CrxFileBuffer3;
unsigned char* CrxFileBuffer4;
unsigned char* CrxFileBuffer5;
unsigned char* CrxFileBuffer6;
unsigned char* CrxFileBuffer7;
unsigned char* pBuff;

unsigned long CrxFileSize1;
unsigned long CrxFileSize2;
unsigned long CrxFileSize3;
unsigned long CrxFileSize4;
unsigned long CrxFileSize5;
unsigned long CrxFileSize6;
unsigned long CrxFileSize7;
unsigned long pSize;

void CopyOutCrxFileToPckFile()
{
	DWORD PckAddr = 0;
	DWORD PatchAddr1 = PckAddr + 0x800;
	DWORD PatchAddr2 = PckAddr + 0x3c800;
	DWORD PatchAddr3 = PckAddr + 0x93000;
	DWORD PatchAddr4 = PckAddr + 0xe9800;
	DWORD PatchAddr5 = PckAddr + 0x140000;
	DWORD PatchAddr6 = PckAddr + 0x190000;
	DWORD PatchAddr7 = PckAddr + 0x1e0000;

	DWORD bSize1 = sizeof(PatchAddr1) - sizeof(CrxFileBuffer1);
	DWORD bSize2 = sizeof(PatchAddr2) - sizeof(CrxFileBuffer2);
	DWORD bSize3 = sizeof(PatchAddr3) - sizeof(CrxFileBuffer3);
	DWORD bSize4 = sizeof(PatchAddr4) - sizeof(CrxFileBuffer4);
	DWORD bSize5 = sizeof(PatchAddr5) - sizeof(CrxFileBuffer5);
	DWORD bSize6 = sizeof(PatchAddr6) - sizeof(CrxFileBuffer6);
	DWORD bSize7 = sizeof(PatchAddr7) - sizeof(CrxFileBuffer7);

	memcpy((void*)PatchAddr1, CrxFileBuffer1, sizeof(CrxFileBuffer1));
	memcpy((void*)PatchAddr2, CrxFileBuffer2, sizeof(CrxFileBuffer2));
	memcpy((void*)PatchAddr3, CrxFileBuffer3, sizeof(CrxFileBuffer3));
	memcpy((void*)PatchAddr4, CrxFileBuffer4, sizeof(CrxFileBuffer4));
	memcpy((void*)PatchAddr5, CrxFileBuffer5, sizeof(CrxFileBuffer5));
	memcpy((void*)PatchAddr6, CrxFileBuffer6, sizeof(CrxFileBuffer6));
	memcpy((void*)PatchAddr7, CrxFileBuffer7, sizeof(CrxFileBuffer7));
}

void FileReader()
{
	FILE* fp1 = fopen("CRX_PACK_FILE\\05.CRX", "rb");
	fseek(fp1, 0, SEEK_END);
	CrxFileSize1 = ftell(fp1);
	fseek(fp1, 0, SEEK_SET);
	CrxFileBuffer1 = new unsigned char[CrxFileSize1];
	fread(CrxFileBuffer1, CrxFileSize1, 1, fp1);
	fclose(fp1);

	FILE* fp2 = fopen("CRX_PACK_FILE\\06.CRX", "rb");
	fseek(fp2, 0, SEEK_END);
	CrxFileSize2 = ftell(fp2);
	fseek(fp2, 0, SEEK_SET);
	CrxFileBuffer2 = new unsigned char[CrxFileSize2];
	fread(CrxFileBuffer2, CrxFileSize2, 1, fp2);
	fclose(fp2);

	FILE* fp3 = fopen("CRX_PACK_FILE\\07.CRX", "rb");
	fseek(fp3, 0, SEEK_END);
	CrxFileSize3 = ftell(fp3);
	fseek(fp3, 0, SEEK_SET);
	CrxFileBuffer3 = new unsigned char[CrxFileSize3];
	fread(CrxFileBuffer3, CrxFileSize3, 1, fp3);
	fclose(fp3);

	FILE* fp4 = fopen("CRX_PACK_FILE\\08.CRX", "rb");
	fseek(fp4, 0, SEEK_END);
	CrxFileSize4 = ftell(fp4);
	fseek(fp4, 0, SEEK_SET);
	CrxFileBuffer4 = new unsigned char[CrxFileSize4];
	fread(CrxFileBuffer4, CrxFileSize4, 1, fp4);
	fclose(fp4);

	FILE* fp5 = fopen("CRX_PACK_FILE\\09.CRX", "rb");
	fseek(fp5, 0, SEEK_END);
	CrxFileSize5 = ftell(fp5);
	fseek(fp5, 0, SEEK_SET);
	CrxFileBuffer5 = new unsigned char[CrxFileSize5];
	fread(CrxFileBuffer5, CrxFileSize5, 1, fp5);
	fclose(fp5);

	FILE* fp6 = fopen("CRX_PACK_FILE\\10.CRX", "rb");
	fseek(fp6, 0, SEEK_END);
	CrxFileSize6 = ftell(fp6);
	fseek(fp6, 0, SEEK_SET);
	CrxFileBuffer6 = new unsigned char[CrxFileSize6];
	fread(CrxFileBuffer6, CrxFileSize6, 1, fp6);
	fclose(fp6);

	FILE* fp7 = fopen("CRX_PACK_FILE\\11.CRX", "rb");
	fseek(fp7, 0, SEEK_END);
	CrxFileSize7 = ftell(fp7);
	fseek(fp7, 0, SEEK_SET);
	CrxFileBuffer7 = new unsigned char[CrxFileSize7];
	fread(CrxFileBuffer7, CrxFileSize7, 1, fp7);
	fclose(fp7);

	FILE* PCK = fopen("CALLENDER.PCK", "rb");
	fseek(PCK, 0, SEEK_END);
	pSize = ftell(PCK);
	fseek(PCK, 0, SEEK_SET);
	pBuff = new unsigned char[pSize];
	fread(pBuff, pSize, 1, PCK);
	fclose(PCK);
	CopyOutCrxFileToPckFile();
	PCK = fopen("CALLENDER.CHS", "wb");
	fwrite(pBuff, pSize, 1, PCK);
	fclose(PCK);
}

int main()
{
	FileReader();
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
