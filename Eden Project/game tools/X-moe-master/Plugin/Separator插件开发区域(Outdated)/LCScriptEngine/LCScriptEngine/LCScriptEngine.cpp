// LCScriptEngine.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "LCScriptEngine.h"
#include <cstdio>

wstring GetPackageName(wstring& fileName)
{
	wstring temp(fileName);
	wstring::size_type pos = temp.find_last_of(L"\\");

	if (pos != wstring::npos)
	{
		temp = temp.substr(pos + 1, temp.length());
	}

	wstring temp2(temp);
	wstring::size_type pos2 = temp2.find_last_of(L"\\");
	if (pos2 != wstring::npos)
	{
		temp2 = temp2.substr(pos + 1, temp2.length());
	}
	return temp2;
}

_declspec (dllexport) void ShowInfo()
{
	printf("LC-ScriptEngine Tool(No Extension)\n");
	printf("X'moe Project\n");
}

_declspec (dllexport) BOOL Match(TCHAR *pszStr, TCHAR *pFile)
{
	//Do Nothing
	//There are no special data can be used as signature
	//Return false always
	return FALSE;
}

extern "C" _declspec (dllexport) BOOL Engine(TCHAR *pszStr)
{
	BOOL ret1 = 0 == _tcsicmp(pszStr, L"LC-ScriptEngine") ? TRUE : FALSE;
	BOOL ret2 = 0 == _tcsicmp(pszStr, L"LCScriptEngine") ? TRUE : FALSE;

	return ret1 || ret2;
}

BOOL CreatePath(const wchar_t *str = NULL)
{
	if (!str)
		return FALSE;

	SECURITY_ATTRIBUTES attribute;
	attribute.nLength = sizeof(attribute);
	attribute.lpSecurityDescriptor = NULL;
	attribute.bInheritHandle = FALSE;
	//创建
	if (CreateDirectoryW(str, &attribute) == 0)
	{
		return FALSE;
	}
	return TRUE;
}

int CovtUniToASCII(UINT cp, char* ASCIIBuf, const wchar_t* UniStr, int nSize)
{
	WideCharToMultiByte(cp, 0, UniStr, -1, ASCIIBuf, nSize, NULL, 0);
	return 0;
}


int CovtASCIIToUni(UINT cp, const char* ASCIIBuf, wchar_t* UniStr, int nSize)
{
	MultiByteToWideChar(cp, 0, ASCIIBuf, -1, UniStr, nSize);
	return 0;
}


enum CMDMask
{
	UsingUnicode = 0x1UL, //文件名会被转码,需要配合ForceXXX使用
	SaveTempData = 2,
	ForceCHSCP = 4, //源是gbk
	ForceJPCP = 8,  //源是sjis
	ExtractSingle = 16, //提取单独的文件
	UsingImageEx = 32 //强制混合32-bit的alpha通道
};

