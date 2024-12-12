/*
 * cdrom.c  CD-ROM controle wrapper
 *
 * Copyright (C) 2000-  Masaki Chikama (Wren) <masaki-c@is.aist-nara.ac.jp>
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
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

#include "portab.h"
#include "cdrom.h"

#if defined(CDROM_LINUX)
extern cdromdevice_t cdrom_linux;
#define DEV_PLAY_MODE &cdrom_linux

#elif defined(CDROM_BSD)
extern cdromdevice_t cdrom_bsd;
#define DEV_PLAY_MODE &cdrom_bsd

#elif defined(CDROM_IRIX)
extern cdromdevice_t cdrom_irix;
#define DEV_PLAY_MODE &cdrom_irix

#else

extern cdromdevice_t cdrom_empty;
#define DEV_PLAY_MODE &cdrom_empty
#endif

#if defined(CDROM_MP3)
extern cdromdevice_t cdrom_mp3;
#endif


/* temporary cdrom device name */
static char *dev = CDROM_DEVICE;

int cd_init(cdromdevice_t *cd) {
	struct stat st;
	int ret = NG;

	/* 構造体を初期化 */
	cd->init=0;
	cd->exit=0;
	cd->start=0;
	cd->stop=0;
	cd->getinfo=0;
	cd->setaudiodev=0;
	cd->need_audiodevice=0;
	
	if (dev == NULL) return -1;
	
	stat(dev, &st);
	if (S_ISBLK(st.st_mode) || S_ISCHR(st.st_mode)) {
		/* CDROM MODE */
		memcpy(cd, DEV_PLAY_MODE, sizeof(cdromdevice_t));
		ret = cd->init(dev);
	}
#if defined(CDROM_MP3)
	else if (S_ISREG(st.st_mode)) {
		/* MP3 MODE */
		memcpy(cd, &cdrom_mp3, sizeof(cdromdevice_t));
		ret = cd->init(dev);
		memcpy(cd, &cdrom_mp3, sizeof(cdromdevice_t));
	}
#endif
	else {
		/* error */
		fprintf(stderr, "no cdrom device available\n");
		ret = NG;
	}
	return ret;
}

void cd_set_devicename(char *name) {
	if (0 == strcmp("none", name)) dev = NULL;
	else                           dev = strdup(name);
}

const char* cd_get_devicename(void) {
	if (dev == NULL) return "none";
	else return dev;
}
