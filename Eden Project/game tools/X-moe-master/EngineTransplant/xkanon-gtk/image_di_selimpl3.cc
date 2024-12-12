#include<math.h>
#include<assert.h>
#include<algorithm>
#include"image_di_seldraw.h"
#include"initial.h"
#include"system.h"

RegisterSelMacro(70, TLI4(70,71,72,73), WithMask) /* sel 70-73: ランダムに線を広げていくように描画 */
::Exec(DI_Image& dest, DI_Image& src, char* mask, SEL_STRUCT* sel, int count) {
	int width = sel->x2 - sel->x1 + 1; int height = sel->y2 - sel->y1 + 1;
	if (count == 0) {
		// 初期化
		if (sel->sel_no < 70 || sel->sel_no > 73) sel->sel_no = 70;
		int line_deal; int line_width;
		if (sel->sel_no <= 71) {line_deal = height; line_width = width; }
		else { line_deal = width; line_width = height; }

		if (sel->arg4 == 0) sel->arg4 = 0x20;
		if (sel->arg5 == 0) sel->arg5 = 0x40;
		int* line_counts = new int[line_deal];
		int* line_firsts = new int[line_deal];
		int i; for (i=0; i<line_deal; i++) {
			line_counts[i] = -sel->arg4 * (AyuSys::Rand(sel->arg5));
		}
		sel->params[0] = int(line_counts);
		sel->params[1] = line_deal;
		sel->params[2] = line_width;
		sel->params[3] = int(line_firsts);
		sel->params[4] = -1;
	}
	int* line_counts = (int*)sel->params[0];
	int line_deal = sel->params[1];
	int line_width = sel->params[2];
	int* line_firsts = (int*)sel->params[3];
	int draw_width = sel->arg4 * (count-sel->params[4]);
	sel->params[4] = count;

	/* 今回描画する分を決める */
	if (sel->sel_no == 70 || sel->sel_no == 72) {
		/* 順方向描画 */
		int i; for (i=0; i<line_deal; i++) {
			if (line_counts[i] >= line_width) {
				line_firsts[i] = -8000;
				continue;
			}
			int tmp = line_counts[i];
			if (tmp+draw_width >= line_width) tmp = line_width - draw_width;
			if (tmp+draw_width < 0) {
				line_firsts[i] = -8000;
				continue;
			}
			line_firsts[i] = tmp;
		}
	} else {
		/* 逆方向描画 */
		int i; for (i=0; i<line_deal; i++) {
			if (line_counts[i] >= line_width) {
				line_firsts[i] = -8000;
				continue;
			}
			int tmp = line_width - draw_width - line_counts[i];
			if (tmp >= line_width) {
				line_firsts[i] = -8000;
				continue;
			}
			if (tmp < 0) tmp = 0;
			line_firsts[i] = tmp;
		}
	}
	/* 描画する */
	char* dest_pt = dest.data; char* src_pt = src.data;
	dest_pt += sel->x3 * ByPP + sel->y3 * dest.bpl;
	src_pt += sel->x1 * ByPP + sel->y1 * src.bpl;
	char* mask_pt = mask + sel->x1 + sel->y1 * src.width;
	if (sel->sel_no <= 71) { // 縦方向
		int i; for (i=0; i<line_deal; i++) {
			int lf = line_firsts[i]; int dw = draw_width;
			if (lf == -8000) { dest_pt += dest.bpl; src_pt += src.bpl; mask_pt += src.width; continue;}
			if (lf < 0) { dw += lf; lf = 0;}
			if (lf+dw > line_width) { dw = line_width-lf;}
			char* d = dest_pt + lf * ByPP;
			char* s = src_pt + lf * ByPP;
			char* m = mask_pt + lf;
			int j; for (j=0; j<dw; j++) {
				char mask_char = IsMask ? *m : -1;
				if (mask_char) {
					if (mask_char == -1) Copy1Pixel(d ,s);
					else SetMiddleColor(d, s, mask_char);
				}
				d+= ByPP; s+=ByPP; if (IsMask) m++;
			}
			dest_pt += dest.bpl; src_pt += src.bpl; mask_pt += src.width;
		}
	} else { // 横方向
		int i; for (i=0; i<line_deal; i++) {
			int lf = line_firsts[i]; int dw = draw_width;
			if (lf == -8000) { dest_pt += ByPP; src_pt += ByPP; mask_pt++; continue;}
			if (lf < 0) { dw += lf; lf = 0;}
			if (lf+dw > line_width) { dw = line_width-lf;}
			char* d = dest_pt + lf*dest.bpl;
			char* s = src_pt + lf*src.bpl;
			char* m = mask_pt + lf*src.width;
			int j; for (j=0; j<dw; j++) {
				char mask_char = IsMask ? *m : -1;
				if (mask_char) {
					if (mask_char == -1) Copy1Pixel(d,s);
					else SetMiddleColor(d, s, mask_char);
				}
				d += dest.bpl; s += src.bpl; if (IsMask) m += src.width;
			}
			dest_pt+=ByPP; src_pt+=ByPP; mask_pt++;
		}
	}
	/* 終わり。カウントを変更。描画が終わっていれば終了 */
	int i, end_flag = 1;
	for (i=0; i<line_deal; i++) {
		line_counts[i] += draw_width;
		if (line_counts[i] < line_width) end_flag = 0;
	}
	if (end_flag) {
		delete[] line_firsts; delete[] line_counts;
	}
	return end_flag + 1;
}
RegisterSelMacro(80, TLI4(80,81,82,83), NoMask) /* sel 80 - 83 : ランダムに線を広げていくように描画(スクロール付き) */
::Exec(DI_Image& dest, DI_Image& src, char* mask, SEL_STRUCT* sel, int count) {
	int width = sel->x2 - sel->x1 + 1; int height = sel->y2 - sel->y1 + 1;
	if (count == 0) {
		// 初期化
		if (sel->sel_no < 80 || sel->sel_no > 83) sel->sel_no = 80;
		int line_deal; int line_width;
		if (sel->sel_no <= 81) {line_deal = height; line_width = width; }
		else { line_deal = width; line_width = height; }

		if (sel->arg4 == 0) sel->arg4 = 0x20;
		if (sel->arg5 == 0) sel->arg5 = 0x40;
		int* line_counts = new int[line_deal];
		int* line_firsts = new int[line_deal];
		int i; for (i=0; i<line_deal; i++) {
			line_counts[i] = -sel->arg4 * (AyuSys::Rand(sel->arg5));
		}
		sel->params[0] = int(line_counts);
		sel->params[1] = line_deal;
		sel->params[2] = line_width;
		sel->params[3] = int(line_firsts);
		sel->params[4] = -1;
	}
	int* line_counts = (int*)sel->params[0];
	int line_deal = sel->params[1];
	int line_width = sel->params[2];
	int* line_firsts = (int*)sel->params[3];
	int draw_width = sel->arg4 * (count-sel->params[4]);
	if (draw_width > line_width) {
		count = sel->params[4] + line_width/2/sel->arg4;
		draw_width = sel->arg4 * (count-sel->params[4]);
	}
	sel->params[4] = count;

	/* 今回描画する分を決める */
	if (sel->sel_no == 80 || sel->sel_no == 82) {
		/* 順方向描画 */
		int i; for (i=0; i<line_deal; i++) {
			if (line_counts[i] >= line_width) {
				line_firsts[i] = -8000;
				continue;
			}
			int tmp = line_counts[i];
			if (tmp+draw_width < 0) {
				line_firsts[i] = -8000;
				continue;
			}
			line_firsts[i] = line_width-tmp-draw_width;
		}
	} else {
		/* 逆方向描画 */
		int i; for (i=0; i<line_deal; i++) {
			if (line_counts[i] >= line_width) {
				line_firsts[i] = -8000;
				continue;
			}
			int tmp = line_width - draw_width - line_counts[i];
			if (tmp >= line_width) {
				line_firsts[i] = -8000;
				continue;
			}
			line_firsts[i] = line_width-(tmp+draw_width);
		}
	}
	/* 全体をスクロールする */
	if (sel->sel_no <= 81) {
		char* dest_buf = dest.data + sel->x3*ByPP + sel->y3*dest.bpl;
		if (sel->sel_no == 80) {
			int i; for (i=0; i<height; i++) {
				if (line_firsts[i] == -8000) {
				} else if (line_firsts[i] < 0) {
					int dw = draw_width+line_firsts[i];
					memmove(dest_buf+dw*ByPP, dest_buf, (line_width-dw)*ByPP);
				} else if (line_firsts[i]+draw_width > line_width) {
				} else {
					int dw = width - draw_width - line_firsts[i];
					memmove(dest_buf+draw_width*ByPP, dest_buf, dw*ByPP);
				}
				dest_buf += dest.bpl;
			}
		} else { /* sel 81 */
			int i; for (i=0; i<height; i++) {
				if (line_firsts[i] < 0) {
				} else if (line_firsts[i]+draw_width > line_width) {
					int dw = line_width-line_firsts[i];
					memmove(dest_buf, dest_buf+dw*ByPP, (line_width-dw)*ByPP);
				} else {
					int dw = width - draw_width - line_firsts[i];
					memmove(dest_buf+dw*ByPP, dest_buf+(draw_width+dw)*ByPP, line_firsts[i]*ByPP);
				}
				dest_buf += dest.bpl;
			}
		}
	} else  { // 上下から
		int i,j;
		int* copy_firsts = new int[width];
		int* copy_lasts = new int[width];
		int* copy_width = new int[width];
		int* copy_incr = new int[width];
		int incr_count = 0; int cur_incr = 0;
		/* コピー情報をしらべる */
		/* copy_firsts - copy_last の領域を copy_width だけコピー */
		if (sel->sel_no == 82) {
			int i; for (i=0; i<width; i++) {
				cur_incr++;
				if (line_firsts[i] == -8000) {
					continue;
				} else if (line_firsts[i] < 0) {
					int dw = draw_width + line_firsts[i];
					copy_firsts[incr_count] = 0;
					copy_lasts[incr_count] = line_width-copy_firsts[incr_count];
					copy_width[incr_count] = dw*dest.bpl;
				} else if (line_firsts[i]+draw_width > line_width) {
					continue;
				} else {
					int dw = line_width - draw_width - line_firsts[i];
					copy_firsts[incr_count] = 0;
					copy_lasts[incr_count] = dw;
					copy_width[incr_count] = draw_width*dest.bpl;
				}
				copy_incr[incr_count++] = cur_incr*ByPP;
				cur_incr = 0;
			}
		} else { /* sel 83 */
			int i; for (i=0; i<width; i++) {
				cur_incr++;
				if (line_firsts[i] < 0) {
					continue;
				} else if (line_firsts[i]+draw_width > line_width) {
					int dw = line_width-line_firsts[i];
					copy_firsts[incr_count] = dw;
					copy_lasts[incr_count] = line_width;
					copy_width[incr_count] = dw*dest.bpl;
				} else {
					int dw = line_width - draw_width - line_firsts[i];
					copy_firsts[incr_count] = dw+draw_width;
					copy_lasts[incr_count] = dw+line_firsts[i]+draw_width;
					copy_width[incr_count] = draw_width*dest.bpl;
				}
				copy_incr[incr_count++] = cur_incr*ByPP;
				cur_incr = 0;
			}
		}
		/* コピーは一応、スキャンライン順に行う */
		char* dest_buf = dest.data + sel->x3*ByPP + sel->y3*dest.bpl;
		for (i=0; i<height; i++) {
			if (sel->sel_no == 82) { /* 上から下へ */
				int y = height-1-i;
				char* d = dest_buf + y*dest.bpl - ByPP;
				for (j=0; j<incr_count; j++) {
					d += copy_incr[j];
					if (y < copy_firsts[j] || y >= copy_lasts[j]) continue;
					Copy1Pixel(d+copy_width[j], d);
				}
			} else /* sel_no == 83 */ { /* 下から上へ */
				int y = i;
				char* d = dest_buf + y*dest.bpl - ByPP;
				for (j=0; j<incr_count; j++) {
					d += copy_incr[j];
					if (y < copy_firsts[j] || y >= copy_lasts[j]) continue;
					Copy1Pixel(d-copy_width[j], d);
				}
			}
		}
		delete[] copy_firsts; delete[] copy_lasts; delete[] copy_width; delete[] copy_incr;
	}
	/* 描画する */
	char* dest_pt = dest.data; char* src_pt = src.data;
	dest_pt += sel->x3*ByPP + sel->y3 * dest.bpl;
	src_pt += sel->x1*ByPP + sel->y1 * src.bpl;
	if (sel->sel_no <= 81) { // 縦方向
		if (sel->sel_no == 81) dest_pt += (line_width-draw_width)*ByPP;
		int i; for (i=0; i<line_deal; i++) {
			int lf = line_firsts[i]; int dw = draw_width;
			char* d = dest_pt;
			char* s = src_pt + lf*ByPP;
			dest_pt += dest.bpl; src_pt += src.bpl;
			if (lf == -8000) continue;
			if (lf < 0) {
				dw += lf; lf = 0; s = src_pt-src.bpl;
				if (sel->sel_no == 81) d += (draw_width-dw)*ByPP;
			}
			if (lf+dw > line_width) {
				dw = line_width-lf;
				if (sel->sel_no == 81) d += (draw_width-dw)*ByPP;
			}
			memcpy(d, s, dw*ByPP);
		}
	} else { // 横方向
		if (sel->sel_no == 83) dest_pt += (line_width-draw_width)*dest.bpl;
		int i; for (i=0; i<line_deal; i++) {
			int lf = line_firsts[i]; int dw = draw_width;
			char* d = dest_pt;
			char* s = src_pt + lf*src.bpl;
			dest_pt+=ByPP; src_pt+=ByPP;
			if (lf == -8000) continue;
			if (lf < 0) {
				dw += lf; lf = 0; s = src_pt-ByPP;
				if (sel->sel_no == 83) d += (draw_width-dw)*dest.bpl;
			}
			if (lf+dw > line_width) {
				dw = line_width-lf;
				if (sel->sel_no == 83) d += (draw_width-dw)*dest.bpl;
			}
			int j; for (j=0; j<dw; j++) {
				Copy1Pixel(d,s);
				d += dest.bpl; s += src.bpl;
			}
		}
	}
	/* 終わり。カウントを変更。描画が終わっていれば終了 */
	int i, end_flag = 1;
	for (i=0; i<line_deal; i++) {
		line_counts[i] += draw_width;
		if (line_counts[i] < line_width) end_flag = 0;
	}
	if (end_flag) {
		delete[] line_firsts; delete[] line_counts;
	}
	return end_flag + 1;
}

