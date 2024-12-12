// #include "system.h"
// #include <string.h>
// #include<stdio.h>
// #include<stdlib.h>
#include <math.h>
#include<assert.h>
#include<algorithm>
#include"image_di_seldraw.h"
#include"initial.h"

RegisterSelMacro(50, TLI1(50), WithMask) /* sel50: 重ね合わせで徐々に表示 */
::Exec(DI_Image& dest, DI_Image& src, char* mask, SEL_STRUCT* sel, int count) {
	/* 変数初期化 */
	int width = sel->x2-sel->x1+1; int height = sel->y2-sel->y1+1;
	char* dest_buf = dest.data + sel->x3*ByPP + sel->y3*dest.bpl;
	DifImage* difimage = (DifImage*)sel->params[0];
	int i,j;

	if (count == 0) {
		// 描画開始時の初期化
		difimage = MakeDifImage(dest, src, mask, sel);
		sel->params[0] = int(difimage);
		sel->params[2] = 0;
		return 1;
	}

	int max = sel->arg4; if (max == 0) max = 16;
	// まず、テーブルを作る
	// 描画カウントの更新
	int old_count = sel->params[2];
	if (count > max) count = max;
	int new_count = count;
	sel->params[2] = count;

	// [old|new]_count : 0-1024 の間の数値を取る
	old_count = old_count*1024/max;
	new_count = new_count*1024/max;
	int new_c, old_c;
	if (Bpp == 16) {
		int dat1[64];
		new_c=0, old_c=0;
		for (i=0; i<32; i++) {
			int c = new_c/1024 - old_c/1024;
			dat1[i] = c;
			dat1[ (-i)&0x3f] = -c;
			new_c += new_count; old_c += old_count;
		}
		int dat2[128];
		new_c=0, old_c=0;
		for (i=0; i<64; i++) {
			int c = new_c/1024 - old_c/1024;
			dat2[i] = c;
			dat2[ (-i)&0x7f] = -c;
			new_c += new_count; old_c += old_count;
		}
		// 差分データからデータをつくる
		char* dif_buf = difimage->difimage;
		int* draw_xlist = difimage->draw_xlist;
		for (i=0; i<height; i++) {
			char* d = dest_buf; dest_buf += dest.bpl;
			char* dif = dif_buf; dif_buf += width*3;
	
			int x_max = draw_xlist[1] - draw_xlist[0];
			d += 2*draw_xlist[0]; dif += 3*draw_xlist[0];
			draw_xlist += 2;
			for (j=0; j<x_max; j++) {
				int c = *(unsigned short*)d;
				c += dat1[(int)dif[0]]<<11;
				c += dat2[(int)dif[1]]<<5;
				c += dat1[(int)dif[2]];
				*(unsigned short*)d = c;
				d += 2; dif += 3;
			}
		}
	} else { /* Bpp == 32 */
		int dat3[512];
		new_c=0, old_c=0;
		for (i=0; i<256; i++) {
			int c = new_c/1024 - old_c/1024;
			dat3[i] = c;
			dat3[ (-i)&0x1ff] = -c;
			new_c += new_count; old_c += old_count;
		}
		// 差分データからデータをつくる
		int* dif_buf = (int*)(difimage->difimage);
		int* draw_xlist = difimage->draw_xlist;
		for (i=0; i<height; i++) {
			char* d = dest_buf; dest_buf += dest.bpl;
			int* dif = dif_buf; dif_buf += width;
	
			int x_max = draw_xlist[1] - draw_xlist[0];
			d += ByPP*draw_xlist[0]; dif += draw_xlist[0];
			draw_xlist += 2;
			for (j=0; j<x_max; j++) {
				int c = *dif;
				d[0] += dat3[c&0x1ff];
				d[1] += dat3[(c>>9)&0x1ff];
				d[2] += dat3[(c>>18)&0x1ff];
				d += ByPP; dif ++;
			}
		}
	}
	if (count == max) {
		delete difimage;
		return 2;
	} else {
		return 1;
	}
}

RegisterSelMacro(54, TLI1(54), NoMask) /* sel54: 輝度順に描画 */
::Exec(DI_Image& dest, DI_Image& src, char* mask, SEL_STRUCT* sel, int count) {
	if (count == 0) {
		// 初期化
		int width = sel->x2-sel->x1+1; int height = sel->y2-sel->y1+1;
		char* src_buf = src.data + sel->x1*ByPP + sel->y1*src.bpl;
		/* 輝度計算 */
		char* kido_buf =
			sel->arg5 ? CalcKido(dest.data+sel->x3*ByPP+sel->y3*dest.bpl, dest.bpl, width, height, sel->arg4)
			: CalcKido(src_buf, src.bpl, width, height, sel->arg4);
		/* バッファに格納 */
		sel->params[0] = int(src_buf); sel->params[1] = int(kido_buf);
		sel->params[2] = width; sel->params[3] = height; sel->params[4] = src.bpl;
		sel->params[5] = 0; sel->params[6] = 0;
		return 1;
	} else {
		int last_count = sel->arg4; if (last_count == 0) last_count = 16;
		if (count > last_count) count = last_count;
		int min = sel->params[6]; int max = count;
		/* データ取り出し */
		int i,j;
		char* dest_buf = dest.data+sel->x3*ByPP+sel->y3*dest.bpl;
		char* src_buf = (char*)sel->params[0];
		char* kido_buf = (char*)sel->params[1];
		int width = sel->params[2]; int height = sel->params[3];
		int sbpl = sel->params[4];
		/* 輝度順にコピー */
		for (i=0; i<height; i++) {
			char* d = dest_buf; char* s = src_buf;
			for (j=0; j<width; j++) {
				if (*kido_buf>=min && *kido_buf<max) Copy1Pixel(d,s);
				d += ByPP; s += ByPP; kido_buf++;
			}
			dest_buf += dest.bpl; src_buf += sbpl;
		}
		/* 次の準備 */
		/* 終わりならバッファを開放 */
		sel->params[6] = max;
		if (count == last_count) {
			if (sel->params[5]) delete[] (char*)sel->params[0]; // WithMask なら params[0] を解放
			delete[] (char*)sel->params[1];
			return 2;
		} else
			return 1;
	}
}

