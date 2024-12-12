/*  window_text.cc
 *     ��å�����������ɥ��δط������
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

#include <gdk/gdkx.h>
#include <unistd.h>
#include "window.h"

// �ƥ����ȥ�����ɥ����Ȥ�ʸ���δֳ�
#define TEXT_X_MARGIN1	32 /* �� */
#define TEXT_X_MARGIN2	32 /* �� */
#define TEXT_Y_MARGIN1	16 /* �� */
#define TEXT_Y_MARGIN2	8 /* �� */
#define ICON_MARGIN 32

// �����κ����
#define SELECT_SIZE 20

struct TextWinInfo {
private:
	int twsize;
	int csize;
	std::vector<int> pos;
	std::vector<int> first;
	std::vector<GdkColor> color;
	std::vector<GdkColor> back_color;
public:
	TextWinInfo() { twsize = 0; }
	void SetLines(int s) {
		twsize = s; csize = 0;
		pos.clear();
		first.clear();
		color.clear();
		back_color.clear();
	}
	void End(void) {}
	void Start(void) {
		csize = 0;
		pos.clear();
		first.clear();
		color.clear();
		back_color.clear();
	}
	void NextLine(int t, int p, GdkColor c, GdkColor bc) {
		if (csize >= twsize) return;
		pos.push_back(p);
		first.push_back(t);
		color.push_back(c);
		back_color.push_back(bc);
		csize++;
	}
	int size(void) {
		return csize;
	}
	int TextPos(int s) {
		if (csize <= s) return 10;
		return pos[s];
	}
	int TextFirst(int s) {
		if (csize <= s) return 0;
		return first[s];
	}
	GdkColor Color(int s) {
		if (csize <= s) {
			GdkColor c = {0,0xffff,0xffff,0xffff};
			return c;
		}
		return color[s];
	}
	GdkColor BackColor(int s) {
		if (csize <= s) {
			GdkColor c = {0,0,0,0};
			return c;
		}
		return back_color[s];
	}
};

// �ƥ����Ȥ��礭���ʣ�ʸ�����礭����
// AyuSys::MojiSize{X/Y} ���֤������Ѥ�
// #define CHAR_X_WIDTH 24
// #define CHAR_Y_HEIGHT 27

// �꥿���󡦥�������ΰ���
// #define RETN_X 590
// #define RETN_Y 436

// �ƥ���������λ��Ρ���������Ʊ�Τδֳ�
#define SPACE_BOT 0

static char han_to_zen_table[] =
"��������������������������������" // 0x00
"��������������������������������" // 0x10
"�����ɡ������ǡʡˡ��ܡ��ݡ���" // 0x20
"���������������������������䡩" // 0x30
"�����£ãģţƣǣȣɣʣˣ̣ͣΣ�" // 0x40
"�Уѣңӣԣգ֣ףأ٣ڡΡ�ϡ���" // 0x50
"������������������" // 0x60
"�������������������Сáѡ���" // 0x70
"��������������������������������" // 0x80
"��������������������������������" // 0x90
"�����֡ס����򥡥������������" // 0xa0
"��������������������������������" // 0xb0
"�����ĥƥȥʥ˥̥ͥΥϥҥեإۥ�" // 0xc0
"�ߥ������������󡫡�" // 0xd0
"��������������������������������" // 0xe0
"��������������������������������" // 0xf0
;

/* kinsoku_flag �ξ��� */
#define KINSOKU_NONE 0
#define KINSOKU_TOP 1 /* �꥿�����ľ�� */
#define KINSOKU_TOP2 2  /* ̾��ɽ����ľ�� */
#define KINSOKU_KAIWA 4 /* ����ʸ����Ƭ��ʸ������ǥ�� */

#define NO_KINSOKU 0
#define KINSOKU_HEAD 1 /* ��Ƭ�ˤʤ� */
#define KINSOKU_TAIL 2 /* �����ˤʤ� */
#define NAME_HEAD 3 /* ̾����̤���Ƭ*/
#define NAME_TAIL 4 /* ̾����̤�����*/

static int kinsoku_table1[] = {
/* 0 1 2 3 4 5 6 7     0 1 2 3 4 5 6 7 */
   0,0,2,2,2,2,0,0, /* �ء������������� */
   0,2,2,0,0,0,0,0, /* ���������������� */
   0,0,0,0,0,0,0,0, /* ���������������� */
   0,0,0,0,2,0,0,0, /* ���������������� */
   0,2,0,0,2,2,1,2, /* �����¡áġšơ� */
   1,2,1,2,1,2,1,2, /* �ȡɡʡˡ̡͡Ρ� */
   1,2,1,2,1,2,1,2, /* �Сѡҡӡԡա֡� */
   1,2,3,4,0,0,0,0, /* �ء١ڡۡܡݡޡ� */
   0,0,0,0,0,0,0,0, /* ��������� */
   0,0,0,0,0,0,0,0, /* ��������� */
   0,0,0,0,0,0,0,0, /* ������������ */
   0,0,0,0,0,0,0,0, /* ���������������� */
   0
};
static int kinsoku_table2[] = {
  0,2,0,2,0,2,0,2,0,2,0,0,0,0,0,0, /* �������������������������������� */
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, /* �������������������������������� */
  0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0, /* �����¤äĤŤƤǤȤɤʤˤ̤ͤΤ� */
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, /* �ФѤҤӤԤդ֤פؤ٤ڤۤܤݤޤ� */
  0,0,0,2,0,2,0,2,0,0,0,0,0,0,2,0, /* ����������������� */
  0,0,0,0,0,2,2,0,0,0,0,0,0,0,0,0, /* ���������� */
  0
};


// �ƥ����ȥ�����ɥ��˴�Ϣ����ư��

// �����ƥ९�饹�Υ�åѡ�
void AyuSys::DrawText(char* str) {
	if (main_window)
		main_window->DrawText(str);
}
void AyuSys::SetDrawedText(char* str) {
	if (main_window)
		main_window->SetDrawedText(str);
}
int AyuSys::DrawTextEnd(int flag) {
	if (main_window) return main_window->DrawTextEnd(flag);
	else return 1;
}
int AyuSys::SelectItem(TextAttribute* str, int deal, int type) {
	if (main_window)
		return main_window->SelectItem(str, deal, type);
	else
		return 0;
}
void AyuSys::DrawTextWindow(void) {
	if (main_window)
		main_window->DrawTextWindow();
}
void AyuSys::DeleteTextWindow(void) {
	if (main_window)
		main_window->DeleteTextWindow();
}
void AyuSys::DrawReturnCursor(int type) {
	if (main_window)
		main_window->DrawReturnCursor(type);
}
void AyuSys::DeleteReturnCursor(void) {
	if (main_window)
		main_window->DeleteReturnCursor();
}

void AyuSys::DeleteText(void) {
	if (main_window)
		main_window->DeleteText();
}

void AyuSys::DrawTextPDT(int x, int y, int pdt, const char* text, int c1, int c2, int c3) {
	if (GrpFastMode() == 3) return;
	if (main_window && SyncPDT(pdt) == 0) {
//		main_window->DrawTexttoImage(pdt_buffer[pdt]->original, x, y, text, c1, c2, c3);
	}
}

