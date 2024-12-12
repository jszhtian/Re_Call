/*  window.cc
 *     メイン・ウィンドウの定義
 *     ウィンドウのイベント処理に関連するメソッド
 *     また、他に入れにくいメソッドもここにある。
 *     そのほか、AyuSys クラスのインターフェースなど
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

#include <gtk--/main.h>
#include <gtk--/window.h>
#include <gtk--/button.h>
#include <gtk--/pixmap.h>
#include <gtk--/box.h>
#include <gtk--/packer.h>
#include <gtk--/menu.h>
#include <gtk--/label.h>
#include <gtk--/eventbox.h>
#include <gtk--/fixed.h>
#include <glib.h>
#include <sys/time.h>
#include <unistd.h>
#include <gdk/gdkx.h>
#include <gdk/gdkkeysyms.h>
#include "window.h"
#include "image_pdt.h"
#include "image_di_record.h"

void AyuWindow::DisplaySync(void)
{
//	XSync( ( (GdkWindowPrivate*)main.get_window().gdkobj() )->xdisplay,False);
	gdk_flush(); /* XSync() と同じ */
}

void AyuWindow::TranslateImage(int x1, int y1, int x2, int y2) {
	if (! is_translation_required) return;
	if (image == image_with_text) return;
	int dbpl = image->gdkobj()->bpl;
	int dbypp = dbpl / image->gdkobj()->width;
	GdkVisual* vis = image->gdkobj()->visual;
	char* dest = ((char*)image->gdkobj()->mem) + dbpl*y1 + dbypp*x1;
	DI_Image* src_im = local_system.ScreenImage();
	int sbpl = src_im->bpl;
	int sbypp = src_im->bypp;
	char* src = src_im->data + src_im->bpl*y1 + src_im->bypp*x1;
	int i,j; int width = x2-x1+1; int height = y2-y1+1;
	int sysbpp = local_system.DefaultBypp();
	for (i=0; i<height; i++) {
		char* d = dest;
		unsigned char* s = (unsigned char*)src;
		if (sysbpp == 2) {
			if (vis->depth > 24) {
				for (j=0; j<width; j++) {
					unsigned int sp = *(unsigned short*)s;
					unsigned int pixel = r_table[sp>>11] | g_table[(sp>>5)&0x3f] | b_table[sp&0x1f];
					d[0] = pixel; d[1] = pixel>>8; d[2] = pixel>>16; d[3] = pixel>>24;
					d += dbypp; s += sbypp;
				}
			} else if (vis->depth > 16) {
				for (j=0; j<width; j++) {
					unsigned int sp = *(unsigned short*)s;
					unsigned int pixel = r_table[sp>>11] | g_table[(sp>>5)&0x3f] | b_table[sp&0x1f];
					d[0] = pixel; d[1] = pixel>>8; d[2] = pixel>>16;
					d += dbypp; s += sbypp;
				}
			} else if (vis->depth > 8) {
				for (j=0; j<width; j++) {
					unsigned int sp = *(unsigned short*)s;
					unsigned int pixel = r_table[sp>>11] | g_table[(sp>>5)&0x3f] | b_table[sp&0x1f];
					d[0] = pixel; d[1] = pixel>>8;
					d += dbypp; s += sbypp;
				}
			} else {
				for (j=0; j<width; j++) {
					unsigned int sp = *(unsigned short*)s;
					unsigned int pixel = r_table[sp>>11] | g_table[(sp>>5)&0x3f] | b_table[sp&0x1f];
					d[0] = pixel;
					d += dbypp; s += sbypp;
				}
			}
		} else {
			if (vis->depth > 24) {
				for (j=0; j<width; j++) {
					unsigned int pixel = r_table[s[2]] | g_table[s[1]] | b_table[s[0]];
					d[0] = pixel; d[1] = pixel>>8; d[2] = pixel>>16; d[3] = pixel>>24;
					d += dbypp; s += sbypp;
				}
			} else if (vis->depth > 16) {
				for (j=0; j<width; j++) {
					unsigned int pixel = r_table[s[2]] | g_table[s[1]] | b_table[s[0]];
					d[0] = pixel; d[1] = pixel>>8; d[2] = pixel>>16;
					d += dbypp; s += sbypp;
				}
			} else if (vis->depth > 8) {
				for (j=0; j<width; j++) {
					unsigned int pixel = r_table[s[2]] | g_table[s[1]] | b_table[s[0]];
					d[0] = pixel; d[1] = pixel>>8;
					d += dbypp; s += sbypp;
				}
			} else {
				for (j=0; j<width; j++) {
					unsigned int pixel = r_table[s[2]] | g_table[s[1]] | b_table[s[0]];
					d[0] = pixel;
					d += dbypp; s += sbypp;
				}
			}
		}
		dest += dbpl;
		src += sbpl;
	}
}

void AyuWindow::Draw(void) {
	if (local_system.ScreenImage()->IsChangedRegionAll())
		TranslateImage(0, 0, local_system.DefaultScreenWidth()-1, local_system.DefaultScreenHeight()-1);
	else {
		int i; int x,y,w,h;
		for (i=0; local_system.ScreenImage()->GetChangedRegion(0, i, x, y, w, h); i++) {
			TranslateImage(x, y, x+w-1, y+h-1);
		}
	}
	if (not_synced_flag == 2) DisplaySync(); /* pixmap への描画が起きたので image 変更前に DisplaySync() */
	cursor->DrawImage(image);
	if (local_system.ScreenImage()->IsChangedRegionAll()) {
		main.get_window().draw_image(gc, *image, 0,0, 0,0, local_system.DefaultScreenWidth(), local_system.DefaultScreenHeight());
	} else {
		int i; int x,y,w,h;
		for (i=0; local_system.ScreenImage()->GetChangedRegion(0, i, x, y, w, h); i++) {
			main.get_window().draw_image(gc, *image,x, y, x, y, w, h);
		}
	}
	DisplaySync();
	cursor->RestoreImage();
	if (image == image_without_text)
		local_system.ScreenImage()->ClearChangedRegion(0);
	else if (di_image_text) /* icon 描画用のアドホック */
		di_image_text->ClearChangedRegion(0);
	redraw_x=0; redraw_y=0; redraw_width=local_system.DefaultScreenWidth(); redraw_height=local_system.DefaultScreenHeight();
	not_synced_flag = 1;
}

