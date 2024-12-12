/*  system_graphics.cc
 *      senario ファイルでのグラフィック操作を実際に DI_Image に
 *      渡し、また Window との同期をとるための AyuSys のメソッド
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
#include <string.h>
#include <unistd.h>
#include "image_di.h"
#include "image_pdt.h"

//#define DEBUG

int AyuSys::CheckPDT(int pdt_number) {
	if (main_window == 0) return -1;
	if (pdt_number >= PDT_BUFFER_DEAL) return -1;
//	if (pdt_buffer_orig[pdt_number] == 0) return -1;
	if (pdt_buffer[pdt_number] == 0) return -1; /* あり得ないはず */
	return 0;
}

int AyuSys::GetUnusedPDT(void) {
	if (main_window == 0) return -1;
	int i;
	for (i=PDT_BUFFER_DEAL-1; i>0; i--) {
		if (CheckPDT(i) == -1) return i;
	}
	return -1;
}

void AyuSys::ClearAllPDT(void) {
	int i;for (i=1; i<PDT_BUFFER_DEAL; i++) {
		if (CheckPDT(i)) continue;
		if (DisconnectPDT(i)) continue;
		ClearPDTBuffer(i, 0, 0, 0);
	}
}

// PDT buffer を消去する
void AyuSys::ClearPDTBuffer(int n, int c1, int c2, int c3) {
	if (GrpFastMode() == 3) return;
	if (DisconnectPDT(n)) return;
	ClearAll(*pdt_buffer[n], c1, c2, c3);
	if (n == 0) CallUpdateFunc();
}
void AyuSys::ClearPDTRect(int n, int x1, int y1, int x2, int y2, int c1, int c2, int c3) {
	if (GrpFastMode() == 3) return;
	if (SyncPDT(n)) return;
	ClearRect(pdt_buffer[n], x1, y1, x2, y2, c1, c2, c3);
	if (n == 0) CallUpdateFunc();
}
void AyuSys::ClearPDTWithoutRect(int n, int x1, int y1, int x2, int y2, int c1, int c2, int c3) {
	if (GrpFastMode() == 3) return;
	if (SyncPDT(n)) return;
	ClearWithoutRect(*pdt_buffer[n], x1, y1, x2, y2, c1, c2, c3);
	if (n == 0) CallUpdateFunc();
}

void AyuSys::FadePDTBuffer(int n, int x1, int y1, int x2, int y2, int c1, int c2, int c3, int count) {
	if (GrpFastMode() == 3) return;
	if (SyncPDT(n)) return;
	FadeRect(pdt_buffer[n], x1, y1, x2, y2, c1, c2, c3, count);
	if (n == 0) CallUpdateFunc();
}

bool AyuSys::check_para_sg(int& src_x, int& src_y, int& src_x2, int& src_y2, int& src_pdt,
	int& dest_x, int& dest_y, int& dest_pdt, int& width, int& height) {
	if (GrpFastMode() == 3) return false;
	if (SyncPDT(dest_pdt) || CheckPDT(src_pdt)) return false;
        // PDT buffer の image を画面に描画

	/* 座標をきちんとする */
	if (src_x > src_x2) { dest_x = dest_x - src_x + src_x2; int tmp = src_x; src_x = src_x2; src_x2 = tmp;}
	if (src_y > src_y2) { dest_y = dest_y - src_y + src_y2; int tmp = src_y; src_y = src_y2; src_y2 = tmp;}

	if (src_x < 0) { dest_x -= src_x; src_x = 0;}
	if (src_y < 0) { dest_y -= src_y; src_y = 0;}
	if (dest_x < 0) { src_x -= dest_x; dest_x = 0;}
	if (dest_y < 0) { src_y -= dest_y; dest_y = 0;}
	if (src_x2 >= pdt_buffer[src_pdt]->width) src_x2 = pdt_buffer[src_pdt]->width - 1;
	if (src_y2 >= pdt_buffer[src_pdt]->height) src_y2 = pdt_buffer[src_pdt]->height - 1;
	if (src_x > src_x2 || src_y > src_y2) return false;
	
	width = src_x2 - src_x + 1; height = src_y2 - src_y + 1;
	if (dest_x+width > pdt_buffer[dest_pdt]->width) width = pdt_buffer[dest_pdt]->width-dest_x;
	if (dest_y+height> pdt_buffer[dest_pdt]->height) height= pdt_buffer[dest_pdt]->height-dest_y;

	if (width <= 0 || height <= 0) return false;
	return true;
}