// �ե���Ȥ�����
void AyuWindow::SetFontSize(int size) {
	if (size != 0) fontsize = size;
	if (font && font_layout) {
		pango_font_description_set_size(font, fontsize * PANGO_SCALE);
		pango_layout_set_font_description(font_layout, font);
	}
}
void AyuWindow::SetFont(char* _fontn) {
	if (_fontn) {
		fontname = new char[strlen(_fontn)+1];
		strcpy(fontname, _fontn);
	}
	if (font) { 
		pango_font_description_free(font);
		font = 0;
	}
	if (font_layout == 0) return;
	if (fontname) {
		font = pango_font_description_from_string(fontname);
	}
	if (font == 0 && default_fontname) {
		font = pango_font_description_from_string(default_fontname);
	}
	if (font == 0) {
		font = pango_font_description_from_string("Sans");
	}
	pango_font_description_set_size(font, fontsize * PANGO_SCALE);
	pango_layout_set_font_description(font_layout, font);
}

// �ե���Ȥν�����ʤ�
void AyuWindow::InitText(void) {
	font_context = gtk_widget_create_pango_context(GTK_WIDGET(wid));
	font_layout = pango_layout_new(font_context);
	fore_color = white_color;
	back_color = black_color;


	drawed_text[0] = 0;
	text_pos = 0;
	text_window_type = -1;
	text_window_brightness = 100;
	assign_window_type = -1;

	text_window_x = 0;
	text_window_y = 0;
	text_window_width = 0;
	text_window_height = 0;

	/* �ƥ����ȼ���Υѥ�᡼���ν���� */
	SetFont(0);
}

/* �ƥ����ȼ���κ�ɸ���Ѳ����줿�顢
** �ѥ�᡼���ѹ�����
*/

void AyuWindow::CalcTextGeom(int type) {
	/* config ������ */
	int moji_size_x, moji_size_y;
	int msg_pos_x, msg_pos_y;
	int msg_size_x, msg_size_y;
	if (type == 1) {
		local_system.config->GetParam("#WINDOW_MSG_POS", 2, &msg_pos_x,&msg_pos_y);
		local_system.config->GetParam("#MESSAGE_SIZE", 2, &msg_size_x, &msg_size_y);
	} else {
		local_system.config->GetParam("#WINDOW_COM_POS", 2, &msg_pos_x,&msg_pos_y);
		local_system.config->GetParam("#COM_MESSAGE_SIZE", 2, &msg_size_x, &msg_size_y);
	}
	local_system.config->GetParam("#MSG_MOJI_SIZE", 2, &moji_size_x, &moji_size_y);
	/* �׻� */
	char_width = moji_size_x*2; char_height = moji_size_y;
	text_x_first = msg_pos_x; text_y_first = msg_pos_y;
	if (local_system.config->GetParaInt("#NVL_SYSTEM")) text_x_first -= char_width;
	if (msg_size_x <= 0) msg_size_x = 23;
	if (msg_size_y <= 0) msg_size_y = 3;
	if (msg_size_x > (local_system.DefaultScreenWidth()-TEXT_X_MARGIN1-TEXT_X_MARGIN2)/char_width)
		msg_size_x = (local_system.DefaultScreenWidth()-TEXT_X_MARGIN1-TEXT_X_MARGIN2)/char_width;
	if (msg_size_y > (local_system.DefaultScreenHeight()-ICON_MARGIN-TEXT_Y_MARGIN1-TEXT_Y_MARGIN2)/char_height)
		msg_size_y = (local_system.DefaultScreenHeight()-ICON_MARGIN-TEXT_Y_MARGIN1-TEXT_Y_MARGIN2)/char_height;
	int size_x = msg_size_x; int size_y = msg_size_y;

	// ����ä�;͵����
	text_x_first += TEXT_X_MARGIN1; size_x--;
	text_y_first += TEXT_Y_MARGIN1;

	text_x_end = text_x_first + char_width*size_x;
	text_y_end = text_y_first + char_height*size_y;

	/* ��§�����ѤΥޡ������Ȥ� */
	/* text_x_first -= char_width; */

	if (text_x_end > local_system.DefaultScreenWidth()-TEXT_X_MARGIN2) {
		text_x_first -= text_x_end-(local_system.DefaultScreenWidth()-TEXT_X_MARGIN2);
		text_x_end = local_system.DefaultScreenHeight()-TEXT_X_MARGIN2;
	}
	if (text_y_end > local_system.DefaultScreenHeight()-ICON_MARGIN-TEXT_Y_MARGIN2) {
		text_y_first -= text_y_end-(local_system.DefaultScreenHeight()-ICON_MARGIN-TEXT_Y_MARGIN2);
		text_y_end = (local_system.DefaultScreenHeight()-ICON_MARGIN-TEXT_Y_MARGIN2);
	}

	text_x_pos = text_x_first;
	text_y_pos = text_y_first;
	kinsoku_flag = KINSOKU_TOP;

	text_first = text_x_first;

	/* retn_x,y ������ */
	if (local_system.config->GetParaInt("#NVL_SYSTEM")) {
		retn_x = -1; retn_y = -1;
	} else {
		retn_x = text_x_end + TEXT_X_MARGIN2;
		retn_y = text_y_end + TEXT_Y_MARGIN2;
		if (retn_x > local_system.DefaultScreenWidth()) retn_x = local_system.DefaultScreenWidth();
		if (retn_y > local_system.DefaultScreenHeight()) retn_y = local_system.DefaultScreenHeight();
		retn_x -= 28; retn_y -= 28;
	}

	/* twinfo ������ */
	if (twinfo == 0) twinfo = new TextWinInfo;
	twinfo->SetLines(size_y);

	return;
}

/* �ƥ����ȼ���Υѥ�᡼�����ѹ����줿
** �ݤˤϿ����� pixmap �������Ƥ�
*/
void AyuWindow::AssignTextPixmap(int type) {
	if (type <= 0) return;
	if (type == assign_window_type && (! local_system.config->IsChanged())) return; // assign ��ɬ�פʤ�
	assign_window_type = type;
	local_system.config->ClearChange();
	FreeTextPixmap(); // �ѹ�������в������Ƥ���
	CalcTextGeom(type);
	int msg_size_x, msg_size_y;
	if (type == 1) {
		local_system.config->GetParam("#MESSAGE_SIZE", 2, &msg_size_x, &msg_size_y);
	} else {
		local_system.config->GetParam("#COM_MESSAGE_SIZE", 2, &msg_size_x, &msg_size_y);
	}
	if (select_pix == 0) {
		select_pix = gdk_pixmap_new(GDK_DRAWABLE(wid->window), (msg_size_x+1)*char_width, char_height*(msg_size_y*SELECT_SIZE), -1);
	}
}

/* �ƥ����ȼ���Υѥ�᡼�����ѹ�����ݤˤ�
** pixmap ��������Ƥ���ɬ�פ�����
*/
void AyuWindow::FreeTextPixmap(void) {
	if (select_pix) {
		g_object_unref(select_pix);
		select_pix = 0;
	}
}