// 指定された領域の画像をコピー
void AyuWindow::Draw(int x1, int y1, int x2, int y2) {
	fix_axis(0,x1,x2,local_system.DefaultScreenWidth()-1);
	fix_axis(0,y1,y2,local_system.DefaultScreenHeight()-1);
	TranslateImage(x1, y1, x2, y2);
	int width = x2-x1+1; int height = y2-y1+1;

	if (not_synced_flag == 1) {
		/* image -> pixmap の転送が起きてない */
		/* redraw_x,y の再構成 */
		int redraw_x2 = redraw_x + redraw_width - 1;
		int redraw_y2 = redraw_y + redraw_height - 1;
		if (redraw_x > x1) redraw_x = x1;
		if (redraw_y > y1) redraw_y = y1;
		if (redraw_x2< x2) redraw_x2= x2;
		if (redraw_y2< y2) redraw_y2= y2;
		redraw_width = redraw_x2 - redraw_x + 1;
		redraw_height= redraw_y2 - redraw_y + 1;
	} else {
		/* redraw_x,y の設定 */
		redraw_x=x1; redraw_y=y1; redraw_width=width; redraw_height=height;
	}
	if (not_synced_flag == 2) DisplaySync(); /* pixmap への描画が起きたので image 変更前に DisplaySync() */
	cursor->DrawImage(image);
	main.get_window().draw_image(gc, *image, x1,y1, x1,y1, width,height);
	DisplaySync();
	cursor->RestoreImage();
	not_synced_flag = 1;
}

// 画面の位置をずらす
// (x,y)->(0,0)
void AyuWindow::Shake(int x, int y) {
	DrawTextEnd(1);
	// いらない部分は黒で塗りつぶす
	if (x>0) { main.get_window().draw_rectangle(text_gc2, 1, local_system.DefaultScreenWidth()-x, 0, local_system.DefaultScreenWidth(), local_system.DefaultScreenHeight());
	} else { main.get_window().draw_rectangle(text_gc2, 1, 0, 0, -x, local_system.DefaultScreenHeight());
	}
	if (y>0) { main.get_window().draw_rectangle(text_gc2, 1, 0, local_system.DefaultScreenHeight()-y, local_system.DefaultScreenWidth(), local_system.DefaultScreenHeight());
	} else { main.get_window().draw_rectangle(text_gc2, 1, 0, 0, local_system.DefaultScreenWidth(), -y);
	}
	// コピー
	main.get_window().draw_pixmap(gc, *pix_image, x, y, 0, 0, local_system.DefaultScreenWidth(), local_system.DefaultScreenHeight());
	return;
}

// 画面を瞬かせる
void AyuWindow::BlinkWindow(int c1, int c2, int c3, int wait_time, int count) {
	c1 &= 0xff; c2 &= 0xff; c3 &= 0xff;
	char color_name[50]; sprintf(color_name, "#%02x%02x%02x",c1,c2,c3);
	DrawTextEnd(1);
	// GC をつくる
	Gdk_GC blink_gc(main_window);
	Gdk_Color blink_color(color_name);
	Gdk_Colormap::get_system().alloc(blink_color);
	blink_gc.set_foreground(blink_color);

	void* timer = local_system.setTimerBase(); int now_time = 0;
	int i; for (i=0; i<count; i++) {
		main_window.draw_rectangle(blink_gc, TRUE, 0, 0, local_system.DefaultScreenWidth(), local_system.DefaultScreenHeight());
		local_system.waitUntil(timer, now_time + wait_time/2);
		main_window.draw_pixmap(blink_gc, *pix_image, 0, 0, 0, 0);
		now_time += wait_time;
		local_system.waitUntil(timer, now_time);
	}
	local_system.freeTimerBase(timer);
}

// window is top level.

void QuitAll(void);

AyuWindow::AyuWindow(AyuSys& sys, char* _font) : Gtk::Window_AllScreen(GTK_WINDOW_TOPLEVEL),
	main(),
	local_system(sys)
	{

	menu_window = 0;

	fontname = 0; ttfontname = 0; fontsize = 20;
	if (_font == 0) { 
		default_fontname = "-*-*-*-*-*--26-*-*-*-*-*-jisx0208.1983-*,-*-*-*-r-*--24-*-*-*-*-*-iso8859-*";
	} else {
		default_fontname = new char[strlen(_font)+1];
		strcpy(default_fontname, _font);
	}
	add(main_vbox); main_vbox.show();
	// make menu bar
	CreateMenu();
	// add main window
	main.set_usize(local_system.DefaultScreenWidth(),local_system.DefaultScreenHeight());
	main_vbox.pack_end(main); main.show();
	pix = 0; image = 0; expose_flag = 0; not_synced_flag = 0;
	redraw_x=0; redraw_y=0; redraw_width=local_system.DefaultScreenWidth(); redraw_height=local_system.DefaultScreenHeight();
	font =0; white_color = 0; black_color = 0;
	is_translation_required = false;
	return_cursor_viewed = 0; return_cursor_type = 0; is_initialized = 0;
	is_all_screen = 0; select_pix = 0;
	image_temporary_text = 0; font_pix_image = 0;
	mouse_timer = 0; retn_timer = 0;
	di_image_text = 0; di_image_icon = 0; di_image_icon_back = 0;
	twinfo = 0;

	main.motion_notify_event.connect(SigC::slot(this, &AyuWindow::motionNotify));
	main.leave_notify_event.connect(SigC::slot(this, &AyuWindow::leaveNotify));
	main.enter_notify_event.connect(SigC::slot(this, &AyuWindow::enterNotify));
	main.expose_event.connect(SigC::slot(this, &AyuWindow::exposeEvent));
	main.button_press_event.connect(SigC::slot(this, &AyuWindow::buttonEvent));
	main.button_release_event.connect(SigC::slot(this, &AyuWindow::buttonEvent));
	key_press_event.connect(SigC::slot(this, &AyuWindow::keyEvent));
	key_release_event.connect(SigC::slot(this, &AyuWindow::keyEvent));
	focus_in_event.connect(SigC::slot(this, &AyuWindow::focusEvent));
	focus_out_event.connect(SigC::slot(this, &AyuWindow::focusEvent));
        destroy.connect(SigC::slot(this, &AyuWindow::Destroy));

	main.set_events( main.get_events() | GDK_EXPOSURE_MASK |
		GDK_ENTER_NOTIFY_MASK | GDK_LEAVE_NOTIFY_MASK |
		// GDK_KEY_PRESS_MASK | GDK_KEY_RELEASE_MASK | GDK_FOCUS_CHANGE_MASK |
		GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK | //GDK_POINTER_MOTION_HINT_MASK |
		GDK_POINTER_MOTION_MASK);
	set_events( get_events() |
		GDK_KEY_PRESS_MASK | GDK_KEY_RELEASE_MASK | GDK_FOCUS_CHANGE_MASK
	);
	show();
}
AyuWindow::~AyuWindow() {
	g_object_unref(pix);
	g_object_unref(main);
	g_object_unref(main_vbox);

	g_object_unref(select_pix);
	g_object_unref(gc);
	g_object_unref(text_gc1);
	g_object_unref(text_gc2);
	g_object_unref(text_gc3);
	g_object_unref(font);
	g_object_unref(font_pix_image);

	g_object_unref(image);
	g_object_unref(image_with_text);
	g_object_unref(image_without_text);
	g_object_unref(image_temporary_text);
	g_object_unref(pix_image);

??	free? black, white

	GdkGC* gc;
	GdkGC* text_gc1; GdkGC* text_gc2; int text_color, text_backcolor;
	GdkGC* text_gc3; int text3_color;
	GdkFont* font;
#if WITH_FREETYPE
	GdkFont_FreeType *font_pix_image;
#else /* WITH_FREETYPE */
#if WITH_FREETYPE
	GdkPixmap_FreeType* select_pix;
#else /* WITH_FREETYPE */
	GdkPixmap* select_pix;

	if (select_pix) delete select_pix;
	if (pix_image) delete pix_image;
	if (image_without_text) delete image_without_text;
	if (image_with_text) delete image_with_text;
	if (sys_im) delete sys_im;
	if (font) delete font;
	if (black_color) delete black_color;
	if (white_color) delete white_color;
}

