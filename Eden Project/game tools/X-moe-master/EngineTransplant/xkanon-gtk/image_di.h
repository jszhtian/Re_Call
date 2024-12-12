/*  image_di.h
 *       GdkImage の代わりになるクラス DI_Image
 *	 image への操作はクラス外のメソッドで行う
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

#ifndef __KANON_DI_IMAGE_H__
#define __KANON_DI_IMAGE_H__

/* DI_Image class の定義
**	DI_Image とは、GdkImage のデータを実際に操作するためのクラス
*/

#include<gdk/gdktypes.h> // forward declaration of GdkImage

class DI_Image {
private:
	DI_Image(const DI_Image&) {}
public:
	bool data_notdiscard; // data が破棄不要ならセットする
	char* data;
	int width, height;
	int bypp, bpl; // bypp = 2 なら、data は short, 565 。bypp = 4 なら data は int, バイトごとにrgb
		// それ以外の bypp は未サポート 。 bypp == byte per pixel
	DI_Image(void) {
		data = 0; width = 0; height = 0;
		bypp = 0; bpl = 0;
		data_notdiscard = 0;
	};
	void CreateImage(int _width, int _height, int _bypp) {
		if (data && (!data_notdiscard)) delete[] data;
		data = new char[_width*_height*_bypp];
		width = _width;
		height = _height;
		bypp = _bypp;
		bpl = width * bypp;
		data_notdiscard = false;
		RecordChangedRegionAll();
	}
	void SetImage(char* _data, int _width, int _height, int _bypp, int _bpl, bool _data_notdiscard=false) {
		if (data && (!data_notdiscard)) delete[] data;
		data = _data;
		width = _width;
		height = _height;
		bypp = _bypp;
		bpl = _bpl;
		data_notdiscard = _data_notdiscard;
		RecordChangedRegionAll();
	}
	void SetHeight(int h) { height = h;}
	void SetImage(GdkImage* orig);
	virtual ~DI_Image() {
		if (data && (!data_notdiscard)) delete[] data;
	};
	/* 描画された領域を保存するためのメソッド群 */
	/* 画面への描画が行われると画面idとともに clear される */
	virtual void RecordChangedRegion(int x1, int y1, int width, int height);
	virtual void RecordChangedRegionAll(void);
	virtual bool IsChangedRegionAll(void);
	virtual bool GetChangedRegion(int index, int count, int& x, int& y, int& w, int& h);
	virtual void ClearChangedRegion(int index);
	virtual int GetLastIndex(void);
};

class DI_ImageMask : virtual public DI_Image { // マスク付き
	bool mask_notdiscard; // mask が破棄不要ならセットする
	char* mask;
	int system_used; // 現在使用中か
public:
	DI_ImageMask(void) : DI_Image() {
		mask = 0; system_used = 0; mask_notdiscard = false;
	}
	void SetMask(char* _mask, bool discard=false) {
		if (mask && mask_notdiscard == false) delete[] mask;
		mask = _mask;
		mask_notdiscard = discard;
	}
	void SetCopyMask(DI_ImageMask* src);
	char* Mask(void) const { return mask; }
	void ClearUsed(void) { system_used = 0; }
	void SetUsed(void) { system_used = 1; }
	int IsUsed(void) const { return (system_used != 0); }
	virtual ~DI_ImageMask() {
		if (mask && mask_notdiscard == false) delete mask;
	}
};

DI_ImageMask* CreateIcon(class AyuSys& sys);

// 描画のメソッド
// コピー
void CopyAllWithMask_16bpp(DI_Image& dest, DI_Image& src, char* mask);
void CopyAllWithMask_32bpp(DI_Image& dest, DI_Image& src, char* mask);
void CopyRectWithMask_16bpp(DI_Image& dest, int dest_x, int dest_y, DI_Image& src, int src_x, int src_y, int width, int height, char* mask);
void CopyRectWithMask_32bpp(DI_Image& dest, int dest_x, int dest_y, DI_Image& src, int src_x, int src_y, int width, int height, char* mask);
void CopyAll(DI_Image& dest, DI_Image& src);
void CopyRect(DI_Image& dest, int dest_x, int dest_y, DI_Image& src, int src_x, int src_y, int width, int height);
void CopyRectWithoutColor(DI_Image& dest, int dest_x, int dest_y, DI_Image& src, int src_x, int src_y, int width, int height, int c1, int c2, int c3);
void SwapRect(DI_Image& dest, int dest_x, int dest_y, DI_Image& src, int src_x, int src_y, int width, int height);

