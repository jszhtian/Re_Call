/*
 * pcm.c  PCMで最大４チャンネルまでの合成しながら再生を行う
 *
 * Copyright (C)   2000-     Kazunori Ueno(JAGARL) <jagarl@creator.club.ne.jp>
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

/* #define MEAS_TIME */

/* #define SUPRESS_SERVER */ /* server を起動しない */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <time.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/time.h>
#include <sys/resource.h>

#include "portab.h"
#include "music.h"
#include "audio.h"
#include "audioIO.h"
#include "wavfile.h"

#define WAVE_SIZE 16384 /* 中間バッファのブロック数 */
#define WAVE_CHANNEL_DEAL 4
#define WAVE_DEFAULT_MARGIN 4096 /* 一回に読み込むブロック数 */
#define WAVE_MAX_BLKSIZ 32768 /* 0.2秒 ; ブロック数でなくバイト数なので short, stereo なら /4 のブロック数 */

#define CHN_EFFEC 1
#define CHN_BGM 0
#define CHN_KOE 2


typedef struct {
	int is_mix;
	int read_pointer[WAVE_CHANNEL_DEAL];
	int play_pointer;
	int next_read_margin[WAVE_CHANNEL_DEAL];
	int next_read_flag[WAVE_CHANNEL_DEAL];
	int read_deal[WAVE_CHANNEL_DEAL];
	short wave_buf[WAVE_SIZE*WAVE_CHANNEL_DEAL*2];
	pid_t	child_pid[WAVE_CHANNEL_DEAL];
	int device_release_query;
} PCMDATA;

extern FILE* OpenWaveFile(const char* path, int* length);
extern AvgKoeInfo OpenKoeFile(const char* path);
extern short* decode_koe(AvgKoeInfo info, int* len);
extern pid_t forkpg_local(void); /* 新しいプロセスグループを作成する fork */

/* static variable */
static boolean pcm_initialized = FALSE; /* pcm_init() が呼ばれたかどうか */
static int is_mix_pre_define = TRUE;
static int is_8to16_usetable = FALSE;

static PCMDATA* pcm = NULL;            /* 共有PCM再生バッファ*/
static int   svrIPC_Wave  = -1;        /* pcm 再生バッファ通信用キー */
static int   svrIPC_Wait  = -1;        /* pcm 再生待ちセマフォキー */
static WAVFILE wfile;                  /* audio 側に渡すデータ */
#define PCM_SEM_USING 0
#define PCM_SEM_WAIT 1
#define PCM_SEM_SERVER_WAIT 2
#define PCM_SEM_DEVICE 3
#define PCM_SEM_CHN 4
#define PCM_SEM_DEAL 9 /* 4 + 4channel+1 */

extern pid_t effec_start(char* path, int loop_count);
extern pid_t bgm_loop_start(char* path, int loop_count);
extern pid_t koe_start(const char* path);
extern int pcm_init(void);
extern void pcm_remove(int is_abort);

static void signal_int_pcm(int sig_num);
static void signal_abort_pcm(int sig_num);
static void signal_int(int sig_num);
static void read_wave(int number, FILE* f, WAVFILE* wf, int first_pt, int wf_datalen, int end_offset);

/* セマフォ周りの処理 */
/* permission が未定義なら定義する */
#ifndef SEM_A
#  define SEM_A           0200    /* alter permission */
#endif /* SEM_A */
#ifndef SEM_R
#  define SEM_R           0400    /* read permission */
#endif /* SEM_R */

/* pcm server が時間待ちに入ったら呼ぶ */
static void wait_for_play(int channel);
/* play の時間待ちを終えたら呼ぶ */
static void end_wait_for_play(int channel);
/* pcm server 側が時間待ちに入る */
static void server_wait(int is_block);

static void wait_next_read(int number, int margin);
static void clear_pcm(int number);
static void proceed_pcm(int number, int count);
static void output_pcm(int number, short* src, int count);
static void write_raw(int number, short* data, int length);
static int calc_playtime(int number);
static int conv_wave_rate(short* in_buf, int length, int rate, int* ext_data);

/* log output */
/* 排他処理はしないがそれほどひどいことにはならないはず…… */
#define NO_LOGOUT
static int log_fd = -1;
static struct timeval log_tv;
static void LogOpen(void) {
#ifndef NO_LOGOUT
	log_fd = open("/tmp/pcm.log",O_WRONLY|O_CREAT|O_TRUNC, 0644);
	gettimeofday(&log_tv, 0);
#endif /* NO_LOGOUT */
}
static void LogPrintf(const char* format, ...) {
	va_list ap; char buf[1024]; struct timeval now_tv; int tm;
	if (log_fd == -1) return;
	gettimeofday(&now_tv, 0);
	tm = (now_tv.tv_sec-log_tv.tv_sec)*1000+(now_tv.tv_usec-log_tv.tv_usec)/1000; /* 開始からの時間 */
	sprintf(buf, "%5d: ",tm);
	va_start(ap, format);
	vsnprintf(buf+strlen(buf),1000,format, ap);
	strcat(buf,"\n");
	va_end(ap);
	lseek(log_fd,0,2); write(log_fd, buf,strlen(buf));
	return;
}

/* extern methods */
extern WAVFILE *mixWaveFile(WAVFILE *wfileL, WAVFILE *wfileR);

static void signal_alarm(int sig){
}
static void set_signalhandler(void (*handler)(int), int sig) {
	struct sigaction act;
	sigset_t smask;
	
	sigemptyset(&smask);
	sigaddset(&smask, sig);
	
	act.sa_handler = handler;
	act.sa_mask = smask;
	act.sa_flags = 0;
	
	sigaction(sig, &act, NULL);
}

static void signal_int_pcm(int sig_num) {
	audioStop();
	audioClose();
	_exit(0);
}
static void signal_abort_pcm(int sig_num) {
	_exit(0);
}

