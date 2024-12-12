/*
 * wavmix.c WAV ファイルのミックス
 *
 * Copyright (C) 1997-1998 Masaki Chikama (Wren) <chikama@kasumi.ipl.mech.nagoya-u.ac.jp>
 *               1998-                           <masaki-c@is.aist-nara.ac.jp>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "portab.h"
#include "audio.h"

extern void System_error(const char *msg);
extern void System_errorOutOfMemory(const char *msg);

WAVFILE *mixWaveFile(WAVFILE *wfileL, WAVFILE *wfileR) {
	DSPINFO info;
	WAVFILE *wfileM;

	UInt32 rate;
	Chan   channel;
	UInt16 bits;
	int    i;
	UInt32 max_samples;
	UInt32 min_samples;
	
	if (wfileL->wavinfo.SamplingRate != wfileR->wavinfo.SamplingRate) return NULL;
	if (wfileL->wavinfo.Channels     != wfileR->wavinfo.Channels)     return NULL;
	if (wfileL->wavinfo.DataBits     != wfileR->wavinfo.DataBits)     return NULL;

	rate    = info.SamplingRate = wfileL->wavinfo.SamplingRate;
        channel = info.Channels     = Stereo;
	bits    = info.DataBits     = wfileL->wavinfo.DataBits;

	// printf("rate = %d, cahhael = %d, bits = %d\n",rate, channel == Mono ? 1 : 2, bits);
	CheckDSP(&info);
	if (!info.Available)
		channel = Mono;
	
	if (wfileL->wavinfo.Samples > wfileR->wavinfo.Samples) {
		max_samples = wfileL->wavinfo.Samples;
		min_samples = wfileR->wavinfo.Samples;
	} else {
		max_samples = wfileR->wavinfo.Samples;
		min_samples = wfileL->wavinfo.Samples;
	}
	
	// printf("max = %d, min = %d\n",max_samples, min_samples);

	wfileM = (WAVFILE *)malloc(sizeof(WAVFILE));
	if (wfileM == NULL) {
		System_error("mixWavFile(): Out of memory\n");
	}
	
	wfileM->wavinfo.SamplingRate = rate;
	wfileM->wavinfo.Channels     = channel;
	wfileM->wavinfo.Samples      = max_samples;
	wfileM->wavinfo.DataBits     = bits;
	wfileM->wavinfo.DataBytes    = 
	wfileM->wavinfo.DataBytes_o  = max_samples * (channel == Stereo ? 2 : 1) * (bits == 16 ? 2 : 1);
	wfileM->data = (char *)malloc(max_samples * (channel == Stereo ? 2 : 1) * (bits == 16 ? 2 : 1));
	if (wfileM->data == NULL) {
		System_errorOutOfMemory("mixWavFile()");
	}
	switch(bits) {
	case 8:
		if (channel == Mono) {
			for (i = 0; i < min_samples; i++) {
				*(wfileM->data + i) = (*(wfileL->data + i) + *(wfileR->data + i)) >> 1;
			}
			if (min_samples == max_samples) break;
			if (wfileL->wavinfo.Samples > wfileR->wavinfo.Samples) {
				for (i = min_samples; i < max_samples; i++) {
					*(wfileM->data + i) = *(wfileL->data + i);
				}
			} else {
				for (i = min_samples; i < max_samples; i++) {
					*(wfileM->data + i) = *(wfileR->data + i);
				}
			}
		} else {
			for (i = 0; i < min_samples; i++) {
				*(wfileM->data + i * 2    ) = *(wfileL->data + i);
				*(wfileM->data + i * 2 + 1) = *(wfileR->data + i);
			}
			if (min_samples == max_samples) break;
			if (wfileL->wavinfo.Samples > wfileR->wavinfo.Samples) {
				for (i = min_samples; i < max_samples; i++) {
					*(wfileM->data + i * 2    ) = *(wfileL->data + i);
					*(wfileM->data + i * 2 + 1) = 0x80;
				}
			} else {
				for (i = min_samples; i < max_samples; i++) {
					*(wfileM->data + i * 2    ) = 0x80;
					*(wfileM->data + i * 2 + 1) = *(wfileR->data + i);
				}
			}
		}
		break;
	case 16: {
		WORD *srcR = (WORD *)wfileR->data;
		WORD *srcL = (WORD *)wfileL->data;
		WORD *dst  = (WORD *)wfileM->data;
		
		if (channel == Mono) {
			for (i = 0; i < min_samples; i++) {
				*(dst + i) = (*(srcL + i) + *(srcR + i)) >> 1;
			}
			if (min_samples == max_samples) break;
			if (wfileL->wavinfo.Samples > wfileR->wavinfo.Samples) {
				for (i = min_samples; i < max_samples; i++) {
					*(dst + i) = *(srcL + i);
				}
			} else {
				for (i = min_samples; i < max_samples; i++) {
					*(dst + i) = *(srcR + i);
				}
			}
		} else {
			for (i = 0; i < min_samples; i++) {
				*(dst + i * 2    ) = *(srcL + i);
				*(dst + i * 2 + 1) = *(srcR + i);
			}
			if (min_samples == max_samples) break;
			if (wfileL->wavinfo.Samples > wfileR->wavinfo.Samples) {
				for (i = min_samples; i < max_samples; i++) {
					*(dst + i * 2    ) = *(srcL + i);
					*(dst + i * 2 + 1) = 0;
				}
			} else {
				for (i = min_samples; i < max_samples; i++) {
					*(dst + i * 2    ) = 0;
					*(dst + i * 2 + 1) = *(srcR + i);
				}
			}
		}
		break; }
	default:
		break;
	}
	return wfileM;
}
