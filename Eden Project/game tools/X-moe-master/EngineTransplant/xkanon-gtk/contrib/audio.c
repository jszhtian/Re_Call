/*
 * audio.c  audio acesss wrapper
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
#include <errno.h>
#include <string.h>
#include "audio.h"
#include "audioIO.h"

static char*   dev_dsp      = AUDIODEV;     /* DSP device name */

void pcm_setDeviceName(char *name) {
	dev_dsp = name;
}

DSPFILE *OpenDSP(WAVFILE *wfile) {
	int e;
	
	DSPFILE *dfile = (DSPFILE *) malloc(sizeof (DSPFILE));
	if ( dfile == NULL ) {
		fprintf(stderr, "audio(): Opening DSP device\n");
		return NULL;
	}
	memset(dfile, 0, sizeof *dfile);
	dfile->dspbuf = NULL;
	
	if (audioOpen(dfile, wfile, dev_dsp) < 0) {
		goto eexit;
	}
	return dfile;
 eexit:
	e = errno;
	if (dfile->fd >= 0)        audioClose();
	if (dfile->dspbuf != NULL) free(dfile->dspbuf);
	free(dfile);
	errno = e;
	return NULL;
}

int CloseDSP(DSPFILE *dfile) {
	int e;
	
	if (dfile == NULL) {
		fprintf(stderr,"CloseDSP(): DSPFILE is not open\n");
		return -1;
	}

	e = audioClose();
	if ( dfile->dspbuf != NULL ) {
		free(dfile->dspbuf);
	}
	
	free(dfile);
	return e;
}

int CheckDSP(DSPINFO *info) {
	return audioCheck(info, dev_dsp);
}

