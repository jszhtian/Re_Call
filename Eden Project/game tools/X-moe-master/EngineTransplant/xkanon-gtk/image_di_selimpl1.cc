// #include "system.h"
// #include <string.h>
// #include<stdio.h>
// #include<stdlib.h>
// #include <math.h>
#include<assert.h>
#include<algorithm>
#include"image_di_seldraw.h"
#include"initial.h"

/**********************************************
**
**	各描画関数の実装
*/

extern int sel_4_xy[18];
extern int sel_5_xy[32];

RegisterSelMacro(4, TLI2(4,5), WithMask) /* sel 4,5 : タイルを使って少しずつ描画 */
::Exec(DI_Image& dest, DI_Image& src, char* mask, SEL_STRUCT* sel, int count) {
	int* pattern; int pattern_max; int pattern_incr;
	if (sel->sel_no == 4) {
		pattern = sel_4_xy;
		pattern_max = 9;
		pattern_incr = 3;
	} else {
		pattern = sel_5_xy;
		pattern_max = 16;
		pattern_incr = 4;
	}
	if (count == 0) {
		// 初期化
		sel->params[0] = 0;
	}
	if (count >= pattern_max) count = pattern_max-1;
	int width = sel->x2 - sel->x1 + 1;
	int height = sel->y2 - sel->y1 + 1;
	int i, j, k;
	count++;
	for (i=sel->params[0]; i<count; i++) {
		int dx = pattern[i*2];
		int dy = pattern[i*2+1];
		char* dest_pt = dest.data; char* src_pt = src.data;
		char* mline;
		mline = mask + sel->x1 + sel->y1 * src.width;
		dest_pt += sel->x3 * ByPP + sel->y3 * dest.bpl;
		src_pt += sel->x1 * ByPP + sel->y1 * src.bpl;
		dest_pt += dy*dest.bpl; src_pt += dy*src.bpl;
		mline += dy*src.width;
		for (j=dy; j<height; j+=pattern_incr) {
			char *d, *s, *m;
			d = dest_pt + dx*ByPP;
			s = src_pt + dx*ByPP;
			m = mline + dx;
			for (k=dx; k<width; k+=pattern_incr) {
				unsigned char mask_char = IsMask ? *m : 0xff;
				if (mask_char) {
					if (mask_char == 0xff) {
						Copy1Pixel(d, s);
					} else {
						SetMiddleColor(d,s, mask_char);
					}
				}
				if (IsMask) m += pattern_incr;
				d += pattern_incr*ByPP;
				s += pattern_incr*ByPP;
			}
			dest_pt += dest.bpl*pattern_incr; src_pt += src.bpl*pattern_incr;
			mline += src.width*pattern_incr;
		}
	}
	sel->params[0] = count-1;
	if (count >= pattern_max) return 2;
	return 1;
}

RegisterSelMacro(10, TLI4(10,11,12,13), WithMask) /*	sel10-13: 画面の上から下へ、すこしずつ表示していく */
::Exec(DI_Image& dest, DI_Image& src, char* mask, SEL_STRUCT* sel, int count) {
	int end_flag = 0;
	int last_count=0, next_count=0;
	if (count == 0) {
		// 初期化
		sel->params[0] = 0;
	} else {
		/* 表示を適当なカウントごとにする */
		if (sel->arg4 == 0) {
			last_count = sel->params[0];
			next_count = count;
		} else {
			last_count = (sel->params[0]-1)*sel->arg4+1;
			next_count = count * sel->arg4;
		}
	}
	int width = sel->x2 - sel->x1 + 1; int height = sel->y2 - sel->y1 + 1;
	if (sel->sel_no < 12){
		if (count >= height) {
			count = height-1;
			end_flag = 1;
		}
	} else {
		if (count >= width) {
			count = width-1;
			end_flag = 1;
		}
	}
	int x=0, y=0;
	switch(sel->sel_no) {
	case 10: /* 上から下へ */
		y = sel->params[0];
		height = count+1 - sel->params[0];
		break;
	case 11: /* 下から上へ */
		y = height - 1 - count;
		height = count+1 - sel->params[0];
		break;
	case 12: /* 左から右へ */
		x = sel->params[0];
		width = count+1 - sel->params[0];
		break;
	case 13: /* 右から左へ */
		x = width - 1 - count;
		width = count+1 - sel->params[0];
		break;
	}
	int sbpl = src.bpl;
	int dbpl = dest.bpl;
	int mbpl = src.width;
	char* src_buf = src.data + (x+sel->x1)*ByPP + (y+sel->y1)*sbpl;
	char* dest_buf = dest.data + (x+sel->x3)*ByPP + (y+sel->y3)*dbpl;
	mask += (x+sel->x1) + (y+sel->y1) * mbpl;
	int i,j;
	for (i=0; i<height; i++) {
		char *s, *d, *m;
		s = src_buf;
		d = dest_buf;
		m = mask;
		for (j=0; j<width; j++) {
			char mask_char = IsMask ? *m : -1;
			if (mask_char) {
				if (mask_char == -1) {
					Copy1Pixel(d, s);
				} else {
					SetMiddleColor(d,s, mask_char);
				}
			}
			d+=ByPP; s+=ByPP; if (IsMask) m++;
		}
		src_buf += sbpl; dest_buf += dbpl; mask += mbpl;
	}
	sel->params[0] = count+1;
	return end_flag+1;
}

