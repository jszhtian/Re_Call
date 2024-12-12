/*
 * audioIO_alsa09.c  alsa lowlevel acess
 * Copyright (C) 2008 Kazunori Ueno (JAGARL) <jagarl@creator.club.ne.jp>
 * branched from audio_alsa09.c in xsystem35-1.7.3-pre3
 *
 * audio_alsa09.c  alsa lowlevel acess (for 0.9.x)
 *  $Id: audio_alsa09.c,v 1.7 2004/10/31 04:18:06 chikama Exp
 *
 * Copyright (C) 1997-1998 Masaki Chikama (Wren) <chikama@kasumi.ipl.mech.nagoya-u.ac.jp>
 *               1998-                           <masaki-c@is.aist-nara.ac.jp>
 * rewrited      2000-     Fumihiko Murata <fmurata@p1.tcnet.ne.jp>
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
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
*/

#include <stdio.h>
#include <stdlib.h>
#include <alsa/asoundlib.h>
#include "audio.h"
#include <sys/poll.h>

#ifndef SND_PROTOCOL_VERSION
#define SND_PROTOCOL_VERSION(major, minor, subminor) (((major)<<16)|((minor)<<8)|(subminor))
#endif

#define BUFFERSIZE 1536

static snd_pcm_t *pcm_handle = 0;