/* wave を一回の出力分合成し、dfile 内のバッファに代入する */
static int mix_wave(DSPFILE* dfile, short** dsp_tmpbuf) {
	int len = dfile->dspblksiz / 4;
	int i, k=0;
	int volume[WAVE_CHANNEL_DEAL];
	short* data[WAVE_CHANNEL_DEAL];
	int cur_len;
	short* buf = (short*)dfile->dspbuf;

	int play_pt, old_play_pt;
	int read_pt[WAVE_CHANNEL_DEAL];

	/* まず、共有メモリから必要な情報をコピーする */
	play_pt = old_play_pt = pcm->play_pointer;
	for (i=0; i<WAVE_CHANNEL_DEAL; i++)
		read_pt[i] = pcm->read_pointer[i];

	/* 再生の必要がないならここでチェック */
	for (i=0; i<WAVE_CHANNEL_DEAL; i++) {
		if (read_pt[i] < 0) continue;
	/*	if (read_pt[i] == play_pt) continue; */
		break;
	}
	if (i == WAVE_CHANNEL_DEAL) return -1;
	/* 合成を行うチャンネル選択と再生長の設定 */
	for (i=0; i<WAVE_CHANNEL_DEAL; i++) {
		/* 再生終了か、読み込みに追いついたら再生しない */
		if (play_pt == read_pt[i]) continue;
		if (read_pt[i] < 0) continue;
		LogPrintf("mix play_pt %5d chn %d",play_pt,i);

		/* volume が０なら再生しない */
		/* volume は、全チャンネル合成後 /4096 して short になるようにする */
		volume[k] = mus_mixer_get_level(MIX_PCM_TOP + i);
		volume[k] = volume[k] * 4096 / 100 / WAVE_CHANNEL_DEAL;
		if (volume[k] == 0) continue;

		data[k] = pcm->wave_buf + play_pt*8 + i*2;

		/* 読み込みが遅れて再生に追いつかない場合、
		** 最も読み込みの遅い物にlenを合わせる
		*/
		cur_len = read_pt[i] - play_pt;
		if (cur_len < 0) cur_len += WAVE_SIZE;

		if (len > cur_len) len = cur_len;
		k++;
	}
	/* 読み込みバッファの末尾を越えて読み込みをする場合
	** tmpbuf にコピーしておこなう
	*/
	/* どっちが速いんだろう・・・ */
	/* キャッシュミスが速度を支配するなら、無駄なようでいて上の方が速いかも */
#if 1
	if (play_pt+len > WAVE_SIZE) {
		memcpy(dsp_tmpbuf[0], pcm->wave_buf + play_pt*8, (WAVE_SIZE-play_pt)*WAVE_CHANNEL_DEAL*2*2);
		memcpy(dsp_tmpbuf[0]+(WAVE_SIZE-play_pt)*WAVE_CHANNEL_DEAL*2,
			pcm->wave_buf, (play_pt+len-WAVE_SIZE)*WAVE_CHANNEL_DEAL*2*2);
		for (i=0; i<k; i++) {
			data[i] -= play_pt*8;
			data[i] = dsp_tmpbuf[ (data[i] - pcm->wave_buf)/2 ];
		}
		/* 使用していないチャンネルを無音にしておく */
		for (i=k; i<WAVE_CHANNEL_DEAL; i++) {
			volume[i] = 0; data[i] = dsp_tmpbuf[0];
		}
	} else {
		/* 使用していないチャンネルを無音にしておく */
		for (i=k; i<WAVE_CHANNEL_DEAL; i++) {
			volume[i] = 0; data[i] = pcm->wave_buf;
		}
	}
#else
	for (i=0; i<k; i++) {
		short* d; short* s; short* s_end; short* d_end;
		if (play_pt + len < WAVE_SIZE) continue;

		s = data[i]; s_end = pcm->wave_buf + WAVE_SIZE * WAVE_CHANNEL_DEAL * 2;
		d = dsp_tmpbuf[i]; d_end = d + len * WAVE_CHANNEL_DEAL * 2;
		for (; s < s_end; s += 2*WAVE_CHANNEL_DEAL, d += 2*WAVE_CHANNEL_DEAL) {
			d[0] = s[0]; d[1] = s[1];
		}
		s -= WAVE_SIZE*WAVE_CHANNEL_DEAL*2;
		for (; d < d_end; s += 2*WAVE_CHANNEL_DEAL, d += 2*WAVE_CHANNEL_DEAL) {
			d[0] = s[0]; d[1] = s[1];
		}
		data[i] = dsp_tmpbuf[i];
	}
	/* 使用していないチャンネルを無音にしておく */
	for (i=k; i<WAVE_CHANNEL_DEAL; i++) {
		volume[i] = 0; data[i] = pcm->wave_buf;
	}
#endif
	/* 一般的な合成処理 */
	if (k == 0) {
		memset(buf, 0, len*4);
	} else {
		if (pcm->is_mix) {
			for (i=0; i<len; i++) {
				int j; int l=0, r=0;
				for (j=0; j<WAVE_CHANNEL_DEAL; j++) {
					l += data[j][0] * volume[j];
					r += data[j][1] * volume[j];
					data[j] += 8;
				}
				l >>= 12; r >>= 12; /* 4096 で割る */
				*buf++ = l; *buf++ = r;
			}
		} else { /* no mix */
			int vol = volume[0];
			for (i=0; i<len; i++) {
				*buf++ = (data[0][0]*vol)>>10;
				*buf++ = (data[0][1]*vol)>>10;
				data[0] += 8;
			}
		}
	}

	/* データの更新 */
	play_pt += len;
	if (play_pt < WAVE_SIZE) pcm->play_pointer = play_pt;
	else pcm->play_pointer = play_pt - WAVE_SIZE;
	LogPrintf("mix length %5d new pt %5d\n",len,play_pt);

	/* next_read_flag == -1 なら、次の wave 読み込みのチェック
	** 次のwave読み込みをすべきなら、next_read_flag = 1 にする
	** また、pcm再生が読み込みにおいついてしまっている場合、
	** 読み込みポインタを再生位置に動かしておく
	*/
	
	for (i=0; i<WAVE_CHANNEL_DEAL; i++) {
		int margin;
		if (read_pt[i] < 0) continue;
		LogPrintf("mix alarm check chn %d read_pt %d "
			"play_pt %d next_read %d margin %d",
			i,read_pt[i],
			play_pt,pcm->next_read_flag[i],pcm->next_read_margin[i]);
		if (read_pt[i] == play_pt-len) {
			if (read_pt[i] == pcm->read_pointer[i]) {
				if (pcm->next_read_flag[i] == 2)
					pcm->read_pointer[i] = -1;
				else
					pcm->read_pointer[i] = pcm->play_pointer;
				if (pcm->next_read_flag[i] < 0) {
					pcm->next_read_flag[i] = 1;

					kill(pcm->child_pid[i],SIGALRM);
					LogPrintf("mix alarm(1) %d", i);
				}
			}
			continue;
		}
		if (pcm->next_read_flag[i] != -1) continue;
		/* play_pt - margin よりも read pt が古いなら
		** flag が立つ
		*/
		margin = pcm->next_read_margin[i];
		if (play_pt < margin) {
			if (read_pt[i] > play_pt && read_pt[i] < play_pt + WAVE_SIZE - margin) {
				pcm->next_read_flag[i] = 1;
				kill(pcm->child_pid[i],SIGALRM);
				LogPrintf("mix alarm(2) %d", i);
			}
		} else {
			if (read_pt[i] < play_pt - margin || read_pt[i] > play_pt) {
				pcm->next_read_flag[i] = 1;
				kill(pcm->child_pid[i],SIGALRM);
				LogPrintf("mix alarm(2) %d", i);
			}
		}
	}
	return len*4;
}