void AyuWindow::DeleteTextWindow(int* old_type, int* old_bri) {
	if (!is_initialized) return;
	if (old_type) *old_type = text_window_type;
	if (old_bri) *old_bri = text_window_brightness;
	DrawTextEnd(1);
	text_window_type = -1; text_window_brightness = 100;
	if (image == image_without_text) return; // �ʤˤ⤷�ʤ�
	image = image_without_text; /* �ƥ������������� */
	/* return cursor ��ä� */
	if (return_cursor_viewed == 2) {
		sys_im->DeleteReturnPixmap(cursor);
		return_cursor_viewed = 1;
	}
	DeleteIconRegion();
	SyncPixmap();
	/* ���̤�ɽ�� */
	if (text_window_width != 0)
		Draw(text_window_x, text_window_y, text_window_x+text_window_width, text_window_y+text_window_height);
	/* text_window �� region �򥯥ꥢ */
	text_window_x = 0; text_window_y=0; text_window_width=0; text_window_height=0;
}

void AyuWindow::DrawTextWindow(int new_window_type, int new_window_brightness) {
	if (!is_initialized) return;
	if (new_window_brightness == -1) new_window_brightness = text_window_brightness;

	/* �����ε��ʲ� */
	if (new_window_brightness < 0) new_window_brightness = 0;
	else if (new_window_brightness > 100) new_window_brightness = 100;
	if (new_window_brightness != 100 && new_window_type == -1) new_window_type = 0;
	if (new_window_brightness == 100 && new_window_type == 0) new_window_type = -1;

	/* �Ѳ����ʤ��ʤ�ʤˤ⤷�ʤ� */
	if (new_window_type == text_window_type &&
		new_window_brightness == text_window_brightness) return;
	/* delete text window? */
	if (new_window_type == -1) {
		DeleteTextWindow();
		return;
	}
	image = image_without_text; /* �ƥ��������������� */
	
	/* ICON �ϻϤᡢ�����ʤ����� */
	DeleteIconRegion();

	/* ���� */
	if (new_window_type != 0) AssignTextPixmap(new_window_type);
	text_window_type = new_window_type;
	text_window_brightness = new_window_brightness;
	/* �ޤ���image �򥳥ԡ� */
	TranslateImage(0,0,local_system.DefaultScreenWidth()-1,local_system.DefaultScreenHeight()-1);
	memcpy( image_with_text->mem,
		image_without_text->mem,
		image_with_text->bpl * image->height);
	/* ���ٻ��� */
	if (new_window_brightness != 100) {
		sys_im->SetBrightness(image_with_text, new_window_brightness, local_system);
	}
	/* �Ȥ�� */
	if (new_window_type > 0)
		sys_im->DrawWaku(image_with_text,
			text_x_first /* +char_width */ -TEXT_X_MARGIN1,
			text_y_first-TEXT_Y_MARGIN1,
			text_x_end-text_x_first /* -char_width */+TEXT_X_MARGIN1+TEXT_X_MARGIN2,
			text_y_end-text_y_first+TEXT_Y_MARGIN1+TEXT_Y_MARGIN2,
			local_system);
	/* �����ΰ����� */
	int new_x, new_y, new_w, new_h;
	if (new_window_brightness != 100 || local_system.config->GetParaInt("#NVL_SYSTEM") != 0) {
		/* ���������� */
		new_x = 0;
		new_y = 0;
		new_w = local_system.DefaultScreenWidth();
		new_h = local_system.DefaultScreenHeight();
	} else {
		new_x = text_x_first-TEXT_X_MARGIN1;
		new_y = text_y_first-TEXT_Y_MARGIN1;
		new_w = text_x_end-text_x_first+TEXT_X_MARGIN1+TEXT_X_MARGIN2;
		new_h = text_y_end-text_y_first+TEXT_Y_MARGIN1+TEXT_Y_MARGIN2;
	}
	/* �����ΰ���� */
	int old_x = text_window_x;
	int old_y = text_window_y;
	int old_w = text_window_width;
	int old_h = text_window_height;
	text_window_x = new_x;
	text_window_y = new_y;
	text_window_width = new_w;
	text_window_height = new_h;
	if (old_w != 0 || old_h != 0) {
		if (old_x+old_w > new_x+new_w) new_w = old_x+old_w-new_x;
		if (old_y+old_h > new_y+new_h) new_h = old_y+old_h-new_y;
		if (old_x < new_x) { new_w += new_x-old_x; new_x = old_x; }
		if (old_y < new_y) { new_h += new_y-old_y; new_y = old_y; }
	}
	image = image_with_text; /* �ƥ���������򳫻� */
	/* �����Ԥ� */
	SyncPixmap();
	Draw(new_x, new_y, new_x+new_w, new_y+new_h);
	SyncPixmap();
	/* ���ߤΥƥ����Ȥ�ɽ�� */
	if (text_window_type > 0) {
		DrawCurrentText();
		DrawTextEnd(1);
	}
	/* ɬ�פʤ顢return pixmap ��ɽ�� */
	if (return_cursor_viewed && text_window_type > 0) DrawReturnCursor();
	/* icon ���֤��᤹ */
	CheckIconRegion();
}

int AyuWindow::CheckBacklogButton(void) {
	if (! is_initialized) return 0;
	if (text_window_type <= 0) return 0; /* ������ɥ���ɽ������Ƥʤ��ΤǤʤˤ⤷�ʤ� */
	int x1, y1, x2, y2;
	if (local_system.config->GetParam("#WINDOW_MSGBK_LBOX_POS", 4, &x1, &y1, &x2, &y2) != 0) return 0;
	x1 = text_x_end+TEXT_X_MARGIN2-1 - x1;
	y1 = text_y_end+TEXT_Y_MARGIN2-1 - y1;
	x2 = text_x_end+TEXT_X_MARGIN2-1 - x2;
	y2 = text_y_end+TEXT_Y_MARGIN2-1 - y2;
	if (x1 > x2 || y1 > y2) return 0;
	if (mouse_x >= x1 && mouse_x <= x2 && mouse_y >= y1 && mouse_y <= y2) return 1;
	return 0;
}

void AyuWindow::DrawReturnCursor(int type) {
	if (! is_initialized) return;
	if (local_system.config->GetParaInt("#NVL_SYSTEM")) type++;
	sys_im->SetCursorType(type);
	DrawReturnCursor();
}
void AyuWindow::DrawReturnCursor(void) {
	if (! is_initialized) return;
	if (text_window_type <= 0) return; // �ʤˤ⤷�ʤ�
	if (return_cursor_viewed == 2) return;
	if (retn_x == -1) { /* ʸ����κǸ��ɽ�� */
		int x = text_x_pos; int y = text_y_pos;
		if (x >= text_x_end) {
			x = text_x_first;
			y += char_height;
		}
		sys_im->DrawReturnPixmap(main_window, GDK_DRAWABLE(main_window),
			GDK_DRAWABLE(pix_image), x,y, cursor); // ��ɸ��Ŭ��
	} else { /* ��ޤä����֤�ɽ�� */
		sys_im->DrawReturnPixmap(main_window, GDK_DRAWABLE(main_window),
			GDK_DRAWABLE(pix_image), retn_x,retn_y, cursor); // ��ɸ��Ŭ��
	}
	return_cursor_viewed = 2;
}

