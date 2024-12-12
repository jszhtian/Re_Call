/*
 * audioIO_oss.c  oss lowlevel access
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
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>

/* この部分はxsystem35 1.7.2のもので差し替え */
#if defined(__FreeBSD__)
#  if __FreeBSD__ < 4
#    include <machine/soundcard.h>
#  else
#    include <sys/soundcard.h>
#  endif
#elif defined(__OpenBSD__) || defined(__NetBSD__)
#  include <soundcard.h>
#else
#  include <sys/soundcard.h>
#endif

#include "audioIO.h"

static int oss_fd = -1;

int audioOpen(DSPFILE *dfile, WAVFILE *wfile, char *dev_dsp) {
	int t;
	u_long ul;
	
	if ( (dfile->fd = open(dev_dsp, O_WRONLY, 0)) < 0 ) {
		fprintf(stderr, "audioOpen(): Opening audio device %s failed\n", dev_dsp);
		return -1;
	}
	
	oss_fd = dfile->fd;
	/*
	 * Set the data bit size:
	 */
	t = wfile->wavinfo.DataBits;
        if ( ioctl(dfile->fd, SNDCTL_DSP_SAMPLESIZE, &t) < 0 ) {
		fprintf(stderr, "audioOpen(): Setting DSP to %d bits\n",(unsigned)t);
		return -1;
	}

	/*
	 * Set the mode to be Stereo or Mono:
	 */
	t = wfile->wavinfo.Channels == Stereo ? 1 : 0;
	if ( ioctl(dfile->fd, SNDCTL_DSP_STEREO, &t) < 0 ) {
		fprintf(stderr, "audioOpen(): Unable to set DSP to %s mode\n",t?"Stereo":"Mono");
		return -1;
	}		
      
	/*
	 * Set the sampling rate:
	 */
	ul = wfile->wavinfo.SamplingRate;
	if ( ioctl(dfile->fd, SNDCTL_DSP_SPEED, &ul) < 0 ) {
		fprintf(stderr, "audioOpen(): Unable to set audio sampling rate\n");
		return -1;
	}

	/*
	 * Determine the audio device's block size:
	 */
	if ( ioctl(dfile->fd, SNDCTL_DSP_GETBLKSIZE, &dfile->dspblksiz) < 0 ) {
		fprintf(stderr, "audioOpen(): Optaining DSP's block size\n");
		return -1;
	}

	/*
	 * Allocate a buffer to do the I/O through:
	 */
	if ( (dfile->dspbuf = (char *) malloc(dfile->dspblksiz)) == NULL ) {
		fprintf(stderr, "audioOpen(): For DSP I/O buffer\n");
		return -1;
	}
	
	/*
	 * Return successfully opened device:
	 */
	return 0;
}

int audioClose(void) {
	int ret = close(oss_fd);
	oss_fd = -1;
	return ret;
}

int audioRest(DSPFILE *dfile) {
	audio_buf_info buf_info;

	if (dfile->fd == -1) return 0;
	if (ioctl(dfile->fd, SNDCTL_DSP_GETOSPACE, &buf_info) != 0) {
		fprintf(stderr, "audioRest() : ioctl(%d, SNDCTL_DSP_GETOSPACE, buf_info)\n",dfile->fd);
		return 0;
	}
	return (buf_info.fragstotal * buf_info.fragsize) - buf_info.bytes;
}

void audioFlush(DSPFILE *dfile) {
	if (dfile->fd == -1) return;
	if ( ioctl(dfile->fd,SNDCTL_DSP_SYNC,0) != 0 )
		fprintf(stderr, "audioFlush(): ioctl(%d,SNDCTL_DSP_SYNC,0)\n",dfile->fd);
}

int audioWrite(DSPFILE *dfile, int cnt) {
	if (dfile->fd == -1) return 0;
	return (write(dfile->fd, dfile->dspbuf, cnt));
}

int audioCheck(DSPINFO *info, char *dev_dsp) {
	int fd = -1, t;
	u_long ul;
	
	if ( (fd = open(dev_dsp, O_WRONLY, 0)) < 0 ) {
		fprintf(stderr,"audioCheck(): Opening audio device %s failed\n", dev_dsp);
		goto eexit;
	}

	t = info->DataBits;
        if ( ioctl(fd, SNDCTL_DSP_SAMPLESIZE, &t) < 0 ) {
		fprintf(stderr, "audioCheck(): Setting DSP to %u bits\n",(unsigned)t);
		goto eexit;
	}

	/*
	 * Set the mode to be Stereo or Mono:
	 */
	t = info->Channels == Stereo ? 1 : 0;
	if ( ioctl(fd,SNDCTL_DSP_STEREO,&t) < 0 ) {
		fprintf(stderr, "audioCheck(): Unable to set DSP to %s mode\n",t?"Stereo":"Mono");
		goto eexit;
	}		
      
	/*
	 * Set the sampling rate:
	 */
	ul = info->SamplingRate;
	if ( ioctl(fd, SNDCTL_DSP_SPEED, &ul) < 0 ) {
		fprintf(stderr, "CheckDSP(): Unable to set audio sampling rate\n");
		goto eexit;
	}
	if (((double)abs(ul - info->SamplingRate) / (double)info->SamplingRate) > 0.2) {
		fprintf(stderr, "CheckDSP(): Too differnt from desierd sampling rate\n");
		goto eexit;
	}
	
	close(fd);
	info->Available = TRUE;
	return OK;
 eexit:
	info->Available = FALSE;
	if (fd >= 0) close(fd);
	return NG;
}

void audioStop(void) {
	ioctl(oss_fd, SNDCTL_DSP_RESET, 0);
}

#include "audioMix_OSS.c"

