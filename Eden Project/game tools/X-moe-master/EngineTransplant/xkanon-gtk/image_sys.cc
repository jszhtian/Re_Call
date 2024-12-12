/* image_sys.cc
 *	文字ウィンドウの枠、マウスカーソル、リターンカーソルなどを
 *	システムの pdt ファイルから読み込み、描画する
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

#include "file.h"
#include "system.h"
#include "image_sys.h"
#include "image_cursor.h"
#include <gdk/gdk.h>

#ifdef SUPPORT_MSB_FIRST
void FitImage(GdkImage *dest, GdkImage *src);
#endif


// 枠をかく：下位クラス

static void copyrect(char* dest,char* src, int dest_width,int src_width, int height) {
	int i;
	int width;
	if (dest_width < src_width) width = dest_width;
	else width = src_width;
	for (i=0; i<height;i++) {
		memcpy(dest, src, width);
		dest += dest_width; src += src_width;
	}
}


class WAKU_PDT {
	friend class SYSTEM_PDT_IMAGE; // ここからしかアクセスできない
	// 枠のイメージ
	GdkImage* waku_top_left;	// 48x8
	char* mask_waku_top_left;
	GdkImage* waku_top_center;	// 16x8
	char* mask_waku_top_center;
	GdkImage* waku_top_right;	// 48x8
	char* mask_waku_top_right;
	GdkImage* waku_bottom_left;	// 48x8
	char* mask_waku_bottom_left;
	GdkImage* waku_bottom_center;	// 16x8
	char* mask_waku_bottom_center;
	GdkImage* waku_bottom_right;	// 48x8
	char* mask_waku_bottom_right;
	GdkImage* waku_left_top;		// 8x16
	char* mask_waku_left_top;
	GdkImage* waku_left_center;		// 8x16
	char* mask_waku_left_center;
	GdkImage* waku_left_bottom;		// 8x16
	char* mask_waku_left_bottom;
	GdkImage* waku_right_top;		// 8x16
	char* mask_waku_right_top;
	GdkImage* waku_right_center;		// 8x16
	char* mask_waku_right_center;
	GdkImage* waku_right_bottom;		// 8x16
	char* mask_waku_right_bottom;

	WAKU_PDT(void);
	~WAKU_PDT();
	void Init(GdkWindow* win, char* data, char* mask_data, int x, int y, int width); // 枠の初期化
	void Draw(GdkImage* d, int x, int y, int width, int height); // width x height の枠を x,y に描く
	void MakeImage(GdkWindow* win,  // image を作る
		char* data, char* mask_data, int x, int y, int width, int height, int src_width,
		GdkImage*& image, char*& mask);
	void CopyImage(GdkImage* d, GdkImage* s, char* mask, int x, int y);  // 上で作った image / mask を描く
};

WAKU_PDT::WAKU_PDT(void) {
	waku_top_left = 0;
	waku_top_center = 0;
	waku_top_right = 0;
	waku_bottom_left = 0;
	waku_bottom_center = 0;
	waku_bottom_right = 0;
	waku_left_top = 0;
	waku_left_center = 0;
	waku_left_bottom = 0;
	waku_right_top = 0;
	waku_right_center = 0;
	waku_right_bottom = 0;
	mask_waku_top_left = 0;
	mask_waku_top_center = 0;
	mask_waku_top_right = 0;
	mask_waku_bottom_left = 0;
	mask_waku_bottom_center = 0;
	mask_waku_bottom_right = 0;
	mask_waku_left_top = 0;
	mask_waku_left_center = 0;
	mask_waku_left_bottom = 0;
	mask_waku_right_top = 0;
	mask_waku_right_center = 0;
	mask_waku_right_bottom = 0;
}

WAKU_PDT::~WAKU_PDT() {
	if (waku_top_left == 0) return;

	g_object_unref(waku_top_left);
	g_object_unref(waku_top_center);
	g_object_unref(waku_top_right);
	g_object_unref(waku_bottom_left);
	g_object_unref(waku_bottom_center);
	g_object_unref(waku_bottom_right);
	g_object_unref(waku_left_top);
	g_object_unref(waku_left_center);
	g_object_unref(waku_left_bottom);
	g_object_unref(waku_right_top);
	g_object_unref(waku_right_center);
	g_object_unref(waku_right_bottom);
	delete mask_waku_top_left;
	delete mask_waku_top_center;
	delete mask_waku_top_right;
	delete mask_waku_bottom_left;
	delete mask_waku_bottom_center;
	delete mask_waku_bottom_right;
	delete mask_waku_left_top;
	delete mask_waku_left_center;
	delete mask_waku_left_bottom;
	delete mask_waku_right_top;
	delete mask_waku_right_center;
	delete mask_waku_right_bottom;
}

void WAKU_PDT::MakeImage(GdkWindow* win,  // image を作る
		char* srcdata, char* mask_data, int x, int y, int width, int height, int src_width,
		GdkImage*& image, char*& mask)
{
	// data
	char* data = new char[width*height*4];
	copyrect( data, srcdata + (x + y * src_width)*4, width*4, src_width*4, height);

	// imageをつくる
	if (image) g_object_unref(image);
	image = gdk_image_new(GDK_IMAGE_NORMAL,gdk_drawable_get_visual(GDK_DRAWABLE(win)), width, height);
	// コピー
	SetImage(image, (unsigned char*) data);
	delete[] data;

	// mask をつくる
	if (mask) delete mask;
	mask = new char[ (width+1)*height ];
	int i,j; for (i=0; i<height; i++) {
		char* cur_mask = mask + (width+1)*i + 1;
		char* cur_src_mask = mask_data + x + src_width*(y+i);
		int count = 0;
		for (j=0; j<width; j++) {
			if ( (*cur_mask++ = *cur_src_mask++) != 0) count++;
		}
		if (count == width) count = -1;
		else if (count != 0) count = 1;
		mask[ (width+1)*i] = count;
	}
}

void WAKU_PDT::Init(GdkWindow* win, char* data, char* mask_data, int x, int y, int width)
{
	data += ( x + y*width ) * 4; mask_data += ( x + y*width );
	MakeImage(win, data, mask_data, 0, 0, 48, 8, width, waku_top_left, mask_waku_top_left);
	MakeImage(win, data, mask_data, 48, 0, 16, 8, width, waku_top_center, mask_waku_top_center);
	MakeImage(win, data, mask_data, 64, 0, 48, 8, width, waku_top_right, mask_waku_top_right);
	MakeImage(win, data, mask_data, 0, 56, 48, 8, width, waku_bottom_left, mask_waku_bottom_left);

	MakeImage(win, data, mask_data, 48, 56, 16, 8, width, waku_bottom_center, mask_waku_bottom_center);

	MakeImage(win, data, mask_data, 64, 56, 48, 8, width, waku_bottom_right, mask_waku_bottom_right);
	MakeImage(win, data, mask_data, 0, 8, 8, 16, width, waku_left_top, mask_waku_left_top);
	MakeImage(win, data, mask_data, 0, 24, 8, 16, width, waku_left_center, mask_waku_left_center);
	MakeImage(win, data, mask_data, 0, 40, 8, 16, width, waku_left_bottom, mask_waku_left_bottom);
	MakeImage(win, data, mask_data, 104, 8, 8, 16, width, waku_right_top, mask_waku_right_top);
	MakeImage(win, data, mask_data, 104, 24, 8, 16, width, waku_right_center, mask_waku_right_center);
	MakeImage(win, data, mask_data, 104, 40, 8, 16, width, waku_right_bottom, mask_waku_right_bottom);
}

void WAKU_PDT::CopyImage(GdkImage* d, GdkImage* s, char* mask, int x, int y) {
	// データ
	char* src_pt = (char*)s->mem;
	char* dest_pt = (char*)d->mem;
	// width, bpl など
	int src_bpl = s->bpl; int dest_bpl = d->bpl;
	int src_width = s->width; int height = s->height;
	int bypp = (s->bpl)/src_width; // dest_bypp は等しいはず。
	
	dest_pt += bypp*x + dest_bpl*y; // 座標の設定
	
	// コピー
	int i;
	for (i=0; i<height; i++) {
		int m = *mask;
		if (m == 0) { // なにも描かない
			mask += src_width + 1;
		} else if (m == -1) { // そのまま描く
			memcpy(dest_pt, src_pt, src_bpl);
			mask += src_width + 1;
		} else { // マスクをとって描く
			int j,k; mask++; char* tmp_src = src_pt; char* tmp_dest = dest_pt;
			for (j=0; j<src_width; j++) {
				if (*mask++) {
					for (k=0; k<bypp; k++) *tmp_dest++ = *tmp_src++;
				} else {
					tmp_dest += bypp; tmp_src += bypp;
				}
			}
		}
		dest_pt += dest_bpl; src_pt += src_bpl;
	}
}

void WAKU_PDT::Draw(GdkImage* d, int x, int y, int width, int height) {
	if (mask_waku_top_left == 0) return;
	if (width < 96 || height < 48) {
		fprintf(stderr, "Runtime Error in WAKU_PDT::Draw : too little width / height value.\n");
		return;
	}
	int i; int count; int destx, desty;
	// 枠線を描く
	count = (width-81)/16; destx = x+48; desty = y;
	for (i=0; i<count ;i++) {
		CopyImage(d, waku_top_center, mask_waku_top_center, destx, desty);
		destx += 16;
	}
	destx = x+48; desty = y+height-8;
	for (i=0; i<count ;i++) {
		CopyImage(d, waku_bottom_center, mask_waku_bottom_center, destx, desty);
		destx += 16;
	}
	count = (height-33)/16; destx = x; desty = y+24;
	for (i=0; i<count ;i++) {
		CopyImage(d, waku_left_center, mask_waku_left_center, destx, desty);
		desty += 16;
	}
	destx = x+width-8; desty = y+24;
	for (i=0; i<count ;i++) {
		CopyImage(d, waku_right_center, mask_waku_right_center, destx, desty);
		desty += 16;
	}
	// ４隅を描く
	CopyImage(d, waku_top_left, mask_waku_top_left, x, y);
	CopyImage(d, waku_left_top, mask_waku_left_top, x, y+8);
	CopyImage(d, waku_top_right, mask_waku_top_right, x+width-48, y);
	CopyImage(d, waku_right_top, mask_waku_right_top, x+width-8, y+8);
	CopyImage(d, waku_bottom_left, mask_waku_bottom_left, x, y+height-8);
	CopyImage(d, waku_left_bottom, mask_waku_left_bottom, x, y+height-8-16);
	CopyImage(d, waku_bottom_right, mask_waku_bottom_right, x+width-48, y+height-8);
	CopyImage(d, waku_right_bottom, mask_waku_right_bottom, x+width-8, y+height-8-16);
}

SYSTEM_PDT_IMAGE::SYSTEM_PDT_IMAGE(const char* fname) {
	filename = new char[strlen(fname)+1];
	strcpy(filename, fname);
	// その他、メンバーの初期化
	return_screen1 = return_screen2 = 0;
	return_x = return_y = 0;
	return_count = 0;

	waku[0] = 0; waku[1] = 0; waku[2] = 0; waku[3] = 0;
	return_count = 0;

	gc = 0;
	return_tmpimage = 0;
	return_back = 0;
	mouse_pixmap = 0;
	mouse_bitmap = 0;

	return_screen1 = 0;
	return_screen2 = 0;
}

SYSTEM_PDT_IMAGE::~SYSTEM_PDT_IMAGE()
{
	delete[] filename;
	if (waku[0] != 0) { // already initialized.
		int i;for (i=0; i<4; i++) delete waku[i];
		g_object_unref(gc);
		g_object_unref(return_tmpimage);
		g_object_unref(mouse_pixmap);
		g_object_unref(mouse_bitmap);
		for (i=0; i<4; i++)
			g_object_unref(return_cursor[i].return_image);
	}
	if (return_back) g_object_unref(return_back);
	if (return_screen1) g_object_unref(return_screen1);
	if (return_screen2) g_object_unref(return_screen2);
}

int SYSTEM_PDT_IMAGE::Init(GdkWindow* window, AyuSys& local_system) {
	ARCINFO* info = file_searcher.Find(FILESEARCH::PDT, filename,".PDT.G00.GPD");
	if (info == 0) return false;
	const char* fdata = info->Read();
	GRPCONV* conv = GRPCONV::AssignConverter(fdata, info->Size(), "");
	char* data = (char*)conv->Read();
	delete info;
	if (data == 0) { delete conv; return false;}
	int width = conv->Width(); int height = conv->Height();
	InitPixmaps(window, data, width, height, local_system); // すべての pixmap などを初期化
	delete conv;
	return true;
}
void SYSTEM_PDT_IMAGE::InitPixmaps(GdkWindow* window,char* data, int width, int height, AyuSys& local_system)
{
	int i,j,k;
	unsigned int* srcbuf = (unsigned int*) data; // int == 32bit
	char* mask_data = new char[width*height];
	char* maskbuf = mask_data;

	unsigned int mask_pixel = *srcbuf;
	// mask をつくる
	for (i=0; i<height; i++) {
		for (j=0; j<width; j++) {
			if (*srcbuf == mask_pixel) {
				*maskbuf = 0;
				*srcbuf = 0;
			} else {
				*maskbuf = 0xff;
			}
			srcbuf++; maskbuf++;
		}
	}

	gc = gdk_gc_new(GDK_DRAWABLE(window));

	// return pixmap をつくる
	GdkVisual* vis = gdk_drawable_get_visual(GDK_DRAWABLE(window));
	/* retn_width / height などの初期化 */
	int max_retnw=0, max_retnh = 0;
	for (i=0; i<4; i++) {
		/* 属性を得る */
		if (i == 0) {
			return_cursor[i].retn_width = local_system.config->GetParaInt("#RETN_XSIZE");
			return_cursor[i].retn_height = local_system.config->GetParaInt("#RETN_YSIZE");
			return_cursor[i].retn_patterns = local_system.config->GetParaInt("#RETN_CONT");
		} else {
			return_cursor[i].retn_width = 32;
			return_cursor[i].retn_height = 32;
			return_cursor[i].retn_patterns = 8;
		}
		if (max_retnw < return_cursor[i].retn_width) max_retnw = return_cursor[i].retn_width;
		if (max_retnh < return_cursor[i].retn_height) max_retnh = return_cursor[i].retn_height;
		int ht = return_cursor[i].retn_height;
		int rw = return_cursor[i].retn_width * return_cursor[i].retn_patterns;
		/* データの作成 */
		char* retn_data = new char[rw * ht * 4];
		char* retn_mask = new char[rw * ht];
		int x0 = 48*i;
		if (i == 0) x0 += 8;
		copyrect(retn_data, data + (160 + (width*x0))*4 , rw*4, width*4, ht);
		copyrect(retn_mask, mask_data + (160+(width*x0)), rw, width, ht);
		/* image 化 */
		return_cursor[i].return_image = gdk_image_new(GDK_IMAGE_NORMAL, vis, rw, ht);

		SetImage(return_cursor[i].return_image, (unsigned char*) retn_data);
		return_cursor[i].return_mask = retn_mask;
	}
	// return_tmpimage をつくる
	return_tmpimage = gdk_image_new(GDK_IMAGE_SHARED, vis, max_retnw, max_retnh);
	if (return_tmpimage == 0) {
		return_tmpimage = gdk_image_new(GDK_IMAGE_NORMAL, vis, max_retnw, max_retnh);
	}
	// マウス・カーソルをつくる

	// (160,200) から、32x32 の大きさでカーソルが存在
	char* mouse_data = new char[32*32*4];
	copyrect(mouse_data, data + (160 + (width*200))*4, 32*4, width*4, 32);
	// pixmap にする
	mouse_pixmap = gdk_pixmap_new(GDK_DRAWABLE(window), 32, 32, -1);
	GdkImage* mouse_image = gdk_image_new(GDK_IMAGE_NORMAL, vis, 32, 32);
	SetImage(mouse_image, (unsigned char*)mouse_data);
