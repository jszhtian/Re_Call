/*
 * audio.h  audio acesss wrapper
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

#ifndef __AUDIO__
#define __AUDIO__

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif

#include "portab.h"
#include "mixer.h"

/*
 * Types internal to wavplay, in an attempt to isolate ourselves from
 * a dependance on a particular platform.
 */

typedef unsigned char  Byte;
typedef short          Int16;
typedef long           Int32;
typedef unsigned long  UInt32;
typedef unsigned short UInt16;

/*
 * This enumerated type, selects between monophonic sound and
 * stereo sound (1 or 2 channels).
 */
typedef enum {
	Mono,					/* Monophonic sound (1 channel) */
	Stereo					/* Stereo sound (2 channels) */
} Chan;

/*
 * These values represent values found in/or destined for a
 * WAV file.
 */
typedef struct {
	UInt32	SamplingRate;			/* Sampling rate in Hz */
	Chan	Channels;			/* Mono or Stereo */
	UInt32	Samples;			/* Sample count */	
	UInt16	DataBits;			/* Sample bit size (8/12/16) */
	UInt32	DataStart;			/* Offset to wav data */
	UInt32	DataBytes;			/* Data bytes in current chunk */
	UInt32	DataBytes_o;			/* Data bytes in current chunk */
	char	bOvrSampling;			/* True if sampling_rate overrided */
	char	bOvrMode;			/* True if chan_mode overrided */
	char	bOvrBits;			/* True if data_bits is overrided */
} WAVINF;

typedef struct {
	char *data;                             /* real data */ 
	WAVINF wavinfo;                         /* WAV file hdr info */
	int other_data[256];			/* for mixer */
} WAVFILE;

/*
 * This structure manages an opened DSP device.
 */
typedef struct {
	int	fd;				/* Open fd of /dev/dsp */
	int	dspblksiz;			/* Size of the DSP buffer */
	char	*dspbuf;			/* The buffer */
} DSPFILE;

/* DSPの情報取得の際のデータ */

typedef struct {
	UInt32	SamplingRate;			/* Sampling rate in Hz */
	Chan	Channels;			/* Mono or Stereo */
	UInt16	DataBits;			/* Sample bit size (8/12/16) */
	boolean Available;
} DSPINFO;

typedef int (*DSPPROC)(DSPFILE *dfile);		/* DSP work procedure */

extern DSPFILE *OpenDSP(WAVFILE *wfile);
extern int      CloseDSP(DSPFILE *dfile);
extern int      CheckDSP(DSPINFO *info);

#endif /* __AUDIO__ */
