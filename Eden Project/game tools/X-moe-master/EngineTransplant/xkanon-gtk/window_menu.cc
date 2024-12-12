/*  window_menu.cc
 *     ウィンドウのメニューの作成と、イベント処理
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

#include <vector>
#include <stdio.h>
#include <unistd.h>
#include "window.h"

// #define WINDOW_MENU_DEBUG

/**********************************************
**
** セーブ・ロード用のメニューウィンドウ
** 継承されて実際のメニューになる
**
***********************************************
*/
class AyuWin_FileMenu {
public:
	GtkWidget* menu;
	AyuWindow* main_window;
	int item_deal; // メニューの数
	char* menu_string; // メニューの文字(セーブかロード)
	virtual void exec_cmd(int num) = 0; // 実際に実行されるコマンド
	struct ItemShell {
		AyuWin_FileMenu* parent;
		int count;
		GtkWidget* label; // menu label (GtkMenuItem)
	};
	vector<ItemShell*> items;
	AyuWin_FileMenu(AyuWindow* win, int _deal, char* str);
	virtual ~AyuWin_FileMenu();
	// メニューに表示する文字列の設定（セーブ日時など）
	void SetString(char** strlist);
	static void exec_cmd_static(GtkMenuItem *menu_item, gpointer pointer);
};

// AyuWin_FileMenu の実体
class AyuWin_SaveMenu : public AyuWin_FileMenu {
	void exec_cmd(int num);
public:
	AyuWin_SaveMenu(int deal, AyuWindow* parent) :
		AyuWin_FileMenu(parent, deal, "Save") {};
	~AyuWin_SaveMenu() {}
};

class AyuWin_LoadMenu : public AyuWin_FileMenu {
	void exec_cmd(int num);
public:
	AyuWin_LoadMenu(int deal, AyuWindow* parent) :
		AyuWin_FileMenu(parent, deal, "Load") {};
	~AyuWin_LoadMenu() {}
};

AyuWin_FileMenu::~AyuWin_FileMenu(void) {
	vector<ItemShell*>::iterator it;
	for (it=items.begin(); it != items.end(); it++)
		delete *it;
	items.clear();
	gtk_widget_destroy(menu);
}
// deal 個のメニューをつくる
AyuWin_FileMenu::AyuWin_FileMenu(AyuWindow* parent, int deal, char* str) {
	main_window = parent;
	menu = gtk_menu_new();
	item_deal = deal;
	char* gstr = gettext(str);
	menu_string = new char[strlen(gstr)+1]; strcpy(menu_string, gstr);
	int i;
	// メニューを作っていく
	for (i=0; i<deal; i++) {
		// サブメニューをつくる
		// サブメニューのラベル、番号等を含む ItemShell クラスを構成
		ItemShell* shell = new ItemShell;
		shell->parent = this;
		shell->count = i;
		GtkWidget* submenu_container = gtk_menu_item_new();
		GtkWidget* submenu_label = gtk_label_new("  ");
		gtk_misc_set_alignment(GTK_MISC(submenu_label), 0.0, 0.5);
		gtk_container_add(GTK_CONTAINER(submenu_container), submenu_label);
		shell->label = submenu_label;
		items.push_back(shell);

		// メニューの中身："Save" / "Cancel" など
		GtkWidget* submenu = gtk_menu_new();
		GtkWidget* subitem = gtk_menu_item_new_with_label(menu_string);
		gtk_menu_shell_append(GTK_MENU_SHELL(submenu), subitem);
		g_signal_connect(G_OBJECT(subitem), "activate", G_CALLBACK(&exec_cmd_static), shell); /* 'shell' preserves {this,i} */

		subitem = gtk_menu_item_new_with_label(gettext("Cancel"));
		gtk_menu_shell_append(GTK_MENU_SHELL(submenu), subitem);

		// 自分に加える
		gtk_widget_show_all(submenu);
		gtk_widget_show_all(submenu_container);
		gtk_menu_shell_append(GTK_MENU_SHELL(menu), submenu_container);
		gtk_menu_item_set_submenu( GTK_MENU_ITEM(submenu_container), submenu);
	}
	gtk_widget_show(menu);
}

// セーブファイルのタイトル文字列のセット
void AyuWin_FileMenu::SetString(char** strlist) {
	int i; char buf[1024];
	int deal2 = 0; for (; strlist[deal2] != 0; deal2++) ;
	if (deal2 > item_deal) deal2 = item_deal; // 文字列の数をメニューの数に制限する
	vector<ItemShell*>::iterator it = items.begin();
	// メニューの文字列を変えていく
	for (i=0; i<deal2 && it != items.end(); i++, it++) {
		main_window->conv_sjis(strlist[i], buf);
		gtk_label_set_text(GTK_LABEL((*it)->label), buf);
	}
}
extern void SaveFile(int no);
void AyuWin_FileMenu::exec_cmd_static(GtkMenuItem *menu_item, gpointer pointer) {
	ItemShell* shell = (ItemShell*)pointer;
	shell->parent->exec_cmd(shell->count);
}
void AyuWin_SaveMenu::exec_cmd(int no) {
	SaveFile(no);
}
void AyuWin_LoadMenu::exec_cmd(int no) {
	main_window->local_system.SetLoadData(no);
}

/**********************************************
**
** メニューウィンドウ
**
***********************************************
*/
class AyuWin_Menu  {
public:
	AyuWindow* main_window;
	GtkWidget* wid; // GtkMenuBar
	GtkUIManager* ui;
	int in_proc;

