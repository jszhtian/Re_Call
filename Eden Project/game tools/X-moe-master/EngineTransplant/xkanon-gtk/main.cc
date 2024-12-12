/* main.cc
 *    メイン・ルーチン
 *    AyuWindow の作成と、senario のデコードループをつくる
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

// #define MPROF /* memory profiling 制御用： libc_m.a をリンクするときに有効にする */


#include <X11/Xlib.h>
#include <gtk/gtk.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>
#if defined(sun) && (defined(sparc) || defined(__sparc))
#  include <pthread.h>
#endif
#include "initial.h"
#include "window.h"
#include "senario.h"

extern "C" void cdrom_setDeviceName(char *);
extern "C" void mus_exit(int is_abort);

static int first_senario = 0;
static int first_point = 0;

#ifdef DEFAULT_SAVEPATH
	char* savepath = DEFAULT_SAVEPATH;
#else
	char* savepath = "~/.kanon";
#endif

static int is_quit = 0;
// #include<pthread.h>
void QuitAll(void)
{
	global_system.Finalize();
	// gtk_main_quit();
}
// シグナルを無視する
static void null_signal_handler(int sig_num) {
	is_quit = 1;
}
static void alarm_signal_handler(int sig_num) {
//	fprintf(stderr,"alarm : pid %d ; pgid %d\n",getpid(),getpgrp(0));
}

// システムを終了する
static void signal_handler(int sig_num) {
	if (is_quit) {
		if (global_system.MainSenario()) global_system.MainSenario()->WriteSaveHeader();
		mus_exit(0);
		_exit(0);
	}
	is_quit = 1;
	global_system.Intterupt();
}

static void signal_handler_segv(int sig_num) {
	fprintf(stderr, "Segmentation Fault.");
	if (is_quit) abort(); // 強制終了
	is_quit = 1;
	if (global_system.MainSenario())
		global_system.MainSenario()->WriteSaveHeader();
	mus_exit(1); /* abort music */
	_exit(0);
}

// うぐぅ・・・
static SENARIO* senario_f;
void SaveFile(int n) {
	if (senario_f == 0) return;
	if (n < 0) return;
	senario_f->WriteSaveFile(n,
		global_system.GetTitle());
	char** list = senario_f->ReadSaveTitle();
	global_system.UpdateMenu(list);
}

static int idle_isProcess = 0;
extern "C" gint idle_handler(gpointer arg) { // Gtk handler
	
	// window が初期化されてなければなにもしない
	if (global_system.IsIntterupted() == 2) { gtk_main_quit(); return FALSE; }
	if (global_system.main_window == 0) return FALSE;
	if (! global_system.main_window->IsInitialized() ) return FALSE;
	if (! idle_isProcess) {
		// シナリオデコード開始前にシグナルを受け取っていれば、終了
		if (is_quit) {
			QuitAll();
			gtk_main_quit();
			return FALSE;
		}
		
		idle_isProcess = 1;
		SENARIO* s = new SENARIO(savepath, global_system);
		senario_f = s;
		{ char** list = s->ReadSaveTitle();
		  global_system.main_window->UpdateMenu(list);
		}
		global_system.ClearSenarioNumber();
		GlobalStackItem item;
		item.SetGlobal(first_senario, first_point);
		// ここまでにシグナルを受け取っていても終了
		if (is_quit) {
			delete s; senario_f = 0;
			QuitAll();
			idle_isProcess = 0;
			return FALSE;
		}
		// signal を null_handler から有効な handler に変更
		signal(SIGINT, signal_handler);
		signal(SIGTERM, signal_handler);
		signal(SIGSEGV, signal_handler_segv);
		global_system.SetMainSenario(s);
		while(global_system.main_window != 0) {
			item = s->Play(item); // intterupt が入るまでシナリオ実行
			if (is_quit) break; /* シグナルを受け取ったら終了 */
			if (! item.IsValid()) break; // シナリオからのシステム終了
			// なにか、やることがあるか？ : save, load, goto menu
			global_system.CallProcessMessages();
			if (global_system.SaveData() != -1) {
				s->WriteSaveFile(global_system.SaveData(),global_system.GetTitle());
				char** list = s->ReadSaveTitle();
				global_system.UpdateMenu(list);
			} else if (global_system.LoadData() != -1) {
				s->ReadSaveFile(global_system.LoadData(), item);
			} else if (global_system.GoSenario() != -1) {
				item.SetGlobal(global_system.GoSenario(),0);
			}
			global_system.ClearIntterupt();
			global_system.ClearSenarioNumber();
		}
		global_system.SetMainSenario(0);
		delete s;
		senario_f = 0;
		global_system.Finalize();
		idle_isProcess = 0;
		/* システム終了 */
		gtk_main_quit();
	}
	// idle_handler は一回しかよばれないから false を返す
	return FALSE ;
}

