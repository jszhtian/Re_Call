/*
 * cdrom.mp3.c  CD-ROMのかわりにMP3fileだ！
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include "portab.h"
#include "cdrom.h"
#include "counter.h"
#include "audio.h"
#include "music.h"


//extern void sys_set_signalhandler(int SIG, void (*handler)(int));

/*
   CPUパワーがあまってあまってどうしようもない人へ :-)

  使い方

   1. とりあえず mpg123 などの プレイヤーを用意する。
      esd を使いたい場合は Ver 0.59q 以降を入れよう。
      プレーヤーにはあらかじめパスを通しておく。

   2. % cat ~/game/kichiku.playlist
       mpg123 -quite
       $(HOME)/game/kichiku/mp3/trk02.mp3
       $(HOME)/game/kichiku/mp3/trk03.mp3
       $(HOME)/game/kichiku/mp3/trk04.mp3
       $(HOME)/game/kichiku/mp3/trk05.mp3
       $(HOME)/game/kichiku/mp3/trk06.mp3

       ってなファイルを用意する。( $(HOME)は適当にかえてね )
       １行目はプレーヤーとそのオプション
       ２行目以降はトラック２から順にファイルをならべる

   3 configure で --enable-cdrom=mp3 を追加する

   4 実行時オプションに --cddev ~/game/kichiku.playlist のように上で作成したファイルを指定

*/

static int  cdrom_init(char *);
static int  cdrom_exit();
static int  cdrom_start(int);
static int  cdrom_stop();
static int  cdrom_getPlayingInfo(cd_time *);
//static void cdrom_setAudioDevice(audio_t *);

#define cdrom cdrom_mp3
cdromdevice_t cdrom = {
	cdrom_init,
	cdrom_exit,
	cdrom_start,
	cdrom_stop,
	cdrom_getPlayingInfo,
	FALSE/*cdrom_setAudioDevice*/,
	FALSE
};

#define PLAYLIST_MAX 256


typedef struct {
	int   trk;
	pid_t pid;
	long  cnt;
} private_data;

static private_data *save;
static int          ipc_key;
static boolean      enabled = FALSE;
static char         mp3_player[256];
static int          argc;
static char         **argv;
static char         *playlist[PLAYLIST_MAX];
static int          lastindex;

/*
  外部プレーヤーの行の解析
  ** obsolete** 1. 最初の文字が - であった場合、piped play mode
  2. プログラムと引数の分離 (!pipe)
  3. プログラムからプログラム名(argv[0])を分離 (!piped)
*/

static void player_set(char *buf) {
	char *b, *bb;
	int i, j;
	
	if (buf[0] == '-') {
		/* pipedplay = TRUE; */ /* obsolete */
		buf++;
	} else {
	}
	cdrom.need_audiodevice = FALSE;
	
	strncpy(mp3_player, buf, sizeof(mp3_player));
	b = mp3_player;
	
	/* count arguments */
	i = j = 0;		
	while(*b != 0) {
		if (*(b++) == ' ' && j > 0) {
			i++; j = 0;
			while(*b == ' ') b++;
		} else {
			j++;
		}
		if (*b == '\n') *b = 0;
	}
	if (j == 0 && i > 0) i--;
	if (NULL == (argv = (char **)malloc(sizeof(char *) * (i +3)))) {
		return;
	}
	argc = i +1;
	
	/* devide argument */
	b = mp3_player;
	j = 0;
	while(j <= i) {
		argv[j] = b;
		while(*b != ' ' && *b != 0) b++;
		*b = '\0'; b++;
		while(*b == ' ' || *b == 0) b++;
		j++;
	}
	argv[j +1] = NULL;
	
	/* cut down argv[0] */
	bb = b = argv[0];
	while (*b != 0) {
		if ( *b == '/') bb = b +1;
		b++;
	}
	argv[0] = bb;
}


