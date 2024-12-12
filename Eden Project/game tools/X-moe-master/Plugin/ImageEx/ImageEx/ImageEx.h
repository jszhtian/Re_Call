#ifndef __ImageEx_H__
#define __ImageEx_H__

#include "SDL\SDL.h"
#include "SDL\SDL_image.h"
#include <Windows.h>
#include <cstdio>

#pragma comment(lib, "SDL2.lib")
#pragma comment(lib, "SDL2_image.lib")

extern "C" _declspec (dllexport) bool ReSaveImageW(wchar_t* filename, char* buff, unsigned long len);

extern "C" _declspec (dllexport) bool ReSaveImageA(char* filename, char* buff, unsigned long len);

#endif
