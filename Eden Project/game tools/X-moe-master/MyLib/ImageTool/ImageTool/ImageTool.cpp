// ImageTool.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <string>
#include <windows.h>
#include "SDL\SDL.h"
#include "SDL\SDL_image.h"

#include <vector>

#pragma comment(lib, "SDL2.lib")
#pragma comment(lib, "SDL2_image.lib")

using namespace std;

typedef struct ImageBuffer
{
	SDL_Surface *image;
	int x;
	int y;

	ImageBuffer(): image(nullptr), x(0), y(0){}
};

#ifdef WIN32
#undef main
#endif

enum{PNG_TYPE = 0, BMP_TYPE = 1};

//#define _Debug

int main(int argc, char **argv)
{
	vector<ImageBuffer> pImageBuffer;
	if(argc <7 )
	{
		wprintf(L"基佬♂专用的图片工具\nX'moe 2014\n");
		printf("Usage: Image Type(--bmp for bmp format, --png for png format)\n"
			   //"Default: png format\n"
			   "BackGround <ForeGround Count> <ForeGround PosX PosY...> Output\n");
		return -1;
	}

	if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		printf("Failed to init core parts![%s]\n", SDL_GetError());
		return -3;
	}
	IMG_Init(IMG_INIT_PNG|IMG_INIT_JPG);

	int OutPutArgs = PNG_TYPE;
	string OutputType(argv[1]);
	if(!OutputType.compare("--bmp"))
	{
		OutPutArgs = BMP_TYPE;
	}
	else if(!OutputType.compare("--png"))
	{
		OutPutArgs = PNG_TYPE;
	}
	else
	{
		OutPutArgs = PNG_TYPE;
	}

	string BaseFile = string(argv[2]);

	int ForeCount = 0;
	try
	{
		ForeCount  = atoi(argv[3]);
	}
	catch(...)
	{
		printf("Bad Input![2]\n");
		return -2;
	}

	unsigned int iPos = 4;

	int    Parts_x;
	int    Parts_y;
	while(ForeCount--)
	{
		SDL_Surface *fore = nullptr;
		string ExpFile  = string(argv[iPos]);
		iPos++;
		try
		{
			Parts_x  = atoi(argv[iPos]);
		}
		catch(...)
		{
			printf("Bad Input!\n");
			return -2;
		}
		iPos++;
		try
		{
			Parts_y  = atoi(argv[iPos]);
		}
		catch(...)
		{
			printf("Bad Input!\n");
			return -2;
		}
		iPos++;

		fore = IMG_Load(ExpFile.c_str());
		if(fore == nullptr)
		{
			printf("Failed to Load %s\n", ExpFile.c_str());
			return -4;
		}
		ImageBuffer pBuffer;
		pBuffer.image = fore;
		pBuffer.x     = Parts_x;
		pBuffer.y     = Parts_y;

		pImageBuffer.push_back(pBuffer);
	}

	string OutPut   = string(argv[iPos]);

	SDL_Surface *back = nullptr;

	back = IMG_Load(BaseFile.c_str());
	if(back == nullptr)
	{
		printf("Failed to Load %s, %s\n", BaseFile.c_str(), IMG_GetError());
		return -4;
	}



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

	SDL_Surface *out = SDL_CreateRGBSurface(0, back->w, back->h, 32 ,rmask, gmask, bmask, amask);

	SDL_Rect  backRect;
	backRect.x = 0;
	backRect.y = 0;
	backRect.w = back->w;
	backRect.h = back->h;

	SDL_BlitSurface(back, &backRect, out, NULL);


	for(vector<ImageBuffer>::iterator it = pImageBuffer.begin(); it != pImageBuffer.end(); it++)
	{
		SDL_Rect foreRect;
		foreRect.x = it->x;
		foreRect.y = it->y;
		foreRect.w = it->image->w;
		foreRect.h = it->image->h;

		SDL_Rect DrawArea;
		DrawArea.x = 0;
		DrawArea.y = 0;
		DrawArea.w = it->image->w;
		DrawArea.h = it->image->h;

		SDL_BlitSurface(it->image, &DrawArea, out, &foreRect);
	}

	if(OutPutArgs == BMP_TYPE)
		SDL_SaveBMP(out, OutPut.c_str());
	else
		IMG_SavePNG(out, OutPut.c_str());

	for(vector<ImageBuffer>::iterator it = pImageBuffer.begin(); it != pImageBuffer.end(); it++)
	{
		SDL_FreeSurface(it->image);
	}

	return 0;
}