inline void CopyAllWithMask(DI_Image* dest, DI_Image* src, char* mask) {
	if (mask == 0) CopyAll(*dest, *src);
	else if (dest->bypp == 2) CopyAllWithMask_16bpp(*dest,*src,mask);
	else if (dest->bypp == 4) CopyAllWithMask_32bpp(*dest,*src,mask);
}
inline void CopyRectWithMask(DI_Image* dest, int dest_x, int dest_y, DI_Image* src, int src_x, int src_y, int width, int height, char* mask) {
	if (mask == 0) CopyRect(*dest, dest_x, dest_y, *src, src_x, src_y, width, height);
	else if (dest->bypp == 2) CopyRectWithMask_16bpp(*dest, dest_x, dest_y, *src, src_x, src_y, width, height, mask);
	else if (dest->bypp == 4) CopyRectWithMask_32bpp(*dest, dest_x, dest_y, *src, src_x, src_y, width, height, mask);
}

void CopyRectWithoutColor_16bpp(DI_Image&, int, int, DI_Image&, int, int, int, int, int, int, int);
void CopyRectWithoutColor_32bpp(DI_Image&, int, int, DI_Image&, int, int, int, int, int, int, int);
inline void CopyRectWithoutColor(DI_Image* dest, int dest_x, int dest_y, DI_Image* src, int src_x, int src_y, int width, int height, int c1, int c2, int c3) {
	if (dest->bypp == 2) CopyRectWithoutColor_16bpp(*dest, dest_x, dest_y, *src, src_x, src_y, width, height, c1, c2, c3);
	else if (dest->bypp == 4) CopyRectWithoutColor_32bpp(*dest, dest_x, dest_y, *src, src_x, src_y, width, height, c1, c2, c3);
}

inline void CopyAll(DI_Image* dest, DI_ImageMask* src) {
	CopyAllWithMask(dest, src, src->Mask());
}
inline void CopyRect(DI_Image* dest, int dest_x, int dest_y, DI_ImageMask* src, int src_x, int src_y, int width, int height) {
	CopyRectWithMask(dest, dest_x, dest_y, src, src_x, src_y, width, height, src->Mask());
}

// 拡大・縮小
void CopyRectWithStretch_16bpp(DI_Image& dest, int dest_x, int dest_y, int dwidth, int dheight,
	DI_Image& src, int src_x, int src_y, int swidth, int sheight, int fade = 255);
void CopyRectWithStretch_32bpp(DI_Image& dest, int dest_x, int dest_y, int dwidth, int dheight,
	DI_Image& src, int src_x, int src_y, int swidth, int sheight, int fade = 255);

inline void CopyRectWithStretch(DI_Image& dest, int dest_x, int dest_y, int dwidth, int dheight,
	DI_Image& src, int src_x, int src_y, int swidth, int sheight, int fade = 255) {
	if (dest.bypp == 2) CopyRectWithStretch_16bpp(dest, dest_x, dest_y, dwidth, dheight, src, src_x, src_y, swidth, sheight, fade);
	else if (dest.bypp == 4) CopyRectWithStretch_32bpp(dest, dest_x, dest_y, dwidth, dheight, src, src_x, src_y, swidth, sheight, fade);
}

void CopyRectWithTransform_16bpp(DI_Image& dest,
        int dest_x1, int dest_y1, int dest_x2, int dest_y2,
        int dest_x3, int dest_y3, int dest_x4, int dest_y4,
        DI_Image& src, int src_x, int src_y, int src_width, int src_height,
        bool is_fillback, int c1, int c2, int c3, int fade);
void CopyRectWithTransform_32bpp(DI_Image& dest,
        int dest_x1, int dest_y1, int dest_x2, int dest_y2,
        int dest_x3, int dest_y3, int dest_x4, int dest_y4,
        DI_Image& src, int src_x, int src_y, int src_width, int src_height,
        bool is_fillback, int c1, int c2, int c3, int fade);
inline void CopyRectWithTransform(DI_Image& dest,
        int dest_x1, int dest_y1, int dest_x2, int dest_y2,
        int dest_x3, int dest_y3, int dest_x4, int dest_y4,
        DI_Image& src, int src_x, int src_y, int src_width, int src_height,
        bool is_fillback=false, int c1=0, int c2=0, int c3=0, int fade = 255) {
	if (dest.bypp == 2) CopyRectWithTransform_16bpp(dest, dest_x1, dest_y1, dest_x2, dest_y2, dest_x3, dest_y3, dest_x4, dest_y4, src, src_x, src_y, src_width, src_height, is_fillback, c1, c2, c3,fade);
	else if (dest.bypp == 4) CopyRectWithTransform_32bpp(dest, dest_x1, dest_y1, dest_x2, dest_y2, dest_x3, dest_y3, dest_x4, dest_y4, src, src_x, src_y, src_width, src_height, is_fillback, c1, c2, c3,fade);
}

