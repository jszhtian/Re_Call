/*
 * music.c  音関連全般
 *
 * Copyright (C) 1997-1998 Masaki Chikama (Wren) <chikama@kasumi.ipl.mech.nagoya
-u.ac.jp>
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/msg.h>

#include "portab.h"
//#include "dataManager.h"
#include "music.h"
#include "cdrom.h"
#include "audio.h"
#include "wavfile.h"

#include <fcntl.h>
#include <unistd.h>

/* #define ENABLE_MUSIC_LOG */
#define MUSIC_LOG_PARENT "/tmp/music1.log"
#define MUSIC_LOG_CHILD "/tmp/music2.log"

/* extern variable */
boolean cdrom_enable = FALSE; /* デバイスの有効・無効 */
boolean pcm_enable   = FALSE;

/* static vriable */
static boolean music_initilized = FALSE; /* mus_init() が呼ばれたかどうか */
static SRVSTATE* state;                  /* music process間通信用共有メモリ */
static int     current_music_no;         /* 現在演奏中の曲番号 */
static int     loopcnt;                  /* loop 回数 */
static pid_t   pid_cdrom;                /* CD-ROM プロセス */
static pid_t   pid_effec;                  /* PCM プロセス */
static pid_t   pid_bgm;                  /* PCM プロセス */
static pid_t   pid_koe;                  /* PCM プロセス */
static pid_t   pid_mov;                  /* movie プロセス */
static pid_t   pid_fader;                /* fadeout プロセス */
static pid_t   pid_music;                /* Music Server プロセス */
static int     svrIPC_Music = -1;        /* music process間通信用キー (command)*/
static int     svrIPC_state = -1;        /* music process間通信用キー (info)*/
static int     svrIPC_sem = -1;          /* 共有メモリ操作用セマフォ */
static boolean mixerAbort = FALSE;       /* fadeoutが途中で中断されたかどうかのチェック */

static cdromdevice_t cdrom;		  /*cdrom device*/
static boolean cdrom_capable = FALSE;    /* デバイスが使用可能かどうか */
static boolean pcm_capable   = FALSE;

static int mixer_default_level[8];       /* 起動時の mixer level */

/* permission が未定義なら定義する */
#ifndef SEM_A
#  define SEM_A           0200    /* alter permission */
#endif /* SEM_A */
#ifndef SEM_R
#  define SEM_R           0400    /* read permission */
#endif /* SEM_R */

/* static methods */
static void MsgSend(SRVMSG *msg, int msgtype);
static void MsgRecv(SRVMSG *msg, int msgtype, int is_wait);
extern void SendMsgServerToClient(SRVMSG *msg);
extern void RecvMsgServerToClient(SRVMSG *msg, int is_wait);
extern void SendMsgClientToServer(SRVMSG *msg);
extern void RecvMsgClientToServer(SRVMSG *msg, int is_wait);
// static void drifile_free(DRIFILE *dfile);
static void shutdown_process(pid_t* pid);
static void shutdown_process_nowait(pid_t pid);
static void pause_process(pid_t pid);
static void resume_process(pid_t pid);
static void shutdown_fader(void);
static int  music_server();
static void signal_int_cdrom(int sig_num);
static void cdrom_loop_start(int trk, int loop);
extern pid_t movie_start(const char* filename, int window_id, int x1, int y1, int x2, int y2, int loopcount);
static void signal_int_mixer(int sig_num);
static void mixer_fadeout(int device, int time, int min, boolean stopflag);
static void mixer_set_default_volume(int device, int vol);
extern pid_t effec_start(char* path, int loop_count);
extern pid_t bgm_loop_start(char* path, int loop_count);
extern pid_t koe_start(const char* path);
extern int pcm_init(void);
extern void pcm_remove(int is_abort);

extern pid_t fork_local(void); /* FreeBSD のバグに対処するためのfork */

static int log_handle = -1;
/* debug information */
static void OpenMusicLogParent(void) {
#ifdef ENABLE_MUSIC_LOG
	if (log_handle != -1) return;
	log_handle = open(MUSIC_LOG_PARENT, O_RDWR | O_CREAT, 0644);
	if (log_handle != -1) {
		lseek(log_handle, 0, 2);
	}
#endif
}

static void OpenMusicLogChild(void) {
#ifdef ENABLE_MUSIC_LOG
	if (log_handle != -1) return;
	log_handle = open(MUSIC_LOG_CHILD, O_RDWR | O_CREAT, 0644);
	if (log_handle != -1) {
		lseek(log_handle, 0, 2);
	}
#endif
}

static void PrintLog(char* str, SRVMSG* msg) {
#ifdef ENABLE_MUSIC_LOG
	char buf[1024];
	snprintf(buf, 1024, "%s ; mtype %d, msgtype %d\n", str, msg->mtype, msg->msg_type);
	if (log_handle != -1) write(log_handle, buf, strlen(buf));
#endif
	return;
}