#ifdef SUPPORT_MSB_FIRST
	if (mouse_image->byte_order == MSBFirst) {
		GdkImage* mouse_image_fit = gdk_image_new(GDK_IMAGE_NORMAL, vis, 32, 32);
		FitImage(mouse_image_fit, mouse_image);
		gdk_draw_image(GDK_DRAWABLE(mouse_pixmap), gc, mouse_image_fit, 0, 0, 0, 0, 32, 32);
	} else {
		gdk_draw_image(GDK_DRAWABLE(mouse_pixmap), gc, mouse_image, 0, 0, 0, 0, 32, 32);
	}
#else  // SUPPORT_MSB_FIRST
	gdk_draw_image(GDK_DRAWABLE(mouse_pixmap), gc, mouse_image, 0, 0, 0, 0, 32, 32);
#endif // SUPPORT_MSB_FIRST
	g_object_unref(mouse_image);

	// bitmap をつくる
	char* src_mask = mask_data + (160 + (width*200) ); char* mouse_mask = new char[32*32/8];
	char* mouse_mask_cur = mouse_mask;
	for (i=0; i<32; i++) {
		char* src_mask_cur = src_mask;
		for (j=0; j<4; j++) {
			int c = 0;
			for (k=0; k<8; k++) {
				if (*src_mask_cur++) { c>>=1; c|=0x80;}
				else { c>>=1; }
			}
			*mouse_mask_cur++ = c;
		}
		src_mask += width;
	}
	mouse_bitmap = gdk_bitmap_create_from_data(GDK_DRAWABLE(window), mouse_mask, 32, 32);
	delete[] mouse_mask;
	mouse_image_mask = new char[32*32]; src_mask = mask_data + (160 + width*200);
	for (i=0; i<32; i++) {
		memcpy(mouse_image_mask+i*32, src_mask, 32);
		src_mask += width;
	}

	// 枠をつくる
	for (i=0 ;i<4; i++) {
		waku[i] = new WAKU_PDT();
		waku[i]->Init(window, data, mask_data, 8, 8+100*i,width);
	}

	// 終わり
	delete[] mask_data;
}