void AyuWindow::DeleteReturnCursor(void) {
	if (! is_initialized) return;
	if (text_window_type <= 0) return; // �ʤˤ⤷�ʤ�
	sys_im->DeleteReturnPixmap(cursor);
	return_cursor_viewed = 0;
}

AyuWindow* current_window;
static int draw_timer_handle = -1;
static int draw_point;
// ʸ�����
// static int draw_handler(void) {
extern "C" int draw_handler(gpointer data) {
	static int isProcess = 0;
	draw_point++;
	if (isProcess) return TRUE;
	isProcess = 1;
	int ret = 0;
	if (current_window != 0)
		ret = current_window->DrawUpdateText(draw_point);
	isProcess = 0;
	// �ƥ����Ȥ�Ǹ�ޤ��������顢�����ޡ���λ
	if (ret == 0) {
		current_window = 0;
		return FALSE;
	} else {
		return TRUE;
	}
}

void AyuWindow::DrawText(char* str) {
	unsigned char buf[1024*16];
	DrawTextWindow();
	kconv( (unsigned char*)str, buf);
	// ������Ĺ���ˤޤ�Ĺ���ʤꤽ���ʤ饯�ꥢ
	if (strlen(drawed_text) > 10000) {
		drawed_text[0] = '\0';
	}
	strcat(drawed_text, (char*)buf); // �ƥ����Ȥ�ä���
	// �ƥ����ȤΥ����ޡ�����
	draw_point = text_pos; current_window = this;
	int wait = local_system.TextSpeed();
	gtk_timeout_remove(draw_timer_handle);
	draw_timer_handle = gtk_timeout_add(wait, draw_handler, (void*)0);
}

int AyuWindow::DrawTextEnd(int flag) {
	// �ʤˤ�ɽ������Ƥʤ����֤ʤ顢�ʤˤ⤷�ʤ�
	if (drawed_text[0] == 0 &&
		text_x_pos == text_x_first &&
		text_y_pos == text_y_first) return 1;
	if (text_window_type <= 0) {
		drawed_text[0] = 0;
		return 1;
	}
	/* flag ��Ω�äƤ����ɽ�����٤�ʸ��������ɽ�� */
	if (flag) {
		DrawUpdateText(10000);
		current_window = 0;
	}
	return current_window == 0;
}
void AyuWindow::SetDrawedText(char* str) {
	unsigned char buf[1024*16];
	kconv( (unsigned char*) str, (unsigned char*)buf);
	if (text_window_type > 0) {
		/* �ƥ����Ȥ����褵��Ƥ��� */
		if (strcmp((char*)drawed_text, (char*)buf) == 0) return;
		DeleteText();
		DrawText(str);
		DrawTextEnd(1);
	} else { /* �ƥ����ȥ�����ɥ��������Ƥʤ� */
		strcpy((char*)drawed_text, (char*)buf);
		text_pos = strlen(drawed_text);
	}
	return;
}


inline int is_kinsoku(unsigned char c1, unsigned char c2) {
	if (c2 < 0xa1) return 0;
	if (c1 == 0xa4 || c1 == 0xa5) return kinsoku_table2[c2-0xa0];
	else if (c1 == 0xa1) return kinsoku_table1[c2-0xa0];
	return 0;
}

