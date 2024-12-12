/*  image_di_nobpp.cc
 *       DI_Image の操作
 *       bpp に非依存なメソッドを集めたもの
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
#include "initial.h"
#include <string.h>
#include<stdio.h>
#include<math.h>
#include<stdlib.h>

void CopyAll(DI_Image& dest, DI_Image& src) {
	int width = dest.width; if (src.width < width) width = src.width;
	int height = dest.height; if (src.height < height) height = src.height;
	int i; int bpl = width * dest.bypp;
	char* dp = dest.data; char* sp = src.data;
	for (i=0; i<height; i++) {
		memcpy(dp, sp, bpl);
		dp += dest.bpl; sp += src.bpl;
	}
	dest.RecordChangedRegionAll();
	return;
}

void CopyRect(DI_Image& dest_image, int dest_x, int dest_y, DI_Image& src_image,int src_x, int src_y, int width, int height) {
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
	int dbpl = dest_image.bpl; int sbpl = src_image.bpl; int bypp = dest_image.bypp;
	// 開始場所を変更
	src_pt += src_x*bypp + src_y*sbpl;
	dest_pt += dest_x*bypp + dest_y*dbpl;
	int bpl = bypp*width;
	// コピー
	int i; for (i=0; i<height; i++) {
		memcpy(dest_pt, src_pt, bpl);
		dest_pt += dbpl; src_pt += sbpl;
	}
	dest_image.RecordChangedRegion(dest_x, dest_y, width, height);
}

void SwapRect(DI_Image& dest_image, int dest_x, int dest_y, DI_Image& src_image,int src_x, int src_y, int width, int height) {
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
	char* tmp_buffer = new char[dest_image.bpl];
	// image の属性は同じはず
	int dbpl = dest_image.bpl; int sbpl = src_image.bpl; int bypp = dest_image.bypp;
	// 開始場所を変更
	src_pt += src_x*bypp + src_y*sbpl;
	dest_pt += dest_x*bypp + dest_y*dbpl;
	int bpl = bypp*width;
	// コピー
	int i; for (i=0; i<height; i++) {
		memcpy(tmp_buffer, dest_pt, bpl);
		memcpy(dest_pt, src_pt, bpl);
		memcpy(src_pt, tmp_buffer, bpl);
		dest_pt += dbpl; src_pt += sbpl;
	}
	delete[] tmp_buffer;
	dest_image.RecordChangedRegion(dest_x, dest_y, width, height);
	src_image.RecordChangedRegion(src_x, src_y, width, height);
}

void ClearAll(DI_Image& dest, int c1, int c2, int c3) {
	ClearRect(&dest, 0, 0, dest.width-1, dest.height-1, c1, c2, c3);
}

void ClearWithoutRect(DI_Image& dest, int x1, int y1, int x2, int y2, int c1, int c2, int c3) {
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
	x2 = x1+width; x1--;
	y2 = y1+height; y2--;
	// clear
	int dheight = dest.height; int dwidth = dest.width;
	ClearRect(&dest, 0, 0, dwidth-1, y1, c1, c2, c3);
	ClearRect(&dest, 0, y2, dwidth-1, dheight-1, c1, c2, c3);
	y1++; y2--;
	ClearRect(&dest, 0, y1, x1, y2, c1, c2, c3);
	ClearRect(&dest, x2, y1, dwidth-1, y2, c1, c2, c3);
}

/* image_di_Xbpp.cc で使う変数を初期化 */

/* middle_data を初期化するためにクラスのコンストラクタを使う */
unsigned short middle16_data1[32*32];
unsigned short middle16_data2[64*64];
unsigned short middle16_data3[32*32];
unsigned short middle16_data4[64*64];
unsigned char houwa_data4_orig[256*3];
unsigned char* houwa_data4 = houwa_data4_orig + 256;

static void init_setmid(void);
static Init init0(100, init_setmid);