void SYSTEM_PDT_IMAGE::DeleteReturnPixmap(P_CURSOR* cursor) {
	if (return_back == 0) return;
	/* return_screen2 は、マウスカーソル無し */
	int w=return_back->width;
	int h=return_back->height;
	if (return_x+w > global_system.DefaultScreenWidth()) w=global_system.DefaultScreenWidth()-return_x;
	if (return_y+h > global_system.DefaultScreenHeight()) h=global_system.DefaultScreenHeight()-return_y;
	gdk_draw_image(GDK_DRAWABLE(return_screen2), gc, return_back, 0, 0, return_x, return_y, w, h);
	global_system.FlushScreen();
	if (cursor) cursor->DrawImageRelative(return_back, -return_x, -return_y);
	/* return_screen1 はマウスカーソルあり */
	gdk_draw_image(GDK_DRAWABLE(return_screen1), gc, return_back, 0, 0, return_x, return_y, w, h);
	g_object_unref(return_back);
	g_object_unref(return_screen1);
	g_object_unref(return_screen2);
	return_back = 0;
	return_screen1 = 0;
	return_screen2 = 0;
	global_system.FlushScreen();
	cursor->RestoreImage();
}

void SYSTEM_PDT_IMAGE::DrawReturnPixmap(P_CURSOR* cursor)
{
	if (return_back == 0) return;
	int type = CursorType();
	int i,j,k;
	/* 必要な変数を得る */
	GdkImage* return_image = return_cursor[type].return_image;
	char*& return_mask = return_cursor[type].return_mask;
	int retn_patterns = return_cursor[type].retn_patterns;
	int retn_width = return_cursor[type].retn_width;
	int retn_height = return_cursor[type].retn_height;
	if (return_count >= retn_patterns) return_count = 0;
	/* image を描く */
	int bypp = (return_image->bpl) / (return_image->width);
	int mask_bpl = retn_patterns*retn_width; char* mask = return_mask + return_count*retn_width;
	char* src1 = (char*)(return_image->mem) + return_count*retn_width*bypp;
	char* src2 = (char*)(return_back->mem);
	int src1_bpl = return_image->bpl; int src2_bpl = return_back->bpl;
	char* dest = (char*)(return_tmpimage->mem); int dest_bpl = return_tmpimage->bpl;
#ifdef SUPPORT_MSB_FIRST
	int reverse = 0;
	if (return_image->byte_order == MSBFirst)
	    reverse = !0;
#endif // SUPPORT_MSB_FIRST
	for (i=0; i<retn_height; i++) {
		char* s1 = src1; char* s2 = src2; char* d = dest; char* m = mask;
		for (j=0; j<retn_width; j++) {
			char* s = s2;
			if (*m++) s=s1;
#ifdef SUPPORT_MSB_FIRST
			if (s != s2 && bypp == 4 && reverse) {
			    *d++ = 0;
			    *d++ = s[2];
			    *d++ = s[1];
			    *d++ = s[0];
			    s1 += bypp; s2 += bypp;
			} else {
			    for (k=0; k<bypp; k++) d[k]=s[k];
			    // memcpy(d, s, bypp);
			    s1 += bypp; s2 += bypp; d += bypp;
			}
#else // SUPPORT_MSB_FIRST
			for (k=0; k<bypp; k++) d[k]=s[k];
			// memcpy(d, s, bypp);
			s1 += bypp; s2 += bypp; d += bypp; 
#endif // SUPPORT_MSB_FIRST
		}
		src1 += src1_bpl; src2 += src2_bpl; dest += dest_bpl; mask += mask_bpl;
	}
#ifdef SUPPORT_MSB_FIRST1
	GdkImage* return_image_fit = gdk_image_new(GDK_IMAGE_NORMAL, gdk_drawable_get_visual(GDK_DRAWABLE(return_screen2)), retn_width, retn_height);
#endif // SUPPORT_MSB_FIRST1
	/* 画面に描く */
	/* return_screen2 は、マウスカーソル無し */
	if (return_x + retn_width > global_system.DefaultScreenWidth()) retn_width = global_system.DefaultScreenWidth() - return_x;
	if (return_y + retn_height> global_system.DefaultScreenHeight()) retn_height= global_system.DefaultScreenHeight() - return_y;
#ifdef WORDS_BIGENDIAN1
	if (return_tmpimage->byte_order == MSBFirst) {
		FitImage(&return_image_fit, &return_tmpimage);
		gdk_draw_image(GDK_DRAWABLE(return_screen2), gc, return_image_fit, 0, 0, return_x, return_y, retn_width, retn_height);
	} else {
		gdk_draw_image(GDK_DRAWABLE(return_screen2), gc, return_tmpimage, 0, 0, return_x, return_y, retn_width, retn_height);
	}
	g_object_unref(return_image_fit);
#else
	gdk_draw_image(GDK_DRAWABLE(return_screen2), gc, return_tmpimage, 0, 0, return_x, return_y, retn_width, retn_height);
#endif
	global_system.FlushScreen();
	/* return_screen1 はマウスカーソルあり */
	if (cursor) cursor->DrawImageRelative(return_tmpimage, -return_x, -return_y);
#ifdef WORDS_BIGENDIAN1
	if (return_tmpimage->byte_order == MSBFirst) {
		FitImage(&return_image_fit, &return_tmpimage);
		gdk_draw_image(GDK_DRAWABLE(return_screen1), gc, return_image_fit, 0, 0, return_x, return_y, retn_width, retn_height);
	} else {
		gdk_draw_image(GDK_DRAWABLE(return_screen1), gc, return_tmpimage, 0, 0, return_x, return_y, retn_width, retn_height);
	}
#else
	gdk_draw_image(GDK_DRAWABLE(return_screen1), gc, return_tmpimage, 0, 0, return_x, return_y, retn_width, retn_height);
#endif
	global_system.FlushScreen();
	cursor->RestoreImage();
	return_count++;
}

