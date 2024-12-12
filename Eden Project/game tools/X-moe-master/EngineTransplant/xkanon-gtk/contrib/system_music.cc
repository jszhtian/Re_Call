/*  system_music.cc
 *      AyuSys と music.h をつなぐ interface
 */

/*
 *
 *  Copyright (C) 2000-   Kazunori Ueno(JAGARL) <jagarl@creator.club.ne.jp>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
*/

#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <signal.h>
#include <map>
#include <errno.h>
#include <vector>
#include <list>
#include <algorithm>
#include"../system.h"
#include"../file.h"

extern "C" {
#include "music.h"
#include "mixer.h"
}

using namespace std;

// #define delete fprintf(stderr,"smus.cc: %d.",__LINE__), delete

void AyuSys::SetCDROMDevice(char* dev) {
	cd_set_devicename(dev);
}
void AyuSys::SetPCMDevice(char* dev) {
	pcm_setDeviceName(dev);
}
void AyuSys::SetMixDevice(char* dev) {
	mixer_setDeviceName(dev);
}
void AyuSys::SetPCMRate(int rate) {
	if (rate <= 0) return;
	mus_set_default_rate(rate);
}

void AyuSys::PlayCDROM(char* name) {
	char wave[128]; wave[127] = '\0'; wave[0] = '\0';

	is_cdrom_track_changed = 1;
	strcpy(cdrom_track, name);
	int play_count = 10000;
	if (GetCDROMMode() == MUSIC_ONCE) play_count = 1;

	if (GrpFastMode() == 3) return;

	StopCDROM();
	strcpy(cdrom_track, name);

	/* name -> track */
	int track =track_name.CDTrack(name);
	if (track_name.WaveTrack(name) != 0) strncpy(wave, track_name.WaveTrack(name), 127);
	if (track == -1) track = atoi(name);
	if (track != 0 && (! cdrom_enable)) { /* CDROM が使用不可能な場合、pcm を試みる */
		sprintf(wave, "audio_%02d",track);
		track = 0;
	}
	if (track == 0) { // play wave file
		if (wave == 0) return;
		// wave file の長さを制限
		char wave_tmp[128];
		int len = strlen(wave); if (len > 100) len = 100;
		strncpy(wave_tmp, wave, len); wave_tmp[len] = '\0';
		// BGM 再生
		SetUseBGM();
		if (!pcm_enable) return;
		mus_bgm_start(wave_tmp, play_count);
		// 始まるまで待つ
		int pos;
		void* timer = setTimerBase();
		const int wait_time = 2000; // 再生開始まで、最大２秒待つ
		while(mus_bgm_getStatus(&pos) == 0 && getTime(timer) < wait_time)  {
			CallIdleEvent();
			CallProcessMessages();
		}
		freeTimerBase(timer);
		/* 音量を最大にする */
		mus_mixer_fadeout_start(MIX_PCM_BGM, 0, 100, 0);
	} else { // play CDROM once
		if (! cdrom_enable) return;
		SetUseCDROM();
		/* CDROM 再生 */
		mus_setLoopCount(play_count);
		mus_cdrom_start(track);
		/* 再生開始まで待つ */
		cd_time tm;
		void* timer = setTimerBase();
		const int wait_time = 2000; // 再生開始まで、最大２秒待つ
		do {
			mus_cdrom_getPlayStatus(&tm);
			CallIdleEvent();
			CallProcessMessages();
		} while( tm.t != track && getTime(timer) < wait_time);
		/* 音量を最大にする */
		mus_mixer_fadeout_start(MIX_CD, 0, 100, 0);
	}
	return;
}

void AyuSys::StopCDROM(void)
{
	is_cdrom_track_changed = 1;
	cdrom_track[0] = '\0';
	if (GrpFastMode() == 3) return;
	if (IsUseBGM()) {
		if (! pcm_enable) return;
		mus_bgm_stop();
		mus_mixer_fadeout_start(MIX_PCM_BGM, 0, 0, 1);
	} else {
		if (! cdrom_enable) return;
		// CDROM の fadeout を止める
		mus_mixer_stop_fadeout(MIX_CD);
		while(mus_mixer_get_fadeout_state(MIX_CD) == 0) {
			CallProcessMessages();
		}
		mus_mixer_fadeout_start(MIX_CD, 0, 0, 1);
	}
}

