/*  image_di_impl.h
 *       描画に必要な inline 関数などを集めたもの
 */
/*
 *
 *  Copyright (C) 2002-   Kazunori Ueno(JAGARL) <jagarl@creator.club.ne.jp>
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

#ifndef __KANON_DI_IMAGE_IMPL_H__
#define __KANON_DI_IMAGE_IMPL_H__

#include"image_di.h"
#include"typelist.h"
#include<string.h>

struct BlockFadeData {
	char* tables;
	char* old_tables;
	const FadeTableOrig** diftables;
	enum DIR { UtoD=0, DtoU=1, LtoR=0, RtoL=1} direction;
	enum DDIR { ULtoDR=0, DRtoUL=1, URtoDL=2, DLtoUR=3, ULtoDR1=0, DRtoUL1=1, URtoDL1=2, DLtoUR1=3, ULtoDR2=4, DRtoUL2=5, URtoDL2=6, DLtoUR2=7} diag_dir;
	int table_size;
	int blocksize_x;
	int blocksize_y;
	int blockwidth;
	int blockheight;
	int x0, y0;
	int width;
	int height;
	int max_x, min_x;
	int max_y, min_y;
	BlockFadeData* next;
	bool MakeSlideCountTable(int count, int max_count);
	bool MakeDiagCountTable(int count, int max_count);
	bool MakeDiagCountTable2(int count, int max_count);
	BlockFadeData(int blocksize_x, int blocksize_y, int x0, int y0, int width, int height);
	~BlockFadeData();
};

struct DifImage;

template <class T>
struct Drawer {
	enum {bpp = T::value};
	enum {BiPP = T::value};
	enum {ByPP = T::value/8};
	enum {DifByPP = T::value==16 ? 3 : sizeof(int) };
	static void SetMiddleColor(char* dest, char* src, int c);
	static void SetMiddleColorWithTable(char* dest, char* src,const FadeTable& table);
	static unsigned int CreateColor(int c1, int c2, int c3);
	static void Copy1Pixel(char* dest, char* src);
	static void Draw1PixelFromDif(char* dest, char* dif, const FadeTableOrig* table);
	static DifImage* MakeDifImage(DI_Image& dest, DI_Image& src, char* mask, SEL_STRUCT* sel);
	static char* CalcKido(char* data, int dbpl, int width, int height, int max);
	static void BlockDifDraw(DifImage* image, BlockFadeData* instance);

};

struct DifImage {
	int* draw_xlist;
	char* image;
	char* difimage;
	char* destimage;
	int difbpl;
	int destbpl;
	~DifImage() {
		delete[] draw_xlist;
		delete[] image;
	}
	DifImage(int image_size, int xlist_size) {
		draw_xlist = new int[xlist_size];
		image = new char[image_size];
	}
};

typedef Drawer<Int2Type<16> > Bpp16;
typedef Drawer<Int2Type<32> > Bpp32;

extern unsigned short middle16_data1[32*32];
extern unsigned short middle16_data2[64*64];
extern unsigned short middle16_data3[32*32];
extern unsigned short middle16_data4[64*64];

#if 0
/* 本来、行うべき作業はこういうこと */
inline void Bpp16::SetMiddleColor(char* dest, char* src, int c) {
	c &= 0xff; int c1 = (c&0xf8)<<2; int c2 = (c&0xfc)<<4;
	int dest_pix = *(short*)dest;
	int src_pix = *(short*)src;
	// 5-6-5 に分ける
	// *mask/256 する
	// +src_pix する
	// をある程度最適化
	int dest_pix1 = dest_pix & 0xf800;
	int dest_pix2 = dest_pix & 0x07e0;
	int dest_pix3 = dest_pix & 0x001f;
	int src_pix1 = src_pix & 0xf800;
	int src_pix2 = src_pix & 0x07e0;
	int src_pix3 = src_pix & 0x001f;

	dest_pix1 >>= 11; dest_pix2 >>= 5; dest_pix1 &= 0x1f;

	dest_pix1 *= 0x1f - (c>>3); dest_pix2 *= 0x3f-(c>>2); dest_pix3 *= 0x1f-(c>>3);
	dest_pix1 /= 0x1f; dest_pix2 /= 0x3f; dest_pix3 /= 0x1f;

	src_pix1 += dest_pix1<<11;
	src_pix2 += dest_pix2<<5;
	src_pix3 += dest_pix3;
	if (src_pix1 > 0xf800) {src_pix1 = 0xf800;}
	if (src_pix2 > 0x07e0) {src_pix2 = 0x07e0;}
	if (src_pix3 > 0x001f) {src_pix3 = 0x001f;}
	*(short*)dest = src_pix1 | src_pix2 | src_pix3;
}
#else
/*　速度のために
　　・テーブル使用
　　・最大値は飽和加算
　　をしている。
　　なお、飽和加算は
	16bpp:
	 11110 111110 11110 b = 0xf7de == 最小桁隠しマスク
	10000 100000 100000 b = 0x10820== 飽和検知マスク
	10000 010000 100000 b = 0x10420== 飽和検知マスク2
	 01111 011111 01111 b = 0x7bef == (xorで)マスク生成用
	として、a,b の5-6-5 加算で
	c = ( (a&b) + ( (a^b) & 0xf7de) ) & 0x10820
	c = ( ( (((c*3)&0x10420)>>5) + 0x7bef) ^ 0x7bef
	return = (a + b - c) | c
　でやっている。*3 は lea 命令を仮定しているが add ２つでもしかたないかも。
*/
template<>
inline void Bpp16::SetMiddleColor(char* dest, char* src, int c) {
	int c1 = (c&0xf8)<<2; int c2 = (c&0xfc)<<4;

	unsigned int dest_pix = *(unsigned short*)dest;
	unsigned int dest_pix1 = dest_pix >> 11;
	unsigned int dest_pix2 = (dest_pix >> 5) & 0x3f;
	unsigned int dest_pix3 = dest_pix & 0x1f;

	register unsigned int d =
		(middle16_data1[dest_pix1+c1] |
		middle16_data2[dest_pix2+c2] |
		middle16_data3[dest_pix3+c1]);
	register unsigned int s = *(unsigned short*) src;

/*	*(unsigned short*) dest = s + d; */

	register unsigned int m = ( ((s&d)<<1) + ( (s^d)&0xf7de ) ) & 0x10820;
	m = ( (((m*3)&0x20840)>>6) + 0x7bef) ^ 0x7bef;

	unsigned int res =  (s + d - m ) | m;
	*(unsigned short*) dest = res;
}
template<>
inline void Bpp32::SetMiddleColor(char* dest, char* src, int c) {
	unsigned int s,d; c &= 0xff; c = 0x100-c-(c>>7);
	s = *(unsigned char*)src++; d = *(unsigned char*)dest;
	d *= c; d>>=8;
	if (d+s > 0xff) *(unsigned char*)dest = 0xff;
	else *(unsigned char*)dest = d+s;
	dest++;

	s = *(unsigned char*)src++; d = *(unsigned char*)dest;
	d *= c; d>>=8;
	if (d+s > 0xff) *(unsigned char*)dest = 0xff;
	else *(unsigned char*)dest = d+s;
	dest++;

	s = *(unsigned char*)src++; d = *(unsigned char*)dest;
	d *= c; d>>=8;
	if (d+s > 0xff) *(unsigned char*)dest = 0xff;
	else *(unsigned char*)dest = d+s;

}
#endif