static void pcmserver(DSPFILE *dfile) {
	short* dsp_tmpbuf[WAVE_CHANNEL_DEAL];
	int i; int status = 0;

	/* priority を下げておく */
	/* setpriority(PRIO_PROCESS, getpid(), 5); */
	if (dfile->dspblksiz < mus_get_default_rate()*4/20) {
		dfile->dspblksiz = (mus_get_default_rate()*4/20) & (~3);
		free(dfile->dspbuf);
		dfile->dspbuf = (char*)malloc(dfile->dspblksiz);
	}
	if (dfile->dspblksiz > WAVE_MAX_BLKSIZ) {
		dfile->dspblksiz = WAVE_MAX_BLKSIZ;
		free(dfile->dspbuf);
		dfile->dspbuf = (char*)malloc(dfile->dspblksiz);
	}
	if (dfile->dspbuf == NULL) return;

	/* dsp_tmpbuf : wave 合成時の一時バッファ */
	dsp_tmpbuf[0] = (short*)malloc(dfile->dspblksiz / 4 * 2 * 2 * WAVE_CHANNEL_DEAL);
	if (dsp_tmpbuf[0] == NULL) return;
	for (i=1; i<WAVE_CHANNEL_DEAL; i++) {
		dsp_tmpbuf[i] = dsp_tmpbuf[0] + i*2;
	}

#ifndef SUPRESS_SERVER
	audioFlush(dfile);

	while(TRUE) {
		/* 合成しては書き込み、をくりかえす */
		int len; int sleep_len;
		int is_block = 0;

		len = mix_wave(dfile, dsp_tmpbuf);
		if( (sleep_len = audioRest(dfile) - dfile->dspblksiz) > 0) {
			/* sleep_len の長さのデータ再生中、暇 */
			/* とりあえず sleep_len/2 の長さまで寝る */
			/* １つのデータにつき、short, stereo なので 4byte */
			sleep_len = (int)(
				((double)(sleep_len)) * 1000.0 * 1000.0 / mus_get_default_rate() / 4);
			sleep_len /= 2;
			if (sleep_len < 1000) sleep_len = 1000;
			usleep(sleep_len);
		}
		if (len > 0){
			if (status) status--;
			audioWrite(dfile, len);
			/* 書き込みが dspblksiz に満たない場合、
			** 音が鳴るのに時間がかかるのでウェイト無しで
			** 次のデータをつくる
			*/
			if (len < dfile->dspblksiz) {
				status = 2;
			}
		} else if(status==1){
			/* 無音のデータを書いておく */
			status=0;
			len = dfile->dspblksiz;
			memset(dfile->dspbuf, 0, dfile->dspblksiz);
			audioWrite(dfile, len);
			audioWrite(dfile, len);
			audioWrite(dfile, len);
		} else {
			if (status) status--;
			audioFlush(dfile);
			is_block = 1; /* server への要求まで待つ */
		}
		if(status == 0){
			server_wait(is_block);
			usleep(1000);
		}
		if (pcm->device_release_query) break; /* release 要求が来たら一旦終了 */
	}
#else
	sleep(99999);
#endif
	free(dsp_tmpbuf[0]);
}