void AyuSys::FadeCDROM(int time)
{
	is_cdrom_track_changed = 1;
	cdrom_track[0] = '\0';
	if (GrpFastMode() == 3) return;
	if (IsUseBGM()) {
		if (! pcm_enable) return;
		mus_mixer_fadeout_start(MIX_PCM_BGM, time, 0, 1);
	} else {
		if (! cdrom_enable) return;
		mus_mixer_fadeout_start(MIX_CD, time, 0, 1);
	}
}

void AyuSys::WaitStopCDROM(void) {
	int dev;
	if (GrpFastMode() == 3) return;
	if (IsUseBGM()) {
		if (! pcm_enable) return;
		dev = MIX_PCM_BGM;
	} else {
		if (! cdrom_enable) return;
		dev = MIX_CD;
	}
	if (mus_mixer_get_fadeout_state(dev)) return;
	void* timer = setTimerBase();
	const int wait_time = 4000; // 曲の終了まで、最大４秒待つ
	while(mus_mixer_get_fadeout_state(dev) == 0 && getTime(timer) < wait_time && (!IsIntterupted()))  {
		CallIdleEvent();
		CallProcessMessages();
	}
	freeTimerBase(timer);
}


void AyuSys::SetWaveMixer(int is_mix) {
	mus_pcm_set_mix(is_mix);
}

void AyuSys::PlayWave(char* fname) {
	if (! pcm_enable) return;

	is_cdrom_track_changed = 1;
	if (strlen(fname) > 128) {
		effec_track[0] = '\0';
	} else strcpy(effec_track, fname);
	if (GrpFastMode() == 3) return;
	/* 再生 */
	int count = 1;
	if (GetEffecMode() == MUSIC_CONT) count = 10000;
	mus_effec_start(fname,count);
	// 始まるまで待つ
	int pos;
	void* timer = setTimerBase();
	const int wait_time = 2000; // 再生開始まで、最大２秒待つ
	while(mus_effec_getStatus(&pos) == 0 && getTime(timer) < wait_time)  {
		CallIdleEvent();
		CallProcessMessages();
	}
	freeTimerBase(timer);
	/* 音量を最大にする */
	mus_mixer_fadeout_start(MIX_PCM_EFFEC, 0, 100, 0);
	return;
}

void AyuSys::StopWave(void) {
	is_cdrom_track_changed = 1;
	effec_track[0] = '\0';
	if (GrpFastMode() == 3) return;
	if (! pcm_enable) return;
	mus_effec_stop();
}

void AyuSys::WaitStopWave(void) {
	if (GrpFastMode() == 3) return;
	if (! pcm_enable) return;
	void* timer = setTimerBase();
	const int wait_time = 10000; // 曲の終了まで、最大10秒待つ
	int pos;
	while(mus_effec_getStatus(&pos) != 0 && getTime(timer) < wait_time && (!IsIntterupted()))  {
		CallIdleEvent();
		CallProcessMessages();
	}
	freeTimerBase(timer);
}

void AyuSys::PlaySE(int number) {
	if (! pcm_enable) return;
	if (GrpFastMode() == 3) return;
	const char* se_name = track_name.SETrack(number);
	if (se_name == 0) return;
	/* 音量最大で再生 */
	mus_effec_start(se_name, 1);
	mus_mixer_fadeout_start(MIX_PCM_EFFEC, 0, 100, 0);
	return;
}
void AyuSys::StopSE(void) {
	if (! pcm_enable) return;
	if (GrpFastMode() == 3) return;
	mus_effec_stop();
}
void AyuSys::WaitStopSE(void) {
	WaitStopWave();
}

void AyuSys::PlayKoe(const char* fname) {
	if (GrpFastMode() == 3) return;
	if (! pcm_enable) return;
	if (! koe_mode) return;
	/* 再生 */
	mus_koe_start(fname);
	// 始まるまで待つ
	int pos;
	void* timer = setTimerBase();
	const int wait_time = 500; // 再生開始まで、最大0.5秒待つ
	while(mus_koe_getStatus(&pos) == 0 && getTime(timer) < wait_time)  {
		CallIdleEvent();
		CallProcessMessages();
	}
	freeTimerBase(timer);
	mus_mixer_fadeout_start(MIX_PCM_KOE, 0, 100, 0);
	return;
}