void SYSTEM_PDT_IMAGE::DrawReturnPixmap(GdkWindow* win, GdkDrawable* d1, GdkDrawable* d2, int x, int y, P_CURSOR* cursor)
{
	if (return_back != 0) DeleteReturnPixmap(cursor);
	int retn_width = return_cursor[CursorType()].retn_width;
	int retn_height = return_cursor[CursorType()].retn_height;
	/* 座標が不正な場合、修正 */
	if (x < 0) x=0;
	if (x >= global_system.DefaultScreenWidth()) x = global_system.DefaultScreenWidth()-1;
	if (y < 0) y=0;
	if (y >= global_system.DefaultScreenHeight()) y = global_system.DefaultScreenHeight()-1;
	if (x+retn_width >= global_system.DefaultScreenWidth()) retn_width = global_system.DefaultScreenWidth()-x;
	if (y+retn_height >= global_system.DefaultScreenHeight()) retn_height = global_system.DefaultScreenHeight()-y;
	/* return_back をキャプチャ */
	return_back = gdk_drawable_get_image(GDK_DRAWABLE(d2), x, y, retn_width, retn_height);
	/* x, y などのセット */
	return_x = x; return_y = y;
	return_screen1 = d1; return_screen2 = d2;
	g_object_ref(return_screen1);
	g_object_ref(return_screen2);
	/* cursor を描く */
	DrawReturnPixmap(cursor);
}

