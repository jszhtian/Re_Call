#include "ImageEx.h"
#include <string>
#include <vector>

std::string UnicodeToUtf8(const wchar_t* buf)  
{  
    int len = ::WideCharToMultiByte(CP_UTF8, 0, buf, -1, NULL, 0, NULL, NULL); 
    if (len == 0) return "";  
  
    std::vector<char> utf8(len);  
    ::WideCharToMultiByte(CP_UTF8, 0, buf, -1, &utf8[0], len, NULL, NULL);  
  
    return &utf8[0];  
}

_declspec (dllexport)
bool ReSaveImage(wchar_t* filename, char* buff, unsigned long len)
{
	if(!buff || !len)
		return false;
	
	SDL_RWops* fp = NULL;
	fp = SDL_RWFromMem(buff, len);
	if(fp == NULL)
	{
		printf("ImageEx: Failed to read memory\n");
		if(fp)
		{
			SDL_RWclose(fp);
		}
		return false;
	}
	SDL_Surface *back = nullptr;
	back = SDL_LoadBMP_RW(fp, 0);
	if(back == nullptr)
	{
		printf("ImageEx: Failed to load from memory\n");
		if(fp)
		{
			SDL_RWclose(fp);
		}
		return false;
	}

	BITMAPINFOHEADER *bmi = (BITMAPINFOHEADER*)(buff + sizeof(BITMAPFILEHEADER));


	Uint32 rmask, gmask, bmask, amask;

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
#else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
#endif

	SDL_Surface *out = SDL_CreateRGBSurface(0, back->w, back->h, bmi->biBitCount,rmask, gmask, bmask, amask);

	SDL_Rect  backRect;
	backRect.x = 0;
	backRect.y = 0;
	backRect.w = back->w;
	backRect.h = back->h;

	SDL_BlitSurface(back, &backRect, out, NULL);

	std::string OutPut = UnicodeToUtf8(filename);
	SDL_SaveBMP(out, OutPut.c_str());
	SDL_FreeSurface(back);
	SDL_RWclose(fp);
	return true;
}


_declspec (dllexport) bool ReSaveImageA(char* filename, char* buff, unsigned long len)
{
	if(!buff || !len)
		return false;
	
	SDL_RWops* fp = NULL;
	fp = SDL_RWFromMem(buff, len);
	if(fp == NULL)
	{
		printf("ImageEx: Failed to read memory\n");
		if(fp)
		{
			SDL_RWclose(fp);
		}
		return false;
	}
	SDL_Surface *back = nullptr;
	back = SDL_LoadBMP_RW(fp, 0);
	if(back == nullptr)
	{
		printf("ImageEx: Failed to load from memory\n");
		if(fp)
		{
			SDL_RWclose(fp);
		}
		return false;
	}

	BITMAPINFOHEADER *bmi = (BITMAPINFOHEADER*)(buff + sizeof(BITMAPFILEHEADER));


	Uint32 rmask, gmask, bmask, amask;

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
#else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
#endif

	SDL_Surface *out = SDL_CreateRGBSurface(0, back->w, back->h, bmi->biBitCount,rmask, gmask, bmask, amask);

	SDL_Rect  backRect;
	backRect.x = 0;
	backRect.y = 0;
	backRect.w = back->w;
	backRect.h = back->h;

	SDL_BlitSurface(back, &backRect, out, NULL);

	SDL_SaveBMP(out, filename);
	SDL_FreeSurface(back);
	SDL_RWclose(fp);
	return true;
}
