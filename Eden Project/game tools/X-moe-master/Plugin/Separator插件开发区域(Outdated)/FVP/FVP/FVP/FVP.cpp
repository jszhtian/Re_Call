// FVP.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "FVP.h"
#include <string>

static wstring path;
static string  pathA;

#pragma comment(lib,"zlib1.lib")

using namespace std;

int CovtUniToASCII(char* ASCIIBuf , const wchar_t* UniStr, int nSize) 
{
    WideCharToMultiByte(936, 0, UniStr, -1, ASCIIBuf, nSize, NULL, 0); 
    return 0; 
}

BOOL CreatePath(const wchar_t *str = NULL)
{
	if(!str)
		return FALSE;

	SECURITY_ATTRIBUTES attribute;
    attribute.nLength = sizeof(attribute);
    attribute.lpSecurityDescriptor = NULL;
    attribute.bInheritHandle = FALSE;
    //创建
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

int CALLBACK WriteFile32A(const string &filename, const char *buff, const unsigned long len)
{
	string ans = pathA + filename;
	FILE *fin = fopen(ans.c_str(),"wb");
	fwrite(buff, len, 1, fin);
	fclose(fin);
	return 0;
}

int CALLBACK WriteFile32W(const wchar_t *filename, const char *buff, const unsigned long len)
{
	wstring ans = path + filename;
	FILE *fin = _wfopen(ans.c_str(), L"wb");
	fwrite(buff, len, 1, fin);
	fclose(fin);
	return 0;
}


int CALLBACK WriteBMP32A(const string&    filename, unsigned char* buff, unsigned long len,
						 unsigned long    width,
               			 unsigned long    height,
               			 unsigned short   depth)
{
	BITMAPFILEHEADER bmf;
  	BITMAPINFOHEADER bmi;

  	memset(&bmf, 0, sizeof(bmf));
  	memset(&bmi, 0, sizeof(bmi));

  	bmf.bfType     = 0x4D42;
  	bmf.bfSize     = sizeof(bmf) + sizeof(bmi) + len;
  	bmf.bfOffBits  = sizeof(bmf) + sizeof(bmi);

  	bmi.biSize     = sizeof(bmi);
  	bmi.biWidth    = width;
  	bmi.biHeight   = height;
  	bmi.biPlanes   = 1;
  	bmi.biBitCount = depth *8;

	/*
	char pDic[260];
	memset(pDic,0,sizeof(pDic));
	GetCurrentDirectoryA(260, pDic);
  	*/

	string filename_t = pathA + filename;

  	FILE* fp = fopen(filename_t.c_str(), "wb");
  	fwrite(&bmf, sizeof(bmf), 1, fp);
  	fwrite(&bmi, sizeof(bmi), 1, fp);
  	fwrite(buff, len,         1, fp);
  	fclose(fp);
  	return 0;
}

void CALLBACK MakeBMP(unsigned char* buff, unsigned long len, unsigned long    width,
               			 unsigned long    height,
               			 unsigned short   depth,
						 unsigned char* out_buff,
						 unsigned long& out_len)
{
	BITMAPFILEHEADER bmf;
  	BITMAPINFOHEADER bmi;

  	memset(&bmf, 0, sizeof(bmf));
  	memset(&bmi, 0, sizeof(bmi));

  	bmf.bfType     = 0x4D42;
  	bmf.bfSize     = sizeof(bmf) + sizeof(bmi) + len;
  	bmf.bfOffBits  = sizeof(bmf) + sizeof(bmi);

  	bmi.biSize     = sizeof(bmi);
  	bmi.biWidth    = width;
  	bmi.biHeight   = height;
  	bmi.biPlanes   = 1;
  	bmi.biBitCount = depth *8;

	out_len = len+sizeof(bmf) + sizeof(bmi);
	out_buff = new unsigned char[out_len];
	memcpy(out_buff, &bmf, sizeof(bmf));
	memcpy(out_buff + sizeof(bmf), &bmi, sizeof(bmi));
	memcpy(out_buff + sizeof(bmf) + sizeof(bmi), buff, len);
}

int CALLBACK WriteBMP32W(const wchar_t*   filename, unsigned char* buff, unsigned long len,
						 unsigned long    width,
               			 unsigned long    height,
               			 unsigned short   depth)
{
	BITMAPFILEHEADER bmf;
  	BITMAPINFOHEADER bmi;

  	memset(&bmf, 0, sizeof(bmf));
  	memset(&bmi, 0, sizeof(bmi));

  	bmf.bfType     = 0x4D42;
  	bmf.bfSize     = sizeof(bmf) + sizeof(bmi) + len;
  	bmf.bfOffBits  = sizeof(bmf) + sizeof(bmi);

  	bmi.biSize     = sizeof(bmi);
  	bmi.biWidth    = width;
  	bmi.biHeight   = height;
  	bmi.biPlanes   = 1;
  	bmi.biBitCount = depth *8;
  
  	wstring new_filename = path + filename;
  	FILE* fp = _wfopen(new_filename.c_str(), L"wb");
  	fwrite(&bmf, sizeof(bmf), 1, fp);
  	fwrite(&bmi, sizeof(bmi), 1, fp);
  	fwrite(buff, len,         1, fp);
  	fclose(fp);
  	return 0;
}

static string CALLBACK GuessFileExtensionA(const char* buff, unsigned len)
{
	
	char *magic = new char[0x20];
	ZeroMemory(magic, sizeof(magic));
	memcpy(magic, buff, 0x20);
	
	if(!memcmp(magic, "RIFF", 4)) return ".wav";
	else if(!memcmp(magic, "OggS", 4)) return ".ogg";
	else if(!memcmp(magic, "TAG", 3))  return ".mp3";
	//unknown type
	else return NULL;
}

///////////////////////////////////////////////////////////////////////////////////

_declspec (dllexport)
void ShowInfo()
{
	printf("Favorite Bin Tool\n");
	printf("X'moe Project\n");
}

_declspec (dllexport)
BOOL Engine(TCHAR *pszStr)
{
	
	BOOL ret =  0 == _tcsicmp(pszStr, L"FVP")? TRUE:FALSE;
	return ret;
}

_declspec (dllexport)
BOOL Match(TCHAR *pszStr, TCHAR* szFile)
{
	BOOL ret =  0 == _tcscmp(pszStr, L"bin")? TRUE:FALSE;
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
		if(!memcmp(sample, "hzc1", 4))
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

BOOL Match2(const TCHAR* szFile)
{
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
		if(!memcmp(sample, "hzc1", 4))
			ret2 = TRUE;
	}while(false);

	fclose(fin);

	return ret2;
}

bool ExtractNvsg(const string& filename, unsigned char* buff, DWORD len)
{
	if (len < 4 || memcmp(buff, "hzc1", 4))
	{
    	return false;
	}

	HZC1HDR* hzc1hdr         = (HZC1HDR*) buff;
	DWORD    data_len        = len - sizeof(*hzc1hdr);
	unsigned char* data_buff = (unsigned char*) (hzc1hdr + 1);

	if (data_len < 4 || memcmp(data_buff, "NVSG", 4))
	{
    	return false;
	}

	NVSGHDR* nvsghdr = (NVSGHDR*) data_buff;
	data_buff += hzc1hdr->header_length;
  
	DWORD depth = 0;

	switch (nvsghdr->type)
	{
		case NVSGHDR_TYPE_SINGLE_24BIT:
		depth = 3;
		break;

		case NVSGHDR_TYPE_SINGLE_32BIT:
		case NVSGHDR_TYPE_MULTI_32BIT:
		depth = 4;
		break;

		case NVSGHDR_TYPE_SINGLE_8BIT:
		depth = 1;
		break;

		case NVSGHDR_TYPE_SINGLE_1BIT:
		depth = 1;
		break;

		default:
		return false;
	}

	DWORD out_len  = hzc1hdr->original_length;
	unsigned char*  out_buff = new unsigned char[out_len];
	if(bSaveTempData) 
	{
		if(UnicodeCmd)
		{
			string filename_x = filename + ".compressed";
			wchar_t *FileNameW = new wchar_t[512];
			wmemset(FileNameW, 0, sizeof(FileNameW)/sizeof(wchar_t));
			MultiByteToWideChar(932, 0, filename_x.c_str(), -1, FileNameW, -1);
			WriteFile32W(FileNameW, (char*)buff, len);
		}
		else
		{
			string filename_x = filename + ".compressed";
			FILE *rfp = fopen(filename_x.c_str(),"wb");
			fwrite(buff, len, 1, rfp);
			fclose(rfp);
		}
	}
	uncompress(out_buff, &out_len, data_buff, data_len);

	if (nvsghdr->type == NVSGHDR_TYPE_SINGLE_1BIT)
 	{
    	for (DWORD i = 0; i < out_len; i++)
		{
			if (out_buff[i] == 1)
			{
        		out_buff[i] = 0xFF;
      		}
    	}
  	}

	if (nvsghdr->entry_count)
	{
    	DWORD frame_len = nvsghdr->width * nvsghdr->height * depth;
		for (DWORD j = 0; j < nvsghdr->entry_count; j++)
		{
			char index[10];
			memset(index, 0, sizeof(index));
			sprintf(index, "%03d", j);
			string filename2;
			filename2 = filename + index;
			if(!UnicodeCmd)
			{
				if(!bUsingImageEx)
				{
					WriteBMP32A(filename, out_buff + (j * frame_len), frame_len, nvsghdr->width, 0 - nvsghdr->height,
							depth); //need flip
				}
				else
				{
				if(hDLL)
				{
					typedef bool (*pReSaveImageA)(char* filename, char* buff, unsigned long len);
					pReSaveImageA ReSaveImageA = (pReSaveImageA)GetProcAddress(hDLL, "ReSaveImageA" );
					if(ReSaveImageA)
					{
						unsigned char *out_buff = NULL;
						unsigned long  out_len = 0;
						MakeBMP(out_buff + (j * frame_len), frame_len, nvsghdr->width, 0 - nvsghdr->height,
							depth, out_buff, out_len);
						char szFileName[260] = {0};
						StrCpyA(szFileName, filename.c_str());
						ReSaveImageA(szFileName, (char*)out_buff, len);
						delete[] out_buff;
					}
				}
				}
			}
			else
			{
					wchar_t WriteNameW[512];
					wmemset(WriteNameW, 0, sizeof(WriteNameW)/sizeof(wchar_t));
					MultiByteToWideChar(932, 0, filename.c_str(), -1, WriteNameW, -1);
				if(!bUsingImageEx)
				{
					WriteBMP32W(WriteNameW, out_buff + (j * frame_len), frame_len, nvsghdr->width, 0 - nvsghdr->height,
							depth); //need flip
				}
				else
				{
				if(hDLL)
				{
					typedef bool (*pReSaveImageW)(wchar_t* filename, char* buff, unsigned long len);
					pReSaveImageW ReSaveImageW = (pReSaveImageW)GetProcAddress(hDLL, "ReSaveImageW" );
					if(ReSaveImageW)
					{
						unsigned char *out_buff = NULL;
						unsigned long  out_len = 0;
						MakeBMP(out_buff + (j * frame_len), frame_len, nvsghdr->width, 0 - nvsghdr->height,
							depth, out_buff, out_len);
						ReSaveImageW(WriteNameW, (char*)out_buff, len);
						delete[] out_buff;
					}
				}
				}
			}
    	}
  	}
  	else
  	{
  		if(!UnicodeCmd)
  		{
			if(!bUsingImageEx)
			{
				WriteBMP32A(filename + ".bmp", out_buff, out_len, nvsghdr->width, 0 - nvsghdr->height, depth);
			}
			else
			{
				if(hDLL)
				{
					typedef bool (*pReSaveImageA)(char* filename, char* buff, unsigned long len);
					pReSaveImageA ReSaveImageA = (pReSaveImageA)GetProcAddress(hDLL, "ReSaveImageA" );
					if(ReSaveImageA)
					{
						unsigned char *out_buff = NULL;
						unsigned long  out_len = 0;
						MakeBMP(out_buff , out_len, nvsghdr->width, 0 - nvsghdr->height,
							depth, out_buff, out_len);
						char szFileName[260] = {0};
						StrCpyA(szFileName, filename.c_str());
						StrCatA(szFileName, ".bmp");
						ReSaveImageA(szFileName, (char*)out_buff, len);
						delete[] out_buff;
					}
				}
			}
		}
		else
		{
			string filename_t = filename + ".bmp";
			wchar_t WriteNameW2[512];
			wmemset(WriteNameW2, 0, sizeof(WriteNameW2)/sizeof(wchar_t));
			MultiByteToWideChar(932, 0, filename.c_str(), -1, WriteNameW2, -1);

			if(!bUsingImageEx)
			{
				WriteBMP32W(WriteNameW2, out_buff, out_len, nvsghdr->width, 0 - nvsghdr->height, depth);
			}
			else
			{
				if(hDLL)
				{
					typedef bool (*pReSaveImageW)(wchar_t* filename, char* buff, unsigned long len);
					pReSaveImageW ReSaveImageW = (pReSaveImageW)GetProcAddress(hDLL, "ReSaveImageW" );
					if(ReSaveImageW)
					{
						unsigned char *out_buff = NULL;
						unsigned long  out_len = 0;
						MakeBMP(out_buff , out_len, nvsghdr->width, 0 - nvsghdr->height,
							depth, out_buff, out_len);
						ReSaveImageW(WriteNameW2, (char*)out_buff, len);
						delete[] out_buff;
					}
				}
			}
		}

  	}

	delete [] out_buff;
	return true;
}


bool ExtractNvsg2(const wchar_t *filename, unsigned char* buff, DWORD len)
{
	if (len < 4 || memcmp(buff, "hzc1", 4))
	{
    	return false;
	}

	HZC1HDR* hzc1hdr         = (HZC1HDR*) buff;
	DWORD    data_len        = len - sizeof(*hzc1hdr);
	unsigned char* data_buff = (unsigned char*) (hzc1hdr + 1);

	if (data_len < 4 || memcmp(data_buff, "NVSG", 4))
	{
    	return false;
	}

	NVSGHDR* nvsghdr = (NVSGHDR*) data_buff;
	data_buff += hzc1hdr->header_length;
  
	DWORD depth = 0;

	switch (nvsghdr->type)
	{
		case NVSGHDR_TYPE_SINGLE_24BIT:
		depth = 3;
		break;

		case NVSGHDR_TYPE_SINGLE_32BIT:
		case NVSGHDR_TYPE_MULTI_32BIT:
		depth = 4;
		break;

		case NVSGHDR_TYPE_SINGLE_8BIT:
		depth = 1;
		break;

		case NVSGHDR_TYPE_SINGLE_1BIT:
		depth = 1;
		break;

		default:
		return false;
	}

	DWORD out_len  = hzc1hdr->original_length;
	unsigned char*  out_buff = new unsigned char[out_len];

	uncompress(out_buff, &out_len, data_buff, data_len);

	if (nvsghdr->type == NVSGHDR_TYPE_SINGLE_1BIT)
 	{
    	for (DWORD i = 0; i < out_len; i++)
		{
			if (out_buff[i] == 1)
			{
        		out_buff[i] = 0xFF;
      		}
    	}
  	}

	if (nvsghdr->entry_count)
	{
    	DWORD frame_len = nvsghdr->width * nvsghdr->height * depth;
		for (DWORD j = 0; j < nvsghdr->entry_count; j++)
		{
			wchar_t index[MAX_PATH*2];
			wmemset(index, 0, sizeof(index)/sizeof(wchar_t));
			swprintf(index, L"%s%03d.bmp",filename, j);

			if(!bUsingImageEx)
			{
				WriteBMP32W(index, out_buff + (j * frame_len), frame_len, nvsghdr->width, 0 - nvsghdr->height,
							depth); //need flip
			}
			else
			{
				if(hDLL)
				{
					typedef bool (*pReSaveImageW)(wchar_t* filename, char* buff, unsigned long len);
					pReSaveImageW ReSaveImageW = (pReSaveImageW)GetProcAddress(hDLL, "ReSaveImageW" );
					if(ReSaveImageW)
					{
						unsigned char *out_buff = NULL;
						unsigned long  out_len = 0;
						MakeBMP(out_buff + (j * frame_len), frame_len, nvsghdr->width, 0 - nvsghdr->height,
							depth, out_buff, out_len);
						ReSaveImageW(index, (char*)out_buff, len);
						delete[] out_buff;
					}
				}
				else
				{
					printf("Failed to load ImageEx.dll\n");
					WriteBMP32W(index, out_buff + (j * frame_len), frame_len, nvsghdr->width, 0 - nvsghdr->height,
							depth); //need
				}
			}
    	}
  	}
  	else
  	{
			wchar_t index[MAX_PATH*2];
			wmemset(index, 0, sizeof(index)/sizeof(wchar_t));
			swprintf(index, L"%s.bmp",filename);
			WriteBMP32W(index, out_buff, out_len, nvsghdr->width, 0 - nvsghdr->height, depth);
  	}

	delete [] out_buff;
	return true;
}

_declspec (dllexport)
int Exec(const TCHAR *Package, DWORD dwPackParam, DWORD dwCmdLine, const TCHAR *output, const TCHAR *args)
{
	//printf("running\n");
	if(dwCmdLine & SaveTempData)
	{
		bSaveTempData = true;
	}
	if(dwCmdLine & UsingUnicode)
	{
		UnicodeCmd = true;
	}

	if(dwCmdLine & UsingImageEx)
	{
		bUsingImageEx = TRUE;
		hDLL = LoadLibrary(L"ImageEx.dll");
	}

	wstring _pack((wchar_t*)Package);
	wstring packName = GetPackageName(_pack);

	if(Match2(Package))
	{
		printf("dumping FVP image[%s]\n");
		FILE *out = _wfopen(Package, L"rb");
		fseek(out,0,SEEK_END);
	    DWORD FileSize=ftell(out);
	    rewind(out);
	    char *pFile=new char [FileSize];
	    fread(pFile,FileSize,1,out);
	    fclose(out);
		ExtractNvsg2(packName.c_str(), (unsigned char*)pFile, FileSize);
		return 0;
	}

	wprintf(L"Path : %s\n", packName.c_str());

	if(!PathFileExistsW(packName.c_str()))
	{
		if(CreatePath(packName.c_str()))
		{
			path = packName + L"//";
			char aBuf[MAX_PATH];
			memset(aBuf, 0, sizeof(aBuf));
			CovtUniToASCII(aBuf , path.c_str(), MAX_PATH);
			pathA = aBuf;
		}
		else
		{
			wprintf(L"Failed to Create Path %s\n", packName.c_str());
		}
	}
	wstring in_filename = Package;
	FILE* fd = _wfopen(in_filename.c_str(), L"rb"); 

	BINHDR hdr;
	fread(&hdr, 1, sizeof(hdr), fd);

	BINENTRY* entries = new BINENTRY[hdr.entry_count];
	fread(entries, 1, sizeof(BINENTRY) * hdr.entry_count, fd);

	DWORD filenames_len  = hdr.filenames_length;
	unsigned char* filenames_buff = new unsigned char[filenames_len];
	fread(filenames_buff, 1, filenames_len, fd);

	for (DWORD i = 0; i < hdr.entry_count; i++)
 	{
		string filename = (char*) (filenames_buff + entries[i].filename_offset);

		DWORD len  = entries[i].length;
		unsigned char*  buff = new unsigned char[len];
		fseek(fd, entries[i].offset, SEEK_SET);
		fread(buff, 1, len, fd);

		printf("Extracting [%s]\n", filename.c_str());

		if (!ExtractNvsg(filename, buff, len))
		{
			string FileBuffer = filename + GuessFileExtensionA((char*) buff, len);
			if(!UnicodeCmd)
			{
				WriteFile32A(FileBuffer, (char*) buff, len);
			}
			else
			{
				wchar_t *FileNameW = new wchar_t[512];
				wmemset(FileNameW, 0, sizeof(FileNameW)/sizeof(wchar_t));
				MultiByteToWideChar(932, 0, FileBuffer.c_str(), -1, FileNameW, -1);
				WriteFile32W(FileNameW, (char*)buff, len);
			}
			
		}

    	delete [] buff;
	}

	delete [] filenames_buff;
	delete [] entries;
	fclose(fd);
	if(hDLL)
	{
		FreeLibrary(hDLL);
	}
	return 0;
}