static void MsgSend(SRVMSG *msg, int msgtype) {
	int len = ((char *)&msg->u - (char *)msg) - sizeof(msg->mtype) + msg->bytes;
	if (! music_initilized) return;
	msg->mtype = msgtype;
	PrintLog("send",msg);
	if (0 > msgsnd(svrIPC_Music, (struct msgbuf *)msg, len, IPC_NOWAIT))
		fprintf(stderr, "msgsnd %s\n", strerror(errno));
		
}

static void MsgRecv(SRVMSG *msg, int msgtype, int is_wait) {
	int msgflg = 0;
	if (! music_initilized) return;
	if (! is_wait) msgflg = IPC_NOWAIT;
	if (0 > msgrcv(svrIPC_Music, (struct msgbuf *)msg, sizeof(*msg)-sizeof(long), msgtype, msgflg)) {
		if (errno != EINTR && errno != EAGAIN && errno != ENOMSG)
/*			fprintf(stderr, "msgrcv %s\n", strerror(errno));
*/;
	}
	PrintLog("receive",msg);
}

void SendMsgServerToClient(SRVMSG *msg) {
	MsgSend(msg, 1);
}

void RecvMsgServerToClient(SRVMSG *msg, int is_wait) {
	MsgRecv(msg, 1, is_wait);
}

void SendMsgClientToServer(SRVMSG *msg) {
	MsgSend(msg, 2);
}

void RecvMsgClientToServer(SRVMSG *msg, int is_wait) {
	MsgRecv(msg, 2, is_wait);
}

pid_t forkpg_local(void) {
	int i;
	pid_t pid = fork_local();
	if (pid == 0) {
		/* create new process group */
		if (setpgid(getpid(), getpid()) != 0) perror("setpgid : ");
		return pid;
	}
	/* 子プロセス情報を得る */
	mus_shmem_lock();
	for (i=0; i<CHILD_ID_DEAL; i++) {
		if (state->child_ids[i].id == 0) break;
	}
	if (i != CHILD_ID_DEAL) {
		state->child_ids[i].id = pid;
		state->child_ids[i].state = CHILD_STATE_RUN;
	}
	mus_shmem_unlock();
	if (i == CHILD_ID_DEAL) {
		fprintf(stderr,"forkpg_local(pid %d child %d) : too many children\n",getpid(), pid);
	}
	return pid;
}
static void shutdown_process_nowait(pid_t pid) {
	int i;
	if (pid == 0) return;
	/* 子プロセス情報の更新 */
	mus_shmem_lock();
	for (i=0; i<CHILD_ID_DEAL; i++) {
		if (state->child_ids[i].id == pid) break;
	}
	if (i != CHILD_ID_DEAL) {
		state->child_ids[i].state = CHILD_STATE_KILL;
	}
	mus_shmem_unlock();
	if (i == CHILD_ID_DEAL) {
		fprintf(stderr,"shutdown_process_nowait(pid %d child %d) : cannot find child\n",getpid(), pid);
	}
	/* 実際の kill を行う */
	if (killpg(pid, SIGTERM) == -1 && errno == ESRCH) return; /* killpg に失敗したらそこで終了 */
}
static void shutdown_process(pid_t* pid) {
	int i;
	if (*pid == 0) return;
	shutdown_process_nowait(*pid);
	/* *pid == 0 になるまで待つ */
	/* 最大 500ms 程度 */
	for (i=0; *pid != 0 && i < 10; i++) {
		usleep(1000*50); /* wait 50ms */
	}
	*pid = 0;
	return;
}
static void signal_child(int sig_num) {
	int i;
	int status;
	pid_t pid;
	pid = waitpid(-1, &status, WNOHANG);
	if (pid == -1 || pid == 0) { /* error */
		return;
	}
	/* clear pid */
	if (pid == pid_cdrom) pid_cdrom = 0;
	else if (pid == pid_effec) pid_effec = 0;
	else if (pid == pid_bgm) pid_bgm = 0;
	else if (pid == pid_koe) pid_koe = 0;
	else if (pid == pid_mov) pid_mov = 0;
	else if (pid == pid_fader) pid_fader = 0;
	/* 子プロセス情報の更新 */
	mus_shmem_lock();
	for (i=0; i<CHILD_ID_DEAL; i++) {
		if (state->child_ids[i].id == pid) break;
	}
	if (i != CHILD_ID_DEAL) {
		state->child_ids[i].id = 0;
		state->child_ids[i].state = 0;
	}
	mus_shmem_unlock();
	if (i == CHILD_ID_DEAL) {
		fprintf(stderr,"signal_child(pid %d child %d) : cannot find child\n",getpid(), pid);
	}
}

static void pause_process(pid_t pid) {
	if (pid > 0) {
		killpg(pid, SIGSTOP);
	}
}