	/* file menu , popup menu の save /load menu */
	AyuWin_LoadMenu* file_loadMenu;
	AyuWin_SaveMenu* file_saveMenu;
	AyuWin_LoadMenu* popup_loadMenu;
	AyuWin_SaveMenu* popup_saveMenu;
	AyuWin_LoadMenu* loadMenu;

	AyuWin_Menu(AyuWindow* parent);
	void CreateFileMenu(void);
	void Update(char** menu_list); // セーブ・ロードメニューに付け加える
	// popup menu を表示
	void Popup(int button);
	// popup menu の選択
	// load menu を popup 表示
	void PopupLoad(int button);

	// タイトルを設定
	static const char* root_path[];
	void SetTitle(char* title);

	// menu を enable / disable
	void SetSensitive(const char* menu_name, gboolean mode);


	/**********************************************
	**
	** callback の定義
	**
	***********************************************
	*/

	// ファイル：ドロー
	int PdtDraw(int num) {
		main_window->local_system.DeleteTextWindow();
		if (num < 0x80)
			main_window->local_system.CopyBuffer(0,0,639,479, num, 0,0,0, 0);
		else
			main_window->local_system.CopyPDTtoBuffer(0,0,639,479, num-0x80, 0,0,0, 0);
		return 1;
	}
	// ファイル：システム：一時停止・解除
	int StopProcess(int state) {
		if (in_proc&2) return 0; // 再入禁止
		in_proc |= 2;
		main_window->local_system.SetStopProcess(state);
		in_proc &= ~2;
		return 1;
	}
	// オプション：テキスト速度：no wait / 高速 / 中速 / 低速
	int SetTextSpeed(int time) {
		if (in_proc&4) return 0; // 再入禁止
		in_proc |= 4;
		main_window->local_system.SetTextSpeed(time);
		in_proc &= ~4;
		return 1;
	}
	// ファイル : システム : 強制読み飛ばし
	int SetForceFast(bool mode) {
		if (in_proc&8) return 0; // 再入禁止
		in_proc |= 8;
		main_window->local_system.SetForceFast(mode);
		in_proc &= ~8;
		return 1;
	}
	int SetRandomSelect(bool mode) {
		if (in_proc&8192) return 0; // 再入禁止
		in_proc |= 8192;
		main_window->local_system.SetRandomSelect(mode);
		in_proc &= ~8192;
		return 1;
	}
	// ファイル : システム : 画像効果 ON / OFF
	int SetGraphicEffect(int state) {
		if (in_proc&16) return 0; // 再入禁止
		in_proc |= 16;
		main_window->local_system.SetGraphicEffectOff(state);
		in_proc &= ~16;
		return 1;
	}
	// ファイル：終了：メニューへ
	int GoMenu(void) {
		main_window->local_system.SetGoMenu();
		return 1;
	}
	// オプション:画像早送りモード
	int GrpFastMode(int mode) {
		if (in_proc & 2048) return 0; // 再入禁止
		in_proc |= 2048;
		main_window->local_system.SetGrpFastMode(AyuSys::GrpFastType(mode));
		in_proc &= ~2048;
		return 1;
	}
	// オプション:音声モード
	int KoeMode(int mode) {
		if (in_proc & 4096) return 0; // 再入禁止
		in_proc |= 4096;
		main_window->local_system.SetKoeMode(mode);
		in_proc &= ~4096;
		return 1;
	}
	// オプション：テキスト早送り
	int TextFast(bool mode) {
		if (in_proc & 8192) return 0; // 再入禁止
		in_proc |= 8192;
		/* auto mode を無効化 */
		main_window->local_system.SetTextAutoMode(false);
		GtkAction* action_auto = gtk_ui_manager_get_action(ui, "/Popup/AutoSkipText");
		if (action_auto) gtk_toggle_action_set_active(GTK_TOGGLE_ACTION(action_auto), FALSE);

		main_window->local_system.SetTextFastMode(mode);
		in_proc &= ~8192;
		return 1;
	}
	// オプション：テキスト自動送り
	int TextAuto(bool mode) {
		if (in_proc & 8192) return 0; // 再入禁止
		in_proc |= 8192;
		/* skip mode を無効化 */
		GtkAction* action_skip = gtk_ui_manager_get_action(ui, "/Popup/SkipText");
		main_window->local_system.SetTextFastMode(false);
		if (action_skip) gtk_toggle_action_set_active(GTK_TOGGLE_ACTION(action_skip), FALSE);

		main_window->local_system.SetTextAutoMode(mode);
		in_proc &= ~8192;
		return 1;
	}

	// オプション：テキスト枠消去
	int DeleteWaku(void) {
		if (in_proc&1) return 0; // 再入禁止
		in_proc |= 1;
		// 枠消去
		int type, bri;
		main_window->DeleteTextWindow(&type,&bri);
		int x,y,clicked, now_click;
		// マウスが押されるまで待つ
		main_window->ClearMouseState();
		while(1) {
			main_window->GetMouseState(x,y,clicked, now_click);
			if (clicked != -1) break;
			main_window->local_system.CallProcessMessages();
		}
		main_window->ClearMouseState();
		// テキストを書き直して終了
		main_window->DrawTextWindow(type,bri);
		in_proc &= ~1;
		return 1;
	}

