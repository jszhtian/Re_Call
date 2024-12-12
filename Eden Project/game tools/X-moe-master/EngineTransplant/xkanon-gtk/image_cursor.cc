/* image_cursor.cc : X 上で pixmap をカーソルとして使うためのクラス
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


#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif
#include "image_cursor.h"
#include <gdk/gdk.h>
#include <stdio.h>
#include <string.h>

P_CURSOR::P_CURSOR(GdkWindow* win) : p_window(win) {
	// 消えたマウスカーソルをつくる
	GdkColor black = {0,0,0,0};
	GdkColor white = {0,0xffff,0xffff,0xffff};
	gchar buf = 0;
	GdkBitmap* bitmap = gdk_bitmap_create_from_data(p_window, &buf, 1, 1);
	null_cursor = gdk_cursor_new_from_pixmap(
		bitmap, bitmap, &white, &black, 0, 0);
	arrow_cursor = gdk_cursor_new(GDK_ARROW);
	g_object_unref(bitmap);
}
P_CURSOR::~P_CURSOR() {
	gdk_cursor_unref(null_cursor);
	gdk_cursor_unref(arrow_cursor);
}
void P_CURSOR::Draw(void) {
	gdk_window_set_cursor(p_window, arrow_cursor);
}
void P_CURSOR::Delete(void) {
	gdk_window_set_cursor(p_window, null_cursor);
}
void P_CURSOR::DrawCursor(GdkWindow* pwin) {
	gdk_window_set_cursor(pwin, arrow_cursor);
}
void P_CURSOR::DeleteCursor(GdkWindow* pwin) {
	gdk_window_set_cursor(pwin, null_cursor);
}

PIX_CURSOR::PIX_CURSOR(GdkWindow* win, GdkPixmap* pix, GdkBitmap* bitmap, char* im_mask, GdkPixmap* bg)
	: P_CURSOR(win), window(win), pixmap(pix), mask(bitmap), background(bg)
{
	g_object_ref(win);
	g_object_ref(pixmap);
	g_object_ref(mask);
	g_object_ref(background);

	savebuf = 0;
	x = DELETED_MOUSE_X; y = DELETED_MOUSE_X; // cursor is deleted
	image_mask = im_mask;
	// image をつくる
	int width, height;
	gdk_drawable_get_size(GDK_DRAWABLE(pixmap), &width, &height);
	image = gdk_drawable_get_image( GDK_DRAWABLE(pixmap), 0, 0, width, height);
	
	buffer_pixmap = gdk_pixmap_new(GDK_DRAWABLE(window), 64, 64, -1);
	masked_gc = gdk_gc_new(GDK_DRAWABLE(buffer_pixmap));
	gc = gdk_gc_new(GDK_DRAWABLE(buffer_pixmap));
	gdk_gc_set_clip_mask(masked_gc, mask);
}

PIX_CURSOR::~PIX_CURSOR() {
	g_object_unref(masked_gc);
	g_object_unref(gc);
	g_object_unref(buffer_pixmap);
	g_object_unref(image);
	g_object_unref(pixmap);
	g_object_unref(mask);
	g_object_unref(background);
	g_object_unref(window);
}

void PIX_CURSOR::UpdateBuffer(void) {
}

// pixmap に単純に描画する
void PIX_CURSOR::DrawPixmapRelative(GdkPixmap* pix, int xx, int yy) {
	if (x == DELETED_MOUSE_X) return;// マウスが表示されてない
	int nx = x-xx; int ny = y-yy;
	gdk_gc_set_clip_origin(masked_gc, nx, ny);
	gdk_draw_drawable(GDK_DRAWABLE(pix), masked_gc, GDK_DRAWABLE(pixmap), 0, 0, nx, ny, 32, 32);
}

void PIX_CURSOR::Draw(int new_x, int new_y) {
	if (new_x == DELETED_MOUSE_X) {Delete(); return; }
	if (x == DELETED_MOUSE_X) { // 現在、なにも表示されてない -> 表示するだけ
		gdk_gc_set_clip_origin(masked_gc, new_x, new_y);
		gdk_draw_drawable(GDK_DRAWABLE(window), masked_gc, GDK_DRAWABLE(pixmap), 0, 0, new_x, new_y, 32, 32);
		x = new_x; y = new_y;
	} else {
		int dx = new_x - x; int dy = new_y - y;
		if (dx < 0) dx = -dx; if (dy < 0) dy = -dy;
		if (dx >= 32 || dy >= 32) { // 元のカーソルと新しいカーソルに共通部はない
			gdk_gc_set_clip_origin(masked_gc, new_x,new_y);
			gdk_draw_drawable(GDK_DRAWABLE(window),gc, GDK_DRAWABLE(background), x, y, x, y, 32, 32);
			gdk_draw_drawable(GDK_DRAWABLE(window),masked_gc, GDK_DRAWABLE(pixmap), 0, 0, new_x, new_y, 32, 32);
			x = new_x; y = new_y;
		} else { //共通部あり
			int x0, y0, width, height;
			if (x < new_x) { x0=x; width=new_x-x+32;
			} else { x0=new_x; width=x-new_x+32;
			}
			if (y < new_y) { y0=y; height=new_y-y+32;
			} else { y0=new_y; height=y-new_y+32;
			}
			gdk_draw_drawable(GDK_DRAWABLE(buffer_pixmap),gc, GDK_DRAWABLE(background), x0, y0, 0, 0, width, height);
			gdk_gc_set_clip_origin(masked_gc,new_x-x0,new_y-y0);
			gdk_draw_drawable(GDK_DRAWABLE(buffer_pixmap),masked_gc, GDK_DRAWABLE(pixmap), 0, 0, new_x-x0, new_y-y0, 32, 32);
			gdk_draw_drawable(GDK_DRAWABLE(window),gc, GDK_DRAWABLE(buffer_pixmap), 0, 0, x0, y0, width, height);
			x = new_x; y = new_y;
		}
	}
}

void PIX_CURSOR::Delete(void) {
	if (x == DELETED_MOUSE_X) return; // already deleted
	gdk_draw_drawable(GDK_DRAWABLE(window),gc, GDK_DRAWABLE(background), x, y, x, y, 32, 32);
	x = DELETED_MOUSE_X; y = DELETED_MOUSE_X;
}

// dest <- src でコピー。dest の内容は保存
PIX_CURSOR_SAVEBUF::PIX_CURSOR_SAVEBUF(GdkImage* dest_im, GdkImage* src_im, char* mask, int dest_x, int dest_y)
{
	int src_x = 0; int src_y = 0;
	src = 0; dest = 0; bpl = 0;

	int swidth = src_im->width; int sheight = src_im->height;
	int dwidth = dest_im->width; int dheight = dest_im->height;
	int width = swidth; height = sheight;
	/* ２つのウィンドウの交叉を取る */
	/* まず、width を丸める */
	if (src_x < 0) { dest_x -= src_x; src_x = 0;}
	if (src_y < 0) { dest_y -= src_y; src_y = 0;}
	if (dest_x < 0) { src_x -= dest_x; dest_x = 0;}
	if (dest_y < 0) { src_y -= dest_y; dest_y = 0;}
	
	dwidth -= dest_x; dheight -= dest_y;
	swidth -= src_x; sheight -= src_y;
	/* dest の大きさが src の大きさより小さければ、そっちに合わせる */
	if (dwidth < swidth) swidth = dwidth;
	if (dheight < sheight) sheight = dheight;
	/* 範囲チェック：コピーする画像が０以下の大きさになったらエラー */
	if (swidth <= 0 || sheight <= 0) return;

	/* 大きさを得る */
	width = swidth; height = sheight;
	swidth = src_im->width; sheight = src_im->height;
	dwidth = dest_im->width; dheight = dest_im->height;

	// bpp は同じはず
	int bpp = dest_im->bpl / dest_im->width;
	int sbpl = src_im->bpl; dbpl = dest_im->bpl;
	char* from_data = (char*)src_im->mem;
	dest = (char*)dest_im->mem;

	// 座標を合わせる
	from_data += src_x*bpp + src_y*sbpl;
	dest += dest_x*bpp + dest_y*dbpl;
	mask += src_x + src_y*swidth;

