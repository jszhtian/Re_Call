/* window_stab.cc
 *  window を使わない状態で senario の decode を行うための
 *  stab メソッドを定義する
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

#include "system.h"

int AyuSys::SyncPDT(int pdt){}
int AyuSys::DisconnectPDT(int pdt){}
void AyuSys::InitWindow(AyuWindow* main) {
}

void AyuSys::FinalizeWindow(void) {
}

void AyuSys::CallUpdateFunc(void) {
}
void AyuSys::CallUpdateFunc(int x1, int y1, int x2, int y2) {
}

void AyuSys::CallProcessMessages(void) {
}
void AyuSys::WaitNextEvent(void) {
}

void AyuSys::FlushScreen(void)
{
}

void AyuSys::UpdateMenu(char** save_titles)
{
}


void AyuSys::GetMouseInfo(int& x, int& y, int& clicked) {
	x=0;y=0;clicked=0;
}
void AyuSys::ClearMouseInfo(void) {
}
void AyuSys::SetMouseMode(int a) {}
void AyuSys::GetKeyCursorInfo(int& left, int& right, int& up, int& down, int& esc) {
	left=right=up=down=esc=0;
}

void AyuSys::DrawReturnCursor(int t){}
void AyuSys::DeleteReturnCursor(void) {}
void AyuSys::DeleteText(void) {}
void AyuSys::DrawTextWindow(void) {}
int AyuSys::DrawTextEnd(int x) {}
int AyuSys::SelectLoadWindow(void) {return 0;}
void AyuSys::ChangeMenuTextFast(void) {}
void AyuSys::ShowMenuItem(void* items, int active) {}
void AyuSys::Shake(int n) {}
void AyuSys::BlinkScreen(int c1,int c2, int c3, int w, int c) {}
void AyuSys::MakePopupWindow(void) {}
int AyuSys::OpenNameDialog(NameInfo* names, int list_deal) { return 1; }
NameSubEntry* AyuSys::OpenNameEntry(int x, int y, int width, int height, const COLOR_TABLE& fore, const COLOR_TABLE& back) { return 0; }
void AyuSys::SetNameToEntry(NameSubEntry* entry, const char* name) { return; }
const char* AyuSys::GetNameFromEntry(NameSubEntry* entry) { return ""; }
void AyuSys::CloseNameEntry(NameSubEntry* entry){ return; }
void AyuSys::CloseAllNameEntry(void) { return; }
int AyuSys::GetWindowID(void) { return 0; }

void AyuSys::SetTitle(char* t) {
	if (title) delete[] title;
	title = 0;
	if (t == 0) return;
	title = new char[strlen(t)*2+1];
	kconv( (unsigned char*)t, (unsigned char*)title);
	return;
}
void AyuSys::DrawTextPDT(int x, int y, int pdt, const char* text, int c1, int c2, int c3) {
}