	// オプション：全画面モード
	int ToAllScreen(void) {
		if (in_proc & 32) return 0; // 再入禁止
		in_proc |= 32;
		if (main_window->IsAllScreen()) {
			/* ウィンドウモードへ */
			main_window->ToNoAllScreen();
		} else {
			/* 全画面モードへ */
			main_window->ToAllScreen();
		}
		in_proc &= ~32;
		return 1;
	}
	void SetMenuScreenMode(bool mode) {
		GtkAction* action_full = gtk_ui_manager_get_action(ui, "/Popup/FullScreenMode");
		GtkAction* action_win = gtk_ui_manager_get_action(ui, "/Popup/WindowMode");
		
		if (mode) {
			if (action_full) gtk_action_set_visible(action_full, FALSE);
			if (action_win) gtk_action_set_visible(action_win, TRUE);
			//if (action_win) gtk_action_set_accel_path(action_win, "<alt>Return");
		} else {
			if (action_win) gtk_action_set_visible(action_win, FALSE);
			if (action_full) gtk_action_set_visible(action_full, TRUE);
		//	if (action_full) gtk_action_set_accel_path(action_full, "<alt>Return");
		}
		/* ひとはこれをおまじないと言う…… */
		main_window->DisplaySync();
		usleep(1000*100);
		main_window->DisplaySync();
	}

	int SetBacklog(int back) {
		main_window->local_system.SetBacklog(back);
		return 1;
	}

	// オプション：テキスト読み飛ばし
	int TextSkip(int count) {
		main_window->local_system.StartTextSkipMode(count);
		return 1;
	}
};

/**********************************************
**
** AyuWindow <-> AyuWin_Menu のインターフェース
**
***********************************************
*/
void AyuWindow::UpdateMenu(char** menu_list) {
	if (menu_window)
		menu_window->Update(menu_list);
}

void AyuWindow::CreateMenu(void) {
	// menu をつくる
	menu_window = new AyuWin_Menu(this);
	gtk_box_pack_start(GTK_BOX(main_vbox), menu_window->wid, FALSE, FALSE, 0);
	gtk_widget_show_all(menu_window->wid);
}

void AyuWindow::hide_menu(void) {
	gtk_widget_hide(menu_window->wid);
}

void AyuWindow::show_menu(void) {
	gtk_widget_show(menu_window->wid);
}

void AyuWindow::PopupLoadMenu(int button) {
	if (menu_window)
		menu_window->PopupLoad(button);
}
void AyuWindow::PopupMenu(int button) {
	if (menu_window)
		menu_window->Popup(button);
}
void AyuSys::ChangeMenuTextFast(void) {
	if (!main_window) return;
	if (main_window->menu_window) {
		if (IsTextFast()) main_window->menu_window->TextFast(true);
		if (IsTextAuto()) main_window->menu_window->TextAuto(true);
	}
	main_window->CheckIconRegion();
}

// 必要に応じて、メニューアイテムを消去する
void AyuWindow::ShowMenuItem(const char* menu_name, int active) {
	if (! menu_window) return;
	menu_window->SetSensitive(menu_name, active);
}
void AyuWindow::SetMenuTitle(char* title) {
	if (!menu_window) return;
	menu_window->SetTitle(title);
}
void AyuWindow::SetMenuScreenmode(int mode) {
	if (! menu_window) return;
	menu_window->SetMenuScreenMode(mode);
}

/**********************************************
**
** callback の定義
**
***********************************************
*/
/******************
**  menu callbacks
*/

gboolean menucall_textfast(GtkToggleAction* action, gpointer pointer) { return ((AyuWin_Menu*)pointer)->TextFast(gtk_toggle_action_get_active(action)); }
gboolean menucall_autotext(GtkToggleAction* action, gpointer pointer) { return ((AyuWin_Menu*)pointer)->TextAuto(gtk_toggle_action_get_active(action)); }
gboolean menucall_allskip(GtkToggleAction* action, gpointer pointer) { return ((AyuWin_Menu*)pointer)->SetForceFast(gtk_toggle_action_get_active(action)); }
gboolean menucall_randomselect(GtkToggleAction* action, gpointer pointer) { return ((AyuWin_Menu*)pointer)->SetRandomSelect(gtk_toggle_action_get_active(action)); }
gboolean menucall_erasetext(GtkAction* action, gpointer pointer) { return ((AyuWin_Menu*)pointer)->DeleteWaku(); }
gboolean menucall_allscreen(GtkAction* action, gpointer pointer) { return ((AyuWin_Menu*)pointer)->ToAllScreen(); }
gboolean menucall_notallscreen(GtkAction* action, gpointer pointer) { return ((AyuWin_Menu*)pointer)->ToAllScreen(); }
gboolean menucall_rightclick(GtkAction* action, gpointer pointer) { ((AyuWin_Menu*)pointer)->main_window->MouseClick(2); return TRUE;}
gboolean menucall_gomenu(GtkAction* action, gpointer pointer) { ((AyuWin_Menu*)pointer)->GoMenu(); return TRUE;}
gboolean menucall_destroy(GtkAction* action, gpointer pointer) { ((AyuWin_Menu*)pointer)->main_window->local_system.DestroyWindow(); return TRUE;}

gboolean menucall_backlog_enter(GtkAction* action, gpointer pointer) { return ((AyuWin_Menu*)pointer)->SetBacklog(-1);}
gboolean menucall_backlog1(GtkAction* action, gpointer pointer) { return ((AyuWin_Menu*)pointer)->SetBacklog(2);}
gboolean menucall_backlog10(GtkAction* action, gpointer pointer) { return ((AyuWin_Menu*)pointer)->SetBacklog(11);}
gboolean menucall_backlog100(GtkAction* action, gpointer pointer) { return ((AyuWin_Menu*)pointer)->SetBacklog(101);}
gboolean menucall_backlog_choice(GtkAction* action, gpointer pointer) { return ((AyuWin_Menu*)pointer)->SetBacklog(-3);}
gboolean menucall_backlog_day(GtkAction* action, gpointer pointer) { return ((AyuWin_Menu*)pointer)->SetBacklog(-4);}