void AyuSys::StopKoe(void) {
	if (GrpFastMode() == 3) return;
	if (! pcm_enable) return;
	mus_koe_stop();
}
bool AyuSys::IsStopKoe(void) {
	if (GrpFastMode() == 3) return true;
	if (! pcm_enable) return true;
	int pos;
	if (mus_koe_getStatus(&pos) != 0) return false;
	return true;
}

void AyuSys::PlayMovie(char* fname, int x1, int y1, int x2, int y2, int loop_count) {
	if (GrpFastMode() == 3) return;
	if (movie_id != -1) DeletePartWindow(movie_id);
	movie_id = MakePartWindow(x1, y1, x2-x1+1, y2-y1+1);
	if (movie_id == -1) return;
	/* 再生 */
	mus_movie_start(fname, movie_id, 0, 0, x2-x1, y2-y1, loop_count);
	// 始まるまで待つ
	int pos;
	void* timer = setTimerBase();
	const int wait_time = 2000; // 再生開始まで、最大２秒待つ
	while(mus_movie_getStatus(&pos) == 0 && getTime(timer) < wait_time)  {
		CallIdleEvent();
		CallProcessMessages();
	}
	if (mus_movie_getStatus(&pos) == 0) {
		/* 失敗した */
		mus_movie_stop();
		DeletePartWindow(movie_id);
		movie_id = -1;
	}
	freeTimerBase(timer);
	return;
}

void AyuSys::StopMovie(void) {
	mus_movie_stop();
}
void AyuSys::PauseMovie(void) {
	mus_movie_pause();
}
void AyuSys::ResumeMovie(void) {
	mus_movie_resume();
}

void AyuSys::WaitStopMovie(int is_click) {
	if (GrpFastMode() == 3) return;
	int pos;
	if (is_click) {
		SetMouseMode(0);
		ClearMouseInfo();
	}
	while(mus_movie_getStatus(&pos) != 0) {
		if (pos < 0) break;
		if (IsIntterupted()) break;
		if (is_click) {
			int x, y, flag;
			GetMouseInfo(x, y, flag);
			if (flag == 0 || flag == 2 || flag == 4) { // マウスが押されたら終了
				StopMovie();
			}
			ClearMouseInfo();
		}
		CallIdleEvent();
		CallProcessMessages();
		usleep(10000);
	}
	if (is_click) {
		ClearMouseInfo();
	}
}

void AyuSys::SyncMusicState(void) {
	// CDROM / 効果音の再生が終わっていれば
	// 該当する track を "\0" にする
	int dummy;
	if (GetCDROMMode() == MUSIC_ONCE) {
		if (IsUseBGM()) {
			if (mus_bgm_getStatus(&dummy) == 0) {
				is_cdrom_track_changed = 1;
				cdrom_track[0] = '\0';
			}
		} else {
			cd_time tm;
			mus_cdrom_getPlayStatus(&tm);
			if (tm.t == 999) { // error exit -> cdrom stop
				is_cdrom_track_changed = 1;
				cdrom_track[0] = '\0';
			}
		}
	}
        if (GetEffecMode() == MUSIC_ONCE) {
		if (mus_effec_getStatus(&dummy) == 0) {
			is_cdrom_track_changed = 1;
			effec_track[0] = '\0';
		}
	}
}

void AyuSys::DisableMusic(void) {
	if (music_enable == 2) FinalizeMusic();
	music_enable = 0;
}

void AyuSys::InitMusic(void)
{
	if (music_enable != 1) return;
	is_cdrom_track_changed = 1;
	cdrom_track[0] = '\0';
	if (config->GetParaInt("#MUSIC_LINEAR_PAC")) mus_set_8to16_usetable(1);
	mus_init();
	if (mus_mixer_get_default_level(MIX_CD) == 0)
		mus_mixer_set_default_level(MIX_CD, 63);
	if (mus_mixer_get_default_level(MIX_PCM) == 0)
		mus_mixer_set_default_level(MIX_PCM, 80);
	music_enable = 2;
}

void AyuSys::FinalizeMusic(void)
{
	if (music_enable == 2) {
		mus_exit(0);
		music_enable = 1;
	}
}

char* System_tmpDir = "/tmp";
void System_error(const char* msg) {
	fprintf(stderr, "Error  : %s\n",msg);
	fflush(stderr);
}
void System_errorOutOfMemory(const char* msg) {
	System_error(msg);
}