static void resume_process(pid_t pid) {
	if (pid > 0) {
		killpg(pid, SIGCONT);
	}
}

void stop_music(void) {
	shutdown_process(&pid_cdrom);
	shutdown_process(&pid_effec);
	shutdown_process(&pid_koe);
	shutdown_process(&pid_bgm);
	shutdown_process(&pid_fader);
	mus_pcmserver_stop();
}

static void set_signalhandler(void (*handler)(int)) {
	struct sigaction act;
	sigset_t smask;
	
	sigemptyset(&smask);
	sigaddset(&smask,  SIGTERM);
	sigaddset(&smask,  SIGINT);
	
	act.sa_handler = handler;
	act.sa_mask = smask;
	act.sa_flags = 0;
	
	sigaction(SIGTERM, &act, NULL);
}
static int music_server() {
	SRVMSG   msg;

	while(TRUE) {
		memset(&msg, 0, sizeof(msg));
		RecvMsgClientToServer(&msg, 1);
#if FreeBSD_PTHREAD_ERROR == 1
		raise(SIGPROF);
#endif /* PTHREAD_ERROR */
		switch(msg.msg_type) {
		case NoProcess: // error
			break;
		case MUS_CDROM_START:
			cdrom_loop_start(msg.u.tosrv_cdrom_play.trackno,
					 msg.u.tosrv_cdrom_play.loopcnt);
			break;
		case MUS_CDROM_STOP:
			shutdown_process(&pid_cdrom);
			break;
		case MUS_EFFEC_STOP:
			shutdown_process(&pid_effec);
			break;
		case MUS_EFFEC_START:
			pid_effec = effec_start(msg.u.tosrv_pcm_path_play.path,
			                        msg.u.tosrv_pcm_path_play.loopcnt);
			break;
		case MUS_BGM_STOP:
			shutdown_process(&pid_bgm);
			break;
		case MUS_BGM_START:
			pid_bgm = bgm_loop_start(msg.u.tosrv_pcm_path_play.path,
			                         msg.u.tosrv_pcm_path_play.loopcnt);
			break;
		case MUS_KOE_STOP:
			shutdown_process(&pid_koe);
			break;
		case MUS_KOE_START:
			pid_koe = koe_start(msg.u.tosrv_koe_play.path);
			break;
		case MUS_MOV_STOP:
			shutdown_process(&pid_mov);
			/* 終了したら終了を通達 */
			mus_movie_informend();
			mus_pcmserver_resume(); /* stop していたら再開する */
			state->movie_start_time.tv_sec = 0;
			state->movie_start_time.tv_usec = 0;

			break;
		case MUS_MOV_PAUSE:
			pause_process(pid_mov);
			break;
		case MUS_MOV_RESUME:
			resume_process(pid_mov);
			break;
		case MUS_MOV_START:
			pid_mov = movie_start(msg.u.tosrv_mov_path_play.path,
				msg.u.tosrv_mov_path_play.windowid,
				msg.u.tosrv_mov_path_play.x1,
				msg.u.tosrv_mov_path_play.y1,
				msg.u.tosrv_mov_path_play.x2,
				msg.u.tosrv_mov_path_play.y2,
				msg.u.tosrv_mov_path_play.loopcnt);
			if (pid_mov) {
				gettimeofday(&(state->movie_start_time), 0);
			} else {
				state->movie_start_time.tv_sec = -1;
			}
			break;
		case MUS_MIXER_FADE_START:
			shutdown_fader();
			pid_fader = 0;
			mixer_fadeout(msg.u.tosrv_fadeout.device,
				      msg.u.tosrv_fadeout.time,
				      msg.u.tosrv_fadeout.last_vol,
				      msg.u.tosrv_fadeout.stop_flag);
				      
			break;
		case MUS_MIXER_FADE_STOP:
			shutdown_fader();
			pid_fader = 0;
			break;
		case MUS_MIXER_SET_DEFVOL:
			mixer_set_default_volume(msg.u.tosrv_defvol_set.device,
				msg.u.tosrv_defvol_set.volume);
			break;
		case MUS_EXIT:
			if (cdrom.exit) cdrom.exit();
			_exit(0); break;
		case MUS_ABORT:
			_exit(0); break;
		default:
			printf("unknown msg get \n");
		}
	}
}

static void signal_int_cdrom(int sig_num) {
	cdrom.stop();
	_exit(0);
}

static void cdrom_loop_start(int trk, int loop) {
	shutdown_process(&pid_cdrom);
	pid_cdrom = forkpg_local();
	if (pid_cdrom == 0) {
		int cnt = 0;
		cd_time info;
		signal(SIGCHLD, SIG_DFL);
		set_signalhandler(signal_int_cdrom);
		state->cdromStart = FALSE;
		if (loop == -1) loop = 10000;
		cdrom.start(trk);
		state->cdromStart = TRUE;
		while(TRUE) {
			cdrom.getinfo(&info);
			if (info.t == 999) {
				state->cdromStart = FALSE;
				cnt++;
				if (cnt >= loop) break; 
				cdrom.start(trk);
				state->cdromStart = TRUE;
			}
			usleep(1000*100);
		}
		_exit(0);
	}
	usleep(1000);
}

