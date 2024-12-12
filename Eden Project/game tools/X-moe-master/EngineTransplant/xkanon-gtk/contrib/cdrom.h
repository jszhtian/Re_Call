/*
 * cdrom.h  CD-ROM制御
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
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
*/

#ifndef __CDROM__
#define __CDROM__

#include "portab.h"
#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif

#ifndef CDROM_DEVICE
#define CDROM_DEVICE "/dev/cdrom"
#endif


/*
 * CD-ROM へのアクセスが不安定な場合は次の定数を増やしてみて下さい
 */
/* ioctrole retry times */
#define CDROM_IOCTL_RETRY_TIME 3
/* ioctrole retry interval (100ms unit) */
#define CDROM_IOCTL_RETRY_INTERVAL 1

typedef struct {
	int t,m,s,f;
} cd_time;

struct audio;

typedef struct cdromdevice cdromdevice_t;
struct cdromdevice {
	int  (* init)(char *);
	int  (* exit)(void);
	int  (* start)(int);
	int  (* stop)(void);
	int  (* getinfo)(cd_time *);
	void (* setaudiodev)(struct audio *);
	boolean need_audiodevice;
};

extern int  cd_init(cdromdevice_t *);
extern void cd_set_devicename(char *);
extern const char* cd_get_devicename(void);

#endif /* __CDROM__ */
