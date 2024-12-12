/*
 * cdrom.FreeBSD.c  CD-ROM��������
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

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <sys/cdio.h>
#include "portab.h"
#include "cdrom.h"

static int  cdrom_init(char *);
static int  cdrom_exit();
static int  cdrom_start(int);
static int  cdrom_stop();
static int  cdrom_getPlayingInfo(cd_time *);

#define cdrom cdrom_bsd
cdromdevice_t cdrom = {
	cdrom_init,
	cdrom_exit,
	cdrom_start,
	cdrom_stop,
	cdrom_getPlayingInfo,
	NULL,
	FALSE
};

static int     cd_fd;
static boolean enabled = FALSE;
static struct  cd_toc_entry toc_buffer[100];
static boolean msfmode = TRUE;               /* default �� MSFPLAY mode */
static int     lastindex;                    /* �ǽ��ȥ�å� */

/* ioctl�����顼�ǵ��ä������ IOCTL_RETRY_TIME ���ȥ饤���� */
static int do_ioctl(int cmd, void *data) {
	int i;
	for (i = 0; i < CDROM_IOCTL_RETRY_TIME; i++) {
		if (0 <= ioctl(cd_fd, cmd, data)) {
			return 0;
		}
		usleep(CDROM_IOCTL_RETRY_INTERVAL * 100 * 1000);
	}
	return -1;
}

/* CD-ROM ���ܼ����ɤ߽Ф��Ƥ��� */
static int get_cd_entry() {
	int    endtrk, i;
	struct ioc_toc_header     tochdr;
	struct ioc_read_toc_entry toc;
	struct ioc_play_msf       msf;

	/* �ǽ��ȥ�å��ֹ������ */
	if (do_ioctl(CDIOREADTOCHEADER, &tochdr) < 0) {
		perror("CDIOREADTOCHEADER");
		return NG;
	}
	lastindex = endtrk = tochdr.ending_track;
	i = tochdr.ending_track - tochdr.starting_track + 1;
	if (endtrk <= 1) {  /* ���ȥ�å��ʾ�ʤ��ȥ��� */
		fprintf(stderr, "No CD-AUDIO in CD-ROM\n");
		return NG;
	}
	/* ���٤ƤΥȥ�å����ܼ����ɤߤ����ƥ���å��夷�Ƥ��� */
	toc.address_format = CD_MSF_FORMAT;
	toc.starting_track = 0;
	toc.data_len = (i + 1) * sizeof(struct cd_toc_entry);
	toc.data = toc_buffer;
	if (do_ioctl(CDIOREADTOCENTRYS, &toc) < 0) {
		perror("CDIOREADTOCENTRYS");
		return NG;
	}

#ifdef CDROM_USE_TRKIND_ONLY
	msfmode = FALSE;
	return OK;
#endif

	/* CDROMPLAYMSF ��ͭ���������å� */
	msf.start_m = toc_buffer[1].addr.msf.minute;
	msf.start_s = toc_buffer[1].addr.msf.second;
	msf.start_f = toc_buffer[1].addr.msf.frame;
	msf.end_m   = toc_buffer[2].addr.msf.minute;
	msf.end_s   = toc_buffer[2].addr.msf.second;
	msf.end_f   = toc_buffer[2].addr.msf.frame;
	if (do_ioctl(CDIOCPLAYMSF, &msf) < 0) {
		perror("CDIOPLAYMSF");
		fprintf(stderr, "CD-ROM: change TRKMODE\n");
		msfmode = FALSE;
	}
	/* stop */
	if (do_ioctl(CDIOCSTOP, NULL) < 0) {
		perror("CDIOCSTOP");
		return NG;
	}
	return OK;
}

/* �ǥХ����ν���� */
int cdrom_init(char *dev_cd) {
	if (dev_cd == NULL) return NG;

	if ((cd_fd = open(dev_cd, O_RDONLY, 0)) < 0) {
		perror("CDROM_DEVICE OPEN");
		enabled = FALSE;
		return NG;
	}
	if (OK == get_cd_entry()) {
		enabled = TRUE;
		return OK;
	}
	enabled = FALSE;
	return NG;
}

/* �ǥХ����θ���� */
int cdrom_exit() {
	if (enabled) {
		cdrom_stop();
		close(cd_fd);
	}
	return OK;
}

/* �ȥ�å��ֹ� trk �α��� trk = 1~ */
int cdrom_start(int trk) {
	struct ioc_play_msf   msf;
	struct ioc_play_track track;
	
	if (!enabled) return NG;

	/* �ʿ�����¿��������Բ�*/
	if (trk > lastindex) {
		return NG;
	}
#if 0
	/* drive spin up */
        if (do_ioctl(CDIOCSTART, NULL) < 0) {
		perror("CDIOCSTART");
                return NG;
	}
#endif
	if (msfmode) {
		msf.start_m = toc_buffer[trk - 1].addr.msf.minute;
		msf.start_s = toc_buffer[trk - 1].addr.msf.second;
		msf.start_f = toc_buffer[trk - 1].addr.msf.frame;
		msf.end_m = toc_buffer[trk].addr.msf.minute;
		msf.end_s = toc_buffer[trk].addr.msf.second;
		msf.end_f = toc_buffer[trk].addr.msf.frame;
		if (do_ioctl(CDIOCPLAYMSF, &msf) < 0) {
			perror("CDIOPLAYMSF");
			return NG;
		}
	} else {
		track.start_track = track.end_track = trk;
		track.start_index = track.end_index = 0;
		if (do_ioctl(CDIOCPLAYTRACKS, &track) < 0) {
			perror("CDIOCPLAYTRACKS");
			return NG;
		}
	}
	do_ioctl(CDIOCRESUME, NULL);
	return OK;
}

/* ������� */
int cdrom_stop() {
	if (enabled) {
		/* if (do_ioctl(CDIOCSTOP, NULL) < 0) { */
		if (do_ioctl(CDIOCPAUSE, NULL) < 0) {
			/* perror("CDIOCSTOP"); */
			perror("CDIOCPAUSE");
		}
		return OK;
	}
	return NG;
}

/* ���߱�����Υȥ�å�����μ��� */
int cdrom_getPlayingInfo (cd_time *info) {
	struct ioc_read_subchannel s;
	struct cd_sub_channel_info data;
	
	if (!enabled)
		goto errexit;
	memset(&s, 0, sizeof(s));

        s.data = &data;
        s.data_len = sizeof data.header + sizeof data.what.position;
        s.address_format = CD_MSF_FORMAT;
        s.data_format = CD_CURRENT_POSITION;
	if (do_ioctl(CDIOCREADSUBCHANNEL, &s) < 0) {
		perror("CDIOCREADSUBCHANNEL");
		goto errexit;
	}
	if (s.data->header.audio_status != CD_AS_PLAY_IN_PROGRESS) {
		goto errexit;
	}
	info->t = s.data->what.position.track_number;
	info->m = s.data->what.position.reladdr.msf.minute;
	info->s = s.data->what.position.reladdr.msf.second;
	info->f = s.data->what.position.reladdr.msf.frame;
	return OK;
 errexit:
	info->t = info->m = info->s = info->f = 999;
	return NG;
}
