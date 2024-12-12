/*  window_name.cc
 *     名前入力ウィンドウ関係。
 */
/*
 *
 *  Copyright (C) 2001-   Kazunori Ueno(JAGARL) <jagarl@creator.club.ne.jp>
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

#include <unistd.h>
#include <string>
#include <vector>
#include <list>
#include "window.h"

class NameSubWindow {
public:
	GtkWidget* wid;
	struct NameInfo {
		string title;
		string old_name;
		GtkWidget* title_label; /* GtkLabel */
		GtkWidget* old_entry; /* GtkEntry */
		GtkWidget* new_entry; /* GtkEntry */
		NameInfo(const char* t, const char* old_n)
		{
			old_entry = gtk_entry_new();
			new_entry = gtk_entry_new();
			gtk_entry_set_max_length(GTK_ENTRY(old_entry), 10);
			gtk_entry_set_max_length(GTK_ENTRY(new_entry), 10);
			if (t == 0) t = "";
			if (old_n == 0) old_n = "";
			title = t;
			old_name = old_n;
			title_label = gtk_label_new(title.c_str());
			gtk_entry_set_text(GTK_ENTRY(old_entry), old_name.c_str());
			gtk_entry_set_editable(GTK_ENTRY(old_entry), False);
			gtk_entry_set_text(GTK_ENTRY(new_entry), old_name.c_str());
		};
		void Attach(GtkWidget* table, int row) {
			gtk_table_attach(GTK_TABLE(table), title_label, 0, 1, row, row+1, GTK_EXPAND, GTK_EXPAND, 0, 0);
			gtk_table_attach(GTK_TABLE(table), old_entry, 1, 2, row, row+1, GTK_EXPAND, GTK_EXPAND, 0, 0);
			gtk_table_attach(GTK_TABLE(table), new_entry, 3, 4, row, row+1, GTK_EXPAND, GTK_EXPAND, 0, 0);
			gtk_widget_show(title_label);
			gtk_widget_show(old_entry);
			gtk_widget_show(new_entry);
		}
		char* NewName(void) {
			const char* name = gtk_entry_get_text(GTK_ENTRY(new_entry));
			char* ret = new char[strlen(name)*2+1];
			strcpy(ret, name);
			return ret;
		}
	};
	AyuWindow* main_window;
	GtkWidget* root_widget;
	GIConv iconv_rev;
	vector<NameInfo*> info;
	GtkWidget* main; /* GtkVBox */
	GtkWidget* table; /* GtkTable */
	GtkWidget* button_box; /* GtkHBox */
	GtkWidget* label[3]; /* GtkLabel */
	GtkWidget* ok_button; /* GtkButton */
	GtkWidget* cancel_button; /* GtkButton */
	int pos_x, pos_y, size_x, size_y;
	int state; /* 1 で終了 2 でキャンセル */
	int is_init;

	NameSubWindow(AyuWindow* main, GtkWidget* root, AyuSys::NameInfo* info,int list_len);
	~NameSubWindow();
	void GetNames(AyuSys::NameInfo* new_name);
	int State() { return state; }
	static gint ButtonPressEvent1(GtkWidget*,GdkEventButton* button, gpointer pointer) {
		NameSubWindow* cur = (NameSubWindow*)pointer;
		cur->state = 1;
		return FALSE;
	}
	static gint ButtonPressEvent2(GtkWidget*,GdkEventButton* button, gpointer pointer) {
		NameSubWindow* cur = (NameSubWindow*)pointer;
		cur->state = 2;
		return FALSE;
	}
	void ClearState(void) { state = 0; }
	static gboolean destroyEvent(GtkWidget*, GdkEventAny*, gpointer pointer) {
		NameSubWindow* cur = (NameSubWindow*)pointer;
		cur->state = 2;
		return FALSE;
	}
	static gboolean focusEvent(GtkWidget*, GdkEventFocus*, gpointer pointer) {
		NameSubWindow* cur = (NameSubWindow*)pointer;
		cur->state = 2;
		return FALSE;
	}
	/* 親ウィンドウの真ん中に移動 */
	static gboolean configureEvent(GtkWidget*, GdkEventConfigure* event, gpointer pointer) {
		NameSubWindow* cur = (NameSubWindow*)pointer;
		if (cur->is_init) return FALSE;
		cur->is_init = 1;
		int root_width, root_height;
		int window_width = event->width;
		int window_height = event->height;
		gdk_window_get_size(cur->root_widget->window, &root_width, &root_height);
		int x = 0, y = 0;
		if (root_width > window_width) x += (root_width-window_width)/2;
		if (root_height > window_height) y += (root_height-window_height)/2;
		gtk_fixed_move(GTK_FIXED(cur->root_widget), cur->wid, x, y);
		return FALSE;
	}
	static gboolean enterEvent(GtkWidget*, GdkEventCrossing* event, gpointer pointer) {
		NameSubWindow* cur = (NameSubWindow*)pointer;
		GdkCursor* cursor = gdk_cursor_new(GDK_ARROW);
		gdk_window_set_cursor(cur->wid->window, cursor);
		g_object_unref(cursor);
		return FALSE;
	}

};

