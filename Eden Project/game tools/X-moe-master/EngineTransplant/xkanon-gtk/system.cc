/*  system.cc
 *      他に入らなかった AyuSys クラスのメソッド
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

#include "system.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

#include "file.h"


//#define NO_GLIB
//#define SUPRESS_WAIT

// システム周り
// タイマーなど

AyuSys global_system;

#ifndef NO_GLIB /* glib をつかう */
#include<glib.h>
#else /* NO_GLIB */
#include <sys/time.h>

#if HAVE_GETTIMEOFDAY != 1
#error	This program requires either glib or gettimeofday(). Please use glib.
#endif

struct GTimer {
	struct timeval tm;
};
GTimer* g_timer_new(void) {
	GTimer* ret = new GTimer;
	gettimeofday(&(ret->tm), 0);
	return ret;
}

void g_timer_destroy(GTimer* g) {
	delete g;
}

void g_timer_reset(GTimer* g) {
	gettimeofday( &(g->tm), 0);
}

double g_timer_elapsed(GTimer* last, long* utm) {
	struct timeval tm; gettimeofday(&tm,0);
	tm.tv_usec -= last->tm.tv_usec;
	tm.tv_sec -= last->tm.tv_sec;
	while(tm.tv_usec < last->tm.tv_usec) {
		tm.tv_usec += 1000000;
		tm.tv_sec--;
	}
	double ret = double(tm.tv_usec) / 1000000.0 + double(tm.tv_sec);
	if (utm != 0) *utm = tm.tv_usec;
	return 0;
}
#endif /* defined(NO_GLIB) */

void AyuSys::ExpandTimer(void) {
	// タイマーの数を増やす。１回に３つ
	int new_timer_deal = timer_deal + 3;
	void** new_timers = (void**)(new GTimer*[new_timer_deal]);
	int* new_timer_used = new int[new_timer_deal];
	int i;
	// コピーと初期化
	for (i=0; i<timer_deal; i++) {
		new_timers[i] = timers[i];
		new_timer_used[i] = timer_used[i];
	}
	for (; i<new_timer_deal; i++) {
		new_timers[i] = (void*)g_timer_new();
		new_timer_used[i] = 0;
	}
	// 古いメモリの解放
	if (timers != 0) delete[] timers;
	if (timer_used != 0) delete[] timer_used;
	// コピー
	timer_deal = new_timer_deal;
	timers = new_timers;
	timer_used = new_timer_used;
	return;
}

void* AyuSys::UnusedTimer(void) { // 使われてないタイマーを帰す
	while(1) {// 無限ループ
		int i; for (i=0; i<timer_deal; i++) {
			if (!timer_used[i]) {
				timer_used[i] = 1;
				return timers[i];
			}
		}
		ExpandTimer();
	}
}

void AyuSys::DestroyAllTimer(void) { // すべてのタイマーを消す
	int i; for (i=0; i<timer_deal; i++) {
		g_timer_destroy((GTimer*)timers[i]);
	}
	delete[] timers; delete[] timer_used;
	timers = 0; timer_used = 0; timer_deal = 0;
}

void* AyuSys::setTimerBase(void) {
	void* tm = UnusedTimer();
	g_timer_reset( (GTimer*)tm);
	return tm;
}

void AyuSys::waitUntil(void* tm_void, int wait_usec) {
	GTimer* tm = (GTimer*) tm_void;
	double wait_d = double(wait_usec) / 1000.0;
	if (GrpFastMode() == 1) wait_d /= 4;
	else if (GrpFastMode() == 2 || GrpFastMode() == 3) wait_d /= 100;
	double dt = g_timer_elapsed(tm, 0);
#ifndef SUPRESS_WAIT
	while(dt < wait_d) {
		// process messages
		CallProcessMessages();
		dt = g_timer_elapsed(tm, 0);
		if (IsIntterupted()) break;
	}
#endif
	return;
}

