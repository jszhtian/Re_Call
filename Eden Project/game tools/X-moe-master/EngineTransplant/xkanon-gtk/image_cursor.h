/* image_cursor.h : X ��� pixmap �򥫡�����Ȥ��ƻȤ�����Υ��饹
 *     ¾�Υ��饹����Ϥ��ʤꡢ��Ω���Ƥ��롣
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

// image �򥳥ԡ����ꥹ�ȥ�����
class PIX_CURSOR_SAVEBUF {
	char* dest;
	char* src;
	int bpl, dbpl;
	int height;
public:
	PIX_CURSOR_SAVEBUF(GdkImage* dest, GdkImage* src, char* mask, int x, int y);
	~PIX_CURSOR_SAVEBUF();
};

class P_CURSOR { // PIX_CURSOR �Υ�åѡ�
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
	// ����������
	virtual void Draw(void);
	virtual void DrawCursor(GdkWindow* window);
	// ���������ä�
	virtual void Delete(void);
	virtual void DeleteCursor(GdkWindow* window);
	virtual void Draw(int x, int y) { Draw();}
	virtual void Draw_without_Delete(void) {Draw();}
};

class PIX_CURSOR : public P_CURSOR { // ���Ĥ����������¸����뤿��Υ��饹��
	GdkWindow* window; // ������� window
	GdkPixmap* pixmap; // �ޥ������������pixmap
	GdkBitmap* mask; // �ޥ�����������Υޥ���
	GdkPixmap* background; // window ��Ʊ�����Ƥ� pixmap(�طʤ���ΤˤĤ���)
	GdkImage* image; // �ޥ����� image
	char* image_mask; // image �Υޥ���

	GdkPixmap* buffer_pixmap; // �طʤȹ������뤿��ΰ���Хåե�
	GdkGC* masked_gc,* gc;
	PIX_CURSOR_SAVEBUF* savebuf; // image �˥ޥ����������Ȥ�����Ȥ� image ��Ĥ�����ΥХåե�

	int x, y;
public:
	PIX_CURSOR(GdkWindow* win, GdkPixmap* pix, GdkBitmap* bitmap, char* mask, GdkPixmap* background);
	~PIX_CURSOR();
	// GdkImage �ˡ�������������褹�롣���֥�Хåե���Ȥ�ʤ����ˤ������Ĥ���
	// �����ơ�����image��window�����褷���顢(XSync() �������) RestoreImage() ���ʤ���Фʤ�ʤ�
	void DrawImage(GdkImage* im, int x, int y) { 
		if (savebuf) delete savebuf;
		savebuf = 0;
		if (x == DELETED_MOUSE_X) return;
		savebuf = new PIX_CURSOR_SAVEBUF(im, image, image_mask, x, y);
	}
	void DrawImage(GdkImage* im) {
		DrawImage(im,x,y);
	}
	// ���ߥ�������Τ�����֤��Ф�����Ӥ� xx,yy �ΰ��֤�������
	// window �ΰ����� image ���񤭤���Ȥ������� image �˥ޥ��������������������˻Ȥ�
	void DrawImageRelative(GdkImage* im, int xx, int yy) {
		DrawImage(im, x+xx, y+yy);
	}
	void DrawPixmapRelative(GdkPixmap* pix, int xx, int yy);
	void RestoreImage(void) { if (savebuf != 0) delete savebuf; savebuf = 0;}
	void UpdateBuffer(void); // ̤����
	// (x,y) �ΰ��֤˥ޥ�������������ư
	void Draw(int x, int y);
	void Draw(void) { if (x != DELETED_MOUSE_X) Draw(x,y);}
	void Draw_without_Delete(void) { // ���̤ν�ľ���ʤɤ��������Ȥ�����������������ľ��
		if (x == DELETED_MOUSE_X) return;
		int orig_x = x;
		x = DELETED_MOUSE_X;
		Draw(orig_x,y);
	}
	// ���������ä�
	void Delete(void);
};

#endif /* __KANON_IMAGE_CURSOR_H__ */