RegisterSelMacro(160, TLI1(160), NoMask) /* sel 160 : 中心から新しい画像が拡大していく */
::Exec(DI_Image& dest, DI_Image& src, char* mask, SEL_STRUCT* sel, int count) {
	int width, height, max_count;
	width = sel->x2-sel->x1+1;
	height= sel->y2-sel->y1+1;
	if (sel->arg4 != 0) {
		max_count = (width > height) ? width : height;
		max_count /= sel->arg4;
		if (max_count <= 0) max_count = 10;
	} else max_count = 10;
	if (count >= max_count-1) {
		CopyAll(dest, src);
		return 2;
	}
	count++;
	int new_width = width*count/max_count;
	int new_height = height*count/max_count;
	// stretch する
	CopyRectWithStretch(dest, sel->x3+(width-new_width)/2, sel->y3+(height-new_height)/2,
		new_width, new_height,
		src, sel->x1, sel->y1, width, height);
	return 1;
}

RegisterSelMacro(161, TLI1(161), NoMask) /* sel 161 : 画像が中心へと収縮し、残りが新しい画像になる */
::Exec(DI_Image& dest, DI_Image& src, char* mask, SEL_STRUCT* sel, int count) {
	int width, height, max_count;
	width = sel->x2-sel->x1+1;
	height= sel->y2-sel->y1+1;
	if (count == 0) {
		/* 古い画像を保存 */
		DI_Image* dest_orig = new DI_Image;
		dest_orig->CreateImage(width, height, ByPP);
		CopyRect(*dest_orig, 0, 0, dest, sel->x3, sel->y3, width, height);
		sel->params[0] = int(dest_orig->data);
		sel->params[1] = int(dest_orig);
		sel->params[2] = width; sel->params[3] = height;
	}
	if (sel->arg4 != 0) {
		max_count = (width > height) ? width : height;
		max_count /= sel->arg4;
		if (max_count <= 0) max_count = 10;
	} else max_count = 10;
	count++;
	if (count >= max_count) count = max_count;
	/* コピーする領域の決定 */
	int old_width = sel->params[2];
	int old_height= sel->params[3];
	int new_width = width - width*count/max_count;
	int new_height = height - height*count/max_count;
	DI_Image& dest_orig = *(DI_Image*)sel->params[1];
	/* コピーする */
	int old_x1 = (width-old_width)/2;
	int old_y1 = (height-old_height)/2;
	int old_x2 = width - old_x1;
	int old_y2 = height - old_y1;
	int new_x1 = (width-new_width)/2;
	int new_y1 = (height-new_height)/2;
	int new_x2 = width - new_x1;
	int new_y2 = height - new_y1;

	CopyRect(dest, sel->x3+old_x1, sel->y3+old_y1, src, sel->x1+old_x1, sel->y1+old_y1,
		old_x2-old_x1, new_y1-old_y1);
	CopyRect(dest, sel->x3+old_x1, sel->y3+new_y1, src, sel->x1+old_x1, sel->y1+new_y1,
		new_x1-old_x1, new_y2-new_y1);
	CopyRect(dest, sel->x3+new_x2, sel->y3+new_y1, src, sel->x1+new_x2, sel->y1+new_y1,
		old_x2-new_x2, new_y2-new_y1);
	CopyRect(dest, sel->x3+old_x1, sel->y3+new_y2, src, sel->x1+old_x1, sel->y1+new_y2,
		old_x2-old_x1, old_y2-new_y2);
	// stretch する
	if (new_width != 0 && new_height != 0) {
		CopyRectWithStretch(dest, sel->x3+new_x1, sel->y3+new_y1,
			new_x2-new_x1, new_y2-new_y1,
			dest_orig, 0, 0, width, height);
	}
	if (count >= max_count) {
		/* 終了 */
		delete (DI_Image*)sel->params[1];
		return 2;
	}
	sel->params[2] = new_width;
	sel->params[3] = new_height;
	return 1;
}