RegisterSelMacro(15, TLI2(15,16), NoMask) /*	sel15-16: 縦スクロールする */
::Exec(DI_Image& dest, DI_Image& src, char* mask, SEL_STRUCT* sel, int count) {
	if (count == 0) {
		// 初期化
		sel->params[0] = 0;
		return 1;
	}
	int scroll_height = sel->arg4; int scroll_dir = 1;
	int all_height = sel->y2-sel->y1+1;
	int width = sel->x2-sel->x1+1;
	if (width<=0 || width>dest.width) width=dest.width;
	width *= ByPP;
	if (all_height < 0 || all_height > dest.height) all_height = dest.height;
	int all_height2 = dest.height; // dest 側の画面幅
	if (scroll_height < 0) {
		scroll_dir = -1;
		scroll_height = -scroll_height;
	}
	if (sel->sel_no == 16) scroll_dir = -scroll_dir; /* sel16 は逆方向のスクロール */
	if (scroll_height == 0) scroll_height = 1;
	if (count > all_height/scroll_height)
		count = all_height/scroll_height;
	int dcount = count - sel->params[0];
	int bpl = dest.bpl; int sbpl = src.bpl;
	int h = dcount * scroll_height;
	if (scroll_dir == 1) { // 下から上へ、視点がうつる。オープニングなど
		int i;
		char* dest_data = dest.data + sel->x3*ByPP + (sel->y3+all_height2-1)*bpl;
		char* src_data = dest.data + sel->x3*ByPP + (sel->y3+all_height2-h-1)*bpl;
		for (i=all_height2-h-1; i>=0; i--) {
			memmove(dest_data, src_data, width);
			dest_data -= bpl; src_data -= bpl;
		}
		dest_data = dest.data + sel->x3*ByPP + sel->y3*bpl;
		src_data = src.data + sel->x1*ByPP + (sel->y2+1-count*scroll_height)*sbpl;
		for (i=0; i<h; i++) {
			memmove(dest_data, src_data, width);
			dest_data += bpl; src_data += sbpl;
		}
	} else {	// 上から下へ、視点が移る。スタッフロールなど
		int i;
		char* dest_data = dest.data + sel->x3*ByPP + sel->y3*bpl;
		char* src_data = dest.data + sel->x3*ByPP + (sel->y3+h)*bpl;
		for (i=0; i<all_height2-h; i++) {
			memmove(dest_data, src_data, width);
			dest_data += bpl; src_data += bpl;
		}
		dest_data = dest.data + sel->x3*ByPP + (sel->y3+all_height2-h)*bpl;
		src_data = src.data + sel->x1*ByPP + (sel->y1+(count-dcount)*scroll_height)*bpl;
		for (i=0; i<h; i++) {
			memmove(dest_data, src_data, width);
			dest_data += bpl; src_data += sbpl;
		}
	}
	sel->params[0] = count;
	if (count >= all_height/scroll_height) return 2;
	return 1;
}