// 変換
void ConvertMonochrome_16bpp(DI_Image& dest, int x, int y, int width, int height);
void ConvertMonochrome_32bpp(DI_Image& dest, int x, int y, int width, int height);
inline void ConvertMonochrome(DI_Image& dest, int x, int y, int width, int height) {
	if (dest.bypp == 2) ConvertMonochrome_16bpp(dest,x,y,width,height);
	else if (dest.bypp == 4) ConvertMonochrome_32bpp(dest,x,y,width,height);
}
void InvertColor_16bpp(DI_Image& dest, int x, int y, int width, int height);
void InvertColor_32bpp(DI_Image& dest, int x, int y, int width, int height);
inline void InvertColor(DI_Image& dest, int x, int y, int width, int height) {
	if (dest.bypp == 2) InvertColor_16bpp(dest,x,y,width,height);
	else if (dest.bypp == 4) InvertColor_32bpp(dest,x,y,width,height);
}

// ２つの画像を重ね合わせる
void CopyRectWithFade_16bpp(DI_Image& dest, int dest_x, int dest_y, DI_Image& src, int src_x, int src_y, int width, int height, int fade);
void CopyRectWithFade_32bpp(DI_Image& dest, int dest_x, int dest_y, DI_Image& src, int src_x, int src_y, int width, int height, int fade);
void CopyRectWithFadeWithMask_16bpp(DI_Image& dest, int dest_x, int dest_y, DI_Image& src, int src_x, int src_y, int width, int height, char* mask, int fade);
void CopyRectWithFadeWithMask_32bpp(DI_Image& dest, int dest_x, int dest_y, DI_Image& src, int src_x, int src_y, int width, int height, char* mask, int fade);

inline void CopyRectWithFade(DI_Image* dest, int dest_x, int dest_y, DI_Image* src, int src_x, int src_y, int width, int height, int count) {
	if (dest->bypp == 2) CopyRectWithFade_16bpp(*dest, dest_x, dest_y, *src, src_x, src_y, width, height, count);
	else if (dest->bypp == 4) CopyRectWithFade_32bpp(*dest, dest_x, dest_y, *src, src_x, src_y, width, height, count);
}
inline void CopyRectWithFadeWithMask(DI_Image* dest, int dest_x, int dest_y, DI_Image* src, int src_x, int src_y, int width, int height, char* mask, int fade) {
	if (mask == 0) CopyRectWithFade(dest, dest_x, dest_y, src, src_x, src_y, width, height, fade);
	else if (dest->bypp == 2) CopyRectWithFadeWithMask_16bpp(*dest, dest_x, dest_y, *src, src_x, src_y, width, height, mask, fade);
	else if (dest->bypp == 4) CopyRectWithFadeWithMask_32bpp(*dest, dest_x, dest_y, *src, src_x, src_y, width, height, mask, fade);
}
inline void CopyRectWithFade(DI_Image* dest, int dest_x, int dest_y, DI_ImageMask* src, int src_x, int src_y, int width, int height, int count) {
	CopyRectWithFadeWithMask(dest, dest_x, dest_y, src, src_x, src_y, width, height, src->Mask(), count);
}

// クリア
// c1,c2,c3 は rgb
void ClearAll(DI_Image& dest, int c1, int c2, int c3);
void ClearWithoutRect(DI_Image& dest, int x1, int y1, int x2, int y2, int c1, int c2, int c3);
void ClearRect_16bpp(DI_Image& dest, int x1, int y1, int x2, int y2, int c1, int c2, int c3);
void ClearRect_32bpp(DI_Image& dest, int x1, int y1, int x2, int y2, int c1, int c2, int c3);

inline void ClearRect(DI_Image* dest, int x1, int y1, int x2, int y2, int c1, int c2, int c3) {
	if (dest->bypp == 2) ClearRect_16bpp(*dest, x1, y1, x2, y2, c1, c2, c3);
	else if (dest->bypp == 4) ClearRect_32bpp(*dest, x1, y1, x2, y2, c1, c2, c3);
}

// フェード・アウト
void FadeRect_16bpp(DI_Image& dest, int x1, int y1, int x2, int y2, int c1, int c2, int c3, int count);
void FadeRect_32bpp(DI_Image& dest, int x1, int y1, int x2, int y2, int c1, int c2, int c3, int count);
inline void FadeRect(DI_Image* dest, int x1, int y1, int x2, int y2,
	int c1, int c2, int c3, int count) {
	if (dest->bypp ==2) FadeRect_16bpp(*dest, x1, y1, x2, y2, c1, c2, c3, count);
	else if (dest->bypp ==4) FadeRect_32bpp(*dest, x1, y1, x2, y2, c1, c2, c3, count);
}

