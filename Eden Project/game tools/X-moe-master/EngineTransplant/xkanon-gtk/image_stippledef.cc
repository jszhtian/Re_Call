/* image_stippledef.cc
 *	image  ����̤�Ÿ������Ȥ���Ÿ���ѥ���������
 *	���¤˻ȤäƤ���Τ� stipple 3 �� 4 �Τߡ�
 *	stipple4 �Ϥ������礭�������ºݤˤϴ�ñ�ʷ׻���Ƴ�����Ȥ�Ǥ���
**/
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


#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif

#include<stdio.h>
#include<string.h>
/* sel 4 �ǡ������˲��̤����褹��Ȥ��Σ����κ�ɸ */
/* ����Ū�ˤ� pdt_image_stipple3 ��Ʊ�� */
int sel_4_xy[18] = {
	0,0, 2,2, 0,2, 2,0, 1,1, 0,1, 2,1, 1,0, 1,2
};
/* sel 5 �ǡ������˲��̤����褹��Ȥ��Σ������κ�ɸ */
int sel_5_xy[32] = {
	0,0, 2,2, 0,2, 2,0, 1,1, 3,3, 1,3, 3,1,
	0,1, 2,3, 0,3, 2,1, 1,0, 3,2, 1,2, 3,0
};

extern char pdt_image_stipple0[1];
extern char pdt_image_stipple1[4*4*4];
extern char pdt_image_stipple2[8*1*8];
extern char pdt_image_stipple3[3*3*9];

/* pdt_image_stipple4 �򸵤ˤ��ơ�count1 ����count2 �ޤǤ�
** stipple ��Ťͤơ����褹�٤��饤���ret_buf ���֤�
** count1 �� stipple ���礭���ʣ����ˤ�Ķ�����顢0 ���֤�
**/
/* �������Ȥ����Τ�׻��Ǥ�äƤ��� */
static int* make_stipple4_1line(int* ret_buf, int count, int max) {
	int first,end;
	end = count*16;
	if (end < 15*15) first = (end - (end/15)*15);
	else first = end-15*15;
	if (first >= max) return ret_buf;
	if (end >= max) end = max-1;
	int i;for (i=first; i<=end; i+=15)
		*ret_buf++ = i;
	return ret_buf;
}
int make_stipple4(int count1, int count2, int width, int* ret_buf) {
	// ������ȥ����å�
	ret_buf[0] = -1;
	if (count1 >= 55) return 0;
	if (count1 > count2) return 1;
	if (count2 >= 55) count2 = 54;
	int i; for (i=count1; i<=count2; i++)
		ret_buf = make_stipple4_1line(ret_buf, i, width);
	*ret_buf++ = -1;
	return 1;
}
/* sel100-103 �ǻȤ����Τ���Ĥ��� */
int make_stipple100(int count1, int count2, int max_count, int width, int* ret_buf) {
	if (count1 >= max_count) return 0;
	if (count1 > count2) return 1;
	if (count2 >= max_count) count2 = max_count-1;
	int i,j; for (i=0; i<width; i+=max_count) {
		for (j=count1; j<=count2; j++) {
			if (i+j < width) *ret_buf++ = i+j;
		}
	}
	*ret_buf++ = -1;
	return 1;
}
/* sel30/31 �ǻȤ����Τ��ΰ���Ĥ��� */
int make_stipple30(int count1, int count2, int width, int sunoko_width, int* ret_buf) {
	ret_buf[0] = -1;
	int max_count = width/sunoko_width/2;
	if (count1 > count2) return 1;
	/* ü�ν����� */
	if (width == max_count*sunoko_width*2) {
		if (count1 > max_count) return 0;
	} else if (width < (max_count*2+1)*sunoko_width) {
		if (count1 > max_count+1) return 0;
		count1--; count2--;
	} else {
		if (count1 > max_count+1) return 0;
	}
	int i,j;
	for (i=count1; i<=count2; i++) {
		int x0 = sunoko_width*2*i;
		for (j=0; j<sunoko_width; j++) {
			if ( (x0+j)>=0&&(x0+j)<width) *ret_buf++ = x0+j;
		}
		x0 = sunoko_width*(2*(max_count-i)-1);
		for (j=0; j<sunoko_width; j++) {
			if ( (x0+j)>=0&&(x0+j)<width) *ret_buf++ = x0+j;
		}
	}
	*ret_buf = -1;
	return 1;
}

/* �������Ĳ�����ɽ�����뤿��� stipple */
char pdt_image_stipple0[1] = {1};

char pdt_image_stipple1[4*4*4] = {
		1,1,1,1,
		1,0,0,0,
		1,0,0,0,
		1,0,0,0,

		0,0,0,0,
		0,1,1,1,
		0,1,0,0,
		0,1,0,0,

		0,0,0,0,
		0,0,0,0,
		0,0,1,1,
		0,0,1,0,

		0,0,0,0,
		0,0,0,0,
		0,0,0,0,
		0,0,0,1 
	};
char pdt_image_stipple2[8*1*8] = {
		1,0,0,0,0,0,0,0,
		0,1,0,0,0,0,0,0,
		0,0,1,0,0,0,0,0,
		0,0,0,1,0,0,0,0,
		0,0,0,0,1,0,0,0,
		0,0,0,0,0,1,0,0,
		0,0,0,0,0,0,1,0,
		0,0,0,0,0,0,0,1
	};

// �̾�Ȥ��Ƥ��� stipple
char pdt_image_stipple3[3*3*9] = {
		1,0,0,
		0,0,0,
		0,0,0,

		0,0,0,
		0,0,0,
		0,0,1,

		0,0,0,
		0,0,0,
		1,0,0,

		0,0,1,
		0,0,0,
		0,0,0,

		0,0,0,
		0,1,0,
		0,0,0,

		0,0,0,
		1,0,0,
		0,0,0,

		0,0,0,
		0,0,1,
		0,0,0,

		0,1,0,
		0,0,0,
		0,0,0,

		0,0,0,
		0,0,0,
		0,1,0
};


char pdt_image_stipple60_x[4][16*2] = {
	{ 0,0, 2,2, 0,2, 2,0, 1,1, 3,3, 1,3, 3,1, 0,1, 2,3, 0,3, 2,1, 1,0, 3,2, 1,2, 3,0},
	{ 0,0, 1,0, 2,0, 3,0, 3,1, 3,2, 3,3, 2,3, 1,3, 0,3, 0,2, 0,1, 1,1, 2,1, 2,2, 1,2},
	{ 0,0, 1,0, 2,0, 3,0, 0,1, 1,1, 2,1, 3,1, 0,2, 1,2, 2,2, 3,2, 0,3, 1,3, 2,3, 3,3},
	{ 1,2, 2,2, 2,1, 1,1, 0,1, 0,2, 0,3, 1,3, 2,3, 3,3, 3,2, 3,1, 3,0, 2,0, 1,0, 0,0}
};