// 枠の中の塗りつぶし
#if 0 /* obsolete code */
static void DrawWakuIn15bpp(char* dat, int bpl, int width, int height, int c1, int c2, int c3) {
	int i,j;
	// まず、テーブルをつくる
	unsigned short c1_table[32]; for (i=0; i<32; i++) { c1_table[i] = (i*c1/255) << 10; }
	unsigned short c2_table[32]; for (i=0; i<32; i++) { c2_table[i] = (i*c2/255) << 5; }
	unsigned short c3_table[32]; for (i=0; i<32; i++) { c3_table[i] = i*c3/255; }
	// 色の分だけ暗くする
	for (i=0; i<height; i++) {
		unsigned short* line = (unsigned short*)dat;
		for (j=0; j<width; j++) {
			unsigned int c = *line;
			register int cc1 = c>>10;
			register int cc2 = c>>5;
			register int cc3 = c & 0x1f;
			cc1 &= 0x1f; cc2 &= 0x1f;
			*line++ = c1_table[cc1] | c2_table[cc2] | c3_table[cc3];
		}
		dat += bpl;
	}
	
}
#endif

static void DrawWakuIn16bpp(char* dat, int bpl, int width, int height, int c1, int c2, int c3) {
	int i,j;
	// まず、テーブルをつくる
	unsigned short c1_table[32]; for (i=0; i<32; i++) { c1_table[i] = (i*c1/255) << 11; }
	unsigned short c2_table[64]; for (i=0; i<64; i++) { c2_table[i] = (i*c2/255) << 5; }
	unsigned short c3_table[32]; for (i=0; i<32; i++) { c3_table[i] = i*c3/255; }
	// 色の分だけ暗くする
	for (i=0; i<height; i++) {
		unsigned short* line = (unsigned short*)dat;
		for (j=0; j<width; j++) {
			unsigned int c = *line;
			register int cc1 = c>>11;
			register int cc2 = c>>5;
			register int cc3 = c & 0x1f;
			cc1 &= 0x1f; cc2 &= 0x3f;
			*line++ = c1_table[cc1] | c2_table[cc2] | c3_table[cc3];
		}
		dat += bpl;
	}
	
}