RegisterSelMacro(162, TLI1(162), NoMask) /* sel 162 : 画像が最大まで拡大され、その後新しい画像がもとの大きさまで戻る*/
::Exec(DI_Image& dest, DI_Image& src, char* mask, SEL_STRUCT* sel, int count) {
	int width, height, max_count;
	width = sel->x2-sel->x1+1;
	height= sel->y2-sel->y1+1;
	if (count == 0) {
		/* 古い画像を保存 */
		DI_Image* dest_orig = new DI_Image;
		dest_orig->CreateImage(width, height, ByPP);
		CopyRect(*dest_orig, 0, 0, dest, sel->x3, sel->y3, width, height);
		sel->params[0] = int(dest_orig->data);
		sel->params[1] = int(dest_orig);
		if (sel->arg4 != 0) {
			max_count = (width > height) ? width : height;
			max_count /= sel->arg4;
			if (max_count <= 0) max_count = 10;
		} else max_count = 10;
		sel->params[2] = max_count;
	}
	max_count = sel->params[2];
	if (count < max_count) {
		/* dest_orig -> dest のコピー */
		int new_width = width - width*count/max_count;
		int new_height = height - height*count/max_count;
		CopyRectWithStretch(dest, sel->x3, sel->y3, width, height,
			*(DI_Image*)sel->params[1], (width-new_width)/2, (height-new_height)/2, new_width, new_height);
		return 1;
	} else if (count < max_count*2-1) {
		/* src -> dest のコピー */
		count = max_count*2-1-count;
		int new_width = width - width*count/max_count;
		int new_height = height - height*count/max_count;
		CopyRectWithStretch(dest, sel->x3, sel->y3, width, height,
			src, sel->x1+(width-new_width)/2, sel->y1+(height-new_height)/2, new_width, new_height);
		return 1;
	}
	CopyAll(dest, src);
	/* 終了 */
	delete (DI_Image*)sel->params[1];
	return 2;
}

RegisterSelMacro(163, TLI1(163), NoMask) /* sel 163 : 画像が最大まで拡大され、その後新しい画像がもとの大きさまで戻る*/
::Exec(DI_Image& dest, DI_Image& src, char* mask, SEL_STRUCT* sel, int count) {
	int width, height, max_count;
	width = sel->x2-sel->x1+1;
	height= sel->y2-sel->y1+1;
	if (count == 0) {
		/* 古い画像を保存 */
		DI_Image* dest_orig = new DI_Image;
		dest_orig->CreateImage(width, height, ByPP);
		CopyRect(*dest_orig, 0, 0, dest, sel->x3, sel->y3, width, height);
		sel->params[0] = int(dest_orig->data);
		sel->params[1] = int(dest_orig);
		/* max count の計算 */
		int i; for (i=0; i<16; i++) {
			int c = 1<<i;
			if (width < c || height < c) {
				i--;
				break;
			}
		}
		sel->params[2] = i;
		return 0;

	}
	max_count = sel->params[2];
	if (count <= max_count) {
		/* dest_orig -> dest のコピー */
		int new_width = width>>count;
		int new_height = height>>count;
		CopyRectWithStretch(dest, sel->x3, sel->y3, width, height,
			*(DI_Image*)sel->params[1], (width-new_width)/2, (height-new_height)/2, new_width, new_height);
		return 1;
	} else if (count <= max_count*2) {
		/* src -> dest のコピー */
		count = max_count*2+1-count;
		int new_width = width>>count;
		int new_height = height>>count;
		CopyRectWithStretch(dest, sel->x3, sel->y3, width, height,
			src, sel->x1+(width-new_width)/2, sel->y1+(height-new_height)/2, new_width, new_height);
		return 1;
	}
	CopyAll(dest, src);
	/* 終了 */
	delete (DI_Image*)sel->params[1];
	return 2;
}