PartWindow::PartWindow(class AyuWindow* _p, int _x0, int _y0, int w, int h) :
	x0(_x0), y0(_y0), parent(_p) {
	wid = gtk_drawingarea_new();
	gc = 0;
	is_realized = false;
	gtk_widget_set_size_request(GTK_WIDGET(window), w, h);
	g_signal_connect(G_OBJECT(wid), "map_notify", G_CALLBACK(mapEvent), gpointer(this));
	g_signal_connect(G_OBJECT(wid), "expose", G_CALLBACK(exposeEvent), gpointer(this));
	gtk_widget_set_events(GTK_WIDGET(window), GDK_EXPOSURE_MASK | GDK_STRUCTURE_MASK);
}
PartWindow::~PartWindow() {
	if (gc) g_object_unref(gc);
	g_object_unref(window);
}

int AyuWindow::MakePartWindow(int x, int y, int w, int h) {
	PartWindow* p = new PartWindow(this, x, y, w, h);
	gtk_fixed_put(main, p->wid, x, y);
	gtk_widget_show(p->wid);
	parts.push_back(p);
	/* 表示されるまで待つ */
	while(p->WindowID() == -1) {
		local_system.CallProcessMessages();
	}
	return p->WindowID();
}
void AyuWindow::DeletePartWindow(int id) {
	std::vector<PartWindow*>::iterator it;
	for (it = parts.begin(); it != parts.end(); it++) {
		if (id == (*it)->WindowID()) {
			gtk_widget_hide( (*it)->wid);
			parts.erase(it);
			delete (*it);
			break;
		}
	}
	return;
}
int PartWindow::WindowID(void) {
	if (is_realized) return GDK_WINDOW_XWINDOW(wid->window);
	else return -1;
}
gint PartWindow::mapEvent(GdkEventAny* p1, gpointer pointer) {
	cur = (PartWindow*)pointer;
	if (! is_realized) {
		cur->gc = gdk_gc_new(GDK_DRAWABLE(cur->wid->window));
	}
	is_realized = true;
	return FALSE;
}