void AyuSys::CopyPDTtoBuffer(int src_x, int src_y, int src_x2, int src_y2, int src_pdt,
	int dest_x, int dest_y, int dest_pdt, int count) {
	// 特別な処理が必要ない
	if (count <= 0 || count > 0xff) count = 0xff;
	int width, height;
	if (check_para_sg(src_x, src_y, src_x2, src_y2, src_pdt, dest_x, dest_y, dest_pdt, width, height) == false) return;

	// コピー
	if (count == 0xff) {
		CopyRect(pdt_buffer[dest_pdt], dest_x, dest_y, pdt_buffer[src_pdt], src_x, src_y, width, height);
	} else 
		CopyRectWithFade(pdt_buffer[dest_pdt], dest_x, dest_y, pdt_buffer[src_pdt], src_x, src_y, width, height, count);

	if (dest_pdt == 0) {
		CallUpdateFunc();
	}
	return;
}

void AyuSys::CopyBuffer(int src_x, int src_y, int src_x2, int src_y2, int src_pdt,
	int dest_x, int dest_y, int dest_pdt, int count) {
	// 特別な処理が必要ない
	if (count <= 0 || count > 0xff) count = 0xff;
	int width, height;
	if (check_para_sg(src_x, src_y, src_x2, src_y2, src_pdt, dest_x, dest_y, dest_pdt, width, height) == false) return;
	// コピー
	if (src_pdt != dest_pdt || dest_x != src_x || dest_y != src_y) {
		if (count == 0xff) {
			CopyRect(*(DI_Image*)pdt_buffer[dest_pdt], dest_x, dest_y, *(DI_Image*)(pdt_buffer[src_pdt]), src_x, src_y, width, height);
		} else {
			CopyRectWithFade((DI_Image*)pdt_buffer[dest_pdt], dest_x, dest_y, (DI_Image*)(pdt_buffer[src_pdt]), src_x, src_y, width, height,count);
		}
	}
	if (dest_pdt == 0) {
		CallUpdateFunc();
	}
	return;
}

void AyuSys::CopyWithoutColor(int src_x, int src_y, int src_x2, int src_y2, int src_pdt,
	int dest_x, int dest_y, int dest_pdt, int c1, int c2, int c3) {
	int width, height;
	if (check_para_sg(src_x, src_y, src_x2, src_y2, src_pdt, dest_x, dest_y, dest_pdt, width, height) == false) return;
        // PDT buffer の image を画面に描画
	// コピー
	CopyRectWithoutColor(pdt_buffer[dest_pdt], dest_x, dest_y,
		pdt_buffer[src_pdt], src_x, src_y, width, height,
		c1, c2, c3);
	if (dest_pdt == 0) {
		CallUpdateFunc();
	}
	return;
}

void AyuSys::SwapBuffer(int src_x, int src_y, int src_x2, int src_y2, int src_pdt,
	int dest_x, int dest_y, int dest_pdt) {
	int width, height;
	if (check_para_sg(src_x, src_y, src_x2, src_y2, src_pdt, dest_x, dest_y, dest_pdt, width, height) == false) return;
        // PDT buffer の image を交換
	// コピー
	if (src_pdt != dest_pdt) {
		SwapRect(*(DI_Image*)pdt_buffer[dest_pdt], dest_x, dest_y, *(DI_Image*)(pdt_buffer[src_pdt]), src_x, src_y, width, height);
	}
	if (dest_pdt == 0) {
		CallUpdateFunc();
	}
	return;
}