/* Stretch Copy を繰り返す物。src を拡大して dest にする */
/* パラメータの数が足りないので、arg6 の上16bit を max count の代わりに使う */
/* arg1,arg2 = dest x2,y2 arg3,4,5,6 が stretch の最後の src x1,y1,x2,y2 */
RegisterSelMacro(200, TLI1(200), NoMask) /* StretchCopy の繰り返し */
::Exec(DI_Image& dest, DI_Image& src, char* mask, SEL_STRUCT* sel, int count) {
	/* stretch のばあい、自分から自分にstretchの可能性がある */
	if (count == 0) {
		DI_Image* orig = new DI_Image;
		orig->CreateImage(src.width, src.height, ByPP);
		CopyRect(*orig, 0, 0, src, 0, 0, src.width, src.height);
		sel->params[0] = int(orig);
	}
	DI_Image* src_orig = (DI_Image*)sel->params[0];
	int max_count = (sel->arg6 >> 16) - 1;
	if (max_count <= 0) {
		max_count = 1; count = 1;
	}
	if (count > max_count) count = max_count;
	int rev_count = max_count-count;

	int arg6 = sel->arg6 & 0xffff;
	if (arg6 & 0x8000) arg6 |= (-1) & (~(0xffff)); // 符号拡張
	int src_x1 = (sel->x1 * rev_count + sel->arg3 * count) / max_count;
	int src_y1 = (sel->y1 * rev_count + sel->arg4 * count) / max_count;
	int src_x2 = (sel->x2 * rev_count + sel->arg5 * count) / max_count;
	int src_y2 = (sel->y2 * rev_count +      arg6 * count) / max_count;
	if (src_x2 < src_x1) {int tmp = src_x2; src_x2 = src_x1; src_x1 = tmp;}
	if (src_y2 < src_y1) {int tmp = src_y2; src_y2 = src_y1; src_y1 = tmp;}
	
	int src_width = src_x2 - src_x1 + 1;
	int src_height = src_y2 - src_y1 + 1;

	// dest は変化しない
	if (sel->arg1 < sel->x3) { int tmp=sel->arg1; sel->arg1 = sel->x3; sel->x3 = tmp; }
	if (sel->arg2 < sel->y3) { int tmp=sel->arg2; sel->arg2 = sel->y3; sel->y3 = tmp; }
	int dest_width = sel->arg1 - sel->x3 + 1; 
	int dest_height = sel->arg2 - sel->y3 + 1; 

	// stretch する
	CopyRectWithStretch(dest, sel->x3, sel->y3, dest_width, dest_height,
		*src_orig, src_x1, src_y1, src_width, src_height);
	if (count == max_count) {
		delete (DI_Image*)sel->params[0];
		return 2;
	}
	else return 1;
}
inline int CalcStretchCount1(int p1, int p2, int count, int max_count) {
	int c1 = count*count;
	int c2 = max_count*max_count;
	return (p2-p1)*c1/c2 + p1;
}
inline int CalcStretchCount2(int p1, int p2, int count, int max_count) {
	int c1 = count*count;
	int c2 = max_count*max_count;
	int c3 = (max_count-count)*c1/c2 + count;
	return (p2-p1)*c3/max_count + p1;
}

RegisterSelMacro(210, TLI3(210,211,213), NoMask) /* sel211, 213 : 一旦拡大して、新しい画面で縮小して元サイズに戻して終わり / 走る効果 */
::Exec(DI_Image& dest, DI_Image& src, char* mask, SEL_STRUCT* sel, int count) {
	if (count == 0) {
		DI_Image* orig = new DI_Image;
		orig->CreateImage(dest.width, dest.height, ByPP);
		CopyRect(*orig, 0, 0, dest, 0, 0, dest.width, dest.height);
		sel->params[0] = int(orig);
		sel->params[1] = 0;
	}
	DI_Image* dest_orig = (DI_Image*)sel->params[0];
	int max_count = 32;
	if (sel->arg4 != 0) max_count = sel->arg4;
	if ( (max_count&1) == 0) max_count++;
	if (count > max_count) count = max_count;
	/* alpha 度を計算 */
	int fade = 255;
	if (sel->sel_no == 210) {
/*
		fade = 16;
		if (sel->arg5 != 0) fade = sel->arg5;
		if (sel->params[1]+1 != count) {
			fade *= (count-sel->params[1]);
		}
		sel->params[1] = count;
		if (fade > 255) fade = 255;
		if (fade < 16) fade = 16;
*/
		fade = 128;
	}
	int dest_width = sel->x2 - sel->x1 + 1; 
	int dest_height = sel->y2 - sel->y1 + 1; 
	/* 拡大する座標を決める */
	int src_x1, src_y1, src_x2, src_y2;
	if (sel->sel_no <= 211) {
		if (count*2 < max_count) {
			int x_max = sel->x1 + dest_width - 1;
			int y_max = sel->y1 + dest_height - 1;
			int x_half = sel->x1 + dest_width / 2;
			int y_half = sel->y1 + dest_height / 2;
			src_x1 = CalcStretchCount1(sel->x1, x_half, count*2, max_count);
			src_x2 = CalcStretchCount1(x_max, x_half, count*2, max_count);
			src_y1 = CalcStretchCount1(sel->y1, y_half, count*2, max_count);
			src_y2 = CalcStretchCount1(y_max, y_half, count*2, max_count);
		} else {
			int x_half = (sel->x1 + sel->x2) / 2;
			int y_half = (sel->y1 + sel->y2) / 2;
			src_x1 = CalcStretchCount2(x_half, sel->x1, count*2-max_count, max_count);
			src_x2 = CalcStretchCount2(x_half, sel->x2, count*2-max_count, max_count);
			src_y1 = CalcStretchCount2(y_half, sel->y1, count*2-max_count, max_count);
			src_y2 = CalcStretchCount2(y_half, sel->y2, count*2-max_count, max_count);
		}
	} else { /* sel_no == 213 */
		src_x1 = 0;
		src_y1 = 0;
		if (count*2 < max_count) {
			src_x2 = sel->x3 + (dest_width *(max_count-count*2)/max_count)-1;
			src_y2 = sel->y3 + (dest_height*(max_count-count*2)/max_count)-1;
		} else {
			src_x2 = sel->x3 + (dest_width *(count*2-max_count)/max_count)-1;
			src_y2 = sel->y3 + (dest_height*(count*2-max_count)/max_count)-1;
		}
	}
	if (src_x2 < src_x1) {int tmp = src_x2; src_x2 = src_x1; src_x1 = tmp;}
	if (src_y2 < src_y1) {int tmp = src_y2; src_y2 = src_y1; src_y1 = tmp;}
	
	int src_width = src_x2 - src_x1 + 1;
	int src_height = src_y2 - src_y1 + 1;

	// stretch する
	if (count*2 < max_count) {
		CopyRectWithStretch(dest, sel->x3, sel->y3, dest_width, dest_height,
			*dest_orig, src_x1, src_y1, src_width, src_height, fade);
	} else if (count < max_count) {
		CopyRectWithStretch(dest, sel->x3, sel->y3, dest_width, dest_height,
			src, src_x1, src_y1, src_width, src_height, fade);
	} else {
		CopyRect(dest, sel->x3, sel->y3, src, sel->x1, sel->y1, dest_width, dest_height);
		delete (DI_Image*)sel->params[0];
		return 2;
	}
	return 1;
}
RegisterSelMacro(212, TLI1(212), NoMask) /* sel212 :拡大しながら重ね合わせ。それ散るの45番。*/
::Exec(DI_Image& dest, DI_Image& src, char* mask, SEL_STRUCT* sel, int count) {
	if (count == 0) {
		DI_Image* orig = new DI_Image;
		orig->CreateImage(dest.width, dest.height, ByPP);
		CopyRect(*orig, 0, 0, dest, 0, 0, dest.width, dest.height);
		sel->params[0] = int(orig);
		sel->params[1] = 0;
		return 0;
	}
	DI_Image* dest_orig = (DI_Image*)sel->params[0];
	int max_count = 32;
	if (sel->arg4 != 0) max_count = sel->arg4;
	if (count > max_count) count = max_count;

	int dest_width = sel->x2 - sel->x1 + 1; 
	int dest_height = sel->y2 - sel->y1 + 1; 
	/* 拡大する座標を決める */
	int src_x, src_y, src_width, src_height, fade;
	src_x = sel->x1 + (dest_width/2) * (max_count-count) / max_count;
	src_y = sel->y1 + (dest_height/2)* (max_count-count) / max_count;
	src_width = dest_width * count / max_count;
	src_height= dest_height*count / max_count;
	fade = 255 * count / max_count;
	// stretch する
	CopyRect(dest, 0, 0, *dest_orig, 0, 0, dest.width, dest.height);
	CopyRectWithStretch(dest, sel->x3, sel->y3, dest_width, dest_height,
		src, src_x, src_y, src_width, src_height,fade);
	if (count == max_count) {
		delete (DI_Image*)sel->params[0];
		return 2;
	} else return 1;
}