gint AyuWindow::configure_event_impl(GdkEventConfigure* p1) {
	if (! is_initialized) {
		/* 初期化 */
		main_window = main.get_window();
		Gdk_Window top_window = this->get_window();
		Gdk_Bitmap* gb = ::new Gdk_Bitmap();
#if WITH_FREETYPE
		pix_image = ::new Gdk_Pixmap_FreeType(top_window, local_system.DefaultScreenWidth(), local_system.DefaultScreenHeight(), -1);
#else /* WITH_FREETYPE */
		pix_image = ::new Gdk_Pixmap(top_window, local_system.DefaultScreenWidth(), local_system.DefaultScreenHeight(), -1);
#endif /* WITH_FREETYPE */
		/* fill pixmap with black */
		Gdk_GC    black_gc (*pix_image);
		Gdk_Color black (0);
		black_gc.set_foreground (black);
		pix_image->draw_rectangle (black_gc, TRUE, 0, 0, local_system.DefaultScreenWidth(), local_system.DefaultScreenHeight());

		Gdk_Visual visual = top_window.get_visual();
		if (visual.gdkobj()->type != GDK_VISUAL_TRUE_COLOR &&
			visual.gdkobj()->type != GDK_VISUAL_DIRECT_COLOR) {
			fprintf(stderr, "This software only supports true / direct color display.\n");
			local_system.Finalize();
			return FALSE;
		}
		/* visual の判別 */
		if (visual.gdkobj()->depth > 16) local_system.SetDefaultBypp_32();
		else local_system.SetDefaultBypp_565();
		if (local_system.DefaultBypp() == 4) {
			/* 32bpp の内部イメージ形式 == GdkImage かの確認 */
			if (visual.gdkobj()->red_mask   == 0xff0000 &&
			    visual.gdkobj()->green_mask == 0x00ff00 &&
			    visual.gdkobj()->blue_mask  == 0x0000ff) is_translation_required = false;
			else is_translation_required = true;
		} else {
			/* 16bpp の内部イメージ形式 == GdkImage かの確認 */
			if (visual.gdkobj()->red_mask   == 0xf800 &&
			    visual.gdkobj()->green_mask == 0x07e0 &&
			    visual.gdkobj()->blue_mask  == 0x001f) is_translation_required = false;
			else is_translation_required = true;
		}
		/* colormap の取得 */
		if (is_translation_required) {
			int i;
			Gdk_Color c; Gdk_Colormap map = get_colormap();
			if (local_system.DefaultBypp() == 4) {
				for (i=0; i<256; i++) {
					c.set_rgb(i*257, 0, 0);
					map.alloc(c);
					r_table[i] = c.get_pixel();
					c.set_rgb(0, i*257, 0);
					map.alloc(c);
					g_table[i] = c.get_pixel();
					c.set_rgb(0, 0, i*257);
					map.alloc(c);
					b_table[i] = c.get_pixel();
				}
			} else {
				for (i=0; i<32; i++) {
					c.set_rgb((i*33*1025)/16, 0, 0);
					map.alloc(c);
					r_table[i] = c.get_pixel();
					c.set_rgb(0, 0, (i*33*1025)/16);
					map.alloc(c);
					b_table[i] = c.get_pixel();
				}
				for (i=0; i<64; i++) {
					c.set_rgb(0, (i*65*4097)/256, 0);
					map.alloc(c);
					g_table[i] = c.get_pixel();
				}
			}
		}

		/* イメージ転送用の Gdk_Image の作成 */
		image_without_text = ::new Gdk_Image(GDK_IMAGE_FASTEST, visual, local_system.DefaultScreenWidth(), local_system.DefaultScreenHeight());
		if (image_without_text->gdkobj() == 0) {
			delete image_without_text;
			image_without_text = ::new Gdk_Image(GDK_IMAGE_NORMAL, visual, local_system.DefaultScreenWidth(), local_system.DefaultScreenHeight());
		}
		image_with_text = ::new Gdk_Image(GDK_IMAGE_FASTEST, visual, local_system.DefaultScreenWidth(), local_system.DefaultScreenHeight());
		if (image_with_text->gdkobj() == 0) {
			delete image_with_text;
			image_with_text = ::new Gdk_Image(GDK_IMAGE_NORMAL, visual, local_system.DefaultScreenWidth(), local_system.DefaultScreenHeight());
		}
		image = image_without_text;

		gc = Gdk_GC(top_window);
		pix = new Gtk::Pixmap(*pix_image,*gb);
		pix->show();
		main.put(*pix,0,0);
		not_synced_flag = 0;
		sys_im = new SYSTEM_PDT_IMAGE(local_system.config->GetParaStr("#WAKUPDT"));
		if (sys_im->Init(top_window, local_system) == false) {
			delete sys_im;
			sys_im = new SYSTEM_IMAGE();
		}
		cursor = sys_im->CreateCursor(&main_window, pix_image);
		get_pointer(mouse_x, mouse_y);
		mouse_clicked = 0;
		mouse_key = 0;
		mouse_drawed = 0;
		InitText();
		// システムにウィンドウをセット
		local_system.InitWindow(this);
		// テキストウィンドウにボタンを表示するために DI_Image をつくる
		if (!is_translation_required) {
			di_image_text = new DI_Image();
			di_image_text->SetImage(image_with_text);
			di_image_icon = CreateIcon(local_system);
			di_image_icon_back = new DI_Image();
			di_image_icon_back->CreateImage(di_image_icon->width, di_image_icon->height, di_image_icon->bypp);
			icon_state = ICON_NODRAW;
		}
		LoadGaijiTable();
		is_initialized = 1;
		DrawMouse();
	}
	return FALSE;
}

void AyuWindow::DrawImage(Gdk_Image* image, SEL_STRUCT* sel)
{
	/* sel で指定された領域を、指定された方法で main window に描画 */
	DeleteTextWindow();

	int srcx = sel->x1; int srcy = sel->y2;
	int destx = sel->x3; int desty = sel->y3;
	int width = sel->x1 - sel->x2; if (width<0) width = -width; width++;
	int height = sel->y1 - sel->y2; if (height<0) height = -height; height++;
	
	/* pixmap にコピー */
	pix_image->draw_image(gc, *image, srcx, srcy, destx, desty, width, height);
	DisplaySync();
	/* マウスカーソルを書いた状態で、画面に表示 */
	cursor->DrawImage(image);
	main.get_window().draw_image(gc, *image, srcx, srcy, destx, desty, width, height);
	DisplaySync();
	cursor->RestoreImage();
	
}

void AyuWindow::SyncPixmap(void) {
	if (not_synced_flag == 1) {
		pix_image->draw_image( gc, *image, redraw_x, redraw_y, redraw_x, redraw_y, redraw_width, redraw_height);
		cursor->UpdateBuffer();
		/* image の変更前に DisplaySync() を呼び出す必要あり */
		not_synced_flag = 2;
	}
}

void AyuWindow::DrawMouse(void) {
	if (mouse_drawed) return;
	mouse_drawed = 1;
	mouse_key = 0;
	SyncPixmap();
	if (mouse_x != DELETED_MOUSE_X)
		cursor->Draw(mouse_x-local_system.MouseCursorX(), mouse_y-local_system.MouseCursorY());
}

void AyuWindow::DeleteMouse(void) {
	if (!mouse_drawed) return;
	mouse_drawed = 0;
	mouse_key = 0;
	SyncPixmap();
	cursor->Delete();
}

gint AyuWindow::motionNotify(GdkEventMotion* event) {
	if (!is_initialized) return FALSE;
	mouse_x = int(event->x); mouse_y = int(event->y);
	mouse_key = 0;
	CheckIconRegion();
	if (! mouse_drawed) return TRUE;
	SyncPixmap();
	cursor->Draw( int(event->x-local_system.MouseCursorX()), int(event->y-local_system.MouseCursorY()));
	return TRUE;
}

gint AyuWindow::nullNotify(GdkEventCrossing* event) {
	return FALSE;
}