#if 0 /* obsolete code */
static void DrawWakuIn24bpp(char* dat, int bpl, int width, int height, int c1, int c2, int c3) {
	int i,j;
	// まず、テーブルをつくる
	unsigned char c1_table[256]; for (i=0; i<256; i++) { c1_table[i] = i*c1/255; }
	unsigned char c2_table[256]; for (i=0; i<256; i++) { c2_table[i] = i*c2/255; }
	unsigned char c3_table[256]; for (i=0; i<256; i++) { c3_table[i] = i*c3/255; }
	// 色の分だけ暗くする
	for (i=0; i<height; i++) {
		unsigned char* line = (unsigned char*)dat;
		for (j=0; j<width; j++) {
			*line = c3_table[*line]; line++;
			*line = c2_table[*line]; line++;
			*line = c1_table[*line]; line++;
		}
		dat += bpl;
	}
	
}
#endif /* obsolete */

static void DrawWakuIn32bpp(char* dat, int bpl, int width, int height, int c1, int c2, int c3) {
	int i,j;
	// まず、テーブルをつくる
	unsigned char c1_table[256]; for (i=0; i<256; i++) { c1_table[i] = i*c1/255; }
	unsigned char c2_table[256]; for (i=0; i<256; i++) { c2_table[i] = i*c2/255; }
	unsigned char c3_table[256]; for (i=0; i<256; i++) { c3_table[i] = i*c3/255; }
	// 色の分だけ暗くする
	for (i=0; i<height; i++) {
		unsigned char* line = (unsigned char*)dat;
		for (j=0; j<width; j++) {
			*line = c3_table[*line]; line++;
			*line = c2_table[*line]; line++;
			*line = c1_table[*line]; line++;
			*line++ = 0;
		}
		dat += bpl;
	}
	
}


