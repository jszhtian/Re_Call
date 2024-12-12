// OPW.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "OPW.h"

_declspec (dllexport) void ShowInfo()
{
	printf("SiglusEngine OPW File\n");
	printf("X'moe Project\n");
}

_declspec (dllexport) BOOL Match(TCHAR *pszStr, TCHAR *pFile)
{
	BOOL ret = 0 == _tcscmp(pszStr, L"owp") ? TRUE : FALSE;
	return ret;
}

_declspec (dllexport) BOOL Engine(TCHAR *pszStr)
{
	BOOL ret = 0 == _tcscmp(pszStr, L"SiglusEngine") ? TRUE : FALSE;
	return ret;
}

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

bool Find(const char* m, const char* n, unsigned long len)
{
	int i = 0, j = 0;
	bool bOK = false;
	while (i < len && j < strlen(n))
	{
		if (m[i] == n[j])
		{
			i++;
			j++;
			if (j + 1 == strlen(n))
			{
				bOK = true;
				break;
			}
		}
		else
		{
			i++;
			j = 0;
		}
	}
	return bOK;
}

wstring CALLBACK GetFilePrefix(const wstring& filename)
{
	wstring temp(filename);
	wstring::size_type pos = temp.find_last_of(L".");

	if (pos != wstring::npos)
	{
		temp = temp.substr(0, pos);
	}
	return temp;
}


_declspec (dllexport)
int Exec(const TCHAR *Package, DWORD dwPackParam, DWORD dwCmdLine, const TCHAR *output, const TCHAR *args)
{
	unsigned long filelong = 0;

	FILE* fin = _wfopen(Package, L"rb");
	if (fin == NULL)
	{
		wprintf(L"Cannot open file : %s\n", Package);
	}

	wstring _pack((wchar_t*)Package);
	wstring packName = GetPackageName(_pack);
	fseek(fin, 0, SEEK_END);
	filelong = ftell(fin);
	rewind(fin);
	char *buffer = new char[filelong];
	fread(buffer, 1, filelong, fin);
	fclose(fin);
	unsigned long iPos = 0;

	map<unsigned char, int> keyNode;
	map<unsigned char, int>::iterator it;
	while (iPos < 16)
	{
		unsigned char Key = (unsigned char)buffer[iPos];
		it = keyNode.find(Key);
		if (it == keyNode.end())
		{
			keyNode.insert(std::make_pair(Key, 1));
		}
		else
		{
			it->second++;
		}
		iPos++;
	}
	int minv = 0;
	unsigned char RealKey = 0; //xor no needed
	for (it = keyNode.begin(); it != keyNode.end(); it++)
	{
		if (it->second > minv)
		{
			minv = it->second;
			RealKey = it->first;
		}
	}
	iPos = 0;
	while (iPos < filelong)
	{
		buffer[iPos] ^= RealKey;
		iPos++;
	}

	FILE *out = NULL;
	if (!memcmp(buffer, "OggS", 4))
	{
		if (Find(buffer, "OpusHead", filelong))
			out = _wfopen(wstring(GetFilePrefix(packName) + L".opus").c_str(), L"wb");
		else
			out = _wfopen(wstring(GetFilePrefix(packName) + L".ogg").c_str(), L"wb");

		fwrite(buffer, 1, filelong, out);
		fclose(out);
	}
	else
	{
		wprintf(L"Not a invaild opw file\n");
	}
	delete[] buffer;
	return 0;
}