gboolean menucall_backlogN10(GtkAction* action, gpointer pointer) { return ((AyuWin_Menu*)pointer)->TextSkip(10);}
gboolean menucall_backlogN100(GtkAction* action, gpointer pointer) { return ((AyuWin_Menu*)pointer)->TextSkip(100);}
gboolean menucall_backlogNchoice(GtkAction* action, gpointer pointer) { return ((AyuWin_Menu*)pointer)->TextSkip(-1);}
gboolean menucall_backlogNday(GtkAction* action, gpointer pointer) { return ((AyuWin_Menu*)pointer)->TextSkip(-2);}

/******************
**  graphic draw for debugging
*/
gboolean menucall_drawBuf0(GtkAction* action, gpointer pointer) { return ((AyuWin_Menu*)pointer)->PdtDraw(1); }
gboolean menucall_drawBuf1(GtkAction* action, gpointer pointer) { return ((AyuWin_Menu*)pointer)->PdtDraw(1); }
gboolean menucall_drawBuf2(GtkAction* action, gpointer pointer) { return ((AyuWin_Menu*)pointer)->PdtDraw(2); }
gboolean menucall_drawBuf3(GtkAction* action, gpointer pointer) { return ((AyuWin_Menu*)pointer)->PdtDraw(3); }
gboolean menucall_drawBuf4(GtkAction* action, gpointer pointer) { return ((AyuWin_Menu*)pointer)->PdtDraw(4); }
gboolean menucall_drawBuf5(GtkAction* action, gpointer pointer) { return ((AyuWin_Menu*)pointer)->PdtDraw(5); }
gboolean menucall_drawBuf6(GtkAction* action, gpointer pointer) { return ((AyuWin_Menu*)pointer)->PdtDraw(6); }
gboolean menucall_drawBuf7(GtkAction* action, gpointer pointer) { return ((AyuWin_Menu*)pointer)->PdtDraw(7); }
gboolean menucall_drawBuf8(GtkAction* action, gpointer pointer) { return ((AyuWin_Menu*)pointer)->PdtDraw(8); }
gboolean menucall_drawBuf9(GtkAction* action, gpointer pointer) { return ((AyuWin_Menu*)pointer)->PdtDraw(9); }
gboolean menucall_drawBufA(GtkAction* action, gpointer pointer) { return ((AyuWin_Menu*)pointer)->PdtDraw(10); }
gboolean menucall_drawBufB(GtkAction* action, gpointer pointer) { return ((AyuWin_Menu*)pointer)->PdtDraw(11); }

gboolean menucall_drawPdt0(GtkAction* action, gpointer pointer) { return ((AyuWin_Menu*)pointer)->PdtDraw(0x81); }
gboolean menucall_drawPdt1(GtkAction* action, gpointer pointer) { return ((AyuWin_Menu*)pointer)->PdtDraw(0x81); }
gboolean menucall_drawPdt2(GtkAction* action, gpointer pointer) { return ((AyuWin_Menu*)pointer)->PdtDraw(0x82); }
gboolean menucall_drawPdt3(GtkAction* action, gpointer pointer) { return ((AyuWin_Menu*)pointer)->PdtDraw(0x83); }
gboolean menucall_drawPdt4(GtkAction* action, gpointer pointer) { return ((AyuWin_Menu*)pointer)->PdtDraw(0x84); }
gboolean menucall_drawPdt5(GtkAction* action, gpointer pointer) { return ((AyuWin_Menu*)pointer)->PdtDraw(0x85); }
gboolean menucall_drawPdt6(GtkAction* action, gpointer pointer) { return ((AyuWin_Menu*)pointer)->PdtDraw(0x86); }
gboolean menucall_drawPdt7(GtkAction* action, gpointer pointer) { return ((AyuWin_Menu*)pointer)->PdtDraw(0x87); }
gboolean menucall_drawPdt8(GtkAction* action, gpointer pointer) { return ((AyuWin_Menu*)pointer)->PdtDraw(0x88); }
gboolean menucall_drawPdt9(GtkAction* action, gpointer pointer) { return ((AyuWin_Menu*)pointer)->PdtDraw(0x89); }
gboolean menucall_drawPdtA(GtkAction* action, gpointer pointer) { return ((AyuWin_Menu*)pointer)->PdtDraw(0x8a); }
gboolean menucall_drawPdtB(GtkAction* action, gpointer pointer) { return ((AyuWin_Menu*)pointer)->PdtDraw(0x8b); }


/******************
**  radio buttons
*/
gboolean menucall_stopprocess(GtkRadioAction* action, GtkRadioAction* current, gpointer pointer) {
	return ((AyuWin_Menu*)pointer)->StopProcess(gtk_radio_action_get_current_value(action));
}
gboolean menucall_seteffect(GtkRadioAction* action, GtkRadioAction* current, gpointer pointer) {
	return ((AyuWin_Menu*)pointer)->SetGraphicEffect(gtk_radio_action_get_current_value(action));
}
gboolean menucall_setspeed(GtkRadioAction* action, GtkRadioAction* current, gpointer pointer) {
	return ((AyuWin_Menu*)pointer)->SetTextSpeed(gtk_radio_action_get_current_value(action));
}
gboolean menucall_setgrpfast(GtkRadioAction* action, GtkRadioAction* current, gpointer pointer) {
	return ((AyuWin_Menu*)pointer)->GrpFastMode(gtk_radio_action_get_current_value(action));
}
gboolean menucall_setkoemode(GtkRadioAction* action, GtkRadioAction* current, gpointer pointer) {
	return ((AyuWin_Menu*)pointer)->KoeMode(gtk_radio_action_get_current_value(action));
}