extern "C" gint timer(gpointer arg) { // Gtk handler
	// window が初期化されるまで、なにもしない
	if (global_system.IsIntterupted() == 2) { gtk_main_quit(); return FALSE; }
	if (global_system.main_window == 0) return TRUE;
	if (! global_system.main_window->IsInitialized() ) return TRUE;
	// windowが存在するなら、100 ms に１回、リターン・カーソルの形状変更
	global_system.main_window->TimerCall();
	// シナリオのデコード開始(デコード中ならなにもしない)
	if (! idle_isProcess)
		gtk_idle_add(idle_handler, 0);
	return TRUE;
}

void CgmInit(void);
void SetCgmFile(char* path, int flen);

#ifdef MPROF
extern "C" void mprof_stop(void);
extern "C" void mprof_restart(char*);
extern "C" void set_mprof_autosave(int);
#endif

int main(int argc, char *argv[])
{
	int i;
	Initialize::Exec(); // 初期化処理
	char* fontname = "Sans 24"; // 最も一般的なフォント指定
	int fontsize = 0;

#ifdef MPROF
	mprof_stop();
#endif
	// とりあえず、シグナルを無効にしておく
	signal(SIGINT, null_signal_handler);
	signal(SIGTERM, null_signal_handler);
	signal(SIGALRM, alarm_signal_handler);
	signal(SIGSEGV, signal_handler_segv);

	gtk_set_locale();
	CgmInit();

#if 0 && defined(sun) && (defined(sparc) || defined(__sparc))
	pthread_setconcurrency(10);
#endif
	bindtextdomain("ayusys_gtk2", LOCALEDIR);
	bind_textdomain_codeset ("ayusys_gtk2", "UTF-8");
	textdomain("ayusys_gtk2");

	global_system.SetTextSpeed(1000);

#ifdef DEFAULT_DATAPATH
	char* datpath = DEFAULT_DATAPATH;
#else
	char* datpath = "/tmp/kanon";
#endif
	/* まずはじめに gameexe.ini の存在するパスを調べる */
	for (i=1; i<argc; i++) {
		if (strcmp(argv[i], "--path") == 0) {
			datpath = argv[i+1];
		}
	}
	// config ファイルの読み込み
	if (file_searcher.InitRoot(datpath) == -1) {
		parse_option(&argc, &argv, global_system,
		     &fontname, &fontsize, &savepath); // help file の表示など
		fprintf(stderr, "Cannot use %s as root directory ; it cannot be read or there is no dat/ directory.\n",datpath);
		return -1;
	}
	global_system.LoadInitFile();

	parse_option(&argc, &argv, global_system,
		     &fontname, &fontsize, &savepath);
	/* cgm file 読み込み */
	ARCINFO* cgminfo = file_searcher.Find(FILESEARCH::ALL,global_system.config->GetParaStr("#CGM_FILE"),".CGM");
	if (cgminfo != 0) {
		char* cgmdat = cgminfo->CopyRead();
		int clen = cgminfo->Size();
		if (cgmdat != 0) SetCgmFile(cgmdat, clen);
		delete cgminfo; delete[] cgmdat;
	} else {
		/* fprintf(stderr, "Cannot open cgm file : mode.cgm\n"); */
	}
	global_system.InitMusic();
#ifdef MPROF
	mprof_restart("xayusys.mem");
#endif

	gtk_init (&argc, &argv);
	AyuWindow* view = new AyuWindow(global_system);
	if (fontsize != 0)
		view->SetFontSize(fontsize);
	view->SetFont(fontname);
	
	if (argc>1) first_senario = atoi(argv[1]);
	if (argc>2) first_point = atoi(argv[2]);
	if (first_senario == 0) first_senario = global_system.config->GetParaInt("#SEEN_START");
	if (first_senario == 0) first_senario = global_system.config->GetParaInt("#SEEN_SRT");
	if (first_senario == 0) first_senario = 1;

	/* リターンカーソルの blink 用の timeout */
	g_timeout_add(10, timer, 0);

        gtk_main();  // 終了するには ^C する必要がある。

	/* 使用したメモリの大部分を return 前に解放しておく
	** (memory profiling 用)
	*/
	delete view;

        return(0);
}
