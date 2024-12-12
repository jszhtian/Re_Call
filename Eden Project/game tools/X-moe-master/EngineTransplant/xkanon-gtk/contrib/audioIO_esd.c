
/*
 * audioIO_esd.c  esound lowlevel access
 *
 * Copyright (C) 1999-                           <tajiri@wizard.elec.waseda.ac.jp>
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

/*

 midi へのラッパーとしては

 #!/bin/sh
 timidity -idq -Or -o- $1 | esdcat & 
 wait
 exit 0

 のようなものを用意しておきましょう。

 timidity Version 2.0.0ではesdに対応しているので次のようにします。

 #!/bin/sh
 trap 'exit 0' INT TERM
 timidity -idq -Oe
 exit 0

*/

#include <esd.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#ifdef USE_OSS
#include <sys/soundcard.h>
#elif defined(__FreeBSD__)
#  include<sys/param.h>
#  if __FreeBSD_version >= 400000
#    include <sys/soundcard.h>
#  else
#    include <machine/soundcard.h>
#  endif
#elif defined(__OpenBSD__) || defined(__NetBSD__)
#include <soundcard.h>
#endif /* USE_OSS */

#ifdef USE_ALSA
#include "audioMix_alsa.c"
#elif USE_OSS || defined(__FreeBSD__) || defined(__OpenBSD__) || defined(__NetBSD__)
#include "audioMix_OSS.c"
#else
#include "audioMix_dmy.c"
#endif /* USE_ALSA */
#include "audio.h"

static int esd_fd = -1;
static char esd_buf[ESD_BUF_SIZE];
static unsigned int esd_buf_cnt;
static int mywrite(int fd, char *buf, int cnt);

int audioOpen(DSPFILE *dfile, WAVFILE *wfile, char *device)
{
	int bits = ESD_BITS16, channels = ESD_STEREO;
	int mode = ESD_STREAM, func = ESD_PLAY ;
	esd_format_t format = 0;
	int rate = wfile->wavinfo.SamplingRate;
	char* host=NULL;
	char* name=NULL;
	int sock = -1;
	/*
	 * Set the data bit size:
	 */
	bits = wfile->wavinfo.DataBits==8 ? ESD_BITS8 : ESD_BITS16;
	/*
	 * Set the mode to be Stereo or Mono:
	 */
	channels = wfile->wavinfo.Channels == Stereo ? ESD_STEREO : ESD_MONO;
	format = bits | channels | mode | func;
	printf( "opening socket, format = 0x%08x at %d Hz\n", 
	       format, rate );
//	esd_fd = sock = esd_play_stream_fallback( format, rate, host, name );
	esd_fd = sock = esd_play_stream( format, rate, host, name );
	if ((dfile->fd = sock) < 0 ) {
		fprintf(stderr, "audioOpen(): Opening audio server faildd\n");
		return -1;
	}
	dfile->dspblksiz= ESD_BUF_SIZE;
	if ( (dfile->dspbuf = (char *) malloc(dfile->dspblksiz)) == NULL ) {
		fprintf(stderr, "audioOpen(): For DSP I/O buffer\n");
		return -1;
	}
	
	return 0;
}

int audioClose(void) {
	int ret = close(esd_fd);
	esd_fd = -1;
	return ret;
}

void audioFlush(DSPFILE *dfile) {
	if(esd_buf_cnt > 0){
		memset(esd_buf + esd_buf_cnt, 0, ESD_BUF_SIZE - esd_buf_cnt);
		mywrite(dfile->fd, esd_buf, ESD_BUF_SIZE);
		esd_buf_cnt = 0;
	}
	esd_audio_flush();
}

int audioRest(DSPFILE *dfile) {
	return 0;
}

int audioWrite(DSPFILE *dfile, int cnt) {
	char *p = dfile->dspbuf;
	int n;
	int cnt_save = cnt;

	if(esd_buf_cnt > 0){
		if(ESD_BUF_SIZE - esd_buf_cnt > cnt){
			memcpy(esd_buf + esd_buf_cnt, p, cnt);
			esd_buf_cnt += cnt;
			return cnt;
		}else{
			memcpy(esd_buf + esd_buf_cnt, p, ESD_BUF_SIZE - esd_buf_cnt);
			cnt -= ESD_BUF_SIZE - esd_buf_cnt;
			p += ESD_BUF_SIZE - esd_buf_cnt;
			n = mywrite(dfile->fd, esd_buf, ESD_BUF_SIZE);
			esd_buf_cnt = 0;
			if(n < 0) return -1;
		}
	}

	while(cnt >= ESD_BUF_SIZE){
		n = mywrite(dfile->fd, p, ESD_BUF_SIZE);
		if(n < 0) return -1;
		p += ESD_BUF_SIZE;
		cnt -= ESD_BUF_SIZE;
	}

	if(cnt > 0){
		memcpy(esd_buf, p, cnt);
		esd_buf_cnt = cnt;
	}

	return cnt_save;
}

int audioCheck(DSPINFO *info, char *device) {
	int bits = ESD_BITS16, channels = ESD_STEREO;
	int mode = ESD_STREAM, func = ESD_PLAY ;
	int rate = ESD_DEFAULT_RATE;
	int sock=-1;
	char* host=NULL;
	char* name=NULL;
	
	esd_format_t format = 0;
	
	bits=info->DataBits == 8 ? ESD_BITS8 : ESD_BITS16;
	channels =info->Channels == Stereo ? ESD_STEREO : ESD_MONO;
	rate = info->SamplingRate;
	
	format = bits | channels | mode | func;
	printf( "opening socket, format = 0x%08x at %d Hz\n", 
	       format, rate );
	
	/* sock = esd_play_stream( format, rate ); */
//	sock = esd_play_stream_fallback( format, rate, host, name );
	sock = esd_play_stream( format, rate, host, name );
	if(sock<=0){
		info->Available = FALSE;
		if (sock >= 0) close(sock);
		return NG;
	}
	close(sock);
	info->Available = TRUE;
	return OK;
}

void audioStop( void ) {
}

static int mywrite(int fd, char *buf, int cnt)
{
	int write_cnt = 0;
	int n;

	while(cnt > 0){
		n = write(fd, buf, cnt);
		if(n > 0){
			cnt -= n;
			buf += n;
			write_cnt += n;
		}else{
			if(errno != EAGAIN && errno != EINTR){
				return -1;
			}
		}
	}
	return write_cnt;
}