RegisterSelMacro(17, TLI2(17,18), NoMask) /*	sel17-18: 横にスクロールする */
::Exec(DI_Image& dest, DI_Image& src, char* mask, SEL_STRUCT* sel, int count) {
	if (count == 0) {
		// 初期化
		sel->params[0] = 0;
		return 1;
	}
	int scroll_width = sel->arg4; int scroll_dir = 1;
	int height = sel->y2-sel->y1+1;
	if (height<=0 || height>dest.height) height = dest.height;
	int all_width = sel->x2-sel->x1+1;
	if (all_width < 0 || all_width > dest.width) all_width = dest.width;
	int all_width2 = dest.width; // dest 側の画面幅
	if (scroll_width < 0) {
		scroll_dir = -1;
		scroll_width = -scroll_width;
	}
	if (sel->sel_no == 18) scroll_dir = -scroll_dir; /* sel18 は逆方向のスクロール */
	if (scroll_width == 0) scroll_width = 1;
	if (count > all_width/scroll_width)
		count = all_width/scroll_width;
	int dcount = count - sel->params[0];
	int bpl = dest.bpl; int sbpl = src.bpl;
	int w = dcount * scroll_width;
	if (scroll_dir == 1) { // 右から左へ、視点がうつる。オープニングなど
		int i;
		char* dest_data = dest.data + sel->x3*ByPP + sel->y3*bpl;
		char* src_data = src.data + (sel->x2+1-count*scroll_width)*ByPP + sel->y1*bpl;
		int len1 = (all_width2-w)*ByPP; int len2 = w*ByPP;
		for (i=0; i<height; i++) {
			memmove(dest_data+len2, dest_data, len1);
			memmove(dest_data, src_data, len2);
			dest_data += bpl; src_data += sbpl;
		}
	} else {	// 左から右へ、視点が移る。スタッフロールなど
		int i;
		char* dest_data = dest.data + sel->x3*ByPP + sel->y3*bpl;
		char* dest_data2 = dest.data + (sel->x3+all_width2-w)*ByPP + sel->y3*bpl;
		char* src_data = src.data + (sel->x1+(count-dcount)*scroll_width)*ByPP + sel->y1*bpl;
		int len1 = (all_width2-w)*ByPP; int len2 = w*ByPP;
		for (i=0; i<height; i++) {
			memmove(dest_data, dest_data+len2, len1);
			memmove(dest_data2, src_data, len2);
			dest_data += bpl; dest_data2 += bpl; src_data += sbpl;
		}
	}
	sel->params[0] = count;
	if (count >= all_width/scroll_width) return 2;
	return 1;
}

// スライド
RegisterSelMacro(20, TLI4(20,21,22,23), NoMask) /*	sel20-23: スライド(スクロールと似ているが、dest 側は変化なし) */
::Exec(DI_Image& dest, DI_Image& src, char* mask, SEL_STRUCT* sel, int count) {
	int end_flag = 0;
	count++; /* 後の処理を楽にするため */
	int slide_length = sel->arg4; int slide_dir = 1;
	if (slide_length == 0) slide_length = 1;
	if (slide_length < 0) {
		slide_dir = -slide_dir;
		slide_length = -slide_length;
	}
	slide_length *= count;
	if (sel->sel_no == 21 || sel->sel_no == 23) slide_dir = -slide_dir;
	/* スライドで表示する矩形領域を決定 */
	int sx,sy,dx,dy,width,height;
	width = sel->x2-sel->x1+1;
	height = sel->y2-sel->y1+1;
	if (sel->sel_no < 22) { /* 縦スライド */
		if (slide_length >= height) {
			slide_length = height;
			end_flag = 1;
		}
		dx = sel->x3; sx = sel->x1;
		if (slide_dir > 0) { /* 上から表示 */
			dy = sel->y3; sy = sel->y2-slide_length;
		} else { /* 下から表示 */
			dy = sel->y3+height-slide_length;
			sy = sel->y1;
		}
		height = slide_length;
	} else { /* 横スライド */
		if (slide_length >= width) {
			slide_length = width;
			end_flag = 1;
		}
		dy = sel->y3; sy = sel->y1;
		if (slide_dir > 0) { /* 上から表示 */
			dx = sel->x3; sx = sel->x2-slide_length;
		} else { /* 下から表示 */
			dx = sel->x3+width-slide_length;
			sx = sel->x1;
		}
		width = slide_length;
	}
	/* コピー */
	int i;
	char* dest_buf = dest.data + dx*ByPP + dy*dest.bpl;
	char* src_buf = src.data + sx*ByPP + sy*src.bpl;
	for (i=0; i<height; i++) {
		memcpy(dest_buf, src_buf, width*ByPP);
		dest_buf += dest.bpl;
		src_buf += src.bpl;
	}
	if (end_flag) return 2;
	else return 1;
}