void init_setmid(void) {
	unsigned int i,j;
	for (i=0; i<32; i++) {
		for (j=0; j<32; j++) {
			middle16_data1[ j + (i<<5)] =( ( ( 0x1f - i) * j ) / 0x1f ) << 11;
			middle16_data3[ j + (i<<5)] =( ( ( 0x1f - i) * j ) / 0x1f );
		}
	}
	for (i=0; i<64; i++) {
		for (j=0; j<64; j++) {
			middle16_data2[ j + (i<<6)] =( ( ( 0x3f - i) * j ) / 0x3f ) << 5;
			middle16_data4[ j + (i<<6)] =( ( ( 0x3f - i) * j ) / 0x3f );
		}
	}
	for (i=0; i<256; i++) {
		houwa_data4[i] = i;
	}
	memset(houwa_data4-256, 0, 256);
	memset(houwa_data4+256, 255, 256);
}

FadeTableOrig FadeTableOrig::original_tables[256]; // 始めはすべて 0
FadeTableOrig* FadeTableOrig::original_diftables[1024];
const FadeTableOrig* FadeTableOrig::GetTable(int count) {
	// エラー、あるいは count == 0
	if (count <= 0 || count > 0xff) return &original_tables[0];
	// 既にテーブルが初期化済み(内容が0以外)
	if (original_tables[count].table1[31]) return &original_tables[count];
	// テーブル作製
	FadeTableOrig& table = original_tables[count];
	int i;
	for (i=-31; i<32; i++) {
		table.table1[i] = ((count*i)/255) << 11;
		table.table3[i] = ((count*i)/255);
	}
	for (i=-63; i<64; i++) {
		table.table2[i] = ((count*i)/255) << 5;
	}
	for (i=-255; i<256; i++) {
		table.table4[i] = ((count*i)/255);
	}
	return &table;
}
const FadeTableOrig* FadeTableOrig::DifTable(int old_d, int new_d) {
	// エラー
	if (old_d == new_d || old_d < 0 || new_d < 0 || old_d > 32 || new_d > 32) return 0;
	// 既にテーブルが存在
	if (original_diftables[old_d*32+new_d]) return original_diftables[old_d*32+new_d];
	// テーブル作製
	FadeTableOrig* table = original_diftables[old_d*32+new_d] = new FadeTableOrig;
	int i; int new_c, old_c;
	new_c=0, old_c=0;
	for (i=0; i<32; i++) {
		table->table1_minus[i] = ((new_c>>5) - (old_c>>5))<<11;
		table->table2_minus[i] = ((new_c>>5) - (old_c>>5))<<5;
		table->table3_minus[i] = ((new_c>>5) - (old_c>>5));
		table->table4_minus[i] = ((new_c>>5) - (old_c>>5));
		table->table1_minus[(-i)&0x3f] = -((new_c>>5) - (old_c>>5))<<11;
		table->table2_minus[(-i)&0x7f] = -((new_c>>5) - (old_c>>5))<<5;
		table->table3_minus[(-i)&0x3f] = -((new_c>>5) - (old_c>>5));
		table->table4_minus[(-i)&0x1ff] = -((new_c>>5) - (old_c>>5));
		new_c += new_d;
		old_c += old_d;
	}
	for (; i<64; i++) {
		table->table2_minus[i] = ((new_c>>5) - (old_c>>5))<<5;
		table->table4_minus[i] = ((new_c>>5) - (old_c>>5));
		table->table2_minus[(-i)&0x7f] = -((new_c>>5) - (old_c>>5))<<5;
		table->table4_minus[(-i)&0x1ff] = -((new_c>>5) - (old_c>>5));
		new_c += new_d;
		old_c += old_d;
	}
	for (; i<256; i++) {
		table->table4_minus[i] = ((new_c>>5) - (old_c>>5));
		table->table4_minus[(-i)&0x1ff] = -((new_c>>5) - (old_c>>5));
		new_c += new_d;
		old_c += old_d;
	}
	return table;
}
void FadeTable::SetTable(int count) {
	const FadeTableOrig& original = *FadeTableOrig::GetTable(count);
	table16_1 = original.table1;
	table16_2 = original.table2;
	table16_3 = original.table3;
	table32_1 = original.table4;
	table32_2 = original.table4;
	table32_3 = original.table4;
	return;
}
void FadeTable::SetTable(int c1, int c2, int c3) {
	const FadeTableOrig& original1 = *FadeTableOrig::GetTable(c1);
	const FadeTableOrig& original2 = *FadeTableOrig::GetTable(c2);
	const FadeTableOrig& original3 = *FadeTableOrig::GetTable(c3);
	table16_1 = original1.table1;
	table16_2 = original2.table2;
	table16_3 = original3.table3;
	table32_1 = original1.table4;
	table32_2 = original2.table4;
	table32_3 = original3.table4;
	return;
}

