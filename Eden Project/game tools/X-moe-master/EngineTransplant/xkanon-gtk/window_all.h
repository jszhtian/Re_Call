/*  window_all.h
 *
 ************************************************************
 *
 *     全画面モード制御用のクラス
 *
 ************************************************************
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


#ifndef __VIDMODE_GTKMM_WINDOW_ALLSCRN_H__
#define __VIDMODE_GTKMM_WINDOW_ALLSCRN_H__

// #define HAVE_LIBXXF86VM /* これが定義されていれば、X Vidmode Extension を使用する */
#ifdef HAVE_CONFIG_H
#include "config.h" // 上のマクロを（必要なら）定義しているはず。configure をつかわないならいらない
#endif

#include <X11/Xlib.h>
#ifdef HAVE_LIBXXF86VM
#include <X11/extensions/xf86vmode.h>
#define XF86VM_MINMAJOR 0
#define XF86VM_MINMINOR 5
#endif

#include<gtk/gtk.h>

#ifndef HAVE_LIBXXF86VM
/* 定義されてないなら、ダミーをつくる */
struct XF86VidModeModeInfo {void* pointer;};
#endif

class Window_AllScreen {
public:
	GtkWidget*	window;
	void InitWindow(GtkWidget* window);
	Window_AllScreen(void);
	~Window_AllScreen();

	int screen_bpl; // 画面の bytes per line
	int flags; // いろいろなフラグ
	void SetAllScreen(void) { flags |= 1;}
	void UnsetAllScreen(void) { flags &= ~1;}
	void SetUsableAllScreen(void) { flags |= 2;}
	int IsUsableAllScreen(void) { return flags & 2; }

	// 初期状態を保持する
	Display* dpy_restore;
	int screen_restore;
	// ビデオモードの情報
	int vid_count;                    /* モードの数              */
	XF86VidModeModeInfo **modeinfos;  /* モード情報 (0 が最初)   */
	// （全画面モードのとき）現在の画面の大きさ
	int all_width, all_height;
	// （普通モードの時の）画面の位置・大きさ
	int normal_x_pos, normal_y_pos, normal_width, normal_height;

	void GetAllScrnMode(void);
	static gboolean configureEvent(GtkWidget* w, GdkEventConfigure* p1, gpointer pointer);
	static gboolean enterEvent(GtkWidget* w, GdkEventCrossing* event, gpointer pointer);
	static gboolean destroyEvent(GtkWidget* w, GdkEventAny *event, gpointer pointer);


	int IsAllScreen(void) { return (window != 0) && (flags & 1); }
	void RestoreMode(void);	// 画面を元の状態に戻す
	int ToAllScreen(int width, int height); // 画面を全画面モードへ
		// 実際に画面モードが全画面 & width/height になったら、0以外が返る
	XF86VidModeModeInfo* CheckModeLinesRes(int w, int h); // 指定された w/h のモードを探す
};

#endif /* ! defined(__VIDMODE_GTKMM_WINDOW_ALLSCRN_H__) */