static void signal_int_mixer(int sig_num) {
	mixerAbort = TRUE;
}
static void shutdown_fader(void) {
	int device;
	state->killed_mixer_device = -1;
	shutdown_process(&pid_fader);
	pid_fader = 0;
	/* mixer を必要ならセット */
	device = state->killed_mixer_device;
	if (device != -1) {
		mixer_set_level(device, state->mixer_level[device] * mixer_default_level[device]/100);
	}
	/* 必要に応じてプロセスを殺す */
	if (device != -1 && state->mixer_state[device] == 2) {
		if (device == MIX_PCM_BGM)
			shutdown_process(&pid_bgm);
		if (device == MIX_PCM_EFFEC)
			shutdown_process(&pid_effec);
		if (device == MIX_PCM_KOE)
			shutdown_process(&pid_koe);
		if (device == MIX_CD)
			shutdown_process(&pid_cdrom);
		state->mixer_state[device] = 1;
	}
}

static void mixer_set_default_volume(int device, int vol) {
	if (device >= MIX_PCM_TOP) return;
	mixer_default_level[device] = vol;
	state->mixer_default_level[device] = vol;
	mixer_set_level(device, state->mixer_level[device] * mixer_default_level[device]/100);
}

extern void mixer_setvolume(int device, int vol) {
	mixer_fadeout(device, 0, vol, 0);
}

static void mixer_fadeout(int device, int time, int min, boolean stopflag) {
	if( state->mixer_level[device] == 100 && device < MIX_PCM_TOP) {
		mixer_default_level[device] = mixer_get_level(device);
		state->mixer_default_level[device] = mixer_default_level[device];
	}
	if (device == MIX_CD && stopflag != 0) {
		current_music_no = 0;
	}

	if (time == 0) {
		if (device < MIX_PCM_TOP)
			mixer_set_level(device, min * mixer_default_level[device]/100);
		state->mixer_level[device] = min;
		state->mixer_state[device] = 1;
		if (stopflag) {
			if (device == MIX_PCM_BGM)
				shutdown_process(&pid_bgm);
			if (device == MIX_PCM_EFFEC)
				shutdown_process(&pid_effec);
			if (device == MIX_PCM_KOE)
				shutdown_process(&pid_koe);
			if (device == MIX_CD)
				shutdown_process(&pid_cdrom);
		}
		return;
	}
	mixerAbort = FALSE;
	shutdown_process(&pid_fader);
	pid_fader = forkpg_local();
	if (pid_fader == 0) {
		int i, start = state->mixer_level[device];
		int interval;
		set_signalhandler(signal_int_mixer);
		if (start == min) _exit(0);
		interval = time / (start - min);
		
		if (interval == 0) _exit(0);
		
		state->mixer_state[device] = 0;
		if (interval < 0) {
			interval = 0 - interval;
			for (i = start; i <= min; i++) {
				if (mixerAbort) {
					state->mixer_level[device] = min;
					if (stopflag)
						state->mixer_state[device] = 2;
					else
						state->mixer_state[device] = 1;
					if (device < MIX_PCM_TOP) state->killed_mixer_device = device;
					/* kill 後に mixer_set_level() は行われる */
					_exit(0);
				}
				if (device < MIX_PCM_TOP)
					mixer_set_level(device, i * mixer_default_level[device]/100);
				state->mixer_level[device] = i;
				usleep(interval * 1000);
			}
		} else {
			for (i = start; i >=min; i--) {
				if (mixerAbort) {
					state->mixer_level[device] = min;
					if (stopflag)
						state->mixer_state[device] = 2;
					else
						state->mixer_state[device] = 1;
					if (device < MIX_PCM_TOP) state->killed_mixer_device = device;
					/* kill 後に mixer_set_level() は行われる */
					_exit(0);
				}
				if (device < MIX_PCM_TOP)
					mixer_set_level(device, i * mixer_default_level[device]/100);
				state->mixer_level[device] = i;
				usleep(interval * 1000);
			}
		}
		state->mixer_state[device] = 2;
		if (stopflag) {
			if (device == MIX_PCM_BGM)
				shutdown_process_nowait(pid_bgm);
			if (device == MIX_PCM_EFFEC)
				shutdown_process_nowait(pid_effec);
			if (device == MIX_PCM_KOE)
				shutdown_process_nowait(pid_koe);
			if (device == MIX_CD)
				shutdown_process_nowait(pid_cdrom);
		}
		state->mixer_state[device] = 1;
		_exit(0);
	}
	usleep(1000);
}

