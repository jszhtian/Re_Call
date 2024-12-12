/*
 * movie.c  movie を再生する
 *
 * Copyright (C)   2001-     Kazunori Ueno(JAGARL) <jagarl@creator.club.ne.jp>
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

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif

#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "portab.h"
#include "music.h"

extern const char* FindMovieFile(const char* path);
extern void stop_music(void);

static char inputconfig_file[1024]="";
static void signal_handler(int sig) {
	if (inputconfig_file[0])
		unlink(inputconfig_file);
	exit(0);
}
static char input_conf[] =
"RIGHT seek +10\n"
"MOUSE_BTN3 +10\n"
"LEFT  seek -10\n"
"MOUSE_BTN4 +10\n"
"\n"
"ENTER quit\n"
"SPACE quit\n"
"q quit\n"
"MOUSE_BTN0 quit\n"
"MOUSE_BTN1 quit\n"
"MOUSE_BTN2 quit\n";

static void make_input_config(void) {
	int fd;
	strcpy(inputconfig_file, "/tmp/xkanon-mplayer-input-XXXXXXX");
#if HAVE_MKSTEMP
	fd = mkstemp(inputconfig_file);
#else
	mktemp(inputconfig_file);
	fd = open(inputconfig_, O_RDWR | O_CREAT |O_TRUNC, 0600);
#endif
	if (fd == -1) return;
	write(fd, input_conf, strlen(input_conf));
}

#define WITH_VIDEO 2
#define WITH_AUDIO 1
/* xanim で再生可能かのチェック */
static int check_xanim(const char* path) {
	pid_t child;
	int i;
	int pipes[2];
	int retval = 0;
	FILE* in;

	/* xanim が存在しなければどうしようもない */
	if (AVIPLAY[0] == '\0') return 0;
	/* check 用の pipe を作る */
	if (pipe(pipes) == -1) return 0;
	child = forkpg_local();
	if (child == -1) return 0;
	/* audio track の有無を調べる */
	/* +v +Zv オプションで Video Codec という文字がなければ不正、
	** Audio Codec という文字があれば audio track あり
	*/
	if (child == 0) {
		close(pipes[0]);
		close(1);
		dup(pipes[1]);
		close(2);
		dup(pipes[1]);
		close(pipes[1]);
		execl(AVIPLAY, AVIPLAY, "+v", "+Zv", path, 0);
		exit(0);
	}
	close(pipes[1]);
	in = fdopen(pipes[0], "r");

	/* 最大１００行チェック */
	for (i=0; i<100 && (!feof(in)); i++) {
		int i;
		char buf[1024];
		fgets(buf, 1000, in);
		/* 頭の空白を削る */
		for (i=0; buf[i] == ' ' || buf[i] == '\t'; i++) ;
		/* 調べる */
		if (strncmp(buf+i, "Video Codec:", 12) == 0) retval |= WITH_VIDEO;
		if (strncmp(buf+i, "Audio Codec:", 12) == 0) retval |= WITH_AUDIO;
		if (strncmp(buf+i, "AVI Notice: No supported Video frames found", 43) == 0) retval = 0;
	}
	fclose(in); /* 子プロセスがゾンビになるけどとりあえず気にしない */
	return retval;
}
static pid_t play_xanim(const char* path, int window_id, int x1, int y1, int x2, int y2, int loop_count) {
	/* 再生開始 */
	int status;
	pid_t child = forkpg_local();
	if (child == -1) return 0;
	if (child == 0) {
		/* 再生プロセス：終了したら stop を呼び出す */
		pid_t movie_process;
		signal(SIGCHLD, SIG_DFL);
		movie_process = fork_local();
		if (movie_process == -1) exit(0);
		if (movie_process == 0) {
			char opt1[10], opt2[10], opt3[10], opt4[10], opt5[10], opt6[10];
			sprintf(opt1, "+W%d",window_id);
			sprintf(opt2, "+Wx%d", x1);
			sprintf(opt3, "+Wy%d", y1);
			sprintf(opt4, "+l%d", loop_count);
			sprintf(opt5, "+Sx%d", x2-x1+1);
			sprintf(opt6, "+Sy%d", y2-y1+1);
			execl(AVIPLAY, AVIPLAY, "+q", "+Ze", "-Zr", opt1, opt2, opt3, opt4, opt5, opt6, path , 0);
			exit(0);
		}
		waitpid(movie_process, &status, 0);
		mus_movie_stop();
		exit(0);
	}
	return child;
}
/* mplayer で再生可能かのチェック */
static int check_mplayer(const char* path) {
	pid_t child;
	int i;
	int pipes[2];
	int retval = 0;
	FILE* in;

	/* mplayer が存在しなければどうしようもない */
	if (MPLAYER[0] == '\0') return 0;
	/* check 用の pipe を作る */
	if (pipe(pipes) == -1) return 0;
	child = forkpg_local();
	if (child == -1) return 0;
	/* 再生可能かを調べる
	** VIDEO: , AUDIO: が表示され、start play すれば成功
	*/
	if (child == 0) {
		close(pipes[0]);
		close(1);
		dup(pipes[1]);
		close(2);
		dup(pipes[1]);
		close(pipes[1]);
		/* 必要なオプションを（ダミー引数で）すべてつけて実行 */
		signal(SIGTTOU,SIG_IGN);
		execl(MPLAYER, MPLAYER, "-vo", "null", "-ao", "null",
			"-x", "640", "-y", "480", "-zoom",
			"-wid", "0", "-framedrop", "-loop", "1", path, 0);
		exit(0);
	}
	close(pipes[1]);
	in = fdopen(pipes[0], "r");

	/* 最大１００行チェック */
	for (i=0; i<100 && (!feof(in)); i++) {
		int j;
		char buf[128];
		fgets(buf, 100, in);
		/* 頭の空白を削る */
		for (j=0; buf[j] == ' ' || buf[j] == '\t'; j++) ;
		/* 調べる */
		if (strncmp(buf+j, "VIDEO: ", 7) == 0) retval |= WITH_VIDEO;
		if (strncmp(buf+j, "AUDIO: ", 7) == 0) retval |= WITH_AUDIO;
		if (strncmp(buf+j, "Start playing",13) == 0) break;
		if (strncmp(buf+j, "Starting playback",17) == 0) break;
	}
	fclose(in); /* 子プロセスがゾンビになるけどとりあえず気にしない */
	return retval;
}
static pid_t play_mplayer(const char* path, int window_id, int x1, int y1, int x2, int y2, int loop_count) {
	/* 再生開始 */
	int status;
	pid_t child;
	int pipes[2];
	if (pipe(pipes) == -1) return 0;
	child = forkpg_local();
	if (child == -1) return 0;
	if (child == 0) {
		/* 再生プロセス：終了したら stop を呼び出す */
		pid_t movie_process;
		signal(SIGCHLD, SIG_DFL);

		/* input file を作成 */
		make_input_config();

		movie_process = fork_local();
		if (movie_process == -1) exit(0);
		if (movie_process == 0) {
			int fd;
			char opt1[10], opt2[10], opt3[10], opt4[10], opt5[50];
			/* 書き込み端を閉じる */
			close(pipes[1]);
			/* stdin を pipe に繋ぐ */
			close(0);
			dup(pipes[0]);
			close(pipes[0]);
			/* /tmp/mlog にエラー出力 */
			/* fd = open("/tmp/mlog",O_RDWR|O_CREAT,0755); */
			fd = open("/dev/null",O_RDWR,0755);
			if (fd != -1) {
				close(1);
				dup(fd);
				close(2);
				dup(fd);
				close(fd);
			} else {
				perror("open : ");
			}
			sprintf(opt1, "%d",window_id);
			sprintf(opt2, "%d", x2-x1+1);
			sprintf(opt3, "%d", y2-y1+1);
			sprintf(opt4, "%d", loop_count);
			sprintf(opt5, "conf=%s", inputconfig_file);
			execl(MPLAYER, MPLAYER, "-vo", "x11",
				"-x", opt2, "-y", opt3, "-zoom",
				"-input", opt5,
				"-wid", opt1, "-framedrop", "-loop", opt4, path, 0);
			exit(0);
		}
		signal(SIGTERM, signal_handler);
		waitpid(movie_process, &status, 0);
		mus_movie_stop();
		exit(0);
	}
	close(pipes[0]);
	close(pipes[1]); // 書き込み側
	return child;
}

pid_t movie_start(const char* filename, int window_id, int x1, int y1, int x2, int y2, int loop_count) {
	int xanim_result, mplayer_result;
	const char* path;

	path = FindMovieFile(filename);
	if (path == 0) return 0;

	xanim_result = check_xanim(path);
	if (xanim_result & WITH_VIDEO) {
		if (xanim_result & WITH_AUDIO)  {
			/* audio track が存在するなら全ての音楽を終了する */
			stop_music();
		}
		return play_xanim(path, window_id, x1, y1, x2, y2, loop_count);
	}
	mplayer_result = check_mplayer(path);
	if (mplayer_result & WITH_VIDEO) {
		if (mplayer_result & WITH_AUDIO)  {
			/* audio track が存在するなら全ての音楽を終了する */
			stop_music();
		}
		return play_mplayer(path, window_id, x1, y1, x2, y2, loop_count);
	}
	return 0;
}