void AyuSys::ChangeMonochrome(int pdt, int x1, int y1, int x2, int y2) {
	if (GrpFastMode() == 3) return;
	if (SyncPDT(pdt)) return;
	int w = pdt_buffer[pdt]->width; int h = pdt_buffer[pdt]->height;
	if (x1 < 0) x1 = 0;
	if (x2 < 0) x2 = 0;
	if (y1 < 0) y1 = 0;
	if (y2 < 0) y2 = 0;
	if (x1 >= w) x1 = w-1;
	if (x2 >= w) x2 = w-1;
	if (y1 >= h) y1 = h-1;
	if (y2 >= h) y2 = h-1;
	if (x1 > x2) { int tmp = x2; x2 = x1; x1 = tmp; }
	if (y1 > y2) { int tmp = y2; y2 = y1; y1 = tmp; }
	ConvertMonochrome(*(DI_Image*)pdt_buffer[pdt], x1, y1, x2-x1+1, y2-y1+1);
}

void AyuSys::InvertColor(int pdt, int x1, int y1, int x2, int y2) {
	if (GrpFastMode() == 3) return;
	if (SyncPDT(pdt)) return;
	int w = pdt_buffer[pdt]->width; int h = pdt_buffer[pdt]->height;
	if (x1 < 0) x1 = 0;
	if (x2 < 0) x2 = 0;
	if (y1 < 0) y1 = 0;
	if (y2 < 0) y2 = 0;
	if (x1 >= w) x1 = w-1;
	if (x2 >= w) x2 = w-1;
	if (y1 >= h) y1 = h-1;
	if (y2 >= h) y2 = h-1;
	if (x1 > x2) { int tmp = x2; x2 = x1; x1 = tmp; }
	if (y1 > y2) { int tmp = y2; y2 = y1; y1 = tmp; }
	::InvertColor(*(DI_Image*)pdt_buffer[pdt], x1, y1, x2-x1+1, y2-y1+1);
}

void AyuSys::StretchBuffer(int src_x, int src_y, int src_x2, int src_y2, int src_pdt,
                int dest_x, int dest_y, int dest_x2, int dest_y2, int dest_pdt) {
	if (GrpFastMode() == 3) return;
	if (SyncPDT(dest_pdt) || CheckPDT(src_pdt)) return;
	// PDT buffer の image を画面に描画
	if (src_x > src_x2) { int tmp = src_x; src_x = src_x2; src_x2 = tmp;}
	if (src_y > src_y2) { int tmp = src_y; src_y = src_y2; src_y2 = tmp;}
	if (dest_x > dest_x2) { int tmp = dest_x; dest_x = dest_x2; dest_x2 = tmp;}
	if (dest_y > dest_y2) { int tmp = dest_y; dest_y = dest_y2; dest_y2 = tmp;}
	// コピー
	CopyRectWithStretch( *(DI_Image*)pdt_buffer[dest_pdt], dest_x, dest_y,
		dest_x2-dest_x+1, dest_y2-dest_y+1, *(DI_Image*)(pdt_buffer[src_pdt]),
		src_x, src_y, src_x2-src_x+1, src_y2-src_y+1);
	if (dest_pdt == 0) {
		CallUpdateFunc();
	}
	return;
}

// PDT buffer を読み込み
void AyuSys::LoadPDTBuffer(int n, char* path)
{
	if (GrpFastMode() == 3) return;
	if (*path == '*' || *path == '?') { 
		if (n == 1) return;
		// buffer をコピーして、mask をセット
		DisconnectPDT(n);
		CopyAll(*pdt_buffer[n], *pdt_buffer[1]);
		pdt_buffer[n]->SetCopyMask(pdt_buffer[1]);
	} else if (n == 0) {
		DI_Image* image = ReadPDTFile(path);
		CopyAll(*pdt_buffer[0], *image);
	} else {
		pdt_image[n] = ReadPDTFile(path);
		pdt_buffer[n] = pdt_image[n];
	}
	if (n == 0) {
		CallUpdateFunc();
	}
}