// わっふるのことを考えて、浮動小数点の使用は最小限にする
// 座標回転
inline void calc_sincos(int theta, int max, int& sin_ret, int& cos_ret) {
	sin_ret = int(max * sin(3.141592*2*theta/360));
	cos_ret = int(max * cos(3.141592*2*theta/360));
}
inline void calc_rotate(int* axis, int axis_count, int theta, int axis1, int axis2) {
	int sin_v, cos_v;
	calc_sincos(theta,(1<<14),sin_v,cos_v);
	int i;
	for (i=0; i<axis_count; i++) {
		int a1 = axis[axis1]; int a2 = axis[axis2];
		axis[axis1] = (a1*cos_v + a2*sin_v)>>14;
		axis[axis2] =(-a1*sin_v + a2*cos_v)>>14;
		axis += 3;
	}
}
// 平行移動
inline void move_axis(int* axis, int axis_count, int x_count, int y_count, int z_count) {
	int i;
	for (i=0; i<axis_count; i++) {
		axis[0] += x_count;
		axis[1] += y_count;
		axis[2] += z_count;
		axis += 3;
	}
}
// 3D -> 2D に戻す
inline void project_axis(int* axis, int axis_count, int z_count) {
	int i;
	for (i=0; i<axis_count; i++) {
		axis[0] = (z_count+axis[2])*axis[0]/z_count;
		axis[1] = (z_count+axis[2])*axis[1]/z_count;
		axis += 3;
	}
}