RegisterSelMacro(25, TLI4(25,26,40,41), WithMask) /* sel25,26,40,41 中心から外側へ描画(25,26) / テレビのスイッチが切れるような効果(40,41) */
::Exec(DI_Image& dest, DI_Image& src, char* mask, SEL_STRUCT* sel, int count) {
	int width = sel->x2-sel->x1+1;
	int height = sel->y2-sel->y1+1;
	if (count == 0) {
		sel->params[0] = 0; // 初期化
		if (sel->sel_no == 25 || sel->sel_no == 40) {
			sel->params[1] = width/2;
			sel->params[2] = height/2;
			sel->params[3] = width/2;
			sel->params[4] = height/2;
		} else {
			sel->params[1] = 0;
			sel->params[2] = 0;
			sel->params[3] = width;
			sel->params[4] = height;
		}
		return 0;
	}
	int old_x1 = sel->params[1];
	int old_y1 = sel->params[2];
	int old_x2 = sel->params[3];
	int old_y2 = sel->params[4];
	int dw = 0;
	if (sel->sel_no == 40 || sel->sel_no == 41) {
		dw = width/2-height/16;
		if (dw < 0) dw = 0;
	}
	int max_count = ( (width-dw*2)>height) ? (width-dw*2+1)/2 : (height+1)/2;
	int mid_x = width/2; int mid_y = height/2;
	/* 描画領域の計算 */
	int rev_flag = (sel->sel_no == 25 || sel->sel_no == 40) ?
		0 : 1; /* 描画方向 */
	int draw_countx = (rev_flag) ? max_count-count : count;
	int draw_county = draw_countx;
	draw_countx += dw;
	int new_x1 = mid_x-draw_countx;
	int new_x2 = mid_x+draw_countx;
	int new_y1 = mid_y-draw_county;
	int new_y2 = mid_y+draw_county;
	/* パラメータの保存 */
	sel->params[1] = new_x1;
	sel->params[2] = new_y1;
	sel->params[3] = new_x2;
	sel->params[4] = new_y2;
	/* 座標の補正 */
	fix_axis(0,old_x1,new_x1,mid_x);
	fix_axis(mid_x,new_x2,old_x2,width);
	fix_axis(0,old_y1,new_y1,mid_x);
	fix_axis(mid_y,new_y2,old_y2,height);
	/* 描画 */
	if (new_y1 > old_y1 && old_x2 > old_x1) {
		if (new_y1 > old_y1) {
			if (IsMask) CopyRectWithMask(&dest,sel->x3+old_x1,sel->y3+old_y1,&src,sel->x1+old_x1,sel->y1+old_y1,old_x2-old_x1, new_y1-old_y1, mask);
			else CopyRect(dest,sel->x3+old_x1,sel->y3+old_y1,src,sel->x1+old_x1,sel->y1+old_y1,old_x2-old_x1, new_y1-old_y1);
		}
		if (new_y2 < old_y2) {
			if (IsMask) CopyRectWithMask(&dest,sel->x3+old_x1,sel->y3+new_y2, &src,sel->x1+old_x1,sel->y1+new_y2,old_x2-old_x1, old_y2-new_y2, mask);
			else CopyRect(dest,sel->x3+old_x1,sel->y3+new_y2, src,sel->x1+old_x1,sel->y1+new_y2,old_x2-old_x1, old_y2-new_y2);
		}
	}
	if (new_y2 > new_y1) {
		if (new_x1 > old_x1) {
			if (IsMask) CopyRectWithMask(&dest,sel->x3+old_x1,sel->y3+new_y1,&src,sel->x1+old_x1,sel->y1+new_y1,new_x1-old_x1, new_y2-new_y1, mask);
			else CopyRect(dest,sel->x3+old_x1,sel->y3+new_y1,src,sel->x1+old_x1,sel->y1+new_y1,new_x1-old_x1, new_y2-new_y1);
		}
		if (new_x2 < old_x2) {
			if (IsMask) CopyRectWithMask(&dest,sel->x3+new_x2,sel->y3+new_y1,&src,sel->x1+new_x2,sel->y1+new_y1,old_x2-new_x2, new_y2-new_y1, mask);
			else CopyRect(dest,sel->x3+new_x2,sel->y3+new_y1,src,sel->x1+new_x2,sel->y1+new_y1,old_x2-new_x2, new_y2-new_y1);
		}
	}
	if (count >= max_count) return 2;
	else return 1;
}