// text ��ʸ������
void AyuWindow::DrawOneChar(int flag) {
	int text_pos_orig = text_pos;
	if (text_pos > 10000) {
		drawed_text[0] = 0; text_pos = 0;
		return;
	}
	if (text_pos && next_scroll) {
		next_scroll = 0;
		ScrollupText(1);
	}
	next_scroll = 0;
	// ʸ��������äƤ���Ф����
	if (drawed_text[text_pos] == 0) {
		if (twinfo) twinfo->End();
	}
	if (drawed_text[text_pos] < 0 && drawed_text[text_pos+1] == 0) {
		text_pos++;
		if (twinfo) twinfo->End();
		return;
	}
	unsigned char c1, c2;
	// �ǽ��ʸ������Ƭ��gc ������(ʸ��������ˤ���)
	if (text_pos == 0) {
		fore_color = white_color;
		back_color = black_color;
		if (twinfo) {
			twinfo->Start();
			twinfo->NextLine(text_first, text_pos, fore_color, back_color);
		}
	}
	int is_kin = NO_KINSOKU;
	// 1�Х���ʸ�� : ���褷�ʤ�
	if (drawed_text[text_pos] > 0) {
		// ���ԡ����⸵���᤹
		if (drawed_text[text_pos] == '\n') {
			text_x_pos = text_x_first;
			/*  ���ä�������֤�ɬ�פʤ���
			** if (kinsoku_flag & KINSOKU_KAIWA)
			**	text_x_pos += char_width;
			*/
			text_first = text_x_pos;
			kinsoku_flag &= KINSOKU_KAIWA;
			kinsoku_flag |= KINSOKU_TOP;
			text_y_pos += char_height;
			text_pos++;
			fore_color = white_color;
			back_color = black_color;
			if ( (!flag) && twinfo) twinfo->NextLine(text_first, text_pos, fore_color, back_color);
			if ( (!flag) && text_y_pos > text_y_end-char_height) next_scroll = true;
			return;
		} else if (drawed_text[text_pos] == '\r') {
			text_pos++;
			if (drawed_text[text_pos] == '\n') {
				text_pos++;
				text_x_pos = text_first;
				text_y_pos += char_height;
				if ( (!flag) && twinfo) twinfo->NextLine(text_first, text_pos, fore_color, back_color);
				if ( (!flag) && text_y_pos > text_y_end-char_height) next_scroll = true;
			} else {
				text_x_pos = text_x_first;
				if (kinsoku_flag & KINSOKU_KAIWA)
					text_x_pos += char_width;
				text_first = text_x_pos;
			}
			kinsoku_flag &= KINSOKU_KAIWA;
			kinsoku_flag |= KINSOKU_TOP;
			return;
		} else if (drawed_text[text_pos] == 1) {
			text_pos++;
			return;
		} else if (drawed_text[text_pos] == 2) {
			// �ƥ����Ȥο��ѹ�
			text_pos++;
			int index = drawed_text[text_pos++];
			if (index == 0x7f) index = 0;
			COLOR_TABLE col = local_system.ColorTable(index);
			GdkColor new_col = {0, col.c1, col.c2, col.c3};
			gdk_colormap_alloc_color(gdk_drawable_get_colormap(GDK_DRAWABLE(main_window)), &new_col, FALSE, TRUE);
			fore_color = new_col;
			return;
		} else if (drawed_text[text_pos] == 3) {
			// �ƥ����Ȥο��ѹ�
			text_pos++;
			int index = drawed_text[text_pos++];
			if (index == 0x7f) index = 0;
			COLOR_TABLE col = local_system.ColorTable(index);
			GdkColor new_col = {0, col.c1, col.c2, col.c3};
			gdk_colormap_alloc_color(gdk_drawable_get_colormap(GDK_DRAWABLE(main_window)), &new_col, FALSE, TRUE);
			back_color = new_col;
			return;
		}
		// int c = drawed_text[text_pos]; c &= 0x7f;
		// c1 = han_to_zen_table[c*2];
		// c2 = han_to_zen_table[c*2+1];
		c1 = drawed_text[text_pos];
		c2 = 0;
		text_pos += 1;
		if (c1 == ',' || c1 == '.') is_kin = KINSOKU_TAIL;
		else if (c1 == '"' || c1 == '(') is_kin = KINSOKU_HEAD;
	} else if (drawed_text[text_pos] == (char)0x8e) { // hankaku kana
		int c = int(drawed_text[text_pos+1]) & 0xff;
		c1 = han_to_zen_table[c*2];
		c2 = han_to_zen_table[c*2+1];
		text_pos += 2;
	} else {
		c1 = drawed_text[text_pos];
		c2 = drawed_text[text_pos+1];
		text_pos += 2;
		is_kin = is_kinsoku(c1,c2);
	}
	if (is_kin == NAME_HEAD) {
		// ���褷�ʤ���̾����Ϥ��̡�
		if (kinsoku_flag & KINSOKU_KAIWA) {
			/* ���˲���ʸ�桧�ʤˤ⤷�ʤ� */
		} else if (text_x_pos == text_x_first) {
			/* ����ʸƬ�Ǥ� text_x_pos �򥤥�ǥ�� */
			/* ��������ɬ�פʤ� text_x_pos += char_width; */
		}
		kinsoku_flag |= KINSOKU_KAIWA;
		return;
	} else if (is_kin == NAME_TAIL) {
		// ̾����Ϥ��̤�����
		// text_first �򥻥å�
		if (kinsoku_flag & KINSOKU_KAIWA) {
			if (c2 == 0) text_x_pos += char_width/2;
			else text_x_pos += char_width;
		}
		text_first = text_x_pos;
		if (text_first >= text_x_end-char_width)
			text_first = text_x_end-char_width;
		kinsoku_flag |= KINSOKU_TOP2;
		return;
	}
	/* �ƥ�������Ƭ�ǡ֤ʤɤ��褿��硢���ιԤ��飱ʸ����Ƭ�򤺤餹 */
	bool text_first_set = false;
	if ( (kinsoku_flag & KINSOKU_TOP) && is_kin == KINSOKU_HEAD) {
		text_first_set = true;
	} else if ( (kinsoku_flag & KINSOKU_TOP2) && is_kin == KINSOKU_HEAD) {
		if (c2 == 0)
			text_first -= char_width / 2;
		text_x_pos -= char_width;
		text_first_set = true;
	}
	/* �ƥ����������Ƕ�§������Ԥ������̽��� */
	if (text_x_pos >= text_x_end) {
		if (text_x_pos == text_x_end && is_kin == KINSOKU_TAIL) {
			// �ʤˤ⤷�ʤ�
		} else {
			// ���Ԥ��롧��ü return ���ơ�ʸ���Ϻ�����
			text_x_pos = text_first;
			text_y_pos += char_height;
			if (c1 != 0xa1 || c2 != 0xa1) text_pos = text_pos_orig;
			if ( (!flag) && twinfo) twinfo->NextLine(text_first, text_pos, fore_color, back_color);
			if ( (!flag) && text_y_pos > text_y_end-char_height) next_scroll = true;
			return;
		}
	}
	if (c1 == 0xa1 && c2 == 0xf6) { // gaiji // from akz. version
		int i = text_pos; int index = 0;
		for (; drawed_text[i] != 0; i+=2) {
			unsigned char c1 = drawed_text[i];
			unsigned char c2 = drawed_text[i+1];
			if (c1 == 0xa3 && (c2 >= 0xb0 && c2 <= 0xb9)) {
				index = index*10 + (c2-0xb0);
			} else break;
		}
		if (i != text_pos) {
			text_pos = i;
                	DrawGaiji(index, flag);
			text_x_pos += 26; // default size of gaiji (?)
		}
	} else if (flag == 0) {
		// ���̤�����
		char c[30];
		char cf[3];
		cf[0] = c1;
		cf[1] = c2;
		cf[2] = 0;
		conv_euc(cf, c, 30);
		pango_layout_set_text(font_layout, c, strlen(c));
		if (font) {
			gdk_draw_layout_with_colors(GDK_DRAWABLE(pix_image), gc, text_x_pos+1, text_y_pos+1, font_layout, &back_color, 0);
			gdk_draw_layout_with_colors(GDK_DRAWABLE(main->window), gc, text_x_pos+1, text_y_pos+1, font_layout, &back_color, 0);
			gdk_draw_layout_with_colors(GDK_DRAWABLE(pix_image), gc, text_x_pos, text_y_pos, font_layout, &fore_color, 0);
			gdk_draw_layout_with_colors(GDK_DRAWABLE(main->window), gc, text_x_pos, text_y_pos, font_layout, &fore_color, 0);
			cursor->Draw();
			int w=0,h;
			pango_layout_get_pixel_size(font_layout, &w, &h);
			// if (w < char_width) w = char_width; // �ǥե���Ȥ���ʸ���������������Τǽ���
			text_x_pos += w;
		}
		kinsoku_flag &= KINSOKU_KAIWA;
	} else {
		// select pixmap ������(���Ϲ�)
		char c[6];
		char cf[3];
		cf[0] = c1;
		cf[1] = c2;
		cf[2] = 0;
		conv_euc(cf, c, 30);
		pango_layout_set_text(font_layout, c, strlen(c));
		int x = text_x_pos-text_x_first; int y = text_y_pos-text_y_first;
		if (font) {
			gdk_draw_layout_with_colors(GDK_DRAWABLE(select_pix), gc, x+1, y+1, font_layout, &white_color, 0);
			gdk_draw_layout_with_colors(GDK_DRAWABLE(select_pix), gc, x, y, font_layout, &black_color, 0);
			int w=0,h;
			pango_layout_get_pixel_size(font_layout, &w, &h);
			// if (w < char_width) w = char_width; // �ǥե���Ȥ���ʸ���������������Τǽ���
			text_x_pos += w;
		}
		kinsoku_flag &= KINSOKU_KAIWA;
	}
	if (text_first_set) {
		text_first = text_x_pos; // zenkaku mode
	}
}

// ���ߤ� text �ΰ��֤ޤ����褷�ʤ���
void AyuWindow::DrawCurrentText(void) {
	int pos = text_pos;
	char buf[1024*16]; strcpy(buf, drawed_text);
	// �ޤ����ƥ����Ȥ�ä�
	DeleteText();
	// ��ľ��
	strcpy(drawed_text, buf);
	DrawUpdateText(pos);
	return;
}