int AyuSys::getTime(void* tm_void) {
	GTimer* tm = (GTimer*) tm_void;
	double dt = g_timer_elapsed(tm, 0);
	dt *= 1000;
	if (IsIntterupted()) dt += 3600*1000;
	if (GrpFastMode() == 1) dt *= 4;
	else if (GrpFastMode() == 2 || GrpFastMode() == 3) dt *= 100;
	return int(dt);
}

void AyuSys::freeTimerBase(void* tm_void) {
	int i; for (i=0; i<timer_deal; i++) {
		if (timers[i] == tm_void) {
			timer_used[i] = 0;
		}
	}
}

void AyuSys::InitTimer(void) {
	// timerの初期化
	timer_deal = 0; timers = 0; timer_used = 0;
	ExpandTimer();
}

void AyuSys::FinalizeTimer(void) {
	DestroyAllTimer();
}

#ifdef HAVE_LRAND48
#  define RAND lrand48
#  define SRAND srand48
#elif defined(HAVE_RANDOM)
#  define RAND random
#  define SRAND srandom
#else /* rand() is bad... */
#  define RAND rand
#  define SRAND srand
#endif

// 乱数生成
int AyuSys::Rand(int max) {
	if (max == 0) return 0;
	if (max < 0) {
		int rnd = RAND(); rnd >>= 10; rnd%=-max;
		return -rnd;
	} else {
		int rnd = RAND(); rnd >>= 10;
		return rnd%max;
	}
}

void AyuSys::InitRand(void) {
	// initialize randomness
	long seed;
//	FILE* f = fopen("/dev/random","rb");
	FILE* f = fopen("/dev/urandom","rb");
	if (f == 0) return;
	fread(&seed,sizeof(long),1,f);
	fclose(f);
	SRAND(seed);
}

void AyuSys::CallIdleEvent(void)  {
	if (idle_event == 0) return;
	if (idle_event->Process() == 0)
		DeleteIdleEvent(idle_event);
	return;
}

void AyuSys::SetIdleEvent(IdleEvent* ev) {
	if (idle_event == 0) {
		idle_event = ev;
	} else {
		IdleEvent* cur = idle_event;
		while(cur->Next() != 0) cur = cur->Next();
		cur->SetNext(ev);
	}
	ev->SetNext(0);
}

void AyuSys::DeleteIdleEvent(IdleEvent* ev) {
	if (idle_event == 0) return;
	if (idle_event == ev) {
		idle_event = 0;
		return;
	}
	
	IdleEvent* cur = idle_event;
	while(cur->Next() != 0) {
		if (cur->Next() == ev) break;
		cur = cur->Next();
	}
	if (cur->Next() == ev)
		cur->SetNext(ev->Next());
	ev->SetNext(0);
	return;
}

IdleEvent::~IdleEvent() {
	local_system.DeleteIdleEvent(this);
}

// 音楽関係でセーブするところ
int AyuSys::MusicStatusLen(void) {
	int len = strlen(cdrom_track)+1 + strlen(effec_track)+1 + 2;
	if (len > 255) return 0;
	return len;
}
char* AyuSys::MusicStatusStore(char* buf, int buf_len) {
	if (buf_len < 1) return 0;
	int len = strlen(cdrom_track)+1 + strlen(effec_track)+1 + 2;
	if (len > 255 || buf_len < len) {
		*buf = 1; return buf+1;
	}
	*(unsigned char*)buf = len; buf++;
	*(unsigned char*)buf = music_mode; buf++;
	strcpy(buf, cdrom_track); buf += strlen(cdrom_track)+1;
	strcpy(buf, effec_track); buf += strlen(effec_track)+1;
	return buf;
}
char* AyuSys::MusicStatusRestore(char* buf) {
	char tmpbuf[128];
	int len = *(unsigned char*)buf;
	if (len < 0 || len > 255) { return buf+1;}
	buf++;
	music_mode = *(unsigned char*)buf; buf++;
	strcpy(tmpbuf, buf); buf += strlen(tmpbuf)+1;
	if (GetCDROMMode() == MUSIC_CONT) PlayCDROM(tmpbuf);
	strcpy(tmpbuf, buf); buf += strlen(tmpbuf)+1;
	if (GetEffecMode() == MUSIC_CONT) PlayWave(tmpbuf);
	return buf;
}