RegisterSelMacro(35, TLI2(35,36), WithMask) /* sel 35, 36 : 偶数行は左(上)から、奇数行は右(下)から描画 */
::Exec(DI_Image& dest, DI_Image& src, char* mask, SEL_STRUCT* sel, int count) {
	int draw_width = sel->arg4;
	int draw_height = sel->arg5;
	if (draw_width <= 0) draw_width = 64; /* たぶん・・・ */
	if (draw_height <= 0) draw_height = 1;
	int width = sel->x2-sel->x1+1;
	int height = sel->y2-sel->y1+1;
	if (count == 0) {
		sel->params[0] = 0; // 初期化
		return 0;
	}
	/* 表示する領域の計算 */
	int x1,w1, x2,w2; int max_w;
	w1 = w2 = draw_width * (count-sel->params[0]);
	if (sel->sel_no == 35) {
		max_w = width;
	} else { /* sel 36 */
		max_w = height;
	}
	x1=sel->params[0]*draw_width;
	x2=max_w-count*draw_width; /* count > 0 */
	if (x2 < 0) {
		w2 += x2;
		x2 = 0;
	}
	if (x1+w1 > max_w) {
		w1 = max_w-x1;
	}
	/* ないとは思うけど・・・ */
	if (w1<0) w1 = 0;
	if (w2<0) w2 = 0;
	/* 表示 */
	int i,j;
	char* dest_buf = dest.data + sel->x3*ByPP + sel->y3*dest.bpl;
	char* src_buf = src.data + sel->x1*ByPP + sel->y1*src.bpl;
	char* mask_buf = mask + sel->x1 + sel->y1*src.width;
	if (sel->sel_no == 35) {
		char* dest_buf1 = dest_buf+x1*ByPP;
		char* dest_buf2 = dest_buf+x2*ByPP;
		char* src_buf1 = src_buf+x1*ByPP;
		char* src_buf2 = src_buf+x2*ByPP;
		char* mask_buf1 = mask_buf+x1;
		char* mask_buf2 = mask_buf+x2;
		int draw_count = 0;
		dest_buf = dest_buf1; src_buf = src_buf1; int w = w1; mask_buf = mask_buf1;
		for (i=0; i<height; i++) {
			/* 左右の描画の切り替え */
			if (draw_count == draw_height) {
				dest_buf = dest_buf2 + draw_height*dest.bpl;
				src_buf = src_buf2 + draw_height*src.bpl;
				mask_buf = mask_buf2 + draw_height*src.width;
				w = w2;
			} else if (draw_count == draw_height*2) {
				draw_count = 0;
				dest_buf1 += draw_height*dest.bpl*2;
				dest_buf2 += draw_height*dest.bpl*2;
				src_buf1 += draw_height*src.bpl*2;
				src_buf2 += draw_height*src.bpl*2;
				mask_buf1 += draw_height*src.width*2;
				mask_buf2 += draw_height*src.width*2;
				dest_buf = dest_buf1;
				src_buf = src_buf1;
				mask_buf = mask_buf1;
				w = w1;
			}
			/* 描画 */
			char* d = dest_buf;
			char* s = src_buf;
			char* m = mask_buf;
			for (j=0; j<w; j++) {
				unsigned char mask_char = IsMask ? *m : 0xff;
				if (mask_char) {
					if (mask_char == 0xff) {
						Copy1Pixel(d,s);
					} else {
						SetMiddleColor(d, s, mask_char);
					}
				}
				d += ByPP; s += ByPP; if (IsMask) m++;
			}
			draw_count++;
			dest_buf += dest.bpl;
			src_buf += src.bpl;
			mask_buf += src.width;
		}
	} else { /* sel 36 */
		char* dest_buf1 = dest_buf + x1*dest.bpl;
		char* src_buf1 = src_buf + x1*src.bpl;
		char* mask_buf1 = mask_buf + x1*src.width;
		for (i=0; i<w1; i++) {
			char* d = dest_buf1;
			char* s = src_buf1;
			char* m = mask_buf1;
			for (j=0; j<width-draw_height; j+=draw_height*2) {
				int k;
				for (k=0; k<draw_height; k++) {
					unsigned char mask_char = IsMask ? *m : 0xff;
					if (mask_char) {
						if (mask_char == 0xff) {
							Copy1Pixel(d,s);
						} else {
							SetMiddleColor(d, s, mask_char);
						}
					}
					d += ByPP; s += ByPP; if (IsMask) m++;
				}
				m += draw_height;
				d += draw_height*ByPP;
				s += draw_height*ByPP;
			}
			if (j < width) {
				int k; for (k=0; k<width-j; k++) {
					unsigned char mask_char = IsMask ? *m : 0xff;
					if (mask_char) {
						if (mask_char == 0xff) {
							Copy1Pixel(d,s);
						} else {
							SetMiddleColor(d, s, mask_char);
						}
					}
					d += ByPP; s += ByPP; if (IsMask) m++;
				}
			}
			dest_buf1 += dest.bpl;
			src_buf1 += src.bpl;
			mask_buf1 += src.width;
		}
		dest_buf1 = dest_buf + x2*dest.bpl;
		src_buf1 = src_buf+x2*src.bpl;
		mask_buf1 = mask_buf+x2*src.width;
		for (i=0; i<w2; i++) {
			char* d = dest_buf1+draw_height*ByPP;
			char* s = src_buf1+draw_height*ByPP;
			char* m = mask_buf1+draw_height;
			for (j=draw_height; j<width-draw_height; j+=draw_height*2) {
				int k;
				for (k=0; k<draw_height; k++) {
					unsigned char mask_char = IsMask ? *m : 0xff;
					if (mask_char) {
						if (mask_char == 0xff) {
							Copy1Pixel(d,s);
						} else {
							SetMiddleColor(d, s, mask_char);
						}
					}
					d += ByPP; s += ByPP; m++;
				}
				if (IsMask) m += draw_height;
				d += draw_height*ByPP;
				s += draw_height*ByPP;
			}
			if (j < width) {
				int k; for (k=0; k<width-j; k++) {
					unsigned char mask_char = IsMask ? *m : 0xff;
					if (mask_char) {
						if (mask_char == 0xff) {
							Copy1Pixel(d,s);
						} else {
							SetMiddleColor(d, s, mask_char);
						}
					}
					d += ByPP; s += ByPP; if (IsMask) m++;
				}
			}
			dest_buf1 += dest.bpl;
			src_buf1 += src.bpl;
			mask_buf1 += src.width;
		}
	}
	sel->params[0]=count;
	if (count*draw_width >= max_w) return 2;
	else return 1;
}
RegisterSelMacro(45, TLI1(45), WithMask) /* sel45 : 十字から各象限ごとに別の方向へ */
::Exec(DI_Image& dest, DI_Image& src, char* mask, SEL_STRUCT* sel, int count) {
	int width = sel->x2-sel->x1+1;
	int height = sel->y2-sel->y1+1;
	int mid_x = (width+1)/2;
	int mid_y = (height+1)/2;
	int max_count = (mid_x > mid_y) ? mid_x : mid_y;
	if (count == 0) {
		sel->params[0] = 0; // 初期化
		return 0;
	}
	if (count >= max_count) count = max_count;
	int p1 = mid_x+sel->params[0];
	int p2 = mid_x+count;
	fix_axis(mid_x,p1,p2,width);
	if (p1 < p2) {
		if (IsMask)
			CopyRectWithMask(&dest, sel->x3+p1, sel->y3, &src, sel->x1+p1, sel->y1, p2-p1, mid_y, mask);
		else
			CopyRect(dest, sel->x3+p1, sel->y3, src, sel->x1+p1, sel->y1, p2-p1, mid_y);
	}
	p1 = mid_y+sel->params[0];
	p2 = mid_y+count;
	fix_axis(mid_y,p1,p2,height);
	if (p1 < p2) {
		if (IsMask)
			CopyRectWithMask(&dest, sel->x3+mid_x, sel->y3+p1, &src, sel->x1+mid_x, sel->y1+p1, width-mid_x, p2-p1, mask);
		else
			CopyRect(dest, sel->x3+mid_x, sel->y3+p1, src, sel->x1+mid_x, sel->y1+p1, width-mid_x, p2-p1);
	}
	p1 = mid_x-count;
	p2 = mid_x-sel->params[0];
	fix_axis(0,p1,p2,mid_x);
	if (p1 < p2) {
		if (IsMask)
			CopyRectWithMask(&dest, sel->x3+p1, sel->y3+mid_y, &src, sel->x1+p1, sel->y1+mid_y, p2-p1, height-mid_y, mask);
		else
			CopyRect(dest, sel->x3+p1, sel->y3+mid_y, src, sel->x1+p1, sel->y1+mid_y, p2-p1, height-mid_y);
	}
	p1 = mid_y-count;
	p2 = mid_y-sel->params[0];
	fix_axis(0,p1,p2,mid_y);
	if (p1 < p2) {
		if (IsMask)
			CopyRectWithMask(&dest, sel->x3, sel->y3+p1, &src, sel->x1, sel->y3+p1, mid_x, p2-p1, mask);
		else
			CopyRect(dest, sel->x3, sel->y3+p1, src, sel->x1, sel->y3+p1, mid_x, p2-p1);
	}
	if (count == max_count) return 2;
	else return 1;
}
extern char* houwa_data4;
RegisterSelMacro(250, TLI1(250), NoMask) /* sel 250(それ散るの64) : 白へ向かって飽和する */
::Exec(DI_Image& dest, DI_Image& src, char* mask, SEL_STRUCT* sel, int count) {
	if (count == 0) {
		sel->params[0] = 0;
		return 0;
	}
	/* 今回の変化分を計算 */
	int max_count = 32;
	if (sel->arg4) max_count = sel->arg4;
	if (count > max_count) count = max_count;
	int fade_data;
	if (BiPP == 16) {
		int dfade1 = count*31/max_count - sel->params[0]*31/max_count;
		int dfade2 = count*63/max_count - sel->params[0]*63/max_count;
		fade_data = (dfade1<<11) | (dfade2<<5) | dfade1;
	} else { /* BiPP == 32 */
		fade_data = count*255/max_count - sel->params[0]*255/max_count;
	}

	char* dest_pt = dest.data + sel->y3*dest.bpl + sel->x3 * ByPP;
	int dbpl = dest.bpl;
	int width = sel->x2 - sel->x1 + 1;
	int height= sel->y2 - sel->y1 + 1;
	int i,j;
	for (i=0; i<height; i++) {
		unsigned char* d = (unsigned char*)dest_pt;
		if (BiPP == 16) {
			unsigned short* dd = (unsigned short*) d;
			for (j=0; j<width; j++) {
				/* 飽和加算で３色同時に計算 */
				/* やっているのは dpix + fade_data を各色で飽和加算しているだけ */
				/* 詳しくは image_di_Xbpp.cc の SetMiddleColor16 を参照 */
				unsigned int dpix = *dd;
				unsigned int m = ( ((fade_data&dpix)<<1) + ( (fade_data^dpix)&0xf7de ) ) & 0x10820;
				m = ( (((m*3)&0x20840)>>6) + 0x7bef) ^ 0x7bef;
				*dd = (fade_data + dpix - m) ^ m;
				dd++;
			}
		} else { /* BiPP == 32 */
			for (j=0; j<width; j++) {
				d[0] = houwa_data4[fade_data + d[0]];
				d[1] = houwa_data4[fade_data + d[1]];
				d[2] = houwa_data4[fade_data + d[2]];
				d += ByPP;
			}
		}
		dest_pt += dbpl;
	}
	sel->params[0] = count;
	if (count >= max_count) return 2;
	return 1;
}