// PDT buffer に、ファイルの内容を重ねる
void AyuSys::LoadToExistPDT(int pdt_number, char* path, int x1, int y1, int x2, int y2, int x3, int y3, int fade) {
	if (*path == '*' || *path == '?') return;
	if (GrpFastMode() == 3) return;
	if (SyncPDT(pdt_number)) return;
	/* ファイルを読み込み */
	DI_ImageMask* im = ReadPDTFile(path);
	if (im == 0) return;

	/* 座標をきちんとする */
	if (x1 > x2) { int tmp = x1; x1 = x2; x2 = tmp;}
	if (y1 > y2) { int tmp = y1; y1 = y2; y2 = tmp;}

	if (x1 < 0) x1=0;
	if (y1 < 0) y1=0;
	if (x2 >= im->width) x2=im->width-1;
	if (y2 >= im->height) y2=im->height-1;
	if (x1 > x2 || y1 > y2) return;
	int width = x2-x1+1; int height = y2-y1+1;
	if (x3+width > pdt_buffer[pdt_number]->width) width = pdt_buffer[pdt_number]->width-x3;
	if (y3+height > pdt_buffer[pdt_number]->height) height = pdt_buffer[pdt_number]->height-y3;
	/* buffer に重ねる */
	if (fade == -1)
		CopyRect(pdt_buffer[pdt_number], x3, y3, im, x1, y1, width, height);
	else
		CopyRectWithFade(pdt_buffer[pdt_number], x3, y3, im, x1, y1, width, height, fade);

	if (pdt_number == 0) {
		CallUpdateFunc();
	}
}

void AyuSys::LoadAnmPDT(char* path) {
	if (main_window == 0) return;
	if (anm_pdt != 0) return;
	if (*path == '*' || *path == '?') { 
		anm_pdt = pdt_buffer[1];
	} else {
		anm_pdt = ReadPDTFile(path);
	}
}

void AyuSys::ClearAnmPDT(void) {
	anm_pdt = 0;
}

void AyuSys::DrawAnmPDT(int dest_x, int dest_y, int src_x, int src_y, int width, int height) {
	if (main_window == 0) return;
	CopyRect(pdt_buffer[0], dest_x, dest_y, anm_pdt, src_x, src_y, width, height);
	CallUpdateFunc();
}

void AyuSys::SetPDTUsed(void) {
	int i; for (i=0; i<PDT_BUFFER_DEAL; i++) {
		if (pdt_image[i]) pdt_image[i]->SetUsed();
	}
	if (anm_pdt) anm_pdt->SetUsed();
}

int AyuSys::IsPDTUsed(int pdt_num) {
	if (pdt_num <= 0 || pdt_num >= PDT_BUFFER_DEAL) return 0; // 未使用
	if (pdt_image[pdt_num] == 0) return 0; // 未使用
	return 1; // 使用中
}

DI_ImageMask* AyuSys::ReadPDTFile(char* f) {
	if (pdt_reader)
		return pdt_reader->Search(f);
	else return 0;
}

void AyuSys::PrereadPDTFile(char* f) {
	if (pdt_reader)
		pdt_reader->Preread(f);
	return;
}