TrackName::TrackName(void) {
	deal = 1;
	track = new char*[deal];
	track_wave = new char*[deal];
	track_num = new int[deal];
	int i; for (i=0; i<deal; i++) track[i] = 0;
	for (i=0; i<deal; i++) track_wave[i] = 0;
	se_deal = 10;
	se_track = new char*[se_deal];
	for (i=0; i<deal; i++) se_track[i] = 0;
}

TrackName::~TrackName() {
	int i; for (i=0; i<deal; i++) {
		if (track[i] != 0) delete[] track[i];
		if (track_wave[i] != 0) delete[] track_wave[i];
	}
	for (i=0; i<se_deal; i++) {
		if (se_track[i]) delete[] se_track[i];
	}
	delete[] track;
	delete[] track_wave;
	delete[] track_num;
	delete[] se_track;
}
void TrackName::Expand(void) {
	int new_deal = deal * 2;
	int* new_track_num = new int[new_deal];
	char** new_track = new char*[new_deal];
	char** new_track_wave = new char*[new_deal];
	int i; for (i=0; i<deal; i++) {
		new_track_num[i] = track_num[i];
		new_track[i] = track[i];
		new_track_wave[i] = track_wave[i];
	}
	for (; i<new_deal; i++) { 
		new_track_num[i] = 0;
		new_track[i] = 0;
		new_track_wave[i] = 0;
	}
	deal = new_deal;
	delete[] track; track = new_track;
	delete[] track_num; track_num= new_track_num;
	delete[] track_wave; track_wave = new_track_wave;
}
void TrackName::ExpandSE(int n) {
	if (n < 0) return;
	n += 10;
	if (se_deal >= n) return;
	char** new_se = new char*[n];
	int i; for (i=0; i<se_deal; i++) new_se[i] = se_track[i];
	for (; i<n; i++) new_se[i] = 0;
	delete[] se_track;
	se_deal = n; se_track = new_se;
}
void TrackName::AddCDROM(char* name, int tk) {
	if (CDTrack(name) != -1) return;
	int i; for (i=0; i<deal; i++) {
		if (track[i] == 0) break;
	}
	int num = i;
	if (i == deal) Expand();
	track[num] = new char[strlen(name)+1]; strcpy(track[num], name);
	track_num[num] = tk;
}
void TrackName::AddWave(char* name, char* file) {
	if (CDTrack(name) != -1) return;
	int i; for (i=0; i<deal; i++) {
		if (track[i] == 0) break;
	}
	int num = i;
	if (i == deal) Expand();
	track_num[num] = 0;
	track[num] = new char[strlen(name)+1]; strcpy(track[num], name);
	track_wave[num] = new char[strlen(file)+1]; strcpy(track_wave[num], file);
}
int TrackName::CDTrack(char* name) {
	int i; for (i=0; i<deal; i++) {
		if (track[i] == 0) return -1;
		if (strcmp(track[i],  name) == 0) {
			return track_num[i];
		}
	}
	return -1;
}
const char* TrackName::WaveTrack(char* name) {
	int i; for (i=0; i<deal; i++) {
		if (track[i] == 0) return 0;
		if (strcmp(track[i],  name) == 0) {
			return track_wave[i];
		}
	}
	return 0;
}
const char* TrackName::SETrack(int n) {
	if (n < 0 || n >= se_deal) return 0;
	return se_track[n];
}
void TrackName::AddSE(int n, char* file) {
	if (se_deal <= n) ExpandSE(n);
	if (se_track[n]) delete[] se_track[n];
	se_track[n] = new char[strlen(file)+1];
	strcpy(se_track[n], file);
}

/* スペース区切りでコマンドライン引数を分割する */
/* pipe からの読み込み用 */
static char** SplitCmdArg(const char* cmd_orig, const char* cmdarg_orig, const char* lastarg_orig) {
	char** args_orig = new char*[strlen(cmdarg_orig)+3];
	char** args = args_orig;
	char* cmd = new char[strlen(cmd_orig)+1]; strcpy(cmd, cmd_orig);
	char* lastarg = new char[strlen(lastarg_orig)+1]; strcpy(lastarg, lastarg_orig);
	char* cmdarg = new char[strlen(cmdarg_orig)+1]; strcpy(cmdarg, cmdarg_orig);
	*args++ = cmd;
	while(1) {
		while(*cmdarg <= 0x20 && *cmdarg != 0) cmdarg++; /* スペース、タブ読み飛ばし */
		if (*cmdarg == 0) break;
		*args++ = cmdarg;
		while(*cmdarg > 0x20) cmdarg++; /* スペース、タブ以外読み飛ばし */
		if (*cmdarg == 0) break;
		*cmdarg++ = '\0';
	}
	*args++ = lastarg;
	*args = 0;
	return args_orig;
}