template<>
inline void Bpp16::SetMiddleColorWithTable(char* dest, char* src,const FadeTable& table) {
	int dest_pix = *(short*)dest;
	int src_pix = *(short*)src;
	// 5-6-5 に分けて加算
	dest_pix += table.table16_1[ ((src_pix&0xf800) - (dest_pix&0xf800))>>11]
		 +  table.table16_2[ ((src_pix&0x07e0) - (dest_pix&0x07e0))>>5]
		 +  table.table16_3[ ((src_pix&0x001f) - (dest_pix&0x001f))];
	*(short*)dest = dest_pix;
}

template<>
inline void Bpp32::SetMiddleColorWithTable(char* dest, char* src, const FadeTable& table) {
	*(unsigned char*)dest += table.table32_1[ int(*(unsigned char*) src++) - int(*(unsigned char*)dest) ]; dest++;
	*(unsigned char*)dest += table.table32_2[ int(*(unsigned char*) src++) - int(*(unsigned char*)dest) ]; dest++;
	*(unsigned char*)dest += table.table32_3[ int(*(unsigned char*) src++) - int(*(unsigned char*)dest) ];
}

template<>
inline unsigned int Bpp16::CreateColor(int c1, int c2, int c3) { // short の色を 2word 並べて、 32bit 数値をつくる
	c1 &= 0xf8; c2 &= 0xfc; c3 &= 0xf8;
	c1 <<= 8; c2 <<= 3; c3 >>= 3;
	unsigned int col = c1 | c2 | c3;
	return (col<<16) | col;
}
template<>
inline unsigned int Bpp32::CreateColor(int c1, int c2, int c3) { // 24bit の色を 1word 並べて、 32bit 数値をつくる
	unsigned int a;
	char* mem = (char*)&a;
	mem[0] = c3; mem[1] = c2; mem[2] = c1; mem[3] = 0;
	return a;
}