gint AyuWindow::enterNotify(GdkEventCrossing* event) {
	if (!is_initialized) return FALSE;
	mouse_clicked &= ~(8|16);
	mouse_key = 0;
	mouse_x = int(event->x); mouse_y = int(event->y);
	CheckIconRegion();
	if (! mouse_drawed) return TRUE;
	SyncPixmap();
	cursor->Draw( int(event->x),int(event->y-32) );
	return TRUE;
}

gint AyuWindow::leaveNotify(GdkEventCrossing* event) {
	if (!is_initialized) return FALSE;
//	DisableDGA();
	mouse_clicked &= ~(8|16);
	mouse_x = DELETED_MOUSE_X; mouse_y = DELETED_MOUSE_X;
	CheckIconRegion();
	mouse_key = 0;
	SyncPixmap();
	cursor->Delete();
	return TRUE;
}

gint AyuWindow::exposeEvent(GtkWidget* widget, GdkEventExpose* event) {
	if (!is_initialized) return FALSE;
	if (expose_flag) return FALSE;
	expose_flag = 1;
	mouse_key = 0;
	SyncPixmap();
	main.expose_event(event); // expose other windows
	expose_flag = 0;
	if (mouse_drawed) cursor->Draw();
	return TRUE;
}

gint PartWindow::exposeEvent(GtkWidget* widget, GdkEventExpose* event, gpointer pointer) {
	PartWindow* cur = (PartWindow*)pointer;
	int x = x0 + event->area.x;
	int y = y0 + event->area.y;
	int width = event->area.width;
	int height = event->area.height;
	
	gdk_draw_image(GDK_DRAWABLE(cur->wid->window), cur->gc, cur->parent->image, x, y, x, y, width, height);
	return TRUE;
}

gint AyuWindow::buttonEvent(GdkEventButton* event) {
	if (event->button == 1) {
		if (event->type == GDK_BUTTON_RELEASE) {
			mouse_clicked &= ~8;
		} else if (PressIconRegion()) {
			// icon が押されたならなにもしない
		} else {
			if (local_system.config->GetParaInt("#MSGBK_MOD")) { /* バックログボタンが有効 */
				int button_no = CheckBacklogButton();
				if (button_no == 1) {
					local_system.SetBacklog(2);
				}
				if (button_no == 1) return TRUE;
			}
			mouse_clicked |= 1 | 8;
			local_system.ClickEvent();
		}
	} else if (event->button == 3) {
		if (event->type == GDK_BUTTON_RELEASE) {
		} else { /* press */
			// mouse_clicked |= 2;
			DisableDGA();
			PopupMenu();
		}
	} else if (event->button == 2) { /* 中ボタンを普通のWindowsの右クリックと同等にする */
		if (event->type == GDK_BUTTON_PRESS) {
			mouse_clicked |= 2;
		}
	} else if (event->button == 4) {  /* ホイールの上 */
		if (event->type == GDK_BUTTON_PRESS) {
			mouse_clicked |= 2048;
		}
	} else if (event->button == 5) {  /* ホイールの下 */
		if (event->type == GDK_BUTTON_PRESS) {
			mouse_clicked |= 4096;
		}
	}
	return TRUE;
}

