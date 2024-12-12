/*  window_all.h
 *
 ************************************************************
 *
 *     �����̥⡼�������ѤΥ��饹
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

// #define HAVE_LIBXXF86VM /* ���줬�������Ƥ���С�X Vidmode Extension ����Ѥ��� */
#ifdef HAVE_CONFIG_H
#include "config.h" // ��Υޥ�����ɬ�פʤ��������Ƥ���Ϥ���configure ��Ĥ���ʤ��ʤ餤��ʤ�
#endif

#include <X11/Xlib.h>
#ifdef HAVE_LIBXXF86VM
#include <X11/extensions/xf86vmode.h>
#define XF86VM_MINMAJOR 0
#define XF86VM_MINMINOR 5
#endif

#include<gtk/gtk.h>

#ifndef HAVE_LIBXXF86VM
/* �������Ƥʤ��ʤ顢���ߡ���Ĥ��� */
struct XF86VidModeModeInfo {void* pointer;};
#endif

class Window_AllScreen {
public:
	GtkWidget*	window;
	void InitWindow(GtkWidget* window);
	Window_AllScreen(void);
	~Window_AllScreen();

	int screen_bpl; // ���̤� bytes per line
	int flags; // ������ʥե饰
	void SetAllScreen(void) { flags |= 1;}
	void UnsetAllScreen(void) { flags &= ~1;}
	void SetUsableAllScreen(void) { flags |= 2;}
	int IsUsableAllScreen(void) { return flags & 2; }

	// ������֤��ݻ�����
	Display* dpy_restore;
	int screen_restore;
	// �ӥǥ��⡼�ɤξ���
	int vid_count;                    /* �⡼�ɤο�              */
	XF86VidModeModeInfo **modeinfos;  /* �⡼�ɾ��� (0 ���ǽ�)   */
	// �������̥⡼�ɤΤȤ��˸��ߤβ��̤��礭��
	int all_width, all_height;
	// �����̥⡼�ɤλ��Ρ˲��̤ΰ��֡��礭��
	int normal_x_pos, normal_y_pos, normal_width, normal_height;

	void GetAllScrnMode(void);
	static gboolean configureEvent(GtkWidget* w, GdkEventConfigure* p1, gpointer pointer);
	static gboolean enterEvent(GtkWidget* w, GdkEventCrossing* event, gpointer pointer);
	static gboolean destroyEvent(GtkWidget* w, GdkEventAny *event, gpointer pointer);


	int IsAllScreen(void) { return (window != 0) && (flags & 1); }
	void RestoreMode(void);	// ���̤򸵤ξ��֤��᤹
	int ToAllScreen(int width, int height); // ���̤������̥⡼�ɤ�
		// �ºݤ˲��̥⡼�ɤ������� & width/height �ˤʤä��顢0�ʳ����֤�
	XF86VidModeModeInfo* CheckModeLinesRes(int w, int h); // ���ꤵ�줿 w/h �Υ⡼�ɤ�õ��
};

#endif /* ! defined(__VIDMODE_GTKMM_WINDOW_ALLSCRN_H__) */