int mus_init() {
	DSPINFO info; int i;
	
	if (0 > (svrIPC_Music = msgget(IPC_PRIVATE, IPC_CREAT|0600))) { 
		fprintf(stderr, "msgget %s\n", strerror(errno));
		return NG;
	}

	if (0 > (svrIPC_state = shmget(IPC_PRIVATE, sizeof(SRVSTATE), IPC_CREAT | 0777))) {
		fprintf(stderr, "shmget %s\n", strerror(errno));
		msgctl(svrIPC_Music, IPC_RMID, NULL);
		return NG;
	}

	if (NULL == (state = (SRVSTATE *)shmat(svrIPC_state, 0, 0))) {
		fprintf(stderr, "shmat %s\n", strerror(errno));
		msgctl(svrIPC_Music, IPC_RMID, NULL);
		shmctl(svrIPC_state, IPC_RMID, 0);
		return NG;
	}
	/* child id の初期化 */
	for (i=0; i<CHILD_ID_DEAL; i++) {
		state->child_ids[i].id = 0;
	}
	/* semaphore の確保 */
	if (0 > (svrIPC_sem = semget(IPC_PRIVATE, 1, IPC_CREAT |
		(SEM_R | SEM_A) | ((SEM_R | SEM_A )>>3) | ((SEM_R | SEM_A )>>6)))) {
		fprintf(stderr, "semget %s\n", strerror(errno));
		msgctl(svrIPC_Music, IPC_RMID, NULL);
		shmdt((char *)state);
		shmctl(svrIPC_state, IPC_RMID, 0);
		return NG;
	}
	music_initilized = TRUE;

	if (cd_init(&cdrom) == OK) {
		cdrom_capable = TRUE;
	}
	cdrom_enable = cdrom_capable;
	
	info.SamplingRate = 11025;
	info.Channels     = Mono;
	info.DataBits     = 8;
	CheckDSP(&info);
	pcm_capable = pcm_enable = info.Available;

	if (pcm_capable) {
		mixer_initilize();
		mixer_default_level[MIX_MASTER] = mixer_get_level(MIX_MASTER);
		mixer_default_level[MIX_CD]     = mixer_get_level(MIX_CD);
		mixer_default_level[MIX_PCM]    = mixer_get_level(MIX_PCM);
		mixer_default_level[MIX_PCM_BGM]= 100;
		mixer_default_level[MIX_PCM_EFFEC]= 100;
		mixer_default_level[MIX_PCM_KOE]= 100;
		mixer_default_level[MIX_PCM_OTHER]= 100;
		if (pcm_init() == NG) pcm_capable = pcm_enable = false;
	}
	
	if (cdrom_capable || pcm_capable) {
		pid_music = forkpg_local();
		if (pid_music == 0) {
			/* child */
			signal(SIGCHLD, signal_child);
			set_signalhandler(SIG_DFL);
			OpenMusicLogChild();
			music_server();
		}
	}
	OpenMusicLogParent();
	
	state->mixer_state[MIX_MASTER] = 1;
	state->mixer_state[MIX_CD]     = 1;
	state->mixer_state[MIX_PCM]    = 1;
	state->mixer_state[MIX_PCM_BGM]    = 1;
	state->mixer_state[MIX_PCM_EFFEC]    = 1;
	state->mixer_state[MIX_PCM_KOE]    = 1;
	state->mixer_state[MIX_PCM_OTHER]    = 1;
	state->mixer_level[MIX_MASTER] = 100;
	state->mixer_level[MIX_CD]     = 100;
	state->mixer_level[MIX_PCM]    = 100;
	state->mixer_level[MIX_PCM_BGM]    = 100;
	state->mixer_level[MIX_PCM_EFFEC]    = 100;
	state->mixer_level[MIX_PCM_KOE]    = 100;
	state->mixer_level[MIX_PCM_OTHER]    = 100;
	state->mixer_default_level[MIX_MASTER] = mixer_default_level[MIX_MASTER];
	state->mixer_default_level[MIX_CD]     = mixer_default_level[MIX_CD];
	state->mixer_default_level[MIX_PCM]    = mixer_default_level[MIX_PCM];
	state->mixer_default_level[MIX_PCM_BGM]   = mixer_default_level[MIX_PCM_BGM];
	state->mixer_default_level[MIX_PCM_EFFEC] = mixer_default_level[MIX_PCM_EFFEC];
	state->mixer_default_level[MIX_PCM_KOE]   = mixer_default_level[MIX_PCM_KOE];
	state->mixer_default_level[MIX_PCM_OTHER] = mixer_default_level[MIX_PCM_OTHER];
	state->bgmInProcess = FALSE;
	state->cdromStart = FALSE;
	state->movie_start_time.tv_sec = 0;
	state->movie_start_time.tv_usec = 0;
	current_music_no = 0;
	return OK;
}

