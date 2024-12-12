#include "stdafx.h"

#include "AVDHD.h"
#include <cstdio>
//#include <Windows.h>

#include <string>

using namespace std;

static wstring path;

struct ARCHDR
{
	unsigned long entry_count;
	unsigned long toc_length;
};

struct ARCENTRY
{
	unsigned long length;
	unsigned long offset;
  //wchar_t       filename[1];
};

struct PNAPHDR
{
	unsigned char signature[4]; // "PNAP"
	unsigned long unknown1;
	unsigned long width;
	unsigned long height;
	unsigned long entry_count;
};

struct PNAPENTRY
{
	unsigned long unknown1;
	unsigned long index;
	unsigned long offset_x;
	unsigned long offset_y;
	unsigned long width;
	unsigned long height;
	unsigned long unknown2;
	unsigned long unknown3;
	unsigned long unknown4;
 	unsigned long length;
};

_declspec (dllexport)
void ShowInfo()
{
	printf("AVDHD ARC Tool\n");
	printf("X'moe Project\n");
}

_declspec (dllexport)
BOOL Engine(TCHAR *pszStr)
{
	
	BOOL ret =  0 == _tcsicmp(pszStr, L"AVDHD")? TRUE:FALSE;
	return ret;
}

_declspec (dllexport)
BOOL Match(TCHAR *pszStr, TCHAR* szFile)
{
	BOOL ret =  0 == _tcscmp(pszStr, L"arc")? TRUE:FALSE;
	BOOL ret2 = FALSE;

	FILE *fin = NULL;
	do
	{
		fin = _wfopen(szFile, L"rb");
		if(fin == NULL)
			break;
		
		char sample[4];
		try
		{
			fread(sample, 1, 4, fin);
		}
		catch(...)
		{
			break;
		}
		if(!memcmp(sample, "pnap", 4))
			ret2 = TRUE;
	}while(false);

	fclose(fin);

	/*
	if(ret == FALSE && ret2 == FALSE)
	{
		printf("Not matchs to FVP!\n");
	}
	*/
	return ret || ret2;
}


BOOL CreatePath(const wchar_t *str = NULL)
{
	if(!str)
		return FALSE;

	SECURITY_ATTRIBUTES attribute;
    attribute.nLength = sizeof(attribute);
    attribute.lpSecurityDescriptor = NULL;
    attribute.bInheritHandle = FALSE;
    //´´½¨
    if(CreateDirectoryW(str, &attribute) == 0)
	{
		return FALSE;
	}
	return TRUE;
}


wstring GetPackageName(wstring& fileName)
{
	wstring temp(fileName);
	wstring::size_type pos = temp.find_last_of(L"\\");

	if (pos != wstring::npos)
	{
    	temp = temp.substr(pos+1,temp.length());
  	}

	wstring temp2(temp);
	wstring::size_type pos2 = temp2.find_last_of(L"\\");
	if (pos2 != wstring::npos)
	{
    	temp2 = temp2.substr(pos+1,temp2.length());
  	}
  	return temp2;
}


int CALLBACK WriteFile32W(const wchar_t *filename, const unsigned char *buff, const unsigned long len)
{
	wstring ans = path + filename;
	FILE *fin = _wfopen(ans.c_str(), L"wb");
	fwrite(buff, len, 1, fin);
	fclose(fin);
	return 0;
}

int CALLBACK WriteFile32W(const std::wstring& filename, const unsigned char *buff, const unsigned long len)
{
	wstring ans = path + filename;
	FILE *fin = _wfopen(ans.c_str(), L"wb");
	fwrite(buff, len, 1, fin);
	fclose(fin);
	return 0;
}

std::wstring StringFormatW(const wchar_t *fmt, int i)
{
	wchar_t tmp[512];
	wmemset(tmp, 0, sizeof(tmp)/sizeof(wchar_t));
	wsprintf(tmp, fmt, i);

	std::wstring ret(tmp);
	return ret;
}

std::wstring GuessFileExtensionW(const unsigned char* buff, unsigned int len)
{
	if(!memcmp(buff, "OggS", 4))
		return L".ogg";
	else if(!memcmp(buff, "RIFF", 4))
		return L".wav";
	return 0;
}

bool process_pnap(wchar_t* filename, unsigned char* buff, unsigned long len)
{
	if (len < 4 || memcmp(buff, "PNAP", 4))
 	{
    	return false;
	}
	wstring prefix = wstring(filename) + L"+PNAP";

	PNAPHDR*       hdr     = (PNAPHDR*) buff;
	PNAPENTRY*     entries = (PNAPENTRY*) (hdr + 1);
	unsigned char* p       = (unsigned char*) (entries + hdr->entry_count);

	for (unsigned long i = 0; i < hdr->entry_count; i++)
 	{
		if (!entries[i].length)
		{
			continue;
		}
		wstring out_filename = prefix + StringFormatW(L"+%03d", i) + GuessFileExtensionW(p, entries[i].length);    
    	WriteFile32W(out_filename, p, entries[i].length);
		p += entries[i].length;
	}
	return true;
}


_declspec (dllexport)
int Exec(const TCHAR *Package, DWORD dwPackParam, DWORD dwCmdLine, const TCHAR *output, const TCHAR *args)
{
	if(!Package)
	{
		printf("Failed to load.\n"); 
		return -1;
	}

	wstring _pack((wchar_t*)Package);
	wstring packName = GetPackageName(_pack);

	if(!PathFileExistsW(packName.c_str()))
	{
		if(CreatePath(packName.c_str()))
		{
			path = packName + L"//";
		}
		else
		{
			wprintf(L"Failed to Create Path %s\n", packName.c_str());
		}
	}

	FILE *fin = _wfopen(Package, L"rb");
	ARCHDR hdr;
	fread(&hdr, sizeof(hdr), 1, fin);

	unsigned long  toc_len  = hdr.toc_length;
	unsigned char* toc_buff = new unsigned char[toc_len];
	if(!toc_buff)
	{
		delete[] toc_buff;
		fclose(fin);
		printf("Failed to Alloc memory.\n");
		return -1;
	}
	fread(toc_buff, toc_len, 1, fin);

	unsigned long  data_base = sizeof(hdr) + toc_len;
	unsigned char* p         = toc_buff;

	for (unsigned long i = 0; i < hdr.entry_count; i++)
 	{
		ARCENTRY* entry = (ARCENTRY*) p;
    	p += sizeof(*entry);

    	wchar_t* filename_wc = (wchar_t*)p;
    	//string   filename    = as::convert_wchar(filename_wc);
    	p += (wcslen(filename_wc) + 1) * 2;

		unsigned long  len  = entry->length;
		unsigned char* buff = new unsigned char[len];
    	_fseeki64(fin, data_base + entry->offset, SEEK_SET);
    	fread(buff, len, 1, fin);
		
		wstring filenamew (filename_wc);
    	if (!process_pnap(filename_wc, buff, len))
		{
			if (filenamew.find(L".mos"))
			{
        		filenamew += GuessFileExtensionW(buff, len);
			}
			WriteFile32W(filenamew, (const unsigned char*)buff, len);
    	}

		delete [] buff;
	}
	delete [] toc_buff;
	fclose(fin);
	return 0;
}
