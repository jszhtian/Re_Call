/*
 * music.h  イサエリマ「チエネフ
 *
 * Copyright (C) 1997-1998 Masaki Chikama (Wren) <chikama@kasumi.ipl.mech.nagoya-u.ac.jp>
 *               1998-                           <masaki-c@is.aist-nara.ac.jp>
 *               2000-     Kazunori Ueno(JAGARL) <jagarl@creator.club.ne.jp>
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
#ifndef __MUSIC__
#define __MUSIC__

#include "portab.h"
#include "cdrom.h"
#include<sys/types.h>
#include<sys/time.h>

/* CD-DA/PCM とのプロセス間通信コマンド */
typedef enum {
	NoProcess,
	MUS_CDROM_START,
	MUS_CDROM_STOP,
	MUS_MIXER_FADE_START,
	MUS_MIXER_FADE_STOP,
	MUS_MIXER_SET_DEFVOL,
	MUS_EFFEC_START, // 既にバッファに読み込まれてデータが与えられる
	MUS_EFFEC_STOP,
	MUS_BGM_START,
	MUS_BGM_STOP,
	MUS_KOE_START,
	MUS_KOE_STOP,
	MUS_MOV_START,
	MUS_MOV_STOP,
	MUS_MOV_PAUSE,
	MUS_MOV_RESUME,
	MUS_MOV_INFORM_END,
	MUS_CHILD_MAKE,
	MUS_CHILD_KILL,
	MUS_CHILD_REMOVE,
	MUS_EXIT,
	MUS_ABORT
} MSGTYP;

/* CD-DA/PCM とのプロセス間通信パラメータ */
typedef struct {
	long   mtype;
	MSGTYP msg_type;
	int    bytes;
	union {
		/* cdrom play start */
		struct {
			int trackno;
			int loopcnt;
		} tosrv_cdrom_play;
		
		/* cdrom play info */
		cd_time cd_info;

		/* fade out */
		struct {
			int     device;
			int     time;
			int     last_vol;
			boolean stop_flag;
		} tosrv_fadeout;

		/* set default volume */
		struct {
			int	device;
			int 	volume;
		} tosrv_defvol_set;

		struct {
			int mixer_level[8];
			int mixer_state[8];
		} toclt_mixerinfo;

		struct {
			int loopcnt;
			char path[1];
		} tosrv_pcm_path_play;

		struct {
			char path[1];
		} tosrv_koe_play;

		struct {
			int loopcnt;
			int windowid;
			int x1,y1;
			int x2,y2;
			char path[1];
		} tosrv_mov_path_play;

		struct {
			pid_t parent_id;
			pid_t child_id;
		} tosrv_child;
        } u;
} SRVMSG;


typedef struct {
	int     bgmInProcess;
	int	cdromStart;
	int	killed_mixer_device;
	int     mixer_level[8];
	int     mixer_state[8];
	int mixer_default_level[8];
	struct timeval movie_start_time;
#define CHILD_ID_DEAL 128
#define CHILD_STATE_RUN 1
#define CHILD_STATE_KILL 2
	struct {pid_t id; int state;}	child_ids[CHILD_ID_DEAL];
} SRVSTATE;

typedef struct {
	FILE* stream;
	int length;
	int offset;
	int rate;
}AvgKoeInfo;


/* defined by music.c */
extern boolean cdrom_enable;
extern boolean pcm_enable;
extern int  mus_init();
extern int  mus_exit(int is_abort);
extern boolean mus_get_pcm_state();
extern boolean mus_get_cdrom_state();
extern void mus_set_pcm_state(boolean _bool);
extern void mus_set_cdrom_state(boolean _bool);
extern int mus_set_8to16_usetable(int is_use);
extern void mus_setLoopCount(int cnt);
extern int  mus_cdrom_start(int track);
extern int  mus_cdrom_stop();
extern int  mus_cdrom_getPlayStatus(cd_time *info);
extern int  mus_effec_start(const char* buf, int loop);
extern int  mus_bgm_start(char* buf, int loop);
extern int  mus_koe_start(const char* fname);
extern int  mus_effec_stop();
extern int  mus_bgm_stop();
extern int  mus_koe_stop();
extern int  mus_effec_getStatus(int *pos);
extern int  mus_bgm_getStatus(int *pos);
extern int  mus_koe_getStatus(int *pos);
extern void mus_pcm_set_mix(int is_mix);
extern void mus_pcmserver_stop(void);
extern void mus_pcmserver_resume(void);
extern void mus_set_default_rate(int);
extern int mus_get_default_rate(void);
/*
extern int  mus_pcm_start(int no, int loop);
extern int  mus_pcm_mix(int noL, int noR, int loop);
extern int  mus_pcm_load(int no);
*/
extern void mus_mixer_fadeout_start(int device, int time, int volume, int stop);
extern int  mus_mixer_get_fadeout_state(int device);
extern void mus_mixer_stop_fadeout(int device);
extern int  mus_mixer_get_level(int device);
extern void mus_mixer_set_default_level(int device, int volume);
extern int mus_mixer_get_default_level(int device);
/* デバイスを直接いじってボリュームを変化させる */
extern void mixer_setvolume(int device, int vol);
/* music server 内の fork の置き換え */
extern pid_t fork_local(void);
extern pid_t forkpg_local(void);

/* movie functions */
extern int mus_movie_start(const char* fname, int window_id, int x1, int y1, int x2, int y2, int loop_count);
extern int mus_movie_stop(void);
extern int mus_movie_pause(void);
extern int mus_movie_resume(void);
extern int mus_movie_getStatus(int* pos);
extern void mus_movie_informend(void);
extern void mus_kill_allchildren(void);

/* device name */
extern void mixer_setDeviceName(char *name);
extern void pcm_setDeviceName(char *name);
extern void cdrom_setDeviceName(char *name);

/* music packet receive / send */
extern void SendMsgServerToClient(SRVMSG *msg);
extern void RecvMsgServerToClient(SRVMSG *msg, int is_wait);
extern void SendMsgClientToServer(SRVMSG *msg);
extern void RecvMsgClientToServer(SRVMSG *msg, int is_wait);

/* music shared memory lock / unlock */
extern void mus_shmem_lock(void);
extern void mus_shmem_unlock(void);

#endif /* __MUSIC__ */
