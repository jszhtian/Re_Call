/* image_sys.h
 *	文字ウィンドウの枠、マウスカーソル、リターンカーソルなどを
 *	システムの pdt ファイルから読み込み、描画するためのクラス
**/
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

#ifndef __KANON_IMAGE_SYS_H__
#define __KANON_IMAGE_SYS_H__

#include <gdk/gdktypes.h>
#include "system.h"
#include "file.h"
#include "image_cursor.h"

using namespace std;

// GdkImage を 32bpp (r,g,b = 8bit) のデータから作成する
// data は、image->width x image->height のサイズであること。
void SetImage(GdkImage* image, unsigned char* data);

// 文字ウィンドウの枠、マウスカーソルなどのシステムに必要な画像
// PDT ファイルからよみこむ

// SYSTEM_PDT_IMAGE のラッパー
// image ないときに dummy を使うため。
class SYSTEM_IMAGE {
	int cursor_type;
public:
	int CursorType(void) { return cursor_type; }
	void SetCursorType(int t) { if (t<0 || t>3) t=0;  cursor_type = t; }
	virtual ~SYSTEM_IMAGE() {};
	SYSTEM_IMAGE(void) { cursor_type = 0;};
	virtual int Init(GdkWindow* window, AyuSys& local_system) {
		return true;
	}
	virtual P_CURSOR* CreateCursor(GdkWindow* window, GdkPixmap* back) {
		return new P_CURSOR(window);
	}
	virtual void DrawReturnPixmap(GdkWindow* win, GdkDrawable* d1, GdkDrawable* d2, int x, int y, P_CURSOR* c) {}
	virtual void DrawReturnPixmap(P_CURSOR* cur) {};
	void DrawReturnPixmap(GdkWindow* win, GdkDrawable* d1, GdkDrawable* d2, int x, int y, P_CURSOR* c, int type) {
		SetCursorType(type);
		DrawReturnPixmap(win,d1,d2,x,y,c);
	}
	void DrawReturnPixmap(P_CURSOR* cur, int type) {SetCursorType(type); DrawReturnPixmap(cur);};
	virtual void DeleteReturnPixmap(P_CURSOR* c) {};
	// 枠の描画：これだけはまじめに
	virtual void DrawWaku(GdkImage* d, int x, int y, int width, int height, AyuSys& local_system);
	// 画面を暗くする。bright==100 でなにもしない、0 で真っ黒
	virtual void SetBrightness(GdkImage* d,int bright, AyuSys& local_system);
};

struct RETN_CURSOR {
	GdkImage* return_image;
	char* return_mask;
	int retn_patterns;
	int retn_width;
	int retn_height;
};

class SYSTEM_PDT_IMAGE : public SYSTEM_IMAGE{
	char* filename;

	class WAKU_PDT* waku[4]; // ４種類

	// return cursor 関係
	GdkDrawable* return_screen1, *return_screen2; // image を描く pixmap
	RETN_CURSOR return_cursor[4];
	GdkImage* return_tmpimage;
	GdkGC* gc;
	int return_x, return_y;
	GdkImage* return_back; // back ground
	int return_count;

	GdkPixmap* mouse_pixmap; GdkBitmap* mouse_bitmap; // mouse cursor の pixmap
	char* mouse_image_mask;

	void InitPixmaps(GdkWindow* window, char* data, int width, int height, AyuSys& local_system);
public:
	~SYSTEM_PDT_IMAGE();
	SYSTEM_PDT_IMAGE(const char* fname);
	// 初期化
	int Init(GdkWindow* window, AyuSys& local_system);
	// マウスカーソルのセット
	P_CURSOR* CreateCursor(GdkWindow* window, GdkPixmap* background);
	// return pixmap の描画 d1, d2 はダブルバッファ用
	void DrawReturnPixmap(GdkWindow* win,GdkDrawable* d1, GdkDrawable* d2,int x, int y, P_CURSOR*);
	void DrawReturnPixmap(P_CURSOR*);
	void DeleteReturnPixmap(P_CURSOR*);
	// 枠の描画
	void DrawWaku(GdkImage* d,  int x, int y, int width, int height, AyuSys& local_system);

};

#endif /* !defined(__KANON_IMAGE_SYS_H__) */
