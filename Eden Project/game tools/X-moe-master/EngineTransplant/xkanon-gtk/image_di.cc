/*  image_di.cc : DI_Image クラス関係のうち、Gdk に依存するメソッドをとりだしたもの */
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



#include <gdk/gdk.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "image_di.h"

void Copy32bpp_Xbpp(char* dest, char* src, int width, int height, int bpl, int bpp, int rgbrev);
void SetImage(GdkImage* image, unsigned char* data) {
	// data で示される画像をimage に変換する。
	// data は4byte で 1pixel を示す、24bpp のデータ
	GdkVisual* vis = image->visual;
	if (vis->type != GDK_VISUAL_TRUE_COLOR &&
	    vis->type != GDK_VISUAL_DIRECT_COLOR) {
		fprintf(stderr, "Warning in SetImage() in file image_di.cc : "
			"SetImage() は visual の type が GDK_VISUAL_TRUE_COLOR"
			"あるいは GDK_VISUAL_DIRECT_COLOR の場合のみをサポートしています。15/16/24/32 bpp の"
			"該当する visual type で X server が起動していることを xdpyinfo などで確認してください\n");
		return;
	}
	int bipp = 16; int rgbrev = 0;
	if (vis->depth == 16) {
		if (vis->red_mask == 0x7c00 && vis->green_mask == 0x3e0 && vis->blue_mask == 0x1f) {
			bipp = 15; rgbrev = 0;
		} else if (vis->red_mask == 0xf800 && vis->green_mask == 0x7e0 && vis->blue_mask == 0x1f) {
			bipp = 16; rgbrev = 0;
		} else if (vis->blue_mask == 0x7c00 && vis->green_mask == 0x3e0 && vis->red_mask == 0x1f) {
			bipp = 15; rgbrev = 1;
		} else if (vis->blue_mask == 0xf800 && vis->green_mask == 0x7e0 && vis->red_mask == 0x1f) {
			bipp = 16; rgbrev = 1;
		}
	} else if (vis->depth == 24 || vis->depth == 32) {
		bipp = (image->bpl / image->width) * 8;
		if (vis->red_mask == 0xff) rgbrev = 1;
		else rgbrev = 0; /* redmask == 0xff0000 */
	}
	Copy32bpp_Xbpp((char*)image->mem, (char*)data, image->width,
		image->height, image->bpl, bipp, rgbrev);
	return;
}

void DI_Image::SetImage(GdkImage* image) {
	if (data && (!data_notdiscard)) delete[] data;
	data_notdiscard = true;
	data = (char*)image->mem;
	width = image->width;
	height = image->height;
	bpl = image->bpl;
	bypp = bpl / width;
	if (bypp != 2 && bypp != 4) {
		fprintf(stderr,"DI_Image::SetImage : Invalid image format.\n");
		data = 0;
		width = height = bpl = bypp = 0;
	}
	RecordChangedRegionAll();
	return;
}

void DI_ImageMask::SetCopyMask(DI_ImageMask* src) {
	if (mask && mask_notdiscard == false) delete[] mask;
	if (src == 0 || src->mask == 0) {
		mask = 0; return;
	}
	mask = new char[width*height];
	memset(mask, 0, width*height);
	int w=width,h=height;
	if (w>src->width) w=src->width;
	if (h>src->height)h=src->height;
	int i;for (i=0;i<h;i++) {
		memcpy(mask+width*i, src->mask+src->width*i, w);
	}
}

#include"image_di_record.h"
void DI_ImageRecord::RecordChangedRegion(int x, int y, int w, int h) {
	Region r;
	/* ないはずなんだけど…… */
	if (x >= width || y >= height || x < 0 || y < 0 ||
	    x+w > width || y+h > height || w < 0 || h < 0) {
		fprintf(stderr,"RecordChangedRegion : Bad parameter! %3d,%3d,%3d,%3d\n",x,y,w,h);
		RecordChangedRegionAll();
		return;
	}
	r.x = x; r.y = y; r.w = w; r.h = h;
	if (regions.empty()) {
		allregion = r;
		sum_area = 50 + w * h;
		allregion_area = sum_area;
	} else {
		/* allregion の拡張 */
		int ax2 = allregion.x + allregion.w;
		int ay2 = allregion.y + allregion.h;
		if (ax2 < x + w) ax2 = x + w;
		if (ay2 < y + h) ay2 = y + h;
		if (allregion.x > x) allregion.x = x;
		if (allregion.y > y) allregion.y = y;
		allregion.w = ax2 - allregion.x;
		allregion.h = ay2 - allregion.y;
		/* スコアの変更 */
		sum_area += 50 + w * h;
		allregion_area = 50 + allregion.w * allregion.h;
	}
	regions.push_back(r);
};
void DI_ImageRecord::RecordChangedRegionAll(void) {
	change_all = true;
};
bool DI_ImageRecord::IsChangedRegionAll(void) {
	return change_all;
}
bool DI_ImageRecord::GetChangedRegion(int index, int count, int& x, int& y, int& w, int& h) {
	if (change_all || last_index != index) {
		/* 全画面コピー */
		if (count == 0) {
			x = 0; y = 0; w = width; h = height;
			return true;
		} else {
			return false;
		}
	} else if (sum_area >= allregion_area) {
		/* allregion を返す */
		if (count == 0) {
			x = allregion.x;
			y = allregion.y;
			w = allregion.w;
			h = allregion.h;
			return true;
		} else {
			return false;
		}
	} else { 
		/* 各領域を返す */
		if (count >= (int)regions.size()) return false;
		x = regions[count].x;
		y = regions[count].y;
		w = regions[count].w;
		h = regions[count].h;
		return true;
	}
}
void DI_ImageRecord::ClearChangedRegion(int index) {
	last_index = index;
	regions.clear();
	change_all = false;
}
int DI_ImageRecord::GetLastIndex(void) {
	return last_index;
}


void DI_Image::RecordChangedRegion(int x, int y, int w, int h) {
};
void DI_Image::RecordChangedRegionAll(void) {
};
bool DI_Image::IsChangedRegionAll(void) {
	return true;
}
bool DI_Image::GetChangedRegion(int index, int count, int& x, int& y, int& w, int& h) {
	if (count == 0) {
		x = 0; y = 0; w = width; h = height;
		return true;
	} else {
		return false;
	}
}
void DI_Image::ClearChangedRegion(int index) {
}
int DI_Image::GetLastIndex(void) {
	return -1;
}