inline void swap_axis(int* axis, int a, int b) {
	int tmp;
	tmp = axis[a]; axis[a] = axis[b]; axis[b] = tmp;
	tmp = axis[a+1]; axis[a+1] = axis[b+1]; axis[b+1] = tmp;
}
RegisterSelMacro(220, TLI2(220,221), NoMask) /* sel220: 縦/横に回転して画面切り替え */
::Exec(DI_Image& dest, DI_Image& src_orig, char* mask, SEL_STRUCT* sel, int count) {
	const int z_height = 1024;
	int width = sel->x2 - sel->x1 + 1; int height = sel->y2 - sel->y1 + 1;
	if (count == 0) {
		DI_Image* orig = new DI_Image;
		orig->CreateImage(dest.width, dest.height, ByPP);
		CopyRect(*orig, 0, 0, dest, 0, 0, dest.width, dest.height);
		sel->params[0] = int(orig);
	}
	int max_count = 31;
	if (sel->arg4 != 0) max_count = sel->arg4;
	if (max_count&1) max_count++;
	if (count >= max_count) {
		CopyRect(dest, sel->x3, sel->y3, src_orig, sel->x1, sel->y1, width, height);
		delete (DI_Image*)sel->params[0];
		return 2;
	}
	int theta = 180*count/max_count;

	DI_Image* srcptr = (DI_Image*)sel->params[0];
	if (count > max_count/2) srcptr = &src_orig;

	// 座標計算
	int axis[12] = {
			sel->x3, sel->y3, 0,
			sel->x3+width-1, sel->y3, 0,
			sel->x3, sel->y3+height-1, 0, 
			sel->x3+width-1, sel->y3+height-1, 0
	};
	int mid_x = sel->x3 + width/2; int mid_y = sel->y3 + height/2;
	// (mid_x, mid_y,-z_height)を中心に適当に回転する
	move_axis(axis, 4, -mid_x, -mid_y, 0);
	if (sel->sel_no == 220) { // 縦：座標はy,z 回転
		calc_rotate(axis, 4, theta, 1, 2);
	} else { // 横：座標はx,z回転
		calc_rotate(axis, 4, -theta, 2, 0);
	}
	project_axis(axis, 4, z_height);
	move_axis(axis, 4, mid_x, mid_y, 0);
	// 正規化：ひっくり返るのは困るので
	if (axis[0] > axis[3]) {
		swap_axis(axis, 0, 3);
		swap_axis(axis, 6, 9);
	}
	if (axis[1] > axis[7]) {
		swap_axis(axis, 0, 6);
		swap_axis(axis, 3, 9);
	}
	// stretchする
	CopyRectWithTransform(dest, axis[0], axis[1], axis[3], axis[4], axis[6], axis[7], axis[9], axis[10],
		*srcptr, sel->x1, sel->y1, width, height,
		true, 0, 0, 0);
	return 1;
}
RegisterSelMacro(222, TLI1(222), NoMask) /* sel222: ぐるぐる回転しながら fade */
::Exec(DI_Image& dest, DI_Image& src_orig, char* mask, SEL_STRUCT* sel, int count) {
	int width = sel->x2 - sel->x1 + 1; int height = sel->y2 - sel->y1 + 1;
	if (count == 0) {
		DI_Image* orig = new DI_Image;
		orig->CreateImage(dest.width, dest.height, ByPP);
		CopyRect(*orig, 0, 0, dest, 0, 0, dest.width, dest.height);
		sel->params[0] = int(orig);
		sel->params[1] = 0;
		return 0;
	}
	int max_count = 31;
	if (sel->arg4 != 0) max_count = sel->arg4;
	if (max_count&1) max_count++;
	if (count >= max_count) {
		CopyRect(dest, sel->x3, sel->y3, src_orig, sel->x1, sel->y1, width, height);
		delete (DI_Image*)sel->params[0];
		return 2;
	}
	int fade_count = 16 * count * count / max_count / max_count + 16;
	//max_count -= max_count/8;
	count += max_count/8; // 逆のほうにゲタを履かせてみる
	int theta=0, dest_width=width, dest_height=height;
	if (count < max_count) {
		theta = 180 - 180*count/max_count;
		dest_width = width * count * count / max_count / max_count;
		dest_height= height * count * count / max_count / max_count;
	}
	// int fade_count = 16 * count * count / max_count / max_count + 16;
	// fade_count *= count-sel->params[1];
	// int fade_count = 64 * count * count / max_count / max_count + 64;
	sel->params[1] = count;

	DI_Image* srcptr = (DI_Image*)sel->params[0];
	if (count > max_count/2) srcptr = &src_orig;

	// 座標計算
	int axis[12] = {
			-dest_width/2, -dest_height/2, 0,
			 dest_width/2, -dest_height/2, 0,
			-dest_width/2,  dest_height/2, 0,
			 dest_width/2,  dest_height/2, 0
	};
	// x,y 回転、平行移動
	calc_rotate(axis, 4, theta, 0, 1);
	move_axis(axis, 4, sel->x3 + width / 2, sel->y3 + height / 2, 0);
	int i; for(i=0; i<4; i++){
		fix_axis(0,axis[i*3+0],dest.width-1);
		fix_axis(0,axis[i*3+1],dest.height-1);
	}
	// stretchする
	CopyRectWithTransform(dest, axis[0], axis[1], axis[3], axis[4], axis[6], axis[7], axis[9], axis[10],
		*srcptr, sel->x1, sel->y1, width, height,
		false, 0, 0, 0, fade_count);
	return 1;
}