/* pcm server の開始 */
int pcm_init(void) {
	int i;
	pid_t pid_master;
	/* shm の確保 */
	if (0 > (svrIPC_Wave = shmget(IPC_PRIVATE, sizeof(PCMDATA), IPC_CREAT | 0777))) {
		fprintf(stderr, "shmget %s\n", strerror(errno));
		goto err;
	}
	if (NULL == (pcm = (PCMDATA*)shmat(svrIPC_Wave, 0, 0))) {
		fprintf(stderr, "shmat %s\n", strerror(errno));
		goto err;
	}
	/* semaphore の確保 */
	if (0 > (svrIPC_Wait = semget(IPC_PRIVATE, PCM_SEM_DEAL, IPC_CREAT |
		(SEM_R | SEM_A) | ((SEM_R | SEM_A )>>3) | ((SEM_R | SEM_A )>>6)))) {
		fprintf(stderr, "semget %s\n", strerror(errno));
		goto err;
	}
	/* 必要なら log を開く */
	LogOpen();
	/* pcm を初期化 */
	pcm->is_mix = is_mix_pre_define;
	pcm->play_pointer = 0;
	pcm->device_release_query = 0;
	for (i=0; i<WAVE_CHANNEL_DEAL; i++) {
		clear_pcm(i);
	}
	
	/* daemon 起動 */
	pid_master = forkpg_local();
	if (pid_master == -1) goto err; /* fork error */
	if (pid_master == 0) { /* child process */
		DSPFILE *dfile;
		set_signalhandler(signal_int_pcm,SIGTERM);
		set_signalhandler(signal_abort_pcm, SIGABRT);
		while(1) {
			wfile.wavinfo.SamplingRate = mus_get_default_rate();
			wfile.wavinfo.Channels = Stereo;
			wfile.wavinfo.Samples = 0;
			wfile.wavinfo.DataBits = 16;
			wfile.wavinfo.DataStart = 0;
			wfile.wavinfo.DataBytes = 0;
			wfile.wavinfo.DataBytes_o = 0;
			wfile.data = NULL;
			if (! pcm->device_release_query) {
				struct sembuf ops[2];
				ops[0].sem_num = PCM_SEM_DEVICE;
				ops[0].sem_op  = 0; /* semaphore が 0 になるまで wait */
				ops[0].sem_flg = 0;
				ops[1].sem_num = PCM_SEM_DEVICE;
				ops[1].sem_op  = 1;
				ops[1].sem_flg = 0;
				if (0 > semop(svrIPC_Wait, ops, 2)) {
					fprintf(stderr,"sem error in pcm_server : %s\n",strerror(errno));
					sleep(1);
					continue;
				}
				if (NULL != (dfile = OpenDSP(&wfile))) {
					if (pcm->is_mix) mixer_setvolume(MIX_PCM, 100);
					pcmserver(dfile);
					CloseDSP(dfile);
				} else {
					sleep(1);
				}
				ops[0].sem_num = PCM_SEM_DEVICE;
				ops[0].sem_op = -1;
				ops[0].sem_flg = 0;
				if (0 > semop(svrIPC_Wait, ops, 1)) {
					fprintf(stderr,"sem error in pcm_server : %s\n",strerror(errno));
					sleep(1);
					continue;
				}
			}
			usleep(10000); /* 一応、ちょっと待つ */
		}
		_exit(0);
	}
	pcm_initialized = TRUE;
	return OK;
err:
	pcm_remove(0);
	return NG;
}

void pcm_remove(int is_abort) {
	/* shm を解放 */
	if (pcm != NULL) {
		if (shmdt( (char*)pcm ) < 0) {
			fprintf(stderr, "shmdt %s\n", strerror(errno));
		}
		pcm = NULL;
	}
	if (svrIPC_Wave != -1) {
		if (shmctl(svrIPC_Wave, IPC_RMID, 0) < 0) {
			fprintf(stderr, "shmctl %s\n", strerror(errno));
		}
		svrIPC_Wave = -1;
	}
	if (svrIPC_Wait != -1) {
		if (semctl(svrIPC_Wait, 0, IPC_RMID) < 0) {
			fprintf(stderr, "semctl %s\n", strerror(errno));
		}
		svrIPC_Wait = -1;
	}
	pcm_initialized = FALSE;
}

/* セマフォ周りの処理 */
/* pcm server が時間待ちに入ったら呼ぶ */
static void wait_for_play(int channel) {
#ifndef SUPRESS_SERVER
	struct sembuf ops[5];
	/* セマフォの動作：
		１，別のセマフォ処理が動いてないことを確認
		２，セマフォ動作中のフラグを立てる
		３，サーバー側にウェイトを求めるフラグを立てる
		４，サーバー側がウェイトに入るまで待つ
	*/
	/* この channel の終了処理のし忘れがないか、確認 */
	end_wait_for_play(channel);

	ops[0].sem_num = PCM_SEM_USING;
	ops[0].sem_op  = 0; /* semaphore が 0 になるまで wait */
	ops[0].sem_flg = 0;

	ops[1].sem_num = PCM_SEM_CHN+channel;
	ops[1].sem_op  = 1; /* semaphore を +1 */
	ops[1].sem_flg = 0;

	ops[2].sem_num = PCM_SEM_USING;
	ops[2].sem_op  = 1; /* semaphore を +1 */
	ops[2].sem_flg = 0;

	ops[3].sem_num = PCM_SEM_WAIT;
	ops[3].sem_op  = 1; /* semaphore を 1 にする */
	ops[3].sem_flg = 0;

	ops[4].sem_num = PCM_SEM_WAIT;
	ops[4].sem_op  = 0; /* server が semaphore をクリアするまで待つ */
	ops[4].sem_flg = 0;

	/* 前処理 */
	if (0 > semop(svrIPC_Wait, ops, 4)) {
		fprintf(stderr,"sem error in wait_for_play : %s\n",strerror(errno));
	}
	/* ウェイト */
	if (0 > semop(svrIPC_Wait, ops+4, 1)) {
		fprintf(stderr,"sem error in wait_for_play : %s\n",strerror(errno));
	}
#if FreeBSD_PTHREAD_ERROR == 1
	raise(SIGPROF);
#endif /* PTHREAD_ERROR */
#endif
	return;
}
/* play の時間待ちを終えたら呼ぶ */
static void end_wait_for_play(int channel) {
#ifndef SUPRESS_SERVER
	/* セマフォの動作：
		１，サーバー側のウェイトフラグを下げる
		２，セマフォ動作中のフラグを下げる
	*/
	struct sembuf ops[3]; int sem_ret;
	/* まず、channel がセマフォを使用中か確認 */

	if (0 > (sem_ret = semctl(svrIPC_Wait, PCM_SEM_CHN+channel, GETVAL))) return;
	if (sem_ret == 0) return; /* セマフォ使用中ではない */

	semctl(svrIPC_Wait, PCM_SEM_WAIT, SETVAL, 0);
	ops[0].sem_num = PCM_SEM_SERVER_WAIT;
	ops[0].sem_op  = -1; /* semaphore のクリア */
	ops[0].sem_flg = IPC_NOWAIT;

	ops[1].sem_num = PCM_SEM_USING;
	ops[1].sem_op  = -1; /* semaphore を クリア */
	ops[1].sem_flg = IPC_NOWAIT;

	ops[2].sem_num = PCM_SEM_CHN+channel;
	ops[2].sem_op  = -1; /* semaphore を クリア */
	ops[2].sem_flg = IPC_NOWAIT;

	/* SERVER_WAIT については立ってない可能性もある */
	/* なので、エラーメッセージは出さない */
	semop(svrIPC_Wait, ops, 1);
	/* のこり二つは、エラーでクリアされてない限り立っている */
	if (0 > semop(svrIPC_Wait, ops+1, 2)) {
		fprintf(stderr,"sem error in wait_for_play : %s\n",strerror(errno));
	}
#endif
	return;
	
}
/* pcm server 側が時間待ちに入る */
static void server_wait(int is_block) {
#ifndef SUPRESS_SERVER
	int sem_ret; int i;
	struct sembuf ops[2];
	/* セマフォの動作：
		１，サーバー のウェイト要求を調べる
		２，(ウェイト要求がないなら終了)
		３，ウェイト要求があるなら、ウェイト動作中フラグを立てる
		４，ウェイト要求をクリア
		５，ウェイト動作中フラグが終了するまで待つ
		　　ただし、最大１００ミリ秒（usleep(100) * 100 まで）
	*/
	
	ops[0].sem_num = PCM_SEM_WAIT;
	ops[0].sem_op  = -1; /* semaphore を クリア */
	ops[0].sem_flg = is_block ? 0 : IPC_NOWAIT; /* is_block が真なら semaphore が立つまで待つ */

	ops[1].sem_num = PCM_SEM_SERVER_WAIT;
	ops[1].sem_op  = 1; /* semaphore を立てる */
	ops[1].sem_flg = 0;

	if (0 > semop(svrIPC_Wait, ops, 2)) {
		if (errno == EAGAIN) return; /* 要求無し */
		goto err; /* それ以外の errno ならエラー */
	}
#if FreeBSD_PTHREAD_ERROR == 1
	raise(SIGPROF);
#endif /* PTHREAD_ERROR */

	/* 最大 100ms 待つ*/
	for (i=0; i<100; i++) {
		if (0 > (sem_ret = semctl(svrIPC_Wait, PCM_SEM_SERVER_WAIT, GETVAL))) goto err;
		if (sem_ret == 0) break;
		usleep(1000);
	}
	if (i != 100) {
		return;
	}
	errno = ETIMEDOUT;

err:
	/* エラーが出たら、全セマフォを強制クリア */
	fprintf(stderr,"sem error in server_wait : clear all semaphores : %s\n",strerror(errno));
	semctl(svrIPC_Wait, PCM_SEM_WAIT, SETVAL, 0);
	semctl(svrIPC_Wait, PCM_SEM_SERVER_WAIT, SETVAL, 0);
	semctl(svrIPC_Wait, PCM_SEM_USING, SETVAL, 0);
#endif
	return;
}