/**********************************************
**
** MenuBar の構造定義
**
***********************************************
*/
static const gchar* ui_desc = 
"<ui>"
"  <menubar name='MenuBar'>"
"    <menu action='FileMenu'>"
"      <menuitem action='Load'/>"
"      <menuitem action='Save'/>"
#ifdef WINDOW_MENU_DEBUG
"      <menu action='Draw'>"
"        <menuitem action='DrawBuf0'/>"
"        <menuitem action='DrawBuf1'/>"
"        <menuitem action='DrawBuf2'/>"
"        <menuitem action='DrawBuf3'/>"
"        <menuitem action='DrawBuf4'/>"
"        <menuitem action='DrawBuf5'/>"
"        <menuitem action='DrawBuf6'/>"
"        <menuitem action='DrawBuf7'/>"
"        <menuitem action='DrawBuf8'/>"
"        <menuitem action='DrawBuf9'/>"
"        <menuitem action='DrawBufA'/>"
"        <menuitem action='DrawBufB'/>"
"        <menuitem action='DrawPdt0'/>"
"        <menuitem action='DrawPdt1'/>"
"        <menuitem action='DrawPdt2'/>"
"        <menuitem action='DrawPdt3'/>"
"        <menuitem action='DrawPdt4'/>"
"        <menuitem action='DrawPdt5'/>"
"        <menuitem action='DrawPdt6'/>"
"        <menuitem action='DrawPdt7'/>"
"        <menuitem action='DrawPdt8'/>"
"        <menuitem action='DrawPdt9'/>"
"        <menuitem action='DrawPdtA'/>"
"        <menuitem action='DrawPdtB'/>"
"      </menu>"
"      <menu action='System'>"
"        <menuitem action='SysPause'/>"
"        <menuitem action='SysResume'/>"
"        <menuitem action='EffectDisable'/>"
"        <menuitem action='EffectEnable'/>"
"      </menu>"
#endif
"      <menu action='Exit'>"
"        <menuitem action='Continue'/>"
"        <menuitem action='GoMenu'/>"
"        <menuitem action='Destroy'/>"
"      </menu>"
"    </menu>"
"    <menu action='OptionMenu'>"
"      <menuitem action='SkipText'/>"
"      <menuitem action='AutoSkipText'/>"
"      <menuitem action='EraseText'/>"
"      <menuitem action='AllSkip'/>"
"      <menuitem action='RandomSelect'/>"
"      <menu action='TextSpeed'>"
"        <menuitem action='TextSpeedNoWait'/>"
"        <menuitem action='TextSpeedFast'/>"
"        <menuitem action='TextSpeedMedium'/>"
"        <menuitem action='TextSpeedSlow'/>"
"      </menu>"
"      <menu action='VisualEffect'>"
"        <menuitem action='GrpFastNormal'/>"
"        <menuitem action='GrpFastFast'/>"
"        <menuitem action='GrpNoEffect'/>"
"        <menuitem action='GrpDisable'/>"
"      </menu>"
"      <menu action='KoeEffect'>"
"        <menuitem action='KoeEnable'/>"
"        <menuitem action='KoeDisable'/>"
"      </menu>"
"      <menuitem action='FullScreenMode'/>"
"      <menuitem action='WindowMode'/>"
"    </menu>"
"    <menu action='BacklogMenu'>"
"      <menu action='BacklogPrevMenu'>"
"        <menuitem action='PrevLog'/>"
"        <menuitem action='Prev10'/>"
"        <menuitem action='Prev100'/>"
"        <menuitem action='PrevSel'/>"
"        <menuitem action='PrevDay'/>"
"      </menu>"
"      <menu action='BacklogNextMenu'>"
"        <menuitem action='Next10'/>"
"        <menuitem action='Next100'/>"
"        <menuitem action='NextSel'/>"
"        <menuitem action='NextDay'/>"
"      </menu>"
"    </menu>"
"    <menuitem action='Title' position='bot'/>"
"  </menubar>"
"  <popup name='Popup'>"
"      <menuitem action='Title'/>"
"      <separator/>"
"      <menuitem action='Load'/>"
"      <menuitem action='Save'/>"
"      <separator/>"
"      <menuitem action='RightClick'/>"
"      <separator/>"
"      <menuitem action='SkipText'/>"
"      <menuitem action='AutoSkipText'/>"
"      <menuitem action='EraseText'/>"
"      <menuitem action='AllSkip'/>"
#ifdef WINDOW_MENU_DEBUG
"      <menuitem action='RandomSelect'/>"
#endif
"      <menu action='BacklogPrevMenu'>"
"        <menuitem action='PrevLog'/>"
"        <menuitem action='Prev10'/>"
"        <menuitem action='Prev100'/>"
"        <menuitem action='PrevSel'/>"
"        <menuitem action='PrevDay'/>"
"      </menu>"
"      <menu action='BacklogNextMenu'>"
"        <menuitem action='Next10'/>"
"        <menuitem action='Next100'/>"
"        <menuitem action='NextSel'/>"
"        <menuitem action='NextDay'/>"
"      </menu>"
"      <menu action='TextSpeed'>"
"        <menuitem action='TextSpeedNoWait'/>"
"        <menuitem action='TextSpeedFast'/>"
"        <menuitem action='TextSpeedMedium'/>"
"        <menuitem action='TextSpeedSlow'/>"
"      </menu>"
"      <menu action='VisualEffect'>"
"        <menuitem action='GrpFastNormal'/>"
"        <menuitem action='GrpFastFast'/>"
"        <menuitem action='GrpNoEffect'/>"
"        <menuitem action='GrpDisable'/>"
"      </menu>"
"      <menu action='KoeEffect'>"
"        <menuitem action='KoeEnable'/>"
"        <menuitem action='KoeDisable'/>"
"      </menu>"
"      <menuitem action='FullScreenMode'/>"
"      <menuitem action='WindowMode'/>"
#ifdef WINDOW_MENU_DEBUG
"      <menu action='Draw'>"
"        <menuitem action='DrawBuf0'/>"
"        <menuitem action='DrawBuf1'/>"
"        <menuitem action='DrawBuf2'/>"
"        <menuitem action='DrawBuf3'/>"
"        <menuitem action='DrawBuf4'/>"
"        <menuitem action='DrawBuf5'/>"
"        <menuitem action='DrawBuf6'/>"
"        <menuitem action='DrawBuf7'/>"
"        <menuitem action='DrawBuf8'/>"
"        <menuitem action='DrawBuf9'/>"
"        <menuitem action='DrawBufA'/>"
"        <menuitem action='DrawBufB'/>"
"        <menuitem action='DrawPdt0'/>"
"        <menuitem action='DrawPdt1'/>"
"        <menuitem action='DrawPdt2'/>"
"        <menuitem action='DrawPdt3'/>"
"        <menuitem action='DrawPdt4'/>"
"        <menuitem action='DrawPdt5'/>"
"        <menuitem action='DrawPdt6'/>"
"        <menuitem action='DrawPdt7'/>"
"        <menuitem action='DrawPdt8'/>"
"        <menuitem action='DrawPdt9'/>"
"        <menuitem action='DrawPdtA'/>"
"        <menuitem action='DrawPdtB'/>"
"      </menu>"
"      <menu action='System'>"
"        <menuitem action='SysPause'/>"
"        <menuitem action='SysResume'/>"
"        <menuitem action='EffectDisable'/>"
"        <menuitem action='EffectEnable'/>"
"      </menu>"
#endif
"      <menu action='Exit'>"
"        <menuitem action='Continue'/>"
"        <menuitem action='GoMenu'/>"
"        <menuitem action='Destroy'/>"
"      </menu>"
"  </popup>"
"</ui>";
static GtkActionEntry action_list[] = {
  {"FileMenu", 0, "File"},
  {"Load", 0, "Load", 0, 0,0},
  {"Save", 0, "Save", 0, 0,0},
  {"Draw", 0, "Draw", 0, 0,0},
  {"System", 0, "System", 0, 0,0},
  {"Exit", 0, "Exit", 0, 0,0},
  {"Title", 0, "Title", 0, 0,0},
  {"Continue", 0, "Continue", 0, 0,0},
  {"GoMenu", 0, "Return to menu", 0, 0,G_CALLBACK(menucall_gomenu)},
  {"Destroy", 0, "Really exit", 0, 0,G_CALLBACK(menucall_destroy)},
  {"OptionMenu", 0, "Option", 0, 0,0},
  {"EraseText", 0, "Erase text field", 0, 0,G_CALLBACK(menucall_erasetext)},
  {"TextSpeed", 0, "Text speed", 0, 0,0},
  {"VisualEffect", 0, "Visual effect", 0, 0,0},
  {"KoeEffect", 0, "Koe effect", 0, 0,0},
  {"FullScreenMode", 0, "Full screen mode", "<alt>Return", 0,G_CALLBACK(menucall_allscreen)},
  {"WindowMode", 0, "Window mode", 0, 0,G_CALLBACK(menucall_notallscreen)},
  {"RightClick", 0, "Right click", 0, 0, G_CALLBACK(menucall_rightclick)},

  {"BacklogMenu", 0, "Log", 0, 0,0},
  {"PrevLog",     0, "Prev", "Up", 0, G_CALLBACK(menucall_backlog1)},
  {"Prev10",      0, "Prev 10", "Right", 0, G_CALLBACK(menucall_backlog10)},
  {"Prev100",     0, "Prev 100", "<ctl>B", 0, G_CALLBACK(menucall_backlog100)},
  {"PrevSel",     0, "Previous choice", "<ctl>P", 0, G_CALLBACK(menucall_backlog_choice)},
  {"PrevDay",     0, "Previous day", "Home", 0, G_CALLBACK(menucall_backlog_day)},
  {"Next10",      0, "Next 10", "Left", 0, G_CALLBACK(menucall_backlogN10)},
  {"Next100",     0, "Next 100", "<ctl>F", 0, G_CALLBACK(menucall_backlogN100)},
  {"NextSel",     0, "Until choice", "<ctl>N", 0, G_CALLBACK(menucall_backlogNchoice)},
  {"NextDay",     0, "Until title change", "End", 0, G_CALLBACK(menucall_backlogNday)},

  {"BacklogPrevMenu", 0, "Previous text"},
  {"BacklogNextMenu", 0, "Skip"},


/* DrawMenu の子 */
  {"DrawBuf0", 0, "DrawBuf0", 0, 0,G_CALLBACK(menucall_drawBuf0)},
  {"DrawBuf1", 0, "DrawBuf1", 0, 0,G_CALLBACK(menucall_drawBuf0)},
  {"DrawBuf2", 0, "DrawBuf2", 0, 0,G_CALLBACK(menucall_drawBuf0)},
  {"DrawBuf3", 0, "DrawBuf3", 0, 0,G_CALLBACK(menucall_drawBuf0)},
  {"DrawBuf4", 0, "DrawBuf4", 0, 0,G_CALLBACK(menucall_drawBuf0)},
  {"DrawBuf5", 0, "DrawBuf5", 0, 0,G_CALLBACK(menucall_drawBuf0)},
  {"DrawBuf6", 0, "DrawBuf6", 0, 0,G_CALLBACK(menucall_drawBuf0)},
  {"DrawBuf7", 0, "DrawBuf7", 0, 0,G_CALLBACK(menucall_drawBuf0)},
  {"DrawBuf8", 0, "DrawBuf8", 0, 0,G_CALLBACK(menucall_drawBuf0)},
  {"DrawBuf9", 0, "DrawBuf9", 0, 0,G_CALLBACK(menucall_drawBuf0)},
  {"DrawBufA", 0, "DrawBufA", 0, 0,G_CALLBACK(menucall_drawBuf0)},
  {"DrawBufB", 0, "DrawBufB", 0, 0,G_CALLBACK(menucall_drawBuf0)},
  {"DrawPdt0", 0, "DrawPdt0", 0, 0,G_CALLBACK(menucall_drawPdt0)},
  {"DrawPdt1", 0, "DrawPdt1", 0, 0,G_CALLBACK(menucall_drawPdt0)},
  {"DrawPdt2", 0, "DrawPdt2", 0, 0,G_CALLBACK(menucall_drawPdt0)},
  {"DrawPdt3", 0, "DrawPdt3", 0, 0,G_CALLBACK(menucall_drawPdt0)},
  {"DrawPdt4", 0, "DrawPdt4", 0, 0,G_CALLBACK(menucall_drawPdt0)},
  {"DrawPdt5", 0, "DrawPdt5", 0, 0,G_CALLBACK(menucall_drawPdt0)},
  {"DrawPdt6", 0, "DrawPdt6", 0, 0,G_CALLBACK(menucall_drawPdt0)},
  {"DrawPdt7", 0, "DrawPdt7", 0, 0,G_CALLBACK(menucall_drawPdt0)},
  {"DrawPdt8", 0, "DrawPdt8", 0, 0,G_CALLBACK(menucall_drawPdt0)},
  {"DrawPdt9", 0, "DrawPdt9", 0, 0,G_CALLBACK(menucall_drawPdt0)},
  {"DrawPdtA", 0, "DrawPdtA", 0, 0,G_CALLBACK(menucall_drawPdt0)},
  {"DrawPdtB", 0, "DrawPdtB", 0, 0,G_CALLBACK(menucall_drawPdt0)},

};