int audioOpen(DSPFILE *dfile, WAVFILE *wfile, char *device) {
	snd_pcm_hw_params_t *hwparams;
#if SND_LIB_VERSION >= SND_PROTOCOL_VERSION(1,0,0)
	snd_pcm_uframes_t len;
#else
	snd_pcm_sframes_t len;
#endif
	int err, periods;
	
	if (0 > snd_pcm_open(&pcm_handle, device, SND_PCM_STREAM_PLAYBACK, SND_PCM_ASYNC)) {
		fprintf(stderr,"WARNING: Opening audio device %s failed\n", device);
		goto _err_exit;
	}
	snd_pcm_hw_params_alloca(&hwparams);
	
	if (0 > snd_pcm_hw_params_any(pcm_handle, hwparams)) {
		fprintf(stderr,"WARNING: param get failed\n");
		goto _err_exit;
	}
	
	if (0 > snd_pcm_hw_params_set_access(pcm_handle, hwparams, SND_PCM_ACCESS_RW_INTERLEAVED)) {
		fprintf(stderr,"WARNING: set access fail\n");
		goto _err_exit;
	}
	
	if (0 > snd_pcm_hw_params_set_format(pcm_handle, hwparams, wfile->wavinfo.DataBits == 16 ? SND_PCM_FORMAT_S16 : SND_PCM_FORMAT_U8)) {
		fprintf(stderr,"WARNING: set format fail\n");
		goto _err_exit;
	}
	
	if (0 > snd_pcm_hw_params_set_channels(pcm_handle, hwparams, wfile->wavinfo.Channels == Stereo ? 2 : 1)) {
		fprintf(stderr,"WARNING: set channel fail\n");
		goto _err_exit;
	}
	
#if SND_LIB_VERSION >= SND_PROTOCOL_VERSION(1,0,0)
	int tmp = wfile->wavinfo.SamplingRate;
	err = snd_pcm_hw_params_set_rate_near(pcm_handle, hwparams, &tmp, 0);
#else
	err = snd_pcm_hw_params_set_rate_near(pcm_handle, hwparams, wfile->wavinfo.SamplingRate, 0);
#endif
	if (err < 0) {
		fprintf(stderr,"WARNING: set rate fail\n");
		goto _err_exit;
	}

	if (tmp != wfile->wavinfo.SamplingRate) {
		fprintf(stderr,"WARNING: set rate fail\n");
		goto _err_exit;
	}

	if (0 > snd_pcm_hw_params_set_periods_integer(pcm_handle, hwparams)) {
		fprintf(stderr,"WARNING: set periods fail\n");
		goto _err_exit;
	}
	
	periods = 2;
	if (0 > snd_pcm_hw_params_set_periods_min(pcm_handle, hwparams, &periods, 0)) {
		fprintf(stderr,"WARNING: set priods min fail\n");
		goto _err_exit;
	}
	
	if (0 > (err = snd_pcm_hw_params_set_buffer_size(pcm_handle, hwparams, BUFFERSIZE))) {
		fprintf(stderr, "WARNING: set buffer fail <- %d(%s)\n",BUFFERSIZE,snd_strerror(err));
		len = BUFFERSIZE;
		if (0 > (err = snd_pcm_hw_params_set_buffer_size_near(pcm_handle, hwparams, &len))) {
			fprintf(stderr, "WARNING: set buffer fail (near) <- %d(%s)\n",len,snd_strerror(err));
			goto _err_exit;
		}
	}	  
	
	if (0 > snd_pcm_hw_params(pcm_handle, hwparams)) {
		fprintf(stderr, "WARNING: set hw parmas fail\n");
		goto _err_exit;
	}	  
#if SND_LIB_VERSION >= SND_PROTOCOL_VERSION(1,0,0)
	
        snd_pcm_hw_params_get_buffer_size(hwparams, &len);
#else
	len = snd_pcm_hw_params_get_buffer_size(hwparams);
#endif
	dfile->dspblksiz = (int)len;
	if ( (dfile->dspbuf = (char *) malloc(dfile->dspblksiz)) == NULL ) {
		fprintf(stderr, "audioOpen(): For DSP I/O buffer\n");
		return -1;
	}

	return 0;
	
 _err_exit:
	if (pcm_handle) {
		snd_pcm_close(pcm_handle);
		pcm_handle = 0;
	}
	return -1;
}
int audioClose(void) {
	if (pcm_handle) {
		snd_pcm_close(pcm_handle);
		pcm_handle =0;
	}
	return 0;
}
int audioWrite(DSPFILE *dfile, int cnt) {
	int len, err;
	
	if (pcm_handle ==0) return 0;
	
	if (cnt == 0) return 0;
	
	len = snd_pcm_bytes_to_frames(pcm_handle, cnt);
	while(0 > (err = snd_pcm_writei(pcm_handle, dfile->dspbuf, len))) {
		if (err == -EPIPE) {
			if (0 > snd_pcm_prepare(pcm_handle)) {
        			fprintf(stderr,"playback status err 1: %s\n",snd_strerror(err));
				return -1;
			}
			continue;
		} else if (err == -ESTRPIPE) {
			while(-EAGAIN == (err = snd_pcm_resume(pcm_handle))) {
				sleep(1);
			}
			if (err < 0) {
				if (0 > snd_pcm_prepare(pcm_handle)) {
        				fprintf(stderr,"playback status err 2: %s\n",snd_strerror(err));
					return -1;
				}
			}
			continue;
		}
		if (0 > snd_pcm_prepare(pcm_handle)) {
			return -1;
		}
	}
	
	return (snd_pcm_frames_to_bytes(pcm_handle, err));
}
void audioFlush(DSPFILE *dfile) {
	while (snd_pcm_state(pcm_handle) == SND_PCM_STATE_RUNNING) {
		snd_pcm_avail_update(pcm_handle);
		snd_pcm_sframes_t	delay;
		if (snd_pcm_delay(pcm_handle, &delay) < 0) return;
		if (delay <= 0) return;
		usleep(1);
	}
}

int audioRest(DSPFILE *dfile) {
	if(snd_pcm_state(pcm_handle) != SND_PCM_STATE_RUNNING) return 0;
	snd_pcm_avail_update(pcm_handle);
	snd_pcm_sframes_t	delay;
	if (snd_pcm_delay(pcm_handle, &delay) < 0) return 0;
	if (delay <= 0) return 0;
	return (snd_pcm_frames_to_bytes(pcm_handle, delay));
}

int audioCheck(DSPINFO *info, char *device) {
	/* ???? */
	info->Available = TRUE;
	return OK;
}
void audioStop(void) {
}

#include "audioMix_OSS.c"