static int current_buf_number = 3;
static void signal_int(int sig_num) {
	end_wait_for_play(current_buf_number);
	pcm->read_pointer[current_buf_number] = -1;
	pcm->next_read_flag[current_buf_number] = 0;
	exit(0);
}
static void set_child_pid(void) {
	pcm->child_pid[current_buf_number] = getpid();
}

pid_t effec_start(char* path, int loop_count) {
	pid_t p;
	FILE* file; char buf[1024];
	WAVFILE* wf; int first; int offset; int i; int length = -1;

	if (! pcm_initialized) return 0;
	/* ファイルを開く */
	file = OpenWaveFile(path, &length);
	if (file == 0) { // ファイルが見付からない
		fprintf(stderr,"effec start: cannot open wave file %s\n",path);
		return 0;
	}
	offset = ftell(file);
	p = forkpg_local();
	if (p == -1) {fclose(file); return 0;}
	if (p != 0) {fclose(file); return p;}
	/* setpriority(PRIO_PROCESS, getpid(), 10); */
	current_buf_number = CHN_EFFEC;
	set_child_pid();
	set_signalhandler(signal_alarm,SIGALRM);
	set_signalhandler(signal_int, SIGTERM);
	/* read wave file */
	for (i=0; i<loop_count; i++) {
		int end_off = offset + length;
		fseek(file, offset, 0);
		fread(buf, 1024, 1, file);
		wf = WavGetInfo(buf);
		if (wf == NULL) {
			fclose(file); exit(0);
		}
		first = wf->data - buf;
		read_wave(CHN_EFFEC, file, wf, first, 1024-first, end_off);
	}
	fclose(file);
	pcm->next_read_flag[current_buf_number] = 2;
	exit(0);
}

pid_t bgm_loop_start(char* path, int loop_count) {
	pid_t p; int i;
	FILE* file; char buf[1024]; int offset;
	WAVFILE* wf; int first; int length = -1;

	if (! pcm_initialized) return 0;
	file = OpenWaveFile(path, &length);
	if (file == 0) { // ファイルが見付からない
		fprintf(stderr,"bgm loop start: cannot open wave file %s\n",path);
		return 0;
	}
	offset = ftell(file);
	p = forkpg_local();
	if (p == -1) {fclose(file); return 0;}
	if (p != 0) {fclose(file); return p;}
	/* setpriority(PRIO_PROCESS, getpid(), 10); */
	current_buf_number = CHN_BGM;
	set_child_pid();
	set_signalhandler(signal_alarm,SIGALRM);
	set_signalhandler(signal_int,SIGTERM);
	/* read wave file */
	for (i=0; i<loop_count; i++) {
		int end_off = offset + length;
		fseek(file, offset, 0);
		fread(buf, 1024, 1, file);
		wf = WavGetInfo(buf);
		if (wf == NULL) {
			fclose(file); exit(0);
		}
		first = wf->data - buf;
		read_wave(CHN_BGM, file, wf, first, 1024-first,end_off);
	}
	fclose(file);
	pcm->next_read_flag[current_buf_number] = 2;
	exit(0);
}

pid_t koe_start(const char* path) {
	pid_t p;
	short* data; int len = 0;
	int ext_data[256];
	AvgKoeInfo koeinfo;

	if (! pcm_initialized) return 0;
	koeinfo = OpenKoeFile(path);
	if (koeinfo.stream == 0) return 0;
	p = forkpg_local();
	if (p == -1) {fclose(koeinfo.stream);return 0;}
	if (p != 0) {fclose(koeinfo.stream); return p;}
	/* setpriority(PRIO_PROCESS, getpid(), 10); */
	current_buf_number = CHN_KOE;
	set_child_pid();
	set_signalhandler(signal_alarm,SIGALRM);
	set_signalhandler(signal_int,SIGTERM);
	/* read koe file */
	data = decode_koe(koeinfo, &len);

	if (data == NULL) {
		exit(0);
	}
	memset(ext_data,0,sizeof(ext_data));
	if (koeinfo.rate < mus_get_default_rate()) {
		short* new_pt = malloc(len*4*(mus_get_default_rate()/koeinfo.rate+1));
		memcpy(new_pt, data, len*4);
		free(data); data = new_pt;
	}
	len = conv_wave_rate(data, len, koeinfo.rate, ext_data);
	write_raw(CHN_KOE, data, len);
	free(data);
	fclose(koeinfo.stream);
	pcm->next_read_flag[current_buf_number] = 2;
	exit(0);
}