static GtkToggleActionEntry toggle_list[] = {
  {"SkipText",     0, "Skip text", 0, 0,G_CALLBACK(menucall_textfast), false},
  {"AutoSkipText", 0, "Auto-skip text", 0, 0,G_CALLBACK(menucall_autotext), false},
  {"AllSkip",      0, "Enable all-skip", 0, 0,G_CALLBACK(menucall_allskip), false},
  {"RandomSelect", 0, "Enable random-select", 0, 0,G_CALLBACK(menucall_randomselect), false},
};

static GtkRadioActionEntry radio_list1[] = { /* menucall_stopprocess */
  {"SysPause",  0, "Pause", 0, 0, 1},
  {"SysResume", 0, "Resume", 0, 0, 0},
};
static GtkRadioActionEntry radio_list2[] = { /* menucall_seteffect */
  {"EffectDisable", 0, "Disable effect", 0, 0, 1},
  {"EffectEnable",  0, "Enable effect", 0, 0, 0},
};
static GtkRadioActionEntry radio_list3[] = { /* menucall_setspeed */
  {"TextSpeedNoWait", 0, "No Wait", 0, 0, 1000},
  {"TextSpeedFast",   0, "Fast", 0, 0, 200},
  {"TextSpeedMedium", 0, "Medium", 0, 0, 70},
  {"TextSpeedSlow",   0, "Slow", 0, 0, 20},
};
static GtkRadioActionEntry radio_list4[] = { /* menucall_setgrpfast */
  {"GrpFastNormal", 0, "Normal", 0, 0, 0},
  {"GrpFastFast",   0, "FastGrp", 0, 0, 1},
  {"GrpNoEffect",   0, "No effect", 0, 0, 2},
  {"GrpDisable",    0, "Disable image", 0, 0, 3},
};
static GtkRadioActionEntry radio_list5[] = { /* menucall_setkoemode */
  {"KoeEnable",  0, "Enable KOE", 0, 0, 1},
  {"KoeDisable", 0, "Disable KOE", 0, 0, 0},
};
/**********************************************
**
** メニューバーの直下のメニューの作成
**
***********************************************
*/
AyuWin_Menu::AyuWin_Menu(AyuWindow* parent) {
	main_window = parent; in_proc = 0;

	/* action group の登録 */
	GtkActionGroup* actions;
	actions = gtk_action_group_new("Actions");
	// gettext を使ってメニューを多言語化することを宣言
	gtk_action_group_set_translation_domain(actions, textdomain(0));
	// action の登録
	int nactions = sizeof(action_list) / sizeof(action_list[0]);
	gtk_action_group_add_actions(actions, action_list, nactions, this);
	nactions = sizeof(toggle_list) / sizeof(toggle_list[0]);
	gtk_action_group_add_toggle_actions(actions, toggle_list, nactions, this);
	/* radio button の登録 */
	/* StopProcess */
	nactions = sizeof(radio_list1) / sizeof(radio_list1[0]);
	gtk_action_group_add_radio_actions(actions, radio_list1, nactions, 0, G_CALLBACK(menucall_stopprocess), this);
	/* GrpEffect */
	nactions = sizeof(radio_list2) / sizeof(radio_list2[0]);
	gtk_action_group_add_radio_actions(actions, radio_list2, nactions, 0, G_CALLBACK(menucall_seteffect), this);
	/* TextSpeed */
	nactions = sizeof(radio_list3) / sizeof(radio_list3[0]);
	gtk_action_group_add_radio_actions(actions, radio_list3, nactions, 70, G_CALLBACK(menucall_setspeed), this);
	/* GrpFast */
	nactions = sizeof(radio_list4) / sizeof(radio_list4[0]);
	gtk_action_group_add_radio_actions(actions, radio_list4, nactions, 1, G_CALLBACK(menucall_setgrpfast), this);
	/* Koe */
	nactions = sizeof(radio_list5) / sizeof(radio_list5[0]);
	gtk_action_group_add_radio_actions(actions, radio_list5, nactions, 1, G_CALLBACK(menucall_setkoemode), this);

	/* UI の初期化 */
	ui = gtk_ui_manager_new();
	gtk_ui_manager_insert_action_group(ui, actions, 0);
	gtk_ui_manager_add_ui_from_string(ui, ui_desc, -1, 0);
	gtk_window_add_accel_group(GTK_WINDOW(main_window->wid), gtk_ui_manager_get_accel_group(ui));
	wid = gtk_ui_manager_get_widget(ui, "/MenuBar");

	GtkWidget* title_wid = gtk_ui_manager_get_widget(ui, "/MenuBar/Title");
	gtk_menu_item_set_right_justified(GTK_MENU_ITEM(title_wid), TRUE);
	/* セーブ・ロード用 Menu 作成 */
	CreateFileMenu();

	// 初期設定
	StopProcess(0);
	SetGraphicEffect(0);
	SetTextSpeed(70);
	GrpFastMode(1);
	TextFast(false);
	SetMenuScreenMode(false);
}