#ifdef SUPPORT_MSB_FIRST
	int reverse = 0;
	if (dest_im->byte_order == MSBFirst)
		reverse = !0;
#endif

	// クラスのメンバーを初期化
	bpl = bpp * width; src = new char[height*bpl];
	// dest -> src へコピー
	char* buf1 = dest; char* buf2 = src;
	int i,j; for (i=0; i<height; i++) {
		memcpy(buf2, buf1, bpl);
		buf1 += dbpl; buf2 += bpl;
	}
	// src_im -> dest_im へコピー
	buf1 = from_data; buf2 = dest;
	for (i=0; i<height; i++) {
		char* b1 = buf1; char* b2 = buf2; char* m = mask;
		for (j=0; j<width; j++) {
			if (*m) {
#ifdef SUPPORT_MSB_FIRST
				if (reverse && bpp == 4) {
					b2[0] = b1[3];
					b2[1] = b1[2];
					b2[2] = b1[1];
					b2[3] = b1[0];
				} else {
				memcpy(b2, b1, bpp);
				}
#else  // SUPPORT_MSB_FIRST
				memcpy(b2, b1, bpp);
#endif // SUPPORT_MSB_FIRST
			}
			b1 += bpp; b2 += bpp; m++;
		}
		buf1 += sbpl; buf2 += dbpl; mask += swidth;
	}
	// 終了
	return;
}
PIX_CURSOR_SAVEBUF::~PIX_CURSOR_SAVEBUF()
{
	// src -> dest へコピー
	char* buf1 = src; char* buf2 = dest;
	int i; for (i=0; i<height; i++) {
		memcpy(buf2, buf1, bpl);
		buf1 += bpl; buf2 += dbpl;
	}
	delete src;
}