int mus_bgm_getStatus(int *pos) {
	if (! pcm_initialized) return 0;
	
	*pos = 0;
	if (pcm->read_pointer[CHN_BGM] < 0) return 0;
	*pos = calc_playtime(CHN_BGM);
	if (*pos < 0) {
		*pos = 0; return 0;
	} else {
		return 1;
	}
}

int mus_koe_getStatus(int *pos) {
	if (! pcm_initialized) return 0;
	
	*pos = 0;
	if (pcm->read_pointer[CHN_KOE] < 0) return 0;
	*pos = calc_playtime(CHN_KOE);
	if (*pos < 0) {
		*pos = 0; return 0;
	} else return 1;
}

int mus_effec_getStatus(int *pos) {
	if (! pcm_initialized) return 0;
	
	*pos = 0;
	if (pcm->read_pointer[CHN_EFFEC] < 0) return 0;
	*pos = calc_playtime(CHN_EFFEC);
	if (*pos < 0) {
		*pos = 0; return 0;
	} else return 1;
}

void mus_pcmserver_stop(void) {
	struct sembuf ops[2];
	if (pcm == 0) return;
	if (pcm->device_release_query != 0) return;
	wait_for_play(4);
	pcm->device_release_query = 1;
	end_wait_for_play(PCM_SEM_CHN);
	ops[0].sem_num = PCM_SEM_DEVICE;
	ops[0].sem_op  = 0; /* semaphore が 0 になるまで wait */
	ops[0].sem_flg = 0;
	ops[1].sem_num = PCM_SEM_DEVICE;
	ops[1].sem_op  = 1;
	ops[1].sem_flg = 0;
	if (0 > semop(svrIPC_Wait, ops, 2)) {
		fprintf(stderr,"sem error in pcm_server_stop : %s\n",strerror(errno));
	}
	pcm->device_release_query = 2;
	/* volume を一時的に小さくする */
	if (pcm->is_mix) mixer_setvolume(MIX_PCM, 25);
	return;
}
void mus_pcmserver_resume(void) {
	struct sembuf ops[1];
	if (pcm == 0) return;
	if (pcm->device_release_query != 2) return;
	pcm->device_release_query = 0;
	ops[0].sem_num = PCM_SEM_DEVICE;
	ops[0].sem_op  = -1;
	ops[0].sem_flg = 0;
	if (0 > semop(svrIPC_Wait, ops, 1)) {
		fprintf(stderr,"sem error in pcm_server_resume : %s\n",strerror(errno));
	}
}

void mus_pcm_set_mix(int is_mix) {
	if (! pcm_initialized) is_mix_pre_define = is_mix;
	else pcm->is_mix = is_mix;
}

extern unsigned short koe_8bit_trans_tbl[256]; /* koedec.c で定義 */
int mus_set_8to16_usetable(int is_use) {
	int old = is_8to16_usetable;
	if (is_use) is_8to16_usetable = TRUE;
	else is_8to16_usetable = FALSE;
	return old;
}
static int pcm_default_rate = 44100;
void mus_set_default_rate(int rate) {
	pcm_default_rate = rate;
}
int mus_get_default_rate(void) {
	return pcm_default_rate;
}

/* pcm->next_read_XXX の更新関係の処理 */
/* 次の読み込みが始まるまでまつ */
static void wait_next_read(int number, int margin) {
#ifdef SUPRESS_SERVER
	sleep(99999);
#else
	int read_pt, play_pt;
	/* margin 以上のデータが書き込み可能になるまで待つ */
	/* まず、margin が小さすぎ・大きすぎの補正 */
	if (margin <= 1024) margin = WAVE_DEFAULT_MARGIN;
	if (margin > WAVE_SIZE/2) margin = WAVE_SIZE/2;
	if (number < 0 || number >= WAVE_CHANNEL_DEAL) number = WAVE_CHANNEL_DEAL-1;
	/* margin 以下しか読み込んでないならすぐに戻る */
	read_pt = pcm->read_pointer[number]; play_pt = pcm->play_pointer;
	if (play_pt < margin && (read_pt > play_pt && read_pt < (play_pt+WAVE_SIZE-margin))) return;
	if (play_pt >= margin && (read_pt < play_pt-margin || read_pt > play_pt)) return;
	
	/* pcmserver からの signal を待つ */
	pcm->next_read_margin[number] = margin;
	pcm->next_read_flag[number] = -1;
	while(pcm->next_read_flag[number] < 0) {
		sleep(999999); /* pcmserver が alarm を出すまで待つ */
	}
	pcm->next_read_flag[number] = 0;
#endif
}
/* pcm->XXX_pointer などの初期化 */
static void clear_pcm(int number) {
	int i; short* d;
	if (number < 0 || number >= WAVE_CHANNEL_DEAL) number = WAVE_CHANNEL_DEAL-1;
	pcm->next_read_flag[number] = 0;
	pcm->next_read_margin[number] = 1024;
	pcm->read_pointer[number] = -1;
	pcm->read_deal[number] = 0;
	d = pcm->wave_buf + number*2;
	for (i=0; i<WAVE_SIZE; i++) {
		d[0] = d[1] = 0; d+=8;
	}
	return;
}

