/* system_graphics_stab.cc
 *     system_graphics.cc で定義されるメソッドに対して、
 *     DI_Image が存在しないときのための stabメソッド
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

void AyuSys::ClearAllPDT(void) {}
void AyuSys::ClearPDTBuffer(int n, int c1, int c2, int c3) {
}
void AyuSys::ClearPDTRect(int n, int x1, int y1, int x2, int y2, int c1, int c2, int c3) {
}
void AyuSys::ClearPDTWithoutRect(int n, int x1, int y1, int x2, int y2, int c1, int c2, int c3) {
}

void AyuSys::FadePDTBuffer(int n, int x1, int y1, int x2, int y2, int c1, int c2, int c3, int count) {
}

void AyuSys::CopyPDTtoBuffer(int src_x, int src_y, int src_x2, int src_y2, int src_pdt,
	int dest_x, int dest_y, int dest_pdt, int flag) {
}

void AyuSys::CopyBuffer(int src_x, int src_y, int src_x2, int src_y2, int src_pdt,
	int dest_x, int dest_y, int dest_pdt, int flag) {
}
void AyuSys::CopyWithoutColor(int src_x, int src_y, int src_x2, int src_y2, int src_pdt,
	int dest_x, int dest_y, int dest_pdt, int c1, int c2, int c3) {
}
void AyuSys::SwapBuffer(int src_x, int src_y, int src_x2, int src_y2, int src_pdt,
	int dest_x, int dest_y, int dest_pdt) {
}

void AyuSys::LoadPDTBuffer(int n, char* path)
{
}

void AyuSys::LoadToExistPDT(int pdt_number, char* path, int x1, int y1, int x2, int y2, int x3, int y3, int fade) {
}

void AyuSys::LoadAnmPDT(char* path) {
}

void AyuSys::ClearAnmPDT(void) {
}

void AyuSys::DrawAnmPDT(int dest_x, int dest_y, int src_x, int src_y, int width, int height) {
}

void AyuSys::SetPDTUsed(void) {
}

DI_ImageMask* AyuSys::ReadPDTFile(char* f) {
	return 0;
}
void AyuSys::PrereadPDTFile(char* f) {
	return;
}

void AyuSys::DrawPDTBuffer(int pdt_number, SEL_STRUCT* sel) {
}

void AyuSys::DrawMouse(void){}
void AyuSys::DeleteMouse(void) {};
void AyuSys::DeleteTextWindow(void) {}
int AyuSys::SelectItem(TextAttribute* item, int deal, int type) {return 1;}
void AyuSys::DrawText(char* s) {};
void AyuSys::DeletePDTMask(int n) {};

#include "anm.h"
ANMDAT::~ANMDAT() {
}

ANMDAT::ANMDAT(char* file, AyuSys& sys) : local_system(sys) { }

int ANMDAT::Init(void) { }

void ANMDAT::ChangeAxis(int x, int y) {
}

void ANMDAT::FixSeenAxis(Seen& seen) { };

void ANMDAT::Play(int seen) { }

int* CountCgmData(void) {
	static int ret[1] = {-1};
	return ret;
}
int SearchCgmData(char* name) {
	return -1;
}
int GetCgmInfo(int number, const char** ret_filename) {
	return -1;
}
int AyuSys::PDTWidth(int pdt) { return 640;}
int AyuSys::PDTHeight(int pdt) { return 640;}
void AyuSys::ChangeMonochrome(int pdt, int x1, int y1, int x2, int y2) {
}
void AyuSys::InvertColor(int pdt, int x1, int y1, int x2, int y2) {
}
void AyuSys::StretchBuffer(int src_x, int src_y, int src_x2, int src_y2, int src_pdt,
                int dest_x, int dest_y, int dest_x2, int dest_y2, int dest_pdt) {
}