/* menu 階層の root を指定する */
const char* AyuWin_Menu::root_path[] = {
	"/MenuBar/",
	"/MenuBar/FileMenu/",
	"/MenuBar/FileMenu/Exit/",
	"/MenuBar/OptiohMenu/",
	"/MenuBar/BacklogMenu/",
	"/Popup/",
	"/Popup/Exit/",
	0
};
static void SetItemLabel(GtkWidget* widget, gpointer  pointer) {
	char* name = (char*)pointer;
	if (GTK_IS_LABEL(widget)) {
		gtk_label_set_label(GTK_LABEL(widget), name);
	}
}
void AyuWin_Menu::SetTitle(char* title) {
	const char** it = root_path;
	do {
		char buf[1024];
		strcpy(buf, *it);
		strcat(buf, "Title");
		GtkWidget* menu_item = gtk_ui_manager_get_widget(ui, buf);
		if (menu_item == 0) continue;
		if (!GTK_IS_CONTAINER(menu_item)) continue;
		gtk_container_foreach(GTK_CONTAINER(menu_item), SetItemLabel, title);
	} while(*++it != 0);
}

void AyuWin_Menu::CreateFileMenu(void) {
	// ファイルメニューの作成
	int save_times = main_window->local_system.config->GetParaInt("#SAVEFILETIME");
	file_loadMenu = new AyuWin_LoadMenu(save_times, main_window);
	file_saveMenu = new AyuWin_SaveMenu(save_times, main_window);
	gtk_menu_item_set_submenu(
		GTK_MENU_ITEM(gtk_ui_manager_get_widget(ui, "/MenuBar/FileMenu/Load")),
		file_loadMenu->menu);
	gtk_menu_item_set_submenu(
		GTK_MENU_ITEM(gtk_ui_manager_get_widget(ui, "/MenuBar/FileMenu/Save")),
		file_saveMenu->menu);

	popup_loadMenu = new AyuWin_LoadMenu(save_times, main_window);
	popup_saveMenu = new AyuWin_SaveMenu(save_times, main_window);
	gtk_menu_item_set_submenu(
		GTK_MENU_ITEM(gtk_ui_manager_get_widget(ui, "/Popup/Load")),
		popup_loadMenu->menu);
	gtk_menu_item_set_submenu(
		GTK_MENU_ITEM(gtk_ui_manager_get_widget(ui, "/Popup/Save")),
		popup_saveMenu->menu);

	loadMenu = new AyuWin_LoadMenu(save_times, main_window);
}

