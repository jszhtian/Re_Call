#include "Common.h"
#include "XWCHAR.h"
#include "ErrorLog.h"
#include <string>
#include <cstdio>

using namespace std;

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

class _declspec(dllimport) AVGHD
{
	public:
	
	AVGHD() :
		bSaveTempData (false),
		CmdUnicode    (false)
	{};
	
	~AVGHD(){};
	
	int Exec(const TCHAR *Package, bool pSaveTempDataz, DWORD dwCmdLine);
	
	protected:
	//封包后缀 
	string PackExtension(){ return "arc"; }
	//封包特殊码
	string PackSign()     { return "pnap"; }
	
	private:
	///
	bool ExtractPnap(wchar_t* filename, unsigned char* buff, DWORD len);
	
	BOOL bSaveTempData;
	BOOL CmdUnicode;
};

bool AVGHD::ExtractPnap(wchar_t* filename, unsigned char* buff, unsigned long len)
{
	if (len < 4 || memcmp(buff, "PNAP", 4))
 	{
    	return false;
	}
	string prefix = GetFilePrefix(filename) + "+PNAP";

	PNAPHDR*       hdr     = (PNAPHDR*) buff;
	PNAPENTRY*     entries = (PNAPENTRY*) (hdr + 1);
	unsigned char* p       = (unsigned char*) (entries + hdr->entry_count);

	for (unsigned long i = 0; i < hdr->entry_count; i++)
 	{
		if (!entries[i].length)
		{
			continue;
		}
		string out_filename = prefix + StringFormatA("+%03d", i) + GuessFileExtensionA(p, entries[i].length);    
    	WriteFile32A(out_filename, p, entries[i].length);
		p += entries[i].length;
	}
	return true;
}

int AVGHD::Exec(const TCHAR* pFile, bool pSaveTempDataz, DWORD dwCmdLine)
{
	if(!pFile)
	{
		printf("Failed to load.\n"); 
		return -1;
	}
	FILE *fin = fopen(pFile.c_str(),"rb");
	ARCHDR hdr;
	fread(&hdr, sizeof(hdr), 1, fin);

	unsigned long  toc_len  = hdr.toc_length;
	unsigned char* toc_buff = new unsigned char[toc_len];
	if(!toc_buff)
	{
		delete[] toc_buff;
		fclose(fin);
		ErrorLog("Failed to Alloc memory.");
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
		
		XWCHAR filenamew (filename_wc);
    	if (!process_pnap(filename_wc, buff, len))
		{
			if (filenamew.find(L".mos"))
			{
        		filenamew += XWCHAR(GuessFileExtensionA(buff, len));
			}
			WriteFile32W(filenamew, buff, len);
    	}

		delete [] buff;
	}
	delete [] toc_buff;
	fclose(fin);
	return 0;
}