// ���ޤǤΥƥ����Ȥ˲ä���pos ���֤ޤ�����
// 0 ���֤����顢���褪���
int AyuWindow::DrawUpdateText(int pos) {
	if (image != image_with_text) return 0;
	if (text_pos >= pos) return 1;
	int old_text_x_pos = text_x_pos;
	int old_text_y_pos = text_y_pos;
	while(text_pos < pos && drawed_text[text_pos] != '\0') {
		DrawOneChar();
	}
	/* true type font �� image ��ʸ�����񤫤�Ƥ���Τǡ�
	**  �񤫤줿ʸ���� pixmap ��ž��
	*/
	/* �ޤ�����ɸ�׻� */
	int x,y,w,h;
	if (old_text_y_pos != text_y_pos) {
		x = text_x_first; w = text_x_end - text_x_first + char_width;
		y = old_text_y_pos; h = text_y_pos - old_text_y_pos + char_height + 1;
		if (h < 0) { y = text_y_first; h = text_y_end - text_y_first;}
	} else {
		// ��§�����ǡ֤ν����򤷤���硢w = 0 �ˤʤ�Τ����
		if (old_text_x_pos == text_x_pos &&
			old_text_x_pos == text_first &&
			old_text_x_pos != text_x_first) {
			x = text_first - char_width; w = char_width;
		} else {
			x = old_text_x_pos; w = text_x_pos - old_text_x_pos;
		}
		y = old_text_y_pos; h = char_height + 1;
	}
	if (drawed_text[text_pos] == '\0') return 0;
	return 1;
}

void AyuWindow::DeleteText(void) {
	// �ʤˤ�ɽ������Ƥʤ����֤ʤ顢�ʤˤ⤷�ʤ�
	if (drawed_text[0] == 0 &&
		text_x_pos == text_x_first &&
		text_y_pos == text_y_first) return;
	if (text_window_type > 0) {
		/* �õ�줿������ɥ���ɽ������ */
		/* �ޥ������������񤤤����֤ǡ����̤�ɽ�� */
		gdk_draw_image(GDK_DRAWABLE(pix_image), gc, image, text_x_first,text_y_first, text_x_first,text_y_first, text_x_end-text_x_first+char_width,text_y_end-text_y_first+1);
		DisplaySync();
		cursor->DrawImage(image);
		gdk_draw_image(GDK_DRAWABLE(main->window), gc, image, text_x_first,text_y_first, text_x_first,text_y_first, text_x_end-text_x_first+char_width,text_y_end-text_y_first+1);
		DisplaySync();
		cursor->RestoreImage();
		/* ɬ�פʤ顢return pixmap ��ɽ�� */
		if (return_cursor_viewed) DrawReturnCursor();
	}
	drawed_text[0] = 0;
	text_x_pos = text_x_first;
	text_y_pos = text_y_first;
	kinsoku_flag = KINSOKU_TOP;
	text_first = text_x_first;
	text_pos = 0;
}

struct SelectGeom {
	int x, y;
	int width, height;
	void SetFirst(int _x, int _y) { x= _x; y = _y;}
	void SetSecond(int _x, int _y) { width = _x-x; height = _y-y; }
	int InScreen(int _x, int _y) {
		if (_x < x || _y < y) return 0;
		_x -= x; _y -= y;
		if (_x >= width || _y >= height) return 0;
		return 1;
	}
	// ���褹��
	void Draw(GdkGC* gc,GtkWidget* main, GdkPixmap* back, P_CURSOR* cur, GdkPixmap* orig, int x0, int y0, int h) {
		int src_x = x-x0; int src_y = y-y0+h;
		int dest_x = x; int dest_y = y;
		gdk_draw_drawable(GDK_DRAWABLE(back), gc, orig, src_x, src_y, dest_x, dest_y, width, height);
		gdk_draw_drawable(GDK_DRAWABLE(main->window), gc, orig, src_x, src_y, dest_x, dest_y, width, height);
		cur->UpdateBuffer();
		cur->Draw_without_Delete();
	}
};

int AyuWindow::SelectItem(TextAttribute* textlist, int deal, int type) {
	if (local_system.IsRandomSelect()) {// ������å������ɤ����Ф��⡼��
		return AyuSys::Rand(deal) + 1;
	}
	char buf[1024*16]; int i;
	DeleteText();
	int window_type_orig = text_window_type;
	DrawTextWindow(type);
	if (deal > SELECT_SIZE) deal = SELECT_SIZE;
	/* �ޤ������̤����� */
	SelectGeom geom[SELECT_SIZE];
	for (i=0; i<deal; i++) {
		buf[0] = buf[1] = 1; /* 1 ��ʸ������κݡ�̵�뤵��� */
		kconv( (unsigned char*)(textlist[i].Text()), (unsigned char*)(buf+2));
		if (textlist[i].Attribute() == 0x20 || textlist[i].Attribute() == 0x22) { /* ���դ� */
			buf[0] = 2; /* escape sequence */
			buf[1] = textlist[i].Value();
		}
		if (textlist[i].Attribute() == 0x21) { /* ɽ�����ʤ������ */
			geom[i].SetFirst(text_x_pos, text_y_pos);
			geom[i].SetSecond(text_x_pos, text_y_pos);
		} else { /* ɽ����������� */
			strcat(drawed_text, buf);
			strcat(drawed_text, "\r\n");
			text_first = text_x_first;
			text_x_pos = text_x_first;
			kinsoku_flag = KINSOKU_TOP;
			geom[i].SetFirst(text_x_pos, text_y_pos);
			DrawUpdateText(10000);
			geom[i].SetSecond(text_x_end+char_width, text_y_pos-SPACE_BOT);
		}
	}
	/* pixmap �˥��ԡ� */
	int width = text_x_end - text_x_first + char_width;
	int height = text_y_end - text_y_first;
	gdk_draw_drawable(GDK_DRAWABLE(select_pix), gc, pix_image, text_x_first, text_y_first, 0, height, width, height);
	/* pixmap ������ */
	gdk_gc_set_foreground(gc, &white_color);
	gdk_draw_rectangle(GDK_DRAWABLE(select_pix), gc, 1, 0, 0, width, height);
	text_x_pos = text_x_first; text_y_pos = text_y_first; text_first = text_x_first;
	kinsoku_flag = KINSOKU_TOP;
	text_pos = 0; while(drawed_text[text_pos] != '\0') {
		if (text_pos>0&&drawed_text[text_pos-1] == '\n') {
			text_first = text_x_first;
			text_x_pos = text_x_first;
			kinsoku_flag = KINSOKU_TOP;
		}
		DrawOneChar(1);
	}
	// ���쥯�Ȥʤ�
	int selection = -1;
	P_CURSOR* cur = cursor;
	int x=-100, y=-100, clicked, now_click;
	int old_x=x, old_y=y;
	while(1) {
		int cur_sel = -1; int changed = 0;
		int l,r,u,d,esc;
		// �ޥ�����������ξ��֤�����
		GetMouseState(x, y, clicked, now_click);
		ClearMouseState();
		GetKeyCursorInfo(l,r,u,d,esc);
		if (old_x != x || old_y != y) {
			// �ޥ�������ư����
			changed = 1;
			for (i=0; i<deal; i++) {
				if (geom[i].InScreen(x,y)) {
					cur_sel = i;
					break;
				}
			}
		} else if (u || d ) {
			// �������뤬�����줿(Up��ͥ�������
			changed = 1;
			if (u) {
				cur_sel = selection-1;
				while (cur_sel >= 0 &&
					(textlist[cur_sel].Attribute() == 0x22 || textlist[cur_sel].Attribute() == 0x21)) cur_sel--;
				if (cur_sel < 0) cur_sel = selection;
			} else if (d) {
				cur_sel = selection+1;
				while(cur_sel <= deal-1 &&
					(textlist[cur_sel].Attribute() == 0x22 || textlist[cur_sel].Attribute() == 0x21)) cur_sel++;
				if (cur_sel >= deal) cur_sel = selection;
			}
		} else if (esc) { // �Хå����⡼�ɤ�����
		/*
		**	selection = -1;
		**	break;
		*/
			/* �Ȥꤢ�������Υ�å���������� */
			selection = -2;
			break;
		} else if (l || clicked == 3) { /* �����ۥ�����ξ� */
			selection = -2;
			break;
		}
				
		old_x = x; old_y = y;
		// ��������С����ľ��
		if (changed && cur_sel != selection) {
			if (selection != -1) {
				// ��������ξõ�
				geom[selection].Draw(gc,  main, pix_image, cur, select_pix,
					text_x_first, text_y_first, height);
			}
			if ((cur_sel != -1 && textlist[cur_sel].Attribute() == 0x22) || textlist[cur_sel].Attribute() == 0x21) selection = -1;
			else selection = cur_sel;
			if (selection != -1) {
				// �������������
				geom[selection].Draw(gc, main, pix_image, cur, select_pix,
					text_x_first, text_y_first, 0);
				// �Ԥ衩
				local_system.PlaySE(0);
			}
		}
		if (clicked == 0 && selection != -1) break;
		if (local_system.IsIntterupted()) break;
		local_system.CallProcessMessages();
		usleep(1000);
	}
	if (local_system.IsIntterupted() || selection < 0) {
		if (local_system.IsIntterupted() != 2) {
			DrawTextWindow(window_type_orig);
		}
		if (selection == -1) return -1;
		else if (selection == -2) return -2; /* ���Υƥ����Ȥ���� */
		else return 0;
	}
	// �Ϥ���
	local_system.PlaySE(1);
	// �ʤ�ɤ��֤�
	void* handle = local_system.setTimerBase();
	geom[selection].Draw(gc, main, pix_image, cur, select_pix,
		text_x_first, text_y_first, height);
	local_system.FlushScreen();
	local_system.waitUntil(handle, 50);
	geom[selection].Draw(gc, main, pix_image, cur, select_pix,
		text_x_first, text_y_first, 0);
	local_system.FlushScreen();
	local_system.waitUntil(handle, 100);
	
	geom[selection].Draw(gc, main, pix_image, cur, select_pix,
		text_x_first, text_y_first, height);
	local_system.FlushScreen();
	local_system.waitUntil(handle, 140);
	geom[selection].Draw(gc, main, pix_image, cur, select_pix,
		text_x_first, text_y_first, 0);
	local_system.FlushScreen();
	local_system.waitUntil(handle, 180);
	
	geom[selection].Draw(gc, main, pix_image, cur, select_pix,
		text_x_first, text_y_first, height);
	local_system.FlushScreen();
	local_system.waitUntil(handle, 200);
	geom[selection].Draw(gc, main, pix_image, cur, select_pix,
		text_x_first, text_y_first, 0);
	local_system.FlushScreen();
	local_system.waitUntil(handle, 220);
	
	geom[selection].Draw(gc, main, pix_image, cur, select_pix,
		text_x_first, text_y_first, height);
	local_system.FlushScreen();
	local_system.waitUntil(handle, 230);
	
	local_system.freeTimerBase(handle);
	DeleteText();
	return selection +1;
}