int mus_exit(int is_abort) {
	if (!music_initilized) return OK;
	
	fprintf(stderr, "Now Music shutdown ... ");
	
	mus_kill_allchildren();
	if (pcm_capable) {
		pcm_remove(is_abort);
		pcm_capable = FALSE;
	}

	if (0 > msgctl(svrIPC_Music, IPC_RMID, NULL)) {
		fprintf(stderr, "msgctl %s\n", strerror(errno));
	}
	svrIPC_Music = -1;
	if (shmdt((char *)state) < 0) {
		fprintf(stderr, "shmdt %s\n", strerror(errno));
	}
	if (shmctl(svrIPC_state, IPC_RMID, 0) < 0) {
		fprintf(stderr, "shmctl %s\n", strerror(errno));
	}
	svrIPC_state = -1;
	if (semctl(svrIPC_sem, 0, IPC_RMID) < 0) {
		fprintf(stderr,"semctl %s\n", strerror(errno));
	}

	fprintf(stderr, "Done!\n");

	/* 全デバイスを無効にする */
	cdrom_enable = FALSE;
	pcm_enable = FALSE;
	music_initilized = FALSE;
	
	return OK;
}

boolean mus_get_pcm_state() {
	return pcm_enable;
}

void mus_set_pcm_state(boolean bool) {
	if (!pcm_capable) return;
	
	pcm_enable = bool;
	if (!bool) {
		mus_bgm_stop();
		mus_effec_stop();
		mus_koe_stop();
	}
}

boolean mus_get_cdrom_state() {
	return cdrom_enable;
}

void mus_set_cdrom_state(boolean bool) {
	static int preno;
	if (!cdrom_capable) return;

	cdrom_enable = bool;
	if (!bool) {
		preno = current_music_no;
		mus_cdrom_stop();
	} else {
		mus_cdrom_start(preno);
	}
}

void mus_setLoopCount(int cnt) {
	loopcnt = cnt;
}

int mus_cdrom_start(int track) {
	SRVMSG msg;
	
	if (!cdrom_capable)            return NG;
	if (track == current_music_no) return OK;

	if (current_music_no != 0) {
		msg.msg_type = MUS_CDROM_STOP;
		msg.bytes = 0;
		SendMsgClientToServer(&msg);
	}

	current_music_no = track;
	
	state->cdromStart = FALSE;
	msg.u.tosrv_cdrom_play.loopcnt = loopcnt;
	msg.u.tosrv_cdrom_play.trackno = track;
	msg.msg_type = MUS_CDROM_START;
	msg.bytes = sizeof(msg.u.tosrv_cdrom_play);
	SendMsgClientToServer(&msg);
	
	return OK;
}

int mus_cdrom_stop() {
	SRVMSG msg;

	if (!cdrom_capable) return NG;
	if (current_music_no != 0) {
		msg.msg_type = MUS_CDROM_STOP;
		msg.bytes = 0;
		SendMsgClientToServer(&msg);
	}
	current_music_no = 0;
	return OK;
}

int mus_cdrom_getPlayStatus(cd_time *info) {
	if (state->cdromStart == FALSE) return NG;
	return cdrom.getinfo(info);
}

int mus_effec_start(const char* path, int loop_c) {
	SRVMSG* msg;
	int len = strlen(path);
	
	if (!pcm_capable) return OK;
	msg = (SRVMSG*)malloc(sizeof(SRVMSG) + len);
	
	msg->msg_type = MUS_EFFEC_STOP;
	msg->bytes = 0;
	SendMsgClientToServer(msg);
	
	msg->u.tosrv_pcm_path_play.loopcnt = loop_c;
	strcpy(msg->u.tosrv_pcm_path_play.path, path);
	msg->msg_type = MUS_EFFEC_START;
	msg->bytes = sizeof(msg->u.tosrv_pcm_path_play) + len;
	SendMsgClientToServer(msg);
	free(msg);
	return OK;
}

int mus_effec_stop() {
	SRVMSG msg; int pos;
	
	if (!pcm_capable) return OK;
	if (mus_effec_getStatus(&pos) == 0) return OK;
	
	msg.msg_type = MUS_EFFEC_STOP;
	msg.bytes = 0;
	SendMsgClientToServer(&msg);
	return OK;
}

int mus_bgm_start(char* path, int loop) {
	SRVMSG* msg;
	int len = strlen(path);
	
	if (!pcm_capable) return OK;
	msg = (SRVMSG*)malloc(sizeof(SRVMSG) + len);
	
	msg->msg_type = MUS_BGM_STOP;
	msg->bytes = 0;
	SendMsgClientToServer(msg);
	
	msg->u.tosrv_pcm_path_play.loopcnt = loop;
	strcpy(msg->u.tosrv_pcm_path_play.path, path);
	msg->msg_type = MUS_BGM_START;
	msg->bytes = sizeof(msg->u.tosrv_pcm_path_play) + len;
	state->bgmInProcess = FALSE;
	state->cdromStart = FALSE;
	SendMsgClientToServer(msg);
	free(msg);
	return OK;
}

