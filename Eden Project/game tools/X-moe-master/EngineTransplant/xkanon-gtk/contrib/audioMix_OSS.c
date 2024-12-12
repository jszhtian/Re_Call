/*
 * audioMix_OSS.c  OSS mixer lowlevel access
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
#include <fcntl.h>

#if defined(__FreeBSD__)
#  include<sys/param.h>
#  if __FreeBSD__ < 4
#    include <machine/soundcard.h>
#  else
#    include <sys/soundcard.h>
#  endif
#elif defined(__OpenBSD__) || defined(__NetBSD__)
#  include <soundcard.h>
#  include <sys/ioctl.h>
#else
#  include <sys/soundcard.h>
#endif

#include "audio.h"

static char *mixer_devicename = MIXERDEV;

static boolean mixer_master_capable = FALSE;
static boolean mixer_cd_capable = FALSE;
static boolean mixer_midi_capable = FALSE;
static boolean mixer_pcm_capable = FALSE;

static int get_mixer_id(int device) {
        switch(device) {
        case MIX_MASTER:
                return SOUND_MIXER_VOLUME;
        case MIX_CD:
                return mixer_cd_capable ? SOUND_MIXER_CD : SOUND_MIXER_VOLUME;
        case MIX_MIDI:
                return mixer_midi_capable ? SOUND_MIXER_SYNTH : SOUND_MIXER_VOLUME;
        case MIX_PCM:
                return mixer_pcm_capable ? SOUND_MIXER_PCM : SOUND_MIXER_VOLUME;
        default:
                return SOUND_MIXER_VOLUME;
        }
        return 0;
}

void mixer_set_level(int device, int level){
	int mixer_fd;
	int lv = (level << 8) + level;
	
	if (!mixer_master_capable) return;

        if ((mixer_fd = open(mixer_devicename, O_RDWR)) < 0) return;
	
	if (ioctl(mixer_fd, MIXER_WRITE(get_mixer_id(device)), &lv) < 0) {
		fprintf(stderr, "mixser_set_level(): mixer write failed\n");
	}
	close(mixer_fd);
}

#include<errno.h>
int mixer_get_level(int device) {
	int volume, mixer_fd;
	
	if (!mixer_master_capable) return 0;
	
        if ((mixer_fd = open(mixer_devicename, O_RDWR)) < 0) return 0;

	if (ioctl(mixer_fd, MIXER_READ(get_mixer_id(device)), &volume) < 0) {
		fprintf(stderr, "mixser_get_level(): mixer read failed\n");
	}
	
	close(mixer_fd);
	return volume & 0xff;
}

int mixer_initilize(void) {
	int mask, mixer_fd;
	
        if ((mixer_fd = open(mixer_devicename, O_RDWR)) < 0) {
		fprintf(stderr, "mixer_initilize(): Opening mixer device %s failed\n", mixer_devicename);
		return NG;
	}
	
	if (ioctl(mixer_fd, SOUND_MIXER_READ_DEVMASK, &mask) < 0) {
		fprintf(stderr, "mixer_initilize(): unable to get mixer info\n");
		return NG;
	}
	
	if (mask & SOUND_MASK_VOLUME) {
		mixer_master_capable = TRUE;
	}
mixer_master_capable = TRUE;
	if (mask & SOUND_MASK_CD) {
		mixer_cd_capable = TRUE;
	}
	if (mask & SOUND_MASK_SYNTH) {
		mixer_midi_capable = TRUE;
	}
	if (mask & SOUND_MASK_PCM) {
		mixer_pcm_capable = TRUE;
	}
	
	close(mixer_fd);
	return OK;
}

int mixer_remove() { 
	/* if (mixer_fd != -1) close(mixer_fd); */
	return OK;
}

void mixer_setDeviceName(char *name) {
	mixer_devicename = name;
}