extern char pdt_image_stipple60_x[4][16*2];

RegisterSelMacro(60, TLI4(60,61,62,63), WithMask) /* sel 60 - 63 : 4pixel x 4pixel のブロックが 4x4 の領域を埋めるようにタイル描画 */
::Exec(DI_Image& dest, DI_Image& src, char* mask, SEL_STRUCT* sel, int count) {
	if (count == 0) {
		// 初期化
		sel->params[0] = 0;
		if (sel->sel_no < 60 || sel->sel_no > 63) sel->sel_no = 60;
	}
	if (count > 15) count = 15;
	int width = sel->x2 - sel->x1 + 1; int height = sel->y2 - sel->y1 + 1;
	int i; int xi,yi,yj,xj;
	count++;
	for (i=sel->params[0]; i<count; i++) {
		int dx = pdt_image_stipple60_x[sel->sel_no-60][i*2]*4;
		int dy = pdt_image_stipple60_x[sel->sel_no-60][i*2+1]*4;
		char* dest_pt = dest.data; char* src_pt = src.data;
		char* mask_pt;
		dest_pt += sel->x3*ByPP + sel->y3 * dest.bpl;
		src_pt += sel->x1*ByPP + sel->y1 * src.bpl;
		dest_pt += dy*dest.bpl; src_pt += dy*src.bpl;
		mask_pt = mask;
		mask_pt += sel->x1 + sel->y1 * src.width;
		mask_pt += dy*src.width;
		for (yi=dy; yi<height; yi+=16) {
			int ylen;
			if (yi+4 < height) ylen = 4;
			else ylen = height-yi;
			for (yj=0; yj<ylen; yj++) {
				char* d = dest_pt + dest.bpl*yj + dx*ByPP;
				char* s = src_pt + src.bpl*yj + dx*ByPP;
				char* m = mask_pt + src.width*yj + dx;
				for (xi=dx; xi<width; xi+=16) {
					int xlen;
					if (xi+4 < width) xlen = 4;
					else xlen = width-xi;
					for (xj=0; xj<xlen; xj++) {
						char mask_char = IsMask ? *m : -1;
						if (mask_char) {
							if (mask_char == -1) Copy1Pixel(d,s);
							else SetMiddleColor( d, s, mask_char);
						}
						d += ByPP; s += ByPP; m++;
					}
					d += (16-xlen)*ByPP; s += (16-xlen)*ByPP; m += 16-xlen;
				}
			}
			dest_pt += dest.bpl*16; src_pt += src.bpl*16;
			mask_pt += src.width*16;
		}
	}
	sel->params[0] = count-1;
	if (count >= 16) return 2;
	return 1;
}


