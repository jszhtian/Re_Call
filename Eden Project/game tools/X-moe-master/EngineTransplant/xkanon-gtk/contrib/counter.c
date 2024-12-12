/*
 * counter.c  ����������
 *
 * Copyright (C) 1997-1998 Masaki Chikama (Wren) <chikama@kasumi.ipl.mech.nagoya-u.ac.jp>
 *               1998-                           <masaki-c@is.aist-nara.ac.jp>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 *
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include "counter.h"

#define HICOUNTER_MAX 260
static struct timeval tv_base;
static int counter_init = 0;

static int counter_init_high[HICOUNTER_MAX];
static struct timeval tv_high[HICOUNTER_MAX];
static int division_high[HICOUNTER_MAX];

int get_counter(int division) {
	long sec, usec, usec2;
	struct timeval tv;

	gettimeofday(&tv, NULL);
	sec  = tv.tv_sec - tv_base.tv_sec;
	usec = tv.tv_usec - tv_base.tv_usec;
	usec2 = sec * 1000l + usec / 1000l;
	return counter_init + (int)(usec2 / division);
}

/* �����󥿡��� val�ǥꥻ�å� */
/* ��������˰��ٸƤ�Ǥ��� */
void reset_counter(int val) {
	counter_init = val;
	gettimeofday(&tv_base, NULL);
}


int get_high_counter(int num) {
	long sec, usec, usec2;
	struct timeval tv;
	struct timeval tv_base = tv_high[num -1];
	int division = division_high[num -1];
	
	gettimeofday(&tv, NULL);
	sec  = tv.tv_sec - tv_base.tv_sec;
	usec = tv.tv_usec - tv_base.tv_usec;
	usec2 = sec * (1000l/division)+ usec / 1000l /division;
	return counter_init_high[num -1] + usec2;
}

/* �����٥����󥿡� thanx tajiri@wizard */
/* �����󥿡��� val�ǥꥻ�å� */
/* ��������˰��ٸƤ�Ǥ��� */
void reset_counter_high(int num,int division,int val) {
	if (num == 0) {
		int i;
		for (i = 0; i < 256; i++) {
			counter_init_high[i] = val;
			gettimeofday(&tv_high[i], NULL);
			division_high[i]=division;
		}
	} else {
		counter_init_high[num -1] = val;
		gettimeofday(&tv_high[num -1], NULL);
		division_high[num -1]=division;
	}
}