NameSubWindow::NameSubWindow(AyuWindow* root_window, GtkWidget* root_wid, AyuSys::NameInfo* sysinfo,int list_len)
{
	root_widget = root_wid;
	wid = gtk_fixed_new();
	gtk_fixed_set_has_window(GTK_FIXED(wid), TRUE);
	main = gtk_vbox_new(true, 0);
	button_box = gtk_hbox_new(true, 0);
	label[0] = gtk_label_new(gettext("old name"));
	label[1] = gtk_label_new(gettext("->"));
	label[2] = gtk_label_new(gettext("new name"));
	table = gtk_table_new(4, list_len <= 0 ? 1 : list_len+1,FALSE) ,
	ok_button = gtk_button_new_with_label(gettext("Ok"));
	cancel_button = gtk_button_new_with_label(gettext("Cancel"));

	// 変換エンジンの初期化
	iconv_rev = g_iconv_open("shift_jis", "utf-8");

	// ウィンドウの初期化
	
	gtk_container_add(GTK_CONTAINER(wid), main);
	gtk_widget_show(main);
	gtk_widget_set_events(wid, gtk_widget_get_events(wid) |
		GDK_VISIBILITY_NOTIFY_MASK | GDK_STRUCTURE_MASK|
		GDK_ENTER_NOTIFY_MASK | GDK_LEAVE_NOTIFY_MASK);
	state = 0; is_init = 0;
	if (list_len <= 0) list_len = 0;
	main_window = root_window;
	gtk_box_pack_start(GTK_BOX(main), table, TRUE, FALSE, 0);
	/* ボタンの作成 */
	gtk_box_pack_start(GTK_BOX(button_box), ok_button, TRUE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(button_box), cancel_button, TRUE, FALSE, 0);
	g_signal_connect(G_OBJECT(ok_button), "button_press_event", G_CALLBACK(ButtonPressEvent1), gpointer(this));
	g_signal_connect(G_OBJECT(cancel_button), "button_press_event", G_CALLBACK(ButtonPressEvent2), gpointer(this));
	gtk_box_pack_start(GTK_BOX(main), button_box, TRUE, FALSE, 0);

	/* エントリなどをつくる */
	gtk_table_attach(GTK_TABLE(table), label[0], 1, 2, 0, 1, GTK_EXPAND, GTK_EXPAND, 0, 0);
	gtk_table_attach(GTK_TABLE(table), label[1], 2, 3, 0, 1, GTK_EXPAND, GTK_EXPAND, 0, 0);
	gtk_table_attach(GTK_TABLE(table), label[2], 3, 4, 0, 1, GTK_EXPAND, GTK_EXPAND, 0, 0);
	int i;
	for (i=0; i<list_len; i++) {
		char title_utf[1024];
		char name_utf[1024];
		main_window->conv_sjis(sysinfo[i].title, title_utf);
		main_window->conv_sjis(sysinfo[i].old_name, name_utf);
		NameInfo* new_info = new NameInfo(title_utf, name_utf);
		new_info->Attach(table, i+1);
		info.push_back(new_info);
	}
	gtk_widget_show_all(wid);
}

NameSubWindow::~NameSubWindow() {
	vector<NameInfo*>::iterator it;
	for (it=info.begin(); it != info.end(); it++)
		delete *it;
	if (iconv_rev) g_iconv_close(iconv_rev);
	gtk_widget_destroy(wid);
	wid = 0;
}

void NameSubWindow::GetNames(AyuSys::NameInfo* sysinfo) {
	vector<NameInfo*>::iterator it;
	for (it=info.begin(); it != info.end(); it++, sysinfo++) {
		char* newname_utf = (*it)->NewName();
		if (iconv_rev) {
			gsize fromsize = strlen(newname_utf);
			gsize newname_len = strlen(newname_utf)*2+2;
			char* newname = new char[newname_len];
			sysinfo->new_name = newname;
 			g_iconv(iconv_rev, (char**)&newname_utf, &fromsize, &newname, &newname_len);
			newname[0] = 0;
			delete[] newname_utf;
		} else {
			sysinfo->new_name = newname_utf;
		}
	}
}

int AyuWindow::OpenNameDialog(AyuSys::NameInfo* names, int list_deal)
{
	int old_brightness = text_window_brightness;
	int old_window_type = text_window_type;
	DrawTextWindow(0, 50); // 画面を暗くする
	DeleteMouse();
	P_CURSOR* cursor = new P_CURSOR(main_window);
	cursor->Draw();

	/* 名前入力用ウィンドウを作る */
	NameSubWindow* sub_window = new NameSubWindow(this, main, names, list_deal);
	
	gtk_fixed_put(GTK_FIXED(main), sub_window->wid, 100,100);
	sub_window->ClearState();
	while(sub_window->State() == 0 && (!local_system.IsIntterupted())) {
		local_system.WaitNextEvent();
	}
	int ret = 0;
	if ( local_system.IsIntterupted() || sub_window->State() == 2) {
		ret = 1;
	} else {
		sub_window->GetNames(names);
	}
	delete sub_window;
	cursor->Delete();
	delete cursor;
	DrawMouse();
	DrawTextWindow(old_window_type, old_brightness);
	return ret;
}