void AyuWin_Menu::Update(char** list) {
	file_loadMenu->SetString(list);
	file_saveMenu->SetString(list);
	popup_loadMenu->SetString(list);
	popup_saveMenu->SetString(list);
	loadMenu->SetString(list);
}

void AyuWin_Menu::Popup(int button) {
	GtkWidget* popup = gtk_ui_manager_get_widget(ui, "/Popup");
	gtk_menu_popup(GTK_MENU(popup), 0, 0, 0, 0, button, gtk_get_current_event_time());
}

void AyuWin_Menu::PopupLoad(int button) {
	gtk_menu_popup(GTK_MENU(loadMenu->menu), 0, 0, 0, 0, button, gtk_get_current_event_time());
}


/**************************************************
**
**	ある名前を持つ MenuItem の状態を変更する
*/

void AyuWin_Menu::SetSensitive(const char* menu_name, gboolean mode) {
	gtk_menu_shell_deselect(GTK_MENU_SHELL(wid));
	gtk_window_activate_focus(GTK_WINDOW(main_window->wid));

	const char** it = root_path;
	do {
		char buf[1024];
		strcpy(buf, *it);
		strcat(buf, menu_name);
		GtkAction* action = gtk_ui_manager_get_action(ui, buf);
		if (action == 0) continue;
		gtk_action_set_sensitive(action, mode);
	} while(*++it != 0);
}