static int cdrom_init(char *config_file) {
	FILE *fp;
	char lbuf[256];
	int lcnt = 0;
	char *s;
	
	if (NULL == (fp = fopen(config_file, "rt"))) return NG;
	fgets(lbuf, sizeof(lbuf), fp); lcnt++;
	
	player_set(lbuf);
	
	while(TRUE) {
		if (++lcnt >= (PLAYLIST_MAX +2)) {
			break;
		}
		if (!fgets(lbuf, sizeof(lbuf) -1, fp)) {
			if (feof(fp)) {
				break;
			} else {
				perror("fgets()");
				fclose(fp);
				return NG;
			}
		}
		if (NULL == (playlist[lcnt -2] = malloc(strlen(lbuf) + 1))) {
			fclose(fp);
			return NG;
		}
		s = lbuf;
		while( *s != '\n' && *s != 0 ) s++;
		if( *s == '\n' ) *s=0;
		strcpy(playlist[lcnt - 2], lbuf);
	}
	lastindex = lcnt -1;
	fclose(fp);
	
	if (0 > (ipc_key = shmget(IPC_PRIVATE, sizeof(private_data), IPC_CREAT | 0700))) {
		perror("shmget");
		return NG;
	}
	if ((void *)-1 == (save = (private_data *)shmat(ipc_key, 0, 0))) {
		perror("shmat");
		return NG;
	}
	memset(save, 0, sizeof(private_data));
	save->trk = 999;
	
	reset_counter_high(SYSTEMCOUNTER_MP3, 10, 0);
	enabled = TRUE;
	
	return OK;
}


static int cdrom_exit() {
	if (enabled) {
		cdrom_stop();
		
		if (0 > shmdt((char *)save)) {
			perror("shmdt");
		}
		if (0 > shmctl(ipc_key, IPC_RMID, 0)) {
			perror("shmctl");
                }
	}
	return OK;
}

/* トラック番号 trk の演奏 trk = 1~ */
static int cdrom_start(int trk) {
	char cmd_pipe[256];
	pid_t pid;
	int err;
	
	if (!enabled) return 0;
	
	/* 曲数よりも多い指定は不可*/
	if (trk > lastindex) {
		return NG;
	}
	
	argv[argc] = playlist[trk -2];
	argv[argc +1] = NULL;
	pid = fork_local();
	if (pid == 0) {
		/* child process */
		execvp(mp3_player, argv);
		perror("execvp");
		_exit(-1);
	} else if (pid < 0) {
		return NG;
	}
	
	save->pid = pid;
	save->trk = trk;
	save->cnt = get_high_counter(SYSTEMCOUNTER_MP3);
	
	return OK;
}

/* 演奏停止 */
static int cdrom_stop() {
	/* killpg されているはずなので stop の処理は必要ない */
	return OK;
#if 0
	int status = 0;

	if (!enabled || save->trk == 0 || save->pid == 0) return OK;
	
	killpg_local(save->pid, SIGTERM);
	waitpid(save->pid, &status, WUNTRACED);
	if (WEXITSTATUS(status) != 0) { enabled = FALSE; }
	
	save->pid = 0;
	save->trk = 999;
	return OK;
#endif
}

/* 現在演奏中のトラック情報の取得 */
static int cdrom_getPlayingInfo (cd_time *inf) {
	int status, cnt, err;

	if (!enabled) {
		inf->t = inf->m = inf->s = inf->f = 999;
		return OK;
	}
	
	if (save->trk == 999 || save->pid == 0) {
		inf->t = inf->m = inf->s = inf->f = 999;
		return OK;
	}
	
	if (save->pid == (err = waitpid(save->pid, &status, 0))) {
		save->pid = 0;
		inf->t = inf->m = inf->s = inf->f = 999;
		return OK;
	}

	cnt = get_high_counter(SYSTEMCOUNTER_MP3) - save->cnt;
	inf->t = save->trk;
	inf->m = cnt / (60*100); cnt %= (60*100); 
	inf->s = cnt / 100;      cnt %= 100;
	inf->f = (cnt * 72) / 100;
	
	return OK;
}
#if 0
static void cdrom_setAudioDevice(audio_t *audio) {
	if (!audio) return;
	if (!audio->pcm) return;
	
	audio_cd = audio;
	pcm_cd = audio->pcm;
}
#endif