/* 名前入力エントリ（一つだけ） */
class NameSubEntry {
public:
	GtkWidget* wid; /* GtkWntry */
	GtkWidget* root_widget; /* GtkFixed */
	static list<NameSubEntry*> all_entries;
	char name[1024];
	NameSubEntry(GtkWidget* root) : root_widget(root) {
		wid = gtk_entry_new();
		all_entries.push_back(this);
	};
	~NameSubEntry(void) {
		gtk_container_remove(GTK_CONTAINER(root_widget), wid);
		gtk_widget_destroy(wid);
		wid = 0;
		all_entries.remove(this);
	}
	const char* GetName(void);
	void SetName(const char* new_name);
	static void CloseAll(void) {
		list<NameSubEntry*>::iterator it;
		for (it = all_entries.begin(); it != all_entries.end(); it++) {
			delete (*it);
		}
		all_entries.clear();
	}
};
list<NameSubEntry*> NameSubEntry::all_entries;
const char* NameSubEntry::GetName(void) {
	GIConv iconv = g_iconv_open("shift_jis", "utf-8");
	if (iconv) {
		char* from = (char*)gtk_entry_get_text(GTK_ENTRY(wid));
		char* to = name;
		gsize tosize = 1000;
		gsize fromsize = strlen(from);
		g_iconv(iconv, &from, &fromsize, &to, &tosize);
		to[0] = 0;
		g_iconv_close(iconv);
		return name;
	} else {
		strncpy(name, gtk_entry_get_text(GTK_ENTRY(wid)), 1000);
		name[1000] = 0;
		return name;
	}
}
void NameSubEntry::SetName(const char* new_name) {
	GIConv iconv = g_iconv_open("utf-8", "shift_jis");
	if (iconv) {
		char* from = (char*)new_name;
		char* to = name;
		gsize tosize = 1000;
		gsize fromsize = strlen(from);
		g_iconv(iconv, &from, &fromsize, &to, &tosize);
		to[0] = 0;
		g_iconv_close(iconv);
	} else {
		strncpy(name, new_name, 1000);
		name[1000] = 0;
	}
	gtk_entry_set_text(GTK_ENTRY(wid), name);
}

NameSubEntry* AyuSys::OpenNameEntry(int x, int y, int width, int height, const COLOR_TABLE& fore, const COLOR_TABLE& back) {
	if (main_window) return main_window->OpenNameEntry(x,y,width,height,fore,back);
	else return 0;
}
NameSubEntry* AyuWindow::OpenNameEntry(int x, int y, int width, int height, const COLOR_TABLE& fore, const COLOR_TABLE& back) {
	if (x < 0) { width += x; x = 0; }
	if (y < 0) { height += y; y = 0;}
	if (x+width >= 640) { width = 640-x; }
	if (y+height>= 480) { height = 480-y; }
	if (width <= 0 || height <= 0) return 0;

	/* エントリを作る */
	NameSubEntry* new_entry = new NameSubEntry(main);
	gtk_widget_set_usize(new_entry->wid,width, height);
	gtk_fixed_put(GTK_FIXED(main), new_entry->wid, x, y);
	gtk_widget_show(new_entry->wid);
	/* 色設定 */
	GdkColor fore_color = {0, fore.c1*65535/255, fore.c2*65535/255, fore.c3*65535/255};
	GdkColor back_color = {0, back.c1*65535/255, back.c2*65535/255, back.c3*65535/255};
	GdkColormap* colormap = gdk_drawable_get_colormap(GDK_DRAWABLE(main_window));
	gdk_colormap_alloc_color(colormap, &fore_color, FALSE, TRUE);
	gdk_colormap_alloc_color(colormap, &back_color, FALSE, TRUE);

	GtkStyle* style = gtk_widget_get_style(new_entry->wid);
	style = gtk_style_copy(style);
	style->fg[GTK_STATE_NORMAL] = fore_color;
	style->bg[GTK_STATE_NORMAL] = back_color;
	style->base[GTK_STATE_NORMAL] = back_color;
	gtk_widget_set_style(new_entry->wid, style);
	g_object_unref(style);
	return new_entry;
}

const char* AyuSys::GetNameFromEntry(NameSubEntry* entry) {
	if (entry) return entry->GetName();
	else return "";
}
void AyuSys::SetNameToEntry(NameSubEntry* entry, const char* name) {
	if (entry) entry->SetName(name);
}
void AyuSys::CloseNameEntry(NameSubEntry* entry){
	if (entry) delete entry;
}
void AyuSys::CloseAllNameEntry(void) {
	NameSubEntry::CloseAll();
}