void AyuSys::SetDefaultScreenSize(int w, int h) {
	if (w < 80 || w > 5000) return;
	if (h < 80 || h > 5000) return;
	if (main_window) return;
	scn_w = w; scn_h = h;
}

AyuSys::AyuSys(void) : call_stack(0x100) {
	stop_flag = 0; koe_mode = 1;
	mouse_pos_x = 0; mouse_pos_y = 32;
	music_enable = 1;  version = -1;
	cdrom_track[0] = 0; is_cdrom_track_changed = 0;
	movie_id = -1;
	effec_track[0] = 0; music_mode = 0;
	text_fast_mode = 0; debug_flag = 0;
	pdt_bypp = 4;
	scn_w = 640; scn_h = 480;
	pdt_buffer = orig_pdt_buffer+1;
	pdt_buffer_orig = orig_pdt_buffer_orig+1;
	pdt_image = orig_pdt_image+1;
	int i; for (i=-1; i<PDT_BUFFER_DEAL; i++) {
		pdt_buffer[i] = 0;
		pdt_buffer_orig[i] = 0;
		pdt_image[i] = 0;
	}
	anm_pdt = 0;
	for (i=0; i<SEL_DEAL; i++) sels[i] = 0;
	for (i=0; i<SHAKE_DEAL; i++) shake[i] = 0;
	main_window = 0; main_senario = 0;
	visual = 0;
	pdt_reader = 0;
	goto_senario = -1;

	is_pressctrl = false; click_event_type = NO_EVENT;
	now_in_kidoku = false; is_allskip = false;
	text_skip_count = -1; is_text_auto = false; is_text_fast = false;
	is_text_dump = false; grp_fast_mode = GF_Normal; is_restoring_grp = false;
	idle_event = 0;

	title = new char[1]; title[0]=0;

	ClearIntterupt();
	InitTimer();
	InitRand();
	InitConfig();
}


AyuSys::~AyuSys() {
}
void AyuSys::Finalize(void) {
	FinalizeTimer();
	FinalizeWindow();
	FinalizeMusic();
}

void kconv(const unsigned char* src, unsigned char* dest) {
	/* input : sjis output: euc */
	while(*src) {
		unsigned int high = *src++;
		if (high < 0x80) {
			/* ASCII */
			*dest++ = high; continue;
		} else if (high < 0xa0) {
			/* SJIS */
			high -= 0x71;
		} else if (high < 0xe0) {
			/* hankaku KANA */
			*dest++ = 0x8e; *dest++ = high;
			continue;
		} else { /* high >= 0xe0 : SJIS */
			high -= 0xb1;
		}
		/* SJIS convert */
		high = (high<<1) + 1;

		unsigned int low = *src++;
		if (low == 0) break; /* incorrect code */
		if (low > 0x7f) low--;
		if (low >= 0x9e) {
			low -= 0x7d;
			high++;
		} else {
			low -= 0x1f;
		}
		*dest++ = high | 0x80; *dest++ = low | 0x80;
	}
	*dest = 0;
}

void kconv_rev(const unsigned char* src, unsigned char* dest) {
	/* input : euc output: sjis */
	while(*src) {
		unsigned int high = *src++;
		if (high < 0x80) {
			/* ASCII */
			*dest++ = high; continue;
		} else if (high == 0x8e) { /* hankaku KANA */
			high = *src;
			if (high >= 0xa0 && high < 0xe0)
				*dest++ = *src++;
			continue;
		} else {
			unsigned int low = *src++;
			if (low == 0) break; /* incorrect code , EOS */
			if (low < 0x80) continue; /* incorrect code */
			/* convert */
			low &= 0x7f; high &= 0x7f;
			low += (high & 1) ? 0x1f : 0x7d;
			high = (high-0x21)>>1;
			high += (high > 0x1e) ? 0xc1 : 0x81;
			*dest++ = high;
			if (low > 0x7f) low++;
			*dest++ = low;
		}
	}
	*dest = 0;
}