const char* fileext_orig[] = {"mp3", 0};
const char* player_orig[] = {MP3CMD, 0};
const char* cmdline_orig[] = {MP3ARG, 0};
extern "C" pid_t fork_local(void);
extern "C" FILE* OpenWaveFile(const char* path, int* size) {
	/* まず wav ファイルを探す */
	ARCINFO* info = file_searcher.Find(FILESEARCH::WAV,path,".wav");
	if (info == 0) info = file_searcher.Find(FILESEARCH::BGM,path,"wav");
	if (info) {
		FILE* f = info->OpenFile(size);
		delete info;
		return f;
	}
	/* なければ mp3 などを探す */
	const char** fileext = fileext_orig;
	const char** player = player_orig;
	const char** cmdline = cmdline_orig;
	if (size) *size = -1;
	while( *fileext != 0) {
		info = file_searcher.Find(FILESEARCH::WAV, path, *fileext);
		if (info == 0) info = file_searcher.Find(FILESEARCH::BGM, path, *fileext);
		if (info != 0 && **player != '\0') break;
		fileext++; player++; cmdline++;
	}
	if (info == 0) return 0;
	/* パイプを開く */
	int pipes[2] = {-1,-1};
	int child_id = 0;
	if (pipe(pipes) != -1 && (child_id=fork_local()) == 0) {
		/* 子プロセス */
		/* パイプを stdout に割り当て、exec する */
		close(pipes[0]);
		close(1);
		dup(pipes[1]);
		close(pipes[1]);
		int null_fd = open("/dev/null", O_WRONLY);
		if (null_fd != -1) {
			close(2);
			dup(null_fd);
			close(null_fd);
		}
		/* コマンドライン引数の解析 */
		char** args = SplitCmdArg(*player, *cmdline, info->Path());
		execv(*player, args);
		sleep(1000); /* エラーがあればプレイ不可能なのでなにもしない */
		exit(-1);
	}
	if (child_id == -1) { /* エラー */
		/* エラーが起きた */
		if (pipes[0] != -1) { close(pipes[0]); close(pipes[1]); }
		delete info;
		return 0;
	} else {
		close(pipes[1]);
		FILE* f = fdopen(pipes[0],"r");
		delete info;
		return f;
	}
}

extern "C" const char* FindMovieFile(const char* path) {
	ARCINFO* info = file_searcher.Find(FILESEARCH::MOV,path,"avi");
	if (info == 0) 
		info = file_searcher.Find(FILESEARCH::MOV,path,"mpg");
	if (info == 0) return 0;
	const char* file = info->Path();
	delete info;
	return file;
}

void AyuSys::ReceiveMusicPacket(void) {
	SRVMSG   msg;

	memset(&msg, 0, sizeof(msg));
	RecvMsgServerToClient(&msg, 0);
#if FreeBSD_PTHREAD_ERROR == 1
	raise(SIGPROF);
#endif /* PTHREAD_ERROR */
	switch(msg.msg_type) {
	case NoProcess:
		break;
	case MUS_MOV_INFORM_END:
		DeletePartWindow(movie_id);
		movie_id = -1;
		break;
	default:
		printf("unknown msg get from server \n");
	}
}

/* 声ファイルのアーカイブ用のキャッシュ */
#define koe_cache_size 7
struct AvgKoeTable {
	int koe_num;
	int length;
	int offset;
	AvgKoeTable(char* buf, int original_offset) {
		koe_num = read_little_endian_short(buf);
		length = read_little_endian_short(buf+2);
		offset = original_offset + read_little_endian_int(buf+4);
	}
	bool operator <(int number) const {
		return koe_num < number;
	}
	bool operator <(const AvgKoeTable& to) const {
		return koe_num < to.koe_num;
	}
	bool operator ==(const AvgKoeTable& to) const {
		return koe_num == to.koe_num;
	}
	bool operator ==(const int to) const {
		return koe_num == to;
	}
};
struct AvgKoeHead {
	FILE* stream;
	int file_number;
	int rate;
	vector<AvgKoeTable> table;
	AvgKoeHead(FILE* stream, int file_number);
	AvgKoeHead(const AvgKoeHead& from);
	~AvgKoeHead();
	AvgKoeTable* Find(int koe_num);
	bool operator !=(int num) const { return file_number != num; }
	bool operator ==(int num) const { return file_number == num; }
};
struct AvgKoeCache {
	list<AvgKoeHead> cache;
	AvgKoeInfo Find(int file_number, int index);
};
static AvgKoeCache koe_cache;