// 文字描画
// charmask は 0-255 の alpha 値
// DC_MONO ならば charmask は 0 か非0
// fc : foreground bc : background wc : wall color
enum DrawCharaType {DC_ALPHA=1, DC_MONO=2, DC_FADEWALL=4, DC_CLEARWALL=8, DC_NOTHING=16};

void DrawChara_16bpp(DI_Image& dest, int x, int y, int width, int height, 
	unsigned char* charmask, int maskwidth,
	DrawCharaType type,
	int fc1, int fc2, int fc3, int bc1=0, int bc2=0, int bc3=0,
	int wc1=255, int wc2=255, int wc3=255);
void DrawChara_32bpp(DI_Image& dest, int x, int y, int width, int height, 
	unsigned char* charmask, int maskwidth,
	DrawCharaType type,
	int fc1, int fc2, int fc3, int bc1=0, int bc2=0, int bc3=0,
	int wc1=255, int wc2=255, int wc3=255);

inline void DrawChara(DI_Image& dest, int x, int y, int width, int height, 
	unsigned char* charmask, int maskwidth,
	DrawCharaType type,
	int fc1, int fc2, int fc3, int bc1=0, int bc2=0, int bc3=0,
	int wc1=255, int wc2=255, int wc3=255) {

	if (dest.bypp == 2)
		DrawChara_16bpp(dest, x, y, width, height,
			charmask, maskwidth, type, fc1, fc2, fc3, bc1, bc2, bc3,
			wc1, wc2, wc3);
	else if (dest.bypp == 4)
		DrawChara_32bpp(dest, x, y, width, height,
			charmask, maskwidth, type, fc1, fc2, fc3, bc1, bc2, bc3,
			wc1, wc2, wc3);
}

// すこしずつ画面に表示
// 返り値：
//	0 -> 実際の画像転送は起きなかった
//	1 -> 普通に転送
//	2 -> 転送終了
//	-1 -> sel が未サポート
// SEL : 描画の方法をしめす
struct SEL_STRUCT {
	int x1,y1;
	int x2,y2;
	int x3,y3;
	int wait_time; // 時間待ち
	int sel_no; // 描画法
	int kasane; // 重ね合わせフラグ
	int arg1, arg2, arg3, arg4, arg5,arg6; // その他パラメータ
	// 描画時に使うための一時パラメータ
	int params[16];
	SEL_STRUCT(void) {
		x1=y1=x2=y2=x3=y3=0;
		wait_time = sel_no = kasane=0;
		arg1=arg2=arg3=arg4=arg5=arg6=0;
		params[0] = params[1] = params[2] = params[3] = 0;
		params[4] = params[5] = params[6] = params[7] = 0;
		params[8] = params[9] = params[10] = params[11] = 0;
		params[12] = params[13] = params[14] = params[15] = 0;
	}
};

int CopyWithSel(DI_Image* dest, DI_ImageMask* src, SEL_STRUCT* sel, int count);

/* min <= v1 <= v2 <= max にする */
inline void fix_axis(int min, int& v1, int& v2, int max) {
	if (v1 > v2) { // swap
		int tmp = v1; v1 = v2; v2 = tmp;
	}
	if (v1 <= min) v1 = min;
	else if (v1 >= max) v1 = max;
	if (v2 <= min) v2 = min;
	else if (v2 >= max) v2 = max;
	return;
}
/* min <= v1 <= max にする */
inline void fix_axis(int min, int& v1, int max) {
	if (v1 <= min) v1 = min;
	else if (v1 >= max) v1 = max;
	return;
}

/* fade 付き描画で使われる構造体 */
struct FadeTable {
	const int* table16_1;
	const int* table16_2;
	const int* table16_3;
	const int* table32_1;
	const int* table32_2;
	const int* table32_3;
	void SetTable(int count);
	void SetTable(int c1, int c2, int c3);
};
struct FadeTableOrig {
	int table1_minus[32];
	int table1[32];
	int table2_minus[64];
	int table2[64];
	int table3_minus[32];
	int table3[32];
	int table4_minus[256];
	int table4[256];
	static const FadeTableOrig* GetTable(int count);
	static const FadeTableOrig* DifTable(int old_d, int new_d);
	static FadeTableOrig original_tables[256];
	static FadeTableOrig* original_diftables[1024];
};
#endif /* defined(__KANON_DI_IMAGE_H__) */