_declspec (dllexport)
int Exec(const TCHAR *Package, DWORD dwPackParam, DWORD dwCmdLine, const TCHAR *output, const TCHAR *args)
{
	bool UnicodeCmd = false;
	static u32 xor_magic;
	if (dwCmdLine & UsingUnicode)
	{
		UnicodeCmd = true;
	}
	int CP = ForceJPCP;
	if (dwCmdLine & ForceCHSCP)
	{
		CP = ForceCHSCP;
	}

	
	static wstring path;
	wstring _pack((wchar_t*)Package);
	wstring packName = GetPackageName(_pack);

	if (wcsstr(packName.c_str(), L".lst"))
	{
		MessageBoxW(NULL, L"You Must Input A Package File Rather Than A List File!",
			L"LC-ScriptEngine", MB_OK);
		return -1;
	}

	wprintf(L"Path : %s\n", packName.c_str());

	if (!PathFileExistsW(packName.c_str()))
	{
		if (CreatePath(packName.c_str()))
		{
			path = packName + L"//";
		}
		else
		{
			wprintf(L"Failed to Create Path %s\n", packName.c_str());
		}
	}

	FILE *file = _wfopen(_pack.c_str(), L"rb");
	if (file == NULL)
	{
		wprintf(L"Cannot open file : %s\n", _pack.c_str());
		return 0;
	}

	wstring ListFile = _pack + L".lst";
	FILE* lst = _wfopen(ListFile.c_str(), L"rb");
	if (lst == NULL)
	{
		wprintf(L"Cannot open list file : %s\n", ListFile.c_str());
		return -1;
	}

	lst_header_t lst_header;
	lst_entry_t *index_buffer;
	unsigned int index_buffer_len;



	fseek(lst, 0, SEEK_SET);
	fread(&lst_header, 1, sizeof(lst_header), lst);
	fseek(lst, 4, SEEK_SET);
	fread(&xor_magic, 1, 4, lst);

	//xor_magic = 0x01010101;
	lst_header.index_entries ^= xor_magic;

	printf("Count : %08x\n", lst_header.index_entries);

	fseek(lst, 4, SEEK_SET);
	index_buffer_len = lst_header.index_entries * sizeof(lst_entry_t);
	index_buffer = (lst_entry_t*)malloc(index_buffer_len);

	fread(index_buffer, 1, index_buffer_len, lst);

	for (int i = 0; i < lst_header.index_entries; i++)
	{
		int str_len = strlen((char *)index_buffer[i].name);
		for (int j = 0; j < str_len; ++j)
			index_buffer[i].name[j] ^= (unsigned char)(0xFF & xor_magic);

		printf("Found File : %s\n", index_buffer[i].name);
		//getchar();
		index_buffer[i].offset ^= xor_magic;
		index_buffer[i].length ^= xor_magic;
		// for type == -1
		if (index_buffer[i].type < 0)
			index_buffer[i].type = -index_buffer[i].type;
	}
	fclose(lst);

	wprintf(L"Try to dumping...\n");
	for (int i = 0; i < lst_header.index_entries; i++)
	{
		fseek(file, index_buffer[i].offset, SEEK_SET);
		unsigned char* raw_data = new unsigned char[index_buffer[i].length];
		fread(raw_data, 1, index_buffer[i].length, file);

		if (index_buffer[i].type == 1)
		{
			/* 文本定位是在前半部分的指令区，由11 00 00 00 02 00 00 00 xx xx xx xx 指令标示，
			* xx是偏移值（从0开始）
			*/
			for (unsigned int j = 0; j < index_buffer[i].length; ++j)
				raw_data[j] ^= xor_magic + 1;
		}
		FILE* out = nullptr;
		if (!UnicodeCmd)//multibyte
		{
			string outFile;
			char szBuf[260] = { 0 };
			CovtUniToASCII(GetACP(), szBuf, path.c_str(), 260);
			outFile = szBuf;
			outFile += index_buffer[i].name;
			outFile += suffixA[index_buffer[i].type];
			out = fopen(outFile.c_str(), "wb");
		}
		else
		{
			wstring outFile;
			if (ForceJPCP)
			{
				wchar_t wBuf[260] = { 0 };
				CovtASCIIToUni(932, index_buffer[i].name, wBuf, 260);
				outFile = path;
				outFile += wBuf;
				outFile += suffixW[index_buffer[i].type];
				out = _wfopen(outFile.c_str(), L"wb");

			}
			else
			{
				wchar_t wBuf[260] = { 0 };
				CovtASCIIToUni(936, index_buffer[i].name, wBuf, 260);
				outFile = path;
				outFile += wBuf;
				outFile += suffixW[index_buffer[i].type];
				out = _wfopen(outFile.c_str(), L"wb");
			}
		}
		fwrite(raw_data, 1, index_buffer[i].length, out);
		fclose(out);
		delete[] raw_data;
	}
	fclose(file);

}