AvgKoeInfo AvgKoeCache::Find(int file_number, int index) {
	AvgKoeInfo info;
	info.stream = 0; info.length = 0; info.offset = 0;

	list<AvgKoeHead>::iterator it;
	it = find(cache.begin(), cache.end(), file_number);
	if (it == cache.end()) {
		/* 新たに head を作る */
		char fname[100];
		sprintf(fname, "z%03d.koe", file_number);
		ARCINFO* arcinfo = file_searcher.Find(FILESEARCH::KOE,fname,".koe");
		if (arcinfo == 0) return info;
		FILE* stream = arcinfo->OpenFile();
		delete arcinfo;
		if (stream == 0) return info;
		cache.push_front(AvgKoeHead(stream, file_number));
		if (cache.size() >= koe_cache_size) cache.pop_back();
		it = cache.begin();
	}
	if (it->file_number != file_number) return info; // 番号がおかしい
	AvgKoeTable* table = it->Find(index);
	if (table == 0) return info; // index が見付からない
	// info を作成する
	info.length = table->length;
	info.offset = table->offset;
	info.rate = it->rate;
	int new_fd = dup(fileno(it->stream));
	if (new_fd == -1) info.stream = 0;
	else info.stream = fdopen(new_fd, "rb");
	return info;
}

AvgKoeHead::AvgKoeHead(const AvgKoeHead& from) {
	if (from.stream) {
		int new_fd = dup(fileno(from.stream));
		if (new_fd == -1) stream = 0;
		else stream = fdopen(new_fd, "rb");
	}
	file_number = from.file_number;
	rate = from.rate;
	table = from.table;
}
AvgKoeHead::AvgKoeHead(FILE* _s, int _file_number) {
	char head[0x20];
	stream = _s; file_number = _file_number;
	int offset = ftell(stream);
	rate = 22050;
	if (stream == 0) return;
	/* header 読み込み */
	fread(head, 0x20, 1, stream);
	if (strncmp(head, "KOEPAC", 7) != 0) { // invalid header
		stream = 0;
		return;
	}
	int table_len = read_little_endian_int(head+0x10);
	rate = read_little_endian_int(head+0x18);
	if (rate == 0) rate = 22050;
	/* table 読み込み */
	table.reserve(table_len);
	char* buf = new char[table_len*8];
	fread(buf, table_len, 8, stream);
	int i; for (i=0; i<table_len; i++) {
		table.push_back(AvgKoeTable(buf+i*8, offset));
	}
	sort(table.begin(), table.end());
}
AvgKoeHead::~AvgKoeHead(void) {
	if (stream) fclose(stream);
	stream = 0;
}
AvgKoeTable* AvgKoeHead::Find(int koe_num) {
	if (table.empty()) return 0;
	vector<AvgKoeTable>::iterator it;
	it = lower_bound(table.begin(), table.end(), koe_num);
	if (it == table.end() || it->koe_num != koe_num) return 0;
	return &table[it-table.begin()];
}

extern "C" AvgKoeInfo OpenKoeFile(const char* path) {
	int radix = 10000;
	if (global_system.Version() >= 2) radix *= 10;
	AvgKoeInfo info;
	info.stream = 0; info.length = 0; info.offset = 0;
	if (isdigit(path[0])) { // 数値
		/* avg32 形式の音声アーカイブのキャッシュを検索 */
		int pointer = atoi(path);
		int file_no = pointer / radix;
		int index = pointer % radix;
		info = koe_cache.Find(file_no, index);
	} else { // ファイル
		int length;
		ARCINFO* arcinfo = file_searcher.Find(FILESEARCH::KOE,path,".WPD");
		if (arcinfo == 0) return info;
		info.stream = arcinfo->OpenFile(&length);
		info.rate = 22050;
		info.length = length;
		info.offset = ftell(info.stream);
		delete arcinfo;
	}
	return info;
}

