/*  image_di_Xbpp.cc
 *       DI_Image の操作
 *	 bpp ごとの差はマクロで吸収。
 *	 実際は image_di_{15,16,24,32}bpp.cc から include される。
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


#include "image_di.h"
// #include "system.h"
#include <string.h>
#include<stdio.h>
// #include<stdlib.h>
// #include <math.h>

// dest と src の中間色をつくる
// 16bpp, 5-6-5 を仮定

extern unsigned short middle16_data1[32*32];
extern unsigned short middle16_data2[64*64];
extern unsigned short middle16_data3[32*32];
extern unsigned short middle16_data4[64*64];

#if 0
/* 本来、行うべき作業はこういうこと */
inline void SetMiddleColor16(char* dest, char* src, int c) {
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
inline void SetMiddleColor16(char* dest, char* src, int c) {
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
inline void SetMiddleColor32(char* dest, char* src, int c) {
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

inline void SetMiddleColorWithTable16(char* dest, char* src,const FadeTable& table) {
	int dest_pix = *(short*)dest;
	int src_pix = *(short*)src;
	// 5-6-5 に分けて加算
	dest_pix += table.table16_1[ ((src_pix&0xf800) - (dest_pix&0xf800))>>11]
		 +  table.table16_2[ ((src_pix&0x07e0) - (dest_pix&0x07e0))>>5]
		 +  table.table16_3[ ((src_pix&0x001f) - (dest_pix&0x001f))];
	*(short*)dest = dest_pix;
}

inline void SetMiddleColorWithTable32(char* dest, char* src, const FadeTable& table) {
	*(unsigned char*)dest += table.table32_1[ int(*(unsigned char*) src++) - int(*(unsigned char*)dest) ]; dest++;
	*(unsigned char*)dest += table.table32_2[ int(*(unsigned char*) src++) - int(*(unsigned char*)dest) ]; dest++;
	*(unsigned char*)dest += table.table32_3[ int(*(unsigned char*) src++) - int(*(unsigned char*)dest) ];
}

inline unsigned short CreateColor16(int c1, int c2, int c3) { // short の色を 2word 並べて、 32bit 数値をつくる
	c1 &= 0xf8; c2 &= 0xfc; c3 &= 0xf8;
	c1 <<= 8; c2 <<= 3; c3 >>= 3;
	unsigned int col = c1 | c2 | c3;
	return (col<<16) | col;
}
inline unsigned int CreateColor32(int c1, int c2, int c3) { // 24bit の色を 1word 並べて、 32bit 数値をつくる
	unsigned int a;
	char* mem = (char*)&a;
	mem[0] = c3; mem[1] = c2; mem[2] = c1; mem[3] = 0;
	return a;
}

inline void Copy1Pixel16(char* dest, char* src) {
	*(short*)dest = *(short*)src;
}
inline void Copy1Pixel32(char* dest, char* src) {
	*(int*)dest = *(int*)src;
}
void CopyAllWithMask_Xbpp(DI_Image& dest_image, DI_Image& src_image, char* mask) {
	int width = dest_image.width; if (src_image.width < width) width = src_image.width;
	int height = dest_image.height; if (src_image.height < height) height = src_image.height;
	int dbpl = dest_image.bpl; int sbpl = src_image.bpl;
	// 開始場所を変更
	char* src_pt = src_image.data;
	char* dest_pt = dest_image.data;
	// コピー
	int i,j;
	for (i=0; i<height; i++) {
		// mask が０でなければ、mask の存在する範囲のみについて、
		// mask == 0xff でコピー、それ以外でアンチエイリアシングする。
		char* mask_line = mask + i*width;
		char* d = dest_pt; char* s = src_pt;
		for (j=0; j<width; j++) {
			char mask_char = *mask_line++;
			if (mask_char) {
				if (mask_char == -1) {
					Copy1Pixel(d,s);
				} else {
					SetMiddleColor(d, s, mask_char);
				}
			}
			s += ByPP; d += ByPP;
		}
		src_pt += sbpl; dest_pt += dbpl;
	}
	dest_image.RecordChangedRegionAll();
}


void CopyRectWithMask_Xbpp(DI_Image& dest_image, int dest_x, int dest_y, DI_Image& src_image,int src_x, int src_y, int width, int height, char* mask) {
	//  まず、領域がおかしい場合に備える
	int swidth = src_image.width; int sheight = src_image.height;
	int dwidth = dest_image.width; int dheight = dest_image.height;
	// width が不正なら修正
	if (dest_x+width > dwidth) width = dwidth-dest_x;
	if (dest_y+height > dheight) height = dheight - dest_y;
	if (src_x+width > swidth) width = swidth-src_x;
	if (src_y+height > sheight) height = sheight - src_y;
	if (width < 0 || height < 0) return;
	// image
	char* src_pt = src_image.data;
	char* dest_pt= dest_image.data;
	// image の属性は同じはず
	int dbpl = dest_image.bpl; int sbpl = src_image.bpl;
	// 開始場所を変更
	src_pt += src_x*ByPP + src_y*sbpl;
	dest_pt += dest_x*ByPP + dest_y*dbpl;
	// コピー
	int i,j;

	for (i=0; i<height; i++) {
		// mask が０でなければ、mask の存在する範囲のみについて、
		// mask == 0xff でコピー、それ以外でアンチエイリアシングする。
		char* mask_line = mask + (i+src_y)*swidth + src_x;
		char* d = dest_pt; char* s = src_pt;
		for (j=0; j<width; j++) {
			char mask_char = *mask_line++;
			if (mask_char) {
				if (mask_char == -1) {
					Copy1Pixel(d,s);
				} else {
					SetMiddleColor(d, s, mask_char);
				}
			}
			s += ByPP; d += ByPP;
		}
		src_pt += sbpl; dest_pt += dbpl;
	}
	dest_image.RecordChangedRegion(dest_x, dest_y, width, height);
}

int CalcTrilinearMatrix(double ret_matrix[3][3], int* dest_axis, int* src_axis);
int ConvertMatrixInt(double from_matrix[3][3], int to_matrix[3][3], int int_max);
/* x0 <= x < x1 かのチェック */
inline int InRange(int x0, int x, int x1) {
	if ( ((unsigned int)(x-x0)) < (unsigned int)(x1-x0)) return 1;
	else return 0;
}
/* 遠近法変換などで使う、矩形→四辺形変換 */
/* アルゴリズムはgimp のソースから */
/* trilinear transform と言うらしい（古川さんによる） */
/* 素性の良い四辺形じゃないと trilinearでもダメなので */
/* 歪みがでるそうな */
void CopyRectWithTransform_Xbpp(DI_Image& dest,
	int dest_x1, int dest_y1, int dest_x2, int dest_y2,
	int dest_x3, int dest_y3, int dest_x4, int dest_y4,
	DI_Image& src, int src_x, int src_y, int src_width, int src_height,
	bool is_fillback, int c1, int c2, int c3, int fade)
{
	double matrix_d[3][3]; /* 変換行列 */
	int matrix_i[3][3];
	int dest_axis[8];
	int src_axis[8];
	int i,j;
	/* 単純な変換なら Stretch にまわす */
	if (dest_x1 == dest_x3 && dest_y1 == dest_y2 && dest_x2 == dest_x4 && dest_y3 == dest_y4) {
		if (is_fillback) ClearWithoutRect(dest, dest_x1, dest_y1, dest_x4, dest_y4, c1, c2, c3);
		CopyRectWithStretch_Xbpp(dest, dest_x1, dest_y1, dest_x4-dest_x1+1, dest_y4-dest_y1+1, src, src_x, src_y, src_width, src_height, fade);
		return;
	}
	/* 座標変換の定義 */
	dest_axis[0] = dest_x1; dest_axis[1] = dest_y1;
	dest_axis[2] = dest_x2; dest_axis[3] = dest_y2;
	dest_axis[4] = dest_x3; dest_axis[5] = dest_y3;
	dest_axis[6] = dest_x4; dest_axis[7] = dest_y4;
	src_axis[0] = src_x; src_axis[1] = src_y;
	src_axis[2] = src_x+src_width-1; src_axis[3] = src_y;
	src_axis[4] = src_x; src_axis[5] = src_y+src_height-1;
	src_axis[6] = src_x+src_width-1; src_axis[7] = src_y+src_height-1;
	/* 変換行列の作成 */
	if (! CalcTrilinearMatrix(matrix_d, dest_axis, src_axis)) {
		return; // 変換行列の作成に失敗
	}
	ConvertMatrixInt(matrix_d, matrix_i, 65536*16);

	/* 画像関係の変数設定 */
	char* dest_mem = dest.data;
	int dbpl = dest.bpl;
	int dwidth = dest.width; int dheight = dest.height;
	char* src_mem = src.data;
	int sbpl = src.bpl;
	Colortype col = CreateColor(c1, c2, c3);
	FadeTable fadetable;
	if (fade != 255) fadetable.SetTable(fade);

	int xinc = matrix_i[0][0];
	int yinc = matrix_i[1][0];
	int winc = matrix_i[2][0];
	for (i=0; i<dheight; i++) {
		int tx, ty, tw;
		tx = matrix_i[0][1]*i + matrix_i[0][2];
		ty = matrix_i[1][1]*i + matrix_i[1][2];
		tw = matrix_i[2][1]*i + matrix_i[2][2];
		int next_ttx = tx / tw;
		int next_tty = ty / tw;
		const int seed = 8;
		char* d = dest_mem;
		/* 高速化のために 8(==seed) ドットずつ
		** 分割し、 描画法を変える
		** 副作用：8ドット以下の細い部分が
		** つぶれる。正確な trilinear でなくなる。
		*/
		for (j=0; j<dwidth; j+=seed) {
			int cur_ttx = next_ttx;
			int cur_tty = next_tty;
			tx += seed * xinc;
			ty += seed * yinc;
			tw += seed * winc;
			next_ttx = tx / tw;
			next_tty = ty / tw;
			int cur_inrange = InRange(0, cur_ttx, src_width) & InRange(0, cur_tty, src_height);
			int next_inrange= InRange(0, next_ttx,src_width) & InRange(0, next_tty,src_height);
			//if ( (cur_inrange^next_inrange)==1 || next_tty < cur_tty-2 || next_tty > cur_tty+2) {
			if (	(j+seed) >= dwidth ||
				(cur_inrange^next_inrange)==1 ||
				((cur_inrange|next_inrange)!=0 && (next_tty < cur_tty-1 || next_tty > cur_tty+1))
			) {
				/* o 変化率が大きいか
				** o 境界点が内部にある
				** o 端をまたぎ越す
				** などの場合、本来の trilinear ルーチンをつかう
				*/
				tx -= seed * xinc;
				ty -= seed * yinc;
				tw -= seed * winc;
				int k; int klen = j+seed < dwidth ? seed : dwidth - j;
				if (fade != 255) {
					for (k=0; k<klen; k++) {
						int ttx = int(tx / tw);
						int tty = int(ty / tw);
						if ( ((unsigned int)(ttx)) >= (unsigned int)(src_width) ||
						     ((unsigned int)(tty)) >= (unsigned int)(src_height)) { // 範囲外
							if (is_fillback) SetMiddleColorWithTable(d, (char*)&col,fadetable);
						} else { // コピー
							SetMiddleColorWithTable(d, src_mem + sbpl*tty + ByPP*ttx, fadetable);
						}
						d += ByPP;
						tx += xinc;
						ty += yinc;
						tw += winc;
					}
				} else {
					for (k=0; k<klen; k++) {
						int ttx = int(tx / tw);
						int tty = int(ty / tw);
						if ( ((unsigned int)(ttx)) >= (unsigned int)(src_width) ||
						     ((unsigned int)(tty)) >= (unsigned int)(src_height)) { // 範囲外
							if (is_fillback) Copy1Pixel(d, (char*)&col);
						} else { // コピー
							Copy1Pixel(d, src_mem + sbpl*tty + ByPP*ttx);
						}
						d += ByPP;
						tx += xinc;
						ty += yinc;
						tw += winc;
					}
				}
				
			} else if ( (cur_inrange|next_inrange) == 0) {
				/* 両端で範囲外：全部範囲外とみなす */
				int k;
				if (is_fillback) {
					if (fade != 255) {
						for (k=0; k<seed; k++) {
							SetMiddleColorWithTable(d, (char*)&col, fadetable);
							d += ByPP;
						}
					} else {
						for (k=0; k<seed; k++) {
							Copy1Pixel(d, (char*)&col);
							d += ByPP;
						}
					}
				} else {
					d += seed * ByPP;
				}
			} else {
				/* y = const, x は線形に変化と近似する */
				char* s = src_mem + sbpl*cur_tty + ByPP*cur_ttx;
				int ddx = next_ttx - cur_ttx;
				int dx = 0;
				int k;
				if (fade != 255) {
					for (k=0; k<seed; k++) {
						SetMiddleColorWithTable(d, s+(dx/seed)*ByPP, fadetable);
						d += ByPP;
						dx += ddx;
					}
				} else {
					for (k=0; k<seed; k++) {
						Copy1Pixel(d, s+(dx/seed)*ByPP);
						d += ByPP;
						dx += ddx;
					}
				}
			}
		}
		dest_mem += dbpl;
	}
	dest.RecordChangedRegion(0, 0, dest.width, dest.height);
}

/* 伸縮つきコピー */
/* 縮小の場合、色情報の平均化などは行わない */
void CopyRectWithStretch_Xbpp(DI_Image& dest, int dest_x, int dest_y, int dest_width, int dest_height,
	DI_Image& src, int src_x, int src_y, int src_width, int src_height, int fade) {
	if (src_width == dest_width && src_height == dest_height) {
		if (fade == 255) 
			CopyRect(dest, dest_x, dest_y, src, src_x, src_y, src_width, src_height);
		else
			CopyRectWithFade_Xbpp(dest, dest_x, dest_y, src, src_x, src_y, src_width, src_height, fade);
		return;
	}
	/* まず、dest <-> src の対応表をつくる */
	int* x_list = new int[dest_width]; int* y_list = new int[dest_height];
	int i;
	for (i=0; i<dest_width; i++)
		x_list[i] = src_x + i * src_width / dest_width;
	for (i=0; i<dest_height; i++)
		y_list[i] = src_y + i * src_height / dest_height;
	/* 座標を補正 */
	int first_x = 0, first_y = 0, last_x = dest_width, last_y = dest_height;
	if (dest_x < 0) first_x = -dest_x;
	if (dest_y < 0) first_y = -dest_y;
	if (dest_x + dest_width > dest.width) last_x = dest.width-dest_x;
	if (dest_y + dest_height > dest.height) last_y = dest.height - dest_y;

	for (;x_list[first_x] < 0 && first_x < last_x; first_x++) ;
	for (;x_list[last_x-1] > src.width && first_x < last_x; last_x--) ;
	for (;y_list[first_y] < 0 && first_y < last_y; first_y++) ;
	for (;y_list[last_y-1] > src.height && first_y < last_y; last_y--) ;

	// テーブルをつくる
	FadeTable table;
	if (fade != 255) table.SetTable(fade);
	/* コピー */
	/* まず、最初の座標設定 */
	int dbpl = dest.bpl; int sbpl = src.bpl;
	char* src_orig = src.data;
	char* dest_orig = dest.data + (dest_x+first_x)*ByPP + (dest_y+first_y)*dbpl;
	
	/* 以降bpp依存 */
	for (i=first_y; i<last_y; i++) {
		char* src_pt = src_orig + y_list[i]*sbpl;
		char* dest_pt = dest_orig;
		if (fade != 255) {
			int j; for (j=first_x; j<last_x; j++) {
				SetMiddleColorWithTable(dest_pt, src_pt+x_list[j]*ByPP, table);
				dest_pt += ByPP;
			}
		} else { /* ただのコピー */
			int j; for (j=first_x; j<last_x; j++) {
				Copy1Pixel(dest_pt, src_pt+x_list[j]*ByPP);
				dest_pt += ByPP;
			}
		}
		dest_orig += dbpl;
	}
	delete[] x_list; delete[] y_list;
	dest.RecordChangedRegion(dest_x, dest_y, dest_width, dest_height);
	return;
}

void CopyRectWithFade_Xbpp(DI_Image& dest_image, int dest_x, int dest_y, DI_Image& src_image,int src_x, int src_y, int width, int height, int fade) {
	int i,j;
	//  まず、領域がおかしい場合に備える
	int swidth = src_image.width; int sheight = src_image.height;
	int dwidth = dest_image.width; int dheight = dest_image.height;
	// width が不正なら修正
	if (dest_x+width > dwidth) width = dwidth-dest_x;
	if (dest_y+height > dheight) height = dheight - dest_y;
	if (src_x+width > swidth) width = swidth-src_x;
	if (src_y+height > sheight) height = sheight - src_y;
	if (width < 0 || height < 0) return;
	// image
	char* src_pt = src_image.data;
	char* dest_pt= dest_image.data;
	// image の属性は同じはず
	int dbpl = dest_image.bpl; int sbpl = src_image.bpl;
	// 開始場所を変更
	src_pt += src_x*ByPP + src_y*sbpl;
	dest_pt += dest_x*ByPP + dest_y*dbpl;
	// テーブルをつくる
	FadeTable table;
	table.SetTable(fade);
	// コピー

	for (i=0; i<height; i++) {
		// mask が０でなければ、mask の存在する範囲のみについて、
		// mask == 0xff でコピー、それ以外でアンチエイリアシングする。
		char* d = dest_pt; char* s = src_pt;
		for (j=0; j<width; j++) {
			SetMiddleColorWithTable(d, s, table);
			d += ByPP; s += ByPP;
		}
		src_pt += sbpl; dest_pt += dbpl;
	}
	dest_image.RecordChangedRegion(dest_x, dest_y, width, height);
}

void CopyRectWithFadeWithMask_Xbpp(DI_Image& dest_image, int dest_x, int dest_y, DI_Image& src_image,int src_x, int src_y, int width, int height, char* mask, int fade) {
	int i,j;
	//  まず、領域がおかしい場合に備える
	int swidth = src_image.width; int sheight = src_image.height;
	int dwidth = dest_image.width; int dheight = dest_image.height;
	// width が不正なら修正
	if (dest_x+width > dwidth) width = dwidth-dest_x;
	if (dest_y+height > dheight) height = dheight - dest_y;
	if (src_x+width > swidth) width = swidth-src_x;
	if (src_y+height > sheight) height = sheight - src_y;
	if (width < 0 || height < 0) return;
	// image
	char* src_pt = src_image.data;
	char* dest_pt= dest_image.data;
	// image の属性は同じはず
	int dbpl = dest_image.bpl; int sbpl = src_image.bpl;
	// 開始場所を変更
	src_pt += src_x*ByPP + src_y*sbpl;
	dest_pt += dest_x*ByPP + dest_y*dbpl;
	// テーブルをつくる
	FadeTable table;
	table.SetTable(fade);

	// コピー
	for (i=0; i<height; i++) {
		// mask が０でなければ、mask の存在する範囲のみについて、
		// mask == 0xff でコピー、それ以外でアンチエイリアシングする。
		char* mask_line = mask + (i+src_y)*swidth + src_x;
		char* d = dest_pt; char* s = src_pt;
		for (j=0; j<width; j++) {
			char mask_char = *mask_line++;
			if (mask_char) {
				if (mask_char == -1) {
					SetMiddleColorWithTable(d, s, table);
				} else {
					int tmp; Copy1Pixel((char*)&tmp, d);
					SetMiddleColor((char*)&tmp, s, mask_char); SetMiddleColorWithTable(d, (char*)&tmp, table);
				}
			}
			s += ByPP; d += ByPP;
		}
		src_pt += sbpl; dest_pt += dbpl;
	}
	dest_image.RecordChangedRegion(dest_x, dest_y, width, height);
}

void CopyRectWithoutColor_Xbpp(DI_Image& dest_image, int dest_x, int dest_y, DI_Image& src_image,int src_x, int src_y, int width, int height, int c1, int c2, int c3) {
	//  まず、領域がおかしい場合に備える
	int swidth = src_image.width; int sheight = src_image.height;
	int dwidth = dest_image.width; int dheight = dest_image.height;
	// width が不正なら修正
	if (dest_x+width > dwidth) width = dwidth-dest_x;
	if (dest_y+height > dheight) height = dheight - dest_y;
	if (src_x+width > swidth) width = swidth-src_x;
	if (src_y+height > sheight) height = sheight - src_y;
	if (width < 0 || height < 0) return;
	// image
	char* src_pt = src_image.data;
	char* dest_pt= dest_image.data;
	// image の属性は同じはず
	int dbpl = dest_image.bpl; int sbpl = src_image.bpl;
	// 開始場所を変更
	src_pt += src_x*ByPP + src_y*sbpl;
	dest_pt += dest_x*ByPP + dest_y*dbpl;
	// コピー
	int i,j;
	Colortype color = CreateColor(c1, c2, c3);

	for (i=0; i<height; i++) {
		char* d = dest_pt; char* s = src_pt;
		for (j=0; j<width; j++) {
#if BiPP == 16
			if ( *(unsigned short*)s != color) *(unsigned short*)d = *(unsigned short*)s;
#elif BiPP == 32
			if ( *(unsigned int*)s != color) *(unsigned int*)d = *(unsigned int*)s;
#else
#error invalid ByPP
#endif
			s += ByPP; d += ByPP;
		}
		src_pt += sbpl; dest_pt += dbpl;
	}
	dest_image.RecordChangedRegion(dest_x, dest_y, width, height);
}

void ClearRect_Xbpp(DI_Image& dest, int x1, int y1, int x2, int y2, int c1, int c2, int c3) {
	Colortype col = CreateColor(c1, c2, c3);
	int tmp;
	// 領域を正しくする
	if (x1 > x2) { tmp=x1; x1=x2; x2=tmp;}
	if (y1 > y2) { tmp=y1; y1=y2; y2=tmp;}
	if (x1<0) x1=0; if (y1<0) y1=0;
	int width = x2-x1+1; int height = y2-y1+1;
	if (width < 0) return;
	if (width >= dest.width) width = dest.width;
	if (height < 0) return;
	if (height >= dest.height) height = dest.height;
	// うめつくす
	int bpl = dest.bpl; int dwidth = dest.width;
	char* data = dest.data + bpl*y1 + ByPP*x1;
	int i;
	for (i=0; i<height; i++) {
		// 端の処理
		char* d = data; int w = width;
		Colortype* dd = (Colortype*)d;
		int j; for (j=0; j<w; j++) *dd++ = col;
		data += dwidth*ByPP;
	}
	dest.RecordChangedRegion(x1, y1, width, height);
	return;
}

static int fr_oldc1=0, fr_oldc2=0, fr_oldc3=0;
#if BiPP == 16
static int* fr_buf16 = 0;
static void FadeRect_MakeTable_16bpp(int c1, int c2, int c3, int count, int*& rbuf1, int*& rbuf2, int*& rbuf3) {
	if (fr_buf16 == 0 || c1 != fr_oldc1 || c2 != fr_oldc2 || c3 != fr_oldc3) {
		int i,j;
		/* テーブルを作り直す */
		if (fr_buf16 == 0) fr_buf16 = new int[128];
		int* buf1 = fr_buf16; int* buf2 = fr_buf16+32; int* buf3 = fr_buf16+96;
		// 現在のカウントでの、各色の最大・最小
		int c1_min = count*c1/256, c1_max = 256-(256-c1)*count/256; int c1_dif = c1_max-c1_min;
		int c2_min = count*c2/256, c2_max = 256-(256-c2)*count/256; int c2_dif = c2_max-c2_min;
		int c3_min = count*c3/256, c3_max = 256-(256-c3)*count/256; int c3_dif = c3_max-c3_min;
		j=0;
		for (i=0; i<32; i++) {
			int col1 = (c1_dif*i/32 + c1_min)>>3;
			int col2_1 = (c2_dif*j/64 + c2_min)>>2;
			int col2_2 = (c2_dif*(j+1)/64 + c2_min)>>2;
			int col3 = (c3_dif*i/32 + c3_min)>>3;
			if (col1>31) col1=31; if (col3>31) col3=31;
			if (col2_1>63) col2_1=63; if (col2_2>63) col2_2=63;
			buf1[i] = col1<<11; buf2[j++] = col2_1<<5; buf2[j++] = col2_2<<5; buf3[i] = col3;
		}
	}
	rbuf1 = fr_buf16; rbuf2 = fr_buf16+32; rbuf3 = fr_buf16+96;
	return;
}
#elif BiPP == 32
static char* fr_buf32 = 0;
static void FadeRect_MakeTable_32bpp(int c1, int c2, int c3, int count, char*& rbuf1, char*& rbuf2, char*& rbuf3) {
	if (fr_buf32 == 0 || c1 != fr_oldc1 || c2 != fr_oldc2 || c3 != fr_oldc3) {
		int i;
		/* テーブルを作り直す */
		if (fr_buf32 == 0) fr_buf32 = new char[256*3];
		char* buf1 = fr_buf32; char* buf2 = fr_buf32+256;char* buf3 = fr_buf32+512;
		// 現在のカウントでの、各色の最大・最小
		int c1_min = count*c1/256, c1_max = 256-(256-c1)*count/256; int c1_dif = c1_max-c1_min;
		int c2_min = count*c2/256, c2_max = 256-(256-c2)*count/256; int c2_dif = c2_max-c2_min;
		int c3_min = count*c3/256, c3_max = 256-(256-c3)*count/256; int c3_dif = c3_max-c3_min;
		for (i=0; i<256; i++) {
			int col1 = (c1_dif*i/256 + c1_min);
			int col2 = (c2_dif*i/256 + c2_min);
			int col3 = (c3_dif*i/256 + c3_min);
			if (col1>256) col1=256;
			if (col2>256) col2=256;
			if (col3>256) col3=256;
			buf1[i] = col1;
			buf2[i] = col2;
			buf3[i] = col3;
		}
	}
	rbuf1 = fr_buf32; rbuf2 = fr_buf32+256; rbuf3 = fr_buf32+512;
	return;
}
#endif

void FadeRect_Xbpp(DI_Image& dest, int x1, int y1, int x2, int y2, int c1, int c2, int c3, int count) {
	if (count == 255) {
		ClearRect_Xbpp(dest, x1, y1, x2, y2, c1, c2, c3);
		return;
	}
	// 領域を正しくする
	int tmp;
	if (x1 > x2) { tmp=x1; x1=x2; x2=tmp;}
	if (y1 > y2) { tmp=y1; y1=y2; y2=tmp;}
	if (x1<0) x1=0; if (y1<0) y1=0;
	int width = x2-x1+1; int height = y2-y1+1;
	if (width < 0) return;
	if (width >= dest.width) width = dest.width;
	if (height < 0) return;
	if (height >= dest.height) height = dest.height;
	
	// テーブルをつくる
	int i,j;

#if BiPP == 16
	j = 0;
	int* buf1, *buf2, *buf3;
	FadeRect_MakeTable_16bpp(c1, c2, c3, count, buf1, buf2, buf3);
#elif BiPP == 32
	char* buf1, *buf2, *buf3;
	FadeRect_MakeTable_32bpp(c1, c2, c3, count, buf1, buf2, buf3);
#else
#error invalid BiPP
#endif
	// fade する
	int bpl = dest.bpl; int dwidth = dest.width;
	char* data = dest.data + bpl*y1 + ByPP*x1;
	for (i=0; i<height; i++) {
		char* d = data;
		for (j=0; j<width; j++) {
#if BiPP == 16
			unsigned int c = *(unsigned short*)d;
			register int c1 = (c>>11);
			register int c2 = (c>>5)&0x3f;
			register int c3 = c&0x1f;
			c = buf1[c1] | buf2[c2] | buf3[c3];
			*(unsigned short*)d = c;
#elif BiPP == 32
			*(unsigned char*)(d+2) = buf1[*(unsigned char*)(d+2)];
			*(unsigned char*)(d+1) = buf2[*(unsigned char*)(d+1)];
			*(unsigned char*)(d+0) = buf3[*(unsigned char*)(d+0)];
#else
#error invalid BiPP
#endif
			d += ByPP;
		}
		data += dwidth*ByPP;
	}
	dest.RecordChangedRegion(x1, y1, width, height);
}

void ConvertMonochrome_Xbpp(DI_Image& im, int x, int y, int w, int h) {
	char* mem = im.data;
	mem += im.bpl * y + ByPP * x;

	int i,j;
	for (i=0; i<h; i++) {
		char* line = mem;
		for (j=0; j<w; j++) {
			// Y = 0.299R + 0.587G + 0.114B ということで。
#if BiPP == 16
			// ただし、R,B=5bit, G=6bit ということで、
			// 結果は 31bit になるようにしてみる。
			// ちょっとげたを履かせて明るめに・・・
			short col = *(short*)line;
			int r = (col>>11)&0x1f;
			int g = (col>>5)&0x3f;
			int b = (col)&0x1f;
			int c = r * 20065550 + g * 19696451 + b * 7659419 + 23710710;
			int c2 = c>>25; c >>= 26;

			col = (c<<11) | (c2<<5) | (c);
			*(short*)line = col;
#elif BiPP == 32
			int r = *(unsigned char*)(line+2);
			int g = *(unsigned char*)(line+1);
			int b = *(unsigned char*)(line);
			int c = r * (20065550/8) + g * (19696451/8) + b * (7659419/8) + 23710710;
			c >>= 23;
			line[0] = c; line[1] = c; line[2] = c;
#else
#error invalid BiPP
#endif
			line += ByPP;
		}
		mem += im.bpl;
	}
	im.RecordChangedRegion(x, y, w, h);
}

void InvertColor_Xbpp(DI_Image& im, int x, int y, int w, int h) {
	char* mem = im.data;
	mem += im.bpl * y + ByPP * x;

	int i,j;
	for (i=0; i<h; i++) {
		char* line = mem;
		for (j=0; j<w; j++) {
			Colortype col = *(Colortype*)line;
			col = ~col;
			*(Colortype*)line = col;
			line += ByPP;
		}
		mem += im.bpl;
	}
	im.RecordChangedRegion(x, y, w, h);
}

/*********************************************************************
**
**	文字描画ルーチン (DrawMoji_Xbpp)
**
**********************************************************************
*/
/*
** 文字描画の処理はテンプレート化。
** ・FadePixel:
**   文字描画の際に
**  ・文字ウィンドウ描画を行う ; ウィンドウは背景を暗くする(SetMojiBit_fadepixel)
**  ・文字ウィンドウ描画を行う ; ウィンドウは一色塗りつぶし(SetMojiBit_clearpixel)
**  ・文字ウィンドウ描画しない (SetMojiBit_nothing)
**
** ・SetMojiBit:
**  ・マスクをalpha値とみなして文字描画(SetMojiBit_alpha)
**  ・マスクを 0 か否かのboolと見なして文字描画(SetMojiBit_mono)
*/

struct SetMojiBit_fadepixel {
	FadeTable table;
	SetMojiBit_fadepixel(int c1, int c2, int c3) {
		table.SetTable(c1, c2, c3);
	}
	void FadePixel(char* dest) const {
		int src = 0;
		SetMiddleColorWithTable(dest, (char*)src, table);
	}
};
struct SetMojiBit_clearpixel {
	Colortype color;
	SetMojiBit_clearpixel(int c1, int c2, int c3) {
		color = CreateColor(c1, c2, c3);
	}
	void FadePixel(char* dest) const {
		Copy1Pixel(dest, (char*)&color);
	}
};
struct SetMojiBit_nothing {
	void FadePixel(char* dest) const {
	}
};
/* 文字描画が alpha 付きか否かの選択 */
struct SetMojiBit_mono {
	void SetMojiBit(unsigned char mask, char* dest, char* src) const {
		if (mask) Copy1Pixel(dest, src);
	}
};
struct SetMojiBit_alpha {
	void SetMojiBit(unsigned char mask, char* dest, char* src) const {
		if (mask == 255) Copy1Pixel(dest, src);
		else if (mask) SetMiddleColor(dest, src, mask);
	}
};

template<class SetMojiBit, class SetMojiBit_fade> void DrawChara_Xbpp(const SetMojiBit& pixeldrawer, const SetMojiBit_fade& pixelfader,
	DI_Image& dest, int x, int y, int width, int height,
	unsigned char* charmask, int maskbpl, 
	int fc1, int fc2, int fc3, int bc1, int bc2, int bc3) {

	Colortype fc = CreateColor(fc1, fc2, fc3);
	Colortype bc = CreateColor(bc1, bc2, bc3);
	char* fc_mem = (char*)&fc;
	char* bc_mem = (char*)&bc;

	if (x < 0) { width += x; charmask -= x; x = 0;}
	if (y < 0) { height += y; charmask -= y*maskbpl; y = 0;}
	if (x+width > dest.width) { width = dest.width - x;}
	if (y+height > dest.height){ height = dest.height- y;}
	if (width <= 0 || height <= 0) return;

	int i,j;

	unsigned char* mask_mem = charmask;
	char* dest_mem = dest.data + x*ByPP + y*dest.bpl;
	char* dest_mem2= dest_mem + ByPP + dest.bpl;
	int backwidth = (x+width-1 <= dest.width) ? width : width-1;

	for (i=0; i<height; i++) {
		unsigned char* m; char* d;
		/* まず background color で描画 */
		if (y+i+1 <= dest.height) {
			m = mask_mem;
			d = dest_mem2;
			for (j=0; j<backwidth; j++) {
				/* 文字のない部分は文字描画ウィンドウの描画 */
				if (*m == 0) pixelfader.FadePixel(d);
				else pixeldrawer.SetMojiBit(*m, d, bc_mem);
			}
		}
		/* foreground 描画 */
		m = mask_mem;
		d = dest_mem;
		for (j=0; j<width; j++) {
			pixeldrawer.SetMojiBit(*m, d, fc_mem);
		}
		mask_mem += maskbpl;
		dest_mem += dest.bpl;
		dest_mem2 += dest.bpl;
	}
	dest.RecordChangedRegion(x, y, width, height);
}
void DrawChara_Xbpp(DI_Image& dest, int x1, int y1, int width, int height, 
	unsigned char* charmask, int maskbpl,
	int fc1, int fc2, int fc3, int bc1, int bc2, int bc3,
	int wc1, int wc2, int wc3, DrawCharaType type) {
	if (type & DC_ALPHA) {
		if (type & DC_FADEWALL) {
			DrawChara_Xbpp(SetMojiBit_alpha(), SetMojiBit_fadepixel(wc1, wc2, wc3),
				dest, x1, y1, width, height, charmask, maskbpl,
				fc1, fc2, fc3, bc1, bc2, bc3);
		} else if (type & DC_CLEARWALL) {
			DrawChara_Xbpp(SetMojiBit_alpha(), SetMojiBit_clearpixel(wc1, wc2, wc3),
				dest, x1, y1, width, height, charmask, maskbpl,
				fc1, fc2, fc3, bc1, bc2, bc3);
		} else { /* DC_NOTHING */
			DrawChara_Xbpp(SetMojiBit_alpha(), SetMojiBit_nothing(),
				dest, x1, y1, width, height, charmask, maskbpl,
				fc1, fc2, fc3, bc1, bc2, bc3);
		}
	} else { /* DC_MONO */
		if (type & DC_FADEWALL) {
			DrawChara_Xbpp(SetMojiBit_mono(), SetMojiBit_fadepixel(wc1, wc2, wc3),
				dest, x1, y1, width, height, charmask, maskbpl,
				fc1, fc2, fc3, bc1, bc2, bc3);
		} else if (type & DC_CLEARWALL) {
			DrawChara_Xbpp(SetMojiBit_mono(), SetMojiBit_clearpixel(wc1, wc2, wc3),
				dest, x1, y1, width, height, charmask, maskbpl,
				fc1, fc2, fc3, bc1, bc2, bc3);
		} else { /* DC_NOTHING */
			DrawChara_Xbpp(SetMojiBit_mono(), SetMojiBit_nothing(),
				dest, x1, y1, width, height, charmask, maskbpl,
				fc1, fc2, fc3, bc1, bc2, bc3);
		}
	}
}