void AyuSys::DrawPDTBuffer(int pdt_number, SEL_STRUCT* sel) {
	// sel 付きで画面描画
	if (CheckPDT(pdt_number)) return; 
	int write3 = 0;
	if (pdt_number != 3 && (
		sel->sel_no == 4  || sel->sel_no == 5  || sel->sel_no == 50 ||
		sel->sel_no == 54 ||
		(sel->sel_no >= 60 && sel->sel_no <= 63) ||
		sel->sel_no == 150||
		(sel->sel_no>=160&&sel->sel_no<=163) ||
		sel->sel_no == 180)) write3 = 1;
	if (pdt_number == 0 && sel->sel_no != 200) { // pdt == 0 なら、sel 付きはできない
		CallUpdateFunc();
		if (write3) CopyBuffer(0,0,scn_w-1,scn_h-1,0, 0,0,3, 0);
		return;
	}
	/* 代替処理：sel0=コピーのみ sel2=コピー&表示 */
	if (sel->sel_no == 0) {
		CopyPDTtoBuffer(sel->x1,sel->y1,sel->x2,sel->y2,pdt_number,
			sel->x3, sel->y3, -1, 0);
		return;
	}
	if (sel->sel_no == 2) {
		CopyPDTtoBuffer(sel->x1,sel->y1,sel->x2,sel->y2,pdt_number,
			sel->x3, sel->y3, 0, 0);
		return;
	}
	/* 読み飛ばしモードによっては描画を適当にする */
	if (GrpFastMode() == 2) {
		CopyBuffer(sel->x1,sel->y1,sel->x2,sel->y2,pdt_number,
			sel->x3, sel->y3, 0, 0);
		CallUpdateFunc();
		if (write3) CopyBuffer(0,0,scn_w-1,scn_h-1,0, 0,0,3, 0);
		return;
	} else if (GrpFastMode() == 3) {
		return;
	}
	// まず、SEL を規格化
	int x1=sel->x1, x2=sel->x2, x3=sel->x3, y1=sel->y1, y2=sel->y2, y3=sel->y3;
	if (x1 > x2) {
		int tmp = x2; x2 = x1; x1 = tmp; x3 = x3 - (x2-x1);
	}
	if (y1 > y2) {
		int tmp = y2; y2 = y1; y1 = tmp; y3 = y3 - (y2-y1);
	}
	if (x2 < 0 || y2 < 0 || x1 >= pdt_buffer[pdt_number]->width || y1 >= pdt_buffer[pdt_number]->height) return; // 画像が画面の外
	if (x1 < 0) { x3 -= x1; x1=0; }
	if (y1 < 0) { y3 -= y1; y1=0; }
	if (x3 < 0) { x1 -= x3; x3=0; }
	if (y3 < 0) { y1 -= y3; y3=0; }
	if (x3+(x2-x1) >= scn_w) { x2 = scn_w-1+x1-x3;}
	if (y3+(y2-y1) >= scn_h) { y2 = scn_h-1+y1-y3;}
	if (x1 > x2 || y1 > y2) return; // 画像が画面の外
	if (x1 == x2 && y1 == y2) return; // 画像がない
	// 戻す
	sel->x1 = x1; sel->x2 = x2; sel->x3 = x3;
	sel->y1 = y1; sel->y2 = y2; sel->y3 = y3;

	/* いくつかの画像効果、あるいはフェード付きの場合、
	** 始めに画像合成をしてしまう
	*/
	if (sel->kasane && (sel->arg1 != 0 && sel->arg1 != 0xff) && (
		(sel->sel_no >= 15 && sel->sel_no <= 18) || /* スクロール */
		(sel->sel_no >= 80 && sel->sel_no <= 83) || /* スクロール付き画像効果 */
		(sel->sel_no >= 160&& sel->sel_no <= 163)   /* stretch コピー */
		)) {
		int new_pdt_number = TMP_PDT_BUFFER;
		if (write3) {
			new_pdt_number = 3;
			write3 = 0;
		}

		// 画像をフェードしたものが新たな画像になる
		DisconnectPDT(new_pdt_number);
		CopyBuffer(sel->x1, sel->y1, sel->x2, sel->y2, 1,
			sel->x3, sel->y3, new_pdt_number, 0);
		CopyPDTtoBuffer(sel->x1, sel->y1, sel->x2, sel->y2, pdt_number,
			sel->x3, sel->y3, new_pdt_number, sel->arg1);
		pdt_number = new_pdt_number;
	}
	if (IsGraphicEffectOff()) { // 画像効果なし
		CopyRect(pdt_buffer[0], sel->x3, sel->y3,
			pdt_buffer[pdt_number], sel->x1, sel->y1, sel->x2, sel->y2);
		CallUpdateFunc();
		if (write3) CopyBuffer(0,0,scn_w-1,scn_h-1,0, 0,0,3, 0);
		return;
	}
	
	// バージョンが古い場合 sel 変化
	if (sel->sel_no == 4 && Version() <= 1) {
		sel->sel_no = 5;
	}
	bool mouse_canceled = false;
	ClearMouseInfo();
	// 時間を計る基準
	void* time_base = setTimerBase();
	int wait_time = 0; int count = 0;
	CopyWithSel(pdt_buffer[0], pdt_buffer[pdt_number], sel, 0);
	count++; wait_time = sel->wait_time; // count == 0 の絵は書かれたから、count=1
	while(1) {
		int ret;
		// 画像表示まで、時間待ち
		//CallUpdateFunc();
		CallUpdateFunc();
		CallProcessMessages();
		usleep(100);
		if (sel->wait_time != 0) {
			if (wait_time > getTime(time_base)) {
				CallProcessMessages();
				while(wait_time > getTime(time_base))
					CallProcessMessages();
			} else {
				if (sel->wait_time > 0)
					count = getTime(time_base) / sel->wait_time;
				else
					count += 10000;
			}
		}
		if (main_window == 0) {
			fprintf(stderr, "Draw screen intterupted!!\n");
			break;
		}
		// マウスによる描画キャンセル
		int x,y,click;
		GetMouseInfo(x, y, click);
		if (click == 0) {
			mouse_canceled = true;
			ClearMouseInfo();
		}
		// 描画
		if (mouse_canceled)
			ret = CopyWithSel(pdt_buffer[0], pdt_buffer[pdt_number], sel, count+99999);
		else
			ret = CopyWithSel(pdt_buffer[0], pdt_buffer[pdt_number], sel, count);
		count++;
		if (ret == 0) continue; // 描画がなければ続行
		if (ret == 2) {
			// 描画が終われば終了
			break;
		}
		if (ret == -1) { // 未サポートの sel
			fprintf(stderr, "Unsupported sel : pdt number = %d, "
				"sel = (%d,%d), (%d,%d), (%d,%d), "
				"wait %d, sel %d ,kasane %d, arg %d,%d,%d,%d,%d,%d\n",
				pdt_number,
				sel->x1, sel->y1, sel->x2, sel->y2, sel->x3, sel->y3,
				sel->wait_time, sel->sel_no, sel->kasane,
				sel->arg1, sel->arg2, sel->arg3, sel->arg4, sel->arg5, sel->arg6
			);
			fflush(stderr);
			CopyRect(pdt_buffer[0], sel->x3, sel->y3,
				pdt_buffer[pdt_number], sel->x1, sel->y1, sel->x2, sel->y2);
			break;
		}
		// 描画続行
		wait_time = count * sel->wait_time;
	}
	freeTimerBase(time_base);
	// CallUpdateFunc();
	if (write3) CopyBuffer(0,0,scn_w-1,scn_h-1,0, 0,0,3, 0);
	CallUpdateFunc();
}

int AyuSys::PDTWidth(int pdt_number) {
	if (CheckPDT(pdt_number)) return scn_w;
	if (pdt_buffer[pdt_number] == 0) return scn_w;
	return pdt_buffer[pdt_number]->width;
}
int AyuSys::PDTHeight(int pdt_number) {
	if (CheckPDT(pdt_number)) return scn_h;
	if (pdt_buffer[pdt_number] == 0) return scn_h;
	return pdt_buffer[pdt_number]->height;
}