#if 0 /* obsolete code */
static void DeleteWakuIn15bpp(char* dat, int bpl, int width, int height, int c1, int c2, int c3) {
	// 色をつくる
	c1 <<= 7; c1 &= 0x7c00;
	c2 <<= 3; c2 &= 0x03e0;
	c3 &= 0x1f;
	unsigned short c = c1 | c2 | c3;
	// 塗る
	int i,j;
	for (i=0; i<height; i++) {
		unsigned short* line = (unsigned short*)dat;
		for (j=0; j<width; j++) {
			*line++ = c;
		}
		dat += bpl;
	}
	return;
}
#endif /* obsolete */

static void DeleteWakuIn16bpp(char* dat, int bpl, int width, int height, int c1, int c2, int c3) {
	// 色をつくる
	c1 <<= 8; c1 &= 0xf800;
	c2 <<= 3; c2 &= 0x07e0;
	c3 &= 0x1f;
	unsigned short c = c1 | c2 | c3;
	// 塗る
	int i,j;
	for (i=0; i<height; i++) {
		unsigned short* line = (unsigned short*)dat;
		for (j=0; j<width; j++) {
			*line++ = c;
		}
		dat += bpl;
	}
	return;
}

#if 0 /* obsolete code */
static void DeleteWakuIn24bpp(char* dat, int bpl, int width, int height, int c1, int c2, int c3) {
	// 色をつくる
	unsigned int c = c1; unsigned char* cc = (unsigned char*)(&c);
	cc[0] = c1; cc[1] = c2; cc[2] = c3; cc[3] = 0;
	// 塗る
	int i,j;
	for (i=0; i<height; i++) {
		char* line = dat;
		for (j=0; j<width; j++) {
			*line++ = c3; *line++ = c2; *line++ = c1;
		}
		dat += bpl;
	}
	return;
}
#endif /* obsolete */

static void DeleteWakuIn32bpp(char* dat, int bpl, int width, int height, int c1, int c2, int c3) {
	// 色をつくる
	unsigned int c = c1; unsigned char* cc = (unsigned char*)(&c);
	cc[0] = c3; cc[1] = c2; cc[2] = c1; cc[3] = 0;
	// 塗る
	int i,j;
	for (i=0; i<height; i++) {
		unsigned int* line = (unsigned int*)dat;
		for (j=0; j<width; j++) {
			*line++ = c;
		}
		dat += bpl;
	}
	return;
}

void SYSTEM_IMAGE::SetBrightness(GdkImage* im,int bright, AyuSys& local_system) {
	if (bright < 0) bright = 0;
	if (bright > 100) bright = 100;
	bright = bright * 255 / 100;
	// 画面を暗くする
	if (bright != 255) {
		int bpl = im->bpl;
		char* dat = (char*)im->mem;
		int width = global_system.DefaultScreenWidth()-8; int height = global_system.DefaultScreenHeight()-8;
		int c1=bright, c2=bright, c3=bright;
#if 0
		if (local_system.DefaultBpp() == 15) {
			DrawWakuIn15bpp(dat, bpl, width+8, height+8, c1, c2, c3);
		} else if (local_system.DefaultBpp() == 16) {
			DrawWakuIn16bpp(dat, bpl, width+8, height+8, c1, c2, c3);
		} else if (local_system.DefaultBpp() == 24) {
			DrawWakuIn24bpp(dat, bpl, width+8, height+8, c1, c2, c3);
		} else if (local_system.DefaultBpp() == 32) {
			DrawWakuIn32bpp(dat, bpl, width+8, height+8, c1, c2, c3);
		}
#endif
		if (local_system.DefaultBypp() == 2) {
			DrawWakuIn16bpp(dat, bpl, width+8, height+8, c1, c2, c3);
		} else {
			DrawWakuIn32bpp(dat, bpl, width+8, height+8, c1, c2, c3);
		}
	}
}