template<>
inline void Bpp16::Copy1Pixel(char* dest, char* src) {
	*(short*)dest = *(short*)src;
}
template<>
inline void Bpp32::Copy1Pixel(char* dest, char* src) {
	*(int*)dest = *(int*)src;
}
template<>
inline void Bpp16::Draw1PixelFromDif(char* dest, char* dif,const FadeTableOrig* table) {
	*(unsigned short*)dest +=
	    table->table1_minus[(int)dif[0]]+
	  + table->table2_minus[(int)dif[1]]+
	  + table->table3_minus[(int)dif[2]];
}
template<>
inline void Bpp32::Draw1PixelFromDif(char* dest, char* dif,const FadeTableOrig* table) {
	int c = *(int*)dif; const int* mtable = table->table4_minus;
	dest[0] += mtable[c&0x1ff];
	dest[1] += mtable[(c>>9)&0x1ff];
	dest[2] += mtable[(c>>18)&0x1ff];
}
// 重ね合わせ用一時バッファの作成
template<class T>
DifImage* Drawer<T>::MakeDifImage(DI_Image& destimage, DI_Image& srcimage, char* mask, SEL_STRUCT* sel) {
	int width = sel->x2-sel->x1+1;
	int height = sel->y2-sel->y1+1;
	char* dest = destimage.data + sel->x3*ByPP + sel->y3*destimage.bpl;
	char* src = srcimage.data+sel->x1*ByPP + sel->y1*srcimage.bpl;
	if (mask) mask += sel->x1 + sel->y1*srcimage.width;
	int dbpl = destimage.bpl;
	int sbpl = srcimage.bpl;
	int mbpl = srcimage.width;

	DifImage* retimage = new DifImage(width*height*DifByPP, height*2);
	char* dif_buf = retimage->image;
	int* draw_xlist = retimage->draw_xlist;
	char* masked_line = new char[width*ByPP];

	retimage->destimage = dest;
	retimage->destbpl = destimage.bpl;
	retimage->difimage = retimage->image;
	retimage->difbpl = width*DifByPP;

	int i,j;

	for (i=0; i<height; i++) {
		if (mask) {
			// この行をマスク付きコピー
			char* s = src + i*sbpl;
			char* d = masked_line;
			char* m = mask + i*mbpl;
			memcpy(masked_line, dest+i*dbpl, width*ByPP);
			for (j=0; j<width; j++) {
				char mask_char = *m;
				if (mask_char) {
					if (mask_char == -1) {
						Copy1Pixel(d, s);
					 }else {
						SetMiddleColor(d, s, mask_char);
					}
				}
				s += ByPP; d += ByPP; m++;
			}
		}
		char* d = dest + i*dbpl;
		char* s = mask ? masked_line : src + i*sbpl;
		if (BiPP == 16) {
			char* dif = dif_buf + i*width*3;
			/* 差分を取る */
			for (j=0; j<width; j++) {
				int c1, c2, cc;
				c1 = *(short*)d;
				c2 = *(short*)s;
				cc = ((c2>>11)&0x1f)-((c1>>11)&0x1f); dif[0] = cc&0x3f;
				cc = ((c2>> 5)&0x3f)-((c1>> 5)&0x3f); dif[1] = cc&0x7f;
				cc = ((c2    )&0x1f)-((c1    )&0x1f); dif[2] = cc&0x3f;
				s += 2; d += 2; dif += 3;
			}
			/* 同一部分の検索 */
			dif = dif_buf + i*width*3;
			for (j=0; j<width; j++) {
				if (dif[0]|dif[1]|dif[2]) break;
				dif += 3;
			}
			draw_xlist[0] = j;
			dif = dif_buf + i*width*3 + width*3;
			for (j=0; j<width; j++) {
				dif -= 3;
				if (dif[0]|dif[1]|dif[2]) break;
			}
			draw_xlist[1] = width-j;
			draw_xlist += 2;
		} else { /* BiPP == 32 */
			/* 差分を取る */
			int* dif = ((int*)dif_buf) + i*width;
			for (j=0; j<width; j++) {
				int c, cc;
				c = int(*(unsigned char*)(s+0)) - int(*(unsigned char*)(d+0)); cc = c&0x1ff;
				c = int(*(unsigned char*)(s+1)) - int(*(unsigned char*)(d+1)); cc |= (c&0x1ff)<<9;
				c = int(*(unsigned char*)(s+2)) - int(*(unsigned char*)(d+2)); cc |= (c&0x1ff)<<18;
				*dif = cc;
				s += ByPP; d += ByPP; dif++;
			}
			/* 同一部分の検索 */
			dif = ((int*)dif_buf) + i*width;
			for (j=0; j<width; j++) {
				if (*dif) break;
				dif++;
			}
			draw_xlist[0] = j;
			dif = ((int*)dif_buf) + i*width + width;
			for (j=0; j<width; j++) {
				dif--;
				if (*dif) break;
			}
			draw_xlist[1] = width-j;
			draw_xlist += 2;
		}
	}
	delete[] masked_line;
	return retimage;
}