void AyuSys::SetTitle(char* t) {
	if (title) delete[] title;
	title = 0;
	if (t == 0) {
		char* tt = strdup("�����ȥ�");
		char buf[1024];
		main_window->conv_euc(tt, buf);
		main_window->SetMenuTitle(buf);
		free(tt);
		return;
	}
	title = new char[strlen(t)*2+1];
	strcpy(title, t);
	const char* window_caption = config->GetParaStr("#CAPTION");
	if (main_window) {
		char buf[1024];
		if (window_caption) main_window->conv_sjis(window_caption, buf);
		else strcpy(buf, "xkanon");
		strcat(buf, "  ");
		char* menu_title = buf + strlen(buf);
		main_window->conv_sjis(t, menu_title);
		gtk_window_set_title(GTK_WINDOW(main_window->wid), buf);
		main_window->SetMenuTitle(menu_title);
	}
	return;
}

#define ICON_X_MIN 320
#define ICON_X_MAX (local_system.DefaultScreenWidth()-1)
#define ICON_Y_MIN (local_system.DefaultScreenHeight()-37)
#define ICON_Y_MAX (local_system.DefaultScreenHeight()-4)
#define ICON_Y_DRAW (local_system.DefaultScreenHeight()-48)
#define ICON_MASK_BPL 320
#define ICON_WIDTH 320
#define ICON_HEIGHT 33

void AyuWindow::DeleteIconRegion(void) {
	if (di_image_text == 0 || di_image_icon == 0) return;
	if (icon_state != ICON_NODRAW) {
		icon_state = ICON_NODRAW;
		CopyRect(*di_image_text, ICON_X_MIN, ICON_Y_MIN, *di_image_icon_back, 0, 0, ICON_WIDTH, ICON_HEIGHT);
		if (image == image_with_text) {
			Draw(ICON_X_MIN, ICON_Y_MIN, ICON_X_MIN+ICON_WIDTH-1, ICON_Y_MIN+ICON_HEIGHT-1);
			SyncPixmap();
		}
	}
}

void AyuWindow::CheckIconRegion(void) {
	if (mouse_x == DELETED_MOUSE_X ||
	    mouse_y < ICON_Y_DRAW) {
		DeleteIconRegion();
		return;
	}
	if (di_image_text == 0 || di_image_icon == 0 || image != image_with_text) return;
	if (text_window_type <= 0) return;
	/* ������Ƥ��� icon �Υ����å� */
	int pressed_icon = 0;
	if (di_image_icon->Mask()[ (mouse_y-ICON_Y_MIN)*ICON_WIDTH + (mouse_x-ICON_X_MIN) ] ) {
		int x = mouse_x - ICON_X_MIN;
		if (x <= 52) {
			pressed_icon = 1;
		} else if (x < 56) {
		} else if (x <= 108) {
			pressed_icon = 2;
		} else if (x < 112) {
		} else if (x <= 144) {
			pressed_icon = 3;
		} else if (x < 163) {
		} else if (x <= 195) {
			pressed_icon = 4;
		} else if (x < 199) {
		} else if (x <= 251) {
			pressed_icon = 5;
		} else if (x < 255) {
		} else if (x <= 307) {
			pressed_icon = 6;
		}
	}
	/* icon ������ */
	ICON_STATE new_icon_state = ICON_STATE(ICON_DRAW | ( local_system.NowInKidoku() ? ICON_FAST : ICON_NODRAW) |( local_system.NowInKidoku() ? ICON_AUTO : ICON_NODRAW));
	if (icon_state != new_icon_state) {
		if (icon_state == ICON_NODRAW) {
			CopyRect(*di_image_icon_back, 0, 0, *di_image_text, ICON_X_MIN, ICON_Y_MIN, ICON_WIDTH, ICON_HEIGHT);
		} else {
			CopyRect(*di_image_text, ICON_X_MIN, ICON_Y_MIN, *di_image_icon_back, 0, 0, ICON_WIDTH, ICON_HEIGHT);
		}
		CopyRect(di_image_text, ICON_X_MIN, ICON_Y_MIN, di_image_icon, 0, 0, 53, ICON_HEIGHT);
		CopyRect(di_image_text, ICON_X_MIN+56, ICON_Y_MIN, di_image_icon, 56, 0, 53, ICON_HEIGHT);
		CopyRect(di_image_text, ICON_X_MIN+112, ICON_Y_MIN, di_image_icon, 112, 0, 33, ICON_HEIGHT);
		if (local_system.NowInKidoku())
			CopyRect(di_image_text, ICON_X_MIN+163, ICON_Y_MIN, di_image_icon, 163, 0, 33, ICON_HEIGHT);
		else
			CopyRect(di_image_text, ICON_X_MIN+163, ICON_Y_MIN, di_image_icon, 163, ICON_HEIGHT, 33, ICON_HEIGHT);
		if (local_system.NowInKidoku())
			CopyRect(di_image_text, ICON_X_MIN+199, ICON_Y_MIN, di_image_icon, 199, 0, 53, ICON_HEIGHT);
		else
			CopyRect(di_image_text, ICON_X_MIN+199, ICON_Y_MIN, di_image_icon, 199, ICON_HEIGHT, 53, ICON_HEIGHT);
		CopyRect(di_image_text, ICON_X_MIN+255, ICON_Y_MIN, di_image_icon, 255, 0, 53, ICON_HEIGHT);
		Draw(ICON_X_MIN, ICON_Y_MIN, ICON_X_MIN+ICON_WIDTH-1, ICON_Y_MIN+ICON_HEIGHT-1);
		SyncPixmap();
		icon_state = new_icon_state;
	}
	return;
}