/* わっふるからコードを持ってきた */
/* (c) Kenjoさん */
RegisterSelMacro(170, TLI1(170), WithMask) /* sel170 : 酔っ払う効果 */
::Exec(DI_Image& dest, DI_Image& src, char* mask, SEL_STRUCT* sel, int count) {
	int width, height;
	width = sel->x2-sel->x1+1;
	height= sel->y2-sel->y1+1;
	/* count = 0 - 168 */
	/* count =   0- 63: 振幅増加 */
	/* count =  64-103: 画像入替 */
	/* count = 104-167: 振幅減少 */

	if (count == 0) {
		/* 前準備 */
		sel->params[2] = 0;
		sel->params[3] = 0;
		/* 古い画像を保存 */
		DI_Image* dest_orig = new DI_Image;
		dest_orig->CreateImage(width, height, ByPP);
		CopyRect(*dest_orig, 0, 0, dest, sel->x3, sel->y3, width, height);
		sel->params[0] = int(dest_orig->data);
		sel->params[1] = int(dest_orig);
	}
	int w_width = 0; int copy_line = 0;
	DI_Image& dest_orig = *(DI_Image*)sel->params[1];
	if (count < 64) {
		w_width = count * 10;
	} else if (count < 104) {
		w_width = dest.width;
		copy_line = (count-63)*4;
	} else if (count < 168) {
		w_width = (167-count) * 10;
		copy_line = 160;
	}
	/* 必要な部分をコピー */
	/* 160line ごと、4line ずつで40回 */
	if (sel->params[2] < copy_line) {
		int i; for (i=0; i<height; i+=160) {
			if (IsMask)
				CopyRectWithMask(&dest_orig, 0, sel->params[2]+i, &src, sel->x1, sel->y1+sel->params[2]+i, width, copy_line-sel->params[2], mask);
			else
				CopyRect(dest_orig, 0, sel->params[2]+i, src, sel->x1, sel->y1+sel->params[2]+i, width, copy_line-sel->params[2]);
		}
printf("copy line %3d->%3d\n",sel->params[2],copy_line);
		sel->params[2] = copy_line;
	}
	double theta = sel->params[3]*(4*3.141592*2/360); /* sin うねりの最初の角度は radian 単位で sel->params[3]*4 度 */
	sel->params[3]++;
	int y;
	char* dest_pt = dest.data + sel->x3*ByPP + sel->y3*dest.bpl;
	char* src_pt = (char*)(sel->params[0]);
	for (y=0; y<height; y++) {
		int x_first = int(sin(theta)*double(w_width));
		int copy_srcx=0, copy_destx=0, copy_width=0;
		int clear_x=0, clear_width=0;
		/* 角度ごとにコピー・クリアする領域を計算 */
		if (x_first < -width) {
			clear_x = 0; clear_width = width;
		} else if (x_first < 0) {
			clear_x = x_first+width; clear_width = -x_first;
			copy_srcx = -x_first; copy_destx = 0;
			copy_width = x_first+width;
		} else if (x_first < width) {
			copy_srcx = 0; copy_destx = x_first;
			copy_width = width-x_first;
			clear_x = 0; clear_width = x_first;
		} else {
			clear_x = 0; clear_width = width;
		}
		/* まず、いらない部分を消去 */
		char* d = dest_pt + clear_x*ByPP;
		memset(d, 0, clear_width*ByPP);
		/* コピー */
		d = dest_pt + copy_destx*ByPP;
		char* s = src_pt + copy_srcx*ByPP;
		memcpy(d, s, copy_width*ByPP);
		/* 次の処理へ */
		theta += 3.141592*2/360; /* 1度 */
		dest_pt += dest.bpl;
		src_pt += dest_orig.bpl;
	}
	if (count >= 167) {
		/* 終了処理 */
		delete (DI_Image*)sel->params[1];
		return 2;
	}
	else return 1;
}