/* pcm->read_pointer の操作：read_point を一定数進める */
static void proceed_pcm(int number, int count) {
	int is_alarm = 0;
	if (number < 0 || number >= WAVE_CHANNEL_DEAL) number = WAVE_CHANNEL_DEAL-1;
	if (pcm->read_pointer[number] < 0 ||
		pcm->read_pointer[number] == pcm->play_pointer) is_alarm = 1;
	if (pcm->read_pointer[number] + count < WAVE_SIZE) {
		pcm->read_pointer[number] += count;
	} else {
		pcm->read_pointer[number] =
			pcm->read_pointer[number] + count - WAVE_SIZE;
	}
}
/* pcm->read_pointer にデータ代入 */
static void output_pcm(int number, short* src, int count) {
	int i; int read_pt; short* dest;
	if (number < 0 || number >= WAVE_CHANNEL_DEAL) number = WAVE_CHANNEL_DEAL-1;
	read_pt = pcm->read_pointer[number];
	LogPrintf("out chn %d, read_pt %d",number,read_pt);
	/* 先に read_pointerは進めておく */
	proceed_pcm(number, count);
	pcm->read_deal[number] += count;

	/* copy */
	if (read_pt + count <= WAVE_SIZE) {
		dest = pcm->wave_buf + number*2 + read_pt*8;
		LogPrintf("out chn %d, out %5d - %5d",number,read_pt,read_pt+count);
		for (i=0; i<count; i++) {
			dest[0] = src[0]; dest[1] = src[1];
			dest += 8; src += 2;
		}
	} else {
		int count1, count2;
		count1 = WAVE_SIZE - read_pt;
		count2 = count - count1;
		dest = pcm->wave_buf + number*2 + read_pt*8;
		LogPrintf("out chn %d, out %5d - %5d",number,read_pt,read_pt+count1);
		for (i=0; i<count1; i++) {
			dest[0] = src[0]; dest[1] = src[1];
			dest += 8; src += 2;
		}
		LogPrintf("out chn %d, out %5d - %5d",number,0,count2);
		dest = pcm->wave_buf + number*2;
		for (i=0; i<count2; i++) {
			dest[0] = src[0]; dest[1] = src[1];
			dest += 8; src += 2;
		}
	}
	LogPrintf("out end chn %d, read_pt %d",number,pcm->read_pointer[number]);
	return;
}

	

/* conv_wave の下請け関数群 */
/* 8bit -> 16bit の変換。 */
static int conv_wave_8(char* in_buf, int length) {
	int i;
	short* out_buf = (short*)in_buf;
	if (is_8to16_usetable) {
		for (i=length-1; i>=0; i--) {
			out_buf[i] =
				koe_8bit_trans_tbl[(unsigned char)(in_buf[i])];
		}
	} else {
		for (i=length-1; i>=0; i--) {
			out_buf[i] =
				((short)in_buf[i]) << 8;
		}
	}
	return length*2;
}

/* unsigned -> signed の変換 */
static int conv_wave_signed(short* in_buf, int length) {
	int i;
	for (i=0; i<length; i++)
		in_buf[i] -= 0x8000;
	return length;
}
/* mono -> stereo の変換(short) */
static int conv_wave_stereo(short* in_buf, int length) {
	int i;short* out_buf;
	out_buf = in_buf + length*2 - 1;
	in_buf += length - 1;
	for (i=0; i<length; i++) {
		*out_buf-- = *in_buf;
		*out_buf-- = *in_buf--;
	}
	return length*2;
}
/* sampling rate の変換 */
/* stereo, signed short のデータを仮定 */
static int conv_wave_rate_half(short* in_buf, int length, int rate, short* ext_data) {
	short* out_buf;
	int i = 0;
	if (length <= 0) return 0;
	out_buf = in_buf;
	if (ext_data[0]) { /* 前回、length が奇数だった */
		*out_buf++ = in_buf[0]/2 + ext_data[1]/2;
		*out_buf++ = in_buf[1]/2 + ext_data[2]/2;
		in_buf += 2; i=1; length++;
	}
	ext_data[0] = 0;
	if (length & 1) {
		ext_data[0] = 1;
		ext_data[1] = in_buf[length*2-2];
		ext_data[2] = in_buf[length*2-1];
		length--;
	}
	length /= 2;
	for (; i<length; i++) {
		*out_buf++ = in_buf[0]/2 + in_buf[2]/2;
		*out_buf++ = in_buf[1]/2 + in_buf[3]/2;
		in_buf += 4;
	}
	return length;
}
static int conv_wave_rate_twice(short* in_buf, int length, int rate, short* ext_data) {
	int i; short* out_buf; short ext1, ext2;
	if (length <= 0) return 0;
	out_buf = in_buf + length*4;
	in_buf += length*2;
	ext1 = ext_data[0]; ext2 = ext_data[1];
	ext_data[0] = in_buf[-2];
	ext_data[1] = in_buf[-1];

	in_buf -= 2;
	for (i=0; i<length-1; i++) {
		in_buf -= 2; out_buf -= 4;
		out_buf[0] = (((int)(in_buf[2]))+((int)(in_buf[0])))/2;
		out_buf[1] = (((int)(in_buf[3]))+((int)(in_buf[1])))/2;
		out_buf[2] = in_buf[2];
		out_buf[3] = in_buf[3];
	}
	out_buf -= 4;
	out_buf[2] = in_buf[0];
	out_buf[3] = in_buf[1];
	out_buf[0] = (((int)(in_buf[0]))+((int)(ext1)))/2;
	out_buf[1] = (((int)(in_buf[1]))+((int)(ext2)))/2;
	
	return length*2;
}