/* matrix calculation for transform */
int CalcTrilinearMatrix(double ret_matrix[3][3], int* dest_axis, int* src_axis);
/* Trilinear 変換をするための行列 M を計算する
** 計算としては、
**  x' = M x として、x = 変換後の座標、y を変換前の座標となる
**  拘束条件 1：転送前座標 4 つが正確に転送後座標 4 つに変換すること
**  拘束条件 2：M_33 = 1 ( w の 定数項は 1 )
**  これで方程式８つに対し不定項が８つになる。
**
** 具体的な計算：
** trilinear 変換の定義により、
** u,v : 転送前座標 x,y : 転送後座標
** (u,v) = (s/w, t/w)
**  a b c   x    s
** (d e f)x(y)= (t)
**  g h 1   1    w
** よって
**  a b c   x    uw
** (d e f)x(y)= (vw)
**  g h 1   1    w
** 展開して
**   ax + by + c = u w
**   dx + ey + f = v w
**   gx + hy + 1 = w
** 不定項 a,b,c,d,e,f,g,h の一次方程式の形にまとめると
**   xa + yb + c - uxg - uyh - u = 0
**   xd + ye + f - vxg - vyh - v = 0
** (u,v) -> (x,y) の組み合わせは 4 つあるので、全部で 8 方程式。
*/

#define CALC_VERBOSE 0

