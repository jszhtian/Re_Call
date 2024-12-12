// SiglusEngineText.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "SiglusEngineText.h"

_declspec (dllexport) void ShowInfo()
{
	printf("Text Dumper For SiglusEngine\n");
	printf("X'moe Project");
}

_declspec (dllexport) BOOL Match(TCHAR *pszStr, TCHAR *pFile)
{
	//用户自定义的ss
	//实际上是没有后缀名的 *.ss后缀名是根据exe得出的
	BOOL ret = 0 == _tcsicmp(pszStr, L"ss") ? TRUE : FALSE;
	return ret;
}

_declspec (dllexport) BOOL Engine(TCHAR *pszStr)
{
	BOOL ret = 0 == _tcsicmp(pszStr, L"SiglusEngine") ? TRUE : FALSE;
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


void decrypt_string(wchar_t* str_buf, wchar_t* new_buf, int length, int key)
{
	key *= 0x7087;
	for (int i = 0; i<length; i++)
	{
		new_buf[i] = str_buf[i] ^ key;
	}
}

_declspec (dllexport)
int Exec(const TCHAR *Package, DWORD dwPackParam, DWORD dwCmdLine, const TCHAR *output, const TCHAR *args)
{
	wstring _pack((wchar_t*)Package);
	wstring packName = GetPackageName(_pack);

	setlocale(0, "Japanese");
	FILE* f = _wfopen(Package, L"rb");
	if (f)
	{
		fseek(f, 0, SEEK_END);
		size_t flength = ftell(f);
		fseek(f, 0, SEEK_SET);
		buf = (unsigned char*)malloc(flength);
		fread(buf, flength, 1, f);
		fclose(f);

		//FILE* txt = _wfopen(L"test.txt",L"wb");
		wchar_t new_name[1024];
		wsprintf(new_name, L"%s.txt", packName.c_str());
		FILE* txt = _wfopen(new_name, L"wb");

		SCENEHEADER *sce_header = (SCENEHEADER*)buf;
		PFILE_INFO string_index = (PFILE_INFO)&buf[sce_header->string_index_pair.offset];
		wchar_t* string_data = (wchar_t*)&buf[sce_header->string_data_pair.offset];
		fwrite("\xFF\xFE", 2, 1, txt);
		for (DWORD x = 0; x<sce_header->string_index_pair.count; x++)
		{
			PFILE_INFO info = &string_index[x];
			wchar_t* info_str = &string_data[info->offset];
			wchar_t* new_str = new wchar_t[info->length * 2];
			memset(new_str, 0, sizeof(wchar_t)* info->length * 2);

			decrypt_string(info_str, new_str, info->length, x);
			for (int i = 0; i<info->length; i++)
			{
				fwprintf(txt, L"%c", new_str[i]);
			}
			fwprintf(txt, L"\r\n");
			delete new_str;
		}
	}
	return 0;
}