int mus_bgm_stop() {
	SRVMSG msg;
	
	if (!pcm_capable) return OK;
	
	msg.msg_type = MUS_BGM_STOP;
	msg.bytes = 0;
	SendMsgClientToServer(&msg);
	state->bgmInProcess = FALSE;
	return OK;
}

int mus_koe_start(const char* path) {
	SRVMSG* msg;
	int len = strlen(path);
	
	if (!pcm_capable) return OK;
	msg = (SRVMSG*)malloc(sizeof(SRVMSG) + len);
	
	msg->msg_type = MUS_KOE_STOP;
	msg->bytes = 0;
	SendMsgClientToServer(msg);
	
	strcpy(msg->u.tosrv_koe_play.path, path);
	msg->msg_type = MUS_KOE_START;
	msg->bytes = sizeof(msg->u.tosrv_koe_play) + len;
	SendMsgClientToServer(msg);
	free(msg);
	return OK;
}


int mus_koe_stop() {
	SRVMSG msg; int pos;
	
	if (!pcm_capable) return OK;
	if (mus_koe_getStatus(&pos) == 0) return OK;
	
	msg.msg_type = MUS_KOE_STOP;
	msg.bytes = 0;
	SendMsgClientToServer(&msg);
	return OK;
}

int mus_movie_start(const char* path, int window_id, int x1, int y1, int x2, int y2, int loop_c) {
	SRVMSG* msg;
	int pos;
	int len = strlen(path);
	
	msg = (SRVMSG*)malloc(sizeof(SRVMSG) + len);
	
	if (!pcm_capable) return OK;
	if (mus_movie_getStatus(&pos) != 0) {
		msg->msg_type = MUS_MOV_STOP;
		msg->bytes = 0;
		SendMsgClientToServer(msg);
	}

	msg->u.tosrv_mov_path_play.loopcnt = loop_c;
	msg->u.tosrv_mov_path_play.windowid = window_id;
	msg->u.tosrv_mov_path_play.x1 = x1;
	msg->u.tosrv_mov_path_play.y1 = y1;
	msg->u.tosrv_mov_path_play.x2 = x2;
	msg->u.tosrv_mov_path_play.y2 = y2;
	strcpy(msg->u.tosrv_mov_path_play.path, path);
	msg->msg_type = MUS_MOV_START;
	msg->bytes = sizeof(msg->u.tosrv_mov_path_play) + len;
	SendMsgClientToServer(msg);
	free(msg);
	return OK;
}

int mus_movie_stop() {
	SRVMSG msg;
	
	if (!pcm_capable) return OK;
	msg.msg_type = MUS_MOV_STOP;
	msg.bytes = 0;
	SendMsgClientToServer(&msg);
	return OK;
}

int mus_movie_pause() {
	SRVMSG msg;
	
	if (!pcm_capable) return OK;
	msg.msg_type = MUS_MOV_PAUSE;
	msg.bytes = 0;
	SendMsgClientToServer(&msg);
	return OK;
}

int mus_movie_resume() {
	SRVMSG msg;
	
	if (!pcm_capable) return OK;
	msg.msg_type = MUS_MOV_RESUME;
	msg.bytes = 0;
	SendMsgClientToServer(&msg);
	return OK;
}

int mus_movie_getStatus(int* pos) {
	struct timeval tv;
	*pos = 0;
	if (!pcm_capable) return OK;
	if (state->movie_start_time.tv_sec == 0 && state->movie_start_time.tv_usec == 0) {
		return 0;
	}
	if (state->movie_start_time.tv_sec == -1) {
		*pos = -1;
		return 1;
	}
	gettimeofday(&tv, 0);
	*pos = (tv.tv_sec-state->movie_start_time.tv_sec)*1000 + (tv.tv_usec-state->movie_start_time.tv_usec)/1000;
	return 1;
}

void mus_mixer_fadeout_start(int device, int time, int volume, int stop) {
	SRVMSG msg;

	if (!pcm_capable) {
		if (device == MIX_CD && stop != 0) mus_cdrom_stop();
		return;
	}


	msg.u.tosrv_fadeout.device    = device;
	msg.u.tosrv_fadeout.time      = time;
	msg.u.tosrv_fadeout.last_vol  = volume;
	msg.u.tosrv_fadeout.stop_flag = stop == 0 ? FALSE : TRUE;
	msg.msg_type = MUS_MIXER_FADE_START;
	msg.bytes = sizeof(msg.u.tosrv_fadeout);
	SendMsgClientToServer(&msg);
	if (stop == 1 && device == MIX_CD) {
		current_music_no = 0;
	}
}	

int mus_mixer_get_fadeout_state(int device) {
	return state->mixer_state[device];
}