static int ResolveMatrix(double matrix[8][9]);
static int PrintMatrix(double matrix[8][9]);
int CalcTrilinearMatrix(double ret_matrix[3][3],
	int* dest_axis, int* src_axis) {
	double eq_matrix[8][9];
	int i,j;
	for (i=0; i<8; i++) {
		for (j=0; j<9; j++) {
			eq_matrix[i][j] = 0;
		}
	}
	for (i=0; i<4; i++) {
		double* eq_matrix1 = eq_matrix[i];
		double* eq_matrix2 = eq_matrix[i+4];
		eq_matrix1[0] = dest_axis[0];
		eq_matrix1[1] = dest_axis[1];
		eq_matrix1[2] = 1;
		eq_matrix1[6] = -src_axis[0] * dest_axis[0];
		eq_matrix1[7] = -src_axis[0] * dest_axis[1];
		eq_matrix1[8] = -src_axis[0];

		eq_matrix2[3] = dest_axis[0];
		eq_matrix2[4] = dest_axis[1];
		eq_matrix2[5] = 1;
		eq_matrix2[6] = -src_axis[1] * dest_axis[0];
		eq_matrix2[7] = -src_axis[1] * dest_axis[1];
		eq_matrix2[8] = -src_axis[1];
		dest_axis += 2;
		src_axis += 2;
	}
	if (! ResolveMatrix(eq_matrix)) {
		/* 方程式を求めるのに失敗した */
		return 0;
	}
	/* 検算 */
	if (CALC_VERBOSE) {
		double r[8]; for (i=0;i<8;i++) r[i]=-eq_matrix[i][8]/eq_matrix[i][i];
		dest_axis -= 8; src_axis -= 8;
		for (i=0; i<4; i++) {
			double x=dest_axis[0];
			double y=dest_axis[1];
			double u=src_axis[0];
			double v=src_axis[1];
			double a1 = x*r[0]+y*r[1]+r[2]-u*x*r[6]-u*y*r[7]-u;
			double a2 = x*r[3]+y*r[4]+r[5]-v*x*r[6]-v*y*r[7]-v;
			printf("dif %lf %lf, ",a1,a2);
			dest_axis += 2;
			src_axis += 2;
		}
		printf("\n");
	}
	/* 結果を格納 */
	for (i=0; i<3; i++) {
		for (j=0; j<3; j++) {
			int n = i*3 + j;
			if (n == 8) {
				ret_matrix[i][j] = 1;
			} else {
				ret_matrix[i][j] = - eq_matrix[n][8] / eq_matrix[n][n];
			}
		}
	}
	return 1;
}
/* matrix の column 行 / 列以降を階段上になるように行入れ換え */
static int SortMatrix(double matrix[8][9], int column) {
	double copy_matrix[8][9];
	int copied_flag[8];
	int copy_line = column;
	int i,j;
	memcpy(copy_matrix, matrix, sizeof(copy_matrix));
	memset(copied_flag, 0, sizeof(copied_flag));
	for (i=column; i<8; i++) {
		/* i 番目の列が 0 でない行を探してコピー */
		for (j=column; j<8; j++) {
			if (copied_flag[j]) continue;
			if (copy_matrix[j][i] != 0) {
				memcpy(matrix[copy_line], copy_matrix[j], sizeof(double)*9);
				copied_flag[j] = 1;
				copy_line++;
			}
		}
	}
	return 1;
}
/* column 番目の行について掃き出しを行う */
static int HakidasiMatrix(double matrix[8][9], int column) {
	int i,j;
	if (matrix[column][column] == 0) {
		SortMatrix(matrix, column);
	}
	/* column 番目の要素を持つ行がない
	** → この一次方程式は不定解を持つ(失敗)
	*/
	if (matrix[column][column] == 0) return 0;
	double n = matrix[column][column];
	double* c_matrix = matrix[column];
	for (i=0; i<8; i++) {
		if (i == column) continue;
		double m = matrix[i][column];
		if (m == 0) continue;
		
		for (j=0; j<9; j++) {
			if (j == column) matrix[i][j] = 0;
			else matrix[i][j] -= c_matrix[j] * m / n;
		}
	}
	return 1; /* 成功 */
}
/* 方程式を解く */
static int ResolveMatrix(double matrix[8][9]) {
	int i;
	if (CALC_VERBOSE) {
		printf("first: \n"); PrintMatrix(matrix);
	}
	for (i=0; i<8; i++) {
		if (CALC_VERBOSE) {
			printf("%d th: \n",i); PrintMatrix(matrix);
		}
		if (! HakidasiMatrix(matrix, i)) return 0;
	}
	if (CALC_VERBOSE) {
		printf("success! \n"); PrintMatrix(matrix);
	}
	return 1; /* 成功 */
}
static int PrintMatrix(double matrix[8][9]) {
	int i,j;
	for (i=0; i<8; i++) {
		for (j=0; j<9; j++) {
			printf("%5.5f ",matrix[i][j]);
		}
		printf("\n");
	}
	return 1;
}

/* double の matrix を、matrix の最大値の絶対値が int_max になるように
** 定数倍した上で int の matrix に変換する
*/
int ConvertMatrixInt(double matrix[3][3], int matrix_i[3][3], int int_max) {
	int i,j;
	double mmax = 0;
	/* 行列の最大値を得る */
	for (i=0; i<3; i++) {
		for (j=0; j<2; j++) {
			double mm = fabs(matrix[i][j]);
			if (mm > mmax) mmax = mm;
		}
		/* 定数項は 1/1024 して比較 */
		double mm = fabs(matrix[i][2])/1024;
		if (mm > mmax) mmax = mm;
	}
	/* 最大値で規格化、整数にする */
	/* 最大値 == 2^20 (=65536*16) */
	for (i=0; i<3; i++) {
		for (j=0; j<3; j++) {
			matrix_i[i][j] = int(matrix[i][j]/mmax*(65536*16));
		}
	}
	return 1;
}

