/*
* Copyright (c) 2009, 2010, Helios (helios.vmg@gmail.com)
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*     * Redistributions of source code must retain the above copyright notice,
*       this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in the
*       documentation and/or other materials provided with the distribution.
*     * The name of the author may not be used to endorse or promote products
*       derived from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY HELIOS "AS IS" AND ANY EXPRESS OR IMPLIED
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
* EVENT SHALL HELIOS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
* EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
* PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
* OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
* WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
* OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
* OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef COMMON_H
#define COMMON_H
#include <SDL/SDL.h>

typedef unsigned long ulong;
typedef unsigned char uchar;

void blit_font(SDL_Surface *dst,const char *string,ulong x,ulong y);
void blit_font(SDL_Overlay *dst,const char *string,ulong x,ulong y);
const char *seconds_to_time_format(double seconds);

struct surfaceData{
	uchar *pixels;
	uchar Roffset,
		Goffset,
		Boffset,
		Aoffset;
	ulong advance,
		pitch,
		w,h;
	bool alpha;
	surfaceData(){}
	surfaceData(const SDL_Surface *surface){
		*this=surface;
	}
	const surfaceData &operator=(const SDL_Surface *surface){
		this->pixels=(uchar *)surface->pixels;
		this->Roffset=(surface->format->Rshift)>>3;
		this->Goffset=(surface->format->Gshift)>>3;
		this->Boffset=(surface->format->Bshift)>>3;
		this->Aoffset=(surface->format->Ashift)>>3;
		this->advance=surface->format->BytesPerPixel;
		this->pitch=surface->pitch;
		this->w=surface->w;
		this->h=surface->h;
		this->alpha=(this->Aoffset!=this->Roffset && this->Aoffset!=this->Goffset && this->Aoffset!=this->Boffset);
		return *this;
	}
};
#endif
