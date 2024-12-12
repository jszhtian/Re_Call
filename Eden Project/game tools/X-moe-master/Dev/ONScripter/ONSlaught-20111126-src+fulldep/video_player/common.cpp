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

#include "common.h"
#include "font.h"
#include <cstdio>

void blit_font(SDL_Surface *dst,const char *string,ulong x,ulong y){
	if (dst->format->BitsPerPixel!=24 && dst->format->BitsPerPixel!=32)
		return;
	//unpack font
	ulong n=strlen(string);
	uchar *buffer=new uchar[n*8*8]; //width*height*depth
	for (ulong a=0;a<n;a++){
		const uchar *glyph=GFX_font+string[a]*8;
		uchar *pixel=buffer+a*8*8;
		for (ulong b=0;b<8;b++,glyph++){
			ulong c=*glyph;
			for (int d=0;d<8;d++)
				*pixel++=((c<<=1)&0x100)?0xFF:0;
		}
	}
	//blit unpacked font
	SDL_LockSurface(dst);
	surfaceData sd=dst;
	ulong pos[2]={0};
	const ulong multiplier=2;
	for (ulong a=0;a<n;a++){
		ulong offset[2]={
			x+pos[0]*8*multiplier,
			y+pos[1]*8*multiplier
		};
		uchar *dst=sd.pixels+(offset[0]*sd.advance)+(offset[1]*sd.pitch);
		uchar *src=buffer+a*8*8;
		if (string[a]=='\n'){
			pos[0]=0;
			pos[1]++;
		}else{
			if (offset[0]+8*multiplier<sd.w && offset[1]+8*multiplier<sd.h){
				for (ulong b=0;b<8;b++){
					uchar *row=dst+b*sd.pitch*multiplier;
					for (ulong c=0;c<8;c++){
						for (ulong d=0;d<multiplier;d++){
							for (ulong e=0;e<multiplier;e++){
								row[e*sd.advance+d*sd.pitch+sd.Roffset]=*src;
								row[e*sd.advance+d*sd.pitch+sd.Goffset]=*src;
								row[e*sd.advance+d*sd.pitch+sd.Boffset]=*src;
								if (sd.alpha)
									row[e*sd.advance+d*sd.pitch+sd.Aoffset]=*src;
							}
						}
						row+=sd.advance*multiplier;
						src++;
					}
				}
			}
			pos[0]++;
		}
	}
	SDL_UnlockSurface(dst);
	delete[] buffer;
}

void blit_font_helper(SDL_Overlay *dst,ulong x,ulong y,ulong plane,uchar *buffer,ulong character,ulong pos[2]){
	ulong w,h;
	ulong multiplier;
	if (!plane){
		w=dst->w;
		h=dst->h;
		multiplier=2;
	}else{
		w=dst->w/2;
		h=dst->h/2;
		multiplier=1;
	}
	ulong offset[2]={
		x+pos[0]*8*multiplier,
		y+pos[1]*8*multiplier
	};
	ulong pitch=dst->pitches[plane];
	uchar *dst0=(uchar *)dst->pixels[plane]+offset[0]+offset[1]*pitch;
	uchar *src=buffer+character*8*8;
	if (offset[0]+8*multiplier<w && offset[1]+8*multiplier<h){
		for (ulong b=0;b<8;b++){
			uchar *row=dst0+b*pitch*multiplier;
			for (ulong c=0;c<8;c++){
				if (!plane){
					for (ulong d=0;d<multiplier;d++)
						for (ulong e=0;e<multiplier;e++)
							row[e+d*pitch]=src[0];
				}else
					*row=0x80;
				row+=multiplier;
				src++;
			}
		}
	}
}

void blit_font(SDL_Overlay *dst,const char *string,ulong x,ulong y){
	if (dst->format!=SDL_YV12_OVERLAY)
		return;
	//unpack font
	ulong n=strlen(string);
	uchar *buffer=new uchar[n*8*8];
	for (ulong a=0;a<n;a++){
		const uchar *glyph=GFX_font+string[a]*8;
		uchar *pixel=buffer+a*8*8;
		for (ulong b=0;b<8;b++,glyph++){
			ulong c=*glyph;
			for (int d=0;d<8;d++)
				*pixel++=((c<<=1)&0x100)?0xFF:0;
		}
	}
	//blit unpacked font
	SDL_LockYUVOverlay(dst);
	ulong pos[2]={0};
	for (ulong a=0;a<n;a++){
		if (string[a]=='\n'){
			pos[0]=0;
			pos[1]++;
		}else{
			blit_font_helper(dst,x,y,0,buffer,a,pos);
			blit_font_helper(dst,x,y,1,buffer,a,pos);
			blit_font_helper(dst,x,y,2,buffer,a,pos);
			pos[0]++;
		}
	}
	SDL_UnlockYUVOverlay(dst);
	delete[] buffer;
}

const char *seconds_to_time_format(double seconds){
	ulong hours,minutes,seconds_int;
	hours=ulong(seconds)/3600;
	seconds-=hours*3600;
	minutes=ulong(seconds)/60;
	seconds-=minutes*60;
	seconds_int=(ulong)seconds;
	seconds-=seconds_int;
	char temp[10];
	static char res[1000];
	sprintf(temp,"%#.3f",seconds);
	sprintf(res,"%02u:%02u:%02u%s",(unsigned)hours,(unsigned)minutes,(unsigned)seconds_int,temp+1);
	return res;
}