gint AyuWindow::keyEvent(GdkEventKey* event) {
	if (event->state & GDK_SHIFT_MASK) mouse_key |= 0x100;
	else mouse_key &= ~0x100;
	int key_bit = 0;
	switch(event->keyval) {
		case GDK_KP_Left: case GDK_KP_4: case GDK_Left: case GDK_h: case GDK_H: key_bit = 1; break;
		case GDK_KP_Right: case GDK_KP_6: case GDK_Right: case GDK_l: case GDK_L: key_bit = 2; break;
		case GDK_KP_Up: case GDK_KP_8: case GDK_Up: case GDK_k: case GDK_K: key_bit = 3; break;
		case GDK_KP_Down: case GDK_KP_2: case GDK_Down: case GDK_j: case GDK_J: key_bit = 4; break;
	}
	if (event->type == GDK_KEY_PRESS) {
		mouse_key |= 1 << (key_bit+8);
	} else if (event->type == GDK_KEY_RELEASE) {
		mouse_key &= ~(1 << (key_bit+8) );
	}
	// SHIFT で読み飛ばし
	if (
	    event->keyval == GDK_Shift_L ||
	    event->keyval == GDK_Shift_R ||
	//    event->keyval == GDK_Control_L ||
	0 ) {
		if (event->type == GDK_KEY_PRESS) mouse_clicked |= 4;
		else if (event->type == GDK_KEY_RELEASE) mouse_clicked &= ~4;

		if (event->type == GDK_KEY_PRESS) local_system.PressCtrl();
		else if (event->type == GDK_KEY_RELEASE) local_system.ReleaseCtrl();
	}

	// その他のキーが押された場合
	/* メニューのアクセラレーションキーはメニューから
	** 呼び出されるのでここではなし
	*/
	if (event->type == GDK_KEY_PRESS) {
		switch(event->keyval) {
		// return か、space なら click と同じ
		case GDK_Return:
		case GDK_KP_Enter:
			if (event->state & GDK_MOD1_MASK) { /* ALT + Return */
				break; /* 実際の処理は menu 側で行われる */
			}
		case GDK_space:
		case GDK_KP_Space:
			mouse_clicked |= 1;
			local_system.ClickEvent();
			break;
		case GDK_KP_Left:
		case GDK_KP_4:
		case GDK_Left:
		case GDK_h:
		case GDK_H:
			mouse_clicked |= 512;
			break;
		case GDK_KP_Right:
		case GDK_KP_6:
		case GDK_Right:
		case GDK_l:
		case GDK_L:
			mouse_clicked |= 1024;
			break;
		case GDK_KP_Up:
		case GDK_KP_8:
		case GDK_Up:
		case GDK_k:
		case GDK_K:
			mouse_clicked |= 32; break;
		case GDK_KP_Down:
		case GDK_KP_2:
		case GDK_Down:
		case GDK_j:
		case GDK_J:
			mouse_clicked |= 64; break;

		/* CTRL/ALT+X の類の CTRL キーの方はメニューのアクセラレーションキーとして
		** 定義されているのでここではチェックする必要はない
		*/
		case GDK_p:
		case GDK_P: /* CTRL / ALT + 'p' : 前の選択肢へ戻る */
			if (event->state & GDK_MOD1_MASK) {
				local_system.SetBacklog(-3);
			}
			break;
		case GDK_n:
		case GDK_N: /* CTRL / ALT + 'n' : 次の選択肢まで読み飛ばし */
			if (event->state & GDK_MOD1_MASK) {
				local_system.StartTextSkipMode(-1);
			}
			break;
		case GDK_b:
		case GDK_B: /* CTRL / ALT + 'b' : 100メッセージ戻る */
			if (event->state & GDK_MOD1_MASK) {
				local_system.SetBacklog(100);
			}
			break;
		case GDK_f:
		case GDK_F: /* CTRL / ALT + 'f' : 100メッセージ読み飛ばし */
			if (event->state & GDK_MOD1_MASK) {
				local_system.StartTextSkipMode(100);
			}
			break;
case GDK_Q:
case GDK_q:
{
/* 一画面描画：29 or 39msec */
/* 320,480 : 15 / 24sec */
/* 320,240 : 8 / 15sec */
/* request 40回：1 / 10msec */
/* request 160回：10 / 11msec */
/* 一回 1/8 msec くらい？ */
/* DisplaySyncのせいで 10msec くらいのwait が入るらしい */
/* 画像：35msec / screen */
/* 120msec / 100x100 x 100 */
/* 240msec / 480req x 100 */
/* 2700msec / 120 6x4 */
/* 220msec / 200x100 */

/* 160 / 230 */
/* / 90 */

int i;
struct timeval t1,t2;
int n=0,m=0;
int j;for(j=0;j<100;j++){
gettimeofday(&t1,0);
for (i=0;i<480;i+=480){
	main.get_window().draw_image(gc,*image,i,i,i,i,local_system.DefaultScreenWidth(),local_system.DefaultScreenHeight());
//	main.get_window().draw_image(gc,*image,i,i,i,i,1,1);
}
gettimeofday(&t2,0);
//printf("1 %d msec\n",(t2.tv_sec-t1.tv_sec)*1000+(t2.tv_usec-t1.tv_usec)/1000);
n+= (t2.tv_sec-t1.tv_sec)*1000*1000+(t2.tv_usec-t1.tv_usec);
DisplaySync();
gettimeofday(&t2,0);
//printf("2 %d msec\n",(t2.tv_sec-t1.tv_sec)*1000+(t2.tv_usec-t1.tv_usec)/1000);
m+= (t2.tv_sec-t1.tv_sec)*1000*1000+(t2.tv_usec-t1.tv_usec);
}
printf("%d / %d\n",n/1000,m/1000);
}
		case GDK_Escape:
		case GDK_Zenkaku_Hankaku:
			mouse_clicked |= 128; break;
		case GDK_Home:
		case GDK_KP_Home:
			local_system.SetBacklog(-4); /* 前の日付まで戻る */
			break;
		case GDK_End:
		case GDK_KP_End:
			local_system.StartTextSkipMode(-2); /* 次の日付まで読み飛ばし */
			break;
		case GDK_Prior:
		case GDK_KP_Prior:
		/* case GDK_Page_Up: */ /* keycode 的には Prior == PageUpらしい */
		/* case GDK_KP_Page_Up: */
			mouse_clicked |= 2048; /* ホイールの上と同じ */
			break;
		case GDK_Next:
		case GDK_KP_Next:
		/* case GDK_Page_Down: */ /* keycode 的には Next == PageDownらしい */
		/* case GDK_KP_Page_Down: */
			mouse_clicked |= 2048; /* ホイールの上と同じ */
			break;
		}
	}
	return FALSE;
}

gint AyuWindow::focusEvent(GdkEventFocus* event) {
	if (event->in == 0) mouse_clicked &= ~4;
	return FALSE;
}

void AyuWindow::SetMouseMode(int is_key_use) {
	if (is_key_use) mouse_key |= 1;
	else mouse_key = 0;
}
void AyuSys::SetMouseMode(int is_use_key) {
	if (main_window)
		main_window->SetMouseMode(is_use_key);
}

void AyuWindow::TimerCall(void) {
	if (retn_timer) {
		int retn_time = local_system.getTime(retn_timer);
		if (retn_time > local_system.config->GetParaInt("#RETN_SPEED")) {
			ChangeRetnCursor();
			local_system.freeTimerBase(retn_timer);
			retn_timer = 0;
		}
	}
	if (retn_timer == 0) retn_timer = local_system.setTimerBase();
	/* キーボードによるマウスの移動 */
	if ( (mouse_key&1) && mouse_timer) {
		int time = (mouse_key>>16);
		mouse_key &= 0xffff;
		time += local_system.getTime(mouse_timer);
		if (mouse_key & 0x100) {
			mouse_key |= (time%5)<<16;
			time /= 5;
		} else {
			mouse_key |= (time%10)<<16;
			time /= 10;
		}
		if (mouse_key & 0x200) {
			mouse_x -= time;
			if (mouse_x < 0) mouse_x = 0;
		} else if (mouse_key & 0x400) {
			mouse_x += time;
			if (mouse_x >= local_system.DefaultScreenWidth()) mouse_x = local_system.DefaultScreenWidth()-1;
		}
		if (mouse_key & 0x800) {
			mouse_y -= time;
			if (mouse_y < 0) mouse_y = 0;
		} else if (mouse_key & 0x1000) {
			mouse_y += time;
			if (mouse_y >= local_system.DefaultScreenHeight()) mouse_y = local_system.DefaultScreenHeight()-1;
		}
		CheckIconRegion();
		if (mouse_key & 0x1e00) {
			SyncPixmap();
			cursor->Draw(mouse_x-local_system.MouseCursorX(), mouse_y-local_system.MouseCursorY());
		}
	}
	if (mouse_timer) {
		local_system.freeTimerBase(mouse_timer);
		mouse_timer = 0;
	}
	if ( (mouse_key&1) ) {
		mouse_timer = local_system.setTimerBase();
	}
}

void AyuWindow::ChangeRetnCursor(void) {
	if (! is_initialized) return;
	if (return_cursor_viewed == 2)
		sys_im->DrawReturnPixmap(cursor);
	return ;
}