static int conv_wave_rate(short* in_buf, int length, int rate, int* ext_data) {
	int input_rate = rate;
	int output_rate = mus_get_default_rate();
	double input_rate_d = input_rate, output_rate_d = output_rate;
	double dtime; int outlen; short* out, * out_orig; int next_sample1, next_sample2;
	short* in_buf_orig = in_buf;
	int i; int time;

	if (rate == mus_get_default_rate()) return length;
	else if (rate == mus_get_default_rate()*2) return conv_wave_rate_half(in_buf,length,rate,(short*)ext_data);
	else if (rate*2 == mus_get_default_rate()) return conv_wave_rate_twice(in_buf,length,rate,(short*)ext_data);
	if (length <= 0) return 0;
	/* 一般の周波数変換：線型補完 */
/* 構造体を使うべきなんだが……*/
#define first_flag ext_data[0]
#define prev_time ext_data[1]
#define prev_sample1 ext_data[2]
#define prev_sample2 ext_data[3]
	/* 初めてならデータを初期化 */
	if (first_flag == 0) {
		first_flag = 1;
		prev_time = 0;
		prev_sample1 = *in_buf++;
		prev_sample2 = *in_buf++;
		length--;
	}
	/* 今回作成するデータ量を得る */
	dtime = prev_time + length * output_rate_d;
	outlen = (int)(dtime / input_rate_d);
	out = malloc( (outlen+1)*sizeof(short)*2);
	out_orig = out;
	if (first_flag == 1) {
		*out++ = prev_sample1;
		*out++ = prev_sample2;
	}
	dtime -= input_rate_d*outlen; /* 次の prev_time */

	time=0;
	next_sample1 = *in_buf++; next_sample2 = *in_buf++;
	for (i=0; i<outlen; i++) {
		/* double で計算してみたけどそう簡単には高速化は無理らしい */
		/* なお、変換は 1分のデータに1秒程度かかる(Celeron 700MHz) */
		time += input_rate;
		while(time-prev_time>output_rate) {
			prev_sample1 = next_sample1;
			next_sample1 = *in_buf++;
			prev_sample2 = next_sample2;
			next_sample2 = *in_buf++;
			prev_time += output_rate;
		}
		*out++ =
			((time-prev_time)*next_sample1 +
			(input_rate-time+prev_time)*prev_sample1) / input_rate;
		*out++ =
			((time-prev_time)*next_sample2 +
			(input_rate-time+prev_time)*prev_sample2) / input_rate;
	}
	prev_time += output_rate; prev_time -= input_rate * outlen;
	prev_sample1 = next_sample1; prev_sample2 = next_sample2;
	if (first_flag == 1) {
		outlen++; first_flag = 2;
	}
	memcpy(in_buf_orig, out_orig, outlen*2*sizeof(short));
	free(out_orig);
	return outlen;
}

/* 最大で length の長さの wave file を読み込み、signed short, stereo, 22050kHz の形式に変換する */
/* in_buf は length*8 以上の長さを持っていること。 */
static int conv_wave(char* in_buf, FILE* in_stream, int length, WAVFILE* wf, int* wf_datalen, int end_offset) {
	int blksize = 1; int blklen;
	/* block size の設定 */
	if (wf->wavinfo.Channels == Stereo) blksize *= 2;
	if (wf->wavinfo.DataBits == 16) blksize *= 2;
	/* ファイルの読み込み */
	/* wf->data にデータの残りがあればそれも読み込む */
	if (*wf_datalen > blksize*length) {
		memcpy(in_buf, wf->data, blksize*length);
		wf->data += blksize*length;
		*wf_datalen -= blksize*length;
		blklen = blksize*length;
	} else {
		int rest_length = end_offset - ftell(in_stream);
		memcpy(in_buf, wf->data, *wf_datalen);
		if (rest_length < blksize*length-*wf_datalen) {
			length = (rest_length+*wf_datalen+blksize-1)/blksize;
		}
		blklen = fread(in_buf+*wf_datalen, 1, blksize*length-*wf_datalen, in_stream);
		blklen += *wf_datalen;
		*wf_datalen = 0;
	}
	blklen /= blksize;
	if (wf->wavinfo.DataBits == 8) conv_wave_8(in_buf, blklen*blksize);
	if (wf->wavinfo.Channels == Mono) conv_wave_stereo( (short*)in_buf, blklen);
	if (wf->wavinfo.SamplingRate != mus_get_default_rate())
		blklen = conv_wave_rate( (short*)in_buf, blklen, wf->wavinfo.SamplingRate, wf->other_data);
	return blklen;
}

static int calc_playtime(int number) {
	int tm;
	tm = pcm->read_pointer[number] - pcm->play_pointer;
	if (tm < 0) tm += WAVE_SIZE;
	tm = pcm->read_deal[number] - tm;
	if (tm < 0) return -1;
	else return tm / 22050;
}

static void read_wave(int number, FILE* f, WAVFILE* wf, int first_pt, int wf_datalen, int end_offset) {
	int data_deal; int blks;
	int margin = WAVE_DEFAULT_MARGIN; /* 先読みは 8192 block(=0.2sec) 行う */
	char* in_buf;
	/* 初期化 */
	clear_pcm(number);
	data_deal = wf->wavinfo.DataBytes_o;
	if (data_deal == 0) {
		data_deal = 1024*1024*1024; /* データ量が不明なら、とりあえず 1G ということにする */
	}
	in_buf = (char*)malloc(margin * 8);
	if (in_buf == NULL) {
		return;
	}

	/* 最初のデータを読む */
	blks = conv_wave(in_buf, f, margin, wf, &wf_datalen, end_offset);
	/* 再生開始：driver が sleep に入るのを待つ */
	wait_for_play(number);
	/* 再生開始 */
	pcm->read_pointer[number] = pcm->play_pointer;
	/* データ代入 */
	output_pcm(number, (short*)in_buf, blks);
	data_deal -= margin;
	/* driver の sleep を解く */
	end_wait_for_play(number);
	/* 再生していく */
	while(data_deal > 0 && ftell(f) < end_offset && !feof(f)) {
		blks = conv_wave(in_buf, f, margin, wf, &wf_datalen, end_offset);
		wait_next_read(number, blks);
		output_pcm(number, (short*)in_buf, blks);
		data_deal -= margin*4;
	}
	free(in_buf);
	return;
}
static void write_raw(int number, short* data, int length) {
	int margin = WAVE_DEFAULT_MARGIN; /* 先読みは 8192 block(=0.2sec) 行う */
	if (margin > length) margin = length;
	/* 初期化 */
	clear_pcm(number);
	/* 再生開始：driver が sleep に入るのを待つ */
	wait_for_play(number);
	/* データ代入 */
	pcm->read_pointer[number] = pcm->play_pointer;
	output_pcm(number, data, margin);
	length -= margin; data += margin*2;
	/* driver の sleep を解く */
	end_wait_for_play(number);
	/* 再生していく */
	while(length > 0) {
		wait_next_read(number, margin);
		if (margin > length) margin = length;
		output_pcm(number, data, margin);
		length -= margin; data += margin*2;
	}
	return;
}