void SYSTEM_IMAGE::DrawWaku(GdkImage* im, int x, int y, int width, int height, AyuSys& local_system)
{
	/* 色、透明フラグを得る */
	int c1, c2, c3, trans;
	local_system.config->GetParam("#WINDOW_ATTR", 3, &c1,&c2,&c3);
//	local_system.config->GetParam("#WINDOW_ATTR_TYPE", 1, &trans);
trans=0;
	if (c1<0) c1=0; if (c1>255) c1=255;
	if (c2<0) c2=0; if (c2>255) c2=255;
	if (c3<0) c3=0; if (c3>255) c3=255;
	/* 枠のマージンを得る */
	int mx1,my1,mx2,my2;
	if (local_system.config->GetParaInt("#NVL_SYSTEM") == 0) {
		local_system.config->GetParam("#WINDOW_ATTR_AREA", 4, &mx1,&my1,&mx2,&my2);
	} else {
		/* 全画面テキストモード */
		mx1 = 0; my1 = 0; mx2 = 0; my2 = 0;
		x = 0; y = 0; width = global_system.DefaultScreenWidth(); height = global_system.DefaultScreenHeight();
	}
	if (mx1<0) mx1=0;
	if (my1<0) my1=0;
	if (mx2<0) mx2=0;
	if (my2<0) my2=0;
	if (mx1+mx2 >= width || my1+my2 >= height) return; /* 無効な値 */
	// 枠の描画部分を(x,y),(width,height)から取り除く
	x += mx1; y += my1; width -= mx1+mx2; height -= my1+my2;
	if (x+width > global_system.DefaultScreenWidth()) width=global_system.DefaultScreenWidth()-x;
	if (y+height> global_system.DefaultScreenHeight()) height=global_system.DefaultScreenHeight()-y;
	// 枠の中をぬりつぶす
	// bpp と、塗りつぶしか透明化で区別
	int bpl = im->bpl; int bypp = bpl / im->width;
	char* dat = (char*)im->mem;
	dat += bpl*y + bypp*x;
#if 0
	if (trans == 0) {
		if (local_system.DefaultBpp() == 15) {
			if (! local_system.DefaultRgbrev())
				DrawWakuIn15bpp(dat, bpl, width, height, c1, c2, c3);
			else
				DrawWakuIn15bpp(dat, bpl, width, height, c3, c2, c1);
		} else if (local_system.DefaultBpp() == 16) {
			if (! local_system.DefaultRgbrev())
				DrawWakuIn16bpp(dat, bpl, width, height, c1, c2, c3);
			else
				DrawWakuIn16bpp(dat, bpl, width, height, c3, c2, c1);
		} else if (local_system.DefaultBpp() == 24) {
			if (! local_system.DefaultRgbrev())
				DrawWakuIn24bpp(dat, bpl, width, height, c1, c2, c3);
			else
				DrawWakuIn24bpp(dat, bpl, width, height, c3, c2, c1);
		} else if (local_system.DefaultBpp() == 32) {
			if (! local_system.DefaultRgbrev())
				DrawWakuIn32bpp(dat, bpl, width, height, c1, c2, c3);
			else
				DrawWakuIn32bpp(dat, bpl, width, height, c3, c2, c1);
		}
	} else {
		if (local_system.DefaultBpp() == 15) {
			if (! local_system.DefaultRgbrev())
				DeleteWakuIn15bpp(dat, bpl, width, height, c1, c2, c3);
			else
				DeleteWakuIn15bpp(dat, bpl, width, height, c3, c2, c1);
		} else if (local_system.DefaultBpp() == 16) {
			if (! local_system.DefaultRgbrev())
				DeleteWakuIn16bpp(dat, bpl, width, height, c1, c2, c3);
			else
				DeleteWakuIn16bpp(dat, bpl, width, height, c3, c2, c1);
		} else if (local_system.DefaultBpp() == 24) {
			if (! local_system.DefaultRgbrev())
				DeleteWakuIn24bpp(dat, bpl, width, height, c1, c2, c3);
			else
				DeleteWakuIn24bpp(dat, bpl, width, height, c3, c2, c1);
		} else if (local_system.DefaultBpp() == 32) {
			if (! local_system.DefaultRgbrev())
				DeleteWakuIn32bpp(dat, bpl, width, height, c1, c2, c3);
			else
				DeleteWakuIn32bpp(dat, bpl, width, height, c3, c2, c1);
		}
	}
#endif
	if (trans == 0) {
		if (local_system.DefaultBypp() == 2) {
			DrawWakuIn16bpp(dat, bpl, width, height, c1, c2, c3);
		} else {
			DrawWakuIn32bpp(dat, bpl, width, height, c1, c2, c3);
		}
	} else {
		if (local_system.DefaultBypp() == 2) {
			DeleteWakuIn16bpp(dat, bpl, width, height, c1, c2, c3);
		} else {
			DeleteWakuIn32bpp(dat, bpl, width, height, c1, c2, c3);
		}
	}
}

void SYSTEM_PDT_IMAGE::DrawWaku(GdkImage* im, int x, int y, int width, int height, AyuSys& local_system)
{
	/* 枠の内側を塗る */
	SYSTEM_IMAGE::DrawWaku(im,x,y,width,height,local_system);
	/* 枠を書く */
	if (local_system.config->GetParaInt("#NVL_SYSTEM") == 0) {
		int type = local_system.config->GetParaInt("#WINDOW_WAKU_TYPE");
		if (type < 0 || type > 3) type = 0;
		waku[type]->Draw(im,x,y,width,height);
	}
}

// マウスカーソルのセット
P_CURSOR* SYSTEM_PDT_IMAGE::CreateCursor(GdkWindow* window, GdkPixmap* background) {
	// まず、マウスカーソルを消す
	GdkColor black = {0,0,0,0};
	GdkColor white = {0,0xffff,0xffff,0xffff};

	gchar buf = 0;
	GdkBitmap* bitmap = gdk_bitmap_create_from_data(window, &buf, 1, 1);
	GdkCursor* cursor = gdk_cursor_new_from_pixmap(
		bitmap, bitmap, &white, &black, 0, 0);
	gdk_window_set_cursor(window, cursor);
	g_object_unref(bitmap);
	return new PIX_CURSOR(window, mouse_pixmap, mouse_bitmap, mouse_image_mask, background);
}
