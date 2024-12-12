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
#include "window.h"

int AyuWindow::OpenNameDialog(AyuSys::NameInfo* names, int list_deal)
{
	return 1;
}

/* 名前入力エントリ（一つだけ） */
class NameSubEntry {
};
NameSubEntry* AyuSys::OpenNameEntry(int x, int y, int width, int height, const COLOR_TABLE& fore, const COLOR_TABLE& back) {
	return 0;
}
NameSubEntry* AyuWindow::OpenNameEntry(int x, int y, int width, int height, const COLOR_TABLE& fore, const COLOR_TABLE& back) {
	return 0;
}

const char* AyuSys::GetNameFromEntry(NameSubEntry* entry) {
	return "";
}
void AyuSys::SetNameToEntry(NameSubEntry* entry, const char* name) {
}
void AyuSys::CloseNameEntry(NameSubEntry* entry){
}
void AyuSys::CloseAllNameEntry(void) {
}