/* system2.o 用 */
#ifdef MAKE_LOADInit
void AyuSys::LoadInitFile(void)
{
}
#endif

/* configure で関数が見つからないとき */
#if HAVE_MKDIR == 0
extern "C" int mkdir(const char * name, unsigned short mode) {
	fprintf(stderr,"mkdir() is not implemented in this system ; please make directory '%s' with mode %04o manually\n", name, mode);
	errno = EPERM;
	return -1;
}
#endif
#if HAVE_SNPRINTF == 0
extern "C" int snprintf(char *str, size_t size, const char *format, ...) {
	va_list ap;
	va_start(ap, format);
	int ret_size = vsprintf(str, format, ap);
	if (strlen(str) >= size) {
		fprintf(stderr, "buffer overrun occurred in snprintf() ; the memory must be corrupted!\n");
	}
	return ret_size;
}
#endif

/* text skip state の変化 */
void AyuSys::StartTextSkipMode(int count) {
	if (count < 0) {
		text_skip_count_end = 500;
		if (count == -1) {
			text_skip_type = SKIPSELECT;
		} else if (count == -2) {
			text_skip_type = SKIPSCENE;
		}
	} else {
		text_skip_count_end = count;
		text_skip_type = SKIPCOUNT;
	}
	text_skip_count = 0;
	ChangeMenuTextFast();
}
void AyuSys::InclTextSkipCount(void) {
	if (text_skip_count < 0) return;
	text_skip_count++;
	if (text_skip_count >= text_skip_count_end) {
		// DeleteText();
		text_skip_count = -1;
		RestoreGrp();
		return;
	}
	if (text_skip_count == 1) { // はじめ
		// DeleteText();
	} else if (text_skip_count == 500 && text_skip_type != SKIPCOUNT) {
		// DeleteText();
		text_skip_count = 0;
	}
	if ( (text_skip_count%10) == 0) {
		// DrawText(".");
		// DrawTextEnd(1);
	}
	return;
}
void AyuSys::TitleEvent(void) {
	if (text_skip_count >= 0 && text_skip_type == SKIPSCENE) text_skip_count_end = 0;
	InclTextSkipCount();
}
void AyuSys::SelectEvent(void) {
	if (text_skip_count >= 0 && text_skip_type == SKIPSELECT) text_skip_count_end = 0;
	InclTextSkipCount();
}
void AyuSys::ClickEvent(void) {
	if (click_event_type & END_TEXTFAST) {
		if (text_skip_count >= 0) {
			StopTextSkip();
			if (NowInKidoku()) {
				SetTextFastMode(false);
			}
		} else if (NowInKidoku()) {
			if (is_text_fast) SetTextFastMode(false);
			else if (is_text_auto) SetTextAutoMode(false);
		}
		RestoreGrp();
	}
}
AyuSys::TextFastState AyuSys::TextFastMode(void) {
	/* SKIP? */
	if (text_skip_count >= 0) return TF_SKIP;
	/* FAST? */
	if (is_pressctrl) return TF_FAST;
	if ( (NowInKidoku()) && is_text_fast) return TF_FAST;
	/* AUTO? */
	if ( (NowInKidoku()) && is_text_auto) return TF_AUTO;
	return TF_NORMAL;
}
void AyuSys::SetBacklog(int count) {
	SetClickEvent(END_TEXTFAST);
	StopTextSkip();
	SetTextFastMode(false);
	Intterupt();
	backlog_count = count;
}