// pdt_buffer_orig = pdt_buffer にする
int AyuSys::SyncPDT(int pdt_number) {
	if (main_window == 0) return -1;
	if (pdt_number >= PDT_BUFFER_DEAL) return -1;
	// buffer == buffer_orig : なにもしない
	if (pdt_buffer[pdt_number] == pdt_buffer_orig[pdt_number]) {
		/* pdt_buffer が未作成なら、作成して終了 */
		/* 本来ならエラー終了でもいい気がする…… */
		if (pdt_buffer_orig[pdt_number] == 0) return DisconnectPDT(pdt_number);
		return 0;
	}
	// 無効な buffer の内容
	if (pdt_buffer[pdt_number] == 0) return -1;
	if (pdt_image[pdt_number] == 0) return -1;
	/* pdt の大きさが合わなければ assign しなおす */
	if (pdt_buffer_orig[pdt_number] == 0 ||
		pdt_buffer[pdt_number]->width != pdt_buffer_orig[pdt_number]->width ||
		pdt_buffer[pdt_number]->height != pdt_buffer_orig[pdt_number]->height) {
		if (pdt_buffer_orig[pdt_number]) {
			delete pdt_buffer_orig[pdt_number];
		}
		pdt_buffer_orig[pdt_number] = new DI_ImageMask;
		pdt_buffer_orig[pdt_number]->CreateImage(
			pdt_buffer[pdt_number]->width, pdt_buffer[pdt_number]->height, DefaultBypp());
	}
	// data をコピー
	CopyRect( *(DI_Image*)pdt_buffer_orig[pdt_number], 0, 0,
		*(DI_Image*)pdt_buffer[pdt_number], 0, 0, pdt_buffer[pdt_number]->width, pdt_buffer[pdt_number]->height);
	// data をセット
	pdt_buffer[pdt_number] = pdt_buffer_orig[pdt_number];
	pdt_buffer[pdt_number]->SetCopyMask(pdt_image[pdt_number]);
	pdt_image[pdt_number] = 0;
	return 0;
}

int AyuSys::DisconnectPDT(int pdt_number) {
	if (main_window == 0) return -1;
	if (pdt_number >= PDT_BUFFER_DEAL) return -1;
	/* pdt の大きさが合わなければ assign しなおす */
	if (pdt_buffer_orig[pdt_number] == 0 ||
		(DefaultScreenWidth() != pdt_buffer_orig[pdt_number]->width || DefaultScreenHeight() != pdt_buffer_orig[pdt_number]->height)) {
		if (pdt_buffer_orig[pdt_number]) {
			delete pdt_buffer_orig[pdt_number];
		}
		Gdk_Visual vis = main_window->main_window.get_visual();
		pdt_buffer_orig[pdt_number] = new DI_ImageMask();
		pdt_buffer_orig[pdt_number]->CreateImage(DefaultScreenWidth(), DefaultScreenHeight(), DefaultBypp());
	}
	pdt_buffer[pdt_number] = pdt_buffer_orig[pdt_number];
	pdt_buffer[pdt_number]->SetCopyMask(0);
	pdt_image[pdt_number] = 0;
	return 0;
}

int AyuSys::DeletePDT(int pdt_number) {
	if (main_window == 0) return -1;
	if (pdt_number >= PDT_BUFFER_DEAL) return -1;
	if (pdt_number <= 0) return -1;
	if (pdt_buffer_orig[pdt_number]) {
		delete pdt_buffer_orig[pdt_number];
	}
	pdt_buffer_orig[pdt_number] = 0;
	pdt_buffer[pdt_number] = 0;
	pdt_image[pdt_number] = 0;
	return 0;
}

void AyuSys::DeleteAllPDT(void) {
	int i;
	for (i=0; i<PDT_BUFFER_DEAL; i++) DeletePDT(i);
	pdt_reader->ClearAllCache();
}

void AyuSys::InitWindow(AyuWindow* main) {
	if (! main_window) {
		// pdt buffer の初期化
		pdt_buffer_orig[0] = new DI_ImageMaskRecord();
		if (main->is_translation_required) {
			pdt_buffer_orig[0]->CreateImage(
				main->image_without_text->gdkobj()->width,
				main->image_without_text->gdkobj()->height,
				pdt_bypp);
		} else
			pdt_buffer_orig[0]->SetImage(main->image_without_text);
		pdt_buffer[0] = pdt_buffer_orig[0];
		pdt_buffer[-1] = pdt_buffer[0];
		pdt_buffer_orig[-1] = pdt_buffer_orig[0];

		int use_pdt_deal = PDT_BUFFER_DEAL; /* 10 */
		int i; for (i=1; i<use_pdt_deal; i++) {
			pdt_buffer_orig[i] = 0;
			pdt_buffer[i] = 0;
			pdt_image[i] = 0;
		}
		pdt_reader = new PDT_Reader(MaxPDTImage, *this);
		main_window = main;
	}
}

/* InitWindow() した内容を解放 */
void AyuSys::FinalizeWindow(void) {
	if (pdt_buffer_orig[0]) delete pdt_buffer_orig[0];
	pdt_buffer_orig[0] = 0;
	int i; for (i=1; i<PDT_BUFFER_DEAL; i++) {
		if (pdt_buffer_orig[i]) {
			delete pdt_buffer_orig[i]; // DI_Image
		}
		pdt_buffer_orig[i] = 0;
	}
	if (pdt_reader) delete pdt_reader;
	pdt_reader = 0;
}

void AyuSys::CallUpdateFunc(void) {
	if (main_window)
		main_window->Draw();
}

void AyuSys::CallUpdateFunc(int x1, int y1, int x2, int y2) {
	if (main_window)
		main_window->Draw(x1,y1,x2,y2);
}

void AyuSys::CallProcessMessages(void) {
	ReceiveMusicPacket();
	if (! main_window) return;
	gdk_flush(); /* XSync() と同じ */
	while(gtk_events_pending()) gtk_main_iteration();
	while (stop_flag == 1) {
		stop_flag = 2;
		if (IsIntterupted()) break;
		while(gtk_events_pending()) gtk_main_iteration();
		if (stop_flag == 2) stop_flag = 1;
	}
}
void AyuSys::WaitNextEvent(void) {
	CallProcessMessages();
	if (main_window) gtk_main_iteration(); /* 次の timer call(10ms) かイベントが起きるまで待つ */
}