RegisterSelMacro(230, TLI9(230,231,232,233,234,235,236,237, 238), WithMask) /* sel23x: フェードしながら上下左右に画像が入れ換え */
::Exec(DI_Image& dest, DI_Image& src, char* mask, SEL_STRUCT* sel, int count) {
	/* 変数初期化 */
	bool end_flag = true;
	int width = sel->x2 - sel->x1 + 1;
	int height = sel->y2 - sel->y1 + 1;
	DifImage* difimage = (DifImage*)sel->params[0];
	BlockFadeData* blockdata = (BlockFadeData*)sel->params[1];
	BlockFadeData* curblock;
	const BlockFadeData::DIR DtoU = BlockFadeData::DtoU;
	const BlockFadeData::DIR UtoD = BlockFadeData::UtoD;
	if (count == 0) {
		// 画像の差分を得る
		difimage = MakeDifImage(dest, src, mask, sel);
		sel->params[0] = int(difimage);

		// テーブルを初期化
		if (sel->sel_no < 234) { /* 上下左右に fade */
			int blkx = width, blky = 4;
			if (sel->sel_no >= 232) blkx = 4, blky = height;
			blockdata = new BlockFadeData(blkx, blky, 0, 0, width, height);
			if (sel->sel_no & 1) blockdata->direction = DtoU;
			else blockdata->direction = UtoD;
		} else if (sel->sel_no < 238) { /* 開く / 閉じる */
			if (sel->sel_no <= 235) { /* 縦に開閉 */
				blockdata = new BlockFadeData(width, 4, 0, 0, width, height/2);
				blockdata->next = new BlockFadeData(width, 4, 0, height/2, width, height-height/2);
			} else { /* 横に開閉 */
				blockdata = new BlockFadeData(4, height, 0, 0, width/2, height);
				blockdata->next = new BlockFadeData(4, height, width/2, 0, width-width/2, height);
			}
			if (sel->sel_no & 1) {
				blockdata->direction = UtoD;
				blockdata->next->direction = DtoU;
			} else {
				blockdata->direction = DtoU;
				blockdata->next->direction = UtoD;
			}
		} else if (sel->sel_no == 238) { /* 十字から開く */
			BlockFadeData* cur;
			cur = new BlockFadeData(width/2, 4, 0, 0, width/2, height/2);
			cur->direction = DtoU;
			blockdata = cur;

			cur = new BlockFadeData(4, height/2, width/2, 0, width-width/2, height/2);
			cur->direction = UtoD;
			blockdata->next = cur;

			cur = new BlockFadeData(4, height-height/2, 0, height-height/2, width/2, height-height/2);
			cur->direction = DtoU;
			blockdata->next->next = cur;

			cur = new BlockFadeData(width-width/2, 4, width-width/2, height-height/2, width-width/2, height-height/2);
			cur->direction = UtoD;
			blockdata->next->next->next = cur;
		}
		sel->params[1] = int(blockdata);
		return 0;
	}

	/* 速度設定 */
	int max_count = 16;
	if (sel->arg4 != 0) max_count = sel->arg4;
	
	/* 変化する領域の設定 */
	for (curblock = blockdata; curblock != 0; curblock = curblock->next) {
		end_flag = curblock->MakeSlideCountTable(count, max_count) && end_flag;
		BlockDifDraw(difimage, curblock);
	}
	if (end_flag) {
		delete difimage;
		delete blockdata;
		return 2;
	}
	return 1;
}
RegisterSelMacro(260, TLI8(260,261,262,263,264,265,266,267), WithMask) /* sel26x: フェードしながら斜め方向に画像が入れ換え */
::Exec(DI_Image& dest, DI_Image& src, char* mask, SEL_STRUCT* sel, int count) {
	/* 変数初期化 */
	bool end_flag = true;
	int width = sel->x2 - sel->x1 + 1;
	int height = sel->y2 - sel->y1 + 1;
	DifImage* difimage = (DifImage*)sel->params[0];
	BlockFadeData* blockdata = (BlockFadeData*)sel->params[1];
	BlockFadeData* curblock;
	if (count == 0) {
		// 画像の差分を得る
		difimage = MakeDifImage(dest, src, mask, sel);
		sel->params[0] = int(difimage);

		// テーブルを初期化
		blockdata = new BlockFadeData(4, 4, 0, 0, width, height);
		blockdata->diag_dir = BlockFadeData::DDIR( (sel->sel_no-260)%4);
		sel->params[1] = int(blockdata);
		return 0;
	}

	/* 速度設定 */
	int max_count = 16;
	if (sel->arg4 != 0) max_count = sel->arg4;
	
	/* 変化する領域の設定、描画 */
	for (curblock = blockdata; curblock != 0; curblock = curblock->next) {
		if (sel->sel_no < 264)
			end_flag = curblock->MakeDiagCountTable(count, max_count) && end_flag;
		else
			end_flag = curblock->MakeDiagCountTable2(count, max_count) && end_flag;
		BlockDifDraw(difimage, curblock);
	}
	if (end_flag) {
		delete difimage;
		delete blockdata;
		return 2;
	}
	return 1;
}
RegisterSelMacro(270, TLI4(270,271,272,273), WithMask) /* sel27x: フェードしながら中心から開く・閉じる(270,271:長方形状 272,273: 菱形状) */
::Exec(DI_Image& dest, DI_Image& src, char* mask, SEL_STRUCT* sel, int count) {
	/* 変数初期化 */
	bool end_flag = true;
	int width = sel->x2 - sel->x1 + 1;
	int height = sel->y2 - sel->y1 + 1;
	DifImage* difimage = (DifImage*)sel->params[0];
	BlockFadeData* blockdata = (BlockFadeData*)sel->params[1];
	BlockFadeData* curblock;
	if (count == 0) {
		// 画像の差分を得る
		difimage = MakeDifImage(dest, src, mask, sel);
		sel->params[0] = int(difimage);

		// テーブルを初期化
		curblock = new BlockFadeData(4, 4, 0, 0, width/2, height/2);
		blockdata = curblock;
		curblock = new BlockFadeData(4, 4, width/2, 0, width-width/2, height/2);
		blockdata->next = curblock;
		curblock = new BlockFadeData(4, 4, 0, height/2, width/2, height-height/2);
		blockdata->next->next = curblock;
		curblock = new BlockFadeData(4, 4, width/2, height/2, width-width/2, height-height/2);
		blockdata->next->next->next = curblock;
		switch(sel->sel_no) {
		case 270: case 272: /* 開く */
			blockdata->diag_dir = BlockFadeData::DRtoUL;
			blockdata->next->diag_dir = BlockFadeData::DLtoUR;
			blockdata->next->next->diag_dir = BlockFadeData::URtoDL;
			blockdata->next->next->next->diag_dir = BlockFadeData::ULtoDR;
			break;
		case 271: /* 閉じる */
			blockdata->diag_dir = BlockFadeData::ULtoDR2;
			blockdata->next->diag_dir = BlockFadeData::URtoDL2;
			blockdata->next->next->diag_dir = BlockFadeData::DLtoUR2;
			blockdata->next->next->next->diag_dir = BlockFadeData::DRtoUL2;
			break;
		case 273: /* 閉じる */
			blockdata->diag_dir = BlockFadeData::ULtoDR;
			blockdata->next->diag_dir = BlockFadeData::URtoDL;
			blockdata->next->next->diag_dir = BlockFadeData::DLtoUR;
			blockdata->next->next->next->diag_dir = BlockFadeData::DRtoUL;
			break;
		}
		sel->params[1] = int(blockdata);
		return 0;
	}

	/* 速度設定 */
	int max_count = 16;
	if (sel->arg4 != 0) max_count = sel->arg4;
	
	/* 変化する領域の設定、描画 */
	for (curblock = blockdata; curblock != 0; curblock = curblock->next) {
		if (sel->sel_no < 272)
			end_flag = curblock->MakeDiagCountTable(count, max_count) && end_flag;
		else
			end_flag = curblock->MakeDiagCountTable2(count, max_count) && end_flag;
		BlockDifDraw(difimage, curblock);
	}
	if (end_flag) {
		delete difimage;
		delete blockdata;
		return 2;
	}
	return 1;
}
RegisterSelMacro(240, TLI4(240,241,242,243), NoMask) /* sel240: スライド＋ストレッチ描画 */
::Exec(DI_Image& dest, DI_Image& src, char* mask, SEL_STRUCT* sel, int count) {
	if (count == 0) {
		DI_Image* orig = new DI_Image;
		orig->CreateImage(dest.width, dest.height, ByPP);
		CopyRect(*orig, 0, 0, dest, 0, 0, dest.width, dest.height);
		sel->params[0] = int(orig);
	}
	DI_Image* dest_orig = (DI_Image*)sel->params[0];
	int width = sel->x2 - sel->x1 + 1; int height = sel->y2 - sel->y1 + 1;
	int max_count = 32;
	if (sel->arg4 != 0) max_count = sel->arg4;
	if (count >= max_count) {
		CopyRect(dest, sel->x3, sel->y3, src, sel->x1, sel->y1, width, height);
		delete (DI_Image*)sel->params[0];
		return 2;
	}
	int sx1,sx2,sy1,sy2;
	int dx1,dx2,dy1,dy2;
	if (sel->sel_no & 1) { // 描画方向を逆にする
		count = max_count - count;
	}
	if (sel->sel_no < 242) { // 上下方向の描画
		dx1 = sx1 = sel->x3;
		dx2 = sx2 = sel->x3 + width-1;
		sy1 = sel->y3;
		dy1 = sel->y3 + (height*count/max_count)-1;
		sy2 = dy1-1;
		if (sy1 >= dy1) {
			sy2 = sy1;
			dy1 = sy1+1;
		}
		dy2 = sel->y3 + height-1;
	} else { // 横方向の描画
		sy1 = dy1 = sel->y3;
		sy2 = dy2 = sel->y3 + height-1;
		sx1 = sel->x3;
		dx1 = sel->x3 + (width*count/max_count)-1;
		sx2 = dx1-1;
		if (sx1 >= dx1) {
			sx2 = sx1;
			dx1 = sx1+1;
		}
		dx2 = sel->x3 + width-1;
	}
	if (sel->sel_no & 1) { // src/dest の方向を逆にする
		int tmp;
		tmp = dx1; dx1 = sx1; sx1 = tmp;
		tmp = dy1; dy1 = sy1; sy1 = tmp;
		tmp = dx2; dx2 = sx2; sx2 = tmp;
		tmp = dy2; dy2 = sy2; sy2 = tmp;
	}
	// 描画する
	CopyRectWithStretch(dest, dx1, dy1, dx2-dx1+1, dy2-dy1+1, *dest_orig, 0, 0, dest_orig->width, dest_orig->height);
	CopyRectWithStretch(dest, sx1, sy1, sx2-sx1+1, sy2-sy1+1, src, sel->x1, sel->y1, width, height);
	return 1;
}