extern int make_stipple4(int count1, int count2, int width, int* ret_buf);
extern int make_stipple100(int count1, int count2, int max_count, int width, int* ret_buf);
extern int make_stipple30(int count1, int count2, int width, int sunoko_width, int* ret_buf);
RegisterSelMacro(120, TLI10(30,31,100,101,102,103,120,121,122,123), NoMask) /* sel 30: 偶数列は左から、奇数列は右から描画 sel 31: 偶数行は上から、奇数行は下から描画 sel 10x : 左右上下から短冊状に描画 sel 120 : 上下左右からすのこが広がるように描画 */
::Exec(DI_Image& dest, DI_Image& src, char* mask, SEL_STRUCT* sel, int count) {
	if (count == 0) {
		// 初期化
		sel->params[0] = 0;
	}
	int width = sel->x2 - sel->x1 + 1; int height = sel->y2 - sel->y1 + 1;
	int draw_points[dest.width];
	int sel_no = sel->sel_no;
	if (sel_no == 30) {
		if (make_stipple30(sel->params[0], count, width, 1, draw_points) == 0) return 2; // 終了
		sel_no = 120;
	} else if (sel_no == 31) {
		if (make_stipple30(sel->params[0], count, height, 1, draw_points) == 0) return 2; // 終了
		sel_no = 122;
	} else if (sel_no <= 101) {
		if (sel->arg4 <= 0) sel->arg4 = 16;
		if (make_stipple100(sel->params[0],count,sel->arg4,width,draw_points) == 0) return 2;
		sel_no += 20;
	} else if (sel_no <= 103) {
		if (sel->arg4 <= 0) sel->arg4 = 16;
		if (make_stipple100(sel->params[0],count,sel->arg4, height,draw_points) == 0) return 2;
		sel_no += 20;
	} else if (sel_no <= 121) {
		if (make_stipple4(sel->params[0], count, width, draw_points) == 0) return 2; // 終了
	} else {
		if (make_stipple4(sel->params[0], count, height, draw_points) == 0) return 2; // 終了
	}
	if (draw_points[0] == -1) return 2; // 終了
	// 描くデータ
	int sbpl = src.bpl; int dbpl = dest.bpl; int mbpl = src.width;
	char *src_buf=0, *dest_buf=0;
	if (sel_no == 120) {
		src_buf = src.data + sel->x1*ByPP + sel->y1*sbpl;
		dest_buf = dest.data + sel->x3*ByPP + sel->y3*dbpl;
		mask += sel->x1 + sel->y1 * mbpl;
	} else if (sel_no == 121) {
		src_buf = src.data + sel->x2*ByPP + sel->y1*sbpl;
		dest_buf = dest.data + (sel->x3+sel->x2-sel->x1)*ByPP + sel->y3*dbpl;
		mask += sel->x2 + sel->y1 * mbpl;
	} else if (sel_no == 122) {
		src_buf = src.data + sel->x1*ByPP + sel->y1*sbpl;
		dest_buf = dest.data + sel->x3*ByPP + sel->y3*dbpl;
		mask += sel->x1 + sel->y1 * mbpl;
	} else if (sel_no == 123) {
		src_buf = src.data + sel->x1*ByPP + sel->y2*sbpl;
		dest_buf = dest.data + sel->x3*ByPP + (sel->y3+sel->y2-sel->y1)*dbpl;
		mask += sel->x1 + sel->y2 * mbpl;
	}
	if (sel_no == 120 || sel_no == 121) {
		int i,j; for (i=0; draw_points[i] != -1; i++) {
			char *s, *d, *m;
			if (sel_no == 120) {
				s = src_buf + draw_points[i] * ByPP;
				d = dest_buf + draw_points[i] * ByPP;
				m = mask + draw_points[i];
			} else {
				s = src_buf - draw_points[i] * ByPP;
				d = dest_buf - draw_points[i] * ByPP;
				m = mask - draw_points[i];
			}
			for (j=0; j<height; j++) {
				char mask_char = IsMask ? *m : -1;
				if (mask_char) {
					if (mask_char == -1) {
						Copy1Pixel(d, s);
					} else {
						SetMiddleColor(d,s, mask_char);
					}
				}
				d += dbpl; s += sbpl; if (IsMask) m += mbpl;
			}
		}
	} else {
		int i,j; for (i=0; draw_points[i] != -1; i++) {
			char *s, *d, *m;
			if (sel_no == 122) {
				s = src_buf + draw_points[i] * sbpl;
				d = dest_buf + draw_points[i] * dbpl;
				m = mask + draw_points[i]*src.width;
			} else {
				s = src_buf - draw_points[i] * sbpl;
				d = dest_buf - draw_points[i] * dbpl;
				m = mask - draw_points[i]*src.width;
			}
			for (j=0; j<width; j++) {
				char mask_char = IsMask ? *m : -1;
				if (mask_char) {
					if (mask_char == -1) {
						Copy1Pixel(d, s);
					} else {
						SetMiddleColor(d, s, mask_char);
					}
				}
				d += ByPP; s += ByPP; m++;
			}
		}
	}
	sel->params[0] = count+1;
	return 1;
}