// 輝度計算
template<class T>
char* Drawer<T>::CalcKido(char* data, int dbpl, int width, int height, int max) {
	if (max == 0) max = 16;
	char* kido = new char[width*height];
	char kido_table[256];
	int i,j;
	for (i=0; i<256; i++) {
		kido_table[i] = i * max / 256;
	}
	for (i=0; i<height; i++) {
		char* line = data; char* k = kido+i*width;
		data += dbpl;
		for (j=0; j<width; j++) {
			// Y = 0.299R + 0.587G + 0.114B ということで。
			// ただし、R,B=5bit, G=6bit ということで、
			// 結果は 31bit になるようにしてみる。
			// ちょっとげたを履かせて明るめに・・・
			int c;
			if (BiPP == 16) {
				short col = *(short*)line;
				int r = (col>>11)&0x1f;
				int g = (col>>5)&0x3f;
				int b = (col)&0x1f;
				c = r * 20065550 + g * 19696451 + b * 7659419 + 23710710;
				c >>= 23;
			} else { /* BiPP == 32 */
				int r = *(unsigned char*)(line+2);
				int g = *(unsigned char*)(line+1);
				int b = *(unsigned char*)(line);
				c = r * (20065550/8) + g * (19696451/8) + b * (7659419/8) + 23710710;
				c >>= 23;
			}
			*k++ =  kido_table[c]; line += ByPP;
		}
	}
	return kido;
}

extern void CalcDifTable(BlockFadeData* instance);
template<class T>
void Drawer<T>::BlockDifDraw(DifImage* image, BlockFadeData* blockdata) {

	int i,j;
	CalcDifTable(blockdata);
	// オフセット移動
	int dest_bpl = image->destbpl;
	int dif_bpl = image->difbpl;
	int x0 = blockdata->x0 + blockdata->min_x * blockdata->blocksize_x;
	int y0 = blockdata->y0 + blockdata->min_y * blockdata->blocksize_y;
	char* dest_buf = image->destimage + x0*ByPP + y0*dest_bpl;
	char* dif_buf  = image->difimage  + x0*DifByPP + y0*dif_bpl;
	const FadeTableOrig** diftable = blockdata->diftables + blockdata->min_x + blockdata->min_y * blockdata->blockwidth;

	int ylen = blockdata->max_y - blockdata->min_y + 1;
	int xlen = blockdata->max_x - blockdata->min_x + 1;
	if (ylen <= 0 || xlen <= 0) return; // 描画なし
	int cur_y = blockdata->min_y * blockdata->blocksize_y;
	// 差分データからデータをつくる
	for (i=0; i<ylen; i++) {
		int next_y = cur_y + blockdata->blocksize_y;
		if (next_y > blockdata->height) next_y = blockdata->height;
		for (; cur_y < next_y; cur_y++) {
			char* d = dest_buf;  dest_buf += dest_bpl;
			char* dif = dif_buf; dif_buf += dif_bpl;
			int cur_x = blockdata->min_x * blockdata->blocksize_x;
			for (j=0; j<xlen; j++) {
				const FadeTableOrig* table = diftable[j];
				int next_x = cur_x + blockdata->blocksize_x;
				if (next_x > blockdata->width) next_y = blockdata->width;
				if (table == 0) {
					d += (next_x-cur_x) * ByPP;
					dif += (next_x-cur_x)*DifByPP;
				} else {
					for (; cur_x < next_x; cur_x++) {
						Draw1PixelFromDif(d, dif, table);
						d += ByPP; dif += DifByPP;
					}
				}
			}
		}
		diftable += blockdata->blockwidth;
	}
}
#endif /* __KANON_IMAGE_DI_IMPL__ */