bool AyuWindow::PressIconRegion(void) {
	if (di_image_text == 0 || di_image_icon == 0 || image != image_with_text) return false;
	if (mouse_x == DELETED_MOUSE_X) return false;
	if (mouse_y < ICON_Y_DRAW) return false;

	// icon ��¸�ߤ����ΰ褫�Υ����å�
	if (mouse_x < ICON_X_MIN || mouse_x > ICON_X_MAX) return false;
	if (mouse_y < ICON_Y_MIN || mouse_y > ICON_Y_MAX) return false;
	// icon ��������Ƥ���
	if (di_image_icon->Mask()[ (mouse_y-ICON_Y_MIN)*ICON_WIDTH + (mouse_x-ICON_X_MIN) ] ) {
		int x = mouse_x - ICON_X_MIN;
		if (x <= 52) {
			local_system.SetBacklog(-3); // �������ޤ����
			return true;
		} else if (x < 56) {
		} else if (x <= 108) {
			local_system.SetBacklog(50); // 50message �ȤФ�
			return true;
		} else if (x < 112) {
		} else if (x <= 144) {
			local_system.SetBacklog(2); // 1message���
			return true;
		} else if (x < 163) {
		} else if (x <= 195) {
			if (local_system.NowInKidoku())
				local_system.SetTextAutoMode(true);
			return true;
		} else if (x < 199) {
		} else if (x <= 251) {
			if (local_system.NowInKidoku())
				local_system.SetTextFastMode(true);
			return true;
		} else if (x < 255) {
		} else if (x <= 307) {
			local_system.StartTextSkipMode(-1); // ���������ޤǿʤ�
			return true;
		}
	}
	return false;
}
void AyuWindow::ScrollupText(int lines) {
	if (twinfo->size() == 0) return;
	if (lines > twinfo->size()) // ¿�̤Υ�����������ƥ����Ȥ��õ�
		lines = twinfo->size() - 1;
	// �ƥ����Ȥ���
	char buf[1024*16];
	int pos = twinfo->TextPos(lines);
	int len = strlen(drawed_text);
	if (len < pos) pos = len;
	strcpy(buf, drawed_text+pos);
	int new_pos = text_pos - pos;
	DeleteText();
	// text ����������
	text_first = twinfo->TextFirst(lines);
	text_x_pos = text_first;
	fore_color = twinfo->Color(lines);
	back_color = twinfo->BackColor(lines);
	// ����
	strcpy(drawed_text, buf);
	DrawUpdateText(new_pos);
}
void AyuWindow::LoadGaijiTable(void) {
        const char *str;
        char *str2;

	gaiji_pdt = 0;
        str   = local_system.config->GetParaStr("#EXFONT_N_NAME");
        if (str == NULL) return;
        str2  = strdup(str);
        xcont = local_system.config->GetParaInt("#EXFONT_N_XCONT");
        ycont = local_system.config->GetParaInt("#EXFONT_N_YCONT");
        xsize = local_system.config->GetParaInt("#EXFONT_N_XSIZE");
        ysize = local_system.config->GetParaInt("#EXFONT_N_YSIZE");
        // printf("EXFONT : %s | grid : %dx%d | size : %dx%d\n", str, xcont, ycont, xsize, ysize);

        gaiji_pdt = local_system.ReadPDTFile(str2);
        free(str2);
        if (!gaiji_pdt) {
                printf("Cannot load Gaiji table!\n");
                return;
        } else {
                printf("Gaiji table loaded\n");
        }

}

void AyuWindow::DrawGaiji(int index, int flag) {
        int index_x, index_y;

        // No gaiji_pdt present
        if (gaiji_pdt == NULL) return;

        index_x = index % xcont;
        index_y = index / xcont;
	char* mem = gaiji_pdt->data + (index_y * gaiji_pdt->bpl * ysize + index_x * gaiji_pdt->bypp * xsize);
        // Naive way to draw a gaiji but it works
        if (font) {
		gdk_gc_set_foreground(gc, &back_color);
                for (int y = 0; y < ysize; y++) {
			char* m = mem + gaiji_pdt->bpl * y;;
                        for (int x = 0; x < xsize; x++) {
                                if (*m) {
                                        if (!flag) {
                                                gdk_draw_point(GDK_DRAWABLE(pix_image), gc, text_x_pos+x+1, text_y_pos+y+1);
                                                gdk_draw_point(GDK_DRAWABLE(main->window),gc, text_x_pos+x+1, text_y_pos+y+1);
                                        } else
                                                gdk_draw_point(GDK_DRAWABLE(select_pix),gc, text_x_pos+x+1, text_y_pos+y+1);
                                }
				m += gaiji_pdt->bypp;
                        }
                }
		gdk_gc_set_foreground(gc, &fore_color);
                for (int y = 0; y < ysize; y++) {
			char* m = mem + gaiji_pdt->bpl * y;;
                        for (int x = 0; x < xsize; x++) {
                                if (*m) {
                                                if (!flag) {
                                                        gdk_draw_point(GDK_DRAWABLE(pix_image),gc, text_x_pos+x, text_y_pos+y);
							gdk_draw_point(GDK_DRAWABLE(main->window), gc, text_x_pos+x, text_y_pos+y);
                                                } else
                                                        gdk_draw_point(GDK_DRAWABLE(select_pix),gc, text_x_pos+x, text_y_pos+y);
                                }
				m += gaiji_pdt->bypp;
                        }
                }
        } else {
        }
        if (!flag) cursor->Draw();
}