void AyuSys::FlushScreen(void)
{
	if (main_window)
		main_window->DisplaySync();
}

void AyuSys::DrawMouse(void) {
	if (main_window) main_window->DrawMouse();
}

void AyuSys::DeleteMouse(void) {
	if (main_window) main_window->DeleteMouse();
}

/* GetMouseInfo: 前回の ClearMouseInfo 呼び出しから今回までにクリックされた状態を返す
** ClearMouseInfo: クリック状態をクリアする
*/
void AyuSys::GetMouseInfo(int& x, int& y, int& clicked) {
	int now_click;
	if (main_window) {
		main_window->GetMouseState(x,y,clicked,now_click);
	} else {
		x=0; y=0; clicked=-1;
	}
}
void AyuSys::ClearMouseInfo(void) {
	if (main_window) {
		main_window->ClearMouseState();
	}
}
void AyuSys::GetKeyCursorInfo(int& left, int& right, int& up, int& down, int& esc) {
	if (main_window)
		main_window->GetKeyCursorInfo(left,right,up,down,esc);
}
void AyuSys::ChangeMenuTextFast(void) {
	if (main_window)
		main_window->ChangeMenuTextFast();
}
void AyuSys::ShowMenuItem(void* items, int active) {
	if (main_window)
		main_window->ShowMenuItem(*(std::vector<const char*>*)items, active);
}
void AyuSys::MakePopupWindow(void) {
	if (main_window) {
		main_window->PopupMenu();
	}
}
void AyuSys::UpdateMenu(char** save_titles) {
	if (main_window)
		main_window->UpdateMenu(save_titles);
}

int AyuSys::OpenNameDialog(NameInfo* names, int list_deal) {
	if (main_window)
		return main_window->OpenNameDialog(names, list_deal);
	else return 1;
}


int AyuSys::SelectLoadWindow(void) {
	if (main_window)
		main_window->PopupLoadMenu();
	return 0;
}
// 画面を揺らす
void AyuSys::Shake(int shake_number) {
	if (shake_number >= SHAKE_DEAL) return;
	if (shake[shake_number] == 0) return;
	if (main_window == 0 ) return;
	if (GrpFastMode() == 2 || GrpFastMode() == 3) return;
	int* cur = shake[shake_number];
	// マウスカーソルは消しておく
	DeleteMouse();
	// 画面を揺らす
	while(cur[2] != -1) {
		int x = cur[0]; int y = cur[1]; int tm = cur[2];
		void* timer = setTimerBase();
		main_window->Shake(x,y);
		waitUntil(timer, tm);
		cur += 3;
	}
	main_window->Shake(0,0);
	// 戻す
	DrawMouse();
}

// 画面を瞬かせる
void AyuSys::BlinkScreen(int c1, int c2, int c3, int wait_time, int count) {
	if (main_window == 0 ) return;
	if (GrpFastMode() == 2 || GrpFastMode() == 3) return;
	// マウスカーソルは消しておく
	DeleteMouse();
	// 瞬く
	main_window->BlinkWindow(c1, c2, c3, wait_time, count);
	// 戻す
	DrawMouse();
}

// 全画面モードにする
// 全画面モードでは main == this なので、event のつけかえ
// (grab の悪影響をなくすため)
void AyuWindow::ToAllScreen(void) {
	if (is_all_screen) return;
	hide_menu();
	main.leave_notify_event.connect(SigC::slot(this, &AyuWindow::nullNotify));
	main.enter_notify_event.connect(SigC::slot(this, &AyuWindow::nullNotify));
	leave_notify_event.connect(SigC::slot(this, &AyuWindow::leaveNotify));
	enter_notify_event.connect(SigC::slot(this, &AyuWindow::enterNotify));
	if (Gtk::Window_AllScreen::ToAllScreen(local_system.DefaultScreenWidth(), local_system.DefaultScreenHeight())) {
		is_all_screen = 1;
		SetMenuScreenmode(is_all_screen);
		return;
	};
	leave_notify_event.connect(SigC::slot(this, &AyuWindow::nullNotify));
	enter_notify_event.connect(SigC::slot(this, &AyuWindow::nullNotify));
	main.leave_notify_event.connect(SigC::slot(this, &AyuWindow::leaveNotify));
	main.enter_notify_event.connect(SigC::slot(this, &AyuWindow::enterNotify));
	show_menu();
	return;
}
void AyuWindow::ToNoAllScreen(void) {
	if (! is_all_screen) return;
	is_all_screen = 0;
	RestoreMode();
	SetMenuScreenmode(is_all_screen);
	leave_notify_event.connect(SigC::slot(this, &AyuWindow::nullNotify));
	enter_notify_event.connect(SigC::slot(this, &AyuWindow::nullNotify));
	main.leave_notify_event.connect(SigC::slot(this, &AyuWindow::leaveNotify));
	main.enter_notify_event.connect(SigC::slot(this, &AyuWindow::enterNotify));
	show_menu();
}

int AyuSys::TextStatusStoreLen(void) {
	return 0;
}
char* AyuSys::TextStatusStore(char* buf, int buf_len) {
	return 0;
}
char* AyuSys::TextStatusRestore(char* buf) {
	return 0;
}

int AyuSys::StatusStoreLen(void) {
	return MusicStatusLen() + TextStatusStoreLen();
}
char* AyuSys::StatusStore(char* buf, int buf_len) {
	buf = TextStatusStore(buf, buf_len);
	return MusicStatusStore(buf, buf_len);
}
char* AyuSys::StatusRestore(char* buf) {
	buf = TextStatusRestore(buf);
	return MusicStatusRestore(buf);
}

int AyuSys::GetWindowID(void) {
	if (main_window == 0) return 0;
	return main_window->GetWindowID();
}

int AyuWindow::GetWindowID(void) {
	return GDK_WINDOW_XWINDOW(main.get_window().gdkobj());
}
int AyuSys::MakePartWindow(int x, int y, int w, int h) {
	if (main_window == 0) return -1;
	return main_window->MakePartWindow(x, y, w, h);
}
void AyuSys::DeletePartWindow(int id) {
	if (main_window == 0) return;
	return main_window->DeletePartWindow(id);
}