void mus_mixer_stop_fadeout(int device) {
	SRVMSG msg;
	
	if (!pcm_capable) return;
	
	msg.u.tosrv_fadeout.device    = device;
	msg.msg_type = MUS_MIXER_FADE_STOP;
	msg.bytes = 0;
	SendMsgClientToServer(&msg);
}

void mus_movie_informend(void) {
	SRVMSG msg;
	
	msg.msg_type = MUS_MOV_INFORM_END;
	msg.bytes = 0;
	SendMsgServerToClient(&msg);
}

int mus_mixer_get_level(int device) {
	return state->mixer_level[device];
}

void mus_mixer_set_default_level(int device, int volume) {
	SRVMSG msg;
	msg.u.tosrv_defvol_set.device = device;
	msg.u.tosrv_defvol_set.volume = volume;
	msg.msg_type = MUS_MIXER_SET_DEFVOL;
	msg.bytes = sizeof(msg.u.tosrv_defvol_set);
	SendMsgClientToServer(&msg);
}

int mus_mixer_get_default_level(int device) {
	return state->mixer_default_level[device];
}

#if FreeBSD_PTHREAD_ERROR
#if HAVE__THREAD_SYS_SIGALTSTACK
int _thread_sys_sigaltstack(const struct sigaltstack *ss, struct sigaltstack *oss);
#endif /* HAVE__THREAD_SYS_SIGALTSTACK */
#endif /* FreeBSD_PTHREAD_ERROR */

pid_t fork_local(void) {
#if FreeBSD_PTHREAD_ERROR
#if HAVE__THREAD_SYS_SIGALTSTACK
	struct sigaltstack alt;
#endif /* HAVE__THREAD_SYS_SIGALTSTACK */
#endif /* FreeBSD_PTHREAD_ERROR */
	pid_t pid = fork();
	if (pid != 0) return pid;
	/* 子の場合、FreeBSD なら stack の初期化をする必要がある */
#if FreeBSD_PTHREAD_ERROR
#if HAVE__THREAD_SYS_SIGALTSTACK
	alt.ss_sp = malloc(SIGSTKSZ);
	alt.ss_size = SIGSTKSZ;
	alt.ss_flags = 0;
	_thread_sys_sigaltstack(&alt,0);
#endif /* HAVE__THREAD_SYS_SIGALTSTACK */
#endif /* FreeBSD_PTHREAD_ERROR */
	return 0;
}

void mus_shmem_lock(void) {
	struct sembuf ops[2];

	if (svrIPC_sem == -1) return;
	ops[0].sem_num = 0;
	ops[0].sem_op  = 0; /* semaphore が 0 になるまで wait */
	ops[0].sem_flg = 0;
	ops[1].sem_num = 0;
	ops[1].sem_op  = 1;
	ops[1].sem_flg = 0;
	if (0 > semop(svrIPC_sem, ops, 2)) {
		fprintf(stderr,"sem error in pcm_server : %s\n",strerror(errno));
		fprintf(stderr,"release semaphore (pid %d)\n",getpid());
		semctl(svrIPC_sem, 0, IPC_RMID); /* セマフォを開放(すでに他のプロセスで開放されている可能性もある */
		svrIPC_sem = -1;
	}
	return;
}

void mus_shmem_unlock(void) {
	struct sembuf ops[2];

	if (svrIPC_sem == -1) return;
	ops[0].sem_num = 0;
	ops[0].sem_op  = -1;
	ops[0].sem_flg = 0;
	if (0 > semop(svrIPC_sem, ops, 1)) {
		fprintf(stderr,"sem error in pcm_server : %s\n",strerror(errno));
		fprintf(stderr,"release semaphore (pid %d)\n",getpid());
		semctl(svrIPC_sem, 0, IPC_RMID); /* セマフォを開放(すでに他のプロセスで開放されている可能性もある */
		svrIPC_sem = -1;
	}
	return;
}
void mus_kill_allchildren(void) {
	int i;
	/* kill されたが wait() に引っかかってないプロセスに SIGTERM を送る */
	for (i=0; i<CHILD_ID_DEAL; i++) {
		if (state->child_ids[i].id != 0 && state->child_ids[i].state == CHILD_STATE_KILL) {
			fprintf(stderr,"Killed but not removed process : %d\n",state->child_ids[i].id);
			killpg(state->child_ids[i].id, SIGTERM);
		}
	}
	/* kill されて欲しいので、ちょっと(10ms)待つ */
	usleep(10*1000);
	/* 残っているプロセスを全部消す */
	for (i=0; i<CHILD_ID_DEAL; i++) {
		if (state->child_ids[i].id != 0) {
			if (killpg(state->child_ids[i].id, SIGTERM) != 0) {
				fprintf(stderr, "kill process(%d) failed : %s\n",state->child_ids[i].id, strerror(errno));
			}
		}
	}
	
}
