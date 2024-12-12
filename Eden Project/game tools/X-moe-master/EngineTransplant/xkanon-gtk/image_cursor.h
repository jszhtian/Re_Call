/* image_cursor.h : X 上で pixmap をカーソルとして使うためのクラス
 *     他のクラスからはかなり、独立している。
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


#ifndef __KANON_IMAGE_CURSOR_H__
#define __KANON_IMAGE_CURSOR_H__

#include <gdk/gdktypes.h>
#define DELETED_MOUSE_X 100000

// image をコピー、リストアする
class PIX_CURSOR_SAVEBUF {
	char* dest;
	char* src;
	int bpl, dbpl;
	int height;
public:
	PIX_CURSOR_SAVEBUF(GdkImage* dest, GdkImage* src, char* mask, int x, int y);
	~PIX_CURSOR_SAVEBUF();
};

class P_CURSOR { // PIX_CURSOR のラッパー
	GdkWindow* p_window;
	GdkCursor* null_cursor;
	GdkCursor* arrow_cursor;
public:
	P_CURSOR(GdkWindow* win);
	virtual ~P_CURSOR();
	virtual void DrawImage(GdkImage* im, int x, int y) {}
	virtual void DrawImage(GdkImage* im) {}
	virtual void DrawImageRelative(GdkImage* im, int xx, int yy) {}
	virtual void DrawPixmapRelative(GdkPixmap* pix, int xx, int yy) {}
	virtual void RestoreImage(void) {}
	virtual void UpdateBuffer(void) {}
	// カーソルを書く
	virtual void Draw(void);
	virtual void DrawCursor(GdkWindow* window);
	// カーソルを消す
	virtual void Delete(void);
	virtual void DeleteCursor(GdkWindow* window);
	virtual void Draw(int x, int y) { Draw();}
	virtual void Draw_without_Delete(void) {Draw();}
};

class PIX_CURSOR : public P_CURSOR { // 色つきカーソルを実現するためのクラス。
	GdkWindow* window; // 描画先の window
	GdkPixmap* pixmap; // マウスカーソルのpixmap
	GdkBitmap* mask; // マウスカーソルのマスク
	GdkPixmap* background; // window と同じ内容の pixmap(背景を取るのにつかう)
	GdkImage* image; // マウスの image
	char* image_mask; // image のマスク

	GdkPixmap* buffer_pixmap; // 背景と合成するための一時バッファ
	GdkGC* masked_gc,* gc;
	PIX_CURSOR_SAVEBUF* savebuf; // image にマウスを描くとき、もとの image を残すためのバッファ

	int x, y;
public:
	PIX_CURSOR(GdkWindow* win, GdkPixmap* pix, GdkBitmap* bitmap, char* mask, GdkPixmap* background);
	~PIX_CURSOR();
	// GdkImage に、カーソルを描画する。ダブルバッファを使わない時にこちらをつかう
	// 描いて、そのimageをwindowに描画したら、(XSync() した上で) RestoreImage() しなければならない
	void DrawImage(GdkImage* im, int x, int y) { 
		if (savebuf) delete savebuf;
		savebuf = 0;
		if (x == DELETED_MOUSE_X) return;
		savebuf = new PIX_CURSOR_SAVEBUF(im, image, image_mask, x, y);
	}
	void DrawImage(GdkImage* im) {
		DrawImage(im,x,y);
	}
	// 現在カーソルのある位置に対し、比較で xx,yy の位置に描く。
	// window の一部に image を上書きするとき、その image にマウスカーソルを描くために使う
	void DrawImageRelative(GdkImage* im, int xx, int yy) {
		DrawImage(im, x+xx, y+yy);
	}
	void DrawPixmapRelative(GdkPixmap* pix, int xx, int yy);
	void RestoreImage(void) { if (savebuf != 0) delete savebuf; savebuf = 0;}
	void UpdateBuffer(void); // 未使用
	// (x,y) の位置にマウスカーソルを移動
	void Draw(int x, int y);
	void Draw(void) { if (x != DELETED_MOUSE_X) Draw(x,y);}
	void Draw_without_Delete(void) { // 画面の書き直しなどが起きたとき、カーソルだけを書き直す
		if (x == DELETED_MOUSE_X) return;
		int orig_x = x;
		x = DELETED_MOUSE_X;
		Draw(orig_x,y);
	}
	// カーソルを消す
	void Delete(void);
};

#endif /* __KANON_IMAGE_CURSOR_H__ */
